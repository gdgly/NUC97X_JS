

#ifndef Data_Save_H
#define Data_Save_H

void PowerDown_ISR(void);//��Դ�����ж�
void PowerDown_Data_Save(void);//��Դ�������ݱ���
void Battery_Data_Save(void);//��ع���ʱ�ػ�ǰ���ݱ���
//void Timer_Data_Save(void);//��ʱ���ݱ���
void PowerUp_Data_Recovery(void);//��Դ�ϵ�ʱ���ݻָ�
void Power_Down_Display(void);//�������ݱ������ʱ����ʾ(��1mS�жϿ���ǰ����,����1mS�жϼ���)

#endif

