
//AFN04��չ
#ifndef AFN04Ext_H
#define AFN04Ext_H

#define ADDR_AFN04F104     ADDR_AFN04Ext_START//���߶����ܶȲ���(�������)(ͬ��const u8 CurveSetFnList[])
//������1
	//AFN0DF73 �ܼ����й��������߶����ܶȲ��� 1
	//AFN0DF74 �ܼ����޹��������߶����ܶȲ��� 2
	//AFN0DF75 �ܼ����й����������߶����ܶȲ��� 3
	//AFN0DF76 �ܼ����޹����������߶����ܶȲ��� 4
	//AFN0DF81 �������й��������߶����ܶȲ��� 5
	//AFN0DF82 ������A���й��������߶����ܶȲ��� 6
	//AFN0DF83 ������B���й��������߶����ܶȲ��� 7
	//AFN0DF84 ������C���й��������߶����ܶȲ��� 8
	//AFN0DF85 �������޹��������߶����ܶȲ��� 9
	//AFN0DF86 ������A�޹��������߶����ܶȲ��� 10
	//AFN0DF87 ������B�޹��������߶����ܶȲ��� 11
	//AFN0DF88 ������C�޹��������߶����ܶȲ��� 12
	//AFN0DF89 ������A���ѹ���߶����ܶȲ��� 13
	//AFN0DF90 ������B���ѹ���߶����ܶȲ��� 14
	//AFN0DF91 ������C���ѹ���߶����ܶȲ��� 15
	//AFN0DF92 ������A��������߶����ܶȲ��� 16
	//AFN0DF93 ������B��������߶����ܶȲ��� 17
	//AFN0DF94 ������C��������߶����ܶȲ��� 18
	//AFN0DF95 ����������������߶����ܶȲ��� 19
	//AFN0DF97 �����������й��ܵ��������߶����ܶȲ��� 20
	//AFN0DF98 �����������޹��ܵ��������߶����ܶȲ��� 21
	//AFN0DF99 �����㷴���й��ܵ��������߶����ܶȲ��� 22
	//AFN0DF100 �����㷴���޹��ܵ��������߶����ܶȲ��� 23
	//AFN0DF101 �����������й��ܵ���ʾֵ�����ܶȲ��� 24
	//AFN0DF102 �����������޹��ܵ���ʾֵ�����ܶȲ��� 25
	//AFN0DF103 �����㷴���й��ܵ���ʾֵ�����ܶȲ��� 26
	//AFN0DF104 �����㷴���޹��ܵ���ʾֵ�����ܶȲ��� 27
	//AFN0DF105 �������ܹ������������ܶȲ��� 28
	//AFN0DF106 ������A�๦�����������ܶȲ��� 29
	//AFN0DF107 ������B�๦�����������ܶȲ��� 30
	//AFN0DF108 ������C�๦�����������ܶȲ��� 31
	//AFN0DF109 �������ѹ��λ�����߶����ܶȲ��� 32
	//AFN0DF110 �����������λ�����߶����ܶȲ��� 33
	//AFN0DF138 ֱ��ģ�����������߶����ܶȲ��� 34
	//AFN0DF145 ������1�����޹��ܵ���ʾֵ���߶����ܶȲ��� 35
	//AFN0DF146 ������4�����޹��ܵ���ʾֵ���߶����ܶȲ��� 36
	//AFN0DF147 ������2�����޹��ܵ���ʾֵ���߶����ܶȲ��� 37
	//AFN0DF148 ������3�����޹��ܵ���ʾֵ���߶����ܶȲ��� 38
//...
//������n
#define LEN_AFN04F104_PerPn     38
#define ADDR_AFN04F105     ADDR_AFN04F104+(LEN_AFN04F104_PerPn*AFN04F104MaxPn)//���ܱ����ݷּ��������(�������)
//������1
//���ݷּ����BIN 1 (1-MaxMeterEventClass)
//...
//������n
#define ADDR_AFN04F106     ADDR_AFN04F105+(1*AFN04F105MaxPn)//���ܱ����ݷּ�����(���ݷּ����)
//���ݷּ����1
//��1 �����ܱ����ݱ�ʶBIN 4 ��ӦDL/T 645
//��1 �����ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
//����
//��n �����ܱ����ݱ�ʶBIN 4 ��ӦDL/T 645
//��n �����ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
//...
//���ݷּ����N
//��1 �����ܱ����ݱ�ʶBIN 4 ��ӦDL/T 645
//��1 �����ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
//����
//��n �����ܱ����ݱ�ʶBIN 4 ��ӦDL/T 645
//��n �����ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
#define NUM_AFN04F106_PerC     96//ÿ�ּ���洢�ĵ��ܱ����ݱ�ʶ����(������=LEN_DATABUFF/5)
#define ADDR_AFN04F107     ADDR_AFN04F106+((5*NUM_AFN04F106_PerC)*AFN04F106MaxPn)//���ܱ����ݷּ����ڲ���(���ݵȼ���)
//���ݵȼ���1
//������ֵBIN 1
//���ڵ�λBIN 1
//..
//���ݵȼ���N

#define ADDR_AutoReportWord     ((ADDR_AFN04F107+(2*AFN04F107MaxPn)+0xfff)&0xfffff000)//���ܱ������ϱ�״̬��
//AFN04F10�������1
//96byte �����ϱ��¼���������hex(ƫ�ƶ�Ӧ״̬�ֵ�λ��,��ʼ��ֵ0)
//1BYTE ��96BYTE�����ݻ�(��ʼ��ֵ0)
//12byte �����ϱ��¼���λ��־�Ĵ�,���ڷ���λ����(��ʼ��ֵ0)
//...
//AFN04F10�������n
#define LEN_AutoReportWord     (96+1+12)
#define ADDR_MeterEventCount     ADDR_AutoReportWord+(LEN_AutoReportWord*MaxRS485AddCarrierPn)//�Ĵ���ܱ���¼�ԭ��������
//������1
//3byte ��1�����ܱ��¼����ݱ�ʶԭ��������BCD(��ʼ��ֵ0xff)
//1byte ��������hex(��ʼ��ֵ0xff)
//...
//3byte ��n�����ܱ��¼����ݱ�ʶԭ��������BCD(��ʼ��ֵ0xff)
//1byte ��������hex(��ʼ��ֵ0xff)

//3byte 0x03060000//39 B68������Դʧ��������3byte���ۼ�ʱ��
//3byte 0x03050000//45 B74ȫʧѹ�������3byte���ۼ�ʱ��

//...
//������n
#define NUM_MeterEventDI     (60+6)//�����ı�ʶ����
#define LEN_MeterEventCount     ((4*NUM_MeterEventDI)+6)//ÿ�����㳤��

#define ADDR_MeterEvent          ((ADDR_MeterEventCount+(LEN_MeterEventCount*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//��ȡ���Ǳ��¼�
//���ܱ��¼�1
//1byte �˿ں�(1,2,3,4��31)
//6byte ���ܱ�ͨ�ŵ�ַ(BCD)

//1byte ����(ֵ4+3��4+6��4+18,����ֵ��Ч;ר���ڷ��������洢)//�Ϻ�:����(ֵ4+3��4+6��4+18��4+3+4+3����ֵ��Ч,�������������ۼ�ʱ��,4+3+4+3�е��������ۼ�ʱ�����ݱ�ʶ������)
//2byte �ܳ���(1-10���¼����ݴ洢)

//22byte ���ݱ�ʶ������(FLASH�ռ�̶�����22byte;ר���ڷ��������洢)//�Ϻ�:�������������ۼ�ʱ��
//256byte �����ݱ�ʶ����,���ݳ������ݱ�ʶ������,...(256��1030)//�Ϻ�:�������ݱ�ʶ����,���ݳ��ȵ�Ԫ
//...
//���ܱ��¼�256
#define LEN_MeterEvent     (10+22+256)//(10+22+1030)//���ܴ���LEN_UARTnTx���������ϱ��᲻���ϱ���ѭ��

#define ADDR_ACSampleCurve     ((ADDR_MeterEvent+(LEN_MeterEvent*256)+0xfff)&0xfffff000)//�ڲ�����1���ӵ�ѹ����,������ʱ��������ʱ�ֶ�00��1��1��0ʱ0�ּ���洢λ��(��ʼ��ʱ�Զ�ѡ������������Ϊ0xffֵ)
//5byte ������ʱ��
//2byte A���ѹ����
//2byte B���ѹ����
//2byte C���ѹ����
#define LEN_ACSampleCurve     11//ÿ���ֽ���

	#if ((USER/100)==8)//�û���ʶ��ͨ�ð�����
#define ADDR_AFN04Ext1_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
		#if ((ADDR_AFN04Ext1_END)-(ADDR_AFN04Ext_START))>0x000E0000
			#error "((ADDR_AFN04Ext1_END)-(ADDR_AFN04Ext_START))>0x000E0000"
		#endif
#define ADDR_AFN0DExt_Start     (ADDR_EFLASH_Start+0x007E0000)
//3byte �ն���������ʱ�� Td_d ������
#define MaxDate_AFN0DF321F322     (31)//AFN0DF321��F322���洢����
#define ADDR_AFN0DF321_Ext     (ADDR_AFN0DExt_Start+3)//����ʱ��������ͳ������P0
//2byte ʱ�䳬�� 5min ����������
//2byte ����ʱ����ֵδ����5min���������� 
#define ADDR_AFN0DF322_Ext     (ADDR_AFN0DF321_Ext+4)//����ʱ��������ͳ����ϸPn
//3byte ������ n ���ʱ��
		#if (3+4+MaxRS485AddCarrierPn*3)*MaxDate_AFN0DF321F322>0x00020000
			#error "(3+4+MaxRS485AddCarrierPn*3)*MaxDate_AFN0DF321F322>0x00020000"
		#endif
#define ADDR_AFN0DExt_END     (ADDR_AFN0DExt_Start+0x00020000)
#define ADDR_AFN04Ext_END     (ADDR_AFN0DExt_END)
	#else
#define ADDR_AFN04Ext_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
	#endif


#if ((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000
	#error "((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000"
#endif
//#define ADDR_AFN04Ext_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
//
//
//#if ((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000
//	#error "((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000"
//#endif


#endif






















