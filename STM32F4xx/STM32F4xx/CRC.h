

#ifndef CRC_H
#define CRC_H


void Table_CRC(u32 ADDR_Table,u32 Poly);//����CRC32�ȶԱ�;���:ADDR_Table=�ȶԱ��ŵ�ַ(��256*4Byte),Poly=У��Ķ���ʽֵ
u32 CRC32(u32 ADDR_DATA,u32 ADDR_Table,u32 Byte,u32 InitCRC);//CRC32����;���:ADDR_DATA=����,ADDR_Table=�ȶԱ�,InitCRC=CRC��ʼֵ

u32 RAM_CRC32_Software(u32 ADDR_DATA,u32 Byte);//RAM������CRC32����

#endif
