
//菜单7 终端信息
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"


#if ((Project/100)==3 && (USER/100)==5)//上海专变
extern const u8 ProtocolList[][13];//通信协议列表
extern const u8 CommunicationSpeedList[][6];//通信速率列表
//终端信息
//第1屏：电表1信息：通道、协议、表地址、波特率
//第2屏：电表2信息：通道、协议、表地址、波特率
//第3屏：主通道信息（GPRS/CDMA）:终端地址（行政区码-终端地址）、IP、端口号、APN、信号强度值。
//第4屏：软件版本号、硬件版本号
//第5屏：主通道信息（以太网）：终端地址（行政区码-终端地址）、本地IP、网关、端口号、子网掩码
void Menu_7(void)
{
	u32 i;
	u32 j=0;
	u64 x;
	u32 Row;
	u8 *p8;
	u8 *p8s;
	u16 *p16a;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"电表1信息\n\r"
																	"波特率:\n\r"
																	"通  道:\n\r"
																	"协  议:\n\r"
																	"表地址:\n\r"
																	);//显示菜单

	p16a=(u16*)ADDR_DATABUFF;
	for(i=1;i<=MaxRS485AddCarrierPn;i++)
	{
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+2,2);
		if(p16a[0]==1)
		{//找到测量点1
			break;
		}
	}
	if(i!=MaxRS485AddCarrierPn+1)
	{//测量点号存在
		Row=2;
		//通信速率
		p8=(u8*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+4,2);
		DisplayString(Row,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
		//端口号
		Row++;
		p8s=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
		bcd_p8(p8s,p8[0] & 0x1F,2);
		//通信协议类型
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
		x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);
		DisplayData(x,12,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_7_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_7_4);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_7_1(void)
{
	u32 i;
	u32 j=0;
	u64 x;
	u32 Row;
	u8 *p8;
	u8 *p8s;
	u16 *p16a;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"电表2信息\n\r"
																	"波特率:\n\r"
																	"通  道:\n\r"
																	"协  议:\n\r"
																	"表地址:\n\r"
																	);//显示菜单

	p16a=(u16*)ADDR_DATABUFF;
	for(i=1;i<=MaxRS485AddCarrierPn;i++)
	{
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+2,2);
		if(p16a[0]==2)
		{//找到测量点2
			break;
		}
	}
	if(i!=MaxRS485AddCarrierPn+1)
	{//测量点号存在
		Row=2;
		//通信速率
		p8=(u8*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+4,2);
		DisplayString(Row,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
		//端口号
		Row++;
		p8s=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
		bcd_p8(p8s,p8[0] & 0x1F,2);
		//通信协议类型
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
				j=5;
				break;
		}
		DisplayString(Row,8,0,(u8 *)ProtocolList[j]);//通信协议列表
		//通信地址
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+8*2);
		x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);
		DisplayData(x,12,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_7_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_7);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_7_2(void)
{
	u32 x;
	u32 y;
	u32 Row;
	u32 i;
	u8 *p;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"主通道GPRS/CDMA\n\r"
																	"终端地址:****-\n\r"
																	"IP:\n\r"
																	"端口:\n\r"
																	"APN:\n\r"
																	"信号强度:\n\r"
																	);//显示菜单

	Row=2;
	//1.行政区域码
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	//2.终端地址
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+14*2);
	x=MRR(ADDR_TERMINAL,2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);
	//3.主站主IP
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//4.主站端口号
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	p16=(u16*)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN04F3+4,2);
	bcd_p8(p,p16[0],5);
	//5.APN
	Row++;
	p=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+16,1);
	MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
	DisplayString(Row,4,0,p);//显示字符串
	//6.信号强度
	Row++;
/*
	信号强度（上海扩展）
	信号强度	信号品质
	   0        <=-113dBm
	   1        -111dBm
	   2        -109dBm
	  ...        ...
	   31        >=-51dBm
	   99       未知或无法获取
*/
	x=Terminal_Ram->SignaldBm;//信号强度0-0x31或0x99
	x=bcd_hex(x);
	if(x<=31)
	{
		if(x==0)
		{
			DisplayString(Row,9,0,"<=");//显示字符串
		}
		if(x==31)
		{
			DisplayString(Row,9,0,">=");//显示字符串
		}
		x*=2;
		x=113-x;
		x=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+11*2);
		p16[0]='-'+0x2000;
		DisplayData_OffZero(x,3,0,p16+1);
		DisplayString(Row,15,0,"dBm");//显示字符串
	}
	else
	{
		DisplayString(Row,10,0,"----");//显示字符串
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_7_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_7_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_7_3(void)
{
	u32 Row;
	u64 x;
	u8 *p8;
	u16 *p16;

	//清全屏
	DisplayClr();//清全屏
	Row=1;
	DisplayMenuString_SH(Row,0,0,(u8 *)
																	"软件版本:\n\r"
																	"软件日期:\n\r"
																	"硬件版本:\n\r"
																	"硬件日期:\n\r"
																	"ICCID号:\n\r"
																	" \n\r"
//																	"SIM卡手机号:\n\r"
//																	" \n\r"
																	);//显示菜单字符串

	//1.软件版本
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]='Z'+0x2000;
	p16[1]='Z'+0x2000;
	p16[2]=((SVER>>4)&0xf)+0x30+0x2000;
	p16[3]=((SVER>>0)&0xf)+0x30+0x2000;
	//软件发行日期
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]=((SVDATE>>20)&0xf)+0x30+0x2000;
	p16[1]=((SVDATE>>16)&0xf)+0x30+0x2000;
	p16[2]='-'+0x2000;
	p16[3]=((SVDATE>>12)&0xf)+0x30+0x2000;
	p16[4]=((SVDATE>>8)&0xf)+0x30+0x2000;
	p16[5]='-'+0x2000;
	p16[6]=((SVDATE>>4)&0xf)+0x30+0x2000;
	p16[7]=((SVDATE>>0)&0xf)+0x30+0x2000;
	//2.硬件版本
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]='V'+0x2000;
	p16[1]=((HVER>>8)&0xf)+0x30+0x2000;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x30+0x2000;
	p16[4]=((HVER>>0)&0xf)+0x30+0x2000;
	//硬件发行日期
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]=((HVDATE>>20)&0xf)+0x30+0x2000;
	p16[1]=((HVDATE>>16)&0xf)+0x30+0x2000;
	p16[2]='-'+0x2000;
	p16[3]=((HVDATE>>12)&0xf)+0x30+0x2000;
	p16[4]=((HVDATE>>8)&0xf)+0x30+0x2000;
	p16[5]='-'+0x2000;
	p16[6]=((HVDATE>>4)&0xf)+0x30+0x2000;
	p16[7]=((HVDATE>>0)&0xf)+0x30+0x2000;
	//ICCID号
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+20,1);
	MR(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3+4+3,20);
	DisplayString(Row,0,0,p8);
	//SIM卡手机号
/*
	设置参数F243终端（SIM卡）手机号码：
	手机号码	BCD	8
	--不足16位前面补0，例如SIM卡手机号码13800138000 设置即为 "0000013800138000"
	只作为记录SIM卡手机号码跟终端对应关系
*/
	Row++;
	DisplayString(Row,0,0,"SIM卡手机号:");
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+4*2);
	x=MRR(ADDR_AFN04F243,8);
	DisplayData_OffZero(x,16,0,p16);

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_7_4);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_7_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_7_4(void)
{
	u32 x;
	u32 y;
	u32 i;
	u32 Row;
	u8 *p;
	u8 *p8;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"主通道以太网\n\r"
																	"终端地址:****-\n\r"
																	"本地IP:\n\r"
																	"\n\r"
																	"子网:\n\r"
																	"网关:\n\r"
																	"端口:\n\r"
																	);//显示菜单

	Row=2;
	//1.行政区域码
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	//2.终端地址
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+14*2);
	x=MRR(ADDR_TERMINAL,2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);

	//终端IP、子网掩码、网关
	p8=(u8 *)ADDR_DATABUFF;
	if(MRR(ADDR_AFN04F7+64,1)==0)
	{//0 固定
		MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//读终端IP、子网掩码、网关
	}
	else
	{//1 自动
		MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//读终端IP、子网掩码、网关
	}
	//终端IP
	Row++;
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[1+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//子网掩码
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[5+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//网关
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[9+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//终端侦听端口
	Row++;
	MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//外部FLASH读
	p8=(u8 *)ADDR_DATABUFF;
	i=20+1+p8[0];
	MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//外部FLASH读
	i+=1+p8[0];
	x=MRR(ADDR_AFN04F7+i,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_7);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_7_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
#else

extern const u8 UnitList[][6];//通用单位列表
extern const u8 OnlineModeList[][11];//在线模式列表
extern const u8 ConnectWayList[][6];//连接方式列表

const u8 pMenu_7[]=
{
	"终端信息\n\r"
	"  1.基本信息\n\r"
	"  2.无线信息\n\r"
	"  3.专网信息\n\r"
};

const u8 pMenu_7_1[]=
{
	"基本信息\n\r"
	"行政区码:\n\r"
	"终端地址:\n\r"
	"厂商代码:\n\r"
	"设备编号:\n\r"
	"软件版本:\n\r"
	"硬件版本:\n\r"
	"当前日期:\n\r"
	"当前时间:\n\r"
};

const u8 pMenu_7_2[]=
{
	"无线信息\n\r"
	"主IP:\n\r"
	"端口:\n\r"
	"备IP:\n\r"
	"端口:\n\r"
	"APN :\n\r"
};

const u8 pMenu_7_3[]=
{
	"专网信息\n\r"
	"在线模式:\n\r"
	"连接方式:\n\r"
	"重拨间隔:\n\r" 
	"重拨次数:\n\r"
	"断线时间:\n\r" 
	"心跳周期:\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3
// Descriptions:        终端信息专网信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_3(void)
{
	u16 *p16;
	u32 i;
	u32 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3);//显示菜单
	//工作模式
	x=MRR(ADDR_AFN04F8,1);
	DisplayString(1,10,0,(u8 *)OnlineModeList[x&0x03]);
	DisplayString(2,10,0,(u8 *)ConnectWayList[(x&0x80)>>7]);
	//重拨间隔
	x=MRR(ADDR_AFN04F8+1,2);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
	p16=DisplayData_OffZero(x,5,0,p16);
	for(i=11;i<17;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(3,i,0,(u8 *)UnitList[9]);//9 s
	//重拨次数
	x=MRR(ADDR_AFN04F8+3,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(4,i,0,(u8 *)UnitList[10]);//10 次
	//自动断线时间
	x=MRR(ADDR_AFN04F8+4,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(5,i,0,(u8 *)UnitList[8]);//8 min
	//心跳周期
	x=MRR(ADDR_AFN04F1+5,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(6,i,0,(u8 *)UnitList[8]);//8 min
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2
// Descriptions:        终端信息无线信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_2(void)
{
	u8 *p;
	u16 *p16;
	u32 i;
	u32 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2);//显示菜单
	//1.主站IP
	p16=(u16 *)(ADDR_STRINGBUFF+(84*1)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//3.备用IP
	p16=(u16 *)(ADDR_STRINGBUFF+(84*3)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+6+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//2.主站端口号
	x=MRR(ADDR_AFN04F3+4,2);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
	DisplayData_OffZero(x,5,0,p16);
	//4.备用端口号
	x=MRR(ADDR_AFN04F3+10,2);
	x=hex_bcd(x);
	p16=(u16*)(ADDR_STRINGBUFF+4*84+4+5*2);
	DisplayData_OffZero(x,5,0,p16);
	//5.APN
	p=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+16,1);
	MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
	DisplayString(5,5,0,p);//显示字符串
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1
// Descriptions:        终端信息基本信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;
	u32 Row=1;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1);//显示菜单
	//1.行政区域码
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	Row++;
	//2.终端地址
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=MRR(ADDR_TERMINAL,2);
	p16=DisplayData_OffZero(x,4,0,p16);
	p16[0]=0x2000+'H';
	p16[1]=0x2000+',';
	p16=p16+2;
	y=hex_bcd(x);
	p16=DisplayData_OffZero(y,5,0,p16);
	Row++;
	//3.厂商代号、4.设备编号、5.软件版本
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF1,4+8+4);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[i];
		p[i*2+1]=0x20;
	}
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=7;i++)
	{
		p[i*2]=p8[4+i];
		p[i*2+1]=0x20;
	}
	Row++;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[4+8+i];
		p[i*2+1]=0x20;
	}
	Row++;
	//6.硬件版本
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	Row++;
	//7.当前日期
	p8=(u8 *)ADDR_TYMDHMS;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p[0*2]=(p8[5]>>4)+0x30;//年
	p[1*2]=(p8[5]&0xf)+0x30;
	p[2*2]='-';
	p[3*2]=(p8[4]>>4)+0x30;//月
	p[4*2]=(p8[4]&0xf)+0x30;
	p[5*2]='-';
	p[6*2]=(p8[3]>>4)+0x30;//日
	p[7*2]=(p8[3]&0xf)+0x30;
	for(i=0;i<=7;i++)
	{
		p[i*2+1]=0x20;
	}
	Row++;
	//8.当前时间
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p[0*2]=(p8[2]>>4)+0x30;//时
	p[1*2]=(p8[2]&0xf)+0x30;
	p[2*2]=':';
	p[3*2]=(p8[1]>>4)+0x30;//分
	p[4*2]=(p8[1]&0xf)+0x30;
	p[5*2]=':';
	p[6*2]=(p8[0]>>4)+0x30;//秒
	p[7*2]=(p8[0]&0xf)+0x30;
	for(i=0;i<=7;i++)
	{
		p[i*2+1]=0x20;
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        终端信息菜单
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_7_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_7_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_7_3);//创建下级菜单
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
