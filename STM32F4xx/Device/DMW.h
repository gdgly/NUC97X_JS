

#ifndef DMW_H
#define DMW_H

void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//ֱ�Ӵ洢��д(�޲���)
void DMC(u8 Data,u32 ADDR_Dest,u32 Byte);//ֱ�Ӵ洢�����(�޲���)
void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte);//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)


#endif

