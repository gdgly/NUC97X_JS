

#ifndef Terminal_ReadMeter_DL645_H
#define Terminal_ReadMeter_DL645_H

void Uart_DL645_RxTx(u32 PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
void DL645_68H_Add33H_CS_16H(u32 TxBuffAddr);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
void Read_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
void Read_DL645_2007_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr,u32 Frames);//DL645-2007����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ,����֡��
void DL645_2007_TxData_04001503(u32 BuffAddr,u64 CommAddr);//(�ܳ�36�ֽ�)DL645-2007д����֡��λ�����ϱ�״̬��,���:Ҫ����Ļ����ַ,ͨ�ŵ�ַ
void ReadMeter_DL645_2007_TxCurveData(u32 BuffAddr,u32 DI,u64 CommAddr,u64 YMDHM);//DL645-2007��������������֡
void Read_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
void Read_DL645_1997_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-1997����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
u32 DL645_Rx(u32 PORTn);//DL645֡����;����0û���,1���,2��ʱ
u32 DL645_2007_RxData(u32 PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
u32 DL645_1997_RxData(u32 PORTn);//DL645_1997֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
void Terminal_BroadcastTime_DL645(u32 PORTn);//DL645�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0

#endif

