

#ifndef STM32F4xx_EXTI_H
#define STM32F4xx_EXTI_H
#ifdef __cplusplus
 extern "C" {
#endif

void Enable_EXTI(u32 PinName,u32 Trigger);//�����ⲿ�����ж�;���:Trigger=0�½����ж�,=1�������ж�,=2�����ж�,(NUC97x���� 3=�͵�ƽ�ж�,4=�ߵ�ƽ�ж�)
void Disable_EXTI(u32 PinName);//ȡ���ⲿ�����ж�


#ifdef __cplusplus
 }
#endif
#endif


