

#ifndef Terminal_ReadMeter_Event_H
#define Terminal_ReadMeter_Event_H



void RMEveneTimer(u32 PORTn);//�ּ������ܱ��¼���ʱ
u32 NewRMEvene(u32 PORTn);//�»�ָ������ܱ��¼�����;����0=û�½���,1=�½���
void DelRMEvene(u32 PORTn);//ɾ�������ܱ��¼�����
void StoreRMEvene(u32 PORTn);//�ݴ����ڶ����ܱ��¼�������
void ClrRMEvene(u32 PORTn);//������ܱ��¼�
#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
void RMEveneTimer_ShH(u32 PORTn,u32 SuccessRate);//�Ϻ��������ּ������ܱ��¼���ʱ
#endif


#endif
