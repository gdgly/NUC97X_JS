
#ifndef NUC97x_TMR_H
#define NUC97x_TMR_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
typedef struct
{
	u32 CSR;
	u32 CMPR;
	u32 DR;
}TMRx_TypeDef;

void Init_NUC97x_TMR2(void);//��ʼ��������ʱ��TMR2Ϊ��0��ʼ��1MHz�Ӽ���(���ڲ���)
u32 Get_NUC97x_TMR2_Count(void);//ȡTMR2Ϊ��0��ʼ��24λ1MHz�Ӽ���(���ڲ���)�ļ���ֵ,ÿ����ֵ��ʾ1us
void Init_NUC97x_TMR3(void);//��ʼ��������ʱ��TMR3Ϊ1ms�ظ��ж�
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
