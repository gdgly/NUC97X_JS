

#ifndef NUC97x_PWM_H
#define NUC97x_PWM_H
#ifdef __cplusplus
 extern "C" {
#endif



void On38KHz(void);//开38KHz时钟
void Off38KHz(void);//关38KHz时钟

u32 onSquareWave(u32 Hz);//开方波发生器;入口:Hz=波形的频率(Max=500000Hz,Min=1Hz),返回:0=失败,1=成功
void offSquareWave(void);//关方波发生器

void onPWMRUNLED(u32 ms);//开运行LED指示灯(PWM2)(用于USB插入时指示),入口:输出周期时间ms
void offPWMRUNLED(void);//关运行LED指示灯(PWM2)(用于USB插入时指示),入口:输出周期时间ms
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
