

#ifndef Terminal_ReadMeter_3762_H
#define Terminal_ReadMeter_3762_H

void DisplayRouterVer(u32 PORTn);//��ʾ·�ɳ��ҺͰ汾��
void Terminal_ReadMeter_3762(u32 PORTn);//�ն˳���Q/GDW376.2
void Terminal_RouterSyn(u32 PORTn);//·��������ͬ��
void Terminal_RouterCtrl(u32 PORTn);//·��������
void RouterBroadcastTime(u32 PORTn);//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
void Terminal_RouterInfo(u32 PORTn);//��·����Ϣ,���:Ҫ���Ĳ����������Terminal_Router->RouterInfoPn,��Terminal_Router->RouterInfo=3=�����

void ClrRMTaskComplete_3762(u32 RMTask,u32 All);//��˿ڳ���������ɱ�־
#endif


