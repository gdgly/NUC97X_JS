

#ifndef STM32F4xx_WDG_H
#define STM32F4xx_WDG_H
#ifdef __cplusplus
 extern "C" {
#endif

void WWDT_Enable_Feed(u32 TimerOutMS);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
void WWDT_RESET(void);//立即使能看门狗复位

#ifdef __cplusplus
 }
#endif
#endif
