

#ifndef Uart_3762_RxTx_H
#define Uart_3762_RxTx_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 UART_3762_Rx(u32 PORTn);//UART 3762����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
u32 Uart_3762_RxTx(u32 PORTn);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���;����:0=��������,1=������
u32 Uart_3762_CS(u32 ADDR_BUFF);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
void Uart_3762_TxData(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
u32 Uart_3762_AFN13F1_Rx(u32 PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������

u32 Get_Uart_3762_RxData_AFN_Fn(u32 PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
u32 Uart_3762_RxData_AFN00_F1(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data);//UART_3762����AFN00F1ȷ������,���AFN,Fn,LEN_Data����ʧ���ط��ò���;����0δȷ��1ȷ��2ʧ��
void Uart_3762_RxData(u32 PORTn);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���

	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif

