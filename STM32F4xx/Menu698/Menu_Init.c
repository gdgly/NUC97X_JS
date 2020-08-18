

#include "Project698.h"
#include "../Menu/Menu_Init.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"


void Init_Menu(void)//启动初始化
{
	
	
}


void Init_Menu_SoftwareUpdate(void)//软件更新初始化
{
	u32 i;
	u32 x;
	u32 Block;
	u32 Byte;
	u32 LENmax;
	u16 *p16;
	u32 *p32;
	u32 Err;
	
	//PACK包下载时,Menu总是第一个更新
	//生产程序和出厂程序可能不同，软件更新时nand128kbuff和nand4kbuff可能变换,为避免出厂程参数初始化错误,在此先检查写buff
//4kbuff检查
	Err=0;
	p16=(u16*)(ADDR_BKPSRAM);
	if((p16[1]>4096)||(p16[1]<4))
	{//错,总字节数(全部字节,也即下块的开始地址)
		Block=0;
		Err=1;
	}
	Block=p16[0];
	Byte=4;
	LENmax=4096-4-8;
	for(i=0;i<Block;i++)
	{
		if(Byte>4096)
		{
			Err=1;
			break;
		}
		p32=(u32*)(ADDR_BKPSRAM+Byte);
		p16=(u16*)(ADDR_BKPSRAM+Byte+4);
		if((p32[0]<0x30000000)||(p32[0]>0xA0000000)||(p16[1]>p16[0]))
		{//错
			Err=1;
			break;
		}
		if(p16[0]>LENmax)
		{
			Err=1;
			break;
		}
		Byte+=8+p16[0];
		LENmax-=8+p16[0];
	}
	p16=(u16*)(ADDR_BKPSRAM);
	if(Byte!=p16[1])
	{
		Err=1;
	}
	else
	{
		if(Byte<=(4096-4))
		{
			i=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM中数据CRC32计算
			p32=(u32*)(ADDR_BKPSRAM+(4096-4));
			if(p32[0]!=i)
			{
				Err=1;
			}
		}
	}
	if(Err)
	{
		p32=(u32*)(ADDR_BKPSRAM);
		p32[0]=0x40000;//初始为总块数=0,总字节数=4
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM中数据CRC32计算
		Err=0;
	}
	else
	{
		Err=1;
	}
//128buff检查
	Err=0;
	p16=(u16*)(ADDR_BKPSDRAM);
	p32=(u32*)(ADDR_BKPSDRAM+2);
	LENmax=(128*1024)-6-12;
	if((p32[0]>(128*1024))||(p32[0]<6))
	{//错,总字节数(全部字节,也即下块的开始地址)
		Err=1;
	}
	Block=p16[0];
	if(Block>(((128*1024)-6)/12))
	{
		Block=0;
		Err=1;
	}
	Byte=6;
	for(i=0;i<Block;i++)
	{
		if(Byte>(128*1024))
		{
			Err=1;
			break;
		}
		p32=(u32*)(ADDR_BKPSDRAM+Byte);
		if((p32[0]<0x30000000)||(p32[0]>0xA0000000))
		{//错
			Err=1;
			break;
		}
		x=p32[1];
		if(p32[2]>x)
		{
			Err=1;
			break;
		}
		if(p32[1]>LENmax)
		{
			Err=1;
			break;
		}
		Byte+=12+p32[1];
		LENmax-=12+p32[1];
	}
	p32=(u32*)(ADDR_BKPSDRAM+2);
	if(Byte!=p32[0])
	{
		Err=1;
	}
	else
	{
		if(Byte<=((128*1024)-4))
		{
			if(Byte>(4096-4))
			{
				Byte=(4096-4);
			}
			i=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM中数据CRC32计算
			p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
			if(p32[0]!=i)
			{
				Err=1;
			}
		}
	}
	if(Err)
	{
		p32=(u32*)(ADDR_BKPSDRAM);
		p32[0]=0x60000;
		p32[1]=0;
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM中数据CRC32计算
		Err=0;
	}
	else
	{
		Err=1;
	}
	
}



