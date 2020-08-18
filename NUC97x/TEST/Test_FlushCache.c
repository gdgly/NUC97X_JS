
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../Display/Display.h"
#include "../Device/NAND.h"
#include "../Device/MEMRW.h"


void Test_FlushCache_ReadPage(u32 ADDR_Dest,u32 ADDR_Sour)//NAND_FLASH 读页数据;返回:0=没错误,!=0错误
{
	u32 i;
	u32 x;
	u32 *p32NANDCTL;
	u32 *p32DMA;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32* p32s;
//	u32* p32d;
//	u32 *p32DATA;
	
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	
//先读页校验区(64byte)
	i=ADDR_Sour;
	*p32CMD=0x00;//1st Cycle Page Read Command
	*p32ADDR=0;//i;//Col.Add.1
	*p32ADDR=0x08;//(i>>8)&0x07;//Col.Add.2
	*p32ADDR=(i>>11);//Row Add. 1
#if (NAND_2G|NAND_4G|NAND_8G)
	*p32ADDR=(i>>19);//Row Add. 2
	*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
#else
	*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
#endif
	*p32CMD=0x30;//2nd Cycle Cycle Page Read Command
	Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
	p32NANDCTL=(u32*)(REG_NANDCTL);
	i=p32NANDCTL[0];
	i|=
	(0<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
	(1<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
	(0<<2)|//[2]DWR_EN DMA Write Data Enable
	(0<<1);//[1]DRD_EN DMA Read Data Enable
	p32NANDCTL[0]=i;
	while(p32NANDCTL[0]&(1<<3));
//读页数据
	i=ADDR_Sour;
	*p32CMD=0x00;//1st Cycle Page Read Command
	*p32ADDR=0;//i;//Col.Add.1
	*p32ADDR=0;//(i>>8)&0x07;//Col.Add.2
	*p32ADDR=(i>>11);//Row Add. 1
#if (NAND_2G|NAND_4G|NAND_8G)
	*p32ADDR=(i>>19);//Row Add. 2
	*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
#else
	*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
#endif
	*p32CMD=0x30;//2nd Cycle Cycle Page Read Command
	
	Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
	p32s=(u32*)(REG_NANDINTSTS);
	p32s[0]|=0x405;//清[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IF位
	
	p32DMA=(u32*)(REG_FMI_DMASA);
	p32DMA[0]=ADDR_Dest;
	p32NANDCTL=(u32*)(REG_NANDCTL);
	i=p32NANDCTL[0];
	i|=
	(0<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
	(0<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
	(0<<2)|//[2]DWR_EN DMA Write Data Enable
	(1<<1);//[1]DRD_EN DMA Read Data Enable
	p32NANDCTL[0]=i;
	while(1)
	{
		p32s=(u32*)(REG_NANDINTSTS);
		x=p32s[0];
		if(x&4)
		{//[2]ECC_FLD_IF,需清0否则DMA会停止不再传输
			p32s[0]=4;//清ECC_FLD_IF
		}
		if(x&1)
		{//[0]DMA_IF位
			break;
		}
	}
}


void Test_FlushCache(void)//Cache测试
{
	u32 i;
	u8* p8;
	u16* p16;
	u32* p32;
	u32 TestResult;
	
	TestResult=0;
	DisplayClr();//清屏
	DisplayStringAutoUp(0,9,(u8*)"\x0""FlushCache TEST");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol	
//0xDA
	MC(0xDA,ADDR_Download_ENTRY,2048);
//p8
	p8=(u8*)(ADDR_DATABUFF);
	for(i=0;i<2048;i++)
	{
		p8[i]=0x08;
	}
	Test_FlushCache_ReadPage(ADDR_DATABUFF,ADDR_Download_ENTRY-ADDR_EFLASH_Start);
	for(i=0;i<2048;i++)
	{
		if(p8[i]!=0xDA)
		{
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	for(i=0;i<2048;i++)
	{
		if(p8[i]!=0xDA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p8_DATA  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol	
			break;
		}
	}
//p16
	p16=(u16*)(ADDR_DATABUFF);
	for(i=0;i<1024;i++)
	{
		p16[i]=0x16;
	}
	Test_FlushCache_ReadPage(ADDR_DATABUFF,ADDR_Download_ENTRY-ADDR_EFLASH_Start);
	for(i=0;i<1024;i++)
	{
		if(p16[i]!=0xDADA)
		{
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	for(i=0;i<1024;i++)
	{
		if(p16[i]!=0xDADA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p16_DATA  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			break;
		}
	}
//p32
	p32=(u32*)(ADDR_DATABUFF);
	for(i=0;i<512;i++)
	{
		p32[i]=0x32;
	}
	Test_FlushCache_ReadPage(ADDR_DATABUFF,ADDR_Download_ENTRY-ADDR_EFLASH_Start);
	for(i=0;i<512;i++)
	{
		if(p32[i]!=0xDADADADA)
		{
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	for(i=0;i<512;i++)
	{
		if(p32[i]!=0xDADADADA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p32_DATA  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			break;
		}
	}
//p8,p16,p32
	p8=(u8*)(ADDR_DATABUFF);
	p16=(u16*)(ADDR_DATABUFF);
	p32=(u32*)(ADDR_DATABUFF);
	for(i=0;i<512;i++)
	{
		p32[i]=0x32;
	}
	for(i=0;i<1024;i++)
	{
		p16[i]=0x16;
	}
	for(i=0;i<2048;i++)
	{
		p8[i]=0x08;
	}
	Test_FlushCache_ReadPage(ADDR_DATABUFF,ADDR_Download_ENTRY-ADDR_EFLASH_Start);
	for(i=0;i<2048;i++)
	{
		if(p8[i]!=0xDA)
		{
			break;
		}
	}
	for(i=0;i<1024;i++)
	{
		if(p16[i]!=0xDADA)
		{
			break;
		}
	}
	for(i=0;i<512;i++)
	{
		if(p32[i]!=0xDADADADA)
		{
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	for(i=0;i<2048;i++)
	{
		if(p8[i]!=0xDA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p8_DATA2  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			break;
		}
	}
	for(i=0;i<1024;i++)
	{
		if(p16[i]!=0xDADA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p16_DATA2  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			break;
		}
	}
	for(i=0;i<512;i++)
	{
		if(p32[i]!=0xDADADADA)
		{
			TestResult=1;
			DisplayStringAutoUp(0,9,(u8*)"\x0""p32_DATA2  ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			break;
		}
	}
	if(TestResult==0)
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
	
}























