
//ERTC
#ifndef ERTC_H
#define ERTC_H
#ifdef __cplusplus
 extern "C" {
#endif



void DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231读
void RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T读
void DS3231_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231写
void RX8025T_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T写



void Init_RTC(void);//内外部实时钟初始化

//--------------------------------------------------------------------------------------------------------
// Function name:       ReadRTC
// Descriptions:        读外部硬实时钟到指定缓冲地址
// input parameters:    缓冲地址
// output parameters:   年月日周时分秒读到缓冲地址中
// Returned value:      无
//--------------------------------------------------------------------------------------------------------
void ReadRTC(u32 ADDR_BUFF);//读实时钟到指定缓冲地址


//--------------------------------------------------------------------------------------------------------
// Function name:       WriteRTC
// Descriptions:        写外部硬实时钟
// input parameters:    年月日周时分秒已在缓冲地址中
// output parameters:   无
// Returned value:      无
//--------------------------------------------------------------------------------------------------------
void WriteRTC(u32 ADDR_BUFF);//写实时钟

#ifdef __cplusplus
 }
#endif
#endif
