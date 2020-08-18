

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"



#ifndef LINUX
void MR(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//�洢����
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[130])((u32) ADDR_Dest,(u32) ADDR_Sour,(u32) Byte);
}
void MW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//�洢��д
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[131])((u32) ADDR_Sour,(u32) ADDR_Dest,(u32) Byte);
}
void MC(u32 Data,u32 ADDR_Dest,u32 Byte)//�洢����0
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[132])((u32) Data,(u32) ADDR_Dest,(u32) Byte);
}
void ME(u32 ADDR_Dest,u32 Byte)//FLASH�����
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[133])((u32) ADDR_Dest,(u32) Byte);
}
u64 MRR(u32 ADDR_Sour,u32 Byte)//�洢�������Ĵ���(<=8�ֽ�)
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	return (*(u64(*)())p32[134])((u32) ADDR_Sour,(u32) Byte);
}
void MWR(u64 Data,u32 ADDR_Dest,u32 Byte)//�Ĵ���д���洢��(<=8�ֽ�)
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[135])((u64) Data,(u32) ADDR_Dest,(u32) Byte);
}

void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢��д(�޲���)
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[136])((u32) ADDR_Sour,(u32) ADDR_Dest,(u32) Byte);
}
void DMC(u8 Data,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢�����(�޲���)
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[137])((u32) Data,(u32) ADDR_Dest,(u32) Byte);
}
void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte)//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[138])((u64) Data,(u32) ADDR_Dest,(u32) Byte);
}

void LCDDataOut(void)//�������������LCD��
{
	u32 *p32;
	p32=(u32*)ADDR_MS_ENTRY;
	(*(void(*)())p32[140])();
}
#else//#ifndef LINUX
#include "../MS/MS.h"
void MR(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//�洢����
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->MR(ADDR_Dest,ADDR_Sour,Byte);
}
void MW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//�洢��д
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->MW(ADDR_Sour,ADDR_Dest,Byte);
}
void MC(u32 Data,u32 ADDR_Dest,u32 Byte)//�洢����0
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->MC(Data,ADDR_Dest,Byte);
}
void ME(u32 ADDR_Dest,u32 Byte)//FLASH�����
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->ME(ADDR_Dest,Byte);
}
u64 MRR(u32 ADDR_Sour,u32 Byte)//�洢�������Ĵ���(<=8�ֽ�)
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	return ms->MRR(ADDR_Sour,Byte);
}
void MWR(u64 Data,u32 ADDR_Dest,u32 Byte)//�Ĵ���д���洢��(<=8�ֽ�)
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->MWR(Data,ADDR_Dest,Byte);
}

void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢��д(�޲���)
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->DMW(ADDR_Sour,ADDR_Dest,Byte);
}

void DMC(u8 Data,u32 ADDR_Dest,u32 Byte)//ֱ�Ӵ洢�����(�޲���)
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->DMC(Data,ADDR_Dest,Byte);
}

void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte)//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->DMWR(Data,ADDR_Dest,Byte);
}

void LCDDataOut(void)//�������������LCD��
{
	u32 *p32;
	ms_Type *ms;
	p32=(u32*)ADDR_ms_entry;
	ms=(ms_Type*)p32[0];
	ms->LCDDataOut();
}
#endif

