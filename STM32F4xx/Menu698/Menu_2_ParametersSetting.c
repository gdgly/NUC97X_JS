
//菜单2 终端参数设置
#include "Project698.h"
#include "../Display/Display.h"
#include "../Display/Draw.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/RTC.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_JSON.h"

extern u64 Get_TSA_BY_PN(u32 Pn);//根据配置序号，得到TSA。不包含数据类型。
extern const u8 UnitList[][6];//通用单位列表
extern const u8 ProtocolList[][13];




const u8 pMenu2_2_Project2[]=//集中器参数设置与查看
{
	"参数设置与查看\n\r"
	"  1.通信通道设置\n\r"
	"  2.电表参数设置\n\r"
	"  3.集中器时间设置\n\r"
	"  4.界面密码设置\n\r"
	"  5.集中器编号\n\r"
};
const u8 pMenu2_2_Project3[]=//专变参数设置与查看
{
	"参数设置与查看\n\r"
	"  1.通信通道设置\n\r"
	"  2.电表参数设置\n\r"
	"  3.终端时间设置\n\r"
	"  4.界面密码设置\n\r"
	"  5.终端编号\n\r"
	#if (USER/100)==15
	"  6.设备地理位置\n\r"
	#endif
};

#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
const u8 pMenu2_2_1[]=
{	
	"通信通道设置\n\r"
	"  1.无线公网设置\n\r"
	"  2.以太网设置\n\r"
	"  3.公网远程通信\n\r"
};
#else
const u8 pMenu2_2_1[]=
{	
	"通信通道设置\n\r"
	"  1.无线公网设置\n\r"
	"  2.以太网设置\n\r"
};

#endif
#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
const u8 pMenu2_2_2[]=
{
	"配置序号\n\r"
	"波特率\n\r"
	"通信端口\n\r"
	"通信规约\n\r"
	"地址\n\r"
	"用户类型\n\r"
	"采集器\n\r"
	"      确认设置\n\r"
};
#else
const u8 pMenu2_2_2[]=
{
	"电能表参数设置\n\r"
	"配置序号\n\r"
	"波特率\n\r"
	"通信端口\n\r"
	"通信规约\n\r"
	"地址\n\r"
	"费率数\n\r"
	"接线方式\n\r"
	"采集器\n\r"
	"      确认设置\n\r"
};
#endif
const u8 pMenu2_2_3[]=
{
	"时间设置\n\r"
	"日期:\n\r"
	"时间:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_2_5[]=
{
	"通信地址设置\n\r"
	"地址\n\r"
	"      确认设置\n\r"
};
#if(USER/100)==15
const u8 pMenu2_2_6[]=
{
	"设备地理位置设置\n\r"
	"经度方位:\n\r"
	"经度:\n\r"
	"纬度方位:\n\r"
	"纬度:\n\r"
	"高度:\n\r"
	"      确认设置\n\r"
};
#endif
const u8 pMenu2_2_4[]=
{
	"菜单密码\n\r"
	"密码:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_2_1_1[]=
{
	"无线公网设置\n\r"
	"  1.通信模式设置\n\r"
	"  2.侦听端口设置\n\r"
	"  3.主站通信参数设置\n\r"
	"  4.APN设置\n\r"
	"  5.用户名设置\n\r"
	"  6.密码设置\n\r"
};

#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
const u8 pMenu2_2_1_1_1[]=
{
	"工作模式:\n\r"
	"在线方式:\n\r"
	"连接方式:\n\r"
	"应用方式:\n\r"
	"超时时间:\n\r"
	"重发次数:\n\r"
	"心跳周期:\n\r"
	"      确认设置\n\r"
};
#else
const u8 pMenu2_2_1_1_1[]=
{
	"通信模式设置\n\r"
	"工作模式:\n\r"
	"在线方式:\n\r"
	"连接方式:\n\r"
	"应用方式:\n\r"
	"超时时间:\n\r"
	"重发次数:\n\r"
	"心跳周期:\n\r"
	"      确认设置\n\r"
};
#endif


const u8 pMenu2_2_1_1_2[]=
{
	"侦听端口设置\n\r"
	"侦听端口:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_2_1_1_3[]=
{
	"主站IP及端口设置\n\r"
	"主IP:\n\r"
	"主端口:\n\r"
	"备IP:\n\r"
	"备端口:\n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_2_1_2[]=
{
	"以太网设置\n\r"
	"  1.通信配置\n\r"
	"  2.侦听端口设置\n\r"
	"  3.主站通信参数设置\n\r"
	"  4.终端IP设置\n\r"
	"  5.PPPoE用户名设置\n\r"
	"  6.PPPoE密码设置\n\r"
	"  7.MAC地址设置\n\r"
};
#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
const u8 pMenu2_2_1_2_1[]=
{
	"通信配置\n\r"
	"工作模式:\n\r"
	"连接方式:\n\r"
	"应用方式:\n\r"
	"超时时间:\n\r"
	"重发次数:\n\r"
	"心跳周期:\n\r"
	"      确认设置\n\r"
};
#else
const u8 pMenu2_2_1_2_1[]=
{
	"通信配置\n\r"
	"工作模式:\n\r"
	"连接方式:\n\r"
	"应用方式:\n\r"
	"超时时间:\n\r"
	"重发次数:\n\r"
	"心跳周期:\n\r"
	"      确认设置\n\r"
};
#endif
const u8 pMenu2_2_1_2_4[]=
{
	"终端IP设置\n\r"
	"类型:\n\r"
	"IP  :\n\r"
	"子网:\n\r"
	"网关:\n\r"
	"      确认设置\n\r"
};


const u8 pMenu2_2_1_2_6[]=
{
	"MAC地址设置\n\r"
	"  1.十进制\n\r"
	"  2.十六进制\n\r"
};

const u8 pMenu2_2_1_2_6_1[]=
{
	"MAC地址设置\n\r"
	"MAC:\n\r"
	" \n\r"
	"      确认设置\n\r"
};

const u8 pMenu2_2_1_2_6_2[]=
{
	"MAC地址设置\n\r"
	"MAC\n\r"
	"      确认设置\n\r"
};

const u8 IpTypeList[][7]=//IP类型列表
{
	"DHCP ",//0
	"静态 ",//1
	"PPPoE",//2
};




const u8 OnlineModeList[][9]=//在线模式列表
{
	"永久在线", //0
	"被动激活", //1
};

const u8 WorkModeList[][7]=//工作模式列表
{
	"混合  ",   //0
	"客户机", //1
	"服务器", //2
};

const u8 ConnectList[][4]=//连接方式列表
{
	"TCP",   //0
	"UDP",   //1
};

const u8 ApplicationList[][7]=//连接方式列表
{
	"主备  ",   //0
	"多连接",   //1
};

const u8 CommunicationSpeedList[][7]=//通信速率列表
{
	"300   ",      //0
	"600   ",     //1
	"1200  ",     //2
	"2400  ",     //3
	"4800  ",     //4
	"7200  ",     //5
	"9600  ",     //6
	"19200 ",     //7
	"38400 ",     //8
	"57600 ",     //9
	"115200",    //10
	"自适应",    //255
};


const u8 PortList[][13]=//通信协议列表
{
	"未知        ",//0 
	"交采        ",
	"485-1       ",
	"485-2       ",
	"载波        ",
};

const u8 ConnectType[][9]=//接线方式
{
	"未知    ",
	"单相    ",
	"三相三线",
	"三相四线"
};

#if (USER/100)==15
const u8 Longitude[][5]=//经度方位
{
	"东经",//0
	"西经",
};
const u8 Latitude[][5]=//纬度方位
{
	"南纬",//0
	"北纬",
};
const u8 BearingUnits[][3]=//方位单位
{
	"度",
	"分",
	"秒",
};

#endif

const u8 default_6000[]={0x02,0x04,
						0x12,0x00,0x00,
						0x02,0x0a,
							0x55,0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x00,
							0x16,0xff, //波特率自适应
							0x16,0x00,// 规约 未知
							0x51,0xe2,0x01,0x02,0x01, //端口 未知
							0x09,0x02,0x00,0x00,
							0x11,0x00,//费率个数 0
							0x11,0x00,
							0x16,0x01,
							0x12,0x08,0x98,
							0x12,0x00,0x0f,
						0x02,0x04,
							0x55,0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x00,
							0x09,0x02,0x00,0x00,
							0x12,0x00,0x01,
							0x12,0x00,0x01,
						0x01,0x00
	                    };


//更新非固定长度的成员内容 适用于 DataType_bit_string  DataType_octet_string  DataType_visible_string,长度是不超过的256的。
//成功返回1 失败返回0 
//输入参数 u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data, 同 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);
//u32 New_data_addr,u32 New_data_len 成员新数据的地址和长度
u32 Update_ElementDATA_unfixed(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data,u32 New_data_addr,u32 New_data_len)
{
	//新数据先存放到ADDR_128KDATABUFF+32K的地方
	u32 old_addr;
	u32 perupdate_datalen;//更新前,改成员的长度，包括数据类型那一字节的
	u32 perupdate_sumlen;//更新前整个结构的长度，用于判断更新后的总长是否会超出LENmax_Data
	u32 len;
//	u8 *p;
	u32 DataType;
	old_addr=Get_ElementADDR(ADDR_Data,Element,SubElement,LENmax_Data);//计算元素地址(实际地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
	if(old_addr==0)//要更新的成员地址不存在
	{
		return 1;
	}
	DataType=MRR(old_addr,1);
	perupdate_datalen=Get_DL698DataLen_S((u8 *)old_addr,LENmax_Data);
	perupdate_sumlen=Get_DL698DataLen_S((u8 *)ADDR_Data,LENmax_Data);
	if(New_data_len>(perupdate_datalen-1))//更新后的长度是增加的
	{
		if((New_data_len-(perupdate_datalen-1))>(LENmax_Data-perupdate_sumlen))//剩余字节不够用
		{
			return 1;
		}
	}
//	p=(u8 *)ADDR_128KDATABUFF+32*1024;
	MR(ADDR_128KDATABUFF+32*1024,ADDR_Data,old_addr-ADDR_Data+1);//先存插入点前的数据 + 插入点的数据类型那一位
	len=old_addr-ADDR_Data+1;
	if(DataType==DataType_TSA)//TSA特别 55 07 05 00 00 00 00 00 00 
	{
		MWR(New_data_len+1,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
		MWR(New_data_len-1,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
	}
	else if(DataType==DataType_array){//增加一组成员，成员不能是ARRAY 或 STRUCTUCE
		;
	}
	else
	{
		MWR(New_data_len,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
	}
	MR(ADDR_128KDATABUFF+32*1024+len,New_data_addr,New_data_len);//存 更新的那些数据 
	len+=New_data_len;
	MR(ADDR_128KDATABUFF+32*1024+len,ADDR_Data+(old_addr-ADDR_Data)+perupdate_datalen,perupdate_sumlen-(old_addr-ADDR_Data)-perupdate_datalen);//补上更新点后不变成员的数据
	len+=(perupdate_sumlen-(old_addr-ADDR_Data)-perupdate_datalen);
	MW(ADDR_128KDATABUFF+32*1024,ADDR_Data,len);
	return 0;
}
//更新固定长度的成员内容 
//输入参数 u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data, 同 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);
//u64 New_data,u32 New_data_len
//成功返回1 失败返回0 
u32 Update_ElementDATA_fixed(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data,u32 New_data_addr,u32 New_data_byte)
{
	u32 ret;
	ret=Get_ElementADDR(ADDR_Data,Element,SubElement,LENmax_Data);//计算元素地址(实际地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
	if(ret==0)//要更新的成员地址不存在
	{
		return 1;
	}
	MW(New_data_addr,ret+1,New_data_byte);
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_1
// Descriptions:        通信模式设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Flag;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Flag=0;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
				//清全屏
				DisplayClr();//清全屏
				Draw_Clr();
				DisplayMenuString(Ln+1,0,0,(u8 *)pMenu2_2_1_1_1);
			#else
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_1);//显示菜单
			#endif
			//工作模式:混合模式（0），客户机模式（1），服务器模式（2）
			p8=Get_Element((u8*)ADDR_4500_2,1,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+1,10,0,(u8 *)WorkModeList[p8[1]&0x03]);
			}
		  //在线方式:永久在线（0），被动激活（1）
			p8=Get_Element((u8*)ADDR_4500_2,2,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+2,10,0,(u8 *)OnlineModeList[p8[1]&0x03]);
			}
		  //连接方式:TCP（0），UDP（1）
			p8=Get_Element((u8*)ADDR_4500_2,3,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+3,10,0,(u8 *)ConnectList[p8[1]&0x03]);
			}
		  //连接应用方式:主备模式（0），多连接模式（1）
			p8=Get_Element((u8*)ADDR_4500_2,4,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+4,10,0,(u8 *)ApplicationList[p8[1]&0x03]);
			}
		  //超时时间及重发次数：bit7~bit2：超时时间（秒）bit1~bit0：重发次数
			p8=Get_Element((u8*)ADDR_4500_2,11,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				//超时时间（秒）
				i=(p8[1]>>2);
				p=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
				bcd_p8(p,i,3);
				DisplayString(Ln+5,13,0,(u8 *)UnitList[9]);//0 s
				//重发次数
				i=(p8[1]&0x03);
				p=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
				bcd_p8(p,i,1);
				DisplayString(Ln+6,11,0,(u8 *)UnitList[10]);//1 次
			}
			//心跳周期(秒)  long-unsigned
			p8=Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
        		i=p8[1];
				i<<=8;
				i|=p8[2];
				p=(u8*)ADDR_STRINGBUFF+(Ln+7)*84+4+10*2;
				bcd_p8(p,i,5);
			  DisplayString(Ln+7,15,0,(u8 *)UnitList[9]);//0 s				
			}
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=8)
					{
						DisplayString(Ln+8,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://在线方式:永久在线（0），被动激活（1）
							MenuCount->v1=10;
							MenuCount->v2=17;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://连接方式:TCP（0），UDP（1）
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4://连接应用方式:主备模式（0），多连接模式（1）
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5://超时时间
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6://重发次数
							MenuCount->v1=10;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7://心跳周期(秒)  long-unsigned
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
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
							case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://混
										i=1;
										break;
									case 0xF8://客
										i=2;
										break;
									case 0x7A://服
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://在线方式:永久在线（0），被动激活（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://永
										i=1;
										break;
									case 0x8C://被
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 永久在线 2 被动激活 3 时段在线
								break;
							case 3://连接方式:TCP（0），UDP（1）
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 4://连接应用方式:主备模式（0），多连接模式（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://主
										i=1;
										break;
									case 0x82://多
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 5://超时时间
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<63)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 6://重发次数
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x<3)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 7://心跳周期(秒)  long-unsigned
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
							case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://混
										i=2;
										break;
									case 0xF8://客
										i=0;
										break;
									case 0x7A://服
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://在线方式:永久在线（0），被动激活（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://永
										i=1;
										break;
									case 0x8C://被
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 永久在线 2 被动激活 3 时段在线
								break;
							case 3://连接方式:TCP（0），UDP（1）
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 4://连接应用方式:主备模式（0），多连接模式（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://主
										i=1;
										break;
									case 0x82://多
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 5://超时时间
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,63,3);
								}
								break;
							case 6://重发次数
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>0)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,3,1);
								}
								break;
							case 7://心跳周期(秒)  long-unsigned
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 5://超时时间
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 6://重发次数
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 7://心跳周期(秒)  long-unsigned
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 8://确认设置
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//保存设置的数据
								//1.工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								x=Get_ElementADDR(ADDR_4500_2,1,0,LENmax_4500_2);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
									switch(p8[0])
									{
										case 0x46://混
											i=0;
											break;
										case 0xF8://客
											i=0x01;
											break;
										case 0x7A://服
											i=0x02;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
								}
								
								//2.在线方式:永久在线（0），被动激活（1）
								x=Get_ElementADDR(ADDR_4500_2,2,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
									switch(p8[0])
									{
										case 0x8E://永
											i=0;
											break;
										case 0x8C://被
											i=0x01;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
								}
								//3.连接方式:TCP（0），UDP（1）
								x=Get_ElementADDR(ADDR_4500_2,3,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
									switch(p8[0])
									{
										case 0x54://TCP
											i=0;
											break;
										case 0x55://UDP
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//4.连接应用方式:主备模式（0），多连接模式（1）
								x=Get_ElementADDR(ADDR_4500_2,4,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)								
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
									switch(p8[0])
									{
										case 0x30://主
											i=0;
											break;
										case 0x82://多
											i=0x01;
											break;
									}
									MWR(i,x+1,1);									
								}
								//5.超时时间及重发次数：bit7~bit2：超时时间（秒）bit1~bit0：重发次数
								x=Get_ElementADDR(ADDR_4500_2,11,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									//超时时间（秒）
									p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
									i=p8_bcd(p8,3);
									i<<=2;
									//重发次数
									p8=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
									k=p8_bcd(p8,1);
									i|=k;
									MWR(i,x+1,1);									
								}
								//7.心跳周期(秒)  long-unsigned
								x=Get_ElementADDR(ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+7)*84+4+10*2;
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);									
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
					i=Stack->MenuStack[MenuCount->Count].EditRow;
					if(i>0)
					{
						i--;
						Stack->MenuStack[MenuCount->Count].EditRow=i;
						Flag=1;
					}
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if(Flag!=0)
					{
						i=Stack->MenuStack[MenuCount->Count].EditRow;
						i++;
						Stack->MenuStack[MenuCount->Count].EditRow=i;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 5://超时时间
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>63)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 6://重发次数
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>3)
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 7://7.心跳周期(秒)
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
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_2
// Descriptions:        侦听端口设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_2);//显示菜单
			p8=Get_Element((u8*)ADDR_4500_2,5,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{
				if(p8[1])
				{//数组个数不为0，显示第1个侦听端口
					i=p8[3];
					i<<=8;
					i|=p8[4];
					p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+9*2;
					bcd_p8(p,i,5);					
				}
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
						DisplayString(Ln+2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{					
						case 1://侦听端口
							MenuCount->v1=9;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://确认设置
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
							case 1://侦听端口  long-unsigned
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
							case 1://侦听端口  long-unsigned
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
								//保存设置的数据
							  //侦听端口
								x=Get_ElementADDR(ADDR_4500_2,5,1,LENmax_4500_2);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									MWR(k,x+1,2);
								}
								else//原来是空的 
								{
									MR(ADDR_DATABUFF,ADDR_4500_2,LENmax_4500_2);
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									k<<=8;
									k|=0x12;//成员类型是0X12
									k<<=8;
									k|=1;//ARRAY的个数是1
									Update_ElementDATA_unfixed(ADDR_DATABUFF,5,0,LENmax_4500_2,(u32)&k,4);
									MW(ADDR_DATABUFF,ADDR_4500_2,LENmax_4500_2);
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
// Function name:       Menu2_2_1_1_3
// Descriptions:        主站通信参数设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;
#if (USER/100)==15
  u8 *p8a;
#endif
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif ((Project/100)==2&&(USER/100)==17) //江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_3);//显示菜单
			//IP
			p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//端口
			p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//端口
				p=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			//备IP
			p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//备端口
			p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//端口
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			MenuCount->v1=5;
			MenuCount->v2=21;
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
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{	
						case 1://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 2://端口
							MenuCount->v1=7;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 4://端口
							MenuCount->v1=7;
							MenuCount->v2=11;
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
							case 1://IP 按右键不加
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP 按左键不减
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2://端口
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,"OK");
								//保存设置的数据
								x=Get_ElementADDR(ADDR_4500_3,1,1,LENmax_4500_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//写IP
								}	
								x=Get_ElementADDR(ADDR_4500_3,1,2,LENmax_4500_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//写端口
								}	

								x=Get_ElementADDR(ADDR_4500_3,2,1,LENmax_4500_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//写IP
								}	
								x=Get_ElementADDR(ADDR_4500_3,2,2,LENmax_4500_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//写端口
								}	
#if (USER/100)==15//安徽系
                MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
						    p8=(u8*)ADDR_128KDATABUFF;
                j=p8[1];
                p8+=2;
                for(i=0;i<j;i++)
                {
                  p8a=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(k=0;k<=3;k++)
									{
                    p8a[k]=p8_bcd(p+8*k,3);
									}
                  p=Get_Element(p8,8,0,0);//找到了主站通信参数 array structure 中Array的地址
									p+=2+2+2;
                  MW(ADDR_DATABUFF,(u32)p,4);//写主IP
                  
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									x=p8_bcd(p,5);
									k=(x&0xff);
									k<<=8;
									k|=(x>>8);
                  p=Get_Element(p8,8,0,0);//找到了主站通信参数 array structure 中Array的地址
									p+=2+2+6+1;
									MWR(k,(u32)p,2);//写主端口
                  
                  p8a=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(k=0;k<=3;k++)
									{
                    p8a[k]=p8_bcd(p+8*k,3);
									}
                  p=Get_Element(p8,8,0,0);//找到了主站通信参数 array structure 中Array的地址
									p+=2+2+6+3+2+2;
                  MW(ADDR_DATABUFF,(u32)p,4);//写备用IP
                  
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									x=p8_bcd(p,5);
									k=(x&0xff);
									k<<=8;
									k|=(x>>8);
                  p=Get_Element(p8,8,0,0);//找到了主站通信参数 array structure 中Array的地址
									p+=2+2+6+3+2+2+4+1;
									MWR(k,(u32)p,2);//写备用端口
                  p8+=Get_DL698DataLen_S(p8,0);
                }
                MW(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
#endif
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
// Function name:       Menu2_2_1_1_4
// Descriptions:        APN设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_4(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
//	u32 k;
	u32 MinLn=4;//软键盘最小行号
	u32 MaxLn=8;//软键盘最大行号
	u32 LastLnMaxCol=14;//软键盘的最后一行最大列号
	u32 MinCol=0;//软键盘最小列号
	u32 MaxCol=19;//软键盘最大列号
	u32 EditLn=2;
	u32 EditCol=2;
	u32 EditLength=16;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8d;
#endif
#if (Project/100)==2&&(USER/100)==17 //江苏集中器
	Ln=1;//菜单从第1行开始
#else 
	Ln=0;//菜单从第1行开始
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=(u8 *)" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//清全屏
			DisplayString(Ln,6,0,(u8 *)"APN设置");
			DisplayString(MinLn,MinCol,0,p8);
			p8=Get_Element((u8*)ADDR_4500_2,6,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
			MC(0,ADDR_DATABUFF,LENmax_4500_2);
			if(p8)
			{
				MenuCount->v3=p8[1];//APN长度
				p8+=2;
				if(MenuCount->v3>16)
				{//暂固定APN长度为16字节
					MenuCount->v3=16;
					
				}
				MC(0,ADDR_DATABUFF+MenuCount->v3,1);
				MR(ADDR_DATABUFF,(u32)p8,MenuCount->v3);
			}
			p=(u8 *)ADDR_DATABUFF;
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
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					p8=(u8 *)ADDR_DATABUFF;
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=15;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							p8[i]=0;
							break;
						}
					}
					Update_ElementDATA_unfixed(ADDR_4500_2,6,0,LENmax_4500_2,ADDR_DATABUFF,i);
					//设置成功提示ＯＫ
					DisplayClrRow(1,9);
					DisplayString(1,0,0,p8);
					DispalyAlign(1,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_5
// Descriptions:        用户名设置、密码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
//	u32 x;
	u32 MinLn=4;//软键盘最小行号
	u32 MaxLn=8;//软键盘最大行号
	u32 LastLnMaxCol=14;//软键盘的最后一行最大列号
	u32 MinCol=0;//软键盘最小列号
	u32 MaxCol=19;//软键盘最大列号
	u32 EditCol=0;
	u32 EditLength=20;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8d;
#endif
#if((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第一行开始
#else
	Ln=0;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0：初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//清全屏
			DisplayString(MinLn,MinCol,0,p8);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 5:
					DisplayString(Ln,5,0,"用户名设置");
					p=Get_Element((u8*)ADDR_4500_2,7,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					if(p)
					{
						i=p[1];//用户名长度
						p+=2;
						if(i>32)
						{//暂固定用户名长度32字节
							i=32;
						}
						MC(0,ADDR_DATABUFF+i,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
				break;
				case 6:
					DisplayString(Ln,6,0,"密码设置");
					p=Get_Element((u8*)ADDR_4500_2,8,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					if(p)
					{
						i=p[1];//用户名长度
						p+=2;
						if(i>32)
						{//暂固定用户名长度32字节
							i=32;
						}
						MC(0,ADDR_DATABUFF+i,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
					break;
			}			
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10Ms定时器
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
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+(EditCol)*2);
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
					switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
					{
						case 5:
							Update_ElementDATA_unfixed(ADDR_4500_2,7,0,LENmax_4500_2,ADDR_DATABUFF,i);
							break;
						case 6:
							Update_ElementDATA_unfixed(ADDR_4500_2,8,0,LENmax_4500_2,ADDR_DATABUFF,i);
							break;
					}
					//设置成功提示OK
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//行显示对其(1左对齐,2居中,3右对齐)
					DispalyAlign(2,0,2);//行显示对其(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
					p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用 0=否1=是
					p8d[0]=1;//菜单显示顶、底行用 0=否1=是
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用 0=否1=是
				p8d[0]=1;//菜单显示顶、底行用 0=否1=是
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_1
// Descriptions:        通信配置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11) || ((USER/100)==17))//上海专变、黑龙江、吉林,江苏
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
				//清全屏
				DisplayClr();//清全屏
				Draw_Clr();
				//DisplayMenuString(Ln+1,0,0,(u8 *)pMenu2_2_1_2_1);
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_1);//显示菜单
			#else
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_1);//显示菜单
			#endif
			//工作模式:混合模式（0），客户机模式（1），服务器模式（2）
			p8=Get_Element((u8*)ADDR_4510_2,1,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+1,10,0,(u8 *)WorkModeList[p8[1]&0x03]);
			}
		  //连接方式:TCP（0），UDP（1）
			p8=Get_Element((u8*)ADDR_4510_2,2,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+2,10,0,(u8 *)ConnectList[p8[1]&0x03]);
			}
		  //连接应用方式:主备模式（0），多连接模式（1）
			p8=Get_Element((u8*)ADDR_4510_2,3,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+3,10,0,(u8 *)ApplicationList[p8[1]&0x03]);
			}
		  //超时时间及重发次数：bit7~bit2：超时时间（秒）bit1~bit0：重发次数
			p8=Get_Element((u8*)ADDR_4510_2,7,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				//超时时间（秒）
				i=(p8[1]>>2);
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
				bcd_p8(p,i,3);
				DisplayString(Ln+4,13,0,(u8 *)UnitList[9]);//0 s
				//重发次数
				i=(p8[1]&0x03);
				p=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
				bcd_p8(p,i,1);
				DisplayString(Ln+5,11,0,(u8 *)UnitList[10]);//1 次
			}
			//心跳周期(秒)  long-unsigned
			p8=Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
        		i=p8[1];
				i<<=8;
				i|=p8[2];
				p=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
				bcd_p8(p,i,5);
			  DisplayString(Ln+6,15,0,(u8 *)UnitList[9]);//0 s				
			}
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=7)
					{
						DisplayString(Ln+7,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://连接方式:TCP（0），UDP（1）
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://连接应用方式:主备模式（0），多连接模式（1）
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4://超时时间
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5://重发次数
							MenuCount->v1=10;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6://心跳周期(秒)  long-unsigned
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
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
							case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://混
										i=1;
										break;
									case 0xF8://客
										i=2;
										break;
									case 0x7A://服
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://连接方式:TCP（0），UDP（1）
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 3://连接应用方式:主备模式（0），多连接模式（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://主
										i=1;
										break;
									case 0x82://多
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 4://超时时间
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<63)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5://重发次数
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x<3)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 6://心跳周期(秒)  long-unsigned
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
							case 1://工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://混
										i=2;
										break;
									case 0xF8://客
										i=0;
										break;
									case 0x7A://服
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://连接方式:TCP（0），UDP（1）
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 3://连接应用方式:主备模式（0），多连接模式（1）
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://主
										i=1;
										break;
									case 0x82://多
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 4://超时时间
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,63,3);
								}
								break;
							case 5://重发次数
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>0)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,3,1);
								}
								break;
							case 6://心跳周期(秒)  long-unsigned
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 4://超时时间
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5://重发次数
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 6://心跳周期(秒)  long-unsigned
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 7://确认设置
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//保存设置的数据
								//1.工作模式:混合模式（0），客户机模式（1），服务器模式（2）
								x=Get_ElementADDR(ADDR_4510_2,1,0,LENmax_4510_2);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
									switch(p8[0])
									{
										case 0x46://混
											i=0;
											break;
										case 0xF8://客
											i=0x01;
											break;
										case 0x7A://服
											i=0x02;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
									
								}
								//2.连接方式:TCP（0），UDP（1）
								x=Get_ElementADDR(ADDR_4510_2,2,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
									switch(p8[0])
									{
										case 0x54://TCP
											i=0;
											break;
										case 0x55://UDP
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//3.连接应用方式:主备模式（0），多连接模式（1）
								x=Get_ElementADDR(ADDR_4510_2,3,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)								
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
									switch(p8[0])
									{
										case 0x30://主
											i=0;
											break;
										case 0x82://多
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//4.超时时间及重发次数：bit7~bit2：超时时间（秒）bit1~bit0：重发次数
								x=Get_ElementADDR(ADDR_4510_2,7,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									//超时时间（秒）
									p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
									i=p8_bcd(p8,3);
									i<<=2;
									//重发次数
									p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
									k=p8_bcd(p8,1);
									i|=k;
									MWR(i,x+1,1);
								}
								//6.心跳周期(秒)  long-unsigned
								x=Get_ElementADDR(ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);
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
							case 4://超时时间
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>63)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5://重发次数
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>3)
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 6://6.心跳周期(秒)
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
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_2
// Descriptions:        侦听端口设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_2);//显示菜单
			p8=Get_Element((u8*)ADDR_4510_2,4,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{
				if(p8[1])
				{//数组个数不为0，显示第1个侦听端口
					i=p8[3];
					i<<=8;
					i|=p8[4];
					p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+9*2;
					bcd_p8(p,i,5);					
				}
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
						DisplayString(Ln+2,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{					
						case 1://侦听端口
							MenuCount->v1=9;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://确认设置
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
							case 1://侦听端口  long-unsigned
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
							case 1://侦听端口  long-unsigned
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+2,9,0,"OK");
								//保存设置的数据
							  //侦听端口
								x=Get_ElementADDR(ADDR_4510_2,4,1,LENmax_4510_2);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									MWR(k,x+1,2);
								}	
								else//原来是空的 
								{
									MR(ADDR_DATABUFF,ADDR_4510_2,LENmax_4510_2);
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									k<<=8;
									k|=0x12;//成员类型是0x12
									k<<=8;
									k|=1;//ARRAY的个数是1
									Update_ElementDATA_unfixed(ADDR_DATABUFF,5,0,LENmax_4500_2,(u32)&k,4);
									MW(ADDR_DATABUFF,ADDR_4510_2,LENmax_4510_2);
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
// Function name:       Menu2_2_1_2_3
// Descriptions:        主站通信参数设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_3);//显示菜单
			//IP
			p8=Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//端口
			p8=Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//端口
				p=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			//备IP
			p8=Get_Element((u8*)ADDR_4510_3,2,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//备端口
			p8=Get_Element((u8*)ADDR_4510_3,2,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
			if(p8)
			{//端口
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			MenuCount->v1=5;
			MenuCount->v2=21;
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
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{	
						case 1://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 2://端口
							MenuCount->v1=7;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 4://端口
							MenuCount->v1=7;
							MenuCount->v2=11;
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
							case 1://IP 按右键不加
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP 按左键不减
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2://端口
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,"OK");
								//保存设置的数据
								x=Get_ElementADDR(ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//写IP
								}	
								x=Get_ElementADDR(ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//写端口
								}	

								x=Get_ElementADDR(ADDR_4510_3,2,1,LENmax_4510_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//写IP
								}	
								x=Get_ElementADDR(ADDR_4510_3,2,2,LENmax_4510_3);//计算元素地址(实际逻辑地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//写端口
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
// Function name:       Menu2_2_1_2_4
// Descriptions:        终端IP设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_4(void)
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

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_4);//显示菜单
			//显示内存中数据
			//1.获取类型
			p8=Get_Element((u8*)ADDR_4510_4,1,0,LENmax_4510_4);//计算元素地址,使用ADDR_128K_DATABUFF
		  if(p8)
			{
				i=p8[1];//IP类型
				DisplayString(Ln+1,5,0,(u8 *)IpTypeList[(i&0x03)]);
			}	
			if(i==1)
			{//1=静态
				MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//读终端IP
				MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//读子网掩码
				MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//读网关
			}
			else
			{//0=DHCP,2=PPPoE
				MC(0,ADDR_DATABUFF,12);//读终端IP、子网掩码、网关
			}
			p8=(u8 *)ADDR_DATABUFF;
			//2.终端IP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//3.子网掩码
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[4+i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//4.网关
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[8+i],3);
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
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=9;
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
									case 0x44://DHCP
										i=1;
										MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//读终端IP
										MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//读子网掩码
										MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//读网关
										break;
									case 0x06://静态
										i=2;
										MC(0,ADDR_DATABUFF,12);//读终端IP、子网掩码、网关
										break;
									case 0x50://PPPoE
										i=0;
										MC(0,ADDR_DATABUFF,12);//读终端IP、子网掩码、网关
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//终端IP
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//子网掩码
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[4+i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//网关
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[8+i],3);
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
									case 0x44://DHCP
										i=2;
										MC(0,ADDR_DATABUFF,12);//读终端IP、子网掩码、网关
										break;
									case 0x06://静态
										i=0;
										MC(0,ADDR_DATABUFF,12);//读终端IP、子网掩码、网关
										break;
									case 0x50://PPPoE
										i=1;
										MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//读终端IP
										MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//读子网掩码
										MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//读网关
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//终端IP
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//子网掩码
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[4+i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//网关
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[8+i],3);
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
//								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
//								//判断内存数据有错 则初始化1=静态
//								if(p8[0]==0x24)
//								{//错误IP类型
//									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v1,0,(u8 *)IpTypeList[1]);
//								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//固定IP可以设置 自动不能
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,"OK");
								//保存设置的数据
							  //1.IP类型
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p[0]==0x44)
								{//DHCP
									p8[0]=0x00;
								}
								if(p[0]==0x06)
								{//静态
									p8[0]=0x01;
								}
								if(p[0]==0x50)
								{//PPPoE
									p8[0]=0x02;
								}
								MW(ADDR_DATABUFF,ADDR_4510_4+3,1);//写IP类型
								//2.IP
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[1+i]=p8_bcd(p+8*i,3);
								}
								//3.子网掩码
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[5+i]=p8_bcd(p+8*i,3);
								}
								//4.网关
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[9+i]=p8_bcd(p+8*i,3);
								}
								if(p8[0]==1)
								{//静态
									MW(ADDR_DATABUFF+1,ADDR_4510_4+6,4);//写终端IP
									MW(ADDR_DATABUFF+1+4,ADDR_4510_4+12,4);//写子网掩码
									MW(ADDR_DATABUFF+1+4+4,ADDR_4510_4+18,4);//写网关									
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
// Function name:       Menu2_2_1_2_5
// Descriptions:         PPPoE用户名设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	//u32 x;
	u32 MinLn=4;//软键盘最小行号
	u32 MaxLn=8;//软键盘最大行号
	u32 LastLnMaxCol=14;//软键盘的最后一行最大列号
	u32 MinCol=0;//软键盘最小列号
	u32 MaxCol=19;//软键盘最大列号
	u32 EditCol=0;
	u32 EditLength=20;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	u8 * p8d;
#endif
#if((Project/100)==2&&(USER/100)==17)//江苏集中器
		Ln=1;//菜单从第一行开始
#else
		Ln=0;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0：初始化
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//清全屏
			DisplayString(MinLn,MinCol,0,p8);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 5:
					DisplayString(Ln,3,0,"PPPoE用户名设置");
					p=Get_Element((u8*)ADDR_4510_4,5,0,LENmax_4510_4);//计算元素地址,使用ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
					if(p)
					{
						i=p[1];//用户名长度
						p+=2;
						if(i>32)
						{//暂固定用户名长度32字节
							i=32;
						}
						MC(0,ADDR_DATABUFF+32,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
				break;
				case 6:
					DisplayString(Ln,4,0,"PPPoE密码设置");
					p=Get_Element((u8*)ADDR_4510_4,6,0,LENmax_4510_4);//计算元素地址,使用ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
					if(p)
					{
						i=p[1];//用户名长度
						p+=2;
						if(i>32)
						{//暂固定用户名长度32字节
							i=32;
						}
						MC(0,ADDR_DATABUFF+32,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
					break;
			}			
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10Ms定时器
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
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
					p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+(EditCol)*2);
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
						case 5:
							Update_ElementDATA_unfixed(ADDR_4510_4,5,0,LENmax_4510_4,ADDR_DATABUFF,i);
							break;
						case 6:
							Update_ElementDATA_unfixed(ADDR_4510_4,6,0,LENmax_4510_4,ADDR_DATABUFF,i);
							break;
					}
					//设置成功提示OK
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//行显示对其(1左对齐,2居中,3右对齐)
					DispalyAlign(2,0,2);//行显示对其(1左对齐,2居中,3右对齐)
					DisplayString(4,7,0,"设置OK");
					DisplayString(5,4,0,"按任意键退出");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
					p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用 0=否1=是
					p8d[0]=1;//菜单显示顶、底行用 0=否1=是
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//返回上级菜单
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//键使用后清0
			break;
		case 3://设置成功按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用 0=否1=是
				p8d[0]=1;//菜单显示顶、底行用 0=否1=是
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_6_1
// Descriptions:        MAC地址设置(十进制)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu2_2_1_2_6_1(void)
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

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第一行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6_1);//显示菜单
			//显示内存中数据
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//读MAC
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
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
						DisplayString(Ln+3,6,0,"确认设置");
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
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
								MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//读MAC
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
									Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
									MW(ADDR_DATABUFF,ADDR_4510_5+2,6);//写MAC
									DisplayString(Row,9,0,"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxx为0表示可设置；1表示已有唯一MAC，故不可设置
										DisplayString(Row,2,0,"MAC唯一无需设置");
									}
									else
									{//(p8[0]&0x01)==1 组播地址
										DisplayString(Row,1,0,"首字节为奇数不允许");
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
// Function name:       Menu2_2_1_2_6_2
// Descriptions:        MAC地址设置（十六进制）
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void Menu2_2_1_2_6_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第一行开始
#else
	Ln=0;//菜单从第0行开始
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6_2);//显示菜单
			//显示内存中数据
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
			MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//读MAC
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
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
						DisplayString(Ln+2,6,0,"确认设置");
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
								MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//读MAC
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
									Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
									MW(ADDR_DATABUFF,ADDR_4510_5+2,6);//写MAC
									DisplayString(Row,9,0,"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxx为0表示可设置；1表示已有唯一MAC，故不可设置
										DisplayString(Row,6,0,"MAC 唯一");
									}
									else
									{//(p8[0]&0x01)==1 组播地址
										DisplayString(Row,4,0,"首字节为奇数");
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
// Function name:       Menu2_2_1_2_6
// Descriptions:         MAC地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_6(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6);//显示菜单
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
						MenuCreate((u32)Menu2_2_1_2_6_1);//创建下级菜单   MAC地址设置(十进制)         
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2_6_2);//创建下级菜单   MAC地址设置(十六进制)
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
// Function name:       Menu2_2_1_1
// Descriptions:        无线公网设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1);//显示菜单
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
						MenuCreate((u32)Menu2_2_1_1_1);//通信模式设置           
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_1_2);//侦听端口设置           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_1_3);//主站通信参数设置           
						break;
					case 4:
						MenuCreate((u32)Menu2_2_1_1_4);//APN设置           
						break;
					case 5:
					case 6:
#if(Project/100)==2&&(USER/100)==17//江苏集中器
						MenuCount->v1=2;
#else
						MenuCount->v1=1;
#endif
						MenuCreate((u32)Menu2_2_1_1_5);//5、6用户名设置 、密码设置
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
// Function name:       Menu2_2_6
// Descriptions:        以太网设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2);//显示菜单
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
						MenuCreate((u32)Menu2_2_1_2_1);//1.通信配置           
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2_2);//2.侦听端口设置           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_2_3);//3.主站通信参数设置           
						break;
					case 4:
						MenuCreate((u32)Menu2_2_1_2_4);//4.终端IP设置           
						break;
					case 5:
					case 6:
#if((Project/100)==2&&(USER/100)==17)//江苏集中器
						MenuCount->v1=2;
#else
						MenuCount->v1=1;
#endif
						MenuCreate((u32)Menu2_2_1_2_5);//5、6 PPPoE用户名、密码设置           
						break;
					case 7:
						MenuCreate((u32)Menu2_2_1_2_6);//7. MAC地址设置           
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

const u8 pMenu2_2_1_3_JS[]=//公网远程通信多接入点备用通道
{
	"公网远程通信\n\r"
	"  1.查询备用通道\n\r"
	"  2.添加一组通道\n\r"
};

const u8 pMenu2_2_1_3_1_JS[]=//公网远程通信多接入点备用通道
{
	"通道组号:\n\r"
	"  1.运营商网络类型\n\r"
	"  2.APN设置\n\r"
	"  3.用户名设置\n\r"
	"  4.密码设置\n\r"
	"  5.服务器地址和端口\n\r"
	"  6.主站通信参数\n\r"	
	"      删除通道\n\r"
};
	
const u8 pMenu2_2_1_3_1_1_JS[]=//运营商和网络类型
{
	"运营商和网络类型\n\r"
	"运营商:\n\r"
	"网络类型:\n\r"	
	"      确认设置"
};

const u8 OperatorList[][10]=//运营商
{
	"中国移动",
	"中国电信",
	"中国联通",
	"  未知  ",
};
const u8 OperatorList_Size=sizeof(OperatorList)/10;

const u8 NetworkList[][8]=//网络类型
{
	" 2G ",
	" 3G ",
	" 4G ",
	"未知",
};
const u8 NetworkList_Size=sizeof(NetworkList)/8;

const u8 pMenu2_2_1_3_1_5_JS[]=//代理服务器地址和端口
{
	"地址和端口设置\n\r"
	"地址:\n\r"
	"端口:\n\r"
	"      确认设置\n\r"
};
	
const u8 pMenu2_2_1_3_1_6_JS[]=//主站通信参数设置
{
	"主站IP及端口设置\n\r"
	"主IP:\n\r"
	"主端口:\n\r"
	"备IP:\n\r"
	"备端口:\n\r"
	"      确认设置\n\r"
};
	
const u8 pMenu2_2_1_3_2_JS[]=//公网远程通信多接入点备用通道
{
	"添加备用通道\n\r"
	"  1.运营商网络类型\n\r"
	"  2.APN设置\n\r"
	"  3.用户名设置\n\r"
	"  4.密码设置\n\r"
	"  5.服务器地址和端口\n\r"
	"  6.主站通信参数\n\r"	
};
	
const u8 default_4520[]=//公网远程通信多接入点备用通道
{
	//主站通信参数表∷=array 主站通信参数
	//DataType_array,
	//1,
	//主站通信参数：：=structure
	DataType_structure,
	8,
	//运营商 enum 
	//{ 
	//CMCC（移动） = 0， 
	//CTCC（电信） = 1， 
	//CUCC（联通） = 2， 
	//未知 = 255 
	//}，
    DataType_enum,
    0,
// 网络类型 enum 
// { 2G = 0，3G = 1， 4G = 2， 未知 = 255 }，
    DataType_enum,
    0,
//APN            visible-string，	
	DataType_visible_string,
	//0,
	0,
//用户名         visible-string，
	DataType_visible_string,
	0,
//密码           visible-string，
	DataType_visible_string,
    0,
//代理服务器地址 octet-string，
	DataType_octet_string,
	0,
//代理端口            long-unsigned，
	DataType_long_unsigned,
	0,0,
//主站通信参数 array structure 
//{ 
//IP地址 octet-string， 
//端口 long-unsigned 
    DataType_array,
	2,
    DataType_structure,
	2,
	DataType_octet_string,
	4,
	0,0,0,0,
	DataType_long_unsigned,
	0,0,
	DataType_structure,
	2,
	DataType_octet_string,
	4,
	0,0,0,0,
	DataType_long_unsigned,
	0,0,
};

u32 Display_4520_2(u32 Element,u32 SubElement,u32 SSubElement,u32 Row,u32 Col)//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element(),rt目前就APN用户名和密码用的到(跟DisplayString返回值一样)
{
	u8 *p8; 
	u8 *p;
	u32 i;
	u32 rt=0;
	//运营商
	p8=Get_Element((u8 *)ADDR_4520_2,MenuCount->v3,Element,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
	switch (Element)
	{
		case 1://运营商		
			if(p8)
			{		
				DisplayString(Row,Col,0,(u8 *)OperatorList[p8[1]&0x03]);
			}
			break;
		case 2://网络类型	
			if(p8)
			{		
				DisplayString(Row,Col,0,(u8 *)NetworkList[p8[1]&0x03]);
			}
			break;
		case 3://APN设置
		case 4://用户名
		case 5://密码
			if(p8)
			{
				i=p8[1];//APN长度
				if(i!=0)
				{
					MR((u32)ADDR_DATABUFF,(u32)p8+2,i);
					p8=(u8 *)ADDR_DATABUFF;
					p8[i]=0;//最后一个字符为0
					rt=DisplayString(Row,Col,0,p8);
				}else
				{
					rt=Row+(Col<<8);
				}
			}
			break;
		case 6://代理服务器地址
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+Row*84+4+5*2;
				if(p8[1]==4)//有IP地址
				{					
					for(i=0;i<=3;i++)
					{
						bcd_p8(p+i*8,p8[i+2],3);
					}
					for(i=0;i<=2;i++)
					{
						p[3*2+i*8]='.';
						p[3*2+i*8+1]=0x20;
					}
				}else
				{
					for(i=0;i<=3;i++)
					{
						bcd_p8(p+i*8,0,3);
					}
					for(i=0;i<=2;i++)
					{
						p[3*2+i*8]='.';
						p[3*2+i*8+1]=0x20;
					}
				}
			}
			break;
		case 7://代理服务器端口
			if(p8)
			{//端口
				p=(u8*)ADDR_STRINGBUFF+(Row)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			break;
		case 8:
			p8=Get_Element((u8 *)p8,SubElement,SSubElement,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			switch (SSubElement)
			{
				case 1://主站地址
					if(p8)
					{//IP
						p=(u8*)ADDR_STRINGBUFF+(Row)*84+4+5*2;
						if(p8[1]==4)//有IP地址
						{
							for(i=0;i<=3;i++)
							{
								bcd_p8(p+i*8,p8[i+2],3);
							}
							for(i=0;i<=2;i++)
							{
								p[3*2+i*8]='.';
								p[3*2+i*8+1]=0x20;
							}
						}else
						{
							for(i=0;i<=3;i++)
							{
								bcd_p8(p+i*8,0,3);
							}
							for(i=0;i<=2;i++)
							{
								p[3*2+i*8]='.';
								p[3*2+i*8+1]=0x20;
							}
						}
					}
					break;
				case 2://主站端口号
					if(p8)
					{//端口
						p=(u8*)ADDR_STRINGBUFF+(Row)*84+4+7*2;
						i=p8[1];
						i<<=8;
						i|=p8[2];
						bcd_p8(p,i,5);
					}
					break;
			}
			break;
		default:
			break;			
	}
	return rt;
}

//字符显示的位置信息
#define GETSTRING_MINROW(Val)   		((Val>>28)&0xF) 		//字符显示最小行BIT31-28
#define GETSTRING_MAXROW(Val)   		((Val>>24)&0xF) 		//字符显示最大行BIT27-24
#define GETSTRING_STRCOL(Val)			((Val>>16)&0xFF) 		//字符开始最大列BIT23-16
#define GETSTRING_MIDCOL(Val)			((Val>>8)&0xFF)			//字符中间最大列BIT15-8
#define GETSTRING_ENDCOL(Val)   		(Val&0xFF) 				//字符结束最大列BIT0-7

//功能按键的位置信息
#define GETKEYBOARD_FUNC1(FuncVal)   	((FuncVal>>8)&0xFF) 	//第一个功能按键位置
#define GETKEYBOARD_FUNC2(FuncVal)   	((FuncVal)&0xFF) 		//第二个功能按键位置

//软键盘字符按键的位置信息
#define GETKEYBOARD_MINROW(Val)   		((Val>>28)&0xF) 		//软键盘最小行号
#define GETKEYBOARD_MAXROW(Val)   		((Val>>24)&0xF) 		//软键盘最大行号
#define GETKEYBOARD_LASTMAXCOL(Val)   	((Val>>16)&0xFF) 		//软键盘的最后一行最大列号
#define GETKEYBOARD_MINCOL(Val)   		((Val>>8)&0xFF) 		//软键盘最小列号
#define GETKEYBOARD_MAXCOL(Val)   		(Val&0xFF) 				//软键盘最大列号

void NegativeKeyBoard(u32 Row,u32 Col,u32 Val)//反显软键盘 Row反显的行,Col反显的列,Val软键盘的参数
{
	u16 * p16;
	u32 i;
	u32 j;
	u32 MINROW=GETKEYBOARD_MINROW(Val);
	u32 MAXROW=GETKEYBOARD_MAXROW(Val);
	for(j=MINROW;j<=MAXROW;j++)//软键盘其他行都不反显
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(j*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+Col*2);
	if(Col>GETKEYBOARD_LASTMAXCOL(Val)&&Row==MAXROW)
	{
		p16[0]|=0x8000;//反显指定行、列
		p16[1]|=0x8000;//反显指定行、列
	}else
	{
		*p16|=0x8000;//反显指定行、列
	}
}

void NegativeCursor(u32 Row,u32 Col)//反显光标 Row反显的行,Col反显的列
{
	u16 * p16;	
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+Col*2);
	p16[0]|=0x8000;
}

void CreateKeyBoard(u32 Val)//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
{
	u8 *p8;
	p8=(u8 *)"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<> 删 存";
	DisplayClr();//清全屏
	DisplayString(GETKEYBOARD_MINROW(Val),GETKEYBOARD_MINCOL(Val),0,p8);
	//反显键盘
	NegativeKeyBoard(GETKEYBOARD_MINROW(Val),GETKEYBOARD_MINCOL(Val),Val);
}

void HandleKeyBoard(u32 StrVal ,u32 Val ,u32 FuncVal)//在屏幕下面创建软键盘 StartRow字符显示开始行,StartCol字符显示开始列VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 EditCol=Stack->MenuStack[MenuCount->Count].EditCol;//当前列
	u32 EditRow=Stack->MenuStack[MenuCount->Count].EditRow;//当前行
	u32 MowRow=MenuCount->v1;//移动光标
	u32 SRow=(MowRow>>8)&0xFF;//光标行
	u32 SCol=(MowRow)&0xFF;	//光标列
	
	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//右移键按下
		//最后一行因为有功能按钮要做特殊处理
		if((EditRow==GETKEYBOARD_MAXROW(Val)))//软键盘最大行号
		{
			if(EditCol==GETKEYBOARD_LASTMAXCOL(Val))
			{
				EditCol=GETKEYBOARD_FUNC1(FuncVal);//第一个功能按键位置
			}else if(EditCol==GETKEYBOARD_FUNC1(FuncVal))
			{
				EditCol=GETKEYBOARD_FUNC2(FuncVal);//第一个功能按键位置
			}else if(EditCol==GETKEYBOARD_FUNC2(FuncVal))
			{
				EditCol=0;
			}else
			{
				EditCol++;
			}
		}else
		{
			if(EditCol==GETKEYBOARD_MAXCOL(Val))//软键盘最大列号
			{
				EditCol=0;
			}else
			{
				EditCol++;
			}
		}
		Stack->MenuStack[MenuCount->Count].EditCol=EditCol;//第一个功能按键位置
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)
	{//左移键按下
		//最后一行因为有功能按钮要做特殊处理
		if((EditRow==GETKEYBOARD_MAXROW(Val)))//软键盘最大行号
		{
			if(EditCol==0)
			{
				EditCol=GETKEYBOARD_FUNC2(FuncVal);////第二个功能按键位置
			}else if(EditCol==GETKEYBOARD_FUNC2(FuncVal))
			{
				EditCol=GETKEYBOARD_FUNC1(FuncVal);////第二个功能按键位置
			}else if(EditCol==GETKEYBOARD_FUNC1(FuncVal))
			{
				EditCol=GETKEYBOARD_LASTMAXCOL(Val);//软键盘的最后一行最大列号
			}else
			{
				EditCol--;
			}
		}else
		{
			if(EditCol==0)//软键盘最大列号
			{
				EditCol=GETKEYBOARD_MAXCOL(Val);
			}else
			{
				EditCol--;
			}
		}
		Stack->MenuStack[MenuCount->Count].EditCol=EditCol;
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//向下键按下
		if(EditRow==GETKEYBOARD_MAXROW(Val))//最后一行
		{
			if(EditCol>GETKEYBOARD_LASTMAXCOL(Val))//当前选中是的最后一行的功能按键
			{//跳转到第一行的最后一个字符
				EditCol=GETKEYBOARD_MAXCOL(Val);	
				Stack->MenuStack[MenuCount->Count].EditCol=EditCol;
			}
			EditRow=GETKEYBOARD_MINROW(Val);
		}else
		{
			if(EditRow==GETKEYBOARD_MAXROW(Val)-1)//最后第二行
			{
				if(EditCol>GETKEYBOARD_LASTMAXCOL(Val))//当前选中是的最后一行的功能按键
				{//跳转到第一行的最后一个字符
					EditCol=GETKEYBOARD_FUNC1(FuncVal);	
					Stack->MenuStack[MenuCount->Count].EditCol=EditCol;	
				}
			}
			EditRow++;
		}  
		Stack->MenuStack[MenuCount->Count].EditRow=EditRow;
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//向上键按下
		if(EditRow==GETKEYBOARD_MINROW(Val))//第一行
		{
			if(EditCol>GETKEYBOARD_LASTMAXCOL(Val))//当前选中是的最后一行的功能按键
			{//跳转到最后一行的功能按钮
				EditCol=GETKEYBOARD_FUNC1(FuncVal);
				Stack->MenuStack[MenuCount->Count].EditCol=EditCol;
			}
			EditRow=GETKEYBOARD_MAXROW(Val);
		}else
		{
			if(EditRow==GETKEYBOARD_MAXROW(Val))//最后一行
			{
				//如果当前选中的是功能按钮,跳转到最后第二行的末尾
				if(EditCol>GETKEYBOARD_LASTMAXCOL(Val))
				{
					EditCol=GETKEYBOARD_MAXCOL(Val);
					Stack->MenuStack[MenuCount->Count].EditCol=EditCol;
				}
			}
			EditRow--;
		}
		Stack->MenuStack[MenuCount->Count].EditRow=EditRow;   
	}
	//反向现实当前的位置
	NegativeKeyBoard(EditRow,EditCol,Val);
	if((Comm_Ram->ButtonVal&0x10)!=0)
	{//确认
		//1.键盘功能键
		if(EditRow==GETKEYBOARD_MAXROW(Val)&&EditCol>GETKEYBOARD_LASTMAXCOL(Val))
		{
			if(EditCol==GETKEYBOARD_FUNC1(FuncVal))//按键功能1
			{//删除字符,先移动光标到字符串处
				//原来光标处反显的位置不反显
				p16=(u16 *)(ADDR_STRINGBUFF+(SRow)*84+4+(SCol)*2);
				p16[0]&=0x7FFF;
				//1.移动光标
				if(SCol==0 && SRow>GETSTRING_MINROW(StrVal))
				{
					SRow--;
					SCol=GETSTRING_MIDCOL(StrVal);
				}else
				{
					if(SRow<=GETSTRING_MINROW(StrVal) && SCol<=GETSTRING_STRCOL(StrVal))
					{
						SRow=GETSTRING_MINROW(StrVal);
						SCol=GETSTRING_STRCOL(StrVal);
					}else
					{
						SCol--;
					}
				}
				//2.删除字符
				p16=(u16 *)(ADDR_STRINGBUFF+(SRow)*84+4+(SCol)*2);
				x=p16[0]&0xFF;
				if(x!=' ')//空格
				{
					p16[0]=' '+0x2000;
				}				
			}
			if(EditCol==GETKEYBOARD_FUNC2(FuncVal))//按键功能2
			{
				//直接跳转到保存字符串数据
				Stack->MenuStack[MenuCount->Count].Task=2;
			}
		}else//2.键盘字母
		{
			if(SRow<GETSTRING_MAXROW(StrVal) || SCol<=GETSTRING_ENDCOL(StrVal))
			{
				p8=(u8 *)(ADDR_STRINGBUFF+(EditRow)*84+4+(EditCol)*2);
				//字符填到光标处
				p16=(u16 *)(ADDR_STRINGBUFF+(SRow)*84+4+(SCol)*2);
				*p16=(u16)(*p8+0x2000);			
				SCol++;
				if(SCol>GETKEYBOARD_MAXCOL(Val))
				{
					SCol=0;
					SRow++;
				}
			}
			
		}
		MenuCount->v1=(SRow<<8)|SCol;
	}
	//反显光标的位置
	NegativeCursor(SRow,SCol);//反显光标 Row反显的行,Col反显的列
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}
//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_2_1_3_1_1_JS
// Descriptions:		运行商和网络类型
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_1_JS(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 Ln;
	Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_3_1_1_JS);//显示菜单
			//运营商	
			Display_4520_2(1,0,0,Ln+1,8);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			//网络类型	
			Display_4520_2(2,0,0,Ln+2,12);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,16,8);	
			break;
		case 1://任务1:选择	
			BoxSelect();
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1://运营商
					ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,16,8);
					break;
				case 2://网络类型
					ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+1,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,16,10);
					break;
				case 3://确认设置
					for(x=0;x<10;x++)
					{
						if(x!=Stack->MenuStack[MenuCount->Count].EditRow)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(x+Ln)*84+4+16*2);
							if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
							{
								for(i=2;i<=3;i++)
								{
									p16[i]=(p16[i]&0x8000)+0x2020;
								}
							}
						}
					}
					MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						Stack->MenuStack[MenuCount->Count].Task=2;
						Comm_Ram->ButtonVal&=~0x10;//按键消除
					}
					break;
					default:
						break;
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右键
				Comm_Ram->ButtonVal&=~0x08;//键使用后清0				
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://运营商
						p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+(8)*2);
						for(i=0;i<OperatorList_Size;i++)
						{
							if(LCD_CmpString(p16,(u8 *)OperatorList[i],4)==0)//中国移动,中国电信,中国联通
							{
								break;
							}
						}
						if(i<OperatorList_Size)
						{
							i++;
							if(i==OperatorList_Size)//搜到了最后一个
							{
								DisplayString(Ln+1,8,0,(u8 *)OperatorList[0]);
							}else
							{
								DisplayString(Ln+1,8,0,(u8 *)OperatorList[i]);
							}
						}
						break;
					case 2://网络类型
						p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+(12)*2);
						for(i=0;i<NetworkList_Size;i++)
						{
							if(LCD_CmpString(p16,(u8 *)NetworkList[i],2)==0)
							{
								break;
							}
						}
						if(i<NetworkList_Size)
						{
							i++;
							if(i==NetworkList_Size)//搜到了最后一个
							{
								DisplayString(Ln+2,12,0,(u8 *)NetworkList[0]);
							}else
							{
								DisplayString(Ln+2,12,0,(u8 *)NetworkList[i]);
							}
						}
						break;
				}
				
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左键
				Comm_Ram->ButtonVal&=~0x04;//键使用后清0
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://运营商
						p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+(8)*2);
						for(i=0;i<OperatorList_Size;i++)
						{
							if(LCD_CmpString(p16,(u8 *)OperatorList[i],4)==0)//中国移动,中国电信,中国联通
							{
								break;
							}
						}
						if(i<OperatorList_Size)
						{
							if(i==0)//如果是第一个
							{
								DisplayString(Ln+1,8,0,(u8 *)OperatorList[OperatorList_Size-1]);
							}else
							{
								i--;
								DisplayString(Ln+1,8,0,(u8 *)OperatorList[i]);
							}
						}				
						break;
					case 2://网络类型
						p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+(12)*2);
						for(i=0;i<NetworkList_Size;i++)
						{
							if(LCD_CmpString(p16,(u8 *)NetworkList[i],2)==0)
							{
								break;
							}
						}
						if(i<NetworkList_Size)
						{
							if(i==0)//第一个
							{
								DisplayString(Ln+2,12,0,(u8 *)NetworkList[NetworkList_Size-1]);
							}else
							{
								i--;
								DisplayString(Ln+2,12,0,(u8 *)NetworkList[i]);
							}
						}
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
			case 2://保存数据操作		
				DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
				DisplayString(Ln+3,9,0,"OK");
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
				//1.从Flash中读出数据							
				MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);//从Flash中读出来数据
				//2.修改Buf中的数据
				p8=(u8 *)ADDR_DATABUFF;
				x=p8[1];
				if(x>=MenuCount->v3)//修改的通道号包含在里面才会修改这部分的数据
				{
					//运营商
					p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+(8)*2);
					for(i=0;i<OperatorList_Size;i++)
					{
						if(LCD_CmpString(p16,(u8 *)OperatorList[i],4)==0)//中国移动,中国电信,中国联通
						{
							break;
						}
					}
					if(i<OperatorList_Size)
					{														
						p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p8[0]==0x02)//结构体
						{
							p8=Get_Element((u8*)p8,1,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
							if(p8[0]==0x16)
							{
								p8[1]=i;
							}
						}												
					}
					//网络类型
					p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+(12)*2);
					for(i=0;i<NetworkList_Size;i++)
					{
						if(LCD_CmpString(p16,(u8 *)NetworkList[i],2)==0)//中国移动,中国电信,中国联通
						{
							break;
						}
					}
					if(i<NetworkList_Size)
					{
						p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p8[0]==0x02)//结构体
						{
							p8=Get_Element((u8*)p8,2,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
							if(p8[0]==0x16)
							{
								p8[1]=i;
							}
						}
					}				
					//3.保存到Flash中
					MW((u32 )ADDR_DATABUFF,(u32 )ADDR_4520_2,LENmax_4520_2);
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
// Function name:		Menu2_2_1_3_1_2_JS
// Descriptions:		APN设置
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_2_JS(void)
{
	u8 *p;
	u8 *p8;
	u32 i;
	u32 k;
	u32 x;
	u32 Len;
	u32 Ln;//界面从第几行开始显示

	Ln=1;
//==============================软键盘参数=========================================
	u32 KeyBoardSize=(4<<28)|(8<<24)|(13<<16)|(0<<8)|19;//软键盘最小行号,//软键盘最大行号,//软键盘的最后一行最大列号,//软键盘最小列号,//软键盘最大列号
	u32 KeyBoardFunc=(15<<8)|18;//第一个功能按键的位置//第二个功能按键的位置
	u32 StringSize=((Ln+1)<<28)|((Ln+2)<<24)|(2<<16)|(19<<8)|18;//字符显示最小行BIT31-28,字符显示最大行BIT27-24,字符开始列BIT23-16,字符中间列BIT15-8,字符结束列BIT7-0
//================================================================================= 
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			CreateKeyBoard(KeyBoardSize);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			DisplayString(Ln,6,0,(u8 *)"APN设置");
			i=Display_4520_2(3,0,0,GETSTRING_MINROW(StringSize),GETSTRING_STRCOL(StringSize));//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()	
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			//确定字符数据的光标显示位置
			x=i&0xFF;//这是字符最大的行号
			if(x>GETSTRING_MAXROW(StringSize))//屏幕已经放不下所有的字符(另外做处理,目前没有处理)
			{
				Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
				break;
			}
			if(x==GETSTRING_MAXROW(StringSize))//如果当前已经是最大行
			{
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_ENDCOL(StringSize)+1)//光标可以放在最后一个位置
				{
					Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
					break;
				}
			}else
			{//中间行或者是第一行的话
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_MIDCOL(StringSize))
				{
					x++;//光标指向下一行
					k=0;//并且列号为0
				}
			}
			NegativeCursor(x,k);
			MenuCount->v1=(x<<8)|k;//BIT0-7光标列的位置,BIT8-15光标行的位置
			Stack->MenuStack[MenuCount->Count].EditRow=GETKEYBOARD_MINROW(KeyBoardSize);//软键盘最小行号
			Stack->MenuStack[MenuCount->Count].EditCol=GETKEYBOARD_MINCOL(KeyBoardSize);//软键盘最小列号
			break;
		case 1://任务1：键盘选择
			HandleKeyBoard(StringSize,KeyBoardSize,KeyBoardFunc);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			break;
		case 2://保存APN
			//保存数据
			//1.读出原来的BUF
			MR((u32 )ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2); 				
			//2.读出现在的BUF长度 k是APN的长度p8保存APN的内容
			p8=(u8 *)ADDR_DATABUFF+1024;
			k=0;
			for(x=GETSTRING_MINROW(StringSize);x<=GETSTRING_MAXROW(StringSize);x++)
			{
				if(x==GETSTRING_MINROW(StringSize))
				{
					i=GETSTRING_STRCOL(StringSize);
					Len=GETSTRING_MIDCOL(StringSize);	
				}else if(x==GETSTRING_MAXROW(StringSize))
				{
					i=0;
					Len=GETSTRING_ENDCOL(StringSize);
				}else
				{
					i=0;
					Len=GETSTRING_MIDCOL(StringSize);
				}
				p=(u8 *)(ADDR_STRINGBUFF+(x)*84+4);
				for(;i<=Len;i++)
				{
					if(p[i*2]!=0x20 && p[i*2]!=0x00)
					{
						p8[k++]=p[i*2];
					}
					else
					{
						p8[k++]=0;
						break;
					}
				}
			}
			//3.整合新的bUF到原来的空间里面去
			i=0;
			p=(u8 *)ADDR_DATABUFF;
			p=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{	
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			p=Get_Element((u8*)p,3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			i=p[1];//apn长度
			Len= Get_DL698DataLen_S((u8 *)ADDR_DATABUFF,0);//数据长度一共有Len那么长
			if(i==k)
			{
				MW((u32)p8,(u32)p+2,i);
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,Len);
			}else 
			{
				//如果保存的数据比较大 要做一个判断
				if(k>i)
				{
					if((Len+(k-i))>LENmax_4520_2)
					{
						Stack->MenuStack[MenuCount->Count].Task=4;
						break;
					}
				}
				//1.保存头
				//01 02 02 08 16 00 16 00 0A
				p8=(u8 *)ADDR_DATABUFF;
				if(p>p8)
				{
					x=p-p8;
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,x+1);
				//2.保存APN
				p=(u8 *)ADDR_DATABUFF+2048+x+1;
				p[0]=k;
				MW((u32)ADDR_DATABUFF+1024,(u32)ADDR_DATABUFF+2048+x+2,k);
				//3.保存尾巴
				if(Len>x+i+2)
				{
					MW((u32)ADDR_DATABUFF+x+i+2,(u32)ADDR_DATABUFF+2048+x+2+k,Len-x-i-2);
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
			}
			//4.保存
			MW((u32 )ADDR_DATABUFF+2048,(u32)ADDR_4520_2,LENmax_4520_2);
			//设置成功提示ＯＫ
			DisplayClrRow(1,9);
			DisplayString(1,2,0,(u8 *)ADDR_DATABUFF+1024);	
			DisplayString(4,7,0,"设置OK");
			DisplayString(5,4,0,"按任意键退出");
			Stack->MenuStack[MenuCount->Count].Task=3;
			break;
		case 3://按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 4:
			DisplayClrRow(1,9); 
			DisplayString(3,7,0,"设置失败");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 0xFF://错误信息
			DisplayClrRow(1,9); 
			DisplayString(3,6,0,"字符长度过长");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
			default:
			break;
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_2_1_3_1_3_JS
// Descriptions:		用户名和密码设置
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_3_JS(void)
{
	u8 *p;
	u8 *p8;
	u32 i;
	u32 k;
	u32 x;
	u32 Len;
	u32 Ln;//界面从第几行开始显示

	Ln=1;
//==============================软键盘参数=========================================
	u32 KeyBoardSize=(4<<28)|(8<<24)|(13<<16)|(0<<8)|19;//软键盘最小行号,//软键盘最大行号,//软键盘的最后一行最大列号,//软键盘最小列号,//软键盘最大列号
	u32 KeyBoardFunc=(15<<8)|18;//第一个功能按键的位置//第二个功能按键的位置
	u32 StringSize=((Ln+1)<<28)|((Ln+2)<<24)|(2<<16)|(19<<8)|18;//字符显示最小行BIT31-28,字符显示最大行BIT27-24,字符开始列BIT23-16,字符中间列BIT15-8,字符结束列BIT7-0
//================================================================================= 
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			CreateKeyBoard(KeyBoardSize);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			DisplayString(Ln,6,0,(u8 *)"用户名设置");
			i=Display_4520_2(4,0,0,GETSTRING_MINROW(StringSize),GETSTRING_STRCOL(StringSize));//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()	
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			//确定字符数据的光标显示位置
			x=i&0xFF;//这是字符最大的行号
			if(x>GETSTRING_MAXROW(StringSize))//屏幕已经放不下所有的字符(另外做处理,目前没有处理)
			{
				Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
				break;
			}
			if(x==GETSTRING_MAXROW(StringSize))//如果当前已经是最大行
			{
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_ENDCOL(StringSize)+1)//光标可以放在最后一个位置
				{
					Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
					break;
				}
			}else
			{//中间行或者是第一行的话
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_MIDCOL(StringSize))
				{
					x++;//光标指向下一行
					k=0;//并且列号为0
				}
			}
			NegativeCursor(x,k);
			MenuCount->v1=(x<<8)|k;//BIT0-7光标列的位置,BIT8-15光标行的位置
			Stack->MenuStack[MenuCount->Count].EditRow=GETKEYBOARD_MINROW(KeyBoardSize);//软键盘最小行号
			Stack->MenuStack[MenuCount->Count].EditCol=GETKEYBOARD_MINCOL(KeyBoardSize);//软键盘最小列号
			break;
		case 1://任务1：键盘选择
			HandleKeyBoard(StringSize,KeyBoardSize,KeyBoardFunc);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			break;
		case 2://保存APN
			//保存数据
			//1.读出原来的BUF
			MR((u32 )ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2); 				
			//2.读出现在的BUF长度 k是APN的长度p8保存APN的内容
			p8=(u8 *)ADDR_DATABUFF+1024;
			k=0;
			for(x=GETSTRING_MINROW(StringSize);x<=GETSTRING_MAXROW(StringSize);x++)
			{
				if(x==GETSTRING_MINROW(StringSize))
				{
					i=GETSTRING_STRCOL(StringSize);
					Len=GETSTRING_MIDCOL(StringSize);	
				}else if(x==GETSTRING_MAXROW(StringSize))
				{
					i=0;
					Len=GETSTRING_ENDCOL(StringSize);
				}else
				{
					i=0;
					Len=GETSTRING_MIDCOL(StringSize);
				}
				p=(u8 *)(ADDR_STRINGBUFF+(x)*84+4);
				for(;i<=Len;i++)
				{
					if(p[i*2]!=0x20 && p[i*2]!=0x00)
					{
						p8[k++]=p[i*2];
					}
					else
					{
						p8[k++]=0;
						break;
					}
				}
			}
			//3.整合新的bUF到原来的空间里面去
			i=0;
			p=(u8 *)ADDR_DATABUFF;
			p=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{	
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			p=Get_Element((u8*)p,4,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			i=p[1];//apn长度
			Len= Get_DL698DataLen_S((u8 *)ADDR_DATABUFF,0);//数据长度一共有Len那么长
			if(i==k)
			{
				MW((u32)p8,(u32)p+2,i);
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,Len);
			}else 
			{
				//如果保存的数据比较大 要做一个判断
				if(k>i)
				{
					if((Len+(k-i))>LENmax_4520_2)
					{
						Stack->MenuStack[MenuCount->Count].Task=4;
						break;
					}
				}
				//1.保存头
				//01 02 02 08 16 00 16 00 0A
				p8=(u8 *)ADDR_DATABUFF;
				if(p>p8)
				{
					x=p-p8;
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,x+1);
				//2.保存APN
				p=(u8 *)ADDR_DATABUFF+2048+x+1;
				p[0]=k;
				MW((u32)ADDR_DATABUFF+1024,(u32)ADDR_DATABUFF+2048+x+2,k);
				//3.保存尾巴
				if(Len>x+i+2)
				{
					MW((u32)ADDR_DATABUFF+x+i+2,(u32)ADDR_DATABUFF+2048+x+2+k,Len-x-i-2);
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
			}
			//4.保存
			MW((u32 )ADDR_DATABUFF+2048,(u32)ADDR_4520_2,LENmax_4520_2);
			//设置成功提示ＯＫ
			DisplayClrRow(1,9);
			DisplayString(1,0,0,(u8 *)ADDR_DATABUFF+1024);
			DispalyAlign(1,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			DisplayString(4,7,0,"设置OK");
			DisplayString(5,4,0,"按任意键退出");
			Stack->MenuStack[MenuCount->Count].Task=3;
			break;
		case 3://按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 4:
			DisplayClrRow(1,9); 
			DisplayString(3,7,0,"设置失败");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 0xFF://错误信息
			DisplayClrRow(1,9); 
			DisplayString(3,6,0,"字符长度过长");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
			default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_2_1_3_1_4_JS
// Descriptions:		用户名和密码设置
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_4_JS(void)
{
	u8 *p;
	u8 *p8;
	u32 i;
	u32 k;
	u32 x;
	u32 Len;
	u32 Ln;//界面从第几行开始显示

	Ln=1;
//==============================软键盘参数=========================================
	u32 KeyBoardSize=(4<<28)|(8<<24)|(13<<16)|(0<<8)|19;//软键盘最小行号,//软键盘最大行号,//软键盘的最后一行最大列号,//软键盘最小列号,//软键盘最大列号
	u32 KeyBoardFunc=(15<<8)|18;//BIT15-BIT8第一个功能按键的位置//BIT7-BIT0第二个功能按键的位置
	u32 StringSize=((Ln+1)<<28)|((Ln+2)<<24)|(2<<16)|(19<<8)|18;//字符显示最小行BIT31-28,字符显示最大行BIT27-24,字符开始列BIT23-16,字符中间列BIT15-8,字符结束列BIT7-0
//================================================================================= 
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			CreateKeyBoard(KeyBoardSize);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			DisplayString(Ln,6,0,(u8 *)"密码设置");
			i=Display_4520_2(5,0,0,GETSTRING_MINROW(StringSize),GETSTRING_STRCOL(StringSize));//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()	
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			//确定字符数据的光标显示位置
			x=i&0xFF;//这是字符最大的行号
			if(x>GETSTRING_MAXROW(StringSize))//屏幕已经放不下所有的字符(另外做处理,目前没有处理)
			{
				Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
				break;
			}
			if(x==GETSTRING_MAXROW(StringSize))//如果当前已经是最大行
			{
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_ENDCOL(StringSize)+1)//光标可以放在最后一个位置
				{
					Stack->MenuStack[MenuCount->Count].Task=0xFF;//错误信息
					break;
				}
			}else
			{//中间行或者是第一行的话
				k=(i>>8)&0xff;//获取列的位置信息
				if(k>GETSTRING_MIDCOL(StringSize))
				{
					x++;//光标指向下一行
					k=0;//并且列号为0
				}
			}
			NegativeCursor(x,k);
			MenuCount->v1=(x<<8)|k;//BIT0-7光标列的位置,BIT8-15光标行的位置
			Stack->MenuStack[MenuCount->Count].EditRow=GETKEYBOARD_MINROW(KeyBoardSize);//软键盘最小行号
			Stack->MenuStack[MenuCount->Count].EditCol=GETKEYBOARD_MINCOL(KeyBoardSize);//软键盘最小列号
			break;
		case 1://任务1：键盘选择
			HandleKeyBoard(StringSize,KeyBoardSize,KeyBoardFunc);//在屏幕下面创建软键盘 VAL代表BIT31-28软键盘最小行号,BIT27-24软键盘最大行号,BITBIT23-16软键盘的最后一行最大列号,BIT15-8软键盘最小列号,BIT7-0软键盘最大列号
			break;
		case 2://保存APN
			//保存数据
			//1.读出原来的BUF
			MR((u32 )ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2); 				
			//2.读出现在的BUF长度 k是APN的长度p8保存APN的内容
			p8=(u8 *)ADDR_DATABUFF+1024;
			k=0;
			for(x=GETSTRING_MINROW(StringSize);x<=GETSTRING_MAXROW(StringSize);x++)
			{
				if(x==GETSTRING_MINROW(StringSize))
				{
					i=GETSTRING_STRCOL(StringSize);
					Len=GETSTRING_MIDCOL(StringSize);	
				}else if(x==GETSTRING_MAXROW(StringSize))
				{
					i=0;
					Len=GETSTRING_ENDCOL(StringSize);
				}else
				{
					i=0;
					Len=GETSTRING_MIDCOL(StringSize);
				}
				p=(u8 *)(ADDR_STRINGBUFF+(x)*84+4);
				for(;i<=Len;i++)
				{
					if(p[i*2]!=0x20 && p[i*2]!=0x00)
					{
						p8[k++]=p[i*2];
					}
					else
					{
						p8[k++]=0;
						break;
					}
				}
			}
			//3.整合新的bUF到原来的空间里面去
			i=0;
			p=(u8 *)ADDR_DATABUFF;
			p=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{	
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			p=Get_Element((u8*)p,5,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(!p)
			{
				Stack->MenuStack[MenuCount->Count].Task=4;
				break;
			}
			i=p[1];//apn长度
			Len= Get_DL698DataLen_S((u8 *)ADDR_DATABUFF,0);//数据长度一共有Len那么长
			if(i==k)
			{
				MW((u32)p8,(u32)p+2,i);
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,Len);
			}else 
			{
				//如果保存的数据比较大 要做一个判断
				if(k>i)
				{
					if((Len+(k-i))>LENmax_4520_2)
					{
						Stack->MenuStack[MenuCount->Count].Task=4;
						break;
					}
				}
				//1.保存头
				//01 02 02 08 16 00 16 00 0A
				p8=(u8 *)ADDR_DATABUFF;
				if(p>p8)
				{
					x=p-p8;
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
				MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+2048,x+1);
				//2.保存APN
				p=(u8 *)ADDR_DATABUFF+2048+x+1;
				p[0]=k;
				MW((u32)ADDR_DATABUFF+1024,(u32)ADDR_DATABUFF+2048+x+2,k);
				//3.保存尾巴
				if(Len>x+i+2)
				{
					MW((u32)ADDR_DATABUFF+x+i+2,(u32)ADDR_DATABUFF+2048+x+2+k,Len-x-i-2);
				}else
				{
					Stack->MenuStack[MenuCount->Count].Task=4;
					break;
				}
			}
			//4.保存
			MW((u32 )ADDR_DATABUFF+2048,(u32)ADDR_4520_2,LENmax_4520_2);
			//设置成功提示ＯＫ
			DisplayClrRow(1,9);
			DisplayString(1,0,0,(u8 *)ADDR_DATABUFF+1024);
			DispalyAlign(1,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			DisplayString(4,7,0,"设置OK");
			DisplayString(5,4,0,"按任意键退出");
			Stack->MenuStack[MenuCount->Count].Task=3;

			break;
		case 3://按任意键退出
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 4:
			DisplayClrRow(1,9); 
			DisplayString(3,7,0,"设置失败");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		case 0xFF://错误信息
			DisplayClrRow(1,9); 
			DisplayString(3,6,0,"字符长度过长");
			DisplayString(5,4,0,"按任意键退出");
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
			default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_2_1_3_1_5_JS
// Descriptions:		代理服务器地址和端口
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_5_JS(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 k;
	u32 j;
	u32 x;
	u32 Ln;
	u32 NegativelIpMaxCol=21;
	u32 NegativelIpMinCol=5;
	u32 NegativelPortMaxCol=11;
	u32	NegativelPortMinCol=7;
	Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_3_1_5_JS);//显示菜单
			//IP
			Display_4520_2(6,0,0,Ln+1,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			//端口
			Display_4520_2(7,0,0,Ln+2,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()		
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelIpMaxCol,NegativelIpMinCol); 	
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
					case 1://IP
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelIpMaxCol,NegativelIpMinCol);
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
						break;
					case 2://端口
						ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelPortMaxCol,NegativelPortMinCol);
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
						case 1://IP 按右键不加
							break;
						case 2://端口
							p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
						case 1://IP 按左键不减
							break;	
						case 2://端口
							p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
						case 1://IP
							Stack->MenuStack[MenuCount->Count].EditCol=NegativelIpMinCol;
							break;
						case 2://端口
							Stack->MenuStack[MenuCount->Count].EditCol=NegativelPortMinCol;
							DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
							break;
						case 3:
							DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(Ln+3,9,0,"OK");
							//保存设置的数据							
							MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);			
							p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
							//保存IP
							p8=Get_Element((u8*)p8,6,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
							if(p8)
							{
								p8+=2;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[i]=p8_bcd(p+8*i,3);
								}
							}
							//保存端口	
							p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF							
							p8=Get_Element((u8*)p8,7,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
							if(p8)
							{
								p8++;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
								i=p8_bcd(p,5);
								p8[0]=((i&0xff00)>>8);
								p8[1]=(i&0xff);
							}	
							MW((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);								
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
			}else
			{
				switch(Stack->MenuStack[MenuCount->Count].EditRow)
				{
					case 1:
					Select(NegativelIpMaxCol,NegativelIpMinCol);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{
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
						Stack->MenuStack[MenuCount->Count].EditCol=0;	
					}
					Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					break;
					case 2:
						Select(NegativelPortMaxCol,NegativelPortMinCol);
						UpDownSelect();
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'左右箭头'
							p16[3]=33;
							p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
							x=p8_bcd(p8,5);
							if(x>65535)
							{
								bcd_p8(p8,0,5);
							}
							Stack->MenuStack[MenuCount->Count].Task=1;
							Stack->MenuStack[MenuCount->Count].EditCol=0;	
						}			
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
						break;
					default:
					break;
				}
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
// Function name:		Menu2_2_1_3_1_6_JS
// Descriptions:		主站通信参数
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_6_JS(void)
{
	u8 *p;
	u8 *p8;
	u8 *p1;
	u8 *p2;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 NegativelIpMaxCol=21;
	u32 NegativelIpMinCol=5;
	u32 NegativelPortMaxCol=11;
	u32	NegativelPortMinCol=7;
	u32 Ln;
	Ln=1;//菜单从第1行开始
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_3_1_6_JS);//显示菜单
			//IP
			Display_4520_2(8,1,1,Ln+1,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			//端口
			Display_4520_2(8,1,2,Ln+2,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			
			//备IP
			Display_4520_2(8,2,1,Ln+3,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			//备端口
			Display_4520_2(8,2,2,Ln+4,0);//在界面上面显示4520的内容,pStr传入的数据,GroupNum备用通道号,SubElement,SSubElement参数介绍参照Get_Element()
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelIpMaxCol,NegativelIpMinCol); 	
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayString(Ln+5,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{	
						case 1://IP
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelIpMaxCol,NegativelIpMinCol);
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
							break;
						case 2://端口
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelPortMaxCol,NegativelPortMinCol);
							break;
						case 3://IP
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelIpMaxCol,NegativelIpMinCol);
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
							break;
						case 4://端口
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,NegativelPortMaxCol,NegativelPortMinCol);
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
							case 1://IP 按右键不加
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP 按左键不减
							case 3:
								break;	
							case 2://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=NegativelIpMinCol;
								break;
							case 2://端口
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=NegativelPortMinCol;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(Ln+5,9,0,"OK");
								//保存设置的数据
								MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);			
								#ifdef DEBUG_MENU
								myprintf("[%s:%d]:MenuCount->v3=%d\n",(u32)&__func__,(u32)__LINE__,MenuCount->v3);
								#endif
								p1=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,8,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								p2=Get_Element((u8*)p1,1,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								//主IP
								p8=Get_Element((u8*)p2,1,1,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(p8)
								{
									p8+=2;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
								}
								//主端口					
								p8=Get_Element((u8*)p2,1,2,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(p8)
								{
									p8++;
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									i=p8_bcd(p,5);
									p8[0]=((i&0xff00)>>8);
									p8[1]=(i&0xff);
								}	
								//备用IP
								p2=Get_Element((u8*)p1,2,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF							
								p8=Get_Element((u8*)p2,1,1,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(p8)
								{
									p8+=2;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
								}
								//备用端口					
								p8=Get_Element((u8*)p2,1,2,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
								if(p8)
								{
									p8++;
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									i=p8_bcd(p,5);
									p8[0]=((i&0xff00)>>8);
									p8[1]=(i&0xff);
								}
								#ifdef DEBUG_MENU
								myprintf("[%s:%d]:22222222222222222222\n",(u32)&__func__,(u32)__LINE__,0);
								#endif
								MW((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
						case 3:
							Select(NegativelIpMaxCol,NegativelIpMinCol);
							break;
						case 2:
						case 4:
							Select(NegativelPortMaxCol,NegativelPortMaxCol);
							break;
						default:
						break;
					}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
// Function name:		Menu2_2_1_3_1_JS
// Descriptions:		查询一组备用通道 
// input parameters:	无
// output parameters:	无
// Returned value:		无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_1_JS(void)
{
	
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 j;
	u32 k;
	u32 Ln;
	u32 GroupMinRol=14;//备用通道号的最小列
	u32 GroupMaxRol=16;//备用通道号的最大列
	Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0:
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//清全屏
			Draw_Clr();
			DisplayMenuString(Ln,0,0,(u8 *)pMenu2_2_1_3_1_JS);//显示菜单
			//备用通道组号
			MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,2);//读到组号就行
			p8=(u8 *)ADDR_DATABUFF;		
			MenuCount->v2=p8[1];
			//显示内存中数据		
			if((MenuCount->v3==0)||(MenuCount->v3>0xFF))//第一次进入
			{
				//备用通道组号				
				MenuCount->v3=1;		
			}
			if(MenuCount->v2==0)//FLASH中没有数据
			{
				MenuCount->v3=0;
			}
			p=(u8*)(ADDR_STRINGBUFF+Ln*84+4+(10)*2);
			bcd_p8(p,(u64)MenuCount->v2,3);//显示一共有多少组通道
			p16=(u16*)(ADDR_STRINGBUFF+Ln*84+4+(13)*2);
			p16[0]='-'+0x2000;	
			p=(u8*)(ADDR_STRINGBUFF+Ln*84+4+(GroupMinRol)*2);
			bcd_p8(p,(u64)MenuCount->v3,3);//显示当前通道
			ArrowLR(Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Ln,GroupMaxRol,GroupMinRol);
			break;
		case 1:
			//对第一行通道号进行编辑
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					MenuSelect();//菜单选择
					if(Stack->MenuStack[MenuCount->Count].EditRow!=8)
					{
						DisplayString(Ln+8,6,0,"删除通道");
					}
					//对第一行做特殊处理
					if(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow ==Ln)
					{
						ArrowLR(Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						NegativeLnMaxColMinCol(Ln,GroupMaxRol,GroupMinRol);
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							Comm_Ram->ButtonVal&=~0x08;//键使用后清0
							i=MenuCount->v3;
							if((i+1)<=MenuCount->v2)
							{//通道号+1
								p=(u8*)(ADDR_STRINGBUFF+Ln*84+4+(GroupMinRol)*2);
								p8_add1(p, 3);
								i++;
								MenuCount->v3=i;
							}
						}
						if((Comm_Ram->ButtonVal&0x04)!=0)
						{//左键
							Comm_Ram->ButtonVal&=~0x04;//键使用后清0
							i=MenuCount->v3;
							if(i>1)
							{//通道号+1
								p=(u8*)(ADDR_STRINGBUFF+Ln*84+4+(GroupMinRol)*2);
								p8_sub1(p, 3);
								i--;
								MenuCount->v3=i;
							}
						}
					}else
					{
						//清除其他行的左右箭头
						for(x=0;x<10;x++)
						{
							if(x!=Stack->MenuStack[MenuCount->Count].EditRow)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(x+Ln)*84+4+16*2);
								if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
								{
									for(i=2;i<=3;i++)
									{
										p16[i]=(p16[i]&0x8000)+0x2020;
									}
								}
							}
						}
						Stack->MenuStack[MenuCount->Count].Task=2;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								DisplayString(Ln, 18, 0, (u8 *)"写");
								Stack->MenuStack[MenuCount->Count].EditCol=GroupMinRol;
								break;
							default:
								break;
						} 
						Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
					}
				}else
				{
					Stack->MenuStack[MenuCount->Count].EditRow--;
					Select(GroupMaxRol,GroupMinRol);
					UpDownSelect();
					Stack->MenuStack[MenuCount->Count].EditRow++;
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确定	
						p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+(GroupMinRol)*2);
						i=(p8[0]-0x30)*100+(p8[2]-0x30)*10+(p8[4]-0x30);
						if(i>MenuCount->v2)
						{
							MenuCount->v3=1;
						}else
						{
							MenuCount->v3=i;
						}
						Stack->MenuStack[MenuCount->Count].Task=0;
						Stack->MenuStack[MenuCount->Count].EditCol=0;
					}
					Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
					Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
					break;
				}
			}
			break;
		case 2:
			MenuSelect();//菜单选择
			if(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow==Ln)
			{
				Stack->MenuStack[MenuCount->Count].Task=1;
			}else
			{
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_3_1_1_JS);//运行商和网络类型
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_3_1_2_JS);//APN设置
						break;
					case 4://用户名
						MenuCreate((u32)Menu2_2_1_3_1_3_JS);//用户名设置
						break;
					case 5://密码设置
						MenuCreate((u32)Menu2_2_1_3_1_4_JS);//密码
						break;
					case 6://
						MenuCreate((u32)Menu2_2_1_3_1_5_JS);//服务器地址和端口
						break;
					case 7:
						MenuCreate((u32)Menu2_2_1_3_1_6_JS);//主站通信参数
						break;
					case 8://删除通道
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确定
							Stack->MenuStack[MenuCount->Count].Task=3;
							Comm_Ram->ButtonVal&=~0x10;//按键消除
						}
					default:
						break;
				} 
				Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
			break;
		case 3://删除通道
			DisplayClrRow_4(Ln+7);//清屏幕显示的指定行(4个控制字不清)
			DisplayString(Ln+7,9,0,"OK");
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			MC(0,ADDR_DATABUFF+LENmax_4520_2,LENmax_4520_2);
			MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);
			p=(u8 *)ADDR_DATABUFF;
			k=p[1];
			switch (k)
			{
				case 0://没有要删除的数据项
					break;
				case 1://清空Flash中的数据项
					p=(u8 *)ADDR_DATABUFF+LENmax_4520_2;
					p[0]=0x01;
					p[1]=0x00;
					MW((u32)ADDR_DATABUFF+LENmax_4520_2,(u32)ADDR_4520_2,LENmax_4520_2);
					break;
				default:
					if(MenuCount->v3==k)//要删除的是最后一个数据
					{					
						p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p8>p)
						{
							i=p8-p;
							if(LENmax_4520_2>i)
							{
								MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+LENmax_4520_2,i);
								p=(u8 *)ADDR_DATABUFF+LENmax_4520_2;
								p[1]=k-1;
								MW((u32)ADDR_DATABUFF+LENmax_4520_2,(u32)ADDR_4520_2,LENmax_4520_2);
							}
						}
						
					}else
					{
						//ADDR_DATABUFF存着原来的报文,新组的报文全部放在ADDR_DATABUFF+LENmax_4520_2,
						//具体按照 保存头 删除中间 保存尾巴的方式
						//1.保存头部						
						j=Get_DL698DataLen_S((u8 *)ADDR_DATABUFF,0);//数据长度一共有j那么长
						p8=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p8>p)
						{
							i=p8-p;
							if(j>i)
							{
								x=i;
								j-=i;
								if(LENmax_4520_2>i)
								{
									MW((u32)ADDR_DATABUFF,(u32)ADDR_DATABUFF+LENmax_4520_2,i);
								}
							}
						}					
						//2.删除中间	
						//找到当前数据的头指针和下一个数据的头指针,地址相减就是头长度
						p=Get_Element((u8*)ADDR_DATABUFF,MenuCount->v3+1,0,LENmax_4520_2);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p)
						{
							if(p>p8)
							{
								i=p-p8;
								if(j>i)
								{
									j-=i;
									//3.保存尾部
									MW((u32)p,(u32)ADDR_DATABUFF+LENmax_4520_2+x,j);
									p=(u8 *)ADDR_DATABUFF+LENmax_4520_2;
									p[1]=k-1;
									MW((u32)ADDR_DATABUFF+LENmax_4520_2,(u32)ADDR_4520_2,LENmax_4520_2);
								}
							}
							
						}
					}
					break;
			}
			Stack->MenuStack[MenuCount->Count].Task=4;
			break;
		case 4:	
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0xFF;//键使用后清0
			break;
		default:
			break;
	}
	
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_3_JS
// Descriptions:        公网远程通信多接入点备用通道 
// input parameters:    MenuCount->v3表示当前选中的组号小于255(初始化为0),MenuCount->v2表示最大组号,MenuCount->v1在APN 用户名 密码里面做光标的参数
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3_JS(void)
{
	u8 *p8;
	u32 i;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_3_JS);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://查询一组通道
						MenuCount->v3=0;//用作标识符,第一次进入组号用1开始显示
						MenuCreate((u32)Menu2_2_1_3_1_JS);//查询公网远程通信多接入点备用通道           
						break;
					case 2://添加一组备用通道
						//直接先添加一组默认的配置(加在原来Flash空间的后面),再里面修改,不需要再删除
						//或外面创建一个空间buf,先全部写在buf里面,在存在ADDR_4520_2
						MR((u32)ADDR_DATABUFF,(u32)ADDR_4520_2,LENmax_4520_2);
						p8=(u8 *)ADDR_DATABUFF;
						i=Get_DL698DataLen_S(p8,0);//计算出原来Flash中的数据有多长
						if((i+sizeof(default_4520))<LENmax_4520_2)
						{
							p8[1]++;//通道组号加1;
							MenuCount->v3=(u32)p8[1];
							MW((u32)default_4520,(u32)p8+i,sizeof(default_4520));
							MW((u32)p8,(u32)ADDR_4520_2,LENmax_4520_2);
							MenuCreate((u32)Menu2_2_1_3_1_JS);//查询公网远程通信多接入点备用通道 
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
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1
// Descriptions:        通信通道设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void)//菜单4_3
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1);//显示菜单
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
						MenuCreate((u32)Menu2_2_1_1);
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2);
						break;
					#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
					case 3:
						MenuCreate((u32)Menu2_2_1_3_JS);
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
#if(USER/100)==15
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_6
// Descriptions:        设备地理位置设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_6(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u8 *p8d;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_6);//显示菜单
			//经度
			MenuCount->v1=9;
			MenuCount->v2=14;
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_4004,LENmax_4004);
			if(p8)
			{
				if(p8[5]==0)
					DisplayString(1,9,0,(u8 *)Longitude[0]);//经度方位-东经
				else
					DisplayString(1,9,0,(u8 *)Longitude[1]);//经度方位-西经
				
				p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
				i=hex_bcd((u32)p8[7]);
				DisplayData(i,3,0,p16);
					
				DisplayString(2,8,0,(u8 *)BearingUnits[0]);
				p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2+5*2);
				i=hex_bcd((u32)p8[9]);
				DisplayData(i,3,0,p16);
				DisplayString(2,13,0,(u8 *)BearingUnits[1]);
				p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2+10*2);
				i=hex_bcd((u32)p8[11]);
				DisplayData(i,3,0,p16);
				DisplayString(2,18,0,(u8 *)BearingUnits[2]);
				
				if(p8[15]==0)
					DisplayString(3,9,0,(u8 *)Latitude[0]);//纬度方位-南纬
				else
					DisplayString(3,9,0,(u8 *)Latitude[1]);//纬度方位-北纬
				
				p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2);
				i=hex_bcd((u32)p8[17]);
				DisplayData(i,3,0,p16);
				DisplayString(4,8,0,(u8 *)BearingUnits[0]);
				p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2+5*2);
				i=hex_bcd((u32)p8[19]);
				DisplayData(i,3,0,p16);
				DisplayString(4,13,0,(u8 *)BearingUnits[1]);
				p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2+10*2);
				i=hex_bcd((u32)p8[21]);
				DisplayData(i,3,0,p16);
				DisplayString(4,18,0,(u8 *)BearingUnits[2]);
				
				p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+5*2);
				i=hex_bcd((u32)((p8[23]<<24)|(p8[24]<<16)|(p8[25]<<8)|(p8[26])));
				DisplayData(i,8,0,p16);
				DisplayString(5,13,0,(u8 *)"cm");
			}
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=6)
					{
						DisplayString(6,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=21;
							for(k=0;k<10;k++)
							{
								if(k!=(Stack->MenuStack[MenuCount->Count].EditRow))
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
							MenuCount->v1=9;
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=5;
							MenuCount->v2=21;
							for(k=0;k<10;k++)
							{
								if(k!=(Stack->MenuStack[MenuCount->Count].EditRow))
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
							MenuCount->v1=5;
							MenuCount->v2=16;
							for(k=0;k<10;k++)
							{
								if(k!=(Stack->MenuStack[MenuCount->Count].EditRow))
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
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Stack->MenuStack[MenuCount->Count].EditRow))
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
					if(((Comm_Ram->ButtonVal&0x08)!=0)||((Comm_Ram->ButtonVal&0x04)!=0))
					{//左右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8d=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_4004,LENmax_4004);
								switch(p8d[0])
								{
									case 0x18://东经
										i=1;
										if(p8[5]!=1)
										{
											MC(0,ADDR_DATABUFF+7,1);
											MC(0,ADDR_DATABUFF+9,1);
											MC(0,ADDR_DATABUFF+11,1);
										}
										break;
									case 0x50://西经
										i=0;
										if(p8[5]!=0)
										{
											MC(0,ADDR_DATABUFF+7,1);
											MC(0,ADDR_DATABUFF+9,1);
											MC(0,ADDR_DATABUFF+11,1);
										}
										break;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8 *)Longitude[i]);
								p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
								i=hex_bcd((u32)p8[7]);
								DisplayData(i,3,0,p16);
								p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2+5*2);
								i=hex_bcd((u32)p8[9]);
								DisplayData(i,3,0,p16);
								p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2+10*2);
								i=hex_bcd((u32)p8[11]);
								DisplayData(i,3,0,p16);
								break;
							case 3:
								p8d=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_4004,LENmax_4004);
								switch(p8d[0])
								{
									case 0x78://北纬
										i=0;
										if(p8[15]!=0)
										{
											MC(0,ADDR_DATABUFF+17,1);
											MC(0,ADDR_DATABUFF+19,1);
											MC(0,ADDR_DATABUFF+21,1);
										}
										break;
									case 0xa8://南纬
										i=1;
										if(p8[15]!=1)
										{
											MC(0,ADDR_DATABUFF+17,1);
											MC(0,ADDR_DATABUFF+19,1);
											MC(0,ADDR_DATABUFF+21,1);
										}
										break;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8 *)Latitude[i]);
								p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2);
								i=hex_bcd((u32)p8[17]);
								DisplayData(i,3,0,p16);
								p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2+5*2);
								i=hex_bcd((u32)p8[19]);
								DisplayData(i,3,0,p16);
								p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2+10*2);
								i=hex_bcd((u32)p8[21]);
								DisplayData(i,3,0,p16);
								break;
						}
						
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
							case 3:
								break;
							case 2:
							case 4:
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 6:
								DisplayClrRow_4(6);//清屏幕显示的指定行(4个控制字不清)
								DisplayString(6,9,0,"OK");
								//保存设置的数据
							
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
								if(p[0]==0x18)
									p8[0]=0;
								if(p[0]==0x50)
									p8[0]=1;
								MW(ADDR_DATABUFF,ADDR_4004+5,1);//经度方位
								
								p=(u8 *)(ADDR_STRINGBUFF+2*84+4+5*2);
								for(i=0;i<3;i++)//经度的度分秒
								{
									p8[1+i]=p8_bcd(p+10*i,3);
									//p8[1+i]=bcd_hex(p8_bcd(p+5*i,3));
								}
								MW(ADDR_DATABUFF+1,ADDR_4004+7,1);//经度的度
								MW(ADDR_DATABUFF+2,ADDR_4004+9,1);//经度的分
								MW(ADDR_DATABUFF+3,ADDR_4004+11,1);//经度的秒
								
								p=(u8 *)(ADDR_STRINGBUFF+3*84+4+9*2);
								if(p[0]==0x78)
									p8[4]=1;
								if(p[0]==0xa8)
									p8[4]=0;
								MW(ADDR_DATABUFF+4,ADDR_4004+15,1);//纬度方位
								p=(u8 *)(ADDR_STRINGBUFF+4*84+4+5*2);
								for(i=0;i<3;i++)//纬度的度分秒
								{
									p8[5+i]=p8_bcd(p+10*i,3);
									//p8[5+i]=bcd_hex(p8_bcd(p+10*i,3));
								}
								MW(ADDR_DATABUFF+5,ADDR_4004+17,1);//纬度的度
								MW(ADDR_DATABUFF+6,ADDR_4004+19,1);//纬度的分
								MW(ADDR_DATABUFF+7,ADDR_4004+21,1);//纬度的秒
								p=(u8 *)(ADDR_STRINGBUFF+5*84+4+5*2);
								i=p8_bcd(p,8);
								k=R_Inverse(i,4);
								MWR(k,ADDR_4004+23,4);
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
				else
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 2:
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+5*2,3);
								if(x>255)
								{
									bcd_p8(p8+5*2,0,3);
								}
								x=p8_bcd(p8+10*2,3);
								if(x>255)
								{
									bcd_p8(p8+10*2,0,3);
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
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_2_JS
// Descriptions:        电能表参数设置  隐藏功能: 波特率=自适应 端口=未知 规约=未知 时，将删除该测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_2_JS(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;
	u32 port_oad;
	u32 delete_pn=0;
	u32 Ln;
#if (LENper_6000+1024)>LEN_DATABUFF
	#error
#endif
Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			Stack->MenuStack[MenuCount->Count].EditRow=0;
			//清全屏
			DisplayClr();//清全屏
			Draw_Clr();
			DisplayMenuString(1,0,0,(u8 *)pMenu2_2_2);	
			//显示内存中数据
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>=NUMmax_6000)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=NUMmax_6000-1;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+1*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//先把参数读到ADDR_DATABUFF+1024里，如果是无效的配置信息，就用default_6000里的默认参数。
			//然后修改的数据都先更新到ADDR_DATABUFF+1024里，界面上的参数都更新完了，再一起写到ADDR_6000
			//这样做的目的是，界面设置项少。防止原来的数据被修改了。
			//先读出来放ADDR_DATABUFF+1024，然后再根据界面参数更新ADDR_DATABUFF+1024内容，再保存，就不会修改界面无法修改的内容
			//测量点号
			p8=(u8*)ADDR_DATABUFF+1024;
			MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
			if(p8[0]==0)//如果本来不存在该配置序号，就应该是新加的，采用默认配置
			{
				MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
			}
			else//存在，看下选择的序号和配置参数里的配置序号是否一致
			{
				p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
				k=p8[2]+p8[1]*0x100;
				if(k!=MenuCount->v3)
				{
					MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
				}
			}
			//通信速率
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,2,LENper_6000);
			p=(u8*)(ADDR_STRINGBUFF+2*84+4+8*2);
			if(p8[1]>10)
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[11]);//显示通信速率
			else
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[p8[1]]);//显示通信速率

			//端口号、通信协议类型
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,4,LENper_6000);
			port_oad=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|(p8[4]);
			if(port_oad==0xf2090201)
				DisplayString(3,8,0,(u8 *)PortList[4]);//显示端口--载波
			else if(port_oad==0xf2080201)
				DisplayString(3,8,0,(u8 *)PortList[1]);//显示端口--交采
			else if(port_oad==0xf2010201)
				DisplayString(3,8,0,(u8 *)PortList[2]);//显示端口--4851
			else if(port_oad==0xf2010202)
				DisplayString(3,8,0,(u8 *)PortList[3]);//显示端口--4582
			else
				DisplayString(3,8,0,(u8 *)PortList[0]);//显示端口--未知
			//通信协议类型	
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,3,LENper_6000);
			if(p8[1]>6)
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[0]);//通信协议列表
			}
			else
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[p8[1]]);//通信协议列表
			}
		
			//通信地址
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+4*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,6);
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
			y=MRR(ADDR_DATABUFF,6);
			DisplayData(y,12,0,p16);		
			//用户类型
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,7,LENper_6000);
			p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+8*2);
			y=(u64)hex_bcd(p8[1]);
			DisplayData(y,3,0,p16);

			//采集器地址
			p16=(u16 *)(ADDR_STRINGBUFF+7*84+4+6*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),3,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,6);
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
			y=MRR(ADDR_DATABUFF,6);
			DisplayData(y,12,0,p16);

			ArrowLR(Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			NegativeLnMaxColMinCol(Ln,MenuCount->v2,MenuCount->v1);
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//下移键按下
						y=Stack->MenuStack[MenuCount->Count].EditRow+1;
						for(i=0;i<10;i++)
						{
							if(StrBuff->Row[i].Row==y)
							{
								break;
							}
						}						
						if(i!=10)
						{
							Stack->MenuStack[MenuCount->Count].EditRow=y;
						}
					}
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//上移键按下
						y=Stack->MenuStack[MenuCount->Count].EditRow;
						if(y>0)
						{
							y--;
							Stack->MenuStack[MenuCount->Count].EditRow=y;
						}
					}   		
					if(Stack->MenuStack[MenuCount->Count].EditRow!=7)
					{
						DisplayString(Ln+7,6,0,"确认设置");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow+Ln)
					{
						case 1://测量点号
							MenuCount->v1=8;
							MenuCount->v2=11;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 2://通信速率
							MenuCount->v1=8;
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 3://通信端口
							MenuCount->v1=8;
							MenuCount->v2=12;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 4://通信协议类型	
							MenuCount->v1=8;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 5://地址
							MenuCount->v1=4;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 6://用户类型
							MenuCount->v1=8;
							MenuCount->v2=10;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 7://采集器地址
							MenuCount->v1=6;
							MenuCount->v2=17;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow+Ln,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow+Ln,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(k+Ln)*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow+Ln);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Ln+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1://测量点号
								p8=(u8 *)(ADDR_STRINGBUFF+Ln*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<(NUMmax_6000-2))
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
							case 2://通信速率
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//自适应
									i=0;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=1;
											break;
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
											i=8;
											break;
										case 38:
											i=9;
											break;
										case 57:
											i=10;
											break;
										case 11:
											i=11;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3://通信端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=1;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//交采
									i=2;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//载波
									i=0;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=3;
										if(x==2)
											i=4;
										break;
									default:
										break;
								}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)PortList[i]);
								break;
							case 4://通信协议类型	
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=1;
								}
								else if(p8[0]==0x41)//ABB
								{
									i=6;
								}
								else if(p8[0]==0x49)//IEC1107
								{
									i=0;
								}
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=2;
										if(x==20)
											i=3;
										break;
									case 69:
										i=4;
										break;
									case 18:
										//i=0;
										i=5;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5://地址
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+4*2);
								p8_add1(p8,12);
								break;
							case 6://用户类型
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								p8_add1(p8,3);
								break;
							case 7://采集器地址
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+6*2);
								p8_add1(p8,12);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow+Ln)
						{
							case 1://测量点号
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,(NUMmax_6000-1),4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2://通讯速率
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//自适应
									i=10;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=11;
											break;
										case 60:
											i=0;
											break;
										case 12:
											i=1;
											break;
										case 24:
											i=2;
											break;
										case 48:
											i=3;
											break;
										case 72:
											i=4;
											break;
										case 96:
											i=5;
											break;
										case 19:
											i=6;
											break;
										case 38:
											i=7;
											break;
										case 57:
											i=8;
											break;
										case 11:
											i=9;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3://端口
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=4;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//交采
									i=0;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//载波
									i=3;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=1;
										if(x==2)
											i=2;
										break;
									default:
										break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)PortList[i]);
								break;
							case 4://规约
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									//i=4;
									i=6;
								}
								else if(p8[0]==0x41)//ABB
								{
									i=4;
								}
								else if(p8[0]==0x49)//IEC1107
								{
									i=5;
								}
								
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=0;
										if(x==20)
											i=1;
										break;
									case 69:
										i=2;
										break;
									case 18:
										i=3;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5://地址
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+4*2);
								p8_sub1(p8,12);
								break;
							case 6://用户类型
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								p8_sub1(p8,3);
								break;
							case 7://采集器地址
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+6*2);
								p8_sub1(p8,12);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow+Ln)
						{
							case 1://测量点号
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,16,0,"输入");
								break;
							case 5://地址
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,16,0,"输入");
								break;
							case 6://用户类型
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,16,0,"输入");
								break;
							case 7://采集器地址
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,16,0,"输");
								break;
							case 8:
								DisplayClrRow_4(8);//清屏幕显示的指定行(4个控制字不清)
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow+Ln)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>NUMmax_6000)
								{//测量点超过最大值 则不可设置
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow+Ln,7,0,"无效测量点");
								}
								else
								{
									DisplayClrRow_4(8);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(8,9,0,"OK");

									p8=(u8*)ADDR_DATABUFF+1024;
									MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									if(p8[0]==0)//如果本来不存在该配置序号，就应该是新加的，采用默认配置
									{
										MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
									}
									else//存在，看下选择的序号和配置参数里的配置序号是否一致
									{
										p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
										k=p8[2]+p8[1]*0x100;
										if(k!=MenuCount->v3)
										{
											MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
										}
									}
									//测量点号
									MWR(MenuCount->v3,ADDR_DATABUFF,2);
									Data_Inverse(ADDR_DATABUFF,2);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),1,0,LENper_6000,ADDR_DATABUFF,2);
									//通讯速率
									p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xa6&&p8[1]==0x63)
									{//自适应
										i=255;
										delete_pn+=1;
									}
									else
									{
										switch(x)
										{
											case 30:
												i=0;
												break;
											case 60:
												i=1;
												break;
											case 12:
												i=2;
												break;
											case 24:
												i=3;
												break;
											case 48:
												i=4;
												break;
											case 72:
												i=5;
												break;
											case 96:
												i=6;
												break;
											case 19:
												i=7;
												break;
											case 38:
												i=8;
												break;
											case 57:
												i=9;
												break;
											case 11:
												i=10;
												break;
											default:
												break;
										}
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,2,LENper_6000,ADDR_DATABUFF,1);
									//端口
									p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//未知
										i=0;
										delete_pn+=1;
									}
									else if(p8[0]==0x5c&&p8[1]==0x50)
									{//交采
										i=0xf2080201;
									}
									else if(p8[0]==0x7a&&p8[1]==0x61)
									{//载波
										i=0xf2090201;
									}
									else
									{
										switch(x)
										{
										case 48:
											x=p8_bcd(p8+8,1);
											if(x==1)
												i=0xf2010201;
											if(x==2)
												i=0xf2010202;
											break;
										default:
											break;
										}
									}	
									MWR(i,ADDR_DATABUFF,4);
									Data_Inverse(ADDR_DATABUFF,4);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,4,LENper_6000,ADDR_DATABUFF,4);
									//规约
									p8=(u8 *)(ADDR_STRINGBUFF+4*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//未知
										i=0;
										delete_pn+=1;
									}
									else if(p8[0]==0x41)//ABB
									{
										i=5;
									}
									else if(p8[0]==0x49)//IEC1107
									{
										i=6;
									}
									else
									{
										switch(x)
										{
										case 64:
											x=p8_bcd(p8+8,2);
											if(x==19)
												i=1;
											if(x==20)
												i=2;
											break;
										case 69:
											i=3;
											break;
										case 18:
											i=4;
											break;
										default:
											break;
										}
										
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,3,LENper_6000,ADDR_DATABUFF,1);

									//地址
									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+5*84+4+4*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),2,1,LENper_6000,ADDR_DATABUFF,6);
									//用户类型
									p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+8*2);
									x=p8_bcd(p8,3);
									MWR(x,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,7,LENper_6000,ADDR_DATABUFF,1);									
									//采集器地址
									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+7*84+4+6*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),3,1,LENper_6000,ADDR_DATABUFF,6);
									
									if(delete_pn==3)//隐藏功能启用
									{
										MC(0,(ADDR_DATABUFF+1024),LENper_6000);
										DisplayString(8,6,0,"删除成功");
									}
									MW((u32)(ADDR_DATABUFF+1024),ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									Terminal_Ram->SET6000=0x55;//采集档案配置表有设置标志:0=没设置,0x55=设置
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow+Ln)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>NUMmax_6000 || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+5*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节，判断内存数据如果有错则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,1,12);
								}
								break;
							case 6:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow+Ln)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节，判断内存数据如果有错则初始化
								for(i=0;i<=1;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=2)
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
// Function name:       Menu2_2_2
// Descriptions:        电能表参数设置  隐藏功能: 波特率=自适应 端口=未知 规约=未知 时，将删除该测量点
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;
	u32 port_oad;
	u32 delete_pn=0;
#if (LENper_6000+1024)>LEN_DATABUFF
 	#error
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if	((Project/100)==2&&(USER/100)==17)//江苏集中器
				//清全屏
				DisplayClr();//清全屏
				Draw_Clr();
				DisplayMenuString(1,0,0,(u8 *)pMenu2_2_2);
			#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_2);//显示菜单
			#endif
			//显示内存中数据
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>=NUMmax_6000)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=NUMmax_6000-1;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3记录配置序号的全局变量
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+1*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//先把参数读到ADDR_DATABUFF+1024里，如果是无效的配置信息，就用default_6000里的默认参数。
			//然后修改的数据都先更新到ADDR_DATABUFF+1024里，界面上的参数都更新完了，再一起写到ADDR_6000
			//这样做的目的是，界面设置项少。防止原来的数据被修改了。
			//先读出来放ADDR_DATABUFF+1024，然后再根据界面参数更新ADDR_DATABUFF+1024内容，再保存，就不会修改界面无法修改的内容
			//测量点号
			p8=(u8*)ADDR_DATABUFF+1024;
			MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
			if(p8[0]==0)//如果本来不存在该配置序号，就应该是新加的，采用默认配置
			{
				MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
			}
			else//存在，看下选择的序号和配置参数里的配置序号是否一致
			{
				p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
				k=p8[2]+p8[1]*0x100;
				if(k!=MenuCount->v3)
				{
					MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
				}
			}
			//通信速率
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,2,LENper_6000);
			p=(u8*)(ADDR_STRINGBUFF+2*84+4+8*2);
			if(p8[1]>10)
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[11]);//显示通信速率
			else
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[p8[1]]);//显示通信速率

			//端口号、通信协议类型
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,4,LENper_6000);
			port_oad=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|(p8[4]);
			if(port_oad==0xf2090201)
				DisplayString(3,8,0,(u8 *)PortList[4]);//显示端口--载波
			else if(port_oad==0xf2080201)
				DisplayString(3,8,0,(u8 *)PortList[1]);//显示端口--交采
			else if(port_oad==0xf2010201)
				DisplayString(3,8,0,(u8 *)PortList[2]);//显示端口--4851
			else if(port_oad==0xf2010202)
				DisplayString(3,8,0,(u8 *)PortList[3]);//显示端口--4582
			else
				DisplayString(3,8,0,(u8 *)PortList[0]);//显示端口--未知
			//通信协议类型	
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,3,LENper_6000);
			if(p8[1]>6)
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[0]);//通信协议列表
			}
			else
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[p8[1]]);//通信协议列表
			}
		
			//通信地址
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+4*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,6);
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
			y=MRR(ADDR_DATABUFF,6);
			DisplayData(y,12,0,p16);
			//费率数
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,6,LENper_6000);
			p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+6*2);
			DisplayData(p8[1],2,0,p16);
			
			//接线方式
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,8,LENper_6000);
			if(p8[1]>4)
			{
				DisplayString(7,8,0,(u8 *)ConnectType[0]);//通信协议列表
			}
			else
			{
				DisplayString(7,8,0,(u8 *)ConnectType[p8[1]]);//通信协议列表
			}


			//采集器地址
			p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+6*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),3,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,6);
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
			y=MRR(ADDR_DATABUFF,6);
			DisplayData(y,12,0,p16);

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
							DisplayString(9,7,0,"确认设置");
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
							DisplayString(0,3,0,"电能表参数设置");
					#endif
					#if ((Project/100)==3)
							DisplayString(0,5,0,"电能表配置");
					#endif
					#if ((Project/100)==4)
							if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
							{
								DisplayString(0,3,0,"电能表参数设置");
							}
							else
							{
								DisplayString(0,5,0,"电能表配置");
							}
					#endif
						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//清屏幕显示的指定行(4个控制字不清)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,7,0,"确认设置");
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
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=4;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=6;
							MenuCount->v2=8;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=6;
							MenuCount->v2=17;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
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
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<(NUMmax_6000-2))
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
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//自适应
									i=0;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=1;
											break;
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
											i=8;
											break;
										case 38:
											i=9;
											break;
										case 57:
											i=10;
											break;
										case 11:
											i=11;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=1;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//交采
									i=2;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//载波
									i=0;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=3;
										if(x==2)
											i=4;
										break;
									default:
										break;
								}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)PortList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=1;
								}
								else if(p8[0]==0x41)//ABB
								{
									i=6;
								}
								else if(p8[0]==0x49)//IEC1107
								{
									i=0;
								}
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=2;
										if(x==20)
											i=3;
										break;
									case 69:
										i=4;
										break;
									case 18:
										//i=0;
										i=5;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
								p8_add1(p8,12);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_add1(p8,2);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xca&&p8[1]==0x5c)//未知
								{
									i=1;
								}
								else if(p8[0]==0x50&&p8[1]==0x4a)//单项
								{
									i=2;
								}
								else if(p8[8]==0xf4&&p8[9]==0x58)//三项三线
								{
									i=3;
								}
								else//三项四线
								{
									i=0;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ConnectType[i]);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_add1(p8,12);
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
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,(NUMmax_6000-1),4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//自适应
									i=10;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=11;
											break;
										case 60:
											i=0;
											break;
										case 12:
											i=1;
											break;
										case 24:
											i=2;
											break;
										case 48:
											i=3;
											break;
										case 72:
											i=4;
											break;
										case 96:
											i=5;
											break;
										case 19:
											i=6;
											break;
										case 38:
											i=7;
											break;
										case 57:
											i=8;
											break;
										case 11:
											i=9;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									i=4;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//交采
									i=0;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//载波
									i=3;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=1;
										if(x==2)
											i=2;
										break;
									default:
										break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)PortList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//未知
									//i=4;
									i=6;
								}
								else if(p8[0]==0x41)//ABB
								{
									i=4;
								}
								else if(p8[0]==0x49)//IEC1107
								{
									i=5;
								}
								
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=0;
										if(x==20)
											i=1;
										break;
									case 69:
										i=2;
										break;
									case 18:
										i=3;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
								p8_sub1(p8,12);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_sub1(p8,2);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xca&&p8[1]==0x5c)//未知
								{
									i=3;
								}
								else if(p8[0]==0x50&&p8[1]==0x4a)//单项
								{
									i=0;
								}
								else if(p8[8]==0xf4&&p8[9]==0x58)//三项三线
								{
									i=1;
								}
								else//三项四线
								{
									i=2;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ConnectType[i]);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_sub1(p8,12);
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
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,18,0,"输");
								break;
							case 9:
								DisplayClrRow_4(9);//清屏幕显示的指定行(4个控制字不清)
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>NUMmax_6000)
								{//测量点超过最大值 则不可设置
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,7,0,"无效测量点");
								}
								else
								{
									DisplayClrRow_4(9);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(9,9,0,"OK");

									p8=(u8*)ADDR_DATABUFF+1024;
									MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									if(p8[0]==0)//如果本来不存在该配置序号，就应该是新加的，采用默认配置
									{
										MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
									}
									else//存在，看下选择的序号和配置参数里的配置序号是否一致
									{
										p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
										k=p8[2]+p8[1]*0x100;
										if(k!=MenuCount->v3)
										{
											MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
										}
									}
									
									MWR(MenuCount->v3,ADDR_DATABUFF,2);
									Data_Inverse(ADDR_DATABUFF,2);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),1,0,LENper_6000,ADDR_DATABUFF,2);
									
									p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xa6&&p8[1]==0x63)
									{//自适应
										i=255;
										delete_pn+=1;
									}
									else
									{
										switch(x)
										{
											case 30:
												i=0;
												break;
											case 60:
												i=1;
												break;
											case 12:
												i=2;
												break;
											case 24:
												i=3;
												break;
											case 48:
												i=4;
												break;
											case 72:
												i=5;
												break;
											case 96:
												i=6;
												break;
											case 19:
												i=7;
												break;
											case 38:
												i=8;
												break;
											case 57:
												i=9;
												break;
											case 11:
												i=10;
												break;
											default:
												break;
										}
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,2,LENper_6000,ADDR_DATABUFF,1);
									
									p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//未知
										i=0;
										delete_pn+=1;
									}
									else if(p8[0]==0x5c&&p8[1]==0x50)
									{//交采
										i=0xf2080201;
									}
									else if(p8[0]==0x7a&&p8[1]==0x61)
									{//载波
										i=0xf2090201;
									}
									else
									{
										switch(x)
										{
										case 48:
											x=p8_bcd(p8+8,1);
											if(x==1)
												i=0xf2010201;
											if(x==2)
												i=0xf2010202;
											break;
										default:
											break;
										}
									}
								
									MWR(i,ADDR_DATABUFF,4);
									Data_Inverse(ADDR_DATABUFF,4);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,4,LENper_6000,ADDR_DATABUFF,4);
									
									p8=(u8 *)(ADDR_STRINGBUFF+4*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//未知
										i=0;
										delete_pn+=1;
									}
									else if(p8[0]==0x41)//ABB
									{
										i=5;
									}
									else if(p8[0]==0x49)//IEC1107
									{
										i=6;
									}
									else
									{
										switch(x)
										{
										case 64:
											x=p8_bcd(p8+8,2);
											if(x==19)
												i=1;
											if(x==20)
												i=2;
											break;
										case 69:
											i=3;
											break;
										case 18:
											i=4;
											break;
										default:
											break;
										}
										
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,3,LENper_6000,ADDR_DATABUFF,1);

									p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+6*2);
									x=p8_bcd(p8,2);
									MWR(x,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,6,LENper_6000,ADDR_DATABUFF,1);
									
									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+5*84+4+4*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),2,1,LENper_6000,ADDR_DATABUFF,6);

									i=0;
									p8=(u8 *)(ADDR_STRINGBUFF+7*84+4+8*2);
									if(p8[0]==0xca&&p8[1]==0x5c)//未知
									{
										i=0;
									}
									else if(p8[0]==0x50&&p8[1]==0x4a)//单项
									{
										i=1;
									}
									else if(p8[8]==0xf4&&p8[9]==0x58)//三项三线
									{
										i=2;
									}
									else if(p8[8]==0xb6&&p8[9]==0x5a)//三项四线
									{
										i=3;
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,8,LENper_6000,ADDR_DATABUFF,1);
																		

									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+8*84+4+6*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),3,1,LENper_6000,ADDR_DATABUFF,6);
									
									if(delete_pn==3)//隐藏功能启用
									{
										MC(0,(ADDR_DATABUFF+1024),LENper_6000);
										DisplayString(9,6,0,"删除成功");
									}
									MW((u32)(ADDR_DATABUFF+1024),ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									Terminal_Ram->SET6000=0x55;//采集档案配置表有设置标志:0=没设置,0x55=设置
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
								if(x>NUMmax_6000 || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+5*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节，判断内存数据如果有错则初始化
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,1,12);
								}
								break;
							case 6:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+(MenuCount->v1)*2);
								//通信地址BCD占6个字节，判断内存数据如果有错则初始化
								for(i=0;i<=1;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=2)
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
	//通信端口号
	/*
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
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,"	   ");			
			break;
	}*/
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_2
// Descriptions:        集中器时间设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;
	u32 x;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif(Project/100)==2&&(USER/100)==17//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_3);//显示菜单
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//日期
			MR(ADDR_DATABUFF,ADDR_DL698YMDHMS,2);
			Data_Inverse(ADDR_DATABUFF,2);//数据倒序(高低字节调换),入口Le>=2
			x=MRR(ADDR_DATABUFF,2);//年
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			x-=0x2000;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
			p[0*2]=((x>>4)&0xf)+0x30;
			p[1*2]=(x&0xf)+0x30;
			p[2*2]='-';
			x=MRR(ADDR_DL698YMDHMS+2,1);//月
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			p[3*2]=(x>>4)+0x30;
			p[4*2]=(x&0xf)+0x30;
			p[5*2]='-';
			x=MRR(ADDR_DL698YMDHMS+3,1);//日
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			p[6*2]=(x>>4)+0x30;
			p[7*2]=(x&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			//时间
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
			x=MRR(ADDR_DL698YMDHMS+4,1);//时
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			p[0*2]=(x>>4)+0x30;
			p[1*2]=(x&0xf)+0x30;
			p[2*2]=':';
			x=MRR(ADDR_DL698YMDHMS+5,1);//分
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			p[3*2]=(x>>4)+0x30;
			p[4*2]=(x&0xf)+0x30;
			p[5*2]=':';
			x=MRR(ADDR_DL698YMDHMS+6,1);//秒
			x=hex_bcd(x);//HEX数转为8位压缩BCD数
			p[6*2]=(x>>4)+0x30;
			p[7*2]=(x&0xf)+0x30;
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
						DisplayString(Ln+3,6,0,"确认设置");
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
							DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"输入");
						}
						else//if(Stack->MenuStack[MenuCount->Count].EditRow==3)
						{
							//保存设置的数据
							DisplayClrRow_4(Ln+3);//清屏幕显示的指定行(4个控制字不清)
							DisplayString(Ln+3,9,0,"OK");
							p8=(u8 *)(ADDR_RTCBUFF);
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
							p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//秒
							p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//分
							p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//时
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
							p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//日
							p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//月
							p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//年
//							WriteLPCRTC(ADDR_RTCBUFF);//写内部RTC
//							SyncLPCRTC();//同步内部RTC(读内部写外部)
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
// Function name:       Menu2_2_5
// Descriptions:        集中器地址设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_5(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u32 Ln;
	UARTCtrl_TypeDef * UARTCtrl;
	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_5);//显示菜单
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
			MenuCount->v1=4;
			MenuCount->v2=15;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//显示内存中数据
			//通信地址
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+(MenuCount->v1)*2);
				p8=(u8 *)ADDR_DATABUFF;
				MR(ADDR_DATABUFF,ADDR_4001+1,LENmax_4001-1);
				if(p8[0]>6)
				{
					p8[0]=6;
				}
				Data_Inverse(ADDR_DATABUFF+1,p8[0]);//数据倒序(高低字节调换),入口Le>=2
				x=MRR(ADDR_DATABUFF+1,p8[0]);
				DisplayData(x,12,0,p16);
			break;
		case 1://任务1:选择
			if(((USER/100) != 9)||(Comm_Ram->Factory==0x55))
			{//河南现场程序不能编辑终端地址
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
								MenuCount->v1=4;
								MenuCount->v2=15;
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									p8_add1(p8,12);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									p8_sub1(p8,12);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									//通信地址BCD占6个字节，判断内存数据如果有错则初始化
									for(i=0;i<=11;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
										{
											break;
										}
									}
									if(i!=12)
									{
										bcd_p8(p8,1,12);
									}
									break;
								case 2:
									DisplayClrRow_4(Ln+2);//清屏幕显示的指定行(4个控制字不清)
									DisplayString(Ln+2,9,0,"OK");
									//保存设置的数据
									p8=(u8 *)ADDR_DATABUFF;
									MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);//读通信地址
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+4*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									i=DataComp(ADDR_DATABUFF,ADDR_DATABUFF+1024+1,6);
									if((i!=0)||(p8[1024]!=6))//设置了新的地址
									{
										MWR(6,ADDR_4001+1,1);//写通信地址
										MW(ADDR_DATABUFF,ADDR_4001+2,6);//写通信地址
										UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
										UARTCtrl->ReNewLink=0x55;//0x55=通信参数变化当前帧发送完成后重新连接
										UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
										UARTCtrl->ReNewLink=0x55;//0x55=通信参数变化当前帧发送完成后重新连接
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
								case 1:
									p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'左右箭头'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+((Ln+1))*84+4+(MenuCount->v1)*2);
									//通信地址BCD占6个字节，判断内存数据如果有错则初始化
									for(i=0;i<=11;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30)
										{
											break;
										}
									}
									if(i!=12)
									{
										bcd_p8(p8,1,12);
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
// Function name:       Menu2_2_4
// Descriptions:        界面密码设置
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu2_2_4(void)
{
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//上海专变、黑龙江、吉林
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_4);//显示菜单
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
// Function name:       Menu2_2
// Descriptions:        集中器参数设置与查看
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void PassWordCreateEdit(void);//创建密码编辑框
void Menu2_2(void)//集中器参数设置与查看
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
		#if ((Project/100)==2)//集中器
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_Project2);//显示菜单
		#endif
		#if ((Project/100)==3)//专变
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_Project3);//显示菜单
		#endif
		#if ((Project/100)==4)//三合一
			if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_Project2);//显示菜单
			}
			else
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_Project3);//显示菜单
			}
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
//				#if (USER/100)==0//电科院测试
//					#if ((Project/100)==2)//集中器
//						if(Comm_Ram->Factory==0x55)
//						{//工厂状态
//							MenuCreate((u32)Menu2_2_1);//通信通道设置  
//						}
//						else
//						{
//							PassWordCreateEdit();//创建密码编辑框
//						}
//					#else
//						MenuCreate((u32)Menu2_2_1);//通信通道设置   
//					#endif
//				#else
						MenuCreate((u32)Menu2_2_1);//通信通道设置
//				#endif
						break;
					case 2:
#if (Project/100)==2&&(USER/100)==17 //江苏集中器
						MenuCount->v3=1;
						MenuCreate((u32)Menu2_2_2_JS);//电表参数设置
#else
						MenuCreate((u32)Menu2_2_2);//电表参数设置
#endif
						break;
					case 3:
						MenuCreate((u32)Menu2_2_3);//集中器时间设置
						break;
					case 4:
						MenuCreate((u32)Menu2_2_4);//界面密码设置
						break;
					case 5:
						MenuCreate((u32)Menu2_2_5);//集中器地址设置
						break;
				#if (USER/100)==15
					case 6:
						MenuCreate((u32)Menu2_2_6);//集中器地址设置
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
