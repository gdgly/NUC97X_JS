

#ifndef Meter07_Energy_ATT7022E_H
#define Meter07_Energy_ATT7022E_H


void Meter07_ActiveEnergy_Total(void);//合相有功电能
void Meter07_ActiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy);//分相有功电能,入口:相电能地址
void Meter07_ReactiveEnergy_Total(void);//合相无功电能
void Meter07_ReactiveEnergy_Phase(u32 REGaddr,u32 DirBit,u32 ADDR_Energy);//分相无功电能,入口:相电能地址
void Meter07_AH(void);//安时计算



#endif
