

#ifndef STM32F4xx_DAC_H
#ifdef __cplusplus
 extern "C" {
#endif

void PLAY_DAC1(void);//DAC1为WAV音响输出,并使用DMA1的流5通道7

u32 onSineWave(u32 Hz);////开正弦波发生器(DAC1,DMA1的流5通道7);入口:Hz=波形的频率(Max=58333Hz,Min=1Hz),返回:0=失败,1=成功
void offSineWave(void);//关正弦波发生器(DAC1)
u32 onSquareWave(u32 Hz);//开方波发生器(DAC1,DMA1的流5通道7);入口:Hz=波形的频率(Max=500000Hz,Min=1Hz),返回:0=失败,1=成功
void offSquareWave(void);//关方波发生器(DAC1)
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif

