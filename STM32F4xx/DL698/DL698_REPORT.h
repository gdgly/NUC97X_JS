
#ifndef Terminal698_REPORT_H
#define Terminal698_REPORT_H
#ifdef __cplusplus
 extern "C" {
#endif

void REPORT_PORTn(msFILE_TypeDef *file);//ͨ�������ϱ�ʱ�����ϱ����ݸ���=��ǰ���ݸ�����ʹ��ADDR_DATABUFF
void REPORT_Notification(u32 PORTn);//�ϱ�֪ͨ(��ʱ����)
void EVENT_REPORT_Notification(u32 PORTn);//�ϱ�֪ͨ(�¼�)
void REPORT_Response(u32 PORTn);//�ϱ�Ӧ��
	 
	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
