


#ifndef EFLASH_H
#define EFLASH_H

#define ADDR_EFLASH_Start     0x30000000
//NAND_FILE_BLOCK_COUNT*128*1024字节长度文件系统
//NAND_REPLACE_BLOCK_COUNT*128*1024字节长度错误块替换
#define ADDR_Download_ENTRY     ADDR_EFLASH_Start+((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024)//块地址对齐
#define LENmax_Download         (1024*1024)
//1024K MS,OS,Router更新时备份或下载传输暂存
	

//掉电时数据保护区--------------------------------------------------------------------------------
#define ADDR_DISK_Start     (ADDR_Download_ENTRY+LENmax_Download)
//128K 电源掉电时数据保存1//iMeter,Terminal
//128K 电源掉电时数据保存2//msFILE
//128K 电源掉电时数据保存3
//128K 电源掉电时数据保存4//128k NANDBuff

//128K 电池工作关机前数据保存1//iMeter,Terminal
//128K 电池工作关机前数据保存2//msFILE
//128K 电池工作关机前数据保存3
//128K 电池工作关机前数据保存4//128k NANDBuff

#define LEN_msFILEmanage_Ext     (256*1024)//msFILE扩展;0或128K的整数倍
//LEN_msFILEmanage_Ext字节 电源掉电时数据保存//msFILE扩展
//LEN_msFILEmanage_Ext字节 电池工作关机前数据保存//msFILE扩展

#define LEN_DISK     ((8*128*1024)+(2*LEN_msFILEmanage_Ext))//掉电时数据保护区长度
#define ADDR_DISK_End     (ADDR_DISK_Start+LEN_DISK)
//掉电时数据保护区结束----------------------------------------------------------------------------


//公共数据-----------------------------------------------------------------------------------------------
#define ADDR_Com_EFLASH_Start     ADDR_DISK_End
#define ADDR_RTCOffice     ADDR_Com_EFLASH_Start//终端时钟相对仪表时钟差值(仅网络表时用)
//4BYTE HEX补码,秒
#define ADDR_Language     ADDR_RTCOffice+4//显示语言同步RAM中的Comm_Ram->Language
//1BYTE 0=中文,1=英文,2=
//3BYTE 保留
#define ADDR_FileDown     ADDR_Language+4//自定义软件下载
//软件下载1
//1BYTE 信道0=RS232,1=GPRS或RJ45
//1BYTE 保留
//1BYTE 文件传输控制	BIN	1
//1BYTE 文件编号(0-255)	BIN	1
//16BYTE 文件名称	ASCII	16
//2BYTE 文件总段数n(n>=1)	BIN	2
//2BYTE 文件数据当前段数号（i=1~n）	BIN	2
//2BYTE 文件每段字节长度(L)	BIN	2
//4BYTE 已下载总字节数
//2BYTE 保留
//软件下载2
//....
//软件下载8
//...
#define ADDR_LEN_AFN0FF1    ADDR_FileDown+(32*8)//376.1 F1下载文件的总长度计算后寄存
//4byte
#define ADDR_AFN0FF1     ADDR_LEN_AFN0FF1+4//376.1 F1下载
//1BYTE 文件标识:00H清除下装文件;01H-06H升级文件
//1BYTE 文件属性：起始、中间帧：00H；结束帧：01H。
//1BYTE 文件指令：00H：报文方式下装；01H：FTP 方式下装；02H：启动组地址升级。
//2BYTE 总段数n
//4BYTE 第i 段标识或偏移（i=0～n）
//2BYTE 第i 段数据长度Lf
//xBYTE 文件数据
#define LEN_AFN0FF1     256
#define ADDR_AFN0CF14     ADDR_AFN0FF1+LEN_AFN0FF1//文件下装未收到数据段
//4BYTE 组号 BIN 4
//128BYTE 组内各数据段未收到标志 BS8 128
#define ADDR_FTPFileFlags     ADDR_AFN0CF14+4+128//FTP文件下载标志
//4BYTE 空
//1BYTE 下载文件计数;0-x,=MaxFileNum关FTP,=(MaxFileNum+1)返回帧,=(MaxFileNum+2)下载完成显示,=(MaxFileNum+3)结束下载显示
//1BYTE MS文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE OS文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE Meter文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE Terminal文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE Menu文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE Font文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE Module文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//1BYTE ?文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
//8BYTE 
#define ADDR_AFN0FF1_Rx     ADDR_FTPFileFlags+5+16//FTP接收帧的前256字节保存
//256BYTE
#define ADDR_AFN0FF1_Tx     ADDR_AFN0FF1_Rx+256//FTP返回帧的前18字节保存
//18BYTE
#define ADDR_UART_PORT_SET     ADDR_AFN0FF1_Tx+18//通信口设置
//UART0 (RS232)
//1BYTE 功能 0=终端监视口,!=0终端维护口
//1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE 保留
//1BYTE 保留
//UART2(RS485-1)
//1BYTE 功能 0=终端维护口,!=0终端抄表口
//1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE 保留
//1BYTE 保留
//UART3(RS485-2)
//1BYTE功能 0=终端维护口,!=0终端抄表口
//1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE 保留
//1BYTE 保留
//UART4(RS485-3)
//1BYTE 功能 0=终端维护口,!=0终端抄表口
//1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE 保留
//1BYTE 保留
//UART5(红外)
//1BYTE 功能 0=终端维护口,!=0终端抄表口
//1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE 保留
//1BYTE 保留
#define LEN_UART_PORT_SET     4


#define ADDR_Com_EFLASH_BASEEnd     (ADDR_UART_PORT_SET+(LEN_UART_PORT_SET*5))


#if (USER/100)==5//上海
#define ADDR_AFN0FF97     ADDR_Com_EFLASH_BASEEnd
//1byte 命令发起标志:0=不是F97命令,1=是F97命令发起
//FTP服务器IP地址	BIN	6
//FTP服务器IP地址端口号	BIN	2
//FTP服务器IP备用地址	BIN	6
//FTP服务器IP备用地址端口号	BIN	2
//用户名	BIN	10
//用户登录密码	BIN	10
//服务器程序文件路径	ASCII	20
//备用服务器文件路径	ASCII	20
//配置文件名	ASCII	10
//程序文件名1	ASCII	10
//程序文件名2	ASCII	10
//程序文件名3	ASCII	10
//程序文件名4	ASCII	10
#define ADDR_AFN0FF98     ADDR_AFN0FF97+127
//升级前终端软件版本号	ASCII	10
//升级后终端软件版本号	ASCII	10
//发生时间：分时日月年	见附录A.15	5
#define ADDR_Com_EFLASH_End     (ADDR_AFN0FF98+25)
#else
#define ADDR_Com_EFLASH_End     ADDR_Com_EFLASH_BASEEnd
#endif


#define LEN_Com_EFLASH     (128*1024)//限定总长度字节数(1个扇区)
#if (ADDR_Com_EFLASH_End-ADDR_Com_EFLASH_Start)>LEN_Com_EFLASH
#error ADDR_Com_EFLASH_End Over
#endif
//公共数据结束--------------------------------------------------------------------------------------------





//电能表数据----------------------------------------------------------------------------------------------
#define ADDR_Meter_EFLASH_Start     (ADDR_Com_EFLASH_Start+LEN_Com_EFLASH)
#if ((Project/100)==5)||((Project/100)==6)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
#define ADDR_Meter_EFLASH_End     (ADDR_Meter_EFLASH_Start+0)
#define LEN_Meter_EFLASH     0//限定总长度字节数
#else
#include "EFLASH_Meter698.h"
	#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
		#define LEN_Meter_EFLASH     (512*1024)//限定总长度字节数
	#else
		#define LEN_Meter_EFLASH     (6*1024*1024)//限定总长度字节数
	#endif
#endif

#if (ADDR_Meter_EFLASH_End-ADDR_Meter_EFLASH_Start)>LEN_Meter_EFLASH
#error ADDR_Meter_EFLASH_End Over
#endif
//电能表数据结束------------------------------------------------------------------------------------------





//终端数据-----------------------------------------------------------------------------------------------
#define ADDR_Terminal_EFLASH_Start     (ADDR_Meter_EFLASH_Start+LEN_Meter_EFLASH)
#if ((Project/100)==0)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Terminal_EFLASH_End     (ADDR_Terminal_EFLASH_Start+0)
	#define LEN_Terminal_EFLASH     0//限定总长度字节数
#else
	#if NAND_1G==1//0=没,1=有
		#define LEN_Terminal_EFLASH     (((128-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		//NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		//2M 减去Download,掉电,COM
		//6M 减去Meter
		//1M 减去LOG
		//共减去33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (91*1024*1024)
	#endif
	#if NAND_2G==1//0=没,1=有
	 #ifndef LINUX
		#define LEN_Terminal_EFLASH     (((256-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		//NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		//2M 减去Download,掉电,COM
		//6M 减去Meter
		//1M 减去LOG
		//共减去33M
			#if (USER/100)==14//福建
		#define LEN_FLASH_DL698_SET     (2*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (219*1024*1024)
			#else
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (220*1024*1024)
			#endif
			
	 #else//#ifndef LINUX
		#define LEN_Terminal_EFLASH     (((256-13)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		//NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统32M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		//2M nand_program
		//4M 减去Download,掉电,COM
		//6M 减去Meter
		//1M 减去LOG
		//共减去53M
			#if (USER/100)==14//福建
		#define LEN_FLASH_DL698_SET     (2*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (199*1024*1024)
			#else
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (200*1024*1024)
			#endif
	 #endif
	#endif
	#if NAND_4G==1//0=没,1=有
		#define LEN_Terminal_EFLASH     (((512-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		//NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		//2M 减去Download,掉电,COM
		//6M 减去Meter
		//1M 减去LOG
		//共减去33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (476*1024*1024)
	#endif
	#if NAND_8G==1//0=没,1=有
		#define LEN_Terminal_EFLASH     (((1024-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		//NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		//2M 减去Download,掉电,COM
		//6M 减去Meter
		//1M 减去LOG
		//共减去33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (732*1024*1024)
	#endif
	#include "EFLASH_Terminal698.h"
#endif

#if (ADDR_Terminal_EFLASH_End-ADDR_Terminal_EFLASH_Start)>LEN_Terminal_EFLASH
#error ADDR_Terminal_EFLASH_End Over
#endif
//终端数据结束--------------------------------------------------------------------------------------------


#define ADDR_EFLASH_End     (ADDR_Terminal_EFLASH_Start+LEN_Terminal_EFLASH)

#if NAND_1G==1//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x08000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_2G==1//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x10000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_4G==1//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x20000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_8G==1//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x40000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif

//LOG数据(总是固定在nand最后块的1M空间)----------------------------------------------------------------------------

#if NAND_1G==1//0=没,1=有
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x08000000-0x00100000)
#endif
#if NAND_2G==1//0=没,1=有
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x10000000-0x00100000)
	#define LENmax_LOG     (3*1024*1024)//每个文件最大尺寸限制6M
#endif
#if NAND_4G==1//0=没,1=有
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x20000000-0x00100000)
#endif
#if NAND_8G==1//0=没,1=有
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x40000000-0x00100000)
#endif

#define ADDR_LOG_698        ADDR_LOG_START
//128K space

//128K space
//128K space
#define ADDR_LOG_1376_1     ADDR_LOG_START+(3*128*1024)
//128K
#define ADDR_LOG_1376_2     (ADDR_LOG_1376_1+(128*1024))
//128K
#define ADDR_LOG_1376_3     (ADDR_LOG_1376_2+(128*1024))
//128K
#define ADDR_LOG_645_1997     (ADDR_LOG_1376_3+(128*1024))
//128K
#define ADDR_LOG_645_2007     (ADDR_LOG_645_1997+(128*1024))
//128K
#define ADDR_LOG_END     (ADDR_LOG_645_2007+(128*1024))

//LOG数据结束-----------------------------------------------------------------------------------------------









#endif





















