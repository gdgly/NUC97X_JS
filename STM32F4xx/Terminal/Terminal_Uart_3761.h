

#ifndef UART3761_H
#define UART3761_H



//export
void Uart_3761_RxTx(u32 PORTn);//UART_3761���շ���
void Uart_3761_Data(u32 PORTn);//UART_3761����


//extern
u32 Uart_376_PW(u32 PORTn);//��Ϣ��֤���ֶ�PW;����0��Ч,1��Ч,4�ѷ�����֡

void Uart_AFN00(u32 PORTn);//ȷ��/����
void Uart_AFN01(u32 PORTn);//��λ
void Uart_AFN02(u32 PORTn);//��·�ӿڼ��
void Uart_AFN04(u32 PORTn);//��������
void Uart_AFN05(u32 PORTn);//��������
void Uart_AFN06(u32 PORTn);//�����֤����ԿЭ��
void Uart_AFN08(u32 PORTn);//���󱻼����ն������ϱ�
void Uart_AFN09(u32 PORTn);//�����ն�����
void Uart_AFN0A(u32 PORTn);//������ѯ
void Uart_AFN0B(u32 PORTn);//������������
void Uart_AFN0C(u32 PORTn);//��1����
void Uart_AFN0D(u32 PORTn);//��2����
void Uart_AFN0E(u32 PORTn);//��3����(�¼�)
void Uart_AFN0F(u32 PORTn);//�ļ�����
void Uart_AFN10(u32 PORTn);//����ת��
void Uart_AFN255(u32 PORTn);//��������(��չ)
void Uart_AFN255(u32 PORTn);//���а汾��Ϣ

void Forwarding(u32 PORTn);//����ת���������
void Cascading(u32 PORTn);//�����������
void Cascading_LoopChack(u32 PORTn);//��������Ѳ��
void AutoCascadingData(u32 PORTn);//�Զ��ϴ�����Ѳ�⵽������


void AFN04_Configure(void);//�ն�֧�ֵĲ�������,����:������ADDR_DATABUFF
void AFN05_Configure(void);//�ն�֧�ֵĿ�������,����:������ADDR_DATABUFF
void AFN0C_Configure(void);//�ն�֧�ֵ�1����������,����:������ADDR_DATABUFF
void AFN0D_Configure(void);//�ն�֧�ֵ�2����������,����:������ADDR_DATABUFF

#if (USER/100)==12//�Ĵ��û�
void Uart_AFN0CF15(u32 PORTn);//�Ĵ���չ��ȡSIM���ź�ǿ��
void Uart_AFN0CF2040(u32 PORTn);//2016��������б��Ĵ�ר����չԶ��ģ���ź�ǿ���ٲ�
void Uart_AFN0CF305(u32 PORTn);//͸������ִ��״̬
void Uart_AFN0CF306(u32 PORTn);//͸������ִ�н��
#endif
#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
void Uart_AFN0CF201(u32 PORTn);//�Ϻ���������չAFN0CF201��ȡ���һ�γ���ʧ�ܱ����P0
void Uart_AFN0CF202(u32 PORTn);//�Ϻ���������չAFN0CF202��ȡ���һ�γ������ܱ�ʧ�ܱ���ϢP0
#endif
#if (USER/100)==8//�û���ʶ��ͨ�ð�
void Uart_AFN0DF321(u32 PORTn);//����ʱ��������ͳ������
void Uart_AFN0DF322(u32 PORTn);//��������ʱ��������ͳ����ϸ
#endif
#if (USER/100)==6//�û���ʶ������
void Uart_AFN0DF40x(u32 PORTn);//AFN0DF400��F401��F402
#endif
#endif


