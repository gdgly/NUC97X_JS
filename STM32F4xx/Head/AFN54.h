

#ifndef AFN54_H
#define AFN54_H
#if (USER/100)==14//����ϵ
#define ADDR_AFN54F1     ADDR_AFN54_Start  //�ն��ⲿ���ݲɼ�ģʽ
//�ն��ⲿ���ݲɼ�ģʽ����  BS8 1
#define ADDR_AFN54F2     (ADDR_AFN54F1+1)//�ն��ŵ�����
//�ն�ͨ�Ŷ˿ں�(1-31)  31*5byte
//�ŵ���;-�ŵ�����  									BS8  1
//�ŵ����õ�Э����(�����֣��ݲ�ʹ��)  BS8  1
//ͨ�ſ�����   												BS8  1
//����ͨ��ģ��Ӧ�ó�ʱʱ��						BS8  1
//���յȴ����ĳ�ʱʱ��								BIN  1
#define ADDR_AFN54F3     (ADDR_AFN54F2+31*5)//�ն˱��ĸ�ʽ��
//���ĸ�ʽ��   1byte    
#define ADDR_AFN54F4     (ADDR_AFN54F3+1)//�ն˹㲥��������
//�㲥����˿ں�(1-31)  31*100(70byte����֡)
//ִ�н��						BIN	1  00:ʧ��  01:�ɹ�  ����:δִ��
//����ʱ��						TIM	6
//����ʱ��						TIM	6
//ͨ�Ŷ�������				BIN	1
//�㲥ģʽ						BIN	1
//�㲥ʱ��						BIN	1
//����ʱ������				BIN	2
//�㲥���ĸ�ʽ��			BIN	1
//�㲥���ĳ���N				BIN	1
//�㲥��������(����)	BIN	N
#define ADDR_AFN54F5     (ADDR_AFN54F4+31*100)//���񷽰�����(�����������񼰶�ʱ����)
#define LEN_AFN54F5_Pn	26//ÿ���������洢����26
#define ADDR_AFN04F205   ADDR_AFN54F5
#define LEN_AFN04F205_Pn  LEN_AFN54F5_Pn
#define MAX_FUJ_TASK	6000//����������������
//������������ 14byte
//��������-��չ	BIN	1		0:�������� 1:��ʱ����
//���񷽰���		BIN	2
//ͨ�Ŷ˿ں�		BIN	1
//�������ȼ�		BIN	1
//��������			BIN	2
//ִ�ж���			BIN	1
//����ִ����Чʱ��	BIN	1
//ʧ�����Դ���	BIN	1
//�������ݴ洢����	BIN	1
//��ȫ��֤ģʽ	BIN	1
//������				BIN	2

//��ʱ����  26byte
//��������-��չ	BIN	1		0:�������� 1:��ʱ����
//���񷽰���	BIN	2
//ͨ�Ŷ˿ں�	BIN	1
//�������ȼ�	BIN	1
//�����ظ�ִ��������ֵ	BIN	1
//�����ظ�ִ�����ڵ�λ	BS8	1
//ʧ������������ֵ	BIN	1
//ʧ���������ڵ�λ	BS8	1
//ʧ�����Դ���	BIN	1
//�������ݴ洢����	BIN	1
//����ִ�п�ʼʱ��	TIM	6
//����ִ�н���ʱ��	TIM	6
//��ȫ��֤ģʽ			BIN	1
//������						BIN	2
#define ADDR_AFN54F10     (ADDR_AFN54F5+LEN_AFN54F5_Pn*MAX_FUJ_TASK)//���������ϱ�����
//���񷽰���(1-60000)
//�����ϱ�ģʽ	BS8	1
//�����ϱ���ʽ	BIN	1
#define ADDR_AFN54_TASK_LIST     (ADDR_AFN54F10+2*MAX_FUJ_TASK)//���񷽰���������������
//#define ADDR_AFN54_TASK_LIST_F		ADDR_AFN54_TASK_LIST//���񷽰��洢��Ч��־MAX_FUJ_TASK/8
//#define LEN_AFN54_TASK_LIST_F			8192
#define ADDR_AFN54_TASK_LIST_M		ADDR_AFN54_TASK_LIST//���񷽰��洢LEN_FuJTaskInfo*65535
#define ADDR_AFN54_TASK_LIST_S		(ADDR_AFN54_TASK_LIST_M+3*65535)//���巽���洢7*65535,�����Ժ���չ
#define MAX_PER_SUB_TASK	2040//ÿ�����񷽰���Ӧ�ľ���������
//���񷽰���������������
#define ADDR_AFN54F8     (ADDR_AFN54_TASK_LIST+(3+7)*65535)//������������
#define ADDR_AFN04F208   ADDR_AFN54F8
#define LEN_AFN54F8_Pn	100//ÿ���������洢����100
#define LEN_AFN04F208_Pn  LEN_AFN54F8_Pn
#define MAX_FUJ_SUB_TASK	50000//����������������--1000�ı�������
//��������									100byte
//��������洢���					BIN		2//����Ϊ50000
//��������� 								BIN		2//ͬһ���������ظ�����ͬ���������ظ�
//ͨ�Ŷ�������							BIN		1
//�������ͨ�ŵ�ַ					ADDR	6
//֡���										BS8		1
//�������顢����ʱ��������	BS8		1
//ʱ������ֵ								BS8		1
//���б��ĸ�ʽ��						BIN		1
//�����������ݱ��ĳ���L			BIN		1
//������������(����)				BIN		L(<100-18)
//���񷵻�����Ԥ�Ʊ��ĳ���	BIN		1
//���ر��ĸ�ʽ��						BIN		1
#define ADDR_AFN54F11     (ADDR_AFN54F8+LEN_AFN54F8_Pn*MAX_FUJ_SUB_TASK)//�ӽڵ�����ע����������
//����ע�������˿�(1-31)
//��������ע���ͨ�Ŷ�������	BIN	1
//����ִ�п�ʼʱ��	TIM	6
//����ִ��ʱ��	BIN	2
#define ADDR_AFN54F20     (ADDR_AFN54F11+31*9)//�ӽڵ��¼������ϱ�����
//���б���ģ��ͨ���ŵ�����ģʽ����	BIN	1
#define ADDR_AFN54F22     (ADDR_AFN54F20+1)//����������ִ�к������ϱ�����
//����������ִ�к������ϱ�������	BS8	1
#define ADDR_AFN54F23     (ADDR_AFN54F22+1)//����ͨ��ģ��ͨ��Э���л�
#define ADDR_AFN04F223   ADDR_AFN54F23
//����˿ں�(1-31)
//����ͨ��ģ��ͨ��Э�鹤������	BIN	1
#define ADDR_AFN54F50     (ADDR_AFN54F23+31*1)//����ͨ��ģ��ͨ��Э���л�
#define MAX_POWER_RATE_CTRL	100
#define MAX_54F50_LEN	60
//���ʿ��Ʒ�������
//���ʿ������񷽰���	BIN	1
//���Ʒ���������Դ������	BIN	2
//���Ʒ���ͨ�Ŷ����ַ	BIN	6
//��������ʼʱ��	TIM	6
//�����������ʱ��	TIM	6
//���Ʒ���ÿ����Чʱ��
//ÿ����Чʱ��	BS8	1
//�ܿ��ִ�	BIN	1
//����ʱ��	BIN	1
//������ֵ
//������	BIN	2
#define ADDR_AFN56F11_RS485_1			(ADDR_AFN54F50+MAX_POWER_RATE_CTRL*MAX_54F50_LEN)//2·485�˿�
#define LEN_PerAFN56F11	8
//ִ��״̬									BIN		1  0x77�ȴ��ж� 0x33���� 0x22ִ�� 0x11д�������־λ 0x00����
//ͨ�ŵ�ַ����							BIN		2
//ͨ�Ŷ�������							BIN		1
//��������ע������ʼʱ��	TIM		6
//��������ע���������ʱ��	TIM		6
//ͨ�Ŷ������1							BIN		2
//ͨ�Ŷ����ַ1							Addr	6
//............							.......
//ͨ�Ŷ������N							BIN		2
//ͨ�Ŷ����ַN							Addr	6
#define ADDR_AFN56F11_RS485_2			(ADDR_AFN56F11_RS485_1+16+LEN_PerAFN56F11*MaxRS485User)//2·485�˿�
#define ADDR_AFN56F11_Carrier			(ADDR_AFN56F11_RS485_2+16+LEN_PerAFN56F11*MaxRS485User)

#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier+16+LEN_PerAFN56F11*MaxRS485AddCarrierPn-3)//������;����3�ֽ�,
//#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier+16+LEN_PerAFN56F11*MaxRS485AddCarrierPn)//��ȷ����,ע������ļ�¼����
//�ϱ�3�ж����д�,ÿ����1���ֽ�
//#define ADDR_AFN56F11_RS485_2			(ADDR_AFN56F11_RS485_1+1+15+LEN_PerAFN56F11*MaxRS485User)//2·485�˿�
//#define ADDR_AFN56F11_Carrier			(ADDR_AFN56F11_RS485_2+1+15+LEN_PerAFN56F11*MaxRS485User)

//#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier-2+15+LEN_PerAFN56F11*MaxRS485AddCarrierPn)
//12byte�ն��ʲ���

#define ADDR_AFN01Fx_FuJ	(ADDR_Terminal_Number+12)
//1byte ��ʼ��ָ��-F4��ʼ��ʱ����ز�ģ����г�ʼ�� 0x55-F4��ʼ��
#define ADDR_ATMYSYSINFO	(ADDR_AFN01Fx_FuJ+1)
//1byte AT����:ǿ������,2=2G, 3=3G, 4=4G, ����=������
#define ADDR_AFN54_LogStartTime    (ADDR_ATMYSYSINFO+1) //��־��¼��ʼ����ʱ��   6�ֽ�  ������ʱ  (���� ����)
#define ADDR_AFN54_End    ((ADDR_AFN54_LogStartTime+6+0x1ffff)&0xfffe0000) //������ַ128K����
#if (ADDR_AFN54_End-ADDR_AFN54_Start)>LEN_Terminal_EFLASH_AFN54
	#error (ADDR_AFN54_End-ADDR_AFN54_Start)>LEN_Terminal_EFLASH_AFN54
#endif
#endif
#endif



