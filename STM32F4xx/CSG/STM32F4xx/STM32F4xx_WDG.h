

#ifndef STM32F4xx_WDG_H
#define STM32F4xx_WDG_H
#ifdef __cplusplus
 extern "C" {
#endif

void WWDT_Enable_Feed(u32 TimerOutMS);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
void WWDT_RESET(void);//����ʹ�ܿ��Ź���λ

#ifdef __cplusplus
 }
#endif
#endif
