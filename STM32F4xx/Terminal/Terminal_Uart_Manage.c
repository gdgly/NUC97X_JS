
//终端Uart管理
#include "Project.h"
#include "Terminal_Uart_Manage.h"
#include "UartAuto_645.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#include "Terminal_AFN0E_Event.h"
#include "Terminal_Contrl_Uart.h"
#include "Terminal_Uart_3761.h"
#include "Terminal_Uart_AFN02_Heartbeat.h"
#include "Terminal_Uart_AFN0F_File.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_Uart_3762_Manage.h"

#include "../Device/GPRS_3763.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/Ethernet.h"


	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif

void Link_Contrl(void)//上行通信模块在线控制
{
//注:在线控制变化时:心跳发出间隔秒定时器=0,心跳发出计数=0,使退出或登录帧马上发出
	u32 i;
	u8 * p8;
	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;

//命令终端断开连接
	i=MRR(ADDR_AFN05F39,1);
	if(i==0xaa)
	{
		i=0;
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
		if(Get_LEN_UARTnCtrl(ENETPORT))//得到UART控制数据长度
		{
			UARTCtrl->LinkCtrl=0;//连接控制0=不连接,1=连接
			if(UARTCtrl->LinkTask<101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				i=1;
			}
		}
		else
		{
			i=1;
		}
		if(Get_LEN_UARTnCtrl(GPRSPORT))//得到UART控制数据长度
		{
			UART1Ctrl->LinkCtrl=0;//连接控制0=不连接,1=连接
			if(UART1Ctrl->LinkTask<101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				i|=2;
			}
		}
		else
		{
			i|=2;
		}
		if(i==3)
		{
			MC(0,ADDR_AFN05F39,1);
		}
		return;
	}

#if IC_LAN8720==1
//以太网总是允许连接,且若连接则关无线模块
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
	if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
	{
		UART1Ctrl->LinkCtrl=0;//连接控制0=不连接,1=连接
		WarningOccur(GPRSPORT,(u8 *)"以太网在线无线关闭");//告警发生,入口告警代码和告警字符串
		return;
	}
#endif

//GPRS无线模块流量,永久在线模式、被动激活模式、时段在线模式
	if(Get_LEN_UARTnCtrl(GPRSPORT))//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
		//月通信流量
		MR(ADDR_DATABUFF,ADDR_AFN0CF10,9);
		p32=(u32 *)(ADDR_DATABUFF);
		i=MRR((u32)&UARTCtrl->RxTxByte,4);//通信流量
		if(i!=0x0)
		{
			MWR(0,(u32)&UARTCtrl->RxTxByte,4);//通信流量
			p32[0]+=i;
			p32[1]+=i;
			MW(ADDR_DATABUFF,ADDR_AFN0CF10,8);
		}
		i=MRR(ADDR_AFN04F36,4);
		if(i!=0x0)
		{//月通信流量门限为0，表示系统不需要终端进行流量控制
			p8=(u8 *)(ADDR_DATABUFF);
			if(p32[1]<i)
			{//月流量没超限
				//WarningCancel(GPRSPORT);//告警取消,入口告警代码
				if(p8[8]!=0x0)
				{
					MC(0,ADDR_AFN0CF10+8,1);
				}
			}
			else
			{//月流量超限
				//WarningOccur(GPRSPORT,(u8 *)"无线月通信流量超限");//告警发生,入口告警代码和告警字符串
				if(p8[8]==0x0)
				{//事件记录标志,0没记录,1=已记录
					if(ERC_Event_Set(32)!=0)//检查事件记录设置,返回0=不记录,1=记录
					{
						MC(1,ADDR_AFN0CF10+8,1);
					}
					ERC32_Event(p32[1],i);//通信流量超门限事件,n1=当月已发生的通信流量,n2=月通信流量门限
				}
				
				if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
				{
					MR(ADDR_DATABUFF,ADDR_AFN0CF7_FM+2,1);//读当前还没上传被确认重要事件计数
					if(p8[0]==0x0)
					{//有重要事件没上报,等上报完成后断开
						UARTCtrl->LinkCtrl=0;//连接控制0=不连接,1=连接
//						UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
//						Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
					}
				}
				else
				{
					UARTCtrl->LinkCtrl=0;//连接控制0=不连接,1=连接
//					UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
//					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
				}
				return;
			}
		}
		//被动激活
		i=MRR(ADDR_AFN04F8,1);
		i&=0x3;
		if(i==2)
		{//1～3依次表示永久在线模式、被动激活模式、时段在线模式
			i=MRR(ADDR_AFN05F38,1);
			if(i==0x55)
			{//收到被动激活命令,置初值重拨次数后命令变为0x5a
				MC(0x5A,ADDR_AFN05F38,1);
				i=MRR(ADDR_AFN04F8+3,1);
				if(i==0)
				{
					i=1;//被动激活模式重拨次数最小为1次
				}
				UARTCtrl->ReLinkNum=i;//被动激活模式重拨次数,收到被动激活命令时置初值
				return;
			}
			if(i==0x5A)
			{//0x55=激活终端连接主站,0xAA=命令终端断开连接,执行后清0
				WarningCancel(GPRSPORT);//告警取消,入口告警代码
				UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
				if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
				{
					if(Terminal_Ram->GPRSAutoDiscon_S_Timer==0)//4 GPRS被动激活模式连续无通信自动断线秒定时器
					{
						MC(0,ADDR_AFN05F38,1);//连续无通信自动断线清除激活命令0x55(0x5a)
					}
				}
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)"远程无线等待被动激活");//告警发生,入口告警代码和告警字符串
				UARTCtrl->LinkCtrl=0;//连接控制0=不连接,1=连接
//				UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
//				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
			}
			return;
		}
		//时段在线
		if(i==3)
		{//1～3依次表示永久在线模式、被动激活模式、时段在线模式
			p8=(u8 *)(ADDR_TYMDHMS+2);
			i=MRR(ADDR_AFN04F8+5,3);
			i>>=bcd_hex(p8[0]);//8位压缩BCD数转为HEX
			i&=0x1;
			if(i==0x0)
			{//当前时段不在线
				WarningOccur(GPRSPORT,(u8 *)"无线当前时段不在线");//告警发生,入口告警代码和告警字符串
				UARTCtrl->LinkCtrl=0;//连接控制0=不连接,1=连接
//				UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
//				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
			}
			else
			{//当前时段在线
				UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
			}
			return;
		}
		//永久在线
		UARTCtrl->LinkCtrl=1;//连接控制0=不连接,1=连接
	}
	
}



//初始化
#if (((Project/100)==0)||((Project/100)==1))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块 
void Terminal_UartInit_Manage(void)//Terminal_Uart初始化管理
{
}
#endif
#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块 
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=控制模块
//UART5=
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
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
//红外
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//初始化UART口,入口通信控制字已填入
	}
//控制模块(专用初始化)
	
}
#endif
#if (((Project/100)==2)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块 
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
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
#endif
#if ((Project/100)==5)||((Project/100)==6)//采集器,通信模块
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=红外
//UART5=
//UART6=
//UART7=终端ESAM通信口号
//UART8=
//UART9=
//UART10=以太网通信口号
//UART11=短信通信口号
//UART12=
void Terminal_UartInit_Manage(void)//Terminal_Uart初始化管理
{
  UARTCtrl_TypeDef * UARTCtrl;
//RS232
	if(Get_LEN_UARTnCtrl(RS232PORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
//红外
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//初始化UART口,入口通信控制字已填入
	}
}
#endif






//接收发送
//智能表,网络表----------------------------------------------------------------------------------
#if ((Project/100)==0)//智能表
void Terminal_Uart_Manage(void)//Terminal_Uart管理
{
}
#endif
#if ((Project/100)==1)//网络表
void Terminal_Uart_Manage(void)//Terminal_Uart管理
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
	//文件传输
		Request_File();//请求文件传输
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761接收发送
			UartAuto_645(RS232PORT);//自动识别DL_645帧
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761接收发送
			UartAuto_645(IRPORT);//自动识别DL_645帧
		}
	//RS485-1
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
	  if(Comm_Ram->Factory==0x55)//0x55=工厂模式
	  {
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
				UartAuto_645(RS485_1PORT);//自动识别DL_645帧
			}
		}
		UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
//	//RS485-2抄表
		UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
		UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
//		ReadMeter_RS485(RS485_2PORT);//RS485-2抄表
	//交流采样通信口号(虚拟)抄表
		ReadMeter_RS485(ACSAMPLEPORT);
	}
	
//上行通信模块在线控制
	Link_Contrl();//上行通信模块在线控制
//主动上报重要事件
	Terminal_Uart_AutoEvent();//主动上报重要事件
//主动上报定时任务
	Terminal_Uart_AutoTask();//主动上报定时任务
//ENET
#if IC_W5200==1
	W5200_3761_RxTx(ENETPORT);//W5200接收发送
	W5200_FTP(ENETPORT);//W5200 FTP文件下载
	Uart_3761_RxTx(ENETPORT);//UART_3761接收发送
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//主动心跳(链路接口检测)
#endif
//GPRS
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	i=Pin_Read(PIN_MODULSEL0);//读引脚,入口引脚使用名;返回0或1
	i|=Pin_Read(PIN_MODULSEL1)<<1;//读引脚,入口引脚使用名;返回0或1
	if(i==3)
	{//GPRS模块盒没插入
		//关模块电源和IO引脚
		All_GPRS_3763_PinIn();//全部GPRS模块引脚为输入无上拉
		Terminal_Ram->MODULSEL=i;//GPRS模块选择0=BGS2,1=,2=,3=空
		Terminal_Ram->Signal=0;//GPRS信号强度0-4
		UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=等待退出登录
		WarningOccur(GPRSPORT,(u8 *)"远程无线模块没插入");//告警发生,入口告警代码和告警字符串
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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//主动心跳(链路接口检测)
		}
	}
//

}
#endif
//智能表,网络表结束------------------------------------------------------------------------------



//专变终端,采集器,通信模块---------------------------------------------------------
#if (((Project/100)==3)||((Project/100)==5)||((Project/100)==6))//专变终端,采集器,通信模块
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=控制模块
//UART5=
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

	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
	//文件传输
		Request_File();//请求文件传输
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761接收发送
			UartAuto_645(RS232PORT);//自动识别DL_645帧
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761接收发送
			UartAuto_645(IRPORT);//自动识别DL_645帧
		}
	//RS485-1抄表
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
	  if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//0x55=工厂模式
	  {
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
				Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
			}
			UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			WarningOccur(RS485_1PORT,(u8 *)"485\x1终端维护");//告警发生,入口告警代码和告警字符串
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
				UartAuto_645(RS485_1PORT);//自动识别DL_645帧
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_1PORT);//RS485-1抄表
		}
	//RS485-2抄表
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
	  if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)//终端维护
	  {
		#if (USER/100)==5//用户标识:上海版
			if(MRR(ADDR_TerminalFMTimer+42,2)==0)//2 RS485-2功能转换分定时器
			{
				MWR(0xff,ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1);
			}
		#endif
		#if (USER/100)==10//河北系
			//河北维护口速率为9600
			if(Comm_Ram->SelfTestCommand!=0x55555555)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
			{
				if(UARTCtrl->BpsCtrl!=(0xb+(6<<5)))//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
				{
					UARTCtrl->BpsCtrl=(0xb+(6<<5));//RS485口默认的速率控制码
					Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
				}
			}
		#else
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
				Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
			}
		#endif
			UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			WarningOccur(RS485_2PORT,(u8 *)"485\x2终端维护");//告警发生,入口告警代码和告警字符串
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
				UartAuto_645(RS485_2PORT);//自动识别DL_645帧
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_2PORT);//RS485-2抄表
		}
	//交流采样通信口号(虚拟)抄表
		ReadMeter_RS485(ACSAMPLEPORT);
#if ((Project/100)==3)//专变终端,采集器,通信模块
	//控制模块
		Terminal_Contrl_Uart_RxTx(RS485_4PORT);//控制模块通信RxTx
#endif
	}
	
//上行通信模块在线控制
	Link_Contrl();//上行通信模块在线控制
//主动上报重要事件
	Terminal_Uart_AutoEvent();//主动上报重要事件
//主动上报定时任务
	Terminal_Uart_AutoTask();//主动上报定时任务
//ENET
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP文件下载
	Uart_3761_RxTx(ENETPORT);//UART_3761接收发送
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//主动心跳(链路接口检测)
	//AutoCascadingData(ENETPORT);//自动上传级联巡测到的数据
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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//主动心跳(链路接口检测)
			//AutoCascadingData(GPRSPORT);//自动上传级联巡测到的数据
		}
	}
//

}
#endif
//专变终端,采集器,通信模块---------------------------------------------------------------




//集中器,公变终端-------------------------------------------------------------------------------
#if (((Project/100)==2)||((Project/100)==4))//集中器,公变终端
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

	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
	//文件传输
		Request_File();//请求文件传输
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761接收发送
			UartAuto_645(RS232PORT);//自动识别DL_645帧
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761接收发送
			UartAuto_645(IRPORT);//自动识别DL_645帧
		}
	//RS485-1抄表
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
		i=MRR(ADDR_AFN04F37,1);//终端级联通信端口号 BIN  1 
		i|=MRR(ADDR_AFN04F37+6,1)<<8;//级联/被级联标志
	  if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//终端维护
	  {
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
				Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
			}
			UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			WarningOccur(RS485_1PORT,(u8 *)"485\x1终端维护");//告警发生,入口告警代码和告警字符串
			UARTCtrl->RMComplete|=0X80;//
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
				UartAuto_645(RS485_1PORT);//自动识别DL_645帧
			}
		}
		else
		{
//			if((i&0xff)==2)
//			{//级联口
//				if((i&0x8000)!=0)
//				{//被动级联
//					x=MRR(ADDR_AFN04F37+1,1);
//					if(UARTCtrl->BpsCtrl!=x)
//					{
//						UARTCtrl->BpsCtrl=x;
//						Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
//					}
//					Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
//					if(UARTCtrl->Task==1)
//					{//1=接收完成
//						Terminal_Ram->CD485RxLED_MS_Timer=100/10;//级联485接收LED指示MS定时器,每10MS减1定时器
//					}
//					if(UARTCtrl->Task==2)
//					{//2=等待单帧数据发送
//						Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
//					}
//				}
//				else
//				{//主动级联
//					Cascading_LoopChack(RS485_1PORT);//主动级联巡测
//				}
//			}
//			else
//			{//非级联口
				ReadMeter_RS485(RS485_1PORT);//RS485-1抄表
//			}
		}
	//RS485-2抄表
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
	  if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)//终端维护
	  {
	#if (USER/100)==5//用户标识:上海版
			if(MRR(ADDR_TerminalFMTimer+42,2)==0)//2 RS485-2功能转换分定时器
			{
				MWR(0xff,ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1);
			}
	#endif
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码
				Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
			}
			UARTCtrl->RMComplete=0x80;//D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
			WarningOccur(RS485_2PORT,(u8 *)"485\x2终端维护");//告警发生,入口告警代码和告警字符串
			if(UARTCtrl->Protocol==0)//通信规约自动识别:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
				UartAuto_645(RS485_2PORT);//自动识别DL_645帧
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_2PORT);//RS485-2抄表
		}
//		if((i&0xff)==3)
//		{//级联口
//			if((i&0x8000)!=0)
//			{//被动级联
//				x=MRR(ADDR_AFN04F37+1,1);
//				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
//				if(UARTCtrl->BpsCtrl!=x)
//				{
//					UARTCtrl->BpsCtrl=x;
//					Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
//				}
//				Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
//				if(UARTCtrl->Task==1)
//				{//1=接收完成
//					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//级联485接收LED指示MS定时器,每10MS减1定时器
//				}
//				if(UARTCtrl->Task==2)
//				{//2=等待单帧数据发送
//					Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
//				}
//			}
//			else
//			{//主动级联
//				Cascading_LoopChack(RS485_2PORT);//主动级联巡测
//			}
//		}
//		else
//		{//非级联口
//			ReadMeter_RS485(RS485_2PORT);//RS485-2抄表
//		}
	//RS485-3
//		if((i&0xff)==4)
//		{//级联口
//			if((i&0x8000)!=0)
//			{//被动级联
//				x=MRR(ADDR_AFN04F37+1,1);
//				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_3PORT));
//				if(UARTCtrl->BpsCtrl!=x)
//				{
//					UARTCtrl->BpsCtrl=x;
//					Init_UARTn(RS485_3PORT);//初始化UART口,入口通信控制字已填入
//				}
//
//				Uart_3761_RxTx(RS485_3PORT);//UART_3761接收发送
//				if(UARTCtrl->Task==1)
//				{//1=接收完成
//					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//级联485接收LED指示MS定时器,每10MS减1定时器
//				}
//				if(UARTCtrl->Task==2)
//				{//2=等待单帧数据发送
//					Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
//				}
//			}
//			else
//			{//主动级联
//				Cascading_LoopChack(RS485_3PORT);//主动级联巡测
//			}
//		}
//		else
//		{//非级联口
//			ReadMeter_RS485(RS485_3PORT);//RS485-3抄表
//		}
	//载波抄表
#if ((Project/100)==2)//集中器
		Terminal_uart_3762_Manage();//376.2路由模块管理
#endif
	//交流采样通信口号(虚拟)抄表
		ReadMeter_RS485(ACSAMPLEPORT);
	}
	
//上行通信模块在线控制
	Link_Contrl();//上行通信模块在线控制
//主动上报重要事件
	Terminal_Uart_AutoEvent();//主动上报重要事件
//主动上报定时任务
	Terminal_Uart_AutoTask();//主动上报定时任务
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
	//精确对时
	if(MRR(ADDR_AFN04F297_Ext,1)!=0)
	{//终端精确对时开关开启
		i=MRR(ADDR_AFN04F297_Ext+2,2);
		i<<=8;
		i=Check_HMS(i);//时分秒合法检查,返回0合法,1非法
		if(i==0)
		{//时钟合法
			i=DataComp(ADDR_AFN04F297_Ext+2,ADDR_TYMDHMS+1,2);
			if((i==0)||(i==2))//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{
				MWR(0,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
				if(MRR(ADDR_AFN04F297_Ext+7,1))
				{//允许对时次数不为0
					i=MRR(ADDR_AFN04F299_Ext+28,1);
					if(i==0)
					{
						if(MRR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,6)==0)
						{
							//精确对时开始
							i=0x80;
							MWR(i,ADDR_AFN04F299_Ext+28,1);
						}
						else
						{
							MC(0,ADDR_DATABUFF,16);
							MR(ADDR_DATABUFF+3,ADDR_AFN04F299_Ext+28+1+4*25+4+1+3,3);
							MR(ADDR_DATABUFF+9,ADDR_TYMDHMS+3,3);
							p8=(u8 *)(ADDR_DATABUFF);
							p8[4]&=0x1f;//去加在月字节上的星期
							p8[10]&=0x1f;//去加在月字节上的星期
							i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
							if(i==0)
							{//时钟合法
								i=TimeDifference((u32)ADDR_DATABUFF,(u32)(ADDR_DATABUFF+6));//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
								//秒化为天
								i/=60;
								i/=60;
								i/=24;
								if(i>=MRR(ADDR_AFN04F297_Ext+1,1))
								{
									//精确对时开始
									i=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28,1);
								}
							}
						}
					}
				}
			}
		}
		i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
		if(i&0x7f)
		{//对过时
			if((i&0x80)==0)
			{//失败
				if(MRR(ADDR_AFN04F297_Ext+7,1)>(i&0x7f))
				{//允许对时次数
					MR(ADDR_DATABUFF,(u32)(ADDR_AFN04F299_Ext+28+1+4*25+4+1),6);
					MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
					p8=(u8 *)ADDR_DATABUFF;
					p8[4]&=0x1f;//去加在月字节上的星期
					p8[10]&=0x1f;//去加在月字节上的星期
					i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
					if(i==0)
					{//时钟合法
						i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
						if(i>=(MRR(ADDR_AFN04F297_Ext+6,1)*60))
						{
							i=MRR(ADDR_AFN04F297_Ext+4,2);
							i<<=8;
							i=Check_HMS(i);//时分秒合法检查,返回0合法,1非法
							if(i==0)
							{//时钟合法
								if(DataComp(ADDR_AFN04F297_Ext+4,ADDR_TYMDHMS+1,2)==1)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
								{
									//精确对时开始
									i=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28,1);
									i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
									i|=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
								}
							}
						}
					}
				}
			}
		}
		i=MRR(ADDR_AFN04F299_Ext+28,1);
		if(i&0x80)
		{
			if(Terminal_Ram->PowerTask)//停电事件任务
			{
				if((i&0x7f)<=24)
				{
					Terminal_AutoAFN0AF299(GPRSPORT);//辽宁、浙江扩展AFN0AF299终端请求主站时钟
					Terminal_AutoAFN0AF299(ENETPORT);//辽宁、浙江扩展AFN0AF299终端请求主站时钟
				}
				else
				{
					MWR(i,ADDR_AFN04F299_Ext+28,1);
				}
			}
		}
	}
	else
	{
		MWR(0,ADDR_AFN04F299_Ext+28,1);
		MWR(0,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
	}
#endif
	
//ENET
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP文件下载
	Uart_3761_RxTx(ENETPORT);//UART_3761接收发送
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//主动心跳(链路接口检测)
	//AutoCascadingData(ENETPORT);//自动上传级联巡测到的数据

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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//主动心跳(链路接口检测)
			//AutoCascadingData(GPRSPORT);//自动上传级联巡测到的数据
		}
	}
//

}
#endif
//集中器,公变终端结束---------------------------------------------------------------------------















