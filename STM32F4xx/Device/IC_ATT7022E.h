

#ifndef ATT7022E_H
#define ATT7022E_H



void EMURW(void);//��дEMU(�Զ�����0������1)



u32 EMUReadWait(u32 Addr);//��EMU(ֱ�����ζ��������,������У��ʱ��)
void EMUWriteWait(u32 Addr,u32 Data);//дEMU(ֱ������д�������,������У��ʱд)



#endif
