

#ifndef RM_Event_DL2007_H
#define RM_Event_DL2007_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
u32 Get_Event645_DI(u32 OAD,u32 RMCount);//�õ�DL645�¼�������DI�����:OAD=�¼�OAD��RMCount=�����������(��0��ʼ)������:������DI(0=û��Ҫ���������ݱ�ʶ)
u32 Get_RxDATA_DL2007_MeterEventCount(u32 PORTn);//��645���¼���¼����֡��ȡ�����ܱ��¼���������(�¼���¼���);����:hex��������,û�ҵ�ʱ����0xffffffff
u32 Change_Frame_645_to_698(u32 PORTn);//��645���¼�����֡ת��698��ʽ
	 
	 
 
	 
#ifdef __cplusplus
 }
#endif
#endif
