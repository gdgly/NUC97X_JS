

#ifndef MS_H
#define MS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "../OS/OS.h"
#include "../OS/My_Heap.h"
#include "../MS/msFILE.h"
#include "../Device/NAND.h"
#include "../Device/MEMRW.h"
	 
	 
	 
	 

typedef struct
{
//重启OS
void (*ReSTART_OS)(void);//重启OS(经实测直到MS在OS启动后的初始化完成需37ms)
//暂停os
void (*Disable_os)(void);//暂停os
//允许os
void (*Enable_os)(void);//允许os
	
//动态内存分配
void *(*ms_malloc) (size_t size);//动态分配我的内存堆
void *(*ms_calloc)(size_t size);//动态分配我的内存堆,并清0
void *(*ms_realloc)(void *pold,size_t newsize);//动态再分配我的内存堆,并copy原数据
void (*ms_free) (void * p);//释放我的内存堆分配
u32 (*ms_malloc_sizemax)(void);//最大可分配的尺寸(字节数)
u32 (*ms_realloc_sizemax)(void *pold);//最大可再分配的尺寸(字节数)
	
//msFILE
void (*msfile_init)(void);//msfile初始化
void (*msfile_format)(void);//msfile格式化化
u32 (*msfile_free)(void);//msfile返回文件自由空间
u32 (*msfile_write)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile写;当offset>=文件总长度时自动加在文件结尾;返回:写入字节数
u32 (*msfile_read)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile读;返回:读出字节数
u32 (*msfile_Delete)(u32 filename);//msfile删除;返回:0=失败,1=成功
u32 (*msfile_len)(u32 filename);//msfile得到文件总长度
u32 (*msfile_set_lenmin)(u32 filename,u32 lenmin);//msfile设置预留最小文件长度(原文件有效数据总长度及内容不变只预占用最小存储空间,;返回:0=失败,1=成功
u32 (*msfile_check)(u32 filename);//msfile文件检查;返回:0=空,1=有效文件
u32 (*msfile_rename)(u32 filename,u32 newfilename);//文件名重命名:返回:0=错误,1=成功

#if MainProtocol==698
void (*msfile_lenclear)(u32 filename);//清0文件相关的长度次数等
u32 (*msfile_getfilename)(u32 FileID);//搜索符合任务FileID的文件名:返回:0-(FILENAMEmax_FLASH+FILENAMEmax_RAM-1),0xffffffff表示没找
#endif

//MRW
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
void (*NAND4kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
#else
void (*NAND128kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif
void (*MR)(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//存储器读
void (*MW)(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//存储器写
void (*MC)(u32 Data,u32 ADDR_Dest,u32 Byte);//存储器清0
void (*ME)(u32 ADDR_Dest,u32 Byte);//FLASH块擦除
u64 (*MRR)(u32 ADDR_Sour,u32 Byte);//存储器读到寄存器(<=8字节)
void (*MWR)(u64 Data,u32 ADDR_Dest,u32 Byte);//寄存器写到存储器(<=8字节)

//LCD
void (*LCDDataOut)(void);//点阵数据输出到LCD屏


//ext add
void (*Init_my_heap)(void);//初始化与内核RTOS无关的我的内存堆
void (*Main_Thread)(void);//主线程

}const ms_Type;

ms_Type *Get_ms(void);//得到ms表地址





#ifdef __cplusplus
 }
#endif 
#endif

