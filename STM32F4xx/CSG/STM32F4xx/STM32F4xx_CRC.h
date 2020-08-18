

#ifndef STM32F4xx_CRC_H
#define STM32F4xx_CRC_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 RAM_CRC32(u32 ADDR_DATA,u32 Byte);//RAM中数据CRC32计算
u32 CRC32(u32 ADDR_Start,u32 ADDR_End);//任意地址数据CRC32计算(使用ADDR_DATABUFF)

#ifdef __cplusplus
 }
#endif 
#endif

