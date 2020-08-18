
#ifndef NUC97x_SPIFILSH_H
#define NUC97x_SPIFILSH_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
u32 ID_SPIFLASH(void);//��SPI FLASH����ID
u64 Unique_ID_SPIFLASH(void);//��SPI FLASH����Unique ID
void Read_SPIFLASH(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//READ SPI FLASH
void Write_SPIFLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//WRITE SPI FLASH
void Clr_SPIFLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//Clr SPI FLASH

u32 LOAD_PROGRAM(u32 ADDR_ENTRY,u32 LENmax);//���װ��,�Զ��������ͱ�����װ��,���Զ������������ͱ�����;����:0=����1=��ȷ
	 
#ifdef __cplusplus
 }
#endif 
#endif
