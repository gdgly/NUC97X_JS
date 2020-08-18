


#ifndef ERAM_H
#define ERAM_H


	#if IC_SDRAM
#ifdef IC_STM32F4xx
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
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
#endif
	#else
#define ADDR_ERAM_START     ADDR_Terminal_Ram_Start
	#endif

#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
//LEN_SaveRam BYTE 从SRAM复制的掉电保护数据,上电时不清0故放在最前以便上电时其他数据清0地址计算
#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#ifdef  NAND_1G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#ifdef  NAND_2G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#ifdef  NAND_4G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#ifdef  NAND_8G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE文件块长度:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)//存储在FLASH中的文件的管理单元字节数
#endif
#if LEN_msFILEunltFlags>=LEN_msFILEmanage
	#error
#endif
/*
typedef __packed struct
{
	u32 ADDR_ENTRY;//文件入口nand flash地址,0=空
	u32 ADDR_END;//文件结束nand flash地址,0=空
	u32 LEN_FILE;//文件有效数据总长度
	
}msFILE_TypeDef;
*/
typedef __packed struct
{
	u16 BLOCK_ENTRY;//文件入口nand flash块号,0=空
	u16 BLOCK_END;//文件结束nand flash块号,0=空
	u32 LEN_DATA;//文件有效数据总长度
	u32 LEN_FILE;//文件预留占有存储空间总长度
}msFILE_TypeDef;
#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)//128K(BKPSDRAM) NAND FLASH写缓冲(仅在数据检查错误时清0)
typedef __packed struct
{
	u32 ADDR_Dest;//4BYTE 写入NAND FLASH的目的地址
	u32 LEN_Data;//4BYTE 数据字节数
	u32 LEN_Write;//4BYTE 已部分写入的字节数
	u8 Data[1];//nBYTE 数据
}BKPSDRAM_BLOCK_TypeDef;
typedef __packed struct
{
	u16 NUM_BLOCK;//2BYTE 总块数
	u32 LEN_TOTAL;//4BYTE 总字节数(全部字节,也即下块的开始地址偏移)
	//数据块
}BKPSDRAM_TypeDef;
#define ADDR_SDRAM_CLR_START     (ADDR_BKPSDRAM+(128*1024))//SDRAM数据清0开始地址

#define ADDR_RAMmsFILEmanage     ADDR_SDRAM_CLR_START//存储在RAM中的文件管理
#define LEN_RAMmsFILEmanage     32768
#define LEN_RAMmsFILEblock     (32*1024)//RAMmsFILE文件块长度

#define ADDR_emWin     ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
//128K emWin编译区
#define ADDR_128KDATABUFF     (ADDR_emWin+(128*1024))
//128K DATABUFF
#define LEN_128KDATABUFF     (128*1024)
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
//128K字节NAND FLASH块写缓冲
#define ADDR_NAND_PAGEREAD_BUFF     (ADDR_128KWRITE_BUFF+(128*1024))
//2048字节NAND FLASH页读缓冲
#define ADDR_ECC_BUFF     (ADDR_NAND_PAGEREAD_BUFF+2048)
//128+128字节ECC缓冲

#define ADDR_MACADDR_ascii     (ADDR_ECC_BUFF+256)//本机MAC地址的ascii码
//18BYTE 格式"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     (ADDR_MACADDR_ascii+18)//本机IP4地址的ascii码
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


#define ADDR_UARTStart    ((ADDR_IP6SECONDARY_DNS_ascii+40+3)&0xfffffffc)//ERAM需字对齐//UART
//UART0
#define ADDR_UART0     (ADDR_UARTStart)
#define ADDR_UART0IntRx     (ADDR_UART0)
#define ADDR_UART0Ctrl     (ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     (ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     (ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     (ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     (ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     (ADDR_UART1)
#define ADDR_UART1IntRx     (ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     (ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     (ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     (ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     (ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     (ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#if IC_SDRAM
#define ADDR_UART2     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#else
#define ADDR_UART2     0x20002000//(Ethernet专用RAM区)
#endif
#define ADDR_UART2IntRx     (ADDR_UART2)
#define ADDR_UART2Ctrl     (ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     (ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     (ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     (ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     (ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     (ADDR_UART3)
#define ADDR_UART3Ctrl     (ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     (ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     (ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     (ADDR_UART3Tx+LEN_UART3TxMax)

#if !IC_SDRAM
#if ((ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)-ADDR_UART2)>(7*1024)
	#error ((ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)-ADDR_UART2)>(7*1024)
#endif
#endif

//UART4
#if IC_SDRAM
#define ADDR_UART4     (ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#else
#define ADDR_UART4     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#endif
#define ADDR_UART4IntRx     (ADDR_UART4)
#define ADDR_UART4Ctrl     (ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     (ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     (ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     (ADDR_UART4Tx+LEN_UART4TxMax)
//UART5
#define ADDR_UART5     (ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)
#define ADDR_UART5IntRx     (ADDR_UART5)
#define ADDR_UART5Ctrl     (ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     (ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     (ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     (ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     (ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     (ADDR_UART6)
#define ADDR_UART6Ctrl     (ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     (ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx     (ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     (ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(终端ESAM)
#define ADDR_UART7     (ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     (ADDR_UART7)
#define ADDR_UART7Ctrl     (ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     (ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     ADDR_UART7Rx
#define ADDR_UART7FnDataBuff     (ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(电表ESAM)
#define ADDR_UART8     (ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     (ADDR_UART8)
#define ADDR_UART8Ctrl     (ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx     (ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     (ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     (ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(电表CARD)
#define ADDR_UART9     (ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     (ADDR_UART9)
#define ADDR_UART9Ctrl     (ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     (ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     (ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff     (ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(以太网)
#define ADDR_UART10     (ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     (ADDR_UART10)
#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     (ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     (ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     (ADDR_UART11)
#define ADDR_UART11Ctrl     (ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     (ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     (ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     (ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(虚拟口)
#define ADDR_UART12     (ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     (ADDR_UART12)
#define ADDR_UART12Ctrl     (ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     (ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     (ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     (ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART13     (ADDR_DATABUFF)
#define ADDR_UART13IntRx     (ADDR_UART13)
#define ADDR_UART13Ctrl     (ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     (ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     (ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     (ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART14     (ADDR_DATABUFF)
#define ADDR_UART14IntRx     (ADDR_UART14)
#define ADDR_UART14Ctrl     (ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     (ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     (ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     (ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART15     (ADDR_DATABUFF)
#define ADDR_UART15IntRx     (ADDR_UART15)
#define ADDR_UART15Ctrl     (ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     (ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     (ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     (ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define ADDR_UART16     (ADDR_DATABUFF)
#define ADDR_UART16IntRx     (ADDR_UART16)
#define ADDR_UART16Ctrl     (ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     (ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     (ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     (ADDR_UART16Tx+LEN_UART16TxMax)


#define ADDR_UARTnCtrl_End     (ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)
#define ADDR_TESAM_User     ADDR_UARTnCtrl_End
//8BYTE TESAM芯片序列号(高低字节同TESAM输出)
//8BYTE 8字节随机数8或4(从TESAM中读取)(高低字节同TESAM输出)
#define ADDR_MESAM_User     ADDR_TESAM_User+16
//8BYTE MESAM芯片序列号
//8BYTE 8字节随机数8或4(从MESAM中读取)
//8BYTE 8字节远程身份认证时主站下发的分散因子
//8BYTE 8字节远程身份认证时主站下发的随机数
#define ADDR_CARD_User     ADDR_MESAM_User+32
//8BYTE CARD复位信息后8字节离散
//8BYTE
#define ADDR_AFN09F9     ADDR_CARD_User+16//远程通信模块版本信息
//厂商代号 ASCII 4
//模块型号 ASCII 8
//软件版本号 ASCII 4
//软件发布日期：日月年 见附录A.20 3
//硬件版本号 ASCII 4
//硬件发布日期：日月年 见附录A.20 3
//ＳＩＭ卡ICCID ASCII 20



#define ADDR_UARTEnd     (ADDR_AFN09F9+46)
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
  u8 Safety;//完全认证
  u8 RMProtocol;//抄表通信规约:0=空,1=DL/T645-1997,2=交流采样装置,3=...,30=DL/T645-2007,31=窄带低压载波
  u16 BpsCtrl;//通信控制字(同规约2007)(半字对齐)
			//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//D4位表示停止位位数0/1分别表示1/2个停止位
			//D3位表示有无校验位0/1分别表示无/有
			//D2位表示偶／奇校验位0/1分别表示偶／奇校验
			//D1-D0 数据位数0～3分别表示5-8位
  u8 Protocol;//通信规约自动识别:0=376.1,1=DL/T645-2007
  u8 NewBps;//波特率更新标志;当为MEASM通道时为MESAM内部数据更新标志0=更新,其他值没更新
  u8 SubTask;//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
  u8 AddrType;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
	//共16Byte
//终端专用部分
  u32 SourDI;//原始DA,DT
  u32 DestDI;//分解过程中的DA,DT
	u32 RxTxByte;//通信流量
	u16 PORT;//本机端口号(半字对齐),同时用于掉电时已连接过1次识别
	u8 TCPUDP;//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
	u8 LinkCtrl;//连接控制0=不连接,1=连接
	u8 LinkCount;//连接计数(2次主IP,1次备用IP)
	u8 ReLinkNum;//被动激活模式重拨次数,收到被动激活命令时置初值
	u8 LinkTask;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	u8 LinkSubTask;//链接子任务
	u8 HeartberatCount;//心跳发出计数,收到确认帧清0
	u8 HostACK;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7-福建四类数据上报
	u8 HostCount;//主动上报重发计数(心跳发出计数另计)
	u8 TimerTaskCount;//主动上报定时任务计数
  u8 RxSEQ;//接收帧序列域SEQ
  u8 TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	
	u16 MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)
	u16 MeterNoBackup;//485抄表电能表/交流采样装置序号(半字对齐)(插入抄读实时数据时备份)
	u8 FnEnd;//485抄表Fn计数结束标志,0=结束,1=没结束
	u8 FnCount;//485抄表Fn计数
	u8 FnCountBackup;//485抄表Fn计数(插入抄读实时数据时备份)
	u8 RMCount;//485抄表命令计数
	u8 ReRMCount;//485抄表命令重发计数
	u8 RMError;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
	u8 RMComplete;//485抄表D7=1上电后至小已完成1个循环抄表,D6-D4保留,D3-D0完成循环抄表计数(在标注未抄表时清0,不溢出回0)
	u8 RMFlag;//0=抄表间隔暂停,1=抄表进行中
	u8 CascadingAddrType;//级联数据及地址类型0=无级联数据,1=单地址,2=组地址,3=广播地址
	u8 CascadingReTxCount;//级联重发计数(当为级联周期巡测启动时此值设为最大0xff即只1次)
	u8 CascadingBroadcast;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
	u8 ATTask;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
	u8 ATWriteTask;//AT数据发送任务:0=发命令,1=发数据
	u8 ATCount;//AT计数
	
	u8 LinkTask2;//通道2链接任务(FTP命令)
	u8 FTPLinkCount;//FTP尝试连接计数
	u8 Task2;//通道2任务(FTP命令)
	u8 FTPErr;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
	u32 FTPByte;//FTP当前文件已完成下载字节数
	u32 FTPFileLen;//FTP当前文件总长度
	u16 DIOffset;//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
	u16 TxFrameCount;//寄存多帧数据时已发送帧计数(半字对齐)
	u16 MulFrameGPCR;//多帧时通用计数寄存器
	u16 DIlen;//数据标识长度寄存器
	//共80Byte
//集中抄表专用部分
	u16 TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
	u8 LockFlags;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7=保留
	u8 RTCS;//实时钟更新标志原始秒
	u8 RTCM;//实时钟更新标志原始分
	u8 RTCH;//实时钟更新标志原始时
	u8 DateReRMCount;//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
	u8 BroadcastTime;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
	u16 MeterNoBackupOfKeyUser;//485抄表电能表/交流采样装置序号(半字对齐)(插入抄读重点户曲线数据时备份)
	u8 FnCountBackupOfKeyUser;//485抄表Fn计数(插入抄读重点户曲线数据时备份)
	u8 RMTaskCOfKeyUser;//进入重点户抄读任务前原任务
	u8 RMTask5;//抄读电能表事件任务分类:B0=等级1,B1=等级2,...,B7=等级8
	u8 SubsequentFrames;//0=无读后续帧,1-255=DL645读续帧的帧号
	u8 F10ReadDICount;//F10已转发直接抄读的数据标识个数
	u8 LINKCHECK_RTCS;//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
	u8 TI[7];//寄存全部测量点时曲线的时标(年月日时分),密度,点数
	u8 RMCountErr;//Fn内有抄读失败或不抄的数据标识计数
	//共104Byte
	u8 BatchTask;//转发批量任务:0=空,1=等待口空闲启动,2=...过程
	u8 BatchTaskLock;//0x55=转发批量任务正在被调用禁止其他程序再次调用
	u16 BatchMeterCount;//转发批量任务电表计数
	u32 Random;//电表身份认证时返回的随机数4byte
	u8 BatchMeterNo[6];//转发批量对时任务从电表读取的表号	
	u8 AutoSearchTask;//自动搜表任务:0=空,1=开始,2...过程
	u8 AutoSearchReCount;//自动搜表任务重发计数
	u16 TransferDelay;//线路上传输延时时间秒
	u8 NextRMTime[6];//抄表间隔周期下次抄表时间秒分时日月年
	//共128Byte
	u8 NETOPENCount;//开启服务连续次数计数(当前次数达到x次时关模块电源)
	u8 RMTaskC;//当前抄表任务号:0=上电任务,1=每日任务,2=每周任务,3=停电记录任务,4=每小时曲线任务,5=,(MaxRMTask-2)=周期抄读电能表事件,(MaxRMTask-1)=循抄任务(除每小时曲线任务外,只有循抄任务才可插入高优先级抄表)
	u8 RMTaskN;//新任务b0-b7分别表示任务0-7有新任务
	u8 FnCountS;//485抄表原循抄任务的Fn计数
	u16 MeterNoS;//485抄表原循抄任务的配置序号计数(半字对齐)
	u8 RTCD[2];//实时钟日更新标志的原始月日
	u8 RTCW[2];//实时钟星期更新标志的原始月日
	u16 SMSRxSEQ;//短信接收接口;B0-B11接收帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	u16 SMSTxSEQ;//短信发送接口;B0-B11发送帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
	u8 NextTimerRMTime[6];//定时抄停电记录间隔周期下次抄表时间秒分时日月年
	u16 MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(抄读电表主动上报前原常规抄表的装置序号寄存)
	u8 FnCountBackupOfAutoReport;//485抄表Fn计数(抄读电表主动上报前原常规抄表的Fn计数寄存)
	u8 RMMeterEvent;//正在抄读电能表事件标志;b0=主动上报事件,b1=
	
	u8 RMEventTaskC;//当前抄读电能表事件任务:0=空,1-3=1-3级
	u8 RMEventTaskN;//新周期抄读电能表事件任务b0-b2分别为事件分级1-3,;b6=月更新(上海每月抄事件用)
	u8 RMCountBackup;//485抄表命令计数(插入抄读实时数据时备份)
	u8 ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
	u32 res0;
	//共160Byte
	u8 RMTaskFlags;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	u8 RMCommandYMDHM[5];//抄读电表曲线的时标年月日时分(全0表示无效)

//	
	u8 SearchDICount;//周期事件抄读AFN04F106设置搜索计数
	u8 RMDICount;//周期事件抄读已抄DI计数
	u8 TotalDICount;//周期事件抄读AFN04F106设置中搜索到的DI总数(在SearchDICount=0时计算)
	u8 res2;
	u8 res3;
	u8 DHCPTask;//DHCP任务
	u32 RMDI;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
	s32 TCPSocket;
	s32 UDPSocket;
	u32 Last_Block_No;//(上海专变用)备份698分帧序号，初始化成0xffffffff
	u32 Addr_Split_Frame;//(上海专变用)存放分帧传输时申请的动态内存地址，为0表示未分配
	u32 Save_Split_Frame_len;//(上海专变用)存放已经存放的分帧的长度,初始化成0
	u32 res10;
	u8 NET_ADDR[20];//NET_ADDR
#if (USER/100)==14//福建系	
	u32 CongealStartTime;//补日冻结起始时间(停电起始:年月日)
	u32 CongealEndTime;//补日冻结结束时间(停电结束:年月日)
	u16 CongealSymbol;//补日冻结Fn标志  D0-D7:0CF129-0CF136转0DF161-0DF168	D8-D16备用
	u8  CongealDays;//补日冻结天数	
	u8 FuJBroadCastTask;//福建广播任务
	u8 FuJBroadCastReCount;//重试次数
	u8 FuJAutoSearchTask;//福建从节点注册任务
	u8 FuJAutoSearchReCount;//重试次数
	u8 TaskType;//任务类型
	u16 TaskID;//任务号
	u8 Prio;//任务优先级
	u8 res11;//对齐
#else
	u8 res11[20];
#endif	
	u32 S_num;//程序运行次数计数
	u32 res7;//uart0，uart4载波福建任务执行异常计数器
	u32 res13;
#if (((USER/100)==6)||((USER/100)==12))//湖南，四川
	u8 Jump;//湖南，四川在冻结时标不正确的情况下，需要跳抄表项。 =1 表示跳  =0 表示不跳
#else
	u8 res8;
#endif	
	u8 AT_NETINFO;//AT命令字 设置4G锁频(1：自动  5:2G+3G    6:2G+4G)
	u8 AT_NETINFO_NUM;//(连接计数)	
	u8 AT_CIMI_NAME;//当前SIM的运行商信息(1：移动  2：联通  3：电信   4：其它)
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

//系统数据
typedef __packed struct
{
#define ADDR_Terminal_ERAM_Start     ((ADDR_UARTEnd+3)&0xfffffffc)//ERAM需半字对齐//ERAM需字对齐
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
	u16 I2SUBTask4_17;//
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
	
#if ((USER/100)==14)//福建
	u16 FuJ_GPRSTick_MS_Timer;//17 福建GPRS维持在线秒定时器
#define Terminal_MS_Timer_Num     17//终端10MS定时器数
#endif
#ifndef Terminal_MS_Timer_Num
#define Terminal_MS_Timer_Num     16//终端10MS定时器数
#endif

#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s定时器
	u16 S_PR;//1 60秒分频产生分
  u16 ENETHeartBeat_S_Timer;//2 以太网心跳秒定时器
	u16 IPLeaseTimer;//3 以太网自动获得IP租赁时间秒定时器
	u16 ENETCheck_S_Timer;//4 以太网IP冲突检查和租赁更新秒定时器
  u16 GPRSHeartBeat_S_Timer;//5 GPRS心跳秒定时器
	u16 GPRSAutoDiscon_S_Timer;//6 GPRS被动激活模式连续无通信自动断线秒定时器
	u16 GPRSReConnection_S_Timer;//7 GPRS永久在线、时段在线模式重拨间隔秒定时器
	u16 GPRSWaitConnection_S_Timer;//8 GPRS发起连接服务等待秒定时器
	u16 GPRSWaitTx_S_Timer;//9 GPRS发送数据等待超时秒定时器
	u16 GPRSCheck_S_Timer;//10 GPRS在线和信号检查秒定时器
	u16 ResetCommand_S_Timer;//11 终端收到复位命令延时执行秒定时器
	u16 GPRS_WaitHostACK_S_Timer;//12 GPRS主动上报等待主站确认秒定时器
	u16 ENET_WaitHostACK_S_Timer;//13 ENET主动上报等待主站确认秒定时器
	u16 RS232_WaitHostACK_S_Timer;//14 RS232主动上报等待主站确认秒定时器
	u16 RMDelay_S_Timer;//15 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
	u16 ACSampleRM_S_Timer;//16 内部交流采样抄表秒定时器
	u16 RS4851RM_S_Timer;//17 RS485-1抄表秒定时器
	u16 RS4852RM_S_Timer;//18 RS485-2抄表秒定时器
	u16 RS4853RM_S_Timer;//19 RS485-3抄表秒定时器
	u16 RS4854RM_S_Timer;//20 RS485-4抄表秒定时器(载波)
	u16 RS4851RMDelay_S_Timer;//21 RS485-1上电延时抄表秒定时器(紧接上抄表定时器不分开)
	u16 RS4852RMDelay_S_Timer;//22 RS485-2上电延时抄表秒定时器
	u16 RS4853RMDelay_S_Timer;//23 RS485-3上电延时抄表秒定时器
	u16 RS4854RMDelay_S_Timer;//24 RS485-4上电延时抄表秒定时器(载波)
	u16 RS4851RMF246_S_Timer;//25 RS485-1停电事件抄读时间限值秒定时器(紧接上抄表定时器不分开)
	u16 RS4852RMF246_S_Timer;//26 RS485-2停电事件抄读时间限值秒定时器
	u16 RS4853RMF246_S_Timer;//27 RS485-3停电事件抄读时间限值秒定时器
	u16 RS4854RMF246_S_Timer;//28 RS485-4停电事件抄读时间限值秒定时器(载波)
	u16 Cascading_LoopChack_S_Timer;//29 级联巡测周期秒定时器
	u16 SlipCYC_S_Timer;//30 终端输入脉冲功率计算和需量脉冲滑移秒定时器
	u16 DemandCYC_S_Timer;//31 终端输入脉冲需量计算秒定时器
	u16 RouterCheck_S_Timer;//32 载波路由器检查秒定时器
	u16 AutoUpdataAFN04F10_S_Timer;//33 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
	u16 AutoUpdataAFN04F10ERC3_S_Timer;//34 自动更新F10主动上报ERC3事件秒定时器;集中器通过本地的自动发现，完成测量点新增、变更或删除后，应立即置测量点参数变更标志（F150参数）相应位为1，并立即上报参数变更事件（ERC3）到主站,除非进行了测量点参数变更标志的复位，否则，每次上报参数变更事件后的m分钟（m=0~30，一般取10）之内，无论参数是否发生变更均不再次上报
	u16 AutoDelAFN04F10_S_Timer;//35 自动删除AFN04F10的电能表配置秒定时器,仅免经常调用影响CPU速度
	u16 AutoSearch_S_Timer;//36 载波自动搜表秒定时器
	u16 RouterSynDelay_S_Timer;//37 路由档案同步延时执行秒定时器
	u16 Power_S_Timer;//38 停电事件秒定时器
//  u16 DisplayLoopTimer;// 循显秒定时器
//  u16 BackLightTimer;// 背光灯显示秒定时器

#if ((USER/100)==14)//福建
	u16 FuJTask_S_Timer;//39 福建定时任务秒定时器
	u16 GPRS_Error_S_Timer;//40 GPRS错误返回定时器
	u16 AFN04HF222_Task_S_Timer;//41 福建4类数据执行暂停秒定时器	  增补团体标准修改
	u16 AFN04HF222_AFN57_S_Timer;//42 福建4类数据上报暂停秒定时器	  增补团体标准修改	
#define BASE_S_Timer_Num     42//秒定时器数
#endif
#ifndef BASE_S_Timer_Num
#define BASE_S_Timer_Num     38//秒定时器数
#endif

#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
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
#endif//#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//终端秒定时器数
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m定时器
	u16 DisconAutoProtect_M_Timer;//1 自动保电允许与主站连续无通信时间分定时器
	u16 AutoSearchMeterTimer;//2 自动搜表允许时间分定时器(初始是湖南要求)
	u16 RS4851RMF246_M_Timer;//3 RS485-1停电事件抄读间隔分定时器
	u16 RS4852RMF246_M_Timer;//4 RS485-2停电事件抄读间隔分定时器
	u16 RS4853RMF246_M_Timer;//5 RS485-3停电事件抄读间隔分定时器
	u16 RS4854RMF246_M_Timer;//6 RS485-4停电事件抄读间隔分定时器(载波)

#if ((USER/100)==14)//福建
	u16 FuJBroadCastTimer;//7 福建广播任务分定时器
	u16 FuJAutoSearchTimer;//8 福建主动注册任务分定时器
	u16 FuJCollectTaskTimer;//9 福建任务方案定时器
#define Terminal_M_Timer_Num     9//终端分定时器数
#endif
#ifndef Terminal_M_Timer_Num 
#define Terminal_M_Timer_Num     6//终端分定时器数
#endif

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
	u8 AFN04F10F11F13F35UpDate;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新;暂无实质性使用
	u8 AFN04F150_Computer_RTCS;//测量点有效标志计算原实时钟秒
	u8 AutoUpdataAFN04F10ERC3;//0=无,0x55=有AFN04F10更新但在10分内不再上报情况
	u8 AFN04F10F35Update;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
	u8 RoundTripFlags;//终端轮次跳闸位标志,B0-B7=0没循跳,=1已循跳
	u8 MODULSEL;//无线模块选择0=,1=华为CDMA MC323,2=GPRS标准3763,3=模块盒没插入
	u8 ENETSEL;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
	u8 Signal;//GPRS信号强度0-4
	u8 SignaldBm;//信号强度0-31或99(BCD码)
	u8 F246PnListUpDate;//要抄读的掉电记录数据Pn列表已更新标志,0=要更新,!=0已更新
	u8 PowerDownTask;//电源掉电过程任务
	u8 PowerDownSubTask;//电源掉电过程子任务
	u16 CopyTxByte;//寄存GPRS需发送字节数(半字对齐)
  u16 CopyTxCount;//寄存GPRS已发送计数(半字对齐)
	u16 ReqTxByte;//寄存GPRS请求发送字节数(半字对齐)
	u8 PowerNote;//上电事件记录:0=没记录,!=0已记录;
	u8 PowerTask;//停电事件任务
	u32 res13;//菜单中已使用了(黄从海)	
	u32 res4;
	u32 res5;
	u16 res6;
#if (USER/100)==14//福建系		4G,SIM7100模块
#define ADDR_AFN0CF251     ADDR_Other+42
	u16 PowerState;//电源工作状态  2BYTE  Bit5停电超级电容(0正常，1故障)   Bit4时钟电池 (0正常，1欠压)
								//Bit3停电工作电池(0正常，1故障)    Bit2Bit1供电方式(00主电源，01辅助电源，10电池供电)
								
#define ADDR_AFN0CF252     ADDR_AFN0CF251+2								
	u16 BatterCapacity;//停电工作电池有效容量  2BYTE  数据格式07  0-100
	u16 BatterV;//停电工作电池电压  2BYTE  数据格式07  V

#else
	u16 res7;
	u16 rse8;
	u16 rse9;	
#endif
	u32 res10;
	u32 res11;
	u32 res12;
	u16 res3;
//#define ADDR_AFN0CF253    ADDR_Other+42+6
//	u32 Lac;//基站号  BCD  4BYTE
//	u32 CELL_ID;//小区号   BCD  3BYTE
//	u16 ARFCN;//频点号     BCD   2BYTE
//	u16 RxLevel;//接收信号强度   2BYTE   dBm  BCD（数据格式02）   一般在-10~-140之间
//	u16 TxPower;//当前发送功率   2BYTE   dBm  BCD（数据格式02）	
	u16 ALIGN_FALG;//对齐测试标志  0：未测试   0xffff：测试数据都对齐
}Terminal_Ram_TypeDef;
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


//类3(事件)秒减定时器
#define ADDR_Class3Data_ESRAM_Start     (((ADDR_Other+64)+3)&0xfffffffc)//ERAM需半字对齐
#define ADDR_ERC10_Timer     ADDR_Class3Data_ESRAM_Start//ERC10断相失压事件延时秒定时器
#define ERC10_TNum     AFN04F25MaxPn//ERC10断相失压事件秒定时器个数
#define ADDR_ERC22_Timer     ADDR_ERC10_Timer+(2*ERC10_TNum)//ERC22差动越限事件秒定时器
#define ERC22_TNum     MaxDiffGroup//ERC22差动越限事件秒定时器个数
#define ADDR_ERC29_Timer     ADDR_ERC22_Timer+(2*ERC22_TNum)//ERC29电能表飞走秒定时器
#define ERC29_TNum     ERC29MaxPn//ERC29电能表飞走秒定时器个数
#define ADDR_ERC30_Timer     ADDR_ERC29_Timer+(2*ERC29_TNum)//ERC30电能表停走秒定时器
#define ERC30_TNum     ERC30MaxPn//ERC30电能表停走秒定时器个数

#define EVENT_S_TNum     (ERC10_TNum+ERC22_TNum+ERC29_TNum+ERC30_TNum)//总秒定时器个数

//类3(事件)原始数据
#define ADDR_ERC8_SOUR     ADDR_ERC10_Timer+(2*EVENT_S_TNum)//ERC8电能表参数变更事件原始数据
//1BYTE B0-B7分别表示第1套第1-8日时段表0=没抄1=正确抄(上电初始化0xee)
//4BYTE 第1套第1-8日时段表对正确抄数据的CRC(上电初始化0xee)
//1BYTE B0-B7分别表示第2套第1-8日时段表0=没抄1=正确抄(上电初始化0xee)
//4BYTE 第2套第1-8日时段表对正确抄数据的CRC(上电初始化0xee)
//6BYTE 每月第1-3结算日(上电初始化0xee)
//3BYTE 电表有功常数(上电初始化0xee)
//3BYTE 电表无功常数(上电初始化0xee)
//3BYTE 电流互感器变比(上电初始化0xee)
//3BYTE 电压互感器变比(上电初始化0xee)
//6BYTE 最近一次编程发生时刻(上电初始化0xee)
//3BYTE 最大需量清零次数(上电初始化0xee)
#define ERC8_SOUR_Pn_LEN     37//ERC8电能表参数变更事件原始数据每Pn长度
#define ADDR_ERC9_SOUR     ADDR_ERC8_SOUR+(ERC8_SOUR_Pn_LEN*ERC8MaxPn)//ERC9电流回路异常事件
//1BYTE B0-B5每2位分别表示A,B,C相,值0-3:0=空、1=短路、2=开路、3=反向(初始化值0)
//3BYTE A相断流总次数(初始化0xee)
//3BYTE B相断流总次数(初始化0xee)
//3BYTE C相断流总次数(初始化0xee)
#define LEN_ERC9_SOUR_Pn     10//ERC9电流回路异常事件原始数据每Pn长度
#define ADDR_ERC10_SOUR     ADDR_ERC9_SOUR+(LEN_ERC9_SOUR_Pn*ERC9MaxPn)//ERC10电压回路异常事件
//1BYTE 0 B0-B5每2位分别表示A,B,C相原事件记录,值0-3:0=恢复、1=断相、2=失压、3=(初始化值0)
//1BYTE 1 瞬时断相失压状态(初始化值0)
//3BYTE 2 全失压总次数(初始化0xee)
//3BYTE 5 失压总次数,DL645-1997表时为总断相次数(初始化0xee)
//3BYTE 8 A断相次数(初始化0xee)
//3BYTE 11 B断相次数(初始化0xee)
//3BYTE 14 C断相次数(初始化0xee)
//3BYTE 17 A相失压次数(初始化0xee)
//3BYTE 20 B相失压次数(初始化0xee)
//3BYTE 23 C相失压次数(初始化0xee)
#define LEN_ERC10_SOUR_Pn     26//ERC10电压回路异常事件原始数据每Pn长度
#define ADDR_ERC11_SOUR     ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*ERC10MaxPn)//ERC11相位异常事件
#if (AFN0CF49MaxPn%8)==0x0
#define ERC11_SOUR_LEN     (AFN0CF49MaxPn/8)
#else
#define ERC11_SOUR_LEN     ((AFN0CF49MaxPn/8)+1)
#endif
//每测量点用1个位表示原状态,0=恢复,1=产生
#define ADDR_ERC12_SOUR     ADDR_ERC11_SOUR+ERC11_SOUR_LEN//ERC12电能表时间超差事件
#if (AFN0CF27MaxPn%8)==0x0
#define ERC12_SOUR_LEN     (AFN0CF27MaxPn/8)
#else
#define ERC12_SOUR_LEN     ((AFN0CF27MaxPn/8)+1)
#endif
//每测量点用1个位表示原状态,0=恢复,1=产生
#define ADDR_ERC13_SOUR     ADDR_ERC12_SOUR+ERC12_SOUR_LEN//ERC13电表故障信息事件原始数据
//3BYTE 电能表编程次数(上电初始化0xee)
//3BYTE 最大需量清零次数(上电初始化0xee)
//1BYTE B0-B2分别表示A,B,C断相次数抄表标志
//3BYTE 电能表断相次数,A,B,C断相次数和(上电初始化0xee)
//3BYTE 电能表失压次数(上电初始化0xee)
//3BYTE 电能表停电次数(上电初始化0xee)
//1BYTE 电能表电池欠压,0=没欠,1=欠压(上电初始化0xee)
#define ERC13_SOUR_Pn_LEN     17
#define ADDR_ERC14SOUR     ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*ERC13MaxPn)//ERC14事件记录标志
//1BYTE 0 上电事件记录:0=没记录,!=0已记录;
//1BYTE 1 
//1BYTE 2 ERC
//1BYTE 3 事件长度
//1BYTE 4 属性标志
//5BYTE 5 最近次停电时间
//5BYTE 10 最近次上电时间
//1BYTE 15 停电时记为0xee,上电记录时清为0,用于记录指针加1识别
//6BYTE 16 最近次停电时间YMDHMS
//6BYTE 22 最近次上电时间YMDHMS
//1byte 28 
#define ADDR_ERC22_TotalEnergy     ADDR_ERC14SOUR+29//ERC22区间定时总加电量
//差动组1
//4BYTE 总加组1总电量,数据格式03
//...
//4BYTE 总加组N总电量,数据格式03
//差动组2
//4BYTE 总加组1总电量,数据格式03
//...
//4BYTE 总加组N总电量,数据格式03
//...
//差动组N
#define ADDR_ERC22_Remainder     ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*MaxDiffGroup)//ERC22区间定时总加电量计算用小数位
//总加组1
//4BYTE 总,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N
#define ADDR_ERC22_SOUR     ADDR_ERC22_Remainder+(4*(TMaxTariff+1)*MaxTotalAddGroup)//原差动组超限标志,0=没超,1=超限
//1BYTE 原差动组超限标志,0=没超,1=超限
//..
//1BYTE
#define ADDR_ERC27_SOUR     ADDR_ERC22_SOUR+MaxDiffGroup//ERC27电能表示度下降事件
//1BYTE 0=没记录,!=0已记录
#define LEN_ERC27_SOUR     1
#define ADDR_ERC29_SOUR     ADDR_ERC27_SOUR+(LEN_ERC27_SOUR*ERC27MaxPn)//ERC29电能表飞走事件,ERC28电能表超差事件
//5BYTE 0 上次抄表时间秒分时日月或测量点正有功电能示值;初始化值0xee
//1BYTE 5 飞走事件:B0=0恢复=1产生;B4-B7=恢复延时计数(376.1测试软件问题恢复事件需延时2个抄表周期后产生);初始化值0
//1BYTE 6 超差事件:B0=0恢复=1产生;初始化值0
//3BYTE 7 日正有功功率平均加的年月日(BCD);初始化值0
//3BYTE 10 日正有功功率平均加次数(HEX);初始化值0
//6BYTE 13 日正有功功率加(HEX);初始化值0
//3BYTE 19 上日正有功平均功率(BCD);初始化值0xee(仅1字节)
#define LEN_ERC29_SOUR     22
#define ADDR_ERC30_SOUR     ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*ERC29MaxPn)//ERC30电能表停走事件定时开始时电量值
//测量点1-N
//5BYTE 测量点正有功电能示值;初始化值0xee
//1BYTE 恢复产生标志0=恢复,1=产生;初始化值0
#define LEN_ERC30_SOUR     6
#define ADDR_ERC37_38_SOUR     ADDR_ERC30_SOUR+(LEN_ERC30_SOUR*ERC30MaxPn)//ERC37ERC38电能表开表盖端盖事件记录原始数据
//测量点1-N
//3BYTE 原开表盖次数;初始化值0xeeeeee
//3BYTE 原开端盖次数;初始化值0xeeeeee
#define LEN_ERC37_38_SOUR     6
#define ADDR_ERC39_SOUR     ADDR_ERC37_38_SOUR+(LEN_ERC37_38_SOUR*ERC37MaxPn)
//1byte ERC39补抄失败事件记录标志:0=空(上电或日更新),1=失败记录,2=成功记录
#define ADDR_ERC40_SOUR     ADDR_ERC39_SOUR+MaxRS485AddCarrierPn//ERC40磁场异常事件记录
//1BYTE 设备类型0(终端外接传感器),B0-B7位表示产生恢复:b0=高频磁场异常,b1=工频磁场异常,b2=强磁场异常;初始化值0
//测量点1-N
//1BYTE 设备类型1(采集器),B0-B7位表示产生恢复:b0=高频磁场异常,b1=工频磁场异常,b2=强磁场异常;初始化值0
//1BYTE 设备类型2(电能表),B0-B7位表示产生恢复:b0=高频磁场异常,b1=工频磁场异常,b2=强磁场异常;初始化值0
//3BYTE 原电能表恒定磁场干扰发生总次数;初始化值0xee
#define LEN_ERC40_SOUR     5
#define ADDR_ERC45_SOUR     ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*ERC40MaxPn)//ERC45终端电池失效事件
//1byte 恢复产生标志0=恢复,1=产生;初始化值0

	#if (USER/100)==5//上海
#define ADDR_ERC41_SOUR     ADDR_ERC45_SOUR+1//ERC41直接读电能表对时事件记录
//测量点1-N
//2byte 原读对时事件的月日;初始化值0xee
//3byte 原电能表时钟修改次数;初始化值0xee
#define LEN_ERC41_SOUR     5
#define ADDR_ERC12_SOUR_Ext     ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*MaxRS485AddCarrierPn)
//3BYTE 上次校对时钟的年月日或周;初始化值0
#define LEN_ERC12_SOUR_Ext     3
#define ADDR_ERC68_SOUR     ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*MaxRS485AddCarrierPn)//ERC68电能表负荷开关误动作事件(上海扩展)
//测量点1-N
//3byte 原动作次数;初始化值0xeeeeee
#define ADDR_Class3Data_ESRAM_End     (ADDR_ERC68_SOUR+(3*MaxRS485AddCarrierPn))
	#else
#define ADDR_Class3Data_ESRAM_End     (ADDR_ERC45_SOUR+1)
	#endif


//统计用原始数据(初始化命令全部清除)
#define ADDR_Terminal_StatData_ESRAM_Start     ADDR_Class3Data_ESRAM_End
#define ADDR_AFN0DF25F33_SOUR     ADDR_Terminal_StatData_ESRAM_Start//有功功率为零时间分更新加计数
//1BYTE 分更新加计数
#define ADDR_AFN0DF57F60_SOUR     ADDR_AFN0DF25F33_SOUR+(1*AFN0DF25MaxPn)//总加组有功功率为零时间分更新加计数
//1BYTE 分更新加计数
#define ADDR_AFN0DF57F60_S_Timer     ADDR_AFN0DF57F60_SOUR+(1*AFN0DF57MaxPn)//总加组最大最小有功功率在抄表完成后计算秒定时器
//1BYTE	秒减定时器
#define ADDR_AFN0DF27F35_SOUR     ADDR_AFN0DF57F60_S_Timer+1//日月电压统计
//1BYTE 分更新加计数
//2BYTE A电压越限持续时间秒定时
//1BYTE 当前A电压越限标志,0=没越限,1=越上上限,2=越下下限
//1BYTE A电压越限稳定标志,0=没越限,1=越上上限,2=越下下限
//2BYTE B电压越限持续时间秒定时
//1BYTE 当前B电压越限标志,0=没越限,1=越上上限,2=越下下限
//1BYTE B电压越限稳定标志,0=没越限,1=越上上限,2=越下下限
//2BYTE C电压越限持续时间秒定时
//1BYTE 当前C电压越限标志,0=没越限,1=越上上限,2=越下下限
//1BYTE C电压越限稳定标志,0=没越限,1=越上上限,2=越下下限
//1BYTE 越限事件记录标志,B0-B2分别ABC越上上限,B4-B6分别ABC越下下限
#define LEN_AFN0DF27F35_SOUR     14
#define ADDR_AFN0DF29F37_SOUR     ADDR_AFN0DF27F35_SOUR+(LEN_AFN0DF27F35_SOUR*AFN0DF27MaxPn)//日月电流统计
//1BYTE 分更新加计数
//2BYTE A电流越限持续时间秒定时
//1BYTE 当前A电流越限标志,0=没越限,1=越上上限,2=越上限
//1BYTE A电流越限稳定标志,0=没越限,1=越上上限,2=越上限
//2BYTE B电流越限持续时间秒定时
//1BYTE 当前B电流越限标志,0=没越限,1=越上上限,2=越上限
//1BYTE B电流越限稳定标志,0=没越限,1=越上上限,2=越上限
//2BYTE C电流越限持续时间秒定时
//1BYTE 当前C电流越限标志,0=没越限,1=越上上限,2=越上限
//1BYTE C电流越限稳定标志,0=没越限,1=越上上限,2=越上限
//2BYTE 零序电流越限持续时间秒定时
//1BYTE 当前零序电流越限标志,0=没越限,1=越上限
//1BYTE 零序电流越限稳定标志,0=没越限,1=越上限
//1BYTE B0-B2分别ABC越上上限事件已记录,B4-B6分别ABC越上限事件已记录
#define LEN_AFN0DF29F37_SOUR     18
#define ADDR_AFN0DF28F36_SOUR      ADDR_AFN0DF29F37_SOUR+(LEN_AFN0DF29F37_SOUR*AFN0DF29MaxPn)//电压电流不平衡
//1BYTE 分更新加计数
//2BYTE 电流不平衡越限持续时间秒定时
//1BYTE 当前电流不平衡越限标志,0=没越限,1=越限
//1BYTE 电流不平衡越限稳定标志,0=没越限,1=越限
//2BYTE 电压不平衡越限持续时间秒定时
//1BYTE 当前电压不平衡越限标志,0=没越限,1=越限
//1BYTE 电压不平衡越限稳定标志,0=没越限,1=越限
//1BYTE 事件已记录标志,B0=电压不平衡越限,B1=电流不平衡越限
//2BYTE 当前电流不平衡度(上电和初始化时清为0)
//2BYTE 当前电压不平衡度(上电和初始化时清为0)
#define LEN_AFN0DF28F36_SOUR     14
#define ADDR_AFN0DF30F38_SOUR     ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*AFN0DF28MaxPn)//视在功率越限
//1BYTE 分更新加计数
//2BYTE 视在功率越限持续时间秒定时
//1BYTE 当前视在功率越限标志,0=没越限,1=越上上限,2=越上限
//1BYTE 视在功率越限稳定标志,0=没越限,1=越上上限,2=越上限
//1BYTE 视在功率越限事件记录标志,0=没,B0=越上上限,B1=越上限
#define LEN_AFN0DF30F38_SOUR     6
#define ADDR_AFN0DF65F66_SOUR     ADDR_AFN0DF30F38_SOUR+(LEN_AFN0DF30F38_SOUR*AFN0DF30MaxPn)//总加组超功率定值月电能量定值
//1BYTE 分更新加计数
//4BYTE 功控超定值已用于计算的原总加有功电能量
//4BYTE 超月电能量定值已用于计算的原总加有功电能量
#define ADDR_AFN0DF43F44_SOUR     ADDR_AFN0DF65F66_SOUR+(9*AFN0DF65MaxPn)//功率因数区段累计时间
//1BYTE 分更新加计数
#define ADDR_AFN0DF129F130_SOUR     ADDR_AFN0DF43F44_SOUR+(1*AFN0DF43MaxPn)//测量点直流模拟量越限累计时间
//1BYTE 分更新加计数
//1BYTE 当前越限标志,B0=越上限,B1=越下限
//1BYTE 事件已记录标志,B0=越上限,B1=越下限
#define LEN_AFN0DF129F130_SOUR     3



//2类冻结用原始数据
#if (IC_SDRAM==0)
#define ADDR_CLASS2DATASOUR_ESRAM_Start     ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*AFN0DF129MaxPn)
#define ADDR_CLASS2DATASOUR_ESRAM_End     ADDR_CLASS2DATASOUR_ESRAM_Start
#else//#if (IC_SDRAM==0)
#define ADDR_CLASS2DATASOUR_ESRAM_Start     ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*AFN0DF129MaxPn)
#define  ADDR_AFN0DF113_SOUR      ADDR_CLASS2DATASOUR_ESRAM_Start//测量点A相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF114_SOUR      ADDR_AFN0DF113_SOUR+(114*AFN0DF113MaxPn)//测量点B相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF115_SOUR      ADDR_AFN0DF114_SOUR+(114*AFN0DF114MaxPn)//测量点C相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF116_SOUR      ADDR_AFN0DF115_SOUR+(114*AFN0DF115MaxPn)//测量点A相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF117_SOUR      ADDR_AFN0DF116_SOUR+(114*AFN0DF116MaxPn)//测量点B相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF118_SOUR      ADDR_AFN0DF117_SOUR+(114*AFN0DF117MaxPn)//测量点C相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF121_SOUR      ADDR_AFN0DF118_SOUR+(114*AFN0DF118MaxPn)//测量点A相谐波越限日统计数据
//日冻结
#define  ADDR_AFN0DF122_SOUR      ADDR_AFN0DF121_SOUR+(77*AFN0DF121MaxPn)//测量点B相谐波越限日统计数据
//日冻结
#define  ADDR_AFN0DF123_SOUR      ADDR_AFN0DF122_SOUR+(77*AFN0DF122MaxPn)//测量点C相谐波越限日统计数据
//日冻结


#define ADDR_HOURFLASH_RAM_START     ADDR_AFN0DF123_SOUR+(77*AFN0DF123MaxPn)//每小时RAM数据保存到FLASH开始地址
//1byte 上次RAM保存到FLASH的实时钟时(上电清为0,日月冻结后清为0xff使立即进行保存)
#define ADDR_AFN0DF124_SOUR     ADDR_HOURFLASH_RAM_START+1//日冻结A 相电压谐波电压含有率、谐波电流95%概率值
//1BYTE 上次计算的实时钟分;每0,15,30,45分计算每日最多取96点,每小时数据保存到FLASH
//2BYTE 计算次数计数
//1BYTE 谐波次数N（≤19）
//2BYTE 总畸变电压含有率95%概率值(半字对齐)
//2byte 奇次畸变电压含有率95%概率值
//2byte 偶次畸变电压含有率95%概率值
//2*18byte 2-19次谐波电压含有率95%概率值
//2byte 总畸变电流95%概率值
//2*18byte 2-19次谐波电流95%概率值

//2*5BYTE 总畸变电压含有率95%概率值(第5大值-第1大值)
//2*5BYTE 奇次畸变电压含有率95%概率值(第5大值-第1大值)
//2*5BYTE 偶次畸变电压含有率95%概率值(第5大值-第1大值)
//2*5*18BYTE 2-19次谐波电压含有率95%概率值(第5大值-第1大值)
//2*5byte 总畸变电流95%概率值(第5大值-第1大值)
//2*5*18byte 2-19次谐波电流95%概率值(第5大值-第1大值)
#define LEN_AFN0DF124_SOUR     (4+80+(80*5))
#define ADDR_AFN0DF125_SOUR     ADDR_AFN0DF124_SOUR+(LEN_AFN0DF124_SOUR*AFN0DF124MaxPn)//日冻结B 相电压谐波电压含有率、谐波电流95%概率值
//同ADDR_AFN0DF124_SOUR
#define LEN_AFN0DF125_SOUR     LEN_AFN0DF124_SOUR
#define ADDR_AFN0DF126_SOUR     ADDR_AFN0DF125_SOUR+(LEN_AFN0DF125_SOUR*AFN0DF125MaxPn)//日冻结C 相电压谐波电压含有率、谐波电流95%概率值
//同ADDR_AFN0DF124_SOUR
#define LEN_AFN0DF126_SOUR     LEN_AFN0DF124_SOUR
#define ADDR_AFN0DF211_SOUR     ADDR_AFN0DF126_SOUR+(LEN_AFN0DF126_SOUR*AFN0DF126MaxPn)//日冻结三相电压、电流不平衡度95%概率值
//1BYTE 上次计算的实时钟分;每0,15,30,45分计算每日最多取96点;每小时数据保存到FLASH
//1BYTE 保留,为半字对齐
//2BYTE 计算次数计数
//2BYTE 电流不平衡度95%概率值(半字对齐)
//2BYTE 电压不平衡度95%概率值
//2*5BYTE 电流不平衡度95%概率值(第5大值-第1大值)
//2*5BYTE 电压不平衡度95%概率值(第5大值-第1大值)
#define LEN_AFN0DF211_SOUR     28
#define ADDR_AFN0DF212_SOUR     ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*AFN0DF211MaxPn)//月冻结三相电压、电流不平衡度95%概率值
//1BYTE 上次计算的实时钟分;每0,15,30,45分计算每月最多取96*31点;每小时数据保存到FLASH
//1BYTE 保留,为半字对齐
//2BYTE 计算次数计数
//2BYTE 电流不平衡度95%概率值(半字对齐)
//2BYTE 电压不平衡度95%概率值
//2*150BYTE 电流不平衡度95%概率值(第150大值-第1大值);96*31*0.05=148.8
//2*150BYTE 电压不平衡度95%概率值(第150大值-第1大值);96*31*0.05=148.8
#define LEN_AFN0DF212_SOUR     608
#define ADDR_HOURFLASH_RAM_END     ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*AFN0DF212MaxPn)//每小时RAM数据保存到FLASH结束地址
#define LEN_HOURFLASH_RAM     ((ADDR_HOURFLASH_RAM_END)-(ADDR_HOURFLASH_RAM_START))//每小时RAM数据保存到FLASH总长度
#if (LEN_HOURFLASH_RAM>LEN_HOURFLASH)
#error LEN_HOURFLASH_RAM>LEN_HOURFLASH
#endif


#define ADDR_ERC15_SOUR     ADDR_HOURFLASH_RAM_END//ERC15谐波越限事件原始数据
//2BYTE 秒加计数计算分
//3BYTE A电压谐波越限标志D0-D18分别表示总畸变,2次-19次
//3BYTE A电流谐波越限标志D0-D18分别表示总畸变,2次-19次
//3BYTE B电压谐波越限标志D0-D18分别表示总畸变,2次-19次
//3BYTE B电流谐波越限标志D0-D18分别表示总畸变,2次-19次
//3BYTE C电压谐波越限标志D0-D18分别表示总畸变,2次-19次
//3BYTE C电流谐波越限标志D0-D18分别表示总畸变,2次-19次
#define LEN_ERC15_SOUR     20
#define ADDR_CLASS2DATASOUR_ESRAM_End     ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*AFN0CF57MaxPn)

#endif//#if (IC_SDRAM==0)



#define ADDR_Terminal_StatData_ESRAM_End     (ADDR_CLASS2DATASOUR_ESRAM_End)//统计用原始数据(初始化命令全部清除)结束







//终端控制
#define ADDR_Terminal_CTRL_ESRAM_START     ADDR_Terminal_StatData_ESRAM_End//终端控制ESRAM开始地址

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START//终端费率时段和功控时段
//1BYTE 终端当前执行费率(HEX 费率1,2,3,4 分别对应尖,峰,平,谷等费率),0=无效
//1BYTE 
//1BYTE 当前功控时段号1-8,=0表示时段号>9错误
//1BYTE 0=当前功控时段号不控制,1或2当前功控时段号控制

#define ADDR_Terminal_Protect     ADDR_Terminal_Period+4//终端保电,0=不保电,1=命令保电,2=自动保电
//1BYTE 0=不保电,1=命令保电,2=自动保电
#define ADDR_Terminal_Attr_State     ADDR_Terminal_Protect+1//催费告警控制状态
//1BYTE 0=没输出,1=输出
#define ADDR_Terminal_Total_State     ADDR_Terminal_Attr_State+1//终端总跳闸
//1BYTE 位标志(轮次1-8)当前总跳闸状态(1跳闸,0允许合闸)
//1BYTE 位标志(轮次1-8)允许合闸通知(总跳闸状态相应位转为合闸时置位,通知定时到后清0)
//2BYTE 保留
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//控制模块输出输入数据
//1BYTE	要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
//1BYTE 要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯,b2-b6,b7=轮次继电器动作方式(0=脉冲1=内部自动测试时电平)
//1BYTE	从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
//1BYTE 从控制模块输入的断线标志字节1

#define Terminal_Ctrl_Pn_Len     12//终端控制每Pn数据长度
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4//终端控制-遥控跳闸
//控制轮次1-N
//1BYTE 控制状态,0=上电或不控制,1=投入,2=...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 
//2BYTE 跳闸时功率(数据格式2)
//1BYTE !=0值表示投入但没控原因号1=...
//1BYTE 
//2BYTE  
//4BYTE 
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*MaxControlRound)//终端控制-功控.当前功率下浮控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=投入,2=计算定值,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//2BYTE 跳闸时功率(数据格式2)
//1BYTE !=0值表示投入但没控原因号1=...
//1BYTE 
//2BYTE
//1BYTE 超定值标志0=没超,1=超定值
//1BYTE 当前功控跳闸轮次B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制-功控.营业报停功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//2BYTE 跳闸时功率(数据格式2)
//1BYTE !=0值表示投入但没控原因号1=...
//1BYTE 
//2BYTE
//1BYTE 超定值标志0=没超,1=超定值
//1BYTE 当前功控跳闸轮次B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制-功控.厂休功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//2BYTE 跳闸时功率(数据格式2)
//1BYTE !=0值表示投入但没控原因号1=...
//1BYTE 
//2BYTE
//1BYTE 超定值标志0=没超,1=超定值
//1BYTE 当前功控跳闸轮次B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制-功控.时段功控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
//1BYTE 已跳闸轮次B0-B7
//2BYTE 跳闸时功率(数据格式2)
//1BYTE !=0值表示投入但没控原因号1=...
//1BYTE 方案号0-2表示1-3
//2BYTE 当前时段定值
//1BYTE 超定值标志0=没超,1=超定值
//1BYTE 当前功控跳闸轮次B0-B7
//2BYTE
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制-电控.月电控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
//1BYTE 已跳闸轮次B0-B7
//4BYTE 月电量定值(数据格式3)
//2BYTE 
//1BYTE	
//3BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制-电控.购电控
//总加组1-N
//1BYTE 控制状态,0=上电或不控制,1=,2=,3=控制...,100=报警,101=跳闸准备,102=跳闸
//1BYTE 已跳闸轮次B0-B7
//4BYTE 
//1BYTE 购电类别在剩余电量(费)计算时识别,0=购电量,1=购电费
//1BYTE 
//4BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//终端控制ESRAM结束地址


//抄表成功失败计数和标志
#define ADDR_ERC21_Flag     ADDR_Terminal_CTRL_ESRAM_END//ERC21 485抄表故障事件记录标志
//1byte 0=没记录,1=已记录
#define ADDR_RM_CountFlags     (((ADDR_ERC21_Flag+1)+3)&0xfffffffc)//抄表成功失败计数和标志(ERAM需字对齐)
//2BYTE RS485-1	本轮抄表成功计数
//2BYTE RS485-1 本轮抄表失败计数
//2BYTE RS485-2	本轮抄表成功计数
//2BYTE RS485-2 本轮抄表失败计数
//2BYTE RS485-3	本轮抄表成功计数
//2BYTE RS485-3 本轮抄表失败计数
//2BYTE 载波口  本轮抄表成功计数
//2BYTE 载波口  本轮抄表失败计数

//2BYTE RS485-1	上轮抄表成功计数
//2BYTE RS485-1 上轮抄表失败计数
//2BYTE RS485-2	上轮抄表成功计数
//2BYTE RS485-2 上轮抄表失败计数
//2BYTE RS485-3	上轮抄表成功计数
//2BYTE RS485-3 上轮抄表失败计数
//2BYTE 载波口  上轮抄表成功计数
//2BYTE 载波口  上轮抄表失败计数
//(MaxRS485AddCarrierPn/4)字节,按电能表序号排列,每表用2位原电能表抄表标志(0=成功,1=失败->成功,2=成功->失败,3=失败)
#define ADDR_RM_CountFlags_End     (ADDR_RM_CountFlags+32+(MaxRS485AddCarrierPn/4))


#define ADDR_AFN0DF26F34_SOUR     ADDR_RM_CountFlags_End//日冻结日总及分相最大需量及发生时间
//测量点1
//5BYTE 上次计算确定最大需量的实时钟年月日时分
//1byte 上次计算平均加的实时钟分

//1BYTE 总有功功率加平均加次数(hex)
//1BYTE A有功功率加平均加次数(hex)
//1BYTE B有功功率加平均加次数(hex)
//1BYTE C有功功率加平均加次数(hex)

//4byte 总有功功率加(hex)
//4byte A有功功率加(hex)
//4byte B有功功率加(hex)
//4byte C有功功率加(hex)
//测量点n
#define LEN_AFN0DF26F34_SOUR     26
#define ADDR_AFN0DF26_SOUR     ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*AFN0DF26MaxPn)//日冻结日总及分相最大需量及发生时间
//测量点1 (初始值0,日冻结后清0)
//三相总有功最大需量	见附录A.23	kW	3
//三相总有功最大需量发生时间	见附录A.18	分时日	3
//A相有功最大需量	见附录A.23	kW	3
//A相有功最大需量发生时间	见附录A.18	分时日	3
//B相有功最大需量	见附录A.23	kW	3
//B相有功最大需量发生时间	见附录A.18	分时日	3
//C相有功最大需量	见附录A.23	kW	3
//C相有功最大需量发生时间	见附录A.18	分时日	3
//...
//测量点n (初始值0,日冻结后清0)
#define ADDR_AFN0DF34_SOUR     ADDR_AFN0DF26_SOUR+(24*AFN0DF26MaxPn)//月冻结日总及分相最大需量及发生时间
//测量点1 (初始值0,月冻结后清0)
//三相总有功最大需量	见附录A.23	kW	3
//三相总有功最大需量发生时间	见附录A.18	分时日	3
//A相有功最大需量	见附录A.23	kW	3
//A相有功最大需量发生时间	见附录A.18	分时日	3
//B相有功最大需量	见附录A.23	kW	3
//B相有功最大需量发生时间	见附录A.18	分时日	3
//C相有功最大需量	见附录A.23	kW	3
//C相有功最大需量发生时间	见附录A.18	分时日	3
//...
//测量点n (初始值0,月冻结后清0)

#define ADDR_KeyUserPnList     ((ADDR_AFN0DF34_SOUR+(24*AFN0DF34MaxPn)+3)&0xfffffffc)//重点户曲线冻结Pn列表,半字对齐
//2BYTE 测量点1冻结的曲线对应的实际测量点号,0为无效
//...
//2BYTE 测量点MaxKeyUser冻结的曲线对应的实际测量点号,0为无效
#define ADDR_F246PnList     ADDR_KeyUserPnList+(2*MaxKeyUser)//要抄读的掉电记录数据Pn列表,半字对齐
//2BYTE Pn=1冻结对应的实际测量点号,0为无效
//...
//2BYTE Pn=F246MaxPn冻结对应的实际测量点号,0为无效
#define ADDR_PnVSAFN04F10SetNo     ADDR_F246PnList+(2*F246MaxPn)//测量点号Pn对应AFN04F10配置序号,0为无效
//2byte 
//2byte p1对应AFN04F10配置序号,0为无效
//...
//2byte pn对应AFN04F10配置序号,0为无效
#define ADDR_PnVSSetList     ADDR_PnVSAFN04F10SetNo+2+(2*MaxRS485AddCarrierPn)//测量点号Pn对应AFN04F10配置列表
//6byte Pn=0
//6byte Pn=1对应AFN04F10配置
//6byte ...
//6byte Pn=n对应AFN04F10配置
#define ADDR_CurveCongealList     ADDR_PnVSSetList+6+(6*MaxRS485AddCarrierPn)//已有曲线冻结时标列表
//2byte B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
//5byte YMDHM
//1byte 
//...
#define ADDR_AllRMList     ((ADDR_CurveCongealList+2+(6*MaxCurveCongeal)+3)&0xfffffffc)//485口抄表信息表
typedef __packed struct
{
	u8 res1;//
	u8 res2;//
	u8 RMCompleteS[MaxRMTask];//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
}RMList_TypeDef;
#define LEN_AllRMList     (2+MaxRMTask)//每列表字节数
typedef __packed struct
{
  RMList_TypeDef     Pn[1+MaxRS485AddCarrierPn];
}AllRMList_TypeDef;
#define AllRMList     ((AllRMList_TypeDef *)(ADDR_AllRMList))//485口抄表信息表(按Pn=0-n排列)



#if ((Project/100)!=2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#define ADDR_ERAM1_End     ((ADDR_AllRMList+(LEN_AllRMList*(1+MaxRS485AddCarrierPn))+3)&0xfffffffc)//终端在eram数据结束地址
#else
//载波集中器数据区
#define ADDR_Router     ((ADDR_AllRMList+(LEN_AllRMList*(1+MaxRS485AddCarrierPn))+3)&0xfffffffc)//路由器
typedef __packed struct
{
	u8 RouterAddr[6];//路由主节点地址(总是放在最前,通信库地址定位用)//0xC014AD24
	u8 RouterCODE[4];//厂商代码 ASCII 2,芯片代码 ASCII 2
	u8 RouterVer[5];//路由器版本号,分别为:日,月,年,2BYTE版本

	u8 RouterCtrl;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
	u8 RouterCtrlTask;//路由器控制任务计数;完成后自动回到0
	u8 ReRouterCtrl;//路由器控制重发计数
	u8 RUNState;//从模块读取的当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
	u8 UpDateTask;//软件更新任务
	u8 UpDateReCount;//软件更新重试次数
	u8 res1;

	u8 RouterInitTask;//路由器初始化任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	u8 RouterInitSubTask;//路由器初始化子任务
	u8 RouterInitReCount;//路由器初始化命令重发计数
	u8 AFN04F10;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
	u8 MaxOverTime;//从节点监控最大超时时间秒
	
	u8 RouterSynTask;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
	u8 RouterRealTimeTask;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
	u8 RouterKeyUserTask;//载波重点户抄读任务

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
	u16 res5;
	u16 res6;
	u32 StopFlags;//暂停路由标志:(字对齐)
		//b0=抄表间隔定时,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
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
#if (USER/100)==14//福建系		
	u8 Protocol;//协议:2-福建增补  其他-国网
	u8 FuJTNum;//福建每次预告抄读对象数量 BIN 1	
	u16 FuJTSum;//福建每轮次支持的总预告任务对象数量  BIN 2
	u64 FuJAutoSerTime;//福建主动注册任务开始时间
	u8 res7[30];
#else	
	u8 res7[42];
#endif
    u8 MeterCourts;//台区识别标志
    u8 MaxTxNum;//最大并发数
    u8 HPLC_S_Sum;//宽带载波模块并发抄表计数
	u8 HPLC_Over;//0=空,!=0溢出;宽带载波并发溢出
	u16 HPLC_MeterNum;//当前宽带并发抄表序号，模块升级时作延时计数
    u8 TimerS;//并发超时时间定时s
    u8 TxNum;//还可发送帧数
    u8 HPLC_CurveTaSK;//载波模块抄读HPLC曲线 TASK  0:抄上一小时数据    1:抄当天0点到现在的数据    2:抄上一天0点到现在的数据
#if ((USER/100)==6)//湖南集中器
    u8 AFN04F251_FLAG;//需计算标志
#else
    u8 res9;
#endif
}Terminal_Router_TypeDef;
#define LEN_Router     128//路由器控制区数据长度
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

#define ADDR_NoteList     (ADDR_Router+LEN_Router)//路由器中节点表
typedef __packed struct
{
	u8 Addr[6];//从节点地址(字对齐)
	u16 MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
	u16 Pn;//对应测量点号
	u8 FnCount;//抄表Fn计数
	u8 RMCount;//抄表数据标识计数
	u8 ReRMCount;//重发计数
	u8 RMCountErr;//Fn内有抄读失败或不抄的数据标识计数
	u8 RMErr;//当前抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败
	u8 RMErr_0x901F;//载波DL645-1997日冻结0xE3无0x901F命令时用0x9010抄表的标记;0=空,1=发0x901f,2=0x901F抄表返回标识错误
	u8 FnCountSOfMeterEvent;//插入周期抄读电能表事件前原抄表的Fn计数
	u8 FnCountBackupOfAutoReport;//抄读主动上报前原抄表的Fn计数
	u8 FnCountSOfKeyUser;//插入每小时抄读曲线任务前原循抄任务的抄表Fn计数
	u8 FnCountS;//插入任务前原循抄任务的抄表Fn计数
	u16 res1;
	u8 CollectorAddr[6];//采集器地址
	u8 RMCompleteS[MaxRMTask];//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
	u8 FnDataBuff[LEN_UART4FnDataBuff];//Fn数据缓冲
#if HPLC==1
		u8 TxSEQ;//发送序列桢
		u8 EndRMCount;//抄表组桢计数       u8 TASK;//并发任务  //0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
		u16 res2;
#endif	
}NoteList_TypeDef;
#if HPLC==1
	#define LEN_NoteList     (28+MaxRMTask+LEN_UART4FnDataBuff+4)//每列表字节数  总长度需是4的倍数
#else
	#define LEN_NoteList     (28+MaxRMTask+LEN_UART4FnDataBuff)//每列表字节数  总长度需是4的倍数
#endif

	
typedef __packed struct
{
  NoteList_TypeDef     List[MaxRS485AddCarrierPn];
}Terminal_NoteList_TypeDef;
#define Terminal_Note     ((Terminal_NoteList_TypeDef *)(ADDR_NoteList))//载波路由器中节点表(按读路由和添加顺序排列)


#define ADDR_ERAM1_End     (ADDR_NoteList+(LEN_NoteList*MaxRS485AddCarrierPn))//终端在eram数据结束地址
#endif//#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=




//终端1类AFN0C数据
#define ADDR_AFN0C_Start     ADDR_ERAM1_End
#include "AFN0C.h"
#define LEN_AFN0C     (ADDR_AFN0C_End-ADDR_AFN0C_Start)//实时1类数据RAM缓冲总长度



#define ADDR_HOURCONGEAL_BUFF_START     ADDR_AFN0C_End//小时冻结缓冲开始地址
#include "AFN0C_HourCongeal_Buff.h"
#define ADDR_AFNOD_SOUR_Start     ADDR_HOURCONGEAL_BUFF_END//类2原始数据和类2数据冻结缓冲开始地址
#include "AFN0D_Congeal_Buff.h"



#define ADDR_AFN0D_UART_Buff_Start     ADDR_AFN0D_Congeal_Buff_End//集中器UART376.1召读2类当日数据时临时缓存区

#define ADDR_DATABUFF_ERAM     ADDR_AFN0D_Congeal_Buff_End+LEN_AFN0D_Congeal_Buff
#define LEN_DATABUFF_ERAM     (8*1024)//ERAM数据缓冲最小4096(第1次定义因用于分类事件存储)

//扩展
#define ADDR_ERAM_EXT_START     ((ADDR_DATABUFF_ERAM+LEN_DATABUFF_ERAM+3)&0xfffffffc)//字对齐
typedef __packed struct
{
	u16 MeterNo;//寄存对应电能表配置序号(半字对齐)(从1开始)
	u8 FnCount;//寄存抄表Fn计数
	u8 RMCount;//寄存抄表命令计数
	u8 RMTaskC;//寄存原抄表任务
	u8 RMEventTaskC;//寄存抄读电能表事件任务:0=空,1-3=1-3级
	u16 res2;
	u32 rse3;
	u32 res4;
}RMEventStack_TypeDef;//抄读电能表事件时的寄存栈定义
#define LEN_RMEventStack     16
typedef __packed struct
{
	u8 YMDHMS[6];//6byte YMDHMS 上次用于定时计算的年月日时分秒(全0表示无效)
	u8 YMDHMS1[6];//6byte YMDHMS 分级1事件下次定时开始的年月日时分秒(全0表示无效)
	u8 YMDHMS2[6];//6byte YMDHMS 分级2事件下次定时开始的年月日时分秒(全0表示无效)
	u8 YMDHMS3[6];//6byte YMDHMS 分级3事件下次定时开始的年月日时分秒(全0表示无效)
	u32 res2;
	u8 res3;
	u8 res4;
	u8 TaskStore;//暂存读电能表事件标志:0=没,1=有
	u8 StackCount;//栈计数(值1-3,4固定为临时用)
	//栈计数=0 栈空
	//栈1 事件抄读按优先级入栈
	//栈2 事件抄读按优先级入栈
	//栈3 事件抄读按优先级入栈
	//栈4 固定事件抄读临时退出入栈
	RMEventStack_TypeDef     RMEventStack[MaxMeterEventLevel];//
}UARTRMEvent_TypeDef;//UART口事件抄读结构定义
#define LEN_UARTRMEvent     (32+(LEN_RMEventStack*4))

#define ADDR_UARTRMEvent_4851     ADDR_ERAM_EXT_START//485-1
#define UARTRMEvent_4851     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4851))
#define ADDR_UARTRMEvent_4852     ADDR_UARTRMEvent_4851+LEN_UARTRMEvent//485-2
#define UARTRMEvent_4852     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4852))
#define ADDR_UARTRMEvent_4853     ADDR_UARTRMEvent_4852+LEN_UARTRMEvent//485-3
#define UARTRMEvent_4853     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4853))
#define ADDR_UARTRMEvent_4854     ADDR_UARTRMEvent_4853+LEN_UARTRMEvent//485-4(载波)
#define UARTRMEvent_4854     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4854))



#define ADDR_ERAM_END     ((ADDR_UARTRMEvent_4854+LEN_UARTRMEvent+3)&0xfffffffc)

#if (((Project/100)==2)&&((USER/100)==14))//集中器方案-福建系
#define MAX_HOSTACK_NUM 8//最大ACK记数
#define FUJ_RIni_S	150//起始任务号
#define FUJ_RIni_E	159//暂停任务编号
#define FUJ_RRun_S	160//福建载波任务
#define FUJ_RRun_E	170//结束任务号
#define FUJ_ROUTINIT(N)  ((FUJ_RIni_S<=N)&&(N<=FUJ_RIni_E))
#define FUJ_ROUTRUN(N)  ((FUJ_RRun_S<=N)&&(N<=FUJ_RRun_E))
#define FUJTASK_DISABLE_FLAG 	0x00//无效任务
#define FUJTASK_ENABLE_FLAG 	0x01//有效任务-等待判断
#define FUJTASK_RUNNING_FLAG	0x02//正在执行--已压入任务队列
#define FUJTASK_END_FLAG			0x03//已完成，无需运行
#define FUJTASK_ACT_FLAG			0x04//触发型任务
#define FUJTASK_PAUSE_FLAG		0x05//暂停任务
typedef __packed struct
{
	u8 State;//任务状态
	u8 Prio;//优先级
//	u8 TaskPerio;//任务执行周期数值
//	u8 TaskUnit;//任务执行周期单位
//	u8 RetryPerio;//失败重试周期数值
//	u8 RetryUnit;//失败重试周期单位
	u8 RetryTimes;//重试次数,首次读取失败时，从FLASH提取最大重试次数，每次执行，该值减1，为0时抄读失败，则停止
	u8 ReportFlag;//上报状态  0-无需上报  1-等待上报  2-等待被动召测 3-被动召测已完成
	u8 LastRTime[6];//上一次执行的时间--首轮启动时，为0
	u8 NextSTime[6];//下一次启动时间
}FuJTask_TypeDef;//福建定时任务方案判断变量
//#define ADDR_FUJTASK_BUF	(ADDR_UARTRMEvent_4854+LEN_UARTRMEvent)
#define ADDR_FUJTASK_BUF	(ADDR_ERAM_END)
#define FuJClloectTask     ((FuJTask_TypeDef *)(ADDR_FUJTASK_BUF))
#define FUJ_CTL_LEN	16//sizeof(FuJTask_TypeDef)
#define LEN_FUJTASK_BUF		FUJ_CTL_LEN*MAX_FUJ_TASK//16byte*60000(1byte state + 4byte BaseTime + 2byte Per + 2byte RetryPer + 4byte CurRound)
//状态 基准时间  执行周期  重试周期  当前轮次((LastEndTime-BaseTime) / Per)--防止同一任务不停执行
typedef __packed struct
{
	u16 TaskID;//任务号1-60000
//	u32 StartTime;//起始时间
}FuJTaskStack_TypeDef;//福建任务队列,任务按照优先级大小及任务号大小排列,顺序由低到高，便于维护
#define ADDR_FUJTASK_STACK	(ADDR_FUJTASK_BUF+LEN_FUJTASK_BUF)
#define LEN_Per_StackObj		2
#define LEN_FUJTASK_STACK		LEN_Per_StackObj*MAX_FUJ_TASK
#define FuJTaskStack     ((FuJTaskStack_TypeDef *)(ADDR_FUJTASK_STACK))

#define ADDR_FUJTASK_PRIO_BUF	(ADDR_FUJTASK_STACK+LEN_FUJTASK_STACK)//福建任务队列中优先级对应的任务个数
#define LEN_FUJTASK_PRIO_BUF	2*254//2byte*优先级总数，每个优先级的任务方案数数量 0-253

typedef __packed struct
{
//任务总控制  4字节
	u8 TaskCtrl;//任务总控制
	u8 TaskFlag;//D0-D1终端主动上报状态 1允许，2禁止 D2-D3四类数据任务执行状态 1允许 2禁止	
	u16 Num;//自动更新已处理的总数量
	
//当前正在执行的任务控制  1+ 31字节
	u8 Res1;//备用	对齐
	u8 TaskState;//任务状态;0-99任务初始化;100-199任务执行阶段;200-219任务结束阶段
	u8 ReTimes;//重复周期数                  本轮次,当前时间与重试周期的比值
	u8 MaxCount;//最大重试次数
	u8 TaskReCount;//单帧重试计数
	u8 Port;//执行端口
	u16 TaskID;//任务号1-60000
	u8  STime[6];//起始时间-相对于任务起始时间
	u8  ETime[6];//本轮结束时间
	u32 Round;//轮次
	u16 SubTaskID;//当前正在抄读的具体任务号
	u16 Total;//采集总数量
	u16 Success;//采集成功数量
	u8  ReportFlag;//上报标志
	u8 FrameCheck;//报文格式字
	//预告对象控制  1+  15字节	
	u16 MeterIndex;//已提取的预告对象数量--根据具体任务数量定,通信地址可重复
	u16 LastIndex;//上一次提取位置
	u16 MeterSum;//已预告对象数量--
	u16 EndReadSum;//已抄读结束对象数量
	u16 CurNum;//当前正在预告的对象数量
	u8  Delay;//通信延时修正
	u8  Times;//预告总次数
	u8  TimeOutNum;//预告后无回复超时轮次
	u8 Res2;//备用	对齐	
	u16  S_Timer;//超时时间
//任务队列控制    35字节
	u16 Head;//队首位置
	u16 Depth;//任务队列总长度
	u16 SubTask[10];
	u8  Recount[10];//失败重试次数计数值
	u8 LogFlag;
	u8 Res[7];//备用
}FuJCurTask_TypeDef;//福建当前正在执行的任务
#define ADDR_FuJCurTask_CTRL	(ADDR_FUJTASK_PRIO_BUF+LEN_FUJTASK_PRIO_BUF)
#define LEN_FuJCurTask_CTRL		94//总字节长度94
#define FuJCurTask     ((FuJCurTask_TypeDef *)(ADDR_FuJCurTask_CTRL))

typedef __packed struct
{
	u16 SubTaskID;//任务号
	u8 Type;//通信对象类型
	u8 Addr[6];//通信对象地址
	u8 State;//状态  00-等待预告  1-正在进行预告 2-已预告 10-执行失败  11-执行成功	
}FuJTaskObj_TypeDef;//福建预告对象定义
#define ADDR_FuJTask_OBJ	(ADDR_FuJCurTask_CTRL+LEN_FuJCurTask_CTRL)//当前预告对象
#define LEN_FuJTask_OBJ_Per 10//sizeof(FuJTaskObj_TypeDef)
#define MAX_FuJTask_OBJ   2040//最大支持的对象数
#define LEN_FuJTask_OBJ		LEN_FuJTask_OBJ_Per*MAX_FuJTask_OBJ//总字节长度2040*10
#define FuJTaskObj     ((FuJTaskObj_TypeDef *)(ADDR_FuJTask_OBJ))

#define ADDR_FuJTask_Cur_OBJ	(ADDR_FuJTask_OBJ+LEN_FuJTask_OBJ)//当前预告对象
#define MAX_FuJTask_OBJ_Per		256//单次预告最大的对象数
#define LEN_FuJTask_Cur_OBJ 	MAX_FuJTask_OBJ_Per*9//



//485抄表队列
#define MAX_FuJTask_485_Num  100//485通道支持的最大任务数
#define ADDR_FUJTASK_485_STACK	(ADDR_FuJTask_Cur_OBJ+LEN_FuJTask_Cur_OBJ)
#define LEN_FUJTASK_485_STACK		LEN_Per_StackObj*MAX_FuJTask_485_Num

//485抄表具体任务列表
#define ADDR_FuJTask_485_OBJ	(ADDR_FUJTASK_485_STACK+LEN_FUJTASK_485_STACK)//当前预告对象
#define LEN_FuJTask_485_OBJ		(LEN_FuJTask_OBJ_Per*MAX_FuJTask_OBJ)//总字节长度2040*10

typedef __packed struct
{
//任务总控制    2字节
	u8 TaskCtrl;//任务总控制
	u8 TaskFlag;//D0-D1终端主动上报状态 1允许，2禁止 D2-D3四类数据任务执行状态 1允许 2禁止
	
//当前正在执行的任务控制   3+ 31字节
	u8 Res1[3];//备用	对齐	
	u8 TaskState;//任务状态;0-99任务初始化;100-199任务执行阶段;200-219任务结束阶段
	u8 ReTimes;//重复周期数
	u8 MaxCount;//最大重试次数
	u8 TaskReCount;//单帧重试计数
	u8 Port;//执行端口
	u16 TaskID;//任务号1-60000
	u8  STime[6];//起始时间-相对于任务起始时间
	u8  ETime[6];//本轮结束时间
	u32 Round;//轮次
	u16 SubTaskID;//当前正在抄读的具体任务号
	u16 Total;//采集总数量
	u16 Success;//采集成功数量
	u8  ReportFlag;//上报标志
	u8 FrameCheck;//报文格式字
	//预告对象控制     1+ 7字节
	u16 MeterIndex;//已提取的预告对象数量--根据具体任务数量定,通信地址可重复
	u16 CurNum;//当前正在预告的对象数量
	u8  Delay;//通信延时修正
	u8 Res2;//备用	对齐
	u16  S_Timer;//超时时间
//任务队列控制      2字节
	u16 Depth;//任务队列总长度
	u8 Res[18];//备用
}FuJCur485Task_TypeDef;//福建当前正在执行的任务
#define ADDR_FuJCurTask_485_CTRL	((ADDR_FuJTask_485_OBJ+LEN_FuJTask_485_OBJ+3)&0xFFFFFFFC)
#define LEN_FuJCurTask_485_CTRL		64//总字节长度64
#define FuJCur485Task     ((FuJCur485Task_TypeDef *)(ADDR_FuJCurTask_485_CTRL))


typedef __packed struct
{
	u8 ReportFlag;//54F22
	u8 AutoPackFlag;//批量召测标志
	u8 Num;//当前上报总数量
	u8 Res;//备用	对齐	
	u16 Task;//当前正在上报的任务号
	u16 SubTaskID;//具体任务对应的序号
	u32 CurRound;//当前读取的轮次
	u32 MaxRound;//最大轮次
	u32 EventIndex;//上一次上报的事件序号	
	u16 SubTaskIDList[15];//上报具体任务存储序号列表
	u16 EventNum[7];//正在上报的事件序号
}FuJTaskReport_TypeDef;//福建主动上报控制
#define ADDR_FuJTaskReportCtrl	(ADDR_FuJCurTask_485_CTRL+LEN_FuJCurTask_485_CTRL)
#define LEN_FuJTaskReportCtrl 64
#define FuJTaskReportCtrl  ((FuJTaskReport_TypeDef *)(ADDR_FuJTaskReportCtrl))
//LOG
#define ADDR_LOGOUTDATA		(ADDR_FuJTaskReportCtrl+LEN_FuJTaskReportCtrl)
#define LEN_LOGOUTDATA	(4+256*1024)//2byteLen+8190byteData
#define ADDR_ERAM_EXT_END     (ADDR_LOGOUTDATA+LEN_LOGOUTDATA)

//#define ADDR_ERAM_EXT_END     (ADDR_FuJTaskReportCtrl+LEN_FuJTaskReportCtrl)
#else
#define ADDR_TEMP_TIMER		ADDR_UARTRMEvent_4854+LEN_UARTRMEvent
#define ADDR_LOGOUTDATA   ADDR_TEMP_TIMER+1//打印LOG缓冲
#define LEN_LOGOUTDATA	(4+256*1024)//2byteLen+8190byteData
#define ADDR_ERAM_EXT_END     (ADDR_LOGOUTDATA+LEN_LOGOUTDATA)//ADDR_UARTRMEvent_4854+LEN_UARTRMEvent
#endif  
#if ((ADDR_ERAM_EXT_END)-ADDR_ERAM_START)>0x01000000
    #error ADDR_FuJTask_END>32M-16M
#endif


//剩余ERAM空间全用于自动分配(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>8M-2M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//堆为8字节对齐
	#define LEN_MyHeap     0x00200000//剩余ERAM空间长度
#endif
#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>16M-8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//堆为8字节对齐
	#define LEN_MyHeap     0x00800000//剩余ERAM空间长度
#endif
#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=没,1=有
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x01000000
    #error ADDR_ERAM_END>32M-16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x01000000)//堆为8字节对齐
	#define LEN_MyHeap     0x01000000//剩余ERAM空间长度
#endif

#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=没,1=有(64M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00A00000
    #error ADDR_ERAM_END>10M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00A00000)//堆为8字节对齐
	#ifndef LINUX
	#define LEN_MyHeap     0x02600000//剩余ERAM空间长度
	#else//#ifndef LINUX
	
	#define LEN_MyHeap     0//剩余ERAM空间长度
	
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

