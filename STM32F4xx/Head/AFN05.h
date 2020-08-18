

#ifndef AFN05_H
#define AFN05_H



//�ն�AFN05�����������ݴӴ˿�ʼ
#define ADDR_AFN05F1     ADDR_AFN05_Start//ң����բ(�����ִ�)
#define ADDR_AFN05F2     ADDR_AFN05F1//�����բ(�����ִ�)
//1byte 0=���,0x55=Ͷ��,������ƺ�0x5A
//�澯��ʱʱ��	�޵�ʱ��	BIN	1
#define ADDR_AFN05F9     ADDR_AFN05F1+(2*MaxControlRound)//ʱ�ι���Ͷ��(�ܼ����)
#define ADDR_AFN05F17     ADDR_AFN05F9//ʱ�ι��ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��
//ʱ�ι���Ͷ���־	  BS8	1
//ʱ�ι��ض�ֵ������ BIN	l
#define ADDR_AFN05F10     ADDR_AFN05F9+(3*MaxTotalAddGroup)//���ݹ���Ͷ��(�ܼ����)
#define ADDR_AFN05F18     ADDR_AFN05F10//���ݹ��ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F11     ADDR_AFN05F10+(1*MaxTotalAddGroup)//Ӫҵ��ͣ����Ͷ��(�ܼ����)
#define ADDR_AFN05F19     ADDR_AFN05F11//Ӫҵ��ͣ���ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F12     ADDR_AFN05F11+(1*MaxTotalAddGroup)//��ǰ�����¸���Ͷ��(�ܼ����)
#define ADDR_AFN05F20     ADDR_AFN05F12//��ǰ�����¸��ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��,������ƺ�0x5A
//��ǰ�����¸��ض�ֵ����ʱ��	BIN	min	1
//��ǰ�����¸��ض�ֵ����ϵ����	���ݸ�ʽ04	��	1
//�غ��ܼ��й����ʶ�����ʱʱ��	BIN	min	1
//��ǰ�����¸��صĿ���ʱ��	BIN	0.5h	1
//��ǰ�����¸��ص�1�ָ澯ʱ��	BIN	min	1
//��ǰ�����¸��ص�2�ָ澯ʱ��	BIN	min	1
//��ǰ�����¸��ص�3�ָ澯ʱ��	BIN	min	1
//��ǰ�����¸��ص�4�ָ澯ʱ��	BIN	min	1
#define ADDR_AFN05F15     ADDR_AFN05F12+(9*MaxTotalAddGroup)//�µ��Ͷ��(�ܼ����)
#define ADDR_AFN05F23     ADDR_AFN05F15//�µ�ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F16     ADDR_AFN05F15+(1*MaxTotalAddGroup)//�����Ͷ��(�ܼ����)
#define ADDR_AFN05F24     ADDR_AFN05F16//����ؽ��(�ܼ����)
//1byte 0=���,0x55=Ͷ��

#define ADDR_AFN05F25     ADDR_AFN05F16+(1*MaxTotalAddGroup)//�ն˱���Ͷ��
#define ADDR_AFN05F33     ADDR_AFN05F25//�ն˱�����
//1byte 0=���,0x55=Ͷ��,���붨ʱ��0x5A
//1byte �������ʱ��	    BIN	  0.5h	    1
#define ADDR_AFN05F26     ADDR_AFN05F25+2//�߷Ѹ澯Ͷ��
#define ADDR_AFN05F34     ADDR_AFN05F26//�߷Ѹ澯���
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F27     ADDR_AFN05F26+1//�����ն�����վͨ��
#define ADDR_AFN05F35     ADDR_AFN05F27//��ֹ�ն�����վͨ��
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F28     ADDR_AFN05F27+1//�ն��޳�Ͷ��
#define ADDR_AFN05F36     ADDR_AFN05F28//�ն��޳����
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F29     ADDR_AFN05F28+1//�����ն������ϱ�
#define ADDR_AFN05F37     ADDR_AFN05F29//��ֹ�ն������ϱ�
//1byte 0=���,0x55=Ͷ��
#define ADDR_AFN05F30     ADDR_AFN05F29+1//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
//1byte 0x55
//1byte 0=�رգ�1=����������ֵ�ر�
#define ADDR_AFN05F31     ADDR_AFN05F30+2//F31��ʱ����
//������Ҫ������ִ��
#define ADDR_AFN05F32     ADDR_AFN05F31+6//F32������Ϣ
//202*16��ͨ��Ϣ16��;1byte+1byte+200byte
//202*16��Ҫ��Ϣ16��;1byte+1byte+200byte	

#define ADDR_AFN05F38     ADDR_AFN05F32+(202*16*2)//�����ն�������վ
#define ADDR_AFN05F39     ADDR_AFN05F38//�����ն˶Ͽ�����
//1byte 0x55=�����ն�������վ,0xAA=�����ն˶Ͽ�����,ִ�к���0

#define ADDR_AFN05F41     ADDR_AFN05F38+1//����������Ͷ��(�������)
//1byte 0x55=�յ�������,ִ�к���0
//��������	  BSl6	 2
#define ADDR_AFN05F42     ADDR_AFN05F41+(3*AFN05F41MaxPn)//�����������г�(�������)
//1byte 0x55=�յ�������,ִ�к���0
//��������	  BSl6	 2
#define ADDR_AFN05F49     ADDR_AFN05F42+(3*AFN05F42MaxPn)//����ָ��ͨ�Ŷ˿���ͣ����
#define ADDR_AFN05F50     ADDR_AFN05F49//����ָ��ͨ�Ŷ˿ڻָ�����
//1byte ͨ�Ŷ˿�1(0=�ָ�,0x55=��ͣ)
//...
//1byte ͨ�Ŷ˿�n(0=�ָ�,0x55=��ͣ)
#define ADDR_AFN05F51     ADDR_AFN05F49+(1*31)//����ָ��ͨ�Ŷ˿����³���
//1byte ͨ�Ŷ˿�1(0x55=�յ�������,�������³����0x5A,��ɺ���0)
//...
//1byte ͨ�Ŷ˿�n(0x55=�յ�������,�������³����0x5A,��ɺ���0)
#define ADDR_AFN05F52     ADDR_AFN05F51+(1*31)//��ʼ��ָ��ͨ�Ŷ˿��µ�ȫ���м�·����Ϣ
//1byte ͨ�Ŷ˿�1(0x55=�յ�������,ִ�к���0)
//...
//1byte ͨ�Ŷ˿�n(0x55=�յ�������,ִ�к���0)
#define ADDR_AFN05F53     ADDR_AFN05F52+(1*31)//ɾ��ָ��ͨ�Ŷ˿��µ�ȫ�����
//1byte ͨ�Ŷ˿�1(0x55=�յ�������,ִ�к���0)
//...
//1byte ͨ�Ŷ˿�n(0x55=�յ�������,ִ�к���0)

#define ADDR_AFN05F103     ADDR_AFN05F53+(1*31)//��������������δ֪���(����ϵ��չ)
//1byte 0=���,0x55=�յ�������
//2byte ������������ʱ�䣨���ӣ�	BIN�����ֽ���ǰ	2
//1byte ִ�б�־	BS8	1

#define ADDR_AFN05F149     ADDR_AFN05F103+4//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
//1byte 0=���,0x55=�յ�������
//1byte �Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
//1BYTE 0=���,0x55=�յ�������,0x5a=����ִ��,0xaa=ÿ��ִ��;RS485-1
//1BYTE 0=���,0x55=�յ�������,0x5a=����ִ��,0xaa=ÿ��ִ��;RS485-2
//1BYTE 0=���,0x55=�յ�������,0x5a=����ִ��,0xaa=ÿ��ִ��;RS485-3
//1BYTE 0=���,0x55=�յ�������,0x5a=����ִ��,0xaa=ÿ��ִ��;�ز�
#define ADDR_AFN05F150     ADDR_AFN05F149+6//��λ��������������־
//1byte 0x55=�յ�������,ִ�к���0

#define ADDR_kWCtrl1_FixedValue     ADDR_AFN05F150+1//��ǰ�����¸��ض�ֵ
//�ܼ���1
//2BYTE ���ݸ�ʽ2
//...
//�ܼ���N


//�ն�AFN10����ת������
#define MaxAFN10F12TaskType     2//���ת��F12����������(ֻ�����֤�Ͷ�ʱ)
#define LEN_AFN10F12TaskData    256//�������ת��F12�ڵ��������ݳ���
#define LEN_AFN10F12PerTask    (2+1+2+LEN_AFN10F12TaskData+2+(40*MaxRS485AddCarrierPn))//����ת��F12ÿ�����������ܳ���
#define ADDR_AFN10F12     (ADDR_kWCtrl1_FixedValue+(2*MaxTotalAddGroup))//����������·���������
//��������0
//�����ʽ	BIN	2
//��������	BIN	1
//�������ݳ���Ln	BIN	2
//��������	BIN	LEN_AFN10F12TaskData
//�������õ������n	BIN	2
//���1	BIN	8
//�����Կ����1	BIN	32
//���2	BIN	8
//�����Կ����2	BIN	32
//...
//���n	BIN	8
//�����Կ����n	BIN	32

//��������1
//...
//��������n
//...
#define ADDR_AFN10F13     ADDR_AFN10F12+(LEN_AFN10F12PerTask*MaxAFN10F12TaskType)//��������·������ִ��״̬
//��������0
//1BYTE 0=û���,1=���,����ֵ=ִ�д���
//...
//1BYTE ��n
//��������1
//...
//��������n
//...
#define ADDR_AFN05_End     (ADDR_AFN10F13+(MaxRS485AddCarrierPn*MaxAFN10F12TaskType))//AFN05���������ַ

#if (ADDR_AFN05_End-ADDR_AFN05_Start)>LEN_Terminal_EFLASH_AFN05
	#error (ADDR_AFN05_End-ADDR_AFN05_Start)>LEN_Terminal_EFLASH_AFN05
#endif

#endif

