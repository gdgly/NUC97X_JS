


#ifndef EFLASH_H
#define EFLASH_H

#define ADDR_EFLASH_Start     0x30000000
// NAND_FILE_BLOCK_COUNT*128*1024字节长度文件系统
// NAND_REPLACE_BLOCK_COUNT*128*1024字节长度错误块替换
#define ADDR_Download_ENTRY     ADDR_EFLASH_Start+((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024)//块地址对齐
#define LENmax_Download         (1024*1024)
// 512K MS,OS,Router更新时备份


// 掉电时数据保护区--------------------------------------------------------------------------------
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
// 掉电时数据保护区结束----------------------------------------------------------------------------


//电能表数据----------------------------------------------------------------------------------------------
#define ADDR_Meter_EFLASH_Start     ADDR_DISK_End 		// (ADDR_Com_EFLASH_Start+LEN_Com_EFLASH)
#if ((Project/100)==5)||((Project/100)==6)				// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Meter_EFLASH_End     (ADDR_Meter_EFLASH_Start+0)
	#define LEN_Meter_EFLASH     0						// 限定总长度字节数
#else
	#include "EFLASH_MeterCSG.h"
	#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))		// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		#define LEN_Meter_EFLASH     (512*1024)			// 限定总长度字节数
	#else
		#define LEN_Meter_EFLASH     (6*1024*1024)							// 限定总长度字节数
	#endif
#endif

#if (ADDR_Meter_EFLASH_End-ADDR_Meter_EFLASH_Start)>LEN_Meter_EFLASH
#error ADDR_Meter_EFLASH_End Over
#endif
//电能表数据结束------------------------------------------------------------------------------------------





//终端数据-----------------------------------------------------------------------------------------------
#define ADDR_Terminal_EFLASH_Start     (ADDR_Meter_EFLASH_Start+LEN_Meter_EFLASH)
#if ((Project/100)==0)					// 方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define ADDR_Terminal_EFLASH_End     (ADDR_Terminal_EFLASH_Start+0)
	#define LEN_Terminal_EFLASH     0	// 限定总长度字节数
#else
	#if NAND_1G==1//0=没,1=有
		#define LEN_Terminal_EFLASH     (((128-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))	// 限定总长度字节数
		// NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		// 2M 减去Download,掉电,COM
		// 6M 减去Meter
		// 1M 减去LOG
		// 共减去33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (91*1024*1024)
	#endif
	#if NAND_2G==1	//0=没,1=有
		#define LEN_Terminal_EFLASH     (((256-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))	// 限定总长度字节数
		// NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		// 2M 减去Download,掉电,COM
		// 6M 减去Meter
		// 1M 减去LOG
		// 共减去33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (220*1024*1024)
	#endif
	#if NAND_4G==1	//0=没,1=有
		#define LEN_Terminal_EFLASH     (((512-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		// NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		// 2M 减去Download,掉电,COM
		// 6M 减去Meter
		// 1M 减去LOG
		// 共减去33M
		#define LEN_FLASH_QCSG_SET    	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (476*1024*1024)
	#endif
	#if NAND_8G==1	//0=没,1=有
		#define LEN_Terminal_EFLASH     (((1024-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//限定总长度字节数
		// NAND_FILE_BLOCK_COUNT*128*1024  减去文件系统16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 减去块替换8M
		// 2M 减去Download,掉电,COM
		// 6M 减去Meter
		// 1M 减去LOG
		// 共减去33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (732*1024*1024)
	#endif
	#include "EFLASH_TerminalCSG.h"
#endif

#if (ADDR_Terminal_EFLASH_End-ADDR_Terminal_EFLASH_Start)>LEN_Terminal_EFLASH
#error ADDR_Terminal_EFLASH_End Over
#endif
// 终端数据结束--------------------------------------------------------------------------------------------


#define ADDR_EFLASH_End     (ADDR_Terminal_EFLASH_Start+LEN_Terminal_EFLASH)

#if NAND_1G==1		//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x08000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_2G==1		//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x10000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_4G==1		//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x20000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_8G==1		//0=没,1=有
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x40000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif

// LOG数据(总是固定在nand最后块的1M空间)----------------------------------------------------------------------------

#if NAND_1G==1					// 0=没,1=有
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x08000000-0x00100000)
#endif
#if NAND_2G==1					// 0=没,1=有
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x10000000-0x00100000)
	#define LENmax_LOG     (3*1024*1024)//每个文件最大尺寸限制6M
#endif
#if NAND_4G==1					// 0=没,1=有
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x20000000-0x00100000)
#endif
#if NAND_8G==1					// 0=没,1=有
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x40000000-0x00100000)
#endif

#define ADDR_LOG_698        ADDR_LOG_START
// 128K space
// 128K space
// 128K space
#define ADDR_LOG_1376_1     	(ADDR_LOG_START+(3*128*1024))
// 128K
#define ADDR_LOG_1376_2     	(ADDR_LOG_1376_1+(128*1024))
// 128K
#define ADDR_LOG_1376_3     	(ADDR_LOG_1376_2+(128*1024))
// 128K
#define ADDR_LOG_645_1997     	(ADDR_LOG_1376_3+(128*1024))
// 128K
#define ADDR_LOG_645_2007     	(ADDR_LOG_645_1997+(128*1024))
// 128K
#define ADDR_LOG_END     		(ADDR_LOG_645_2007+(128*1024))

//LOG数据结束-----------------------------------------------------------------------------------------------









#endif





















