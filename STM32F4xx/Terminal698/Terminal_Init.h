

#ifndef Terminal_Init_H
#define Terminal_Init_H
#ifdef __cplusplus
 extern "C" {
#endif



void Init_Terminal(void);//������ʼ��
void Init_Terminal_SoftwareUpdate(void);//������³�ʼ��
void FLASHtoSDRAM(void);//����FLASHtoSDRAM
void DL698_INIT(u32 Command);//��ʼ��,���Command:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��


#ifdef __cplusplus
 }
#endif
#endif
