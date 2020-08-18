
//终端Uart管理
#include "Project698.h"
#include "Terminal698_Uart_Manage.h"
#include "UartAuto_645.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#include "Terminal_Contrl_Uart.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_LINK.h"

#include "../DL698/RM_RS485_Manage.h"
#include "../DL698/RM_3762_Manage.h"

#include "../Device/GPRS_3763.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/Ethernet.h"
#include "../DL698/DL698TEST.h"
//#include "../MS/MS.h"


	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif

void Link_Contrl(void)//上行通信模块在线控制
{
//注:在线控制变化时:心跳发出间隔秒定时器=0,心跳发出计数=0,使退出或登录帧马上发出
//	u32 i;
//	u8 * p8;
//	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;
	
	

#if ((IC_LAN8720==1)||(IC_IP101==1))
//以太网总是允许连接,且若连接则关无线模块
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
//	if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
//	{
//		UART1Ctrl->LinkCtrl=0;//连接控制0=不连接,1=连接
//		WarningOccur(GPRSPORT,(u8 *)"以太网在线无线关闭");//告警发生,入口告警代码和告警字符串
//		return;
//	}
#endif
//GPRS无线模块流量,永久在线模式、被动激活模式、时段在线模式
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	//永久在线
	UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
	
}



//初始化
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=载波
//UART5=RS485-3
//UART6=红外
//UART7=终端ESAM通信口号
//UART8=
//UART9=
//UART10=以太网通信口号
//UART11=短信通信口号
//UART12=交流采样通信口号(虚拟),使用DATABUFF
void Terminal_UartInit_Manage(void)//Terminal_Uart初始化管理
{
  UARTCtrl_TypeDef * UARTCtrl;
//RS232
	if(Get_LEN_UARTnCtrl(RS232PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
	#if DEBUG_PORTtoRS232==1
		UARTCtrl->BpsCtrl=0xb+(10<<5);//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	#else
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	#endif
	  Init_UARTn(RS232PORT);//初始化UART口,入口通信控制字已填入
	}
//GPRS

//RS485-1
	if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
	}
//RS485-2
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
	}
//RS485-3
	if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_3PORT);//初始化UART口,入口通信控制字已填入
	}
//RS485-4(载波)
	if(Get_LEN_UARTnCtrl(RS485_4PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
		UARTCtrl->BpsCtrl=0xb+(6<<5);
	  Init_UARTn(RS485_4PORT);//初始化UART口,入口通信控制字已填入
	}
//红外
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//初始化UART口,入口通信控制字已填入
	}
}


//接收发送
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=载波
//UART5=RS485-3
//UART6=红外
//UART7=终端ESAM通信口号
//UART8=
//UART9=
//UART10=以太网通信口号
//UART11=短信通信口号
//UART12=交流采样通信口号(虚拟),使用DATABUFF
void Terminal_Uart_Manage(void)//Terminal_Uart管理
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
	u8 *p8;
#endif
// #ifdef LINUX
// ms_Type * ms;
// ms=Get_ms();
//#endif
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_698_RxTx(RS232PORT);//UART_698接收发送
			UartAuto_645(RS232PORT);//自动识别DL_645帧
		}
//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_698_RxTx(IRPORT);//UART_698接收发送
			UartAuto_645(IRPORT);//自动识别DL_645帧
		}
		
//交流采样通信口号(虚拟)抄表
		RM_RS485_Manage(ACSAMPLEPORT);

#if RUN_DL698TEST_1==0//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度
//RS485-1
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
		if(UARTCtrl->FunctionTimer!=Comm_Ram->RTCBuff[0])
		{
			UARTCtrl->FunctionTimer=Comm_Ram->RTCBuff[0];
			if(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0)//终端维护
			{
				UARTCtrl->Function=0;//口功能 0=终端维护口,!=0终端抄表口
				if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
				{
					UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
					Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
				}
			}
			else
			{
				UARTCtrl->Function=1;//口功能 0=终端维护口,!=0终端抄表口
			}
		}
		if(UARTCtrl->Function==0)//口功能 0=终端维护口,!=0终端抄表口
		{
			WarningOccur(RS485_1PORT,(u8 *)"485\x1终端维护");//告警发生,入口告警代码和告警字符串
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_698_RxTx(RS485_1PORT);//UART_698接收发送
				UartAuto_645(RS485_1PORT);//自动识别DL_645帧
			}
		}
		else
		{
			RM_RS485_Manage(RS485_1PORT);//RS485-1抄表
		}

//RS485-2
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
		if(UARTCtrl->FunctionTimer!=Comm_Ram->RTCBuff[0])
		{
			if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0))//终端维护
			{
				UARTCtrl->Function=0;//口功能 0=终端维护口,!=0终端抄表口
				if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)
				{
					UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
					Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
				}
			}
			else
			{
				UARTCtrl->Function=1;//口功能 0=终端维护口,!=0终端抄表口
			}
		}
		if(UARTCtrl->Function==0)//口功能 0=终端维护口,!=0终端抄表口
		{
				WarningOccur(RS485_2PORT,(u8 *)"485\x2终端维护");//告警发生,入口告警代码和告警字符串
				if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
				{
					Uart_698_RxTx(RS485_2PORT);//UART_698接收发送
					UartAuto_645(RS485_2PORT);//自动识别DL_645帧
				}
		}
		else
		{
			RM_RS485_Manage(RS485_2PORT);//RS485-2抄表
		}
//载波抄表
	#if ((Project/100)==2)//集中器
		RM_3762_Manage();//376.2路由模块管理
	#endif
	#if ((Project/100)==3)//专变
//控制模块
		Terminal_Contrl_Uart_RxTx(RS485_4PORT);//控制模块通信RxTx
	#endif
	#if ((Project/100)==4)//三合一终端
		Pin_Configure(PIN_PLC_ID,//模块插入引脚,输入上拉
								0|//B3-B0:复用功能选择AF(0-15)
					 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
		if(Pin_Read(PIN_PLC_ID)==0)
		{
			if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
			{//载波抄表
				RM_3762_Manage();//376.2路由模块管理
			}
			else
			{//控制模块
				Terminal_Contrl_Uart_RxTx(RS485_4PORT);//控制模块通信RxTx
			}
		}
		else
		{
			Comm_Ram->MODE3in1=0;//三合一终端当前运行模式:0=专变,1=集中器
			UARTCtrl->LinkTask=0;
			WarningOccur(RS485_4PORT,(u8 *)"控制或载波模块没插入");//告警发生,入口告警代码和告警字符串
		}
	#endif
#endif//#if RUN_DL698TEST_1==0//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度

	}

//上行通信模块在线控制
	Link_Contrl();//上行通信模块在线控制
//主动上报重要事件
//	Terminal_Uart_AutoEvent();//主动上报重要事件
//主动上报定时任务
//	Terminal_Uart_AutoTask();//主动上报定时任务


//ENET

//#warning "too much time start"
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP文件下载
	Uart_698_RxTx(ENETPORT);//UART_698接收发送
	LINK_request(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);
	//AutoCascadingData(ENETPORT);//自动上传级联巡测到的数据
//#warning "too much time end"


//GPRS
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	i=Pin_Read(PIN_GPRS_ID0);//读引脚,入口引脚使用名;返回0或1
	i|=Pin_Read(PIN_GPRS_ID1)<<1;//读引脚,入口引脚使用名;返回0或1
	if(i==3)
	{//GPRS模块盒没插入
		//关模块电源和IO引脚
		UnInit_GPRS_3763();//GPRS模块引脚为输入无上拉
		Terminal_Ram->MODULSEL=i;//GPRS模块选择0=,1=,2=,3=空
		Terminal_Ram->Signal=0;//GPRS信号强度0-4
		UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
		WarningOccur(GPRSPORT,(u8 *)"远程无线模块没插入");//告警发生,入口告警代码和告警字符串
	
//#ifdef LINUX
//		#undef  RJ45SELECT
//		#define RJ45SELECT     0//浠ュお缃慠J45閫氶亾閫夋嫨:0=eth0(IP101),1=eth1(LAN8720),2=eth0鍜宔th1浜屼釜鐙珛鍙?eth0鐢ㄤ簬浠ュお缃慠J45閫氶亾
//	 #if RJ45SELECT==0//浠ュお缃慠J45閫氶亾閫夋嫨:0=eth0(IP101),1=eth1(LAN8720),2=eth0鍜宔th1浜屼釜鐙珛鍙?eth0鐢ㄤ簬浠ュお缃慠J45閫氶亾
//		 ms->link_system("/sbin/ifconfig eth0 down");
//	 #elif RJ45SELECT==1
//		 ms->link_system("/sbin/ifconfig eth1 down");
//	 #elif RJ45SELECT==2
//		 ms->link_system("/sbin/ifconfig eth0 down");
//		 ms->link_system("/sbin/ifconfig eth1 down");
//	 #endif
//#endif
	}
	else
	{//GPRS模块盒插入
		if(i!=Terminal_Ram->MODULSEL)
		{//GPRS模块变化
			WarningCancel(GPRSPORT);//告警取消,入口告警代码
			Terminal_Ram->MODULSEL=i;//GPRS模块选择0=BGS2,1=,2=,3=空
			UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
		}
		else
		{
			switch(i)
			{
				case 0://无线GPRS模块BGS2
					GPRS_3763_RxTx();//GPRS_3763模块接收发送;不响应AT指令
					break;
				case 1://
					//CDMA_MC323_RxTx();//CDMA_MC323模块接收发送
					GPRS_3763_RxTx();//GPRS_3763模块接收发送;响应AT指令
					break;
				case 2://
					GPRS_3763_RxTx();//GPRS_3763模块接收发送;响应AT指令
					break;
			}
			LINK_request(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);
			//AutoCascadingData(GPRSPORT);//自动上传级联巡测到的数据
			//启动PPP连接
			
		}
	}
//

}
















