
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Test/Test_RAM.h"


u32 RAM_Test(u32 ADDR_Start,u32 Byte)//RAM²âÊÔ;·µ»Ø:0=ÕýÈ·,1=´íÎó
{
	u32 i;
	u32 x;
	u32 *p32;
	
	p32=(u32*)ADDR_Start;
//Ð´Ë³ÐòÊý
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
//Ð´0xff
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
//Ð´0x00
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




