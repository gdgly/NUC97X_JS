

#ifndef Terminal_AFN0C_RealTimeData_H
#define Terminal_AFN0C_RealTimeData_H


void Init_AFN0CF1(void);//��ʼ���ն���1����F1,�汾��Ϣ
void Init_Class1Data_UpPower(void);//��1���ݳ�ʼ��(�ϵ�)
void Init_Class1Data_Command(void);//��1���ݳ�ʼ��(����,ȫ�����)
void Terminal_Class1Data_Computer(void);//�ն���1���ݼ���
void HourCongeal_AutoRW(void);//Сʱ�����Զ�IO��д����0,��ɺ�������
void TotalAddEnergyInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//���������ܼӵ���������,���ֵ��DATABUFF
void PointkWhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//��������������й�����������,���ֵ��DATABUFF
void PointkvarhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//���������޹�����������,���ֵ��DATABUFF
void Terminal_AFN0CF2_Computer(void);//�ն���1����F2,�ն�����ʱ��


#endif


