
#ifndef Terminal_ReadMeter_HX_H
#define Terminal_ReadMeter_HX_H

//extern void Uart_HX_RxTx(u32 PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
extern void Read_HX_6E_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
extern u32 HX_Rx(u32 PORTn);//DL645֡����;����0û���,1���,2��ʱ
extern u32 HX_6E_RxData(u32 PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��

#endif
