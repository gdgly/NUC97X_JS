
#ifndef EFLASH_Terminal_H
#define EFLASH_Terminal_H




#define ADDR_DL698_SET_START     (ADDR_Terminal_EFLASH_Start)
#include "../H698/FLASH_DL698_SET.h"
#define ADDR_FLASH_DL698_DATA_START     ADDR_DL698_SET_START+LEN_FLASH_DL698_SET
#include "../H698/FLASH_DL698_DATA.h"
#define ADDR_FLASH_FILE_START     ADDR_FLASH_DL698_DATA_START+LEN_FLASH_DL698_DATA
#define msFILE//定义ms文件系统
#define LEN_msFILEunit     (128*1024)//文件分配单元


#define ADDR_Terminal_EFLASH_End     ADDR_FLASH_FILE_START+LEN_FLASH_FILE



#endif


