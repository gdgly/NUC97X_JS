
#include "../Hcsg/ProjectCSG.h"

#include "../Test/Test_RAM.h"


u32 RAM_Test(u32 ADDR_Start,u32 Byte)//RAM测试;返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	u32 *p32;
	
	p32=(u32*)ADDR_Start;
//写顺序数
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
//写0xff
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
//写0x00
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




