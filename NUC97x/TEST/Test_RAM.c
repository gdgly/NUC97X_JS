
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/TEST/Test_RAM.h"
#include "../../NUC97x/Nuc97x/NUC97x_AIC.h"


u32 RAM_Test(u32 ADDR_Start,u32 Byte)//RAM����;����:0=��ȷ,1=����
{
	u32 i;
	u32 x;
	u32 *p32;
	
	p32=(u32*)ADDR_Start;
//д˳����
	for(i=0;i<(Byte/4);i++)
	{
		p32[i]=i;
	}
	for(i=0;i<(Byte/4);i++)
	{
		x=p32[i];
		if(x!=i)
		{
			return 1;
		}
	}
//д0xff
	for(i=0;i<(Byte/4);i++)
	{
		p32[i]=0xffffffff;
	}
	for(i=0;i<(Byte/4);i++)
	{
		if(p32[i]!=0xffffffff)
		{
			return 1;
		}
	}
//д0x00
	for(i=0;i<(Byte/4);i++)
	{
		p32[i]=0;
	}
	for(i=0;i<(Byte/4);i++)
	{
		if(p32[i]!=0)
		{
			return 1;
		}
	}
	return 0;
}

u32 Test_SDRAM(u32 ADDR_BASE)//����DDR2;����:0=��ȷ,1=����
{
	u32 i;
	u32 Len;
	u32* p32s;
	u32* p32d;
	u32 TestResult;
	
	__disable_irq();//���ж�
#if ((ADDR_TerminalDisplay_IRAM_End)-(ADDR_SystemStack))>(56*1024)
	#error
#endif
//����ԭ��ʾ����
	p32s=(u32*)(ADDR_SystemStack);
	p32d=(u32*)0x3c000000;
	Len=((ADDR_TerminalDisplay_IRAM_End)-(ADDR_SystemStack))/4;
	for(i=0;i<Len;i++)
	{
		p32d[i]=p32s[i];
	}
#ifndef LINUX
	TestResult=RAM_Test(ADDR_ERAM_START,0x02000000-ADDR_ERAM_START);//RAM����;����:0=��ȷ,1=����
#else
	TestResult=RAM_Test(ADDR_ERAM_START,0x04000000-ADDR_ERAM_START);//RAM����;����:0=��ȷ,1=����
#endif
	
//�ָ�ԭ��ʾ����
	p32d=(u32*)(ADDR_SystemStack);
	p32s=(u32*)0x3c000000;
	for(i=0;i<Len;i++)
	{
		p32d[i]=p32s[i];
	}
	__enable_irq();//�����ж�
	return TestResult;
}















