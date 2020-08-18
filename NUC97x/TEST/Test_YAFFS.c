
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/MS/MS.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Calculate/ascii.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"





#define TEST_YAFFS_BYTE     (1024*1024)

u32 Test_YAFFS(void)//测试YAFFS文件系统;返回：0=ok,1=fail
{
	u32 i;
	u32 LEN_File;
	u32* p32s;
	u32* p32d;
	os_Type *os;
	ms_Type *ms;
	int yaffs;
	struct yaffs_stat stat;
	u32 Data;
	u32 ReNum;
	
	Data=0;
	DisplayStringAutoUp(0,9,(u8*)"\x0""YAFFS");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	os=Get_os();
	ms=Get_ms();
	p32s=ms->ms_malloc(1024*1024);
	p32d=ms->ms_malloc(1024*1024);
	
	ReNum=2;
	while(ReNum--)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		for(i=0;i<TEST_YAFFS_BYTE/4;i++)
		{
			p32s[i]=Data;
			Data++;
		}
		Data-=TEST_YAFFS_BYTE/4;
		yaffs=os->yaffs_open("user/TEST/TEST.txt",O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
		if(yaffs<0)
		{
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		os->yaffs_WRITE(yaffs,(void *)p32s,TEST_YAFFS_BYTE);
		os->yaffs_close(yaffs);
		MR((u32)p32d,ADDR_Download_ENTRY,TEST_YAFFS_BYTE);
		
		yaffs=os->yaffs_open("user/TEST/TEST.txt",O_RDWR,0);
		if(yaffs<0)
		{
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		os->yaffs_fstat(yaffs, &stat);
		if(stat.st_size!=TEST_YAFFS_BYTE)
		{
			os->yaffs_close(yaffs);
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		LEN_File=os->yaffs_READ(yaffs,(void *)p32d,TEST_YAFFS_BYTE);
		if(LEN_File!=TEST_YAFFS_BYTE)
		{
			os->yaffs_close(yaffs);
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		os->yaffs_close(yaffs);
		for(i=0;i<TEST_YAFFS_BYTE/4;i++)
		{
			if(p32d[i]!=Data)
			{
				break;;
			}
			Data++;
		}
		if(i<TEST_YAFFS_BYTE/4)
		{
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		os->yaffs_unlink("user/TEST/TEST.txt");
		yaffs=os->yaffs_open("user/TEST/TEST.txt",O_RDWR,0);
		if(yaffs>=0)
		{
			ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
			os->yaffs_mount("user");
			continue;
		}
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
		return 0;
	}
	DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	return 1;
}



void Test_YAFFS_LOOP(void)//循环测试YAFFS文件读写可靠性
{
	u32 i;
	u32 LEN_File;
	u16* p16;
	u32* p32s;
	u32* p32d;
	os_Type *os;
	ms_Type *ms;
	int yaffs;
	struct yaffs_stat stat;
	u32 Count;
	u32 Data;

	Count=0;
	Data=0;
	DisplayClr();//清屏
	DisplayStringAutoUp(0,9,(u8*)"\x0""YAFFS TEST");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	os=Get_os();
	ms=Get_ms();
	p32s=ms->ms_malloc(1024*1024);
	p32d=ms->ms_malloc(1024*1024);
	
//---TEST---
//for(i=0;i<NAND_FILE_BLOCK_COUNT;i++)
//{
//	BlockErase_NAND(i*128*1024,0);//NAND_FLASH 块擦除;没块替换;入口:Busy_Wait=1NAND忙时等待Busy_Wait=0不等待
//}
////os->yaffs_unmount("user");
//os->yaffs_mount("user");
//---TEST END---
	
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//读引脚,入口引脚使用名;返回0或1
		{
			return;
		}
		#endif
		for(i=0;i<TEST_YAFFS_BYTE/4;i++)
		{
			p32s[i]=Data;
			Data++;
		}
		Data-=TEST_YAFFS_BYTE/4;
		yaffs=os->yaffs_open("user/TEST/TEST.txt",O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
		if(yaffs<0)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""yaffs_open_write");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_WRITE(yaffs,(void *)p32s,TEST_YAFFS_BYTE);
		os->yaffs_close(yaffs);
		MR((u32)p32d,ADDR_Download_ENTRY,TEST_YAFFS_BYTE);
		
//sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//os->yaffs_mount("user");
//sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
		
		yaffs=os->yaffs_open("user/TEST/TEST.txt",O_RDWR,0);
		if(yaffs<0)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""yaffs_open_read");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_fstat(yaffs, &stat);
		if(stat.st_size!=TEST_YAFFS_BYTE)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""stat.st_size!=1M");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			for(i=0;i<NAND_FILE_BLOCK_COUNT;i++)
			{
				BlockErase_NAND(i*128*1024,0);//NAND_FLASH 块擦除;没块替换;入口:Busy_Wait=1NAND忙时等待Busy_Wait=0不等待
			}
			continue;
		}
		LEN_File=os->yaffs_READ(yaffs,(void *)p32d,TEST_YAFFS_BYTE);
		if(LEN_File!=TEST_YAFFS_BYTE)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""LEN_File!=1M");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_close(yaffs);
		for(i=0;i<TEST_YAFFS_BYTE/4;i++)
		{
			if(p32d[i]!=Data)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""File Data");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;;
			}
			Data++;
		}
		if(i<TEST_YAFFS_BYTE/4)
		{
			continue;
		}
		
//os->yaffs_unmount("user");
//for(i=0;i<NAND_FILE_BLOCK_COUNT;i++)
//{
//	BlockErase_NAND(i*128*1024,0);//NAND_FLASH 块擦除;没块替换;入口:Busy_Wait=1NAND忙时等待Busy_Wait=0不等待
//}
//os->yaffs_mount("user");
//sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
		
		Count++;
		p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			p16[i]=0;
		}
		i=hex_bcd(Count);
		DisplayData_OffZero(i,8,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		LCDDataOut();//点阵数据输出到LCD屏
	}
}























