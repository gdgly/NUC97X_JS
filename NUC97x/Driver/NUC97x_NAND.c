
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


#define BlockMax_NandBuff     128//nand������󻺳����ݿ���

void Init_NAND(void)//��λnand
{
	u32 i;
	u32 CS0_disable;
	u32 CS1_disable;
	u32 *p32;
	u32 irq;
	
//�O��CLK_HCLKEN�Ĵ���FMI��NANDλ
	p32=(u32*)(REG_CLK_HCLKEN);
	i=p32[0];
	i|=3<<20;
	p32[0]=i;
//AHB IP Reset
	p32=(u32*)(REG_SYS_AHBIPRST);
	p32[0]=1<<20;//FMI controller reset enabled.
	p32[0]=0;
	
//�x��๦�ܿ��ƣ�NAND�ж��M��GPC0~14��GPI1~15
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
		//This field controls the CLE/ALE setup/hold time to �CWE.
		//The setup/hold time can be calculated using following equation:
		//tCLS = (CALE_SH+1)*TAHB.
		//tCLH = ((CALE_SH*2)+2)*TAHB.
		//tALS = (CALE_SH+1)*TAHB.
		//tALH = ((CALE_SH*2)+2)*TAHB.
	(2<<16)|
	//[15:8] HI_WID
		//Read/Write Enable Signal High Pulse Width
		//This field controls the high pulse width of signals �CRE and �CWE while H/W mode page access
		//is enabled. The pulse width is a multiple of period of AHB bus clock. ( The actual width time
		//will be [clock period*(HI_WID+1)] )
	(3<<8)|
	//[7:0] LO_WID
		//Read/Write Enable Signal Low Pulse Width
		//This field controls the low pulse width of signals �CRE and �CWE while H/W mode page access
		//is enabled. The pulse width is a multiple of period of AHB bus clock. ( The actual width time
		//will be [clock period*(LO_WID+1)] )
	5;
	
//ʹ��FMI(Reset Value 0x0000_0000)
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
		//If PSIZE (FMI_NANDCTL[17:16]) = 2��b00, this field will be set 0x10 (16bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2��b01, this field will be set 0x40 (64bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2��b10, this field will be set 0x80 (128 bytes) automatically.
		//If PSIZE (FMI_NANDCTL[17:16]) = 2��b11, this field will be set 0x100 (256bytes) automatically.
		//Note: The REA128EN must be 4 byte aligned, so bit1 and bit0 can��t be filled 1 to it.
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
		//1 = Enable scatter-gather operation. DMAC will treat the starting address in FMI_DMASA as a starting address of Physical Address Descriptor (PAD) table. The format of these Pads�� will be described later.
	//[2]//Reserved.
	//[1]//SW_RST
		//Software Engine Reset
		//0 = Writing 0 to this bit has no effect.
		//1 = Writing 1 to this bit will reset the internal state machine and pointers. The contents of control register will not be cleared. This bit will auto clear after few clock cycles.
		//NOTE: The software reset DMA region.
	(1<<1)|
	//[0]//DMACEN
		//DMAC Engine Enable
		//Setting this bit to 1 enables DMAC��s operation. If this bit is cleared, DMAC will ignore all DMA request from FMI and force Bus Master into IDLE state.
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
		//1 = Enable. NAND controller will check 1��s count for byte 2, 3 of redundant data of the ECC in each field. If count value is greater than 8, NAND controller will treat this field as none used field; otherwise, it��s used. If that field is none used field, NAND controller will ignore its ECC check result.
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
		//This bit enables NAND controller to transfer data (1 page) from DMAC��s embedded frame buffer into NAND Flash or NAND type flash.
		//0 = No effect.
		//1 = Enable DMA write data transfer.
		//NOTE: When DMA transfer completed, this bit will be cleared automatically.
	//[1]DRD_EN
		//DMA Read Data Enable
		//This bit enables NAND controller to transfer data (1 page) from NAND Flash or NAND type flash into DMAC��s embedded frame buffer.
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
u32 Wait_NAND(u32 usMax)//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
{
	u32 i;
	u32 t;
	u32 *p32CMD;
	u32 *p32DATA;
	
	p32CMD=(u32*)(REG_NANDCMD);
	p32DATA=(u32*)(REG_NANDDATA);
	while(1)//�������ʱֵi=9;��ǰ���ֵ100
	{
//Status Register Definition
//SR    Program       Program Page   Page Read     Page Read     Block Erase    Description
//Bit		Page          Cache Mode                   Cache Mode
//7     Write protect Write protect  Write protect Write protect Write protect  0 = Protected,1 = Not protected
//6     RDY           RDY cache      RDY           RDY cache     RDY            0 = Busy,1 = Ready
//5     ARDY          ARDY           ARDY          ARDY          ARDY           Don't Care
//4     �C            �C             �C            �C            �C              Don't Care
//3     �C            �C             Rewrite       �C            �C              0 = Normal or uncorrectable,1 = Rewrite recommended
//                                   recommended
//2     �C            �C             �C            �C            �C              Don't Care
//1     FAILC(N-1)    FAILC(N-1)     Reserved      �C            �C              Don't Care
//0     FAIL          FAIL (N)       FAIL          �C            FAIL            0 = Successful PROGRAM/ERASE/READ,1 = Error in PROGRAM/ERASE/READ
		*p32CMD=0x70;
		i=*p32DATA;
		if((i&0x40)==0x40)
		{
			*p32CMD=0x00;//1st Cycle Page Read Command
			t=4;//������ʱtWHR=60ns
			while(t--);
			return 0;
		}
		SoftDelayUS(1);//���us��ʱ
		if(usMax==0)
		{
			break;
		}
		usMax--;
	}
	*p32CMD=0x00;//1st Cycle Page Read Command
	t=4;//������ʱtWHR=60ns
	while(t--);
	return i;
}
u64 ID_NAND(void)//������ID,ͬʱnand����λ����Զ�ʶ��
{
	u64 id;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32 *p32DATA;
	
	Init_NAND();//��λnand
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

	FileDownload->NAND_BUSBIT=8;//nand����λ����Զ�ʶ��:8=8bit,16=16bit
	return id;
}

void Enable_Internal_ECC(void)//����nand�ڲ�ECC
{
}
void Disable_Internal_ECC(void)//ȡ��nand�ڲ�ECC
{
}

void BlockErase_NAND(u32 ADDR_Dest,u32 Busy_Wait)//NAND_FLASH ������ǰ1����;û���滻;���:Busy_Wait=1 NANDæʱ�ȴ�Busy_Wait=0���ȴ�
{
	u32 ReNum;
	u32 Err;
	u32 *p32CMD;
	u32 *p32ADDR;
//	u32 *p32DATA;
	
	Init_NAND();//��λnand
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
		Err=Wait_NAND(3000*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		if(Err==0)
		{
			break;
		}
	}

}

u32 Read_Page(u32 ADDR_Dest,u32 ADDR_Sour)//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
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
//�ȶ�ҳУ����(64byte)
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
	Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
	p32NANDCTL=(u32*)(REG_NANDCTL);
	i=p32NANDCTL[0];
	i|=
	(0<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
	(1<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
	(0<<2)|//[2]DWR_EN DMA Write Data Enable
	(0<<1);//[1]DRD_EN DMA Read Data Enable
	p32NANDCTL[0]=i;
	while(p32NANDCTL[0]&(1<<3));
//��ҳ����
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
	
	Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
	//while(!p32s[0]&0x400);
	p32s=(u32*)(REG_NANDINTSTS);
	p32s[0]|=0x405;//��[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IFλ
	
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
		{//[2]ECC_FLD_IF,����0����DMA��ֹͣ���ٴ���
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
						{//��������
							ErrRead=1;
							break;
						}
						else
						{//������
							ErrBitNUM=(ErrStatus>>2)&0x1f;//����λ��
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
			p32s[0]=4;//��ECC_FLD_IF
		}
		if(x&1)
		{//[0]DMA_IFλ
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
	return ErrRead;
}

u32 Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//NAND_FLASH û�滻��;����:0=û����,!=0����
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
		while(1)//�����жϳ�ͻ�ض�
		{
			PageByte=2048-(ADDR_Sour&0x7FF);//��ǰPage�ܶ��ֽ�
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			if(((ADDR_Sour&0x7FF)==0)&&(Byte>=2048))
			{//�����nandҳ����,ͬʱ��ȡ��>=ҳ�ֽ�2048
				ErrRead|=Read_Page((u32)p8DEST,ADDR_Sour);//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
				p8DEST+=PageByte;
			}
			else
			{
				ErrRead|=Read_Page(ADDR_NAND_PAGEREAD_BUFF,ADDR_Sour);//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
			//ȡ����
				p8SOUR=(u8*)ADDR_NAND_PAGEREAD_BUFF+(ADDR_Sour&0x7ff);
				i=PageByte;
				if((((u32)p8SOUR&1)==0)&&(((u32)p8DEST&1)==0))
				{//ͬʱΪż
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
					{//ͬʱΪ��
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
			if(Comm_Ram->IntFlags&0x08)//�жϷ�����ʹ�ó�ͻ��־
			{
				Comm_Ram->IntFlags=0;//�жϷ�����ʹ�ó�ͻ��־
				continue;
			}
			break;
		}
		ADDR_Sour+=PageByte;
		p8DEST+=PageByte;
		Byte-=PageByte;
	}
	return ErrRead;//����:0=û����,!=0����
}

u32 BLOCK_Write_NAND(u32 ADDR_Dest)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����;û���滻
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

	
//---����nand�滻��ʱ---
//	if(ADDR_Dest==(ADDR_Download_ENTRY-ADDR_EFLASH_Start))
//	{
//		return 1;//����nand�滻��ʱ
//	}
//---����nand�滻��ʱ END---
	
	Init_NAND();//��λnand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	ADDR_Dest&=0xfffe0000;
	
//��д
	ReNum=2;//д������ظ�����
	while(ReNum--)
	{
		//�����
		BlockErase_NAND(ADDR_Dest,0);//NAND_FLASH �����;û���滻
		//��д��(ÿ��ҳ2048�ֽ�)
		sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
		for(Page=0;Page<64;Page++)
		{
			while(1)//�����жϳ�ͻ��д
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
				p32s[0]|=0x405;//��[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IFλ
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
					{//[0]DMA_IFλ
						break;
					}
				}
				//__disable_irq();
				if(Comm_Ram->IntFlags&0x08)//�жϷ�����ʹ�ó�ͻ��־
				{
					Comm_Ram->IntFlags=0;//�жϷ�����ʹ�ó�ͻ��־
					//__enable_irq();
					continue;
				}
				*p32CMD=0x10;//Command
				//__enable_irq();
				break;
			}
			Wait_NAND(600*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		}
//���Ƚ�
		for(Page=0;Page<64;Page++)
		{
			Read_Page(ADDR_NAND_PAGEREAD_BUFF,ADDR_Dest+(Page*2048));//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
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
			return 0;//����;0=��ȷ,1=����
		}
	}
	return 1;//����;0=��ȷ,1=����
}

u32 RePlace_BLOCK_ADDR(u32 ADDR)//ȡ�滻���ַ
{
	u32 i;
	u8 *p8EEPROM;
	
	p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
	i=ADDR/(128*1024);//��ǰ�����

	i=p8EEPROM[i];
	if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
	{//���滻
		i--;
		ADDR=((NAND_FILE_BLOCK_COUNT+i)*128*1024)+(ADDR&0x1ffff);
	}
	return ADDR;
}

u32 RePlace_Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//NAND_FLASH ���滻��;����:0=û����,1=����
{
	u32 i;
	u32 BlockByte;
	u32 Err;
	
	Err=0;
	while(Byte!=0)
	{
		BlockByte=(128*1024)-(ADDR_Sour&0x1FFFF);//��ǰPageҪ���ֽ�
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		i=RePlace_BLOCK_ADDR(ADDR_Sour);
		i=Read_NAND(ADDR_Dest,i,BlockByte);//NAND_FLASH û�滻��
		if(i>Err)
		{
			Err=i;//����:0=û����,1=1λ����,2=�޷�У����2λ���ϴ���
		}
		ADDR_Dest+=BlockByte;
		ADDR_Sour+=BlockByte;
		Byte-=BlockByte;
	}
	return Err;
}

void RePlace_BLOCK_Write_NAND(u32 ADDR_Dest)//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
{
	u32 i;
	u32 x;
	u32 BLOCK_COUNT;
	u32 RePlace_BLOCK_COUNT;
	u8 *p8EEPROM;
	
	p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
	BLOCK_COUNT=ADDR_Dest/(128*1024);//��ǰ�����
	
	if(BLOCK_COUNT<(NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT))
	{//���ļ�ϵͳ��������滻��Ŀ�;����ʱ�Ų�����Щ��,�����滻,ֻ���־����
		//�ñ����
		x=BLOCK_Write_NAND(ADDR_Dest);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(x==0)
		{//��ȷ
			MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		else
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		return;
	}
	//���п��滻,�����滻��
	x=ADDR_Dest;
	i=p8EEPROM[BLOCK_COUNT];
	if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
	{//���滻
		i--;
		x=((NAND_FILE_BLOCK_COUNT+i)*128*1024);
		i++;
	}
	x=BLOCK_Write_NAND(x);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
	if(x==0)
	{//��ȷ
		if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
		{//���滻,�滻�����ñ�ע
			i--;
			i+=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT;
			p8EEPROM=(u8*)(i-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
			if(p8EEPROM[0]!=0xFD)
			{
				MC(0xFD,i,1);//EEPROM���
			}
		}
		else
		{
			i=ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT;
			p8EEPROM=(u8*)(i-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
			if(p8EEPROM[0]!=0xFD)
			{
				MC(0xFD,i,1);//EEPROM���
			}
		}
	}
	else
	{//����
		if((i>=1)&&(i<=NAND_REPLACE_BLOCK_COUNT))
		{//���滻,�滻��Ҳ����
			i--;
			i+=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT;
			MC(0xFE,i,1);//EEPROM���
		}
		//�ҿյ��滻��д
		RePlace_BLOCK_COUNT=0;
		while(1)
		{
			p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP+NAND_FILE_BLOCK_COUNT;
			for(;RePlace_BLOCK_COUNT<NAND_REPLACE_BLOCK_COUNT;RePlace_BLOCK_COUNT++)
			{
				x=p8EEPROM[RePlace_BLOCK_COUNT];//�滻�õĿ�ֻ��2����ʾֵ0xFD��0xFE�����ʾδ��ʼ��
				if(x!=0xFD)
				{//�ҵ��ջ�ԭ�ѱ�עΪ����Ŀ�
					break;
				}
			}
			if(RePlace_BLOCK_COUNT<NAND_REPLACE_BLOCK_COUNT)
			{//�滻�黹�п�
				x=((NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT)*128*1024);
				x=BLOCK_Write_NAND(x);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
				if(x==0)
				{//��ȷ
					//д�滻���
					MC(RePlace_BLOCK_COUNT+1,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
					//�滻�����ñ�ע
					i=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT;
					MC(0xFD,i,1);//EEPROM���
					break;
				}
				//�滻�ٴδ�
				i=ADDR_NANDFLASH_BLOCK_REPLACE+NAND_FILE_BLOCK_COUNT+RePlace_BLOCK_COUNT;
				MC(0xFE,i,1);//EEPROM���
				RePlace_BLOCK_COUNT++;
			}
			else
			{//�滻����û��,�ñ����д
				x=BLOCK_Write_NAND(ADDR_Dest);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
				if(x==0)
				{//��ȷ
					MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
				}
				else
				{//����
					MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
				}
				return;
			}
		}
	}
}


void RePlace_Write_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//NAND_FLASHд;����������滻
{
	u32 i;
	u32 BlockByte;
	u8 *pSOUR;
	u8 *pBUFF;
	u32 Same;
	
	pSOUR=(u8*)ADDR_Sour;
	while(Byte!=0)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		//��ԭ128K������
		RePlace_Read_NAND(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//NAND_FLASH S34MLxxG2���滻��
		//BUFF�и�������ͬʱ�Ƚ�
		BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		pBUFF=(u8*)(ADDR_128KWRITE_BUFF);
		pBUFF+=(ADDR_Dest&0x1ffff);//ƫ��
		Same=1;//��ͬ
		for(i=0;i<BlockByte;i++)
		{
			if(*pBUFF!=*pSOUR)
			{
				Same=0;//����ͬ
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
			RePlace_BLOCK_Write_NAND(ADDR_Dest);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
		}
		ADDR_Dest+=BlockByte;
		Byte-=BlockByte;
	}
}

void RePlace_Clr_NAND(u32 Data,u32 ADDR_Dest,u32 Byte)//NAND_FLASH��0;����������滻
{
	u32 i;
	u32 BlockByte;
	u8 *p8BUFF;
	u32 Same;
	
	while(Byte!=0)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		//��ԭ128K������
		RePlace_Read_NAND(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//NAND_FLASH S34MLxxG2���滻��
		//��������ͬʱ�Ƚ�
		BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰBack��д�ֽ�
		if(BlockByte>Byte)
		{
			BlockByte=Byte;
		}
		p8BUFF=(u8*)(ADDR_128KWRITE_BUFF);
		p8BUFF+=(ADDR_Dest&0x1ffff);//ƫ��
		Same=1;//��ͬ
		for(i=0;i<BlockByte;i++)
		{
			if(*p8BUFF!=Data)
			{
				Same=0;//����ͬ
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
			RePlace_BLOCK_Write_NAND(ADDR_Dest);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
		}
		ADDR_Dest+=BlockByte;
		Byte-=BlockByte;
	}
}

void RePlace_BlockErase_NAND(u32 ADDR_Dest,u32 Byte)//NAND_FLASH �����;�п��滻
{
	u32 i;
	
	while(Byte)
	{
		i=RePlace_BLOCK_ADDR(ADDR_Dest);
		BlockErase_NAND(i,1);//NAND_FLASH �����;û���滻
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
#if NAND128kBuff==0//����128k nandд����
u32 Check_4kNandBuff(void)//���4K(BKPSRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
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
	
				
//---���籣�ֲ���---------------------
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
	if(p8[0]==0x55)//1BYTE 4K(BKPSRAM)���籣��,0=OK,1=ERR,0xff=û����,0x55=���в���
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
		{//����
			MWR(1,ADDR_BASE+(OFFSET_POWER_DOWN_Characteristics),1);
			p8=(u8*)ADDR_BASE+(OFFSET_TestResult_Characteristics);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
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
//---���籣�ֲ���End-------------------------
	
//���
	Err=0;
	p16=(u16*)(ADDR_BKPSRAM);
	if((p16[1]>4096)||(p16[1]<4))
	{//��,���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַ)
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
		{//��
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
			i=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM������CRC32����
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
		p32[0]=0x40000;//��ʼΪ�ܿ���=0,���ֽ���=4
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM������CRC32����
		Err=0;
	}
	else
	{
		Err=1;
	}
	return Err;//0=����,1=��ȷ
}
void NR_4kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//��NAND_FLASH��д�뻺��
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
		{//�����볤�Ȳ������
			p32=(u32*)(ADDR_BKPSRAM);
			p32[0]=0x40000;//��ʼΪ�ܿ���=0,���ֽ���=4
			p32=(u32*)(ADDR_BKPSRAM+(4096-4));
			p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM������CRC32����
			return;
		}
		p832=(u8*)(ADDR_BUFF);
		p816=(u8*)(ADDR_BUFF+4);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p816[0]|(p816[1]<<8);
		ReadByte=0;
		//BUFF�����ݿ�ʼ��ַ>=Ҫ����NAND��ʼ��ַ
		if((i>=ADDR_Sour)&&(i<(ADDR_Sour+Byte)))
		{
			p8s=(u8*)ADDR_BUFF+8;
			p8d=(u8*)ADDR_Dest+(i-ADDR_Sour);
			ReadByte=(ADDR_Sour+Byte)-i;//���ֽ���
			if(ReadByte>x)
			{
				ReadByte=x;//�������ֽ���
			}
		}
		else
		{
		//BUFF�����ݿ�ʼ��ַ<Ҫ����NAND��ʼ��ַ
			if((ADDR_Sour>=i)&&(ADDR_Sour<(i+x)))
			{
				p8s=(u8*)ADDR_BUFF+8+(ADDR_Sour-i);
				p8d=(u8*)ADDR_Dest;
				ReadByte=x-(ADDR_Sour-i);//�������ֽ���
				if(ReadByte>Byte)
				{
					ReadByte=Byte;//���ֽ���
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
void NB_4kBuff(u32 ADDR_Dest,u32 Byte)//����NAND_FLASHʱͬʱ����д�뻺��
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
		//BUFF�����ݿ�ʼ��ַ>=Ҫ����NAND��ʼ��ַ
		if((i>=ADDR_Dest)&&(i<(ADDR_Dest+Byte)))
		{
			p8d=(u8*)ADDR_BUFF+8;
			WriteByte=Byte-(i-ADDR_Dest);//д�ֽ���
			if(WriteByte>x)
			{
				WriteByte=x;//�������ֽ���
			}
		}
		else
		{
		//BUFF�����ݿ�ʼ��ַ<Ҫ����NAND��ʼ��ַ
			if((ADDR_Dest>=i)&&(ADDR_Dest<(i+x)))
			{
				p8d=(u8*)ADDR_BUFF+8+(ADDR_Dest-i);
				WriteByte=x-(ADDR_Dest-i);//�������ֽ���
				if(WriteByte>Byte)
				{
					WriteByte=Byte;//д�ֽ���
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
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM������CRC32����
	}
}

void NAND4kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0)//��ͬ4K(BKPSRAM)��������д��NAND FLASH
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
		{//����1����
			TBlock=1;
		//���ݿ�1
		//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
		//2BYTE �����ֽ���
		//2BYTE �Ѳ���д����ֽ���
			p32=(u32*)(ADDR_BKPSRAM+4);
			p32[0]=0;
			p32[1]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSRAM+4;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
		{//����
			if(Byte0==0)
			{
				p816=(u8*)(ADDR_BKPSRAM+2);
				x=p816[0]|(p816[1]<<8);
				if(x<=(4096-4))
				{
					p32=(u32*)(ADDR_BKPSRAM+(4096-4));
					p32[0]=RAM_CRC32(ADDR_BKPSRAM,x);//RAM������CRC32����
				}
				YAFFS_FMI_SET();//nand FMI��yaffs�е�����
				return;//��ͣд
			}
		}
		p816=(u8*)(ADDR_SRAM+4);
		n=p816[0]|(p816[1]<<8);//����д�ֽ�
		n-=p816[2]|(p816[3]<<8);
		if(n|Byte0)
		{//���ֽ���д��
			p832=(u8*)(ADDR_SRAM);
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//��ʼ��ַ+��д�ֽ�
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					i=p816[2]|(p816[3]<<8);//��д�ֽ���
					i+=BlockByte;
					p816[2]=i;
					p816[3]=i>>8;
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//�¿�
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					p816=(u8*)(ADDR_SRAM+4);
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//��ʼ��ַ+��д�ֽ�
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//���ַ��ͬ
						n=p816[0]|(p816[1]<<8);//����д�ֽ�
						n-=p816[2]|(p816[3]<<8);
						if(n)
						{//���ֽ���д��
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//��ǰ����д�ֽ�
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
							i=p816[2]|(p816[3]<<8);//��д�ֽ���
							i+=BlockByte;
							p816[2]=i;
							p816[3]=i>>8;
						}
					}
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//�¿�
				}
				//�������������
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					Byte0-=BlockByte;//��д�ֽ���
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
				ADDR_SRAM=ADDR_BKPSRAM+4;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;
	}
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=0x40000;//��ʼΪ�ܿ���=0,���ֽ���=4
	p32=(u32*)(ADDR_BKPSRAM+(4096-4));
	p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM������CRC32����
	YAFFS_FMI_SET();//nand FMI��yaffs�е�����
}

void NAND4kBuff_Clr_NAND(u32 Data0,u32 ADDR_Dest0,u32 Byte0)//��ͬ4K(BKPSRAM)��������д��NAND FLASH
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
		{//����1����
			TBlock=1;
		//���ݿ�1
		//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
		//2BYTE �����ֽ���
		//2BYTE �Ѳ���д����ֽ���
			p32=(u32*)(ADDR_BKPSRAM+4);
			p32[0]=0;
			p32[1]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSRAM+4;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
		{//����
			if(Byte0==0)
			{
				p816=(u8*)(ADDR_BKPSRAM+2);
				x=p816[0]|(p816[1]<<8);
				if(x<=(4096-4))
				{
					p32=(u32*)(ADDR_BKPSRAM+(4096-4));
					p32[0]=RAM_CRC32(ADDR_BKPSRAM,x);//RAM������CRC32����
				}
				YAFFS_FMI_SET();//nand FMI��yaffs�е�����
				return;//��ͣд
			}
		}
		p816=(u8*)(ADDR_SRAM+4);
		n=p816[0]|(p816[1]<<8);//����д�ֽ�
		n-=p816[2]|(p816[3]<<8);
		if(n|Byte0)
		{//���ֽ���д��
			p832=(u8*)(ADDR_SRAM);
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//��ʼ��ַ+��д�ֽ�
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					i=p816[2]|(p816[3]<<8);//��д�ֽ���
					i+=BlockByte;
					p816[2]=i;
					p816[3]=i>>8;
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//�¿�
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					p816=(u8*)(ADDR_SRAM+4);
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p816[2]|(p816[3]<<8));//��ʼ��ַ+��д�ֽ�
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//���ַ��ͬ
						n=p816[0]|(p816[1]<<8);//����д�ֽ�
						n-=p816[2]|(p816[3]<<8);
						if(n)
						{//���ֽ���д��
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//��ǰ����д�ֽ�
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
							i=p816[2]|(p816[3]<<8);//��д�ֽ���
							i+=BlockByte;
							p816[2]=i;
							p816[3]=i>>8;
						}
					}
					ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;//�¿�
				}
				//�������������
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					Byte0-=BlockByte;//��д�ֽ���
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
				ADDR_SRAM=ADDR_BKPSRAM+4;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p816[0]|(p816[1]<<8))+8;
	}
	p32=(u32*)(ADDR_BKPSRAM);
	p32[0]=0x40000;//��ʼΪ�ܿ���=0,���ֽ���=4
	p32=(u32*)(ADDR_BKPSRAM+(4096-4));
	p32[0]=RAM_CRC32(ADDR_BKPSRAM,4);//RAM������CRC32����
	YAFFS_FMI_SET();//nand FMI��yaffs�е�����
}

void NW_4kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//д4k Nand FLASH����
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
	{//����������
		NAND4kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
		return;
	}
	i=p816[0]|(p816[1]<<8);
	if(i>=BlockMax_NandBuff)
	{//�ܿ��� ��Ϊ��ͬnand��ʱ64*20ms=1280ms
		NAND4kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
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
	p32[0]=i;//���������ֽ�����32λһ��д�룬��������жϵ�����ͬ��!
	
	if(Byte<=(4096-4))
	{
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM������CRC32����
	}
}

void NC_4kBuff(u32 Data,u32 ADDR_Dest,u32 Byte)//���д��Nand FLASH�������ȵ�4kBuff
{
	u32 i;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	
	p816=(u8*)(ADDR_BKPSRAM);
	i=p816[2]|(p816[3]<<8);
	if((i+8+Byte)>4096)
	{//����������
		NAND4kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
		return;
	}
	i=p816[0]|(p816[1]<<8);
	if(i>=BlockMax_NandBuff)
	{//�ܿ��� ��Ϊ��ͬnand��ʱ64*20ms=1280ms
		NAND4kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//��ͬ4K(BKPSRAM)��������д��NAND FLASH
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
	p32[0]=i;//���������ֽ�����32λһ��д�룬��������жϵ�����ͬ��!
	
	if(Byte<=(4096-4))
	{
		p32=(u32*)(ADDR_BKPSRAM+(4096-4));
		p32[0]=RAM_CRC32(ADDR_BKPSRAM,Byte);//RAM������CRC32����
	}
}
#else//#if NAND128kBuff==0//����128k nandд����

u32 Check_128kNandBuff(void)//���128K(BKPSDRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
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
	{//��,���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַ)
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
		{//��
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
			i=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM������CRC32����
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
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM������CRC32����
		Err=0;
	}
	else
	{
		Err=1;
	}
	return Err;//0=����,1=��ȷ
}
void NR_128kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//��NAND_FLASH��д�뻺��
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
		{//�����볤�Ȳ������
			p32=(u32*)(ADDR_BKPSDRAM);
			p32[0]=0x60000;//��ʼΪ�ܿ���=0,���ֽ���=6
			p32[1]=0;
			p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
			p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM������CRC32����
			return;
		}
		p832=(u8*)(ADDR_BUFF);
		i=p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24);
		x=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		ReadByte=0;
		//BUFF�����ݿ�ʼ��ַ>=Ҫ����NAND��ʼ��ַ
		if((i>=ADDR_Sour)&&(i<(ADDR_Sour+Byte)))
		{
			p8s=(u8*)ADDR_BUFF+12;
			p8d=(u8*)ADDR_Dest+(i-ADDR_Sour);
			ReadByte=(ADDR_Sour+Byte)-i;//���ֽ���
			if(ReadByte>x)
			{
				ReadByte=x;//�������ֽ���
			}
		}
		else
		{
		//BUFF�����ݿ�ʼ��ַ<Ҫ����NAND��ʼ��ַ
			if((ADDR_Sour>=i)&&(ADDR_Sour<(i+x)))
			{
				p8s=(u8*)ADDR_BUFF+12+(ADDR_Sour-i);
				p8d=(u8*)ADDR_Dest;
				ReadByte=x-(ADDR_Sour-i);//�������ֽ���
				if(ReadByte>Byte)
				{
					ReadByte=Byte;//���ֽ���
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

void NB_128kBuff(u32 ADDR_Dest,u32 Byte)//����NAND_FLASHʱͬʱ����д�뻺��
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
		//BUFF�����ݿ�ʼ��ַ>=Ҫ����NAND��ʼ��ַ
		if((i>=ADDR_Dest)&&(i<(ADDR_Dest+Byte)))
		{
			p8d=(u8*)ADDR_BUFF+12;
			WriteByte=Byte-(i-ADDR_Dest);//д�ֽ���
			if(WriteByte>x)
			{
				WriteByte=x;//�������ֽ���
			}
		}
		else
		{
		//BUFF�����ݿ�ʼ��ַ<Ҫ����NAND��ʼ��ַ
			if((ADDR_Dest>=i)&&(ADDR_Dest<(i+x)))
			{
				p8d=(u8*)ADDR_BUFF+12+(ADDR_Dest-i);
				WriteByte=x-(ADDR_Dest-i);//�������ֽ���
				if(WriteByte>Byte)
				{
					WriteByte=Byte;//д�ֽ���
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
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM������CRC32����
	}
}

void NAND128kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0)//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
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
		{//����1����
			TBlock=1;
		//���ݿ�1
		//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
		//4BYTE �����ֽ���
		//4BYTE �Ѳ���д����ֽ���
			p32=(u32*)(ADDR_BKPSDRAM+6);
			p32[0]=0;
			p32[1]=0;
			p32[2]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSDRAM+6;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
		{//����
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
					p32[0]=RAM_CRC32(ADDR_BKPSDRAM,x);//RAM������CRC32����
				}
				YAFFS_FMI_SET();//nand FMI��yaffs�е�����
				return;//��ͣд
			}
		}
		p832=(u8*)(ADDR_SRAM);
		//n=p32[1]-p32[2];//����д�ֽ�
		n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
		if(n|Byte0)
		{//���ֽ���д��
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24)) + (p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//��ʼ��ַ+��д�ֽ�
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					i+=BlockByte;//��д�ֽ���
					p832[8]=i;
					p832[9]=i>>8;
					p832[10]=i>>16;
					p832[11]=i>>24;
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//�¿�
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24)) + (p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//��ʼ��ַ+��д�ֽ�
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//���ַ��ͬ
						//n=p32[1]-p32[2];//����д�ֽ�
						n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
						n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
						if(n)
						{//���ֽ���д��
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//��ǰ����д�ֽ�
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
							i+=BlockByte;//��д�ֽ���
							p832[8]=i;
							p832[9]=i>>8;
							p832[10]=i>>16;
							p832[11]=i>>24;
						}
					}
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//�¿�
				}
				//�������������
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					Byte0-=BlockByte;//��д�ֽ���
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
				ADDR_SRAM=ADDR_BKPSDRAM+6;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;
	}
	p32=(u32*)(ADDR_BKPSDRAM);
	p32[0]=0x60000;//��ʼΪ�ܿ���=0,���ֽ���=6
	p32[1]=0;
	p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
	p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM������CRC32����
	YAFFS_FMI_SET();//nand FMI��yaffs�е�����
}

void NAND128kBuff_Clr_NAND(u32 Data0,u32 ADDR_Dest0,u32 Byte0)//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
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
		{//����1����
			TBlock=1;
		//���ݿ�1
		//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
		//2BYTE �����ֽ���
		//2BYTE �Ѳ���д����ֽ���
			p32=(u32*)(ADDR_BKPSDRAM+6);
			p32[0]=0;
			p32[1]=0;
			p32[2]=0;
		}
	}
	ADDR_SRAM=ADDR_BKPSDRAM+6;
	for(x=0;x<TBlock;x++)
	{
		if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
		{//����
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
					p32[0]=RAM_CRC32(ADDR_BKPSDRAM,x);//RAM������CRC32����
				}
				YAFFS_FMI_SET();//nand FMI��yaffs�е�����
				return;//��ͣд
			}
		}
		p832=(u8*)(ADDR_SRAM);
		//n=p32[1]-p32[2];//����д�ֽ�
		n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
		n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
		if(n|Byte0)
		{//���ֽ���д��
			ADDR_Dest=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//��ʼ��ַ+��д�ֽ�
			if(((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))||Byte0)
			{//NAND
				if(Byte0!=0)
				{
					ADDR_Dest=ADDR_Dest0;
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
				}
				else
				{
					MR(ADDR_128KWRITE_BUFF,ADDR_Dest&0xfffe0000,128*1024);//������128K����
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					i+=BlockByte;//��д�ֽ���
					p832[8]=i;
					p832[9]=i>>8;
					p832[10]=i>>16;
					p832[11]=i>>24;
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//�¿�
					x++;
				}
				for(;x<TBlock;x++)
				{
					p832=(u8*)ADDR_SRAM;
					ADDR_DestX=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24))+(p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24));//��ʼ��ַ+��д�ֽ�
					if((ADDR_Dest&0xfffe0000)==(ADDR_DestX&0xfffe0000))
					{//���ַ��ͬ
						//n=p32[1]-p32[2];//����д�ֽ�
						n=p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24);
						n-=p832[8]|(p832[9]<<8)|(p832[10]<<16)|(p832[11]<<24);
						if(n)
						{//���ֽ���д��
							BlockByte=(128*1024)-(ADDR_DestX&0x1ffff);//��ǰ����д�ֽ�
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
							i+=BlockByte;//��д�ֽ���
							p832[8]=i;
							p832[9]=i>>8;
							p832[10]=i>>16;
							p832[11]=i>>24;
						}
					}
					ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;//�¿�
				}
				//�������������
				if(Byte0!=0)
				{
					BlockByte=(128*1024)-(ADDR_Dest&0x1ffff);//��ǰ����д�ֽ�
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
					Byte0-=BlockByte;//��д�ֽ���
				}
				RePlace_BLOCK_Write_NAND(ADDR_Dest-ADDR_EFLASH_Start);//���滻��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ������������滻��
				ADDR_SRAM=ADDR_BKPSDRAM+6;
				x=0xffffffff;
				continue;
			}
		}
		ADDR_SRAM+=(p832[4]|(p832[5]<<8)|(p832[6]<<16)|(p832[7]<<24))+12;
	}
	p32=(u32*)(ADDR_BKPSDRAM);
	p32[0]=0x60000;//��ʼΪ�ܿ���=0,���ֽ���=6
	p32[1]=0;
	p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
	p32[0]=RAM_CRC32(ADDR_BKPSDRAM,6);//RAM������CRC32����
	YAFFS_FMI_SET();//nand FMI��yaffs�е�����
}
void NW_128kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//д��Nand FLASH�������ȵ�128kBuff
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
	i=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24));//���ֽ���
	if((i+12+Byte)>(128*1024))
	{//����������
		NAND128kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
		return;
	}
	if((p816[0]|(p816[1]<<8))>=BlockMax_NandBuff)
	{//�ܿ��� ��Ϊ��ͬnand��ʱ64*20ms=1280ms
		NAND128kBuff_Write_NAND(ADDR_Sour,ADDR_Dest,Byte);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
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
	//���������ֽ�����32λһ��д�룬��������жϵ�����ͬ��!
	irq=off_irq();//��IRQ�ж�,����:�ر�ǰ����״̬
	p816[0]=i;
	p816[1]=i>>8;
	p832[0]=Byte;
	p832[1]=Byte>>8;
	p832[2]=Byte>>16;
	p832[3]=Byte>>24;
	destore_irq(irq);//�ָ�IRQ�ж�,���:�ر�ǰ����״̬
	if(Byte<=((128*1024)-4))
	{
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		if(Byte>(4096-4))
		{
			Byte=(4096-4);
		}
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM������CRC32����
	}
}
void NC_128kBuff(u32 Data,u32 ADDR_Dest,u32 Byte)//���д��Nand FLASH�������ȵ�128kBuff
{
	u32 i;
	u8 *p8d;
	u8 *p816;
	u8 *p832;
	u32* p32;
	u32 irq;
	
	p816=(u8*)(ADDR_BKPSDRAM);
	p832=(u8*)(ADDR_BKPSDRAM+2);
	i=(p832[0]|(p832[1]<<8)|(p832[2]<<16)|(p832[3]<<24));//���ֽ���
	if((i+12+Byte)>(128*1024))
	{//����������
		NAND128kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
		return;
	}
	if((p816[0]|(p816[1]<<8))>=BlockMax_NandBuff)
	{//�ܿ��� ��Ϊ��ͬnand��ʱ64*20ms=1280ms
		NAND128kBuff_Clr_NAND(Data,ADDR_Dest,Byte);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH
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
	//���������ֽ�����32λһ��д�룬��������жϵ�����ͬ��!
	irq=off_irq();//��IRQ�ж�,����:�ر�ǰ����״̬
	p816[0]=i;
	p816[1]=i>>8;
	p832[0]=Byte;
	p832[1]=Byte>>8;
	p832[2]=Byte>>16;
	p832[3]=Byte>>24;
	destore_irq(irq);//�ָ�IRQ�ж�,���:�ر�ǰ����״̬
	if(Byte<=((128*1024)-4))
	{
		p32=(u32*)(ADDR_BKPSDRAM+((128*1024)-4));
		if(Byte>(4096-4))
		{
			Byte=(4096-4);
		}
		p32[0]=RAM_CRC32(ADDR_BKPSDRAM,Byte);//RAM������CRC32����
	}
}
#endif

//1st cycle A7-A0
//2nd cycle A11-A8
//3rd cycle A19-A12
//4th cycle A27-A20
//5th cycle A28















