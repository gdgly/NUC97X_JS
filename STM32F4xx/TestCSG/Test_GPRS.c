
//无线GPRS模块 1376.3标准
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
//#include "../Device/ATNo.h"
//#include "../Terminal/ATString.h"
//#include "../Device/GPRS_FTP_RxTx.h"
//#include "../Device/GPRS_SMS_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
//#include "../Terminal/Terminal_Uart_3761.h"
//#include "../Terminal/Terminal_Uart_3761_COM.h"

#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
//#include "../Display/Warning.h"
//#include "../TEST/LOG.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Display/Display.h"
#include "../TEST/LOG.h"




//定义AT命令号
#include "../Device/ATNo.h"


u32 ATRxSearch(u32 ADDR_String)//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
{
	u32 i;
	u32 x;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;

	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_String);
	Byte=0;//字符串比较字节数
	while(p8d[Byte]!=0)
	{
		Byte++;
	}
	while(p0!=p1)
	{
		if(p0>=p1)
		{
			i=p0-p1;
		}
		else
		{
			i=(p0+(LEN_UART1IntATRx-10))-p1;
		}
		if(i<Byte)
		{
			return 0;
		}
		x=p1;
		for(i=0;i<Byte;i++)
		{
			if(p8s[x]!=p8d[i])
			{
				break;
			}
			x++;
			if(x>=(LEN_UART1IntATRx-10))
			{
				x=0;
			}
		}
		if(i==Byte)
		{
			return (((u32)p8s)+x);
		}
		p1++;
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
	}
	return 0;
}

u32 GetATRxString(u32 ADDR_StrBuff)//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
{
	u32 x;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;

	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_StrBuff);
	Byte=0;
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	//去字符串前的无效0x0d,0x0a
	while(Byte!=0)
	{
		if((p8s[p1]==0x0d)||(p8s[p1]==0x0a))
		{
			Byte--;
			p1++;
			if(p1>=(LEN_UART1IntATRx-10))
			{
				p1=0;
			}
		}
		else
		{
			break;
		}
	}
	//取字符串
	x=0;
	while(Byte>=2)
	{
		if(p8s[p1]==0x0d)
		{
			p1++;
			if(p1>=(LEN_UART1IntATRx-10))
			{
				p1=0;
			}
			if(p8s[p1]==0x0a)
			{
				p1++;
				if(p1>=(LEN_UART1IntATRx-10))
				{
					p1=0;
				}
				p16[1]=p1;
				return x;
			}
			else
			{
				if(p8s[p1]==0x0d)
				{//中兴cdma模块
					p1++;
					if(p1>=(LEN_UART1IntATRx-10))
					{
						p1=0;
					}
					if(p8s[p1]==0x0a)
					{
						p1++;
						if(p1>=(LEN_UART1IntATRx-10))
						{
							p1=0;
						}
						p16[1]=p1;
						return x;
					}
				}
			}
			return 0;
		}
		p8d[x]=p8s[p1];
		x++;
		if(x>=LEN_DATABUFF)
		{//通常字符串先读到ADDR_DATABUFF
			x=0;
		}
		p1++;
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
		Byte--;
	}
	return 0;
}

u32 GetATRxByte(void)//中断接收,计算当前p0对应p1还有未处理字节数
{
	u32 p0;
	u32 p1;
	u16 *p16;
	
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		p16[0]=0;
		p16[1]=0;
		p16[2]=0;
		return 0;
	}
	while(p1>=(LEN_UART1IntATRx-10))
	{
		p1-=(LEN_UART1IntATRx-10);
		p16[1]=p1;
	}
	if(p0<p1)
	{
		p0+=(LEN_UART1IntATRx-10);
	}
	return(p0-p1);
}

void GPRS_Signal(u32 Addr)//GPRS信号强度0-4
{
	u32 i;
	u8 *p8;
	u8 *p8s;
#if (USER/100)==11//吉林系
	u32 x;
#endif

	p8s=(u8*)(Addr);
#if MainProtocol==376
	p8=(u8 *)(ADDR_AFN0CF4+1+16+6);
#else
	p8=(u8 *)(ADDR_DATABUFF);
#endif
	p8[0]=p8s[0];
	i=p8s[0]-0x30;
	if(p8s[1]==0x2c)//','号
	{
		p8[1]=0;
	}
	else
	{
		p8[1]=p8s[1];
		p8[2]=0;
		i<<=4;
		i|=p8s[1]-0x30;
	}
	Terminal_Ram->SignaldBm=i;//信号强度0-31或99
	if(i==0x99)
	{
		i=0;
	}
	i=bcd_hex(i);//8位压缩BCD数转为HEX
#if (USER/100)==11//吉林系
	//扩展AFN0CF201终端网络信号品质
	x=i;
	if(x>31)
	{
		x=0;
	}
	x*=2;
	x=113-x;
	x=hex_bcd(x);
	x<<=4;
	x|=0x8000;
	p8=(u8 *)(ADDR_AFN0CF201);
	p8[0]=x;
	p8[1]=(x>>8);
#endif	
	if(i>=10)
	{
		i=(i-10)/4;//信号强度代号(0-4)
		if(i>4)
		{
			i=4;
		}
		if(i==0)
		{
			i=1;//>=10以上最小为1格
		}
		Terminal_Ram->Signal=i;
	}
	else
	{
		Terminal_Ram->Signal=0;
	}
}


//SocketID 0-5使用约定
//0 主连接
//1 FTP控制
//2 FTP数据
//3-5 未用


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
	ALIGN 4	
};





__asm void GPRS_3763_AT_Command_Lib(void)//AT顺序执行库
{
//波特率检查
	dcd AT_AT
	dcd ADDR_ATStr_AT//AT命令字符串地址
	dcd ADDR_ERRStr_AT//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)

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
	dcd 1000//AT命令返回第1个字符定时(ms)

//读取SIM卡服务运营商名称
	dcd AT_CSPN
	dcd ADDR_ATStr_CSPN//AT命令字符串地址
	dcd ADDR_ERRStr_CSPN//错误提示字符串地址
	dcd 1000//AT命令返回第1个字符定时(ms)


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
	dcd 1000//AT命令返回第1个字符定时(ms)

//获取当前网络注册状态
	dcd AT_CREG
	dcd ADDR_ATStr_CREG//AT命令字符串地址
	dcd ADDR_ERRStr_CREG//错误提示字符串地址
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
			Terminal_Ram->GPRSWaitTx_S_Timer=30;//等待注册定时
			break;
		case AT_MYNETACT://激活网络连接
			Terminal_Ram->GPRSWaitTx_S_Timer=30;//等待激活网络连接定时
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


void GPRS_3763_AT_Tx(void)//AT命令发送
{
#if MainProtocol==CSG
	
#else
	u32 i;
	u32 x;
	u32 y;
	u32 AT_No;
	u32 TxByte;
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	
	u32 FTPFileCount;//FTP文件下载个数计数
	
	
	p8txbuff=(u8*)(ADDR_ATTx);
	TxByte=0;
	AT_No=UART1Ctrl->ATCount;//AT计数
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT计数
	}
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT://波特率检查
			TxByte=CopyString((u8 *)"AT\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
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
					TxByte=CopyString((u8 *)"AT+IPR=57600\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					break;
			}
			//启动UART1口发送
			Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
			UART1Ctrl->TxByte=TxByte;//发送字节数(半字对齐)
			UART1Ctrl->TxCount=0;
		#if LOG_1376_3==1//0=没,1=有
			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			Start_UARTn_Tx(GPRSPORT);//启动UART口发送
			UART1Ctrl->ATTask=2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			return;
		case AT_CNUM://取本机号码
			if((Comm_Ram->RemoteWirelessModuleType&0x21)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{//中兴CDMA(MC8332)
				TxByte=CopyString((u8 *)"AT+CNUM?\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			break;		
		case AT_MYNETCON_APN://设置APN
			if((Comm_Ram->RemoteWirelessModuleType&0x21)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		#if MainProtocol==376
				MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
				MC(0,ADDR_DATABUFF+16,1);
		#else
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
		#endif
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte]='"';
				TxByte++;
				p8txbuff[TxByte]='\r';
				TxByte++;
			}
			else
			{//中兴CDMA(MC8332)
				//AUTH：鉴权类型，0：NONE；1：PAP；2：CHAP，默认值1
				//AT$MYNETCON=0,"AUTH",1
				TxByte=CopyString((u8 *)"AT$MYNETCON=0,\"AUTH\",1",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte]='\r';
				TxByte++;
			}
			break;
		case AT_MYNETCON_USERPWD://设置USERPWD:用户名和密码
			//AT$MYNETCON=0,"USERPWD","card,card"
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
	#if MainProtocol==376
			if(MRR(ADDR_AFN04F16,1)!=0)
			{
				MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
				MC(0,ADDR_DATABUFF+32,1);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte]=',';
				TxByte++;
				MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{//用户名和密码均为空,分二种命令,有','号和无','号
				if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
				{
					p8txbuff[TxByte]=',';
					TxByte++;
				}
			}
	#else
			p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8)
			{//找到
				i=p8[1];//字节数
				p8+=2;
				p8[i]=0;
				TxByte+=CopyString(p8,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				p8txbuff[TxByte]=',';
				TxByte++;
				p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{//找到
					i=p8[1];//字节数
					p8+=2;
					p8[i]=0;
					TxByte+=CopyString(p8,p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
			}
			else
			{//没找到
				if(UART1Ctrl->LinkSubTask>=2)//尝试AT发送计数
				{
					p8txbuff[TxByte]=',';
					TxByte++;
				}
			}
	#endif
			p8txbuff[TxByte]='"';
			TxByte++;
			p8txbuff[TxByte]='\r';
			TxByte++;
			break;
		case AT_MYIPFILTER://IP访问控制配置
			if(UART1Ctrl->SubTask>=5)
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
	#if MainProtocol==376
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			x=MRR(ADDR_IPFILTER+(4*UART1Ctrl->SubTask),4);
			if(x==0)
			{
				UART1Ctrl->SubTask++;//子任计数
				return;
			}
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
			p8txbuff[TxByte]='"';
			TxByte++;
			p8txbuff[TxByte]=',';
			TxByte++;
			p8txbuff[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)"255.255.255.255",p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8txbuff[TxByte]='"';
			TxByte++;
			p8txbuff[TxByte]='\r';
			TxByte++;
			break;
	#else
			GPRS_3763_NextATCount();//下1个AT
			return;
	#endif
			
		case AT_CMFG://设置短消息模式
		case AT_ZCVF://打开短信功能(仅中兴CDMA(MC8332)时需要)
		case AT_CTA://休眠等待时间(仅CDMA时需要);无数据传输时经过10秒后CDMA通信模块主动释放空中接口资源
			GPRS_3763_NextATCount();//下1个AT
			return;
	
		case AT_MYNETSRV://设置服务参数
			switch(UART1Ctrl->SubTask)//子任务
			{
				case 0://确定工作模式
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它;
			#else
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
				//case 1://TCP客户机模式
				default://TCP客户机模式
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				TCPClient:
					p8txbuff[TxByte]='"';
					TxByte+=1;
					UART1Ctrl->LinkCount++;
					if(UART1Ctrl->LinkCount>=4)
					{
						UART1Ctrl->LinkCount=0;
					}
			#if MainProtocol==376
					if(UART1Ctrl->LinkCount<=2)
					{//主ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3,6);
					}
					else
					{//备用ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3+6,6);
					}
			#else
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
			#endif
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<4;i++)
					{
						y=hex_bcd(p8[i]);//HEX数转为8位压缩BCD数
						TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
						p8txbuff[TxByte]='.';
						TxByte++;
					}
					TxByte--;//IP第4个字节的'.'删去
					//目的端口号
					p8txbuff[TxByte]=':';
					TxByte+=1;
					y=hex_bcd(p8[4]+(p8[5]<<8));//HEX数转为8位压缩BCD数
					TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte]='"';
					TxByte++;
					p8txbuff[TxByte]='\r';
					TxByte++;
					break;
				case 2://TCP服务器模式
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,1,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8txbuff[TxByte]='"';
					TxByte++;
					TxByte+=CopyString((u8 *)"127.0.0.1",p8txbuff+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					p8txbuff[TxByte]=':';
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
					i=MRR(ADDR_4500_2+12,2);
					i=(i>>8)+((i&0xff)<<8);
					i=hex_bcd(i);//HEX数转为8位压缩BCD数
			#endif
					TxByte+=BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte]='"';
					TxByte++;
					p8txbuff[TxByte]='\r';
					TxByte++;
					break;
				case 3://UDP客户机模式
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,2,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					goto TCPClient;
				case 4://UDP服务器模式
					//无此模式
					//WarningOccur(GPRSPORT,(u8 *)"远程无线上行模式错");//告警发生,入口告警代码和告警字符串
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
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
//						WarningOccur(GPRSPORT,(u8 *)"远程无线重拨间隔定时");//告警发生,入口告警代码和告警字符串
						GPRS_3763_NextATCount();//下1个AT
						break;
					}
					//每次登录失败后，经过心跳周期0.5-1.5倍的随机延时（以秒或毫秒计）后重新登录
					i=MRR(ADDR_AFN04F8+1,2);//永久在线、时段在线模式重拨间隔 BIN 秒 2
					Terminal_Ram->GPRSReConnection_S_Timer=i;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
				}
		#else
				
				
				
		#endif
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				Terminal_Ram->GPRSWaitConnection_S_Timer=90;//20 GPRS发起连接服务等待秒定时器
				UART1Ctrl->NETOPENCount++;//开启服务次数计数(当前次数达到x次时关模块电源)
			}
			break;

		case AT_LOOP_NOLINK://AT循环,没连接
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
			if(UART1Ctrl->NETOPENCount>=3)//开启服务连续次数计数(当前次数达到x次时关模块电源)
			{
				UART1Ctrl->NETOPENCount=0;//开启服务连续次数计数(当前次数达到x次时关模块电源)
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
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
			break;
		case AT_SYSINFO:
		case AT_SYSINFO_LINK://
		#if (USER/100)==14//福建系
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->TYMDHMS[0])//
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//
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
		case AT_MYNETOPEN_LINK://连接检查(每秒1次)
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
				return;
			}
			break;
		case AT_MYNETREAD://读取数据
			p16=(u16*)(ADDR_UART1IntRx);
			if(p16[0]!=p16[1])
			{
				UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
//			#if MainProtocol==698
//				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
//			#else
//				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
//			#endif
				return;
			}
			else
			{
				TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				//i=LEN_UART1IntATRx-1024;
				i=1460;
				i=hex_bcd(i);//HEX数转为8位压缩BCD数
				TxByte+=BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
				p8txbuff[TxByte]='\r';
				TxByte++;
			}
			break;
		case AT_MYNETWRITE://发送数据
			switch(UART1Ctrl->ATWriteTask)//AT数据发送任务:0=发命令,1=发数据
			{
				case 0://发AT头
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					i=Terminal_Ram->CopyTxByte;//还需发送字节数
					if(i==0)
					{//还需发送字节数错误
						if(UART1Ctrl->Task==6)//6=多帧数据正在发送
						{
							UART1Ctrl->Task=4;//4=多帧数据续接收完成
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
					TxByte+=BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte]='\r';
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
				
				p8rxbuff=(u8 *)ADDR_DATABUFF;
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MR(ADDR_DATABUFF,ADDR_AFN0FF1_Rx,256);//恢复FTP接收帧的前256字节
				MR((u32)p8txbuff,ADDR_AFN0FF1_Tx,18);//恢复FTP返回帧的前18字节
				MWR(y,((u32)p8txbuff)+18,4);//返回标识4字节
				i=4+4;//返回长度
				i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				i=p8rxbuff[1]+(p8rxbuff[2]<<8);
//				Uart_376_AUX(GPRSPORT,((u32)p8rxbuff)+6+i-6);//附加信息(EC,TP),暂等待单帧数据发送
//				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				
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
				p8rxbuff=(u8*)(ADDR_DATABUFF);
				p8rxbuff[0]='Z';
				p8rxbuff[1]='Z';
				p8rxbuff[2]=((SVER>>4)&0xf)+0x30;
				p8rxbuff[3]=((SVER>>0)&0xf)+0x30;
				//发生时间：分时日月年	见附录A.15	5
				MR(ADDR_DATABUFF+10,ADDR_TYMDHMS+1,5);
				MW(ADDR_DATABUFF,ADDR_AFN0FF98+10,15);
			#endif
				return;
			}
//	#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
//			TxByte=GPRS_FTP_Tx();//GPRS_FTP_Tx发送
//	#else
//			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
//			{
//    		p32=(u32 *)(ADDR_E1_ENTRY);
//    		TxByte=(*(u32(*)())p32[106])();//GPRS_FTP_Tx发送
//			}
//	#endif
			break;
			
		case AT_SMS_NOLINK://短信服务没连接前
		case AT_SMS_LINK://短信服务
//		#if LEN_UART11Ctrl==0
			GPRS_3763_NextATCount();//下1个AT
//		#else
//			TxByte=GPRS_SMS_Tx();//GPRS_SMS_Tx发送;返回发送字节数
//		#endif
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
			
		default:
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
	}
	if(TxByte!=0)
	{
		//启动UART1口发送
		UART1Ctrl->BpsCtrl=0x3+(9<<5);//速率57.6K 
		Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
		UART1Ctrl->TxByte=TxByte;//发送字节数(半字对齐)
    UART1Ctrl->TxCount=0;
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
		UART1Ctrl->ATTask=2;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	}
#endif
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
#if MainProtocol==CSG
	
	
#else
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
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT代号
	{
		case AT_AT://波特率检查
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
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							return;
					}
				}
			}
			else
			{//接收到OK
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				if(UART1Ctrl->SubTask<10)//子任务
				{
					UART1Ctrl->SubTask+=10;//子任务
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				}
				else
				{
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
//				//厂商代号，ASCII，4字节
//				MC(0,ADDR_DATABUFF,4);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				MW(ADDR_DATABUFF,ADDR_AFN09F9,4);
//				//模块型号，ASCII，8字节
//				MC(0,ADDR_DATABUFF,8);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				MW(ADDR_DATABUFF,ADDR_AFN09F9+4,8);
//				//软件版本号，ASCII，4字节
//				MC(0,ADDR_DATABUFF,4);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8,4);
//				//软件发布日期，日月年，3字节
//				MC(0,ADDR_DATABUFF,6);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				i=ASCII_BCD(ADDR_DATABUFF,6);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
//				MWR(i,ADDR_AFN09F9+4+8+4,3);
//				Data_Inverse(ADDR_AFN09F9+4+8+4,3);//数据倒序(高低字节调换),入口Le>=2
//				//硬件版本号，ASCII，4字节
//				MC(0,ADDR_DATABUFF,4);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3,4);
//				//硬件发布日期，日月年，3字节
//				MC(0,ADDR_DATABUFF,6);
//				GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
//				i=ASCII_BCD(ADDR_DATABUFF,6);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
//				MWR(i,ADDR_AFN09F9+4+8+4+3+4,3);
//				Data_Inverse(ADDR_AFN09F9+4+8+4+3+4,3);//数据倒序(高低字节调换),入口Le>=2		
				
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_CPIN://卡检查
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"$MYCCID"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
//				LineATIntRx(Addr+3,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
//			#if (USER/100)!=12//非四川
//				//四川要求AFN09F9的ICCID项填写SIM卡手机号码。
//				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3+4+3,20);
//			#endif
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			p8d=(u8*)(ADDR_MobilePhoneNumber);
			p8d[0]=0;
			Addr=ATRxSearch((u32)((u8*)"+CNUM:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				if((Comm_Ram->RemoteWirelessModuleType&0x21)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
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
				}
				else
				{//中兴CDMA(MC8332)
					LineATIntRx(Addr,ADDR_DATABUFF,20);//取AT中断接收数据线性放在缓冲中
					p8s=(u8*)(ADDR_DATABUFF);
					for(i=0;i<20;i++)
					{
						x=p8s[i];
						if((x<0x30)||(x>0x39))
						{
							break;
						}
						p8d[i]=x;
					}
					p8d[i]=0;
				}
			#if (USER/100)==12//四川
				//四川要求AFN09F9的ICCID项填写SIM卡手机号码。
				MW(ADDR_MobilePhoneNumber,ADDR_AFN09F9+4+8+4+3+4+3,20);//写SIM卡手机号码
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(Addr!=0)
			{
				if((Comm_Ram->RemoteWirelessModuleType&0x21)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
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
				}
				else
				{//中兴CDMA(MC8332)
					Addr=ATRxSearch((u32)((u8*)"+CIMI: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr==0)
					{
						UART1Ctrl->LinkSubTask++;//尝试AT发送计数
						if(UART1Ctrl->LinkSubTask>2)//尝试AT发送计数
						{
							GPRS_3763_NextATCount();//下1个AT
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
		case AT_CSPN://读取SIM卡服务运营商名称
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
//				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
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
			
		case AT_CSQ_NOLINK://读信号没连接前
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
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
//						WarningOccur(GPRSPORT,(u8 *)"远程无线无信号");//告警发生,入口告警代码和告警字符串
						UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					}
				}
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+CREG:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
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
//								WarningOccur(GPRSPORT,(u8 *)"远程无线注册失败");//告警发生,入口告警代码和告警字符串
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '1'://注册了本地网络
							GPRS_3763_NextATCount();//下1个AT
							break;
						case '2'://没有注册，但MS正在搜寻要注册的新的运营商
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待注册定时
							{
//								WarningOccur(GPRSPORT,(u8 *)"远程无线注册搜寻超时");//告警发生,入口告警代码和告警字符串
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							}
							return;
						case '3'://注册被拒绝
//							WarningOccur(GPRSPORT,(u8 *)"远程无线注册被拒绝");//告警发生,入口告警代码和告警字符串
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							return;
						case '5'://注册了漫游网络
//							WarningOccur(GPRSPORT,(u8 *)"远程无线注册漫游网络");//告警发生,入口告警代码和告警字符串
							GPRS_3763_NextATCount();//下1个AT
							break;
					}
					break;
				}
			}
			goto ReTxAT;//重发本AT
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://查询注册网络类型：3G/4G
		#if (USER/100)==14//福建系
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Comm_Ram->RemoteWirelessModuleType=1;//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			Addr=ATRxSearch((u32)((u8*)"$MYSYSINFO: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//				Addr=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
//				if(Addr!=0)
//				{
					p8d = (u8 *)Addr;
//					x = 0;
//					for(i = 0;i < 14;i ++)
//					{
//						if(p8d[i] == ',')
//						{
//							x ++;
//							if(x == 2)
//							{
//								i++;
//								break;
//							}
//						}
//					}
//					if(x == 2)
//					{
//						y = p8d[i++]-0x30;
//						if(p8d[i]!=',')
//							i =y*10+(p8d[i]-0x30);
//						else
//							i = y;
//					}
//					else
//						i = 3;
					i = p8d[0]-0x30;
					switch(i)
					{
//						case 0://无服务
						default:
						{
							break;
						}
						case 2://GSM/GPRS模式 2G
						{
							break;
						}
						case 3://WCDMA模式 3G
						{
							break;
						}
						case 4://17://LTE模式 4G
						{
							Comm_Ram->RemoteWirelessModuleType=(1<<5);//b5=1
							break;
						}
					}
				}
//			}
//			else
//			{
//			}
		#endif
			GPRS_3763_NextATCount();//下1个AT
			break;
		case AT_MYNETACT://激活网络连接
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				GPRS_3763_NextATCount();//下1个AT
			}
			else
			{
				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待激活网络连接定时
				{
//					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					//UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
					GPRS_3763_NextATCount();//下1个AT;不关机可能错误能收短信修复
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask++;//子任务
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_MYNETOPEN://开启服务
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
//				WarningCancel(GPRSPORT);//告警取消,入口告警代码
				UART1Ctrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
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
			}
			else
			{
//				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
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
			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT代号
			{
				LOG_ATRx(0);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
			}
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"+CSQ: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
//				GPRS_Signal(ADDR_DATABUFF);//GPRS信号强度0-4
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
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
		#if LOG_1376_3==1//0=没,1=有
			//LOG_ATRx();//GPRS模块AT接收数据LOG;使用ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
		#if MainProtocol==376
				MC(0,ADDR_AFN0CF4+1,16+6);
				//LocalIP
				p16fifo[1]+=2;
				Addr+=2;
				x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
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
//						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
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
//							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							return;
						}
					}
				}
		#else
				MC(0,ADDR_MobilePhoneNumber+40,16+6);
				//LocalIP
				p16fifo[1]+=2;
				Addr+=2;
				x=ATRxSearch((u32)((u8*)"\""));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
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
//						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
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
//							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
							return;
						}
					}
				}
		#endif
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				GPRS_3763_NextATCount();//下1个AT
				break;
			}
			goto ReTxAT;//重发本AT
		case AT_MYNETREAD://读取数据
			p1=p16fifo[1];//原p1
			Addr=ATRxSearch((u32)((u8*)"$MYNETREAD: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
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
					
					i=MRR((u32)&UART1Ctrl->RxTxByte,4);
					i+=RxByte;
					MWR(i,(u32)&UART1Ctrl->RxTxByte,4);//通信流量
					
					x=p16fifo[1];
					while(x>=(LEN_UART1IntATRx-10))
					{
						x-=(LEN_UART1IntATRx-10);
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
					p8s=(u8*)(ADDR_UART1IntATRx+10);
					p16=(u16*)(ADDR_UART1IntRx);
					y=p16[0];
					if(y>=(LEN_UART1IntRx-10))
					{
						y=0;
					}
					p8d=(u8*)(ADDR_UART1IntRx+10);	
					for(i=0;i<RxByte;i++)
					{
						p8d[y]=p8s[x];
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
						LOG_LINK(13763,0,GPRSPORT,(u8 *)ADDR_UART1IntRx+10,RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
						p16fifo[2]=x;//修改已读指针
						//acii
						LOG_ATRx(1);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
					}
				#endif
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
//			#if MainProtocol==698
//				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
//			#else
//				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
//			#endif
					if(RxByte==0)
					{
						GPRS_3763_NextATCount();//下1个AT
					}
					return;
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
					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
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
						
						//i=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
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
//							WarningOccur(GPRSPORT,(u8 *)"远程无线等待发送超时");//告警发生,入口告警代码和告警字符串
						}
						else
						{
//							WarningOccur(GPRSPORT,(u8 *)"远程无线登录主站失败");//告警发生,入口告警代码和告警字符串
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
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					}
					if(Terminal_Ram->CopyTxByte!=0x0)//还需发送字节数
					{
						UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
					}
					else
					{
						if(UART1Ctrl->Task==6)
						{//多帧数据正在发送
							UART1Ctrl->Task=4;//4=多帧数据续接收完成
						}
						else
						{
							UART1Ctrl->Task=0;//0=接收
						}
						GPRS_3763_NextATCount();//下1个AT
					}
					break;
			}
			break;
		case AT_MYFTP://FTP文件下载
//	#if Extern_GPRS_FTP_RxTx==0//外部程序:0=I2,1=E1,2=E2,3=;W5200 GPRS文件下载
//			GPRS_FTP_Rx();//GPRS_FTP_Rx接收
//	#else
//			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
//			{
//    		p32=(u32 *)(ADDR_E1_ENTRY);
//    		(*(void(*)())p32[107])();//GPRS_FTP_Rx接收
//			}
//	#endif	
			break;
		case AT_SMS_NOLINK://短信服务没连接前
		case AT_SMS_LINK://短信服务
		#if LEN_UART11Ctrl!=0
//			GPRS_SMS_Rx();//GPRS_SMS_Rx接收
		#endif
			break;
			
			
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				Comm_Ram->UART1Timer=15000/10;//关机延时15s
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
				if(UART1Ctrl->LinkSubTask>2)//尝试AT发送计数
				{
//					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//告警发生,入口告警代码和告警字符串
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
				}
				return;
			}
			break;
	}
#endif
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
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			break;
	}
}


void UnInit_GPRS_3763(void)//GPRS模块引脚为输入下拉
{
	u32 i;
	i=
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_ON_GPRS,i);//on/off
	Pin_Configure(PIN_RST_GPRS,i);//reset
	Pin_Configure(PIN_USART6_TX,i);//TXD
	Pin_Configure(PIN_USART6_RX,i);//RXD
	//RI
}

void GPRS_3763_RxTx(void)//远程无线GPRS模块接收发送
{
//	u32 i;
//	u32 x;
	
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
	switch(UART1Ctrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		case 0://0=关机延时
			Terminal_Ram->Signal=0;//GPRS信号强度0-4
			Terminal_Ram->SignaldBm=0x99;//信号强度0-31或99
			UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;//共用UART1Timer直接返回
		case 1://1=关总电源
			UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;//共用UART1Timer直接返回
		case 2://开总电源
			OnR5V0();//打开R5V0电源
			UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		return;//共用UART1Timer直接返回
		case 3://开模块ON/OFF输出=0
			if(Comm_Ram->UART1Timer==0x0)
			{
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
				
				Pin_Configure(PIN_ON_GPRS,//ON输出=0
								0|//B3-B0:复用功能选择AF(0-15)
					 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
					 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
					 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
					 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
					(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				Terminal_Ram->GPRS_ONOFF_MS_Timer=101;//15 GPRS ONOFF脚控制,=1输出高,0=不变
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
			if(Comm_Ram->UART1Timer==0x0)
			{
				UART1Ctrl->BpsCtrl=0x3+(9<<5);//初始速率57.6
				Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
				UART1Ctrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;//共用UART1Timer直接返回
		case 8://初始化AT命令
			break;
		case 100://连接ok
		case 101://登录ok
		case 102://发退出登录
			break;
		case 103://退出登录
		case 104://关机
			if(UART1Ctrl->ATTask==0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
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
//AT接收发送
	if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	{
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
		case 5://等待多帧数据发送
		#if (USER/100)==11//吉林系
			USER11xx_TxAddr(GPRSPORT);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
		#endif
		#if LOG_1376_1==1//0=没,1=有
			LOG(13761,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
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
		case 3://单帧数据正在发送
		case 6://多帧数据正在发送
			if(UART1Ctrl->ATTask==0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			{//不明原因冲突或意外RAM错误等等
//				WarningOccur(GPRSPORT,(u8 *)"GPRS任务有BUG");//告警发生,入口告警代码和告警字符串
				UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				//重拨间隔定时器置为0以便立即重连
				Terminal_Ram->GPRSReConnection_S_Timer=0;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
			}
			break;
		case 4://多帧数据续接收完成
//			#if MainProtocol==698
//				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
//			#else
//				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
//			#endif
			break;
		default:
//			#if MainProtocol==698
//				Uart_698_RxTx(GPRSPORT);//UART_3761接收发送
//			#else
//				Uart_3761_RxTx(GPRSPORT);//UART_3761接收发送
//			#endif
			break;
	}

}
/*
u32 Test_GPRS(u32 ADDR_BASE)//GPRS模块
{
	u32 i;
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""GPRS");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	__enable_irq();//允许中断
	Comm_Ram->GPMSTimer=60000/10;
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		GPRS_3763_RxTx();//远程无线GPRS模块接收发送
		
		i=UART1Ctrl->ATCount;
		if((i>=AT_MYNETOPEN)&&(i<AT_HOLD))//AT代号
		{
			MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+4),1);
			DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
			return 0;//总测试结果0=合格,1=不合格,0xff=无结论
		}
		if((Comm_Ram->GPMSTimer==0)||(i>=AT_HOLD))
		{
			MWR(1,ADDR_BASE+(OFFSET_UART_Characteristics+4),1);
			DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
			return 1;//总测试结果0=合格,1=不合格,0xff=无结论
		}
	}
}
*/

#include "Test_Uart_3762_RxTx.h"
#include "Test_ReadMeter_3762.h"
void Terminal_Contrl_Uart_RxTx(u32 PORTn);//控制模块通信RxTx
u32 Test_GPRS_MODULE(u32 ADDR_BASE)//GPRS模块,载波模块或控制模块
{
#if MainProtocol==CSG
	return 0;
	
#else
	u32 i;
	u32 Result_GPRS;//GPRS结果:0=没完成,1=合格,2=不合格
	u32 Result_MODULE;//MODULE结果:0=没完成,1=合格,2=不合格
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_TypeDef * UARTCtrl;
	u32 TestTask;
	
	TestTask=0;
#endif
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""GPRS");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	DisplayStringAutoUp(0,9,(u8*)"\x0""LOCAL MODULE");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	#endif
	#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	DisplayStringAutoUp(0,9,(u8*)"\x0""CONTRL MODULE");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	#endif
	__enable_irq();//允许中断
	Comm_Ram->GPMSTimer=60000/10;
	Comm_Ram->UART12Timer=60000/10;

	Result_GPRS=0;//GPRS结果:0=没完成,1=合格,2=不合格
	Result_MODULE=0;//MODULE结果:0=没完成,1=合格,2=不合格
	UART1Ctrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	Comm_Ram->UART1Timer=0;
//	Init_UARTn(GPRSPORT);//初始化,OS重启有可能影响DMA!
//	Init_UARTn(RS485_4PORT);//初始化,OS重启有可能影响DMA!
	
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	//GPRS
		if(Result_GPRS==0)
		{
			GPRS_3763_RxTx();//远程无线GPRS模块接收发送
			i=UART1Ctrl->ATCount;
			if((i>=AT_MYNETOPEN)&&(i<AT_HOLD))//AT代号
			{
				MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+4),1);
				DisplayString(AutoDisplayRowCol->Row-2,18,0,(u8*)"\x0""OK");//显示字符串
				LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
				Result_GPRS=1;//GPRS结果:0=没完成,1=合格,2=不合格
			}
			if((Comm_Ram->UART12Timer==0)||(i>=AT_HOLD))
			{
				MWR(1,ADDR_BASE+(OFFSET_UART_Characteristics+4),1);
				DisplayString(AutoDisplayRowCol->Row-2,16,0,(u8*)"\x0""FAIL");//显示字符串
				LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
				Result_GPRS=2;//GPRS结果:0=没完成,1=合格,2=不合格
			}
		}
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	//LOCAL MODULE
		if(Result_MODULE==0)
		{
			Terminal_ReadMeter_3762(RS485_4PORT);//终端抄表Q/GDW376.2
			Uart_3762_RxTx(RS485_4PORT);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(Terminal_Router->RouterInitTask>=10)
			{
				MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
				DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
				LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
				Result_MODULE=1;//MODULE结果:0=没完成,1=合格,2=不合格
			}
			if(Comm_Ram->GPMSTimer==0)
			{
				MWR(0x010101,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
				DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
				LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
				Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
			}
		}
	#endif
	#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	//CONTRL MODULE	
		if(Result_MODULE==0)
		{
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			Terminal_Contrl_Uart_RxTx(RS485_4PORT);//控制模块通信RxTx
			switch(TestTask)
			{
				case 0://跳闸输出1
					Terminal_Ram->Trip1_MS_Timer=10000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 1:
					if(UARTCtrl->LinkTask<3)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x010101,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{	
						Terminal_Ram->Trip1_MS_Timer=1000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
						TestTask++;
					}
					break;
				case 2:
					if(Terminal_Ram->Trip1_MS_Timer!=0)//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					{
						break;//等待原通信超时结束
					}
					MWR(0xff8000,ADDR_ContrlUARTData,3);//无跳闸输出，断线标志
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 3:
					//常闭
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&0x03)!=0)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x000001,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 4:
					//常开动作
					MWR(0x008003,ADDR_ContrlUARTData,3);
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 5:
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&0x03)!=0x03)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x000001,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 6://跳闸输出2
					MWR(0xff8000,ADDR_ContrlUARTData,3);//无跳闸输出，断线标志
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 7:
					//常闭
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&0x0c)!=0)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x000100,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 8:
					//常开动作
					MWR(0x008018,ADDR_ContrlUARTData,3);
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 9:
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&0x0c)!=0x0c)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x000100,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 10://报警输出
					//先确定跳闸1常闭
					MWR(0xff8000,ADDR_ContrlUARTData,3);//
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 11:
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&0x03)!=0)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x010000,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 12:
					//常开动作
					MWR(0x008100,ADDR_ContrlUARTData,3);//报警跳闸输出
					Terminal_Ram->Trip1_MS_Timer=3000/10;//终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
					TestTask++;
					break;
				case 13:
					i=MRR(ADDR_ContrlUARTData+2,1);
					if((i&1)!=1)//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
					{
						if(Terminal_Ram->Trip1_MS_Timer==0)
						{
							MWR(0x010000,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
							DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
							LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
							Result_MODULE=2;//MODULE结果:0=没完成,1=合格,2=不合格
						}
					}
					else
					{
						TestTask++;
					}
					break;
				case 14:
					MWR(0x000000,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
					DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
					LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
					Result_MODULE=1;//MODULE结果:0=没完成,1=合格,2=不合格
					break;
			}
		}
	#endif
		
		if((Result_GPRS!=0)&&(Result_MODULE!=0))
		{
			Result_GPRS--;
			Result_MODULE--;
			return Result_GPRS|Result_MODULE;//总测试结果0=合格,1=不合格,0xff=无结论
		}
	}
#endif
}


