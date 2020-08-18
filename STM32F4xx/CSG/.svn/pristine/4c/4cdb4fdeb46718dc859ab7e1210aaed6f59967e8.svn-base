
//ERTC
#ifndef ERTC_H
#define ERTC_H

#include "type.h"





//--------------------------------------------------------------------------------------------------------
// Function name:       ERTC_Init
// Descriptions:        外部硬实时钟初始化
// input parameters:    无
// output parameters:   无
// Returned value:      无
//--------------------------------------------------------------------------------------------------------
void ERTC_Init(void);//外部硬实时钟初始化

//--------------------------------------------------------------------------------------------------------
// Function name:       ReadRTC
// Descriptions:        读外部硬实时钟到指定缓冲地址
// input parameters:    缓冲地址
// output parameters:   年月日周时分秒读到缓冲地址中
// Returned value:      无
//--------------------------------------------------------------------------------------------------------
void ReadRTC(u32 ADDR_BUFF);//读外部硬实时钟到指定缓冲地址


//--------------------------------------------------------------------------------------------------------
// Function name:       WriteRTC
// Descriptions:        写外部硬实时钟
// input parameters:    年月日周时分秒已在缓冲地址中
// output parameters:   无
// Returned value:      无
//--------------------------------------------------------------------------------------------------------
void WriteRTC(u32 ADDR_BUFF);//写外部硬实时钟

void DS3231_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231写


#endif
