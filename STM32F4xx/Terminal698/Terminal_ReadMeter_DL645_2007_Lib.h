

#ifndef Terminal_ReadMeter_DL645_2007_Lib_H
#define Terminal_ReadMeter_DL645_2007_Lib_H


u32 Get_RM_DL645_2007_MaxFn(void);
u32 Get_RM_DL645_2007_FnCount(u32 Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
u32 DL645_2007_DI_To_RMCount(u32 DI);//�����ϱ��������ܱ���¼�������1-10�μ�¼�����ݱ�ʶתΪ�����ڳ�����ͬ��ʶ�ĵĳ������;����0xffff(65535)��ʾû�ҵ�

#endif

