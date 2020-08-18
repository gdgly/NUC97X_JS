
//无线GPRS模块 南网远程通信标准
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../GPRS/GPRS_QCSG.h"
#include "../GPRS/ATNo.h"
#include "../GPRS/ATString.h"
#include "../GPRS/GPRS_FTP_RxTx.h"
#include "../GPRS/GPRS_SMS_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../TEST/LOG.h"

#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"

#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "Terminal_Task.h"

#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"


// SocketID 0-5使用约定
// 0 主连接
// 1 FTP控制
// 2 FTP数据
// 3-5 未用

const SelfAdaptionCommunicationParaStruct SelfAdaptionParaArry[3][2] = 
{
//移动
	{

		{
			{0x21, 0x1C, 116,162, 20, 100, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'Y', 'N', 'D', 'W', 'F', 'K', '.', 'Y',
			 	'N', 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		},

		{
			{0x78, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			//{0xD8, 0x07, 178,151, 247, 183, 0, 0},		// MasterAddr
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'I', 'O', 'T', 'Y', 'N', 'D',
			 	'W', 'F', 'K', '.', 'Y', 'N', 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}

		}
		

	},
//联通
	{
		{
			{0x21, 0x1C, 46,1, 100, 172, 0, 0},			// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'Y', 'N', 'D', 'W', '.', 'F', 'K', '.', 
			 	'Y', 'N', 'A', 'P', 'N', 0, 0, 0, 
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		},

		{
			{0x79, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		}
	},
//电信

	{
		{
			{0x21, 0x1C, 71,1, 100, 172, 0, 0},			// MasterAddr	
			0x02,										// MasterAddrType
		
			{											// APN
				'Y', 'E', 'P', 'G', 'F', 'K', '.', 'V',
				'P', 'D', 'N', '.', 'Y', 'N', 0, 0
			},
			{											// APNUserName
				'q', 'j', 'd', 'w', '@', 'y', 'n', 'd',
				'w', 's', 'k', '.', 'v', 'p', 'd', 'n',
				'.', 'y', 'n', 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'g', 'd', 'j', '1', '7', '2', 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, 0, 0, 0
			}
		},
		{
			{0x7A, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		}
	}
};


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

//关主动上报
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=0\r",0
ADDR_ERRStr_MYNETURC
	dcb "关主动上报失败",0

//获取SIM卡序列号ICCID
ADDR_ATStr_MYCCID
	dcb "AT$MYCCID\r",0
ADDR_ERRStr_MYCCID
	dcb "读卡CCID失败",0

//取本机号码
ADDR_ATStr_CNUM
	dcb "AT+CNUM\r",0
ADDR_ERRStr_CNUM
	dcb "读本机号码失败",0
	
//查国际移动台标识请求IMSI
ADDR_ATStr_CIMI
	dcb "AT+CIMI\r",0
ADDR_ERRStr_CIMI
	dcb "查国际移动标识失败",0

//读取SIM卡服务运营商名称
ADDR_ATStr_CSPN
	dcb "AT+CSPN?\r",0
ADDR_ERRStr_CSPN
	dcb "读卡运营商名称失败",0

	//连接设置
//设置APN
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=0,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "设置APN失败",0

//设置USERPWD:用户名和密码
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=0,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "设置USERPWD失败",0

//读信号没连接前
ADDR_ATStr_CSQ_NOLINK
	dcb "AT+CSQ\r",0
ADDR_ERRStr_CSQ_NOLINK
	dcb "远程无线读信号失败",0

//获取当前网络注册状态
ADDR_ATStr_CREG
	dcb "AT+CREG?\r",0
ADDR_ERRStr_CREG
	dcb "获取网络注册状态失败",0

//查询注册网络类型：3G/4G
ADDR_ATStr_SYSINFO
	dcb "AT$MYSYSINFO\r",0
ADDR_ERRStr_SYSINFO
	dcb "查询是否注册上4G网络",0

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
	dcb "AT$MYNETOPEN=0\r",0
ADDR_ERRStr_MYNETOPEN
	//dcb "远程无线开启服务失败",0
	dcb 0
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
	dcb "AT$MYNETWRITE=0,",0
ADDR_ERRStr_MYNETWRITE
	dcb "远程无线发送数据失败",0
//短信服务
	

//HOLD暂停
ADDR_ATStr_HOLD
	dcb 0
//关闭连接
ADDR_ATStr_MYNETCLOSE
	dcb "AT$MYNETCLOSE=0\r",0
ADDR_ERRStr_MYNETCLOSE
	dcb "远程无线关闭连接失败",0
//关机
ADDR_ATStr_MYPOWEROFF
	dcb "AT$MYPOWEROFF\r",0
ADDR_ERRStr_MYPOWEROFF
	dcb "远程无线模块关机失败",0
	
//重启 err20190410 用于未检测到SIM卡时进行不断电GPRS模块重启
ADDR_ATStr_MYRESET
	 dcb "AT+CFUN=1,1\r",0
ADDR_ERRStr_MYRESET
	 dcb "远程无线模块重启失败",0

 
	ALIGN 4	
};





__asm void GPRS_QCSG_AT_Command_Lib(void)//AT顺序执行库
{
//波特率检查
	dcd AT_AT
	dcd ADDR_ATStr_AT//AT命令字符串地址
	dcd ADDR_ERRStr_AT//错误提示字符串地址
	dcd 500//AT命令返回第1个字符定时(ms)

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
	dcd 1000//AT命令返回第1个字符定时(ms)

//取本机号码
	dcd AT_CNUM
	dcd ADDR_ATStr_CNUM//AT命令字符串地址
	dcd ADDR_ERRStr_CNUM//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//查国际移动台标识请求IMSI
	dcd AT_CIMI
	dcd ADDR_ATStr_CIMI//AT命令字符串地址
	dcd ADDR_ERRStr_CIMI//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)

//读取SIM卡服务运营商名称
	dcd AT_CSPN
	dcd ADDR_ATStr_CSPN//AT命令字符串地址
	dcd ADDR_ERRStr_CSPN//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)


//设置APN
	dcd AT_MYNETCON_APN
	dcd ADDR_ATStr_MYNETCON_APN//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETCON_APN//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
//设置USERPWD:用户名和密码
	dcd AT_MYNETCON_USERPWD
	dcd ADDR_ATStr_MYNETCON_USERPWD//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETCON_USERPWD//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

//读信号没连接前
	dcd AT_CSQ_NOLINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT命令字符串地址
	dcd ADDR_ERRStr_CSQ_NOLINK//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)

//获取当前网络注册状态
	dcd AT_CREG
	dcd ADDR_ATStr_CREG//AT命令字符串地址
	dcd ADDR_ERRStr_CREG//错误提示字符串地址
	dcd 3000//AT命令返回第1个字符定时(ms)
	
//查询注册网络类型：3G/4G
	dcd AT_SYSINFO
	dcd ADDR_ATStr_SYSINFO//AT命令字符串地址
	dcd ADDR_ERRStr_SYSINFO//错误提示字符串地址
	dcd 3000//AT命令返回第1个字符定时(ms)

//激活网络连接
	dcd AT_MYNETACT
	dcd ADDR_ATStr_MYNETACT//AT命令字符串地址
	dcd ADDR_ERRStr_MYNETACT//错误提示字符串地址
	dcd 90000//AT命令返回第1个字符定时(ms)

//IP访问控制配置
	dcd AT_MYIPFILTER
	dcd ADDR_ATStr_MYIPFILTER//AT命令字符串地址
	dcd ADDR_ERRStr_MYIPFILTER//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)

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
	dcd 1000//AT命令返回第1个字符定时(ms)
//AT循环,没连接
	dcd AT_LOOP_NOLINK
	dcd 0//AT命令字符串地址
	dcd 0//错误提示字符串地址
	dcd 0//AT命令返回第1个字符定时(ms)


//读信号连接后
	dcd AT_CSQ_LINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT命令字符串地址
	dcd ADDR_ERRStr_CSQ_NOLINK//错误提示字符串地址
	dcd 2000//AT命令返回第1个字符定时(ms)
//查询注册网络类型：3G/4G
	dcd AT_SYSINFO_LINK
	dcd ADDR_ATStr_SYSINFO//AT命令字符串地址
	dcd ADDR_ERRStr_SYSINFO//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
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
	dcd 35000//40000//AT命令返回第1个字符定时(ms)
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
	dcd 1000//AT命令返回第1个字符定时(ms)
//关机
	dcd AT_MYPOWEROFF
	dcd ADDR_ATStr_MYPOWEROFF//AT命令字符串地址
	dcd ADDR_ERRStr_MYPOWEROFF//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)
//重启 err20190410 GPRS模块重启
	dcd AT_MYRESET
	dcd ADDR_ATStr_MYRESET//AT命令字符串地址
	dcd ADDR_ERRStr_MYRESET//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

};

#define GPRS_MAX_WRITE_DATA_LEN 		(1460)

void GPRS_QCSG_NextATCount(void)//下1个AT
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	
	pUartCtrl->LinkSubTask = 0;		//尝试AT发送计数
	pUartCtrl->SubTask = 0;			//子任务
	pUartCtrl->ATCount++;
	//下个AT命令定时设置
	switch(pUartCtrl->ATCount)//下个AT命令
	{
		case AT_CSQ_NOLINK://读信号没连接前
			pTerminalRam->GPRSWaitTx_S_Timer = 60;//GPRS等待信号强度查询(连接前)定时
			break;
		case AT_CREG://获取当前网络注册状态
			pTerminalRam->GPRSWaitTx_S_Timer = 60;//等待注册定时
			break;
		case AT_MYNETACT://激活网络连接
			pTerminalRam->GPRSWaitTx_S_Timer = 30;//等待激活网络连接定时
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

const u8 Err_Comm_Addr[COMM_AADR_SIZE] ={0,0,0,0,0,0};//无效地址 


void GPRS_QCSG_AT_Tx(void)//AT命令发送
{
	u32 i, x, y, AT_No, TxByte, FTPFileCount;	//FTP文件下载个数计数
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	CommunicationParaStruct CommPara;
	//SelfAdaptionParaStruct SelfAdaption_Temp;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;

	#if ((Project/100 == 3) && (USER == 3400))//err20190528 云南负控自适应参数
		E000011x_CommunicationParaStruct E0000111_CommPara;
		E000011x_CommunicationParaStruct E0000112_CommPara;
		E000011x_CommunicationParaStruct E0000113_CommPara;
		MemSet((u8*)&E0000111_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000112_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000113_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000111_CommPara, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000112_CommPara, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000113_CommPara, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));
		
	#endif	

	

	if(pTerminalRam->AT_MS_Timer != 0) return;
	p8txbuff = (u8*)(ADDR_ATTx);
	TxByte = 0;
	AT_No = pUartCtrl->ATCount;//AT计数
	if(AT_No > AT_Max)
	{
		AT_No = 0;
		pUartCtrl->ATCount = 0;//AT计数
	}
	p32=(u32*)(&GPRS_QCSG_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT://波特率检查
			TxByte = CopyString((u8 *)"AT\r\n",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			#if LOG_1376_3==1//0=没,1=有
				LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			switch(pUartCtrl->SubTask)	//子任务，表示使用哪个波特率进行发送初始 AT指令
			{
			#if (TEST_ON == 0)
				case 0://速率115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;
				case 1://速率57600
					pUartCtrl->BpsCtrl=0x3+(9<<5);
					break;
				case 2://速率38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;
				case 3://速率19200
					pUartCtrl->BpsCtrl=0x3+(7<<5);
					break;
				case 4://速率9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			#endif

			#if (TEST_ON == 1)
				case 0://速率115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;			
				case 1://速率38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;				
				case 2://速率9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			#endif
				default:
//				#if  (TEST_ON == 0)
//					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
//					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
//					return;
//				#else
					TxByte = CopyString((u8 *)"AT+IPR=57600\r\n",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					break;				
				//#endif
			}
			Init_UARTn(GPRSPORT);			// 初始化UART口,入口通信控制字已填入
			for(i = 0; i < 80000; i++);
			pUartCtrl->TxByte = TxByte;		// 发送字节数(半字对齐)
			pUartCtrl->TxCount = 0;
			#if LOG_1376_3==1//0=没,1=有
				LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			Start_UARTn_Tx(GPRSPORT);	// 启动UART口发送
			pUartCtrl->ATTask = 2;		// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			return;
		case AT_CNUM://取本机号码
			if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{//中兴CDMA(MC8332)
				TxByte = CopyString((u8 *)"AT+CNUM?\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			break;	
		case AT_CSPN: //南网指令不支持AT_CSPN
			GPRS_QCSG_NextATCount();
			return;
		case AT_MYNETCON_APN://设置APN
			if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				
				//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));
				MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));

				//err20190528 云南负控自适应
				if(pCommRam->CSPN == 1)//移动
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000111_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000111_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else if(pCommRam->CSPN == 2)//联通
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000112_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000112_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else if(pCommRam->CSPN == 3)//电信
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000113_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000113_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//确认地址有效
						{
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else 
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
				}

				if(pTerminalRam->LinkCount_SelfAdaption >= 5)//所有通讯参数都已经试过
				{
					pTerminalRam->LinkCount_SelfAdaption = 0;
					pUartCtrl->LinkCount = 0;
					pUartCtrl->LinkCtrl = 0;
//						WarningOccur(GPRSPORT,(u8 *)"远程无线主站连接失败");//告警发生,入口告警代码和告警字符串
					pUartCtrl->State = GPRS_STATE_LINK_FAIL;
					//err20190313 修改GPRS流程，主备IP切完连接失败不直接重启模块，重新配置通信参数后再尝试。
					pTerminalRam->GPRS_Para_Reset_Count++;
					if(pTerminalRam->GPRS_Para_Reset_Count>=3)//大循环尝试超过3次再硬件重启模块
					{
						pTerminalRam->GPRS_Para_Reset_Count = 0;
						pTerminalRam->GPRS_Para_Reset_Flag = 0;
						pUartCtrl->ATCount = AT_MYPOWEROFF;		
					}
					else 
					{
						pTerminalRam->GPRS_Para_Reset_Flag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
					}
					//pUartCtrl->ATCount = AT_MYPOWEROFF;		// 关机
					return;
				}

				
//				if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//				{
//					if(pCommRam->CSPN == 1)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else if(pCommRam->CSPN == 2)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else if(pCommRam->CSPN == 3)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else 
//					{
//						MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
//					}
//				}
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
//				}

				if(pTerminalRam->LinkCount_SelfAdaption >=2)
					MC(0, ADDR_DATABUFF + APN_USER_NAME_MAX_SIZE, 1);
				else 
					MC(0, ADDR_DATABUFF + APN_NAME_MAX_SIZE, 1);
				TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte++]='"';
				p8txbuff[TxByte++]='\r';
			}
			else
			{//中兴CDMA(MC8332)
				//AUTH：鉴权类型，0：NONE；1：PAP；2：CHAP，默认值1
				//AT$MYNETCON=0,"AUTH",1
				TxByte=CopyString((u8 *)"AT$MYNETCON=0,\"AUTH\",1",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte++]='\r';
			}
			break;		
		case AT_MYNETCON_USERPWD://设置USERPWD:用户名和密码
			//AT$MYNETCON=0,"USERPWD","card,card"
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);			//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			
			//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));


			//err20190528 云南负控自适应
			if(pCommRam->CSPN == 1)//移动
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以

				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
				}
				
			}
			else if(pCommRam->CSPN == 2)//联通
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
				}
				
			}
			else if(pCommRam->CSPN == 3)//电信
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{	
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//此处32字节不确定是否可以
				}
				
			}
			else 
			{
				MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
			}



//			if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//			{
//				if(pCommRam->CSPN == 1)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else if(pCommRam->CSPN == 2)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else if(pCommRam->CSPN == 3)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
//				}
//			}
//			else 
//			{
//				MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
//			}
			
			MC(0, ADDR_DATABUFF + APN_USER_NAME_MAX_SIZE, 1);
			TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);	//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8txbuff[TxByte++]=',';


			//err20190528 云南负控自适应
			if(pCommRam->CSPN == 1)//移动
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{		
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
				
			}
			else if(pCommRam->CSPN == 2)//联通
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{		
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
				
			}
			else if(pCommRam->CSPN == 3)//电信
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);					
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{					
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{					
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//此处32字节不确定是否可以
				}
			}
			else 
			{
				MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
			}

			
//			if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//			{
//				if(pCommRam->CSPN == 1)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else if(pCommRam->CSPN == 2)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else if(pCommRam->CSPN == 3)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
//				}
//			}
//			else 
//			{
//				MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
//			}
			
			MC(0, ADDR_DATABUFF + APN_USER_PWD_MAX_SIZE, 1);
		
			TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);	//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8txbuff[TxByte++]='"';
			p8txbuff[TxByte++]='\r';
			break;

		case AT_MYNETACT:
			if(((pTerminalRam->LinkCount_SelfAdaption >= 3) && (pTerminalRam->SelfAdaptionChangeFlag == 1))|| (pTerminalRam->GPRS_Para_Reset_Flag == 1) )//参数切换标识
			//if(pTerminalRam->GPRS_Para_Reset_Flag == 1) //参数切换标识
			{//切换APN、用户名密码参数，去激活
				TxByte=CopyString((u8 *)"AT$MYNETACT=0,0\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else 
			{
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			break;
		case AT_MYIPFILTER://IP访问控制配置
			if(pUartCtrl->SubTask >= 5)
			{
				GPRS_QCSG_NextATCount();//下1个AT
				return;
			}		
			//x = MRR(ADDR_IPFILTER+(4*UART1Ctrl->SubTask),4);
			x = 0;
			if(x == 0)
			{
				pUartCtrl->SubTask++;//子任计数
				return;
			}
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8txbuff[TxByte]='"';
			TxByte++;
			for(i=0;i<4;i++)
			{
				y=hex_bcd(x&0xff);//HEX数转为8位压缩BCD数
				x>>=8;
				TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
				p8txbuff[TxByte]='.';
				TxByte++;
			}
			TxByte--;//IP第4个字节的'.'删去
			p8txbuff[TxByte++] = '"';
			p8txbuff[TxByte++] = ',';
			p8txbuff[TxByte++] = '"';
			TxByte += CopyString((u8 *)"255.255.255.255",p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8txbuff[TxByte++] = '"';
			break;
	#if (USER/100)==0//用户标识,电科院测试不发短信AT
		case AT_CMFG://设置短消息模式
		case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
		case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
			GPRS_QCSG_NextATCount();//下1个AT
			return;
	#else
		#if (USER/100)==14//福建系
			case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
//			case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
				GPRS_QCSG_NextATCount();//下1个AT
				return;
		#else
			case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
			case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
					GPRS_QCSG_NextATCount();//下1个AT
					return;
				}
				else
				{//中兴CDMA(MC8332)
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
				break;
		#endif//(USER/100)==14//福建系
	#endif
		case AT_MYNETSRV://设置服务参数
			pUartCtrl->State = GPRS_STATE_CONFIG_LINK_PARA;
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			switch(pUartCtrl->SubTask)//子任务
			{
				case 0://确定工作模式
					if(CommPara.IpLinkType == 0xFF) CommPara.IpLinkType = 0;
					i = CommPara.IpLinkType << 7;
					//原工作模式(同F8设置字)B7 = 0:TCP,= 1:UDP; B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它;
					if(CommPara.CommunicationMode == 0xFF) CommPara.CommunicationMode = 0;
					i += CommPara.CommunicationMode << 4;
					pUartCtrl->TCPUDP = i;		
					i &= 0x30;
					if((pUartCtrl->TCPUDP & 0x80) != 0)
					{// UDP
						if(i == 0x20)
						{// =2服务器模式
							pUartCtrl->SubTask = 4;
						}
						else
						{//=0混合模式=1客户机模式
							pUartCtrl->SubTask = 3;
						}
					}
					else
					{// TCP
						if(i==0x20)
						{// =2服务器模式
							pUartCtrl->SubTask = 2;
						}
						else
						{// =0混合模式=1客户机模式
							pUartCtrl->SubTask =1 ;
						}
					}
					return;
				case 1://TCP客户机模式
				//default://TCP客户机模式
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				TCPClient:
					p8txbuff[TxByte++] = '"';
		
					//err20190528 云南负控自适应
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(pCommRam->CSPN == 1)//移动
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.MasterAddr, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 2)//联通
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.MasterAddr, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 3)//电信
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.MasterAddr, COMM_AADR_SIZE);
						else 
							MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(pCommRam->CSPN == 1)//移动
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 2)//联通
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 3)//电信
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else 
							MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					/*
					else if(pTerminalRam->LinkCount_SelfAdaption <= 2)
					{
						if(pUartCtrl->LinkCount == 0)
						{//主ip
							if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
							}
							else
							{		
								pUartCtrl->LinkCount = 1;
								pTerminalRam->LinkCount_SelfAdaption = 1;
							}
						}
						if(pUartCtrl->LinkCount == 1)
						{//备用ip1
							if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
							}
							else
							{
								pUartCtrl->LinkCount = 2;
								pTerminalRam->LinkCount_SelfAdaption = 2;
							}
						}
						if(pUartCtrl->LinkCount == 2)
						{//备用iP2
							if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
							}
							else
							{
								pUartCtrl->LinkCount = MASTER_ADDR_NUM;
								pTerminalRam->LinkCount_SelfAdaption = 3;
							}
						}
						if(pUartCtrl->LinkCount >= MASTER_ADDR_NUM)
						{
							pTerminalRam->LinkCount_SelfAdaption = 0;
							pUartCtrl->LinkCount = 0;
							pUartCtrl->LinkCtrl = 0;
	//						WarningOccur(GPRSPORT,(u8 *)"远程无线主站连接失败");//告警发生,入口告警代码和告警字符串
							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
							//err20190313 修改GPRS流程，主备IP切完连接失败不直接重启模块，重新配置通信参数后再尝试。
							pTerminalRam->GPRS_Para_Reset_Count++;
							if(pTerminalRam->GPRS_Para_Reset_Count>=3)//大循环尝试超过3次再硬件重启模块
							{
								pTerminalRam->GPRS_Para_Reset_Count = 0;
								pTerminalRam->GPRS_Para_Reset_Flag = 0;
								pUartCtrl->ATCount = AT_MYPOWEROFF;		
							}
							else 
							{
								pTerminalRam->GPRS_Para_Reset_Flag = 1;
								pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
							}
							//pUartCtrl->ATCount = AT_MYPOWEROFF;		// 关机
							return;
						}
					}*/


					
					//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));

//					if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//					{
//						if(pTerminalRam->LinkCount_SelfAdaption < 2)
//						{
//					
//							if(pCommRam->CSPN == 1)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else if(pCommRam->CSPN == 2)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else if(pCommRam->CSPN == 3)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else 
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
//							}

//							//更新FLASH中的通信参数，方便液晶查看当前通信参数
//							if((pCommRam->CSPN <= 3) && (pCommRam->CSPN >= 1))
//							{
//								MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
//								MR((u32)CommPara.APN , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//								MR((u32)CommPara.APNUserName , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//								MR((u32)CommPara.APNPassWord , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//								MR((u32)CommPara.MasterAddr , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//								MR((u32)CommPara.MasterBackAddr1 , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//								MR((u32)CommPara.MasterBackAddr2 , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
// //								CommPara.MasterAddrType = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType;
// //								CommPara.MasterBackAddr1Type = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType;
// //								CommPara.MasterBackAddr2Type = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType; //此处液晶显示仍不对
//								MR((u32)&CommPara.MasterAddrType , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MR((u32)&CommPara.MasterBackAddr1Type , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MR((u32)&CommPara.MasterBackAddr2Type , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MW((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
//							}				
//						}
//						else 
//						{
//							pTerminalRam->LinkCount_SelfAdaption = 0;
//							pTerminalRam->SelfAdaptionChangeFlag = 0;
//							pUartCtrl->LinkCount = 0;
//							pUartCtrl->LinkCtrl = 0;
//	//						WarningOccur(GPRSPORT,(u8 *)"远程无线主站连接失败");//告警发生,入口告警代码和告警字符串
//							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
//							pUartCtrl->ATCount = AT_MYPOWEROFF;		// 关机
//							return;
//						}
//					}
//					else
//					{
//					
//						if(pUartCtrl->LinkCount == 0)
//						{//主ip
//							if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = 1;
//							}
//						}
//						if(pUartCtrl->LinkCount == 1)
//						{//备用ip1
//							if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = 2;
//							}
//						}
//						if(pUartCtrl->LinkCount == 2)
//						{//备用iP2
//							if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //通信通道类型：02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = MASTER_ADDR_NUM;
//							}
//						}
//						if(pUartCtrl->LinkCount >= MASTER_ADDR_NUM)
//						{
//							pUartCtrl->LinkCount = 0;
//							pUartCtrl->LinkCtrl = 0;
//	//						WarningOccur(GPRSPORT,(u8 *)"远程无线主站连接失败");//告警发生,入口告警代码和告警字符串
//							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
//							//err20190313 修改GPRS流程，主备IP切完连接失败不直接重启模块，重新配置通信参数后再尝试。
//							pTerminalRam->GPRS_Para_Reset_Count++;
//							if(pTerminalRam->GPRS_Para_Reset_Count>=3)//大循环尝试超过3次再硬件重启模块
//							{
//								pTerminalRam->GPRS_Para_Reset_Count = 0;
//								pTerminalRam->GPRS_Para_Reset_Flag = 0;
//								pUartCtrl->ATCount = AT_MYPOWEROFF;		
//							}
//							else 
//							{
//								pTerminalRam->GPRS_Para_Reset_Flag = 1;
//								pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
//							}
//							//pUartCtrl->ATCount = AT_MYPOWEROFF;		// 关机
//							return;
//						}
//					}
					
					p8 = (u8*)(ADDR_DATABUFF);
					for(i = 0; i < 4; i++)
					{
						y = hex_bcd(p8[5-i]);//HEX数转为8位压缩BCD数
						TxByte += BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
						p8txbuff[TxByte++] = '.';
					}
					TxByte--;//IP第4个字节的'.'删去
					//目的端口号
					p8txbuff[TxByte++] = ':';
					y = hex_bcd(p8[0] + (p8[1]<<8));//HEX数转为8位压缩BCD数
					TxByte += BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte++] = '"';
					p8txbuff[TxByte++] = '\r';
					break;
				case 2://TCP服务器模式
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,1,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8txbuff[TxByte++] = '"';
					TxByte += CopyString((u8 *)"127.0.0.1",p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8txbuff[TxByte++] = ':';
					//服务器模式,端口号用设定
					i = CommPara.TcpPortNumber;
					i = hex_bcd(i);//HEX数转为8位压缩BCD数		
					TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte++] = '"';
					break;
				case 3://UDP客户机模式
					TxByte = CopyString((u8 *)"AT$MYNETSRV=0,0,2,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					goto TCPClient;
				case 4://UDP服务器模式
					//无此模式
					WarningOccur(GPRSPORT,(u8 *)"远程无线上行模式错");//告警发生,入口告警代码和告警字符串
					TxByte = CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					goto TCPClient;
			}
			break;	
		case AT_MYNETOPEN://开启服务		
			if(pUartCtrl->LinkCtrl == 0)//连接控制0=不连接,1=连接
			{
				GPRS_QCSG_NextATCount();//下1个AT
			}
			else
			{
				pUartCtrl->State = GPRS_STATE_LINKING;
				if(pTerminalRam->GPRSReConnection_S_Timer != 0x0)		//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
				{
					WarningOccur(GPRSPORT,(u8 *)"远程无线重拨间隔定时");//告警发生,入口告警代码和告警字符串
					GPRS_QCSG_NextATCount();
				}
				else
				{
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					//pTerminalRam->GPRSWaitConnection_S_Timer = 90;//20 GPRS发起连接服务等待秒定时器
					pTerminalRam->GPRSReConnection_S_Timer = 30;
					pUartCtrl->NETOPENCount++;//开启服务次数计数(当前次数达到x次时关模块电源)
				}	
			}
			break;
		case AT_LOOP_NOLINK://AT循环,没连接
			pUartCtrl->LinkSubTask = 0;	// 尝试AT发送计数
			pUartCtrl->SubTask = 0;		// 子任务
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			if(pUartCtrl->NETOPENCount >= CommPara.RedialCnt)//开启服务连续次数计数(当前次数达到x次时关模块电源)
			{
				pUartCtrl->NETOPENCount = 0;			// 开启服务连续次数计数(当前次数达到x次时关模块电源)
				//pTerminalRam->LinkCount_SelfAdaption ++ ;	//切换物联卡参数
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					pUartCtrl->LinkCount++; 
					pTerminalRam->LinkCount_SelfAdaption++;
					if(pTerminalRam->LinkCount_SelfAdaption == 3)//切换到自适应参数 重新设置APN
					{
						pTerminalRam->SelfAdaptionChangeFlag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
					}
				}
				else if(pTerminalRam->LinkCount_SelfAdaption > 2)
				{
					pTerminalRam->LinkCount_SelfAdaption++;
					if(pTerminalRam->LinkCount_SelfAdaption >= 5)//所有通讯参数都已经试过
					{
						pTerminalRam->LinkCount_SelfAdaption = 0;
						pUartCtrl->LinkCount = 0;
						pUartCtrl->LinkCtrl = 0;
	//						WarningOccur(GPRSPORT,(u8 *)"远程无线主站连接失败");//告警发生,入口告警代码和告警字符串
						pUartCtrl->State = GPRS_STATE_LINK_FAIL;
						//err20190313 修改GPRS流程，主备IP切完连接失败不直接重启模块，重新配置通信参数后再尝试。
						pTerminalRam->GPRS_Para_Reset_Count++;
						if(pTerminalRam->GPRS_Para_Reset_Count>=3)//大循环尝试超过3次再硬件重启模块
						{
							pTerminalRam->GPRS_Para_Reset_Count = 0;
							pTerminalRam->GPRS_Para_Reset_Flag = 0;
							pUartCtrl->ATCount = AT_MYPOWEROFF; 	
						}
						else 
						{
							pTerminalRam->GPRS_Para_Reset_Flag = 1;
							pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
						}
					}
					else 
					{
						pTerminalRam->SelfAdaptionChangeFlag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//设置APN参数
					}
				}
				
				
			}
			else
			{
				if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
				{
					pUartCtrl->LINKCHECK_RTCS = pCommRam->RTCBuff[0];
					if(pTerminalRam->GPRSReConnection_S_Timer == 0x0)//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
					{
						pUartCtrl->ATCount = AT_MYNETSRV;//设置服务参数
					}
					else
					{
						pUartCtrl->ATCount = AT_SMS_NOLINK;//短信服务没连接前
					}
				}
			}
			break;
		case AT_CSQ_LINK://读信号连接后(每秒1次)
			if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{
				GPRS_QCSG_NextATCount();//下1个AT
				return;
			}
			break;
		case AT_SYSINFO:
		case AT_SYSINFO_LINK://
			//#if (USER/100)==14//福建系
				if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//
				{
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//
				}
				else
				{
					GPRS_QCSG_NextATCount();
					return;
				}
				break;
//			#else
//				GPRS_QCSG_NextATCount();
//				return;
			//#endif
		case AT_MYNETOPEN_LINK://连接检查(每秒1次)
			if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				pUartCtrl->LINKCHECK_RTCS = pCommRam->RTCBuff[0];
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{
				GPRS_QCSG_NextATCount();//下1个AT
				return;
			}
			break;
		case AT_MYNETREAD://读取数据
			p16 = (u16*)(ADDR_UART1IntRx);
			if(p16[0] != p16[1])
			{
				pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				return;
			}
			else
			{
				TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				i = GPRS_MAX_WRITE_DATA_LEN;
				i = hex_bcd(i);//HEX数转为8位压缩BCD数
				TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
				p8txbuff[TxByte++] = '\r';
			}
			break;
		case AT_MYNETWRITE://发送数据
			switch(pUartCtrl->ATWriteTask)					// AT数据发送任务:0=发命令,1=发数据
			{
				case 0://发AT头
					if(pTerminalRam->AT_MS_Timer != 0) return;
					i = pTerminalRam->CopyTxByte;			// 还需发送字节数
					if(pUartCtrl->Task != UART_TASK_TXING)
					{
						pUartCtrl->ATTask = 0;				// AT任务:0=空闲,1=发送,2=正在发送,3=接收
						pUartCtrl->TxByte = 0;
						GPRS_QCSG_NextATCount();
						return;
					}
					if((i == 0) || (i > GPRS_MAX_WRITE_DATA_LEN))
					{
						pUartCtrl->Task = UART_TASK_RX;
						pUartCtrl->ATTask = 0;				// AT任务:0=空闲,1=发送,2=正在发送,3=接收
						pUartCtrl->TxByte = 0;
						pTerminalRam->CopyTxByte = 0;
						WarningOccur(GPRSPORT,(u8 *)"远程无线发送长度错误");//告警发生,入口告警代码和告警字符串
						GPRS_QCSG_NextATCount();
						return;
					}
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);
					i = hex_bcd(i);
					TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte++] = '\r';
					break;
				case 1://发数据
					i = pTerminalRam->ReqTxByte;
					x = pTerminalRam->CopyTxByte;
					y = pTerminalRam->CopyTxCount;
					if((i > GPRS_MAX_WRITE_DATA_LEN) || (i == 0) || (x == 0))
					{//可发送长度错误,放弃本帧
						pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
						pUartCtrl->Task = UART_TASK_TX_DONE;		// 0=接收
						GPRS_QCSG_NextATCount();					// 下1个AT
						return;
					}
					pTerminalRam->CopyTxByte = x - i;				// 还需发送字节
					pTerminalRam->CopyTxCount = y + i;				// 已发送计数				
					pUartCtrl->TxByte = i;
				    pUartCtrl->TxCount = LEN_ATTx + y;
					#if LOG_1376_3==1//0=没,1=有
						LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
					Start_UARTn_Tx(GPRSPORT);//启动UART口发送
					
					x = MRR((u32)&pUartCtrl->RxTxByte,4);
					x += i;
					MWR(x, (u32)&pUartCtrl->RxTxByte, 4);//通信流量
					pUartCtrl->ATTask = 2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					return;
				default:
					pUartCtrl->ATWriteTask = 0;//AT数据发送任务:0=发命令,1=发数据
					break;
			}
			break;
		case AT_MYFTP://FTP文件下载
			if(pUartCtrl->LinkTask != 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				GPRS_QCSG_NextATCount();//下1个AT
				return;
			}
			// FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			// 下载完成
			FTPFileCount = MRR(ADDR_FTPFileFlags + 4, 1);//下载文件计数;0-x,=MaxFileNum关FTP,=(MaxFileNum+1)返回帧,=(MaxFileNum+2)下载完成显示,=(MaxFileNum+3)结束下载显示
			if(FTPFileCount > (MaxFileNum + 1))		// 最大下载文件个数
			{
				pUartCtrl->LinkTask2 = 0;			// 通道2链接任务(FTP命令)
				pUartCtrl->FTPLinkCount = 0;		// FTP尝试连接计数
				pUartCtrl->FTPErr = 0;				// FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				GPRS_QCSG_NextATCount();			// 下1个AT
				return;
			}
			pCommRam->DisplayAllErrorTimer = 10;//FTP文件下载显示秒定时器
			x = pUartCtrl->FTPErr;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			if((x != 0) || (FTPFileCount == (MaxFileNum + 1)))
			{
				if(x == 1)
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
				
				p8rxbuff=(u8 *)ADDR_DATABUFF;
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MR(ADDR_DATABUFF,ADDR_AFN0FF1_Rx,256);//恢复FTP接收帧的前256字节
				MR((u32)p8txbuff,ADDR_AFN0FF1_Tx,18);//恢复FTP返回帧的前18字节
				MWR(y,((u32)p8txbuff)+18,4);//返回标识4字节
				i=4+4;// 返回长度
				i+=8;// 1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				i=p8rxbuff[1]+(p8rxbuff[2]<<8);
				//Uart_376_AUX(GPRSPORT,((u32)p8rxbuff)+6+i-6);//附加信息(EC,TP),暂等待单帧数据发送
				//Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				
				GPRS_QCSG_NextATCount();//下1个AT
				pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				return;
			}
			//#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
			//		TxByte=GPRS_FTP_Tx();//GPRS_FTP_Tx发送
			//#else
			//		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			//		{
		    //		//p32=(u32 *)(ADDR_E1_ENTRY);
		    //		TxByte=(*(u32(*)())p32[106])();//GPRS_FTP_Tx发送
			//		}
			//#endif
			break;		
		case AT_SMS_NOLINK://短信服务没连接前
		case AT_SMS_LINK://短信服务
			#if LEN_UART11Ctrl == 0
				GPRS_QCSG_NextATCount();//下1个AT
			#else
				TxByte = GPRS_SMS_Tx();//GPRS_SMS_Tx发送;返回发送字节数
			#endif
			break;	
		case AT_LOOP://AT循环,在线控制
			if(pUartCtrl->LinkCtrl == 0)//连接控制0=不连接,1=连接
			{
				if(pUartCtrl->LinkTask == 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					pUartCtrl->LinkTask = 102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					pUartCtrl->HeartberatCount = 0;//心跳发出计数,收到确认帧清0
					pTerminalRam->GPRSHeartBeat_S_Timer = 0;
				}
				if(pUartCtrl->LinkTask == 103)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
					return;
				}
			}
			pUartCtrl->LinkSubTask = 0;		
			pUartCtrl->SubTask = 0;			
			pUartCtrl->ATCount = AT_CSQ_LINK;
			break;	
		case AT_MYRESET://err20190410 GPRS模块重启指令 用于SIM卡未插入时不关GPRS电源
			TxByte=CopyString((u8 *)"AT+CFUN=1,1\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
		default:
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
	}
	if(TxByte != 0)
	{
		#if (TEST_ON == 1)
		switch(pUartCtrl->BpsCtrlBack)
		{
			
				case 0://速率115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;			
				case 1://速率38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;				
				case 2://速率9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			
		}
		#else
			pUartCtrl->BpsCtrl = 0x3+(9<<5);//速率57.6K 
		#endif
		Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
		pUartCtrl->TxByte = TxByte;//发送字节数(半字对齐)
		pUartCtrl->TxCount = 0;
		#if LOG_1376_3==1//0=没,1=有
			switch(p32[4*AT_No])//AT代号
			{
				case AT_CSQ_LINK://读信号连接后(每秒1次)
				case AT_MYNETOPEN_LINK://连接检查(每秒1次)
				case AT_MYNETREAD://读数据			
					break;
				default:
					LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					break;
			}
		#endif
		Start_UARTn_Tx(GPRSPORT);//启动UART口发送
		pUartCtrl->ATTask = 2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	}
}


void GPRS_QCSG_AT_Rx(void)//AT命令接收
{
	u32 i, x, y, AT_No, RxByte, Addr, p0, p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	CommunicationParaStruct CommPara;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	u32 Temp_Code;
	DMA_UARTn_RX_Pointer(GPRSPORT);//计算DMA_UARTn接收指针值(p0)
	AT_No = pUartCtrl->ATCount;//AT计数
	if(AT_No > AT_Max)
	{
		AT_No = 0;
		pUartCtrl->ATCount = 0;//AT计数
	}
	p16fifo = (u16*)(ADDR_UART1IntATRx);
	if(p16fifo[0] != p16fifo[2])
	{//有字节接收
		p16fifo[2] = p16fifo[0];
		pTerminalRam->AT_MS_Timer = 50/10;		//AT接收第一个字节后超时定时值
	}

	if(pTerminalRam->AT_MS_Timer != 0) return;
	p32 = (u32*)(&GPRS_QCSG_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT:// 南网送检要求，上电伊始，发送AT指令，有应答开机成功，否则重试最少三次，最多30次，每秒一次
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成

			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 1S
		  	pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr == 0)
			{//重发本AT
				pUartCtrl->LinkSubTask++;				// 尝试AT发送计数
				if(pUartCtrl->LinkSubTask >= 30)		// 尝试AT发送计数,30次 //err20180626 台体测试时30次改为5次
				{										// 中兴MC8332模块当自动速率时头2次的AT无响应(用于检测),第3次才响应
					pUartCtrl->LinkSubTask=0;			// 尝试AT发送计数
					switch(pUartCtrl->SubTask)			// 子任务
					{
					#if (TEST_ON == 0)
						case 0://速率57600
						case 1://速率115200
						case 2://速率38400
						case 3://速率19200
						case 4://速率9600
					#endif	
					#if (TEST_ON == 1)
						case 0://速率57600						
						case 1://速率38400						
						case 2://速率9600
					#endif	
							pUartCtrl->SubTask++;//子任务
							return;
						default:
							#if (USER/100)==0//用户标识,电科院测试不支持AT+IPR
								WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
								GPRS_QCSG_NextATCount();//下1个AT
							#else
								WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
								pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
							#endif
							return;
					}
				}
				pUartCtrl->State = GPRS_STATE_MATCH_BAUD_RATE;
			}
			else
			{//接收到 OK
				p16fifo[1] = Addr - (ADDR_UART1IntATRx + 10);//修改已读指针
				
			#if (TEST_ON == 0)
				if(pUartCtrl->SubTask < 10)//子任务
				{
					pUartCtrl->SubTask += 10;//子任务
					pUartCtrl->LinkSubTask = 0;//尝试AT发送计数
				}
				else
				{
					GPRS_QCSG_NextATCount();//下1个AT
				}
			#else 
				pUartCtrl ->BpsCtrlBack = pUartCtrl->SubTask;
				GPRS_QCSG_NextATCount();//下1个AT
			#endif
			}
			return;
		case AT_MYTYPE://查询远程通信模块类型
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return;

			#if LOG_1376_3==1	
				LOG_ATRx(0);			//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			pCommRam->RemoteWirelessModuleType = 1;		// GPRS模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			Addr = ATRxSearch((u32)((u8*)"$MYTYPE:"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				x = ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(x != 0)
				{
					p8s = (u8*)x;
					i = ASCII_BCD(x,2);
					pCommRam->RemoteWirelessModuleType = i;//GPRS模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				}
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_QCSG_NextATCount();
				break;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_QCSG_NextATCount();
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2) pUartCtrl->LinkSubTask++;	//尝试AT发送计数
			}
			break;	
		case AT_MYGMR://获取模块软件版本
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));		// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_QCSG_NextATCount();
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_CPIN://卡检查，不做存储
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"READY"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_QCSG_NextATCount();
				break;
			}
			pUartCtrl->LinkSubTask++;//尝试AT发送计数
			if(pUartCtrl->LinkSubTask > 2)//尝试AT发送计数
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
				pUartCtrl->ATCount = AT_MYRESET;//关机
			}
			return;
			//goto ReTxAT;//重发本AT
		case AT_MYCCID://获取SIM卡序列号ICCID
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"$MYCCID"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				LineATIntRx(Addr+3,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_QCSG_NextATCount();//下1个AT
				break;
			}
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			goto ReTxAT;//重发本AT
		case AT_CNUM://取本机号码
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			p8d = (u8*)(ADDR_MobilePhoneNumber);
			p8d[0] = 0;
			Addr = ATRxSearch((u32)((u8*)"+CNUM:"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
					Addr = ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr != 0)
					{
						LineATIntRx(Addr,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
						p8s=(u8*)(ADDR_DATABUFF);
						if(p8s[0] == '"')
						{
							p8s++;
							for(i = 0; i < 19; i++)
							{
								x = p8s[i];
								if(x == '"') break;
								p8d[i] = x;
							}
							p8d[i] = 0;
						}
					}
				}
				else
				{//中兴CDMA(MC8332)
					LineATIntRx(Addr,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
					p8s = (u8*)(ADDR_DATABUFF);
					for(i = 0; i < 20; i++)
					{
						x = p8s[i];
						if((x < 0x30) || (x > 0x39)) break;
						p8d[i] = x;
					}
					p8d[i] = 0;
				}
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//尝试AT发送计数
				{
					pUartCtrl->LinkSubTask++;//尝试AT发送计数
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					break;
				}
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			GPRS_QCSG_NextATCount();//下1个AT
			break;
		case AT_CIMI://查国际移动台标识请求IMSI
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(Addr != 0)
			{
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
					p1 = p16fifo[1];
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					while((p8s[p1] == 0x0d)||(p8s[p1] == 0x0a))
					{
						p1++;
						if(p1 >= (LEN_UART1IntATRx-10))
						{
							p1 -= (LEN_UART1IntATRx-10);
						}
					}
					Addr = ADDR_UART1IntATRx+10+p1;
				}
				else
				{//中兴CDMA(MC8332)
					Addr=ATRxSearch((u32)((u8*)"+CIMI: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						pUartCtrl->LinkSubTask++;//尝试AT发送计数
						if(pUartCtrl->LinkSubTask>2)//尝试AT发送计数
						{
							GPRS_QCSG_NextATCount();//下1个AT
						}
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						}
						break;
					}
				}
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
				
				Temp_Code = 0;
				Temp_Code = (p8d[0]-'0')*10000 + 
							(p8d[1]-'0')*1000 +
							(p8d[2]-'0')*100 +
							(p8d[3]-'0')*10 +
							(p8d[4]-'0')*1;
				switch(Temp_Code)
				{
					case 46000:
					case 46002:
					case 46004:
					case 46007:
					case 46008:
						pCommRam->CSPN=1;	// 1=中国移动通信-----china mobile  CMCC
						break;
					case 46001:
					case 46006:
					case 46009:
					case 46010:
						pCommRam->CSPN=2;	// 2=中国联通通讯-----china unicom  CUCC
						break;
					case 46003:
					case 46005:
					case 46011:
					case 46012:
						pCommRam->CSPN=3;	// 3=中国电信  ------CHINA TELECOM  CTCC
						break;
					default:
						pCommRam->CSPN=0;//空
						break;
				}
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//尝试AT发送计数
				{
					pUartCtrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			GPRS_QCSG_NextATCount();//下1个AT
			break;
		case AT_CSPN://读取SIM卡服务运营商名称
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"+CSPN: \""));// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				//中国移动通信-----china mobile  CMCC
				//中国联通通讯-----china unicom  CUCC
				//中国电信  ------CHINA TELECOM  CTCC	
				x = GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				p8s = (u8*)(ADDR_DATABUFF);
				for(i = 0; i < x; i++)
				{
					if(p8s[i] == '"')
					{
						p8s[i] = 0;
						break;
					}
				}
				i = (p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|(p8s[3]<<0);
				switch(i)
				{
					case ('C'<<24)|('M'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=1;	// 1=中国移动通信-----china mobile  CMCC
						break;
					case ('C'<<24)|('U'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=2;	// 2=中国联通通讯-----china unicom  CUCC
						break;
					case ('C'<<24)|('T'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=3;	// 3=中国电信  ------CHINA TELECOM  CTCC
						break;
					default:
						pCommRam->CSPN=0;//空
						break;
				}
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			}	
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//尝试AT发送计数
				{
					pUartCtrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			GPRS_QCSG_NextATCount();//下1个AT
			break;			
		case AT_CSQ_NOLINK://读信号没连接前
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;				// 下一个AT指令发送间隔时间， 1S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"+CSQ: "));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	//修改已读指针
				pUartCtrl->LinkSubTask = 0;					//尝试AT发送计数
				pUartCtrl->SubTask = 0;						//子任务
				LineATIntRx(Addr,ADDR_DATABUFF,2);			//取AT中断接收数据线性放在缓冲中
				GPRS_Signal(ADDR_DATABUFF);					//GPRS信号强度0-4
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				if((pTerminalRam->SignaldBm >= 5) && (pTerminalRam->SignaldBm != 0x99))
				{//模块测到信号,若没信号则等待120秒
					GPRS_QCSG_NextATCount();
				}
				else
				{
					if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//GPRS等待信号强度查询(连接前)定时
					{
						WarningOccur(GPRSPORT,(u8 *)"远程无线无信号");//告警发生,入口告警代码和告警字符串
						pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
					}
				}
				return;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			goto ReTxAT;//重发本AT
		//以下几个参数设置失败时不关模块,以使能收短信
		case AT_MYNETCON_APN://设置APN
		case AT_MYNETCON_USERPWD://设置USERPWD:用户名和密码
		case AT_MYNETSRV://设置服务参数
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			//pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// 下一个AT指令发送间隔时间， 0.8S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));	// AT命令返回数据搜索；
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//尝试AT发送计数
				{
					pUartCtrl->LinkSubTask++;//尝试AT发送计数
					break;
				}
			}
			GPRS_QCSG_NextATCount();
			break;	
		case AT_CREG://获取当前网络注册状态
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;				// 下一个AT指令发送间隔时间， 1S
			pUartCtrl->ATTask = 0;							// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"+CREG:"));		// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr = ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					i = MRR(Addr,1);
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					switch(i)
					{
						//case '0'://没有注册，ME现在并没有在搜寻要注册的新的运营商
						//case '4'://未知原因
						default:
							if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//等待注册定时
							{
								WarningOccur(GPRSPORT,(u8 *)"远程无线注册失败");//告警发生,入口告警代码和告警字符串
								pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
							}
							return;
						case '1'://注册了本地网络
							GPRS_QCSG_NextATCount();//下1个AT
							break;
						case '2'://没有注册，但MS正在搜寻要注册的新的运营商
							if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//等待注册定时
							{
								WarningOccur(GPRSPORT,(u8 *)"远程无线注册搜寻超时");//告警发生,入口告警代码和告警字符串
								pUartCtrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '3'://注册被拒绝
							WarningOccur(GPRSPORT,(u8 *)"远程无线注册被拒绝");//告警发生,入口告警代码和告警字符串
							pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
							return;
						case '5'://注册了漫游网络
							WarningOccur(GPRSPORT,(u8 *)"远程无线注册漫游网络");//告警发生,入口告警代码和告警字符串
							GPRS_QCSG_NextATCount();//下1个AT
							break;
					}
					break;
				}
			}
			goto ReTxAT;//重发本AT
		case AT_MYNETACT://激活网络连接
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_CONFIG_LINK_PARA;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;			// 下一个AT指令发送间隔时间， 1S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));		// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
			}

			if((pTerminalRam->SelfAdaptionChangeFlag == 1) || (pTerminalRam->GPRS_Para_Reset_Flag == 1))
			{
				pUartCtrl->ATCount = AT_MYNETCON_APN;//设置APN参数
				pTerminalRam->SelfAdaptionChangeFlag = 0;
				pTerminalRam->GPRS_Para_Reset_Flag = 0;
			}
			else 
				GPRS_QCSG_NextATCount();
			break;
		case AT_MYIPFILTER://IP访问控制配置
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;					// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				pUartCtrl->LinkSubTask = 0;//尝试AT发送计数
				pUartCtrl->SubTask++;//子任务
				break;
			}
			goto ReTxAT;
		case AT_MYNETOPEN://开启服务
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pUartCtrl->State = GPRS_STATE_LINKING;
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;					// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr - (ADDR_UART1IntATRx+10);	// 修改已读指针
				WarningCancel(GPRSPORT);					// 告警取消,入口告警代码
				//pUartCtrl->LinkTask = 100;
				pUartCtrl->LinkSubTask = 0;					// 尝试AT发送计数
				pUartCtrl->SubTask = 0;						// 子任务
				pUartCtrl->ATCount = AT_CSQ_LINK;			// 读信号连接后
				pUartCtrl->LinkCount = 0;					// 连接成功后，连接次数清零 
				//pTerminalRam->LinkCount_SelfAdaption = 0;		// 连接成功后，自适应连接次数清零
				//pTerminalRam->SelfAdaptionChangeFlag = 0;		// 连接成功 自适应参数切换标识清零
				MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
				MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
				i = CommPara.HeartBeatTimeGap;
				if(i == 0x0) i = 1;		//最小1分
				i *= 60;
				i += 60;					//加60秒可能的主站发出定时误差
				pTerminalRam->GPRSHeartBeat_S_Timer = 0;
				if((pUartCtrl->TCPUDP & 0x30) == 0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
				{//是服务器模式
					if((pUartCtrl->TCPUDP & 0x80) != 0)
					{//是UDP
						i = 6000;//UDP初始等待主站登入定时60分
					}
					pTerminalRam->GPRSHeartBeat_S_Timer = i;//GPRS心跳秒定时器
				}			
				pUartCtrl->State = GPRS_STATE_LINK_SUCCESS;
			
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
				GPRS_QCSG_NextATCount();
			}
			break;		
		case AT_CSQ_NOLINK2://读信号没连接2	
		case AT_CSQ_LINK://读信号连接后
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				if(p32[4*AT_No] != AT_CSQ_LINK)//AT代号
				{
					LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
				}
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// 下一个AT指令发送间隔时间， 0.8S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"+CSQ: "));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				LineATIntRx(Addr, ADDR_DATABUFF, 2);//取AT中断接收数据线性放在缓冲中
				GPRS_Signal(ADDR_DATABUFF);//GPRS信号强度0-4
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_QCSG_NextATCount();//下1个AT
				break;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			goto ReTxAT;//重发本AT
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://查询注册网络类型：3G/4G
//			#if (USER/100)==14//福建系
				Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
				
				#if LOG_1376_3==1//0=没,1=有
					LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
				#endif
				pTerminalRam->AT_MS_Timer = 50;				// 下一个AT指令发送间隔时间， 0.5S
				pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
				pCommRam->RemoteWirelessModuleType = 1;		// 远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				Addr = ATRxSearch((u32)((u8*)"$MYSYSINFO: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					p8d = (u8 *)Addr;
					i = p8d[0]-0x30;
					switch(i)
					{		
						case 2://GSM/GPRS模式 2G
							pCommRam->RemoteWirelessModuleType = (1 << 0); 	//b0=1
							break;
						case 3://WCDMA模式 3G
							pCommRam->RemoteWirelessModuleType = (1 << 1); 	//b1=1
							break;
						case 4://17://LTE模式 4G
							pCommRam->RemoteWirelessModuleType = (1 << 5); 	//b5=1
							break;
						case 0://无服务
						default:
							break;
					}
				}
//			#endif
			GPRS_QCSG_NextATCount();//下1个AT
			break;
		case AT_MYNETOPEN_LINK://查询网络连接
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// 下一个AT指令发送间隔时间， 0.8S
			pUartCtrl->ATTask = 0;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				if(pUartCtrl->LinkTask < 101)
				{				
					pUartCtrl->LinkTask = 100;
				}
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	// 修改已读指针
				MC(0,ADDR_MobilePhoneNumber+40,16+6);
				//LocalIP
				p16fifo[1] += 2;
				Addr += 2;
				x = ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(x == 0) 
				{
					x = ATRxSearch((u32)((u8*)","));
					p16fifo[1] -= 1;
					Addr -= 1;
				}
				if(x > Addr)
				{
					x -= Addr;
					x -= 1;
					if(x < 16)
					{
						MR(ADDR_MobilePhoneNumber+40, Addr, x);
						//显示本地IP
						CopyString((u8 *)"本IP ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						MR(ADDR_DATABUFF+5, Addr, x);
						MC(0, ADDR_DATABUFF+5+x, 1);
						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
						//local_port
						Addr += x;
						Addr += 2;
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						x = ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if(x > Addr)
						{
							x -= Addr;
							x -= 1;
							if(x < 6) MR(ADDR_MobilePhoneNumber+40+16, Addr, x);
						}	
						CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						i = DataComp(ADDR_MobilePhoneNumber+40, ADDR_DATABUFF, 7);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
						if(i == 0)
						{//断开连接
							Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
							if(Addr != 0)
							{
								p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							}
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
							pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
							return;
						}
					}
				}
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_QCSG_NextATCount();//下1个AT
				break;
			}
			goto ReTxAT;
		case AT_MYNETREAD:		// 读取数据
			Addr = ATRxSearch((u32)((u8*)"$MYNETREAD: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.7S
			pUartCtrl->ATTask = 0;					// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			p1 = p16fifo[1];						// 原p1
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				MC(0, ADDR_DATABUFF, 64);
				i = GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				if(i != 0)
				{
					RxByte = ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
					i = GetATRxByte();//中断接收,计算当前p0对应p1还有未处理字节数
					if(i < (RxByte + 4))//当字节数为0时原RxByte+6为空行0x0d0x0a加OK0x0d0x0a,现改为去掉空行OK0x0d0x0a,为二者兼空用RxByte+4
					{
						Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
						if(Addr != 0)
						{
							p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						}
						GPRS_QCSG_NextATCount();
						return;
					}
					i = MRR((u32)&pUartCtrl->RxTxByte, 4);
					i += RxByte;
					MWR(i, (u32)&pUartCtrl->RxTxByte, 4);//通信流量			
					x = p16fifo[1];
					while(x >= (LEN_UART1IntATRx-10))
					{
						x -= (LEN_UART1IntATRx-10);
					}
					#if LOG_1376_3==1//0=没,1=有
						if(RxByte!=0)
						{
							y=p16fifo[0];//保护p0
							p16fifo[0]=x;//p0用p1值,先仅ASCII码
							p16fifo[1]=p1;
							LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
							p16fifo[0]=y;//恢复p0
							p16fifo[1]=x;//恢复p1
						}
					#endif
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					p16 = (u16*)(ADDR_UART1IntRx);
					y = p16[0];
					if(y >= (LEN_UART1IntRx-10)) y = 0;

					p8d = (u8*)(ADDR_UART1IntRx+10);	
					for(i = 0; i < RxByte; i++)
					{
						p8d[y]=p8s[x];
						x++;
						if(x >= (LEN_UART1IntATRx-10)) x = 0;
						y++;
						if(y >= (LEN_UART1IntRx-10)) y = 0;
					}
					p16fifo[1] = x;			//修改已读指针
					p16[0] = y;				//接收指针
					#if LOG_1376_3==1//0=没,1=有
						if(RxByte != 0)
						{
							LOG_LINK(13763, 0, GPRSPORT, (u8 *)ADDR_UART1IntRx+10, RxByte, 0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
							p16fifo[2] = x;//修改已读指针
							LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
						}
					#endif
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}

					if(RxByte == 0)
					{	
						pUartCtrl->Round++;
						if(pUartCtrl->Round > 60)
						{
							pUartCtrl->Round = 0;
							GPRS_QCSG_NextATCount();
						}
					}
					else
					{
						UartReveiveFrame(GPRSPORT);
						p16[0] = 0;
						p16[1] = 0;
						p16[2] = 0;
						p16[3] = 0;
						p16[4] = 0;
					}
					return;
				}
			}
			else
			{
				Addr = ATRxSearch((u32)((u8*)"ERROR: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10)-3;//修改已读指针,3字节错误代码，err:913
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
					return;
				}
			}
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			GPRS_QCSG_NextATCount();
			break;
		case AT_MYNETWRITE://发送数据
			if(pUartCtrl->ATWriteTask == 0)//AT数据发送任务:0=发命令,1=发数据,2=等待发送完成延时
			{
				Addr = ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
				
				#if LOG_1376_3==1//0=没,1=有
					LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
				#endif	
				y = p16fifo[1];//y=原p1
				Addr = ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					pUartCtrl->ATTask = 1;						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	// 修改已读指针
					MC(0,ADDR_DATABUFF,64);
							
					p0 = p16fifo[0];//p0
					p1 = p16fifo[1];//p1
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					p8d = (u8*)(ADDR_DATABUFF);
					if(p0 >= p1)
					{
						RxByte = p0 - p1;
					}
					else
					{
						RxByte = (p0+(LEN_UART1IntATRx-10)) - p1;
					}
					
					i = 0;
					while(RxByte != 0)	//取长度字符串
					{
						p8d[i] = p8s[p1];
						i++;
						if(i >= 8) i = 0;	//字符串太长错
						p1++;
						if(p1 >= (LEN_UART1IntATRx-10)) p1 = 0;
						RxByte--;
					}
					p16fifo[1] = p1;					
					i = ASCII_HEX(ADDR_DATABUFF);				//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
					if(i >= pTerminalRam->CopyTxByte)			// 本次可以发送字节数
					{
						pTerminalRam->ReqTxByte = i;			// 请求发送字节数
						pUartCtrl->LinkSubTask = 0;				// 尝试AT发送计数
						pUartCtrl->ATWriteTask = 1;				// AT数据发送任务:0=发命令,1=发数据
						pTerminalRam->GPRSWaitTx_S_Timer = 60;	// 1分,GPRS发送数据等待超时秒定时器
					}
					else
					{
						WarningOccur(GPRSPORT,(u8 *)"远程无线发送缓冲不足");//告警发生,入口告警代码和告警字符串
						pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
					}
				}
				else
				{// 发送数据第一步请求失败，则 1S 后重试
					pTerminalRam->AT_MS_Timer = 100;
					pUartCtrl->ATTask = 1;
				}
				if(pTerminalRam->GPRSWaitTx_S_Timer != 0) return;		// 2分,GPRS发送数据等待超时秒定时器
				if(pUartCtrl->LinkTask == 101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					WarningOccur(GPRSPORT,(u8 *)"远程无线等待发送超时");//告警发生,入口告警代码和告警字符串
				}
				else
				{
					WarningOccur(GPRSPORT,(u8 *)"远程无线登录主站失败");//告警发生,入口告警代码和告警字符串
				}
				pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
			}
			else if(pUartCtrl->ATWriteTask == 1)
			{
				Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
				
				#if LOG_1376_3==1//0=没,1=有
					LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
				#endif
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				pUartCtrl->ATWriteTask = 2;
				pTerminalRam->AT_MS_Timer = 50 + ((pTerminalRam->ReqTxByte / 10) << 1);	// 等待发送延时，0.5 + 发送字节数对应的时间
			}
			else
			{
				if(pTerminalRam->AT_MS_Timer != 0) return;
				pUartCtrl->Task = UART_TASK_TX_DONE;
				pUartCtrl->ATTask = 1;			// AT任务:0=空闲,1=发送,2=正在发送,3=接收
				pUartCtrl->LinkSubTask = 0;		// 尝试AT发送计数
				pUartCtrl->ATWriteTask = 0;		// AT数据发送任务:0=发命令,1=发数据,2=等待发送完成延时
				GPRS_QCSG_NextATCount();		// 等待数据发送已经延时，下一条 AT 指令无需再次延时
				pTerminalRam->CopyTxByte = 0;
			}
			break;
		case AT_MYFTP://FTP文件下载
			#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
				GPRS_FTP_Rx();//GPRS_FTP_Rx接收
			#else
				if((pCommRam->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
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
		case AT_HOLD://HOLD暂停
			pUartCtrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			break;
		case AT_MYPOWEROFF://关机
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				pUartCtrl->LinkTask = 0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				pCommRam->UART1Timer = 15000/10;//关机延时15s
			}
			else
			{
				pUartCtrl->LinkSubTask++;			//尝试AT发送计数
				if(pUartCtrl->LinkSubTask <= 2)		//尝试AT发送计数
				{
					pUartCtrl->ATTask = 1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
				}
				else
				{
					//南网要求没有收到确认帧后拉低ON/OFF引脚2s，再延时15s等待关机
					Pin_Configure(PIN_ON_GPRS,//输出
									0|//B3-B0:复用功能选择AF(0-15)
						 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
					pTerminalRam->GPRS_ONOFF_MS_Timer = 201;
					pUartCtrl->LinkTask = 0;
					pCommRam->UART1Timer = 15000/10;	//关机延时15s
					MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
					pTerminalRam->GPRSReConnection_S_Timer = bcd_hex(CommPara.RedialTimeGap);
					pTerminalRam->GPRSReConnection_S_Timer *= 60;
				}
			}

			///////////////////////////ybwhz测试///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64);	
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'G';
			p8[count++] = 'P';
			p8[count++] = 'R';
			p8[count++] = 'S';
			p8[count++] = ' ';
			p8[count++] = 'O';
			p8[count++] = 'F';
			p8[count++] = 'F';
			
			p8[count++] = '#';

			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}
		////////////////////////////ybwhz测试/////////////////////////////
			
			break;

		case AT_MYRESET://err20190410 GPRS模块重启指令
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				pTerminalRam->Signal = 0;			//GPRS信号强度0-4
				pTerminalRam->SignaldBm = 0x99;		//信号强度0-31或99	
				pUartCtrl->LinkCount = 0;
				pUartCtrl->LinkTask = 2;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				pCommRam->UART1Timer = 15000/10;//关机延时15s
			}
			else
			{
				pUartCtrl->LinkSubTask++;			//尝试AT发送计数
				if(pUartCtrl->LinkSubTask > 2)		//尝试AT发送计数
				{
					//pUartCtrl->ATTask = 1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
				}
			}
			//TxByte=CopyString((u8 *)"AT$CFUN=1,1\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
		default:
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //接收没完成
			
			#if LOG_1376_3==1//0=没,1=有
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// 下一个AT指令发送间隔时间， 0.5S
			pUartCtrl->ATTask = 0;					// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_QCSG_NextATCount();//下1个AT
			}
			else
			{
			ReTxAT://重发本AT
				pUartCtrl->LinkSubTask++;//尝试AT发送计数
				if(pUartCtrl->LinkSubTask > 2)//尝试AT发送计数
				{
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
				}
				return;
			}
			break;
	}
}


void GPRS_QCSG_AT_RxTx(void)//GPRS_QCSG模块AT接收发送
{
	u32 *p32;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	switch(pUartCtrl->ATTask)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	{
		case 1://发送
			GPRS_QCSG_AT_Tx();//AT命令发送
			break;
		case 2://正在发送
			if(Wait_UARTn_Tx(GPRSPORT)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				p32=(u32*)(&GPRS_QCSG_AT_Command_Lib);
				switch(p32[4*pUartCtrl->ATCount])//AT代号
				{
					case AT_SMS_NOLINK://短信服务没连接前
					case AT_SMS_LINK://短信服务连接
						if(pUartCtrl->SubTask == 1)//子任务
						{//发SMS的数据
							pTerminalRam->AT_MS_Timer = 30000/10;//GPRS AT命令用10MS定时器
						}
						else
						{
							pTerminalRam->AT_MS_Timer = p32[(4*pUartCtrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
						}
						break;
					default:
						pTerminalRam->AT_MS_Timer = p32[(4*pUartCtrl->ATCount)+3]/10;//GPRS AT命令用10MS定时器
						break;
				}
				pUartCtrl->ATTask = 3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
				break;
			}
			break;
		case 3://接收
			GPRS_QCSG_AT_Rx();//AT命令接收
			break;
		default:
			pUartCtrl->ATTask = 0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			break;
	}
}


void UnInit_GPRS_QCSG(void)//GPRS模块引脚为输入下拉
{
	Pin_Configure(PIN_GPRS_PCTRL,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RST_GPRS,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ON_GPRS,//输出
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		 (1<<11));//B11  :当为输出时：0=输出0,1=输出1
}

void GPRS_QCSG_RxTx(u32 PORTn)//远程无线GPRS模块接收发送
{
	u32 i;
	CommunicationParaStruct* CommPara;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	UartTaskCtrlStruct* UartTask = (UartTaskCtrlStruct*)ADDR_GPRS_UART_Task_Ctrl;

#if ((Project/100)==2)||((Project/100)==3 || ((Project/100)==9))	
	i =  Pin_Read(PIN_GPRS_ID4);
#endif	

	i |= Pin_Read(PIN_GPRS_ID3) << 1;
	i |= Pin_Read(PIN_GPRS_ID2) << 2;
	i |= Pin_Read(PIN_GPRS_ID1) << 3;
	
#if ((Project/100)==2)||((Project/100)==3)	|| ((Project/100)==9)
	i |= Pin_Read(PIN_GPRS_ID0) << 4;
#endif

#if ((Project/100)==2)||((Project/100)==3)	|| ((Project/100)==9)
	if(i == 0x1F)
#endif

#if ((Project/100)==5)	
	if(i == 0x0E)
#endif
	{
		pTerminalRam->MODULSEL = i;
		pTerminalRam->Signal = 0;
		pUartCtrl->LinkTask = 0;
		WarningOccur(GPRSPORT,(u8 *)"远程模块没插入");
		pUartCtrl->State = GPRS_STATE_UNPLUGGED;
		return;
	}
	if(pTerminalRam->MODULSEL != i)
	{
		WarningCancel(GPRSPORT);
		pTerminalRam->MODULSEL = i;
		pUartCtrl->LinkTask = 0;
	}

	if(pCommRam->POWER < 3)					// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		if(pUartCtrl->LinkTask > 2)			// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		{//开电源后
			if(Pin_Read(PIN_ENR5V0) == 0)		// 读引脚,入口引脚使用名;返回0或1
			{//中断中关了电源
				pUartCtrl->PORT = 0;			// 本机端口号,同时用于掉电时已连接过1次识别
				pUartCtrl->LinkTask = 1;		// 1=关电源
				return;
			}
		}
	}
	switch(pUartCtrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		case 0:	// 0=关机延时
			MR((u32)CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			if(pTerminalRam->GPRSReConnection_S_Timer > ((bcd_hex(CommPara->RedialTimeGap))*60))//err20190104 RAM测试 防止RAM意外错误导致上线时间过长
				pTerminalRam->GPRSReConnection_S_Timer = (bcd_hex(CommPara->RedialTimeGap)) * 60;
			if(pTerminalRam->GPRSReConnection_S_Timer == 0)
			{
				
				pTerminalRam->Signal = 0;			//GPRS信号强度0-4
				pTerminalRam->SignaldBm = 0x99;		//信号强度0-31或99	
				if(pCommRam->UART1Timer == 0x0) 
				{
					pUartCtrl->LinkTask++;
				}
				pUartCtrl->LinkCount = 0;
				pTerminalRam->LinkCount_SelfAdaption = 0;//关机 自适应连接次数清零
				pTerminalRam->SelfAdaptionChangeFlag = 0;//关机 自适应切换标识清零
			}
			pUartCtrl->State = GPRS_STATE_POWER_OFF;
			return;
		case 1:	// 1=关总电源
			OffR5V0();				// 关R5V0电源		
			if((pUartCtrl->LinkCtrl != 0) && (pUartCtrl->LinkCount < MASTER_ADDR_NUM))
			{
				UnInit_GPRS_QCSG();	
				pUartCtrl->BpsCtrl = 0x3+(10<<5);	//初始速率115200
				Init_UARTn(GPRSPORT);				//初始化UART口,入口通信控制字已填入
				pCommRam->UART1Timer = 10000/10;
				pUartCtrl->LinkTask++;
			}	
			pUartCtrl->State = GPRS_STATE_POWER_OFF;
			return;
		case 2://开总电源
			if(pCommRam->POWER < 3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{	
				if(pUartCtrl->PORT == 0)
				{//本机端口号,同时用于掉电时已连接过1次识别
					pUartCtrl->PORT = 1; 
				}
				else
				{
					return;//电池供电时连接1次后停止
				}
			}
			//GPRS_Voltage();//GPRS电源电压ADC测量
			//if(pCommRam->UGPRS < 0x0030)//BCD 2位小数
			//{//<0.30V
			//	if(pCommRam->UART1Timer <= 10000) pCommRam->UART1Timer = 0x0;	//至小已延时10秒 + (20秒的关电源延时)
			//}
			if((pCommRam->UART1Timer == 0x0) && 
			   (pUartCtrl->LinkCtrl != 0) && 
			   (pUartCtrl->LinkCount < MASTER_ADDR_NUM))
			{										// 关机后至少 10 S，允许连接时，继续开机进行连接，否则死循环在此			
				Pin_OutH(PIN_RST_GPRS);
				OnR5V0();							// 打开R5V0电源
				Pin_OutH(PIN_GPRS_PCTRL);
				pCommRam->UART1Timer = 201/10;		// 开总电源延时0.2秒
				pUartCtrl->LinkTask++;	
				pUartCtrl->State = GPRS_STATE_POWER_ON;
			}
			return;
		case 3:// 开模块ON/OFF输出=0
			if(pCommRam->UART1Timer != 0x0) return;
			if(pCommRam->POWER == 3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  			{
				GPRS_Voltage();//GPRS电源电压ADC测量
				//if((pCommRam->UGPRS < 0x0350) || (pCommRam->UGPRS > 0x0450))	// BCD 2位小数
				//{																// 3.80V>电压>4.20V(送电科院时免误判,用3.50V和4.50V)
				//	pCommRam->ErrorIC |= 1 << 30;								// B30=GPRS模块电源
				//	pCommRam->DisplayAllErrorTimer = 10;						// 上电全显或系统错误显示秒定时器
				//}
			}
			MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
			pUartCtrl->Task=0;//0=正在接收
			pUartCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
			pUartCtrl->RxByte=0;//接收字节数=0
			pUartCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
			pUartCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0		
			pUartCtrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			pUartCtrl->ATCount=0;//AT计数
			pUartCtrl->LinkSubTask=0;//尝试AT发送计数
			pUartCtrl->SubTask=0;//子任务
			pUartCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
			pUartCtrl->LinkCount = 0;
			pTerminalRam->LinkCount_SelfAdaption = 0;//自适应连接次数 清零
			pTerminalRam->SelfAdaptionChangeFlag = 0;//自适应切换标识 清零
			pUartCtrl->NETOPENCount=0;//开启服务连续次数计数(当前次数达到x次时关模块电源)
			pUartCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			pTerminalRam->GPRSHeartBeat_S_Timer=0;//心跳发出秒定时器
			Pin_Configure(PIN_ON_GPRS,//ON输出=0
							0|//B3-B0:复用功能选择AF(0-15)
				 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				 (0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			pTerminalRam->GPRS_ONOFF_MS_Timer = 102;
			Pin_Configure(PIN_RST_GPRS,//RESET输入,上拉
							0|//B3-B0:复用功能选择AF(0-15)
				 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				 (0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			pCommRam->UART1Timer = 2000/10;	// 2S延时等待中断中完成
			pUartCtrl->LinkTask++;			// 链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;//共用UART1Timer直接返回
		case 4://PIN_ON_RST输出1S±10%延时
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_OutH(PIN_ON_GPRS);//ON输出=1
				pCommRam->UART1Timer = 10000/10;//延时后复位
				pUartCtrl->LinkTask = 7;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;
		case 5://复位
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET
								0|//B3-B0:复用功能选择AF(0-15)
					 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					 (0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				
				pCommRam->UART1Timer = 300/10;//复位延时0.3秒(>=200ms)
				pUartCtrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;
		case 6://复位解除
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET输入,上拉
									0|//B3-B0:复用功能选择AF(0-15)
						 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						 (0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				pCommRam->UART1Timer = 10000/10;//发AT命令前延时;GPRS上电后10秒内完成初始化并能接受AT命令
				pUartCtrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 7://初始化UART1
			if(pCommRam->UART1Timer == 0x0)
			{
				pUartCtrl->BpsCtrl = 0x3+(10<<5);	//初始速率115200
				Init_UARTn(GPRSPORT);				//初始化UART口,入口通信控制字已填入
				pUartCtrl->LinkTask++;				//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				pUartCtrl->State = GPRS_STATE_MATCH_BAUD_RATE;
			}
			return;//共用UART1Timer直接返回
		case 8://初始化AT命令
			break;
		case 100://连接ok
			pUartCtrl->State = GPRS_STATE_LINK_SUCCESS;
			break;
		case 101://登录ok
			pUartCtrl->State = GPRS_STATE_LOG_SUCCESS;	

			break;
		case 102://发退出登录
			break;
		case 103://退出登录
		case 104://关机
			//if(pUartCtrl->ATTask == 0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			//{
			//	pUartCtrl->ATCount = AT_MYPOWEROFF;//关机
			//	pUartCtrl->ATTask = 1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			//}
			pUartCtrl->LinkTask = 1;
			break;
		default:
			pUartCtrl->LinkTask = 0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;
	}
	
	if(pUartCtrl->ATTask != 0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	{
		GPRS_QCSG_AT_RxTx();
	}
	
	switch(pUartCtrl->Task)
	{
		case UART_TASK_RX:
			if((pUartCtrl->ATTask == 0) || (pUartCtrl->ATTask > 3)) pUartCtrl->ATTask = 1;	// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			break;
		case UART_TASK_RX_DONE:
			UartTask->Flag |= UART_FLAG_RX_DONE;
			UartTask->FrameLen = pUartCtrl->RxByte;
			UartTask->FrameType = pUartCtrl->FrameType;
			UartTask->FrameBuf = (u8*)Get_ADDR_UARTnRx(PORTn);
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;	// AT任务:0=空闲,1=发送,2=正在发送,3=接收
		
			#if LOG_1376_1==1		// 0=没,1=有
			LOG(13761, 0, GPRSPORT, UartTask->FrameBuf, UartTask->FrameLen, 0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			break;
		case UART_TASK_TX:					
			if(pUartCtrl->ATTask == 0)						// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			{
				#if LOG_1376_1==1		// 0=没,1=有
				LOG(13761, 1, GPRSPORT, (u8 *)Get_ADDR_UARTnTx(GPRSPORT), pTerminalRam->CopyTxByte, 0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
				//pTerminalRam->CopyTxByte = pUartCtrl->TxByte;	// 发送字节数在串口任务函数中已填入，pUartCtrl->TxByte字段会被其他命令更新，不能使用
				pUartCtrl->TxCount = 0;							// 已发送计数
				pTerminalRam->CopyTxCount = 0;					// 已发送计数
				pTerminalRam->GPRSWaitTx_S_Timer = 2*60;		// 2分,GPRS发送数据等待超时秒定时器
				pUartCtrl->ATCount = AT_MYNETWRITE;				// 发送数据
				pUartCtrl->ATWriteTask = 0;						// AT数据发送任务:0=发命令,1=发数据
				pUartCtrl->Task = UART_TASK_TXING;
				pTerminalRam->AT_MS_Timer = 0;
				pUartCtrl->ATTask = 1;
			}
			break;
		case UART_TASK_TXING:		//数据正在发送
			if(pUartCtrl->ATTask == 0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			{//不明原因冲突或意外RAM错误等等
				WarningOccur(GPRSPORT,(u8 *)"GPRS任务有BUG");//告警发生,入口告警代码和告警字符串
				pUartCtrl->LinkTask = 0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				//重拨间隔定时器置为0以便立即重连
				pTerminalRam->GPRSReConnection_S_Timer = 0;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
			}
			break;
		case UART_TASK_TX_DONE:
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;							// AT任务:0=空闲,1=发送,2=正在发送,3=接收
			pUartCtrl->TxCount = 0;							// 已发送计数
			pTerminalRam->CopyTxByte = 0;					// 发送字节数
			pTerminalRam->CopyTxCount = 0;					// 已发送计数
			pTerminalRam->GPRSWaitTx_S_Timer = 0;			// 2分,GPRS发送数据等待超时秒定时器
			CommPara = (CommunicationParaStruct*)ADDR_DATABUFF;
			MR((u32)CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara->HeartBeatTimeGap);
			pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
			if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
			UartTask->Flag |= UART_FLAG_TX_DONE;
			break;
		default:
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;
			break;
	}

}



void GPRSTest(void)
{

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





