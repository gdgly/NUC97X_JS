

#ifndef Terminal_Init_H
#ifdef __cplusplus
 extern "C" {
#endif


void Init_Terminal(void);//������ʼ��
void Init_ALLDATA(void);//�ն�ȫ�����ݳ�ʼ��(��0)

void Init_Terminal_SoftwareUpdate(void);//������³�ʼ��

void Init_EnergyInc_Souser_Pn(u32 Pn);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
void Init_Energy_Pn(u32 Pn);//���������µ�������Ϊ0(ָ����Pn����)
void Terminal_Init_Command(void);//�ն˳�ʼ������


#ifdef __cplusplus
 }
#endif
#endif
