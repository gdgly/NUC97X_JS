

#ifndef Terminal_Uart_3761_COM_H

u32 Uart_376_Tp(u32 Tpaddr);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
u32 DItoFnPn(u32 PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
void FnPntoDI(u32 DIaddr,u32 FnPn);//FnPn���Ϊ���ݱ�ʶ
void Uart_376_ACK(u32 PORTn,u32 ACK);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,���ݵ�Ԫ��ʶ
void Uart_376_AUX(u32 PORTn,u32 rxTpaddr);//������Ϣ(EC,TP),�ݵȴ���֡���ݷ���
void Uart_376_AUXTp(u32 PORTn,u32 rxTpaddr);//������Ϣ(Tp),�ݵȴ���֡���ݷ���
void Uart_376_NoAUX(u32 PORTn);//û������Ϣ,�ݵȴ���֡���ݷ���
u32 Uart_376_CS(u32 buffaddr);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
void CopyDI(u32 PORTn,u32 DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
u32 Uart_3761_Rx(u32 PORTn);//UART_3761����;����0û���,1���,2��ʱ

u32 CheckUartData_0xEE(u32 DataAddr,u32 Byte);//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE

#endif

