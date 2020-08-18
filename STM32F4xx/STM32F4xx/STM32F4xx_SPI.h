

#ifndef STM32F4xx_SPI_H
#define STM32F4xx_SPI_H
#ifdef __cplusplus
 extern "C" {
#endif

void UnInit_SPI1(void);//缷载SPI1初始化(TESAM),SPI为输入下拉
void Init_SPI1(void);//SPI1初始化(TESAM)

void UnInit_SPI2(void);//缷载SPI2初始化(ATM90E36或 ATT7022),SPI为输入下拉
void Init_SPI2(void);//SPI2初始化(ATM90E36或 ATT7022)

#ifdef IC_NUC9xx
void UnInit_SPI0(void);//缷载SPI0初始化(SPI FLASH),SPI为输入下拉
void Init_SPI0(void);//SPI0初始化(SPI FLASH)
u32 ID_SPIFLASH(void);//读SPI FLASH器件ID
u64 Unique_ID_SPIFLASH(void);//读SPI FLASH器件Unique ID
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//READ SPI FLASH
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//WRITE SPI FLASH
void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//Clr SPI FLASH
	 
#endif	 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif


