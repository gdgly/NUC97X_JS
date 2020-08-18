

#ifndef STM32F4xx_FMC_H
#define STM32F4xx_FMC_H
#ifdef __cplusplus
 extern "C" {
#endif

void SDRAM_FMC_Init(void);
void NAND_BANK3_FMC_Init(void);
void NORSRAM_BANK2_FMC_Init(void);
void LCD_FMC_Init(void);

#ifdef __cplusplus
 }
#endif 
#endif

