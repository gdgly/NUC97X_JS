
//菜单8 终端参数设置
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../terminal/Terminal_AFN0C_RealTimeData_Init.h"
#include "../terminal/Terminal_AFN0D_CongealData_Init.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


//extern u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
//extern void PnVSAFN04F10SetNoList(void);//测量点号Pn对应AFN04F10配置序号列表
//extern void AFN04F150_Computer(void);//测量点有效标志计算

extern const u8 UnitList[][6];//通用单位列表
extern const u8 ProtocolList[][13];//通信协议列表

const u8 OnlineModeList[][11]=//在线模式列表
{
	"数据错误",//存储的信息有错
	"永久在线",
	"被动激活",
	"时段在线",
};

const u8 ConnectWayList[][6]=//连接方式列表
{
	"TCP",
	"UDP",
};

const u8 CommunicationSpeedList[][6]=//通信速率列表
{
	"默认 ",
	"600  ",
	"1200 ",
	"2400 ",
	"4800 ",
	"7200 ",
	"9600 ",
	"19200",
};

const u8 IpTypeList[][7]=//IP类型列表
{
	"固定",
	"自动",
	"错误",
};

const u8 pMenu_8[]=//专变、公变 有脉冲
{
	"终端参数设置\n\r"
	"  1.日期和时间设置\n\r"
	"  2.终端地址设置\n\r"
	"  3.通信参数设置\n\r"
	"  4.IP及端口号设置\n\r"
	"  5.APN设置\n\r"
	"  6.专网用户名设置\n\r"
	"  7.专网密码设置\n\r"
	"  8.电能表配置\n\r"
	"  9.终端脉冲配置\n\r"
};

const u8 pMenu1_8[]=//专变、公变 无脉冲
{
	"终端参数设置\n\r"
	"  1.日期和时间设置\n\r"
	"  2.终端地址设置\n\r"
	"  3.通信参数设置\n\r"
	"  4.IP及端口号设置\n\r"
	"  5.APN设置\n\r"
	"  6.专网用户名设置\n\r"
	"  7.专网密码设置\n\r"
	"  8.电能表配置\n\r"
};

const u8 pMenu_8_2[]=
{
	"终端地址设置\n\r"
	"  1.行政区码设置\n\r"
	"  2.逻辑地址设置\n\r"
	"  3.终端IP设置\n\r"
	"  4.MAC地址设置\n\r"
	"  5.设备编号设置\n\r"
	"  6.终端侦听端口\n\r"
};

const u8 pMenu_8_2_1[]=
{
	"行政区码设置\n\r"
	"行政区码:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_2[]=
{
	"逻辑地址设置\n\r"
	"  1.十进制\n\r"
	"  2.十六进制\n\r"
};

const u8 pMenu_8_2_2_1[]=
{
	"逻辑地址设置\n\r"
	"逻辑地址:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_3[]=
{
	"终端IP设置\n\r"
	"类型:\n\r"
	"IP  :\n\r"
	"子网:\n\r"
	"网关:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_4[]=
{
	"MAC地址设置\n\r"
	"  1.十进制\n\r"
	"  2.十六进制\n\r"
};

const u8 pMenu_8_2_4_1[]=
{
	"MAC地址设置\n\r"
	"MAC:\n\r"
	" \n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_4_2[]=
{
	"MAC地址设置\n\r"
	"MAC\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_5[]=
{
	"设备编号设置\n\r"
	"设备编号\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_2_6[]=
{
	"终端侦听端口设置\n\r"
	"侦听端口:\n\r"
	"      确认设置\n\r"
};

#if ((Project/100)==2)//集中器
const u8 pMenu2_2[]=//集中器参数设置与查看
{
	"参数设置与查看\n\r"
	"  1.通信通道设置\n\r"
	"  2.电能表参数设置\n\r"
	"  3.集中器时间设置\n\r"
	"  4.界面密码设置\n\r"
	"  5.集中器编号\n\r"
};

const u8 pMenu_8_1[]=
{
	"集中器时间设置\n\r"
	"日期:\n\r"
	"时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_3[]=
{
	"通信模式设置\n\r"
	"在线模式:\n\r"
	"连接方式:\n\r"
	"重拨间隔:\n\r"
	"重拨次数:\n\r"
	"断线时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_4[]=
{
	"主站IP及端口设置\n\r"
	"主IP:\n\r"
	"端口:\n\r"
	"备IP:\n\r"
	"端口:\n\r"
	"心跳:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_8[]=
{
	"电能表参数设置\n\r"
	"配置序号\n\r"
	"测量点\n\r"
	"通信速率\n\r"
	"通信端口\n\r"
	"通信规约\n\r"
	"通信地址\n\r"
	"采集器\n\r"
	"大类号\n\r"
	"小类号\n\r"
	"      确认设置\n\r"
};
#else
const u8 pMenu_8_1[]=
{
	"日期和时间设置\n\r"
	"日期:\n\r"
	"时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_3[]=
{
	"通信参数设置\n\r"
	"在线模式:\n\r"
	"连接方式:\n\r"
	"重拨间隔:\n\r"
	"重拨次数:\n\r"
	"断线时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_4[]=
{
	"IP及端口号设置\n\r"
	"主IP:\n\r"
	"端口:\n\r"
	"备IP:\n\r"
	"端口:\n\r"
	"心跳:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu_8_8[]=
{
	"电能表配置\n\r"
	"配置序号\n\r"
	"测量点\n\r"
	"通信速率\n\r"
	"通信端口\n\r"
	"通信规约\n\r"
	"通信地址\n\r"
	"采集器\n\r"
	"大类号\n\r"
	"小类号\n\r"
	"      确认设置\n\r"
};
#endif

const u8 pMenu_8_9[]=
{
	"终端脉冲配置\n\r"
	"端口号  :\n\r"
	"测量点  :\n\r"
	"脉冲属性:\n\r"
	"电表常数:\n\r"
	"      确认设置\n\r"
};

const u8 PulseAttributeList[][20]=//脉冲属性列表
{
	"正向有功",
	"正向无功",
	"反向有功",
	"反向无功",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1
// Descriptions:        日期和时间设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/ )//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_1);//显示菜单
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//日期
			p8=(u8 *)ADDR_TYMDHMS;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
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
			//时间
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
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
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayString(Ln+3,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=13;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
									{//29日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if(p8[7*2]>=0x30 && p8[7*2]<0x39)
								{
									p8[7*2]++;
								}
								else if(p8[7*2]==0x39)
								{
									if(p8[6*2]>=0x30 && p8[6*2]<0x39)
									{
										p8[7*2]=0x30;
										p8[6*2]++;
									}
								}
								if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>59)
								{
									p8[7*2]=0x30;
									p8[6*2]=0x30;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if(p8[6*2]>0x30 && p8[6*2]<=0x39)
								{
									if(p8[7*2]>0x30 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
								}
								else if(p8[6*2]==0x30)
								{
									if(p8[7*2]>0x31 && p8[7*2]<=0x39)
									{
										p8[7*2]--;
									}
									else if(p8[7*2]==0x31)
									{
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
										{
											p8[7*2]=0x31;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
										{
											p8[7*2]=0x30;
											p8[6*2]=0x33;
										}
										if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
										{
											p8[7*2]=0x39;
											p8[6*2]=0x32;
										}
									}
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if(p8[7*2]>0x30 && p8[7*2]<=0x39)
								{
									p8[7*2]--;
								}
								else if(p8[7*2]==0x30)
								{
									if(p8[6*2]>0x30 && p8[6*2]<=0x39)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
									else if(p8[6*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]=0x35;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						if(Stack->MenuStack[MenuCount->Count].EditRow<3)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
							DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
						}
						else//if(Stack->MenuStack[MenuCount->Count].EditRow==3)
						{
							//保存设置的数据
							DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(Ln+3,9,0,(u8*)"OK");
							p8=(u8 *)(ADDR_RTCBUFF);
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
							p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//秒
							p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//分
							p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//时
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
							p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
							p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
							p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年
//				            WriteLPCRTC(ADDR_RTCBUFF);//写内部RTC
//				            SyncLPCRTC();//同步内部RTC(读内部写外部)
							WriteRTC(ADDR_RTCBUFF);//写实时钟
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
						if(Stack->MenuStack[MenuCount->Count].EditRow<3)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30日
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//闰年2月有29日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//平年2月只有28日
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28日
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if((p8[0*2]-0x30)*10+(p8[1*2]-0x30)*1>23)
								{
									p8[0*2]=0x30;
									p8[1*2]=0x30;
								}
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>59)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x30;
								}
								if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>59)
								{
									p8[7*2]=0x30;
									p8[6*2]=0x30;
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
// Function name:       Menu_8_2_1
// Descriptions:        行政区码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_1);//显示菜单
			//显示内存中数据
			//行政区域码
			x=MRR(ADDR_AREACODE,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			DisplayData(x,4,0,p16);

			ArrowLR(Ln+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=9;
			MenuCount->v2=12;
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
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,4);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,4);
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//行政区域码BCD占2个字节(0~65535) 判断内存数据如果有错，则初始化
								for(i=0;i<=3;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//保存设置的数据
								MR(ADDR_DATABUFF,ADDR_AREACODE,2);//读行政区域码
								p16=(u16 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16[0]=(p[0]-0x30)*16*16*16+(p[2]-0x30)*16*16+(p[4]-0x30)*16+(p[6]-0x30)*1;
								MW(ADDR_DATABUFF,ADDR_AREACODE,2);//写行政区域码
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
// Function name:       Menu_8_2_2_1
// Descriptions:        逻辑地址设置（十进制）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_2_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//显示菜单
			//显示内存中数据
			//逻辑地址
			x=MRR(ADDR_TERMINAL,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			y=hex_bcd(x);
			if(y==0)
			{
				DisplayString(Ln+1,9,0,(u8*)"无效");
			}
			else
			{
				DisplayData(y,5,0,p16);
			}

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=9;
			MenuCount->v2=13;
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
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=13;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,1,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>1)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//终端地址BIN占2个字节（1~65535）0表示无效地址 判断为无效，则初始化
								for(i=0;i<=4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=5)
								{
									bcd_p8(p8,1,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//保存设置的数据
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AREACODE+2,2);//读逻辑地址
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16[0]=p8_bcd(p,5);
								MW(ADDR_DATABUFF,ADDR_AREACODE+2,2);//写逻辑地址
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>65535 || x<1)
								{
									bcd_p8(p8,1,5);
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
// Function name:       Menu_8_2_2_2
// Descriptions:        逻辑地址设置（十六进制）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//显示菜单
			//显示内存中数据
			//逻辑地址
			x=MRR(ADDR_TERMINAL,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			if(x==0)
			{
				DisplayString(Ln+1,9,0,(u8*)"无效");
			}
			else
			{
				p16 = DisplayData(x,4,0,p16);
				p16[0] = 0x2000+'H';
			}

			MenuCount->v1=9;
			MenuCount->v2=12;
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
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AREACODE+2,2);//读逻辑地址
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								for(i=0;i<=1;i++)
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MW(ADDR_DATABUFF,ADDR_AREACODE+2,2);//写逻辑地址
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								for(i=0;i<4;i++)
								{
									p16[i]=0x0;
								}
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
// Function name:       Menu_8_2_2
// Descriptions:        逻辑地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
#if ((USER/100)==11)//吉林集中器
void Menu_8_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u64 y;
	u32 Ln;

	if(Comm_Ram->Factory==0x55)
	{//工厂状态
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2);//显示菜单
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
							MenuCreate((u32)Menu_8_2_2_1);//创建下级菜单            
							break;
						case 2:
							MenuCreate((u32)Menu_8_2_2_2);//创建下级菜单
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
	{//非工厂状态
		Ln=0;//菜单从第0行开始
//		Ln=1;//菜单从第1行开始		   //20160624
		switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
		{
			case 0://任务0:初始化
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//显示菜单
				//显示内存中数据
				//交采通信地址
				x=MRR(ADDR_04000401,6);
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
				DisplayData(x,8,0,p16);
	
				ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
				MenuCount->v1=9;
				MenuCount->v2=16;
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
							DisplayString(Ln+2,6,0,(u8*)"确认设置");
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								MenuCount->v1=9;
								MenuCount->v2=16;
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
							default:
								break;
						}
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							switch(Stack->MenuStack[MenuCount->Count].EditRow)
							{
								case 1:
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,5);
									if(x<99999999)
									{
										p8_add1(p8,8);
									}
									else
									{
										bcd_p8(p8,1,8);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,8);
									if(x>1)
									{
										p8_sub1(p8,8);
									}
									else
									{
										bcd_p8(p8,99999999,8);
									}
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
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18,0,(u8*)"输");
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
									//终端地址BCD码 4字节 判断为无效，则初始化
									for(i=0;i<=7;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
										{
											break;
										}
									}
									if(i!=8)
									{
										bcd_p8(p8,1,8);
									}
									break;
								case 2:
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,(u8*)"OK");
									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_04000401,6);
									x=MRR(ADDR_04000401,6);//读交采通信地址
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									y=p8_bcd(p,8);
									y=hex_bcd16(y);
									x=(x&0xffff00000000)+y;
									MWR(x,ADDR_04000401,6);
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
									p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
									p16[0]=32;//'左右箭头'
									p16[1]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,8);
									if(x>99999999 || x<1)
									{
										bcd_p8(p8,1,8);
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
}
#else
void Menu_8_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2);//显示菜单
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
						MenuCreate((u32)Menu_8_2_2_1);//创建下级菜单            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2_2);//创建下级菜单
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
// Function name:       Menu_8_2_3
// Descriptions:        终端IP设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_3(void)
{
	u8 *p;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_3);//显示菜单
			//显示内存中数据
			//获取类型
			p8=(u8 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F7+64,1);
			if(p8[0]==0 || p8[0]==1)
			{
				DisplayString(Row,5,0,(u8 *)IpTypeList[p8[0]]);
			}
			else
			{
				DisplayString(Row,5,0,(u8 *)IpTypeList[2]);
			}
			Row++;
			
			if(MRR(ADDR_AFN04F7+64,1)==0)
			{//0 固定
				MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//读终端IP、子网掩码、网关
			}
			else
			{//1 自动
				MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//读终端IP、子网掩码、网关
			}
			//终端IP
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
			Row++;
			//子网掩码
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
			Row++;
			//网关
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

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=5;
			MenuCount->v2=8;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayString(Ln+5,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=8;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
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
								p8d=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8=(u8 *)ADDR_DATABUFF;
								switch(p8d[0])
								{
									case 0x8E://固定
										i=1;
										MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//读终端IP、子网掩码、网关
										break;
									case 0xA6://自动
										i=0;
										MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//读终端IP、子网掩码、网关
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//终端IP
								Row=Ln+2;
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
								Row++;
								//子网掩码
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
								Row++;
								//网关
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
								p8d=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8=(u8 *)ADDR_DATABUFF;
								switch(p8d[0])
								{
									case 0x8E://固定
										i=1;
										MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//读终端IP、子网掩码、网关
										break;
									case 0xA6://自动
										i=0;
										MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//读终端IP、子网掩码、网关
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//终端IP
								Row=Ln+2;
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
								Row++;
								//子网掩码
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
								Row++;
								//网关
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//IP获取类型BIN占1个字节（0固定1自动），判断内存数据有错 则初始化
								if(p8[0]==0x24)
								{//错误IP类型
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v1,0,(u8 *)IpTypeList[0]);
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,(u8*)"OK");
								Row=1;
								Row+=Ln;
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								if(p[0]==0x8E)
								{//固定
									p8[0]=0x00;
								}
								if(p[0]==0xA6)
								{//自动
									p8[0]=0x01;
								}
								MW(ADDR_DATABUFF,ADDR_AFN04F7+64,1);//写IP类型
								
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[1+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[5+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[9+i]=p8_bcd(p+8*i,3);
								}
								if(p8[0]==0)
								{//固定
									MW(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//写终端IP、子网掩码、网关
								}
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
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
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
// Function name:       Menu_8_2_4_1
// Descriptions:        MAC地址设置(十进制)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4_1);//显示菜单
			//显示内存中数据
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);//读MAC
			for(i=0;i<3;i++)
			{
				p16 = DisplayData(hex_bcd(p8[i]),3,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			for(i=0;i<3;i++)
			{
				p16 = DisplayData(hex_bcd(p8[3+i]),3,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;			
			MenuCount->v1=5;
			MenuCount->v2=15;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(Ln+3,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=15;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=15;
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
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//判断内存数据有错 则初始化
								for(i=0;i<3;i++)
								{
									for(k=4*i;k<=4*i+2;k++)
									{
										if(p8[k*2]<0x30 || p8[k*2]>0x39 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=4*i+3)
									{
										break;
									}
								}
								if(i!=3)
								{
									for(i=0;i<3;i++)
									{
										for(k=4*i;k<=4*i+2;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//判断内存数据有错 则初始化
								for(i=0;i<3;i++)
								{
									for(k=4*i;k<=4*i+2;k++)
									{
										if(p8[k*2]<0x30 || p8[k*2]>0x39 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=4*i+3)
									{
										break;
									}
								}
								if(i!=3)
								{
									for(i=0;i<3;i++)
									{
										for(k=4*i;k<=4*i+2;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 3:
								//保存设置的数据
								Row=1;
								Row+=Ln;
								MR(ADDR_DATABUFF,ADDR_AREACODE+12,6);//读MAC
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<3;i++)
								{
									p8[i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<3;i++)
								{
									p8[3+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								DisplayClrRow_4(Row);//清屏幕显示的指定行(4个控制字不清)
								if(IC_24AAxxx==0 && (p8[0]&0x01)==0)
								{
									MW(ADDR_DATABUFF,ADDR_AREACODE+12,6);//写MAC
									DisplayString(Row,9,0,(u8*)"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxx为0表示可设置；1表示已有唯一MAC，故不可设置
										DisplayString(Row,2,0,(u8*)"MAC唯一无需设置");
									}
									else
									{//(p8[0]&0x01)==1 组播地址
										DisplayString(Row,1,0,(u8*)"首字节为奇数不允许");
									}
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//键使用后清0
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
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								for(i=0;i<8;i++)
								{
									p8[11*2+i]=0x0;
								}
								if((p8[2*2]-0x30)&0x01)
								{//0单播地址 1组播地址 MAC地址为单播地址 首字节末位便是单播/组播地址
									p8[2*2]=p8[2*2]-1;
								}
								break;
							case 2:
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								for(i=0;i<8;i++)
								{
									p8[11*2+i]=0x0;
								}
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
// Function name:       Menu_8_2_4_2
// Descriptions:        MAC地址设置（十六进制）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Row=1;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4_2);//显示菜单
			//显示内存中数据
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
			MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);//读MAC
			for(i=0;i<6;i++)
			{
				p16 = DisplayData(p8[i],2,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;
			MenuCount->v1=3;
			MenuCount->v2=19;
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
						DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=3;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//判断内存数据有错 则初始化
								for(i=0;i<=5;i++)
								{
									for(k=3*i;k<=3*i+1;k++)
									{
										if(p8[k*2]<0x30 || (p8[k*2]>0x39 && p8[k*2]<0x41) || p8[k*2]>0x46 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=3*i+2)
									{
										break;
									}
								}
								if(i!=6)
								{
									for(i=0;i<=5;i++)
									{
										for(k=3*i;k<=3*i+1;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								Row=1;
								Row+=Ln;
								MR(ADDR_DATABUFF,ADDR_AREACODE+12,6);//读MAC
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
								p8=(u8 *)ADDR_DATABUFF;
								for(i=0;i<=5;i++)
								{
									if((p[(0+3*i)*2]>=0x30) && (p[(0+3*i)*2]<=0x39))
									{
										if((p[(1+3*i)*2]>=0x30) && (p[(1+3*i)*2]<=0x39))
										{
											p8[i]=(p[(0+3*i)*2]-0x30)*16+(p[(1+3*i)*2]-0x30)*1;
										}
										if((p[(1+3*i)*2]>=0x41) && (p[(1+3*i)*2]<=0x46))
										{
											p8[i]=(p[(0+3*i)*2]-0x30)*16+(p[(1+3*i)*2]-0x37)*1;
										}
									}
									if((p[(0+3*i)*2]>=0x41) && (p[(0+3*i)*2]<=0x46))
									{
										if((p[(1+3*i)*2]>=0x30) && (p[(1+3*i)*2]<=0x39))
										{
											p8[i]=(p[(0+3*i)*2]-0x37)*16+(p[(1+3*i)*2]-0x30)*1;
										}
										if((p[(1+3*i)*2]>=0x41) && (p[(1+3*i)*2]<=0x46))
										{
											p8[i]=(p[(0+3*i)*2]-0x37)*16+(p[(1+3*i)*2]-0x37)*1;
										}
									}
								}
								Row++;
								DisplayClrRow_4(Row);//清屏幕显示的指定行(4个控制字不清)
								if(IC_24AAxxx==0 && (p8[0]&0x01)==0)
								{
									MW(ADDR_DATABUFF,ADDR_AREACODE+12,6);//写MAC
									DisplayString(Row,9,0,(u8*)"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxx为0表示可设置；1表示已有唯一MAC，故不可设置
										DisplayString(Row,6,0,(u8*)"MAC 唯一");
									}
									else
									{//(p8[0]&0x01)==1 组播地址
										DisplayString(Row,4,0,(u8*)"首字节为奇数");
									}
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+3*2);
						if(p8[1*2]>=0x30 && p8[1*2]<=0x39)
						{
							if((p8[1*2]-0x30)&0x01)
							{//0单播地址 1组播地址 MAC地址为单播地址 首字节末位表示单播/组播地址
								p8[1*2]=p8[1*2]-1;
							}
						}
						else if(p8[1*2]>=0x41 && p8[1*2]<=0x46)
						{
							if((p8[1*2]-0x37)&0x01)
							{//0单播地址 1组播地址 MAC地址为单播地址 首字节末位表示单播/组播地址
								p8[1*2]=p8[1*2]-1;
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
// Function name:       Menu_8_2_4
// Descriptions:        MAC地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4);//显示菜单
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
						MenuCreate((u32)Menu_8_2_4_1);//创建下级菜单            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_4_2);//创建下级菜单
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
// Function name:       Menu_8_2_5
// Descriptions:        设备编号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_5);//显示菜单
			//显示内存中数据
			//设备编号
			p8=(u8 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);
			for(i=0;i<=7;i++)
			{
				p[i*2]=p8[i];
				p[i*2+1]=0x20;
			}
			MenuCount->v1=8;
			MenuCount->v2=15;
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
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=15;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//判断内存数据有错 则初始化
								for(i=0;i<8;i++)
								{
									if(p8[i*2]<0x30 || (p8[i*2]>0x39 && p8[i*2]<0x41) || p8[i*2]>0x46 ||p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=8)
								{
									for(i=0;i<8;i++)
									{
										p8[i*2]=0x30;
										p8[i*2+1]=0x20;
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//保存设置的数据
								MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);//读设备编号
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);
								p8=(u8 *)ADDR_DATABUFF;
								for(i=0;i<8;i++)
								{
									p8[i]=p[i*2];
								}
								MW(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);//写设备编号
								MW(ADDR_DATABUFF,ADDR_TERMINAL+2,8);//写设备编号
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								for(i=0;i<4;i++)
								{
									p16[i]=0x0;
								}
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
// Function name:       Menu_8_2_6
// Descriptions:        终端侦听端口设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2_6(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_6);//显示菜单
			//显示内存中数据
			//终端侦听端口
			MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//外部FLASH读
			p8=(u8 *)ADDR_DATABUFF;
			i=20+1+p8[0];
			MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//外部FLASH读
			i+=1+p8[0];
			x=MRR(ADDR_AFN04F7+i,2);
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
			y=hex_bcd(x);
			DisplayData(y,5,0,p16);

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=9;
			MenuCount->v2=13;
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
						DisplayString(Ln+2,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=13;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								for(i=0;i<=4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=5)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//保存设置的数据
								MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//外部FLASH读
								p8=(u8 *)ADDR_DATABUFF;
								i=20+1+p8[0];
								MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//外部FLASH读
								i+=1+p8[0];
								MR(ADDR_DATABUFF,ADDR_AFN04F7+i,2);//外部FLASH读
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16=(u16 *)ADDR_DATABUFF;
								p16[0]=p8_bcd(p,5);
								MW(ADDR_DATABUFF,ADDR_AFN04F7+i,2);//写终端侦听端口
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
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
// Function name:       Menu_8_2
// Descriptions:        终端地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu_8_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2);//显示菜单
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
						MenuCreate((u32)Menu_8_2_1);//创建下级菜单            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_8_2_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_8_2_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu_8_2_5);//创建下级菜单
						break;
					case 6:
						MenuCreate((u32)Menu_8_2_6);//创建下级菜单
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
// Function name:       Menu_8_3
// Descriptions:        通信参数设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3);//显示菜单
			DisplayString(Ln+3,15,0,(u8 *)UnitList[9]);//9 s
			DisplayString(Ln+4,13,0,(u8 *)UnitList[10]);//10 次
			DisplayString(Ln+5,13,0,(u8 *)UnitList[8]);//8 min
			//工作模式、重拨间隔、重拨次数、自动断线时间
			p=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F8,5);
			DisplayString(Ln+1,10,0,(u8 *)OnlineModeList[p[0]&0x03]);
			DisplayString(Ln+2,10,0,(u8 *)ConnectWayList[(p[0]&0x80)>>7]);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
			bcd_p8(p8,p[3],3);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
			bcd_p8(p8,p[4],3);
			//读重拨间隔
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F8+1,2);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
			bcd_p8(p8,p16[0],5);
			
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=10;
			MenuCount->v2=17;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=6)
					{
						DisplayString(Ln+6,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=10;
							MenuCount->v2=17;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://永
										i=2;
										break;
									case 0x8C://被
										i=3;
										break;
									case 0xD4://时
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 永久在线 2 被动激活 3 时段在线
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x54://TCP
										i=1;
										break;
									case 0x55://UDP
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectWayList[i]);//0 TCP,1 UDP
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://永
										i=3;
										break;
									case 0x8C://被
										i=1;
										break;
									case 0xD4://时
										i=2;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 永久在线 2 被动激活 3 时段在线
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x55://UDP
										i=0;
										break;
									case 0x54://TCP
										i=1;
										break;
								}
								DisplayString((Ln+Stack->MenuStack[MenuCount->Count].EditRow),10,0,(u8 *)ConnectWayList[i]);//0 TCP,1 UDP
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
								}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//在线方式1~3 判断内存数据如果有错，则初始化
								if(p8[0]==0x24)
								{
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[1]);//1 永久在线
								}
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 6:
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8*)"OK");
								//保存设置的数据
								p=(u8*)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								MR(ADDR_DATABUFF,ADDR_AFN04F8,5);//读工作模式、重拨间隔、重拨次数、自动掉线时间
								switch(p8[0])
								{
									case 0x8E://永
										p[0]&=0xFD;
										p[0]|=0x01;
										break;
									case 0x8C://被
										p[0]&=0xFE;
										p[0]|=0x02;
										break;
									case 0xD4://时
										p[0]|=0x03;
										break;
									default:
										break;
								}

								p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
								switch(p8[0])
								{
									case 0x54://TCP
										p[0]&=0x7F;
										break;
									case 0x55://UDP
										p[0]|=0x80;
										break;
								}
								p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
								x=p8_bcd(p8,5);
								p[1]=x&0xff;
								p[2]=x>>8;
								p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
								p[3]=p8_bcd(p8,3);
								p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
								p[4]=p8_bcd(p8,3);
								MW(ADDR_DATABUFF,ADDR_AFN04F8,5);//写工作模式、重拨间隔、重拨次数、自动掉线时间
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
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
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
// Function name:       Menu_8_4
// Descriptions:        IP及端口号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_4(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_4);//显示菜单
			DisplayString(Ln+5,8,0,(u8 *)UnitList[8]);//8 min
			//显示内存中数据
			//1.主站IP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F3,4);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//3.备用IP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			MR(ADDR_DATABUFF+4,ADDR_AFN04F3+6,4);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[4+i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//5.心跳周期
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
			MR(ADDR_DATABUFF+8,ADDR_AFN04F1+5,1);
			bcd_p8(p,p8[8],3);
			//2.主站端口号
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F3+4,2);
			bcd_p8(p,p16[0],5);
			//4.备用端口号
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
			MR(ADDR_DATABUFF+2,ADDR_AFN04F3+10,2);
			bcd_p8(p,p16[1],5);
			
			MenuCount->v1=5;
			MenuCount->v2=19;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=6)
					{
						DisplayString(Ln+6,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=9;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=5;
							MenuCount->v2=9;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=5;
							MenuCount->v2=7;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
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
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 3:
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
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
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 3:
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
								}
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
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 6:
								DisplayClrRow_4(Ln+6);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+6,9,0,(u8*)"OK");
								//保存设置的数据
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F3,12);//读主站IP、主站端口号、备用IP、备用端口号
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[i]=p8_bcd(p+8*i,3);
								}
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p,5);
								p8[4]=x&0xff;
								p8[5]=x>>8;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[6+i]=p8_bcd(p+8*i,3);
								}
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p,5);
								p8[10]=x&0xff;
								p8[11]=x>>8;
								MW(ADDR_DATABUFF,ADDR_AFN04F3,12);//写主站IP、主站端口号、备用IP、备用端口号
								
								p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								p8[12]=p8_bcd(p,3);
								MW(ADDR_DATABUFF+12,ADDR_AFN04F1+5,1);//写5.心跳周期
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
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 3:
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
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
// Function name:       Menu_8_5
// Descriptions:        APN设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------
void Menu_8_5(void)//
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 MinLn=4;//软键盘最小行号
	u32 MaxLn=8;//软键盘最大行号
	u32 LastLnMaxCol=14;//软键盘的最后一行最大列号
	u32 MinCol=0;//软键盘最小列号
	u32 MaxCol=19;//软键盘最大列号
	u32 EditLn=2;
	u32 EditCol=2;
	u32 EditLength=16;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8d;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=(u8*)" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(0,6,0,(u8*)"APN设置");
			DisplayString(MinLn,MinCol,0,p8);
			
			p=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF+16,1);
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			DisplayString(EditLn,EditCol,0,p);
			
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://任务1：键盘选择
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,EditLn,EditCol);//键盘选择
			break;
		case 2://任务2：编辑框选择
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//下键
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					Stack->MenuStack[MenuCount->Count].MoveRow ++;
					if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//左键
					if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow --;
					}
					else
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
					}
				}
				NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//保存数据
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF+16,1);
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=15;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							for(j=i;j<=15;j++)
							{
								p8[j]=0x00;
							}
							break;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
					//设置成功提示ＯＫ
					DisplayClrRow(1,9);
					DisplayString(1,0,0,p8);
					DispalyAlign(1,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,(u8*)"设置OK");
					DisplayString(5,4,0,(u8*)"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
					p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
					p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功后按任意键退出
			Stack->MenuStack[MenuCount->Count].MoveRow =0;
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
				p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}

#if (((Project/100)==3) || ((Project/100)==4) || ((Project/100)==2))//专变终端、公变终端、集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_7
// Descriptions:        专网用户名、密码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_7(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 MinLn=4;//软键盘最小行号
	u32 MaxLn=8;//软键盘最大行号
	u32 LastLnMaxCol=14;//软键盘的最后一行最大列号
	u32 MinCol=0;//软键盘最小列号
	u32 MaxCol=19;//软键盘最大列号
	u32 EditCol=0;
	u32 EditLength=20;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8d;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=(u8*)" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(MinLn,MinCol,0,p8);
			p=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF+32,1);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 6:
					DisplayString(0,3,0,(u8*)"专网用户名设置");
					MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
					break;
				case 7:
					DisplayString(0,4,0,(u8*)"专网密码设置");
					MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
					break;
			}
			DisplayString(MenuCount->v1,EditCol,0,p);
			
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			NegativeLnCol(MenuCount->v1,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://任务1：键盘选择
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,MenuCount->v1,EditCol);//键盘选择
			break;
		case 2://任务2：编辑框选择
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//下键
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//右键
					switch(MenuCount->v1)
					{
						case 1:
							Stack->MenuStack[MenuCount->Count].MoveRow ++;
							if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								(MenuCount->v1)++;
							}
							break;
						case 2:
							Stack->MenuStack[MenuCount->Count].MoveRow ++;
							if(Stack->MenuStack[MenuCount->Count].MoveRow == 12)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								(MenuCount->v1)--;
							}
							break;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//左键
					switch(MenuCount->v1)
					{
						case 1:
							if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow --;
							}
							else
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 11;
								(MenuCount->v1)++;
							}
							break;
						case 2:
							if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow --;
							}
							else
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
								(MenuCount->v1)--;
							}
							break;
					}
				}
				NegativeLnCol(MenuCount->v1,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//确认
					//保存数据
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF+32,1);
					p=(u8 *)(ADDR_STRINGBUFF+1*84+4+(EditCol)*2);
					for(i=0;i<=19;i++)
					{
						if(p[i*2]==0x20 || p[i*2]==0x00)
						{
							p[i*2]=0x00;
						}
						p8[i]=p[i*2];
					}
					p=(u8 *)(ADDR_STRINGBUFF+2*84+4+0*2);
					for(i=0;i<=11;i++)
					{
						if(p[i*2]==0x20 || p[i*2]==0x00)
						{
							p[i*2]=0x00;
						}
						p8[20+i]=p[i*2];
					}
					for(i=0;i<=31;i++)
					{
						if(p8[i]==0x00)
						{
							break;
						}
					}
					if(i!=32)
					{
						for(j=i;j<=31;j++)
						{
							p8[j]=0x00;
						}
					}
					switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
					{
						case 6:
							MW(ADDR_DATABUFF,ADDR_AFN04F16,32);
							break;
						case 7:
							MW(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
							break;
					}
					//设置成功提示ＯＫ
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,(u8*)"设置OK");
					DisplayString(5,4,0,(u8*)"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
					p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
					p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功后按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
				p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}
#endif

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8
// Descriptions:        电能表配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;
	#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8s;
	#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_SH(0,Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//显示菜单
			//显示内存中数据
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxRS485AddCarrierPn)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//测量点号
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
			if(p16[0])
			{//测量点号不为0时，显示相应的信息
				p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
				bcd_p8(p,p16[0],4);
				//通信速率及端口号、通信协议类型
				p8=(u8*)ADDR_DATABUFF;
				p=(u8*)(ADDR_STRINGBUFF+3*84+4+8*2);
				MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,2);
				DisplayString(3,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
				p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
				bcd_p8(p,p8[0] & 0x1F,2);
				switch(p8[1])
				{
					case 31://"串行接口连接窄带低压载波通信模块"接口协议
						j++;
					case 30://DL/T 645-2007
						j++;
				#if (((Project/100)==3&&(USER/100)==5))//上海专变
					case 21://上海规约
						j++;
				#endif
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
				DisplayString(5,8,0,(u8 *)ProtocolList[j]);//通信协议列表
				//通信地址
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
				DisplayData(x,12,0,p16);
				//所属采集器通信地址
				p16=(u16 *)(ADDR_STRINGBUFF+7*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
				DisplayData(x,12,0,p16);
				//大类号及小类号
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
				p=(u8*)(ADDR_STRINGBUFF+8*84+4+8*2);
				bcd_p8(p,x>>4,2);
				p=(u8*)(ADDR_STRINGBUFF+9*84+4+8*2);
				bcd_p8(p,x&0x0f,2);
			}
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].Task==2)
					{
						Stack->MenuStack[MenuCount->Count].Task=1;
						if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
						{
							MR(ADDR_STRINGBUFF+0*84+0*2,ADDR_STRINGBUFF+1*84+0*2,84*9);
							for(i=0;i<=8;i++)
							{
								StrBuff->Row[i].Row --;
							}
							DisplayClrRow_4(9);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(9,6,0,(u8*)"确认设置");
						}
						else
						{
							for(i=9;i>0;i--)
							{
								MR(ADDR_STRINGBUFF+i*84+0*2,ADDR_STRINGBUFF+(i-1)*84+0*2,84);
								StrBuff->Row[i].Row ++;
							}
							DisplayClrRow(0,0);
#if ((Project/100)==2)//集中器
							DisplayString(0,3,0,(u8*)"电能表参数设置");
#else
							DisplayString(0,5,0,(u8*)"电能表配置");
#endif

						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								bcd_p8(p,p16[0],4);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								DisplayString(3-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								bcd_p8(p,p8[0] & 0x1F,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p=(u8*)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								if(p16[0]==0)
								{//原始测量点号为0时默认DL/T 645-2007
									j=4;
								}
								else
								{
									p8=(u8*)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+5,1);
									switch(p8[0])
									{
										case 31://"串行接口连接窄带低压载波通信模块"接口协议
											j++;
										case 30://DL/T 645-2007
											j++;
									#if (((Project/100)==3&&(USER/100)==5))//上海专变
										case 21://上海规约
											j++;
									#endif	
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
								}
								DisplayString(5-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)ProtocolList[j]);//通信协议列表
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x>>4,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(9-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x&0x0f,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//默
									i=1;
								}
								else
								{
									switch(x)
									{
										case 60:
											i=2;
											break;
										case 12:
											i=3;
											break;
										case 24:
											i=4;
											break;
										case 48:
											i=5;
											break;
										case 72:
											i=6;
											break;
										case 96:
											i=7;
											break;
										case 19:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<31)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x5C && p8[1]==0x47)
								{//保留
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=6;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//低
													i=0;
												}
												else
												{
													if(p8[0]==0x54 && p8[1]==0x59)
													{//上海规约
														i=5;
													}
													else//698.45
														i=4;
												}
											}
										}
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>0)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//默
									i=7;
								}
								else
								{
									switch(x)
									{
										case 19:
											i=6;
											break;
										case 96:
											i=5;
											break;
										case 72:
											i=4;
											break;
										case 48:
											i=3;
											break;
										case 24:
											i=2;
											break;
										case 12:
											i=1;
											break;
										case 60:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,31,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//低
									i=5;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=4;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//保留
													i=6;
												}
												else
												{
													if(p8[0]==0x54 && p8[1]==0x59)//上海规约
														i=3;
													else//698.45
														i=2;
												}
											}
										}
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 3:
								//通信速率（D7~D5）(1~7)及通信端口号（D4~D0）(1~31)BIN占1个字节  无需判断内存数据是否有错
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								//通信速率（D7~D5）(1~7)及通信端口号（D4~D0）(1~31)BIN占1个字节  无需判断内存数据是否有错
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x86 && p8[1]==0x47)
								{//备用
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[0]);//0 保留
								}
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节 判断内存数据如果有错，则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//所属采集器通信地址BCD占6个字节 判断内存数据如果有错，则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 9:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 10:
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								if(p8_bcd(p8,4)>MaxRS485AddCarrierPn)
								{//测量点超过最大值 则不可设置
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"无效测量点");
								}
								else
								{
									if(MeterPnCompare(p8_bcd(p,4),p8_bcd(p8,4)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									{//测量点重复 不可设置
										DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"测量点重复");
									}
									else
									{//测量点不重复 可设置
//Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
//Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
//u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
//PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
//AFN04F150_Computer();//测量点有效标志计算
//Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表

//当通信端口为31时要进行档案同步，通信地址相同不可设
										p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										x=p8_bcd(p,2);//新端口号
										p8=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										y=p8_bcd(p8,12);//新通信地址
										y=hex_bcd16(y);
										y=Check_AFN04F10_Addr(x,y);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
										p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										i=p8_bcd(p,4);//新测量点号
										
									#if(USER/100)==13//黑龙江
										//交采测量点不能被删除
										p8=(u8 *)ADDR_DATABUFF;
										MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
										if((p8[2]+(p8[3]<<8))==1)
										{//原测量点号为1
											if((p8[4]&0x1f)==1)
											{//原端口号为1
												if(p8[5]==2)
												{//原通信协议类型为交采
													p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
													if((i!=1) || (x!=1) || (p[0]!=0x5C || p[1]!=0x50))
													{//设置的测量点号!=1或设置的端口号!=1或设置的通信协议类型不是交采
														DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,4,0,(u8*)"交采不能删除");
														break;
													}
												}
											}
										}
									#endif
										if(i==0)
										{//测量点号为0 可设置
											k=1;//k为1可设置
										}
										else
										{//测量点号不为0
											if((y!=0) && (y!=MenuCount->v3))
											{//相同通信地址的配置序号和当前设置的配置序号不同，不可设置
												k=0;//k为0不可设置
												DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"表地址重复");
											}
											else
											{
												k=1;//k为1可设置
											}
										}
										if(k==1)
										{//
											if(x!=31)
											{
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块,700=			
												if((p8[0]&0x1F)==31)
												{//原端口31进行档案同步
													Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
													Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
												}
#endif
											}
											else
											{
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块,700=			
												Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
												Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
#endif
											}
											DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
											DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8*)"OK");
											//保存设置的数据
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
											p8d=(u8 *)ADDR_DATABUFF+100;
											MR(ADDR_DATABUFF+100,ADDR_DATABUFF,LEN_AFN04F10_Pn);
											p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[0]=x&0xff;
											p8[1]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[2]=x&0xff;
											p8[3]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											if(p[0]==0x62 && p[1]==0x55)
											{//默认 D7~D5:000
												p8[4]&=0x1F;
											}
											else
											{
												switch(x)
												{
													case 60://600 D7~D5:001
														p8[4]&=0x3F;
														p8[4]|=0x20;
														break;
													case 12://1200 D7~D5:010
														p8[4]&=0x5F;
														p8[4]|=0x40;
														break;
													case 24://2400 D7~D5:011
														p8[4]&=0x7F;
														p8[4]|=0x60;
														break;
													case 48://4800 D7~D5:100
														p8[4]&=0x9F;
														p8[4]|=0x80;
														break;
													case 72://7200 D7~D5:101
														p8[4]&=0xBF;
														p8[4]|=0xA0;
														break;
													case 96://9600 D7~D5:110
														p8[4]&=0xDF;
														p8[4]|=0xC0;
														break;
													case 19://19200 D7~D5:111
														p8[4]&=0xfF;
														p8[4]|=0xE0;
														break;
													default:
														break;
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											p8[4]=(p8[4]&0xE0)+x;
											p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											if(p[0]==0xA0 && p[1]==0x4A)
											{//低压载波 31
												p8[5]=31;
											}
											else
											{
												if(p[9*2]==0x30)
												{//DL/T645-2007 30
													p8[5]=30;
												}
												else
												{
													if(p[0]==0x5C && p[1]==0x50)
													{//交流采样 2
														p8[5]=2;
													}
													else
													{
														if(p[9*2]==0x39)
														{//DL/T645-1997 1
															p8[5]=1;
														}
														else
														{
															if(p[0]==0x5C && p[1]==0x47)//保留 0
															{
																p8[5]=0;
															}
															else
															{
																if(p8[0]==0x54 && p8[1]==0x59)//上海规约
																	p8[5]=21;
																else//698.45
																	p8[5]=3;
															}
														}
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												if(p[(11-(2*i+1))*2]>=0x30 && p[(11-(2*i+1))*2]<=0x39)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
												if(p[(11-(2*i+1))*2]>=0x41 && p[(11-(2*i+1))*2]<=0x46)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												p8[20+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
											}
											p=(u8 *)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											p8[26]=((p8_bcd(p,2))<<4)+(p8_bcd(p+84,2));
			//以下为AFN04F10写入数据与原数据比较
											for(i=0;i<=(LEN_AFN04F10_Pn-1);i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=LEN_AFN04F10_Pn)
											{//写入数据与原数据不相同
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
											}
			//以上为AFN04F10写入数据与原数据比较
											if(p8[18]==0)
											{//费率为0
												p8[18]=4;//默认写入费率4
											}
											MW(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
	
											PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
											AFN04F150_Computer();//测量点有效标志计算
											Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
		
	//菜单中有对AFN04F10配置的地方若发生测量点号变化，考虑是否用上功能。
	//Init_Class1Data(pnType,Pn);//类1数据初始化(指定Pn数据)
	//Init_Class2Data(Pn);//类2数据初始化(指定的测量点数据,Pn=1~n)
	//pnType宏定义为测量点类型，可直接用此名称;Pn为从1开始的测量点号
	
											if((p8[2]+(p8[3]<<8))!=(p8d[2]+(p8d[3]<<8)))
											{//测量点号不同
												if((p8d[2]+(p8d[3]<<8))!=0)
												{
													Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//类1数据初始化(指定Pn数据)
													Init_Class2Data(p8d[2]+(p8d[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
												}
												if((p8[2]+(p8[3]<<8))!=0)
												{//新测量点号
													Init_Class1Data(pnType,p8[2]+(p8[3]<<8));//类1数据初始化(指定Pn数据)
													Init_Class2Data(p8[2]+(p8[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
												}
											}
											else
											{
												if((p8d[2]+(p8d[3]<<8))!=0)
												{//原测量点号
													for(i=6;i<=12;i++)
													{
														if(p8[i]!=p8d[i])
														{
															break;
														}
													}
													if(i!=13)
													{//通信地址变化
														Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//类1数据初始化(指定Pn数据)
														Init_Class2Data(p8d[2]+(p8d[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
													}
												}
											}
										}
									}
								}
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
					Select_SH(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					UpDownSelect_SH(Stack->MenuStack[MenuCount->Count].EditRow);
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>31 || x<1)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								break;
							case 8:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
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
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
				p8s=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
				p8s[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	//通信端口号
	p8=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
	x=p8_bcd(p8,2);
	switch(x)
	{
		case 1:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":交采 ");			
			break;
		case 2:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":485I ");			
			break;
		case 3:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":485II");			
			break;
		case 31:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":载波 ");			
			break;
		default:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)"      ");			
			break;
	}
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8
// Descriptions:        电能表配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//显示菜单
			//显示内存中数据
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxRS485AddCarrierPn)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//测量点号
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
			if(p16[0])
			{//测量点号不为0时，显示相应的信息
				p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
				bcd_p8(p,p16[0],4);
				//通信速率及端口号、通信协议类型
				p8=(u8*)ADDR_DATABUFF;
				p=(u8*)(ADDR_STRINGBUFF+3*84+4+8*2);
				MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,2);
				DisplayString(3,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
				p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
				bcd_p8(p,p8[0] & 0x1F,2);
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
#elif ((USER/100)==5)//上海用户
				switch(p8[1])
				{
					case 31://"串行接口连接窄带低压载波通信模块"接口协议
						j++;
					case 30://DL/T 645-2007
						j++;
					case 21://上海规约
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
				DisplayString(5,8,0,(u8 *)ProtocolList[j]);//通信协议列表
				//通信地址
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
				DisplayData(x,12,0,p16);
				//所属采集器通信地址
				p16=(u16 *)(ADDR_STRINGBUFF+7*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
				DisplayData(x,12,0,p16);
				//大类号及小类号
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
				p=(u8*)(ADDR_STRINGBUFF+8*84+4+8*2);
				bcd_p8(p,x>>4,2);
				p=(u8*)(ADDR_STRINGBUFF+9*84+4+8*2);
				bcd_p8(p,x&0x0f,2);
			}
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].Task==2)
					{
						Stack->MenuStack[MenuCount->Count].Task=1;
						if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
						{
							MR(ADDR_STRINGBUFF+0*84+0*2,ADDR_STRINGBUFF+1*84+0*2,84*9);
							for(i=0;i<=8;i++)
							{
								StrBuff->Row[i].Row --;
							}
							DisplayClrRow_4(9);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(9,6,0,(u8*)"确认设置");
						}
						else
						{
							for(i=9;i>0;i--)
							{
								MR(ADDR_STRINGBUFF+i*84+0*2,ADDR_STRINGBUFF+(i-1)*84+0*2,84);
								StrBuff->Row[i].Row ++;
							}
							DisplayClrRow(0,0);
#if ((Project/100)==2)//集中器
							DisplayString(0,3,0,(u8*)"电能表参数设置");
#else
							DisplayString(0,5,0,(u8*)"电能表配置");
#endif

						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,6,0,(u8*)"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								bcd_p8(p,p16[0],4);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								DisplayString(3-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//显示通信速率
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								bcd_p8(p,p8[0] & 0x1F,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p=(u8*)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								if(p16[0]==0)
								{//原始测量点号为0时默认DL/T 645-2007
#if ((Project/100)==3 && (USER/100)==10)//河北专变
									j=4;
#elif ((USER/100)==5)//上海
									j=4;
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
									j=4;
#else
									j=3;
#endif
								}
								else
								{
									p8=(u8*)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+5,1);
#if ((Project/100)==3 && (USER/100)==10)//河北专变
									switch(p8[0])
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
#elif ((USER/100)==5)//上海用户
									switch(p8[0])
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
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
									switch(p8[0])
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
									switch(p8[0])
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
								}
								DisplayString(5-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)ProtocolList[j]);//通信协议列表
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x>>4,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(9-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x&0x0f,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//默
									i=1;
								}
								else
								{
									switch(x)
									{
										case 60:
											i=2;
											break;
										case 12:
											i=3;
											break;
										case 24:
											i=4;
											break;
										case 48:
											i=5;
											break;
										case 72:
											i=6;
											break;
										case 96:
											i=7;
											break;
										case 19:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<31)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
#if ((Project/100)==3 && (USER/100)==10)//河北专变
								if(p8[0]==0x5C && p8[1]==0x47)
								{//保留
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//低
													i=0;
												}
												else
												{//威胜规约
													i=4;
												}
											}
										}
									}
								}
#elif ((USER/100)==5)//上海用户
								if(p8[0]==0x5C && p8[1]==0x47)
								{//保留
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//低
													i=0;
												}
												else
												{//上海规约
													i=4;
												}
											}
										}
									}
								}
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
								if(p8[0]==0x5C && p8[1]==0x47)
								{//保留
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//低
													i=0;
												}
												else
												{//红相规约
													i=4;
												}
											}
										}
									}
								}
#else
								if(p8[0]==0x5C && p8[1]==0x47)
								{//保留
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=4;
											}
											else//if(p8[0]==0xA0 && p8[1]==0x4A)//低
											{
												i=0;
											}
										}
									}
								}
#endif
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>0)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//默
									i=7;
								}
								else
								{
									switch(x)
									{
										case 19:
											i=6;
											break;
										case 96:
											i=5;
											break;
										case 72:
											i=4;
											break;
										case 48:
											i=3;
											break;
										case 24:
											i=2;
											break;
										case 12:
											i=1;
											break;
										case 60:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,31,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
#if ((Project/100)==3 && (USER/100)==10)//河北专变
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//低
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//保留
													i=4;
												}
												else
												{//威胜规约
													i=2;
												}
											}
										}
									}
								}
#elif ((USER/100)==5)//上海用户
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//低
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//保留
													i=4;
												}
												else
												{//上海规约
													i=2;
												}
											}
										}
									}
								}
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//低
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//保留
													i=4;
												}
												else
												{//红相规约
													i=2;
												}
											}
										}
									}
								}
#else
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//低
									i=3;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//交
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else//if(p8[0]==0x5C && p8[1]==0x47)//保留
											{
												i=4;
											}
										}
									}
								}
#endif
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 3:
								//通信速率（D7~D5）(1~7)及通信端口号（D4~D0）(1~31)BIN占1个字节  无需判断内存数据是否有错
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								//通信速率（D7~D5）(1~7)及通信端口号（D4~D0）(1~31)BIN占1个字节  无需判断内存数据是否有错
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x86 && p8[1]==0x47)
								{//备用
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[0]);//0 保留
								}
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节 判断内存数据如果有错，则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//所属采集器通信地址BCD占6个字节 判断内存数据如果有错，则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 9:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"输入");
								break;
							case 10:
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								if(p8_bcd(p8,4)>MaxRS485AddCarrierPn)
								{//测量点超过最大值 则不可设置
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"无效测量点");
								}
								else
								{
//u32 Check_AFN04F10_Pn(u32 Pn)//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
//									if(MeterPnCompare(p8_bcd(p,4),p8_bcd(p8,4)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									if(((Check_AFN04F10_Pn(p8_bcd(p8,4))!=0)&& (Check_AFN04F10_Pn(p8_bcd(p8,4))!=MenuCount->v3)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									{//测量点重复 不可设置
										DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"测量点重复");
									}
									else
									{//测量点不重复 可设置
//Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
//Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
//u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
//PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
//AFN04F150_Computer();//测量点有效标志计算
//Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表

//当通信端口为31时要进行档案同步，通信地址相同不可设
										p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										x=p8_bcd(p,2);//新端口号
										p8=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										y=p8_bcd(p8,12);//新通信地址
										y=hex_bcd16(y);
										y=Check_AFN04F10_Addr(x,y);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
										p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										i=p8_bcd(p,4);//新测量点号
										
										if(i==0)
										{//测量点号为0 可设置
											k=1;//k为1可设置
										}
										else
										{//测量点号不为0
											if((y!=0) && (y!=MenuCount->v3))
											{//相同通信地址的配置序号和当前设置的配置序号不同，不可设置
												k=0;//k为0不可设置
												DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"表地址重复");
											}
											else
											{
												k=1;//k为1可设置
											}
										}
										if(k==1)
										{//
											if(x!=31)
											{
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块,700=			
												if((p8[0]&0x1F)==31)
												{//原端口31进行档案同步
													Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
													Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
												}
#endif
											}
											else
											{
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块,700=			
												Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
												Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
#endif
											}
											DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
											DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
											//保存设置的数据
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
											p8d=(u8 *)ADDR_DATABUFF+100;
											MR(ADDR_DATABUFF+100,ADDR_DATABUFF,LEN_AFN04F10_Pn);
											p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[0]=x&0xff;
											p8[1]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[2]=x&0xff;
											p8[3]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											if(p[0]==0x62 && p[1]==0x55)
											{//默认 D7~D5:000
												p8[4]&=0x1F;
											}
											else
											{
												switch(x)
												{
													case 60://600 D7~D5:001
														p8[4]&=0x3F;
														p8[4]|=0x20;
														break;
													case 12://1200 D7~D5:010
														p8[4]&=0x5F;
														p8[4]|=0x40;
														break;
													case 24://2400 D7~D5:011
														p8[4]&=0x7F;
														p8[4]|=0x60;
														break;
													case 48://4800 D7~D5:100
														p8[4]&=0x9F;
														p8[4]|=0x80;
														break;
													case 72://7200 D7~D5:101
														p8[4]&=0xBF;
														p8[4]|=0xA0;
														break;
													case 96://9600 D7~D5:110
														p8[4]&=0xDF;
														p8[4]|=0xC0;
														break;
													case 19://19200 D7~D5:111
														p8[4]&=0xfF;
														p8[4]|=0xE0;
														break;
													default:
														break;
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											p8[4]=(p8[4]&0xE0)+x;
											p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											if(p[0]==0xA0 && p[1]==0x4A)
											{//低压载波 31
												p8[5]=31;
											}
											else
											{
												if(p[9*2]==0x30)
												{//DL/T645-2007 30
													p8[5]=30;
												}
												else
												{
													if(p[0]==0x5C && p[1]==0x50)
													{//交流采样 2
														p8[5]=2;
													}
													else
													{
														if(p[9*2]==0x39)
														{//DL/T645-1997 1
															p8[5]=1;
														}
														else
														{
#if ((Project/100)==3 && (USER/100)==10)//河北专变
															if(p[0]==0x5C && p[1]==0x47)//保留 0
															{
																p8[5]=0;
															}
															else
															{//威胜规约
																p8[5]=7;
															}
#elif ((USER/100)==5)//上海用户
															if(p[0]==0x5C && p[1]==0x47)//保留 0
															{
																p8[5]=0;
															}
															else
															{//上海规约
																p8[5]=21;
															}
#elif ((Project/100)==3 && (USER/100)==12)//四川专变
															if(p[0]==0x5C && p[1]==0x47)//保留 0
															{
																p8[5]=0;
															}
															else
															{//红相规约
																p8[5]=4;
															}
#else
//if(p[0]==0x5C && p[1]==0x47)//保留 0
															p8[5]=0;
#endif
														}
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												if(p[(11-(2*i+1))*2]>=0x30 && p[(11-(2*i+1))*2]<=0x39)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
												if(p[(11-(2*i+1))*2]>=0x41 && p[(11-(2*i+1))*2]<=0x46)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												p8[20+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
											}
											p=(u8 *)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											p8[26]=((p8_bcd(p,2))<<4)+(p8_bcd(p+84,2));
			//以下为AFN04F10写入数据与原数据比较
											for(i=0;i<=(LEN_AFN04F10_Pn-1);i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=LEN_AFN04F10_Pn)
											{//写入数据与原数据不相同
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
											}
			//以上为AFN04F10写入数据与原数据比较
											if(p8[18]==0)
											{//费率为0
												p8[18]=4;//默认写入费率4
											}
											MW(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
	
											PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
											AFN04F150_Computer();//测量点有效标志计算
											Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
	
	//菜单中有对AFN04F10配置的地方若发生测量点号变化，考虑是否用上功能。
	//Init_Class1Data(pnType,Pn);//类1数据初始化(指定Pn数据)
	//Init_Class2Data(Pn);//类2数据初始化(指定的测量点数据,Pn=1~n)
	//pnType宏定义为测量点类型，可直接用此名称;Pn为从1开始的测量点号
	
											if((p8[2]+(p8[3]<<8))!=(p8d[2]+(p8d[3]<<8)))
											{//测量点号不同
												if((p8d[2]+(p8d[3]<<8))!=0)
												{
													Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//类1数据初始化(指定Pn数据)
													Init_Class2Data(p8d[2]+(p8d[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
												}
												if((p8[2]+(p8[3]<<8))!=0)
												{//新测量点号
													Init_Class1Data(pnType,p8[2]+(p8[3]<<8));//类1数据初始化(指定Pn数据)
													Init_Class2Data(p8[2]+(p8[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
												}
											}
											else
											{
												if((p8d[2]+(p8d[3]<<8))!=0)
												{//原测量点号
													for(i=6;i<=12;i++)
													{
														if(p8[i]!=p8d[i])
														{
															break;
														}
													}
													if(i!=13)
													{//通信地址变化
														Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//类1数据初始化(指定Pn数据)
														Init_Class2Data(p8d[2]+(p8d[3]<<8));//类2数据初始化(指定的测量点数据,Pn=1~n)
													}
												}
											}
										}
									}
								}
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>31 || x<1)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								break;
							case 8:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
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
	//通信端口号
	p8=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
	x=p8_bcd(p8,2);
	switch(x)
	{
		case 1:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":交采 ");			
			break;
		case 2:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":485I ");			
			break;
		case 3:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":485II");			
			break;
		case 31:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":载波 ");			
			break;
		default:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,"      ");			
			break;
	}
}
	#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_9
// Descriptions:        终端脉冲配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8_9(void)
{
#if MaxImpInPort
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_9);//显示菜单
			MenuCount->v1=10;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxImpInPort)
			{//MenuCount->v3记录脉冲输入端口号
				MenuCount->v3=MaxImpInPort;
			}
			if(MenuCount->v3<1)
			{
				MenuCount->v3=1;
			}		
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
			bcd_p8(p8,MenuCount->v3,2);
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//显示内存中数据
			//脉冲端口所属测量点号
			x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+1,1);
			if(x)
			{//测量点不为0时显示相应的测量点号、脉冲属性、电表常数
				p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
				bcd_p8(p,x,2);
				//脉冲属性
				x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+2,1);
				DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[x&0x3]);//脉冲属性列表
				//电表常数
				x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+3,2);			
				p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
				bcd_p8(p,x,5);
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=10;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=10;
							MenuCount->v2=11;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+1,1);
							p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
							if(p[0]==0x0)
							{
								bcd_p8(p,x,2);
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=10;
							MenuCount->v2=17;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+2,1);
							p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
							if(p[0]==0x0)
							{
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[x&0x3]);//脉冲属性列表
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=10;
							MenuCount->v2=14;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+3,2);			
							p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
							if(p[0]==0x0)
							{
								bcd_p8(p,x,5);
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x<MaxImpInPort)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x<MaxRS485AddCarrierPn && x<64)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
								switch(p8[0*2])
								{
									case 0x80://正向
										switch(p8[4*2])
										{
											case 0xAE://有功
												i=1;
												break;
											case 0x1E://无功
												i=2;
												break;
										}
										break;
									case 0xE6://反向
										switch(p8[4*2])
										{
											case 0xAE://有功
												i=3;
												break;
											case 0x1E://无功
												i=0;
												break;
										}
										break;
								}
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[i]);//脉冲属性列表
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,MaxImpInPort,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									if(MaxRS485AddCarrierPn<64)
									{
										bcd_p8(p8,MaxRS485AddCarrierPn,2);
									}
									else
									{
										bcd_p8(p8,64,2);
									}
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
								switch(p8[0*2])
								{
									case 0x80://正向
										switch(p8[4*2])
										{
											case 0xAE://有功
												i=3;
												break;
											case 0x1E://无功
												i=0;
												break;
										}
										break;
									case 0xE6://反向
										switch(p8[4*2])
										{
											case 0xAE://有功
												i=1;
												break;
											case 0x1E://无功
												i=2;
												break;
										}
										break;
								}
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[i]);//脉冲属性列表
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
								if(p8_bcd(p8,2)>MaxRS485AddCarrierPn)
								{//测量点超过最大值 则不可设置
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,"无效测量点");
								}
								else
								{
									if(MeterPnCompare(MaxRS485AddCarrierPn+1,p8_bcd(p8,2)))
									{//电表测量点重复 不可设置
										DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,"测量点重复");
									}
									else
									{//电表测量点不重复
										if(PulsePnCompare(p8_bcd(p,2),p8_bcd(p8,2)))
										{//脉冲测量点号重复 判断脉冲属性
											p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
											switch(p8[0*2])
											{
												case 0x80://正向
													switch(p8[4*2])
													{
														case 0xAE://有功
															i=0;
															break;
														case 0x1E://无功
															i=1;
															break;
													}
													break;
												case 0xE6://反向
													switch(p8[4*2])
													{
														case 0xAE://有功
															i=2;
															break;
														case 0x1E://无功
															i=3;
															break;
													}
													break;
											}
											p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
											for(k=0;k<MaxImpInPort;k++)
											{
												if(k!=MenuCount->v3-1)
												{
													if(p8_bcd(p8,2) == MRR(ADDR_AFN04F11+5*k+1,1) && i == MRR(ADDR_AFN04F11+5*(k)+2,1))
													{
														break;
													}
												}
											}
											if(k == MaxImpInPort)
											{//脉冲属性不重复 可设置
												DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
												DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
												//保存设置的数据
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
												p8[0]=p8_bcd(p,1);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
												p8[1]=p8_bcd(p,2);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
												switch(p[0*2])
												{
													case 0x80://正向
														switch(p[4*2])
														{
															case 0xAE://有功
																i=0;
																break;
															case 0x1E://无功
																i=1;
																break;
														}
														break;
													case 0xE6://反向
														switch(p[4*2])
														{
															case 0xAE://有功
																i=2;
																break;
															case 0x1E://无功
																i=3;
																break;
														}
														break;
												}
												p8[2]=(p8[2]&0xFC)+i;
												p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
												x=p8_bcd(p,5);
												p8[3]=(x&0xff);
												p8[4]=(x>>8);
												MW(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
				//以下为AFN04F11写入数据与原数据比较
												p8d=(u8 *)(ADDR_AFN04F11+5*(MenuCount->v3-1));
												for(i=0;i<5;i++)
												{
													if(p8[i]!=p8d[i])
													{
														break;
													}
												}
												if(i!=5)
												{//写入数据与原数据不相同
													Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
												}
				//以上为AFN04F11写入数据与原数据比较
											}
											else
											{
												DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
												DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,6,0,"属性重复");
											}
										}
										else
										{
											DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
											DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
											//保存设置的数据
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
											p8[0]=p8_bcd(p,1);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
											p8[1]=p8_bcd(p,2);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
											switch(p[0*2])
											{
												case 0x80://正向
													switch(p[4*2])
													{
														case 0xAE://有功
															i=0;
															break;
														case 0x1E://无功
															i=1;
															break;
													}
													break;
												case 0xE6://反向
													switch(p[4*2])
													{
														case 0xAE://有功
															i=2;
															break;
														case 0x1E://无功
															i=3;
															break;
													}
													break;
											}
											p8[2]=(p8[2]&0xFC)+i;
											p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
											x=p8_bcd(p,5);
											p8[3]=(x&0xff);
											p8[4]=(x>>8);
											MW(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
			//以下为AFN04F11写入数据与原数据比较
											p8d=(u8 *)(ADDR_AFN04F11+5*(MenuCount->v3-1));
											for(i=0;i<5;i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=5)
											{//写入数据与原数据不相同
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
											}
			//以上为AFN04F11写入数据与原数据比较
										}
									}
								}
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>MaxImpInPort || x<1)
								{
									bcd_p8(p8,1,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
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
#endif
}

#if ((USER/100)==5)//上海用户
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_SH_8_10
// Descriptions:        SIM卡手机号码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_SH_8_10(void)
{
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u32 Ln;
#if ((Project/100)==3)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"SIM卡手机号设置\n\r"
																			" \n\r"
																			"      确认设置\n\r"
																			);//显示菜单
			/*显示内存中数据*/
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+2*2);
			x=MRR(ADDR_AFN04F243,8);
			DisplayData(x,16,0,p16);

			MenuCount->v1=2;
			MenuCount->v2=17;
			ArrowLR(Ln+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
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
							MenuCount->v1=2;
							MenuCount->v2=17;
							ArrowLR(Ln+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+2))
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8_add1(p8,16);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8_sub1(p8,16);
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
								DisplayString(Ln+1,18,0,"输");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
								//保存设置的数据
								MR(ADDR_DATABUFF,ADDR_AFN04F243,8);//读SIM卡手机号码
								p8d=(u8 *)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+2*2);
								for(i=0;i<8;i++)
								{
									p8d[7-i]=((p8[4*i]-0x30)*16)+(p8[2+4*i]-0x30);
								}
								MW(ADDR_DATABUFF,ADDR_AFN04F243,8);//写SIM卡手机号码
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+18*2);
								p16[0]=32;//'左右箭头'
								p16[1]=33;
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
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        终端参数设置菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
#if ((Project/100)==3 || (Project/100)==4)//专变终端、公变终端
	#if ((Project/100)==3 && ((USER/100)==5 || (USER/100)==12) || (USER/100==13))//上海专变、四川专变、黑龙江
void Menu_8(void)//菜单8
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"终端参数设置\n\r"
																			"  1.SIM卡手机号设置\n\r"
																			"  2.终端地址设置\n\r"
																			"  3.通信参数设置\n\r"
																			"  4.IP及端口号设置\n\r"
																			"  5.APN设置\n\r"
																			"  6.专网用户名设置\n\r"
																			"  7.专网密码设置\n\r"
																			"  8.电能表配置\n\r"
																			"  9.终端脉冲配置\n\r"
																			"  10.日期和时间设置\n\r"
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
#if ((Project/100)==3 && (USER/100)==5)//上海专变
						MenuCreate((u32)Menu_SH_8_10);//创建下级菜单 SIM卡手机号码设置
#endif
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);//创建下级菜单
						break;
					case 5:
						DisplayClr();
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
#endif
						MenuCreate((u32)Menu_8_5);//创建下级菜单
						break;
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
#endif
						MenuCreate((u32)Menu_8_7);//创建下级菜单
						break;
					case 8:
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//上海专变、黑龙江
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
#endif
						MenuCreate((u32)Menu_8_8);//创建下级菜单
						break;
					case 9:
						MenuCreate((u32)Menu_8_9);//创建下级菜单
						break;
					case 10:
						MenuCreate((u32)Menu_8_1);//创建下级菜单 时间和日期设置            
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
void Menu_8(void)//菜单8
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
		#if (MaxImpInPort)//有脉冲
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8);//显示菜单
		#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu1_8);//显示菜单
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
						MenuCreate((u32)Menu_8_1);//创建下级菜单            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);//创建下级菜单
						break;
					case 5:
						DisplayClr();
						MenuCreate((u32)Menu_8_5);//创建下级菜单
						break;
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
						MenuCreate((u32)Menu_8_7);//创建下级菜单
						break;
					case 8:
						MenuCreate((u32)Menu_8_8);//创建下级菜单
						break;
					case 9:
					#if (MaxImpInPort)//有脉冲
						MenuCreate((u32)Menu_8_9);//创建下级菜单
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
	#endif
#endif
#if ((Project/100)==2)//集中器
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_3
// Descriptions:        信道类型设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3(void)//信道类型设置
{
	DisplayClr();
	DisplayString(4,1,0,"当前自适应无需设置");
	DisplayString(5,4,0,"按任意键返回");
	Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
	if((Comm_Ram->ButtonVal&0x3F)!=0)
	{
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1
// Descriptions:        通道详细设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1(void)//通道详细设置
{
#if ((USER/100)==11)//吉林
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"通道详细设置\n\r"
																		"  1.主站IP及端口设置\n\r"
																		"  2.APN设置\n\r"
																		"  3.终端IP设置\n\r"
																		"  4.终端侦听端口设置\n\r"
																		"  5.MAC地址设置\n\r"
																	#if ((USER/100)==5)//上海用户
																		"  6.SIM卡手机号设置\n\r"
																	#endif
																	#if ((USER/100)==11)//吉林
																		"  6.专网用户名设置\n\r"
																		"  7.专网密码设置\n\r"
																	#endif
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
						MenuCreate((u32)Menu_8_4);//主站IP及端口设置           
						break;
					case 2:
						DisplayClr();
					#if ((USER/100)==11)//吉林
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
					#endif
						MenuCreate((u32)Menu_8_5);//APN设置           
						break;
					case 3:
						MenuCreate((u32)Menu_8_2_3);//终端IP设置           
						break;
					case 4:
						MenuCreate((u32)Menu_8_2_6);//终端侦听端口设置           
						break;
					case 5:
						MenuCreate((u32)Menu_8_2_4);//MAC地址设置           
						break;
				#if ((USER/100)==5)//上海用户
					case 6:
						MenuCreate((u32)Menu_SH_8_10);//SIM卡手机号设置
						break;
				#endif
				#if ((USER/100)==11)//吉林
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
						MenuCreate((u32)Menu_8_7);//创建下级菜单
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1
// Descriptions:        通信通道设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void)//通信通道设置（信道类型设置、通信模式设置、通道详细设置）
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"通信通道设置\n\r"
																		"  1.通道详细设置\n\r"
																		"  2.通信模式设置\n\r"
																		"  3.信道类型设置\n\r"
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
						MenuCreate((u32)Menu2_2_1_1);//通道详细设置           
						break;
					case 2:
						MenuCreate((u32)Menu_8_3);//通信模式设置           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_3);//信道类型设置           
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
// Function name:       Menu2_5
// Descriptions:        集中器编号设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_5(void)//集中器编号设置
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"集中器编号设置\n\r"
																		"  1.行政区域代码设置\n\r"
																		"  2.集中器地址设置\n\r"
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
						MenuCreate((u32)Menu_8_2_1);//行政区域码设置            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2);//终端地址设置            
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
// Function name:       Menu2_2
// Descriptions:        集中器参数设置与查看
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2(void)//集中器参数设置与查看
{
#if ((USER/100)==11)//吉林
u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2);//显示菜单
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
						MenuCreate((u32)Menu2_2_1);//通信通道设置（信道类型设置、通信模式设置、通道详细设置）       
						break;
					case 2:
					#if ((USER/100)==11)//吉林
						p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
					#endif
						MenuCreate((u32)Menu_8_8);//电能表参数设置
						break;
					case 3:
						MenuCreate((u32)Menu_8_1);//日期和时间设置
						break;
					case 4:
						MenuCreate((u32)Menu_9_5);//界面密码设置
						break;
					case 5:
						MenuCreate((u32)Menu2_5);//集中器编号设置
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
/*
//20120718测量点或脉冲属性变更时，要清除原测量点、设置后测量点的1类数据、日月电能量等。
//暂时菜单中配置测量点时，没有初始化这些数据。
	x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//原测量点号和脉冲属性
	MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5),5);
	y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//设置后测量点号和脉冲属性
	if(x!=y)
	{//测量点号或脉冲属性变化
		x&=0xff;
		y&=0xff;
		Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
		Init_Class1Data_PnMap(pnType,x);//类1数据初始化(指定Pn数据),Pn映射后的Pn
		Init_Class1Data_PnMap(pnType,y);//类1数据初始化(指定Pn数据),Pn映射后的Pn
		//x=MapPn(x);//测量点映射Pn值
		//y=MapPn(y);//测量点映射Pn值
		Init_EnergyInc_Souser_Pn(x);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
		Init_EnergyInc_Souser_Pn(y);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
		Init_Energy_Pn(x);//测量点日月电能量清为0(指定的Pn数据)
		Init_Energy_Pn(y);//测量点日月电能量清为0(指定的Pn数据)
	}
//Init_Class1Data(pnType,Pn);//类1数据初始化(指定Pn数据)
//Init_Class2Data(Pn);//类2数据初始化(指定的测量点数据,Pn=1~n)
//pnType宏定义为测量点类型，可直接用此名称;Pn为从1开始的测量点号

Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
AFN04F150_Computer();//测量点有效标志计算
Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
*/
