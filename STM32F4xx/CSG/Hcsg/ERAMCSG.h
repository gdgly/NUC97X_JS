


#ifndef ERAM_H
#define ERAM_H


#ifdef IC_STM32F4xx
#define LEN_ERAM_BUFF_UART_TIMER     (2*1024*1024)		// 数据缓冲区和UART和定时器等系统数据总长度
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
#define LEN_ERAM_BUFF_UART_TIMER     ((4*1024*1024)+(2*LEN_msFILEmanage_Ext))//数据缓冲区和UART和定时器等系统数据总长度

//0x00000000-0x003FFFFF 4M-512-16byte Program
#define ADDR_ETHMAC     (0x00400000-512-16)//以太网MAC地址
//6byte MACADDR0
//6byte MACADDR1
//4byte 
#define ADDR_IrqHandlerTable     (0x00400000-512)//注:关联OS LINK
//256byte IRQ中断表
#define ADDR_FiqHandlerTable     (0x00400000-256)//注:关联OS LINK
//256byte FIQ中断表

//0x00400000-0x0043FFFF //256K MS Stack and Heap
//0x00440000-0x0047FFFF //256K ETH1
//0x00480000-0x00AFFFFF //7M-512K OS Heap
//0x00B00000-0x00BFFFFF //1M OS Stack

#define ADDR_SPI_MAP     0x00C00000
#define LEN_SPI_MAP     0x00080000
//0x00C00000-0x00C7FFFF 512K IEEPROM(SPIFLASH) 映射
#define ADDR_NAND_MAP     0x00C80000
#define LEN_NAND_MAP     0x00280000
//0x00C80000-0x00EFFFFF 2.5M NAND_FLASH 映射
//0x00F00000-0x00FFFFFF 1M ISRAM
#define ADDR_ERAM_START     0x01000000
#endif


#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
// LEN_SaveRam BYTE 从SRAM复制的掉电保护数据,上电时不清0故放在最前以便上电时其他数据清0地址计算

#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#ifdef  NAND_1G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(16*1024)									// msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)		// 存储在FLASH中的文件的管理单元字节数
#endif
#ifdef  NAND_2G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(16*1024)									// msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)		//存储在FLASH中的文件的管理单元字节数 (256-32)*1024/16 = 1433.6 最大文件单元数1433
#endif
#ifdef  NAND_4G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(32*1024)  									// msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)		//存储在FLASH中的文件的管理单元字节数
#endif
#ifdef  NAND_8G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(32*1024)									// msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)	// 存储在FLASH中的文件的管理单元字节数
#endif
#if LEN_msFILEunltFlags >= LEN_msFILEmanage
	#error
#endif
// 8192*2byte 块号:0=未使用,1=占用但无下块块号,2-8191下块的块号
// LEN_msFILEmanage-标志表-12字节 文件入口表
	// 文件入口表1
	// ...
	// 文件入口表n
// 12BYTE
	// 4byte 文件分配单元计数
	// 4byte有效标识0x12345678
	// 4byte CRC32


// 256K 复制(当前仅用于手动强制复位时数据恢复用)

typedef __packed struct
{
	u16 BLOCK_ENTRY;		//文件入口:nand flash块号,0=空;RAM 开始地址高2字节(值有效时!=0)
	u16 BLOCK_END;			//文件结束:nand flash块号,0=空;RAM 开始地址低2字节
	u32 LEN_FILE;			//文件预留占有存储空间总长度
	u32 LEN_DATA;			//文件有效数据总长度
	
	u32 FILEID;				//FILEID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	// 文件分类:
	// 0=普通采集方案数据记录
	// 1=
	// 2=按OI分类的事件记录
	//
	//
	u32 FILECOUNT;			//相同FILEID的文件计数号
	u16 NUM_DATA;			//文件存入数据个数计数(同抄表成功个数)	//存储了多少个表
	u16 NUM_FAIL;			//文件头中写入抄表失败地址个数
	u8 date_time_s[7];		//采集存储时标
	u8 res1;				//仅为对齐
	
	u16 NUM_GPRSreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
	u16 NUM_ENETreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
}msFILE_TypeDef;

#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)	// 128K(BKPSDRAM) NAND FLASH写缓冲(仅在数据检查错误时清0)
// 2BYTE 总块数
// 4BYTE 总字节数(全部字节,也即下块的开始地址偏移)
// 数据块1
// 4BYTE 写入NAND FLASH的目的地址
// 4BYTE 数据字节数
// 4BYTE 已部分写入的字节数
// nBYTE 数据
// ...
// 数据块n
typedef __packed struct
{
	u32 ADDR_Dest;		// 4BYTE 写入NAND FLASH的目的地址
	u32 LEN_Data;		// 4BYTE 数据字节数
	u32 LEN_Write;		// 4BYTE 已部分写入的字节数
	u8 Data[1];			// nBYTE 数据
}BKPSDRAM_BLOCK_TypeDef;
typedef __packed struct
{
	u16 NUM_BLOCK;		// 2BYTE 总块数
	u32 LEN_TOTAL;		// 4BYTE 总字节数(全部字节,也即下块的开始地址偏移)
	//数据块
}BKPSDRAM_TypeDef;
#define ADDR_SDRAM_CLR_START	(ADDR_BKPSDRAM+(128*1024))	// SDRAM数据清0开始地址


#define ADDR_RAMmsFILEmanage    ADDR_SDRAM_CLR_START		// 存储在RAM中的文件管理
#define LEN_RAMmsFILEmanage     (4*1024)
#define LEN_RAMmsFILEblock     	(32*1024)					// RAMmsFILE文件块长度


#define ADDR_emWin     			ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
// 128K emWin编译区
#define ADDR_128KDATABUFF     	(ADDR_emWin+(128*1024))
// 128K DATABUFF
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
// 128K字节NAND FLASH块写缓冲
#define ADDR_NAND_PAGEREAD_BUFF (ADDR_128KWRITE_BUFF+(128*1024))
// 2048字节NAND FLASH页读缓冲
#define ADDR_ECC_BUFF     		(ADDR_NAND_PAGEREAD_BUFF+2048)
// 128+128字节ECC缓冲

#define ADDR_MACADDR_ascii     	(ADDR_ECC_BUFF+256)						//本机MAC地址的ascii码
// 18BYTE 格式"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     	(ADDR_MACADDR_ascii+18)					//本机IP4地址的ascii码
// 16BYTE 格式"192.168.0.100"
#define ADDR_IP4MASK_ascii     	(ADDR_IP4ADDR_ascii+16)					//本机IP4 MASK地址的ascii码
// 16BYTE 格式"255.255.255.0"
#define ADDR_IP4GATEWAY_ascii   (ADDR_IP4MASK_ascii+16)					//本机IP4 GATEWAY地址的ascii码
// 16BYTE 格式"192.168.0.254"
#define ADDR_IP4PRIMARY_DNS_ascii     	(ADDR_IP4GATEWAY_ascii+16)		//本机IP4 PRIMARY_DNS的ascii码
// 16BYTE 格式"8.8.8.8"
#define ADDR_IP4SECONDARY_DNS_ascii   	(ADDR_IP4PRIMARY_DNS_ascii+16)	//本机IP4 SECONDARY_DNS的ascii码
// 16BYTE 格式"8.8.4.4"
#define ADDR_IP6ADDR_ascii     			(ADDR_IP4SECONDARY_DNS_ascii+16)	//本机IP6地址的ascii码
// 40BYTE
#define ADDR_IP6GATEWAY_ascii     		(ADDR_IP6ADDR_ascii+40)				//本机IP6 GATEWAY地址的ascii码
// 40BYTE
#define ADDR_IP6PRIMARY_DNS_ascii     	(ADDR_IP6GATEWAY_ascii+40)			//本机IP6 PRIMARY_DNS的ascii码
// 40BYTE
#define ADDR_IP6SECONDARY_DNS_ascii     (ADDR_IP6PRIMARY_DNS_ascii+40)		//本机IP6 SECONDARY_DNS的ascii码
// 40BYTE


//#define ADDR_UARTnCtrl_Start     	(ADDR_IP6SECONDARY_DNS_ascii+40)//UART
//20190513 此处使后面的数据地址4字节对齐
#define ADDR_UARTnCtrl_Start		((ADDR_IP6SECONDARY_DNS_ascii + 40 + 3) & 0xfffffffc)
//UART0
#define ADDR_UART0     				(ADDR_UARTnCtrl_Start)
#define ADDR_UART0IntRx     		ADDR_UART0
#define ADDR_UART0Ctrl     			(ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     			(ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     			(ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     	(ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     				(ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     		ADDR_UART1
#define ADDR_UART1IntRx     		(ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     			(ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     			(ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     				(ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     			(ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     	(ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#define ADDR_UART2     				(ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#define ADDR_UART2IntRx     		ADDR_UART2
#define ADDR_UART2Ctrl     			(ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     			(ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     			(ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     	(ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     				(ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     		ADDR_UART3
#define ADDR_UART3Ctrl     			(ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     			(ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     			(ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     	(ADDR_UART3Tx+LEN_UART3TxMax)
//UART4
#define ADDR_UART4     				(ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#define ADDR_UART4IntRx     		ADDR_UART4
#define ADDR_UART4Ctrl     			(ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     			(ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     			(ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     	(ADDR_UART4Tx+LEN_UART4TxMax)
//UART5
#define ADDR_UART5     				(ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)
#define ADDR_UART5IntRx     		ADDR_UART5
#define ADDR_UART5Ctrl     			(ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     			(ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     			(ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     	(ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     				(ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     		ADDR_UART6
#define ADDR_UART6Ctrl     			(ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     			(ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx    	 		(ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     	(ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(终端ESAM)
#define ADDR_UART7     				(ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     		ADDR_UART7
#define ADDR_UART7Ctrl     			(ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     			(ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     			(ADDR_UART7Rx+LEN_UART7Rx)
#define ADDR_UART7FnDataBuff     	(ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(电表ESAM)
#define ADDR_UART8     				(ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     		ADDR_UART8
#define ADDR_UART8Ctrl     			(ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx    	 		(ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     			(ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     	(ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(电表CARD)
#define ADDR_UART9     				(ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     		ADDR_UART9
#define ADDR_UART9Ctrl     			(ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     			(ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     			(ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff    	(ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(以太网)
#define ADDR_UART10     			(ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     		ADDR_UART10
#define ADDR_UART10Ctrl     		(ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     			(ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     			(ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     	(ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     			(ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     		ADDR_UART11
#define ADDR_UART11Ctrl     		(ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     			(ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     			(ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     	(ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(虚拟口)
#define ADDR_UART12     			(ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     		ADDR_UART12
#define ADDR_UART12Ctrl     		(ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     			(ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     			(ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     	(ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART13     			(ADDR_DATABUFF)
#define ADDR_UART13IntRx     		ADDR_UART13
#define ADDR_UART13Ctrl     		(ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     			(ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     			(ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     	(ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART14     			(ADDR_DATABUFF)
#define ADDR_UART14IntRx     		ADDR_UART14
#define ADDR_UART14Ctrl     		(ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     			(ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     			(ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     	(ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART15     			(ADDR_DATABUFF)
#define ADDR_UART15IntRx     		ADDR_UART15
#define ADDR_UART15Ctrl     		(ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     			(ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     			(ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     	(ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART16     			(ADDR_DATABUFF)
#define ADDR_UART16IntRx     		ADDR_UART16
#define ADDR_UART16Ctrl     		(ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     			(ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     			(ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     	(ADDR_UART16Tx+LEN_UART16TxMax)

#define ADDR_UARTnCtrl_End     		(ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)

typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
}RMAddrList_TypeDef;			//UART抄表地址列表
#define LEN_RMAddrList     		6
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u16 LEN_DATA;
}RecordFileHead_TypeDef;		//抄表记录文件头
#define LEN_RecordFileHead     	8
typedef __packed struct
{
	u8 OAD[4];
	u16 LEN_DATA;
	u8 REPORT;					//上报标志:0=不需上报,1=需上报，2=已上报
	u8 res1;
}CongealFileHead_TypeDef;		//冻结文件头
#define LEN_CongealFileHead     8
typedef __packed struct
{
	u16 LEN_DATA;				//总长度,事件数据+OAD顺序表(array OAD)
	u16 LEN_EVENT;				//事件数据长度
	u8 REPORT;					//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
	u8 OCCUR;					//产生恢复:0=恢复,1=产生,2=无
	u8 ACK;						//上报确认:0=没确认,1=确认
	u8 res1;
}EventFileHead_TypeDef;	//事件记录文件头
#define LEN_EventFileHead     	8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 AddrType;						//头文件中记录的地址类:0=电表地址,1=采集器地址
	u8 LEN_DATA;
}SearchMeterFileHead_TypeDef;	//搜表记录文件头
#define LEN_SearchMeterFileHead     8


#define UART_TASK_RX				(0)
#define UART_TASK_RX_DONE			(1)
#define UART_TASK_RX_TIMEOUT		(2)

#define UART_TASK_TX				(3)
#define UART_TASK_TX_TIMEOUT		(5)
#define UART_TASK_TXING				(6)
#define UART_TASK_TX_DONE			(7)

#define UART_LOCK					(0x55)
#define UART_UNLOCK					(0)

typedef __packed struct
{
//通用部分
	u8 Task;		// 串口当前任务标识
					// 0=正在接收,1=接收完成,2=等待单帧数据发送,3=单帧数据正在发送
					// 4=多帧数据续接收完成,5=等待多帧数据发送,6=多帧数据正在发送
					// 7=等待发送器空转为接收
	u8 Lock;		// 0x55=通信功能正在被调用禁止其他程序再次调用
	u16 RxByte;		//接收字节数(半字对齐)
	u16 TxByte;		//发送字节数(半字对齐)
	u16 TxCount;	//已发送计数(半字对齐)
	u8 Safety;		//安全认证(交采用)
	//u8 res1;		//
	u8 BpsCtrlBack;	//台体测试临时添加  0:115200  1:38400  2:9600  GPRS波特率
	u16 BpsCtrl;	//通信控制字(同规约2007)(半字对齐)
					//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
					//D4位表示停止位位数0/1分别表示1/2个停止位
					//D3位表示有无校验位0/1分别表示无/有
					//D2位表示偶／奇校验位0/1分别表示偶／奇校验
					//D1-D0 数据位数0～3分别表示5-8位
	u8 Protocol;	//通信规约自动识别:0=698.45,1=DL/T645-2007
	u8 NewBps;		//波特率更新标志;当为MESAM通道时为MESAM内部数据更新标志0=更新,其他值没更新
	u8 SubTask;		//子任务:用哪个波特率去尝试
	u8 AddrType;	//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
					//共16Byte
	//终端专用部分
	u32 RxTxByte;			//通信流量
	u16 PORT;				//本机端口号(半字对齐),同时用于掉电时已连接过1次识别
	u8 TCPUDP;				//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
	u8 LinkCtrl;			//连接控制0=不连接,1=连接
	u8 LinkCount;			//连接计数，用于表示当前连接的是哪个主站参数(0=主站参数，1=备用1主站，2=备用2主站)
	//u8 LinkCount_SelfAdaption; //err20181210 GPRS参数自适应 定义此处系统出现问题！故做修改 连接计数，用于表示自适应过程当前连接的是哪个主站参数(0=普通卡，1=物联卡)
	//u8 SelfAdaptionChangeFlag;//err20181210 GPRS参数自适应 定义此处系统出现问题！故做修改 自适应参数切换标识，用于网络去激活 
	u8 ReLinkNum;			//被动激活模式重拨次数,收到被动激活命令时置初值
	u8 LinkTask;			//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	u8 LinkSubTask;			//链接子任务
	u8 HeartberatCount;		//心跳发出计数,收到确认帧清0
	u8 HostACK;				//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
	u8 HostCount;			//主动上报重发计数(心跳发出计数另计)
	u8 TimerTaskCount;		//主动上报定时任务计数
	
	u8 ATTask;				//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	u8 ATWriteTask;			//AT数据发送任务:0=发命令,1=发数据,2=等待发送延时
	u8 ATCount;				//AT计数
	u8 State;
	u8 LinkTask2;			//通道2链接任务(FTP命令)
	u8 FTPLinkCount;		//FTP尝试连接计数
	u8 Task2;				//通道2任务(FTP命令)
	u8 FTPErr;				//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
	u32 FTPByte;			//FTP当前文件已完成下载字节数
	u32 FTPFileLen;			//FTP当前文件总长度

	//集中抄表专用部分
	u8 LINKCHECK_RTCS;		//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
	u8 RTCS;				//实时钟更新标志原始秒
	u8 RTCM;				//实时钟更新标志原始分
	u8 RTCH;				//实时钟更新标志原始时
	u8 RTCD[2];				//实时钟日更新标志的原始月日
	u8 RTCW[2];				//实时钟星期更新标志的原始月日
	//u16 res7;				//拆分给负控用与EDMI抄表
	u8 res7;
	u8 Jump_Flag;			//云南负控EDMI接收数据解析用，0表示不做转换 1表示0x10减0x40解析。
	//u8 LinkTask_Temp;			//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	//u8 LinkSubTask_Temp;			//链接子任务
	u8 LINKCHECK_RTCD;		//连接检查，每天一次，主要用于连接失败时，当天是否可以重连，此值与当前日相同时，连接失败后当天不再重连
	u8 LastAutoSearchRTCD;	//上次自动搜表实时钟日
	u8 AutoSearchTask;		//自动搜表任务:0=空,1=等待,2...过程
	u8 AutoSearchReCount;	//自动搜表任务重发计数
	u16 AutoSearchTime;		//自动搜表持续时间分，0表示不限时间直至搜表结束
	u16 TransferDelay;		//线路上传输延时时间秒
	u8 NETOPENCount;		//开启服务连续次数计数(当前次数达到x次时关模块电源)
	u8 ReTxSEQ;				//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
	u16 SMSRxSEQ;			//短信接收接口;B0-B11接收帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	u16 SMSTxSEQ;			//短信发送接口;B0-B11发送帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	//共24Byte
	u8 Function;			//口功能 0=终端维护口,!=0终端抄表口
	u8 FunctionTimer;		//上次口功能检查的实时钟秒
	u8 PowerUpDealy;		//上电延时:0=没延时,!=0已到延时
	u8 DHCPTask;			//DHCP任务
	s32 TCPSocket;
	s32 UDPSocket;
	u8 NET_ADDR[20];		//NET_ADDR

	u32 TaskID;				//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	u8 FrameType;
	u8 ReceivedHostAck;		//收到主站确认帧
	u16 Round;
}UARTCtrl_TypeDef;

#define UART0Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART0Ctrl))
#define UART1Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART1Ctrl))
#define UART2Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART2Ctrl))
#define UART3Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART3Ctrl))
#define UART4Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART4Ctrl))
#define UART5Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART5Ctrl))
#define UART6Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART6Ctrl))
#define UART7Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART7Ctrl))
#define UART8Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART8Ctrl))
#define UART9Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART9Ctrl))
#define UART10Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART10Ctrl))
#define UART11Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART11Ctrl))
#define UART12Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART12Ctrl))
#define UART13Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART13Ctrl))
#define UART14Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART14Ctrl))
#define UART15Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART15Ctrl))
#define UART16Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART16Ctrl))


typedef __packed struct
{
	u8 RouterAddr[6];		// 路由主节点地址(总是放在最前,通信库地址定位用)
	u8 RouterDate[3];		// 通信模块接口协议发布日期
	u8 RouterMFCode[2];		// 厂商代码 ASCII 2
	u8 RouterICCode[2];		// 芯片代码 ASCII 2 
	u8 RouterVerDate[3];	// 路由器版本号,分别为:日,月,年
	u8 RouterVer[2];		// 路由器版本号,2BYTE版本

	u8 RouterCtrl;			// 路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
	u8 RouterCtrlTask;		// 路由器控制任务计数;完成后自动回到0
	u8 RetryCnt;			// 路由器控制重发计数
	u8 RUNState;			// 从模块读取的当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
	u8 UpDateTask;			// 软件更新任务
	u8 UpDateReCount;		// 软件更新重试次数
	
	u8 RouterInitTask;		// 路由器初始化任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	u8 RouterInitSubTask;	// 路由器初始化子任务
	u8 RouterInitReCount;	// 路由器初始化命令重发计数
	u8 RouterResetCnt;		// 路由器复位次数计数
	u8 MaxOverTime;			// 从节点监控最大超时时间秒
	
	u8 RouterSynTask;		// 档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
	u8 RouterRealTimeTask;	// 载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
	u8 RouterKeyUserTask;	// 载波重点户抄读任务

	u16 AddNodeNum;			// 档案同步时已添加的从节点数量
	u16 TotalNodeNum;		// 当前路由器中已知总从节点数(半字对齐)
	u16 CurReadNodeNum;		// 查询从节点信息时，当前帧返回的从节点数量
	u16 CurAddNodeNum;		// 添加从节点时，当前帧添加的从节点数量
	u16 TempCount;			// 删除或添加节点等临时计数(半字对齐)
	u16 UpDateCount;		// 软件更新帧计数(半字对齐)
	u16 ReadNodeNum;		// 档案同步时从模块已读取的节点数(半字对齐)
	u16 RS485Time;			// 采集器搜485电表定时值分(BCD)(半字对齐)
	u16 KeepTime;			// 注册持续时间定时值分(半字对齐)
	u8 n;					// 轮次1-3=立即搜表,0=每日搜表
	u8 RMTD;				// 0=非抄表时段,1=抄表时段
	u8 NodeNumOver;			// AFN04F10配置的从节点数>模块支持的最大从节点数;0=否,1=是;仅用于无线模块在显示正在组网时显示超限信息
	u8 Old_RouterInitTask;	// res5; //台体 保存原来任务状态
	u16 UnfinishTaskNum;	//res6;	//台体 未完成任务数
	u32 StopFlags;			//暂停路由标志:(字对齐)
		//b0=非任务号1,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
		//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
		//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	u8 RouterRunMode_1;		//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	u8 RouterRunMode_2;		//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
	u8 RouterRunMode_3;		//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
	u8 RouterRunMode_4;		//路由运行模式_周期抄表模式： 01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
	u8 RouterRunMode_5;		//路由运行模式_传输延时参数支持：由D2、D1、D0等三个位表示，分别代表广播、从节点监控、路由主动抄表等三种命令支持向集中器提供传输延时参数的情况，1表示支持提供传输延时参数，0表示不支持
	u8 RouterRunMode_6;		//路由运行模式_失败节点切换发起方式：D3置1表示通信模块自主切换待抄节点，D4置1表示集中器发起通知通信模块切换待抄节点
	u8 RouterRunMode_7;		//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
	u8 RouterRunMode_8;		//路由运行模式_广播命令信道执行方式：0表示执行广播命令不需要信道标识，1表示广播命令要根据报文中的信道标识逐个来发送，其它取值保留
	u16 MaxBCastOverTime;	//最大广播超时时间秒(半字对齐)
	u16 MaxTxByte;			//最大支持报文长度(半字对齐)
	u16 MaxFileByte;		//文件传输支持的最大单个数据包长度(半字对齐)
	u16 MaxUpDateTime;		//升级操作最大等待时间秒(半字对齐)
	u16 MaxNodeNum;			//支持的最大从节点数(半字对齐)
	u16 MaxRWNodeNum;		//单次读写节点的最大数量
	u8 RadioChannel;		//无线信道组:0～63，64～253：保留，254：自动选择，255：保持不变
	u8 RadioPower;			//无线主节点发射功率:00：最高发射功率，01：次高发射功率，02：次低发射功率，03：最低发射功率，04～254：保留，255：保持不变

	u16 ResDistribTaskNum;  //台体 剩余可分配任务数
	u8 res7[48];
}Terminal_Router_TypeDef;
#define LEN_Router     128								//路由器控制区数据长度
#define ADDR_Router     	ADDR_UARTnCtrl_End			//路由器(集中器载波)
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

//#define ADDR_NoteList     (ADDR_Router+LEN_Router)	//路由器中节点表
typedef __packed struct
{
	u8 Addr[6];				//从节点地址(字对齐)
	u8 CollectorAddr[6];	//采集器地址
//	u16 MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
//	u16 Pn;//对应测量点号
//	u8 FnCount;//抄表Fn计数
//	u8 RMCount;//抄表数据标识计数
//	u8 ReRMCount;//重发计数
//	u8 RMCountErr;//Fn内有抄读失败或不抄的数据标识计数
//	u8 RMErr;//当前抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败
//	u8 RMErr_0x901F;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
//	u8 FnCountSOfMeterEvent;//插入周期抄读电能表事件前原抄表的Fn计数
//	u8 FnCountBackupOfAutoReport;//抄读主动上报前原抄表的Fn计数
//	u8 FnCountSOfKeyUser;//插入每小时抄读曲线任务前原循抄任务的抄表Fn计数
//	u8 FnCountS;//插入任务前原循抄任务的抄表Fn计数
//	u16 res1;
}NoteList_TypeDef;
#define LEN_NoteList     12				//每列表字节数
typedef __packed struct
{
  NoteList_TypeDef     List[NUM_RMmax];
}Terminal_NoteList_TypeDef;
//#define Terminal_Note     ((Terminal_NoteList_TypeDef *)(ADDR_NoteList))//载波路由器中节点表(按读路由和添加顺序排列)


#define ADDR_Terminal_ERAM_Start     ((ADDR_Router + LEN_Router) & 0xfffffffc)		//ERAM需半字对齐

//终端系统数据
typedef __packed struct
{
	u8 res0;
	u8 I2SUBTask1;//终端类1数据计算任务
	u8 I2SUBTask2;//终端类2数据计算任务
	u8 I2SUBTask3;//终端类3数据计算任务
	u8 I2SUBTask4;//终端统计数据计算任务
	u8 I2SUBTask5;//终端控制任务
	u8 ERC22Task;//ERC22有功总电能量差动越限计算
	u8 Cascading_LoopChack_Count;//级联周期巡测终端计数
	u8 CurveCongealTask;//曲线冻结任务(当时,上1-24时)
	u8 DateCongealCheckCount;//日冻冻结检查计数
	u8 MonthCongealCheckCount;//月冻冻结检查计数
	u8 CurveCongealCheckCount;//曲线冻结检查计数
	u8 ExtTask1;//终端类1数据计算扩展任务
	u8 res1;
	u16 I2SUBTask4_1;//终端统计数据计算任务.日月总及分相最大有功功率及发生时间,有功功率为零时间(半字对齐)
	u16 I2SUBTask4_2;//终端统计数据计算任务.日月总加组最大最小有功功率及其发生时间,有功功率为零累计时间(半字对齐)
	u16 I2SUBTask4_3;//终端统计数据计算任务.电压统计数据分步计数(半字对齐)
	u16 I2SUBTask4_4;//终端统计数据计算任务.电流统计数据(半字对齐)
	u16 I2SUBTask4_5;//终端统计数据计算任务.电压电流不平衡分步计数(半字对齐)
	u16 I2SUBTask4_6;//终端统计数据计算任务.视在功率越限累计时间(半字对齐)
	u16 I2SUBTask4_7;//终端统计数据计算任务.负载率
	u16 I2SUBTask4_8;//终端统计数据计算任务.总加组超功率定值月电能量定值的月累计时间累计电能量
	u16 I2SUBTask4_9;//终端统计数据计算任务.功率因数区段累计时间
	u16 I2SUBTask4_10;//终端统计数据计算任务.谐波越限
	u16 I2SUBTask4_11;//终端统计数据计算任务.直流模拟量越限
	u16 I2SUBTask4_12;//终端统计数据计算任务.日三相电压、电流不平衡度95%概率值
	u16 I2SUBTask4_13;//终端统计数据计算任务.月三相电压、电流不平衡度95%概率值
	u16 I2SUBTask4_14;//终端统计数据计算任务.日冻结电压谐波电压含有率、谐波电流95%概率值
	u16 I2SUBTask4_15;//ERC10_Event();//电压回路异常(断相、失压)
	u16 I2SUBTask4_16;//
	u16 res2;
#define ADDR_Terminal_MS_Timer     (ADDR_Terminal_ERAM_Start+48)//ms定时器
	u16 RequestFileTimer;			// 1 请求文件传输MS定时器,每10MS减1定时器
  	u16 BUZZMSTimer;				// 2 蜂鸣器每10MS减1定时器
	u16 Trip1_MS_Timer;				// 3 终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
	u16 Trip2_MS_Timer;				// 4 终端跳闸2 10MS定时器;1200ms,>=1000跳,<1000合
	u16 Trip3_MS_Timer;				// 5 终端跳闸3 10MS定时器;1200ms,>=1000跳,<1000合;微功率无线学习完成读路由状态定时器
	u16 Trip4_MS_Timer;				// 6 终端跳闸4 10MS定时器;1200ms,>=1000跳,<1000合;批量对时延时等待电表生成校时记录定时器
	u16 AT_MS_Timer;				// 7 GPRS AT命令用10MS定时器
	u16 GPRS_RxLED_MS_Timer;		// 8 GPRS 接收LED 10MS定时器
	u16 GPRS_TxLED_MS_Timer;		// 9 GPRS 发送LED 10MS定时器
	u16 RS485_1_TxLED_MS_Timer;		// 10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
	u16 RS485_1_RxLED_MS_Timer;		// 11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
	u16 RS485_2_TxLED_MS_Timer;		// 12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	u16 RS485_2_RxLED_MS_Timer;		// 13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
	u16 RS485_3_TxLED_MS_Timer;		// 14 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	u16 RS485_3_RxLED_MS_Timer;		// 15 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
	u16 ENETLED_MS_Timer;			// 16 以太网接收发送LED指示MS定时器,每10MS减1定时器
	u16 GPRS_ONOFF_MS_Timer;		// 17 GPRS ONOFF脚控制 10MS定时器
	u16 RouterCtrl_MS_Timer;		// 18 路由控制恢复延时 10MS定时器
	//u16 CD485TxLED_MS_Timer;		// 级联485发送LED指示MS定时器,每10MS减1定时器
	//u16 CD485RxLED_MS_Timer;		// 级联485接收LED指示MS定时器,每10MS减1定时器
	//u16 DisplayScanTimer;			// 显示扫描 每10MS减1定时器
	u16 GPRS_ONLINELED_MS_Timer;//19 GPRS在线LED指示MS定时器,每10MS减1定时器
	u16 GPRS_WEAK_SINGLE_LED_MS_Timer;//20 GPRS信号最弱LED指示MS定时器,每10MS减1定时器
	u16 GPRS_STRONG_SINGLE_LED_MS_Timer;//21 GPRS信号最强LED指示MS定时器,每10MS减1定时器
#define Terminal_MS_Timer_Num     21//终端10MS定时器数
#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s定时器
	u16 S_PR;						// 1 60秒分频产生分
	u16 PORT0_S_Timer;				// 2 秒定时器
	u16 PORT1_S_Timer;				// 3 秒定时器
	u16 PORT2_S_Timer;				// 4 秒定时器
	u16 PORT3_S_Timer;				// 5 秒定时器
	u16 PORT4_S_Timer;				// 6 秒定时器
	u16 PORT5_S_Timer;				// 7 秒定时器
	u16 PORT6_S_Timer;				// 8 秒定时器
	u16 PORT7_S_Timer;				// 9 秒定时器
	u16 PORT8_S_Timer;				// 10 秒定时器
	u16 PORT9_S_Timer;				// 11 秒定时器
	u16 PORT10_S_Timer;				// 12 秒定时器
	u16 PORT11_S_Timer;				// 13 秒定时器
	u16 PORT12_S_Timer;				// 14 秒定时器
	u16 PORT13_S_Timer;				// 15 秒定时器
	u16 PORT14_S_Timer;				// 16 秒定时器
	u16 PORT15_S_Timer;				// 17 秒定时器
	u16 PORT16_S_Timer;				// 18 秒定时器

    u16 ENETHeartBeat_S_Timer;		// 19 以太网心跳秒定时器
	u16 IPLeaseTimer;				// 20 以太网自动获得IP租赁时间秒定时器
	u16 ENETCheck_S_Timer;			// 21 以太网IP冲突检查和租赁更新秒定时器
    u16 GPRSHeartBeat_S_Timer;		// 22 GPRS心跳秒定时器
	u16 GPRSAutoDiscon_S_Timer;		// 23 GPRS被动激活模式连续无通信自动断线秒定时器
	u16 GPRSReConnection_S_Timer;	// 24 GPRS永久在线、时段在线模式重拨间隔秒定时器
	u16 GPRSWaitConnection_S_Timer;	// 25 GPRS发起连接服务等待秒定时器
	u16 GPRSWaitTx_S_Timer;			// 26 GPRS发送数据等待超时秒定时器
	u16 GPRSCheck_S_Timer;			// 27 GPRS在线和信号检查秒定时器
	u16 ResetCommand_S_Timer;		// 28 终端收到复位命令延时执行秒定时器
	u16 GPRS_WaitHostACK_S_Timer;	// 29 GPRS主动上报等待主站确认秒定时器
	u16 ENET_WaitHostACK_S_Timer;	// 30 ENET主动上报等待主站确认秒定时器
	u16 RS232_WaitHostACK_S_Timer;	// 31 RS232主动上报等待主站确认秒定时器
	
	u16 Cascading_LoopChack_S_Timer;// 32 级联巡测周期秒定时器
	u16 SlipCYC_S_Timer;			// 33 终端输入脉冲功率计算和需量脉冲滑移秒定时器
	u16 DemandCYC_S_Timer;			// 34 终端输入脉冲需量计算秒定时器
	u16 RouterCheck_S_Timer;		// 35 载波路由器检查秒定时器
	u16 RouterAck_S_Timer;			// 36 载波路由等待 ACK 应答秒定时器
	u16 AutoUpdataAFN04F10_S_Timer;	// 37 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
	u16 AutoUpdataAFN04F10ERC3_S_Timer;	// 38 自动更新F10主动上报ERC3事件秒定时器;集中器通过本地的自动发现，完成测量点新增、变更或删除后，应立即置测量点参数变更标志（F150参数）相应位为1，并立即上报参数变更事件（ERC3）到主站,除非进行了测量点参数变更标志的复位，否则，每次上报参数变更事件后的m分钟（m=0~30，一般取10）之内，无论参数是否发生变更均不再次上报
	u16 AutoDelAFN04F10_S_Timer;		// 39 自动删除AFN04F10的电能表配置秒定时器,仅免经常调用影响CPU速度
	u16 AutoSearch_S_Timer;				// 40 载波自动搜表秒定时器
	u16 RouterSynDelay_S_Timer;			// 41 路由档案同步延时执行秒定时器
	u16 Power_S_Timer;					// 42 停电事件秒定时器
	u16 MeterSyncToRam_S_Timer;			// 43 测量点档案同步到 SDRAM 秒定时器
	u16 RS485_1_Ack_S_Timer;			// 44 RS485_1等待 ACK 应答秒定时器
	u16 RS485_2_Ack_S_Timer;			// 45 RS485_2等待 ACK 应答秒定时器
	u16 RS485_3_Ack_S_Timer;			// 46 RS485_3等待 ACK 应答秒定时器
	u16 IR_WaitHostAck_S_Timer;			// 47 IR 红外等待 ACK 应答秒定时器
//  u16 DisplayLoopTimer;				// 循显秒定时器
//  u16 BackLightTimer;					// 背光灯显示秒定时器

#define BASE_S_Timer_Num     47			// 秒定时器数

#if ((Project/100) == 3)				// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	u16 ReTrip_S_Timer;				// 1 终端补跳闸秒定时器
	u16 EnSwitchON_S_Timer;			// 2 终端合闸允许通知秒定时器
	u16 Remote1_S_Timer;			// 3 遥控跳闸轮次1报警延时秒定时器
	u16 Remote2_S_Timer;			// 4 遥控跳闸轮次2报警延时秒定时器
	u16 Remote3_S_Timer;			// 5 遥控跳闸轮次3报警延时秒定时器
	u16 Remote4_S_Timer;			// 6 遥控跳闸轮次4报警延时秒定时器
	u16 Remote5_S_Timer;			// 7 遥控跳闸轮次5报警延时秒定时器
	u16 Remote6_S_Timer;			// 8 遥控跳闸轮次6报警延时秒定时器
	u16 Remote7_S_Timer;			// 9 遥控跳闸轮次7报警延时秒定时器
	u16 Remote8_S_Timer;			// 10 遥控跳闸轮次8报警延时秒定时器
	u16 PowerCtrl1_S_Timer;			// 11 终端总加组1功控功率计算秒定时器
	u16 PowerCtrl2_S_Timer;			// 12 终端总加组2功控功率计算秒定时器
	u16 PowerCtrl3_S_Timer;			// 13 终端总加组3功控功率计算秒定时器
	u16 PowerCtrl4_S_Timer;			// 14 终端总加组4功控功率计算秒定时器
	u16 PowerCtrl5_S_Timer;			// 15 终端总加组5功控功率计算秒定时器
	u16 PowerCtrl6_S_Timer;			// 16 终端总加组6功控功率计算秒定时器
	u16 PowerCtrl7_S_Timer;			// 17 终端总加组7功控功率计算秒定时器
	u16 PowerCtrl8_S_Timer;			// 18 终端总加组8功控功率计算秒定时器
	u16 PowerAttr1_S_Timer;			// 19 终端总加组1功控报警秒定时器
	u16 PowerAttr2_S_Timer;			// 20 终端总加组2功控报警秒定时器
	u16 PowerAttr3_S_Timer;			// 21 终端总加组3功控报警秒定时器
	u16 PowerAttr4_S_Timer;			// 22 终端总加组4功控报警秒定时器
	u16 PowerAttr5_S_Timer;			// 23 终端总加组5功控报警秒定时器
	u16 PowerAttr6_S_Timer;			// 24 终端总加组6功控报警秒定时器
	u16 PowerAttr7_S_Timer;			// 25 终端总加组7功控报警秒定时器
	u16 PowerAttr8_S_Timer;			// 26 终端总加组8功控报警秒定时器
	u16 PowerCtrl1_Congeal_S_Timer;	// 27 终端总加组1当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl2_Congeal_S_Timer;	// 28 终端总加组2当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl3_Congeal_S_Timer;	// 29 终端总加组3当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl4_Congeal_S_Timer;	// 30 终端总加组4当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl5_Congeal_S_Timer;	// 31 终端总加组5当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl6_Congeal_S_Timer;	// 32 终端总加组6当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl7_Congeal_S_Timer;	// 33 终端总加组7当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl8_Congeal_S_Timer;	// 34 终端总加组8当前功率下浮控控后冻结秒定时器
	u16 UpPower_Protect_S_Timer;	// 35 终端上电保电秒定时器
	u16 ContrlLoopCheck_S_Timer;	// 36 控制输出回路检测有效等待秒定时器
	#define Contrl_S_Timer_Num     36
#else
	#define Contrl_S_Timer_Num     0
#endif	//#if ((Project/100)==3)	// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//终端秒定时器数
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m定时器
	u16 DisconAutoProtect_M_Timer;		// 1 自动保电允许与主站连续无通信时间分定时器
	u16 AutoSearchMeterTimer;			// 2 自动搜表允许时间分定时器(初始是湖南要求)
	u16 RS4851RMF246_M_Timer;			// 25 RS485-1停电事件抄读间隔分定时器
	u16 RS4852RMF246_M_Timer;			// 26 RS485-2停电事件抄读间隔分定时器
	u16 RS4853RMF246_M_Timer;			// 27 RS485-3停电事件抄读间隔分定时器
	u16 RS4854RMF246_M_Timer;			// 28 RS485-4停电事件抄读间隔分定时器(载波)

#define Terminal_M_Timer_Num     6		// 终端分定时器数
#define ADDR_SwitchIn     ADDR_Terminal_M_Timer+(2*Terminal_M_Timer_Num)//开关量输入
	u8 SwitchIn1;						// 开关量输入1原状态
	u8 SwitchInFilter1;					// 开关量输入1滤波次数
	u8 SwitchIn2;						// 开关量输入2原状态
	u8 SwitchInFilter2;					// 开关量输入2滤波次数
	u8 SwitchIn3;						// 开关量输入3原状态
	u8 SwitchInFilter3;					// 开关量输入3滤波次数
	u8 SwitchIn4;						// 开关量输入4原状态
	u8 SwitchInFilter4;					// 开关量输入4滤波次数
	u8 SwitchIn5;						// 开关量输入5原状态
	u8 SwitchInFilter5;					// 开关量输入5滤波次数
	u8 SwitchIn6;						// 开关量输入6原状态
	u8 SwitchInFilter6;					// 开关量输入6滤波次数
	u8 SwitchIn7;						// 开关量输入7原状态
	u8 SwitchInFilter7;					// 开关量输入7滤波次数
	u8 SwitchIn8;						// 开关量输入8原状态
	u8 SwitchInFilter8;					// 开关量输入8滤波次数
#define ADDR_Other      ADDR_SwitchIn+(2*8)
	u8 CongealFlags;			// 在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
 	u8 LastCongealTime[5];		// 上次冻结时的实时钟年月日时分寄存
	u8 Terminal_S_Count;		// 终端秒加计数,用于其他秒定时器
	u8 Terminal_M_Count;		// 终端分加计数,用于其他分定时器
	u8 SET6000;					// 采集档案配置表有设置标志:0=没设置,0x55=设置
	u8 SETTask;					// 任务配置表采集方案有设置标志:0=没设置,0xff=设置
	u8 FLASHtoSDRAM;			// 参数FLASHtoSDRAM上次进行的实时钟时(或其他)
	u8 res4;
	u8 res5;
	u8 RoundTripFlags;			// 终端轮次跳闸位标志,B0-B7=0没循跳,=1已循跳
	u8 MODULSEL;				// 无线模块选择0=,1=华为CDMA MC323,2=GPRS标准3763,3=模块盒没插入
	u8 ENETSEL;					// 以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
	u8 Signal;					// GPRS信号强度0-4
	u8 SignaldBm;				// 信号强度0-31或99(BCD码)
	u8 PowerDownTask;			// 电源掉电过程任务
	u8 PowerDownSubTask;		// 电源掉电过程子任务
	u16 CopyTxByte;				// 寄存GPRS需发送字节数(半字对齐)
	u16 CopyTxCount;			// 寄存GPRS已发送计数(半字对齐)
	u16 ReqTxByte;				// 寄存GPRS请求发送字节数(半字对齐)
	u16 RT_AMRTimeOut;			// 终端实时抄表超时时间计数
	
	
	u32 Ticks;
	u32 Seconds;
	u32 LCD_RT_AMR_REQ_TEST_START_TIME;//res10; //err20180730 台体 微功率无线模块实时召测项测试时切换波特率，下发一个GUI实时召测帧适应波特率。
	u8 LinkCount_SelfAdaption; //err20181210 GPRS参数自适应 连接计数，用于表示自适应过程当前连接的是哪个主站参数(0=普通卡，1=物联卡)(2=主站IP1，3=备用IP1，4=备用IP2)err20190528 云南负控自适应
	u8 SelfAdaptionChangeFlag;	//err20181210 GPRS参数自适应 自适应参数切换标识，用于网络去激活
	u8 GPRS_Para_Reset_Flag;		//err20190313 GPRS上线失败后重新设置通信参数 1:重新设置参数
	u8 GPRS_Para_Reset_Count;		//err20190313 GPRS参数重置次数计数，达到一定次数后再硬件复位
	//u8 res11;
	//u32 res11;
	u16 AT_AMSTimeOut;          // RS485自动搜表超时时间计数
	u16 res16;
	//u32 res12;
	
	u32 res13;
	u32 res14;
	u32 res15;
	u32 rse16;
}Terminal_Ram_TypeDef;
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


#define ADDR_BUFF_UART_TIMER_END     (((ADDR_Other + 64) + 3) & 0xfffffffc)
#if (ADDR_BUFF_UART_TIMER_END - ADDR_ERAM_START) > LEN_ERAM_BUFF_UART_TIMER
#error
#endif

#define ADDR_QCSG_ERAM_START     	ADDR_BUFF_UART_TIMER_END
//#define ADDR_QCSG_ERAM_START     	(ADDR_ERAM_START + LEN_ERAM_BUFF_UART_TIMER)
#define LEN_Terminal_ERAM			(2*1024*1024)
#define ADDR_MobilePhoneNumber     	ADDR_QCSG_ERAM_START					//通信卡的手机号码和IMSI号码(用于显示)
// 20byte 手机号码(ASCII)
// 20byte IMSI号码(ASCII)
// 16字节无线模块本地IP地址,ASCII
// 6字节无线模块本地端口号,ASCII
// 6字节无线模块信号分贝值,ASCII,单位dB

//终端控制
#define ADDR_Terminal_CTRL_ESRAM_START     ADDR_MobilePhoneNumber+68	//终端控制ESRAM开始地址

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START			//终端费率时段和功控时段
// 1BYTE 终端当前执行费率(HEX 费率1,2,3,4 分别对应尖,峰,平,谷等费率),0=无效
// 1BYTE 
// 1BYTE 当前功控时段号1-8,=0表示时段号>9错误
// 1BYTE 0=当前功控时段号不控制,1或2当前功控时段号控制

#define ADDR_Terminal_Protect     ADDR_Terminal_Period+4				// 终端保电,0=不保电,1=命令保电,2=自动保电
// 1BYTE 0=不保电,1=命令保电,2=自动保电
#define ADDR_Terminal_Attr_State     ADDR_Terminal_Protect+1			// 催费告警控制状态
// 1BYTE 0=没输出,1=输出
#define ADDR_Terminal_Total_State     ADDR_Terminal_Attr_State+1		// 终端总跳闸
// 1BYTE 位标志(轮次1-8)当前总跳闸状态(1跳闸,0允许合闸)
// 1BYTE 位标志(轮次1-8)允许合闸通知(总跳闸状态相应位转为合闸时置位,通知定时到后清0)
// 2BYTE 保留
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//控制模块输出输入数据
// 1BYTE 要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
// 1BYTE 要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯,b2-b6,b7=轮次继电器动作方式(0=脉冲1=内部自动测试时电平)
// 1BYTE 从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
// 1BYTE 从控制模块输入的断线标志字节1

#define Terminal_Ctrl_Pn_Len     12										// 终端控制每Pn数据长度
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4			// 终端控制-遥控跳闸
// 控制轮次1-N
// 1BYTE 控制状态,0=上电或不控制,1=投入,2=...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
// 1BYTE 
// 2BYTE 跳闸时功率(数据格式2)
// 1BYTE !=0值表示投入但没控原因号1=...
// 1BYTE 
// 2BYTE  
// 4BYTE 
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*NUMmax_CR)		//终端控制-功控.当前功率下浮控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=投入,2=计算定值,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
// 1BYTE 已跳闸轮次B0-B7
// 2BYTE 跳闸时功率(数据格式2)
// 1BYTE !=0值表示投入但没控原因号1=...
// 1BYTE 
// 2BYTE
// 1BYTE 超定值标志0=没超,1=超定值
// 1BYTE 当前功控跳闸轮次B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)	//终端控制-功控.营业报停功控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
// 1BYTE 已跳闸轮次B0-B7
// 2BYTE 跳闸时功率(数据格式2)
// 1BYTE !=0值表示投入但没控原因号1=...
// 1BYTE 
// 2BYTE
// 1BYTE 超定值标志0=没超,1=超定值
// 1BYTE 当前功控跳闸轮次B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-功控.厂休功控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
// 1BYTE 已跳闸轮次B0-B7
// 2BYTE 跳闸时功率(数据格式2)
// 1BYTE !=0值表示投入但没控原因号1=...
// 1BYTE 
// 2BYTE
// 1BYTE 超定值标志0=没超,1=超定值
// 1BYTE 当前功控跳闸轮次B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-功控.时段功控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
// 1BYTE 已跳闸轮次B0-B7
// 2BYTE 跳闸时功率(数据格式2)
// 1BYTE !=0值表示投入但没控原因号1=...
// 1BYTE 方案号0-2表示1-3
// 2BYTE 当前时段定值
// 1BYTE 超定值标志0=没超,1=超定值
// 1BYTE 当前功控跳闸轮次B0-B7
// 2BYTE
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-电控.月电控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
// 1BYTE 已跳闸轮次B0-B7
// 4BYTE 月电量定值(数据格式3)
// 2BYTE 
// 1BYTE	
// 3BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)		//终端控制-电控.购电控
// 总加组1-N
// 1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
// 1BYTE 已跳闸轮次B0-B7
// 4BYTE 
// 1BYTE 购电类别在剩余电量(费)计算时识别,0=购电量,1=购电费
// 1BYTE 
// 4BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)		//终端控制ESRAM结束地址


// 终端统计用实时数据
//#define ADDR_213x_REAL     ADDR_F20B_2+LENmax_F20B_2//当月相电压合格率
// 1BYTE 0 上次计算RTC秒
// 1BYTE 1 上电延时秒加定时
// 1BYTE 2 秒计数
// 1BYTE 3 电压加平均计数

// 4BYTE 4 电压加平均
// 1BYTE 8 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
// 1BYTE 9 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限

// 4BYTE 10 电压加平均
// 1BYTE 14 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
// 1BYTE 15 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限

// 4BYTE 16 电压加平均
// 1BYTE 20 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
// 1BYTE 21 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
//#define LEN_2131_REAL     22

//#define ADDR_PowerEvent     ADDR_213x_REAL+LEN_2131_REAL//终端停/上电事件原始数据
typedef __packed struct
{
	u8 Tstoptime[7];		//终端停电时间hexYYMDHMS
	u8 Tstarttime[7];		//终端上电时间hexYYMDHMS
	u8 RTCS;				//上次计算时RTC的秒
	u8 task;				//任务
	u8 bit;					//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
	u8 res1;
	u8 res2;
	u8 NUMrm;				//停上电事件需抄读电表个数
	u16 setno[NUMmax_Event3106RM];//停/上电事件需抄读电表的配置序号(从0开始)
}PowerEvent_typedef;
//#define LEN_PowerEvent     	(20+(2*NUMmax_Event3106RM))
//#define PowerEvent     		((PowerEvent_typedef*)(ADDR_PowerEvent))

//#define ADDR_Event_Sour     	((ADDR_PowerEvent+LEN_PowerEvent+3)&0xfffffffc)//电能量超差,飞走,停走事件原始数据(档案配置序号从0开始)
typedef __packed struct
{
	u16 RMAddr[3];			//电能表地址
	u16 res;
	u32 Occur;				//事件产生恢复,bit=0恢复=1产生;b0=抄表失败(310F),b1=
	u32 TimeCount;			//电能表停走时间计时(秒)
}EventSour_typedef;
#define LEN_Event_Sour     16


#define ADDR_RealTimeAMRTaskCtrl			((ADDR_Terminal_CTRL_ESRAM_END + 3) & 0xfffffffc)
#define LEN_RealTimeAMRTaskCtrl				(8192 + 2048)

#define ADDR_RS232_TxBuf					(ADDR_RealTimeAMRTaskCtrl + LEN_RealTimeAMRTaskCtrl)
#define LEN_RS232_TxBuf						(2048)

#define ADDR_IR_TxBuf						(ADDR_RS232_TxBuf + LEN_RS232_TxBuf)
#define LEN_IR_TxBuf						(2048)

#define ADDR_GPRS_TxBuf						(ADDR_IR_TxBuf + LEN_IR_TxBuf)
#define LEN_GPRS_TxBuf						(2048)

#define ADDR_ETHE_TxBuf						(ADDR_GPRS_TxBuf + LEN_GPRS_TxBuf)
#define LEN_ETHE_TxBuf						(2048)

#define ADDR_RS485_CH1_TxBuf				(ADDR_ETHE_TxBuf + LEN_ETHE_TxBuf)
#define LEN_RS485_CH1_TxBuf					(2048)

#define ADDR_RS485_CH2_TxBuf				(ADDR_RS485_CH1_TxBuf + LEN_RS485_CH1_TxBuf)
#define LEN_RS485_CH2_TxBuf					(2048)

#define ADDR_RS485_CH3_TxBuf				(ADDR_RS485_CH2_TxBuf + LEN_RS485_CH2_TxBuf)
#define LEN_RS485_CH3_TxBuf					(2048)

#define ADDR_PLC_YX_TxBuf					(ADDR_RS485_CH3_TxBuf + LEN_RS485_CH3_TxBuf)
#define LEN_PLC_YX_TxBuf					(2048)

#define ADDR_AC_SAMPLE_TxBuf				(ADDR_PLC_YX_TxBuf + LEN_PLC_YX_TxBuf)
#define LEN_AC_SAMPLE_TxBuf					(256)

#define ADDR_RS485_CH1_NormalTaskCtrl		(ADDR_AC_SAMPLE_TxBuf + LEN_AC_SAMPLE_TxBuf)
#define LEN_AMRNormalTaskCtrl				(20*1024 + 512)
#define ADDR_RS485_CH2_NormalTaskCtrl		(ADDR_RS485_CH1_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define ADDR_RS485_CH3_NormalTaskCtrl		(ADDR_RS485_CH2_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define ADDR_PLC_NormalTaskCtrl				(ADDR_RS485_CH3_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)

#define ADDR_LCD_MeterAMRDataBuf			(ADDR_PLC_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define LEN_LCD_MeterAMRDataBuf				(1024)

#define ADDR_NodeList						(ADDR_LCD_MeterAMRDataBuf + LEN_LCD_MeterAMRDataBuf)
#define LEN_NodeList						(32*1024)

#define ADDR_PnDataBuf						(ADDR_NodeList + LEN_NodeList)
#define LEN_PnDataBuf						(64*1024)

#define ADDR_ReportDataBuf					(ADDR_PnDataBuf + LEN_PnDataBuf)
#define LEN_ReportDataBuf					(64*1024)

/*
#define ADDR_AFN_0A_DataBuf					(ADDR_ReportDataBuf + LEN_ReportDataBuf)
#define LEN_AFN_0A_DataBuf					(256*1024)				

#define ADDR_AFN_0E_DataBuf					(ADDR_AFN_0A_DataBuf + LEN_AFN_0A_DataBuf)			// 读事件记录数据缓冲
#define LEN_AFN_0E_DataBuf					(32*1024)				// 多出的 64 字节为防止指针访问溢出

#define ADDR_AFN_12_DataBuf					(ADDR_AFN_0E_DataBuf + LEN_AFN_0E_DataBuf + 64)	// 读任务数据缓冲
#define LEN_AFN_12_DataBuf					(64*1024)				// 多出的 64 字节为防止指针访问溢出

#define ADDR_AFN_13_DataBuf					(ADDR_AFN_12_DataBuf + LEN_AFN_12_DataBuf + 64)	// 读告警数据缓冲
#define LEN_AFN_13_DataBuf					(32*1024)				// 多出的 64 字节为防止指针访问溢出

#define ADDR_AFN_0D_DataBuf					(ADDR_AFN_13_DataBuf + LEN_AFN_13_DataBuf + 64)	// 读历史数据缓冲
#define LEN_AFN_0D_DataBuf					(2*1024*1024)			// 多出的 64 字节为防止指针访问溢出
*/

#define ADDR_RS232_QCSG_DataBuf				(ADDR_ReportDataBuf + LEN_ReportDataBuf)
#define LEN_RS232_QCSG_DataBuf				(512*1024)

#define ADDR_GPRS_QCSG_DataBuf				(ADDR_RS232_QCSG_DataBuf + LEN_RS232_QCSG_DataBuf)
#define LEN_GPRS_QCSG_DataBuf				(512*1024)

#define ADDR_ENET_QCSG_DataBuf				(ADDR_GPRS_QCSG_DataBuf + LEN_GPRS_QCSG_DataBuf)
#define LEN_ENET_QCSG_DataBuf				(512*1024)

#define ADDR_IR_QCSG_DataBuf				(ADDR_ENET_QCSG_DataBuf + LEN_ENET_QCSG_DataBuf)
#define LEN_IR_QCSG_DataBuf					(64*1024)

#define ADDR_RS485_1_QCSG_DataBuf			(ADDR_IR_QCSG_DataBuf + LEN_IR_QCSG_DataBuf)
#define LEN_RS485_1_QCSG_DataBuf			(256*1024)

#define ADDR_RS485_2_QCSG_DataBuf			(ADDR_RS485_1_QCSG_DataBuf + LEN_RS485_1_QCSG_DataBuf)
#define LEN_RS485_2_QCSG_DataBuf			(256*1024)

#define ADDR_RS485_3_QCSG_DataBuf			(ADDR_RS485_2_QCSG_DataBuf + LEN_RS485_2_QCSG_DataBuf)
#define LEN_RS485_3_QCSG_DataBuf			(256*1024)

#define ADDR_QCSG_ERAM_END					(ADDR_RS485_3_QCSG_DataBuf + LEN_RS485_3_QCSG_DataBuf)

#if (ADDR_QCSG_ERAM_END - ADDR_ERAM_START) > (LEN_Terminal_ERAM + LEN_ERAM_BUFF_UART_TIMER)
	#error ADDR_QCSG_ERAM_END Over
#endif

#define ADDR_LIN_YUN_ERAM_START			(ADDR_ERAM_START + LEN_ERAM_BUFF_UART_TIMER + LEN_Terminal_ERAM)
#if(((Project/100) == 2) || ((Project/100) == 3) || ((Project/100) == 5) || ((Project/100) == 9))			//方案200=集中器,300=专变终端
	#define LEN_LIN_YUN_ERAM     		(1024*1024)			// 限定总长度字节数
	#include "../Hcsg/ERAM_For_LinYun.h"
#else	
	#define ADDR_LIN_YUN_ERAM_END     	(ADDR_LIN_YUN_ERAM_START+0)
	#define LEN_LIN_YUN_ERAM     		0						// 限定总长度字节数	
#endif

#if (ADDR_LIN_YUN_ERAM_END - ADDR_LIN_YUN_ERAM_START) > LEN_LIN_YUN_ERAM
	#error ADDR_LIN_YUN_ERAM_END Over
#endif


#define ADDR_MXN_ERAM_START				(ADDR_LIN_YUN_ERAM_START + LEN_LIN_YUN_ERAM)
#if(((Project/100) == 2) || ((Project/100) == 3) || ((Project/100) == 5) || ((Project/100) == 9))			//方案200=集中器,300=专变终端
	#define LEN_MXN_ERAM     			(1024*1024)			// 限定总长度字节数
	#include "../Hcsg/ERAM_For_Mxn.h"
#else	
	#define ADDR_MXN_ERAM_END     		(ADDR_MXN_ERAM_START+0)
	#define LEN_MXN_ERAM     			0						// 限定总长度字节数	
#endif

#if (ADDR_MXN_ERAM_END - ADDR_MXN_ERAM_START) > LEN_MXN_ERAM
	#error ADDR_MXN_ERAM_END Over
#endif

#define ADDR_ERAM_END 					(ADDR_MXN_ERAM_END & 0xfffffffc)		

//剩余ERAM空间全用于自动分配(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00400000
    #error ADDR_ERAM_END>4//4M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00400000)//堆为8字节对齐
	#define LEN_MyHeap     0x00400000//剩余ERAM空间长度
#endif

#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=没,1=有
#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>6//6M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//堆为8字节对齐
	#define LEN_MyHeap     0x00A00000//剩余ERAM空间长度
#else
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8//8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//堆为8字节对齐
	#define LEN_MyHeap     0x00800000//剩余ERAM空间长度
#endif
#endif

#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00700000
    #error ADDR_ERAM_END>7M//32M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00700000)//堆为8字节对齐
	#define LEN_MyHeap     0x01900000//剩余ERAM空间长度
#endif
#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00900000
    #error ADDR_ERAM_END>9M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00900000)//堆为8字节对齐
	#define LEN_MyHeap     0x02700000//剩余ERAM空间长度
#endif

#if IC_SDRAM==0
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>LEN_Terminal_Ram
    #error ADDR_ERAM_END>LEN_Terminal_Ram
  #endif
	#define ADDR_MyHeap     0
	#define LEN_MyHeap     0//剩余ERAM空间长度
#endif



#endif

















