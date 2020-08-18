/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 15/05/28 5:19p $
 * @brief    Access a NAND flash formatted in YAFFS2 file system
 *
 * @note
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#if MainProtocol==376
#include "../../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include <string.h>

#include "nuc970.h"
#include "sys.h"
#include "yaffs_glue.h"

extern void nand_init(void);
unsigned char nand0InitializeFlag=0;//nand0初始化标志:0=没,1=初始化
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

void SYS_Init(void)
{
    /* enable FMI NAND */
    outpw(REG_CLK_HCLKEN, (inpw(REG_CLK_HCLKEN) | 0x300000));

    /* select NAND function pins */
    if (inpw(REG_SYS_PWRON) & 0x08000000)
    {
        /* Set GPI1~15 for NAND */
        outpw(REG_SYS_GPI_MFPL, 0x55555550);
        outpw(REG_SYS_GPI_MFPH, 0x55555555);
    }
    else
    {
        /* Set GPC0~14 for NAND */
        outpw(REG_SYS_GPC_MFPL, 0x55555555);
        outpw(REG_SYS_GPC_MFPH, 0x05555555);
    }
}

static char CommandLine[256];

/*----------------------------------------------*/
/* Get a line from the input                    */
/*----------------------------------------------*/
void get_line (char *buff, int len);
/*
void get_line (char *buff, int len)
{
    char c;
    int idx = 0;

    for (;;) {
        c = sysGetChar();
        sysPutChar(c);
        if (c == '\r') break;
        if ((c == '\b') && idx) idx--;
        if ((c >= ' ') && (idx < len - 1)) buff[idx++] = c;
    }
    buff[idx] = 0;

    sysPutChar('\n');
}
*/

int yaffs_unlinkdir(const char *path);//删除目录下的全部文件
int yaffs_main(void)
{
    char *ptr;
    char mtpoint[] = "user";
    char buf[8];
    int volatile i;

	sysInitializeUART();
    sysprintf("\n");
    sysprintf("==========================================\n");
    sysprintf("          FMI NAND YAFFS2                 \n");
    sysprintf("==========================================\n");

	sysDisableCache();
	sysInvalidCache();
	sysSetMMUMappingMethod(MMU_DIRECT_MAPPING);
	sysEnableCache(CACHE_WRITE_BACK);

	if(nand0InitializeFlag==0)//nand0初始化标志:0=没,1=初始化
	{//发现2次初始化和mount，128M变为256M等..错误！
    SYS_Init();
    nand_init();
    //cmd_yaffs_devconfig(mtpoint, 0, 0xb0, 0x3ff);
		cmd_yaffs_devconfig(mtpoint, 0, 0, NAND_FILE_BLOCK_COUNT-1);//用于yaffs_的块配置;开始块,结束块
    cmd_yaffs_dev_ls();
		cmd_yaffs_mount(mtpoint);
    cmd_yaffs_dev_ls();
	}
   sysprintf("\n");

    for (;;) {

        sysprintf(">");
        ptr = CommandLine;
        get_line(ptr, sizeof(CommandLine));
        switch (*ptr++) {

        case 'q' :  /* Exit program */
          	cmd_yaffs_umount(mtpoint);
            sysprintf("Program terminated!\n");
            return 0;

        case 'l' :  /* ls */
            if (*ptr++ == 's') {
                while (*ptr == ' ') ptr++;
                cmd_yaffs_ls(ptr, 1);
            }
            break;

        case 'w' :  /* wr */
            if (*ptr++ == 'r') {
                while (*ptr == ' ') ptr++;
                cmd_yaffs_write_file(ptr, 0x55, 10);    /* write 0x55 into file 10 times */
            }
            break;

        case 'r' :
            if (*ptr == 'd') {  /* rd */
                ptr++;
                while (*ptr == ' ') ptr++;
                sysprintf("Reading file %s ...\n\n", ptr);
                cmd_yaffs_read_file(ptr);
                sysprintf("\ndone.\n");
            }
            else if (*ptr == 'm') {  /* rm */
                ptr++;
                if (*ptr == 'd')
                {
                    i = 0;
                    while(*ptr != ' ')
                        buf[i++] = *ptr++;
                    ptr++;
                    sysprintf("Remove dir %s ...\n\n", ptr);
                    cmd_yaffs_rmdir(ptr);
                }
                else
                {
                    while (*ptr == ' ') ptr++;
                    sysprintf("Remove file %s ...\n\n", ptr);
                    cmd_yaffs_rm(ptr);
                }
            }
            break;

        case 'm' :  /* mkdir */
            i = 0;
            while(*ptr != ' ')
						{
               buf[i++] = *ptr++;
						}
						buf[i]=0;
            ptr++;

            if (!strcmp(buf, "kdir"))
						{
               cmd_yaffs_mkdir(ptr);
						}
            break;
				case 'd':
					yaffs_unlinkdir("user/TEST/");//删除目录下的全部文件
					sysprintf("Delete user/TEST/ directory all files\n");
					break;
        case '?':       /* Show usage */
            sysprintf("ls    <path>     - Show a directory. ex: ls user/test ('user' is mount point).\n");
            sysprintf("rd    <file name> - Read a file. ex: rd user/test.bin ('user' is mount point).\n");
            sysprintf("wr    <file name> - Write a file. ex: wr user/test.bin ('user' is mount point).\n");
            sysprintf("rm    <file name> - Delete a file. ex: rm user/test.bin ('user' is mount point).\n");
            sysprintf("mkdir <dir name> - Create a directory. ex: mkdir user/test ('user' is mount point).\n");
            sysprintf("rmdir <dir name> - Create a directory. ex: mkdir user/test ('user' is mount point).\n");
						sysprintf("d <dir name> - Delete user/TEST/ directory all files. ex:d.\n");
            sysprintf("\n");
        }
    }
}


#include "../../OS/API/yaffsAPI.h"
#include "../../NUC97x/NUC97x_AIC.h"
void SoftDelayMS(u32 ms);//软件ms延时(TMR4)

void yaffs_Init(void)
{
	char mtpoint[] = "user";
	u32 irq;
	u32 i;
	u32* p32;
	
		// enable FMI NAND
	outpw(REG_CLK_HCLKEN, (inpw(REG_CLK_HCLKEN) | 0x300000));
	//select NAND function pins
	irq=off_irq();//关IRQ中断,返回:关闭前允许状态
	if (inpw(REG_SYS_PWRON) & 0x08000000)
	{
			// Set GPI1~15 for NAND
			outpw(REG_SYS_GPI_MFPL, 0x55555550);
			outpw(REG_SYS_GPI_MFPH, 0x55555555);
	}
	else
	{
			// Set GPC0~14 for NAND
			outpw(REG_SYS_GPC_MFPL, 0x55555555);
			outpw(REG_SYS_GPC_MFPH, 0x05555555);
	}
	destore_irq(irq);//恢复IRQ中断,入口:关闭前允许状态

	nand_init();
		
	cmd_yaffs_devconfig(mtpoint, 0, 0, NAND_FILE_BLOCK_COUNT-1);//用于yaffs_的块配置;开始块,结束块

	nand0InitializeFlag=1;//nand0初始化标志:0=没,1=初始化
//  cmd_yaffs_dev_ls();
//	cmd_yaffs_mount(mtpoint);//挂载
//  cmd_yaffs_dev_ls();
}

void yaffs_tset(void)
{
	u32* p32;
	u32 i;
	
//---TEST---
	
	char mtpoint[] = "user";
	
	yaffs_Init();
	yaffs_mount(mtpoint);//挂载
	i=yaffs_totalspace(mtpoint);
	if(i)
	{
		i=i;
	}
	
	p32=(u32*)(REG_NANDCTL);
	i=p32[0];
	if(i)
	{
		i=i;
	}
	p32=(u32*)(REG_NANDRACTL);
	i=p32[0];
	if(i)
	{
		i=i;
	}
	p32=(u32*)(REG_FMI_DMAINTEN);
	i=p32[0];
	if(i)
	{
		i=i;
	}
	p32=(u32*)(REG_NANDECTL);
	i=p32[0];
	if(i)
	{
		i=i;
	}
	p32=(u32*)(REG_NANDINTEN);
	i=p32[0];
	if(i)
	{
		i=i;
	}
	
//---TEST END---
	while (1)
	{
		
		p32=(unsigned long*)(0x4000);
		(*(void(*)())p32[115])();
//		taskYIELD();//强制任务切换
		
	}
	
}
