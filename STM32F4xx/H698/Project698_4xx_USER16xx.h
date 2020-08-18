
//16xx=内蒙系
#ifndef Terminal_H
#define Terminal_H



//版本-------------------------------------------------------------------------------------
#define UpDataReset          1//软件更新后重启:0=否,1=是(注:若MS更新则强制重启)
#define MS_UpDataInit        3//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define OS_UpDataInit        0//3//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define Meter_UpDataInit     3//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
#define Terminal_UpDataInit  3//7//软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化,7=参数初始化(档案和nand文件数据转移到新地址)
#define Menu_UpDataInit      0//软件更新仪表初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化

#define EnProject       0//0=否,1=是;软件更新时是否判别项目类型
#define EnHV            0//0=否,1=是;软件更新时是否判别硬件版本
#define EnUSER          0//1//0=否,1=是;软件更新时是否判别用户标识
#define EnSV            0//0=否,1=是;软件更新时是否判别软件版本号,当允许时只有>=当前版本号才有效
#define EnMeter         0//0=否,1=是;电能表软件更新时是否判别电表常数接线方式等电表规格
#define EnMainProtocol  1//0=否,1=是;软件更新时是否判别主通信协议

#ifdef IC_STM32F4xx
#define SVER     0x100//软件版本号:电科院测试为V1.0更新到V1.1
#define SVDATE   0x181210//软件发行日期:年月日
//#define HVER     0x400//硬件版本号:4.00
#define HVER     0x410//硬件版本号:4.10;nandbusbit=8,LCD总线分开
//#define HVER    	 0x420//硬件版本号:4.20;增加超级电容
#define HVDATE   0x180109//0x160701//硬件发行日期:年月日
#endif
#ifdef IC_NUC9xx
#define SVER     0x100//软件版本号:电科院测试为V1.0更新到V1.1
#define SVDATE   0x181210//软件发行日期:年月日
	#ifndef LINUX
#define HVER     0x500//NUC97x硬件版本号:5.00 BOOT_SPI
	#else
#define HVER     0x510//NUC97x硬件版本号:5.10 BOOT_NAND linux
	#endif
#define HVDATE   0x190501//0x160701//硬件发行日期:年月日
#endif

#define FactoryCode      'Z','J','Z','T'//'0','0','0','0'//厂商代码       visible-string(SIZE (4))
#define FactoryInfoExt   'D','J','1','7', 'G','2','0','0'//'C','H','I','N','T',' ',' ',' '//厂家扩展信息   visible-string(SIZE (8))
#define ProduceDate_year        2018//生产日期年(hex)
#define ProduceDate_year_BCD    0x2018//生产日期年(BCD)
#define ProduceDate_month       4//5//6//生产日期月(hex) 
#define ProduceDate_month_BCD   0x04//0x05//0x06//生产日期月(BCD) 
#define ProduceDate_day     2//生产日期日(hex) 
#define ProduceDate_hour    8//生产日期时(hex)
#define ProduceDate_minute  0//生产日期分(hex)
#define ProduceDate_second  0//生产日期秒(hex)

#define emWin      0//emWin GUI;0=没,1=有
#define Font_ttf   0//ttf字体;0=没,1=有

#define SAME_MS        0//0=否,1=是;软件需同时更新标志
#define SAME_OS        0//0=否,1=是;软件需同时更新标志
#define SAME_Meter     0//0=否,1=是;软件需同时更新标志
#define SAME_Terminal  0//0=否,1=是;软件需同时更新标志
#define SAME_Menu      0//0=否,1=是;软件需同时更新标志
#define SAME_Font      0//0=否,1=是;软件需同时更新标志

#ifdef IC_STM32F4xx
//0x08000000	16K BOOT
//0x08004000	16K 
//0x08008000	16K 内部交采误差和自检结果数据
//0x0800C000	16K NAND FLASH非法块表
#define ADDR_MS_ENTRY        0x08010000//64K
#define ADDR_OS_ENTRY        0x08020000//512K
#define ADDR_Font_ENTRY      0x080A0000//256K
#define ADDR_Meter_ENTRY     0x080E0000//128K
#define ADDR_Terminal_ENTRY  0x08100000//256*3K
#define ADDR_Menu_ENTRY      0x081C0000//256K
#define ADDR_Test_ENTRY      ADDR_Menu_ENTRY//256K(同菜单入口和长度)

#define LENmax_MS     		(64*1024)//空间长度
#define LENmax_OS     		(512*1024)//空间长度
#define LENmax_Font       (256*1024)//空间长度
#define LENmax_Meter      (128*1024)//空间长度
#define LENmax_Terminal   (768*1024)//空间长度
#define LENmax_Menu       (256*1024)//空间长度
#define LENmax_Test       LENmax_Menu//空间长度(同菜单入口和长度)
#endif
#ifdef IC_NUC9xx
	#ifndef LINUX
//      BOOT                 0x00000000	16K
#define ADDR_MS_ENTRY        0x00004000//128K
#define ADDR_OS_ENTRY        0x00024000//768K
#define ADDR_Font_ENTRY      0x000E4000//256K
#define ADDR_Meter_ENTRY     0x00124000//256K
#define ADDR_Terminal_ENTRY  0x00164000//768K
#define ADDR_Menu_ENTRY      0x00224000//256K
#define ADDR_Test_ENTRY      ADDR_Menu_ENTRY//256K(同菜单入口和长度)
														//0x00264000//保留624K
#define LENmax_BOOT       (16*1024)//空间长度
#define LENmax_MS     		(128*1024)//空间长度
#define LENmax_OS     		(768*1024)//空间长度
#define LENmax_Font       (256*1024)//空间长度
#define LENmax_Meter      (256*1024)//空间长度
#define LENmax_Terminal   (768*1024)//空间长度
#define LENmax_Menu       (256*1024)//空间长度
#define LENmax_Test       LENmax_Menu//空间长度(同菜单入口和长度)
	#else//#ifndef LINUX

	#endif
#endif
//版本结束---------------------------------------------------------------------------------


#define LEN_DATABUFF     4096//读写数据缓冲长度,最小4096
#define LEN_128KDATABUFF     (128*1024)



//---终端DL698---------------------------
#define TMaxTariff      4//费率数
#define NAND128kBuff    0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
#define FileID_10x      1//0=否,1=是;方案类型_采集类型_存储时标类型=10x(普通采集方案,采集当前数据)文件标识区分任务ID号
#define FileID_12xD     0//0=否,1=是;方案类型_采集类型_存储时标类型=12x(普通采集方案,按冻结时标采集日冻结)文件标识区分任务ID号
#define FileID_12xM     0//0=否,1=是;方案类型_采集类型_存储时标类型=12x(普通采集方案,按冻结时标采集月冻结)文件标识区分任务ID号
#define FileID_136      0//0=否,1=是;方案类型_采集类型_存储时标类型=136(普通采集方案,按时标间隔采集曲线)文件标识区分任务ID号
#define FileID_2xx      1//0=否,1=是;方案类型_采集类型_存储时标类型=2xx(事件采集方案)文件标识区分任务ID号
#define RecordFILEtoPORT       0//抄表记录文件按端口号记录:0=否,1=是
#define RecordFILEallinFLASH   0//全部普通抄表方案数据保存在nand flash:0=否(只抄OAD时存在RAM),1=是
#define NUM_RMmax    					 1024//最大抄表数
#define NUM_RMLISTmax  		   16//NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
#define RM_HPLC_Concurrency    1//0=否,1=是;宽带载波集中器主动并发抄表
	#if (NUM_RMmax*8)>(128*1024)
		#error //记录文件的地址和数据块长度列表区,列表区长度固定8*NUM_RMmax byte通常会1次读到ADDR_128KDATABUFF抄表标志判断用
	#endif


#define RMTASKmax 			64//最大任务数
#define NUMmax_6012_2   	RMTASKmax//最大抄表任务数
#define NUMmax_6014     	64//最大普通采集方案数
#define NUMmax_6016     	64//最大事件采集方案数
#define NUMmax_6018     	64//最大透明方案数
#define NUMmax_601C     	64//最大上报方案数(此数影响文件管理的最大文件个数)(当前V1.10配置)
#define GPRS_ENET_REPORT 	1//GPRS/ENET主动上报:0=上报分开,1=上报合并(当前V1.10配置)


#define ExtRMTASKmax 			0//扩展任务数
#define RMTASKnoPOWER    	0//停上电事件固定任务号
#define RMovertimeACKfail 0//1//载波路由主动请求抄表大于连续次失败时:0=继续发抄表帧,1=返回抄表失败(跳表)
#define RMSOMETASK    		1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
#define MeterAutoReportFileID_DL645     0x00000005//DL645电表主动上报文件ID:b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
#define MeterAutoReportFileID_DL698     0x21000000//DL698电表主动上报文件ID:b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
#define MeterAutoReportFileID_3011      0x23000000//电能表停电主动上报文件ID:b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
#define EnRS485AutoSearchMeter 		 0//0=否,1=是;允许RS485口自动搜表
#define FORCEiACTIVE  				 		 0//0=否,1=是;强制集中器为主动抄表(与路由抄表模式无关)

#define LENmax_APDU     		2048//最大APDU尺寸
#define LENmax_TxAPDU     	2048//1400//链路层分帧发送每帧最大APDU字节数(若不做链路层分帧则此值置为同LEN_UARTnTx)
#define REPORT_NUMFILEmax   1//16//1帧主动上报数据来自不同文件的最大个数
#define CongealTime_5002    15//分冻结(曲线)间隔时间分

#define NUMmax_Event  			   64//15//最大事件记录数
#define NUMmax_Event3106RM     4//停/上电事件最大抄表个数
#define NUMmax_SWITCHIN    		 4//最大开关量输入路数(门接点)(<=8)
#define NUMmax_IMPIN    		   2//最大脉冲输入路数
#define NUMmax_230x    			   8//最大总加组成员个数
#define NUMmax_TAG    			   8//最大总加组数<=8
#define NUMmax_CR     			   2//最大控制轮次数
#define NUMmax_DC  				     0//最大直流模拟量路数
#define NUMmax_RS485  			   2//最大RS485口数
#define TimerS_UARTRAMFREE     600//600秒,UART分配的内存自动释放延时
#define PARAMETER_601E   	   	0//设置参数601E采集规则库使用:0=内部默认,1=可设置
#define ALLDATANULLSEL     	   0//全部数据空时：0=无返回,1=用空数据0
#define RM_SECURITY_MeterNo     0//0=否,1=是;DL698安全模式抄表时先抄读电表表号 或 ESAM序列号
#define RM_SECURITY_MeterNo_OAD 0xF1000200//DL698安全模式抄表时用于先抄读的OAD,电能表表号=0x40020200,电能表ESAM序列号=0xF1000200
#define RM_SECURITY_CompMAC     1//0=否,1=是,安全模式抄表时进行MAC校验码比较
#define CompareTime_5002    	   0//抄读分钟冻结(采曲线数据)是否比较冻结时间:0=不比较,1=比较
//#define CompareTime_5004     	   0//抄读日冻结是否比较冻结时间:0=不比较,1=比较
#define CompareMeterEventCount     1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
#define CurveTaskTImin     (60*60)//载波口曲线抄表任务执行频率最小值(秒)
#define CurveTaskTImax     (24*60*60)//载波口曲线抄表任务执行频率最大值(秒),此值决定按采集方案采集间隔时间可向前补抄的曲线点数
#define NUMmax_TIDOT     (3*96)//数据招读时最大按TI间隔的数据点数(用于限制数据搜索时间)
#define EnMeterAutoReportEvent     1//允许电能表主动上报事件:0=不允许,1=允许
#define NUM_UART4RxCopyMax     0//32//电能表主动上报接收最大缓存帧数
#define NUMmax_50xx     127//最大冻结对象OI可设置个数据(<=127)
#define SEL_RouterRunMode3ADDR     1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
#define USE_ClientConnectInfo     0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用


#define WaitHeartberatACKMinTimer     30//心跳发出等待主站确认最小定时秒
#define WaitHeartberatACKMaxTimer     180//600//心跳发出等待主站确认最大定时秒
#define WaitHostACKMinTimer     5//主动上报等待主站确认最小定时秒
#define WaitHostACKMaxTimer     120//60//主动上报等待主站确认最大定时秒
#define RouterCheckTimerVal     (120*60)//载波路由器死机秒定时值
#define RMRS485OverTimer     3000//RS485抄表超时定时值ms
#define RS232DefaultBPS     (0xb+(6<<5))//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     (0xb+(6<<5))//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     (0xb+(2<<5))//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...

#define SMSEQ2Byte     1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
#define SM_CMGF     0//短信模式:0=PDU模式,1=文本模式
#define LCDLn9Code     1//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态(0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功)

#define ProtocolNO_ABB     	5//ABB表通信协议编号
#define ProtocolNO_Landis   6//Landis表通信协议编号(IEC1107 模式C)
//---终端结束--------------------------------



//UART------------------------------------------------------------------------------------------
#define RS232PORT     0//RS232通信口号
#define GPRSPORT     1//GPRS通信口号
#define RS485_1PORT     2//RS485_1通信口号
#define RS485_2PORT     3//RS485-2通信口号
#define RS485_4PORT     4//RS485_4通信口号(集中器载波,专变控制模块)
#define RS485_3PORT     5//RS485_3通信口号(WI-FI)
#define IRPORT        6//红外通信口号
#define TESAMPORT     7//终端ESAM通信口号
#define MESAMPORT     8//电表ESAM通信口号
#define CARDPORT     9//电表CPUCARD通信口号
#define ENETPORT     10//以太网通信口号
#define SMSPORT     11//短信通信口号
#define ACSAMPLEPORT     12//交流采样通信口号(虚拟)
#define EFTPCMDPORT     13//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define EFTPDATAPORT     14//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPCMDPORT     15//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPDATAPORT     16//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)


//UART0(RS232)
#define LEN_UART0IntRx     2048//UART0中断接收缓冲长度
#define LEN_UART0Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART0控制数据长度
#define LEN_UART0Rx     2048//UART0接收缓冲长度
#define LEN_UART0Tx     2048//UART0发送缓冲长度
#define LEN_UART0TxMax     (2*LEN_UART0Tx)//UART0发送缓冲预留的最大长度(注:确保LEN_UART0Tx<=LEN_UART0TxMax)
#define LEN_UART0FnDataBuff     8192//UART0代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART1(GPRS)
#define LEN_UART1IntATRx     (LEN_DATABUFF+1024)//UART1中断接收缓冲长度AT命令
#define LEN_UART1IntRx     (LEN_DATABUFF+1024)//UART1中断接收缓冲长度
#define LEN_UART1Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART1控制数据长度
#define LEN_UART1Rx     8192//UART1接收缓冲长度
#define LEN_ATTx     64//AT发送缓冲长度
#define LEN_UART1Tx     2048//UART1发送缓冲长度
#define LEN_UART1TxMax     (2*LEN_UART1Tx)//UART1发送缓冲预留的最大长度(注:确保LEN_UART1Tx<=LEN_UART1TxMax)
#define LEN_UART1FnDataBuff     8192//UART1代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART2(RS485-1)
#define LEN_UART2IntRx     2048//UART2中断接收缓冲长度
#define LEN_UART2Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART2控制数据长度
#define LEN_UART2Rx     2048//UART2接收缓冲长度
#define LEN_UART2Tx     2048//UART2发送缓冲长度
#define LEN_UART2TxMax     (2*LEN_UART2Tx)//UART2发送缓冲预留的最大长度(注:确保LEN_UART2Tx<=LEN_UART2TxMax)
#define LEN_UART2FnDataBuff     8192//UART2抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
//UART3(RS485-2)
#define LEN_UART3IntRx     2048//UART3中断接收缓冲长度
#define LEN_UART3Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART3控制数据长度
#define LEN_UART3Rx     2048//UART3接收缓冲长度
#define LEN_UART3Tx     2048//UART3发送缓冲长度
#define LEN_UART3TxMax     (2*LEN_UART3Tx)//UART3发送缓冲预留的最大长度(注:确保LEN_UART3Tx<=LEN_UART3TxMax)
#define LEN_UART3FnDataBuff     8192//UART3抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
//UART4(RS485-4)(载波)
#define LEN_UART4IntRx     2048//UART4中断接收缓冲长度
#define LEN_UART4Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART4控制数据长度
#define LEN_UART4Rx     2048//UART4接收缓冲长度
#define LEN_UART4Tx     2048//UART4发送缓冲长度
#define LEN_UART4TxMax     2048//UART4发送缓冲预留的最大长度(注:确保LEN_UART4Tx<=LEN_UART4TxMax)
	#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
#define LEN_UART4FnDataBuff     8192//UART4抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
	#else
#define LEN_UART4FnDataBuff     2560//UART4抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
	#endif
//UART5(RS485-3)(WI-FI)
#define LEN_UART5IntRx     8//UART5中断接收缓冲长度
#define LEN_UART5Ctrl     0//UART5控制数据长度
#define LEN_UART5Rx     0//UART5接收缓冲长度
#define LEN_UART5Tx     0//UART5发送缓冲长度
#define LEN_UART5TxMax     0//(2048+64)//UART5发送缓冲预留的最大长度(注:确保LEN_UART5Tx<=LEN_UART5TxMax)
#define LEN_UART5FnDataBuff     0//UART5抄表Fn数据缓冲长度
//UART6(红外)
#define LEN_UART6IntRx     2048//UART6中断接收缓冲长度
#define LEN_UART6Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART6控制数据长度
#define LEN_UART6Rx     2048//UART6接收缓冲长度
#define LEN_UART6Tx     2048//UART6发送缓冲长度
#define LEN_UART6TxMax     (2*LEN_UART6Tx)//UART6发送缓冲预留的最大长度(注:确保LEN_UART6Tx<=LEN_UART6TxMax)
#define LEN_UART6FnDataBuff     2048//UART6代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART7(终端ESAM)
#define LEN_UART7IntRx     8//UART7中断接收缓冲长度
#define LEN_UART7Ctrl     1024//UART7控制数据长度
#define LEN_UART7Rx     2048//UART7接收缓冲长度
#define LEN_UART7Tx     2048//UART7发送缓冲长度
#define LEN_UART7TxMax     2048//UART7发送缓冲预留的最大长度(注:确保LEN_UART7Tx<=LEN_UART7TxMax)
#define LEN_UART7FnDataBuff     0//UART7抄表Fn数据缓冲长度
//UART8(电表ESAM)
#define LEN_UART8IntRx     8//UART8中断接收缓冲长度
#define LEN_UART8Ctrl     0//UART8控制数据长度
#define LEN_UART8Rx     0//UART8接收缓冲长度
#define LEN_UART8Tx     0//UART8发送缓冲长度
#define LEN_UART8TxMax     0//UART8发送缓冲预留的最大长度(注:确保LEN_UART8Tx<=LEN_UART8TxMax)
#define LEN_UART8FnDataBuff     0//UART8抄表Fn数据缓冲长度
//UART9(电表CPUCARD)
#define LEN_UART9IntRx     8//UART9中断接收缓冲长度
#define LEN_UART9Ctrl     0//UART9控制数据长度
#define LEN_UART9Rx     0//UART9接收缓冲长度
#define LEN_UART9Tx     0//UART9发送缓冲长度
#define LEN_UART9TxMax     0//UART9发送缓冲预留的最大长度(注:确保LEN_UART9Tx<=LEN_UART9TxMax)
#define LEN_UART9FnDataBuff     0//UART9抄表Fn数据缓冲长度
//UART10(Ethernet)
#define LEN_UART10IntRx     8192//UART10中断接收缓冲长度
#define LEN_UART10Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART10控制数据长度
#define LEN_UART10Rx     8192//UART10接收缓冲长度
#define LEN_UART10Tx     2048//UART10发送缓冲长度
#define LEN_UART10TxMax     (2*LEN_UART10Tx)//UART10发送缓冲预留的最大长度(注:确保LEN_UART10Tx<=LEN_UART10TxMax)
#define LEN_UART10FnDataBuff     8192//UART10代理转发数据缓冲+主动上报帧缓冲，长度>=2*LEN_UARTnTx
//UART11(SMS)
#define LEN_UART11IntRx     8//UART11中断接收缓冲长度
#define LEN_UART11Ctrl     0//UART11控制数据长度
#define LEN_UART11Rx     0//UART11接收缓冲长度
#define LEN_UART11Tx     0//UART11发送缓冲长度
#define LEN_UART11TxMax     0//UART11发送缓冲预留的最大长度(注:确保LEN_UART11Tx<=LEN_UART11TxMax)
#define LEN_UART11FnDataBuff     0//UART11抄表Fn数据缓冲长度
//UART12(虚拟口)
#define LEN_UART12IntRx     8//UART12中断接收缓冲长度
#define LEN_UART12Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART12控制数据长度
#define LEN_UART12Rx     2048//UART12接收缓冲长度
#define LEN_UART12Tx     2048//UART12发送缓冲长度
#define LEN_UART12TxMax     2048//UART12发送缓冲预留的最大长度(注:确保LEN_UART12Tx<=LEN_UART12TxMax)
#define LEN_UART12FnDataBuff     8192//UART12抄表Fn数据缓冲长度

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART13IntRx     (10+1024+2)//UART13中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART13Ctrl     1024//UART13控制数据长度
#define LEN_UART13Rx     0//UART13接收缓冲长度
#define LEN_UART13Tx     1024//UART13发送缓冲长度
#define LEN_UART13TxMax     1024//UART13发送缓冲预留的最大长度(注:确保LEN_UART13Tx<=LEN_UART13TxMax)
#define LEN_UART13FnDataBuff     0//UART13抄表Fn数据缓冲长度
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART14IntRx     (10+4096+2)//UART14中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART14Ctrl     1024//UART14控制数据长度
#define LEN_UART14Rx     0//UART14接收缓冲长度
#define LEN_UART14Tx     2048//UART14发送缓冲长度
#define LEN_UART14TxMax     2048//UART14发送缓冲预留的最大长度(注:确保LEN_UART14Tx<=LEN_UART14TxMax)
#define LEN_UART14FnDataBuff     0//UART14抄表Fn数据缓冲长度
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART15IntRx     8//UART15中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART15Ctrl     0//UART15控制数据长度
#define LEN_UART15Rx     0//UART15接收缓冲长度
#define LEN_UART15Tx     0//UART15发送缓冲长度
#define LEN_UART15TxMax     0//UART15发送缓冲预留的最大长度(注:确保LEN_UART15Tx<=LEN_UART15TxMax)
#define LEN_UART15FnDataBuff     0//UART15抄表Fn数据缓冲长度
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART16IntRx     8//UART16中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART16Ctrl     0//UART16控制数据长度
#define LEN_UART16Rx     0//UART16接收缓冲长度
#define LEN_UART16Tx     0//UART16发送缓冲长度
#define LEN_UART16TxMax     0//UART16发送缓冲预留的最大长度(注:确保LEN_UART16Tx<=LEN_UART16TxMax)
#define LEN_UART16FnDataBuff     0//UART16抄表Fn数据缓冲长度



#define SocketPortnMin     1025//本机端口号最小值
#define SocketPortnMax     65535//本机端口号最大值
//UARTEND---------------------------------------------------------------------------------------




//电能表----------------------------------------------------------------------------------------
#define iMeterProtocol  2//内部交采抄表规约:2=DL/T645-2007，3=DL/T698.45
#define MMaxTariff     	4//费率数
#define MeterHarmonic   0//谐波计算0=没,1=有
#define MeterMaxLadder  6//阶梯数
#define MaxMonth     		12//月冻结数
#define MaxMeterCurveCongeal     (96*10)//最大曲线点数,60的整数倍(搜索时跳越用)
#define MeterMaxDemandCalculate  1//最大需量计算：0=脉冲法,1=功率平均法

#define MeterCtrl     0//计费控制:0无控制,1仅远程,2本地和远程
#define MeterType     0//表计类型:0非预付费表,1电量型预付费表,2电费型预付费表
#define MeterSpec     20//电能表规格
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
//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
//MeterSpec=20 自适应:0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh

#if MeterSpec==0
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.31//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==1
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     1//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     1000//参比电压xxx.x(V)
#define MeterUBbcd     0x1000//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.14//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==2
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.33//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi    0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==3
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.31//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==4
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     1//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     1000//参比电压xxx.x(V)
#define MeterUBbcd     0x1000//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.14//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==5
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.33//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif


#if MeterSpec==16
//MeterSpec=16 0.5S 3P4W 50Hz 6400imp/kWh 220V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     30//参比电流xx.xx(A)
#define MeterIBbcd     0x030//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.31//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==17
//MeterSpec=17 0.5S 3P3W 50Hz 20000imp/kWh 100V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     1//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     1000//参比电压xxx.x(V)
#define MeterUBbcd     0x1000//参比电压xxx.x(V)
#define MeterIBhex     30//参比电流xx.xx(A)
#define MeterIBbcd     0x030//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.14//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==18
//MeterSpec=18 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     30//参比电流xx.xx(A)
#define MeterIBbcd     0x030//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.33//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.068//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==19
//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     3200//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x3200//电表有功常数imp/kWh
#define MeterReactiveConst     3200//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x3200//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     100//参比电流xx.xx(A)
#define MeterIBbcd     0x100//参比电流xx.xx(A)
#define MeterIMaxhex     8000//最大电流xx.xx(A)
#define MeterIMaxbcd     0x8000//最大电流xx.xx(A)
#define MeterPStartupHex     ((MeterUBhex*MeterIBhex*0.00066)/2)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     ((MeterUBhex*MeterIBhex*0.00066)/2)//无功起动功率xxxxx.xxx(var)
#define ADC_Vu     0.14//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.033//In时电流ADC输入有效值(ATT7022用)
#endif
#if MeterSpec==20//自动适应电能表规格
//MeterSpec=20 0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
//#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     10000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x10000//电表有功常数imp/kWh
#define MeterReactiveConst     10000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x10000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)(用最小电压标称值主要是失压事件等,非校验原因)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     100//参比电流xx.xx(A)(用最小电压标称值主要是失流事件等,非校验原因)
#define MeterIBbcd     0x100//参比电流xx.xx(A)
#define MeterIMaxhex     8000//最大电流xx.xx(A)(用最大电流标称值是校验电流计算原因)
#define MeterIMaxbcd     0x8000//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)

#define ADC_Vu     0.14//Un时电压ADC输入有效值(ATT7022用)
#define ADC_Vi     0.033//In时电流ADC输入有效值(ATT7022用)
#define MeterActiveConst220V     3200//220V电表有功常数imp/kWh
#define MeterActiveConst100V     10000//100V电表有功常数imp/kWh
#define MeterActiveConst57V     10000//57.7V电表有功常数imp/kWh
#endif
//电能表结束------------------------------------------------------------------------------------












//硬件版本--------------------------------------------------------------------------------------
#if (HVER==0x410)//硬件版本号4.10
#define IC_STM32F42x     1//0=没,1=有
#define IC_STM32F46x     0//0=没,1=有
#define IC_NUC972     0//0=没,1=有
#define IC_NUC975     0//0=没,1=有
#define HCLK     168000000//32000000//84000000//168000000//系统时钟(Hz):168,84,32
#define LCDPixels     3//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE

#define IC_MT29F1G     0//0=没,1=有(128 Mbyte)
#define IC_MT29F2G     1//0=没,1=有(256 Mbyte)
#define IC_MT29F4G     0//0=没,1=有(512 Mbyte)
#define IC_MT29F8G     0//0=没,1=有(1024 Mbyte)
#define IC_S34ML1G     0//0=没,1=有(128 Mbyte)
#define IC_S34ML2G     0//0=没,1=有(256 Mbyte)
#define IC_S34ML4G     0//0=没,1=有(512 Mbyte)
#define IC_S34ML8G     0//0=没,1=有(512 Mbyte)
#define NAND_BUS_WIDTH     8//仅OS和TEST需要,其他程序自动识别;8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
#define NAND_1G     1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
#define NAND_2G     1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
#define NAND_4G     1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
#define NAND_8G     1
#endif
#ifdef  NAND_1G
#define NAND_BLOCK_COUNT     1024//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_2G
#define NAND_BLOCK_COUNT     2048//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_4G
#define NAND_BLOCK_COUNT     4096//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_8G
#define NAND_BLOCK_COUNT     8192//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     250//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#if NAND_REPLACE_BLOCK_COUNT>250//1-250(0xFA)=有替换的替换区的替换块号,0xFB=保留,0xFC=保留,0xFD=未损坏(无替换),0xFE=块损坏(无替换),(0x00,0xFF可能片子出厂初始值禁用),其他值=表示未初始化
#error NAND_REPLACE_BLOCK_COUNT>250
#endif

#define IC_MT48LC4M16     0//0=没,1=有(8 Mbyte)
#define IC_MT48LC8M16     1//0=没,1=有(16 Mbyte)
#define IC_MT48LC16M16     0//0=没,1=有(32 Mbyte)
#define IC_MT48LC32M16     0//0=没,1=有(64 Mbyte)
#define IC_MT48LC8M8     0//0=没,1=有(8 Mbyte)
#define IC_MT48LC16M8     0//0=没,1=有(16 Mbyte)
#define IC_MT48LC32M8     0//0=没,1=有(32 Mbyte)
#define IC_MT48LC64M8     0//(A11不能配置暂不选用)0=没,1=有(64 Mbyte)
#define IC_SDRAM     (IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      1//0=没,1=有
#define IC_DS3231     0//0=没,1=有

#define IC_LAN8720     1//0=没,1=有
#define IC_90E36     1//0=没,1=有
#define IC_ATT7022     0//0=没,1=有

#define IC_UC1698     1//0=没,1=有(点阵显示)
#define IC_ST75161     0//0=没,1=有(点阵显示)
#define IC_PCF8562     0//0=没,1=有(段式显示)
#define IC_CPUCARD     0//0=没,1=有
#define IC_MESAM     0//0=没,1=有
#define IC_TESAM     1//0=没,1=有
#define IC_24AAxxx     1//0=没,1=有
#define LOADSWITCH     0//智能表项目的负荷开关0=内部,1=外部
#define IC_BGS2     0//GPRS模块BGS2;0=没,1=有
#define IC_GPRS3763     1//GPRS模块376.3标准;0=没,1=有
#define IC_MC323     1//CDMA华为MC323模块;0=没,1=有
#define KEY_SET     0//设置按键0=没,1=有

#define MaxFileNum     8//最大下载文件个数
//生产目录文件名
	#define FileFactory_PACK          "PACK698.*"
	#define FileFactory_MS            "MS698.*"
	#define FileFactory_OS            "OS698.*"
	#define FileFactory_Meter         "Meter698.*"
	#define FileFactory_Terminal      "Terminal698.*"
	#define FileFactory_Menu          "Menu698.*"
	#define FileFactory_Font          "Font.*"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test698.*"
	#define FileFactory_Modele        "Module698.*"
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
#define FileUserDIR          "/JK1600"
#if MeterSpec==0//电能表规格//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK698_JK1600_000_V41.*"
	#define FileUser_MS        "MS698_JK1600_000_V41.*"
	#define FileUser_OS        "OS698_JK1600_000_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_000_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_000_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_000_V41.*"
	#define FileUser_Font      "Font698_JK1600_000_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_000_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_000_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_000_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_000_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_000_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_000_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_000_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_000_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_000_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_000_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_000_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_000_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_000_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_000_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_000_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_000_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_000_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_000_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_000_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_000_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_000_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_000_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_000_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_000_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_000_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_000_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_000_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_000_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_000_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_000_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_000_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_000_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_000_V41_SR.zip"
#endif

#if MeterSpec==13//电能表规格//MeterSpec=13 0.5S 3P4W 50Hz 6400imp/kWh 220V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1300_V41.*"
	#define FileUser_MS        "MS698_JK1600_1300_V41.*"
	#define FileUser_OS        "OS698_JK1600_1300_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1300_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1300_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1300_V41.*"
	#define FileUser_Font      "Font698_JK1600_1300_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1300_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1300_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1300_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1300_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1300_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1300_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1300_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1300_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1300_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1300_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1300_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1300_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1300_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_1300_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1300_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1300_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1300_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1300_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1300_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1300_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1300_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1300_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1300_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1300_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1300_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1300_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1300_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1300_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1300_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1300_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1300_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1300_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1300_V41_SR.zip"
#endif
#if MeterSpec==14//电能表规格//MeterSpec=14 0.5S 3P3W 50Hz 20000imp/kWh 100V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1400_V41.*"
	#define FileUser_MS        "MS698_JK1600_1400_V41.*"
	#define FileUser_OS        "OS698_JK1600_1400_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1400_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1400_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1400_V41.*"
	#define FileUser_Font      "Font698_JK1600_1400_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1400_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1400_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1400_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1400_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1400_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1400_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1400_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1400_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1400_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1400_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1400_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1400_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1400_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_1400_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1400_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1400_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1400_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1400_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1400_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1400_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1400_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1400_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1400_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1400_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1400_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1400_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1400_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1400_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1400_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1400_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1400_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1400_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1400_V41_SR.zip"
#endif
#if MeterSpec==15//电能表规格//MeterSpec=15 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1500_V41.*"
	#define FileUser_MS        "MS698_JK1600_1500_V41.*"
	#define FileUser_OS        "OS698_JK1600_1500_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1500_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1500_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1500_V41.*"
	#define FileUser_Font      "Font698_JK1600_1500_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1500_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1500_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1500_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1500_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1500_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1500_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1500_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1500_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1500_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1500_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1500_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1500_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1500_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_1500_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1500_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1500_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1500_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1500_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1500_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1500_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1500_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1500_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1500_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1500_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1500_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1500_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1500_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1500_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1500_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1500_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1500_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1500_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1500_V41_SR.zip"
#endif
#if MeterSpec==19//电能表规格//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
	#define FileUser_PACK      "PACK698_JK1600_1900_V41.*"
	#define FileUser_MS        "MS698_JK1600_1900_V41.*"
	#define FileUser_OS        "OS698_JK1600_1900_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1900_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1900_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1900_V41.*"
	#define FileUser_Font      "Font698_JK1600_1900_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1900_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1900_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1900_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1900_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1900_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1900_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1900_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1900_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1900_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1900_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1900_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1900_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1900_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_1900_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1900_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1900_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1900_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1900_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1900_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1900_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1900_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1900_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1900_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1900_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1900_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1900_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1900_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1900_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1900_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1900_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1900_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1900_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1900_V41_SR.zip"
#endif
#if MeterSpec==20//电能表规格///MeterSpec=20 0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh
	#define FileUser_PACK      "PACK698_JK1600_2000_V41.*"
	#define FileUser_MS        "MS698_JK1600_2000_V41.*"
	#define FileUser_OS        "OS698_JK1600_2000_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_2000_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_2000_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_2000_V41.*"
	#define FileUser_Font      "Font698_JK1600_2000_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_2000_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_2000_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_2000_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_2000_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_2000_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_2000_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_2000_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_2000_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_2000_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_2000_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_2000_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_2000_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_2000_V41_SR.*"
//FTP用文件名
	#define FileFTP_PACK      "PACK698_JK1600_2000_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_2000_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_2000_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_2000_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_2000_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_2000_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_2000_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_2000_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_2000_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_2000_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_2000_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_2000_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_2000_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_2000_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_2000_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_2000_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_2000_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_2000_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_2000_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_2000_V41_SR.zip"
#endif
#endif
//硬件版本4.10结束----------------------------------------------------------------------------------

//硬件版本--------------------------------------------------------------------------------------
#if (HVER==0x500)//硬件版本号5.00
#define IC_STM32F42x     0//0=没,1=有
#define IC_STM32F46x     0//0=没,1=有
#define IC_NUC972     1//0=没,1=有
#define IC_NUC975     0//0=没,1=有
#define HCLK     			168000000//84000000//32000000//系统时钟(Hz):168,84,32
#define LCDPixels     3//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
#define SuperCapacitorChargingTime     10//超级电容充电时间(秒)(用于掉电时是否进入数据保存)

#define IC_MT29F1G     0//0=没,1=有(128 Mbyte)
#define IC_MT29F2G     1//0=没,1=有(256 Mbyte)
#define IC_MT29F4G     0//0=没,1=有(512 Mbyte)
#define IC_MT29F8G     0//0=没,1=有(1024 Mbyte)
#define IC_S34ML1G     0//0=没,1=有(128 Mbyte)
#define IC_S34ML2G     0//0=没,1=有(256 Mbyte)
#define IC_S34ML4G     0//0=没,1=有(512 Mbyte)
#define IC_S34ML8G     0//0=没,1=有(512 Mbyte)
#define NAND_BUS_WIDTH     8//仅OS和TEST需要,其他程序自动识别;8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
#define NAND_1G     1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
#define NAND_2G     1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
#define NAND_4G     1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
#define NAND_8G     1
#endif
#ifdef  NAND_1G
#define NAND_BLOCK_COUNT     1024//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_2G
#define NAND_BLOCK_COUNT     2048//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_4G
#define NAND_BLOCK_COUNT     4096//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#ifdef  NAND_8G
#define NAND_BLOCK_COUNT     8192//NANDFLASH总有块数(每块128K,每页2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH用于文件件系统的块数(修改此值同时要修改文件配置头文件中的值)
#define NAND_REPLACE_BLOCK_COUNT     250//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH用于替换的块数(紧接文件系统后)
#endif
#if NAND_REPLACE_BLOCK_COUNT>250//1-250(0xFA)=有替换的替换区的替换块号,0xFB=保留,0xFC=保留,0xFD=未损坏(无替换),0xFE=块损坏(无替换),(0x00,0xFF可能片子出厂初始值禁用),其他值=表示未初始化
#error NAND_REPLACE_BLOCK_COUNT>250
#endif

#define IC_MT48LC4M16     0//0=没,1=有(8 Mbyte)
#define IC_MT48LC8M16     0//0=没,1=有(16 Mbyte)
#define IC_MT48LC16M16     0//0=没,1=有(32 Mbyte)
#define IC_MT48LC32M16     1//0=没,1=有(64 Mbyte)
#define IC_MT48LC8M8     0//0=没,1=有(8 Mbyte)
#define IC_MT48LC16M8     0//0=没,1=有(16 Mbyte)
#define IC_MT48LC32M8     0//0=没,1=有(32 Mbyte)
#define IC_MT48LC64M8     0//(A11不能配置暂不选用)0=没,1=有(64 Mbyte)
#define IC_SDRAM     (IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      1//0=没,1=有
#define IC_DS3231     0//0=没,1=有

#define IC_IP101     1//0=没,1=有(eth0)
#define IC_LAN8720     1//0=没,1=有(eth1)
#define RJ45SELECT     0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
#define IC_90E36     1//0=没,1=有
#define IC_ATT7022     0//0=没,1=有

#define IC_UC1698     1//0=没,1=有(点阵显示)
#define IC_ST75161     0//0=没,1=有(点阵显示)
#define IC_PCF8562     0//0=没,1=有(段式显示)
#define IC_CPUCARD     0//0=没,1=有
#define IC_MESAM     0//0=没,1=有
#define IC_TESAM     1//0=没,1=有
#define IC_24AAxxx     1//0=没,1=有
#define LOADSWITCH     0//智能表项目的负荷开关0=内部,1=外部
#define IC_BGS2     0//GPRS模块BGS2;0=没,1=有
#define IC_GPRS3763     1//GPRS模块376.3标准;0=没,1=有
#define IC_MC323     1//CDMA华为MC323模块;0=没,1=有
#define KEY_SET     0//设置按键0=没,1=有

#define MaxFileNum     8//最大下载文件个数
//生产目录文件名
	#define FileFactory_PACK          "PACK698N.zip"
	#define FileFactory_MS            "MS698N.zip"
	#define FileFactory_OS            "OS698N.zip"
	#define FileFactory_Meter         "Meter698N.zip"
	#define FileFactory_Terminal      "Terminal698N.zip"
	#define FileFactory_Menu          "Menu698N.zip"
	#define FileFactory_Font          "FontN.zip"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test698N.zip"
	#define FileFactory_Modele        "Module698N.zip"
	#define FileFactory_PLC_50        "PLC_50.zip"
	#define FileFactory_PLC_TC        "PLC_TC.zip"
	#define FileFactory_PLC_ES        "PLC_ES.zip"
	#define FileFactory_PLC_CX        "PLC_CX.zip"
	#define FileFactory_PLC_01        "PLC_01.zip"
	#define FileFactory_PLC_LH        "PLC_LH.zip"
	#define FileFactory_PLC_NX        "PLC_NX.zip"
	#define FileFactory_PLC_QL        "PLC_QL.zip"
	#define FileFactory_PLC_MX        "PLC_MX.zip"
	#define FileFactory_PLC_FC        "PLC_FC.zip"
	#define FileFactory_PLC_NW        "PLC_NW.zip"
	#define FileFactory_PLC_SR        "PLC_SR.zip"

//用户目录全名
#define FileUserDIR          "/JK1600"
#if MeterSpec==20//自动适应电能表规格
	#define FileUser_PACK      "PACK698N_JK1600_2000_V50.zip"
	#define FileUser_MS        "MS698N_JK1600_2000_V50.zip"
	#define FileUser_OS        "OS698N_JK1600_2000_V50.zip"
	#define FileUser_Meter     "Meter698N_JK1600_2000_V50.zip"
	#define FileUser_Terminal  "Terminal698N_JK1600_2000_V50.zip"
	#define FileUser_Menu      "Menu698N_JK1600_2000_V50.zip"
	#define FileUser_Font      "FontN_JK1600_2000_V50.zip"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698N_JK1600_2000_V50.zip"
	#define FileUser_PLC_50    "PLC_JK1600_2000_V50_50.zip"
	#define FileUser_PLC_TC    "PLC_JK1600_2000_V50_TC.zip"
	#define FileUser_PLC_ES    "PLC_JK1600_2000_V50_ES.zip"
	#define FileUser_PLC_CX    "PLC_JK1600_2000_V50_CX.zip"
	#define FileUser_PLC_01    "PLC_JK1600_2000_V50_01.zip"
	#define FileUser_PLC_LH    "PLC_JK1600_2000_V50_LH.zip"
	#define FileUser_PLC_NX    "PLC_JK1600_2000_V50_NX.zip"
	#define FileUser_PLC_QL    "PLC_JK1600_2000_V50_QL.zip"
	#define FileUser_PLC_MX    "PLC_JK1600_2000_V50_MX.zip"
	#define FileUser_PLC_FC    "PLC_JK1600_2000_V50_FC.zip"
	#define FileUser_PLC_NW    "PLC_JK1600_2000_V50_NW.zip"
	#define FileUser_PLC_SR    "PLC_JK1600_2000_V50_SR.zip"
//FTP用文件名
	#define FileFTP_PACK      "PACK698N_JK1600_2000_V50.zip"
	#define FileFTP_MS        "MS698N_JK1600_2000_V50.zip"
	#define FileFTP_OS        "OS698N_JK1600_2000_V50.zip"
	#define FileFTP_Meter     "Meter698N_JK1600_2000_V50.zip"
	#define FileFTP_Terminal  "Terminal698N_JK1600_2000_V50.zip"
	#define FileFTP_Menu      "Menu698N_JK1600_2000_V50.zip"
	#define FileFTP_Font      "FontN_JK1600_2000_V50.zip"
	#define FileFTP_MODULE    "Module698N_JK1600_2000_V50.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_2000_V50_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_2000_V50_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_2000_V50_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_2000_V50_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_2000_V50_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_2000_V50_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_2000_V50_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_2000_V50_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_2000_V50_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_2000_V50_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_2000_V50_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_2000_V50_SR.zip"
#endif
#endif
//硬件版本5.00结束----------------------------------------------------------------------------------









#endif






























