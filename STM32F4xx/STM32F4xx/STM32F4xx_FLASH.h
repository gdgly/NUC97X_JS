

#ifndef STM32F4xx_FLASH_H
#define STM32F4xx_FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif

void Erase_STM32F4xx_Sector(u32 Sector);//��������
void Write_STM32F4xx_AnySector(u32 *ADDR_Sour,u32 *ADDR_FLASH,u32 Byte);//дSTM32F4xx��������

void Write_STM32F4xx_FLASH(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//��FLASHд;ʹ��ADDR_128KWRITE_BUFF
void Clr_STM32F4xx_FLASH(u32 Data,u32 ADDR_Dest,u32 Byte);//��FLASH��0;ʹ��ADDR_128KWRITE_BUFF
void STM32F4xx_FLASH_OPT(void);//дѡ���ֽ�(������д��������λ��ֵ��ѹ(BOR reset Level)��

	 
#ifdef __cplusplus
 }
#endif 
#endif
