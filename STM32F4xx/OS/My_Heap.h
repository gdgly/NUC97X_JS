

#ifndef My_Heap_H
#define My_Heap_H

#include "stdlib.h"//为定义size_t包含

#ifdef __cplusplus
 extern "C" {
#endif
	 
void Init_my_heap(void);//初始化与内核RTOS无关的我的内存堆
void *ms_malloc(size_t size);//动态分配我的内存堆
void *ms_calloc(size_t size);//动态分配我的内存堆,并清0
void *ms_realloc(void *pold,size_t newsize);//动态再分配我的内存堆,copy全部原数据
void ms_free(void * p);//释放我的内存堆分配
u32 ms_malloc_sizemax(void);//最大可分配的尺寸(字节数)
u32 ms_realloc_sizemax(void *pold);//最大可再分配的尺寸(字节数)
	 
#ifdef __cplusplus
 }
#endif 
#endif
