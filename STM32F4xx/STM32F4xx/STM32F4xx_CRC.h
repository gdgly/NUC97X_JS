

#ifndef STM32F4xx_CRC_H
#define STM32F4xx_CRC_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 RAM_CRC32(u32 ADDR_DATA,u32 Byte);//RAM������CRC32����
u32 CRC32(u32 ADDR_Start,u32 ADDR_End);//�����ַ����CRC32����(ʹ��ADDR_DATABUFF)
u32 RAMCRC16_Soft(u32 ADDR_RAM_Start,u32 Len);//RAM������CRC16����(����㷨)
	 
void Data_Save_CRC(void);//���籣������CRC���� 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif

