

#ifndef NUC97x_PWM_H
#define NUC97x_PWM_H
#ifdef __cplusplus
 extern "C" {
#endif



void On38KHz(void);//��38KHzʱ��
void Off38KHz(void);//��38KHzʱ��

u32 onSquareWave(u32 Hz);//������������;���:Hz=���ε�Ƶ��(Max=500000Hz,Min=1Hz),����:0=ʧ��,1=�ɹ�
void offSquareWave(void);//�ط���������

void onPWMRUNLED(u32 ms);//������LEDָʾ��(PWM2)(����USB����ʱָʾ),���:�������ʱ��ms
void offPWMRUNLED(void);//������LEDָʾ��(PWM2)(����USB����ʱָʾ),���:�������ʱ��ms
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
