
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Driver/NUC97x_SPIFLASH.h"




u32 Test_SPIFLASH(void)//≤‚ ‘SPIFLASH
{
	u32 i;
	u32 x;
	u32* p32;
	u32 ReNum;
	
	ReNum=8;
	x=0;
	while(ReNum--)
	{
		p32=(u32*)(ADDR_128KWRITE_BUFF);
		for(i=0;i<(256*1024)/4;i++)
		{
			p32[i]=x;
			x++;
		}
		Write_SPIFLASH(ADDR_128KWRITE_BUFF,0x00300000,256*1024);
		Read_SPIFLASH(ADDR_128KWRITE_BUFF,0x00300000,256*1024);
		x-=(256*1024)/4;
		for(i=0;i<(256*1024)/4;i++)
		{
			if(p32[i]!=x)
			{
				return 1;
			}
			x++;
		}
	}
	return 0;
}





















