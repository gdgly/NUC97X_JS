

#ifndef STM32F4xx_ECC_H
#define STM32F4xx_ECC_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 STM32F4xx_ECC_Correct_Data(u8 *Data,u32 *WriteECC,u32 *ReadECC);//NAND错误数据修正;返回:0=没错误,1=1位错误,2=无法校正的2位以上错误
u32 ECC_TEST(void);//ECC计算和校验测试(数据中的每字节按位取反测试,循环次数=长度*8);返回：0=正确,1=错误

	 
#ifdef __cplusplus
 }
#endif
#endif


