

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
//����OS
void (*ReSTART_OS)(void);//����OS(��ʵ��ֱ��MS��OS������ĳ�ʼ�������37ms)
//��ͣos
void (*Disable_os)(void);//��ͣos
//����os
void (*Enable_os)(void);//����os
	
//��̬�ڴ����
void *(*ms_malloc) (size_t size);//��̬�����ҵ��ڴ��
void *(*ms_calloc)(size_t size);//��̬�����ҵ��ڴ��,����0
void *(*ms_realloc)(void *pold,size_t newsize);//��̬�ٷ����ҵ��ڴ��,��copyԭ����
void (*ms_free) (void * p);//�ͷ��ҵ��ڴ�ѷ���
u32 (*ms_malloc_sizemax)(void);//���ɷ���ĳߴ�(�ֽ���)
u32 (*ms_realloc_sizemax)(void *pold);//�����ٷ���ĳߴ�(�ֽ���)
	
//msFILE
void (*msfile_init)(void);//msfile��ʼ��
void (*msfile_format)(void);//msfile��ʽ����
u32 (*msfile_free)(void);//msfile�����ļ����ɿռ�
u32 (*msfile_write)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfileд;��offset>=�ļ��ܳ���ʱ�Զ������ļ���β;����:д���ֽ���
u32 (*msfile_read)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile��;����:�����ֽ���
u32 (*msfile_Delete)(u32 filename);//msfileɾ��;����:0=ʧ��,1=�ɹ�
u32 (*msfile_len)(u32 filename);//msfile�õ��ļ��ܳ���
u32 (*msfile_set_lenmin)(u32 filename,u32 lenmin);//msfile����Ԥ����С�ļ�����(ԭ�ļ���Ч�����ܳ��ȼ����ݲ���ֻԤռ����С�洢�ռ�,;����:0=ʧ��,1=�ɹ�
u32 (*msfile_check)(u32 filename);//msfile�ļ����;����:0=��,1=��Ч�ļ�
u32 (*msfile_rename)(u32 filename,u32 newfilename);//�ļ���������:����:0=����,1=�ɹ�

#if MainProtocol==698
void (*msfile_lenclear)(u32 filename);//��0�ļ���صĳ��ȴ�����
u32 (*msfile_getfilename)(u32 FileID);//������������FileID���ļ���:����:0-(FILENAMEmax_FLASH+FILENAMEmax_RAM-1),0xffffffff��ʾû��
#endif

//MRW
#if NAND128kBuff==0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
void (*NAND4kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
#else
void (*NAND128kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
#endif
void (*MR)(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//�洢����
void (*MW)(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//�洢��д
void (*MC)(u32 Data,u32 ADDR_Dest,u32 Byte);//�洢����0
void (*ME)(u32 ADDR_Dest,u32 Byte);//FLASH�����
u64 (*MRR)(u32 ADDR_Sour,u32 Byte);//�洢�������Ĵ���(<=8�ֽ�)
void (*MWR)(u64 Data,u32 ADDR_Dest,u32 Byte);//�Ĵ���д���洢��(<=8�ֽ�)

//LCD
void (*LCDDataOut)(void);//�������������LCD��


//ext add
void (*Init_my_heap)(void);//��ʼ�����ں�RTOS�޹ص��ҵ��ڴ��
void (*Main_Thread)(void);//���߳�

}const ms_Type;

ms_Type *Get_ms(void);//�õ�ms���ַ





#ifdef __cplusplus
 }
#endif 
#endif

