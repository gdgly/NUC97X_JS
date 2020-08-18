

#ifndef STM32F4xx_FLASH_H
#define STM32F4xx_FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif

void Erase_STM32F4xx_Sector(u32 Sector);//擦除扇区
void Write_STM32F4xx_AnySector(u32 *ADDR_Sour,u32 *ADDR_FLASH,u32 Byte);//写STM32F4xx任意扇区

void Write_STM32F4xx_FLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//主FLASH写;使用ADDR_128KWRITE_BUFF
void Clr_STM32F4xx_FLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//主FLASH清0;使用ADDR_128KWRITE_BUFF
void STM32F4xx_FLASH_OPT(void);//写选项字节(扇区读写保护，复位阈值电压(BOR reset Level)）

	 
#ifdef __cplusplus
 }
#endif 
#endif
