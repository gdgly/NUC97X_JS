
//菜单2 参数定值
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"



extern const u8 UnitList[][6];//通用单位列表
extern const u8 SList[][2];//正负极性列表
extern const u8 CommunicationSpeedList[][6];//通信速率列表

const u8 QUnitList[][4]=//电量单位列表
{
	"kWh",
	"MWh",
};

#if ((Project/100)==3 && (USER/100)==10)//河北专变
const u8 ProtocolList[][13]=//通信协议列表
{
	"保留       ",//0 终端无需对本序号的电能表/交流采样装置进行抄表
	"DLT645-1997",//1 DL/T 645-1997
	"交流采样   ",//2 交流采样装置通信协议	
	"威胜规约   ",//7 威胜规约
	"DLT645-2007",//30 DL/T 645-2007
	"低压载波   ",//31 "串行接口连接窄带低压载波通信模块"接口协议
	"备用       ",//其他
};
#elif ((USER/100)==5)//上海
const u8 ProtocolList[][13]=//通信协议列表
{
	"保留       ",//0 终端无需对本序号的电能表/交流采样装置进行抄表
	"DLT645-1997",//1 DL/T 645-1997
	"交流采样   ",//2 交流采样装置通信协议	
	"698.45规约 ",//698规约
	"上海规约   ",//21 威胜规约
	"DLT645-2007",//30 DL/T 645-2007
	"低压载波   ",//31 "串行接口连接窄带低压载波通信模块"接口协议
	"备用       ",//其他
};
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
const u8 ProtocolList[][13]=//通信协议列表
{
	"保留       ",//0 终端无需对本序号的电能表/交流采样装置进行抄表
	"DLT645-1997",//1 DL/T 645-1997
	"交流采样   ",//2 交流采样装置通信协议	
	"红相规约   ",//4 红相规约
	"DLT645-2007",//30 DL/T 645-2007
	"低压载波   ",//31 "串行接口连接窄带低压载波通信模块"接口协议
	"备用       ",//其他
};
#else
const u8 ProtocolList[][13]=//通信协议列表
{
	"保留       ",//0 终端无需对本序号的电能表/交流采样装置进行抄表
	"DLT645-1997",//1 DL/T 645-1997
	"交流采样   ",//2 交流采样装置通信协议	
	"DLT645-2007",//30 DL/T 645-2007
	"低压载波   ",//31 "串行接口连接窄带低压载波通信模块"接口协议
	"备用       ",//其他
};
#endif

const u8 S0List[][5]=//上下浮动列表
{
	"上浮",
	"下浮",
};

const u8 pMenu3_2[]=
{
	"参数定值\n\r"
	"  1.时段控参数\n\r"
	"  2.厂休控参数\n\r"
	"  3.营业报停控参数\n\r"
	"  4.功率下浮控参数\n\r"
	"  5.月电控参数\n\r"
	"  6.kv ki kp配置\n\r"
	"  7.电能表参数\n\r"
	"  8.配置参数\n\r"
#if (USER/100)==13//黑龙江
	"  9.终端管理与维护\n\r"
#endif
};

const u8 pMenu4_2[]=
{
	"参数定值\n\r"
	"  1.kv ki配置\n\r"
	"  2.电能表参数\n\r"
};

const u8 pMenu_2_1[]=
{
	"时段控参数\n\r"
	"  1.方案1\n\r"
	"  2.方案2\n\r"
	"  3.方案3\n\r"
};

const u8 pMenu_2_1_1[]=
{
	"时段控?参数\n\r"
	"总加组\n\r"
	"浮动系数:\n\r"
	"投入轮次:\n\r"
	"功控时段及定值(kW):\n\r"
};

const u8 pMenu_2_2[]=
{
	"厂休控参数\n\r"
	"总加组\n\r"
};

const u8 WorkersDayOffList[][20]=//厂休控参数列表
{
	"功率定值  :",
	"起始时间  :",
	"持续时间  :",
	"每周限电日:",
	"投入轮次  :",
};

const u8 pMenu_2_3[]=
{
	"功率下浮控参数\n\r"
	"总加组\n\r"
};

const u8 PowerLowerList[][20]=//功率下浮控列表
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	"浮动系数:",
	"当前功率:",
	"控制时间:",
	"投入轮次:",
	"轮1告警 :",
	"轮2告警 :",
#else
	"功率定值:",
	"浮动系数:",
	"当前功率:",
	"控制时间:",
	"投入轮次:",
	"轮1告警 :",
	"轮2告警 :",
#endif
};

#if((USER/100)!=5)//非上海用户
const u8 pMenu_2_4[]=
{
	"营业报停控参数\n\r"
	"总加组\n\r"
};

const u8 BusinessQuitList[][20]=//营业报停控参数列表
{
	"功率定值:",
	"起始时间:",
	"结束时间:",
	"投入轮次:",
};

const u8 pMenu_2_5[]=
{
	"月电控参数\n\r"
	"总加组\n\r"
};

const u8 MonthEQList[][20]=//月电控参数列表
{
	"电控定值:",
	"报警系数:",
	"定值系数:",
	"总用电量:",
	"投入轮次:",
};
#endif

const u8 pMenu_2_6[]=
{
	"Kv Ki Kp配置\n\r"
	"  1.Kp配置\n\r"
	"  2.Kv Ki配置\n\r"
};

const u8 pMenu_2_6_1[]=
{
	"Kp配置\n\r"
	"脉冲\n\r"
};

const u8 pMenu_2_6_2[]=
{
	"Kv Ki配置\n\r"
	"测量点\n\r"
};

const u8 KvKiList[][20]=//KvKi列表
{
	"Kv:",
	"Ki:",
};

const u8 pMenu_2_7[]=
{
	"电能表参数\n\r"
	"测量点\n\r"
};

#if(((Project/100)==3) && ((USER/100)==5))//上海专变
const u8 MeterList[][20]=//电能表参数列表
{
	"测量点:",
	"通  道:",
	"协  议:",
	"表地址:",
	"大类号:",
	"小类号:",
};
#else
const u8 MeterList[][20]=//电能表参数列表
{
	"局编号:",
	"通道  :",
	"协议  :",
	"表地址:",
};
#endif

const u8 MeterParameterList[][20]=//台区电表参数列表
{
	"测量点",
	"通信速率",
	"通信端口",
	"通信规约",
	"通信地址",
	"采集器",
	"大类号",
	"小类号",
};

const u8 pMenu2_2_3[]=
{
	"抄表运行参数\n\r"
	"  1.抄表时段与间隔\n\r"
	"  2.抄表日\n\r"
};

const u8 MeterReadingDateList[][20]=//抄表日列表
{
	"抄表日",
	"1 2 3 4 5 6 7 8 9",
	"10 11 12 13 14 15",
	"16 17 18 19 20 21",
	"22 23 24 25 26 27",
	"28 29 30 31",
};

#if ((Project/100)==3 || (Project/100)==4)//专变终端、公变终端
#if ((Project/100)==3 && (USER/100)==5)//上海专变
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_7
// Descriptions:        电能表参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_7(void)//菜单2_7
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 Row;
	u32 i;
	u32 j=0;
	u64 x;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"电能表参数\n\r"
																			"序  号:\n\r"
																			);//显示菜单
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+2*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+16*2);
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
	DisplayClrRow(3,8);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)MeterList[i]);
		}
		//测量点号
		p16=(u16*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+2,2);
		if(p16[0])
		{//测量点号不为0时，显示相应的信息
			Row=3;
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,p16[0],4);
			//通信速率及端口号、通信协议类型
			Row++;
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+4,2);
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,p8[0] & 0x1F,2);
			x=hex_bcd(p8[0] & 0x1F);
			switch(x)
			{
				case 1:
					DisplayString(Row,10,0,":交采");			
					break;
				case 2:
					DisplayString(Row,10,0,":485I");			
					break;
				case 3:
					DisplayString(Row,10,0,":485II");			
					break;
				case 31:
					DisplayString(Row,10,0,":载波");			
					break;
				default:
					break;
			}
			Row++;
			switch(p8[1])
			{
				case 31://"串行接口连接窄带低压载波通信模块"接口协议
					j++;
				case 30://DL/T 645-2007
					j++;
				case 21://上海规约
					j++;
				case 3://698.45
					j++;
				case 2://交流采样装置通信协议
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://终端无需对本序号的电能表/交流采样装置进行抄表
					break;
				default:
					j=6;
					break;
			}
			DisplayString(Row,8,0,(u8 *)ProtocolList[j]);//通信协议列表
			//通信地址
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+8*2);
			x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+6,6);
			DisplayData(x,12,0,p16);
			//大类号及小类号
			Row++;
			x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+26,1);
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,x>>4,2);
			Row++;
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,x&0x0f,2);
		}
	}
	else
	{
		DisplayString(3,4,0,"无效测量点号");	
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_7
// Descriptions:        电能表参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_7(void)//菜单2_7
{
	u8 *p8;
	u16 *p16;
	u16 *p;
	u32 i;
	u32 j=0;
	u64 x;
	u32 Ln;
#if (USER/100)==13//黑龙江
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_7);//显示菜单
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
	DisplayClrRow(Ln+2,Ln+5);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=3;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)MeterList[i]);
		}
		p=(u16*)ADDR_DATABUFF;
		for(i=1;i<=MaxRS485AddCarrierPn;i++)
		{
			MR(ADDR_DATABUFF,ADDR_AFN04F10+27*(i-1)+2,2);
			if(p[0]==MenuCount->v2)
			{//找到该测量点
				break;
			}
		}
		if(i!=MaxRS485AddCarrierPn+1)
		{//测量点号存在
			//局编号
			p8=(u8 *)ADDR_DATABUFF;						                              
			MC(0,ADDR_DATABUFF+12,1);
			MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(i-1),12);
			DisplayString(Ln+2,8,0,p8);//显示字符串
			//通信速率及端口号、通信协议类型
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+27*(i-1)+4,2);
			x=hex_bcd(p8[0] & 0x1F);
			switch(x)
			{
				case 1:
					DisplayString(Ln+3,10,0,":交采");			
					break;
				case 2:
					DisplayString(Ln+3,10,0,":485I");			
					break;
				case 3:
					DisplayString(Ln+3,10,0,":485II");			
					break;
				case 31:
					DisplayString(Ln+3,10,0,":载波");			
					break;
				default:
					break;
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
			DisplayData_OffZero(x,2,0,p16);
#if ((Project/100)==3 && (USER/100)==10)//河北专变
			switch(p8[1])
			{
				case 31://"串行接口连接窄带低压载波通信模块"接口协议
					j++;
				case 30://DL/T 645-2007
					j++;
				case 7://威胜规约
					j++;
				case 2://交流采样装置通信协议
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://终端无需对本序号的电能表/交流采样装置进行抄表
					break;
				default:
					j=6;
					break;
			}
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
			switch(p8[1])
			{
				case 31://"串行接口连接窄带低压载波通信模块"接口协议
					j++;
				case 30://DL/T 645-2007
					j++;
				case 4://红相规约
					j++;
				case 2://交流采样装置通信协议
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://终端无需对本序号的电能表/交流采样装置进行抄表
					break;
				default:
					j=6;
					break;
			}
#else
			switch(p8[1])
			{
				case 31://"串行接口连接窄带低压载波通信模块"接口协议
					j++;
				case 30://DL/T 645-2007
					j++;
				case 2://交流采样装置通信协议
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://终端无需对本序号的电能表/交流采样装置进行抄表
					break;
				default:
					j=5;
					break;
			}
#endif
			DisplayString(Ln+4,8,0,(u8 *)ProtocolList[j]);//通信协议列表
			//通信地址
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
			x=MRR(ADDR_AFN04F10+27*(i-1)+6,6);
			DisplayData(x,12,0,p16);
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6_2
// Descriptions:        Kv Ki配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_6_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_2);//显示菜单
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
	DisplayClrRow(Ln+2,Ln+3);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)KvKiList[i]);
		}
		//电压互感器倍率Kv
		x=MRR(ADDR_AFN04F25+11*(MenuCount->v2-1),2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
		//电流互感器倍率Ki
		x=MRR(ADDR_AFN04F25+11*(MenuCount->v2-1)+2,2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
#endif

#if ((Project/100)==3)//专变终端
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6_1
// Descriptions:        Kp配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_6_1(void)
{
#if MaxImpInPort
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_1);//显示菜单
			if(MenuCount->v2>MaxImpInPort)
			{//MenuCount->v2记录脉冲输入端口号
				MenuCount->v2=MaxImpInPort;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://任务1:选择
			NdigitSelect(1,9,1,MaxImpInPort);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+2);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxImpInPort && MenuCount->v2>=1)
	{
		DisplayString(Ln+2,0,0,"Kp:");
		//电表常数Kp
		x=MRR(ADDR_AFN04F11+5*(MenuCount->v2-1)+3,2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+4*2);
		DisplayData_OffZero(y,5,0,p16);
	}
	else
	{
		DisplayString(Ln+2,5,0,"无效脉冲号");	
	}
#endif
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6
// Descriptions:        Kv Ki Kp配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_6(void)//菜单2_6
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_2_6_1);
						break;
					case 2:
						MenuCreate((u32)Menu_2_6_2);
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

#if((USER/100)!=5)//非上海用户
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_5
// Descriptions:        月电控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_5(void)//菜单2_5
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;
#if (USER/100)==13//黑龙江
	u32 Row=3;
#else
	u32 Row=2;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_5);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Row-2+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Row-2+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
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
	DisplayClrRow(Row,Row+4);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Row+i,0,0,(u8 *)MonthEQList[i]);
		}
		//月电控定值
		y=MRR(ADDR_AFN04F46+5*(MenuCount->v2-1),4);				
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(Row,9,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
		}
		Row++;
		//报警门限系数
		y=MRR(ADDR_AFN04F46+5*(MenuCount->v2-1)+4,1);				
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		p16=DisplayData_OffZero(y,2,0,p16);
		DisplayString(Row,13,0,(u8 *)UnitList[6]);//6 %
		Row++;
		//月电量控定值浮动系数
		y=MRR(ADDR_AFN04F20,1);			                  
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(14*2));
		DisplayString(Row,10,0,(u8 *)S0List[y>>7]);//上浮 下浮
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(Row,i,0,(u8 *)UnitList[6]);//6 %
		Row++;
		//当月总加有功总电能量
		y=MRR(GetClass1DataAddr(21,MenuCount->v2)+1,4);	       
//		y=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*(MenuCount->v2-1))+1,4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
		}
		Row++;
		//月电投入轮次
		x=MRR(ADDR_AFN04F48+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Row,4,0,"无效总加组号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_4
// Descriptions:        营业报停控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_4(void)//菜单2_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 Ln;
#if (USER/100)==13//黑龙江
	Ln=1;
#else
	Ln=0;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_4);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
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
	DisplayClrRow(Ln+2,Ln+5);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=3;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)BusinessQuitList[i]);
		}
		//营业报停控功率定值
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+6,2);			
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Ln+2,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+2)*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Ln+2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Ln+2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//起始时间 年月日
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+3)*84)+(10*2));
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+1,2);			
		DisplayData1(y,4,2,'-',p16);
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1),1);			
		DisplayString(Ln+3,15,0,"-");
		DisplayData(y,2,0,p16+6);
		//结束时间 年月日
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+4)*84)+(10*2));
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+4,2);
		DisplayData1(y,4,2,'-',p16);
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+3,1);
		DisplayString(4,15,0,"-");
		DisplayData(y,2,0,p16+6);
		//功控投入轮次
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+5)*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效总加组号");	
	}
}
#endif

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        功率下浮控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//菜单2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_3);//显示菜单
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
			break;
		default:
			break;
	}
	DisplayClrRow(3,8);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PowerLowerList[i]);
		}
		//下浮系数
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1)+2,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(14*2));
		DisplayString(3,10,0,(u8 *)S0List[y>>7]);//上浮 下浮
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(3,i,0,(u8 *)UnitList[6]);//6 %
		//当前功率
		y=MRR(ADDR_AFN0CF17+2*(MenuCount->v2-1),2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(4*84)+(10*2));
		p16 = PowerP16(y,p16);
		DisplayString(4,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(4,i,0,(u8 *)UnitList[0]);//0 kW
		//下浮控控制时间
		y=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);			
		z=hex_bcd(y/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
		p16=DisplayData_OffZero(z,3,0,p16);
		if(y%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=11;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(5,i,0,(u8 *)UnitList[7]);//7 h
		//功控投入轮次
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//轮1告警时间
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(7,i,0,(u8 *)UnitList[8]);//8 min
		//轮2告警时间
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+5,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(8,i,0,(u8 *)UnitList[8]);//8 min
	}
	else
	{
		DisplayString(3,4,0,"无效总加组号");	
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        功率下浮控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//菜单2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_3);//显示菜单
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
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PowerLowerList[i]);
		}
		//当前功控定值
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1),2);			
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(2,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//下浮系数
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1)+2,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(14*2));
		DisplayString(3,10,0,(u8 *)S0List[y>>7]);//上浮 下浮
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(3,i,0,(u8 *)UnitList[6]);//6 %
		//当前功率
		y=MRR(ADDR_AFN0CF17+2*(MenuCount->v2-1),2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(4*84)+(10*2));
		p16 = PowerP16(y,p16);
		DisplayString(4,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(4,i,0,(u8 *)UnitList[0]);//0 kW
		//下浮控控制时间
		y=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);			
		z=hex_bcd(y/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
		p16=DisplayData_OffZero(z,3,0,p16);
		if(y%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=11;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(5,i,0,(u8 *)UnitList[7]);//7 h
		//功控投入轮次
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//轮1告警时间
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(7,i,0,(u8 *)UnitList[8]);//8 min
		//轮2告警时间
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+5,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(8,i,0,(u8 *)UnitList[8]);//8 min
	}
	else
	{
		DisplayString(2,4,0,"无效总加组号");	
	}
}
#endif						  

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_2
// Descriptions:        厂休控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_2(void)//菜单2_2
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_2);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
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
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)WorkersDayOffList[i]);
		}
		//厂休控定值
		y=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1),2);			
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Ln+2,12,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+(12*2));
			p16 = PowerP16(y,p16);
			DisplayString(Ln+2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=13;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Ln+2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//限电起始时间
		y=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+2,2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+(12*2));
		DisplayData1(y,4,2,':',p16);
		//限电延续时间
		x=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+4,1);			
		y=hex_bcd(x/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+(12*2));
		p16=DisplayData_OffZero(y,2,0,p16);
		if(x%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=13;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=13;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(Ln+4,i,0,(u8 *)UnitList[7]);//7 h
		//每周限电日
		x=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+5,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+(12*2));
		for(i=1;i<=7;i++)
		{
			if((x>>i)&0x01)
			{
				p16[i-1]=i+0x2030;
			}
			else
			{
				p16[i-1]=0x2000+'-';
			}
		}
		//功控投入轮次
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+(12*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效总加组号");	
	}
}

	#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_1
// Descriptions:        时段控参数方案1 2 3
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)
{
	u8 *a;
	u8 *b;
	u8 *c;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;
	u32 m=0;
	u32 n=0;
	u32 N;//方案号0 1 2
	u32 x;
	u32 y;
	u32 l;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	u8 * p8d;
#endif

//方案号
	N = (Stack->MenuStack[MenuCount->Count-1].EditRow)-1;
//计算时段 目前“保留”为一个独立的控制时段
	p8=(u8 *)ADDR_DATABUFF;
	a=(u8 *)(ADDR_DATABUFF+200);//时段起始位置
	b=(u8 *)(ADDR_DATABUFF+250);//时段结束位置
	c=(u8 *)(ADDR_DATABUFF+300);//时段对应的控制方式：控制1、控制2、不控制
	MR(ADDR_DATABUFF,ADDR_AFN04F18,12);
	for(i=0;i<12;i++)
	{
		p8[12+4*i]=p8[i]&0x03;
		p8[13+4*i]=(p8[i]>>2)&0x03;
		p8[14+4*i]=(p8[i]>>4)&0x03;
		p8[15+4*i]=(p8[i]>>6)&0x03;
	}
	for(i=0;i<48;)
	{
		if(m>=8)
		{
			break;
		}
		else
		{
			switch(p8[12+i])
			{
				case 0:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=0)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=0;
					i=j;
					break;
				case 1:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=1)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=1;
					i=j;
					break;
				case 2:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=2)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=2;
					i=j;
					break;
				case 3:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=3)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=3;
					i=j;
					break;
			}
		}
	}
	MenuCount->v3=m;//一天分为多少个时段
//MenuCount->v2记录总加组号 若超限赋值
	if(MenuCount->v2>MaxTotalAddGroup)
	{
		MenuCount->v2=MaxTotalAddGroup;
	}
	if(MenuCount->v2<1)
	{
		MenuCount->v2=1;
	}
//计算第1屏显示了多少个设置定值的时段
	p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
	if(p16[0]==0x40B8 && p16[1]==0x40B9)
	{//右下角有↓
		x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
		if((x>>N)&0x01)
		{
			y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
			for(j=0;j<4;j++)
			{
				if((y>>j)&0x01)
				{
					n++;
				}
			}
			MenuCount->v1=n;//第1屏显示了多少个设置定值的时段
		}
	}
	n=0;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_SH(0,Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1_1);//显示菜单
			p16=(u16 *)(ADDR_STRINGBUFF+0*84+4+10*2);
			p16[0]=N+0x2031;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			//时段控定值浮动系数
			x=MRR(ADDR_AFN04F19,1);			                    
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(13*2));
			DisplayString(2,9,0,(u8 *)S0List[x>>7]);//上浮 下浮
			x&=0x7f;
			p16=DisplayData_OffZero(x,2,0,p16);
			for(i=14;i<17;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[6]);//6 %
			//功控投入轮次
			x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			  
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(9*2));
			for(i=0;i<=7;i++)
			{
				if(((x>>i)&0x01)==1)
				{//受控
					p16[i]=i+0x2031;
				}
				else
				{
					p16[i]=0x2000+'-';
				}
			}
			
			if(MenuCount->v3<=4)
			{
				for(j=0;j<MenuCount->v3;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//控制1,控制2,保留
						p16[11]=34;// '√'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<MenuCount->v3;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"无");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
						for(j=0;j<MenuCount->v3;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"无");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//方案N时段n功率定值
									if(l==0xEEEE)
									{//无此数据填-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
								}
						}
						break;
				}
			}
			else
			{
				DisplayString(9,18,0,"↓");
				for(j=0;j<4;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//控制1,控制2,保留
						p16[11]=34;// '√'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<4;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"无");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
						for(j=0;j<4;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"无");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//方案N时段n功率定值
									if(l==0xEEEE)
									{//无此数据填-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
							}
						}
						break;
				}
			}
			break;
		case 1://任务1:选择
			x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
				if(p16[0]==0x40B8 && p16[1]==0x40B9)
				{//右下角有↓
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//下键
						DisplayClrRow(2,9);
						DisplayString(0,18,0,"↑");
						for(j=4;j<MenuCount->v3;j++)
						{
							Ln=j-2;
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
							if(c[j])
							{//控制1,控制2,保留
								p16[11]=34;// '√'
							}
							x=hex_bcd(a[j]>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if((a[j]&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
							p16[5]=0x2000+'-';
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
							x=hex_bcd((b[j]+1)>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if(((b[j]+1)&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
						}

						x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
						switch((x>>N)&0x01)
						{
							case 0:
								for(j=4;j<MenuCount->v3;j++)
								{
									Ln=j-2;
									DisplayString(Ln,13,0,"无");	
								}
								break;
							case 1:
								y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
								for(j=4;j<MenuCount->v3;j++)
								{
									switch((y>>j)&0x01)
									{
										case 0:
											Ln=j-2;
											DisplayString(Ln,13,0,"无");	
											break;
										case 1:
											n++;
											Ln=j-2;
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
											l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(MenuCount->v1+n-1),2);//方案N时段MenuCount->v1+n功率定值
											if(l==0xEEEE)
											{//无此数据填-------
												DisplayString(Ln,13,0,"-------");	
											}
											else
											{
												p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
												PowerP16(l,p16);
												DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
											}
											break;
									}
								}
								break;
						}
					}
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//右上角有↑
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//上
						Stack->MenuStack[MenuCount->Count].Task=0;
					}
				}
				Comm_Ram->ButtonVal&=~0x03;//键使用后清0
			}
			NdigitSelect_SH(1,1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
				p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_1
// Descriptions:        时段控参数方案1 2 3
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)
{
	u8 *a;
	u8 *b;
	u8 *c;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;
	u32 m=0;
	u32 n=0;
	u32 N;//方案号0 1 2
	u32 x;
	u32 y;
	u32 l;

//方案号
	N = (Stack->MenuStack[MenuCount->Count-1].EditRow)-1;
//计算时段 目前“保留”为一个独立的控制时段
	p8=(u8 *)ADDR_DATABUFF;
	a=(u8 *)(ADDR_DATABUFF+200);//时段起始位置
	b=(u8 *)(ADDR_DATABUFF+250);//时段结束位置
	c=(u8 *)(ADDR_DATABUFF+300);//时段对应的控制方式：控制1、控制2、不控制
	MR(ADDR_DATABUFF,ADDR_AFN04F18,12);
	for(i=0;i<12;i++)
	{
		p8[12+4*i]=p8[i]&0x03;
		p8[13+4*i]=(p8[i]>>2)&0x03;
		p8[14+4*i]=(p8[i]>>4)&0x03;
		p8[15+4*i]=(p8[i]>>6)&0x03;
	}
	for(i=0;i<48;)
	{
		if(m>=8)
		{
			break;
		}
		else
		{
			switch(p8[12+i])
			{
				case 0:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=0)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=0;
					i=j;
					break;
				case 1:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=1)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=1;
					i=j;
					break;
				case 2:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=2)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=2;
					i=j;
					break;
				case 3:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=3)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=3;
					i=j;
					break;
			}
		}
	}
	MenuCount->v3=m;//一天分为多少个时段
//MenuCount->v2记录总加组号 若超限赋值
	if(MenuCount->v2>MaxTotalAddGroup)
	{
		MenuCount->v2=MaxTotalAddGroup;
	}
	if(MenuCount->v2<1)
	{
		MenuCount->v2=1;
	}
//计算第1屏显示了多少个设置定值的时段
	p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
	if(p16[0]==0x40B8 && p16[1]==0x40B9)
	{//右下角有↓
		x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
		if((x>>N)&0x01)
		{
			y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
			for(j=0;j<4;j++)
			{
				if((y>>j)&0x01)
				{
					n++;
				}
			}
			MenuCount->v1=n;//第1屏显示了多少个设置定值的时段
		}
	}
	n=0;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1_1);//显示菜单
			p16=(u16 *)(ADDR_STRINGBUFF+0*84+4+10*2);
			p16[0]=N+0x2031;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			//时段控定值浮动系数
			x=MRR(ADDR_AFN04F19,1);			                    
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(13*2));
			DisplayString(2,9,0,(u8 *)S0List[x>>7]);//上浮 下浮
			x&=0x7f;
			p16=DisplayData_OffZero(x,2,0,p16);
			for(i=14;i<17;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[6]);//6 %
			//功控投入轮次
			x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			  
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(9*2));
			for(i=0;i<=7;i++)
			{
				if(((x>>i)&0x01)==1)
				{//受控
					p16[i]=i+0x2031;
				}
				else
				{
					p16[i]=0x2000+'-';
				}
			}
			
			if(MenuCount->v3<=4)
			{
				for(j=0;j<MenuCount->v3;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//控制1,控制2,保留
						p16[11]=34;// '√'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<MenuCount->v3;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"无");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
						for(j=0;j<MenuCount->v3;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"无");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//方案N时段n功率定值
									if(l==0xEEEE)
									{//无此数据填-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
								}
						}
						break;
				}
			}
			else
			{
				DownArrow();
				for(j=0;j<4;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//控制1,控制2,保留
						p16[11]=34;// '√'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<4;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"无");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
						for(j=0;j<4;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"无");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//方案N时段n功率定值
									if(l==0xEEEE)
									{//无此数据填-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
							}
						}
						break;
				}
			}
			break;
		case 1://任务1:选择
			x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
				if(p16[0]==0x40B8 && p16[1]==0x40B9)
				{//右下角有↓
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//下键
						DisplayClrRow(2,9);
						UpArrow();
						for(j=4;j<MenuCount->v3;j++)
						{
							Ln=j-2;
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
							if(c[j])
							{//控制1,控制2,保留
								p16[11]=34;// '√'
							}
							x=hex_bcd(a[j]>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if((a[j]&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
							p16[5]=0x2000+'-';
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
							x=hex_bcd((b[j]+1)>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if(((b[j]+1)&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
						}

						x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//方案标志
						switch((x>>N)&0x01)
						{
							case 0:
								for(j=4;j<MenuCount->v3;j++)
								{
									Ln=j-2;
									DisplayString(Ln,13,0,"无");	
								}
								break;
							case 1:
								y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//方案N时段号
								for(j=4;j<MenuCount->v3;j++)
								{
									switch((y>>j)&0x01)
									{
										case 0:
											Ln=j-2;
											DisplayString(Ln,13,0,"无");	
											break;
										case 1:
											n++;
											Ln=j-2;
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
											l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(MenuCount->v1+n-1),2);//方案N时段MenuCount->v1+n功率定值
											if(l==0xEEEE)
											{//无此数据填-------
												DisplayString(Ln,13,0,"-------");	
											}
											else
											{
												p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
												PowerP16(l,p16);
												DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
											}
											break;
									}
								}
								break;
						}
					}
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//右上角有↑
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//上
						Stack->MenuStack[MenuCount->Count].Task=0;
					}
				}
				Comm_Ram->ButtonVal&=~0x03;//键使用后清0
			}
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
}
	#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1
// Descriptions:        时段控参数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2_1(void)
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				if(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow>=1 && Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow<=3)
				{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
					p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
					p8[0]=0;//菜单显示顶、底行用，0=否，1=是
#endif
					MenuCreate((u32)Menu_2_1_1);
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

#if ((Project/100)==3 && (USER/100)==5)//上海专变
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2
// Descriptions:        参数定值菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2(void)//菜单2
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"参数定值\n\r"
																			"  1.时段控参数\n\r"
																			"  2.厂休控参数\n\r"
																			"  3.下浮控参数\n\r"
																			"  4.Kv Ki Kp\n\r"
																			"  5.电能表参数\n\r"
																			"  6.配置参数\n\r"
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
						MenuCreate((u32)Menu_2_1);
						break;
					case 2:
						MenuCreate((u32)Menu_2_2);
						break;
					case 3:
						MenuCreate((u32)Menu_2_3);
						break;
					case 4:
						MenuCreate((u32)Menu_2_6);
						break;
					case 5:
						MenuCreate((u32)Menu_2_7);
						break;
					case 6:
						//按下编程键后、输入正确密码，方可进入设置菜单
						//工厂模式下，随时可进入设置菜单
						if(Comm_Ram->Factory==0x55)
						{//工厂状态
							MenuCreate((u32)Menu_8);//创建下级菜单
						}
						else
						{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//按下编程键
								PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
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
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2
// Descriptions:        参数定值菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_2(void)//菜单2
{
	#if ((Project/100)==3)//专变终端
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu3_2);//显示菜单
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
							MenuCreate((u32)Menu_2_1);
							break;
						case 2:
							MenuCreate((u32)Menu_2_2);
							break;
						case 3:
							MenuCreate((u32)Menu_2_4);
							break;
						case 4:
							MenuCreate((u32)Menu_2_3);
							break;
						case 5:
							MenuCreate((u32)Menu_2_5);
							break;
						case 6:
							MenuCreate((u32)Menu_2_6);
							break;
						case 7:
							MenuCreate((u32)Menu_2_7);
							break;
						case 8:
							//按下编程键后、输入正确密码，方可进入设置菜单
							//工厂模式下，随时可进入设置菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								MenuCreate((u32)Menu_8);//创建下级菜单
							}
							else
							{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//按下编程键
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
							}
							break;
						#if (USER/100)==13//黑龙江
						case 9:
							//按下编程键后、输入正确密码，方可进入设置菜单
							//工厂模式下，随时可进入设置菜单
							if(Comm_Ram->Factory==0x55)
							{//工厂状态
								MenuCreate((u32)Menu_9);//创建下级菜单
							}
							else
							{
//								if(MEMReadReg(ADDR_MeterFMTimer,2))
//								{//按下编程键
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//创建下级菜单
//								}
							}
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
	#endif
	#if ((Project/100)==4)//公变终端
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu4_2);//显示菜单
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
							MenuCreate((u32)Menu_2_6_2);
							break;
						case 2:
							MenuCreate((u32)Menu_2_7);
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
}
#endif
