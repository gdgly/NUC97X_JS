

#ifndef ATString_H
#define ATString_H


u32 ATRxSearch(u32 ADDR_String);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
u32 GetATRxString(u32 ADDR_StrBuff);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
u32 GetATRxByte(void);//�жϽ���,���㵱ǰp0��Ӧp1����δ�����ֽ���
void GPRS_Signal(u32 Addr);//GPRS�ź�ǿ��0-4


#endif
