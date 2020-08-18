
//无线GPRS模块 1376.3标准
#if MainProtocol==376
#include "../Head/Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../terminal/Terminal_USER11xx.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_DataDef.h"
#endif

#include "../Device/GPRS_3763.h"
#include "../Device/ATNo.h"
#include "../Terminal/ATString.h"
#include "../Device/GPRS_FTP_RxTx.h"
#include "../Device/GPRS_SMS_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../TEST/LOG.h"
#include "../MS/MS.h"
#if (USER/100)==14
#include "../DL698/DL698_FJ_TASKM_USER14xx.h"
#endif

#define DEBUG_GPRS    0////0=发行,1=调试

#define F_DUPFD		0	/* Duplicate file descriptor.  */
#define F_GETFD		1	/* Get file descriptor flags.  */
#define F_SETFD		2	/* Set file descriptor flags.  */
#define F_GETFL		3	/* Get file status flags.  */
#define F_SETFL		4	/* Set file status flags.  */

#define EINTR       4     /* Interrupted system call*/
#define EAGAIN      11    /* Try again*/
#define EWOULDBLOCK EAGAIN/* Operation would block */
#define	ENOTSOCK    88    /* Socketoperation on non-socket */
#define	EADDRINUSE  98    /* Address already in use */
#define ECONNRESET  104    /*Connection reset by peer */
#define EISCONN     106   /*Transport endpoint is already connected*/
#define ETIMEDOUT   110   /*Connection timed out*/            // 连接超时
#define EALREADY    114   /*Operation already in progress*/   // 操作已在进程中
#define	EINPROGRESS	115	  /* Operation now in progress */

#define SOL_SOCKET	1
#define SO_REUSEADDR	2
#define SO_BINDTODEVICE	25

#define NUM_ReATTx     1//AT重发次数(实际发送次数=重发次数+1)

//SocketID 0-5使用约定
//0 服务器模式下，客户机可能的侦听socket,
//1 服务器模式下，客户机可能的侦听socket
//2 侦听客户机连接
//3 主连接
//4 FTP控制
//5 FTP数据
//3-5 未用
//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket
#define TCP_0_OPEN 0x80  //TCP clinet  socket_open
#define TCP_0 0x40       //TCP clinet link
#define TCP_1_LISTEN_OPEN 0x20  //TCP server  socket_open
#define TCP_1 0x10       //TCP server link
#define TCP_SELECT_SERVER 0x08  //SELECT TCP_clinet or TCP_server
#define FORCE_TCP_1 0x04 //FORCE_TCP_server
#define TCP_1_SOCKET_MASK 0x3 //TCP server  socket mask

#define TCP_0_SOCKET 0x3 //TCP clinet  socket
#define TCP_1_LISTEN 2   //TCP server  listen
#if (PPP_ENABLE==1)
void PPP_RxTx(void);
void Myprint_ATRx(void);
void log_out(u32  ADDR,u32 Byte, char x);
u32 Search_String(u32 ADDR_String,u32 ADDR_T,u32 Len);
u32 Check_pppd_exist(void);//存在返回1，不存在返回0
#endif

#define GPRSNETTOTALCOUNT 3
#define GPRSNETMAINIPCOUNT 1
#define GPRSNETOPENCOUNT 3
#define GPRSAUTOTODISSCON_S_Timer 120//23 GPRS被动激活模式连续无通信自动断线秒定时器,用于无通讯关闭socket定时
#define GPRSWait_SRVST_Timer 240    //等待模块上报^SRVST: 2
__asm void GPRS_3763_AT_Command_Str(void)
{
//波特率检查
ADDR_ATStr_AT
	dcb "AT\r",0
ADDR_ERRStr_AT
	dcb "AT指令无响应",0
	
	//模块信息
//关回显且串口检查
ADDR_ATStr_ATE0
	dcb "ATE0\r",0
ADDR_ERRStr_ATE0
	dcb "没检测到远程无线模块",0

//查询远程通信模块类型
ADDR_ATStr_MYTYPE
	dcb "AT$MYTYPE?\r",0
ADDR_ERRStr_MYTYPE
	dcb "查询远程无线类型失败",0

//获取模块软件版本
ADDR_ATStr_MYGMR
	dcb "AT$MYGMR\r",0
ADDR_ERRStr_MYGMR
	dcb "读远程无线版本失败",0

//卡检查
ADDR_ATStr_CPIN
	dcb "AT+CPIN?\r",0
ADDR_ERRStr_CPIN
	dcb "远程无线无SIM/UIM卡",0

//主动上报
#if (USER/100)==17//江苏系 开启主动上报
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=1\r",0
#else //关主动上报
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=0\r",0
#endif
ADDR_ERRStr_MYNETURC
	dcb "关主动上报失败",0

//获取SIM卡序列号ICCID
ADDR_ATStr_MYCCID
	dcb "AT$MYCCID\r",0
ADDR_ERRStr_MYCCID
	dcb "读卡CCID失败",0

//查国际移动台标识请求IMSI
ADDR_ATStr_CIMI
	dcb "AT+CIMI\r",0
ADDR_ERRStr_CIMI
	dcb "查国际移动标识失败",0

/*
//读取SIM卡服务运营商名称
ADDR_ATStr_CSPN
	dcb "AT+CSPN?\r",0
ADDR_ERRStr_CSPN
	dcb "读卡运营商名称失败",0
*/

	//连接设置
//读信号没连接前
ADDR_ATStr_CSQ_NOLINK
	dcb "AT+CSQ\r",0
ADDR_ERRStr_CSQ_NOLINK
	dcb "远程无线读信号失败",0

//获取当前网络注册状态
ADDR_ATStr_CREG
#if (USER/100)==13//黑龙江
	dcb "AT+CGREG?\r",0
#else
	dcb "AT+CREG?\r",0
#endif
ADDR_ERRStr_CREG
	dcb "获取网络注册状态失败",0

//读取SIM卡服务运营商名称
#if (USER/100)!=0//不是电科院测试(1376.3标准无此命令)
ADDR_ATStr_COPS2
	dcb "AT+COPS=0,2\r",0
ADDR_ERRStr_COPS2
	dcb "AT+COPS=2失败",0
ADDR_ATStr_COPS
	dcb "AT+COPS?\r",0
ADDR_ERRStr_COPS
	dcb "读卡运营商名称失败",0
#endif

//取本机号码
ADDR_ATStr_CNUM
	dcb "AT+CNUM\r",0
ADDR_ERRStr_CNUM
	dcb "读本机号码失败",0
	
//查询注册网络类型：3G/4G
ADDR_ATStr_SYSINFO
	dcb "AT$MYSYSINFO\r",0
ADDR_ERRStr_SYSINFO
	dcb "查询是否注册上4G网络",0


//设置APN
#if PPP_ENABLE==1
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=1,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "设置APN失败",0

//设置USERPWD:用户名和密码
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=1,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "设置USERPWD失败",0
#else
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=0,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "设置APN失败",0

//设置USERPWD:用户名和密码
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=0,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "设置USERPWD失败",0
#endif


//激活网络连接
ADDR_ATStr_MYNETACT
	dcb "AT$MYNETACT=0,1\r",0
ADDR_ERRStr_MYNETACT
	dcb "激活网络连接失败",0

//IP访问控制配置
ADDR_ATStr_MYIPFILTER
	dcb "AT$MYIPFILTER=0,1,",0
ADDR_ERRStr_MYIPFILTER
	dcb "IP访问配置失败",0
	
//设置短消息模式(0=PDU,1=TEXT)
ADDR_ATStr_CMFG
 #if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
	dcb "AT+CMGF=0\r",0
 #else
	dcb "AT+CMGF=1\r",0
 #endif
ADDR_ERRStr_CMFG
	dcb "设置短消息模式失败",0

//打开短信功能(仅中兴CDMA(MC8332)时需要)
ADDR_ATStr_ZCVF
	dcb "AT+ZCVF=2\r",0
ADDR_ERRStr_ZCVF
	dcb "打开短信功能失败",0

//休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
ADDR_ATStr_CTA
	dcb "AT+CTA=10\r",0
ADDR_ERRStr_CTA
	dcb "设置休眠等待时间失败",0



	//没连接循环开始
//设置服务参数
ADDR_ATStr_MYNETSRV
	dcb 0
ADDR_ERRStr_MYNETSRV
	dcb "设置服务参数失败",0
//开启服务
ADDR_ATStr_MYNETOPEN
	dcb "AT$MYNETOPEN=3\r",0
ADDR_ERRStr_MYNETOPEN
	dcb "远程无线开启服务失败",0
//短信服务没连接前
//读信号没连接2
//AT循环,没连接

	//连接检查
ADDR_ATStr_MYNETOPEN_LINK
	dcb "AT$MYNETOPEN?\r",0
ADDR_ERRStr_MYNETOPEN_LINK
	dcb "远程无线网络被断开",0
	//数据收发
//读取数据
ADDR_ATStr_MYNETREAD
	dcb 0
ADDR_ERRStr_MYNETREAD
	dcb "远程无线读数据失败",0
//发送数据
ADDR_ATStr_MYNETWRITE
	dcb "AT$MYNETWRITE=3,",0
ADDR_ERRStr_MYNETWRITE
	dcb "远程无线发送数据失败",0
//短信服务
	

//HOLD暂停
ADDR_ATStr_HOLD
	dcb 0
//关闭连接
ADDR_ATStr_MYNETCLOSE
	dcb "AT$MYNETCLOSE=3\r",0
ADDR_ERRStr_MYNETCLOSE
	dcb "远程无线关闭连接失败",0
//关机
ADDR_ATStr_MYPOWEROFF
	dcb "AT$MYPOWEROFF\r",0
ADDR_ERRStr_MYPOWEROFF
	dcb "远程无线模块关机失败",0
	
//关闭网络连接
ADDR_ATStr_MYNETACTOFF
	dcb "AT$MYNETACT=0,0\r",0
ADDR_ERRStr_MYNETACTOFF
	dcb "关闭网络连接失败",0
	
ADDR_ATStr_CGDCONT
	dcb "AT+CGDCONT=1,",'"',"IP",'"',',','"',0
ADDR_ERRStr_CGDCONT
	dcb "设置isp接入网关失败",0
	
ADDR_ATStr_ATDT
	dcb "ATDT*",0
ADDR_ERRStr_ATDT
	dcb "设置接入号码失败",0
	
	ALIGN 4	
};





__asm void GPRS_3763_AT_Command_Lib(void)//AT顺序执行库
{
//波特率检查
	dcd AT_AT
	dcd ADDR_ATStr_AT//AT命令字符串地址
	dcd ADDR_ERRStr_AT//错误提示字符串地址
	dcd 4000//AT命令返回第1个字符定时(ms)

//关回显
	dcd AT_ATE0
	dcd ADDR_ATStr_ATE0//AT命令字符串地址
	dcd ADDR_ERRStr_ATE0//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//查询远程通信模块类型
	dcd AT_MYTYPE
	dcd ADDR_ATStr_MYTYPE//AT命令字符串地址
	dcd ADDR_ERRStr_MYTYPE//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//获取模块软件版本
	dcd AT_MYGMR
	dcd ADDR_ATStr_MYGMR//AT命令字符串地址
	dcd ADDR_ERRStr_MYGMR//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//卡检查
	dcd AT_CPIN
	dcd ADDR_ATStr_CPIN//AT命令字符串地址
	dcd ADDR_ERRStr_CPIN//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//关主动上报
	dcd AT_MYNETURC
	dcd ADDR_ATStr_MYNETURC//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETURC//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//获取SIM卡序列号ICCID
	dcd AT_MYCCID
	dcd ADDR_ATStr_MYCCID//AT命令字符串地址
	dcd ADDR_ERRStr_MYCCID//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)

//查国际移动台标识请求IMSI
	dcd AT_CIMI
	dcd ADDR_ATStr_CIMI//AT命令字符串地址
	dcd ADDR_ERRStr_CIMI//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//读取SIM卡服务运营商名称
/*
	dcd AT_CSPN
	dcd ADDR_ATStr_CSPN//AT命令字符串地址
	dcd ADDR_ERRStr_CSPN//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
*/

//读信号没连接前
	dcd AT_CSQ_NOLINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT命令字符串地址
	dcd ADDR_ERRStr_CSQ_NOLINK//错误提示字符串地址
	#if PPP_ENABLE==1
	dcd 4000//AT命令返回第1个字符定时(ms)
	#else
	dcd 1000//AT命令返回第1个字符定时(ms)
	#endif

//获取当前网络注册状态
	dcd AT_CREG
	dcd ADDR_ATStr_CREG//AT命令字符串地址
	dcd ADDR_ERRStr_CREG//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
	
#if (USER/100)!=0//不是电科院测试(1376.3标准无此命令)
	dcd AT_COPS2
	dcd ADDR_ATStr_COPS2//AT命令字符串地址
	dcd ADDR_ERRStr_COPS2//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

	dcd AT_COPS
	dcd ADDR_ATStr_COPS//AT命令字符串地址
	dcd ADDR_ERRStr_COPS//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
#endif

//取本机号码
	dcd AT_CNUM
	dcd ADDR_ATStr_CNUM//AT命令字符串地址
	dcd ADDR_ERRStr_CNUM//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
	
//查询注册网络类型：2G/3G/4G
	dcd AT_SYSINFO
	dcd ADDR_ATStr_SYSINFO//AT命令字符串地址
	dcd ADDR_ERRStr_SYSINFO//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//设置APN(上紧接AT_SYSINFO不分开)
	dcd AT_MYNETCON_APN
	dcd ADDR_ATStr_MYNETCON_APN//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETCON_APN//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
//设置USERPWD:用户名和密码
	dcd AT_MYNETCON_USERPWD
	dcd ADDR_ATStr_MYNETCON_USERPWD//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETCON_USERPWD//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//激活网络连接
	dcd AT_MYNETACT
	dcd ADDR_ATStr_MYNETACT//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETACT//错误提示字符串地址
	dcd 90000//AT命令返回第1个字符定时(ms)

//IP访问控制配置
	dcd AT_MYIPFILTER
	dcd ADDR_ATStr_MYIPFILTER//AT命令字符串地址
	dcd ADDR_ERRStr_MYIPFILTER//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//设置短消息模式
	dcd AT_CMFG
	dcd ADDR_ATStr_CMFG//AT命令字符串地址
	dcd ADDR_ERRStr_CMFG//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//打开短信功能(仅中兴CDMA(MC8332)时需要)
	dcd AT_ZCVF
	dcd ADDR_ATStr_ZCVF//AT命令字符串地址
	dcd ADDR_ERRStr_ZCVF//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
	dcd AT_CTA
	dcd ADDR_ATStr_CTA//AT命令字符串地址
	dcd ADDR_ERRStr_CTA//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)


	//没连接循环开始
//设置服务参数
	dcd AT_MYNETSRV
	dcd ADDR_ATStr_MYNETSRV//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETSRV//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
//开启服务
	dcd AT_MYNETOPEN
	dcd ADDR_ATStr_MYNETOPEN//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETOPEN//错误提示字符串地址
	dcd 90000//AT命令返回第1个字符定时(ms)
//短信服务没连接前
	dcd AT_SMS_NOLINK
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 6000//AT命令返回第1个字符定时(ms)
//读信号没连接2
	dcd AT_CSQ_NOLINK2
	dcd ADDR_ATStr_CSQ_NOLINK//AT命令字符串地址
	dcd ADDR_ERRStr_CSQ_NOLINK//错误提示字符串地址
	#if PPP_ENABLE==1
	dcd 4000//AT命令返回第1个字符定时(ms)
	#else
	dcd 1000//AT命令返回第1个字符定时(ms)
	#endif
//AT循环,没连接
	dcd AT_LOOP_NOLINK
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 0//AT命令返回第1个字符定时(ms)


//读信号连接后
	dcd AT_CSQ_LINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT命令字符串地址
	dcd ADDR_ERRStr_CSQ_NOLINK//错误提示字符串地址
	#if PPP_ENABLE==1
	dcd 4000//AT命令返回第1个字符定时(ms)
	#else
	dcd 1000//AT命令返回第1个字符定时(ms)
	#endif
//查询注册网络类型：2G/3G/4G
	dcd AT_SYSINFO_LINK
	dcd ADDR_ATStr_SYSINFO//AT命令字符串地址
	dcd ADDR_ERRStr_SYSINFO//错误提示字符串地址
	#if PPP_ENABLE==1
	dcd 4000//AT命令返回第1个字符定时(ms)
	#else
	dcd 1000//AT命令返回第1个字符定时(ms)
	#endif
//查询网络连接
	dcd AT_MYNETOPEN_LINK
	dcd ADDR_ATStr_MYNETOPEN_LINK//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETOPEN_LINK//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//读取数据
	dcd AT_MYNETREAD
	dcd ADDR_ATStr_MYNETREAD//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETREAD//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
//发送数据
	dcd AT_MYNETWRITE
	dcd ADDR_ATStr_MYNETWRITE//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETWRITE//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
//FTP文件下载
	dcd AT_MYFTP
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 40000//AT命令返回第1个字符定时(ms)
//短信服务
	dcd AT_SMS_LINK
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 6000//AT命令返回第1个字符定时(ms)


//AT循环,在线控制
	dcd AT_LOOP
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 0//AT命令返回第1个字符定时(ms)




//HOLD暂停
	dcd AT_HOLD
	dcd ADDR_ATStr_HOLD//AT命令字符串地址
	dcd ADDR_ERRStr_ATE0//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
//关闭连接
	dcd AT_MYNETCLOSE
	dcd ADDR_ATStr_MYNETCLOSE//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETCLOSE//错误提示字符串地址
	dcd 6000//AT命令返回第1个字符定时(ms)
//关机
	dcd AT_MYPOWEROFF
	dcd ADDR_ATStr_MYPOWEROFF//AT命令字符串地址
	dcd ADDR_ERRStr_MYPOWEROFF//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
	
	//关闭网络连接
	dcd AT_MYNETACTOFF
	dcd ADDR_ATStr_MYNETACTOFF//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETACTOFF//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
  //接受客户端连接
	dcd AT_ACCEPT
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
	dcd AT_CGDCONT
	dcd ADDR_ATStr_CGDCONT//AT命令字符串地址
	dcd ADDR_ERRStr_CGDCONT//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
	dcd AT_ATDT
	dcd ADDR_ATStr_ATDT//AT命令字符串地址
	dcd ADDR_ERRStr_ATDT//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
};


void GPRS_3763_NextATCount(void)//下1个AT
{
	UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
	UART1Ctrl->SubTask=0;//子任务
	UART1Ctrl->ATCount++;
	//下个AT命令定时设置
	switch(UART1Ctrl->ATCount)//下个AT命令
	{
		case AT_CSQ_NOLINK://读信号没连接前
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//GPRS等待信号强度查询(连接前)定时
			break;
		case AT_CREG://获取当前网络注册状态
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//等待注册定时
			break;
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://查询注册网络类型：2G/3G/4G
			Terminal_Ram->GPRSWaitTx_S_Timer=10;//等待返回正确注册网络类型：2G/3G/4G
			break;
		case AT_MYNETACT://激活网络连接
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//等待激活网络连接定时
			break;
		
	}
}

void LineATIntRx(u32 ADDR_ATIntRx,u32 ADDR_BUFF,u32 Byte)//取AT中断接收数据线性放在缓冲中
{
	u32 i;
	u8 *p8s;
	u8 *p8d;
	
	while(ADDR_ATIntRx>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
	{
		ADDR_ATIntRx-=(LEN_UART1IntATRx-10);
	}
	p8s=(u8*)(ADDR_ATIntRx);
	p8d=(u8*)(ADDR_BUFF);
	for(i=0;i<Byte;i++)
	{
		p8d[0]=p8s[0];
		p8s++;
		p8d++;
		if(((u32)p8s)>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
		{
			p8s=(u8*)(ADDR_UART1IntATRx+10);
		}
	}
}
#if (USER/100)==17
//线性清空Byte字节的缓存，包括当前字节
//地址，方向，字节数
void LineClearCache(u32 ADDR,u32 Byte,u32 Direction)//取AT中断接收数据线性放在缓冲中
{
	u8 *p8;
	p8=(u8 *)ADDR;
	while(Byte--)
	{
		p8[0]=' ';
		if(Direction)//增量删除
		{
			if(((u32)p8)>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
			{
				p8=(u8*)(ADDR_UART1IntATRx+10);
			}
			else
			{
				p8++;
			}
		}
		else//减量删除
		{
			if(((u32)p8)<=(ADDR_UART1IntATRx+10))
			{
				p8=(u8*)(ADDR_UART1IntATRx+LEN_UART1IntATRx);
			}
			else
			{
				p8--;
			}
		}
	}
	
	
}
u32 GPRS_URC_Rx(void)//主动上报接收
{
  u32 i;
	u32 x;
//	u32 y;
	u32 AT_No;
//	u32 RxByte;
	u32 Addr;
//	u8 *p8s;
//	u8 *p8d;
//	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	u32 rcode;
#if PPP_ENABLE==1
	ms_Type * ms;
	ms=Get_ms();
#endif
	rcode=1;
	AT_No=UART1Ctrl->ATCount;//AT计数
	p16fifo=(u16*)(ADDR_UART1IntATRx);
#if PPP_ENABLE==1
#if((DEBUG==1)&&(DEBUG_GPRS==1))
    Addr=ATRxSearch((u32)((u8*)"\r\n"));
		if(Addr)
		{
			Myprint_ATRx();
		}
#endif
#endif
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_MYPOWEROFF://关机检查
		rcode=0;//在此步骤不做任何检查
		break;
		case AT_AT://关闭回显
		  switch(UART1Ctrl->SubTask)
			{
			  case 10://为0时等待模块上报
					MC(0,ADDR_DATABUFF,20);
				  CopyString((u8 *)"等待上报SRVST:   秒",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					x=Terminal_Ram->GPRSWaitTx_S_Timer;
//					i+=(i/10)*6;//一字节DEC转BCD
				  i=x/100;
					BCD_ASCII(i,ADDR_DATABUFF+14);
				  x%=100;
				  i=x/10;
				  BCD_ASCII(i,ADDR_DATABUFF+15);
				  i=x%10;
				  BCD_ASCII(i,ADDR_DATABUFF+16);
					WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				  Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(!Addr)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return rcode;
						}
					}
					if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
					{
//						MR(ADDR_DATABUFF,ADDR_4500_5+38,8);//判断是否是公转一体模块，如果是则重启，否则继续JS18GZYT
//						i=Compare_DL698DataString((u8*)ADDR_DATABUFF,(u8*)"JS18GZYT",8);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//						if(!i)
//						{//==0,
//							UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
//							rcode=0;
//						}
//						else
						{
							UART1Ctrl->SubTask=11;
						}
						break;
					}
					
					Addr=ATRxSearch((u32)((u8*)"^SRVST: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr)
					{
					#if LOG_1376_3==1//0=没,1=有
			       LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		      #endif
					#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
							LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
					#endif
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						i = ASCII_BCD(Addr,1);
						if((i==2)&&(MRR(Addr+1,1)=='\r'))
						{//已经注册到服务
							UART1Ctrl->SubTask=11;
						}
						LineClearCache(Addr,9,0);//向低地址清9个字节，防止重复识别
					}
//实际用到的意义太小，注释掉
//					Addr=ATRxSearch((u32)((u8*)"+CGREG: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//					if(Addr)
//					{
//					  #if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//							LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
//					  #endif
//						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//						i = ASCII_BCD(Addr,1);
//						if(i==2)//2	未注册，但 MT 正在搜寻要注册的新的运营商 
//						{
//							Terminal_Ram->GPRSWaitTx_S_Timer=90;//等待模块上报^SRVST: 2
//						}
//					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
				  break;
				case 11://为1时等待已经结束，返回去发送ATEO
				  UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					UART1Ctrl->SubTask=12;
				  break;
				case 12://已经去发送了ATEO，去处理接收
				default:
					rcode=0;
				  break;
			}
			break;
			default:
			Addr=ATRxSearch((u32)((u8*)"^"));//查询是否有以“^”为开始的上报
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//没有上报完
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//接收超时后重发本次AT命令
						Addr=ATRxSearch((u32)((u8*)"^"));//查询是否有以“^”为开始的上报
						LineClearCache(Addr,2,0);//向低地址清9个字节，防止重复识别
						UART1Ctrl->Task=0;
					}
				  break;
				}
				#if LOG_1376_3==1//0=没,1=有
			  LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		    #endif
				#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
				#endif
				Addr=ATRxSearch((u32)((u8*)"SRVST: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr)
				{
					i = ASCII_BCD(Addr,1);
					if((i!=2)||((i==2)&&(MRR(Addr+1,1)!=0x0D)))
					{//没有注册到服务，需要等待注册
					  UART1Ctrl->LinkTask=8;
						Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;
						MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
						UART1Ctrl->Task=0;//0=正在接收
						UART1Ctrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						UART1Ctrl->RxByte=0;//接收字节数=0
						UART1Ctrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
						UART1Ctrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
						Terminal_Ram->GPRSHeartBeat_S_Timer=0;//心跳发出秒定时器
						UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->ATCount=AT_AT;//AT计数
						UART1Ctrl->SubTask=10;//子任务
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
						UART1Ctrl->NETOPENCount=0;//开启服务连续次数计数(当前次数达到x次时关模块电源)
						UART1Ctrl->res3=0;//45200200中已经试图设置参数的个数
				    UART1Ctrl->res4=0;//客户端和服务器选择的链路
				    UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//远端客户机为连接时为bit1-bit0为0x3,实际socket只能为0或者1
						Terminal_Ram->CopyTxByte=0;
						LineClearCache(Addr,9,0);//向低地址清9个字节，清掉已经上报的SRVST: 防止重复识别
#if PPP_ENABLE==1
						if(UART1Ctrl->TCPSocket>2)
						{
							ms->link_close(UART1Ctrl->TCPSocket);
							UART1Ctrl->TCPSocket=0;
						}
						if(Svr_Skt->ppp0_clint>2)
						{
							ms->link_close(Svr_Skt->ppp0_clint);
							Svr_Skt->ppp0_clint=0;
						}
						Svr_Skt->ppp0_clint=0;
						//是重启模块，还是只再次建立侦听socket
						if(Svr_Skt->ppp0_listen>2)
						{
							ms->link_close(Svr_Skt->ppp0_listen);
							Svr_Skt->ppp0_listen=0;
						}
#endif

						break;
					}
				}
				p16fifo[1]=x;
			}

			Addr=ATRxSearch((u32)((u8*)"+"));//查询是否有以“+”为开始的上报
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//没有上报完
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//接收超时后重发本次AT命令
						Addr=ATRxSearch((u32)((u8*)"+"));//查询是否有以“^”为开始的上报
						LineClearCache(Addr,2,0);//向低地址清9个字节，防止重复识别
					}
				  break;
				}
//没有作用去掉
//				Addr=ATRxSearch((u32)((u8*)"CGREG: "));
//				if(Addr)
//				{
//					#if LOG_1376_3==1//0=没,1=有
//			    LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
//		      #endif
//				  #if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//							LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
//					#endif
//					i = ASCII_BCD(Addr,1);
//					if((i!=1)&&(i!=5))//1	注册了本地网络,5	注册了漫游网络 
//					{//没有注册到服务，需要等待注册
//						UART1Ctrl->ATCount=AT_ATE0;
//						UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//						UART1Ctrl->SubTask=0;
//						break;
//					}
//				}
				p16fifo[1]=x;
			}

//#warning "test_end"
//      switch(p32[4*AT_No])
//			{
//			 case AT_ACCEPT://填写OK;
//			   p16fifo[0]=6;
//			   p16fifo[1]=0;
//				 p8s=(u8*)(ADDR_UART1IntATRx+10);
//				 x=CopyString((u8 *)"OK\r\n",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//			   break;
//			 case AT_MYNETOPEN_LINK://填写OK;
//				 i=UART1Ctrl->res4;
//			 if(!(i&TCP_0_OPEN))
//			 {
//				 if(((i&TCP_1_SOCKET_MASK)==TCP_1_SOCKET_MASK))
//				 {//如果socket为空，
//					 p16fifo[0]=42;
//					 p16fifo[1]=0;
//					 p8s=(u8*)(ADDR_UART1IntATRx+10);
//					 x=CopyString((u8 *)"$MYURCCLIENT: 0,\"100.192.1.52\",58575\r\n",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//				 }
//			 }
//				 
//			   break;
//			 default:
//			   break;
//			}
//#warning "test_end"		
			Addr=ATRxSearch((u32)((u8*)"$"));//查询是否有以“$”为开始的上报
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//没有上报完
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//接收超时后重发本次AT命令
						Addr=ATRxSearch((u32)((u8*)"$"));//查询是否有以“^”为开始的上报
						LineClearCache(Addr,2,0);//向低地址清9个字节，防止重复识别
					}
				  break;
				}
				Addr=ATRxSearch((u32)((u8*)"MYURCCLIENT: "));//$MYURCCLIENT: 1,“172.16.23.100”,31256
				if(Addr)
				{
					#if LOG_1376_3==1//0=没,1=有
			     LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		      #endif
				  #if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
						LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
					#endif
					i = ASCII_BCD(Addr,1);//TCP server socket
					if(i<3)
					{//上报的是符合要求的socket
						
					  UART1Ctrl->res4&=~TCP_1_SOCKET_MASK;
					  UART1Ctrl->res4|=i;
						UART1Ctrl->ATTask=1;
						UART1Ctrl->ATCount=AT_ACCEPT;
						UART1Ctrl->LinkSubTask=0;
						UART1Ctrl->SubTask=0;
						rcode=1;//如果返回的是1则返回
					}
					else
					{//否则不接受，略过
					  rcode=0;
					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));
					if(Addr)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					break;
				}
				Addr=ATRxSearch((u32)((u8*)"MYURCCLOSE: "));//$MYURCCLIENT: 1,“172.16.23.100”,31256
				if(Addr)
				{
					i = ASCII_BCD(Addr,1);//上报的关闭的端口
					switch(i)
					{
						case TCP_0_SOCKET://TCP clinet  socket
							UART1Ctrl->res4&=~TCP_0_OPEN;//TCP clinet  socket_close通道建立
				      UART1Ctrl->res4&=~TCP_0;//TCP clinet link通道建立
							break;
						case TCP_1_LISTEN://TCP server  listen
							UART1Ctrl->res4|=TCP_1_LISTEN_OPEN;
							break;
						case 0://TCP server  socket
						case 1://TCP server  socket
							UART1Ctrl->res4&=~TCP_1;//TCPserver通道断开
						  UART1Ctrl->res4|=TCP_1_SOCKET_MASK;
							break;
						default :
							break;
					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));
					if(Addr)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					break;
				}

//				Addr=ATRxSearch((u32)((u8*)"MYURCACT: "));//$MYURCCLIENT: 1,“172.16.23.100”,31256
//				if(Addr)
//				{
//					Addr=ATRxSearch((u32)((u8*)"\r\n"));
//				}
//				Addr=ATRxSearch((u32)((u8*)"MYURCSRVPORT: "));//$MYURCCLIENT: 1,“172.16.23.100”,31256
//				if(Addr)
//				{
//					Addr=ATRxSearch((u32)((u8*)"\r\n"));
//				}
				
				p16fifo[1]=x;
			}
			rcode=0;
			break;
	}
	return rcode;
}
void DISPLAY_WarningOccur(u32 AT_No)
{
	u32 i;
	switch(AT_No)
	{
		case AT_AT:
			WarningOccur(GPRSPORT,(u8 *)"AT波特率同步");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYTYPE:
			WarningOccur(GPRSPORT,(u8 *)"查询远程通信模块类型");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYGMR:
			WarningOccur(GPRSPORT,(u8 *)"获取模块软件版本");//告警发生,入口告警代码和告警字符串
			break;
		case AT_CPIN:
			WarningOccur(GPRSPORT,(u8 *)"卡检查");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYNETURC:
			WarningOccur(GPRSPORT,(u8 *)"开启主动上报");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYCCID:
			WarningOccur(GPRSPORT,(u8 *)"获取SIM卡序列号ICCID");//告警发生,入口告警代码和告警字符串
			break;
		case AT_CIMI:
			WarningOccur(GPRSPORT,(u8 *)"查国际移动台标识请求IMSI");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYNETCON_APN:
		case AT_MYNETCON_USERPWD:
			MC(0,ADDR_DATABUFF,20);
		  if(AT_No==AT_MYNETCON_APN)
			{
				CopyString((u8 *)"设置    运营商 GAPN",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			}
			else
			{
				CopyString((u8 *)"设置    运营商 G密码",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			}
		  i=Comm_Ram->CSPN;
		  switch(i)
			{
				case 1: //1=中国移动通信china mobile
					CopyString((u8 *)"移动",(u8*)(ADDR_DATABUFF+4));//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 2: //2=中国联通通讯china unicom
					CopyString((u8 *)"联通",(u8*)(ADDR_DATABUFF+4));//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 3: //3=中国电信CHINA TELECOM
					CopyString((u8 *)"电信",(u8*)(ADDR_DATABUFF+4));//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;		
				case 54://?754: 注册上江苏电力1.8GHz精控业务网络
					CopyString((u8 *)"专网",(u8*)(ADDR_DATABUFF+4));//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
				default:
					break;
			}
		  i=Comm_Ram->RemoteWirelessModuleSYSINFO; //远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
      BCD_ASCII(i,ADDR_DATABUFF+14);
			WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYNETACT:
			WarningOccur(GPRSPORT,(u8 *)"激活网络连接");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYNETOPEN:
			WarningOccur(GPRSPORT,(u8 *)"开启远程服务");//告警发生,入口告警代码和告警字符串
			break;
		case AT_MYPOWEROFF:
			WarningOccur(GPRSPORT,(u8 *)"关闭远程模块，重启");//告警发生,入口告警代码和告警字符串
			break;
		case AT_CGDCONT:
			WarningOccur(GPRSPORT,(u8 *)"设置ISP接入网关");//告警发生,入口告警代码和告警字符串
			break;
		case AT_ATDT:
			WarningOccur(GPRSPORT,(u8 *)"启动ATDT拨号");//告警发生,入口告警代码和告警字符串
			break;
	}
}
#endif //#if (USER/100)==17



void GPRS_3763_AT_Tx(void)//AT命令发送
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 AT_No;
	u32 TxByte;
	u8 *p8;
#if (PPP_ENABLE==1)//使用PPP拨号建立连接
#else
	u16 *p16;
#endif
	u32 *p32;
	u8 *p8rx;
	u8 *p8tx;
	
	u32 FTPFileCount;//FTP文件下载个数计数
#ifdef LINUX
	ms_Type * ms;
	ms=Get_ms();
	ms->linux_uart_receive(GPRSPORT);
#endif
	n=n;
	p8tx=(u8*)(ADDR_ATTx);
	TxByte=0;
	AT_No=UART1Ctrl->ATCount;//AT计数
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT计数
	}
	#if (USER/100)==17
	DMA_UARTn_RX_Pointer(GPRSPORT);//计算DMA_UARTn接收指针值(p0)
	i=GPRS_URC_Rx();//主动上报接收，默认返回1
	if(i)//如果返回的是1则返回
	{
		return;
	}
	#endif
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	
#if (USER/100)==17
  DISPLAY_WarningOccur(p32[4*AT_No]);
#endif
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT://波特率检查
			TxByte=CopyString((u8 *)"AT\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			switch(UART1Ctrl->SubTask)//子任务
			{
				case 0://速率57600
					UART1Ctrl->BpsCtrl=0x3+(9<<5);
					break;
				case 1://速率115200
					UART1Ctrl->BpsCtrl=0x3+(10<<5);
					break;
				case 2://速率38400
					UART1Ctrl->BpsCtrl=0x3+(8<<5);
					break;
				case 3://速率19200
					UART1Ctrl->BpsCtrl=0x3+(7<<5);
					break;
				case 4://速率9600
					UART1Ctrl->BpsCtrl=0x3+(6<<5);
					break;
				case 5://速率4800
					UART1Ctrl->BpsCtrl=0x3+(5<<5);
					break;
				case 6://速率2400
					UART1Ctrl->BpsCtrl=0x3+(4<<5);
					break;
				case 7://速率1200
					UART1Ctrl->BpsCtrl=0x3+(3<<5);
					break;
//				case 10://下1个AT
//					GPRS_3763_NextATCount();//不需设置,57.6k可通信
//					return;
				default:
					TxByte=CopyString((u8 *)"AT+IPR=57600\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					break;
			}
			//启动UART1口发送
//#ifndef LINUX
		Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
//#else
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
#if PPP_ENABLE==1
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			  log_out((u32)p8tx,TxByte, 'c');
#endif
#endif
			UART1Ctrl->TxByte=TxByte;//发送字节数(半字对齐)
			UART1Ctrl->TxCount=0;
		#if LOG_1376_3==1//0=没,1=有
			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			Start_UARTn_Tx(GPRSPORT);//启动UART口发送
			UART1Ctrl->ATTask=2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			return;
		case AT_CNUM://取本机号码
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;		
		case AT_MYNETCON_APN://设置APN
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
	#if MainProtocol==376
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			MC(0,ADDR_DATABUFF+16,1);
	#else
			//DL698
			i=MRR(ADDR_4520_2+1,1);//取备用通道数
			if(i==0)
			{//备用通道数=0
				p8=Get_Element((u8 *)ADDR_4500_2,6,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{//没找到
					CopyString((u8 *)"cmnet\x0",(u8 *)ADDR_DATABUFF);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
			}
			else
			{//有备用通道数
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//取备用通道数
				i=Comm_Ram->CSPN;//0=空,1=中国移动通信-----china mobile  CMCC,2=中国联通通讯-----china unicom  CUCC,3=中国电信  ------CHINA TELECOM  CTCC		
				switch(i)
				{
					default:
					case 1://1=中国移动通信-----china mobile  CMCC
						n=0;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
					case 2://2=中国联通通讯-----china unicom  CUCC
						n=2;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
					case 3://3=中国电信  ------CHINA TELECOM  CTCC	
						n=1;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
#if (USER/100)==17
          case 54://54江苏专网
						 n=i;
					break;
#endif
				}
				p8+=2;
				y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				for(i=0;i<y;i++)//略过已经发送的APN参数
				{
				  p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				for(;i<x;i++)//此时i已经=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				{
					if(p8[3]==n)//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
					{
		#if COMPARE_2G_3G_4G==1 //设置APN时比对2G，3G，4G
						y=p8[5];//2G = 0，3G = 1， 4G = 2， 未知 = 255 ， 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
		#else
						//设置APN时不比对2G，3G，4G
		#endif
						{
							break;
						}
					}
					#if (USER/100)==17
					if((p8[3]==0x95)&&(n==54))
					{//江苏有两台终端在协议确定前就挂到现场了，专网类型还是0x95
						break;
					}
					#endif
					p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				UART1Ctrl->res3=i;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				
				if(i<x)
				{//找到
					p8+=6;
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					return;
				}
			}
	#endif
			TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case AT_MYNETCON_USERPWD://设置USERPWD:用户名和密码
			//AT$MYNETCON=0,"USERPWD","card,card"
		#if (USER/100)==14//福建
			#if MainProtocol==376
				MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
				p8=(u8*)(ADDR_DATABUFF);
				if(((p8[0]!='f')&&(p8[0]!='F'))
				|| ((p8[1]!='j')&&(p8[1]!='J'))
				|| ((p8[2]!='e')&&(p8[2]!='E'))
				|| ((p8[3]!='p')&&(p8[3]!='P'))
				|| (p8[4]!='.')
				|| ((p8[5]!='f')&&(p8[4]!='F'))
				|| ((p8[6]!='j')&&(p8[6]!='J')))
				{//非特定APN，用户名密码采取AFN04F16设置值
				}
				else
				{
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,12);//设备编号
					p8[12]=0;//字符串结尾
					//用户名
					TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8tx[TxByte++]=',';
					//密码
					for(i=0;i<12;i++)
					{//倒序拷贝
						p8tx[TxByte+i]=p8[11-i];
					}
					TxByte+=12;
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				}
			#endif
		#endif
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
	#if MainProtocol==376
			if(MRR(ADDR_AFN04F16,1)!=0)
			{
				MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
				MC(0,ADDR_DATABUFF+32,1);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8tx[TxByte]=',';
				TxByte++;
				MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{//用户名和密码均为空,分二种命令,有','号和无','号
				if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
				{
					p8tx[TxByte]=',';
					TxByte++;
				}
			}
	#else
			#if (USER/100)==14
			p8=Get_Element((u8 *)ADDR_4500_2,6,0,LENmax_4500_2);//获取APN
			if((p8)&&(p8[1]==7)&&((p8[0]=='f')||(p8[0]=='F'))
			&&((p8[1]=='j')||(p8[1]=='J'))
			&&((p8[3]=='p')||(p8[3]=='P'))
			&&(p8[4]=='.')
			&&((p8[5]=='f')||(p8[4]=='F'))
			&&((p8[6]=='j')||(p8[6]=='J')))
			{//设备拨号认证时发送的APN用户名应自动添加相应的后缀（即当识别到通信模块为GPRS时为A@fjep.fj，当识别到通信模块为CDMA时为A@fjep.vpdn.fj）
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到用户名
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					TxByte+=CopyString(p8,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//   （2）01：China Mobile（中国移动）
//   （3）02：China Unicom（中国联通）
//   （4）03：China Telecom（中国电信）
					if((Comm_Ram->CSPN==1)||(Comm_Ram->CSPN==2))
					{
						TxByte+=CopyString((u8 *)"@fjep.fj",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
					else if(Comm_Ram->CSPN==3)
					{
						TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
					else
					{
						TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
					p8tx[TxByte]=',';
					TxByte++;
					p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//找到密码
						i=p8[1];//字节数
						p8+=2;
						p8[i]=0;
						TxByte+=CopyString(p8,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
				}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
			}
			#endif
			
			//DL698
			i=MRR(ADDR_4520_2+1,1);//取备用通道数
			if(i==0)
			{//备用通道数=0
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					TxByte+=CopyString(p8,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8tx[TxByte]=',';
					TxByte++;
					p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//找到
						i=p8[1];//字节数
						p8+=2;
						p8[i]=0;
						TxByte+=CopyString(p8,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
				}
				else
				{//没找到;用户名和密码均为空,分二种命令,有','号和无','号
					if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
					{
						p8tx[TxByte]=',';
						TxByte++;
					}
				}
			}
			else
			{//有备用通道数
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//取备用通道数
				p8+=2;
				y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数,在设置APN时已经确定
				for(i=0;i<y;i++)//略过已经发送的用户名密码
				{
				  p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				if(i<x)
				{//找到
					p8+=6;
					p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					//用户名
					if(p8[0]==DataType_visible_string)
					{
						i=p8[1];//字节数
						p8+=2;
						MR(ADDR_DATABUFF,(u32)p8,i);
						MC(0,ADDR_DATABUFF+i,1);
						TxByte+=CopyString((u8*)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
						p8+=i;
						//密码
						p8tx[TxByte]=',';
						TxByte++;
						if(p8[0]==DataType_visible_string)
						{
							i=p8[1];//字节数
							p8+=2;
							MR(ADDR_DATABUFF,(u32)p8,i);
							MC(0,ADDR_DATABUFF+i,1);
							TxByte+=CopyString((u8*)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
						}
					}
					else
					{//没找到;用户名和密码均为空,分二种命令,有','号和无','号
						if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
						{
							p8tx[TxByte]=',';
							TxByte++;
						}
					}
				}
				else
				{//没找到;用户名和密码均为空,分二种命令,有','号和无','号
					if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
					{
						p8tx[TxByte]=',';
						TxByte++;
					}
				}
			}
	#endif
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case AT_MYIPFILTER://IP访问控制配置
			if(UART1Ctrl->SubTask>=5)
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
	#if MainProtocol==376
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			x=MRR(ADDR_IPFILTER+(4*UART1Ctrl->SubTask),4);
			if(x==0)
			{
				UART1Ctrl->SubTask++;//子任计数
				return;
			}
			p8tx[TxByte]='"';
			TxByte++;
			for(i=0;i<4;i++)
			{
				y=hex_bcd(x&0xff);//HEX数转为8位压缩BCD数
				x>>=8;
				TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
				p8tx[TxByte]='.';
				TxByte++;
			}
			TxByte--;//IP第4个字节的'.'删去
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)"255.255.255.255",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
	#else
	    switch(UART1Ctrl->SubTask)//子任务
			{
				default:
			  case 0://确定工作模式
				  MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
					p8=(u8*)ADDR_128KDATABUFF;
					i=p8[1]<<4;//B5-B4=0混合模式=1客户机模式=2服务器模式
					i|=p8[5]<<7;//B7=0TCP=1UDP
					UART1Ctrl->TCPUDP=i;
					i&=0x30;
					if((i==0x20)||(i==0x00))
					{//=2服务器模式=0混合模式
					  UART1Ctrl->SubTask=1;
						return;
					}
					else
					{
						GPRS_3763_NextATCount();//下1个AT
						return;
					}
				case 1://	2：删除全部IP认证通道“192.168.0.23”,“255.255.255.255”
					TxByte=CopyString((u8 *)"AT$MYIPFILTER=0,2,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
          p8tx[TxByte++]='"';
				  TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				  p8tx[TxByte++]='"';
				  p8tx[TxByte++]=',';
				  p8tx[TxByte++]='"';
				  TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				  p8tx[TxByte++]='"';
				  p8tx[TxByte++]='\r';
					break;
				case 2://设置IP监听.先设置主用IP，
				case 3://设置IP监听.再设置备用IP
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					if(UART1Ctrl->SubTask==2)
					{
						x=1;
						p8tx[14]='0';
					}
					else
					{
						x=2;
						p8tx[14]='1';
					}
					i=MRR(ADDR_4520_2+1,1);//取备用通道数
					if(i==0)
					{
					  i=MRR(ADDR_4500_3+1,1);//主站IP地址个数
						if(x<=i)
						{
						  p8=Get_Element((u8*)ADDR_4500_3,x,1,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
							MR(ADDR_DATABUFF,(u32)p8+2,4);
						}
						else
						{
						  GPRS_3763_NextATCount();//下1个AT
							return;
						}
					}
					else
					{
					  MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
						p8=(u8*)ADDR_128KDATABUFF;
						p8+=2;
						y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数,在设置APN时已经确定
						for(i=0;i<y;i++)//略过已经发送的用户名密码
						{
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
						p8=Get_Element(p8,8,0,LENmax_4520_2);//计算元素地址,使用ADDR_128KDATABUFF
						//此时p8指向4520中特定通讯参数的主站IP数组列表
						i=p8[1];
						if(x<=i)
						{
						  p8=Get_Element(p8,x,1,LENmax_4520_2);//找到IP地址
						  MR(ADDR_DATABUFF,(u32)p8+2,4);
						}
						else
						{
						  GPRS_3763_NextATCount();//下1个AT
							return;
						}
					}
					//AT$MYIPFILTER=0,1,“192.168.0.23”,“255.255.255.255”	// 添加指定的IP认证通道
					
					//dcb "AT$MYIPFILTER=0,1,",0
					p8tx[TxByte]='"';
					TxByte++;
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<4;i++)
					{
						y=hex_bcd(p8[i]);//HEX数转为8位压缩BCD数
						TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
						p8tx[TxByte]='.';
						TxByte++;
					}
					TxByte--;//IP第4个字节的'.'删去
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]=',';
					TxByte++;
					p8tx[TxByte]='"';
					TxByte++;
					TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
				break;
			}
			
			break;
	#endif
			
	#if (USER/100)==0//用户标识,电科院测试不发短信AT
		case AT_CMFG://设置短消息模式
		case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
		case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
			GPRS_3763_NextATCount();//下1个AT
			return;
	#else
		#if (USER/100)==14//福建系
		case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
//		case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
			GPRS_3763_NextATCount();//下1个AT
			return;
		#else
		case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
		case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
			GPRS_3763_NextATCount();//下1个AT
			return;
		#endif//(USER/100)==14//福建系
	#endif
	
		case AT_MYNETSRV://设置服务参数
			switch(UART1Ctrl->SubTask)//子任务
			{
				case 0://确定工作模式
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它;
			#else
				//DL698
				//通信配置∷=structure
				//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
				//在线方式      enum{永久在线（0），被动激活（1）}，
				//连接方式      enum{TCP（0），UDP（1）}，
				//连接应用方式  enum{主备模式（0），多连接模式（1）}，
					MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
					p8=(u8*)ADDR_128KDATABUFF;
					i=p8[1]<<4;//B5-B4=0混合模式=1客户机模式=2服务器模式
					i|=p8[5]<<7;//B7=0TCP=1UDP
			#endif
					UART1Ctrl->TCPUDP=i;
					i&=0x30;
					if(UART1Ctrl->TCPUDP&0x80)
					{//UDP
						if(i==0x20)
						{//=2服务器模式
							UART1Ctrl->SubTask=4;
						}
						else
						{//=0混合模式=1客户机模式
							UART1Ctrl->SubTask=3;
						}
					}
					else
					{//TCP
						if(i==0x20)
						{//=2服务器模式
							UART1Ctrl->SubTask=2;
						}
						else
						{//=0混合模式=1客户机模式
							UART1Ctrl->SubTask=1;
						}
					}
					return;
				case 1://TCP客户机模式
				default://TCP客户机模式
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,3,0,0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				TCPClient:
					p8tx[TxByte]='"';
					TxByte+=1;
					UART1Ctrl->LinkCount++;
			#if MainProtocol==376
					if(UART1Ctrl->LinkCount>=4)
					{
						UART1Ctrl->LinkCount=0;
					}
					if(UART1Ctrl->LinkCount<=2)
					{//主ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3,6);
					}
					else
					{//备用ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3+6,6);
					}
			#else
					//DL698
					i=MRR(ADDR_4520_2+1,1);//取备用通道数
					if(i==0)
					{//备用通道数=0
						if(UART1Ctrl->LinkCount>=4)
						{
							UART1Ctrl->LinkCount=0;
						}
						i=MRR(ADDR_4500_3+1,1);
						if((UART1Ctrl->LinkCount<=2)||(i<2))
						{//主ip
							p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
							if(p8)
							{
								MR(ADDR_DATABUFF,(u32)p8+2,4);
								p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
								if(p8)
								{
									i=p8[1]<<8;
									i|=p8[2];
									p8=(u8*)(ADDR_DATABUFF);
									p8[4]=i;
									p8[5]=i>>8;
								}
							}
						}
						else
						{//备用ip
							p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
							if(p8)
							{
								MR(ADDR_DATABUFF,(u32)p8+2,4);
								p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
								if(p8)
								{
									i=p8[1]<<8;
									i|=p8[2];
									p8=(u8*)(ADDR_DATABUFF);
									p8[4]=i;
									p8[5]=i>>8;
								}
							}
						}
					}
					else
					{//有备用通道数
						MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
						p8=(u8*)ADDR_128KDATABUFF;
						x=p8[1];
						p8+=2;
						y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数,在设置APN时已经确定
						for(i=0;i<y;i++)//略过已经发送的用户名密码
						{
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
						//此时p8指向4520中特定通讯参数的主站IP数组列表
						MC(0,ADDR_DATABUFF,6);
						if(i<x)
						{//找到
							if(UART1Ctrl->LinkCount>=4)
							{
								UART1Ctrl->LinkCount=0;
							}
							p8=Get_Element(p8,8,0,0);//计算元素地址,使用ADDR_128KDATABUFF
							if(p8)
							{
								i=p8[1];//主站通信参数 array数
								if((UART1Ctrl->LinkCount<=2)||(i<2))
								{//主ip
									p8+=2;
									MR(ADDR_DATABUFF,(u32)p8+4,4);
									p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{
										i=p8[1]<<8;
										i|=p8[2];
										p8=(u8*)(ADDR_DATABUFF);
										p8[4]=i;
										p8[5]=i>>8;
									}
								}
								else
								{//备用ip
									p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{
										MR(ADDR_DATABUFF,(u32)p8+4,4);
										p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{
											i=p8[1]<<8;
											i|=p8[2];
											p8=(u8*)(ADDR_DATABUFF);
											p8[4]=i;
											p8[5]=i>>8;
										}
									}
								}
							}
						}
					}
			#endif
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<4;i++)
					{
						y=hex_bcd(p8[i]);//HEX数转为8位压缩BCD数
						TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
						p8tx[TxByte]='.';
						TxByte++;
					}
					TxByte--;//IP第4个字节的'.'删去
					//目的端口号
					p8tx[TxByte]=':';
					TxByte+=1;
					y=hex_bcd(p8[4]+(p8[5]<<8));//HEX数转为8位压缩BCD数
					TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 2://TCP服务器模式
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,2,1,0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8tx[14]=0x30+TCP_1_LISTEN;//修改socket值
					p8tx[TxByte]='"';
					TxByte++;
					TxByte+=CopyString((u8 *)"127.0.0.1",p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8tx[TxByte]=':';
					TxByte+=1;
					//服务器模式,端口号用设定
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F7+20,1);//终端侦听端口 BIN 2
					i+=(20+1);
					i+=(1+MRR(ADDR_AFN04F7+i,1));
					i=MRR(ADDR_AFN04F7+i,2);
					i=hex_bcd(i);//HEX数转为8位压缩BCD数
			#else
//#define ADDR_4500_2     ADDR_4003+LENmax_4003//公网通信模块1;2通信配置
//通信配置∷=structure
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
//在线方式      enum{永久在线（0），被动激活（1）}，
//连接方式      enum{TCP（0），UDP（1）}，
//连接应用方式  enum{主备模式（0），多连接模式（1）}，
//侦听端口列表  array long-unsigned，
//APN            visible-string，
//用户名         visible-string，
//密码           visible-string，
//代理服务器地址 octet-string，
//代理端口            long-unsigned，
//超时时间及重发次数  bit-string(SIZE(8))
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
//心跳周期(秒)  long-unsigned
//}
					i=MRR(ADDR_4500_2+13,2);
					i=(i>>8)+((i&0xff)<<8);
					i=hex_bcd(i);//HEX数转为8位压缩BCD数
			#endif
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 3://UDP客户机模式
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,2,0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					goto TCPClient;
				case 4://UDP服务器模式
					//无此模式
					WarningOccur(GPRSPORT,(u8 *)"远程无线上行模式错");//告警发生,入口告警代码和告警字符串
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					goto TCPClient;
			}
			break;
		case AT_MYNETOPEN://开启服务
			if(UART1Ctrl->LinkCtrl==0)//连接控制0=不连接,1=连接
			{
				GPRS_3763_NextATCount();//下1个AT
			}
			else
			{
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F8,1);
				i&=0x3;
				if(i==2)
				{//被动激活模式
					if(UART1Ctrl->ReLinkNum==0)//被动激活模式重拨次数,收到被动激活命令时置初值
					{//被动激活模式重拨次数=0;
						i=MRR(ADDR_AFN05F38,1);
						if(i!=0x0)
						{
							MC(0,ADDR_AFN05F38,1);//连续无通信自动断线清除激活命令0x55(0x5a)
						}
						GPRS_3763_NextATCount();//下1个AT
						break;
					}
					UART1Ctrl->ReLinkNum--;//被动激活模式重拨次数,收到被动激活命令时置初值
				}
				else
				{//永久在线模式,时段在线模式
					if(Terminal_Ram->GPRSReConnection_S_Timer!=0x0)//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
					{
						WarningOccur(GPRSPORT,(u8 *)"远程无线重拨间隔定时");//告警发生,入口告警代码和告警字符串
						GPRS_3763_NextATCount();//下1个AT
						break;
					}
					//每次登录失败后，经过心跳周期0.5-1.5倍的随机延时（以秒或毫秒计）后重新登录
				#if (USER/100)==13//黑龙江专变
					Terminal_Ram->GPRSReConnection_S_Timer=0;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
				#else
					i=MRR(ADDR_AFN04F8+1,2);//永久在线、时段在线模式重拨间隔 BIN 秒 2
					Terminal_Ram->GPRSReConnection_S_Timer=i;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
				#endif
				}
		#else
				//698.45
				i=UART1Ctrl->TCPUDP;
				i&=0x30;
				switch(UART1Ctrl->SubTask)
				{
				  default:
				  case 0://客户端open socket
						if((i==0x10)||(i==0x00))
						{//=1客户机模式=0混合模式
						  if(!(UART1Ctrl->res4&TCP_0_OPEN))
							{//还没有open socket
							  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
								break;
							}
						}
						//服务器模式或者已经打开socket了
						UART1Ctrl->SubTask++;
					  return;
					case 1://服务端open socket
						if((i==0x20)||(i==0x00))
						{//=2服务器模式
						  if(!(UART1Ctrl->res4&TCP_1_LISTEN_OPEN))
							{//还没有open socket
							  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
						    p8tx[13]=0x30+TCP_1_LISTEN;//使用TCP_1_LISTEN号soket监听
								break;
							}
						}
						//其他模式或者已经打开socket了
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				    UART1Ctrl->SubTask=0;//子任务
				    UART1Ctrl->ATCount=AT_CSQ_LINK;//读信号连接后
						return;
				}
		#endif
				Terminal_Ram->GPRSWaitConnection_S_Timer=120;//20 GPRS发起连接服务等待秒定时器
				UART1Ctrl->NETOPENCount++;//开启服务次数计数(当前次数达到x次时关模块电源)
			}
			break;
		case AT_LOOP_NOLINK://AT循环,没连接
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
		#if (USER/100)==13//黑龙江专变
			if(UART1Ctrl->NETOPENCount>=5)//开启服务连续次数计数(当前次数达到x次时关模块电源)
		#else
			if(UART1Ctrl->NETOPENCount>=3)//开启服务连续次数计数(当前次数达到x次时关模块电源)
		#endif
			{
				UART1Ctrl->NETOPENCount=0;//开启服务连续次数计数(当前次数达到x次时关模块电源)
				i=MRR(ADDR_4520_2+1,1);//取备用通道数
				if(UART1Ctrl->res3<i)//res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				{//还没有尝试完所有的APN和密码
					UART1Ctrl->res3++;
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					UART1Ctrl->LinkCount=0;
					return;
				}
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
			}
			else
			{
				if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
				{
					UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
					x=0;//要重新发起连接标志
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);
					i&=0x3;
			#else
					i=0;
					
			#endif
					if(i==2)
					{//被动激活模式
						if(UART1Ctrl->ReLinkNum!=0)//被动激活模式重拨次数,收到被动激活命令时置初值
						{//被动激活模式重拨次数=0;
							x=1;//要重新发起连接标志
						}
					}
					else
					{//永久在线模式,时段在线模式
						if(Terminal_Ram->GPRSReConnection_S_Timer==0x0)//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
						{
							x=1;//要重新发起连接标志
						}
					}
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->SubTask=0;//子任务
					if(x==0)//要重新发起连接标志
					{
						UART1Ctrl->ATCount=AT_SMS_NOLINK;//短信服务没连接前
					}
					else
					{
						UART1Ctrl->ATCount=AT_MYNETSRV;//设置服务参数
					}
				}
			}
			break;
		case AT_CSQ_LINK://读信号连接后(每秒1次)
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
			break;
		case AT_SYSINFO:
		case AT_SYSINFO_LINK://
	#if MainProtocol==376
		#if (USER/100)!=0//不是电科院送检
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->TYMDHMS[0])//
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//
			}
			else
			{
				GPRS_3763_NextATCount();//??1??AT
				return;
			}
			break;
		#else
			GPRS_3763_NextATCount();//??1??AT
			return;
		#endif
	#endif
	#if MainProtocol==698
		#if (USER/100)!=0//不是电科院送检
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);
#if (PPP_ENABLE==1)//使用PPP拨号建立连接 
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
#endif
			}
			else
			{
				GPRS_3763_NextATCount();//??1??AT
				return;
			}
			break;
		#else
			GPRS_3763_NextATCount();//??1??AT
			break;
		#endif
	#endif
		case AT_MYNETOPEN_LINK://连接检查(每秒1次)
#if (PPP_ENABLE==1)//使用PPP拨号建立连接  
		if(0==(Comm_Ram->RTCBuff[0])%5)
		{
				if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])
				{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("recheck----AT_CSQ_LINK\n",0,0,0);
#endif
			    UART1Ctrl->ATCount=AT_CSQ_LINK;
				}
		}
		  return;
#else
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];


				if((UART1Ctrl->LINKCHECK_RTCS%5)<3)
				{
					GPRS_3763_NextATCount();//下1个AT
					return;
				}
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
  #if (USER/100)==17
				UART1Ctrl->SubTask=0;//江苏市场的中兴模块会连续回两种不同格式的状态，该标志用于处理状态1和状态2
  #endif

			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
			break;
#endif
		case AT_MYNETREAD://读取数据
#if (PPP_ENABLE==1)//使用PPP拨号建立连接
     GPRS_3763_NextATCount();//下1个AT
		 return;
#else
			p16=(u16*)(ADDR_UART1IntRx);
			if(p16[0]!=p16[1])
			{
				UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
			#endif
				return;
			}
			else
			{
			  i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket
				if(0x00==(UART1Ctrl->TCPUDP&0x30))
				{//当前混合模式
					if(!(i&FORCE_TCP_1))//当前socket未被锁定，则更改socket
					{
						i^=TCP_SELECT_SERVER;
					}
					UART1Ctrl->res4=i;
				}
//				  if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0)&&(i&TCP_0_OPEN))
          if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
					{//客户机模式读取socket0
					  TxByte=CopyString((u8 *)"AT$MYNETREAD=3,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					}
					else if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server 通道建立
           //远程TCP客户端建立连接
						TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
						p8tx[13]=0x30+(i&TCP_1_SOCKET_MASK);//修改socket值
					}
					else
					{
					  if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
						{
							Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
							if(UART1Ctrl->HostACK>=4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
							{//有其他主动上报等待确认返回
								if(!((i&TCP_0_OPEN)||(i&TCP_0)))
								{//socket不存在或者通道连接不存在
									UART1Ctrl->NETOPENCount=0;
									UART1Ctrl->ATCount=AT_MYNETSRV;
									return;
								}
							}
						}
					  GPRS_3763_NextATCount();//下1个AT
					  return;
					}
				  //i=LEN_UART1IntATRx-1024;
					i=1460;
					i=hex_bcd(i);//HEX数转为8位压缩BCD数
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8tx[TxByte]='\r';
					TxByte++;
			}
			break;
#endif
		case AT_MYNETWRITE://发送数据
//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket
//#define TCP_0_OPEN 0x80  //TCP clinet  socket_open
//#define TCP_0 0x40       //TCP clinet link
//#define TCP_1_LISTEN_OPEN 0x20  //TCP server  socket_open
//#define TCP_1 0x10       //TCP server link
//#define TCP_SELECT_SERVER 0x08  //SELECT TCP_clinet and TCP_server
//#define FORCE_TCP_1 0x04 //FORCE_TCP_server
//#define TCP_1_SOCKET_MASK 0x3 //TCP server  socket

//#define TCP_1_LISTEN 4   //TCP server  listen
#if (PPP_ENABLE==1)//使用PPP拨号建立连接
     GPRS_3763_NextATCount();//下1个AT
		 return;
#else
			switch(UART1Ctrl->ATWriteTask)//AT数据发送任务:0=发命令,1=发数据
			{
				case 0://发AT头
			    i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server 通道建立//远程TCP客户端建立连接
						p8tx[14]=0x30+(i&TCP_1_SOCKET_MASK);//修改socket值
					}
					else //if(!(i&TCP_SELECT_SERVER))
					{//轮询客户机模式socket0
#if (USER/100)==17
						if(Terminal_Ram->CopyTxByte!=0)
						{//当前需要发送数据
							if(!((i&TCP_0_OPEN)||(i&TCP_0)))
							{//socket不存在或者通道连接不存在
								UART1Ctrl->NETOPENCount=0;
							  UART1Ctrl->ATCount=AT_MYNETSRV;
								return;
							}
						}
#endif
					}
					i=Terminal_Ram->CopyTxByte;//还需发送字节数
					if(i==0)
					{//还需发送字节数错误
						if(UART1Ctrl->Task==6)//6=多帧数据正在发送
						{
						#if MainProtocol==376
							UART1Ctrl->Task=4;//4=多帧数据续接收完成
						#else
							UART1Ctrl->Task=0;
						#endif
						}
						else
						{
							UART1Ctrl->Task=0;//0=接收
						}
						UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						GPRS_3763_NextATCount();//下1个AT
						return;
					}
					if(i>1460)
					{
						i=1460;
					}
					i=hex_bcd(i);
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 1://发数据
					i=Terminal_Ram->ReqTxByte;
					x=Terminal_Ram->CopyTxByte;
					y=Terminal_Ram->CopyTxCount;
					if((i>LEN_UART1Tx)||(i>x)||(i==0))
					{//可发送长度错误,放弃本帧
						UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->Task=7;//0=接收
						GPRS_3763_NextATCount();//下1个AT
						return;
					}
					Terminal_Ram->CopyTxByte=x-i;//还需发送字节
					Terminal_Ram->CopyTxCount=y+i;//已发送计数
					
					UART1Ctrl->TxByte=i;
			    UART1Ctrl->TxCount=LEN_ATTx+y;
				#if LOG_1376_3==1//0=没,1=有
					LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
				#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
					
					Start_UARTn_Tx(GPRSPORT);//启动UART口发送
					
					x=MRR((u32)&UART1Ctrl->RxTxByte,4);
					x+=i;
					MWR(x,(u32)&UART1Ctrl->RxTxByte,4);//通信流量
					UART1Ctrl->ATTask=2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					return;
				default:
					UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
					break;
			}
			break;
#endif
		case AT_MYFTP://FTP文件下载
		 	if(UART1Ctrl->LinkTask!=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
		//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
		//下载完成
			FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);//下载文件计数;0-x,=MaxFileNum关FTP,=(MaxFileNum+1)返回帧,=(MaxFileNum+2)下载完成显示,=(MaxFileNum+3)结束下载显示
			if(FTPFileCount>(MaxFileNum+1))//最大下载文件个数
			{
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
				UART1Ctrl->FTPLinkCount=0;//FTP尝试连接计数
				UART1Ctrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
			Comm_Ram->DisplayAllErrorTimer=10;//FTP文件下载显示秒定时器
			x=UART1Ctrl->FTPErr;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			if((x!=0)||(FTPFileCount==(MaxFileNum+1)))
			{
				if(x==1)
				{//服务器无法登录
					y=0xFFFFFFFE;//FTP 服务器无法登录，填写0xFFFFFFFEH
				}
				else
				{
					if(x==2)
					{//服务器中找不到指定文件
						y=0xFFFFFFFD;//FTP 服务器中找不到指定文件，填写0xFFFFFFFDH
					}
					else
					{
						for(i=0;i<MaxFileNum;i++)
						{
							x=MRR(ADDR_FTPFileFlags+5+i,1);
							if(x==5)//5=文件错误
							{
								break;
							}
						}
						if(i<MaxFileNum)
						{//有文件错误
							y=0xFFFFFFFF;//检查发现文件错误，填写0xFFFFFFFFH
						}
						else
						{
							y=0;
						}
					}
				}
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//FTP文件下载且已返回帧标志
				
				p8rx=(u8 *)ADDR_DATABUFF;
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MR(ADDR_DATABUFF,ADDR_AFN0FF1_Rx,256);//恢复FTP接收帧的前256字节
				MR((u32)p8tx,ADDR_AFN0FF1_Tx,18);//恢复FTP返回帧的前18字节
				MWR(y,((u32)p8tx)+18,4);//返回标识4字节
				i=4+4;//返回长度
				i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
				p8tx[1]=i;
				p8tx[2]=i>>8;
				i=p8rx[1]+(p8rx[2]<<8);
//				Uart_376_AUX(GPRSPORT,((u32)p8rx)+6+i-6);//附加信息(EC,TP),暂等待单帧数据发送
//				Uart_376_CS((u32)p8tx);//和校验计算,同时加结束0x16;修改长度字节的标志位
				
				GPRS_3763_NextATCount();//下1个AT
				UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收

			#if (USER/100)==5//上海
				i=MRR(ADDR_AFN0FF97,1);//1byte 命令发起标志:0=不是F97命令,1=是F97命令发起
				if(i!=0)
				{//是F97命令发起,无返回帧
					UART1Ctrl->Task=0;//0=接收
				}
				//升级后终端软件版本号	ASCII	10
				MC(0,ADDR_DATABUFF,10);
				p8rx=(u8*)(ADDR_DATABUFF);
				p8rx[0]='Z';
				p8rx[1]='Z';
				p8rx[2]=((SVER>>4)&0xf)+0x30;
				p8rx[3]=((SVER>>0)&0xf)+0x30;
				//发生时间：分时日月年	见附录A.15	5
				MR(ADDR_DATABUFF+10,ADDR_TYMDHMS+1,5);
				MW(ADDR_DATABUFF,ADDR_AFN0FF98+10,15);
			#endif
				return;
			}
	#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
			TxByte=GPRS_FTP_Tx();//GPRS_FTP_Tx发送
	#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
    		p32=(u32 *)(ADDR_E1_ENTRY);
    		TxByte=(*(u32(*)())p32[106])();//GPRS_FTP_Tx发送
			}
	#endif
			break;
			
		case AT_SMS_NOLINK://短信服务没连接前
		case AT_SMS_LINK://短信服务
		#if LEN_UART11Ctrl==0
			GPRS_3763_NextATCount();//下1个AT
		#else
			TxByte=GPRS_SMS_Tx();//GPRS_SMS_Tx发送;返回发送字节数
		#endif
			break;
			
		case AT_LOOP://AT循环,在线控制
			if(UART1Ctrl->LinkCtrl==0)//连接控制0=不连接,1=连接
			{
				if(UART1Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					UART1Ctrl->LinkTask=102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					UART1Ctrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//2 心跳秒定时器
				}
				if(UART1Ctrl->LinkTask==103)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					return;
				}
			}
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
			UART1Ctrl->ATCount=AT_CSQ_LINK;
			break;
		case AT_MYNETCLOSE://关闭socket连接
		  if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
			{
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		  break;
		case AT_MYNETACTOFF://关闭网络连接
		  if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
			{
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount=AT_MYNETACT;
				return;
			}
		  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		  break;
		case AT_ACCEPT://集中器做服务器时，处理模块上报的主站IP
		  if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
			{
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}
		  TxByte=CopyString((u8 *)"AT$MYNETACCEPT=0,0,0\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[15]=0x30+(UART1Ctrl->res4&TCP_1_SOCKET_MASK);
		  break;
		case AT_CGDCONT://集中器做服务器时，处理模块上报的主站IP
		  if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
			{
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}//AT+CGDCONT=1,"IP"," 
		  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			
			i=MRR(ADDR_4520_2+1,1);//取备用通道数
			if(i==0)
			{//备用通道数=0
				p8=Get_Element((u8 *)ADDR_4500_2,6,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{//没找到
					CopyString((u8 *)"cmnet\x0",(u8 *)ADDR_DATABUFF);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
			}
			else
			{//有备用通道数
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//取备用通道数
				i=Comm_Ram->CSPN;//0=空,1=中国移动通信-----china mobile  CMCC,2=中国联通通讯-----china unicom  CUCC,3=中国电信  ------CHINA TELECOM  CTCC		
				switch(i)
				{
					default:
					case 1://1=中国移动通信-----china mobile  CMCC
						n=0;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
					case 2://2=中国联通通讯-----china unicom  CUCC
						n=2;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
					case 3://3=中国电信  ------CHINA TELECOM  CTCC	
						n=1;//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
						break;
#if (USER/100)==17
          case 54://54: 江苏专网
						 n=i;
					break;
#endif
				}
				p8+=2;
				y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				for(i=0;i<y;i++)//略过已经发送的APN参数
				{
				  p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				for(;i<x;i++)//此时i已经=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				{
					if(p8[3]==n)//CMCC（移动） = 0，CTCC（电信） = 1，CUCC（联通） = 2
					{
						#if COMPARE_2G_3G_4G==1 //设置APN时比对2G，3G，4G
						y=p8[5];//2G = 0，3G = 1， 4G = 2， 未知 = 255 ， 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						#else
										//设置APN时不比对2G，3G，4G
						#endif
						{
							break;
						}
					}
					p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				UART1Ctrl->res3=i;//用于远程模块连接使用45200200参数时，已经试过的参数计数
				
				if(i<x)
				{//找到
					p8+=6;
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					return;
				}
			}
			TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;	
			#if LOG_1376_3==1//0=没,1=有
			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
		  break;
		case AT_ATDT://设置拨号方式，进行拨号
		    UART1Ctrl->ATTask=1;
		    UART1Ctrl->LinkTask=89;
				UART1Ctrl->ATCount=AT_CSQ_LINK;//查信号强度
				return;
//			TxByte=CopyString((u8 *)"ATD*99***1#\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//			#if LOG_1376_3==1//0=没,1=有
//			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//			#endif
//			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//			#endif
//		  break;
		default:
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//			#if LOG_1376_3==1//0=没,1=有
//			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//			#endif
//			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//			#endif
			break;
	}
	if(TxByte!=0)
	{
		//启动UART1口发送
		UART1Ctrl->BpsCtrl=0x3+(9<<5);//速率57.6K 
//#ifndef LINUX
//		Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
//#else
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
		UART1Ctrl->TxByte=TxByte;//发送字节数(半字对齐)
    UART1Ctrl->TxCount=0;
	#if LOG_1376_3==1//0=没,1=有
		switch(p32[4*AT_No])//AT代号
		{
			case AT_CSQ_LINK://读信号连接后(每秒1次)
			case AT_MYNETOPEN_LINK://连接检查(每秒1次)
			case AT_MYNETREAD://读数据
			  LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				break;
			default:
				LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				break;
		}
	#endif
	#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
		switch(p32[4*AT_No])//AT代号
		{
			case AT_CSQ_LINK://读信号连接后(每秒1次)
			case AT_MYNETOPEN_LINK://连接检查(每秒1次)
			case AT_MYNETREAD://读数据
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				break;
			default:
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				break;
		}
	#endif
#if PPP_ENABLE==1
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			  log_out((u32)p8tx,TxByte, 'c');
#endif
#endif
		Start_UARTn_Tx(GPRSPORT);//启动UART口发送
		UART1Ctrl->ATTask=2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	}
}

/*
void GPRS_URC_Rx(void)//主动上报接收
{
	u8 *p8;
	u32 *p32;
	u32 Addr;
	u32 ClrByte;
	
	//数据到来主动上报 $MYURCREAD
	Addr=ATRxSearch((u32)((u8*)"$MYURCREAD"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
	if(Addr!=0)
	{
		ClrByte=10;//需清0字节数
	ClrURC:
		Addr-=(ADDR_UART1IntATRx+6);
		p8=(u8*)(ADDR_UART1IntATRx+6);
		while(ClrByte!=0)
		{
			if(Addr==0)
			{
				Addr=(LEN_UART1IntATRx-6);
			}
			Addr--;
			p8[Addr]=0;
			ClrByte--;
		}
		p32=(u32*)(&GPRS_3763_AT_Command_Lib);
		Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//AT接收第一个字节超时定时值(秒)
		return;
	}
	//链路断开主动上报
	Addr=ATRxSearch((u32)((u8*)"$MYURCCLOSE: 0"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
	if(Addr!=0)
	{
		WarningOccur(GPRSPORT,(u8 *)"GPRS链路被主站断开");//告警发生,入口告警代码和告警字符串
		UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
		UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		ClrByte=10;//需清0字节数
		goto ClrURC;
	}
	//网络连接状态主动上报
	Addr=ATRxSearch((u32)((u8*)"$MYURCACT: 0,0"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
	if(Addr!=0)
	{
		WarningOccur(GPRSPORT,(u8 *)"GPRS网络主动断开");//告警发生,入口告警代码和告警字符串
		UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
		UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		ClrByte=10;//需清0字节数
		goto ClrURC;
	}
	//客户端连接主动上报
	
	
	//FTP连接断开主动上报
	
	
}
*/
void GPRS_3763_AT_Rx(void)//AT命令接收
{
	u32 i;
	u32 x;
	u32 y;
	u32 AT_No;
	u32 RxByte;
	u32 Addr;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	u32 p0;
	u32 p1;
#if LOG_1376_3||DEBUG_GPRStoRS232
	u8* p8LOG;
#endif
	
#ifdef LINUX
	ms_Type * ms;
	ms=Get_ms();
	ms->linux_uart_receive(GPRSPORT);
#endif

	DMA_UARTn_RX_Pointer(GPRSPORT);//计算DMA_UARTn接收指针值(p0)
	
	AT_No=UART1Ctrl->ATCount;//AT计数
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT计数
	}
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	if(p16fifo[0]!=p16fifo[2])
	{//有字节接收
		p16fifo[2]=p16fifo[0];
		Terminal_Ram->AT_MS_Timer=1000/10;//AT接收第一个字节后超时定时值
	}
//	if(Terminal_Ram->AT_MS_Timer!=0)
//	{//接收没完成
//		return;
//	}
//	UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收

	#if (USER/100)==17
	i=GPRS_URC_Rx();//主动上报接收，默认返回1
	if(i)//如果返回的是1则返回
	{
		return;
	}
	#endif
	
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT://波特率检查
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=2;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{//重发本AT
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				if(UART1Ctrl->LinkSubTask>=3)//尝试AT发送计数
				{//中兴MC8332模块当自动速率时头2次的AT无响应(用于检测),第3次才响应
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					switch(UART1Ctrl->SubTask)//子任务
					{
						case 0://速率57600
						case 1://速率115200
						case 2://速率38400
						case 3://速率19200
						case 4://速率9600
						case 5://速率4800
						case 6://速率2400
						//case 7://速率1200
							UART1Ctrl->SubTask++;//子任务
							return;
						//case 10://下1个AT
						default:
						#if (USER/100)==0//用户标识,电科院测试不支持AT+IPR
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
							GPRS_3763_NextATCount();//下1个AT
						#else
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
						#endif
							return;
					}
				}
			}
			else
			{//接收到OK
			  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			  if(UART1Ctrl->SubTask<10)//用于同步波特率
			  {
#if (USER/100)==17
					UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;//等待模块上报^SRVST: 2
					UART1Ctrl->SubTask=10;
#else
          UART1Ctrl->SubTask+=10;//子任务
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
#endif
			  }
				else if(UART1Ctrl->SubTask>10)//子任务
				{//已经下发AT+IPR波特率已经
				  if(UART1Ctrl->BpsCtrl!=0x3+(9<<5))
				  {//只有波特率不为57600时，才去初始化串口
				    UART1Ctrl->BpsCtrl=0x3+(9<<5);
          //#ifndef LINUX 
            Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
          //#else
          //    ms->linux_Init_UARTn(GPRSPORT);
          //#endif
          }
					GPRS_3763_NextATCount();//下1个AT
				}
			}
			return;
		case AT_MYTYPE://查询远程通信模块类型
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Comm_Ram->RemoteWirelessModuleType=1;//GPRS模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			Addr=ATRxSearch((u32)((u8*)"$MYTYPE:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				x=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(x!=0)
				{
					p8s=(u8*)x;
					i = ASCII_BCD(x,2);
//					i=((p8s[0]-0x30)<<4)+(p8s[1]-0x30);
					Comm_Ram->RemoteWirelessModuleType=i;//GPRS模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				}
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
			{
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				break;
			}
			GPRS_3763_NextATCount();//下1个AT
			break;
			
		case AT_MYGMR://获取模块软件版本
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
			#if MainProtocol==376
				//厂商代号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_AFN09F9,4);
				//模块型号，ASCII，8字节
				MC(0,ADDR_DATABUFF,8);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4,8);
				//软件版本号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8,4);
				//软件发布日期，日月年，3字节
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				i=ASCII_BCD(ADDR_DATABUFF,6);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
				MWR(i,ADDR_AFN09F9+4+8+4,3);
				Data_Inverse(ADDR_AFN09F9+4+8+4,3);//数据倒序(高低字节调换),入口Le>=2
				//硬件版本号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3,4);
				//硬件发布日期，日月年，3字节
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				i=ASCII_BCD(ADDR_DATABUFF,6);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
				MWR(i,ADDR_AFN09F9+4+8+4+3+4,3);
				Data_Inverse(ADDR_AFN09F9+4+8+4+3+4,3);//数据倒序(高低字节调换),入口Le>=2		
				
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
			#endif
			#if MainProtocol==698
//版本信息∷=structure
//{
//2 厂商代码       visible-string(SIZE (4))，
//8 软件版本号     visible-string(SIZE (4))，
//14 软件版本日期   visible-string(SIZE (6))，
//22 硬件版本号     visible-string(SIZE (4))，
//28 硬件版本日期   visible-string(SIZE (6))，
//36 厂家扩展信息   visible-string(SIZE (8))
//}
				//厂商代号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_4500_5+4,4);
				//模块型号，ASCII，8字节
				MC(0,ADDR_DATABUFF,8);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_4500_5+38,8);
				//软件版本号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_4500_5+10,4);
				//软件发布日期，日月年，3字节
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_4500_5+16,6);
				//硬件版本号，ASCII，4字节
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				MW(ADDR_DATABUFF,ADDR_4500_5+24,4);
				//硬件发布日期，日月年，3字节
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				MW(ADDR_DATABUFF,ADDR_4500_5+30,6);
//				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
//				{//==0,
//          i=Compare_DL698DataString((u8*)ADDR_4500_5+38,(u8*)"JS18GZYT",8);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//					if(!i)
//					{//等待SVRST倒计时已经为0
//						UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//					  UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
//						return;
//					}
//				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
			#endif
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_CPIN://卡检查
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=3;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"READY"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_MYCCID://获取SIM卡序列号ICCID
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"$MYCCID"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(!Addr)
			{
			  Addr=ATRxSearch((u32)((u8*)"$myccid"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			}
			if(Addr!=0)
			{
				LineATIntRx(Addr+3,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
		#if MainProtocol==698
				p8s=(u8*)ADDR_4500_7;//公网通信模块1;7 SIM卡的ICCID
				//visible-string(SIZE(20))
				p8s[0]=DataType_visible_string;
				p8s[1]=20;
				MW(ADDR_DATABUFF,(u32)p8s+2,20);
		#endif
		#if MainProtocol==376
			#if (USER/100)!=12//非四川
				//四川要求AFN09F9的ICCID项填写SIM卡手机号码。
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3+4+3,20);
			#endif
			#if (USER/100)==12//四川
				//四川扩展AFN0CF2040填写ICCID
//				MW(ADDR_DATABUFF,ADDR_AFN0CF16+8,20);
				MW(ADDR_DATABUFF,ADDR_AFN0CF2040+7,20);
			#endif
			#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"获取SIM卡ICCID");//告警发生,入口告警代码和告警字符串
			#endif
		#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
#if (USER/100)==17
    if(UART1Ctrl->LinkSubTask>=(NUM_ReATTx))//尝试AT发送计数
		{
		  GPRS_3763_NextATCount();//下1个AT
			return;
		}
#endif
			goto ReTxAT;//重发本AT
		case AT_CNUM://取本机号码
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			p8d=(u8*)(ADDR_MobilePhoneNumber);
			p8d[0]=0;
			Addr=ATRxSearch((u32)((u8*)"+CNUM:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					LineATIntRx(Addr,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
					p8s=(u8*)(ADDR_DATABUFF);
					if(p8s[0]=='"')
					{
						p8s++;
						for(i=0;i<19;i++)
						{
							x=p8s[i];
							if(x=='"')
							{
								break;
							}
							p8d[i]=x;
						}
						p8d[i]=0;
					}
				}
			#if MainProtocol==698
				p8s=(u8*)(ADDR_MobilePhoneNumber);
				p8d=(u8*)ADDR_4500_10;
				//visible-string(SIZE(16))
				p8d[0]=DataType_visible_string;
				for(i=0;i<16;i++)
				{
					if(p8s[i]==0)
					{
						break;
					}
					p8d[2+i]=p8s[i];
				}
				p8d[1]=i;
			#endif
		#if MainProtocol==376
			#if (USER/100)==12//四川
				//四川要求AFN09F9的ICCID项填写SIM卡手机号码。
				for(i=0;i<20;i++)
				{
					if(p8d[i]==0)
					{
						break;
					}
				}
				MC(0x30,ADDR_AFN09F9+4+8+4+3+4+3,(20-i));
				MW(ADDR_MobilePhoneNumber,ADDR_AFN09F9+4+8+4+3+4+3+(20-i),i);//写SIM卡手机号码
				if(i>8)
				{//四川扩展6个字节BCD码电话号码
					y=ASCII_BCD(ADDR_MobilePhoneNumber+i-8,8);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
					MWR(y,ADDR_AFN0CF2040+1,4);//写SIM卡手机号码
					y=ASCII_BCD(ADDR_MobilePhoneNumber,(i-8));//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
					MWR(y,ADDR_AFN0CF2040+1+4,2);//写SIM卡手机号码
					Data_Inverse(ADDR_AFN0CF2040+1,6);//数据倒序(高低字节调换),入口Le>=2
				}
				else
				{
					y=ASCII_BCD(ADDR_MobilePhoneNumber,i);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
					MWR(y,ADDR_AFN0CF2040+1,4);//写SIM卡手机号码
					MWR(0,ADDR_AFN0CF2040+1+4,2);//写SIM卡手机号码
					Data_Inverse(ADDR_AFN0CF2040+1,6);//数据倒序(高低字节调换),入口Le>=2
				}		
			#endif
		#endif
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->LinkSubTask++;//尝试AT发送计数
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					break;
				}
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			GPRS_3763_NextATCount();//下1个AT
			break;
		case AT_CIMI://查国际移动台标识请求IMSI
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(Addr!=0)
			{
				p1=p16fifo[1];
				p8s=(u8*)(ADDR_UART1IntATRx+10);
				while((p8s[p1]==0x0d)||(p8s[p1]==0x0a))
				{
					p1++;
					if(p1>=(LEN_UART1IntATRx-10))
					{
						p1-=(LEN_UART1IntATRx-10);
					}
				}
				Addr=ADDR_UART1IntATRx+10+p1;
				LineATIntRx(Addr,ADDR_DATABUFF,19);//取AT中断接收数据线性放在缓冲中
				p8s=(u8*)(ADDR_DATABUFF);
				p8d=(u8*)(ADDR_MobilePhoneNumber+20);
				for(i=0;i<19;i++)
				{
					if((p8s[i]==0x0d)||(p8s[i]==0x0a))
					{
						break;
					}
					p8d[i]=p8s[i];
				}
				p8d[i]=0;
			#if MainProtocol==698
				p8s=(u8*)(ADDR_DATABUFF);
				p8d=(u8*)(ADDR_4500_8);//公网通信模块1;8 IMSI
				//visible-string(SIZE(20))
				p8d[0]=DataType_visible_string;
				p8d[1]=19;
				p8d+=2;
				for(i=0;i<19;i++)
				{
					if((p8s[i]==0x0d)||(p8s[i]==0x0a))
					{
						break;
					}
					p8d[i]=p8s[i];
				}
			#endif
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			GPRS_3763_NextATCount();//下1个AT
			break;
/*
		case AT_CSPN://读取SIM卡服务运营商名称
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=4;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+CSPN: \""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//中国移动通信-----china mobile  CMCC
//中国联通通讯-----china unicom  CUCC
//中国电信  ------CHINA TELECOM  CTCC	
				x=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				p8s=(u8*)(ADDR_DATABUFF);
				for(i=0;i<x;i++)
				{
					if(p8s[i]=='"')
					{
						p8s[i]=0;
						break;
					}
				}
				i=(p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|(p8s[3]<<0);
				switch(i)
				{
					case ('C'<<24)|('M'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
						break;
					case ('C'<<24)|('U'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=2;//2=中国联通通讯-----china unicom  CUCC
						break;
					case ('C'<<24)|('T'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=3;//3=中国电信  ------CHINA TELECOM  CTCC
						break;
					default:
						Comm_Ram->CSPN=255;//255=未知
						break;
				}
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			}
			
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			GPRS_3763_NextATCount();//下1个AT
			break;
*/
#if (USER/100)!=0//不是电科院测试(1376.3标准无此命令)
		case AT_COPS://读取SIM卡服务运营商名称
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=4;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+COPS:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
//中国移动通信-----china mobile  CMCC   46000 46002 46004 46007 46008
//中国联通通讯-----china unicom  CUCC   46001 46006 46009 46010
//中国电信  ------CHINA TELECOM  CTCC	 46003 46005 46011 46012
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					x=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
					i=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
					switch(i)
					{
						case 46000:
						case 46002:
						case 46004:
						case 46007:
						case 46008:
							Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
							WarningOccur(GPRSPORT,(u8*)"CMCC");//告警发生,入口告警代码和告警字符串
							break;
						case 46001:
						case 46006:
						case 46009:
						case 46010:
							Comm_Ram->CSPN=2;//2=中国联通通讯-----china unicom  CUCC
							WarningOccur(GPRSPORT,(u8*)"CUCC");//告警发生,入口告警代码和告警字符串
							break;
						case 46003:
						case 46005:
						case 46011:
						case 46012:
							Comm_Ram->CSPN=3;//3=中国电信  ------CHINA TELECOM  CTCC
							WarningOccur(GPRSPORT,(u8*)"CTCC");//告警发生,入口告警代码和告警字符串
							break;
						default:
#if MainProtocol==376
              Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
							WarningOccur(GPRSPORT,(u8*)"未知运营商");//告警发生,入口告警代码和告警字符串
#endif
#if MainProtocol==698
              Comm_Ram->CSPN=0;
#endif
							break;
					}
				}
#if MainProtocol==698
        i=Comm_Ram->CSPN;
        if(i==0)
        {
          p8d=(u8*)ADDR_DATABUFF;
          p8s=(u8*)(ADDR_4500_8);//公网通信模块1;8 IMSI
          MR(ADDR_DATABUFF,(u32)p8s+2,5);
          p8d[5]=0;
          i=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
          switch(i)
          {
            case 46000:
            case 46002:
            case 46004:
            case 46007:
            case 46008:
              Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
              WarningOccur(GPRSPORT,(u8*)"CMCC");//告警发生,入口告警代码和告警字符串
              break;
            case 46001:
            case 46006:
            case 46009:
            case 46010:
              Comm_Ram->CSPN=2;//2=中国联通通讯-----china unicom  CUCC
              WarningOccur(GPRSPORT,(u8*)"CUCC");//告警发生,入口告警代码和告警字符串
              break;
            case 46003:
            case 46005:
            case 46011:
            case 46012:
              Comm_Ram->CSPN=3;//3=中国电信  ------CHINA TELECOM  CTCC
              WarningOccur(GPRSPORT,(u8*)"CTCC");//告警发生,入口告警代码和告警字符串
              break;
            default:
              Comm_Ram->CSPN=0;
              break;
          }
          i=Comm_Ram->CSPN;
          if(i==0)
          {
            p8s=(u8*)ADDR_4500_7;//公网通信模块1;7 SIM卡的ICCID
            MR(ADDR_DATABUFF,(u32)p8s+2,6);
            p8d[6]=0;
            i=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
            //四川要求376终端的ICCID项填写SIM卡手机号码，698终端未知
            switch(i)
            {
              case 898600:
              case 898602:
              case 898604:
              case 898607:
              case 898608:
                Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
                WarningOccur(GPRSPORT,(u8*)"CMCC");//告警发生,入口告警代码和告警字符串
                break;
              case 898601:
              case 898606:
              case 898609:
              case 898610:
                Comm_Ram->CSPN=2;//2=中国联通通讯-----china unicom  CUCC
                WarningOccur(GPRSPORT,(u8*)"CUCC");//告警发生,入口告警代码和告警字符串
                break;
              case 898603:
              case 898605:
              case 898611:
              case 898612:
                Comm_Ram->CSPN=3;//3=中国电信  ------CHINA TELECOM  CTCC
                WarningOccur(GPRSPORT,(u8*)"CTCC");//告警发生,入口告警代码和告警字符串
                break;
              default:
                Comm_Ram->CSPN=1;//1=中国移动通信-----china mobile  CMCC
                WarningOccur(GPRSPORT,(u8*)"未知运营商");//告警发生,入口告警代码和告警字符串
                break;
            }
          }
        }
#endif
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			GPRS_3763_NextATCount();//下1个AT
			break;
#endif	
			//以下几个参数设置失败时不关模块,以使能收短信
		case AT_MYNETCON_APN://设置APN
		case AT_MYNETCON_USERPWD://设置USERPWD:用户名和密码
		case AT_MYNETSRV://设置服务参数
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(Addr!=0)
			{
			#if (USER/100)==13//黑龙江
				if(p32[4*AT_No]==AT_MYNETCON_APN)
				{
					WarningOccur(GPRSPORT,(u8 *)"设置APN");//告警发生,入口告警代码和告警字符串
				}
				if(p32[4*AT_No]==AT_MYNETCON_USERPWD)
				{
					WarningOccur(GPRSPORT,(u8 *)"设置用户名密码");//告警发生,入口告警代码和告警字符串
				}
			#endif
			  if(p32[4*AT_No]==AT_MYNETSRV)
				{
					i=UART1Ctrl->TCPUDP;
					i&=0x30;
					if(UART1Ctrl->TCPUDP&0x80)
					{//UDP
						if(i==0x00)
						{//=2服务器模式=0混合模式=1客户机模式
							UART1Ctrl->SubTask=4;
						}
					}
					else
					{//TCP
						if(i==0x00)
						{//=2服务器模式=0混合模式=1客户机模式
							if(!(UART1Ctrl->res4&TCP_1_LISTEN_OPEN))
							{//侦听端口还没有打开
							   if(UART1Ctrl->SubTask!=2)
								{
									UART1Ctrl->SubTask=2;
									UART1Ctrl->LinkSubTask=0;
									p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
									break;
								}
							} 
						}
					}
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			#if (PPP_ENABLE==1)//使用PPP拨号建立连接
			if(p32[4*AT_No]==AT_MYNETCON_USERPWD)
			{
       UART1Ctrl->ATCount=AT_CGDCONT;//设置isp接入网关
		   break;
			}
			else
      #endif
			{
			  GPRS_3763_NextATCount();//下1个AT
			}
			break;
			
		case AT_CSQ_NOLINK://读信号没连接前
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=5;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+CSQ: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
				GPRS_Signal(ADDR_DATABUFF);//GPRS信号强度0-4
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				if((Terminal_Ram->SignaldBm>=5)&&(Terminal_Ram->SignaldBm!=0x99))
				{//模块测到信号,若没信号则等待120秒
					GPRS_3763_NextATCount();//下1个AT
				}
				else
				{
					if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//GPRS等待信号强度查询(连接前)定时
					{
						WarningOccur(GPRSPORT,(u8 *)"远程无线无信号");//告警发生,入口告警代码和告警字符串
						UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					}
				}
			#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"远程无线读信号");//告警发生,入口告警代码和告警字符串
			#endif
				return;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			goto ReTxAT;//重发本AT
		case AT_CREG://获取当前网络注册状态
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		#if (USER/100)==13//黑龙江
			Addr=ATRxSearch((u32)((u8*)"+CGREG:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
		#else
			Addr=ATRxSearch((u32)((u8*)"+CREG:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
		#endif
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					i=MRR(Addr,1);
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					switch(i)
					{
						//case '0'://没有注册，ME现在并没有在搜寻要注册的新的运营商
						//case '4'://未知原因
						default:
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待注册定时
							{
								WarningOccur(GPRSPORT,(u8 *)"远程无线注册失败");//告警发生,入口告警代码和告警字符串
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '1'://注册了本地网络
							GPRS_3763_NextATCount();//下1个AT
							break;
						case '2'://没有注册，但MS正在搜寻要注册的新的运营商
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待注册定时
							{
								WarningOccur(GPRSPORT,(u8 *)"远程无线注册搜寻超时");//告警发生,入口告警代码和告警字符串
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '3'://注册被拒绝
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待注册定时
							{
								WarningOccur(GPRSPORT,(u8 *)"远程无线注册被拒绝");//告警发生,入口告警代码和告警字符串
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '5'://注册了漫游网络
							WarningOccur(GPRSPORT,(u8 *)"远程无线注册漫游网络");//告警发生,入口告警代码和告警字符串
							GPRS_3763_NextATCount();//下1个AT
							break;
					}
					break;
				}
			}
			goto ReTxAT;//重发本AT
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://查询注册网络类型：2G/3G/4G
//命令
//AT$MYSYSINFO
//返回值
//<CR><LF>$ MYSYSINFOsysmode>，<MNC>
//<CR><LF>OK<CR><LF>
//注：
//（a）<sysmode>:当前注册的网络制式：
//   （1）0：No service
//   （2）2：2G（包括GPRS,CDMA）
//   （3）3：3G（包括WCDMA,TD—S CDMA,CDMA EVD0）
//   （4）4：4G（包括FDD—LTE, FDD—LTE）
//（b）<MNC>:网络运营商代码：
//   （1）00：注册失败
//   （2）01：China Mobile（中国移动）
//   （3）02：China Unicom（中国联通）
//   （4）03：China Telecom（中国电信）
//示例：
//AT$MYSYSINFO
//$MYSYSINFO：4，01  //表示当前注册在中国移动的4G网络下
//OK
		
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Comm_Ram->RemoteWirelessModuleType=1;//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			Addr=ATRxSearch((u32)((u8*)"$MYSYSINFO: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				p8d = (u8 *)Addr;
				i=p8d[0]-0x30;
//				i=0;//后面可能用到
//				for(x=0;x<3;x++)
//				{
//				  if(p8d[x]==',')
//					{
//					  break;
//					}
//					i<<=4;
//					i|=p8d[x]-0x30;
//				}
				switch(i)
				{
					//case 0://无服务
					default:
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 0://无服务
						Addr=ATRxSearch((u32)((u8*)"OK\r\n"));
						if(Addr)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						}
						if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
						{
		#if (USER/100)==17
							UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
							UART1Ctrl->ATCount=AT_ATE0;
							UART1Ctrl->SubTask=0;
							Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;
							UART1Ctrl->res3=0;//45200200中已经试图设置参数的个数
							UART1Ctrl->res4=0;//客户端和服务器选择的链路
							UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//远端客户机为连接时为bit1-bit0为0x3,实际socket只能为0或者1
	            Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
							#else
							GPRS_3763_NextATCount();//下1个AT					
		#endif
						}
						else
						{//等待返回正确注册网络类型：2G/3G/4G
							UART1Ctrl->LinkSubTask=0;//尝试AT发送计数											
							goto ReTxAT;
						}
						break;
					case 2://GSM/GPRS模式 2G
						//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 3://WCDMA模式 3G
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 4://17://LTE模式 4G
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						//Comm_Ram->RemoteWirelessModuleType=(1<<5);//b5=1
						break;
				}
				i=0;
				for(x=2;x<5;x++)
				{
				  if(p8d[x]==0x0D)
					{
					  break;
					}
					i<<=4;
					i|=p8d[x]-0x30;
				}
				switch(i)
				{
					case 0:
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
							{						
								GPRS_3763_NextATCount();//下1个AT
							}
							else
							{//等待返回正确注册网络类型：2G/3G/4G
								UART1Ctrl->LinkSubTask=0;//尝试AT发送计数											
								goto ReTxAT;
							}
							break;
					case 1: //1=中国移动通信china mobile
					case 2: //2=中国联通通讯china unicom
					case 3: //3=中国电信CHINA TELECOM
					default:
							Comm_Ram->CSPN=i;
							break;
#if (USER/100)==17
					case 0x754://?754: 注册上江苏电力1.8GHz精控业务网络
					case 0x854://?854: 注册上江苏电力 1.8GHz I、II区业务（除精控）网络
					case 0x954://?954: 注册上江苏电力 1.8GHz III、IV区业务网络
						 Comm_Ram->CSPN=54;
					break;
#endif //(USER/100)==17
				}

#if (USER/100)==15//
		   	i=p8d[0]-0x30;
				x=Comm_Ram->RemoteWirelessModuleSYSINFO;//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				Comm_Ram->RemoteWirelessModuleSYSINFO=i;//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				if(x!=i)
				{//注册网络变化(2G,3G,4G)
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->SubTask=0;//子任务
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					return;
				}
				i=p8d[3]-0x30;
        x=Comm_Ram->CSPN;
        Comm_Ram->CSPN=i;
        if(x!=i)
				{//注册网络变化(1=中国移动通信china mobile,2=中国联通通讯china unicom,3=中国电信CHINA TELECOM)
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->SubTask=0;//子任务
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					return;
				}
#endif
#if (USER/100)==9//部分河南市场的电信物联网卡只支持4G网络，如果没有注册到4G网络则在此等待“GPRSWaitTx_S_Timer”秒
        x=UART1Ctrl->LinkTask;
        if(x<100)
        {//只有未登录主站时才判断是否注册到4G，登录在线之后不判断
          x=Comm_Ram->CSPN;
          if(x==3)//河南市场的电信物联网卡有可能只支持4G网络
          {
            i=Comm_Ram->RemoteWirelessModuleSYSINFO;
            switch(i)
            {
              case 4:
                break;
              default:
                if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
                {
                  break;						
                }
                else
                {//等待返回正确注册网络类型：2G/3G/4G
                  UART1Ctrl->LinkSubTask=0;//尝试AT发送计数											
                  goto ReTxAT;
                }
            }
          }
        }
#endif

      }
    Addr=ATRxSearch((u32)((u8*)"OK\r\n"));
		if(Addr)
		{
			p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
		}
			GPRS_3763_NextATCount();//下1个AT
			break;
		case AT_MYNETACT://激活网络连接
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=6;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
			#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"激活网络连接");//告警发生,入口告警代码和告警字符串
			#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
			}
			else
			{
				Addr=ATRxSearch((u32)((u8*)"ERROR: 902"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr)
				{//已经激活PDP
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//修改已读指针
					UART1Ctrl->ATCount=AT_MYNETACTOFF;
					break;
				}
				Addr=ATRxSearch((u32)((u8*)"ERROR: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr)
				{//
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					i=MRR(ADDR_4520_2+1,1);//取备用通道数
			    if(UART1Ctrl->res3<i)//res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
			    {//还没有尝试完所有的APN和密码
					  UART1Ctrl->res3++;
						UART1Ctrl->ATCount=AT_MYNETCON_APN;
						return;
					}
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机,江苏的通讯参数中同一个运营商网络有不同的APN和USERPASS，没办法走下去
					return;
				}
				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待激活网络连接定时
				{
				  i=MRR(ADDR_4520_2+1,1);//取备用通道数
			    if(UART1Ctrl->res3<i)//res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
			    {//还没有尝试完所有的APN和密码
					  UART1Ctrl->res3++;
						UART1Ctrl->ATCount=AT_MYNETCON_APN;
						return;
					}
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
#if (USER/100)==17
          UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机,江苏的通讯参数中同一个运营商网络有不同的APN和USERPASS，没办法走下去
#else //(USER/100)==17
					//UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					GPRS_3763_NextATCount();//下1个AT;不关机可能错误能收短信修复
#endif
				}
				return;
			}
			break;
		case AT_MYIPFILTER://IP访问控制配置
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
			#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"IP访问控制配置");//告警发生,入口告警代码和告警字符串
			#endif
#if MainProtocol==376
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//修改已读指针
#elseif MainProtocol==698
        p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
#endif
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask++;//子任务
				if(UART1Ctrl->SubTask>3)//sub0确认工作模式，sub1删除所有防火墙，sub2设置主用IP，sub3设置备用IP
				{
					GPRS_3763_NextATCount();//下1个AT
				}
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_MYNETOPEN://开启服务
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=7;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				
				i=UART1Ctrl->TCPUDP;
				i&=0x30;
				switch(UART1Ctrl->SubTask)
				{
				  default:
				  case 0://客户端open socket
						//客户机模式
						UART1Ctrl->res4|=TCP_0_OPEN;  //TCP clinet  socket_open
						UART1Ctrl->res4&=~TCP_SELECT_SERVER;
						UART1Ctrl->NETOPENCount=0;
#if (USER/100)==17
			      Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//用于连续无通信主动关闭socket定时//23 GPRS被动激活模式连续无通信自动断线秒定时器
#endif
						if(i==0x00)
						{//混合模式
							if(UART1Ctrl->HostACK!=0)
							{

								UART1Ctrl->ATCount=AT_MYNETWRITE;
								return;
							}
						  UART1Ctrl->SubTask=1;
							return;
						}
						break;
					case 1://服务端open socket
						UART1Ctrl->res4|=TCP_1_LISTEN_OPEN;  //TCP server  socket_open
						UART1Ctrl->res4|=TCP_SELECT_SERVER;
					  break;
				}
				if(UART1Ctrl->HostACK!=0)
				{
				  UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					UART1Ctrl->ATCount=AT_MYNETWRITE;
					return;
				}
				WarningCancel(GPRSPORT);//告警取消,入口告警代码
				if((i==0x10)||(i==0x00))
				{//客户机和混合模式下发登陆帧
					if(UART1Ctrl->LinkTask<100)
					{
						UART1Ctrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					}
				}
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount=AT_CSQ_LINK;//读信号连接后
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F1+5,1);
				if(i==0x0)
				{
					i=1;//最小1分
				}
				i*=60;
				i+=60;//加60秒可能的主站发出定时误差
		#else
				//DL698.45
				i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(i)
				{
					i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
					i=(i>>8)+((i&0xff)<<8);
					if(i==0)
					{
						i=60;//最小1分
					}
				}
				else
				{
					i=60;//最小1分
				}
		#endif
				if((UART1Ctrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
				{//是服务器模式
					if((UART1Ctrl->TCPUDP&0x80)!=0)
					{//是UDP
						i=6000;//UDP初始等待主站登入定时60分
					}
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS心跳秒定时器
				}
			#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"远程无线开启服务");//告警发生,入口告警代码和告警字符串
			#endif
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
				GPRS_3763_NextATCount();//下1个AT
			}
			break;
			
		case AT_CSQ_NOLINK2://读信号没连接2	
		case AT_CSQ_LINK://读信号连接后
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
//			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT代号
			{
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			}
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT代号
			{
				LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			}
		#endif
			
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+CSQ: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
				GPRS_Signal(ADDR_DATABUFF);//GPRS信号强度0-4
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			goto ReTxAT;//重发本AT
		case AT_MYNETOPEN_LINK://查询网络连接
#if (USER/100)==17
			switch(UART1Ctrl->SubTask)
			{
				case 0:
				{
#endif
					Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return;
						}
					}
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;使用ADDR_DATABUFF
			#endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
				LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;使用ADDR_DATABUFF
			#endif
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//					i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket		
//					if(i&TCP_0_OPEN)
//					{
//						Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 3"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//						if(Addr!=0)
//						{
//							Addr++;
//						}
//					}
//					else if(i&TCP_1_LISTEN_OPEN)
//					{
//					  MC(0,ADDR_DATABUFF,64);
//						p8s=(u8*)ADDR_DATABUFF;
//						CopyString((u8 *)"$MYNETOPEN: 0",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//						p8s[12]=0x30+TCP_1_LISTEN;//修改socket值
//						Addr=ATRxSearch(ADDR_DATABUFF);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//					}
//					else if(i^TCP_1_SOCKET_MASK)
//					{
//					  MC(0,ADDR_DATABUFF,64);
//						p8s=(u8*)ADDR_DATABUFF;
//						CopyString((u8 *)"$MYNETOPEN: 0",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//						p8s[12]=0x30+(i&TCP_1_SOCKET_MASK);//(i&TCP_1_SOCKET_MASK);//修改socket值
//						Addr=ATRxSearch(ADDR_DATABUFF);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//					}
//					else
//					{
//						Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//						if(Addr!=0)
//						{
//							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//						}
//						GPRS_3763_NextATCount();//下1个AT
//						break;
//					}
					Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						Addr+=2;
					}
          p1=p16fifo[1];
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				#if MainProtocol==376
						MC(0,ADDR_AFN0CF4+1,16+6);
						//LocalIP     
						p16fifo[1]+=1;   //模块返回帧有差异,有带"号和不带"号的,兼容处理		
						Addr+=1;
						x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if((x-1)==Addr)
						{
							Addr = x;
						}
						x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址				
						y=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if((x<y)||(x==0))
						{
							x = y-1;
						}
						if(x>Addr)
						{
							x-=Addr;
							x-=1;
							if(x<16)
							{
								MR(ADDR_AFN0CF4+1,Addr,x);
								//显示本地IP
								CopyString((u8 *)"本IP ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								MR(ADDR_DATABUFF+5,Addr,x);
								MC(0,ADDR_DATABUFF+5+x,1);
								WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
								//local_port
								Addr+=x;
								Addr+=2;
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
								x=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
								if(x>Addr)
								{
									x-=Addr;
									x-=1;
									if(x<6)
									{
										MR(ADDR_AFN0CF4+1+16,Addr,x);
									}
								}
								
								CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								i=DataComp(ADDR_AFN0CF4+1,ADDR_DATABUFF,7);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
								if(i==0)
								{//断开连接
									Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
									if(Addr!=0)
									{
										p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
									}
									WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
									UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
									return;
								}
							}
						}
				#else
						//DL698
						MC(0,ADDR_MobilePhoneNumber+40,16+6);
						//LocalIP			
						p16fifo[1]+=1;   //模块返回帧有差异,有带"号和不带"号的,兼容处理		
						Addr+=1;
						x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if((x-1)==Addr)
						{
							Addr = x;
						}
						x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址				
						y=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if((x<y)||(x==0))
						{
							x = y-1;
						}
						if(x>Addr)
						{
							x-=Addr;
							x-=1;
							if(x<16)
							{
								MR(ADDR_MobilePhoneNumber+40,Addr,x);
								//显示本地IP
								CopyString((u8 *)"本IP ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								MR(ADDR_DATABUFF+5,Addr,x);
								MC(0,ADDR_DATABUFF+5+x,1);
								WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
								//local_port
								Addr+=x;
								Addr+=2;
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
								x=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
								if(x>Addr)
								{
									x-=Addr;
									x-=1;
									if(x<6)
									{
										MR(ADDR_MobilePhoneNumber+40+16,Addr,x);
									}
								}
								
								CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								i=DataComp(ADDR_MobilePhoneNumber+40,ADDR_DATABUFF,7);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
								if(i==0)
								{//断开连接
									Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
									if(Addr!=0)
									{
										p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
									}
									WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
									UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
									return;
								}
							}
						}
						
//						//用于客户机服务器混合模式下的处理
////						$MYNETOPEN: 2,"100.32.73.171",5100,"100.32.73.84","0.0.0.0","0.0.0.0",1,"0:0:0:0",0\r
////            $MYNETOPEN: 0,"100.32.73.171",5100,"100.32.73.84","0.0.0.0","0.0.0.0",0,"127.0.0.1",5100\r
//						p16fifo[1]=lastp16fifo;
//						
////						p8s=(u8*)(ADDR_UART1IntATRx+10+p16fifo[1]);测试代码
////						i=CopyString((u8 *)"$MYNETOPEN: 0,\r",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
//						
//						UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket		
//						UART1Ctrl->res4&=~TCP_1;//TCPserver通道建立
//						Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//						Addr2=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//						while(Addr<Addr2)
//						{
//							if(!Addr)
//							{//Addr==0
//								break;
//							}
//							p8s = (u8 *)Addr;
//							i=p8s[0]-0x30;
//							switch(i)
//							{
//								case 3://客户机socket
//									break;
//								case 0://服务器socket
//								case 1:
//									UART1Ctrl->res4&=~TCP_1_SOCKET_MASK;//更新socket
//					        UART1Ctrl->res4|=i;
//								  UART1Ctrl->res4|=TCP_1;//TCPserver通道建立
//									break;
//								case 2://侦听socket
//									break;
//								default:
//									break;
//							}
//				      Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//							Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//						}
						
						
				#endif
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						}
	#if (USER/100)==17
						p8s=(u8 *)ADDR_4500_5+4;
						if((p8s[0]==0x5A)&&(p8s[1]==0x58)&&(p8s[2]==0x47)&&(p8s[3]==0x44))//'ZXGD',中兴高达的模块回重复回两次
						{
							Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
							UART1Ctrl->SubTask=1;
							UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
							return;
						}								
						//	i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket		
					if(UART1Ctrl->res4&TCP_0_OPEN)
					{
						if(!UART1Ctrl->HostACK)
						{//上报定时器时间为0
							if(!Terminal_Ram->GPRSAutoDiscon_S_Timer)//用于连续无通信主动关闭socket定时//23 GPRS被动激活模式连续无通信自动断线秒定时器
							{//连续无通信定时器归零
								UART1Ctrl->ATCount=AT_MYNETCLOSE;
								return;
							}
						}
					}
						          
	#endif
						GPRS_3763_NextATCount();//下1个AT
						break;
					}
				   goto ReTxAT;//重发本AT
					 
#if (USER/100)==17
				}
				case 1:
					Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return;
						}
					}
					Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						}
						UART1Ctrl->SubTask=0;
						UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						GPRS_3763_NextATCount();//下1个AT
						break;
					}
					goto ReTxAT;//重发本AT
				default:
				  UART1Ctrl->SubTask=0;
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				  goto ReTxAT;//重发本AT
			}
			break;
#endif
		case AT_MYNETREAD://读取数据
			p1=p16fifo[1];//原p1
			if(!(UART1Ctrl->res4&TCP_SELECT_SERVER))
			{
			  Addr=ATRxSearch((u32)((u8*)"$MYNETREAD: 3,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			}
			else
			{
			  MC(0,ADDR_DATABUFF,64);
				p8s=(u8*)ADDR_DATABUFF;
			  CopyString((u8 *)"$MYNETREAD: 0,",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
        p8s[12]=0x30+(i&TCP_1_SOCKET_MASK);//修改socket值
				Addr=ATRxSearch(ADDR_DATABUFF);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			}
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				MC(0,ADDR_DATABUFF,64);
				i=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				if(i!=0)
				{
					RxByte=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
					i=GetATRxByte();//中断接收,计算当前p0对应p1还有未处理字节数
					if(i<(RxByte+4))//当字节数为0时原RxByte+6为空行0x0d0x0a加OK0x0d0x0a,现改为去掉空行OK0x0d0x0a,为二者兼空用RxByte+4
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							p16fifo[1]=p1;//原p1
						}
						else
						{
							Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
							if(Addr!=0)
							{
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							}
							UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
							GPRS_3763_NextATCount();//下1个AT

						}
						return;
					}
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					if(RxByte)
					{
						i=MRR((u32)&UART1Ctrl->RxTxByte,4);
						i+=RxByte;
						MWR(i,(u32)&UART1Ctrl->RxTxByte,4);//通信流量
					}
					
					x=p16fifo[1];
					while(x>=(LEN_UART1IntATRx-10))
					{
						x-=(LEN_UART1IntATRx-10);
					}
					
				#if LOG_1376_3==1//0=没,1=有
					if(RxByte!=0)
					{
						LOG(13763,0,GPRSPORT,(u8 *)ADDR_UART1IntATRx+10,x,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					}
				#endif
				#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					if(RxByte!=0)
					{
						LOGRS232(13763,0,GPRSPORT,(u8 *)ADDR_UART1IntATRx+10,x,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					}
				#endif
					
					p8s=(u8*)(ADDR_UART1IntATRx+10);
					p16=(u16*)(ADDR_UART1IntRx);
					y=p16[0];
					if(y>=(LEN_UART1IntRx-10))
					{
						y=0;
					}
					p8d=(u8*)(ADDR_UART1IntRx+10);
				#if LOG_1376_3||DEBUG_GPRStoRS232
					p8LOG=(u8*)ADDR_DATABUFF;
				#endif
					for(i=0;i<RxByte;i++)
					{
						p8d[y]=p8s[x];
				#if (LOG_1376_3||DEBUG_GPRStoRS232)
						if(RxByte<=LEN_DATABUFF)
						{
							p8LOG[0]=p8s[x];
							p8LOG++;
						}
				#endif
						x++;
						if(x>=(LEN_UART1IntATRx-10))
						{
							x=0;
						}
						y++;
						if(y>=(LEN_UART1IntRx-10))
						{
							y=0;
						}
					}
					p16fifo[1]=x;//修改已读指针
					p16[0]=y;//接收指针
				#if LOG_1376_3==1//0=没,1=有
					if(RxByte!=0)
					{
						//hex数据
						LOG_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
						p16fifo[2]=x;//修改已读指针
						//acii
						LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
					}
				#endif
				#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					if(RxByte!=0)
					{
						//hex数据
						LOGRS232_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
						p16fifo[2]=x;//修改已读指针
						//acii
						LOGRS232_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
					}
				#endif
					
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
			#if MainProtocol==698	
				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
			#endif
					if(RxByte==0)
					{
					  if((UART1Ctrl->Task!=2)||(UART1Ctrl->Task!=4))
						{
						  UART1Ctrl->res4&=~FORCE_TCP_1;//解锁当前socket
						}
						GPRS_3763_NextATCount();//下1个AT
					}
					else
					{
					  UART1Ctrl->res4|=FORCE_TCP_1;//锁定当前socket
					}
					return;
				}
			}
			else
			{
				Addr=ATRxSearch((u32)((u8*)"ERROR\r"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
        if(Addr)
				{//返回错误
					if(!(UART1Ctrl->res4&TCP_SELECT_SERVER))
					{//集中器主动通道
						UART1Ctrl->res4&=~TCP_0;//TCPclent通道建立
						UART1Ctrl->res4&=~TCP_0_OPEN;//TCPclent通道建立
					}
					else
					{
						UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket		
						UART1Ctrl->res4&=~TCP_1;//TCPserver通道建立
					}
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			    GPRS_3763_NextATCount();//下1个AT
					break;
				}
			}
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//接收没完成
				p16fifo[1]=p1;//原p1
				return;
			}
			
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
//			if((UART1Ctrl->TCPUDP&0x30)!=0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
//			{//不是服务器模式
//				goto ReTxAT;//重发本AT
//			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			GPRS_3763_NextATCount();//下1个AT
			break;
		case AT_MYNETWRITE://发送数据
			switch(UART1Ctrl->ATWriteTask)//AT数据发送任务:0=发命令,1=发数据
			{
				case 0://发AT头
					i=UART1Ctrl->res4;
				  if(!(i&TCP_SELECT_SERVER))
					{
					  Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 3,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
          }
					else
					{
						MC(0,ADDR_DATABUFF,64);
						p8s=(u8*)ADDR_DATABUFF;
						CopyString((u8 *)"$MYNETWRITE: 0,",p8s);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
						p8s[13]=0x30+(i&TCP_1_SOCKET_MASK);//修改socket值
						Addr=ATRxSearch(ADDR_DATABUFF);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					}
//					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return;
						}
					}
					UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					y=p16fifo[1];//y=原p1
					
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						MC(0,ADDR_DATABUFF,64);
								
						p0=p16fifo[0];//p0
						p1=p16fifo[1];//p1
						p8s=(u8*)(ADDR_UART1IntATRx+10);
						p8d=(u8*)(ADDR_DATABUFF);
						if(p0>=p1)
						{
							RxByte=p0-p1;
						}
						else
						{
							RxByte=(p0+(LEN_UART1IntATRx-10))-p1;
						}
						//取字符串
						i=0;
						while(RxByte!=0)
						{
							p8d[i]=p8s[p1];
							i++;
							if(i>=8)
							{//字符串太长错
								i=0;
							}
							p1++;
							if(p1>=(LEN_UART1IntATRx-10))
							{
								p1=0;
							}
							RxByte--;
						}
						p16fifo[1]=p1;
						
//						i=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
						if(i==0)
						{//不含0x0d0x0a的字符串长度
							if(Terminal_Ram->AT_MS_Timer!=0)
							{//接收没完成
								UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
								p16fifo[1]=y;//y=原p1
								return;
							}
						}
						else
						{
							i=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
							if(i!=0x0)
							{
								x=Terminal_Ram->CopyTxByte;//还需发送字节数
								if(x>1460)
								{
									x=1460;
								}
								if((i>=x)||(Terminal_Ram->AT_MS_Timer==0))//i>=还需发送字节数或接收超时
								{
						#if LOG_1376_3==1//0=没,1=有
							p16fifo[1]=y;//y=原p1
							LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
							p16fifo[1]=p1;
						#endif
						#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
							p16fifo[1]=y;//y=原p1
							LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
							p16fifo[1]=p1;
						#endif
									Terminal_Ram->ReqTxByte=i;//请求发送字节数
									UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
									UART1Ctrl->ATWriteTask=1;//AT数据发送任务:0=发命令,1=发数据
									Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//1分,GPRS发送数据等待超时秒定时器
									return;
								}
								else
								{
									UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
									p16fifo[1]=y;//y=原p1
									return;
								}
							}
						}
					}
					if(Terminal_Ram->GPRSWaitTx_S_Timer!=0)//2分,GPRS发送数据等待超时秒定时器
					{
						return;
					}
					else
					{
						if(UART1Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
						{
							WarningOccur(GPRSPORT,(u8 *)"远程无线等待发送超时");//告警发生,入口告警代码和告警字符串
						}
						else
						{
							WarningOccur(GPRSPORT,(u8 *)"远程无线登录主站失败");//告警发生,入口告警代码和告警字符串
						}
						UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					}
					break;
				case 1://发数据
					Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return;
						}
					}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
          UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
#if (USER/100)==17
          p8s=Get_Element((u8*)ADDR_4500_5,1,0,LENmax_4500_5);
					if((p8s[2]==0x48)&&(p8s[3]==0x57))//'HW'
					{
					  p8s=Get_Element((u8*)ADDR_4500_5,4,0,LENmax_4500_5);
						Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
						UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->ATWriteTask=2;//AT数据发送任务:0=发命令,1=发数据,2，等待上报用于江苏市场新联模块华为方案的特殊处理
						return;
					}
#endif
          
					UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
					if(Terminal_Ram->CopyTxByte!=0x0)//还需发送字节数
					{
						UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					}
					else
					{
						if(UART1Ctrl->Task==6)
						{//多帧数据正在发送
						#if MainProtocol==376
							UART1Ctrl->Task=4;//4=多帧数据续接收完成
						#else
							UART1Ctrl->Task=0;//0=接收
						#endif
						}
						else
						{
							UART1Ctrl->Task=0;//0=接收
						}
//						if(UART1Ctrl->HostACK>0)
//						{
//						  UART1Ctrl->res4|=FORCE_TCP_1;//锁定当前socket
//						}
//						else
//						{
//						  UART1Ctrl->res4&=~FORCE_TCP_1;//解锁当前socket
//						}
						GPRS_3763_NextATCount();//下1个AT
					}
					break;
#if (USER/100)==17
				case 2://发数据
				  Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//接收没完成
							return;
						}
					}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
					if(Terminal_Ram->CopyTxByte!=0x0)//还需发送字节数
					{
						UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					}
					else
					{
						if(UART1Ctrl->Task==6)
						{//多帧数据正在发送
						#if MainProtocol==376
							UART1Ctrl->Task=4;//4=多帧数据续接收完成
						#else
							UART1Ctrl->Task=0;//0=接收
						#endif
						}
						else
						{
							UART1Ctrl->Task=0;//0=接收
						}
						GPRS_3763_NextATCount();//下1个AT
					}
					break;
#endif
			}
			break;
		case AT_MYFTP://FTP文件下载
	#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
			GPRS_FTP_Rx();//GPRS_FTP_Rx接收
	#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
    		p32=(u32 *)(ADDR_E1_ENTRY);
    		(*(void(*)())p32[107])();//GPRS_FTP_Rx接收
			}
	#endif	
			break;
		case AT_SMS_NOLINK://短信服务没连接前
		case AT_SMS_LINK://短信服务
		#if LEN_UART11Ctrl!=0
			GPRS_SMS_Rx();//GPRS_SMS_Rx接收
		#endif
			break;
		case AT_MYNETCLOSE://关闭socket连接
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"$MYNETCLOSE: 3"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		  #endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			if(Addr!=0)
			{
			  UART1Ctrl->res4&=~TCP_0_OPEN;//TCP clinet  socket_close通道建立
				UART1Ctrl->res4&=~TCP_0;//TCP clinet link通道建立
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkSubTask=2;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_MYNETACTOFF://关闭网络连接
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		 #endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			if(Addr)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->ATCount=AT_MYNETACT;
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_ACCEPT://集中器做服务器时，处理模块上报的主站IP
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		 #endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			if(Addr)
			{
			  UART1Ctrl->res4|=TCP_1;//TCPserver通道建立
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->ATCount=AT_MYNETREAD;
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_CGDCONT://设置isp接入网关
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		  #endif
			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			if(Addr)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			  GPRS_3763_NextATCount();//下1个AT
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_ATDT://设置isp接入号码
		   UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			 return;
//		  Addr=ATRxSearch((u32)((u8*)"CONNECT\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//			if(Addr==0)
//			{
//				if(Terminal_Ram->AT_MS_Timer!=0)
//				{//接收没完成
//					return;
//				}
//			}
//			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//			Addr=ATRxSearch((u32)((u8*)"CONNECT"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//			#if LOG_1376_3==1//0=没,1=有
//			  LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
//		  #endif
//			#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
//					LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
//			#endif
//			if(Addr)
//			{
//				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//			  UART1Ctrl->ATCount=AT_CSQ_LINK;
//				break;
//			}
//			if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
//			{
//				i=MRR(ADDR_4520_2+1,1);//取备用通道数
//				if(UART1Ctrl->res3<i)//res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
//				{//还没有尝试完所有的APN和密码
//					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
//				  UART1Ctrl->SubTask=0;//子任务
//					UART1Ctrl->res3++;
//					UART1Ctrl->ATCount=AT_CGDCONT;
//					return;
//				}
//				return;
//			}
//			goto ReTxAT;//重发本AT
		case AT_HOLD://HOLD暂停
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			break;
		case AT_MYPOWEROFF://关机
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				Comm_Ram->UART1Timer=15000/10;//关机延时15s
#if (USER/100)==13//黑龙江
				WarningOccur(GPRSPORT,(u8 *)"模块正在关机");//告警发生,入口告警代码和告警字符串
#endif
			}
			else
			{
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
				}
				else
				{
					UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					Comm_Ram->UART1Timer=15000/10;//关机延时15s
				}
			}
			break;
		default:
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=否,1=是;调试:GPRS口收发数据打印到RS232口
			LOGRS232_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
			}
			else
			{
			ReTxAT://重发本AT
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				if(UART1Ctrl->LinkSubTask>=(NUM_ReATTx+1))//尝试AT发送计数
				{
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
				}
				return;
			}
			break;
	}
}


void GPRS_3763_AT_RxTx(void)//GPRS_3763模块AT接收发送
{
	u32 *p32;
	switch(UART1Ctrl->ATTask)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	{
		case 1://发送
			GPRS_3763_AT_Tx();//AT命令发送
			break;
		case 2://正在发送
			if(Wait_UARTn_Tx(GPRSPORT)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				p32=(u32*)(&GPRS_3763_AT_Command_Lib);
				switch(p32[4*UART1Ctrl->ATCount])//AT代号
				{
					case AT_SMS_NOLINK://短信服务没连接前
					case AT_SMS_LINK://短信服务连接
						if(UART1Ctrl->SubTask==1)//子任务
						{//发SMS的数据
							Terminal_Ram->AT_MS_Timer=30000/10;//GPRS AT命令用10MS定时器
						}
						else
						{
							Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
						}
						break;
					default:
						Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
						break;
				}
				UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				break;
			}
			break;
		case 3://接收
			GPRS_3763_AT_Rx();//AT命令接收
			break;
		default:
#if PPP_ENABLE==1
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
#else
      UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
#endif
			break;
	}
}


void UnInit_GPRS_3763(void)//GPRS模块引脚为输入下拉
{
#ifdef IC_STM32F4xx
	u32 i;
	i=
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_ON_GPRS,i);//on/off
	Pin_Configure(PIN_RST_GPRS,i);//reset
	i=
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_USART6_TX,i);//TXD
	Pin_Configure(PIN_USART6_RX,i);//RXD
	//RI
#endif
#ifdef IC_NUC9xx
	u32 i;
	i=
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_ON_GPRS,i);//on/off
	Pin_Configure(PIN_RST_GPRS,i);//reset
	i=
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_UART_TXD6,i);//TXD
	Pin_Configure(PIN_UART_RXD6,i);//RXD
	//RI
#endif
}

void GPRS_3763_RxTx(void)//远程无线GPRS模块接收发送
{
	u32 i;
	u32 x;
#if MainProtocol==698
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	u8* p8tx;
#endif
#if (PPP_ENABLE==1)
	u16* p16;
	u32 p0;
	int fd;
	int err;
	s32 socket;
	UARTCtrl_TypeDef * UARTCtrl;
	ms_Type * ms;
	u16 * p16timer;
	u8 *p8s;
	ms=Get_ms();
#endif
	
/*
//自测命令
	if(Comm_Ram->SelfTestCommand==0x55555555)
	{
		if(UART1Ctrl->LinkCount>2)
		{//备ip
			UART1Ctrl->LinkCount=0;//以太网登主IP
			if(UART1Ctrl->LinkTask>=8)
			{//8=初始化AT命令
				UART1Ctrl->LinkTask=1;//关总电源
			}
		}
	}
*/
//初始化
	if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		if(UART1Ctrl->LinkTask>2)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		{//开电源后
			if(Pin_Read(PIN_ENR5V0)==0)//读引脚,入口引脚使用名;返回0或1
			{//中断中关了电源
				//UART1Ctrl->PORT=0;//本机端口号,同时用于掉电时已连接过1次识别
				UART1Ctrl->LinkTask=1;//1=关电源
				return;
			}
		}
	}
	switch(UART1Ctrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		case 0://0=关机延时
			Terminal_Ram->Signal=0;//GPRS信号强度0-4
			Terminal_Ram->SignaldBm=0x99;//信号强度0-31或99
			if(Comm_Ram->UART1Timer==0x0)
			{
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 1://1=关总电源
			UnInit_GPRS_3763();//GPRS模块引脚为输入下拉
		
//			i=MRR(ADDR_TERMINAL,1);//终端地址的低1字节作为随机数
//			i&=0x0f;
//			i*=100;//0-30秒的随机延时
//			if((Comm_Ram->Factory==0x55)||(Comm_Ram->POWER<3))
//			{//工厂模式或电池供电时登录不加随机延时方便工厂生产测试和立即登录
//				i=0;
//			}
			i=0;
		
			x=Comm_Ram->MYMDHMS[1]+(Comm_Ram->MYMDHMS[2]<<8);//时分
			if(x<0x0030)
			{//是小于0时30分,关电源
				OffR5V0();//关R5V0电源
				//关闭总电源延时20秒+(0-30秒的随机延时)
				i+=20000/10;
			}
			else
			{//大于0时30分,不关电源
			#if ((USER/100)==7)//用户标识:浙江集中器加超级电容版  				
			  if(Pin_Read(PIN_PWR_DOWN)==0)//读引脚,入口引脚使用名;返回0或1	
				{
					OffR5V0();//关R5V0电源
					i=10000/10;//10s
        }	
			#endif	
			#if ((USER/100)==17)//用户标识:江苏集中器
				OffR5V0();//关R5V0电源
				//关闭总电源延时20秒+(0-30秒的随机延时)
				i+=10000/10;
			#endif
			}
			Comm_Ram->UART1Timer=i;
			UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;//共用UART1Timer直接返回
		case 2://开总电源
			if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{
				//本机端口号,同时用于掉电时已连接过1次识别
				if(UART1Ctrl->PORT==0)
				{
					UART1Ctrl->PORT=1;
				}
				else
				{
					return;//电池供电时连接1次后停止
				}
			}
			GPRS_Voltage();//GPRS电源电压ADC测量
			if(Comm_Ram->UGPRS<0x0030)//BCD 2位小数
			{//<0.30V
				
//				i=MRR(ADDR_TERMINAL,1);//终端地址的低1字节作为随机数
//				i&=0x0f;
//				i*=100;
				i=0;
				
				i+=10000/10;
				if(Comm_Ram->UART1Timer<=i)
				{//至小已延时10秒+(0-30秒的随机延时)
					Comm_Ram->UART1Timer=0x0;
				}
			}
			if(Comm_Ram->UART1Timer==0x0)
			{
				OnR5V0();//打开R5V0电源
				Comm_Ram->UART1Timer=1000/10;//开总电源延时1秒
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 3://开模块ON/OFF输出=0
			if(Comm_Ram->UART1Timer==0x0)
			{
  			if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  			{
					Comm_Ram->UART1Timer=1000/10;//延时
					while(1)
					{
						GPRS_Voltage();//GPRS电源电压ADC测量
						x=Comm_Ram->UGPRS;
						if((x<0x0350)||(x>0x0450))//BCD 2位小数
						{//3.80V>电压>4.20V(送电科院时免误判,用3.50V和4.50V)
							if(Comm_Ram->UART1Timer==0)
							{
								Comm_Ram->ErrorIC|=1<<30;//B30=GPRS模块电源
								Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
				UART1Ctrl->Task=0;//0=正在接收
				UART1Ctrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
				UART1Ctrl->RxByte=0;//接收字节数=0
				UART1Ctrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
				UART1Ctrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//心跳发出秒定时器
				UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				UART1Ctrl->ATCount=0;//AT计数
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
				UART1Ctrl->NETOPENCount=0;//开启服务连续次数计数(当前次数达到x次时关模块电源)
				UART1Ctrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				UART1Ctrl->res3=0;//45200200中已经试图设置参数的个数
				UART1Ctrl->res4=0;//客户端和服务器选择的链路
				UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//远端客户机为连接时为bit1-bit0为0x3,实际socket只能为0或者1
				Terminal_Ram->CopyTxByte=0;
#if PPP_ENABLE==1
				if(UART1Ctrl->TCPSocket>2)
				{
					ms->link_close(UART1Ctrl->TCPSocket);
					UART1Ctrl->TCPSocket=0;
				}
				if(Svr_Skt->ppp0_clint>2)
				{
					ms->link_close(Svr_Skt->ppp0_clint);
					Svr_Skt->ppp0_clint=0;
				}
				Svr_Skt->ppp0_clint=0;
				//是重启模块，还是只再次建立侦听socket
				if(Svr_Skt->ppp0_listen>2)
				{
					ms->link_close(Svr_Skt->ppp0_listen);
					Svr_Skt->ppp0_listen=0;
				}
#endif
			#if MainProtocol==698
				UART1Ctrl->ReNewLink=0;//0x55=通信参数变化当前帧发送完成后重新连接
			#endif
				Terminal_Ram->GPRS_ONOFF_MS_Timer=1010/10;//15 GPRS ONOFF脚控制,=1输出高,0=不变
				Pin_Configure(PIN_ON_GPRS,//ON输出=0
								0|//B3-B0:复用功能选择AF(0-15)
					 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1
				Pin_Configure(PIN_RST_GPRS,//RESET输入,上拉
								0|//B3-B0:复用功能选择AF(0-15)
					 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				Comm_Ram->UART1Timer=2000/10;//2S延时等待中断中完成
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 4://PIN_ON_RST输出1S±10%延时
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_OutH(PIN_ON_GPRS);//ON输出=1
				Comm_Ram->UART1Timer=3000/10;//延时后复位
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 5://复位
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET
								0|//B3-B0:复用功能选择AF(0-15)
					 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				
				Comm_Ram->UART1Timer=300/10;//复位延时0.3秒(>=200ms)
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 6://复位解除
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET输入,上拉
								0|//B3-B0:复用功能选择AF(0-15)
					 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				Comm_Ram->UART1Timer=10000/10;//发AT命令前延时;GPRS上电后10秒内完成初始化并能接受AT命令
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 7://初始化UART1
		#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
			DOTMIXPMODE->Ln9Code=1;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
		#endif
			if(Comm_Ram->UART1Timer==0x0)
			{
				UART1Ctrl->BpsCtrl=0x3+(9<<5);//初始速率57.6
//#ifndef LINUX
		Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
//#else
//    ms=Get_ms();
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 8://初始化AT命令
#if PPP_ENABLE==1//PPP拨号开始前需要获取SYSINFO来确定通信参数
      GPRS_3763_AT_RxTx();//GPRS_3763模块AT接收发送
			return;
		case 88://kill pppd进程退出透明模式
		case 89://检查拨号串口设备
		case 90://执行拨号程序,通过SYSINFO读取到的网络类型，下发用户名密码 
		case 91://查询拨号结果,如果失败或者超时跳转至task=88
		case 92://确定集中器工作模式
		case 93://客户机模式下的socket连接
		case 94://建立服务器侦听socket
		case 95://空闲，置task=100
		  GPRS_3763_AT_RxTx();//GPRS_3763模块AT接收发送//用于检查模块上报的信息和信号强度等
		  PPP_RxTx();//socket连接建立等
		return;
#else
     break;
#endif
		case 100://连接ok
		case 101://登录ok
		case 102://发退出登录
			break;
		case 103://退出登录
		case 104://关机
#if PPP_ENABLE==1
#else
			if(UART1Ctrl->ATTask==0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
#endif
			{
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
				UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				UART1Ctrl->LinkTask=5;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			break;
		default:
			UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;
	}

	
	if(UART1Ctrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}	
#if PPP_ENABLE==1
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("\n",0,0,0);
			ms->link_printf("start main loop\n",0,0,0);
#endif
//检查pppd拨号进程是否仍然存在
  {
    i=Check_pppd_exist();//存在返回1，不存在返回0
		if(!i)
		{
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_system((char*)"/bin/cat /tmp/pppdpid");
			ms->link_printf("pppd pid not exist,restart\n",0,0,0);
#endif
		 UART1Ctrl->LinkTask=89;
//		  UART1Ctrl->LinkTask=8;
//			UARTCtrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//			UARTCtrl->ATCount=AT_MYPOWEROFF;//关机
		  return;
		}
		CopyString((u8 *)"本IP",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		MR(ADDR_DATABUFF+4,ADDR_PPP0_IP4ADDR_ASCII,19);
		WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符
	}
	if(UART1Ctrl->res4&TCP_1_LISTEN_OPEN)
	{//TCP侦听端口建立
		socket=ms->link_accept(Svr_Skt->ppp0_listen,NULL,NULL);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("\n",0,0,0);
		ms->link_printf("ms->link_accept=%d\n",socket,0,0);
#endif
		if(socket>0)
		{
			if(Svr_Skt->ppp0_clint>2)
			{
				ms->link_close(Svr_Skt->ppp0_clint);
			}
			if((fd=ms->link_fcntl(socket,F_GETFL,0))>=0)//获取文件描述符的属性
			{
				if((fd=ms->link_fcntl(socket, F_SETFL, fd | O_NONBLOCK))>=0)	//将文件描述符的属性设置为非阻塞
				{
					UARTCtrl->res4|=TCP_1;
			    Svr_Skt->ppp0_clint=socket;
				}
				else
				{
				  if(socket>2)
				  {
					  ms->link_close(socket);
					}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("------------------------------------------------------\n",0,0,0);
		ms->link_printf("ms->link_accept_close=%d\n",socket,0,0);
#endif
				}
			}
			else
			{
				ms->link_close(socket);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("------------------------------------------------------\n",0,0,0);
		ms->link_printf("ms->link_accept_close=%d\n",socket,0,0);
#endif
			}
		}
		else
		{
			i=ms->link_errno();
	#if ((DEBUG==1)&&(DEBUG_GPRS==1))
				ms->link_printf("ms->link_accept_error=%d\n",i,0,0);
	#endif
			if(i!=EWOULDBLOCK)
			{//出现异常
				if(Svr_Skt->ppp0_listen>2)
				{
					ms->link_close(Svr_Skt->ppp0_listen);
					Svr_Skt->ppp0_listen=0;
				}
				
				UART1Ctrl->res4&=~TCP_1_LISTEN_OPEN;//清掉侦听socket建立的标志
				UARTCtrl->LinkTask=94;
				return;
			}
		}
	}
	else
	{
	 if((0x00==(UART1Ctrl->TCPUDP&0x30))||(0x20==(UART1Ctrl->TCPUDP&0x30)))//B5-B4=0混合模式=1客户机模式=2服务器模式
	 {
		 if(Svr_Skt->ppp0_listen>2)
		{
			ms->link_close(Svr_Skt->ppp0_listen);
			Svr_Skt->ppp0_listen=0;
		}
		UARTCtrl->LinkTask=94;
		return;
	 }
	}
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(GPRSPORT));
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UARTCtrl->Task=%d\n",UARTCtrl->Task,0,0);
#endif
	switch(UARTCtrl->Task)
	{
		case 0://接收
		  GPRS_3763_AT_RxTx();//用于检查模块上报的信息和信号强度等
		  i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet 通道建立了,bit5,==1socket_open,==0socket_closebit4=1TCP server 通道建立,bit3,==0客户端通讯==1服务端通讯bit2强制服务端通讯bit1-0 tcp服务端socket
			if(0x00==(UART1Ctrl->TCPUDP&0x30))//B5-B4=0混合模式=1客户机模式=2服务器模式
			{//当前混合模式
				if((i&TCP_0_OPEN)&&(!(i&TCP_1)))
				{//客户机模式连接上，服务模式下无连接
				  i&=~TCP_SELECT_SERVER;
				}
				else if((!(i&TCP_0_OPEN))&&(i&TCP_1))
				{//客户机模式无连接，服务模式有客户端连接
				  i|=TCP_SELECT_SERVER;
				}
				else
				{
				  if(!(i&FORCE_TCP_1))//当前socket未被锁定，则更改要读的socket//#define FORCE_TCP_1 0x04 
				  {
					  i^=TCP_SELECT_SERVER;//#define TCP_SELECT_SERVER 0x08  
				  }
				}
				UART1Ctrl->res4=i;
			}
			else if(0x10==(UART1Ctrl->TCPUDP&0x30))
			{//客户机模式
			  UART1Ctrl->res4&=~TCP_SELECT_SERVER;
			}
			else if(0x20==(UART1Ctrl->TCPUDP&0x30))
			{//服务器模式
			  UART1Ctrl->res4|=TCP_SELECT_SERVER;
			}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UART1Ctrl->res4=%x\n",UART1Ctrl->res4,0,0);
#endif
			p16=(u16*)Get_ADDR_UARTn(GPRSPORT);//得到UART缓冲首地址
			LEN_UARTnTx=Get_LEN_UARTnIntRx(GPRSPORT);//得到UART中断接收缓冲长度
			LEN_UARTnTx-=10;
			p0=p16[0];
			if(p0>=LEN_UARTnTx)
			{
				p0=0;
			}
			x=LEN_UARTnTx-p0;//可接收长度
			fd=0;
			if((UARTCtrl->TCPUDP&0x80)==0x0)
			{//TCP
				i=UART1Ctrl->res4;
				if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
				{//客户机模式
					fd=ms->link_recv(UARTCtrl->TCPSocket,(void*)((u32)p16+10+p0),x,MSG_DONTWAIT);

#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("current mod=clinet mode\n",0,0,0);
			ms->link_printf("tcplink_recv_socket=%d\n",UARTCtrl->TCPSocket,0,0);
			ms->link_printf("link_recv_return=%d\n",fd,0,0);
			ms->link_printf("recv_buffer_addr=%x\n",(u32)p16,0,0);
			err=ms->link_errno();
			ms->link_printf("link_recv_error=%d\n",err,0,0);
			if(fd>0)
			{
			  ms->link_printf("link_recv_data------------------------\n",0,0,0);
			  log_out((u32)p16+10+p0,fd, 'x');
			}
#endif
					if(fd==0)
					{
					 	UARTCtrl->res4&=~TCP_0_OPEN;
						return;
					}
					else if(fd<0)
					{//非阻塞模式下的接收
						err=ms->link_errno();
						if(err==ECONNRESET)
						{
						  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
						else if((err!=EINTR)&&(err!=EAGAIN))
						{
							UARTCtrl->res4&=~TCP_0_OPEN;
//							UARTCtrl->LinkTask=93;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
						  return;
						}
#if (USER/100)==17
			      if(Terminal_Ram->GPRSAutoDiscon_S_Timer==0)//用于连续无通信主动关闭socket定时//23 GPRS被动激活模式连续无通信自动断线秒定时器
						{//两分钟时间到
						  if(UARTCtrl->TCPSocket>2)
							{
							  ms->link_close(UARTCtrl->TCPSocket);
							  UARTCtrl->TCPSocket=0;
							}
						  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
#endif
						fd=0;
					}
					else if(fd>0)
					{
#if (USER/100)==17
			      Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//用于连续无通信主动关闭socket定时//23 GPRS被动激活模式连续无通信自动断线秒定时器
#endif
					}
				}
				else if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
				{//TCP server 通道建立
					fd=ms->link_recv(Svr_Skt->ppp0_clint,(void*)((u32)p16+10+p0),x,MSG_DONTWAIT);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("current mod=server mode\n",0,0,0);
			ms->link_printf("tcplink_recv_socket=%d\n",Svr_Skt->ppp0_clint,0,0);
			ms->link_printf("link_recv_return=%d\n",fd,0,0);
			err=ms->link_errno();
			ms->link_printf("link_recv_error=%d\n",err,0,0);
			if(fd>0)
			{
			  ms->link_printf("link_recv_data------------------------\n",0,0,0);
			  log_out((u32)p16+10+p0,fd, 'x');
			}
#endif
					if(fd>0)
					{//如果读到主站(客户端)发来的数据，则强制和主站(客户端)通讯
						UART1Ctrl->res4|=FORCE_TCP_1;
					}
					else if(fd==0)
					{
					  ms->link_close(Svr_Skt->ppp0_clint);
						Svr_Skt->ppp0_clint=0;
					 	UARTCtrl->res4&=~TCP_1;
						return;
					}
					else if(fd<0)
					{//非阻塞模式下的接收
					  UART1Ctrl->res4&=~FORCE_TCP_1;//解除锁定
						err=ms->link_errno();
						if((err!=EINTR)&&(err!=EAGAIN))
						{
							ms->link_close(Svr_Skt->ppp0_clint);
							Svr_Skt->ppp0_clint=0;
							UARTCtrl->res4&=~TCP_1;
						  return;
						}
						fd=0;
					}
				}
			}
			else
			{//UDP
				fd=ms->link_recv(UARTCtrl->UDPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}

			if(fd<=LEN_UARTnTx)
			{
				p0+=fd;
				if(p0>=LEN_UARTnTx)
				{
					p0=0;
				}
				p16[0]=p0;
			}
			//在线控制
			if(UARTCtrl->LinkCtrl==0)//连接控制0=不连接,1=连接
			{
				if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					UARTCtrl->LinkTask=102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 以太网心跳秒定时器
					return;
				}
			}
			break;
		case 2://等待单帧数据发送
		#if MainProtocol==376
		case 5://等待多帧数据发送
			#if (USER/100)==11//吉林系
				USER11xx_TxAddr(GPRSPORT);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
			#endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
			UARTCtrl->Task++;
		#endif
		#if MainProtocol==698
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;
			if(LEN_TAPDU<=LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
			{
			#if LOG_698==1//0=没,1=有
				LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
				LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				UARTCtrl->TxCount=0;
				p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
				UARTCtrl->Task++;
				Terminal_Ram->CopyTxByte=UARTCtrl->TxByte;//发送字节数
				Terminal_Ram->CopyTxCount=0;//已发送计数
			}
			else
			{
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
				UARTCtrl->TxAPDUSEQ=0;//链路层分帧序号计数0-4095
				UARTCtrl->TxAPDUByte=0;//链路层分帧已发送APDU字节数
				UARTCtrl->Task=5;//5=等待链路层分帧数据发送
			}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("------------------------------\n",0,0,0);
			ms->link_printf("UARTCtrl->Task=%d\n",UARTCtrl->Task,0,0);
			ms->link_printf("UARTCtrl->TxByte=%d\n",UARTCtrl->TxByte,0,0);
#endif
		#endif
			break;
	#if MainProtocol==698
		case 5://等待多帧数据发送
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;//长度域L和控制域C=3byte,帧头校验=2byte,帧校验=2byte
			MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
			p8tx[3]|=(1<<5);//分帧标志位
			LEN_Tx=4+LEN_ADDR+2;
			i=UARTCtrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UARTCtrl->RxByte=0;//rx计数=0
				UARTCtrl->Task=0;//0=接收
				return;
			}
			LEN_TxAPDU=LEN_TAPDU-i;
		//分帧格式域定义
		//bit0~bit11：表示分帧传输过程的帧序号，取值范围0~4095，循环使用；
		//bit12~bit13：保留；
		//bit15=0，bit14=0：表示分帧传输数据起始帧；
		//bit15=1，bit14=0：表示分帧传输确认帧（确认帧不包含APDU片段域）；
		//bit15=0，bit14=1：表示分帧传输最后帧；
		//bit15=1，bit14=1：表示分帧传输中间帧。
			x=UARTCtrl->TxAPDUSEQ;//链路层分帧序号计数0-4095
			UARTCtrl->TxAPDUSEQ++;//链路层分帧序号计数0-4095
			x&=0x7ff;
			if(UARTCtrl->TxAPDUByte==0)
			{//第1帧
				LEN_TxAPDU=LENmax_TxAPDU;
			}
			else
			{
				if(LEN_TxAPDU>LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
				{//中间帧
					x|=0xC000;
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{//最后帧
					x|=0x4000;
				}
			}
			p8tx[LEN_Tx]=x;
			p8tx[LEN_Tx+1]=x>>8;
			LEN_Tx+=2;
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
			UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS((u8*)p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		 #if LOG_68==1//0=没,1=有
			LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		 #endif
		 #if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
			LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		 #endif
			Terminal_Ram->CopyTxByte=UARTCtrl->TxByte;//发送字节数
			Terminal_Ram->CopyTxCount=0;//已发送计数
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
			UARTCtrl->Task++;
			break;
	#endif
		case 3://发送单帧数据正在发送
		case 6://多帧数据正在发送
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("------------------------------\n",0,0,0);
			ms->link_printf("Terminal_Ram->CopyTxByte=%d\n",Terminal_Ram->CopyTxByte,0,0);
#endif
			if(Terminal_Ram->CopyTxByte)
			{
				if(UARTCtrl->HostACK)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
				{//如果有主动上报帧
					UART1Ctrl->res4&=~TCP_SELECT_SERVER;
#if (USER/100)==17
			    Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//用于连续无通信主动关闭socket定时//23 GPRS被动激活模式连续无通信自动断线秒定时器
#endif
				}
				if((UARTCtrl->TCPUDP&0x80)==0x0)
				{//TCP
					x=2048;
					if(x>Terminal_Ram->CopyTxByte)
					{
						x=Terminal_Ram->CopyTxByte;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(GPRSPORT);
					p8s+=Terminal_Ram->CopyTxCount;
					i=UART1Ctrl->res4;
					fd=0;
					if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server 通道建立
					  fd=ms->link_send(Svr_Skt->ppp0_clint, (void *)p8s,x, MSG_DONTWAIT);
						
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
					ms->link_printf("tcplink_send_socket=%d\n",Svr_Skt->ppp0_clint,0,0);
					ms->link_printf("link_send_return=%d\n",fd,0,0);
					if(fd<0)
					{
					  err=ms->link_errno();
						ms->link_printf("link_send_error=%d\n",err,0,0);
						if(err==ENOTSOCK)
						{

						}
					}
					if(fd>0)
					{
						log_out((u32)p8s,fd, 'x');
					}
#endif
					  if(fd==0)
						{
						  UART1Ctrl->res4&=~FORCE_TCP_1;//解除锁定
						  ms->link_close(Svr_Skt->ppp0_clint);
							UARTCtrl->res4&=~TCP_1;
							Svr_Skt->ppp0_clint=0;
							return;
						}
					}
					else if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
					{//客户机模式
					  fd=ms->link_send(UARTCtrl->TCPSocket, (void *)p8s,x, MSG_DONTWAIT);
						if(fd==0)
						{//连接被断开
						  UARTCtrl->LinkTask=93;
					 	  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
					ms->link_printf("tcplink_send_socket=%d\n",UARTCtrl->TCPSocket,0,0);
					ms->link_printf("link_send_return=%d\n",fd,0,0);
					if(fd<0)
					{
					  err=ms->link_errno();
						ms->link_printf("link_send_error=%d\n",err,0,0);
						if(err==ENOTSOCK)
						{
							
						}
					}
					if(fd>0)
					{
					  ms->link_printf("link_send_data------------------------\n",0,0,0);
						log_out((u32)p8s,fd, 'x');
					}
#endif
					}
					else
					{
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("-------------reconnect socket-------------\n",0,0,0);
#endif
					  UARTCtrl->LinkTask=93;
						return;
					}

          if(fd>0)
					{
						Terminal_Ram->CopyTxByte-=fd;
						Terminal_Ram->CopyTxCount+=fd;
						Terminal_Ram->ENETLED_MS_Timer=50/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
					}
					else if(fd<0)
					{
					  err=ms->link_errno();
						if((err==EINTR)||(err==EAGAIN))
						{
							fd=0;
						}
						else
						{
						  UARTCtrl->LinkTask=93;
					 	  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
						
					}
				}
				else
				{//UDP
//暂时不调试这个功能
//					x=1440;
//					if(x>UARTCtrl->TxByte)
//					{
//						x=UARTCtrl->TxByte;
//					}
//					p8s=(u8*)Get_ADDR_UARTnTx(GPRSPORT);
//					p8s+=UARTCtrl->TxCount;
//					UARTCtrl->TxByte-=x;
//					UARTCtrl->TxCount+=x;
//				#if MainProtocol==376
//					MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
//					i=MRR(ADDR_AFN04F3+4,2);
//					i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
//					addr.sin_port = i;//端口号:倒序	
//				#endif
//				#if MainProtocol==698
//					i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
//					if(i)
//					{
//						MR((u32)&addr.sin_addr.s_addr,i+2,4);
//					}
//					else
//					{
//						MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
//					}
//					i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
//					if(i)
//					{
//						i=MRR(i+1,2);
//						addr.sin_port = i;//端口号:倒序	
//					}
//					else
//					{
//						i=1024;
//						i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
//						addr.sin_port = i;//端口号:倒序	
//					}
//				#endif
//				#if MainProtocol==CSG
//				/*
//					MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
//					MR((u32)&addr->port,ADDR_AFN04F3+4,2);
//				*/
//				#endif
//					ms->link_sendto(UARTCtrl->UDPSocket, (void *)p8s,x, MSG_DONTWAIT,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
//					Terminal_Ram->ENETLED_MS_Timer=50/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
				}
			}
			else
			{
				if(UART1Ctrl->HostACK==8)
				 {
					 UART1Ctrl->HostACK=0;
				 }
				 if(UARTCtrl->Task==3)
				{
					UARTCtrl->Task=7;//7=等待发送器空转为接收
				}
				else
				{
				#if MainProtocol==376
					UARTCtrl->Task=4;//4=多帧数据续接收完成
				#else
					UARTCtrl->Task=7;//7=等待发送器空转为接收
				#endif
				}
			}
			break;
	}
	if((UARTCtrl->Task==2)||(UARTCtrl->Task==3)||(UARTCtrl->Task==6))
	{
	  return;
	}
  Uart_698_RxTx(GPRSPORT);//UART_698接收发送
#else
//AT接收发送
	if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	{
		#if (USER/100)==14
		if(UART1Ctrl->ATTask==1)
		{
			if(UART1Ctrl->ATCount>AT_AT)
			{
				FJ_TASK(GPRSPORT);
				if(UART1Ctrl->TaskID!=0)
				{
					return;
				}
			}
		}
	  #endif
		GPRS_3763_AT_RxTx();//GPRS_3763模块AT接收发送

		return;
	}
  
//376.1接收发送
	switch(UART1Ctrl->Task)
	{
		case 0://接收
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			GPRS_3763_AT_RxTx();//GPRS_3763模块AT接收发送
			break;
		case 2://等待单帧数据发送
		#if MainProtocol==376
		case 5://等待多帧数据发送
			#if (USER/100)==11//吉林系
				USER11xx_TxAddr(GPRSPORT);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
			#endif
			Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//发送字节数
			UART1Ctrl->TxCount=0;//已发送计数
			Terminal_Ram->CopyTxCount=0;//已发送计数
			Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2分,GPRS发送数据等待超时秒定时器
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->ATCount=AT_MYNETWRITE;//发送数据
			UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
			UART1Ctrl->Task++;
		#endif
		#if MainProtocol==698
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;
			if(LEN_TAPDU<=LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
			{
			#if LOG_698==1//0=没,1=有
				LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
				LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//发送字节数
				UART1Ctrl->TxCount=0;//已发送计数
				Terminal_Ram->CopyTxCount=0;//已发送计数
				Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2分,GPRS发送数据等待超时秒定时器
				UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				UART1Ctrl->ATCount=AT_MYNETWRITE;//发送数据
				UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
				UART1Ctrl->Task++;
			}
			else
			{
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UART1Ctrl->TxByte);
				UART1Ctrl->TxAPDUSEQ=0;//链路层分帧序号计数0-4095
				UART1Ctrl->TxAPDUByte=0;//链路层分帧已发送APDU字节数
				UART1Ctrl->Task=5;//5=等待链路层分帧数据发送
			}
		#endif
			break;
	#if MainProtocol==698
		case 5://等待多帧数据发送
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;//长度域L和控制域C=3byte,帧头校验=2byte,帧校验=2byte
			MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
			p8tx[3]|=(1<<5);//分帧标志位
			LEN_Tx=4+LEN_ADDR+2;
			i=UART1Ctrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UART1Ctrl->RxByte=0;//rx计数=0
				UART1Ctrl->Task=0;//0=接收
				return;
			}
			LEN_TxAPDU=LEN_TAPDU-i;
		//分帧格式域定义
		//bit0~bit11：表示分帧传输过程的帧序号，取值范围0~4095，循环使用；
		//bit12~bit13：保留；
		//bit15=0，bit14=0：表示分帧传输数据起始帧；
		//bit15=1，bit14=0：表示分帧传输确认帧（确认帧不包含APDU片段域）；
		//bit15=0，bit14=1：表示分帧传输最后帧；
		//bit15=1，bit14=1：表示分帧传输中间帧。
			x=UART1Ctrl->TxAPDUSEQ;//链路层分帧序号计数0-4095
			UART1Ctrl->TxAPDUSEQ++;//链路层分帧序号计数0-4095
			x&=0x7ff;
			if(UART1Ctrl->TxAPDUByte==0)
			{//第1帧
				LEN_TxAPDU=LENmax_TxAPDU;
			}
			else
			{
				if(LEN_TxAPDU>LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
				{//中间帧
					x|=0xC000;
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{//最后帧
					x|=0x4000;
				}
			}
			p8tx[LEN_Tx]=x;
			p8tx[LEN_Tx+1]=x>>8;
			LEN_Tx+=2;
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UART1Ctrl->TxAPDUByte,LEN_TxAPDU);
			UART1Ctrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UART1Ctrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
			#if LOG_698==1//0=没,1=有
			LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
			LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			
			Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//发送字节数
			UART1Ctrl->TxCount=0;//已发送计数
			Terminal_Ram->CopyTxCount=0;//已发送计数
			Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2分,GPRS发送数据等待超时秒定时器
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->ATCount=AT_MYNETWRITE;//发送数据
			UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
			UART1Ctrl->Task++;
			break;
	#endif
			
		case 3://单帧数据正在发送
		case 6://多帧数据正在发送
			if(UART1Ctrl->ATTask==0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			{//不明原因冲突或意外RAM错误等等
				WarningOccur(GPRSPORT,(u8 *)"GPRS任务有BUG");//告警发生,入口告警代码和告警字符串
				UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				//重拨间隔定时器置为0以便立即重连
				Terminal_Ram->GPRSReConnection_S_Timer=0;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
			}
			break;
		case 4://多帧数据续接收完成
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
			#endif
			break;
		default:
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
			#endif
			break;
	}

#endif
	return;
}

#if PPP_ENABLE==1
void log_out(u32  ADDR,u32 Byte, char x)//
{
	u8 *p8str;
#if DEBUG==1//0=发行,1=调试
	u32 i;
	ms_Type * ms;
	ms=Get_ms();
	i=0;
	p8str=(u8*)ADDR;
	while(i<Byte)
	{
	  switch(x)
		{
		case 'd':
		ms->link_printf("%4d",p8str[0],0,0);
		break;
		case 'c':
		ms->link_printf("%c",p8str[0],0,0);
		break;
		case 'x':
		ms->link_printf("%4x",p8str[0],0,0);
		break;
		}
		i++;
		p8str++;
	}
	ms->link_printf("\n",0,0,0);
#else
	if(Byte>2000)
	{
		Byte=2000;
	}
  p8str=(u8*)Get_ADDR_UARTnTx(RS232PORT);
	MR((u32)p8str,ADDR,Byte);
	UART0Ctrl->TxByte=Byte;
	Init_UARTn(RS232PORT);//初始化UART口,入口通信控制字已填入
	Start_UARTn_Tx(RS232PORT);//启动UART口发送
	UART0Ctrl->Task=3;//3=单帧数据正在发送
	while(UART0Ctrl->Task!=0)//0=正在接收,1=接收完成,2=等待单帧数据发送,3=单帧数据正在发送
	{
		switch(UART0Ctrl->Task)
		{
			case 1://1=接收完成
				UART0Ctrl->Task=0;//0=正在接收
				break;
			case 2://2=等待单帧数据发送
				UART0Ctrl->Task=3;//正在发送标志
				Start_UARTn_Tx(RS232PORT);//启动UART口发送
				break;
			case 3://3=单帧数据正在发送
				if(Wait_UARTn_Tx(RS232PORT)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
				{			
					UART0Ctrl->Task=0;//0=正在接收
					UART0Ctrl->RxByte=0;//rx计数=0
				}
				break;
		}
	}
	#endif
	return;
}
void Myprint_ATRx(void)
{
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8d;
	u16 *p16;
	ms_Type * ms;
	ms=Get_ms();
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p1>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	if(Byte<2)
	{
	  return;
	}
	p8d=(u8*)ADDR_UART1IntATRx+10+p1;
	while(Byte--)
	{
		ms->link_printf("%c",p8d[0],0,0);
		p8d++;
		if(((u32)p8d)>=ADDR_UART1IntATRx+LEN_UART1IntATRx)
		{
			p8d=(u8*)ADDR_UART1IntATRx+10;
		}
	}
	return;
}
void tcp_log_start(u32 mode ,void *p)
{
	u8 *str;
	int fd;
	ms_Type * ms;
	ms=Get_ms();
	str=(u8 *)malloc(200);
	fd=ms->link_open("/var/run/tcp_log",O_RDWR|O_CREAT|O_APPEND);
	switch(mode)
	{
		case 1://创建
		CopyString((u8 *)"创建SOCKET",str);
		ms->link_write(fd,str,sizeof("创建SOCKET"));
		ms->link_write(fd,p,sizeof(s32));
		break;
		case 2://绑定
		CopyString((u8 *)"绑定SOCKET",str);
		ms->link_write(fd,str,sizeof("绑定SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 3://连接
		CopyString((u8 *)"连接SOCKET",str);
		ms->link_write(fd,str,sizeof("连接SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 4://侦听
		CopyString((u8 *)"侦听SOCKET",str);
		ms->link_write(fd,str,sizeof("侦听SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 5://读
		break;
		case 6://写
		break;
	}
	ms->link_close(fd);
	free(str);
}
void tcp_log_end(u32 mode ,u32 ack)
{
	u8 *str;
	int fd;
	ms_Type * ms;
	ms=Get_ms();
	str=(u8 *)malloc(200);
	fd=ms->link_open("/var/run/tcp_log",O_RDWR|O_CREAT|O_APPEND);
	switch(mode)
	{
		case 1://创建
		CopyString((u8 *)"创建SOCKET",str);
		ms->link_write(fd,str,sizeof("创建SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 2://绑定
		CopyString((u8 *)"绑定SOCKET",str);
		ms->link_write(fd,str,sizeof("绑定SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 3://连接
		CopyString((u8 *)"连接SOCKET",str);
		ms->link_write(fd,str,sizeof("连接SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 4://侦听
		CopyString((u8 *)"侦听SOCKET",str);
		ms->link_write(fd,str,sizeof("侦听SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 5://读
		break;
		case 6://写
		break;
	}
	ms->link_close(fd);
	free(str);
}
u32 Search_String(u32 ADDR_String,u32 ADDR_T,u32 Len)//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
{
	u32 i;
	u32 offset;
	u32 x;
	u32 Byte;
	u8 *p8s;
	u8 *p8d;
	
  offset=0;//已经偏移的个数
	p8s=(u8*)(ADDR_T);
	p8d=(u8*)(ADDR_String);
	Byte=0;//字符串比较字节数
	while(p8d[Byte]!=0)
	{
		Byte++;
	}
	while(offset<Len)
	{
		if((Len-offset)<Byte)
		{
			return 0;
		}
		x=offset;
		for(i=0;i<Byte;i++)
		{
			if(p8s[x]!=p8d[i])
			{
				break;
			}
			x++;
		}
		if(i==Byte)
		{
			return (((u32)p8s)+x);
		}
		offset++;
	}
	return 0;
}


u32 Check_pppd_exist(void)//存在返回1，不存在返回0
{
  int fd;
//	int err;
	u32 x;
	u32 i;
	u8 * p8s;
  ms_Type * ms;
	
	ms=Get_ms();
	if((fd=ms->link_open("/tmp/pppdpid",O_RDWR))<0)
	{//无文件
	  ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
		ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
		fd=ms->link_open("/tmp/pppdpid",O_RDWR);
	}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("function=Check_pppd_exist\n",0,0,0);
//			ms->link_system((char*)"/bin/ps -w");
#endif
	if(fd>=0)
	{//
		p8s=(u8*)ADDR_DATABUFF;
		x=CopyString((u8 *)"/proc/",p8s);
		p8s+=x;
		ms->link_lseek(fd,0,SEEK_SET);//定位到起始
		if((ms->link_read(fd,p8s,5))!=5)//读取pppd-pid
		{
		  ms->link_close(fd);
		  return 0;
		}
		ms->link_close(fd);
		for(i=0;i<5;i++)
		{
			if(p8s[i]!=' ')
			{
				MR((u32)p8s,(u32)p8s+i,5-i);
				break;
			}
		}
		p8s+=5-i;
		x=CopyString((u8 *)"/status",p8s);
		p8s[x]=0;
		if((fd=ms->link_open((char*)ADDR_DATABUFF,O_RDWR))<0)
		{//路径不存在则
			return 0;
		}
		ms->link_lseek(fd,0,SEEK_SET);//定位到起始
		if((ms->link_read(fd,(u8*)ADDR_DATABUFF,13))!=13)//读取pppd-pid
		{
			ms->link_close(fd);
			UART1Ctrl->LinkTask=88;
			return 0;
		}
		ms->link_close(fd);
		i=Search_String((u32)(u8*)"pppd",ADDR_DATABUFF,13);
		if(!i)
		{//没找到
			return 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
u32 KILL_pppd(void)
{
  int fd;
	u32 x;
  ms_Type * ms;
	
	ms=Get_ms();
	if((fd=ms->link_open("/tmp/pppdpid",O_RDWR))<0)
	{//无文件
	  ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
		ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
		fd=ms->link_open("/tmp/pppdpid",O_RDWR);
	}
	if(fd>=0)
	{//
	  x=CopyString((u8 *)"/bin/kill ",(u8*)ADDR_DATABUFF);
		MC(0,ADDR_DATABUFF+x,7);
		ms->link_lseek(fd,0,SEEK_SET);//定位到起始
		if((ms->link_read(fd,(u8*)ADDR_DATABUFF+x,5))!=5)//读取pppd-pid
		{
		  ms->link_close(fd);
			ms->link_system((char*)"chat -s -v -f /etc/ppp/peers/chat-disconnect");
		  return 0;
		}
		ms->link_close(fd);
		ms->link_system((char*)ADDR_DATABUFF);
	}
	return 1;
}
void PPP_RxTx(void)
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8tx;
	ms_Type * ms;
//	ms_Type * ms2;
	struct sockaddr_in addr;
	int fd;
	int err;
	u32 TxByte;
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(GPRSPORT));
	
	ms=Get_ms();
	switch(UARTCtrl->LinkTask)
	{
	  case 88://退出透明模式/kill $(cat /var/tmp/ppp0.pid)
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
		ms->link_printf("happen err\n",0,0,0);
#endif
      i=Check_pppd_exist();//存在返回1，不存在返回0
			if(i)
			{
			  KILL_pppd();
			}
			i=MRR(ADDR_4520_2+1,1);//取备用通道数
			UARTCtrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(UARTCtrl->res3<i)//res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数
			{//还没有尝试完所有的APN和密码
				UARTCtrl->res3++;
				UARTCtrl->ATCount=AT_MYNETCON_APN;
				UARTCtrl->LinkCount=0;
				UARTCtrl->LinkTask=8;
				return;
			}
			else
			{
			  UARTCtrl->LinkTask=104;
				return;
			}
		case 89://打开串口
		  WarningOccur(GPRSPORT,(u8 *)"检查拨号串口");//告警发生,入口告警代码和告警字符串
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
		  ms->link_system((char*)"/bin/ls -l /dev/ttyUSB*");
		#endif
		  i=Check_pppd_exist();//存在返回1，不存在返回0
			if(i)
			{
			  KILL_pppd();
			}
			p8=(u8*)ADDR_DATABUFF;
			fd=ms->link_open("/etc/ppp/peers/pppd-config",O_RDWR);
			if(fd>=0)
			{
			  ms->link_lseek(fd,0,SEEK_SET);
			  ms->link_read(fd,p8,20);
				ms->link_close(fd);
				if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"HW",2))//link_write
				{
				  x=Search_String((u32)(u8*)"ttyUSB0",(u32)p8,20);
					if(!x)
					{//没找到
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB0 9600' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB0");
				}
				else if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"GSCN",4))
				{
				  x=Search_String((u32)(u8*)"ttyUSB2",(u32)p8,20);
					if(!x)
					{//没找到
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB2 9600' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB2");
				}
				else if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"ZXGD",4))
				{
				  x=Search_String((u32)(u8*)"ttyUSB2 115200",(u32)p8,20);
					if(!x)
					{//没找到
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB2 115200' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB2");
				}
				else
				{
				  fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB*");
				}
			}
			if(fd<0)
			{//串口打开失败
			  UARTCtrl->LinkTask=8;
			  UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
			}
			else
			{
			  ms->link_close(fd);
			  UARTCtrl->LinkTask++;
				UART1Ctrl->LinkCount=0;
			}
			return;
		case 90://执行拨号程序,通过SYSINFO读取到的网络类型，下发用户名密码 
//		pppd  %s 115200 mru 1280 nodetach debug dump %s defaultroute usepeerdns novj  user \"%s\" password\"%s\"  novjccomp noipdefaultipcp-accept-local ipcp-accept-remote connect-delay 5000 ipcp-max-failure 60ipcp-max-configure 60 -am
//    pppd user "card" password "card" file /etc/ppp/peers/chint-config   
//    TxByte=CopyString((u8 *)"/usr/sbin/pppd   file /etc/ppp/peers/pppd-config&",p8tx);
		 WarningOccur(GPRSPORT,(u8 *)"执行拨号程序");//告警发生,入口告警代码和告警字符串
		 ms->link_system((char*)"/bin/mkdir /var/lock");
		 ms->link_system((char*)"/bin/rm -r /var/run/ppp*");
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
		  ms->link_system((char*)"/bin/ls -l /var/lock");
			ms->link_system((char*)"/bin/ls -l /var/run");
		#endif
		  p8tx=(u8*)ADDR_DATABUFF;
			UARTCtrl->LinkTask++;
		  TxByte=CopyString((u8 *)"/usr/sbin/pppd  user \"",p8tx);
		  i=MRR(ADDR_4520_2+1,1);//取备用通道数
			if(i==0)
			{//备用通道数=0
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到用户名
					i=p8[1];//字节数
					p8+=2;
					MR((u32)p8tx+TxByte,(u32)p8,i);
					TxByte+=i;
				}
				TxByte+=CopyString((u8 *)"\" password \"",p8tx+TxByte);
				p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到密码
					i=p8[1];//字节数
					p8+=2;
					MR((u32)p8tx+TxByte,(u32)p8,i);
					TxByte+=i;
				}
			}
			else
			{//有备用通道数
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//取备用通道数
				p8+=2;
				y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数,在设置APN时已经确定
				for(i=0;i<y;i++)//略过已经发送的用户名密码
				{
				  p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				}
				if(i<x)
				{//找到
					p8+=6;
					p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					//用户名
					if(p8[0]==DataType_visible_string)
					{
						i=p8[1];//字节数
						p8+=2;
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
						p8+=i;
					}
					//密码
					TxByte+=CopyString((u8 *)"\" password \"",p8tx+TxByte);
					if(p8[0]==DataType_visible_string)
					{
						i=p8[1];//字节数
						p8+=2;
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
			p8tx[TxByte++]='\"';
			TxByte+=CopyString((u8 *)"  file /etc/ppp/peers/pppd-config&",p8tx+TxByte);
			p8tx[TxByte]=0;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			log_out(ADDR_DATABUFF,TxByte, 'c');//
			ms->link_printf("\n",0,0,0);
#endif
			ms->link_system((char*)ADDR_DATABUFF);
			UART1Ctrl->LinkCount++;
			Terminal_Ram->GPRSWaitConnection_S_Timer=40;//25 GPRS发起连接服务等待秒定时器
			p16timer[0]=4000/10;
			return;
		case 91://查询拨号结果,如果失败或者超时
		  if(p16timer[0]!=0)
			{
				return;
			}
			if(Terminal_Ram->GPRSWaitConnection_S_Timer==0)
			{
			  UARTCtrl->LinkTask=88;
				break;
			}
			p16timer[0]=1000/10;
			ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
			ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
			fd=ms->link_open("/tmp/pppdpid",O_RDWR);
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("fd=%d\n",fd,0,0);
			ms->link_system((char*)"/bin/cat /tmp/pppdpid");
#endif
      if(fd<0)
			{//
			  UARTCtrl->LinkTask=88;
				break;
			}
			ms->link_lseek(fd,0,SEEK_SET);//定位到起始
			err=ms->link_read(fd,(u8*)ADDR_DATABUFF,5);//读取pppd-pid
			ms->link_close(fd);
			if(err<=0)
			{//无数据，进程已经终止
				if(UART1Ctrl->LinkCount>=2)
				{
					UARTCtrl->LinkTask=88;
					UART1Ctrl->LinkCount=0;
				}
				else
				{
					UARTCtrl->LinkTask=90;
				}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("could not find pppd pid \n",0,0,0);
				ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
#endif
			  break;
			}
//			ms->link_system((char*)"/bin/rm -r /var/run/pppdstat");
//		 ms->link_system("/bin/grep -o -E \"IPLOCAL=[0-9].+;\" /var/run/pppd2.tdb > /var/run/ppp0.ip");
		  WarningOccur(GPRSPORT,(u8 *)"等待拨号完成");//告警发生,入口告警代码和告警字符串
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
				ms->link_system((char*)"/sbin/ifconfig");
		#endif
        ms->link_system((char*)"cat /dev/null > /tmp/ppp0.ip");
				ms->link_system((char*)"/sbin/ifconfig ppp0|/bin/grep -o -E \"inet addr:[0-9].+\" > /tmp/ppp0.ip");
				fd=ms->link_open("/tmp/ppp0.ip",O_RDWR);
				if(fd<0)
				{
					break;
				}
				ms->link_lseek(fd,0,SEEK_SET);//定位到起始
				i=ms->link_read(fd,(u8*)ADDR_DATABUFF,30);//inet addr:255.255.255.255;
				ms->link_close(fd);
				if(i==0)
				{//还没有找到IP地址
				  break;
				}
				//ms->link_system((char*)"/bin/rm  /tmp/ppp0.ip");
				i=Search_String((u32)(u8*)"inet addr:",ADDR_DATABUFF,30);
				if(!i)
				{//没有找到IP地址
					break;
				}
				x=Search_String((u32)(u8*)" ",i,17);
				if(!x)
				{//找到空格
					break;
				}
				x--;
				x-=i;
				//复制IP地址到RAM区域
				CopyString((u8 *)"\" ",(u8*)ADDR_PPP0_IP4ADDR_ASCII);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				MR(ADDR_PPP0_IP4ADDR_ASCII+1,i,x);//
				CopyString((u8 *)"\"",(u8*)ADDR_PPP0_IP4ADDR_ASCII+x+1);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				MC(0,ADDR_PPP0_IP4ADDR_ASCII+x+2,1);
				
				CopyString((u8 *)"本IP",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				MR(ADDR_DATABUFF+4,ADDR_PPP0_IP4ADDR_ASCII,19);
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符
				UARTCtrl->LinkTask++;
			return;
		case 92:// 获取ppp0的IP地址并且确定工作模式
		  //WarningOccur(GPRSPORT,(u8 *)"获取终端工作模式");//告警发生,入口告警代码和告警字符串
			MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
			p8=(u8*)ADDR_128KDATABUFF;
			i=p8[1]<<4;//B5-B4=0混合模式=1客户机模式=2服务器模式
			i|=p8[5]<<7;//B7=0TCP=1UDP
			UARTCtrl->TCPUDP=i;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			log_out(ADDR_PPP0_IP4ADDR_ASCII,20, 'c');//
			ms->link_printf("\n",0,0,0);
//			ms->link_system("/sbin/ifconfig eth0 down");
	  	ms->link_system("/bin/ps -w");
		  ms->link_system("/sbin/ifconfig");
	  	ms->link_system("/bin/ip route show");
#endif
			UARTCtrl->LinkTask++;
		  UARTCtrl->LinkCount=0;
			Svr_Skt->ppp0_subtask=0;
			return;
		case 93://客户机模式下的socket连接
		  //WarningOccur(GPRSPORT,(u8 *)"客户机模式连接主站");//告警发生,入口告警代码和告警字符串
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		switch(Svr_Skt->ppp0_subtask)
		{
			case 3:
				if(p16timer[0]==0)
				{
					ms->link_printf("\n",0,0,0);
					ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
					ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				}
				break;
			default:
				ms->link_printf("\n",0,0,0);
        ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
			  ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				break;
		}

#endif
		  i=UARTCtrl->TCPUDP&0x30;
      if((i==0x00)||(i==0x10))////B5-B4=0混合模式=1客户机模式=2服务器模式
			{//混合模式下或者客户机模式
				if(UARTCtrl->TCPUDP&0x80)////B7=0TCP=1UDP
				{//UDP
				  
				}
				else
				{//TCP
			//		ms2->link_printf("\n",0,0,0);
					switch(Svr_Skt->ppp0_subtask)
					{
					  case 0://先检查原来的socket是否存在，如果存在则close
						{
							if(UARTCtrl->res4&TCP_0_OPEN)
							{//客户机socket已经建立
							  UARTCtrl->LinkTask++;
								break;
							}
							if(UARTCtrl->TCPSocket>2)
							{
								ms->link_close(UARTCtrl->TCPSocket);
							}
							UARTCtrl->TCPSocket=0;
							Svr_Skt->ppp0_subtask++;
						}
						break;
						case 1://申请socket
						{
							UARTCtrl->TCPSocket=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprslink_socket=%d\n",UARTCtrl->TCPSocket,0,0);
				#endif
							if(UARTCtrl->TCPSocket<0)
							{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 i=ms->link_errno();
							 ms->link_printf("gprslink_socketerror=%d\n",i,0,0);
				#endif	
								UARTCtrl->LinkTask=88;
								return;
							}
							if(UARTCtrl->TCPSocket<3)
							{
								return;
							}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("to set socket ms->link_fcntl\n",0,0,0);
			 ms->link_printf("gprslink_socket=%d\n",UARTCtrl->TCPSocket,0,0);   
			 ms->link_printf("F_GETFL=%d\n",F_GETFL,0,0);
#endif
				fd=ms->link_fcntl(UARTCtrl->TCPSocket,F_GETFL,0);//获取文件描述符的属性
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("ms->link_fcntl=%d\n",fd,0,0);
#endif
							fd=ms->link_fcntl(UARTCtrl->TCPSocket, F_SETFL, fd | O_NONBLOCK);	//将文件描述符的属性设置为非阻塞
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("ms->link_fcntl=%d\n",fd,0,0);
#endif
							if ((err=ms->link_setsockopt(UARTCtrl->TCPSocket, SOL_SOCKET, SO_BINDTODEVICE, (char *)"ppp0", 5))<0)
							{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("ms->link_setsockopt=%d\n",err,0,0);
#endif
									ms->link_close(UARTCtrl->TCPSocket);
									UARTCtrl->TCPSocket=0;
									UARTCtrl->LinkTask=88;
								return;
							}
							Svr_Skt->ppp0_subtask++;
						}
						break;
						case 2://bind地址
						{
							addr.sin_family = AF_INET;// 使用 IPv4 协议
							
							UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
							x=UARTCtrl->PORT;//当前ppp拨号的PORT
							UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
							i=UARTCtrl->PORT;
							if(i<SocketPortnMin)
							{
								i=SocketPortnMin;
							}
							i++;
							if(i==x)
							{
								i++;
							}
							x=MRR(ADDR_4500_2+13,2);//获取侦听端口
							x=R_Inverse(x,2);
							if(i==x)//如果和侦听端口重复，则继续加1
							{
								i++;
							}
							if((i<SocketPortnMin)||(i>SocketPortnMax))
							{
								i=SocketPortnMin;
							}
							UARTCtrl->PORT=i;
							i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							addr.sin_port = i;//端口号:倒序
							i=IP_ASCII_HEX(ADDR_PPP0_IP4ADDR_ASCII);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("addr.sin_addr.s_addr =%x\n",i,0,0);
				#endif
							addr.sin_addr.s_addr = i;//IP地址，注:字节顺序 "192.168.0.1" = 0x0100A8C0
							fd=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_link_bind_return =%d\n",fd,0,0);
							ms->link_printf("addr.sin_addr.s_addr =%x\n",addr.sin_addr.s_addr,0,0);
							ms->link_printf("addr.sin_port =%x\n",addr.sin_port,0,0);
				#endif
							if(fd)
							{//调用成功时返回0，失败时返回-1
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprslink_binderror=%d\n",err,0,0);
				#endif
				        ms->link_close(UARTCtrl->TCPSocket);
								UARTCtrl->TCPSocket=0;
								UARTCtrl->LinkTask=88;
								return;
							}
							Svr_Skt->ppp0_subtask++;
							UARTCtrl->NETOPENCount=0;
							p16timer[0]=0;
							Terminal_Ram->GPRSWaitConnection_S_Timer=20;//connect超时时间
						}
						break;
						case 3://conect远程服务器
						{
						  if(p16timer[0]!=0)
							{
								return;
							}
							//获取主站IP并且连接
							i=MRR(ADDR_4520_2+1,1);//取备用通道数
							if(i==0)
							{//备用通道数=0
								if(UART1Ctrl->LinkCount>=GPRSNETTOTALCOUNT)
								{
									UART1Ctrl->LinkCount=0;
								}
								i=MRR(ADDR_4500_3+1,1);
								if((UART1Ctrl->LinkCount<=GPRSNETMAINIPCOUNT)||(i<2))
								{//主ip
									p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{
										MR((u32)&addr.sin_addr.s_addr,(u32)p8+2,4);
										p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{
											i=MRR((u32)p8+1,2);
											addr.sin_port = i;//端口号:倒序
										}
									}
								}
								else
								{//备用ip
									p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{
										MR((u32)&addr.sin_addr.s_addr,(u32)p8+2,4);
										p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{
											i=MRR((u32)p8+1,2);
											addr.sin_port = i;//端口号:倒序
										}
									}
								}
							}
							else
							{//有备用通道数
								MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
								p8=(u8*)ADDR_128KDATABUFF;
								x=p8[1];
								p8+=2;
								y=UART1Ctrl->res3;//用于远程模块连接使用45200200参数时，已经试过的参数计数,在设置APN时已经确定
								for(i=0;i<y;i++)//略过已经发送的用户名密码
								{
									p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
								}
								//此时p8指向4520中特定通讯参数的主站IP数组列表
								if(i<x)
								{//找到
									if(UART1Ctrl->LinkCount>=GPRSNETTOTALCOUNT)
									{
										UART1Ctrl->LinkCount=0;
									}
									p8=Get_Element(p8,8,0,0);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{
										i=p8[1];//主站通信参数 array数
										if((UART1Ctrl->LinkCount<=GPRSNETMAINIPCOUNT)||(i<2))
										{//主ip
											p8+=2;
											MR((u32)&addr.sin_addr.s_addr,(u32)p8+4,4);
											p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
											if(p8)
											{
												i=MRR((u32)p8+1,2);
												addr.sin_port = i;//端口号:倒序
											}
										}
										else
										{//备用ip
											p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
											if(p8)
											{
												MR((u32)&addr.sin_addr.s_addr,(u32)p8+4,4);
												p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
												if(p8)
												{
													i=MRR((u32)p8+1,2);
													addr.sin_port = i;//端口号:倒序
												}
											}
										}
									}
								}
							}
							
							addr.sin_family = AF_INET;// 使用 IPv4 协议
							fd=ms->link_connect(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_link_connect_sockaddr =%x\n",addr.sin_addr.s_addr,0,0);
							ms->link_printf("gprs_link_connect_port =%x\n",addr.sin_port,0,0);
				#endif
							if(fd)
							{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprslink_connecterror=%d\n",err,0,0);
				#endif
								err=ms->link_errno();
								if(err==EINPROGRESS)
								{//还正在连接
								  p16timer[0]=1000/10;//下次检测定时
									MC(0,ADDR_DATABUFF+14,2);
									CopyString((u8 *)"第 次连接主站中",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									BCD_ASCII(UART1Ctrl->LinkCount+1,ADDR_DATABUFF+2);
			            WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
									return;
								}
								else if(err==EISCONN)
								{//已经连接了
									UARTCtrl->res4|=TCP_0_OPEN;
								  Svr_Skt->ppp0_subtask=0;
							    UARTCtrl->LinkTask++;
								  break;
								}
								else if(err==EALREADY)
								{//
								  p16timer[0]=1000/10;//下次检测定时
									MC(0,ADDR_DATABUFF+14,2);
									CopyString((u8 *)"第 次连接主站中",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									BCD_ASCII(UART1Ctrl->LinkCount+1,ADDR_DATABUFF+2);
			            WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
									return;
								}
								UART1Ctrl->LinkCount++;
								UARTCtrl->NETOPENCount++;//开启服务次数计数(当前次数达到x次时关模块电源)
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 ms->link_printf("gprslink_connect_UART1Ctrl->LinkCount=%d\n",UART1Ctrl->LinkCount,0,0);
							 ms->link_printf("gprslink_connect_UART1Ctrl->NETOPENCount=%d\n",UART1Ctrl->NETOPENCount,0,0);
				#endif
								WarningOccur(GPRSPORT,(u8 *)"连接主站失败");//告警发生,入口告警代码和告警字符串
								if(UART1Ctrl->NETOPENCount>=GPRSNETOPENCOUNT)//开启服务连续次数计数(当前次数达到x次时关模块电源)
								{
									UARTCtrl->LinkCount=0;
								  ms->link_close(UARTCtrl->TCPSocket);
									UARTCtrl->TCPSocket=0;
                  UARTCtrl->LinkTask=88;
								}
								return;
							}
							UARTCtrl->res4|=TCP_0_OPEN;
							Svr_Skt->ppp0_subtask=0;
							UARTCtrl->LinkTask++;
							UARTCtrl->LinkCount=0;
						}
						break;
						default:
						{
							if(UARTCtrl->TCPSocket>2)
							{
								ms->link_close(UARTCtrl->TCPSocket);
							}
							Svr_Skt->ppp0_subtask=0;
						}
						break;
					}
				}
			}
			return;
		case 94://服务端侦听并建立一个线程侦听客户端连接
		  //WarningOccur(GPRSPORT,(u8 *)"服务器模式开启侦听端口");//告警发生,入口告警代码和告警字符串
      i=UARTCtrl->TCPUDP&0x30;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
      switch(Svr_Skt->ppp0_subtask)
			{
			  case 1:
					if(p16timer[0]==0)
					{
						ms->link_printf("\n",0,0,0);
						ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
						ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
					}
				break;
				default:
				  ms->link_printf("\n",0,0,0);
          ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
			    ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				break;
			}
#endif
      if((i==0x00)||(i==0x20))////B5-B4=0混合模式=1客户机模式=2服务器模式
			{//混合模式下或者服务器模式
			  if(UARTCtrl->TCPUDP&0x80)////B7=0TCP=1UDP
				{//UDP
				  
				}
				else
				{//TCP
				  switch(Svr_Skt->ppp0_subtask)
					{
					case 0://先检查原来的socket是否存在，如果存在则close
					{
					  if(UARTCtrl->res4&TCP_1_LISTEN_OPEN)
						{//侦听socket已经建立
							UARTCtrl->LinkTask++;
							break;
						}
						
						if(Svr_Skt->ppp0_clint>2)
						{
							ms->link_close(Svr_Skt->ppp0_clint);
						}
						Svr_Skt->ppp0_clint=0;
						//是重启模块，还是只再次建立侦听socket
						if(Svr_Skt->ppp0_listen>2)
						{
							ms->link_close(Svr_Skt->ppp0_listen);
							p16timer[0]=120000/10*(UARTCtrl->LinkCount+1);//下次检测定时
						}
						Svr_Skt->ppp0_listen=0;
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 1://申请socket
					{
						WarningOccur(GPRSPORT,(u8 *)"等待建立侦听端口");//告警发生,入口告警代码和告警字符串
					  if(p16timer[0]!=0)
						{
							return;
						}
						Svr_Skt->ppp0_listen=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_svr_link_ppp0_listen=%d\n",Svr_Skt->ppp0_listen,0,0);
				#endif
						if(Svr_Skt->ppp0_listen<0)
						{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprs_svr_link_socketerror=%d\n",err,0,0);
				#endif	
							UARTCtrl->LinkTask=88;
							return;
						}
						if(Svr_Skt->ppp0_listen<3)
						{
							return;
						}
						err=1;
						if ((err=ms->link_setsockopt(Svr_Skt->ppp0_listen, SOL_SOCKET, SO_REUSEADDR, &err, sizeof(err)))<0)
						{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("ms->link_setsockopt=%d\n",err,0,0);
#endif
								ms->link_close(Svr_Skt->ppp0_listen);
								Svr_Skt->ppp0_listen=0;
								UARTCtrl->LinkTask=88;
							return;
						}
						fd=ms->link_fcntl(Svr_Skt->ppp0_listen,F_GETFL,0);//获取文件描述符的属性
						ms->link_fcntl(Svr_Skt->ppp0_listen, F_SETFL, fd | O_NONBLOCK);	//将文件描述符的属性设置为非阻塞
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 2://绑定socket
					{
						addr.sin_family = AF_INET;// 使用 IPv4 协议
						i=MRR(ADDR_4500_2+13,2);//获取侦听端口
						addr.sin_port = i;//端口号:倒序
	//					i=IP_ASCII_HEX(ADDR_PPP0_IP4ADDR_ASCII);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
				    i=0;
						addr.sin_addr.s_addr = i;//IP地址，注:字节顺序 "192.168.0.1" = 0x0100A8C0
						#if((DEBUG==1)&&(DEBUG_GPRS==1))
						ms->link_printf("gprs_svrlisten_socket_s_addr =%x\n",addr.sin_addr.s_addr,0,0);
						ms->link_printf("gprs_svrlisten_socket_s_listenport =%x\n",addr.sin_port,0,0);
				#endif
						fd=ms->link_bind(Svr_Skt->ppp0_listen,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_svr_link_bind_return =%d\n",fd,0,0);
				#endif
						if(fd)
						{//调用成功时返回0，失败时返回-1
							err=ms->link_errno();
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 ms->link_printf("gprs_svr_link_binderror=%d\n",err,0,0);
				#endif
				      if((err==EADDRINUSE)&&(UARTCtrl->LinkCount<2)) /* Address already in use */
							{
							  Svr_Skt->ppp0_subtask=0;//在等于0时会关闭侦听socket，并延时
								UARTCtrl->LinkCount++;
							}
							else
							{
								if(Svr_Skt->ppp0_listen>2)
								{
									ms->link_close(Svr_Skt->ppp0_listen);
								}
								Svr_Skt->ppp0_listen=0;
								UARTCtrl->LinkTask=88;
							}
							return;
						}
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 3://侦听sokcet
					{
						fd=ms->link_listen(Svr_Skt->ppp0_listen,5);
						if(fd)
						{//调用成功时返回0，失败时返回-1
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprs_svr_link_binderror=%d\n",err,0,0);
				#endif
				      if(Svr_Skt->ppp0_listen>2)
							{
								ms->link_close(Svr_Skt->ppp0_listen);
								Svr_Skt->ppp0_listen=0;
							}
							UARTCtrl->LinkTask=88;
							return;
						}
						UARTCtrl->res4|=TCP_1_LISTEN_OPEN;
						Svr_Skt->ppp0_subtask=0;
						UARTCtrl->LinkTask++;
					}
					break;
					default:
					if(Svr_Skt->ppp0_listen>2)
					{
						ms->link_close(Svr_Skt->ppp0_listen);
						Svr_Skt->ppp0_listen=0;
					}
					Svr_Skt->ppp0_subtask=0;
					break;
					}
				}
			}
			return;
		case 95:
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
#endif
    // p16=(u16 *)(ADDR_HeartberatTimer);//心跳发出间隔秒定时器
	if((UART1Ctrl->Task)&&(Terminal_Ram->CopyTxByte))
	{//有数据需要发送时
		UARTCtrl->LinkTask=101;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	}
	else
	{
	  UARTCtrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	}
		break;
		}
		return;
}
#endif

















