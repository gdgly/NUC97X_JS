


#ifndef Info_3762_H
#define Info_3762_H
#ifdef __cplusplus
 extern "C" {
#endif


u8* UART_3762_AFNxFn(u32 AFN,u32 Fn);//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���: AFN,Fn;����:���ݵ�Ԫָ�� 
void UART_3762_AFN10F21(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//��ѯ����������Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
void UART_3762_AFN10F31(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//��ѯ��λ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
void UART_3762_AFN03F101(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
void UART_3762_AFN5F6(u32 PORTn,u32 EnableFlag);//��������ʶ�𿪹� 0���ر� 1����
void UART_3762_AFN5F16(u32 PORTn,u8* data);// ��������ͨѶƵ��
void UART_3762_AFN5F200(u32 PORTn,u8* data);//�ܾ��ڵ��ϱ�ʹ�ܱ�־����ֹ��0����ʹ�ܣ�1��

void READ_Info(void);//����ز���F209_6���õ�TI���ڶ�����������Ϣ,��������Ϣ��(����Ҫ���ڶ�ȡʱ�����ö�ȡ��TI����=0)
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
