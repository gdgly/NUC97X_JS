

#ifndef Meter07_Energy_ATT7022E_H
#define Meter07_Energy_ATT7022E_H


void Meter07_ActiveEnergy_Total(void);//�����й�����
void Meter07_ActiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy);//�����й�����,���:����ܵ�ַ
void Meter07_ReactiveEnergy_Total(void);//�����޹�����
void Meter07_ReactiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy);//�����޹�����,���:����ܵ�ַ
void Meter07_AH(void);//��ʱ����



#endif
