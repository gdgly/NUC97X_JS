

#ifndef PnMap_H
#define PnMap_H


//�ص㻧2����������Fn��ӦҪ����1��ʵʱ����Fn�ⶨ��
typedef __packed struct
{
  vu8 AFN0DFn;//�ص㻧����Fn
  vu8 AFN0CFn;//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
}KeyUserReadMeterLib_TypeDef;
u32 Get_KeyUserReadMeterLib_MaxFn(void);


void KeyUserPnList(void);//���г����ص㻧���߶���Pn�б�
u32 KeyUserMapPn(u32 Pn);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
u32 KeyUserPnSet(u32 Pn);//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList




#endif
