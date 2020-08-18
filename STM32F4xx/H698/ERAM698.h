


#ifndef ERAM_H
#define ERAM_H

#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
#define LEN_ERAM_BUFF_UART_TIMER     ((4*1024*1024)+(2*LEN_msFILEmanage_Ext))//数据缓冲区和UART和定时器等系统数据总长度
#else
#define LEN_ERAM_BUFF_UART_TIMER     ((6*1024*1024)+(2*LEN_msFILEmanage_Ext))//数据缓冲区和UART和定时器等系统数据总长度
#endif

#ifdef IC_STM32F4xx
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
	#ifndef LINUX
//0x00000000-0x003FFFFF 4M-512-16byte Program
#define ADDR_ETHMAC     (0x00400000-512-16)//以太网MAC地址
//6byte MACADDR0
//6byte MACADDR1
#define ADDR_REG_SYS_RSTSTS     (0x00400000-512-4)
//4byte 上电时读取的REG_SYS_RSTSTS值
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

	#else
	
//1M ISRAM
//ADDR_ERAM_linux+0x00001000   256K   Meter
//ADDR_ERAM_linux+0x00040000   768K     Terminal
//ADDR_ERAM_linux+0x00100000   256K     Menu or Test

#define ADDR_SPI_MAP     (ADDR_ERAM_linux+0x00240000)
#define LEN_SPI_MAP     0x00080000
//512K IEEPROM(SPIFLASH) 映射
#define ADDR_NAND_MAP     (ADDR_ERAM_linux+0x002C0000)
#define LEN_NAND_MAP     0x00280000
//0x00C80000-0x00EFFFFF 2.5M NAND_FLASH 映射
#define ADDR_ERAM_START     (ADDR_ERAM_linux+0x00540000)
	#endif
#endif



#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
//LEN_SaveRam BYTE 从SRAM复制的掉电保护数据,上电时不清0故放在最前以便上电时其他数据清0地址计算

#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#if  NAND_1G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (16*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#if  NAND_2G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (16*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#if  NAND_4G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#if  NAND_8G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#if LEN_msFILEunltFlags>=LEN_msFILEmanage
	#error
#endif
//LEN_msFILEunltFlags 块号:0=未使用,1=占用但无下块块号,2-8191下块的块号
//LEN_msFILEmanage-标志表-12字节 文件入口表
	//文件入口表1
	//...
	//文件入口表n
//12BYTE
	//4byte 文件分配单元计数
	//4byte有效标识0x12345678
	//4byte CRC32


//256K或768K 复制(当前仅用于手动强制复位时数据恢复用)

typedef __packed struct
{
	u16 BLOCK_ENTRY;//文件入口:nand flash块号,0=空;RAM 开始地址高2字节(值有效时!=0)
	u16 BLOCK_END;//文件结束:nand flash块号,0=空;RAM 开始地址低2字节
	u32 LEN_FILE;//文件预留占有存储空间总长度
	u32 LEN_DATA;//文件有效数据总长度
	
	u32 FILEID;//FILEID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	//文件分类:
	//0=普通采集方案数据记录
	//1=
	//2=按OI分类的事件记录
	//
	//
	u16 FILECOUNT;//相同FILEID的文件计数号
	u16 NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
	u16 NUM_FAIL;//文件头中写入抄表失败地址个数
	u8 save_date_time_s[7];//采集存储时标
	u8 start_date_time_s[7];//采集启动时间
#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
//GPRS,ENET各自上报
	u16 NUM_GPRSreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
	u16 NUM_ENETreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
#else
//GPRS,ENET合并上报
	u16 NUM_GPRSorENETreport[NUMmax_601C];//已上报的数据个数(最大上报方案数)
#endif
#ifndef LINUX_CROSS_GCC
}msFILE_TypeDef;
#else
}__attribute__ ((packed)) msFILE_TypeDef;
#endif
#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)//128K(BKPSDRAM) NAND FLASH写缓冲(仅在数据检查错误时清0)
//2BYTE 总块数
//4BYTE 总字节数(全部字节,也即下块的开始地址偏移)
//数据块1
//4BYTE 写入NAND FLASH的目的地址
//4BYTE 数据字节数
//4BYTE 已部分写入的字节数
//nBYTE 数据
//...
//数据块n
typedef __packed struct
{
	u32 ADDR_Dest;//4BYTE 写入NAND FLASH的目的地址
	u32 LEN_Data;//4BYTE 数据字节数
	u32 LEN_Write;//4BYTE 已部分写入的字节数
	u8 Data[1];//nBYTE 数据
#ifndef LINUX_CROSS_GCC
}BKPSDRAM_BLOCK_TypeDef;
#else
}__attribute__ ((packed)) BKPSDRAM_BLOCK_TypeDef;
#endif
typedef __packed struct
{
	u16 NUM_BLOCK;//2BYTE 总块数
	u32 LEN_TOTAL;//4BYTE 总字节数(全部字节,也即下块的开始地址偏移)
	//数据块
#ifndef LINUX_CROSS_GCC
}BKPSDRAM_TypeDef;
#else
}__attribute__ ((packed)) BKPSDRAM_TypeDef;
#endif
#define ADDR_SDRAM_CLR_START     (ADDR_BKPSDRAM+(128*1024))//SDRAM数据清0开始地址


#define ADDR_RAMmsFILEmanage     ADDR_SDRAM_CLR_START//存储在RAM中的文件管理
#define LEN_RAMmsFILEmanage     (8*1024)//(4*1024)
#define LEN_RAMmsFILEblock     (32*1024)//RAMmsFILE文件块长度


#define ADDR_emWin     ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
//128K emWin编译区
#define ADDR_128KDATABUFF     (ADDR_emWin+(128*1024))
//128K DATABUFF
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
//128K字节NAND FLASH块写缓冲
#define ADDR_NAND_PAGEREAD_BUFF     (ADDR_128KWRITE_BUFF+(128*1024))
//2048字节NAND FLASH页读缓冲
#define ADDR_ECC_BUFF     (ADDR_NAND_PAGEREAD_BUFF+2048)
//128+128字节ECC缓冲

#define ADDR_MACADDR_ascii     (ADDR_ECC_BUFF+256)//本机MAC地址的ascii码
//20BYTE 格式"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     (ADDR_MACADDR_ascii+20)//本机IP4地址的ascii码
//16BYTE 格式"192.168.0.100"
#define ADDR_IP4MASK_ascii     (ADDR_IP4ADDR_ascii+16)//本机IP4 MASK地址的ascii码
//16BYTE 格式"255.255.255.0"
#define ADDR_IP4GATEWAY_ascii     (ADDR_IP4MASK_ascii+16)//本机IP4 GATEWAY地址的ascii码
//16BYTE 格式"192.168.0.254"
#define ADDR_IP4PRIMARY_DNS_ascii     (ADDR_IP4GATEWAY_ascii+16)//本机IP4 PRIMARY_DNS的ascii码
//16BYTE 格式"8.8.8.8"
#define ADDR_IP4SECONDARY_DNS_ascii     (ADDR_IP4PRIMARY_DNS_ascii+16)//本机IP4 SECONDARY_DNS的ascii码
//16BYTE 格式"8.8.4.4"
#define ADDR_IP6ADDR_ascii     (ADDR_IP4SECONDARY_DNS_ascii+16)//本机IP6地址的ascii码
//40BYTE
#define ADDR_IP6GATEWAY_ascii     (ADDR_IP6ADDR_ascii+40)//本机IP6 GATEWAY地址的ascii码
//40BYTE
#define ADDR_IP6PRIMARY_DNS_ascii     (ADDR_IP6GATEWAY_ascii+40)//本机IP6 PRIMARY_DNS的ascii码
//40BYTE
#define ADDR_IP6SECONDARY_DNS_ascii     (ADDR_IP6PRIMARY_DNS_ascii+40)//本机IP6 SECONDARY_DNS的ascii码
//40BYTE


#define ADDR_UARTnCtrl_Start     (ADDR_IP6SECONDARY_DNS_ascii+40)//UART
//UART0
#define ADDR_UART0     (ADDR_UARTnCtrl_Start)
#define ADDR_UART0IntRx     ADDR_UART0
#define ADDR_UART0Ctrl     (ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     (ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     (ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     (ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     (ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     ADDR_UART1
#define ADDR_UART1IntRx     (ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     (ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     (ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     (ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     (ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     (ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#define ADDR_UART2     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#define ADDR_UART2IntRx     ADDR_UART2
#define ADDR_UART2Ctrl     (ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     (ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     (ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     (ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     (ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     ADDR_UART3
#define ADDR_UART3Ctrl     (ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     (ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     (ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     (ADDR_UART3Tx+LEN_UART3TxMax)
//UART4(集中器载波,专变控制模块)
#define ADDR_UART4     (ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#define ADDR_UART4IntRx     ADDR_UART4
#define ADDR_UART4Ctrl     (ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     (ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     (ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     (ADDR_UART4Tx+LEN_UART4TxMax)

#define ADDR_UART4RxCopy     (ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)//集中器载波通道主动上报帧缓存
#define LEN_UART4RxCopy     128

//UART5
#define ADDR_UART5     (ADDR_UART4RxCopy+(LEN_UART4RxCopy*NUM_UART4RxCopyMax))
#define ADDR_UART5IntRx     ADDR_UART5
#define ADDR_UART5Ctrl     (ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     (ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     (ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     (ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     (ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     ADDR_UART6
#define ADDR_UART6Ctrl     (ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     (ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx     (ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     (ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(终端ESAM)
#define ADDR_UART7     (ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     ADDR_UART7
#define ADDR_UART7Ctrl     (ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     (ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     (ADDR_UART7Rx+LEN_UART7Rx)
#define ADDR_UART7FnDataBuff     (ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(电表ESAM)
#define ADDR_UART8     (ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     ADDR_UART8
#define ADDR_UART8Ctrl     (ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx     (ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     (ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     (ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(电表CARD)
#define ADDR_UART9     (ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     ADDR_UART9
#define ADDR_UART9Ctrl     (ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     (ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     (ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff     (ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(以太网)
#define ADDR_UART10     (ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     ADDR_UART10
#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     (ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     (ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     ADDR_UART11
#define ADDR_UART11Ctrl     (ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     (ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     (ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     (ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(虚拟口)
#define ADDR_UART12     (ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     ADDR_UART12
#define ADDR_UART12Ctrl     (ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     (ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     (ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     (ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART13     (ADDR_DATABUFF)
#define ADDR_UART13IntRx     ADDR_UART13
#define ADDR_UART13Ctrl     (ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     (ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     (ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     (ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART14     (ADDR_DATABUFF)
#define ADDR_UART14IntRx     ADDR_UART14
#define ADDR_UART14Ctrl     (ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     (ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     (ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     (ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART15     (ADDR_DATABUFF)
#define ADDR_UART15IntRx     ADDR_UART15
#define ADDR_UART15Ctrl     (ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     (ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     (ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     (ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART16     (ADDR_DATABUFF)
#define ADDR_UART16IntRx     ADDR_UART16
#define ADDR_UART16Ctrl     (ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     (ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     (ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     (ADDR_UART16Tx+LEN_UART16TxMax)

#define ADDR_UARTnCtrl_End     (ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)//(ADDR_UART19FnDataBuff+LEN_UART19FnDataBuff)

typedef __packed struct
{
	u8 Run;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
	u8 res2;//
	u8 StdTime[7];//任务频率点标准时间YYMDHMS,
	u8 StartTime[7];//任务执行开始时间YYMDHMS,
	u8 EndTime[7];//任务执行结束时间YYMDHMS
	u8 res1;
#ifndef LINUX_CROSS_GCC
}UARTnTaskTime_TypeDef;
#else
}__attribute__ ((packed)) UARTnTaskTime_TypeDef;
#endif
#define LEN_UARTnTask     24//需半字对齐,清0时用16bit写
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
#ifndef LINUX_CROSS_GCC
}RMAddrList_TypeDef;//UART抄表地址列表
#else
}__attribute__ ((packed)) RMAddrList_TypeDef;
#endif
#define LEN_RMAddrList     6
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 ReRMCount;//连续重发次数计数
	u8 SpaceData;//0=失败,1=,2=抄表正常但数据为空标志失败(台体测试采集监控计为抄表失败)
#ifndef LINUX_CROSS_GCC
}RMokfailLIST_TypeDef;//抄表成功失败地址列表
#else
}__attribute__ ((packed)) RMokfailLIST_TypeDef;
#endif
#define LEN_RMokfailLIST     8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u16 LEN_DATA;
#ifndef LINUX_CROSS_GCC
}RecordFileHead_TypeDef;//抄表记录文件头
#else
}__attribute__ ((packed)) RecordFileHead_TypeDef;
#endif
#define LEN_RecordFileHead     8
typedef __packed struct
{
	u8 OAD[4];
	u8 LEN_data[2];
	u8 REPORT;//上报标志:0=不需上报,1=需上报，2=已上报
	u8 res1;
#ifndef LINUX_CROSS_GCC
}CongealFileHead_TypeDef;//冻结文件头
#else
}__attribute__ ((packed)) CongealFileHead_TypeDef;
#endif
#define LEN_CongealFileHead     8
typedef __packed struct
{
	u8 LEN_data[2];//总长度,事件数据+OAD顺序表(array OAD)
	u8 LEN_event[2];//事件数据长度
	u8 REPORT;//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
	u8 OCCUR;//产生恢复:0=恢复,1=产生,2=无
	u8 ACK;//上报确认:0=没确认,1=确认
	u8 res1;
#ifndef LINUX_CROSS_GCC
}EventFileHead_TypeDef;//事件记录文件头
#else
}__attribute__ ((packed)) EventFileHead_TypeDef;
#endif
#define LEN_EventFileHead     8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 AddrType;//头文件中记录的地址类:0=电表地址,1=采集器地址
	u8 LEN_DATA;
#ifndef LINUX_CROSS_GCC
}SearchMeterFileHead_TypeDef;//搜表记录文件头
#else
}__attribute__ ((packed)) SearchMeterFileHead_TypeDef;
#endif
#define LEN_SearchMeterFileHead     8









typedef __packed struct
{
	u8 APPVER[2];//  期望的应用层协议版本号    long-unsigned，
	u8 Protocol[8];// 	期望的协议一致性块        ProtocolConformance，
	u8 Function[16];//  期望的功能一致性块        FunctionConformance，
	u8 Len_Tx[2];//  客户机发送帧最大尺寸      long-unsigned，
	u8 Len_Rx[2];//  客户机接收帧最大尺寸      long-unsigned，
	u8 NUM_RxFrame;//  客户机接收帧最大窗口尺寸  unsigned，
	u8 Len_APDU[2];//  客户机最大可处理APDU尺寸 long-unsigned，
	u8 ConnectOVERTime[4];//  期望的应用连接超时时间    double-long-unsigned，
	u8 ConnectMechanismInfo;//  认证请求对象              ConnectMechanismInfo
	//ConnectMechanismInfo∷=CHOICE
	//{
	//  公共连接    [0] NullSecurity，
	//  一般密码    [1] PasswordSecurity，
	//  对称加密    [2] SymmetrySecurity，
	//  数字签名    [3] SignatureSecurity
	//}
#ifndef LINUX_CROSS_GCC
}ClientConnectInfo_TypeDef;//客户机连接信息
#else
}__attribute__ ((packed)) ClientConnectInfo_TypeDef;
#endif
#define LEN_ClientConnectInfo     38

typedef __packed struct
{
//	服务器厂商版本信息        FactoryVersion，
	u8 code[4];//	厂商代码         visible-string(SIZE (4))
	u8 sver[4];//	软件版本号      visible-string(SIZE (4))
	u8 svdate[6];//	软件版本日期   visible-string(SIZE (6))
	u8 hver[4];//	硬件版本号      visible-string(SIZE (4))
	u8 hvdate[6];//	硬件版本日期   visible-string(SIZE (6))
	u8 Info[8];//	厂家扩展信息   visible-string(SIZE (8))
	u8 APPVER[2];//  商定的应用层协议版本号    long-unsigned，
	u8 Protocol[8];//  商定的协议一致性块        ProtocolConformance，
	u8 Function[16];//  商定的功能一致性块        FunctionConformance，
	u8 Len_Tx[2];//  服务器发送帧最大尺寸      long-unsigned，
	u8 Len_Rx[2];//  服务器接收帧最大尺寸      long-unsigned，
	u8 NUM_RxFrame;//  服务器接收帧最大窗口尺寸  unsigned，
	u8 Len_APDU[2];//  服务器最大可处理APDU尺寸 long-unsigned，
	u8 ConnectOVERTime[4];//  商定的应用连接超时时间    double-long-unsigned，
	u8 ConnectResult;//应用连接请求认证的结果
	//ConnectResult∷=ENUMERATED
	//{
	//  允许建立应用连接     （0），
	//密码错误             （1），
	//对称解密错误         （2），
	//非对称解密错误       （3），
	//签名错误             （4），
	//协议版本不匹配       （5），
	//其他错误             （255）
	//}
#ifndef LINUX_CROSS_GCC
}ServerConnectInfo_TypeDef;//服务机连接信息
#else
}__attribute__ ((packed)) ServerConnectInfo_TypeDef;
#endif
#define LEN_ServerConnectInfo     70




typedef __packed struct
{
//通用部分
  u8 Task;//主任务:
					//0=正在接收,1=接收完成,2=等待单帧数据发送,3=单帧数据正在发送
					//4=多帧数据续接收完成,5=等待多帧数据发送,6=多帧数据正在发送
					//7=等待发送器空转为接收
          //8=等待通过其它口数据转发,9=等待启动发送延时,10=正在通过其它口发送数据,11=正在通过其它口接收数据
          //20=级联接收完成,21=启动转发级联数据,22=正在转发级联数据,23=接收级联数据
 					//
	u8 Lock;//0x55=通信功能正在被调用禁止其他程序再次调用
  u16 RxByte;//接收字节数(半字对齐)
  u16 TxByte;//发送字节数(半字对齐)
  u16 TxCount;//已发送计数(半字对齐)
  u8 Safety;//安全认证(交采用)
	u8 SECURITY_Request;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
  u16 BpsCtrl;//通信控制字(同规约2007)(半字对齐)
			//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//D4位表示停止位位数0/1分别表示1/2个停止位
			//D3位表示有无校验位0/1分别表示无/有
			//D2位表示偶(Even)／奇(Odd)校验位0/1分别表示偶／奇校验
			//D1-D0 数据位数0～3分别表示5-8位
  u8 Protocol;//通信规约自动识别:0=698.45,1=DL/T645-2007
  u8 NewBps;//波特率更新标志;当为MESAM通道时为MESAM内部数据更新标志0=更新,其他值没更新
  u8 SubTask;//子任务
  u8 AddrType;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
	//共16Byte
//终端专用部分
	u32 RxTxByte;//通信流量
	u16 PORT;//本机端口号(半字对齐),同时用于掉电时已连接过1次识别
	u8 TCPUDP;//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
	u8 LinkCtrl;//连接控制0=不连接,1=连接
	u8 LinkCount;//连接计数(2次主IP,1次备用IP)
	u8 ReLinkNum;//被动激活模式重拨次数,收到被动激活命令时置初值
	u8 LinkTask;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	u8 LinkSubTask;//链接子任务
	u8 HeartberatCount;//心跳发出计数,收到确认帧清0
	u8 HostACK;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
	u8 HostCount;//主动上报重发计数(心跳发出计数另计)
	u8 TimerTaskCount;//主动上报定时任务计数
  u8 RxSEQ;//接收帧序列域SEQ
  u8 TxSEQ;//启动帧序列域SEQ
	
	u8 ReNewLink;//0x55=通信参数变化当前帧发送完成后重新连接
	u8 res3;//
	u8 res4;//
	u8 ATTask;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	u8 ATWriteTask;//AT数据发送任务:0=发命令,1=发数据
	u8 ATCount;//AT计数
	
	u8 LinkTask2;//通道2链接任务(FTP命令)
	u8 FTPLinkCount;//FTP尝试连接计数
	u8 Task2;//通道2任务(FTP命令)
	u8 FTPErr;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
	u32 FTPByte;//FTP当前文件已完成下载字节数
	u32 FTPFileLen;//FTP当前文件总长度
	u16 TxAPDUSEQ;//链路层分帧序号计数0-4095
	u16 TxAPDUByte;//链路层分帧已发送APDU字节数
	//共40Byte
//集中抄表专用部分
	u8 LINKCHECK_RTCS;//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
	u8 RTCS_TASK;//实时钟更新标志原始秒(任务运算)
	u8 RTCM;//实时钟更新标志原始分
	u8 RTCh;//实时钟更新标志原始时
	u8 RTCD[2];//实时钟日更新标志的原始月日
	u8 RTCW[2];//实时钟星期更新标志的原始月日
	u8 res7;
	u8 Lock_InfoTask;//读路由网络拓扑信息等任务:0x55=锁定
	u8 InfoTask;//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
	u8 LastAutoSearchRTCD;//上次自动搜表实时钟日
	u8 AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
	u8 AutoSearchReCount;//自动搜表任务重发计数
	u16 AutoSearchTime;//自动搜表持续时间分，0表示不限时间直至搜表结束
	u16 TransferDelay;//线路上传输延时时间秒
	u8 NETOPENCount;//开启服务连续次数计数(当前次数达到x次时关模块电源)
	u8 ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
	u16 SMSRxSEQ;//短信接收接口;B0-B11接收帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	u16 SMSTxSEQ;//短信发送接口;B0-B11发送帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	//共24Byte
//DL698
	u8 Function;//口功能 0=终端维护口,!=0终端抄表口
	u8 FunctionTimer;//上次口功能检查的实时钟秒
	u8 PowerUpDealy;//上电延时:0=没延时,!=0已到延时
	u8 DHCPTask;//DHCP任务
	s32 TCPSocket;
	s32 UDPSocket;
	u8 NET_ADDR[20];//NET_ADDR
	u8 GET_Reques_CHOICE;//原得到请求选择
	u8 PIID;
	u8 DAR;//DL698数据访问结果
	u8 NEXT;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	u32 pRxBuff;//DL698分帧传输接收缓冲指针寄存	
	u32 NEXTOADNum;//DL698分帧还需传输的OAD个数
	u32 NEXTOADSubNum;//DL698分帧传输OAD内子计数

	u8* pTxData;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度(包括本身2byte)
	u32 LEN_TxData;//需发送数据总长度(包括每段前的2字节段长度)
	u32 LEN_ReadData;//已读取的发送数据总长度(包括每段前的2byte段长度)
	u16 NEXTBlock;//DL698分帧传输过程的帧序号，取值范围0~4095
	
	u8 CONNECT;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	u8 RMMeterCountAuto;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
  u8 RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
	u8 RMCSDCount;//抄表CSD计数
	u8 RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
	u8 RMCSDSubCount;//抄表CSD子计数(非DL698抄表时)
	u8 RMCSDCountMax;//抄表CSD最大个数
	u8 RMROADCountMax;//抄表CSD的ROAD内有关联的OAD最大个数(非DL698抄表规约库设定)
	u8 RMCSDSubCountMax;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
	u8 RMROADCountOK;//抄表写文件数据缓冲中ROAD时已填入关联的OAD数据个数计数
	u8 ReRMCount;//抄表重发计数
	u8 RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
	u8 RM_DI_CHOICE;//抄表DI选择:0=主DI,1=备用DI
	u8 SetChange6000;//采集档案配置表有变化标志:0=没变化,0x55=变化
	u32 TaskID;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	u32 RM_DI;//DL/T645-2007/1997当前抄表数据标识
	u16 res8;//
	u16 RM_DI_N;//采集上第N次
	u8 res9;
	u8 RM_DI_TI[3];//按时标间隔采集(高位数在低字节同698)
	u16 RM_DI_TI_Count;//按时标间隔采集时的间隔计数
	u16 RM_SubsequentFrames;//0=无读后续帧,1-n=读续帧的帧号
	u16 TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
  u8 RM_Rx_date_time_s[8];//抄表接收到的数据时标(无数据类型),7byte,仅为对齐定义8byte
  u8 RM_Rx_date_40020200[10];//抄表接收到的数据事件次数暂存及用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
	
	u8 ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
	u8 ProxyPORTn;//发起代理的端口号
	u8 ProxyBpsCtrl;//代理通信控制字(同规约2007)(半字对齐)
	u8 ProxyRMprotocol;//代理抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	u16 ProxyTxRxByte;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
	u16 ProxyOverTime;//代理1个服务器的超时时间秒
	u16 REPORT_OVER_SECOND;//上报响应超时时间寄存
	u8 REPORT_NUMmax;//若主站没应答最大还需重发上报次数,每上报1次减1
	u8 REPORT_RTCS;//上报空闲时进入的RTC秒
	u8 REPORT_PIID;//上报启动帧PIID
	u8 REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
	u16 REPORT_filename[REPORT_NUMFILEmax];//1帧主动上报数据不同文件名列表
	u16	REPORT_NUM_Data[REPORT_NUMFILEmax];//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
#define LEN_REPORT_file   (4*REPORT_NUMFILEmax)
	//122+LEN_REPORT_file byte
	u8 EVENT_FAIL;//设备故障事件记录(485或载波通道),0=空或恢复,1=产生
	u8 BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
	u8 LastBroadcastRTCD2;//上次执行广播校时-2的实时钟日
	u8 LastBroadcastRTCD3;//上次执行广播校时-3的实时钟日
	u8 BroadcastTimeProxyTask;//代理透明转发广播校时任务:0=空,1=有代理
	u8 RM_Rx_date_NULL;//抄表接收返回:0=有数据,!=0无数据(空数据)
	
	u16 AutoReportCopyNUM;//主动上报接收已经缓存帧数
	u16 SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
	u8 NUM_SomeTaskID;//当前同类采集任务的id号的列表个数
	u8 NUM_LastSomeTaskID;//上最近次同类采集任务的id号的列表个数
	u16 NUM_SetMeter;//本端口在档案中配置的仪表总数
	u16 res23;//
	u32 res24;//
	u32 res35;//
	u32 res36;//
	u8 Rand[16];//抄表安全模式时发送给电表的16字节随机数
	u8 OAD_44000400[2];//应用连接:当前连接的客户机地址∷=unsigned
	//54 byte
	u8 ConnectInfo[80];//连接信息
#if ((LEN_ClientConnectInfo>80)||(LEN_ServerConnectInfo>80))
	#error
#endif
#if (LEN_REPORT_file+80)>512
	#error
#endif
	u8 res104[512-(LEN_REPORT_file+80)];
	
	u8 AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
	u8 AutoReportRMprotocol;//电能表主动上报规约=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	u8 AutoReportRMAddr[6];//电能表主动上报地址(直接读接收帧有645和698二种地址顺序)
	u8 AutoReportData[248];//电能表主动上报数据寄存
		//DL645:12byte状态字寄存用于发复位命令+96byte对应B0-B95主动上报事件新增次数hex
		//DL698:
	
//以下仅698抄表任务专用,非抄表口不需要(错了,主动上报任务有用到)
	//byte=LEN_UARTnTask*(RMTASKmax+ExtRMTASKmax)
	UARTnTaskTime_TypeDef TaskTime[RMTASKmax+ExtRMTASKmax];
	//byte=(15*(RMTASKmax+ExtRMTASKmax))+128    (非抄表口)
	//byte=(15*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax)   (抄表口)
	u16 MeterCount[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
	u8 RMLOOP[RMTASKmax+ExtRMTASKmax];//RMTASKmax byte 全部电能表抄读一遍的遍数计数,任务号数组
	u16 NUMrmTOTAL[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte各任务需采集电能表总数,主动上报时用于文件搜索计数
	u16 NUMrmOK[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte各任务已采集完成电能表总数
	u16 NUMrmFAIL[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte各任务已采集失败电能表总数
	u16 NUMrmBAD[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte各任务已采集完成的电能表数中因配置变更等原因不再需抄读的数
	u8 date_time_s[7];//当前记录文件中已抄读地址列表时的!!!数据存储时标(无数据类型),7byte,仅为对齐定义8byte
	u8 TSA[3];//TSA的数据类型长度等开始3字节,用于事件产生源
	u16 RMAddr[3];//6 byte 当前正在采集通信地址(高位数在低字节)(无数据类型),全0表示无效地址
	u8 RMOADROAD;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
#define LEN_RMCSD     (128-17)
	u8 RMCSD[LEN_RMCSD];//128-x byte 当前正在抄的CSD(有数据类型)
	RMokfailLIST_TypeDef rmOKFAILlist[RMTASKmax+ExtRMTASKmax][NUM_RMmax];//抄表成功失败地址列表
	u16 SomeTaskid[RMTASKmax+ExtRMTASKmax];//RMTASKmax*2 byte 当前同类采集任务的b15-b8任务号,b7-b0方案编号
	u16 LastSomeTaskid[RMTASKmax+ExtRMTASKmax];//RMTASKmax*2 byte 上最近次同类采集任务的b15-b8任务号,b7-b0方案编号
#ifndef LINUX_CROSS_GCC
}UARTCtrl_TypeDef;
#else
}__attribute__ ((packed)) UARTCtrl_TypeDef;
#endif

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





#define ADDR_RMTaskID     ADDR_UARTnCtrl_End//当前待执行的抄表任务列表
//4byte 有效RMTaskID个数
//4*NUMmax_6012_2 byte TaskID
#define ADDR_REPORTTaskID     ADDR_RMTaskID+(4+(4*NUMmax_6012_2))//当前待执行的上报任务列表
//4byte 有效REPORTTaskID个数
//4*NUMmax_6012_2 byte TaskID
#define ADDR_AddrList     ADDR_REPORTTaskID+(4+(4*NUMmax_6012_2))//地址列表
//2byte 列表数
//typedef __packed struct
//{
//	u16 Addr0;
//	u16 Addr1;
//	u16 Addr2;
//	u16 LEN_DATA;
//}RecordFileHead_TypeDef;//抄表记录文件头
#define ADDR_1kDATABUFF     ADDR_AddrList+4+(NUM_RMmax*LEN_RecordFileHead)//ADDR_AddrList+2+(NUM_RMmax*LEN_RecordFileHead)
//1k byte 主要为RCSD为全选时和设置事件时的OAD列表
#define LEN_1kDATABUFF     1024

#define ADDR_Router     ADDR_1kDATABUFF+LEN_1kDATABUFF//路由器(集中器载波)
typedef __packed struct
{
	u8 RouterAddr[6];//路由主节点地址(总是放在最前,通信库地址定位用)
	u8 RouterCODE[4];//厂商代码 ASCII 2,芯片代码 ASCII 2
	u8 RouterVer[5];//路由器版本号,分别为:日,月,年,2BYTE版本

	u8 RouterCtrl;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
	u8 RouterCtrlTask;//路由器控制任务计数;完成后自动回到0
	u8 ReRouterCtrl;//路由器控制重发计数
	u8 RUNState;//从模块读取的当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
	u8 UpDateTask;//软件更新任务
	u8 UpDateReCount;//软件更新重试次数
	u8 MinuteCollect;

	u8 RouterInitTask;//路由器初始化任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	u8 RouterInitSubTask;//路由器初始化子任务
	u8 RouterInitReCount;//路由器初始化命令重发计数
	u8 NUM_Enable_Concurrency;//宽带载波当前还允许并发帧数
	u8 MaxOverTime;//从节点监控最大超时时间秒
	//#if (USER/100)==17//江苏系
//	u8 MinuteCollect;//是否支持分钟采集
//	u16 NoteNum1;
 //#endif
	
	u8 RouterSynTask;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
	u16 RMListCount;//抄表地址的列表号自动分配计数0-(NUM_RMLISTmax-1)

	u16 ModeNoteNum;//档案同步时从模块读取的模块返回当前从节点数(半字对齐)
	u16 NoteNum;//当前路由器中已知总从节点数(半字对齐)
	u16 TempCount;//删除或添加节点等临时计数(半字对齐)
	u16 UpDateCount;//软件更新帧计数(半字对齐)
	u16 ReadNoteNum;//档案同步时从模块已读取的节点数(半字对齐)
	u16 RS485Time;//采集器搜485电表定时值分(BCD)(半字对齐)
	u16 KeepTime;//注册持续时间定时值分(半字对齐)
	u8 n;//轮次1-3=立即搜表,0=每日搜表
	u8 RMTD;//0=非抄表时段,1=抄表时段
	u8 NoteNumOver;//AFN04F10配置的从节点数>模块支持的最大从节点数;0=否,1=是;仅用于无线模块在显示正在组网时显示超限信息
	u8 res4;
	u16 NoteNum1;
	u16 BandRate;
	u32 StopFlags;//暂停路由标志:(字对齐)
		//b0=任务号0抄停电事件,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
		//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
		//b16=路由软件更新,b17=抄主动上报事件,b18=,b19=,b20=,b21=,b22=,b23=
	u8 RouterRunMode_1;//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	u8 RouterRunMode_2;//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
	u8 RouterRunMode_3;//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
	u8 RouterRunMode_4;//路由运行模式_周期抄表模式： 01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
	u8 RouterRunMode_5;//路由运行模式_传输延时参数支持：由D2、D1、D0等三个位表示，分别代表广播、从节点监控、路由主动抄表等三种命令支持向集中器提供传输延时参数的情况，1表示支持提供传输延时参数，0表示不支持
	u8 RouterRunMode_6;//路由运行模式_失败节点切换发起方式：D3置1表示通信模块自主切换待抄节点，D4置1表示集中器发起通知通信模块切换待抄节点
	u8 RouterRunMode_7;//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
	u8 RouterRunMode_8;//路由运行模式_广播命令信道执行方式：0表示执行广播命令不需要信道标识，1表示广播命令要根据报文中的信道标识逐个来发送，其它取值保留
	u16 MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
	u16 MaxTxByte;//最大支持报文长度(半字对齐)
	u16 MaxFileByte;//文件传输支持的最大单个数据包长度(半字对齐)
	u16 MaxUpDateTime;//升级操作最大等待时间秒(半字对齐)
	u16 MaxNoteNum;//支持的最大从节点数(半字对齐)
	u8 RadioChannel;//无线信道组:0～63，64～253：保留，254：自动选择，255：保持不变
	u8 RadioPower;//无线主节点发射功率:00：最高发射功率，01：次高发射功率，02：次低发射功率，03：最低发射功率，04～254：保留，255：保持不变
	
	u8 res7[52];
#ifndef LINUX_CROSS_GCC
}Terminal_Router_TypeDef;
#else
}__attribute__ ((packed)) Terminal_Router_TypeDef;
#endif
#define LEN_Router     128//路由器控制区数据长度
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

#define ADDR_NoteList     (ADDR_Router+LEN_Router)//路由器中节点表
typedef __packed struct
{
	u8 Addr[6];//从节点地址(字对齐)
	u8 CollectorAddr[6];//采集器地址
#ifndef LINUX_CROSS_GCC
}Note_TypeDef;
#else
}__attribute__ ((packed)) Note_TypeDef;
#endif
#define LEN_NoteList     12//每列表字节数

typedef __packed struct
{
  Note_TypeDef     List[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}NoteList_TypeDef;
#else
}__attribute__ ((packed)) NoteList_TypeDef;
#endif
#define NoteList     ((NoteList_TypeDef *)(ADDR_NoteList))//载波路由器中节点表(按读路由和添加顺序排列)


#define ADDR_RMList     (ADDR_NoteList+(LEN_NoteList*NUM_RMmax))//抄表计数列表(以前:按档案配置序号从0开始)
typedef __packed struct
{
 u16 RMAddr[3];//鍘熷湴鍧�
 u8 CONNECT;//DL698杩炴帴:0=绌?1=鍚姩杩炴帴锛?=锛?00=杩炴帴,101=鎶勮〃涓?102=鍏抽棴杩炴帴
  u8 RMprotocol;//鎶勮〃瑙勭害绫诲瀷=鏈煡 (0)锛孌L/T645-1997锛?锛夛紝DL/T645-2007锛?锛夛紝DL/T698.45锛?锛夛紝CJ/T 188-2004锛?锛?
 u8 RMCSDCount;//鎶勮〃CSD璁℃暟
 u8 RMROADCount;//鎶勮〃CSD鐨凴OAD鍐呭叧鑱旂殑OAD璁℃暟(闈濪L698鎶勮〃鏃?
 u8 RMCSDSubCount;//鎶勮〃CSD瀛愯鏁?闈濪L698鎶勮〃鏃?
 u8 RMCSDCountMax;//鎶勮〃CSD鏈�澶т釜鏁?
 u8 RMROADCountMax;//鎶勮〃CSD鐨凴OAD鍐呮湁鍏宠仈鐨凮AD鏈�澶т釜鏁?闈濪L698鎶勮〃瑙勭害搴撹瀹?
 u8 RMCSDSubCountMax;//鎶勮〃CSD鏈夊瓙鎶勮〃涓暟(闈濪L698鎶勮〃瑙勭害搴撹瀹?
 u8 RMROADCountOK;//鎶勮〃鍐欐枃浠舵暟鎹紦鍐蹭腑ROAD鏃跺凡濉叆鍏宠仈鐨凮AD鏁版嵁涓暟璁℃暟
 u8 TASK;//骞跺彂浠诲姟:0=绌洪棽,1=瀹屾垚1甯у彂鏀?2=鎺ユ敹瓒呮椂(鎴栭敊璇?,3=鍚姩鍙戦�?4=姝ｅ湪鍙戦�?5=姝ｅ湪鎺ユ敹,6=鎶勮〃鍙戦�?鍚孯S485鐨?=鍚姩鍙戦�?,7=鍋滅敤(涓轰簡鑳芥墽琛屼笅涓垪琛ㄥ彿)
 u8 TIMER_S;//骞跺彂鍚姩鍙戦�佸悗绛夊緟鎺ユ敹绉掑畾鏃?
 u8 TxSEQ;//骞跺彂鍚姩鍙戦�佸抚搴忓彿
 u8 RM_OKFAIL;//鎶勮〃鎴愬姛澶辫触鏍囧織;=0澶辫触锛?1鎴愬姛,2=鎴愬姛浣嗘棤鏁版嵁
 u8 RM_DI_CHOICE;//鎶勮〃DI閫夋嫨:0=涓籇I,1=澶囩敤DI
 u32 RM_DI;//DL/T645-2007/1997褰撳墠鎶勮〃鏁版嵁鏍囪瘑
 u16 RM_DI_TI_Count;//鎸夋椂鏍囬棿闅旈噰闆嗘椂鐨勯棿闅旇鏁?
 u16 RM_SubsequentFrames;//0=鏃犺鍚庣画甯?1-n=璇荤画甯х殑甯у彿
  u8 RM_Rx_date_time_s[8];//鎶勮〃鎺ユ敹鍒扮殑鏁版嵁鏃舵爣(鏃犳暟鎹被鍨?,7byte,浠呬负瀵归綈瀹氫箟8byte
  u8 RM_Rx_date_40020200[10];//鎶勮〃鎺ユ敹鍒扮殑鏁版嵁浜嬩欢娆℃殏瀛樺強鐢ㄤ簬MAC楠岃瘉鐨勭數鑳借〃琛ㄥ彿;0th=鐘舵�?0=绌?1=姝ｅ湪鎶?2=瀹屾垚),1th=琛ㄥ彿闀垮害(<=6),2th-7th琛ㄥ彿鍊?
 u8 RMOADROAD;//鎶勮〃浠诲姟鏈夋妱杩囩殑OAD绫诲瀷:b0=鏈塐AD,b1=鏈塕OAD,b3-b7淇濈暀;鐢ㄤ簬鎶勮〃鏁版嵁瀛樿串鏃舵槸瀛樺湪RAM杩樻槸NAND
 u8 Rand[16];//鎶勮〃瀹夊叏妯″紡鏃跺彂閫佺粰鐢佃〃鐨?6瀛楄妭闅忔満鏁?
 u8 RMCSD[LEN_RMCSD];//128-x byte 褰撳墠姝ｅ湪鎶勭殑CSD(鏈夋暟鎹被鍨?
 u8 FnDataBuff[LEN_UART4FnDataBuff];//Fn鏁版嵁缂撳啿
 #if (USER/100)==17//姹熻嫃绯?
 u16 collectoraddr[4];
 u8 PIID;
 u8 res23;
 u16 res24;
 #endif
#ifndef LINUX_CROSS_GCC
}RMCount_TypeDef;
#else
}__attribute__ ((packed)) RMCount_TypeDef;
#endif
#if (USER/100)!=17//涓嶆槸姹熻嫃绯?
#define LEN_RMLIST     (63+LEN_RMCSD+LEN_UART4FnDataBuff)//姣忓垪琛ㄥ瓧鑺傛暟
#else
#define LEN_RMLIST     (63+LEN_RMCSD+LEN_UART4FnDataBuff+12)//姣忓垪琛ㄥ瓧鑺傛暟
#endif

typedef __packed struct
{
  RMCount_TypeDef     List[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}RMList_TypeDef;
#else
}__attribute__ ((packed)) RMList_TypeDef;
#endif
#define RMLIST     ((RMList_TypeDef *)(ADDR_RMList))//载波路由器中节点表(按读路由和添加顺序排列)


//系统数据
typedef __packed struct
{
#define ADDR_Terminal_ERAM_Start     ((ADDR_RMList+(LEN_RMLIST*NUM_RMLISTmax))&0xfffffffc)//ERAM需半字对齐//ERAM需字对齐
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
	u16 RequestFileTimer;//1 请求文件传输MS定时器,每10MS减1定时器
  	u16 BUZZMSTimer;//2 蜂鸣器每10MS减1定时器
	u16 Trip1_MS_Timer;//3 终端跳闸1 10MS定时器;1200ms,>=1000跳,<1000合
	u16 Trip2_MS_Timer;//4 终端跳闸2 10MS定时器;1200ms,>=1000跳,<1000合
	u16 Trip3_MS_Timer;//5 终端跳闸3 10MS定时器;1200ms,>=1000跳,<1000合;微功率无线学习完成读路由状态定时器
	u16 Trip4_MS_Timer;//6 终端跳闸4 10MS定时器;1200ms,>=1000跳,<1000合;批量对时延时等待电表生成校时记录定时器
	u16 AT_MS_Timer;//7 GPRS AT命令用10MS定时器
	u16 GPRS_RxLED_MS_Timer;//8 GPRS 接收LED 10MS定时器
	u16 GPRS_TxLED_MS_Timer;//9 GPRS 发送LED 10MS定时器
	u16 RS485_1_TxLED_MS_Timer;//10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
	u16 RS485_1_RxLED_MS_Timer;//11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
	u16 RS485_2_TxLED_MS_Timer;//12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	u16 RS485_2_RxLED_MS_Timer;//13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
	u16 ENETLED_MS_Timer;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
	u16 GPRS_ONOFF_MS_Timer;//15 GPRS ONOFF脚控制 10MS定时器
	u16 RouterCtrl_MS_Timer;//16 路由控制恢复延时 10MS定时器
	//u16 CD485TxLED_MS_Timer;// 级联485发送LED指示MS定时器,每10MS减1定时器
	//u16 CD485RxLED_MS_Timer;// 级联485接收LED指示MS定时器,每10MS减1定时器
	//u16 DisplayScanTimer;// 显示扫描 每10MS减1定时器
	
#define Terminal_MS_Timer_Num     16//终端10MS定时器数
#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s定时器
	u16 S_PR;//1 60秒分频产生分
	u16 PORT0_S_Timer;//2 秒定时器
	u16 PORT1_S_Timer;//3 秒定时器
	u16 PORT2_S_Timer;//4 秒定时器
	u16 PORT3_S_Timer;//5 秒定时器
	u16 PORT4_S_Timer;//6 秒定时器
	u16 PORT5_S_Timer;//7 秒定时器
	u16 PORT6_S_Timer;//8 秒定时器
	u16 PORT7_S_Timer;//9 秒定时器
	u16 PORT8_S_Timer;//10 秒定时器
	u16 PORT9_S_Timer;//11 秒定时器
	u16 PORT10_S_Timer;//12 秒定时器
	u16 PORT11_S_Timer;//13 秒定时器
	u16 PORT12_S_Timer;//14 秒定时器
	u16 PORT13_S_Timer;//15 秒定时器
	u16 PORT14_S_Timer;//16 秒定时器
	u16 PORT15_S_Timer;//17 秒定时器
	u16 PORT16_S_Timer;//18 秒定时器

  u16 ENETHeartBeat_S_Timer;//19 以太网心跳秒定时器
	u16 IPLeaseTimer;//20 以太网自动获得IP租赁时间秒定时器
	u16 ENETCheck_S_Timer;//21 以太网IP冲突检查和租赁更新秒定时器
  u16 GPRSHeartBeat_S_Timer;//22 GPRS心跳秒定时器
	u16 GPRSAutoDiscon_S_Timer;//23 GPRS被动激活模式连续无通信自动断线秒定时器
	u16 GPRSReConnection_S_Timer;//24 GPRS永久在线、时段在线模式重拨间隔秒定时器
	u16 GPRSWaitConnection_S_Timer;//25 GPRS发起连接服务等待秒定时器
	u16 GPRSWaitTx_S_Timer;//26 GPRS发送数据等待超时秒定时器
	u16 GPRSCheck_S_Timer;//27 GPRS在线和信号检查秒定时器
	u16 ResetCommand_S_Timer;//28 终端收到复位命令延时执行秒定时器
	u16 GPRS_WaitHostACK_S_Timer;//29 GPRS主动上报等待主站确认秒定时器
	u16 ENET_WaitHostACK_S_Timer;//30 ENET主动上报等待主站确认秒定时器
	u16 RS232_WaitHostACK_S_Timer;//31 RS232主动上报等待主站确认秒定时器
	
	u16 Cascading_LoopChack_S_Timer;//32 级联巡测周期秒定时器
	u16 SlipCYC_S_Timer;//33 终端输入脉冲功率计算和需量脉冲滑移秒定时器
	u16 DemandCYC_S_Timer;//34 终端输入脉冲需量计算秒定时器
	u16 RouterCheck_S_Timer;//35 载波路由器检查秒定时器
	u16 RouterTASK_S_Timer;//36 载波路由主动抄表发送后到接收超时时调用TASK()任务计算秒定时器
	u16 res21;//37
	u16 res22;//38 
	u16 AutoSearch_S_Timer;//39 载波自动搜表秒定时器
	u16 RouterSynDelay_S_Timer;//40 路由档案同步延时执行秒定时器
	u16 Power_S_Timer;//41 停电事件秒定时器
	u16 res23;//42
//  u16 DisplayLoopTimer;// 循显秒定时器
//  u16 BackLightTimer;// 背光灯显示秒定时器

#define BASE_S_Timer_Num     42//秒定时器数

#if (((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	u16 ReTrip_S_Timer;//1 终端补跳闸秒定时器
	u16 EnSwitchON_S_Timer;//2 终端合闸允许通知秒定时器
	u16 Remote1_S_Timer;//3 遥控跳闸轮次1报警延时秒定时器
	u16 Remote2_S_Timer;//4 遥控跳闸轮次2报警延时秒定时器
	u16 Remote3_S_Timer;//5 遥控跳闸轮次3报警延时秒定时器
	u16 Remote4_S_Timer;//6 遥控跳闸轮次4报警延时秒定时器
	u16 Remote5_S_Timer;//7 遥控跳闸轮次5报警延时秒定时器
	u16 Remote6_S_Timer;//8 遥控跳闸轮次6报警延时秒定时器
	u16 Remote7_S_Timer;//9 遥控跳闸轮次7报警延时秒定时器
	u16 Remote8_S_Timer;//10 遥控跳闸轮次8报警延时秒定时器
	u16 PowerCtrl1_S_Timer;//11 终端总加组1功控功率计算秒定时器
	u16 PowerCtrl2_S_Timer;//12 终端总加组2功控功率计算秒定时器
	u16 PowerCtrl3_S_Timer;//13 终端总加组3功控功率计算秒定时器
	u16 PowerCtrl4_S_Timer;//14 终端总加组4功控功率计算秒定时器
	u16 PowerCtrl5_S_Timer;//15 终端总加组5功控功率计算秒定时器
	u16 PowerCtrl6_S_Timer;//16 终端总加组6功控功率计算秒定时器
	u16 PowerCtrl7_S_Timer;//17 终端总加组7功控功率计算秒定时器
	u16 PowerCtrl8_S_Timer;//18 终端总加组8功控功率计算秒定时器
	u16 PowerAttr1_S_Timer;//19 终端总加组1功控报警秒定时器
	u16 PowerAttr2_S_Timer;//20 终端总加组2功控报警秒定时器
	u16 PowerAttr3_S_Timer;//21 终端总加组3功控报警秒定时器
	u16 PowerAttr4_S_Timer;//22 终端总加组4功控报警秒定时器
	u16 PowerAttr5_S_Timer;//23 终端总加组5功控报警秒定时器
	u16 PowerAttr6_S_Timer;//24 终端总加组6功控报警秒定时器
	u16 PowerAttr7_S_Timer;//25 终端总加组7功控报警秒定时器
	u16 PowerAttr8_S_Timer;//26 终端总加组8功控报警秒定时器
	u16 PowerCtrl1_Congeal_S_Timer;//27 终端总加组1当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl2_Congeal_S_Timer;//28 终端总加组2当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl3_Congeal_S_Timer;//29 终端总加组3当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl4_Congeal_S_Timer;//30 终端总加组4当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl5_Congeal_S_Timer;//31 终端总加组5当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl6_Congeal_S_Timer;//32 终端总加组6当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl7_Congeal_S_Timer;//33 终端总加组7当前功率下浮控控后冻结秒定时器
	u16 PowerCtrl8_Congeal_S_Timer;//34 终端总加组8当前功率下浮控控后冻结秒定时器
	u16 UpPower_Protect_S_Timer;//35 终端上电保电秒定时器
	u16 ContrlLoopCheck_S_Timer;//36 控制输出回路检测有效等待秒定时器
#define Contrl_S_Timer_Num     36
#else
#define Contrl_S_Timer_Num     0
#endif//#if (((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//终端秒定时器数
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m定时器
	u16 DisconAutoProtect_M_Timer;//1 自动保电允许与主站连续无通信时间分定时器
	u16 AutoSearch_M_Timer;//2 自动搜表允许时间分定时器

#define Terminal_M_Timer_Num     2//终端分定时器数
#define ADDR_SwitchIn     ADDR_Terminal_M_Timer+(2*Terminal_M_Timer_Num)//开关量输入
	u8 SwitchIn1;//开关量输入1原状态
	u8 SwitchInFilter1;//开关量输入1滤波次数
	u8 SwitchIn2;//开关量输入2原状态
	u8 SwitchInFilter2;//开关量输入2滤波次数
	u8 SwitchIn3;//开关量输入3原状态
	u8 SwitchInFilter3;//开关量输入3滤波次数
	u8 SwitchIn4;//开关量输入4原状态
	u8 SwitchInFilter4;//开关量输入4滤波次数
	u8 SwitchIn5;//开关量输入5原状态
	u8 SwitchInFilter5;//开关量输入5滤波次数
	u8 SwitchIn6;//开关量输入6原状态
	u8 SwitchInFilter6;//开关量输入6滤波次数
	u8 SwitchIn7;//开关量输入7原状态
	u8 SwitchInFilter7;//开关量输入7滤波次数
	u8 SwitchIn8;//开关量输入8原状态
	u8 SwitchInFilter8;//开关量输入8滤波次数
#define ADDR_Other      ADDR_SwitchIn+(2*8)
  u8 CongealFlags;//在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
 	u8 LastCongealTime[5];//上次冻结时的实时钟年月日时分寄存
  u8 Terminal_S_Count;//终端秒加计数,用于其他秒定时器
  u8 Terminal_M_Count;//终端分加计数,用于其他分定时器
	u8 SET6000;//采集档案配置表有设置标志:0=没设置,0x55=设置
	u8 SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
	u8 FLASHtoSDRAM;//参数FLASHtoSDRAM上次进行的实时钟时(或其他)
	u8 res4;//SETtime;//时钟设置标志:0=没设置,0xff=设置,用于电科院载波口互换性测试
	u8 res5;
	u8 RoundTripFlags;//终端轮次跳闸位标志,B0-B7=0没循跳,=1已循跳
	u8 MODULSEL;//无线模块选择0=,1=华为CDMA MC323,2=GPRS标准3763,3=模块盒没插入
	u8 ENETSEL;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
	u8 Signal;//GPRS信号强度0-4
	u8 SignaldBm;//信号强度0-31或99(BCD码)
	u8 PowerDownTask;//电源掉电过程任务
	u8 PowerDownSubTask;//电源掉电过程子任务
	u16 CopyTxByte;//寄存GPRS需发送字节数(半字对齐)
  u16 CopyTxCount;//寄存GPRS已发送计数(半字对齐)
	u16 ReqTxByte;//寄存GPRS请求发送字节数(半字对齐)
	u8 res6;
	u8 res7;
	
	u32 res8;
	u32 res9;
	u32 res10;
	u32 res11;
	u32 res12;

	u32 res13;//菜单中已使用了(黄从海)
	u32 res14;//菜单中已使用了(黄从海)
	u32 res15;//菜单中已使用了(黄从海)
	u32 rse16;//菜单中已使用了(黄从海)
#ifndef LINUX_CROSS_GCC
}Terminal_Ram_TypeDef;
#else
}__attribute__ ((packed)) Terminal_Ram_TypeDef;
#endif
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


#define ADDR_BUFF_UART_TIMER_END     (((ADDR_Other+64)+3)&0xfffffffc)
#if (ADDR_BUFF_UART_TIMER_END-ADDR_ERAM_START)>LEN_ERAM_BUFF_UART_TIMER
	#error
#endif

#define ADDR_DL698_ERAM_START     (ADDR_ERAM_START+LEN_ERAM_BUFF_UART_TIMER)

#define ADDR_MobilePhoneNumber     ADDR_DL698_ERAM_START//通信卡的手机号码和IMSI号码(用于显示)
//20byte 手机号码(ASCII)
//20byte IMSI号码(ASCII)
//16字节无线模块本地IP地址,ASCII
//6字节无线模块本地端口号,ASCII
//6字节无线模块信号分贝值,ASCII,单位dB

#define ADDR_4022     ADDR_MobilePhoneNumber+68//插卡状态字
//属性2∷=bit-string(SIZE(16))，无单位，无换算
#define LENmax_4022     4
#define ADDR_4500_5     ADDR_4022+LENmax_4022//公网通信模块1;5版本信息
//版本信息∷=structure
//{
//厂商代码       visible-string(SIZE (4))，
//软件版本号     visible-string(SIZE (4))，
//软件版本日期   visible-string(SIZE (6))，
//硬件版本号     visible-string(SIZE (4))，
//硬件版本日期   visible-string(SIZE (6))，
//厂家扩展信息   visible-string(SIZE (8))
//}
#define LENmax_4500_5     48
#define ADDR_4500_6     ADDR_4500_5+LENmax_4500_5//公网通信模块1;6支持规约列表
//数据格式 array visible-string
#define LENmax_4500_6     16
#define ADDR_4500_7     ADDR_4500_6+LENmax_4500_6//公网通信模块1;7 SIM卡的ICCID
//visible-string(SIZE(20))
#define LENmax_4500_7     22
#define ADDR_4500_8     ADDR_4500_7+LENmax_4500_7//公网通信模块1;8 IMSI
//visible-string(SIZE(20))
#define LENmax_4500_8     22
#define ADDR_4500_9     ADDR_4500_8+LENmax_4500_8//公网通信模块1;9 信号强度
//long，单位：dBm
#define LENmax_4500_9     3
#define ADDR_4500_10     ADDR_4500_9+LENmax_4500_9//公网通信模块1;10 SIM卡号码
//visible-string(SIZE(16))
#define LENmax_4500_10     18


#define ADDR_6000_SDRAM     (((ADDR_4500_10+LENmax_4500_10)+3)&0xfffffffc)//采集档案配置表
//从FLASH中COPY
#define ADDR_6012_2_SDRAM     ADDR_6000_SDRAM+LENmax_6000//任务配置表
//从FLASH中COPY
#define ADDR_6014_SDRAM     ADDR_6012_2_SDRAM+LENmax_6012_2//普通采集方案
//从FLASH中COPY
#define ADDR_6016_SDRAM     ADDR_6014_SDRAM+LENmax_6014//事件采集方案集
//从FLASH中COPY
#define ADDR_601C_SDRAM     ADDR_6016_SDRAM+LENmax_6016///上报方案集
//从FLASH中COPY


#define ADDR_6002_10     ADDR_601C_SDRAM+LENmax_601C//搜表状态
//属性10∷=enum
//{
//空闲（0），搜表中（1） 
//}
#define LENmax_6002_10     2
#define ADDR_6032     ADDR_6002_10+LENmax_6002_10//采集状态集
//一个采集状态∷=structure//删除时structure的数据单元清为0(NULL)
//{
//通信地址              TSA，
//中继级别              unsigned，
//中继地址              TSA，
//端口                  OAD，
//最后一次采集成功时间  date_time_s，
//采集失败次数          unsigned，
//相位       enum{未知（0），A相（1），B相（2），C相（3）}，
//相序异常   enum{正常（0），LN互易（1），逆相序（2）}
//}
#define LENper_6032     64
#define NUMmax_6032     NUM_RMmax
#define LENmax_6032     (LENper_6032*NUMmax_6032)
#define ADDR_6034     ADDR_6032+LENmax_6032//采集任务监控集
//采集任务监控单元∷=structure (在读通信时填写)
//{
//任务ID       unsigned
//任务执行状态  enum
//{
//	未执行（0），
//	执行中（1），
//	已执行（2）
//}，
//任务执行开始时间   date_time_s，
//任务执行结束时间   date_time_s，
//采集总数量         long-unsigned，
//采集成功数量       long-unsigned，
//已发送报文条数     long-unsigned，
//已接收报文条数     long-unsigned
//}
//说明：采集成功数量、已发送报文条数、已接收报文条数，每次任务执行先清零。
#define LENper_6034     34
#define NUMmax_6034     RMTASKmax
#define LENmax_6034     (LENper_6034*NUMmax_6034)


#define ADDR_240x_5     ADDR_6034+LENmax_6034//脉冲口功率
//功率∷=double-long
#define LENper_240x_5     5
#define LENmax_240x_5     (LENper_240x_5*NUMmax_IMPIN)

#define ADDR_230x_3     ADDR_240x_5+LENmax_240x_5//总加有功功率
//long64 
#define LENper_230x_3     9
#define LENmax_230x_3     (LENper_230x_3*NUMmax_TAG)
#define ADDR_230x_4     ADDR_230x_3+LENmax_230x_3//总加无功功率
//long64
#define LENper_230x_4     9
#define LENmax_230x_4     (LENper_230x_4*NUMmax_TAG)
#define ADDR_230x_5     ADDR_230x_4+LENmax_230x_4//总加滑差时间内平均有功功率
//long64
#define LENper_230x_5     9
#define LENmax_230x_5     (LENper_230x_5*NUMmax_TAG)
#define ADDR_230x_6     ADDR_230x_5+LENmax_230x_5//总加滑差时间内平均无功功率
//long64
#define LENper_230x_6     9
#define LENmax_230x_6     (LENper_230x_6*NUMmax_TAG)
#define ADDR_230x_12     ADDR_230x_6+LENmax_230x_6//当前功率下浮控控后总加有功功率冻结值
//long64
#define LENper_230x_12     9
#define LENmax_230x_12     (LENper_230x_12*NUMmax_TAG)
#define ADDR_kWCtrl1_FixedValue     ADDR_230x_12+LENmax_230x_12//当前功率下浮控定值


//终端控制
#define ADDR_Terminal_CTRL_ESRAM_START      ADDR_230x_12+LENmax_230x_12//终端控制ESRAM开始地址

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START//终端费率时段和功控时段
//1BYTE 终端当前执行费率(HEX 费率1,2,3,4 分别对应尖,峰,平,谷等费率),0=无效
//1BYTE 
//1BYTE 当前功控时段号1-8,=0表示时段号>9错误
//1BYTE 0=当前功控时段号不控制,1或2当前功控时段号控制

#define ADDR_Terminal_Total_State     ADDR_Terminal_Period+4//终端总跳闸
//1BYTE 位标志(轮次1-8)当前总跳闸状态(1跳闸,0允许合闸)
//1BYTE 位标志(轮次1-8)允许合闸通知(总跳闸状态相应位转为合闸时置位,通知定时到后清0)
//2BYTE 保留
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//控制模块输出输入数据
//1BYTE	要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
//1BYTE 要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯,b2-b6,b7=轮次继电器动作方式(0=脉冲1=内部自动测试时电平)
//1BYTE	从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
//1BYTE 从控制模块输入的断线标志字节1

#define Terminal_Ctrl_Pn_Len     32//终端控制每Pn数据长度
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4//终端控制-遥控跳闸
//控制轮次1-N
//1BYTE 控制状态,0=上电或不控制,1=投入,2=...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//7BYTE 
//8BYTE	跳闸发生前总加有功功率long64（单位：kW，换算-4）
//8BYTE 跳闸后2分钟总加有功功率long64(单位：W，换算-1)，
//8BYTE
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*NUMmax_CR)//终端控制-功控.当前功率下浮控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=投入,2=计算定值,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//1BYTE 当前功控跳闸轮次B0-B7
//1byte 当前功率下浮控定值浮动系数
//4byte 
//8BYTE 功率定值long64（单位：kW，换算-4）
//8BYTE	跳闸发生前总加有功功率long64（单位：kW，换算-4）
//8BYTE 跳闸后2分钟总加有功功率long64(单位：W，换算-1)，
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-功控.营业报停功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//1byte 当前功控跳闸轮次B0-B7
//5byte 
//8BYTE 功率定值long64（单位：kW，换算-4）
//8BYTE	跳闸发生前总加有功功率long64（单位：kW，换算-4）
//8BYTE 跳闸后2分钟总加有功功率long64(单位：W，换算-1)，
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-功控.厂休功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//1byte 当前功控跳闸轮次B0-B7
//5byte 
//8BYTE 功率定值long64（单位：kW，换算-4）
//8BYTE	跳闸发生前总加有功功率long64（单位：kW，换算-4）
//8BYTE 跳闸后2分钟总加有功功率long64(单位：W，换算-1)，
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-功控.时段功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//1byte 当前功控跳闸轮次B0-B7
//5byte 
//8BYTE 功率定值long64（单位：kW，换算-4）
//8BYTE	跳闸发生前总加有功功率long64（单位：kW，换算-4）
//8BYTE 跳闸后2分钟总加有功功率long64(单位：W，换算-1)，
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-电控.月电控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
//1BYTE 已跳闸轮次B0-B7
//1byte 
//5byte 
//8BYTE 月电量定值 long64（单位：kWh，换算：-4）
//8BYTE 跳闸发生时总加电能量  long64（单位：kwh/元，换算-4）
//8BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制-电控.购电控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
//1BYTE 已跳闸轮次B0-B7
//1byte 
//4byte
//1byte 购电类别0=购电量,1=购电费
//8BYTE 报警门限值  long64（单位：kWh/元，换算：-4）
//8BYTE 跳闸发生时剩余电能量/费  long64（单位：kwh/元，换算-4）
//8BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//终端控制ESRAM结束地址


#define ADDR_230x_16     ADDR_Terminal_CTRL_ESRAM_END//总加组控制设置状态
//总加组控制设置状态∷=structure//数据在读通信时填入
//{
//时段控定值方案号	   unsigned，
//功控时段有效标志位  bit-string(SIZE(8))，
//功控状态            PCState，
//电控状态            ECState，
//功控轮次状态        TrunState，
//电控轮次状态        TrunState
//}
//时段控定值方案号：表示所投入的功控定值方案号。
//功控时段有效标志位：bit0～bit7按顺序对位表示1～8时段控投入的有效时段，置“1”：有效，置“0”：无效
//PCState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
//bit0：时段控
//bit1：厂休控
//bit2：营业报停控
//bit3：当前功率下浮控
//bit4～bit7：备用。
//ECState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
//bit0：月电控
//bit1：购电控
//bit2～bit7：备用
//TrunState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示1～8轮次开关的受控状态；置“1”：受控，置“0”：不受控。
#define LENper_230x_16     19
#define LENmax_230x_16     (LENper_230x_16*NUMmax_TAG)
#define ADDR_230x_17     ADDR_230x_16+LENmax_230x_16//总加组当前控制状态
//当前控制状态∷=structure
//{
//	当前功控定值             long64（单位：W 换算：-1），     
//	当前功率下浮控浮动系数   integer（单位：%），
//	功控跳闸输出状态         OutputState，
//	月电控跳闸输出状态       OutputState，
//	购电控跳闸输出状态       OutputState，
//	功控越限告警状态         PCAlarmState，
//	电控越限告警状态         ECAlarmState
//}
//OutputState∷=bit-string(SIZE(8))
//bit0～bit7分别表示终端1～8轮次跳闸输出状态，置“1”：处于跳闸状态，置“0”：未处于跳闸状态。
//PCAlarmState∷=bit-string(SIZE(8))
//按顺序对位表示；置“1”：处于某种功控越限告警状态；置“0”：未处于相应状态；
//bit0：时段控
//bit1：厂休控
//bit2：营业报停控
//bit3：当前功率下浮控
//bit4～bit7：备用。
//ECAlarmState∷=bit-string(SIZE(8))
//置“1”：处于某种电控越限告警状态；置“0”：未处于相应状态；
//bit0：月电控
//bit1：购电控
//bit2～bit7：备用。
#define LENper_230x_17     28
#define LENmax_230x_17     (LENper_230x_17*NUMmax_TAG)

#define ADDR_8000_3     ADDR_230x_17+LENmax_230x_17//遥控-属性3（继电器输出状态，只读)
//属性3（继电器输出状态，只读)∷=bit-string(SIZE(8))
//继电器输出状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸输出状态，置“1”：跳闸状态；置“0”：合闸状态。
#define LENmax_8000_3     3
#define ADDR_8000_4     ADDR_8000_3+LENmax_8000_3//遥控-属性4(告警状态，只读)
//属性4(告警状态，只读)∷=bit-string(SIZE(8))
//告警状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控告警输出状态，置“1”：处于告警状态；置“0”：未处于告警状态。
#define LENmax_8000_4     3
#define ADDR_8000_5     ADDR_8000_4+LENmax_8000_4//遥控-属性4属性5（命令状态，只读)
//属性5（命令状态，只读)∷=bit-string(SIZE(8))
//继电器命令状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸命令输出状态，置“1”：跳闸命令；置“0”：合闸命令。
#define LENmax_8000_5     3
#define ADDR_8001_2     ADDR_8000_5+LENmax_8000_5//保电-属性2（保电状态，只读）
//属性2（保电状态，只读）∷=enum{解除（0），保电（1），自动保电（2）}
#define LENmax_8001_2     2
#define ADDR_8002_2     ADDR_8001_2+LENmax_8001_2//催费告警状态，只读
//属性2（催费告警状态，只读）∷=enum{未告警(0)，告警(1)}
#define LENmax_8002_2     2

#define ADDR_8103_4     ADDR_8002_2+LENmax_8002_2//时段功控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENper_8103_4     8
#define LENmax_8103_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8103_5     ADDR_8103_4+LENmax_8103_4//时段功控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENper_8103_5     7
#define LENmax_8103_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8104_4     ADDR_8103_5+LENmax_8103_5//厂休控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENmax_8104_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8104_5     ADDR_8104_4+LENmax_8104_4//厂休控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENmax_8104_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8105_4     ADDR_8104_5+LENmax_8104_5//营业报停控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENmax_8105_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8105_5     ADDR_8105_4+LENmax_8105_4//营业报停控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENmax_8105_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8106_4     ADDR_8105_5+LENmax_8105_5//当前功率下浮控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENmax_8106_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8106_5     ADDR_8106_4+LENmax_8106_4//当前功率下浮控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENmax_8106_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8107_4     ADDR_8106_5+LENmax_8106_5//购电控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENmax_8107_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8107_5     ADDR_8107_4+LENmax_8107_4//购电控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENmax_8107_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8108_4     ADDR_8107_5+LENmax_8107_5//月电控-总加组控制输出状态
//控制输出状态∷=array  一个总加组控制输出状态
//一个总加组控制输出状态∷=structure
//{
//  总加组对象    OI，
//  控制输出状态  bit-string(SIZE(8))
//}
//用于表示n个总加组的控制输出状态。
//控制输出状态：
//bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）。
#define LENmax_8108_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8108_5     ADDR_8108_4+LENmax_8108_4//月电控-总加组告警输出状态
//越限告警状态∷=array 一个总加组告警输出状态
//一个总加组告警输出状态∷=structure
//{
//  总加组对象    OI，
//  告警输出状态  enum
//{
//未告警（0），告警（1）
//}
//}
#define LENmax_8108_5     (2+(LENper_8103_5*NUMmax_TAG))


#define ADDR_F100_2     ADDR_8108_5+LENmax_8108_5
//2．ESAM序列号	（static）	octet-string
#define LENmax_F100_2     16
#define ADDR_F100_3     ADDR_F100_2+LENmax_F100_2
//3．ESAM版本号	（static）	octet-string
#define LENmax_F100_3     16
#define ADDR_F100_4     ADDR_F100_3+LENmax_F100_3
//4．对称密钥版本	（static）	octet-string
#define LENmax_F100_4     16
#define ADDR_F100_5     ADDR_F100_4+LENmax_F100_4
//5．会话时效门限	（static）	double-long-unsigned 单位：分钟
#define LENmax_F100_5     16
#define ADDR_F100_6     ADDR_F100_5+LENmax_F100_5
//6．会话时效剩余时间	（dyn.）	double-long-unsigned 单位：分钟
#define LENmax_F100_6     16
#define ADDR_F100_7     ADDR_F100_6+LENmax_F100_6
//7．当前计数器	（static）
//structure
//{
//单地址应用协商计数器 double-long-unsigned，
//主动上报计数器       double-long-unsigned，
//应用广播通信序列号   double-long-unsigned
//}
#define LENmax_F100_7     16
#define ADDR_F100_8     ADDR_F100_7+LENmax_F100_7
//8．证书版本	（static）
//structure
//{
//终端证书版本  octet-string，
//主站证书版本  octet-string
//}
#define LENmax_F100_8     16
#define ADDR_F100_9     ADDR_F100_8+LENmax_F100_8
//9．终端证书序列号	（static）	octet-string
#define LENmax_F100_9     16
#define ADDR_F100_10     ADDR_F100_9+LENmax_F100_9
//10．终端证书	（static）	octet-string
#define LENmax_F100_10     16
#define ADDR_F100_11     ADDR_F100_10+LENmax_F100_10
//11．主站证书序列号	（static）	octet-string
#define LENmax_F100_11     16
#define ADDR_F100_12     ADDR_F100_11+LENmax_F100_11
//12. 主站证书	（static）	octet-string
//主站带MAC下发，同主站证书一起下发（后4个字节为 mac）
#define LENmax_F100_12     16
#define ADDR_F100_13     ADDR_F100_12+LENmax_F100_12
//13. ESAM安全存储对象列表	（static）	array
//需要存储到ESAM中的对象
//ESAM安全存储对象列表∷=array OAD
#define LENmax_F100_13     16


#define ADDR_F203_2     ADDR_F100_13+LENmax_F100_13
//属性2（设备对象列表，只读）∷=array 开关量单元
//开关量单元∷=structure
//{
//状态ST  unsigned，
//变位CD  unsigned
//}
//状态ST——0：“分”状态；1：“合”状态。
//变位CD——0：自前次遥信传送后无状态变化；1：自前次遥信传送后至少有一次状态变化。
#define LENper_F203_2     6
#define NUMmax_F203_2     NUMmax_SWITCHIN
#define LENmax_F203_2     (2+(LENper_F203_2*NUMmax_F203_2))
#define ADDR_F204_2     ADDR_F203_2+LENmax_F203_2
//属性2（设备对象列表，只读）∷=array 直流模拟量
//直流模拟量∷=double-long
#define LENper_F204_2     3
#define NUMmax_F204_2     NUMmax_DC
#define LENmax_F204_2     (2+(LENper_F203_2*NUMmax_F203_2))
#define ADDR_F205_2     ADDR_F204_2+LENmax_F204_2
//属性2（设备对象列表，只读）∷=array 继电器单元(在读通信时填写)
//继电器单元∷=structure
//{
//  描述符    visible-string，
//当前状态  enum{未输出（0），输出（1）}，
//开关属性  enum{脉冲式（0），保持式（1）}，
//接线状态  enum{接入（0），未接入（1) }
//}
#define LENper_F205_2     16
#define LENmax_F205_2     (2+(LENper_F205_2*NUMmax_CR))
#define ADDR_F206_2     ADDR_F205_2+LENmax_F205_2//告警输出
//属性2（设备对象列表，只读）∷=array 告警输出(在读通信时填写)
//告警输出∷=enum
//{
//未输出（0），输出（1）
//}
#define LENper_F206_2     2
#define LENmax_F206_2     (2+(LENper_F206_2*1))
#define ADDR_F207_2     ADDR_F206_2+LENmax_F206_2
//属性2（设备对象列表，只读）∷=array 端子功能
//端子功能∷=enum
//{
//     秒脉冲输出（0），
//     需量周期  （1），
//     时段投切  （2）
//}
#define LENper_F207_2     2
#define NUMmax_F207_2     1
#define LENmax_F207_2     (2+(LENper_F207_2*NUMmax_F207_2))
#define ADDR_F209_2     ADDR_F207_2+LENmax_F207_2
//属性2（设备对象列表，只读）∷=array 本地通信模块单元
//本地通信模块单元∷=structure
//{
//端口描述符   visible-string，
//通信参数     COMDCB，
//版本信息     VersionInfo
//}
//VersionInfo∷=structure
//{
//  厂商代码    visible-string(SIZE(2))，
//  芯片代码    visible-string(SIZE(2))，
//  版本日期     Date，
//  软件版本     long-unsigned
//}
#define LENper_F209_2     64
#define NUMmax_F209_2     1
#define LENmax_F209_2     (2+(LENper_F209_2*NUMmax_F209_2))
#define ADDR_F20A_2     ADDR_F209_2+LENmax_F209_2
//属性2（设备对象列表，只读）∷=array 脉冲输入端口描述符
//脉冲输入端口描述符∷=visible-string
#define LENper_F20A_2     8
#define NUMmax_F20A_2     NUMmax_IMPIN
#define LENmax_F20A_2     (2+(LENper_F20A_2*NUMmax_F20A_2))
#define ADDR_F20B_2     ADDR_F20A_2+LENmax_F20A_2
//属性2（设备对象列表，只读）∷=array 蓝牙模块
//蓝牙模块∷=structure
//{
//端口描述符   visible-string，
//通信参数     COMDCB
//}
#define LENper_F20B_2     8
#define NUMmax_F20B_2     0
#define LENmax_F20B_2     (2+(LENper_F20B_2*NUMmax_F20B_2))




//终端统计用实时数据
#define ADDR_213x_REAL     (((ADDR_F20B_2+LENmax_F20B_2)+3)&0xfffffffc)//当月相电压合格率
//1BYTE 0 上次计算RTC秒
//1BYTE 1 上电延时秒加定时
//1BYTE 2 秒计数
//1BYTE 3 电压加平均计数

//4BYTE 4 电压加平均
//1BYTE 8 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
//1BYTE 9 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限

//4BYTE 10 电压加平均
//1BYTE 14 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
//1BYTE 15 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限

//4BYTE 16 电压加平均
//1BYTE 20 电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
//1BYTE 21 越限事件记录标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
#define LEN_2131_REAL     22

#define ADDR_PowerEvent     ADDR_213x_REAL+LEN_2131_REAL//终端停/上电事件原始数据
typedef __packed struct
{
	u8 Tstoptime[7];//终端停电时间hexYYMDHMS
	u8 Tstarttime[7];//终端上电时间hexYYMDHMS
	u8 RTCS;//上次计算时RTC的秒
	u8 task;//任务
	u8 bit;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
	u8 TimeErr;//有抄读的电能表停上电事件未完整产生
	u8 res2;
	u8 NUMrm;//停上电事件需抄读电表个数
	u16 setno[NUMmax_Event3106RM];//停/上电事件需抄读电表的配置序号(从0开始)
#ifndef LINUX_CROSS_GCC
}PowerEvent_typedef;
#else
}__attribute__ ((packed)) PowerEvent_typedef;
#endif
#define LEN_PowerEvent     (20+(2*NUMmax_Event3106RM))
#define PowerEvent     ((PowerEvent_typedef*)(ADDR_PowerEvent))

#define ADDR_Event_Sour     ((ADDR_PowerEvent+LEN_PowerEvent+3)&0xfffffffc)//电能量超差,飞走,停走事件原始数据(按档案配置序号排列,从0开始)
typedef __packed struct
{
	u16 RMAddr[3];//电能表地址
	u8 res;
	u8 Re;//抄表失败(310F)重试轮次
	u32 Occur;//事件产生恢复,bit=0恢复=1产生;b0=抄表失败(310F),b1=,...,b7=
	u32 TimeCount;//电能表停走时间计时(秒)
#ifndef LINUX_CROSS_GCC
}EventSour_typedef;
#else
}__attribute__ ((packed)) EventSour_typedef;
#endif
#define LEN_Event_Sour     16

#define ADDR_Read_Info_Time     (ADDR_Event_Sour+(NUM_RMmax*LEN_Event_Sour))
//7byte YYMDHMS
#define ADDR_AFN10F21     ADDR_Read_Info_Time+7//从路由读取的网络拓扑信息
//2byte 节点总数量
//6byte 节点1地址
//5byte 节点1网络拓扑信息
//...
//6byte 节点n地址
//5byte 节点n网络拓扑信息
#define ADDR_AFN10F111     ADDR_AFN10F21+2+(11*NUM_RMmax)//从路由读取的多网络信息
//多网络节点总数量n BIN 1
//本节点网络标识号 BIN 3
//本节点主节点地址 BIN 6
//邻居节点1网络标识号 BIN 3
//……
//邻居节点n网络标识号 BIN 3
#define ADDR_AFN10F112     ADDR_AFN10F111+10+(3*256)//从路由读取的宽带载波芯片信息
//节点总数量 BIN 2
//节点1地址 BIN 6
//节点1设备类型 BIN 1
//节点1芯片ID信息 BIN 24
//节点1芯片软件版本信息 BIN 2
//...
//节点n地址 BIN 6
//节点n设备类型 BIN 1
//节点n芯片ID信息 BIN 24
//节点n芯片软件版本信息 BIN 2

#if (PPP_ENABLE==0)//1使用PPP连接,0不使用PPP连接
#define ADDR_TEST     		ADDR_AFN10F112+2+(33*NUM_RMmax)//功能测试,总是放在最后
#else
#define SERVER_LINK_ADDR 	((ADDR_AFN10F112+2+(33*NUM_RMmax)+3)&0xfffffffc)
typedef __packed struct
{
 u32 ppp0_subtask;
 s32 ppp0_listen;//
 s32 ppp0_clint;//
 u32 eth0_subtask;
 s32 eth0_listen;//
 s32 eth0_clint;//
#ifndef LINUX_CROSS_GCC
}SERVER_TypeDef;
#else
}__attribute__ ((packed)) SERVER_TypeDef;
#endif
#define SERVER_LINK_LEN     24//
#define Svr_Skt     ((SERVER_TypeDef*)(SERVER_LINK_ADDR))

//#define ADDR_UART10IntRx     ADDR_UART10
//#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
//#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
//#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define  ADDR_PPP0_IP4ADDR_ASCII   SERVER_LINK_ADDR+SERVER_LINK_LEN
//"192.168.247.236"
#define  ADDR_TEST     	       ADDR_PPP0_IP4ADDR_ASCII+20//功能测试,总是放在最后
#endif

#if (USER/100)==17//江苏系
#define ADDR_NoteList1     (((ADDR_TEST+8+3)+3)&0xfffffffc)//路由器中节点表
typedef __packed struct
{
	u8 Addr[6];//从节点地址(字对齐)
	u8 CollectorAddr[6];//采集器地址
	u16 user;
#ifndef LINUX_CROSS_GCC
}Note_TypeDef1;
#else
}__attribute__ ((packed)) Note_TypeDef1;
#endif
#define LEN_NoteList1     14//每列表字节数

typedef __packed struct
{
  Note_TypeDef1     List1[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}NoteList_TypeDef1;
#else
}__attribute__ ((packed)) NoteList_TypeDef1;
#endif
#define NoteList1     ((NoteList_TypeDef1 *)(ADDR_NoteList1))//载波路由器中节点表(按读路由和添加顺序排列)

#define ADDR_AFN10F31     (((ADDR_NoteList1+(LEN_NoteList1*NUM_RMmax))+3)&0xfffffffc)//相位信息  ((ADDR_NoteList1+(LEN_NoteList1*NUM_RMmax))&0xfffffffc)

#define ADDR_BatteryEvent 	ADDR_AFN10F31+2+(11*NUM_RMmax)//电池设备监测

#define ADDR_NetTopFlag 	ADDR_BatteryEvent+4//网络拓扑开启flag

#define ADDR_AFN10F104     	ADDR_NetTopFlag+1//从路由读取的宽带载波芯片信息
//节点总数量 BIN 2
//节点1芯片软件版本日期 BIN 15
//...
//节点n芯片软件版本日期 BIN 15

#define ADDR_AFN10F112_old     (ADDR_AFN10F104 + 2 + (15 * NUM_RMmax))//从路由读取的宽带载波芯片信息
//节点总数量 BIN 2
//节点1地址 BIN 6
//节点1设备类型 BIN 1
//节点1芯片ID信息 BIN 24
//节点1芯片软件版本信息 BIN 2
//...
//节点n地址 BIN 6
//节点n设备类型 BIN 1
//节点n芯片ID信息 BIN 24
//节点n芯片软件版本信息 BIN 2

#define ADDR_AFN10F104_old 			(ADDR_AFN10F112_old + 2 + (33 * NUM_RMmax))
//节点总数量 BIN 2
//节点1芯片软件版本日期 BIN 15
//...
//节点n芯片软件版本日期 BIN 15

#define ADDR_AFN3F12			(ADDR_AFN10F104_old + 2 + (15 * NUM_RMmax))
//模块厂商代码 ASCII 2
//模块 ID 长度 BIN 1
//模块 ID 格式 BIN 1
//模块 ID 对应模块 ID 格式 M 最大50

#define ADDR_AFN10F7     	(ADDR_AFN3F12 + 54) //从路由读取的宽带载波芯片信息
//节点总数量 BIN					 2

//从节点1地址	BCD				 6 
//从节点1节点类型BS				 1 
//从节点1模块厂商代码ASCII 			 2 
//从节点1节点模块ID号长度BIN			 1 
//从节点1节点模块ID号格式BIN			 1 
//从节点1节点模块ID号				 50
//...
//从节点N地址	BCD 			 6 
//从节点N节点类型BS				 1 
//从节点N模块厂商代码ASCII 			 2 
//从节点N节点模块ID号长度BIN			 1 
//从节点N节点模块ID号格式BIN			 1 
//从节点N节点模块ID号				 50

#define ADDR_AFN10F7_old    (ADDR_AFN10F7 + 2 + (61 * NUM_RMmax)) //从路由读取的宽带载波芯片信息
//节点总数量 BIN					 2

//从节点1地址	BCD				 6 
//从节点1节点类型BS				 1 
//从节点1模块厂商代码ASCII 			 2 
//从节点1节点模块ID号长度BIN			 1 
//从节点1节点模块ID号格式BIN			 1 
//从节点1节点模块ID号				 50
//...
//从节点N地址	BCD 			 6 
//从节点N节点类型BS				 1 
//从节点N模块厂商代码ASCII 			 2 
//从节点N节点模块ID号长度BIN			 1 
//从节点N节点模块ID号格式BIN			 1 
//从节点N节点模块ID号				 50
//#define ADDR_TEST     		(ADDR_AFN10F7 + 2 + (61 * NUM_RMmax))	//功能测试,总是放在最后




#define ADDR_45000C00    	ADDR_AFN10F7_old+2+(33*NUM_RMmax) //从路由读取的宽带载波芯片信息
// 远程模块ID信息，长度15字节


//JSON相关参数
#define ADDR_JSON	(((ADDR_45000C00+15)+3)&0xfffffffc)
typedef __packed struct
{
	//JSON打包的运行状态
	u8 RealFileName[84];//实时数据的文件名
	u8 PackFrzJsonTask;//冻结JSON任务计数
	u8 PackRealJsonTask;//当前JSON任务计数
	u8 PackJsonTask;//终端任务计数
	u8 Lastrealtime[7];//打包前的时间需要保存,
	u8 reportfiletime[7];//上报的时间段
	u8 IsCreateFrzJson;//今天是不是打包了日冻结,0没有,非0就是没有打包过
	u8 Is_AllRealData;//是不是处理全部的电表数据(包括没有的数据项,只会在当前打包文件中有)
	u8 res;//字节对齐补充
	u16 CurFrzMetCount;//冻结包表计数
	u16 CurRealMetCount;//当前包表计数
	//上报
	u16 g_Block_Number;//文件上报总块
	u16 g_Need_report_Block;//当前文件上报的块
	u32 g_Filesize;//当前需要上传的文件大小
	u8 Reportjsonfile_Task;//上报任务计数
	u8 g_ReportFlag;//是否需要上报的标志	
	u16 res2;//字节对齐补充
#ifndef LINUX_CROSS_GCC
}ADDR_JSON_typedef;
#else
}__attribute__ ((packed)) ADDR_JSON_typedef;
#endif

#define JSON ((ADDR_JSON_typedef*)(ADDR_JSON))//JSON数据打包相关参数

//#define ADDR_ERAM_END  (((ADDR_45000C00+15)+3)&0xfffffffc)
#define ADDR_ERAM_END  (((ADDR_JSON+120)+3)&0xfffffffc)

#endif


//#endif
//?字节 功能测试1
//?字节 功能测试2
//...
//?字节 功能测试n

#ifndef ADDR_ERAM_END
	#define ADDR_ERAM_END     ((ADDR_TEST+8+3)&0xfffffffc)
#endif






//剩余ERAM空间全用于自动分配(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=没,1=有(8M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00400000
    #error ADDR_ERAM_END>4//8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00400000)//堆为8字节对齐
	#define LEN_MyHeap     0x00400000//剩余ERAM空间长度
#endif

#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=没,1=有(16M)
#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>6//16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//堆为8字节对齐
	#define LEN_MyHeap     0x00A00000//剩余ERAM空间长度
#else
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8//16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//堆为8字节对齐
	#define LEN_MyHeap     0x00800000//剩余ERAM空间长度
#endif
#endif

#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=没,1=有(32M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8M//32M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//堆为8字节对齐
	#define LEN_MyHeap     0x01800000//剩余ERAM空间长度
#endif

#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=没,1=有(64M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00A00000
    #error ADDR_ERAM_END>10M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00A00000)//堆为8字节对齐
	#ifndef LINUX
	#define LEN_MyHeap     0x02600000//剩余ERAM空间长度
	#else//#ifndef LINUX
	
	#define LEN_MyHeap     (0x02000000-0x00540000-0x00A00000)//剩余ERAM空间长度
	
	#endif
#endif

#if IC_SDRAM==0
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>LEN_Terminal_Ram
    #error ADDR_ERAM_END>LEN_Terminal_Ram
  #endif
	#define ADDR_MyHeap     0
	#define LEN_MyHeap     0//剩余ERAM空间长度
#endif



#endif





