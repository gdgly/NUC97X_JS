


#ifndef EVENT_CALC_iMeter_H
#define EVENT_CALC_iMeter_H
#ifdef __cplusplus
 extern "C" {
#endif


	 
void EVENT_CALC_300F_3010(void);//��ѹ�����������¼�
	 
void EventCurrentvalue_NUM(u8* pADDR,u32 Phase);//�¼���ǰֵ��¼��������+1�����:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��)
void EventCurrentvalue_TIME(u8* pADDR,u32 Phase,u32 s);//�¼���ǰֵ��¼���ۼ�ʱ�䣬���:pADDR=��¼���ַ,Phase=��(b0=A��,b1=B��,b2=C��),s=ʱ����
void EVENT_CALC_3003(void);//�����־���¼�(�����¼�)


	 
 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
