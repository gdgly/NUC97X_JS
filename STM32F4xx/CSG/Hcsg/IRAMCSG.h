

#ifndef IRAM_H
#define IRAM_H


#ifdef IC_STM32F4xx
//0x10000000 64k(CCM DATA RAM)
//0x20000000 112k
//0x2001c000 16k (Ethernet or the USB OTG HS)
//0x20020000 64k(STM32F42xxx)
//0x40024000 4K(BKPSRAM)
//RTC 备份寄存器 20个
#define ADDR_IRAM_Start     0x10000000//64KB(CCM DATA RAM)
#endif
#ifdef IC_NUC9xx
#define ADDR_IRAM_Start     0x00F00000//256K
#endif


#define ADDR_Comm_Ram_Start     ADDR_IRAM_Start
// 公用数据上电初始化清0--------------------------------------------------------------------------------------------
// 4 字节留给显示字符转换为点阵LCDDot汇编程序用(仅为固定地址0x10000000要求)
#define ADDR_MRWBUFF     (ADDR_IRAM_Start+4)
#define LEN_MRWBUFF     12
// 12字节存储器读写到寄存器缓冲,8+4字节,其中4字节仅为显示保留
// 4byte 存储器读写的ADDR_Sour或Data(当读写标志=MC时)
// 4byte 存储器读写的ADDR_Dest
// 4byte 存储器读写的Byte
// 1byte 存储器读写的标志:0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
// 1byte NAND读写标志
// 1byte nand内部ECC:0=不允许,1=允许
// 1byte os状态:0=没起动,1=起动,2=暂停
typedef __packed struct
{
	u32 ADDR_Sour_or_Data;			// 存储器读写的ADDR_Sour或Data(当读写标志=MC时)
	u32 ADDR_Dest;					// 存储器读写的ADDR_Dest
	u32 Byte;						// 存储器读写的Byte
	u8 RWC;							// 存储器读写的标志:0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	u8 NAND;						// NAND读写标志
	u8 NAND_ECC;					// nand内部ECC,0x55=允许,其他值=不允许
	u8 osSTATE;						// os状态:0=没起动,1=起动,2=暂停
}MRW_TypeDef;
#define MRW     ((MRW_TypeDef *)(ADDR_MRWBUFF+LEN_MRWBUFF))
#define ADDR_DATABUFF     (ADDR_MRWBUFF+LEN_MRWBUFF+16)
									// 4K字节 LEN_DATABUFF字节读写数据缓冲
#define ADDR_4KWRITE_BUFF     (ADDR_DATABUFF+LEN_DATABUFF)
									// 4K字节FLASH写缓冲(上紧接ADDR_DATABUFF,特殊使用时LEN_DATABUFF至少达到8K)

#define ADDR_FileDownload     (ADDR_4KWRITE_BUFF+4096)
typedef __packed struct
{
	u32 File;				// 文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,B8=Test,...B31=
	u8 USBH_Initialize;		// USBH:0=没初始化,1=初始化
	u8 DownloadCount;		// 文件下载计数
	u8 UpdateCount;			// 文件更新计数
	u8 New;					// 有新文件载入标志:0=有,1=没
	u8 USB;					// 插入状态滤波:b0=主机插入,b1=设备插入
	u8 PORTn;				// 完成下载的端口号
	u8 NAND_BUSBIT;//nand数据位宽度自动识别:8=8bit,16=16bit
	u8 USB0Task;
}FileDownload_TypeDef;
#define FileDownload     ((FileDownload_TypeDef *)(ADDR_FileDownload))

#define ADDR_FilePlay     (ADDR_FileDownload+12)
typedef __packed struct
{
	u32 LEN_FILE;			// 文件总长度
	u32 LEN_FILEREAD;		// 已读文件长度
	u32 ADDR_Myalloc_Play;	// 当前播放的文件分配的内存指针寄存,用于暂时占有内存将来释放,同时当!=0表示正在播放
    u32 ADDR_WAVDATA;		// WAV数据开始地址
	u32 Hz;					// 采样频率
	u8  Bits;				// 位数
	u8  CHs;				// 通道数
	u8  res1;
	u8  res2;
  	u32 LEN_WAVDATA;		// WAV数据长度
  	u32 LEN_WAVDATAOUT;		// 已播放WAV数据长度
	u8 FileName[32];		// 当前播放的文件名
}FilePlay_TypeDef;
#define FilePlay     ((FilePlay_TypeDef *)(ADDR_FilePlay))

typedef __packed struct
{
#define ADDR_SYSTEM     (ADDR_FilePlay+64)
	u32 IRQ0;				// 重启OS时原中断允许保存
	u32 IRQ1;				// 重启OS时原中断允许保存
	u32 IRQ2;				// 重启OS时原中断允许保存
	u32 res0;
	u32 res1;
	u32 ADDR_Myalloc_TTF;	// ttf文件分配的内存指针寄存
	u8 Init_LOG;			// 上电后LOG已初始化标志:0=没，!=0已初始化
	u8 Init_emWin;			// 上电后emWin已初始化标志:=0没初始化,=1初始化失败,>=2初始化成功
	u8 RealDataSave;		// 上次实时数据保护的秒
	u8 TimerDataSave;		// 上次定时数据保护的秒或分或时
	u8 outclk;				// 输出时钟:B0=TEASMCLK,B1=MESAMCLK,B2=CARDCLK
	u8 POWER;				// 电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	u8 outpower;			// 输出电源B0=A5V0,B1=BACKLIGHT,B2=LED3V3,B3=E5V0,B4=R5V0,B5=USB5V0,B6=,B7=中断关电源标志
	u8 Program;				// (汇编取地址)当前运行的程序段(用于HardFault中断时重启OS识别):0=空,1=FS,2=USB0,3=USB1,4=ETH,5=...
	u8 Init_OS;				// OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	u8 After_Init_OS;		// OS重启后使用标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	u8 MS_Task;				// MS任务
	u8 IntFlags;			// 中断服务冲突标志
							// B0表示中断中使用CRC计算冲突标志
							// B1=中断中写IEEPROM冲突标志
							// B2=中断中使用ADC冲突标志
							// B3=中断中读写EFLASH冲突标志
							//
	u8 RESET;				// 0=看门狗复位,1=上电复位,0x55=重启动(立即使用看门狗复位)
	u8 ACTION_RESET_PORTn;	// 操作复位的通信端口号
	u16 msFILESearchCount;	// 文件搜索计数
	u8 msFILEchange;		// 文件管理数据变化:0=变化,!=没变化(1=管理数据保存,2=不需主动上报处理)
	u8 DataSave;			// 数据保护:0=空,1=电网有电超过10秒数据需保护,2=数据已保护
	u16 TESTKEY;
							// B0=下移按键(手动)
							// B1=上移按键(手动)
							// B2=左移按键(手动)
							// B3=右移按键(手动)
							// B4=确认按键(手动)
							// B5=退出按键(手动)
							// B6=设置按键(手动)
							// B7=工厂按键(手动)
							// B8=表盖按键(手动)
							// B9=端盖按键(手动)
	u32 ErrorIC;			// 实时运行时发现的错误
							// B0=STM32F4xx SRAM
							// B1=ERAM 外部RAM芯片(MT48LC4M16或MT48LC8M16)
							// B2=EFLASH1 S25FLxxx
							// B3=EFLASH2 S34MLxxG1
							// B4=RTC 外部RTC时钟芯片(DS3231或RX8025T)
							// B5=EMU 计量芯片(ATT7022E,IDC90E36)
							// B6=PCF8562 段式LCD驱动芯片
							// B7=LAN8720A	以太网PHY芯片
							// B8=MESAM 电能表用ESAM芯片
							// B9=TESAM 终端用ESAM芯片
							// B10=24AAxxx 以太网用MAC地址芯片
							// B11=
							// B12=STM32F4xx ECC
							// B13=STM32F4xx RTC
							// B14=红外通信(自动,非仿真口用自发自收检测,IO仿真口在中断中检测)
							// B15=
							// B16=电表掉电保护数据错误
							// B17=终端掉电保护数据错误
							// B18=抄表数据缓冲长度不够
							// B19=系统堆栈长度不够
							// B20=UART0收发引脚
							// B21=UART1收发引脚
							// B22=UART2收发引脚
							// B23=UART3收发引脚
							// B24=UART4收发引脚
							// B25=UART5收发引脚
							// B26=UART6收发引脚
							// B27=
							// B28=
							// B29=电池充电电源
							// B30=GPRS模块电源
							// B31=LPC11xx专变用控制模块
#define ADDR_RTCBUFF      	(ADDR_SYSTEM+48)		// 时钟缓冲(字对齐)
  u8 RTCBuff[8];									// 秒,分,时,周,日,月,年,NC
#define ADDR_MYMDHMS     	(ADDR_RTCBUFF+8)		// 仪表时钟
  u8 MYMDHMS[8];									// 秒,分,时,日,月,年,周,NC
#define ADDR_DL698YMDHMS    (ADDR_MYMDHMS+8)		// DL698时钟(hex)
  u8 DL698YMDHMS[10];								// 年年(先高字节后低字节),月,日,时,分,秒,百分秒,周
#define ADDR_URTC     		(ADDR_DL698YMDHMS+10)	// 时钟电池电压(内部)(半字对齐)
  u16 URTC;											// BCD 2位小数
#define ADDR_ULCD     		(ADDR_URTC+2)			// 停电抄表电池电压(外部)(半字对齐)
  u16 ULCD;											// BCD 2位小数
#define ADDR_UBACKUP     	(ADDR_ULCD+2)			// 可充电备用电池电压(外部)(半字对齐)
  u16 UBACKUP;										// BCD 2位小数
#define ADDR_UGPRS     		(ADDR_UBACKUP+2)		// GPRS电源电压(半字对齐)
  u16 UGPRS;										// BCD 2位小数
#define ADDR_UDC     		(ADDR_UGPRS+2)			// 直流模拟量(半字对齐)
  u16 UDC;											// HEX
#define ADDR_TEMP     		(ADDR_UDC+2)			// 表内温度(半字对齐)
  u16 TEMP;											// BCD 1位小数(b15符号位)
#define ADDR_BUTTON     	(ADDR_TEMP+2)
													// 位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
  u16 ButtonVal;									// 按键状态(使用后清0)
  u16 ButtonKeep;									// 按下保持3秒状态(使用后清0)
  u16 ButtonCurrent;								// 中间过程
  u8 Factory;										// 0x55=工厂状态
  u8 msFILEcheck;									// 上次msFILE检查时的实时钟
#define ADDR_Time_Int     	(ADDR_BUTTON+8)
	u8 MS1_PR; 										// 1mS的10分频产生10mS
	u8 MS10_PR; 									// 10mS的100分频产生1S
	u8 RS485_CTRL1_Timer;							// RS485-1在1ms定时中断中当定时值减为0时转为接收
	u8 RS485_CTRL2_Timer;							// RS485-2在1ms定时中断中当定时值减为0时转为接收
#define ADDR_Comm_MS_Timer     (ADDR_Time_Int+4)
#define ADDR_UART0Timer     	ADDR_Comm_MS_Timer
#define ADDR_UART1Timer     	ADDR_UART0Timer+2
#define ADDR_UART2Timer     	ADDR_UART1Timer+2
#define ADDR_UART3Timer     	ADDR_UART2Timer+2
#define ADDR_UART4Timer     	ADDR_UART3Timer+2
#define ADDR_UART5Timer     	ADDR_UART4Timer+2
#define ADDR_UART6Timer     	ADDR_UART5Timer+2
#define ADDR_UART7Timer     	ADDR_UART6Timer+2
#define ADDR_UART8Timer     	ADDR_UART7Timer+2
#define ADDR_UART9Timer     	ADDR_UART8Timer+2
#define ADDR_UART10Timer     	ADDR_UART9Timer+2
#define ADDR_UART11Timer     	ADDR_UART10Timer+2
#define ADDR_UART12Timer     	ADDR_UART11Timer+2
#define ADDR_UART13Timer     	ADDR_UART12Timer+2
#define ADDR_UART14Timer     	ADDR_UART13Timer+2
#define ADDR_UART15Timer     	ADDR_UART14Timer+2
#define ADDR_UART16Timer     	ADDR_UART15Timer+2
#define ADDR_GPMSTimer     		ADDR_UART16Timer+2
#define ADDR_SPI1Timer     		ADDR_GPMSTimer+2
#define ADDR_SPI2Timer     		ADDR_SPI1Timer+2
#define ADDR_SPI3Timer     		ADDR_SPI2Timer+2
  u16 UART0Timer;			// 1 UART0 每10MS减1定时器
  u16 UART1Timer;			// 2 UART1 每10MS减1定时器
  u16 UART2Timer;			// 3 UART2 每10MS减1定时器
  u16 UART3Timer;			// 4 UART3 每10MS减1定时器
  u16 UART4Timer;			// 5 UART4 每10MS减1定时器
  u16 UART5Timer;			// 6 UART5 每10MS减1定时器
  u16 UART6Timer;			// 7 UART6 每10MS减1定时器
  u16 UART7Timer;			// 8 UART7 每10MS减1定时器
  u16 UART8Timer;			// 9 UART8 每10MS减1定时器
  u16 UART9Timer;			// 10 UART9 每10MS减1定时器
  u16 UART10Timer;			// 11 UART10 每10MS减1定时器
  u16 UART11Timer;			// 12 UART11 每10MS减1定时器
  u16 UART12Timer;			// 13 UART12 每10MS减1定时器
  u16 UART13Timer;			// 14 UART13 每10MS减1定时器
  u16 UART14Timer;			// 15 UART14 每10MS减1定时器
  u16 UART15Timer;			// 16 UART15 每10MS减1定时器
  u16 UART16Timer;			// 17 UART16 每10MS减1定时器
  u16 GPMSTimer;			// 18 通用(兼虚拟UART11) 每10MS减1定时器
  u16 SPI1Timer;			// 19 SSP0 每10MS减1定时器
  u16 SPI2Timer;			// 20 SSP1 每10MS减1定时器
  u16 EMCTimer;				// 21 检测到有EMC干扰每10MS减1定时器   SPI3Timer;//21 SSP2 每10MS减1定时器
  u16 RUNTimer;				// 22 运行LED指示灯 每10MS减1定时器
  u16 ButtonTimer;			// 23 按键按下延时 每10MS减1定时器
  u16 ButtonKeepTimer;		// 24 按键保持 每10MS减1定时器
  u16 MenuOutTimer;			// 25 菜单退出 每10MS减1定时器(终端)
  u16 USBMSTimer;			// 26 USB 每10MS减1定时器(终端)
  u16 DisplayScanTimer;		// 27 显示扫描 每10MS减1定时器(终端)
  u16 CheckLEDTimer;		// 28 上电全部LED检查显示 每10MS减1定时器
  u16 ReadRTCTimer;			// 29 读ERTC每10MS减1定时器
  u16 ChargeBatteryTimer;	// 30 无充电电池接入时>6V到有电池接入定时,电源下降因有超级电容误判为正常电池电压,延时一定时间电容放电后ADC值再作为电压值
  u16 DataSaveTimer;		// 31 数据保存定时器 每10ms减1定时器


#ifdef IC_NUC9xx
 u16 WaitUartCompleteMSTimer;//32 等待UART帧返回定时器 每10ms减1定时器
#define Comm_MS_Timer_Num     32//共用10MS定时器数
#endif 

#ifndef Comm_MS_Timer_Num
	#define Comm_MS_Timer_Num     31//共用10MS定时器数
#endif


#define ADDR_Comm_S_Timer     (ADDR_Comm_MS_Timer+(2*Comm_MS_Timer_Num))
	u16 DisplayAllErrorTimer;		// 1 上电全显或系统错误显示秒定时器
	u16 DisplayLoopTimer;			// 2 循显秒定时器(终端)
	u16 BackLightTimer;				// 3 背光灯显示秒定时器(终端)
	u16 BattWork_S_timer;			// 4 电池工作关机秒定时器
	
#define Comm_S_Timer_Num     4		//共用秒定时器数

#define ADDR_BUZZCTRL     (ADDR_Comm_S_Timer+(2*Comm_S_Timer_Num))//蜂鸣器控制缓冲
  u8 BUZZCTRL[9];
		// 1BYTE 0 0=运行当前,1=运行插入
		// 1BYTE 1 当前蜂鸣号0=不蜂鸣,!=0蜂鸣号
		// 1BYTE 2 当前运行插入号0-3
		// 1BYTE 3 运行级数计数
		// 1byte 4 等待插入0蜂鸣号0=不蜂鸣,!=0蜂鸣号
		// 1byte 5 等待插入1蜂鸣号0=不蜂鸣,!=0蜂鸣号
		// 1byte 6 等待插入2蜂鸣号0=不蜂鸣,!=0蜂鸣号
		// 1byte 7 等待插入3蜂鸣号0=不蜂鸣,!=0蜂鸣号
		// 1BYTE 8 蜂鸣器声音允许/禁止,=0禁止,!=0允许
#define ADDR_WaitFLASHErase     ADDR_BUZZCTRL+9
  u8 WaitFLASHErase;								// 0x55=等待FLASH擦除插入任务标志
#define ADDR_RouterType     ADDR_WaitFLASHErase+1
	u8 RouterType;									// 路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙
													// 100=保留,101=麦希,102=友讯达,103=有方,104=桑锐
#define ADDR_RemoteWirelessModuleType     ADDR_RouterType+1
	u8 RemoteWirelessModuleType;					// 远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
#define ADDR_RamLanguage     ADDR_RemoteWirelessModuleType+1//显示语言(同步FLASH中的ADDR_Language)
	u8 Language;									// 0=中文,1=英文,2=
#define ADDR_CSPN     ADDR_RamLanguage+1			//读取SIM卡服务运营商名称
	u8 CSPN;										// 0=空
													// 1=中国移动通信-----china mobile  CMCC
													// 2=中国联通通讯-----china unicom  CUCC
													// 3=中国电信  ------CHINA TELECOM  CTCC
#define ADDR_UARText     (((ADDR_CSPN+1)+1)&0xfffffffe)
	u8 RS485_CTRL3_Timer;							// RS485-3在1ms定时中断中当定时值减为0时转为接收
	u8 UART6TR;										// 0=接收,1=发送
	u8 UART6TIM;									// 0.5BIT定时中断计数
	u8 UART6BITCOUNT;								// 接收发送移位计数
	u16 UART6DATA;									// 数据寄存器
#define ADDR_RUNSPEEDTEST     (ADDR_UARText+6)		// 循环运行速度测试(半字对齐)
//2BYTE 每循环计数
	u16 LOOPCOUNT;
//2BYTE 每循环计数在秒更新时寄存
	u16 LOOPCOUNT_COPY;
#define ADDR_VDD5V0     (ADDR_RUNSPEEDTEST+4)//VDD5V0电压
	u16 VDD5V0;//BCD 2位小数
#define ADDR_MODE3in1     ADDR_VDD5V0+2//三合一终端当前运行模式
	u8 MODE3in1;//三合一终端当前运行模式:0=专变,1=集中器
	u8 res2;//仅为上电清0补足4byte
	u8 res3;//仅为上电清0补足4byte
	u8 res4;//仅为上电清0补足4byte

}Comm_Ram_TypeDef;

#define Comm_Ram     			((Comm_Ram_TypeDef *)(ADDR_SYSTEM))

////ybwhz20190221  测试 用 增加 
#define ADDR_YBWHZ_DATABUFF   (ADDR_MODE3in1 + 4) 	//ybwhz20190221  测试 用 增加 占用64字节 不确定是否有影响?
////ybwhz20190221  测试 用 增加 

#define ADDR_Comm_Ram_End     (ADDR_YBWHZ_DATABUFF + 64) 	//(ADDR_RUNSPEEDTEST + 4) 

#define LEN_Comm_Ram     		(9*1024)	// 限定总长度字节数

#if (ADDR_Comm_Ram_End-ADDR_Comm_Ram_Start) > LEN_Comm_Ram
	#error ADDR_Comm_Ram_End Over
#endif
//公用数据上电初始化清0-------------------------------------------------------------------------------------------------


//电能表数据(掉电不保护)---------------------------------------------------------------------------------------
#define ADDR_Meter_Ram_Start     	(ADDR_IRAM_Start+LEN_Comm_Ram)
#if((Project/100) >= 5) && ((Project/100)!=9) 						// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Meter_Ram_End     	(ADDR_Meter_Ram_Start+0)
	#define LEN_Meter_Ram     0					// 限定总长度字节数
#else
	#define LEN_Meter_Ram     		(2*1024)	//限定总长度字节数
	#include "IRAM_MeterCSG.h"
#endif

#if (ADDR_Meter_Ram_End - ADDR_Meter_Ram_Start) > LEN_Meter_Ram
	#error ADDR_Meter_Ram_End Over
#endif
// 电能表数据(掉电不保护)结束------------------------------------------------------------------------------------


// 终端数据(掉电不保护)-----------------------------------------------------------------------------------------
#define ADDR_Terminal_IRAM_Start     	(ADDR_Meter_Ram_Start+LEN_Meter_Ram)
#if((Project/100) == 0)								//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Terminal_IRAM_End     	(ADDR_Terminal_IRAM_Start+0)
	#define LEN_Terminal_IRAM     		0			// 限定总长度字节数
#else
	#define LEN_Terminal_IRAM     		(53*1024)	// 限定总长度字节数
	#include "IRAM_TerminalCSG.h"
#endif

#if (ADDR_Terminal_IRAM_End - ADDR_Terminal_IRAM_Start) > LEN_Terminal_IRAM
	#error ADDR_Terminal_IRAM_End Over
#endif
// 终端数据(掉电不保护)结束-------------------------------------------------------------------------------------



#define ADDR_IRAM_End     (ADDR_Terminal_IRAM_Start+LEN_Terminal_IRAM)


//#if (LEN_Comm_Ram+LEN_Meter_Ram + LEN_Terminal_Ram) > (64*1024)
//#error LEN_Comm_Ram+LEN_Meter_Ram + LEN_Terminal_Ram > 64K
#if (LEN_Comm_Ram + LEN_Meter_Ram + LEN_Terminal_IRAM) > (64*1024)
#error LEN_Comm_Ram + LEN_Meter_Ram + LEN_Terminal_IRAM > 64K
#endif
#if (ADDR_IRAM_End-ADDR_IRAM_Start) > (64*1024)
	#error ADDR_IRAM_End-ADDR_IRAM_Start > 64K
#endif

//-------------------------------------------------------------------------------------------------



	
// 0x20020000 64k(STM32F42xxx)
// 电能表数据(掉电保护)------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
//0x20020000 64k(STM32F42xxx)
#define ADDR_Meter_SaveRam_Start     0x20020000
#endif
#ifdef IC_NUC9xx
#define ADDR_Meter_SaveRam_Start     0x00F40000//256K
#endif
#if((Project/100)>=5) && ((Project/100)!=9)						// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Meter_SaveRam_End     (ADDR_Meter_SaveRam_Start+0)
	#define LEN_Meter_SaveRam     0			// 限定总长度字节数
#else
	#define LEN_Meter_SaveRam     (1*1024)	// 限定总长度字节数
	#include "IRAM_MeterSaveCSG.h"
#endif

#if (ADDR_Meter_SaveRam_End-ADDR_Meter_SaveRam_Start)>LEN_Meter_SaveRam
	#error ADDR_Meter_SaveRam_End Over
#endif
//电能表数据(掉电保护)结束-------------------------------------------------------------------------------------------------
//终端数据(掉电保护)-----------------------------------------------------------------------------------------
#define ADDR_Terminal_SaveRam_Start     	(ADDR_Meter_SaveRam_Start+LEN_Meter_SaveRam)
#if((Project/100)==0)								// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Terminal_SaveRam_End     	(ADDR_Terminal_SaveRam_Start+0)
	#define LEN_Terminal_SaveRam     		0		// 限定总长度字节数
#else
	#define LEN_Terminal_SaveRam     		((63*1024) - 8)	// 限定总长度字节数
	#include "IRAM_TerminalSaveCSG.h"
#endif

#if (ADDR_Terminal_SaveRam_End-ADDR_Terminal_SaveRam_Start)>LEN_Terminal_SaveRam
	#error ADDR_Terminal_SaveRam_End Over
#endif
// 终端数据(掉电保护)结束-------------------------------------------------------------------------------------
// 4BYTE 固定标识0x12345678
// 4BYTE 上全部数据CRC32
// 掉电保护结束-------------------------------------------------------------------------------------


#define LEN_SaveRam     (LEN_Meter_SaveRam + LEN_Terminal_SaveRam + 8)	//注:需nand 2048页写对齐



#if LEN_SaveRam > (64*1024)
	#error LEN_SaveRam>64k
#endif
#if (ADDR_Terminal_SaveRam_End-ADDR_Meter_SaveRam_Start)>((64*1024)-8)
	#error ADDR_Terminal_SaveRam_End-ADDR_Meter_SaveRam_Start>64k
#endif



//-------------------------------------------------------------------------------------------------

#ifdef IC_STM32F4xx
//0x40024000 4K(BKPSRAM) NAND FLASH写缓冲(仅在数据检查错误时清0)
#define ADDR_BKPSRAM     0x40024000
#endif
#ifdef IC_NUC9xx
#define ADDR_BKPSRAM     0x00F80000//256k
#endif
// 2BYTE 总块数
// 2BYTE 总字节数(全部字节,也即下块的开始地址偏移)
// 数据块1
// 4BYTE 写入NAND FLASH的目的地址
// 2BYTE 数据字节数
// 2BYTE 已部分写入的字节数
// nBYTE 数据
// ...
// 数据块n


//-------------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
//RTC 备份寄存器(20个) RTC_BASE+Address offset: 0x50
#define ADDR_RTCBKPSRAM_START     (RTC_BASE+0x50)
#endif
#ifdef IC_NUC9xx
//RTC 备份寄存器(16个)
#define ADDR_RTCBKPSRAM_START     RTC_BA

#endif
// RTC->BKP0R
// B0-B7 USB0任务:0=空,1=等待插入,2=主机插入延时,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
// B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
// B16-B23 
// B24-B31

// RTC->BKP1R
// 4BYTE 已更新的需同时更新文件:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router

// RTC->BKP2R
// 擦除掉电保存区FLASH后置为0xffffffff;表示:1.掉电中断时不需擦除.2.在1mS中断中停止计时
// 1BYTE 电源掉电时维持电池电压(BCD x.x)
// 1BYTE 电源掉电时数据保存时间ms
// 2BYTE 电源掉电维持工作时间ms

// RTC->BKP3R
// 4byte nand寿命测试,b0-b30计数,b31=1表示错误停止

// RTC->BKP4R
// 4byte LOG_1376_1 nand缓冲写指针

// RTC->BKP5R
// 4byte LOG_1376_2 nand缓冲写指针

// RTC->BKP6R
// 4byte LOG_1376_3 nand缓冲写指针

// RTC->BKP7R
// 4byte LOG_645_1997 nand缓冲写指针

// RTC->BKP8R
// 4byte LOG_645_2007 nand缓冲写指针

// RTC->BKP9R

// ...
// RTC->BKP19R
// 4byte 停电上事件判断用上电标志;看门狗复位自动重启=0x12345678,其他值为上电
#define LEN_RTCBKPSRAM_START     80

#endif
























