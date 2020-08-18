
//菜单4 电能表示数
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_Record.h"

#include "../MS/msFILE.h"
#include "../OS/OS.h"
#include "../MS/MS.h"

extern const u8 UnitList[][6];//通用单位列表
extern const u8 SList[][2];//正负极性列表
extern u32 RCSDtoID(u8* pRCSD);//记录列选择RCSD相对应的任务号,返回:上报数据在采集任务的任务号,>=RMTASKmax表示无效任务号
extern u32 RCSDtoNextProjectID(u8* pRCSD,u32 ProjectID);//记录列选择RCSD相对应的下个方案号,入口:ProjectID=开始搜索的方案号;返回:方案号,>=NUMmax_6014表示无效方案号
extern u32 ProjectIDtoTaskID(u32 ProjectID);//普通采集方案号转为任务号;返回:采集任务的任务号,>=RMTASKmax表示无效任务号

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
	"配置号\n\r"
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
	"配置号\n\r"
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
//	"  2.当月正向无功\n\r"
	"  2.当月反向有功\n\r"
//	"  4.当月反向无功\n\r"
//	"  5.上月正向有功\n\r"
//	"  6.上月正向无功\n\r"
//	"  7.上月反向有功\n\r"
//	"  8.上月反向无功\n\r"
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


u32 Check_6000_Pn(u32 Pn)//判断配置序号Pn是否有存在有效的参数 不存在=返回0  存在返回 1
{
	u32 i;
	u32 Addr;
//	u32 Meter;
	u8 *p;
	if(Pn>=NUMmax_6000)
	{
		return 0;
	}
	Addr=ADDR_6000+(LENper_6000*Pn);
	MR(ADDR_DATABUFF,Addr,5);
	p=(u8 *)ADDR_DATABUFF;
	if(p[0]==0)// 数据类型是NULL  02 04 12 
	{
		return 0;
	}
	Data_Inverse(ADDR_DATABUFF+3,2);
	i=MRR(ADDR_DATABUFF+3,2);
	if(i!=Pn)
	{
		return 0;
	}
	return 1;
}

u64 Get_TSA_BY_PN_noInverse_menu(u32 Pn)//根据配置序号，得到TSA。不包含数据类型。不调换顺序
{

	u32 i;
	u32 Addr;
	u64 Meter;
	u8 *p8;
	
	i=Check_6000_Pn(Pn);
	if(i==0)
	{
		return INVALID_DATA;
	}
	Addr=ADDR_6000+LENper_6000*Pn;
	
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//计算元素地址,使用ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	if(p8[1]==0)
	{
		return INVALID_DATA;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
	if((p8[1]-1)>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,(p8[1]-1));
	return Meter;
}

u64 Get_TSA_BY_PN(u32 Pn)//根据配置序号，得到TSA。不包含数据类型。
{

	u32 i;
	u32 Addr;
	u64 Meter;
	u8 *p8;
	i=Check_6000_Pn(Pn);
	if(i==0)
	{
		return INVALID_DATA;
	}
	Addr=ADDR_6000+LENper_6000*Pn;
	
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//计算元素地址,使用ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
	Data_Inverse(ADDR_DATABUFF,(p8[1]-1));
	if((p8[1]-1)>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,(p8[1]-1));
	return Meter;
}
//num只在读取需量时有效
//目前只支持读取当前数据，road一直都是0
u64 GetData(u32 road,u32 oad,u32 num,u32 pn)//读取数据，road=0表示当前数，pn 是配置序号,如果内容是结构的num表示第几个成员
{
	u64 res=INVALID_DATA;
	u8 pRCSD[]={DataType_RCSD,0x01,0x00,0x00,0x00,0x00,0x00};
	u32 task_id;//包含oad的任务号
	u32 data_addr;//最近一条记录oad数据的存放地址
	u32 data_len;
	u64 tsa;
	u8 *p_data;
	u8 *pp;
	u32 index=0;
	u32 ProjectID;
	tsa=Get_TSA_BY_PN_noInverse_menu(pn);
	if(tsa==INVALID_DATA)//获取测量点地址失败
	{
		return INVALID_DATA;
	}
	//第一步 根据oad 读出全部由的数据。
	pRCSD[3]=(oad>>24)&0xff;
	pRCSD[4]=(oad>>16)&0xff;
	pRCSD[5]=(oad>>8)&0xff;

	if((Terminal_Ram->res13==(oad&0xffffff00))&&(Terminal_Ram->res14==num)&&(Terminal_Ram->res15==pn))
	{
		if(Terminal_Ram->rse16==INVALID_DATA)
			return INVALID_DATA;
		data_addr=Get_LastRecordFileData(Terminal_Ram->rse16,(u16* )&tsa);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
		if(data_addr==0)
			return INVALID_DATA;
		data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
		if(data_len==0)
			return INVALID_DATA;
		goto read;
	}
	Terminal_Ram->res13=(oad&0xffffff00);
	Terminal_Ram->res14=num;
	Terminal_Ram->res15=pn;
	//后续部分耗时很严重，造成液晶操作按键相应不及时.
	//上面记住之前输入的参数，如果连续相同，就直接从记录下来的task_id里去读数据，
	//如果不同，就从头找一遍。
	for(index=0;index<NUMmax_6014;index++)
	{
		ProjectID=RCSDtoNextProjectID(pRCSD,index);//记录列选择RCSD相对应的下个方案号,入口:ProjectID=开始搜索的方案号;返回:方案号,>=NUMmax_6014表示无效方案号
		if(ProjectID>=NUMmax_6014)
		{
			continue;
		}
		index=ProjectID;
		task_id=ProjectIDtoTaskID(ProjectID);//普通采集方案号转为任务号;返回:采集任务的任务号,>=RMTASKmax表示无效任务号
		if(task_id>=RMTASKmax)
		{
			continue;
		}
		data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
		if(data_addr==0)//在数据记录里面没有该TSA的数据
		{
			continue;
		}
		data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
		if(data_len==0)
		{
			continue;
		}				
		break;
	}
	if(index>=NUMmax_6014)
	{
		Terminal_Ram->rse16=INVALID_DATA;
		return INVALID_DATA;
	}
	Terminal_Ram->rse16=task_id;
	//到这，数据都已经读到ADDR_DATABUFF

	
	/*task_id=RCSDtoID(pRCSD);//记录列选择RCSD相对应的任务号,返回:上报数据在采集任务的任务号,>=RMTASKmax表示无效任务号
	if(task_id>=RMTASKmax)//要查询的OAD不在任何一个任务里面，肯定是没有数据的。
	{
		return INVALID_DATA;
	}
	data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
	if(data_addr==0)//在数据记录里面没有该TSA的数据
	{
		return INVALID_DATA;
	}
	data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
	if(data_len==0)
	{
		return INVALID_DATA;
	}*/
	//到这，数据都已经读到ADDR_DATABUFF
	//
read:	
	p_data=(u8 *)ADDR_DATABUFF;
	if(p_data[0]==DataType_array)//因为前面列选时是选全部的，出来的应该是数组的。
	{
		pp=Get_Element(p_data,oad&0xff,0,LEN_DATABUFF);
		if(pp!=0)
		{
			switch(pp[0])
			{
				case DataType_long_unsigned:
				case DataType_long:
					res=(pp[1]<<8)+pp[2];
					res=hex_bcd(res);
					break;
				case DataType_double_long:
				case DataType_double_long_unsigned:
					res=(pp[1]<<24)+(pp[2]<<16)+(pp[3]<<8)+pp[4];
					if(((oad>>16)==0x2004)||((oad>>16)==0x2005))//功率单位不一样，显示是KWH
					{
						res/=1000;
					}
					res=hex_bcd(res);
					break;
				case DataType_structure://这只能是 需量
					switch(num)
					{
						case 1://需量的量
							res=(pp[3]<<24)+(pp[4]<<16)+(pp[5]<<8)+pp[6];
							res=hex_bcd(res);
							break;
						case 3://发生时间 月-日
							res=(hex_bcd(pp[10])<<8)+hex_bcd(pp[11]);
							break;
						case 2://发生时间 时-分
							res=(hex_bcd(pp[12])<<8)+hex_bcd(pp[13]);
							break;
						default:
							res=INVALID_DATA;
					}
					break;
			}
		}
		else
		{
			res=INVALID_DATA;
		}
	}
	else
	{
		res=INVALID_DATA;
	}
	return res;
}

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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			y=GetData(0,0x00800200+1+i,0,MenuCount->v2);
			//y=MRR(GetClass1DataAddr(136,MenuCount->v2)+6+4*i,4);	                   
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//y=MRR(GetClass1DataAddr(135,MenuCount->v2)+6+4*i,4);	   
			y=GetData(0,0x00700200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  
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
			//y=MRR(GetClass1DataAddr(134,MenuCount->v2)+6+4*i,4);
			y=GetData(0,0x00600200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//局编号
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//显示字符串
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(133,MenuCount->v2)+6+4*i,4);	 
			y=GetData(0,0x00500200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//显示字符串
#endif

		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+29+2*i,2);	 
			y=GetData(0,0x200A0200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(4+i,8,0,"-----");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>15]);//+ -
				y&=0x7fff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,4,3,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[7]);//无符号
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}
/*

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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+27+7*i,3);	   
			y=GetData(0X50060200,0x200A0200+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+30+7*i,2);	
			y=GetData(0X50060200,0x200A0200+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+32+7*i,2);	  
			y=GetData(0X50060200,0x200A0200+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
*/
/*
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
*/
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+27+7*i,3);	
			y=GetData(0,0x10200200+1+3+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+30+7*i,2);
			y=GetData(0,0x10200200+1+3+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+32+7*i,2);	 
			y=GetData(0,0x10200200+1+3+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+6+7*i,3);
			y=GetData(0,0x10200200+1+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+9+7*i,2);	  
			y=GetData(0,0x10200200+1+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+11+7*i,2);	  
			y=GetData(0,0x10200200+1+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
/*
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
*/
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//清屏幕显示的指定行
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//当月正向有功最大需量 平谷
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	   
			y=GetData(0,0x10100200+1+3+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//发生时间 时:分 发生时间 月-日
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	
			y=GetData(0,0x10100200+1+3+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	  
			y=GetData(0,0x10100200+1+3+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//当月正向有功最大需量 总尖峰
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);
			y=GetData(0,0x10100200+1+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	 
			y=GetData(0,0x10100200+1+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//发生时间 月-日
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	 
			y=GetData(0,0x10100200+1+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
					//case 2:
					//	MenuCreate((u32)Menu_4_5_2);//创建下级菜单
					//	break;
					case 2:
						MenuCreate((u32)Menu_4_5_3);//创建下级菜单
						break;
					//case 4:
					//	MenuCreate((u32)Menu_4_5_4);//创建下级菜单
					//	break;
					//case 5:
					//	MenuCreate((u32)Menu_4_5_5);//创建下级菜单
					//	break;
					//case 6:
					//	MenuCreate((u32)Menu_4_5_6);//创建下级菜单
					//	break;
					//case 7:
					//	MenuCreate((u32)Menu_4_5_7);//创建下级菜单
					//	break;
					//case 8:
					//	MenuCreate((u32)Menu_4_5_8);//创建下级菜单
					//	break;
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  	   
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
			//y=MRR(GetClass1DataAddr(132,MenuCount->v2)+6+4*i,4);	
			y=GetData(0,0x00400200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  	   
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
			//y=MRR(GetClass1DataAddr(131,MenuCount->v2)+6+5*i,5);	  
			y=GetData(0,0x00200200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,2,p16);
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	 
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
			//y=MRR(GetClass1DataAddr(130,MenuCount->v2)+6+4*i,4);	
			y=GetData(0,0x00300200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//正向有功电能示值
		for(i=0;i<5;i++)
		{
			//y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);
			y=GetData(0,0x00100200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//无此数据填-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,2,p16);
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+17+3*i,3);	
			y=GetData(0,0x20050200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	DisplayClrRow(3,8);//清屏幕显示的指定行
#else
	DisplayClrRow(2,9);//清屏幕显示的指定行
#endif

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//显示字符串
#endif
		//表地址
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+5+3*i,3);	
			y=GetData(0,0x20040200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//显示字符串
		//表地址
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	 	   
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+37+2*i,2); 
			y=GetData(0,0x20000200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+43+3*i,3);   
			y=GetData(0,0x20010200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
//	DisplayClr();//清全屏
//	DisplayLoop_Contrl(1);
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

