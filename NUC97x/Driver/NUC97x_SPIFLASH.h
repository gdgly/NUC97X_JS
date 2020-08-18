
#ifndef NUC97x_SPIFILSH_H
#define NUC97x_SPIFILSH_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
u32 ID_SPIFLASH(void);//读SPI FLASH器件ID
u64 Unique_ID_SPIFLASH(void);//读SPI FLASH器件Unique ID
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//READ SPI FLASH
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//WRITE SPI FLASH
void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//Clr SPI FLASH

u32 LOAD_PROGRAM(u32 ADDR_ENTRY,u32 LENmax);//软件装入,自动从主区和备份区装入,且自动拷贝到主区和备份区;返回:0=错误，1=正确
	 
#ifdef __cplusplus
 }
#endif 
#endif
