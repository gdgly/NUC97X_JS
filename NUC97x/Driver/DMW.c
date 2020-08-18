

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


void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢��д(�޲���)
{
	u32 i;
	u8* p8;
	if(Byte==0)
	{
		return;
	}
	p8=(u8*)ms_malloc(Byte);//��̬�����ҵ��ڴ��
	if(p8==0)
	{
		return;
	}
	ms_free(p8);//�ͷ��ҵ��ڴ�ѷ���
	MR((u32)p8,ADDR_Dest,Byte);
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xff)
		{
			return;//ԭ���ݲ��ǲ���ֵ0xff
		}
	}
	MW(ADDR_Sour,ADDR_Dest,Byte);//�洢��д
}
void DMC(u8 Data,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢�����(�޲���)
{
	u32 i;
	u8* p8;
	if(Byte==0)
	{
		return;
	}
	p8=(u8*)ms_malloc(Byte);//��̬�����ҵ��ڴ��
	if(p8==0)
	{
		return;
	}
	ms_free(p8);//�ͷ��ҵ��ڴ�ѷ���
	MR((u32)p8,ADDR_Dest,Byte);
	for(i=0;i<Byte;i++)
	{
		if(p8[i]!=0xff)
		{
			return;//ԭ���ݲ��ǲ���ֵ0xff
		}
	}
	MC(Data,ADDR_Dest,Byte);//�洢����0
}
void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte)//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)
{
  u64 * p64;

	if((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))
	{
		p64=(u64 *)(ADDR_MRWBUFF);
		p64[0]=Data;
		DMW(ADDR_MRWBUFF,ADDR_Dest,Byte);//ֱ�Ӵ洢��д(�޲���)
	}
}




