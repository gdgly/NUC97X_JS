
#ifndef Terminal_ReadMeter_DL698_H
#define Terminal_ReadMeter_DL698_H


u32 DL698_Rx(u32 PORTn);//698��Լ֡����;����0û���,1���,2��ʱ
u32 DL698_RxData(u32 PORTn);//698��Լ֡��������,�ȽϷ��յ�ַ��������;����0��ȷ,1��ַ��,2�������
void Terminal_ReadMeter_DL698(u32 PORTn);//�ն˳���RS485��_�Ϻ���Լ
void Terminal_ReadMeter_DL698_Fn(u32 PORTn,u8 *p8RMLib);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)

#endif
