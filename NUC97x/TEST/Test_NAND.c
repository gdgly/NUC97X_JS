
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../NUC97x/TEST/Test_NAND.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Device/NAND.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/Device/MEMRW.h"



#define Test_NAND_ECC_BYTE     (4*1024)

u32 Test_NAND_ReadDataAndParity(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//NAND测试没替换;有ECC校正读数据区和校验区;返回:0=没错误,!=0错误
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 PageByte;
	u32 *p32NANDCTL;
	u32 *p32DMA;
	u32 *p32CMD;
	u32 *p32ADDR;
//	u32 *p32DATA;
	u8* p8SOUR;
	u8 *p8DEST;
	u32* p32s;
	u32* p32d;
	u32 Page;
	u32 ErrStatus;
	u32 ErrBitNUM;
	u32* p32ErrAddr;
	u32* p32ErrData;
	u32 ErrData;
	u32 ErrRead;
	
	
	Init_NAND();//初始化nand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	
	Page=0;
	p8DEST=(u8*)ADDR_Dest;
	while(Byte!=0)
	{
		//先读64byte校验区
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
			sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
			
			p32s=(u32*)(REG_NANDRA0);
			p32d=(u32*)(ADDR_ECC_BUFF+(64*Page));
			for(i=0;i<16;i++)
			{
				p32d[i]=p32s[i];
			}
			
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
			PageByte=2048-(ADDR_Sour&0x7FF);//当前Page能读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			//while(!p32s[0]&0x400);
			p32s=(u32*)(REG_NANDINTSTS);
			p32s[0]|=0x405;//清[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IF位
			p32DMA=(u32*)(REG_FMI_DMASA);
			p32DMA[0]=ADDR_NAND_PAGEREAD_BUFF;
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
					sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
					p32d=(u32*)(REG_NANDECCES0);
					for(n=0;n<4;n++)
					{
						ErrStatus=p32d[n];
						for(m=0;m<4;m++)
						{
							if(ErrStatus&3)
							{
								if((ErrStatus&3)!=1)
								{//不能修正
									ErrRead=1;
									break;
								}
								else
								{//能修正
									ErrBitNUM=(ErrStatus>>2)&0x1f;//错误位数
									p32ErrAddr=(u32*)(REG_NANDECCEA0);//0-11
									p32ErrData=(u32*)(REG_NANDECCED0);//0-5
									while(ErrBitNUM)
									{
										if(ErrBitNUM)
										{
											ErrBitNUM--;
											p8SOUR=(u8*)(ADDR_NAND_PAGEREAD_BUFF+(((n*4)+m)*512)+(p32ErrAddr[0]&0x3ff));
											ErrData=p32ErrData[0]&0xff;
											p8SOUR[0]^=ErrData;
										}
										if(ErrBitNUM)
										{
											ErrBitNUM--;
											p8SOUR=(u8*)(ADDR_NAND_PAGEREAD_BUFF+(((n*4)+m)*512)+((p32ErrAddr[0]>>16)&0x3ff));
											ErrData=(p32ErrData[0]>>8)&0xff;
											p8SOUR[0]^=ErrData;
											p32ErrAddr++;
										}
										if(ErrBitNUM)
										{
											ErrBitNUM--;
											p8SOUR=(u8*)(ADDR_NAND_PAGEREAD_BUFF+(((n*4)+m)*512)+(p32ErrAddr[0]&0x3ff));
											ErrData=(p32ErrData[0]>>16)&0xff;
											p8SOUR[0]^=ErrData;
										}
										if(ErrBitNUM)
										{
											ErrBitNUM--;
											p8SOUR=(u8*)(ADDR_NAND_PAGEREAD_BUFF+(((n*4)+m)*512)+((p32ErrAddr[0]>>16)&0x3ff));
											ErrData=(p32ErrData[0]>>24)&0xff;
											p8SOUR[0]^=ErrData;
											p32ErrAddr++;
											p32ErrData++;
										}
									}
								}
							}
							ErrStatus>>=8;
						}
					}
					p32s=(u32*)(REG_NANDINTSTS);
					p32s[0]=4;//清ECC_FLD_IF
				}
				if(x&1)
				{//[0]DMA_IF位
					break;
				}
			}
		//取数据
			p8SOUR=(u8*)ADDR_NAND_PAGEREAD_BUFF+(ADDR_Sour&0x7ff);
			for(i=0;i<PageByte;i++)
			{
				x=p8SOUR[i];
				*p8DEST=x;
				p8DEST++;
			}
			p8DEST-=PageByte;

		ADDR_Sour+=PageByte;
		p8DEST+=PageByte;
		Byte-=PageByte;
		Page++;
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	return ErrRead;//返回:0=没错误,!=0错误
}

void Test_NAND_BLOCKWriteDataAndParity(u32 ADDR_Dest)//NAND测试没替换;块写ADDR_128KWRITE_BUFF中数据和ADDR_ECC_BUFF到FLASH块
{
	u32 i;
	u32 x;
	u32 Page;
	u8 *p8WBUFF;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32 *p32DATA;

	Init_NAND();//复位nand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
	p32DATA=(u32*)(REG_NANDDATA);
	ADDR_Dest&=0xfffe0000;
//块写
	
	//块擦除
	BlockErase_NAND(ADDR_Dest,0);//NAND_FLASH 块擦除;没块替换
	//块写回(每分页2048字节)
	for(Page=0;Page<64;Page++)
	{
		i=ADDR_Dest+(Page*2048);
		*p32CMD=0x80;//Command
		*p32ADDR=i;//Col.Add.1
		*p32ADDR=(i>>8)&0x7;//Col.Add.2
		*p32ADDR=(i>>11);//Row Add. 1
	#if (NAND_2G|NAND_4G|NAND_8G)
		*p32ADDR=(i>>19);//Row Add. 2
		*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
	#else
		*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
	#endif
		p8WBUFF=(u8*)(ADDR_128KWRITE_BUFF+(2048*Page));
		for(i=0;i<2048;i++)
		{
			x=*p8WBUFF;
			*p32DATA=x;
			p8WBUFF++;
		}
		//写校验区(64byte)
		p8WBUFF=(u8*)(ADDR_ECC_BUFF+(64*Page));
		for(i=0;i<64;i++)
		{
			x=*p8WBUFF;
			*p32DATA=x;
			p8WBUFF++;
		}
		*p32CMD=0x10;//Command
		Wait_NAND(600*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
	}
}


u32 Test_NAND_ECC(u32 ADDR_BASE)//测试NAND_ECC;入口:要测试的块地址;返回:0=正确,1=错误
{
	u32 i;
	u8* p8;
	u32* p32;
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND_ECC");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	p32=(u32*)ADDR_128KWRITE_BUFF;
	for(i=0;i<Test_NAND_ECC_BYTE/4;i++)
	{
		p32[i]=i;
	}
	BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误;没块替换
	Test_NAND_ReadDataAndParity(ADDR_128KWRITE_BUFF,(NAND_BLOCK_COUNT-1)*128*1024,Test_NAND_ECC_BYTE);//NAND测试没替换;有ECC校正读数据区和校验区;返回:0=没错误,!=0错误
//放错误码最多4*64=256个512区
	//1位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*0));
	p8[0]=1;
	//2位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*1));
	p8[0]=3;
	//3位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*2));
	p8[0]=7;
	//4位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*3));
	p8[0]=0x0f;
	//5位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*4));
	p8[0]=0x1f;
	//6位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*5));
	p8[0]=0x3f;
	//7位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*6));
	p8[0]=0x7f;
	//8位错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*7));
	p8[0]=0xff;
	
	//2字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*8));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	//3字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*9));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	//4字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*10));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	if(p8[3]&1)
	{
		p8[3]&=0xfe;
	}
	else
	{
		p8[3]|=1;
	}
	//5字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*11));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	if(p8[3]&1)
	{
		p8[3]&=0xfe;
	}
	else
	{
		p8[3]|=1;
	}
	if(p8[4]&1)
	{
		p8[4]&=0xfe;
	}
	else
	{
		p8[4]|=1;
	}
	//6字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*12));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	if(p8[3]&1)
	{
		p8[3]&=0xfe;
	}
	else
	{
		p8[3]|=1;
	}
	if(p8[4]&1)
	{
		p8[4]&=0xfe;
	}
	else
	{
		p8[4]|=1;
	}
	if(p8[5]&1)
	{
		p8[5]&=0xfe;
	}
	else
	{
		p8[5]|=1;
	}
	//7字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*13));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	if(p8[3]&1)
	{
		p8[3]&=0xfe;
	}
	else
	{
		p8[3]|=1;
	}
	if(p8[4]&1)
	{
		p8[4]&=0xfe;
	}
	else
	{
		p8[4]|=1;
	}
	if(p8[5]&1)
	{
		p8[5]&=0xfe;
	}
	else
	{
		p8[5]|=1;
	}
	if(p8[6]&1)
	{
		p8[6]&=0xfe;
	}
	else
	{
		p8[6]|=1;
	}
	//8字节错
	p8=(u8*)(ADDR_128KWRITE_BUFF+(512*14));
	if(p8[0]&1)
	{
		p8[0]&=0xfe;
	}
	else
	{
		p8[0]|=1;
	}
	if(p8[1]&1)
	{
		p8[1]&=0xfe;
	}
	else
	{
		p8[1]|=1;
	}
	if(p8[2]&1)
	{
		p8[2]&=0xfe;
	}
	else
	{
		p8[2]|=1;
	}
	if(p8[3]&1)
	{
		p8[3]&=0xfe;
	}
	else
	{
		p8[3]|=1;
	}
	if(p8[4]&1)
	{
		p8[4]&=0xfe;
	}
	else
	{
		p8[4]|=1;
	}
	if(p8[5]&1)
	{
		p8[5]&=0xfe;
	}
	else
	{
		p8[5]|=1;
	}
	if(p8[6]&1)
	{
		p8[6]&=0xfe;
	}
	else
	{
		p8[6]|=1;
	}
	if(p8[7]&1)
	{
		p8[7]&=0xfe;
	}
	else
	{
		p8[7]|=1;
	}
	
	Test_NAND_BLOCKWriteDataAndParity((NAND_BLOCK_COUNT-1)*128*1024);//NAND测试没替换;块写ADDR_128KWRITE_BUFF中数据和ADDR_ECC_BUFF到FLASH块
	Read_NAND(ADDR_128KDATABUFF,(NAND_BLOCK_COUNT-1)*128*1024,Test_NAND_ECC_BYTE);//NAND_FLASH 没替换读;返回:0=没错误,!=0错误
	p32=(u32*)ADDR_128KDATABUFF;
	for(i=0;i<Test_NAND_ECC_BYTE/4;i++)
	{
		if(p32[i]!=i)
		{
			DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
			MC(1,ADDR_BASE+OFFSET_NAND_Characteristics,1);
			return 1;
		}
	}
	DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	MC(0,ADDR_BASE+OFFSET_NAND_Characteristics,1);
	return 0;
}





#if MainProtocol==698
u32 Test_NAND_RW(void)//测试nand读写缓冲;返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	u32 Count;
	u8* p8;
	
//写
	Count=0;
	while(Count<128)
	{
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<256;i++)
		{
			p8[i]=i;
		}
		MW(ADDR_128KDATABUFF,ADDR_3000+Count,256);
		MR(ADDR_128KDATABUFF,ADDR_3000,256);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<Count;i++)
		{
			if(p8[i]!=0)
			{
				return 1;
			}
		}
		p8=(u8*)ADDR_128KDATABUFF;
		x=0;
		for(i=Count;i<256;i++)
		{
			if(p8[i]!=(x&0xff))
			{
				return 1;
			}
			x++;
		}
		Count++;
	}
	Count--;
//清0
	MC(0x12,ADDR_3000,Count);
	MR(ADDR_128KDATABUFF,ADDR_3000,256);
	p8=(u8*)ADDR_128KDATABUFF;
	for(i=0;i<Count;i++)
	{
		if(p8[i]!=0x12)
		{
			return 1;
		}
	}
	x=0;
	for(i=Count;i<256;i++)
	{
		if(p8[i]!=(x&0xff))
		{
			return 1;
		}
		x++;
	}
//
	
	return 0;
}
#endif

u32 Test_NAND_ALLBLOCK(void)//测试nand全部块;返回:0=正确,1=错误
{
	u32 i;
	u32* p32;
	u32 Data;
	u32 Block;
	
	Data=0;
	for(Block=0;Block<1024;Block++)
	{
		p32=(u32*)ADDR_128KDATABUFF;
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=Data;
			Data++;
		}
		MW(ADDR_128KDATABUFF,ADDR_EFLASH_Start+((128*1024)*Block),128*1024);
	}
	Data=0;
	for(Block=0;Block<1024;Block++)
	{
		MR(ADDR_128KDATABUFF,ADDR_EFLASH_Start+((128*1024)*Block),128*1024);
		p32=(u32*)ADDR_128KDATABUFF;
		for(i=0;i<((128*1024)/4);i++)
		{
			if(p32[i]!=Data)
			{
				return 1;
			}
			Data++;
		}
	}
	return 0;
}

u32 Test_NAND_RePlace_BLOCK(void)//测试nand替换块
{
	u32 i;
	u32* p32;
	
	MC(0xFF,ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT,NAND_REPLACE_BLOCK_COUNT);
	p32=(u32*)ADDR_128KDATABUFF;
	for(i=0;i<(128*1024)/4;i++)
	{
		p32[i]=i+2;
	}
	MW(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
	MR(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
	for(i=0;i<(128*1024)/4;i++)
	{
		if(p32[i]!=(i+2))
		{
			return 1;
		}
	}
	
	p32=(u32*)ADDR_128KDATABUFF;
	for(i=0;i<(128*1024)/4;i++)
	{
		p32[i]=i;
	}
	MW(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
	MR(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
	for(i=0;i<(128*1024)/4;i++)
	{
		if(p32[i]!=i)
		{
			return 1;
		}
	}
	return 0;
}





























