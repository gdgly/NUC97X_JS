

#ifndef STM32F4xx_ECC_H
#define STM32F4xx_ECC_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 STM32F4xx_ECC_Correct_Data(u8 *Data,u32 *WriteECC,u32 *ReadECC);//NAND������������;����:0=û����,1=1λ����,2=�޷�У����2λ���ϴ���
u32 ECC_TEST(void);//ECC�����У�����(�����е�ÿ�ֽڰ�λȡ������,ѭ������=����*8);���أ�0=��ȷ,1=����

	 
#ifdef __cplusplus
 }
#endif
#endif


