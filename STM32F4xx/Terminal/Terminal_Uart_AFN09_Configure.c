

//请求终端配置
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"




extern  u8 ADDR_AFN09F1[];//终端版本信息

const u8 AFN09F2_Addr[]=//终端支持的输入、输出及通信端口配置
{
	MaxImpInPort,//脉冲量输入路数	0～64	BIN	1
	MaxSwitchInPort,//开关量输入路数	0～64	BIN	1
	MaxDCPn,//直流模拟量输入路数	0～64	BIN	1
	MaxControlRound,//开关量输出路数（轮次）	0～64	BIN	1
	MaxRS485AddCarrierPn&0xff,MaxRS485AddCarrierPn>>8,//支持的抄电能表/交流采样装置最多个数	0～2040	BIN	2
	(LEN_UART1Rx&0xff),(LEN_UART1Rx>>8),//支持的终端上行通信最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART1Tx&0xff),(LEN_UART1Tx>>8),//支持的终端上行通信最大发送缓存区字节数	256～16383	BIN	2
	0,//终端MAC地址1段	--	BIN	1
	0,//终端MAC地址2段	--	BIN	1
	0,//终端MAC地址3段	--	BIN	1
	0,//终端MAC地址4段	--	BIN	1
	0,//终端MAC地址5段	--	BIN	1
	0,//终端MAC地址6段	--	BIN	1

	//通信端口数量n	0～31	BIN	1
#if (Project/100)==0//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  0,
#endif
#if (Project/100)==1//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  3,
	//1.终端交流采样接口通信
	1+(7<<5),(7<<5),//第1个通信端口的端口号及信息字	--	BS16	2
	(230400&0xff),((230400>>8)&0xff),((230400>>16)&0xff),0,//第1个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第1个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART12Rx&0xff),(LEN_UART12Rx>>8),//第1个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART12Tx&0xff),(LEN_UART12Tx>>8),//第1个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-1通信端口
	2+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//3.RS485-2通信端口
	3+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  6,
	//1.终端交流采样接口通信
	1+(7<<5),(0<<5),//第1个通信端口的端口号及信息字	--	BS16	2
	(230400&0xff),((230400>>8)&0xff),((230400>>16)&0xff),0,//第1个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第1个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART12Rx&0xff),(LEN_UART12Rx>>8),//第1个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART12Tx&0xff),(LEN_UART12Tx>>8),//第1个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-1通信端口
	2+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	64,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//3.RS485-2通信端口
	3+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	64,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2

	//4.RS232
	4+(1<<5),(3<<5),//第4个通信端口的端口号及信息字	--	BS16	2
	(115200&0xff),((115200>>8)&0xff),((115200>>16)&0xff),0,//第6个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第6个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART0Rx&0xff),(LEN_UART0Rx>>8),//第6个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART0Tx&0xff),(LEN_UART0Tx>>8),//第6个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//5.红外
	5+(3<<5),(3<<5),//第7个通信端口的端口号及信息字	--	BS16	2
	(1200&0xff),((1200>>8)&0xff),((1200>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第7个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART4Rx&0xff),(LEN_UART4Rx>>8),//第7个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART4Tx&0xff),(LEN_UART4Tx>>8),//第7个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//31.载波通信端口
	31+(2<<5),(2<<5),//第5个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第5个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第5个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART6Rx&0xff),(LEN_UART6Rx>>8),//第5个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART6Tx&0xff),(LEN_UART6Tx>>8),//第5个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  3+2,
	//1.终端交流采样接口通信
	1+(7<<5),(0<<5),//第1个通信端口的端口号及信息字	--	BS16	2
	(230400&0xff),((230400>>8)&0xff),((230400>>16)&0xff),0,//第1个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第1个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART12Rx&0xff),(LEN_UART12Rx>>8),//第1个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART12Tx&0xff),(LEN_UART12Tx>>8),//第1个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-1通信端口
	2+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//3.RS485-2通信端口
	3+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2

	//4.RS232
	4+(1<<5),(3<<5),//第4个通信端口的端口号及信息字	--	BS16	2
	(115200&0xff),((115200>>8)&0xff),((115200>>16)&0xff),0,//第6个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第6个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART0Rx&0xff),(LEN_UART0Rx>>8),//第6个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART0Tx&0xff),(LEN_UART0Tx>>8),//第6个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//5.红外
	5+(3<<5),(3<<5),//第7个通信端口的端口号及信息字	--	BS16	2
	#if (USER/100)==5//上海
	(2400&0xff),((2400>>8)&0xff),((2400>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	#else
	(1200&0xff),((1200>>8)&0xff),((1200>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	#endif
	1,0,//第7个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART4Rx&0xff),(LEN_UART4Rx>>8),//第7个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART4Tx&0xff),(LEN_UART4Tx>>8),//第7个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  3+2,
	//1.终端交流采样接口通信
	1+(7<<5),(7<<5),//第1个通信端口的端口号及信息字	--	BS16	2
	(230400&0xff),((230400>>8)&0xff),((230400>>16)&0xff),0,//第1个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第1个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART12Rx&0xff),(LEN_UART12Rx>>8),//第1个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART12Tx&0xff),(LEN_UART12Tx>>8),//第1个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-1通信端口
	2+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//3.RS485-2通信端口
	3+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2

	//4.RS232
	4+(1<<5),(3<<5),//第4个通信端口的端口号及信息字	--	BS16	2
	(115200&0xff),((115200>>8)&0xff),((115200>>16)&0xff),0,//第6个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第6个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART0Rx&0xff),(LEN_UART0Rx>>8),//第6个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART0Tx&0xff),(LEN_UART0Tx>>8),//第6个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//5.红外
	5+(3<<5),(3<<5),//第7个通信端口的端口号及信息字	--	BS16	2
	(1200&0xff),((1200>>8)&0xff),((1200>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第7个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART4Rx&0xff),(LEN_UART4Rx>>8),//第7个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART4Tx&0xff),(LEN_UART4Tx>>8),//第7个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  2+2,
	//1.RS485-1通信端口
	1+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-2通信端口
	2+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2

	//3.RS232
	3+(1<<5),(3<<5),//第4个通信端口的端口号及信息字	--	BS16	2
	(115200&0xff),((115200>>8)&0xff),((115200>>16)&0xff),0,//第6个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第6个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART0Rx&0xff),(LEN_UART0Rx>>8),//第6个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART0Tx&0xff),(LEN_UART0Tx>>8),//第6个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//4.红外
	4+(3<<5),(3<<5),//第7个通信端口的端口号及信息字	--	BS16	2
	(1200&0xff),((1200>>8)&0xff),((1200>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第7个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART4Rx&0xff),(LEN_UART4Rx>>8),//第7个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART4Tx&0xff),(LEN_UART4Tx>>8),//第7个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif
#if (Project/100)==6//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  3+2,
	//1.终端交流采样接口通信
	1+(7<<5),(7<<5),//第1个通信端口的端口号及信息字	--	BS16	2
	(230400&0xff),((230400>>8)&0xff),((230400>>16)&0xff),0,//第1个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第1个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART12Rx&0xff),(LEN_UART12Rx>>8),//第1个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART12Tx&0xff),(LEN_UART12Tx>>8),//第1个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//2.RS485-1通信端口
	2+(0<<5),(0<<5),//第2个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第2个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第2个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART2Rx&0xff),(LEN_UART2Rx>>8),//第2个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART2Tx&0xff),(LEN_UART2Tx>>8),//第2个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//3.RS485-2通信端口
	3+(0<<5),(0<<5),//第3个通信端口的端口号及信息字	--	BS16	2
	(19200&0xff),((19200>>8)&0xff),((19200>>16)&0xff),0,//第3个通信端口支持的最高波特率（bps）		BIN	4
	32,0,//第3个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART3Rx&0xff),(LEN_UART3Rx>>8),//第3个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART3Tx&0xff),(LEN_UART3Tx>>8),//第3个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2

	//4.RS232
	4+(1<<5),(3<<5),//第4个通信端口的端口号及信息字	--	BS16	2
	(115200&0xff),((115200>>8)&0xff),((115200>>16)&0xff),0,//第6个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第6个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART0Rx&0xff),(LEN_UART0Rx>>8),//第6个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART0Tx&0xff),(LEN_UART0Tx>>8),//第6个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
	//5.红外
	5+(3<<5),(3<<5),//第7个通信端口的端口号及信息字	--	BS16	2
	(1200&0xff),((1200>>8)&0xff),((1200>>16)&0xff),0,//第7个通信端口支持的最高波特率（bps）		BIN	4
	1,0,//第7个通信端口支持的设备个数	0～2040	BIN	2
	(LEN_UART4Rx&0xff),(LEN_UART4Rx>>8),//第7个通信端口支持的最大接收缓存区字节数	256～16383	BIN	2
	(LEN_UART4Tx&0xff),(LEN_UART4Tx>>8),//第7个通信端口支持的最大发送缓存区字节数	256～16383	BIN	2
#endif

};

const u8 AFN09F3_Addr[]=//终端支持的其他配置
{
	MaxRS485AddCarrierPn&0xff,(MaxRS485AddCarrierPn>>8)&0xff,//支持的测量点最多点数	0～2040	BIN	2
	MaxTotalAddGroup,//支持的总加组最多组数	0～8	BIN	1
	MaxTimerTask,//支持的任务最多个数	0～64	BIN	1
	MaxDiffGroup,//支持的有功总电能量差动组最多组数	0～8	BIN	1
	TMaxTariff,//支持的最大费率数	0～14	BIN	1
#if (((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	1,//支持的测量点数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功电能量数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功电能量数据最大冻结密度	见附录C	BIN	1
#else
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	1,//支持的测量点数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功电能量数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功电能量数据最大冻结密度	见附录C	BIN	1
	#else
	1,//支持的测量点数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功功率数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组有功电能量数据最大冻结密度	见附录C	BIN	1
	1,//支持的总加组无功电能量数据最大冻结密度	见附录C	BIN	1
	#endif
#endif
	MaxDateCongeal,//支持的日数据最多存放天数	0～31	BIN	1
	MaxMonthCongeal,//支持的月数据最多存放月数	0～12	BIN	1
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	3,//支持的时段功控定值方案最多个数	0～3	BIN	1
#else
	0,//支持的时段功控定值方案最多个数	0～3	BIN	1
#endif
#if MeterHarmonic==0//谐波计算0=没,1=有
	0,//支持的谐波检测最高谐波次数	0～19	BIN	1
#else
	19,//支持的谐波检测最高谐波次数	0～19	BIN	1
#endif
	MaxCCR,//支持的无功补偿电容器组最多组数	0～16	BIN	1
#if MaxKeyUser<=20
	MaxKeyUser,//支持的台区集中抄表重点户最多户数	0～20	BIN	1
#else
	20,//支持的台区集中抄表重点户最多户数	0～20	BIN	1
#endif
	
	0xff,0xff,//支持的用户大类号标志		BS16	2
	16,//支持0号用户大类下的用户小类号个数	0～16	BIN	1
	16,//支持1号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持2号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持3号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持4号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持5号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持6号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持7号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持8号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持9号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持10号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持11号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持12号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持13号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持14号用户大类下的用户小类号个数	0～16	BIN 	1
	16,//支持15号用户大类下的用户小类号个数	0～16	BIN 	1

};


const u8 AFN09F8_Addr[]=//终端支持的事件记录配置
{
//支持的事件记录标志位	BS64	8
	ERC1+(ERC2<<1)+(ERC3<<2)+(ERC4<<3)+(ERC5<<4)+(ERC6<<5)+(ERC7<<6)+(ERC8<<7),//ERC1-ERC8
	ERC9+(ERC10<<1)+(ERC11<<2)+(ERC12<<3)+(ERC13<<4)+(ERC14<<5)+(ERC15<<6)+(ERC16<<7),//ERC9-ERC16
	ERC17+(ERC18<<1)+(ERC19<<2)+(ERC20<<3)+(ERC21<<4)+(ERC22<<5)+(ERC23<<6)+(ERC24<<7),//ERC17-ERC24
	ERC25+(ERC26<<1)+(ERC27<<2)+(ERC28<<3)+(ERC29<<4)+(ERC30<<5)+(ERC31<<6)+(ERC32<<7),//ERC25-ERC32
	ERC33+(ERC34<<1)+(ERC35<<2)+(ERC36<<3)+(ERC37<<4)+(ERC38<<5)+(ERC39<<6)+(ERC40<<7),//ERC33-ERC40
	ERC41,//ERC41-ERC48
	0,
	0,
};


#if (USER/100)==13//黑龙江版
//485表数量
u32 Statistics_485(void)
{
	u32 x;
	u32 i;
	u32 Pn;
#if IC_SST39VF6401B==0//0=没,1=有
	u32 Addr;
#else
	u8 *p8d;
#endif

	x=0;
#if IC_SST39VF6401B==0//0=没,1=有
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//通信速率及端口号
		Pn&=0x1f;
		if((Pn==1)||(Pn==2)||(Pn==3))
		{//通信端口号
			Pn=MRR(Addr,2);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485Pn))
			{//所属测量点号!=0x0
				x++;
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8d=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8d[2];//通信速率及端口号
		Pn&=0x1f;
		if((Pn==1)||(Pn==2)||(Pn==3))
		{//通信端口号
			Pn=p8d[0]+(p8d[1]<<8);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485Pn))
			{//所属测量点号!=0x0
				x++;
			}
		}
		p8d+=LEN_AFN04F10_Pn;
	}
#endif
	return x;
}
#endif

void Uart_AFN09(u32 PORTn)//请求终端配置
{
	u32 i;
	u32 x;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
  u32 LEN_UARTnTx;
  
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	#if (USER/100)==2//用户标识:山东版
	u16 *p16;
	#endif
	
  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<4)
	{
		goto err;
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	TXDIaddr=((u32)p8txbuff)+14;
	TXaddr=((u32)p8txbuff)+14+4;
	TXlen=0;
	f=0;//有无有效数据填入标志
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	p8d=(u8 *)(TXDIaddr);
	p8d[0]=0;
	p8d[1]=0;
	p8d[2]=0;
	p8d[3]=0;
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
//暂删除 DIerr:
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if((TXlen+14+8+2)>LEN_UARTnTx)
				{
					goto err;
				}
				f=0;
			}
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
			p8d=(u8 *)(TXDIaddr);
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
			p8d[3]=0;
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			switch(Fn)//Fn
			{
				case 1://终端版本信息
					i=41;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,(u32)ADDR_AFN09F1,i);
					MR(TXaddr+4,ADDR_TERMINAL+2,8);//8字节设备编号ASCII
					break;
				case 2://终端支持的输入、输出及通信端口配置
#if (Project/100)==0//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(0));
#endif
#if (Project/100)==1//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(3));
#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*6);
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(3+2));
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(3+2));
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(2+2));
#endif
#if (Project/100)==6//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					i=16+1+(12*(3+2));
#endif 
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,(u32)AFN09F2_Addr,i);
					MR(TXaddr+10,ADDR_TERMINAL+10,6);//6字节终端MAC地址
					break;
				case 3://终端支持的其他配置
					i=BitNum(AFN09F3_Addr[17]+(AFN09F3_Addr[18]<<8));//计算置位为1的个数(0-32)
					i+=19;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,(u32)AFN09F3_Addr,i);
					break;
				case 4://终端支持的参数配置
					AFN04_Configure();//终端支持的参数配置,出口:数据在ADDR_DATABUFF
					i=MRR(ADDR_DATABUFF,1);
					i++;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF,i);
					break;
				case 5://终端支持的控制配置
					AFN05_Configure();//终端支持的控制配置,出口:数据在ADDR_DATABUFF
					i=MRR(ADDR_DATABUFF,1);
					i++;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF,i);
					break;
				case 6://终端支持的1类数据配置
					AFN0C_Configure();//终端支持的1类数据配置,出口:数据在ADDR_DATABUFF
					i=MRR(ADDR_DATABUFF,2);
					i=BitNum(i);//计算置位为1的个数(0-32)
					x=i;
					i*=(MRR(ADDR_DATABUFF+2,1)+1);
					i+=2;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF,2);
					TXlen+=2;
					TXaddr+=2;
					for(;x!=0x0;x--)
					{
						MR(TXaddr,ADDR_DATABUFF+2,MRR(ADDR_DATABUFF+2,1)+1);
						TXlen+=MRR(ADDR_DATABUFF+2,1)+1;
						TXaddr+=MRR(ADDR_DATABUFF+2,1)+1;
					}
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					f=1;
					continue;
				case 7://终端支持的2类数据配置
					AFN0D_Configure();//终端支持的2类数据配置,出口:数据在ADDR_DATABUFF
					i=MRR(ADDR_DATABUFF,2);
					i=BitNum(i);//计算置位为1的个数(0-32)
					x=i;
					i*=(MRR(ADDR_DATABUFF+2,1)+1);
					i+=2;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF,2);
					TXlen+=2;
					TXaddr+=2;
					for(;x!=0x0;x--)
					{
						MR(TXaddr,ADDR_DATABUFF+2,MRR(ADDR_DATABUFF+2,1)+1);
						TXlen+=MRR(ADDR_DATABUFF+2,1)+1;
						TXaddr+=MRR(ADDR_DATABUFF+2,1)+1;
					}
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					f=1;
					continue;
				case 8://终端支持的事件记录配置
					i=8;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,(u32)AFN09F8_Addr,i);
					break;
				case 9://远程通信模块版本信息
					i=46;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_AFN09F9,i);
					break;
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
				case 10://本地通信模块版本信息
					i=15;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,(u32)&Terminal_Router->RouterAddr,i);
					break;
#endif
	#if (USER/100)==2//用户标识:山东版
				case 11://山东扩展：终端有效测量点
					//计算全部有效测量点和信息点标识放ADDR_DATABUFF,因最大测量点号2048也只用4+512 BYTE
					//2BYTE 有效测量点数量
					//2BYTE 长度
					//nBYTE 信息点标识
					MR(ADDR_DATABUFF+1024,ADDR_AFN04F150,256);
					p8d=(u8*)(ADDR_DATABUFF+1024);
					p16=(u16 *)(ADDR_DATABUFF);
					p16[0]=0;//有效测量点数量
					p16[1]=0;//长度
					for(i=0;i<255;i++)
					{
					#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
						x=p8d[i];
					#else
						x=p8d[i]+(p8d[i+1]<<8);
						x>>=1;
						x&=0xff;
					#endif	
						if(x!=0)
						{
							p16[1]+=2;//长度
							p16[2+i]=x+((i+1)<<8);
							x=BitNum(x);//计算置位为1的个数(0-32)
							p16[0]+=x;
						}
					}
					i=p16[1]+4;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF,i);
					break;
	#endif
	#if(USER/100)==13//黑龙江版
				case 249://黑龙江扩展F249：信息汇总，用于读取专变终端整机状态
					i=51;
					if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
					{
						goto err;
					}
					//1 行政区域码 BCD 2
					MR(TXaddr,ADDR_AREACODE,2);
					//2 终端地址 BIN 2
					MR(TXaddr+2,ADDR_TERMINAL,2);
					//3 设备类型 BIN 1(1 集中器；2专变终端1型(FKXA4X)；3专变终端1型(FKXB8X)；4专变终端2型(FKXB2X)；5专变终端2型(FKXB4X)；6专变终端3型(FKXA2X)；7专变终端3型(FKXA4X)；8专变终端3型(FCXA2X)。)
					MWR(6,TXaddr+2+2,1);
					//4 电源接线方式 BS8 1(D0~D1编码表示电源接线方式，数值1~3依次表示三相三线、三相四线、单相二线，0为备用；D2~D7备用。)
					//5 额定电压 1376.1-2013的附录A .7 2
				#if (MeterSpec==0)//电能表规格
				//电能表规格
				//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
				//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
				//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
				//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
				//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
				//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
				
				//MeterSpec=16 0.5S 3P4W 50Hz 6400imp/kWh 220V 0.3(6)A
				//MeterSpec=17 0.5S 3P3W 50Hz 20000imp/kWh 100V 0.3(6)A
				//MeterSpec=18 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 0.3(6)A
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x2200,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==1)//电能表规格
					MWR(1,TXaddr+2+2+1,1);
					MWR(0x1000,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==2)//电能表规格
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x577,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==3)//电能表规格
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x2200,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==4)//电能表规格
					MWR(1,TXaddr+2+2+1,1);
					MWR(0x1000,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==5)//电能表规格
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x577,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==16)//电能表规格
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x2200,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==17)//电能表规格
					MWR(1,TXaddr+2+2+1,1);
					MWR(0x1000,TXaddr+2+2+1+1,2);
				#endif
				#if (MeterSpec==18)//电能表规格
					MWR(2,TXaddr+2+2+1,1);
					MWR(0x577,TXaddr+2+2+1+1,2);
				#endif
					//6 上行信道类型 BIN 1(上行信道类型 ：1 GPRS，2 CDMA，3以太网，4, 230 M,5 EPON, 其他保留)
					x=0;
					if(UART1Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					{//GPRS登录
						if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
						{//GSM网络支持GPRS
							x=1;
						}
						else
						{//中兴CDMA(MC8332)
							x=2;
						}
					}
					if(UART10Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					{//以太网登录
						x=3;
					}
					MWR(x,TXaddr+2+2+1+1+2,1);
					//7 Sim卡卡号	BIN 20
					MR(TXaddr+2+2+1+1+2+1,ADDR_MobilePhoneNumber,20);
					//8 上行无线接口信号强度 BIN 1(0~100(最强为100、无信号为0))
					MWR(Terminal_Ram->SignaldBm,TXaddr+2+2+1+1+2+1+20,1);
					//9 下行信道类型 BIN 1(1 窄带载波，2宽带载波，3微功率无线，4 485， 其他保留)
					//10 载波硬件版本 ASCII 8
					//11 载波厂商代码 ASCII 2
					//12 载波芯片代码 ASCII 2
					//13 载波版本日期-日 BCD 1
					//14 载波版本日期-月 BCD 1
					//15 载波版本日期-年 BCD 1
					//16 载波软件版本 BCD 2
					//17 载波表数量 BIN 2
				#if(Project/100)==3//专变采集终端
					MWR(4,TXaddr+2+2+1+1+2+1+20+1,1);
					MC(0,TXaddr+2+2+1+1+2+1+20+1+1,19);
				#endif
					//18 485表数量 BIN 1
					x=Statistics_485();
					MWR(x,TXaddr+2+2+1+1+2+1+20+1+1+19,1);
					break;
	#endif
								
				default:
					goto err;
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
			TXlen+=i;
			TXaddr+=i;
			if(i!=0x0)
			{
				f=1;
			}
		}
	}
	if(TXlen==0x0)
	{
		goto err;
	}
	TXlen+=8;
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}










