

#ifndef AutoSearchMeter_H
#define AutoSearchMeter_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void AutoSearchMeter_RS485(u32 PORTn);//�Զ��ѱ�����-rs485:0=��,1=�ȴ�,2...����
void AutoSearchMeter_3762(u32 PORTn);//�Զ��ѱ�3762
void AutoSearchMeter_3762_TC(u32 PORTn);//�Զ��ѱ�3762-����
void AutoSearchMeter_3762_ES(u32 PORTn);//�Զ��ѱ�3762-����
void AutoSearchMeter_3762_01(u32 PORTn);//�Զ��ѱ�3762-��˹��
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
