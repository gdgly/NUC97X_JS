#ifndef Terminal_ReadMeter_ShH_H
#define Terminal_ReadMeter_ShH_H


u32 ShH_Rx(u32 PORTn);//�Ϻ���Լ֡����;����0û���,1���,2��ʱ
u32 ShH_RxData(u32 PORTn);//�Ϻ���Լ֡��������,�ȽϷ��յ�ַ�������롢��0x33;����0��ȷ,1��ַ��,2�������
void Terminal_ReadMeter_ShH(u32 PORTn);//�ն˳���RS485��_�Ϻ���Լ
void Terminal_ReadMeter_ShH_Fn(u32 PORTn,u8 *p8RMLib);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
void Read_ShH_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//�Ϻ���Լ������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ


#endif
