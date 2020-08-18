
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../../NUC97x/Driver/NUC97x_SPIFLASH.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_CRC.h"


#define SPIQuadModeRead     0//SPIQuadMode读:0=否,1=是
#define SPIQuadModeWrite     0//SPIQuadMode写:0=否,1=是

u32 ReadStatus_SPIFLASH(u32 Instruction)//读状态;入口:Instruction=05h(Register),35h(Register2),15h(Register3);返回:Status
{
	u32 i;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI0();//SPI0初始化(SPI FLASH)
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发xxh
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=Instruction;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	//[0]GO_BUSY
	1;
	while(p32ctrl[0]&1);
//读
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
	p32RxData=(u32*)(REG_SPI0_RX0);
	i=p32RxData[0];
	i&=0xff;
	return i;
}

void Write_Enable_SPIFLASH(void)//写允许
{
	u32 i;
	u32 *p32ctrl;
	u32 *p32TxData;
	
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发(06h)
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0x06;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
}
void Write_Disable_SPIFLASH(void)//写无效
{
	u32 i;
	u32 *p32ctrl;
	u32 *p32TxData;
	
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发(04h)
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0x04;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
}

void QUAD_ENABLE_SPIFLASH(void)//Quad读写允许
{
	u32 i;
	u32 x;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	x=ReadStatus_SPIFLASH(0x35);//读状态;入口:Instruction=05h(Register),35h(Register2),15h(Register3);返回:Status
	if((x&2)==0)
	{
		x|=2;
//Write Enable
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发(06h)
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=0x06;
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
		
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发31h,new Status
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=(0x31<<8)|x;
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(16<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
	//
		while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
	}	
}

u32 ID_SPIFLASH(void)//读SPI FLASH器件ID
{
	u32 id;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI0();//SPI0初始化(SPI FLASH)
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发0x90,0x00,0x00,0x00
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0x9000;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(16<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//读
	p32TxData[0]=0;
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	id=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(id--);
	p32RxData=(u32*)(REG_SPI0_RX0);
	id=p32RxData[0];
	id&=0xffff;
	return id;
}
u64 Unique_ID_SPIFLASH(void)//读SPI FLASH器件Unique ID
{
	u32 i;
	u64 id;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI0();//SPI0初始化(SPI FLASH)
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发0x4bh
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0x4b;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//发Dummy1,Dummy2,Dummy3,Dummy4
	p32TxData[0]=0;
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//读
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	p32RxData=(u32*)(REG_SPI0_RX0);
	id=p32RxData[0];
	id<<=32;
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	id|=p32RxData[0];
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
	return id;
}


#if SPIQuadModeRead==0//读:0=否,1=是
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//Fast Read(0Bh)
{
	u32 i;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	if(Byte==0)
	{
		return;
	}
	Init_SPI0();//SPI0初始化(SPI FLASH)
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发Fast Read(0Bh),A23-A16,A15-A8,A7-A0
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0x0B000000|ADDR_Sour;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//发Dummy
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	//[21]QUADM:1 = Quad I/O mode Enabled
	//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	
//读
	p8=(u8*)ADDR_Dest;
	p32RxData=(u32*)(REG_SPI0_RX0);
	while(Byte)
	{
		p32ctrl[0]=
		(0<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
		(0<<20)|//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
		i=p32RxData[0];
		if(Byte)
		{
			p8[0]=i>>24;
			p8++;
			Byte--;
		}
		if(Byte)
		{
			p8[0]=i>>16;
			p8++;
			Byte--;
		}
		if(Byte)
		{
			p8[0]=i>>8;
			p8++;
			Byte--;
		}
		if(Byte)
		{
			p8[0]=i;
			p8++;
			Byte--;
		}
	}
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
	Write_Disable_SPIFLASH();//写无效
}
#endif
#if SPIQuadModeWrite==0//写:0=否,1=是
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//Page Program (02h)
{
	u32 i;
	u32 SectorByte;
	u32 Page;
	u8 *p8s;
	u8 *p8d;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 some;
	
	while(Byte)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		SectorByte=4096-(ADDR_Dest&0xfff);//本Sector能写字节
		i=(ADDR_Dest&0xfffff000);//4k Sector 开始地址
//读原Sector数据
		Read_SPIFLASH(ADDR_4KWRITE_BUFF,i,4096);
		if(SectorByte>Byte)
		{
			SectorByte=Byte;
		}
//更新Sector数据
		p8s=(u8*)ADDR_Sour;
		p8d=(u8*)ADDR_4KWRITE_BUFF+(ADDR_Dest&0xfff);
		some=0;
		for(i=0;i<SectorByte;i++)
		{
			if(p8d[i]!=p8s[i])
			{
				some=1;
				for(;i<SectorByte;i++)
				{
					p8d[i]=p8s[i];
				}
				break;
			}
		}
		if(some==0)
		{//相同
		//next
			Byte-=SectorByte;
			ADDR_Sour+=SectorByte;
			ADDR_Dest+=SectorByte;
			continue;
		}
//檫除Sector
		Write_Enable_SPIFLASH();//写允许
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发20h,A23-A16,A15-A8,A7-A0
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=0x20000000|(ADDR_Dest&0xfffff000);
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
		while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		
		p8=(u8*)(ADDR_4KWRITE_BUFF);
		for(Page=0;Page<16;Page++)
		{
			Write_Enable_SPIFLASH();//写允许
	//cs0=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=1;
	//发Page Program02h,A23-A16,A15-A8,A7-A0
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0x02000000|((ADDR_Dest&0xfffff000)+(256*Page));
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p32ctrl[0]=
			//[21]QUADM:1 = Quad I/O mode Enabled
			//[20]DIR_2QM:0=Input,1=Output
			(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
			(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
			1;//[0]GO_BUSY
			while(p32ctrl[0]&1);
	//发数据
			for(i=0;i<256;i++)
			{
				p32TxData[0]=p8[0];
				p32ctrl[0]=
				//[21]QUADM:1 = Quad I/O mode Enabled
				//[20]DIR_2QM:0=Input,1=Output
				(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
				(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
				1;//[0]GO_BUSY
				p8++;
				while(p32ctrl[0]&1);
			}
	//cs0=1
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
			while(i--);
			while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		}
//next
		Byte-=SectorByte;
		ADDR_Sour+=SectorByte;
		ADDR_Dest+=SectorByte;
	}
	Write_Disable_SPIFLASH();//写无效
}

void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte)//Page Program (02h)
{
	u32 i;
	u32 SectorByte;
	u32 Page;
	u8 *p8d;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 some;
	
	while(Byte)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		SectorByte=4096-(ADDR_Dest&0xfff);//本Sector能写字节
		i=(ADDR_Dest&0xfffff000);//4k Sector 开始地址
//读原Sector数据
		Read_SPIFLASH(ADDR_4KWRITE_BUFF,i,4096);
		if(SectorByte>Byte)
		{
			SectorByte=Byte;
		}
//更新Sector数据
		p8d=(u8*)ADDR_4KWRITE_BUFF+(ADDR_Dest&0xfff);
		some=0;
		for(i=0;i<SectorByte;i++)
		{
			if(p8d[i]!=Data)
			{
				some=1;
				for(;i<SectorByte;i++)
				{
					p8d[i]=Data;
				}
				break;
			}
		}
		if(some==0)
		{//相同
		//next
			Byte-=SectorByte;
			ADDR_Dest+=SectorByte;
			continue;
		}
		
//檫除Sector
		Write_Enable_SPIFLASH();//写允许
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发20h,A23-A16,A15-A8,A7-A0
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=0x20000000|(ADDR_Dest&0xfffff000);
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
		while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		
		p8=(u8*)(ADDR_4KWRITE_BUFF);
		for(Page=0;Page<16;Page++)
		{
			Write_Enable_SPIFLASH();//写允许
	//cs0=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=1;
	//发Page Program02h,A23-A16,A15-A8,A7-A0
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0x02000000|((ADDR_Dest&0xfffff000)+(256*Page));
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p32ctrl[0]=
			//[21]QUADM:1 = Quad I/O mode Enabled
			//[20]DIR_2QM:0=Input,1=Output
			(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
			(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
			1;//[0]GO_BUSY
			while(p32ctrl[0]&1);
	//发数据
			for(i=0;i<256;i++)
			{
				p32TxData[0]=p8[0];
				p32ctrl[0]=
				//[21]QUADM:1 = Quad I/O mode Enabled
				//[20]DIR_2QM:0=Input,1=Output
				(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
				(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
				1;//[0]GO_BUSY
				p8++;
				while(p32ctrl[0]&1);
			}
	//cs0=1
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
			while(i--);
			while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		}
//next
		Byte-=SectorByte;
		ADDR_Dest+=SectorByte;
	}
	Write_Disable_SPIFLASH();//写无效
}
#endif

#if SPIQuadModeRead==1//读:0=否,1=是
u32 spiSpeed(u32 MHz);//SPI输入时钟速度;入口:需要的速度MHz,返回:分频值
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//Fast Read Quad I/O (EBh)
{
//注:Quad读写有问题!
	u32 i;
//	u32 x;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	if(Byte==0)
	{
		return;
	}
	Init_SPI0();//SPI0初始化(SPI FLASH)
	
//QUAD ENABLE(s9)
	QUAD_ENABLE_SPIFLASH();//Quad读写允许
//cs0=0
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=1;
//发Fast Read Quad I/O (EBh)
	p32TxData=(u32*)(REG_SPI0_TX0);
	p32TxData[0]=0xEB;
	p32ctrl=(u32*)(REG_SPI0_CNTRL);
	p32ctrl[0]=
	(0<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
	(0<<20)|//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//发I/O:A23-A16,A15-A8,A7-A0,M7-M0
	p32TxData[0]=(ADDR_Sour<<8)|0xF0;
	p32ctrl[0]=
	(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
	(1<<20)|//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//发I/O:Dummy,Dummy
	p32ctrl[0]=
	(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
	(0<<20)|//[20]DIR_2QM:0=Input,1=Output
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(16<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
//读
	p32RxData=(u32*)(REG_SPI0_RX0);
	p8=(u8*)ADDR_Dest;
	while(Byte)
	{
		p32ctrl[0]=
		(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
		(0<<20)|//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
		i=p32RxData[0];
		while(Byte)
		{
			p32ctrl[0]=
			(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
			(0<<20)|//[20]DIR_2QM:0=Input,1=Output
			(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
			(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
			1;//[0]GO_BUSY
			
			if(Byte)
			{
				p8[0]=i>>24;
				p8++;
				Byte--;
			}
			if(Byte)
			{
				p8[0]=i>>16;
				p8++;
				Byte--;
			}
			if(Byte)
			{
				p8[0]=i>>8;
				p8++;
				Byte--;
			}
			if(Byte)
			{
				p8[0]=i;
				p8++;
				Byte--;
			}
			while(p32ctrl[0]&1);
			i=p32RxData[0];
		}
	}
//cs0=1
	p32ctrl=(u32*)(REG_SPI0_SSR);
	p32ctrl[0]=0;
	i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
	while(i--);
	Write_Disable_SPIFLASH();//写无效
}
#endif
#if SPIQuadModeWrite==1//写:0=否,1=是
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//Quad Input Page Program (32h),clock speeds <5MHz
{
//注:Quad读写有问题!
	u32 i;
	u32 SectorByte;
	u32 Page;
	u8 *p8s;
	u8 *p8d;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	while(Byte)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		SectorByte=4096-(ADDR_Dest&0xfff);//本Sector能写字节
		i=(ADDR_Dest&0xfffff000);//4k Sector 开始地址
//读原Sector数据
		Read_SPIFLASH(ADDR_4KWRITE_BUFF,i,4096);
		if(SectorByte>Byte)
		{
			SectorByte=Byte;
		}
//更新Sector数据
		p8s=(u8*)ADDR_Sour;
		p8d=(u8*)ADDR_4KWRITE_BUFF+(ADDR_Dest&0xfff);
		for(i=0;i<SectorByte;i++)
		{
			p8d[i]=p8s[i];
		}
//檫除Sector
		Write_Enable_SPIFLASH();//写允许
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发20h,A23-A16,A15-A8,A7-A0
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=0x20000000|(ADDR_Dest&0xfffff000);
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
		while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		
//Quad Input Page Program
		p8=(u8*)(ADDR_4KWRITE_BUFF);
		for(Page=0;Page<16;Page++)
		{
			Write_Enable_SPIFLASH();//写允许
	//cs0=0
			p32ctrl=(u32*)(REG_SPI0_DIVIDER);
			p32ctrl[0]=spiSpeed(4);//SPI输入时钟速度;入口:需要的速度MHz,返回:分频值
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=1;
			
	//发32h,A23-A16,A15-A8,A7-A0
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0x32000000|((ADDR_Dest&0xfffff000)+(256*Page));
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p32ctrl[0]=
			//[21]QUADM:1 = Quad I/O mode Enabled
			//[20]DIR_2QM:0=Input,1=Output
			(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
			(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
			1;//[0]GO_BUSY
			while(p32ctrl[0]&1);
	//发数据
			for(i=0;i<256;i++)
			{
				p32TxData[0]=p8[0];
				p32ctrl[0]=
				(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
				(1<<20)|//[20]DIR_2QM:0=Input,1=Output
				(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
				(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
				1;//[0]GO_BUSY
				p8++;
				while(p32ctrl[0]&1);
				
			}
	//cs0=1
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
			while(i--);
			while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		}
//next
		Byte-=SectorByte;
		ADDR_Sour+=SectorByte;
		ADDR_Dest+=SectorByte;
	}
	Write_Disable_SPIFLASH();//写无效
}

void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte)//Quad Input Page Program (32h),clock speeds <5MHz
{
//注:Quad读写有问题!
	u32 i;
	u32 SectorByte;
	u32 Page;
	u8 *p8d;
	u8 *p8;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	while(Byte)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		SectorByte=4096-(ADDR_Dest&0xfff);//本Sector能写字节
		i=(ADDR_Dest&0xfffff000);//4k Sector 开始地址
//读原Sector数据
		Read_SPIFLASH(ADDR_4KWRITE_BUFF,i,4096);
		if(SectorByte>Byte)
		{
			SectorByte=Byte;
		}
//更新Sector数据
		p8d=(u8*)ADDR_4KWRITE_BUFF+(ADDR_Dest&0xfff);
		for(i=0;i<SectorByte;i++)
		{
			p8d[i]=Data;
		}
//檫除Sector
		Write_Enable_SPIFLASH();//写允许
	//cs0=0
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=1;
	//发20h,A23-A16,A15-A8,A7-A0
		p32TxData=(u32*)(REG_SPI0_TX0);
		p32TxData[0]=0x20000000|(ADDR_Dest&0xfffff000);
		p32ctrl=(u32*)(REG_SPI0_CNTRL);
		p32ctrl[0]=
		//[21]QUADM:1 = Quad I/O mode Enabled
		//[20]DIR_2QM:0=Input,1=Output
		(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
		(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		1;//[0]GO_BUSY
		while(p32ctrl[0]&1);
	//cs0=1
		p32ctrl=(u32*)(REG_SPI0_SSR);
		p32ctrl[0]=0;
		i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
		while(i--);
		while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		
//Quad Input Page Program
		p8=(u8*)(ADDR_4KWRITE_BUFF);
		for(Page=0;Page<16;Page++)
		{
			Write_Enable_SPIFLASH();//写允许
	//cs0=0
			p32ctrl=(u32*)(REG_SPI0_DIVIDER);
			p32ctrl[0]=spiSpeed(4);//SPI输入时钟速度;入口:需要的速度MHz,返回:分频值
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=1;
	//发32h,A23-A16,A15-A8,A7-A0
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0x32000000|((ADDR_Dest&0xfffff000)+(256*Page));
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p32ctrl[0]=
			//[21]QUADM:1 = Quad I/O mode Enabled
			//[20]DIR_2QM:0=Input,1=Output
			(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
			(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
			1;//[0]GO_BUSY
			while(p32ctrl[0]&1);
	//发数据
			for(i=0;i<256;i++)
			{
				p32TxData[0]=p8[0];
				p32ctrl[0]=
				(1<<21)|//[21]QUADM:1 = Quad I/O mode Enabled
				(1<<20)|//[20]DIR_2QM:0=Input,1=Output
				(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
				(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
				1;//[0]GO_BUSY
				p8++;
				while(p32ctrl[0]&1);
				
			}
	//cs0=1
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			i=30;//CS Deselect Time (for Erase or Program or Write) min=50ns
			while(i--);
			while(ReadStatus_SPIFLASH(0x05)&1);//读状态1;返回:(S7-S0)
		}
//next
		Byte-=SectorByte;
		ADDR_Dest+=SectorByte;
	}
	Write_Disable_SPIFLASH();//写无效
}
#endif




#ifdef BOOT
u32 CRC32_STM32F4xx(u32 initcrc,u8* DataPtr,u32 DataLen,u32 Poly)
{
	u32 i;
	u32 nAccum;
	
	u32 j;
	u32 nData;
	u32 Table_CRC[256];
	for ( i = 0; i < 256; i++ )
	{
		nData =( i << 24 );
		nAccum = 0;
		for ( j = 0; j < 8; j++ )
		{
			if ( ( nData ^ nAccum ) & 0x80000000 )
			{
				nAccum = ( nAccum << 1 ) ^ Poly;
			}
			else
			{
				nAccum <<= 1;
			}
			nData <<= 1;
		}
		Table_CRC[i] = nAccum;
	}
	
	nAccum=initcrc;
	DataLen/=4;
	for ( i = 0; i < DataLen; i++ )
	{
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[3]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[2]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[1]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[0]];
		DataPtr+=4;
	}
	return nAccum;//^0xffffffff;
}
u32 RAM_CRC32(u32 ADDR_DATA,u32 Byte)//RAM中数据CRC32计算
{
	u32 i;
	i=CRC32_STM32F4xx(0xffffffff,(u8*)ADDR_DATA,Byte,0x04C11DB7);
	return i;
}
u32 SoftwareCheck(u32 *p32)//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
{
	u32 i;
	u32 x;
	u32 y;
	
	if(p32[110]!=ADDR_MS_ENTRY)//110: 软件模块开始地址
	{
		return 0;//0=错误
	}
	i=p32[111]-p32[110];//软件模块结束地址-软件模块开始地址
	if((i>(1024*1024))||(i<(4*132)))
	{
		return 0;//0=错误
	}
	i/=4;
	if(p32[i-1]!=0x5A5A5A5A)//固定标识
	{
		return 0;//0=错误
	}
	if((p32[129]&0xff)==0)
	{//KEIL直接写入
	}
	else
	{//非KEIL直接写入
		y=RAM_CRC32((u32)p32,(p32[111]-p32[110])-8);//RAM中数据CRC32计算
		x=p32[i-2];
		if(y!=x)
		{
			return 0;//0=错误
		}
	}
	return 1;
}
#else
u32 SoftwareCheck(u32 *p32);//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
#endif

u32 LOAD_PROGRAM(u32 ADDR_ENTRY,u32 LENmax)//软件装入,自动从主区和备份区装入,且自动拷贝到主区和备份区;返回:0=错误，1=正确
{
	u32 i;
	u32 LEN_ImageHead;
	u32 *p32s;
	u32* p32d;
	
	Read_SPIFLASH((u32)ADDR_DATABUFF,0,24);
	p32s=(u32*)ADDR_DATABUFF;
	LEN_ImageHead=p32s[5];//DDR參數的个数
	LEN_ImageHead*=2;///DDR參數成对
	LEN_ImageHead*=4;
	LEN_ImageHead+=24;
	LEN_ImageHead+=15;//DDR參數16字节对齐
	LEN_ImageHead&=0xfffffff0;
	if(LEN_ImageHead>512)
	{
		LEN_ImageHead=0x190;
	}
	Read_SPIFLASH(ADDR_ENTRY,ADDR_ENTRY+LEN_ImageHead,LENmax);
	i=SoftwareCheck((u32 *)ADDR_ENTRY);//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
	if(i==1)
	{//正确
		Read_SPIFLASH(ADDR_MyHeap,ADDR_ENTRY+LEN_ImageHead+(4*1024*1024),LENmax);
		p32s=(u32*)(ADDR_ENTRY);
		p32d=(u32*)(ADDR_MyHeap);
		for(i=0;i<(LENmax-512)/4;i++)
		{
			if(p32s[i]!=p32d[i])
			{
				Write_SPIFLASH(ADDR_ENTRY,ADDR_ENTRY+LEN_ImageHead+(4*1024*1024),LENmax-512);
				break;
			}
		}
		return 1;
	}
	else
	{//错误
		i=ID_SPIFLASH();//读SPI FLASH器件ID
		if(i==0xEF15)
		{//是8MB
			Read_SPIFLASH(ADDR_ENTRY,ADDR_ENTRY+LEN_ImageHead+(4*1024*1024),LENmax);
			i=SoftwareCheck((u32 *)ADDR_ENTRY);//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
			if(i==1)
			{//正确
				Write_SPIFLASH(ADDR_ENTRY,ADDR_ENTRY+LEN_ImageHead,LENmax-512);
				return 1;
			}
		}
	}
	return 0;
}










