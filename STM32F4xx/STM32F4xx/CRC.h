

#ifndef CRC_H
#define CRC_H


void Table_CRC(u32 ADDR_Table,u32 Poly);//产生CRC32比对表;入口:ADDR_Table=比对表存放地址(需256*4Byte),Poly=校验的多项式值
u32 CRC32(u32 ADDR_DATA,u32 ADDR_Table,u32 Byte,u32 InitCRC);//CRC32计算;入口:ADDR_DATA=数据,ADDR_Table=比对表,InitCRC=CRC初始值

u32 RAM_CRC32_Software(u32 ADDR_DATA,u32 Byte);//RAM中数据CRC32计算

#endif
