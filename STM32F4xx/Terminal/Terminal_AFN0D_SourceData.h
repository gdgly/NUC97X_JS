

#ifndef Terminal_AFN0D_SourceData_H
#define Terminal_AFN0D_SourceData_H



void Init_Class2SourceData(u32 Pn);//��2ԭʼ���ݳ�ʼ��(ָ��Pn����)
void Init_Class2SourceData_Command(void);//��2ԭʼ���ݳ�ʼ��(����,ȫ�����Ϊ0xff)


void Source_Full_CongealBuff(u32 Addr_CongealBuff,u32 CongealType,u32 ADDR_CongealFlags,u32 YMD,u32 Full0xee);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
u32 LastSource_Full_CongealBuff(u32 Addr_CongealBuff,u32 LastYMD);//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��

#endif




