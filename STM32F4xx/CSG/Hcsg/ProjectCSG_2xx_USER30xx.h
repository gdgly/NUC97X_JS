
//0xx=盲样电科院送检版,1xx=标准系


#ifndef Terminal_H
#define Terminal_H



//版本-------------------------------------------------------------------------------------
#define UpDataReset     		1		//软件更新后重启:0=否,1=是(注:若MS更新则强制重启)
#define MS_UpDataInit     		3		//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define OS_UpDataInit     		0		//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define Meter_UpDataInit    	3		//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define Terminal_UpDataInit     3		//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define Menu_UpDataInit     	0		//软件更新仪表初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化

#define EnProject     			0			// 0=否,1=是;软件更新时是否判别项目类型
#define EnHV     				0			// 0=否,1=是;软件更新时是否判别硬件版本
#define EnUSER     				0			// 0=否,1=是;软件更新时是否判别用户标识
#define EnSV     				0			// 0=否,1=是;软件更新时是否判别软件版本号,当允许时只有>=当前版本号才有效
#define EnMeter     			0			// 0=否,1=是;电能表软件更新时是否判别电表常数接线方式等电表规格
#define EnMainProtocol     		1			// 0=否,1=是;软件更新时是否判别主通信协议

#define SVER     				0x100		// 软件版本号:电科院测试为V1.0更新到V1.1
#define SVDATE     				0x170831	// 0x160812//软件发行日期:年月日
#define HVER     				0x400		// 硬件版本号:4.00
#define HVDATE     				0x170831	// 硬件发行日期:年月日

#define FactoryCode     		'0','0','0','0'						// 厂商代码       visible-string(SIZE (4))
#define FactoryInfoExt  		'C','H','I','N','T',' ',' ',' '		// 厂家扩展信息   visible-string(SIZE (8))
#define ProduceDate_year    	2017			// 生产日期年(hex)    
#define ProduceDate_month   	5				// 生产日期月(hex) 
#define ProduceDate_day     	21				// 生产日期日(hex) 
#define ProduceDate_hour    	8				// 生产日期时(hex)
#define ProduceDate_minute  	0				// 生产日期分(hex)
#define ProduceDate_second  	0				// 生产日期秒(hex)

#define emWin      				0				// emWin GUI;0=没,1=有
#define Font_ttf   				0				// ttf字体;0=没,1=有

#define SAME_MS       			0				// 0=否,1=是;软件需同时更新标志
#define SAME_OS        			0				// 0=否,1=是;软件需同时更新标志
#define SAME_Meter     			0				// 0=否,1=是;软件需同时更新标志
#define SAME_Terminal  			0				// 0=否,1=是;软件需同时更新标志
#define SAME_Menu      			0				// 0=否,1=是;软件需同时更新标志
#define SAME_Font      			0				// 0=否,1=是;软件需同时更新标志

// 0x08000000	16K BOOT
// 0x08004000	16K 
// 0x08008000	16K 内部交采误差和自检结果数据
// 0x0800C000	16K NAND FLASH非法块表
#define ADDR_MS_ENTRY        	0x08010000		// 64K
#define ADDR_OS_ENTRY        	0x08020000		// 512K
#define ADDR_Font_ENTRY      	0x080A0000		// 256K
#define ADDR_Meter_ENTRY     	0x080E0000		// 128K

#define ADDR_Terminal_ENTRY  	0x08100000		// 256*3K
#define ADDR_Menu_ENTRY      	0x081C0000		// 256K
#define ADDR_Test_ENTRY      	ADDR_Menu_ENTRY	// 256K(同菜单入口和长度)

#define LENmax_MS     			(64*1024)		//空间长度
#define LENmax_OS     			(512*1024)		//空间长度
#define LENmax_Font       		(256*1024)		//空间长度
#define LENmax_Meter			(128*1024)		//空间长度

#define LENmax_Terminal   		(768*1024)		//空间长度
#define LENmax_Menu       		(256*1024)		//空间长度
#define LENmax_Test       		LENmax_Menu		//空间长度(同菜单入口和长度)
//版本结束---------------------------------------------------------------------------------


#define LEN_DATABUFF     		4096			//读写数据缓冲长度,最小4096
#define LEN_128KDATABUFF     	(128*1024)



//---终端DL698---------------------------
#define TMaxTariff     			4				//费率数
#define NAND128kBuff     		1				//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
#define RecordFILEtoPORT     	0				//抄表记录文件按端口号记录:0=否,1=是
#define RecordFILEallinFLASH    0				//全部普通抄表方案数据保存在nand flash:0=否(只抄OAD时存在RAM),1=是
#define NUM_RMmax     			1024			//最大抄表数
#if (NUM_RMmax*8) > (128*1024)
	#error 										//记录文件的地址和数据块长度列表区,列表区长度固定8*NUM_RMmax byte通常会1次读到ADDR_128KDATABUFF抄表标志判断用
#endif
#define RMTASKmax  				60				//最大任务数
#define ExtRMTASKmax  			4				//扩展任务数
#define RMTASKnoPOWER     		0				//停上电事件固定任务号


#define NUMmax_6012_2   		RMTASKmax		//最大抄表任务数
#define NUMmax_6014     		32				//最大普通采集方案数
#define NUMmax_6016     		32				//最大事件采集方案数
#define NUMmax_6018     		32				//最大透明方案数
#define NUMmax_601C     		32				//最大上报方案数
#define NUMmax_Event     		15				//最大事件记录数
#define NUMmax_Event3106RM     	4				//停/上电事件最大抄表个数
#define NUMmax_SWITCHIN      	4				//最大开关量输入路数(门接点)(<=8)
#define NUMmax_IMPIN      		2				//最大脉冲输入路数
#define NUMmax_TAG     			8				//最大总加组数
#define NUMmax_CR      			4				//最大控制轮次数
#define NUMmax_DC     			1				//最大直流模拟量路数
#define NUMmax_RS485   			2				//最大RS485口数
#define TimerS_UARTRAMFREE     	600				//600秒,UART分配的内存自动释放延时
#define PARAMETER_601E     		0				//设置参数601E采集规则库使用:0=内部默认,1=可设置
#define ALLDATANULLSEL     		0				//全部数据空时：0=无返回,1=用空数据0
#define CompareTime_5002     	0				//抄读分钟冻结(采曲线数据)是否比较冻结时间:0=不比较,1=比较
#define NUMmax_50xx     		127				//最大冻结对象OI可设置个数据(<=127)


#define WaitHeartberatACKMinTimer     	30				//心跳发出等待主站确认最小定时秒
#define WaitHeartberatACKMaxTimer     	180				//600//心跳发出等待主站确认最大定时秒
#define WaitHostACKMinTimer     	  	5				//主动上报等待主站确认最小定时秒
#define WaitHostACKMaxTimer     	  	120				//主动上报等待主站确认最大定时秒
#define RouterCheckTimerVal     		(120*60)		//载波路由器死机秒定时值
#define RMRS485OverTimer     			3000			//RS485抄表超时定时值ms
#define RS232DefaultBPS     			(0xb+(6<<5))	//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     			(0xb+(3<<5))	//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define PLCDefaultBPS     				(0xb+(6<<5))	//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     				(0xb+(2<<5))	//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define GPRSDefaultBPS     				(0xb+(10<<5))	//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...


#define BPS_300     				(0xb+(0<<5))
#define BPS_600     				(0xb+(1<<5))
#define BPS_1200     				(0xb+(2<<5))
#define BPS_2400     				(0xb+(3<<5))
#define BPS_4800   					(0xb+(4<<5))
#define BPS_7200   					(0xb+(5<<5))
#define BPS_9600     				(0xb+(6<<5))
#define BPS_19200     				(0xb+(7<<5))
#define BPS_38400     				(0xb+(8<<5))
#define BPS_57600     				(0xb+(9<<5))
#define BPS_115200   				(0xb+(10<<5))
#define BPS_230400   				(0xb+(11<<5))

#define SMSEQ2Byte     			1						// 0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
#define SM_CMGF    				0						// 短信模式:0=PDU模式,1=文本模式
//---终端结束--------------------------------



//UART------------------------------------------------------------------------------------------
#define RS232PORT     	0		//RS232通信口号
#define GPRSPORT     	1		//GPRS通信口号
#define RS485_1PORT     2		//RS485_1通信口号
#define RS485_2PORT     3		//RS485-2通信口号
#define RS485_3PORT     5		//RS485_3通信口号
#define PLC_PORT     	4		//载波模块通信口号
#define IRPORT     		6		//红外通信口号
#define TESAMPORT     	7		//终端ESAM通信口号
#define MESAMPORT     	8		//电表ESAM通信口号
#define CARDPORT     	9		//电表CPUCARD通信口号
#define ENETPORT     	10		//以太网通信口号
#define SMSPORT     	11		//短信通信口号
#define ACSAMPLEPORT    12		//交流采样通信口号(虚拟)
#define EFTPCMDPORT     13		//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define EFTPDATAPORT    14		//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPCMDPORT     15		//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPDATAPORT    16		//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GUI_PORT		17

//UART0(RS232)
#define LEN_UART0IntRx     		8192					//UART0中断接收缓冲长度
#define LEN_UART0Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART0控制数据长度
#define LEN_UART0Rx     		2048					//UART0接收缓冲长度
#define LEN_UART0Tx     		2048					//UART0发送缓冲长度
#define LEN_UART0TxMax     		(2048+64)				//UART0发送缓冲预留的最大长度(注:确保LEN_UART0Tx<=LEN_UART0TxMax)
#define LEN_UART0FnDataBuff     8192					//UART0代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART1(GPRS)
#define LEN_UART1IntATRx     	(LEN_DATABUFF+1024)		//UART1中断接收缓冲长度AT命令
#define LEN_UART1IntRx     		(LEN_DATABUFF+1024)		//UART1中断接收缓冲长度
#define LEN_UART1Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART1控制数据长度
#define LEN_UART1Rx     		2048					//UART1接收缓冲长度
#define LEN_ATTx     			64						//AT发送缓冲长度
#define LEN_UART1Tx     		2048					//UART1发送缓冲长度
#define LEN_UART1TxMax     		(2048+64)				//UART1发送缓冲预留的最大长度(注:确保LEN_UART1Tx<=LEN_UART1TxMax)
#define LEN_UART1FnDataBuff     8192					//UART1代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART2(RS485-1)
#define LEN_UART2IntRx     		8192					//UART2中断接收缓冲长度
#define LEN_UART2Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART2控制数据长度
#define LEN_UART2Rx     		2048					//UART2接收缓冲长度
#define LEN_UART2Tx     		2048					//UART2发送缓冲长度
#define LEN_UART2TxMax     		(2048+64)				//UART2发送缓冲预留的最大长度(注:确保LEN_UART2Tx<=LEN_UART2TxMax)
#define LEN_UART2FnDataBuff     8192					//UART2抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
//UART3(RS485-2)
#define LEN_UART3IntRx     		8192					//UART3中断接收缓冲长度
#define LEN_UART3Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART3控制数据长度
#define LEN_UART3Rx     		2048					//UART3接收缓冲长度
#define LEN_UART3Tx     		2048					//UART3发送缓冲长度
#define LEN_UART3TxMax     		(2048+64)				//UART3发送缓冲预留的最大长度(注:确保LEN_UART3Tx<=LEN_UART3TxMax)
#define LEN_UART3FnDataBuff     8192					//UART3抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
//UART4(RS485-3)
#define LEN_UART4IntRx     		8192					//UART4中断接收缓冲长度
#define LEN_UART4Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART4控制数据长度
#define LEN_UART4Rx     		2048					//UART4接收缓冲长度
#define LEN_UART4Tx     		2048					//UART4发送缓冲长度
#define LEN_UART4TxMax     		(2048+64)				//UART4发送缓冲预留的最大长度(注:确保LEN_UART4Tx<=LEN_UART4TxMax)
#define LEN_UART4FnDataBuff     8192					//UART4抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
//UART5(PLC)
#define LEN_UART5IntRx     		8192					//UART5中断接收缓冲长度
#define LEN_UART5Ctrl     		1024					//UART5控制数据长度
#define LEN_UART5Rx     		2048					//UART5接收缓冲长度
#define LEN_UART5Tx     		2048					//UART5发送缓冲长度
#define LEN_UART5TxMax     		(2048+64)				//UART5发送缓冲预留的最大长度(注:确保LEN_UART5Tx<=LEN_UART5TxMax)
#define LEN_UART5FnDataBuff     8192					//UART5抄表Fn数据缓冲长度
//UART6(红外)
#define LEN_UART6IntRx     		8192					//UART6中断接收缓冲长度
#define LEN_UART6Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART6控制数据长度
#define LEN_UART6Rx     		2048					//UART6接收缓冲长度
#define LEN_UART6Tx     		2048					//UART6发送缓冲长度
#define LEN_UART6TxMax     		(2048+64)				//UART6发送缓冲预留的最大长度(注:确保LEN_UART6Tx<=LEN_UART6TxMax)
#define LEN_UART6FnDataBuff     8192					//UART6代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART7(终端ESAM)
#define LEN_UART7IntRx     		8						//UART7中断接收缓冲长度
#define LEN_UART7Ctrl     		1024					//UART7控制数据长度
#define LEN_UART7Rx     		2048					//UART7接收缓冲长度
#define LEN_UART7Tx     		2048					//UART7发送缓冲长度
#define LEN_UART7TxMax     		2048					//UART7发送缓冲预留的最大长度(注:确保LEN_UART7Tx<=LEN_UART7TxMax)
#define LEN_UART7FnDataBuff     0						//UART7抄表Fn数据缓冲长度
//UART8(电表ESAM)
#define LEN_UART8IntRx     		8						//UART8中断接收缓冲长度
#define LEN_UART8Ctrl     		0						//UART8控制数据长度
#define LEN_UART8Rx     		0						//UART8接收缓冲长度
#define LEN_UART8Tx     		0						//UART8发送缓冲长度
#define LEN_UART8TxMax     		0						//UART8发送缓冲预留的最大长度(注:确保LEN_UART8Tx<=LEN_UART8TxMax)
#define LEN_UART8FnDataBuff     0						//UART8抄表Fn数据缓冲长度
//UART9(电表CPUCARD)
#define LEN_UART9IntRx     		8						//UART9中断接收缓冲长度
#define LEN_UART9Ctrl     		0						//UART9控制数据长度
#define LEN_UART9Rx     		0						//UART9接收缓冲长度
#define LEN_UART9Tx     		0						//UART9发送缓冲长度
#define LEN_UART9TxMax     		0						//UART9发送缓冲预留的最大长度(注:确保LEN_UART9Tx<=LEN_UART9TxMax)
#define LEN_UART9FnDataBuff     0						//UART9抄表Fn数据缓冲长度
//UART10(Ethernet)
#define LEN_UART10IntRx     	8192					//UART10中断接收缓冲长度
#define LEN_UART10Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART10控制数据长度
#define LEN_UART10Rx     		2048					//UART10接收缓冲长度
#define LEN_UART10Tx     		2048					//UART10发送缓冲长度
#define LEN_UART10TxMax     	(2048+64)				//UART10发送缓冲预留的最大长度(注:确保LEN_UART10Tx<=LEN_UART10TxMax)
#define LEN_UART10FnDataBuff    8192					//UART10代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART11(SMS)
#define LEN_UART11IntRx     	8						//UART11中断接收缓冲长度
#define LEN_UART11Ctrl     		0						//UART11控制数据长度
#define LEN_UART11Rx     		0						//UART11接收缓冲长度
#define LEN_UART11Tx     		0						//UART11发送缓冲长度
#define LEN_UART11TxMax     	0						//UART11发送缓冲预留的最大长度(注:确保LEN_UART11Tx<=LEN_UART11TxMax)
#define LEN_UART11FnDataBuff    0						//UART11抄表Fn数据缓冲长度
//UART12(交流采样通信虚拟口)
#define LEN_UART12IntRx     	8						//UART12中断接收缓冲长度
#define LEN_UART12Ctrl     		(1024)					//(1024+((11+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+(2*LEN_RMAddrList*NUM_RMmax)+128)//UART12控制数据长度
#define LEN_UART12Rx     		2048					//UART12接收缓冲长度
#define LEN_UART12Tx     		256						//UART12发送缓冲长度
#define LEN_UART12TxMax     	256						//UART12发送缓冲预留的最大长度(注:确保LEN_UART12Tx<=LEN_UART12TxMax)
#define LEN_UART12FnDataBuff    2048					//UART12抄表Fn数据缓冲长度

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART13IntRx     	(10+1024+2)				//UART13中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART13Ctrl     		1024					//UART13控制数据长度
#define LEN_UART13Rx     		0						//UART13接收缓冲长度
#define LEN_UART13Tx     		1024					//UART13发送缓冲长度
#define LEN_UART13TxMax     	1024					//UART13发送缓冲预留的最大长度(注:确保LEN_UART13Tx<=LEN_UART13TxMax)
#define LEN_UART13FnDataBuff    0						//UART13抄表Fn数据缓冲长度
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART14IntRx     	(10+4096+2)			//UART14中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART14Ctrl     		1024					//UART14控制数据长度
#define LEN_UART14Rx     		0						//UART14接收缓冲长度
#define LEN_UART14Tx     		2048					//UART14发送缓冲长度
#define LEN_UART14TxMax     	2048					//UART14发送缓冲预留的最大长度(注:确保LEN_UART14Tx<=LEN_UART14TxMax)
#define LEN_UART14FnDataBuff    0						//UART14抄表Fn数据缓冲长度
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART15IntRx     	8						//UART15中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART15Ctrl     		0						//UART15控制数据长度
#define LEN_UART15Rx     		0						//UART15接收缓冲长度
#define LEN_UART15Tx     		0						//UART15发送缓冲长度
#define LEN_UART15TxMax     	0						//UART15发送缓冲预留的最大长度(注:确保LEN_UART15Tx<=LEN_UART15TxMax)
#define LEN_UART15FnDataBuff    0						//UART15抄表Fn数据缓冲长度
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART16IntRx     	8						//UART16中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART16Ctrl     		0						//UART16控制数据长度
#define LEN_UART16Rx     		0						//UART16接收缓冲长度
#define LEN_UART16Tx     		0						//UART16发送缓冲长度
#define LEN_UART16TxMax     	0						//UART16发送缓冲预留的最大长度(注:确保LEN_UART16Tx<=LEN_UART16TxMax)
#define LEN_UART16FnDataBuff    0						//UART16抄表Fn数据缓冲长度

#define SocketPortnMin     		1025					//本机端口号最小值
#define SocketPortnMax     		65535					//本机端口号最大值
//UARTEND---------------------------------------------------------------------------------------




//电能表----------------------------------------------------------------------------------------
#define MMaxTariff     			4						//费率数
#define MeterHarmonic     		0						//谐波计算0=没,1=有
#define MeterMaxLadder     		6						//阶梯数
#define MaxMonth     			12						//月冻结数
#define MaxMeterCurveCongeal    (96*10)					//最大曲线点数,60的整数倍(搜索时跳越用)
#define MeterMaxDemandCalculate 1						//最大需量计算：0=脉冲法,1=功率平均法

#define MeterCtrl     			0						//计费控制:0无控制,1仅远程,2本地和远程
#define MeterType     0									//表计类型:0非预付费表,1电量型预付费表,2电费型预付费表
#define MeterSpec     			0						//电能表规格
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
#if MeterSpec==0
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass    			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq    			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	6400					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400					//电表有功常数imp/kWh
#define MeterReactiveConst     	6400					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x6400					//电表无功常数imp/kvarh
#define MeterUBhex     			2200					//参比电压xxx.x(V)
#define MeterUBbcd     			0x2200					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==1
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
#define MeterClass     			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			1						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	6400					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400					//电表有功常数imp/kWh
#define MeterReactiveConst     	6400					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x6400					//电表无功常数imp/kvarh
#define MeterUBhex     			1000					//参比电压xxx.x(V)
#define MeterUBbcd     			0x1000					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==2
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	20000					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000					//电表有功常数imp/kWh
#define MeterReactiveConst     	20000					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x20000					//电表无功常数imp/kvarh
#define MeterUBhex     			577						//参比电压xxx.x(V)
#define MeterUBbcd     			0x577					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==3
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     			0x10					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	6400					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400					//电表有功常数imp/kWh
#define MeterReactiveConst     	6400					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x6400					//电表无功常数imp/kvarh
#define MeterUBhex     			2200					//参比电压xxx.x(V)
#define MeterUBbcd     			0x2200					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==4
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
#define MeterClass     			0x10					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			1						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	20000					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000					//电表有功常数imp/kWh
#define MeterReactiveConst     	20000					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x20000					//电表无功常数imp/kvarh
#define MeterUBhex     			1000					//参比电压xxx.x(V)
#define MeterUBbcd     			0x1000					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==5
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     			0x10					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	20000					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000					//电表有功常数imp/kWh
#define MeterReactiveConst     	20000					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x20000					//电表无功常数imp/kvarh
#define MeterUBhex     			577						//参比电压xxx.x(V)
#define MeterUBbcd     			0x577					//参比电压xxx.x(V)
#define MeterIBhex     			150						//参比电流xx.xx(A)
#define MeterIBbcd     			0x150					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif


#if MeterSpec==16
//MeterSpec=16 0.5S 3P4W 50Hz 6400imp/kWh 220V 0.3(6)A
#define MeterClass     			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	6400					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400					//电表有功常数imp/kWh
#define MeterReactiveConst     	6400					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x6400					//电表无功常数imp/kvarh
#define MeterUBhex     			2200					//参比电压xxx.x(V)
#define MeterUBbcd     			0x2200					//参比电压xxx.x(V)
#define MeterIBhex     			30						//参比电流xx.xx(A)
#define MeterIBbcd     			0x030					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==17
//MeterSpec=17 0.5S 3P3W 50Hz 20000imp/kWh 100V 0.3(6)A
#define MeterClass     			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			1						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	20000					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000					//电表有功常数imp/kWh
#define MeterReactiveConst     	20000					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x20000					//电表无功常数imp/kvarh
#define MeterUBhex     			1000					//参比电压xxx.x(V)
#define MeterUBbcd     			0x1000					//参比电压xxx.x(V)
#define MeterIBhex     			30						//参比电流xx.xx(A)
#define MeterIBbcd     			0x030					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==18
//MeterSpec=18 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 0.3(6)A
#define MeterClass     			0x05					//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     			0						//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     			0						//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     	20000					//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000					//电表有功常数imp/kWh
#define MeterReactiveConst     	20000					//电表无功常数imp/kvarh
#define MeterReactiveConstbcd   0x20000					//电表无功常数imp/kvarh
#define MeterUBhex     			577						//参比电压xxx.x(V)
#define MeterUBbcd     			0x577					//参比电压xxx.x(V)
#define MeterIBhex     			30						//参比电流xx.xx(A)
#define MeterIBbcd     			0x030					//参比电流xx.xx(A)
#define MeterIMaxhex     		600						//最大电流xx.xx(A)
#define MeterIMaxbcd     		0x600					//最大电流xx.xx(A)
#define MeterPStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     	(MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif

//电能表结束------------------------------------------------------------------------------------













//硬件版本--------------------------------------------------------------------------------------
#if(HVER==0x400)							//硬件版本号4.00
#define IC_STM32F42x     	0				// 0=没,1=有
#define IC_STM32F46x     	1				// 0=没,1=有
#define HCLK     			168000000		// 32000000//84000000//168000000//系统时钟(Hz):168,84,32
#define LCDPixels     		3				// 点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE

#define IC_MT29F1G     		0				// 0=没,1=有(128 Mbyte)
#define IC_MT29F2G     		1				// 0=没,1=有(256 Mbyte)
#define IC_MT29F4G     		0				// 0=没,1=有(512 Mbyte)
#define IC_MT29F8G     		0				// 0=没,1=有(1024 Mbyte)
#define IC_S34ML1G     		0				// 0=没,1=有(128 Mbyte)
#define IC_S34ML2G     		0				// 0=没,1=有(256 Mbyte)
#define IC_S34ML4G     		0				// 0=没,1=有(512 Mbyte)
#define IC_S34ML8G     		0				// 0=没,1=有(512 Mbyte)
#define NAND_BUS_WIDTH      8//16				// 8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     	1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     	1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
	#define NAND_1G     	1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
	#define NAND_2G     	1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
	#define NAND_4G     	1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
	#define NAND_8G     	1
#endif
#ifdef  NAND_1G
	#define NAND_BLOCK_COUNT     		1024	//NANDFLASH总有块数(每块128K,每页2048Byte)
	#define NAND_FILE_BLOCK_COUNT     	128		//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
	#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)	//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_2G
	#define NAND_BLOCK_COUNT     		2048	//NANDFLASH总有块数(每块128K,每页2048Byte)
	#define NAND_FILE_BLOCK_COUNT     	128		//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
	#define NAND_REPLACE_BLOCK_COUNT    ((NAND_BLOCK_COUNT/1024)*32)	//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_4G
	#define NAND_BLOCK_COUNT     		4096	//NANDFLASH总有块数(每块128K,每页2048Byte)
	#define NAND_FILE_BLOCK_COUNT     	128		//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
	#define NAND_REPLACE_BLOCK_COUNT    ((NAND_BLOCK_COUNT/1024)*32)	//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_8G
	#define NAND_BLOCK_COUNT     		8192	//NANDFLASH总有块数(每块128K,每页2048Byte)
	#define NAND_FILE_BLOCK_COUNT     	128		//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
	#define NAND_REPLACE_BLOCK_COUNT    250		//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#if NAND_REPLACE_BLOCK_COUNT > 250				//1-250(0xFA)=有替换的替换区的替换块号,0xFB=保留,0xFC=保留,0xFD=未损坏(无替换),0xFE=块损坏(无替换),(0x00,0xFF可能片子出厂初始值禁用),其他值=表示未初始化
	#error NAND_REPLACE_BLOCK_COUNT > 250
#endif

#define IC_MT48LC4M16     	0		// 0=没,1=有(8 Mbyte)
#define IC_MT48LC8M16     	1		// 0=没,1=有(16 Mbyte)
#define IC_MT48LC16M16     	0		// 0=没,1=有(32 Mbyte)
#define IC_MT48LC32M16     	0		// 0=没,1=有(64 Mbyte)
#define IC_MT48LC8M8     	0		// 0=没,1=有(8 Mbyte)
#define IC_MT48LC16M8     	0		// 0=没,1=有(16 Mbyte)
#define IC_MT48LC32M8     	0		// 0=没,1=有(32 Mbyte)
#define IC_MT48LC64M8     	0		// (A11不能配置暂不选用)0=没,1=有(64 Mbyte)
#define IC_SDRAM     		(IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      	1		// 0=没,1=有
#define IC_DS3231     		0		// 0=没,1=有

#define IC_LAN8720     		1		// 0=没,1=有
#define IC_90E36     		1		// 0=没,1=有

#define IC_UC1698     		1		// 0=没,1=有(点阵显示)
#define IC_ST75161     		0		// 0=没,1=有(点阵显示)
#define IC_PCF8562     		0		// 0=没,1=有(段式显示)
#define IC_CPUCARD     		0		// 0=没,1=有
#define IC_MESAM     		0		// 0=没,1=有
#define IC_TESAM     		1		// 0=没,1=有
#define IC_24AAxxx     		1		// 0=没,1=有
#define LOADSWITCH     		0		// 智能表项目的负荷开关0=内部,1=外部
#define IC_BGS2     		0		// GPRS模块BGS2;0=没,1=有
#define IC_GPRS3763     	1		// GPRS模块376.3标准;0=没,1=有
#define IC_MC323     		1		// CDMA华为MC323模块;0=没,1=有
#define KEY_SET     		0		// 设置按键0=没,1=有

#define MaxFileNum     		8		// 最大下载文件个数
//生产目录文件名
	#define FileFactory_PACK          "PACKQCSG.*"
	#define FileFactory_MS            "MSQCSG.*"
	#define FileFactory_OS            "OSQCSG.*"
	#define FileFactory_Meter         "MeterQCSG.*"
	#define FileFactory_Terminal      "TerminalQCSG.*"
	#define FileFactory_Menu          "MenuQCSG.*"
	#define FileFactory_Font          "Font.*"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "TestQCSG.*"
	#define FileFactory_Modele        "ModuleQCSG.*"
	#define FileFactory_PLC_50        "PLC_50.*"
	#define FileFactory_PLC_TC        "PLC_TC.*"
	#define FileFactory_PLC_ES        "PLC_ES.*"
	#define FileFactory_PLC_CX        "PLC_CX.*"
	#define FileFactory_PLC_01        "PLC_01.*"
	#define FileFactory_PLC_LH        "PLC_LH.*"
	#define FileFactory_PLC_NX        "PLC_NX.*"
	#define FileFactory_PLC_QL        "PLC_QL.*"
	#define FileFactory_PLC_MX        "PLC_MX.*"
	#define FileFactory_PLC_FC        "PLC_FC.*"
	#define FileFactory_PLC_NW        "PLC_NW.*"
	#define FileFactory_PLC_SR        "PLC_SR.*"

//用户目录全名
#define FileUserDIR          "/J3000"
#if MeterSpec==0//电能表规格//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACKQCSG_J3000_000_V4.*"
	#define FileUser_MS        "MSQCSG_J3000_000_V4.*"
	#define FileUser_OS        "OSQCSG_J3000_000_V4.*"
	#define FileUser_Meter     "MeterQCSG_J3000_000_V4.*"
	#define FileUser_Terminal  "TerminalQCSG_J3000_000_V4.*"
	#define FileUser_Menu      "MenuQCSG_J3000_000_V4.*"
	#define FileUser_Font      "Font_J3000_000_V4.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "ModuleQCSG_J3000_000_V4.*"
	#define FileUser_PLC_50    "PLC_J3000_000_V4_50.*"
	#define FileUser_PLC_TC    "PLC_J3000_000_V4_TC.*"
	#define FileUser_PLC_ES    "PLC_J3000_000_V4_ES.*"
	#define FileUser_PLC_CX    "PLC_J3000_000_V4_CX.*"
	#define FileUser_PLC_01    "PLC_J3000_000_V4_01.*"
	#define FileUser_PLC_LH    "PLC_J3000_000_V4_LH.*"
	#define FileUser_PLC_NX    "PLC_J3000_000_V4_NX.*"
	#define FileUser_PLC_QL    "PLC_J3000_000_V4_QL.*"
	#define FileUser_PLC_MX    "PLC_J3000_000_V4_MX.*"
	#define FileUser_PLC_FC    "PLC_J3000_000_V4_FC.*"
	#define FileUser_PLC_NW    "PLC_J3000_000_V4_NW.*"
	#define FileUser_PLC_SR    "PLC_J3000_000_V4_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACKQCSG_J3000_000_V4.zip"
	#define FileFTP_MS        "MSQCSG_J3000_000_V4.zip"
	#define FileFTP_OS        "OSQCSG_J3000_000_V4.zip"
	#define FileFTP_Meter     "MeterQCSG_J3000_000_V4.zip"
	#define FileFTP_Terminal  "TerminalQCSG_J3000_000_V4.zip"
	#define FileFTP_Menu      "MenuQCSG_J3000_000_V4.zip"
	#define FileFTP_Font      "Font_J3000_000_V4.zip"
	#define FileFTP_MODULE    "ModuleQCSG_J3000_000_V4.zip"
	#define FileFTP_PLC_50    "PLC_J3000_000_V4_50.zip"
	#define FileFTP_PLC_TC    "PLC_J3000_000_V4_TC.zip"
	#define FileFTP_PLC_ES    "PLC_J3000_000_V4_ES.zip"
	#define FileFTP_PLC_CX    "PLC_J3000_000_V4_CX.zip"
	#define FileFTP_PLC_01    "PLC_J3000_000_V4_01.zip"
	#define FileFTP_PLC_LH    "PLC_J3000_000_V4_LH.zip"
	#define FileFTP_PLC_NX    "PLC_J3000_000_V4_NX.zip"
	#define FileFTP_PLC_QL    "PLC_J3000_000_V4_QL.zip"
	#define FileFTP_PLC_MX    "PLC_J3000_000_V4_MX.zip"
	#define FileFTP_PLC_FC    "PLC_J3000_000_V4_FC.zip"
	#define FileFTP_PLC_NW    "PLC_J3000_000_V4_NW.zip"
	#define FileFTP_PLC_SR    "PLC_J3000_000_V4_SR.zip"
#endif

//硬件版本结束----------------------------------------------------------------------------------
#endif




#endif

