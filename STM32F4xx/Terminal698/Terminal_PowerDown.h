

#ifndef PowerDown_H
#define PowerDown_H
#ifdef __cplusplus
 extern "C" {
#endif





void RM_Meter_Event3106(void);//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
void Terminal_PowerDown(void);//2=��������ת��ع���
void Event_Power(void);//�ն�ͣ/�ϵ��¼�
	 
void PowerEventTimerComp(u8* pEVENT);//ͣ�ϵ��¼�ʱ��Ƚ�,���:pEVENTָ��698��ʽ�¼��Ŀ�ʼ,����:PowerEvent->bit






#ifdef __cplusplus
}
#endif
#endif


