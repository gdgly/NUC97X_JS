

#ifndef STM32F4xx_SPI_H
#define STM32F4xx_SPI_H
#ifdef __cplusplus
 extern "C" {
#endif

void UnInit_SPI1(void);//����SPI1��ʼ��(TESAM),SPIΪ��������
void Init_SPI1(void);//SPI1��ʼ��(TESAM)

void UnInit_SPI2(void);//����SPI2��ʼ��(ATM90E36�� ATT7022),SPIΪ��������
void Init_SPI2(void);//SPI2��ʼ��(ATM90E36�� ATT7022)

#ifdef IC_NUC9xx
void UnInit_SPI0(void);//����SPI0��ʼ��(SPI FLASH),SPIΪ��������
void Init_SPI0(void);//SPI0��ʼ��(SPI FLASH)
u32 ID_SPIFLASH(void);//��SPI FLASH����ID
u64 Unique_ID_SPIFLASH(void);//��SPI FLASH����Unique ID
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//READ SPI FLASH
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//WRITE SPI FLASH
void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//Clr SPI FLASH
	 
#endif	 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif


