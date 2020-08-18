

#ifndef STM32F4xx_EXTI_H
#define STM32F4xx_EXTI_H
#ifdef __cplusplus
 extern "C" {
#endif

void Enable_EXTI(u32 PinName,u32 Trigger);//允许外部引脚中断;入口:Trigger=0下降沿中断,=1上升沿中断,=2边沿中断,(NUC97x增加 3=低电平中断,4=高电平中断)
void Disable_EXTI(u32 PinName);//取消外部引脚中断


#ifdef __cplusplus
 }
#endif
#endif


