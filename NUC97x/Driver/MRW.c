
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/Device/NAND.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../MS/yaffsAPP.h"


	
void MR_RAM(u8* p8Dest,u8* p8Sour,u32 Byte)//RAM数据读
{
	u32 i;
	u16 *p16s;
	u16 *p16d;
	
	if(Byte<4)
	{
		for(i=0;i<Byte;i++)
		{
			p8Dest[i]=p8Sour[i];
		}
	}
	else
	{
		if(((u32)p8Sour&1)||((u32)p8Dest&1))
		{//地址为奇数
			p8Dest[0]=p8Sour[0];
			p8Sour++;
			p8Dest++;
			Byte--;
		}
		if(((u32)p8Sour&1)||((u32)p8Dest&1))
		{//地址还为奇数
			for(i=0;i<Byte;i++)
			{
				p8Dest[i]=p8Sour[i];
			}
		}
		else
		{//地址均为偶数
			p16s=(u16*)p8Sour;
			p16d=(u16*)p8Dest;
			while(Byte>=2)
			{
				p16d[0]=p16s[0];
				p16s++;
				p16d++;
				Byte-=2;
			}
			if(Byte)
			{
				p8Sour=(u8*)p16s;
				p8Dest=(u8*)p16d;
				p8Dest[0]=p8Sour[0];
			}
		}
	}
}
void MW_RAM(u8* p8Sour,u8* p8Dest,u32 Byte)//RAM数据写
{
	u32 i;
	u16 *p16s;
	u16 *p16d;
	
	if(Byte<4)
	{
		for(i=0;i<Byte;i++)
		{
			p8Dest[i]=p8Sour[i];
		}
	}
	else
	{
		if(((u32)p8Sour&1)||((u32)p8Dest&1))
		{//地址为奇数
			p8Dest[0]=p8Sour[0];
			p8Sour++;
			p8Dest++;
			Byte--;
		}
		if(((u32)p8Sour&1)||((u32)p8Dest&1))
		{//地址还为奇数
			for(i=0;i<Byte;i++)
			{
				p8Dest[i]=p8Sour[i];
			}
		}
		else
		{//地址均为偶数
			p16s=(u16*)p8Sour;
			p16d=(u16*)p8Dest;
			while(Byte>=2)
			{
				p16d[0]=p16s[0];
				p16s++;
				p16d++;
				Byte-=2;
			}
			if(Byte)
			{
				p8Sour=(u8*)p16s;
				p8Dest=(u8*)p16d;
				p8Dest[0]=p8Sour[0];
			}
		}
	}
}
void MC_RAM(u32 Data,u8* p8Dest,u32 Byte)//RAM数据清除
{
	u32 i;
	u16 *p16d;
	
	i=Data&0xff;
	Data=i|(i<<8);
	if(Byte<4)
	{
		for(i=0;i<Byte;i++)
		{
			p8Dest[i]=Data;
		}
	}
	else
	{
		if((u32)p8Dest&1)
		{//地址为奇数
			p8Dest[0]=Data;
			p8Dest++;
			Byte--;
		}
		p16d=(u16*)p8Dest;
		while(Byte>=2)
		{
			p16d[0]=Data;
			p16d++;
			Byte-=2;
		}
		if(Byte)
		{
			p8Dest=(u8*)p16d;
			p8Dest[0]=Data;
		}
	}
}

void MR(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//存储器读
{
	if(Byte==0)
	{
		return;
	}
	MRW->ADDR_Sour_or_Data=ADDR_Sour;
	MRW->ADDR_Dest=ADDR_Dest;
	MRW->Byte=Byte;
	MRW->RWC=1;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	
	if(ADDR_Sour<0x08000000)
	{//128M DDR2
		MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Sour>=0x3c000000)&&(ADDR_Sour<0x3c000000+(56*1024)))
	{//56K ISRAM
		MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Sour>=ADDR_SPIFLASH_START)&&(ADDR_Sour<(ADDR_SPIFLASH_START+0x00400000)))
	{//SPI FLASH
		//SPI直接读
		if(ADDR_Sour<ADDR_IEEPROM_START)
		{
			Read_SPIFLASH(ADDR_Dest,ADDR_Sour-ADDR_SPIFLASH_START,Byte);//READ SPI FLASH
			MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
			return;
		}
		//IEEPROM(SPIFLASH)在DDR2缓存
		if(ADDR_Sour<(ADDR_SPIFLASH_START+0x00400000))
		{
			ADDR_Sour-=ADDR_IEEPROM_START;
			ADDR_Sour+=ADDR_SPI_MAP;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
			return;
		}
	}
	if((ADDR_Sour>=0x30000000)&&(ADDR_Sour<0xA0000000))
	{//NAND_FLASH
#if MainProtocol==698
		if((ADDR_Sour>=ADDR_DL698_SET_START)&&(ADDR_Sour<(ADDR_DL698_SET_START+LEN_FLASH_DL698_SET)))
		{//设置参数在DDR2缓存
			ADDR_Sour-=ADDR_DL698_SET_START;
			ADDR_Sour+=ADDR_NAND_MAP;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;
			return;
		}
#endif
#if MainProtocol==376
		if((ADDR_Sour>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Sour<ADDR_MeterParameter_EFLASH_End)&&(Byte<=((ADDR_MeterParameter_EFLASH_End)-ADDR_Sour)))
		{//设置参数在DDR2缓存
			ADDR_Sour-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Sour+=ADDR_NAND_MAP;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;
			return;
		}
		if((ADDR_Sour>=ADDR_AFN04_Start)&&(ADDR_Sour<(ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04+LEN_Terminal_EFLASH_AFN05))&&(Byte<=((ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04+LEN_Terminal_EFLASH_AFN05)-ADDR_Sour)))
		{
			ADDR_Sour-=ADDR_AFN04_Start;
			ADDR_Sour+=ADDR_NAND_MAP+0x00080000;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;
			return;
		}
#endif
#if MainProtocol==CSG
	#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if((ADDR_Sour>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Sour<ADDR_MeterParameter_EFLASH_End)&&(Byte<=((ADDR_MeterParameter_EFLASH_End)-ADDR_Sour)))
		{//设置参数在DDR2缓存
			ADDR_Sour-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Sour+=ADDR_NAND_MAP;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;
			return;
		}
	#endif
		if((ADDR_Sour>=ADDR_QCSG_SET_START)&&(ADDR_Sour<(ADDR_QCSG_SET_START+LEN_FLASH_QCSG_SET)))
		{
			ADDR_Sour-=ADDR_QCSG_SET_START;
			ADDR_Sour+=ADDR_NAND_MAP+0x00080000;
			MR_RAM((u8*)ADDR_Dest,(u8*)ADDR_Sour,Byte);//RAM数据读
			MRW->RWC=0;
			return;
		}
#endif
		
		RePlace_Read_NAND(ADDR_Dest,ADDR_Sour-ADDR_EFLASH_Start,Byte);//NAND_FLASH 有替换读
	#if NAND128kBuff==0
		NR_4kBuff(ADDR_Dest,ADDR_Sour,Byte);//读NAND_FLASH写入缓冲
	#else
		NR_128kBuff(ADDR_Dest,ADDR_Sour,Byte);//读NAND_FLASH写入缓冲
	#endif
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		
		YAFFS_FMI_SET();
		return;
	}
	
}

void MW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//存储器写
{
	if(Byte==0)
	{
		return;
	}
	MRW->ADDR_Sour_or_Data=ADDR_Sour;
	MRW->ADDR_Dest=ADDR_Dest;
	MRW->Byte=Byte;
	MRW->RWC=2;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	
	if(ADDR_Dest<0x08000000)
	{//128M DDR2
		MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据读
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Dest>=0x3c000000)&&(ADDR_Dest<0x3c000000+(56*1024)))
	{//56K ISRAM
		MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据读
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Dest>=ADDR_SPIFLASH_START)&&(ADDR_Dest<(ADDR_SPIFLASH_START+0x00400000)))
	{//SPI FLASH
		Write_SPIFLASH(ADDR_Sour,ADDR_Dest-ADDR_SPIFLASH_START,Byte);//READ SPI FLASH
		//SPI直接写
		if(ADDR_Dest<ADDR_IEEPROM_START)
		{
			MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
			return;
		}
		//IEEPROM(SPIFLASH)在DDR2缓存
		ADDR_Dest-=ADDR_IEEPROM_START;
		ADDR_Dest+=ADDR_SPI_MAP;
		if(Byte>LEN_SPI_MAP)
		{
			Byte=(ADDR_SPI_MAP+LEN_SPI_MAP)-ADDR_Dest;
		}
		MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据读
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		
		//清校表参数备份
		ADDR_Dest-=ADDR_SPI_MAP;
		ADDR_Dest+=ADDR_IEEPROM_START;
		if((ADDR_Dest>=ADDR_MeterCalibration_IEEPROM_START)&&(ADDR_Dest<(ADDR_MeterCalibration_IEEPROM_START+LEN_MeterCalibration_IEEPROM)))
		{//是校表参数写
			MC(0,ADDR_MeterCalibration_BACK,8);//清备份
		}
		return;
	}
	if((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))
	{//NAND_FLASH
	#if NAND128kBuff==0
		NW_4kBuff(ADDR_Sour,ADDR_Dest,Byte);//写入Nand FLASH的数据先到4kBuff
	#else
		NW_128kBuff(ADDR_Sour,ADDR_Dest,Byte);//写入Nand FLASH的数据先到4kBuff
	#endif
		
#if MainProtocol==698
		if((ADDR_Dest>=ADDR_DL698_SET_START)&&(ADDR_Dest<(ADDR_DL698_SET_START+LEN_FLASH_DL698_SET)))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_DL698_SET_START;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>LEN_NAND_MAP)
			{
				Byte=(ADDR_NAND_MAP+LEN_NAND_MAP)-ADDR_Dest;
			}
			MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();//nand FMI在yaffs中的配置
			
			//清通信参数备份
			ADDR_Dest-=ADDR_NAND_MAP;
			ADDR_Dest+=ADDR_DL698_SET_START;
			if((ADDR_Dest>=ADDR_4001)&&(ADDR_Dest<ADDR_DL698_COMSET_END))
			{//是通信参数写
				MC(0,ADDR_Communication_BACK,4096);//清备份
			}
			return;
		}
#endif
#if MainProtocol==376
		if((ADDR_Dest>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Dest<ADDR_MeterParameter_EFLASH_End))
		{//电能表参数在DDR2缓存
			ADDR_Dest-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>0x00080000)
			{
				Byte=(ADDR_NAND_MAP+0x00080000)-ADDR_Dest;
			}
			MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			
			YAFFS_FMI_SET();//nand FMI在yaffs中的配置
			return;
		}
		if((ADDR_Dest>=ADDR_AFN04_Start)&&(ADDR_Dest<(ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04+LEN_Terminal_EFLASH_AFN05)))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_AFN04_Start;
			ADDR_Dest+=ADDR_NAND_MAP+0x00080000;
			if(Byte>0x00200000)
			{
				Byte=(ADDR_NAND_MAP+0x00300000)-ADDR_Dest;
			}
			MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;

			YAFFS_FMI_SET();//nand FMI在yaffs中的配置
			
			//清通信参数备份
			ADDR_Dest-=ADDR_NAND_MAP+0x00080000;
			ADDR_Dest+=ADDR_AFN04_Start;
			if((ADDR_Dest>=(ADDR_AFN04F89))&&(ADDR_Dest<(ADDR_MenuKey)))
			{//是通信参数写
				MC(0,ADDR_Communication_BACK,4096);//清备份
			}
			return;
		}
#endif
#if MainProtocol==CSG
	#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if((ADDR_Dest>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Dest<ADDR_MeterParameter_EFLASH_End))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>0x00080000)
			{
				Byte=(ADDR_NAND_MAP+0x00080000)-ADDR_Dest;
			}
			MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			
			YAFFS_FMI_SET();//nand FMI在yaffs中的配置
			return;
		}
	#endif
		if((ADDR_Dest>=ADDR_QCSG_SET_START)&&(ADDR_Dest<(ADDR_QCSG_SET_START+LEN_FLASH_QCSG_SET)))
		{
			ADDR_Dest-=ADDR_QCSG_SET_START;
			ADDR_Dest+=ADDR_NAND_MAP+0x00080000;
			if(Byte>0x00200000)
			{
				Byte=(ADDR_NAND_MAP+0x00300000)-ADDR_Dest;
			}
			MW_RAM((u8*)ADDR_Sour,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;

			YAFFS_FMI_SET();//nand FMI在yaffs中的配置
			
			//清通信参数备份
			
			return;
		}
#endif
		
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		YAFFS_FMI_SET();//nand FMI在yaffs中的配置
		return;
	}
	MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
}

void MC(u32 Data,u32 ADDR_Dest,u32 Byte)//存储器清0
{
	if(Byte==0)
	{
		return;
	}
	MRW->ADDR_Sour_or_Data=Data;
	MRW->ADDR_Dest=ADDR_Dest;
	MRW->Byte=Byte;
	MRW->RWC=3;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	
	if(ADDR_Dest<0x08000000)
	{//128M DDR2
		MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据清除
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Dest>=0x3c000000)&&(ADDR_Dest<0x3c000000+(56*1024)))
	{//56K ISRAM
		MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据清除
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Dest>=ADDR_SPIFLASH_START)&&(ADDR_Dest<(ADDR_SPIFLASH_START+0x00400000)))
	{//SPI FLASH
		Clr_SPIFLASH(Data,ADDR_Dest-ADDR_SPIFLASH_START,Byte);//clr SPI FLASH
		//SPI直接写
		if(ADDR_Dest<ADDR_IEEPROM_START)
		{
			MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
			return;
		}
		//IEEPROM(SPIFLASH)在DDR2缓存
		ADDR_Dest-=ADDR_IEEPROM_START;
		ADDR_Dest+=ADDR_SPI_MAP;
		if(Byte>LEN_SPI_MAP)
		{
			Byte=(ADDR_SPI_MAP+LEN_SPI_MAP)-ADDR_Dest;
		}
		MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据清除
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}
	if((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))
	{//NAND_FLASH
	#if NAND128kBuff==0
		NC_4kBuff(Data,ADDR_Dest,Byte);//写入Nand FLASH的数据先到4kBuff
	#else
		NC_128kBuff(Data,ADDR_Dest,Byte);//写入Nand FLASH的数据先到4kBuff
	#endif
		
#if MainProtocol==698
		if((ADDR_Dest>=ADDR_DL698_SET_START)&&(ADDR_Dest<(ADDR_DL698_SET_START+LEN_FLASH_DL698_SET)))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_DL698_SET_START;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>LEN_NAND_MAP)
			{
				Byte=(ADDR_NAND_MAP+LEN_NAND_MAP)-ADDR_Dest;
			}
			MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();
			return;
		}
#endif
#if MainProtocol==376
		if((ADDR_Dest>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Dest<ADDR_MeterParameter_EFLASH_End))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>0x00080000)
			{
				Byte=(ADDR_NAND_MAP+0x00080000)-ADDR_Dest;
			}
			MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();
			return;
		}
		if((ADDR_Dest>=ADDR_AFN04_Start)&&(ADDR_Dest<(ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04+LEN_Terminal_EFLASH_AFN05)))
		{
			ADDR_Dest-=ADDR_AFN04_Start;
			ADDR_Dest+=ADDR_NAND_MAP+0x00080000;
			if(Byte>0x00200000)
			{
				Byte=(ADDR_NAND_MAP+0x00300000)-ADDR_Dest;
			}
			MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();
			return;
		}
#endif
#if MainProtocol==CSG
	#if ((Project/100)!=5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		if((ADDR_Dest>=ADDR_MeterParameter_EFLASH_Start)&&(ADDR_Dest<ADDR_MeterParameter_EFLASH_End))
		{//设置参数在DDR2缓存
			ADDR_Dest-=ADDR_MeterParameter_EFLASH_Start;
			ADDR_Dest+=ADDR_NAND_MAP;
			if(Byte>0x00080000)
			{
				Byte=(ADDR_NAND_MAP+0x00080000)-ADDR_Dest;
			}
			MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();
			return;
		}
	#endif
		if((ADDR_Dest>=ADDR_QCSG_SET_START)&&(ADDR_Dest<(ADDR_QCSG_SET_START+LEN_FLASH_QCSG_SET)))
		{
			ADDR_Dest-=ADDR_QCSG_SET_START;
			ADDR_Dest+=ADDR_NAND_MAP+0x00080000;
			if(Byte>0x00200000)
			{
				Byte=(ADDR_NAND_MAP+0x00300000)-ADDR_Dest;
			}
			MC_RAM(Data,(u8*)ADDR_Dest,Byte);//RAM数据写
			MRW->RWC=0;
			YAFFS_FMI_SET();
			return;
		}
#endif
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		YAFFS_FMI_SET();
		return;
	}
	MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
}

void ME(u32 ADDR_Dest,u32 Byte)//FLASH块擦除;
{
	
	if(Byte==0)
	{
		return;
	}
	if((ADDR_Dest>=0x30000000)&&(ADDR_Dest<0xA0000000))
	{
		//NAND_FLASH
		MRW->ADDR_Dest=ADDR_Dest;
		MRW->Byte=Byte;
		MRW->RWC=4;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR	
		
		RePlace_BlockErase_NAND(ADDR_Dest-ADDR_EFLASH_Start,Byte);//NAND_FLASH 块擦除;有块替换
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	#if NAND128kBuff==0
		NB_4kBuff(ADDR_Dest,Byte);//擦除NAND_FLASH时同时更新写入缓冲
	#else
		NB_128kBuff(ADDR_Dest,Byte);//擦除NAND_FLASH时同时更新写入缓冲
	#endif
		YAFFS_FMI_SET();
		MRW->RWC=0;//0=空,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
		return;
	}	
}



u64 MRR(u32 ADDR_Sour,u32 Byte)//存储器读到寄存器(<=8字节)
{
  u64 * p64;
  p64=(u64 *)(ADDR_MRWBUFF);
  p64[0]=0;
  MR(ADDR_MRWBUFF,ADDR_Sour,Byte);//存储器读
  return p64[0];
}

void MWR(u64 Data,u32 ADDR_Dest,u32 Byte)//寄存器写到存储器(<=8字节)
{
  u64 * p64;
  p64=(u64 *)(ADDR_MRWBUFF);
  p64[0]=Data;
  MW(ADDR_MRWBUFF,ADDR_Dest,Byte);//存储器写
}





















