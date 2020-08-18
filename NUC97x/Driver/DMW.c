

#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/Device/DMW.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/Device/NAND.h"
#include "../OS/My_Heap.h"


void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//直接存储器写(无擦除)
{
	u32 i;
	u8* p8;
	if(Byte==0)
	{
		return;
	}
	p8=(u8*)ms_malloc(Byte);//动态分配我的内存堆
	if(p8==0)
	{
		return;
	}
	ms_free(p8);//释放我的内存堆分配
	MR((u32)p8,ADDR_Dest,Byte);
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xff)
		{
			return;//原数据不是擦除值0xff
		}
	}
	MW(ADDR_Sour,ADDR_Dest,Byte);//存储器写
}
void DMC(u8 Data,u32 ADDR_Dest,u32 Byte)//直接存储器清除(无擦除)
{
	u32 i;
	u8* p8;
	if(Byte==0)
	{
		return;
	}
	p8=(u8*)ms_malloc(Byte);//动态分配我的内存堆
	if(p8==0)
	{
		return;
	}
	ms_free(p8);//释放我的内存堆分配
	MR((u32)p8,ADDR_Dest,Byte);
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xff)
		{
			return;//原数据不是擦除值0xff
		}
	}
	MC(Data,ADDR_Dest,Byte);//存储器清0
}
void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte)//寄存器直接写到存储器(<=8字节)(无擦除)
{
  u64 * p64;

	if((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))
	{
		p64=(u64 *)(ADDR_MRWBUFF);
		p64[0]=Data;
		DMW(ADDR_MRWBUFF,ADDR_Dest,Byte);//直接存储器写(无擦除)
	}
}




