

#ifndef Terminal_AFN0D_CongealData_Init_H
#define Terminal_AFN0D_CongealData_Init_H


void Init_Class2Data(u32 Pn);//��2���ݳ�ʼ��(ָ��Pn����)
void PnHourCongealCount(void);//�������Сʱ�������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
void PnCurveCongealCount(void);//����������߶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
void PnDateCongealCount(void);//��������ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
void PnRMDateCongealCount(void);//������ĳ����ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
void PnMonthCongealCount(void);//��������¶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������

u32 PnHourCongeal(u32 Pn);//������Сʱ������Ч,����=0��Ч,1=��Ч
u32 PnDateCongealNo(u32 CongealNo,u32 Pn);//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
u32 PnRMDateCongealNo(u32 CongealNo,u32 Pn);//�����㳭���ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
u32 PnMonthCongealNo(u32 CongealNo,u32 Pn);//�������¶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
u32 PnCurveCongealNo(u32 CongealNo,u32 Pn);//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)


#endif

