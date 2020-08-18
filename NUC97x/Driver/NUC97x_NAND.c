
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/NAND.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_CRC.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../NUC97x/NUC97x_AIC.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../MS/yaffsAPP.h"


#define BlockMax_NandBuff     128//nand缓冲最大缓冲数据块数

void Init_NAND(void)//复位nand
{
	u32 i;
	u32 CS0_disable;
	u32 CS1_disable;
	u32 *p32;
	u32 irq;
	
//設置CLK_HCLKEN寄存器FMI，NAND位
	p32=(u32*)(REG_CLK_HCLKEN);
	i=p32[0];
	i|=3<<20;
	p32[0]=i;
//AHB IP Reset
	p32=(u32*)(REG_SYS_AHBIPRST);
	p32[0]=1<<20;//FMI controller reset enabled.
	p32[0]=0;
	
//選擇多功能控制，NAND有二組：GPC0~14和GPI1~15
	if (inpw(REG_SYS_PWRON) & 0x08000000)
	{//NUC975 or NUC978
	//Set GPI1~15 for NAND
		CS0_disable=0;
		CS1_disable=1;
		p32=(u32*)(REG_SYS_GPI_MFPL);
		irq=off_irq();
		i=p32[0];
		i&=0x0000000f;
		i|=0x55555550;//Ctrl
		p32[0]=i;
		destore_irq(irq);
		p32=(u32*)(REG_SYS_GPI_MFPH);
		p32[0]=0x55555555;//D0-D7
	}
	else
	{//NUC972
	//Set GPC0~14 for NAND
		CS0_disable=0;
		CS1_disable=1;
		p32=(u32*)(REG_SYS_GPC_MFPL);
		p32[0]=0x55555555;//D0-D7
		p32=(u32*)(REG_SYS_GPC_MFPH);
		p32[0]=0x05555555;//Ctrl
	}
	
//Timing Control(Reset Value 0x0001_0105,NOTE1: The reset value calculated based on 100MHz AHB Clock)
	p32=(u32*)(REG_NANDTMCTL);
	p32[0]=
	//[31:23] Reserved Reserved.
	//[22:16] CALE_SH
		//CLE/ALE Setup/Hold Time
		//This field controls the CLE/ALE setup/hold time to –WE.
		//The setup/hold time can be calculated using following equation:
		//tCLS = (CALE_SH+1)*TAHB.
		//tCLH = ((CALE_SH*2)+2)*TAHB.
		//tALS = (CALE_SH+1)*TAHB.
		//tALH = ((CALE_SH*2)+2)*TAHB.
	(2<<16)|
	//[15:8] HI_WID
		//Read/Write Enable Signal High Pulse Width
		//This field controls the high pulse width of signals –RE and –WE while H/W mode page access
		//is enabled. The pulse width is a multiple of period of AHB bus clock. ( The actual width time
		//will be [clock period*(HI_WID+1)] )
	(3<<8)|
	//[7:0] LO_WID
		//Read/Write Enable Signal Low Pulse Width
		//This field controls the low pulse width of signals –RE and –WE while H/W mode page access
		//is enabled. The pulse width is a multiple of period of AHB bus clock. ( The actual width time
		//will be [clock period*(LO_WID+1)] )
	5;
	
//使能FMI(Reset Value 0x0000_0000)
	p32=(u32*)(REG_FMI_CTL);
	p32[0]=
	//[31:4]Reserved
	//[3]NAND_EN
		//NAND Flash Functionality Enable
		//0 = Disable NAND flash functionality of FMI.
		//1 = Enable NAND flash functionality of FMI.
	(1<<3)|
	//[2]Reserved
	//[1]eMMC_EN
		//eMMC Functionality Enable
		//0 = Disable eMMC functionality of FMI.
		//1 = Enable eMMC functionality of FMI.
	//[0]SW_RST
		//Software Engine Reset
		//0 = Writing 0 to this bit has no effect.
		//1 = Writing 1 to this bit will reset all FMI engines. The contents of control register will not be cleared. This bit will auto clear after few clock cycles.
	1;
	while(p32[0]&1);
	
//Redundant Area Control(Reset Value 0x0000_0000)
	p32=(u32*)(REG_NANDRACTL);
	p32[0]=
	//31:16]MECC
		//Mask ECC During Write Page Data
		//These 16 bits registers indicate NAND controller to write out ECC parity or just 0xFF for each field (every 512 bytes) the real parity data will be write out to FMI_NANDRAx.
		//0x00 = Do not mask the ECC parity for each field.
		//0x01 = Mask ECC parity and write out FF to NAND ECC parity for 512 Bytes page size or 2K/4K/8K page size first 512 field.
		//0x02 = Mask ECC parity and write out FF to NAND ECC parity for 512 Bytes page size or 2K/4K/8K page size second 512 field.
		//0xxx = Mask ECC parity and write out FF to NAND ECC parity for 512 Bytes page size or 2K/4K/8K page size each 512 field.
	//[15:9]Reserved
	//[8:0]RA128EN
		//Redundant Area 128 Byte Enable
		//These bits indicate NAND flash extended redundant area.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2’b00, this field will be set 0x10 (16bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2’b01, this field will be set 0x40 (64bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2’b10, this field will be set 0x80 (128 bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2’b11, this field will be set 0x100 (256bytes) automatically.
		//Note: The REA128EN must be 4 byte aligned, so bit1 and bit0 can’t be filled 1 to it.
		//The maximum redundant area of the controller is 472Bytes.
	0x40;
	
//DMA Control(Reset Value 0x0000_0000)
	p32=(u32*)(REG_FMI_DMACTL);
	//while(p32[0]&0x200);//FMI_BUSY
	p32[0]=
	//[31:10]Reserved.
	//[9]FMI_BUSY
		//FMI DMA Transfer is in progress
		//This bit indicates if FMI is granted and doing DMA transfer or not.
		//0 = FMI DMA transfer is not in progress.
		//1 = FMI DMA transfer is in progress.
	//[8:4]//Reserved.
	//[3]//SG_EN
		//Enable Scatter-Gather Function for FMI
		//Enable DMA scatter-gather function or not.
		//0 = Normal operation. DMAC will treat the starting address in FMI_DMASA as starting pointer of a single block memory.
		//1 = Enable scatter-gather operation. DMAC will treat the starting address in FMI_DMASA as a starting address of Physical Address Descriptor (PAD) table. The format of these Pads’ will be described later.
	//[2]//Reserved.
	//[1]//SW_RST
		//Software Engine Reset
		//0 = Writing 0 to this bit has no effect.
		//1 = Writing 1 to this bit will reset the internal state machine and pointers. The contents of control register will not be cleared. This bit will auto clear after few clock cycles.
		//NOTE: The software reset DMA region.
	(1<<1)|
	//[0]//DMACEN
		//DMAC Engine Enable
		//Setting this bit to 1 enables DMAC’s operation. If this bit is cleared, DMAC will ignore all DMA request from FMI and force Bus Master into IDLE state.
		//0 = Disable DMAC.
		//1 = Enable DMAC.
		//NOTE: If target abort is occurred, DMACEN will be cleared.
	1;
	while(p32[0]&(1<<1));
	
//DMA Interrupt Enable(Reset Value 0x0000_0000)
	p32=(u32*)(REG_FMI_DMAINTEN);
	p32[0]=0;
	
//NAND Flash Control(Reset Value 0x1E88_0090)
	p32=(u32*)(REG_NANDCTL);
	p32[0]=
	//[31:27]Reserved 0x1E88_0090)
	//[26]CS1
		//NAND Flash Chip Select 1 Enable
		//0 = Chip select 1 enable.
		//1 = Chip select 1 disable.
	(CS1_disable<<26)|
	//[25]CS0
		//NAND Flash Chip Select 0 Enable
		//0 = Chip select 0 enable.
		//1 = Chip select 0 disable.
	(CS0_disable<<25)|
	//[24]Reserved
	//[23]ECC_EN
		//ECC Algorithm Enable
		//This field is used to select the ECC algorithm for data protecting. The BCH algorithm can correct 4 or 8 or 12 or 15 or 24 bits.
		//0 = Disable BCH code encode/decode.
		//1 = Enable BCH code encode/decode.
		//Note: If disable ECC_EN and when read data from NAND, NAND controller will ignore its ECC check result. When write data to NAND, NAND controller will write out 0xFF to every parity field.
		//Note: The ECC algorithm only protects data area and hardware ECC parity code in default. By setting PROT_3BEN (FMI_NANDCTL[8]) high, the first 3 bytes of redundant data are also protected by ECC algorithm.
	(1<<23)|
	//[22:18]BCH_TSEL
		//BCH Correct Bit Selection
		//This field is used to select BCH correct bits for data protecting. For BCH algorithm, T can be 4 or 8 or 12 or 15 or 24 for choosing (correct 4 or 8 or 12 or 15 or 24 bits).
		//00001 = Using BCH T24 to encode/decode (T24).(1024 Bytes per block)
		//00010 = Using BCH T4 to encode/decode (T4).
		//00100 = Using BCH T8 to encode/decode (T8).
		//01000 = Using BCH T12 to encode/decode (T12).
		//10000 = Using BCH T15 to encode/decode (T15).
#ifndef LINUX	
	(4<<18)|
#else
	(2<<18)|
#endif
	//[17:16]PSIZE
		//Page Size of NAND
		//This bit indicates the page size of NAND. There are four page sizes for choose, 512bytes/page, 2048bytes/page, 4096bytes/page and 8192bytes/page. Before setting PSIZE register, user must set BCH_TSEL register at first.
		//00 = Page size is 512bytes/page.
		//01 = Page size is 2048bytes/page.
		//10 = Page size is 4096bytes/page.
		//11 = Page size is 8192bytes/page.
	(1<<16)|
	//[15:10]Reserved.
	//[9]SRAM_INT
		//SRAM Initial
		//0 = Writing 0 to this bit has no effect.
		//1 = Writing 1 to this bit will reset the internal FMI_NANDRA0~FMI_NANDRA1 to 0xFFFF_FFFF.
		//The contents of control register will not be cleared. This bit will be auto cleared after few clock cycles.
	//[8]PROT_3BEN
		//Protect_3Byte Software Data Enable
		//The ECC algorithm only protects data area and hardware ECC parity code. User can choose to protect software redundant data first 3 bytes by setting this bit high.
		//0 = Software redundant data is not protected by ECC algorithm.
		//1 = Software redundant data first 3 bytes protected by ECC algorithm.
	(0<<8)|
	//[7]ECC_CHK
		//None Used Field ECC Check After Read Page Data
		//0 = Disable. NAND controller will always check ECC result for each field, no matter it is used or not.
		//1 = Enable. NAND controller will check 1’s count for byte 2, 3 of redundant data of the ECC in each field. If count value is greater than 8, NAND controller will treat this field as none used field; otherwise, it’s used. If that field is none used field, NAND controller will ignore its ECC check result.
	(0<<7)|
	//[6]Reserved
	//[5]PROT_REGION_EN
		//Protect Region Enable
		//This field is used to protect NAND Flash region from address 0 to address {FMI_NANDPRTOA1, FMI_NANDPROTA0} not be written.
		//0 = Disable.
		//1 = Enable.
	//[4]REDUN_AUTO_WEN
		//Redundant Area Auto Write Enable
		//This field is used to auto write redundant data out to NAND Flash. The redundant data area is dependent on FMI_NANDRACTL register.
		//0 = Disable auto write redundant data out to NAND flash.
		//1 = Enable auto write redundant data out to NAND flash.
	(1<<4)|
	//[3]REDUN_REN
		//Redundant Area Read Enable
		//This bit enables NAND controller to transfer redundant data from NAND Flash into FMI_NANDRA, the data size is dependent on FMI_NANDRACTL register.
		//0 = No effect.
		//1 = Enable read redundant data transfer.
		//NOTE: When transfer completed, this bit will be cleared automatically.
	//[2]DWR_EN
		//DMA Write Data Enable
		//This bit enables NAND controller to transfer data (1 page) from DMAC’s embedded frame buffer into NAND Flash or NAND type flash.
		//0 = No effect.
		//1 = Enable DMA write data transfer.
		//NOTE: When DMA transfer completed, this bit will be cleared automatically.
	//[1]DRD_EN
		//DMA Read Data Enable
		//This bit enables NAND controller to transfer data (1 page) from NAND Flash or NAND type flash into DMAC’s embedded frame buffer.
		//0 = No effect.
		//1 = Enable DMA read data transfer.
		//NOTE: When DMA transfer completed, this bit will be cleared automatically.
	//[0]SW_RST
		//Software Engine Reset
		//0 = Writing 0 to this bit has no effect.
		//1 = Writing 1 to this bit will reset the internal state machine and counters (include DWR_EN (FMI_NANDCTL[2]) and DRD_EN (FMI_NANDCTL[1])). The contents of control register will not be cleared. This bit will be auto cleared after few clock cycles.
	1;
	while(p32[0]&1);
	
//Extend Control(Reset Value 0x0000_0000)
	p32=(u32*)(REG_NANDECTL);
	p32[0]=
	//[31:1]Reserved.
	//[0]WP
		//NAND Flash Write Protect Control (Low Active)
		//Set this bit low to make NAND_nWP functional pin low to prevent the write to NAND flash device.
		//0 = NAND flash is write-protected and is not writeable.
		//1 = NAND flash is not write-protected and is writeable.
	1;
//Interrupt Enable(Reset Value 0x0000_0000)
	p32=(u32*)(REG_NANDINTEN);
	p32[0]=0;

}
u32 Wait_NAND(u32 usMax)//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
{
	u32 i;
	u32 t;
	u32 *p32CMD;
	u32 *p32DATA;
	
	p32CMD=(u32*)(REG_NANDCMD);
	p32DATA=(u32*)(REG_NANDDATA);
	while(1)//常规测试时值i=9;当前最大值100
	{
//Status Register Definition
//SR    Program       Program Page   Page Read     Page Read     Block Erase    Description
//Bit		Page          Cache Mode                   Cache Mode
//7     Write protect Write protect  Write protect Write protect Write protect  0 = Protected,1 = Not protected
//6     RDY           RDY cache      RDY           RDY cache     RDY            0 = Busy,1 = Ready
//5     ARDY          ARDY           ARDY          ARDY          ARDY           Don't Care
//4     –            –             –            –            –              Don't Care
//3     –            –             Rewrite       –            –              0 = Normal or uncorrectable,1 = Rewrite recommended
//                                   recommended
//2     –            –             –            –            –              Don't Care
//1     FAILC(N-1)    FAILC(N-1)     Reserved      –            –              Don't Care
//0     FAIL          FAIL (N)       FAIL          –            FAIL            0 = Successful PROGRAM/ERASE/READ,1 = Error in PROGRAM/ERASE/READ
		*p32CMD=0x70;
		i=*p32DATA;
		if((i&0x40)==0x40)
		{
			*p32CMD=0x00;//1st Cycle Page Read Command
			t=4;//至少延时tWHR=60ns
			while(t--);
			return 0;
		}
		SoftDelayUS(1);//软件us延时
		if(usMax==0)
		{
			break;
		}
		usMax--;
	}
	*p32CMD=0x00;//1st Cycle Page Read Command
	t=4;//至少延时tWHR=60ns
	while(t--);
	return i;
}
u64 ID_NAND(void)//读器件ID,同时nand数据位宽度自动识别
{
	u64 id;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32 *p32DATA;
	
	Init_NAND();//复位nand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
	p32DATA=(u32*)(REG_NANDDATA);
	*p32CMD=0x90;
	*p32ADDR=0x0|0x80000000;
	id=*p32DATA;//1st
	id<<=8;
	id|=*p32DATA;//2nd
	id<<=8;
	id|=*p32DATA;//3rd
	id<<=8;
	id|=*p32DATA;//4th
	id<<=8;
	id|=*p32DATA;//5th
	*p32CMD=0;//Page Read Command

	FileDownload->NAND_BUSBIT=8;//nand数据位宽度自动识别:8=8bit,16=16bit
	return id;
}

void Enable_Internal_ECC(void)//允许nand内部ECC
{
}
void Disable_Internal_ECC(void)//取消nand内部ECC
{
}

void BlockErase_NAND(u32 ADDR_Dest,u32 Busy_Wait)//NAND_FLASH 擦除当前1个块;没块替换;入口:Busy_Wait=1 NAND忙时等待Busy_Wait=0不等待
{
	u32 ReNum;
	u32 Err;
	u32 *p32CMD;
	u32 *p32ADDR;
//	u32 *p32DATA;
	
	Init_NAND();//复位nand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	ReNum=2;
	while(ReNum--)
	{
		*p32CMD=0x60;
		*p32ADDR=(ADDR_Dest>>11);//Row Add. 1
	#if (NAND_2G|NAND_4G|NAND_8G)
		*p32ADDR=(ADDR_Dest>>19);//Row Add. 2
		*p32ADDR=(ADDR_Dest>>27)|0x80000000;//Row Add. 3
	#else
		*p32ADDR=(ADDR_Dest>>19)|0x80000000;//Row Add. 2
	#endif
		*p32CMD=0xD0;//Command
		Err=Wait_NAND(3000*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		if(Err==0)
		{
			break;
		}
	}

}

u32 Read_Page(u32 ADDR_Dest,u32 ADDR_Sour)//NAND_FLASH 没替换读页数据;返回:0=没错误,!=0错误
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 *p32NANDCTL;
	u32 *p32DMA;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32 ErrStatus;
	u32 ErrBitNUM;
	u32* p32ErrAddr;
	u32* p32ErrData;
	u32 ErrData;
	u32 ErrRead;
	u8* p8SOUR;
	u32* p32s;
	u32* p32d;
//	u32 *p32DATA;
	
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	
	ErrRead=0;
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
	//while(!p32s[0]&0x400);
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
			sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
			p32d=(u32*)(REG_NANDECCES0);
			for(n=0;n<4;n++)
			{
				ErrStatus=p32d[n];
				for(m=0;m<4;m++)
				{
					if(ErrStatus&3)
					{
						if((ErrStatus&3)!=0x1)
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
									p8SOUR=(u8*)(ADDR_Dest+(((n*4)+m)*512)+(p32ErrAddr[0]&0x3ff));
									ErrData=p32ErrData[0]&0xff;
									p8SOUR[0]^=ErrData;
								}
								if(ErrBitNUM)
								{
									ErrBitNUM--;
									p8SOUR=(u8*)(ADDR_Dest+(((n*4)+m)*512)+((p32ErrAddr[0]>>16)&0x3ff));
									ErrData=(p32ErrData[0]>>8)&0xff;
									p8SOUR[0]^=ErrData;
									p32ErrAddr++;
								}
								if(ErrBitNUM)
								{
									ErrBitNUM--;
									p8SOUR=(u8*)(ADDR_Dest+(((n*4)+m)*512)+(p32ErrAddr[0]&0x3ff));
									ErrData=(p32ErrData[0]>>16)&0xff;
									p8SOUR[0]^=ErrData;
								}
								if(ErrBitNUM)
								{
									ErrBitNUM--;
									p8SOUR=(u8*)(ADDR_Dest+(((n*4)+m)*512)+((p32ErrAddr[0]>>16)&0x3ff));
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
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	return ErrRead;
}

u32 Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//NAND_FLASH 没替换读;返回:0=没错误,!=0错误
{
	u32 i;
	u32 x;
	u32 PageByte;
	u8* p8SOUR;
	u8* p8DEST;
	u16* p16SOUR;
	u16* p16DEST;
	u32 ErrRead;
	
	Init_NAND();
	ErrRead=0;
	p8DEST=(u8*)ADDR_Dest;
	while(Byte!=0)
	{
		while(1)//掉电中断冲突重读
		{
			PageByte=2048-(ADDR_Sour&0x7FF);//当前Page能读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			if(((ADDR_Sour&0x7FF)==0)&&(Byte>=2048))
			{//需读的nand页对齐,同时需取数>=页字节2048
				ErrRead|=Read_Page((u32)p8DEST,ADDR_Sour);//NAND_FLASH 没替换读页数据;返回:0=没错误,!=0错误
				p8DEST+=PageByte;
			}
			else
			{
				ErrRead|=Read_Page(ADDR_NAND_PAGEREAD_BUFF,ADDR_Sour);//NAND_FLASH 没替换读页数据;返回:0=没错误,!=0错误
			//取数据
				p8SOUR=(u8*)ADDR_NAND_PAGEREAD_BUFF+(ADDR_Sour&0x7ff);
				i=PageByte;
				if((((u32)p8SOUR&1)==0)&&(((u32)p8DEST&1)==0))
				{//同时为偶
					p16SOUR=(u16*)p8SOUR;
					p16DEST=(u16*)p8DEST;
					while(i>=2)
					{
						*p16DEST=*p16SOUR;
						p16SOUR++;
						p16DEST++;
						i-=2;
					}
					p8SOUR=(u8*)p16SOUR;
					p8DEST=(u8*)p16DEST;
					if(i)
					{
						*p8DEST=*p8SOUR;
					}
				}
				else
				{
					if(((u32)p8SOUR&1)&&((u32)p8DEST&1))
					{//同时为奇
						if(i)
						{
							*p8DEST=*p8SOUR;
							p8SOUR++;
							p8DEST++;
							i--;
						}
						p16SOUR=(u16*)p8SOUR;
						p16DEST=(u16*)p8DEST;
						while(i>=2)
						{
							*p16DEST=*p16SOUR;
							p16SOUR++;
							p16DEST++;
							i-=2;
						}
						p8SOUR=(u8*)p16SOUR;
						p8DEST=(u8*)p16DEST;
						if(i)
						{
							*p8DEST=*p8SOUR;
						}
					}
					else
					{
						for(i=0;i<PageByte;i++)
						{
							x=p8SOUR[i];
							*p8DEST=x;
							p8DEST++;
						}
					}
				}
			}
			p8DEST-=PageByte;
			if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
			{
				Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
				continue;
			}
			break;
		}
		ADDR_Sour+=PageByte;
		p8DEST+=PageByte;
		Byte-=PageByte;
	}
	return ErrRead;//返回:0=没错误,!=0错误
}

u32 BLOCK_Write_NAND(u32 ADDR_Dest)//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误;没块替换
{
	u32 i;
	u32 x;
	u32 y;
	u32 *p32s;
	u32 *p32d;
	u32 Page;
	u32 ReNum;
	u32 *p32CMD;
	u32 *p32ADDR;
//	u32 *p32DATA;
	u32* p32DMA;
	u32 *p32NANDCTL;

	
//---测试nand替换块时---
//	if(ADDR_Dest==(ADDR_Download_ENTRY-ADDR_EFLASH_Start))
//	{
//		return 1;//测试nand替换块时
//	}
//---测试nand替换块时 END---
	
	Init_NAND();//复位nand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	ADDR_Dest&=0xfffe0000;
	
//块写
	ReNum=2;//写入错误重复次数
	while(ReNum--)
	{
		//块擦除
		BlockErase_NAND(ADDR_Dest,0);//NAND_FLASH 块擦除;没块替换
		//块写回(每分页2048字节)
		sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
		for(Page=0;Page<64;Page++)
		{
			while(1)//掉电中断冲突重写
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
				
				p32s=(u32*)(REG_NANDINTSTS);
				p32s[0]|=0x405;//清[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IF位
				p32DMA=(u32*)(REG_FMI_DMASA);
				p32DMA[0]=ADDR_128KWRITE_BUFF+(Page*2048);
				p32NANDCTL=(u32*)(REG_NANDCTL);
				i=p32NANDCTL[0];
				i|=
				(1<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
				(0<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
				(1<<2)|//[2]DWR_EN DMA Write Data Enable
				(0<<1);//[1]DRD_EN DMA Read Data Enable
				p32NANDCTL[0]=i;
				while(1)
				{
					p32s=(u32*)(REG_NANDINTSTS);
					i=p32s[0];
					if(i&1)
					{//[0]DMA_IF位
						break;
					}
				}
				//__disable_irq();
				if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
				{
					Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
					//__enable_irq();
					continue;
				}
				*p32CMD=0x10;//Command
				//__enable_irq();
				break;
			}
			Wait_NAND(600*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		}
//读比较
		for(Page=0;Page<64;Page++)
		{
			Read_Page(ADDR_NAND_PAGEREAD_BUFF,ADDR_Dest+(Page*2048));//NAND_FLASH 没替换读页数据;返回:0=没错误,!=0错误
			p32s=(u32*)(ADDR_128KWRITE_BUFF+(Page*2048));
			p32d=(u32*)(ADDR_NAND_PAGEREAD_BUFF);
			for(i=0;i<(2048/4);i++)
			{
				x=p32s[i];
				y=p32d[i];
				if(x!=y)
				{
					break;
				}
			}
			if(i<(2048/4))
			{
				break;
			}
		}
		if(Page>=64)
		{
			return 0;//返回;0=正确,1=错误
		}
	}
	return 1;//返回;0=正确,1=错误
}

u32 RePlace_BLOCK_ADDR(u32 ADDR)//取替换块地址
{
	u32 i;
	u8 *p8EEPROM;
	
	p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
	i=ADDR/(128*1024);//当前块计数

	i=p8EEPROM[i];
	if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
	{//有替换
		i--;
		ADDR=((NAND_FILE_BLOCK_COUNT+i)*128*1024)+(ADDR&0x1ffff);
	}
	return ADDR;
}

u32 RePlace_Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//NAND_FLASH 有替换读;返回:0=没错误,1=错误
{
	u32 i;
	u32 BlockByte;
	u32 Err;
	
	Err=0;
	while(Byte!=0)
	{
		BlockByte=(128*1024)-(ADDR_Sour&0x1FFFF);//当前Page要读字节
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		i=RePlace_BLOCK_ADDR(ADDR_Sour);
		i=Read_NAND(ADDR_Dest,i,BlockByte);//NAND_FLASH 没替换读
		if(i>Err)
		{
			Err=i;//返回:0=没错误,1=1位错误,2=无法校正的2位以上错误
		}
		ADDR_Dest+=BlockByte;
		ADDR_Sour+=BlockByte;
		Byte-=BlockByte;
	}
	return Err;
}

void RePlace_BLOCK_Write_NAND(u32 ADDR_Dest)//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
{
	u32 i;
	u32 x;
	u32 BLOCK_COUNT;
	u32 RePlace_BLOCK_COUNT;
	u8 *p8EEPROM;
	
	p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
	BLOCK_COUNT=ADDR_Dest/(128*1024);//当前块计数
	
	if(BLOCK_COUNT<(NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT))
	{//是文件系统块或用于替换块的块;测试时才操作这些块,不需替换,只需标志错误
		//用本身块
		x=BLOCK_Write_NAND(ADDR_Dest);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
		if(x==0)
		{//正确
			MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM清除
		}
		else
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM清除
		}
		return;
	}
	//若有块替换,则用替换块
	x=ADDR_Dest;
	i=p8EEPROM[BLOCK_COUNT];
	if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
	{//有替换
		i--;
		x=((NAND_FILE_BLOCK_COUNT+i)*128*1024);
		i++;
	}
	x=BLOCK_Write_NAND(x);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
	if(x==0)
	{//正确
		if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
		{//有替换,替换块已用标注
			i--;
			i+=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT;
			p8EEPROM=(u8*)(i-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
			if(p8EEPROM[0]!=0xFD)
			{
				MC(0xFD,i,1);//EEPROM清除
			}
		}
		else
		{
			i=ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT;
			p8EEPROM=(u8*)(i-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
			if(p8EEPROM[0]!=0xFD)
			{
				MC(0xFD,i,1);//EEPROM清除
			}
		}
	}
	else
	{//错误
		if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
		{//有替换,替换块也错误
			i--;
			i+=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT;
			MC(0xFE,i,1);//EEPROM清除
		}
		//找空的替换块写
		RePlace_BLOCK_COUNT=0;
		while(1)
		{
			p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP+NAND_FILE_BLOCK_COUNT;
			for(;RePlace_BLOCK_COUNT<NAND_REPLACE_BLOCK_COUNT;RePlace_BLOCK_COUNT++)
			{
				x=p8EEPROM[RePlace_BLOCK_COUNT];//替换用的块只有2个表示值0xFD和0xFE否则表示未初始化
				if(x!=0xFD)
				{//找到空或原已标注为错误的块
					break;
				}
			}
			if(RePlace_BLOCK_COUNT<NAND_REPLACE_BLOCK_COUNT)
			{//替换块还有空
				x=((NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT)*128*1024);
				x=BLOCK_Write_NAND(x);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
				if(x==0)
				{//正确
					//写替换块号
					MC(RePlace_BLOCK_COUNT+1,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM清除
					//替换块已用标注
					i=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT;
					MC(0xFD,i,1);//EEPROM清除
					break;
				}
				//替换再次错
				i=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT;
				MC(0xFE,i,1);//EEPROM清除
				RePlace_BLOCK_COUNT++;
			}
			else
			{//替换块已没空,用本身块写
				x=BLOCK_Write_NAND(ADDR_Dest);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
				if(x==0)
				{//正确
					MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM清除
				}
				else
				{//错误
					MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM清除
				}
				return;
			}
		}
	}
}


void RePlace_Write_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//NAND_FLASH写;若错误则块替换
{
	u32 i;
	u32 BlockByte;
	u8 *pSOUR;
	u8 *pBUFF;
	u32 Same;
	
	pSOUR=(u8*)ADDR_Sour;
	while(Byte!=0)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		//读原128K块数据
		RePlace_Read_NAND(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//NAND_FLASH S34MLxxG2有替换读
		//BUFF中更新数据同时比较
		BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		pBUFF=(u8*)(ADDR_128KWRITE_BUFF);
		pBUFF+=(ADDR_Dest&0x1ffff);//偏移
		Same=1;//相同
		for(i=0;i<BlockByte;i++)
		{
			if(*pBUFF!=*pSOUR)
			{
				Same=0;//不相同
				for(;i<BlockByte;i++)
				{
					*pBUFF=*pSOUR;
					pSOUR++;
					pBUFF++;
				}
				break;
			}
			pSOUR++;
			pBUFF++;
		}
		if(Same==0)
		{
			RePlace_BLOCK_Write_NAND(ADDR_Dest);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
		}
		ADDR_Dest+=BlockByte;
		Byte-=BlockByte;
	}
}

void RePlace_Clr_NAND(u32 Data,u32 ADDR_Dest,u32 Byte)//NAND_FLASH清0;若错误则块替换
{
	u32 i;
	u32 BlockByte;
	u8 *p8BUFF;
	u32 Same;
	
	while(Byte!=0)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		//读原128K块数据
		RePlace_Read_NAND(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//NAND_FLASH S34MLxxG2有替换读
		//更新数据同时比较
		BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前Back能写字节
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		p8BUFF=(u8*)(ADDR_128KWRITE_BUFF);
		p8BUFF+=(ADDR_Dest&0x1ffff);//偏移
		Same=1;//相同
		for(i=0;i<BlockByte;i++)
		{
			if(*p8BUFF!=Data)
			{
				Same=0;//不相同
				for(;i<BlockByte;i++)
				{
					*p8BUFF=Data;
					p8BUFF++;
				}
				break;
			}
			p8BUFF++;
		}
		if(Same==0)
		{
			RePlace_BLOCK_Write_NAND(ADDR_Dest);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
		}
		ADDR_Dest+=BlockByte;
		Byte-=BlockByte;
	}
}

void RePlace_BlockErase_NAND(u32 ADDR_Dest,u32 Byte)//NAND_FLASH 块擦除;有块替换
{
	u32 i;
	
	while(Byte)
	{
		i=RePlace_BLOCK_ADDR(ADDR_Dest);
		BlockErase_NAND(i,1);//NAND_FLASH 块擦除;没块替换
		if(Byte>=(128*1024))
		{
			ADDR_Dest+=(128*1024);
			Byte-=(128*1024);
		}
		else
		{
			Byte=0;
		}
	}
}



//NAND_BUFF
#if NAND128kBuff==0//定义128k nand写缓冲
u32 Check_4kNandBuff(void)//检查4K(BKPSRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
{
	u32 i;
	u32 x;
	u32 Block;
	u32 Byte;
	u32 LENmax;
	u16 *p16;
	u32* p32;
	u8 *p832;
	u32 Err;
//	u8 *p8;
//	u32 ADDR_BASE;
	
				
//---掉电保持测试---------------------
/*
	p8=(u8*)ADDR_Characteristics_NO;
	i=p8[0];
	if(i>2)
	{
		i=0;
	}
	ADDR_BASE=ADDR_Characteristics_BASE1+(i*LEN_BASE_Characteristics);
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_POWER_DOWN_Characteristics;
	if(p8[0]==0x55)//1BYTE 4K(BKPSRAM)掉电保持,0=OK,1=ERR,0xff=没测试,0x55=进行测试
	{
		p32=(u32*)(ADDR_BKPSRAM);
		for(i=0;i<(4096/4);i++)
		{
			if(p32[i]!=i)
			{
				break;
			}
		}
		if(i<((4096-4)/4))
		{//错误
			MWR(1,ADDR_BASE+(OFFSET_POWER_DOWN_Characteristics),1);
			p8=(u8*)ADDR_BASE+(OFFSET_TestResult_Characteristics);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
			if(p8[0]==0)
			{
				MWR(1,ADDR_BASE+(OFFSET_TestResult_Characteristics),1);
			}
		}
		else
		{
			MWR(0,ADDR_BASE+(OFFSET_POWER_DOWN_Characteristics),1);
		}
	}
*/
//---掉电保持测试End-------------------------
	
//检查
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
		p832=(u8*)(ADDR_BKPSRAM+Byte);
		p16=(u16*)(ADDR_BKPSRAM+Byte+4);
		x=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		if((x<0x30000000)||(x>0xA0000000)||(p16[1]>p16[0]))
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
	return Err;//0=错误,1=正确
}
void NR_4kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//读NAND_FLASH的写入缓冲
{
	u32 i;
	u32 x;
	u32 ReadByte;
	u32 ADDR_BUFF;
	u32 TBlock;
	u8 *p816;
	u8 *p832;
	u32* p32;
	u8 *p8s;
	u8 *p8d;
	
	p816=(u8*)(ADDR_BKPSRAM);
	TBlock=p816[0]|(p816[1]<<8);
	ADDR_BUFF=ADDR_BKPSRAM+4;
	while(TBlock--)
	{
		if(ADDR_BUFF>=(ADDR_BKPSRAM+4096))
		{//块数与长度不配错误
			p32=(u32*)(ADDR_BKPSRAM);
			p32[0]=0x40000;//初始为总块数=0,总字节数=4
			p32=(u32*)(ADDR_BKPSRAM+(4096-4));
			p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM中数据CRC32计算
			return;
		}
		p832=(u8*)(ADDR_BUFF);
		p816=(u8*)(ADDR_BUFF+4);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p816[0]|(p816[1]<<8);
		ReadByte=0;
		//BUFF中数据开始地址>=要读的NAND开始地址
		if((i>=ADDR_Sour)&&(i<(ADDR_Sour+Byte)))
		{
			p8s=(u8*)ADDR_BUFF+8;
			p8d=(u8*)ADDR_Dest+(i-ADDR_Sour);
			ReadByte=(ADDR_Sour+Byte)-i;//读字节数
			if(ReadByte>x)
			{
				ReadByte=x;//缓冲中字节数
			}
		}
		else
		{
		//BUFF中数据开始地址<要读的NAND开始地址
			if((ADDR_Sour>=i)&&(ADDR_Sour<(i+x)))
			{
				p8s=(u8*)ADDR_BUFF+8+(ADDR_Sour-i);
				p8d=(u8*)ADDR_Dest;
				ReadByte=x-(ADDR_Sour-i);//缓冲中字节数
				if(ReadByte>Byte)
				{
					ReadByte=Byte;//读字节数
				}
			}
		}
		if((((u32)p8s)+ReadByte)<=(ADDR_BKPSRAM+4096))
		{
			for(i=0;i<ReadByte;i++)
			{
				p8d[i]=p8s[i];
			}
		}
		ADDR_BUFF+=8+x;
	}
}
void NB_4kBuff(u32 ADDR_Dest,u32 Byte)//擦除NAND_FLASH时同时更新写入缓冲
{
	u32 i;
	u32 x;
	u32 WriteByte;
	u32 ADDR_BUFF;
	u32 TBlock;
	u8 *p816;
	u8 *p832;
	u32* p32;
	u8 *p8d;
	
	p816=(u8*)(ADDR_BKPSRAM);
	TBlock=p816[0]|(p816[1]<<8);
	ADDR_BUFF=ADDR_BKPSRAM+4;
	while(TBlock--)
	{
		p832=(u8*)(ADDR_BUFF);
		p816=(u8*)(ADDR_BUFF+4);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p816[0]|(p816[1]<<8);
		WriteByte=0;
		//BUFF中数据开始地址>=要读的NAND开始地址
		if((i>=ADDR_Dest)&&(i<(ADDR_Dest+Byte)))
		{
			p8d=(u8*)ADDR_BUFF+8;
			WriteByte=Byte-(i-ADDR_Dest);//写字节数
			if(WriteByte>x)
			{
				WriteByte=x;//缓冲中字节数
			}
		}
		else
		{
		//BUFF中数据开始地址<要读的NAND开始地址
			if((ADDR_Dest>=i)&&(ADDR_Dest<(i+x)))
			{
				p8d=(u8*)ADDR_BUFF+8+(ADDR_Dest-i);
				WriteByte=x-(ADDR_Dest-i);//缓冲中字节数
				if(WriteByte>Byte)
				{
					WriteByte=Byte;//写字节数
				}
			}
		}
		for(i=0;i<WriteByte;i++)
		{
			p8d[i]=0xff;
		}
		ADDR_BUFF+=8+x;
	}
	p816=(u8*)(ADDR_BKPSRAM+2);
	Byte=p816[0]|(p816[1]<<8);
	if(Byte<=(4096-4))
	{
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM中数据CRC32计算
	}
}

void NAND4kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0)//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
{
	u32 i;
	u32 x;
	u32 n;
	u32 TBlock;
	u32 ADDR_SRAM;
	u32 ADDR_Dest;
	u32 ADDR_DestX;
	u32 BlockByte;
	u8 *p816;
	u8 *p832;
	u8 *p8s;
	u8 *p8d;
	u32* p32;
	
	p816=(u8*)(ADDR_BKPSRAM);
	TBlock=p816[0]|(p816[1]<<8);
	if(TBlock==0)
	{
		if(Byte0!=0)
		{//至少1个块
			TBlock=1;
		//数据块1
		//4BYTE 写入NAND FLASH的目的地址
		//2BYTE 数据字节数
		//2BYTE 已部分写入的字节数
			p32=(u32*)(ADDR_BKPSRAM+4);
			p32[0]=0;
			p32[1]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSRAM+4;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
		{//掉电
			if(Byte0==0)
			{
				p816=(u8*)(ADDR_BKPSRAM+2);
				x=p816[0]|(p816[1]<<8);
				if(x<=(4096-4))
				{
					p32=(u32*)(ADDR_BKPSRAM+(4096-4));
					p32[0]=RAM_CRC32(ADDR_BKPSRAM,x);//RAM中数据CRC32计算
				}
				YAFFS_FMI_SET();//nand FMI在yaffs中的配置
				return;//暂停写
			}
		}
		p816=(u8*)(ADDR_SRAM+4);
		n=p816[0]|(p816[1]<<8);//还需写字节
		n-=p816[2]|(p816[3]<<8);
		if(n|Byte0)
		{//有字节需写入
			p832=(u8*)(ADDR_SRAM);
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//开始地址+已写字节
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>n)
					{
						BlockByte=n;
					}
					p8s=(u8*)ADDR_SRAM+8+(p816[2]|(p816[3]<<8));
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					i=p816[2]|(p816[3]<<8);//已写字节数
					i+=BlockByte;
					p816[2]=i;
					p816[3]=i>>8;
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//下块
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					p816=(u8*)(ADDR_SRAM+4);
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//开始地址+已写字节
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//块地址相同
						n=p816[0]|(p816[1]<<8);//还需写字节
						n-=p816[2]|(p816[3]<<8);
						if(n)
						{//有字节需写入
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//当前块能写字节
							if(BlockByte>n)
							{
								BlockByte=n;
							}
							p8s=(u8*)ADDR_SRAM+8+(p816[2]|(p816[3]<<8));
							p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_DestX&0x1ffff);
							for(n=0;n<BlockByte;n++)
							{
								p8d[n]=p8s[n];
							}
							i=p816[2]|(p816[3]<<8);//已写字节数
							i+=BlockByte;
							p816[2]=i;
							p816[3]=i>>8;
						}
					}
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//下块
				}
				//最后数据最后更新
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>Byte0)
					{
						BlockByte=Byte0;
					}
					p8s=(u8*)ADDR_Sour0;
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					ADDR_Sour0+=BlockByte;
					ADDR_Dest0+=BlockByte;
					Byte0-=BlockByte;//已写字节数
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
				ADDR_SRAM=ADDR_BKPSRAM+4;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;
	}
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=0x40000;//初始为总块数=0,总字节数=4
	p32=(u32*)(ADDR_BKPSRAM+(4096-4));
	p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM中数据CRC32计算
	YAFFS_FMI_SET();//nand FMI在yaffs中的配置
}

void NAND4kBuff_Clr_NAND(u32 Data0,u32 ADDR_Dest0,u32 Byte0)//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
{
	u32 i;
	u32 x;
	u32 n;
	u32 TBlock;
	u32 ADDR_SRAM;
	u32 ADDR_Dest;
	u32 ADDR_DestX;
	u32 BlockByte;
	u8 *p816;
	u8 *p832;
	u8 *p8s;
	u8 *p8d;
	u32* p32;
	
	p816=(u8*)(ADDR_BKPSRAM);
	TBlock=p816[0]|(p816[1]<<8);
	if(TBlock==0)
	{
		if(Byte0!=0)
		{//至少1个块
			TBlock=1;
		//数据块1
		//4BYTE 写入NAND FLASH的目的地址
		//2BYTE 数据字节数
		//2BYTE 已部分写入的字节数
			p32=(u32*)(ADDR_BKPSRAM+4);
			p32[0]=0;
			p32[1]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSRAM+4;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
		{//掉电
			if(Byte0==0)
			{
				p816=(u8*)(ADDR_BKPSRAM+2);
				x=p816[0]|(p816[1]<<8);
				if(x<=(4096-4))
				{
					p32=(u32*)(ADDR_BKPSRAM+(4096-4));
					p32[0]=RAM_CRC32(ADDR_BKPSRAM,x);//RAM中数据CRC32计算
				}
				YAFFS_FMI_SET();//nand FMI在yaffs中的配置
				return;//暂停写
			}
		}
		p816=(u8*)(ADDR_SRAM+4);
		n=p816[0]|(p816[1]<<8);//还需写字节
		n-=p816[2]|(p816[3]<<8);
		if(n|Byte0)
		{//有字节需写入
			p832=(u8*)(ADDR_SRAM);
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//开始地址+已写字节
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>n)
					{
						BlockByte=n;
					}
					p8s=(u8*)ADDR_SRAM+8+(p816[2]|(p816[3]<<8));
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					i=p816[2]|(p816[3]<<8);//已写字节数
					i+=BlockByte;
					p816[2]=i;
					p816[3]=i>>8;
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//下块
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					p816=(u8*)(ADDR_SRAM+4);
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//开始地址+已写字节
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//块地址相同
						n=p816[0]|(p816[1]<<8);//还需写字节
						n-=p816[2]|(p816[3]<<8);
						if(n)
						{//有字节需写入
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//当前块能写字节
							if(BlockByte>n)
							{
								BlockByte=n;
							}
							p8s=(u8*)ADDR_SRAM+8+(p816[2]|(p816[3]<<8));
							p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_DestX&0x1ffff);
							for(n=0;n<BlockByte;n++)
							{
								p8d[n]=p8s[n];
							}
							i=p816[2]|(p816[3]<<8);//已写字节数
							i+=BlockByte;
							p816[2]=i;
							p816[3]=i>>8;
						}
					}
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//下块
				}
				//最后数据最后更新
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>Byte0)
					{
						BlockByte=Byte0;
					}
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=Data0;
					}
					ADDR_Dest0+=BlockByte;
					Byte0-=BlockByte;//已写字节数
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
				ADDR_SRAM=ADDR_BKPSRAM+4;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;
	}
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=0x40000;//初始为总块数=0,总字节数=4
	p32=(u32*)(ADDR_BKPSRAM+(4096-4));
	p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM中数据CRC32计算
	YAFFS_FMI_SET();//nand FMI在yaffs中的配置
}

void NW_4kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//写4k Nand FLASH缓冲
{
	u32 i;
	u8 *p8s;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	
	p816=(u8*)(ADDR_BKPSRAM);
	i=p816[2]|(p816[3]<<8);
	if((i+8+Byte)>4096)
	{//缓冲区不够
		NAND4kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
		return;
	}
	i=p816[0]|(p816[1]<<8);
	if(i>=BlockMax_NandBuff)
	{//总块数 块为不同nand块时64*20ms=1280ms
		NAND4kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
		return;
	}
	p832=(u8*)(ADDR_BKPSRAM+(p816[2]|(p816[3]<<8)));
	p832[0]=ADDR_Dest;
	p832[1]=ADDR_Dest>>8;
	p832[2]=ADDR_Dest>>16;
	p832[3]=ADDR_Dest>>24;
	p832[4]=Byte;
	p832[5]=Byte>>8;
	p832[6]=Byte>>16;
	p832[7]=Byte>>24;
	p8s=(u8*)ADDR_Sour;
	p8d=(u8*)(ADDR_BKPSRAM+(p816[2]|(p816[3]<<8))+8);
	for(i=0;i<Byte;i++)
	{
		p8d[i]=p8s[i];
	}
	i=p816[0]|(p816[1]<<8);
	i++;
	Byte+=(p816[2]|(p816[3]<<8))+8;
	i|=(Byte<<16);
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=i;//块数和总字节数需32位一起写入，避免掉电中断等引起不同步!
	
	if(Byte<=(4096-4))
	{
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM中数据CRC32计算
	}
}

void NC_4kBuff(u32 Data,u32 ADDR_Dest,u32 Byte)//清除写入Nand FLASH的数据先到4kBuff
{
	u32 i;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	
	p816=(u8*)(ADDR_BKPSRAM);
	i=p816[2]|(p816[3]<<8);
	if((i+8+Byte)>4096)
	{//缓冲区不够
		NAND4kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
		return;
	}
	i=p816[0]|(p816[1]<<8);
	if(i>=BlockMax_NandBuff)
	{//总块数 块为不同nand块时64*20ms=1280ms
		NAND4kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
		return;
	}
	p832=(u8*)(ADDR_BKPSRAM+(p816[2]|(p816[3]<<8)));
	p832[0]=ADDR_Dest;
	p832[1]=ADDR_Dest>>8;
	p832[2]=ADDR_Dest>>16;
	p832[3]=ADDR_Dest>>24;
	p832[4]=Byte;
	p832[5]=Byte>>8;
	p832[6]=Byte>>16;
	p832[7]=Byte>>24;
	p8d=(u8*)(ADDR_BKPSRAM+(p816[2]|(p816[3]<<8))+8);
	for(i=0;i<Byte;i++)
	{
		p8d[i]=Data;
	}
	i=p816[0]|(p816[1]<<8);
	i++;
	Byte+=(p816[2]|(p816[3]<<8))+8;
	i|=(Byte<<16);
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=i;//块数和总字节数需32位一起写入，避免掉电中断等引起不同步!
	
	if(Byte<=(4096-4))
	{
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM中数据CRC32计算
	}
}
#else//#if NAND128kBuff==0//定义128k nand写缓冲

u32 Check_128kNandBuff(void)//检查128K(BKPSDRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
{
	u32 i;
	u32 x;
	u32 y;
	u32 Block;
	u32 Byte;
	u32 LENmax;
	u8 *p816;
	u8 *p832;
	u32 Err;
	u32* p32;
	
	Err=0;
	p816=(u8*)(ADDR_BKPSDRAM);
	p832=(u8*)(ADDR_BKPSDRAM+2);
	LENmax=(128*1024)-6-12;
	i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
	if((i>(128*1024))||(i<6))
	{//错,总字节数(全部字节,也即下块的开始地址)
		Err=1;
	}
	Block=p816[0]|(p816[1]<<8);
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
		p832=(u8*)(ADDR_BKPSDRAM+Byte);
		x=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		if((x<0x30000000)||(x>0xA0000000))
		{//错
			Err=1;
			break;
		}
		x=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		y=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
		if(y>x)
		{
			Err=1;
			break;
		}
		if(x>LENmax)
		{
			Err=1;
			break;
		}
		Byte+=12+x;
		LENmax-=12+x;
	}
	p832=(u8*)(ADDR_BKPSDRAM+2);
	i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
	if(Byte!=i)
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
	return Err;//0=错误,1=正确
}
void NR_128kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//读NAND_FLASH的写入缓冲
{
	u32 i;
	u32 x;
	u32 ReadByte;
	u32 ADDR_BUFF;
	u32 TBlock;
	u8 *p832;
	u8 *p8s;
	u8 *p8d;
	u32* p32;
	
	p832=(u8*)(ADDR_BKPSDRAM);
	TBlock=p832[0]|(p832[1]<<8);
	ADDR_BUFF=ADDR_BKPSDRAM+6;
	while(TBlock--)
	{
		if(ADDR_BUFF>=(ADDR_BKPSDRAM+(128*1024)))
		{//块数与长度不配错误
			p32=(u32*)(ADDR_BKPSDRAM);
			p32[0]=0x60000;//初始为总块数=0,总字节数=6
			p32[1]=0;
			p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
			p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM中数据CRC32计算
			return;
		}
		p832=(u8*)(ADDR_BUFF);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		ReadByte=0;
		//BUFF中数据开始地址>=要读的NAND开始地址
		if((i>=ADDR_Sour)&&(i<(ADDR_Sour+Byte)))
		{
			p8s=(u8*)ADDR_BUFF+12;
			p8d=(u8*)ADDR_Dest+(i-ADDR_Sour);
			ReadByte=(ADDR_Sour+Byte)-i;//读字节数
			if(ReadByte>x)
			{
				ReadByte=x;//缓冲中字节数
			}
		}
		else
		{
		//BUFF中数据开始地址<要读的NAND开始地址
			if((ADDR_Sour>=i)&&(ADDR_Sour<(i+x)))
			{
				p8s=(u8*)ADDR_BUFF+12+(ADDR_Sour-i);
				p8d=(u8*)ADDR_Dest;
				ReadByte=x-(ADDR_Sour-i);//缓冲中字节数
				if(ReadByte>Byte)
				{
					ReadByte=Byte;//读字节数
				}
			}
		}
		if((((u32)p8s)+ReadByte)<=(ADDR_BKPSDRAM+(128*1024)))
		{
			for(i=0;i<ReadByte;i++)
			{
				p8d[i]=p8s[i];
			}
		}
		ADDR_BUFF+=12+x;
	}
}

void NB_128kBuff(u32 ADDR_Dest,u32 Byte)//擦除NAND_FLASH时同时更新写入缓冲
{
	u32 i;
	u32 x;
	u32 WriteByte;
	u32 ADDR_BUFF;
	u32 TBlock;
	u8 *p832;
	u8 *p8d;
	u32* p32;
	
	p832=(u8*)(ADDR_BKPSDRAM);
	TBlock=p832[0]|(p832[1]<<8);
	ADDR_BUFF=ADDR_BKPSDRAM+6;
	while(TBlock--)
	{
		p832=(u8*)(ADDR_BUFF);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		WriteByte=0;
		//BUFF中数据开始地址>=要读的NAND开始地址
		if((i>=ADDR_Dest)&&(i<(ADDR_Dest+Byte)))
		{
			p8d=(u8*)ADDR_BUFF+12;
			WriteByte=Byte-(i-ADDR_Dest);//写字节数
			if(WriteByte>x)
			{
				WriteByte=x;//缓冲中字节数
			}
		}
		else
		{
		//BUFF中数据开始地址<要读的NAND开始地址
			if((ADDR_Dest>=i)&&(ADDR_Dest<(i+x)))
			{
				p8d=(u8*)ADDR_BUFF+12+(ADDR_Dest-i);
				WriteByte=x-(ADDR_Dest-i);//缓冲中字节数
				if(WriteByte>Byte)
				{
					WriteByte=Byte;//写字节数
				}
			}
		}
		for(i=0;i<WriteByte;i++)
		{
			p8d[i]=0xff;
		}
		ADDR_BUFF+=12+x;
	}
	
	p832=(u8*)(ADDR_BKPSDRAM+2);
	Byte=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
	if(Byte<=((128*1024)-4))
	{
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		if(Byte>(4096-4))
		{
			Byte=(4096-4);
		}
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM中数据CRC32计算
	}
}

void NAND128kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0)//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
{
	u32 i;
	u32 x;
	u32 n;
	u32 TBlock;
	u32 ADDR_SRAM;
	u32 ADDR_Dest;
	u32 ADDR_DestX;
	u32 BlockByte;
	u8 *p832;
	u8 *p8s;
	u8 *p8d;
	u32* p32;
	
	p832=(u8*)(ADDR_BKPSDRAM);
	TBlock=p832[0]|(p832[1]<<8);
	if(TBlock==0)
	{
		if(Byte0!=0)
		{//至少1个块
			TBlock=1;
		//数据块1
		//4BYTE 写入NAND FLASH的目的地址
		//4BYTE 数据字节数
		//4BYTE 已部分写入的字节数
			p32=(u32*)(ADDR_BKPSDRAM+6);
			p32[0]=0;
			p32[1]=0;
			p32[2]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSDRAM+6;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
		{//掉电
			if(Byte0==0)
			{
				p832=(u8*)(ADDR_BKPSDRAM+2);
				x=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
				if(x<=((128*1024)-4))
				{
					p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
					if(x>(4096-4))
					{
						x=(4096-4);
					}
					p32[0]=RAM_CRC32(ADDR_BKPSDRAM,x);//RAM中数据CRC32计算
				}
				YAFFS_FMI_SET();//nand FMI在yaffs中的配置
				return;//暂停写
			}
		}
		p832=(u8*)(ADDR_SRAM);
		//n=p32[1]-p32[2];//还需写字节
		n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
		if(n|Byte0)
		{//有字节需写入
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24)) + (p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//开始地址+已写字节
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>n)
					{
						BlockByte=n;
					}
					p8s=(u8*)ADDR_SRAM+12+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					i=(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
					i+=BlockByte;//已写字节数
					p832[8]=i;
					p832[9]=i>>8;
					p832[10]=i>>16;
					p832[11]=i>>24;
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//下块
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24)) + (p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//开始地址+已写字节
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//块地址相同
						//n=p32[1]-p32[2];//还需写字节
						n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
						n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
						if(n)
						{//有字节需写入
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//当前块能写字节
							if(BlockByte>n)
							{
								BlockByte=n;
							}
							p8s=(u8*)ADDR_SRAM+12+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
							p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_DestX&0x1ffff);
							for(n=0;n<BlockByte;n++)
							{
								p8d[n]=p8s[n];
							}
							i=(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
							i+=BlockByte;//已写字节数
							p832[8]=i;
							p832[9]=i>>8;
							p832[10]=i>>16;
							p832[11]=i>>24;
						}
					}
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//下块
				}
				//最后数据最后更新
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>Byte0)
					{
						BlockByte=Byte0;
					}
					p8s=(u8*)ADDR_Sour0;
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					ADDR_Sour0+=BlockByte;
					ADDR_Dest0+=BlockByte;
					Byte0-=BlockByte;//已写字节数
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
				ADDR_SRAM=ADDR_BKPSDRAM+6;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;
	}
	p32=(u32*)(ADDR_BKPSDRAM);
	p32[0]=0x60000;//初始为总块数=0,总字节数=6
	p32[1]=0;
	p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
	p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM中数据CRC32计算
	YAFFS_FMI_SET();//nand FMI在yaffs中的配置
}

void NAND128kBuff_Clr_NAND(u32 Data0,u32 ADDR_Dest0,u32 Byte0)//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
{
	u32 i;
	u32 x;
	u32 n;
	u32 TBlock;
	u32 ADDR_SRAM;
	u32 ADDR_Dest;
	u32 ADDR_DestX;
	u32 BlockByte;
	u8 *p832;
	u8 *p8s;
	u8 *p8d;
	u32* p32;
	
	p832=(u8*)(ADDR_BKPSDRAM);
	TBlock=p832[0]|(p832[1]<<8);
	if(TBlock==0)
	{
		if(Byte0!=0)
		{//至少1个块
			TBlock=1;
		//数据块1
		//4BYTE 写入NAND FLASH的目的地址
		//2BYTE 数据字节数
		//2BYTE 已部分写入的字节数
			p32=(u32*)(ADDR_BKPSDRAM+6);
			p32[0]=0;
			p32[1]=0;
			p32[2]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSDRAM+6;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
		{//掉电
			if(Byte0==0)
			{
				p832=(u8*)(ADDR_BKPSDRAM+2);
				x=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
				if(x<=((128*1024)-4))
				{
					p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
					if(x>(4096-4))
					{
						x=(4096-4);
					}
					p32[0]=RAM_CRC32(ADDR_BKPSDRAM,x);//RAM中数据CRC32计算
				}
				YAFFS_FMI_SET();//nand FMI在yaffs中的配置
				return;//暂停写
			}
		}
		p832=(u8*)(ADDR_SRAM);
		//n=p32[1]-p32[2];//还需写字节
		n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
		if(n|Byte0)
		{//有字节需写入
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//开始地址+已写字节
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//读整块128K数据
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>n)
					{
						BlockByte=n;
					}
					p8s=(u8*)ADDR_SRAM+12+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=p8s[n];
					}
					i=(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
					i+=BlockByte;//已写字节数
					p832[8]=i;
					p832[9]=i>>8;
					p832[10]=i>>16;
					p832[11]=i>>24;
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//下块
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//开始地址+已写字节
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//块地址相同
						//n=p32[1]-p32[2];//还需写字节
						n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
						n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
						if(n)
						{//有字节需写入
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//当前块能写字节
							if(BlockByte>n)
							{
								BlockByte=n;
							}
							p8s=(u8*)ADDR_SRAM+12+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
							p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_DestX&0x1ffff);
							for(n=0;n<BlockByte;n++)
							{
								p8d[n]=p8s[n];
							}
							i=(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));
							i+=BlockByte;//已写字节数
							p832[8]=i;
							p832[9]=i>>8;
							p832[10]=i>>16;
							p832[11]=i>>24;
						}
					}
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//下块
				}
				//最后数据最后更新
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//当前块能写字节
					if(BlockByte>Byte0)
					{
						BlockByte=Byte0;
					}
					p8d=(u8*)ADDR_128KWRITE_BUFF+(ADDR_Dest&0x1ffff);
					for(n=0;n<BlockByte;n++)
					{
						p8d[n]=Data0;
					}
					ADDR_Dest0+=BlockByte;
					Byte0-=BlockByte;//已写字节数
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//有替换块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较若错误则用替换块
				ADDR_SRAM=ADDR_BKPSDRAM+6;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;
	}
	p32=(u32*)(ADDR_BKPSDRAM);
	p32[0]=0x60000;//初始为总块数=0,总字节数=6
	p32[1]=0;
	p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
	p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM中数据CRC32计算
	YAFFS_FMI_SET();//nand FMI在yaffs中的配置
}
void NW_128kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//写入Nand FLASH的数据先到128kBuff
{
	u32 i;
	u8 *p8s;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	u32 irq;
	
	p816=(u8*)(ADDR_BKPSDRAM);
	p832=(u8*)(ADDR_BKPSDRAM+2);
	i=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24));//总字节数
	if((i+12+Byte)>(128*1024))
	{//缓冲区不够
		NAND128kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
		return;
	}
	if((p816[0]|(p816[1]<<8))>=BlockMax_NandBuff)
	{//总块数 块为不同nand块时64*20ms=1280ms
		NAND128kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
		return;
	}
	p832=(u8*)(ADDR_BKPSDRAM+i);
	p832[0]=ADDR_Dest;
	p832[1]=ADDR_Dest>>8;
	p832[2]=ADDR_Dest>>16;
	p832[3]=ADDR_Dest>>24;
	p832[4]=Byte;
	p832[5]=Byte>>8;
	p832[6]=Byte>>16;
	p832[7]=Byte>>24;	
	p832[8]=0;
	p832[9]=0;
	p832[10]=0;
	p832[11]=0;
	p8s=(u8*)ADDR_Sour;
	p8d=(u8*)(ADDR_BKPSDRAM+i+12);
	for(i=0;i<Byte;i++)
	{
		p8d[i]=p8s[i];
	}
	i=p816[0]|(p816[1]<<8);
	i++;
	p832=(u8*)(ADDR_BKPSDRAM+2);
	Byte+=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+12;
	//块数和总字节数需32位一起写入，避免掉电中断等引起不同步!
	irq=off_irq();//关IRQ中断,返回:关闭前允许状态
	p816[0]=i;
	p816[1]=i>>8;
	p832[0]=Byte;
	p832[1]=Byte>>8;
	p832[2]=Byte>>16;
	p832[3]=Byte>>24;
	destore_irq(irq);//恢复IRQ中断,入口:关闭前允许状态
	if(Byte<=((128*1024)-4))
	{
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		if(Byte>(4096-4))
		{
			Byte=(4096-4);
		}
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM中数据CRC32计算
	}
}
void NC_128kBuff(u32 Data,u32 ADDR_Dest,u32 Byte)//清除写入Nand FLASH的数据先到128kBuff
{
	u32 i;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	u32 irq;
	
	p816=(u8*)(ADDR_BKPSDRAM);
	p832=(u8*)(ADDR_BKPSDRAM+2);
	i=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24));//总字节数
	if((i+12+Byte)>(128*1024))
	{//缓冲区不够
		NAND128kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
		return;
	}
	if((p816[0]|(p816[1]<<8))>=BlockMax_NandBuff)
	{//总块数 块为不同nand块时64*20ms=1280ms
		NAND128kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
		return;
	}
	p832=(u8*)(ADDR_BKPSDRAM+i);
	p832[0]=ADDR_Dest;
	p832[1]=ADDR_Dest>>8;
	p832[2]=ADDR_Dest>>16;
	p832[3]=ADDR_Dest>>24;
	p832[4]=Byte;
	p832[5]=Byte>>8;
	p832[6]=Byte>>16;
	p832[7]=Byte>>24;	
	p832[8]=0;
	p832[9]=0;
	p832[10]=0;
	p832[11]=0;
	p8d=(u8*)(ADDR_BKPSDRAM+i+12);
	for(i=0;i<Byte;i++)
	{
		p8d[i]=Data;
	}
	i=p816[0]|(p816[1]<<8);
	i++;
	p832=(u8*)(ADDR_BKPSDRAM+2);
	Byte+=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+12;
	//块数和总字节数需32位一起写入，避免掉电中断等引起不同步!
	irq=off_irq();//关IRQ中断,返回:关闭前允许状态
	p816[0]=i;
	p816[1]=i>>8;
	p832[0]=Byte;
	p832[1]=Byte>>8;
	p832[2]=Byte>>16;
	p832[3]=Byte>>24;
	destore_irq(irq);//恢复IRQ中断,入口:关闭前允许状态
	if(Byte<=((128*1024)-4))
	{
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		if(Byte>(4096-4))
		{
			Byte=(4096-4);
		}
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM中数据CRC32计算
	}
}
#endif

//1st cycle A7-A0
//2nd cycle A11-A8
//3rd cycle A19-A12
//4th cycle A27-A20
//5th cycle A28















