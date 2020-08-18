

#ifndef STM32F4xx_SPI_H
#define STM32F4xx_SPI_H
#ifdef __cplusplus
 extern "C" {
#endif

void UnInit_SPI1(void);//缷载SPI1初始化(TESAM),SPI引为输入下拉
void Init_SPI1(void);//SPI1初始化(TESAM)

void UnInit_SPI2(void);//缷载SPI2初始化(TESAM),SPI引为输入下拉
void Init_SPI2(void);//SPI2初始化(ATM90E36)

#ifdef __cplusplus
 }
#endif 
#endif


