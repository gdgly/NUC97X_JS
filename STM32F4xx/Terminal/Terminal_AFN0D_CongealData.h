

#ifndef Terminal_AFN0D_CongealData_H
#define Terminal_AFN0D_CongealData_H




void Init_Class2Data_UpPower(void);//��2���ݳ�ʼ��(�ϵ�)
void Init_Class2Data_Command(void);//��2���ݳ�ʼ��(����,ȫ�����)
void Class2Congeal_AutoRW(void);//��2�����Զ�IO��д����0,��ɺ�������
void Terminal_Class2Data_Computer(void);//�ն���2���ݼ���


void AFN0C_Full_CurveCongeal_Buff(u32 Addr_CongealBuff);//AFN0Cʵʱ�����������߶��Ỻ��
void AFN0C_Full_DateCongeal_Buff(u32 Addr_CongealBuff);//AFN0Cʵʱ���������ն��Ỻ��
void AFN0C_Full_RMDateCongeal_Buff(u32 Addr_CongealBuff,u32 ADDR_CongealFlags);//AFN0Cʵʱ�������볭���ն��Ỻ��;���:������ֶ˿ںŶ����־�Ѽ���
void AFN0C_Full_MonthCongeal_Buff(u32 Addr_CongealBuff);//AFN0Cʵʱ���������¶��Ỻ��


#endif
