
//����GPRSģ��
#ifndef GPRS_3763_H
#define GPRS_3763_H
#ifdef __cplusplus
 extern "C" {
#endif



void GPRS_3763_NextATCount(void);//��1��AT
void LineATIntRx(u32 ADDR_ATIntRx,u32 ADDR_BUFF,u32 Byte);//ȡAT�жϽ����������Է��ڻ�����

void UnInit_GPRS_3763(void);//ȫ��GPRSģ������Ϊ����������
void GPRS_3763_RxTx(void);//GPRS_3763ģ����շ���


#ifdef __cplusplus
 }
#endif
#endif


