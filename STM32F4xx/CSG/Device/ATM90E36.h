

#ifndef ATM90E36_H
#define ATM90E36_H



//-------------------------------------------------------------------------------------------------
// Function name:       EMURW
// Descriptions:        ����оƬEMU��д
// input parameters:    ��2����ͬ����,����0ϵͳռ��,��ϵͳ���ֻ��ʹ������1
                      //SSP1Task1:0=��,0x1=�ȴ���,0x2=����ʼ,0x3=���Ƚ�,0x4=�����,0x5=������;0x81=�ȴ�д,0x82=д��ʼ,0x83=д�Ƚ�,0x84=д���,0x85=д����
                      //SSP1Addr1��д���ݵ�ַ1
                      //SSP1Data1��д����1
// output parameters:   SSP1Task1,SSP1Data1
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void EMURW(void);//��дEMU(�Զ�����0������1)



u32 EMUReadWait(u32 Addr);//��EMU(ֱ�����ζ��������,������У��ʱ��)
void EMUWriteWait(u32 Addr,u32 Data);//дEMU(ֱ������д�������,������У��ʱд)
void EMURead(u32 Addr1,u32 Addr2);//��EMU�������,SSP1Task1Count=n��ʾ��n�����


#endif


























