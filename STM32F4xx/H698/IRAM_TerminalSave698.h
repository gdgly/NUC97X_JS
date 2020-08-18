
#ifndef IRAM_TerminalSave_H
#define IRAM_TerminalSave_H


#define ADDR_TFM_YMDHMS     ADDR_Terminal_SaveRam_Start//����ǰʱ��
//7byte hex��������ʱ����
//1byte 
#define ADDR_POWER_YMDHMS     ADDR_TFM_YMDHMS+8//�����¼��ж��������ʱʱ��
//7byte hex��������ʱ����
//1byte 
#define ADDR_TerminalCongeal_Time     ADDR_POWER_YMDHMS+8//�ϴζ���ʱ��(�̶���ַ,�Ǳ���ʱ�ô�ֵ�ж��ն��Ƿ��Ѷ��ᣬ�ն˶�������Ǳ��ᣬ����������0���Ǳ����)
//7byte hex��������ʱ����
//1byte �ϴ��ж϶���ʱ���ʵʱ����

#define ADDR_TerminalFMTimer     ADDR_TerminalCongeal_Time+8//�ն�FM��ʱ����ַ
//6BYTE �ϴμ�������ʱ������

//�붨ʱ��
//2BYTE 0 1 ��Ӽ��������
//2BYTE 2 2 
#define TFMSTIMER_NUM     2//�ն�FM���붨ʱ����

//�ֶ�ʱ��
//2BYTE 0 1 �ն˱���ֶ�ʱ��
//2BYTE 2 2 ���ü����·ֶ�ʱ��hex
//2BYTE 4 3 ����ģʽ��Ч�ֶ�ʱ��hex

//2BYTE 6 4 �ܼ���1��ǰ�����¸��صĿ���ʱ��ֶ�ʱ��
//2BYTE ...
//2BYTE 20 11 �ܼ���8��ǰ�����¸��صĿ���ʱ��ֶ�ʱ��

//2BYTE 22 12 �����ִ�1ң����բ�޵�ʱ��ֶ�ʱ��
//2BYTE ...
//2BYTE 36 19 �����ִ�8ң����բ�޵�ʱ��ֶ�ʱ��

//2BYTE 38 20 RS232����ת���ֶ�ʱ��
//2BYTE 40 21 RS485-1����ת���ֶ�ʱ��
//2BYTE 42 22 RS485-2����ת���ֶ�ʱ��
#define TFMMTIMER_NUM     (3+16+3)//�ն�FM�зֶ�ʱ����
#define ADDR_TerminalRealTimer     ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2)//ʵ�߶�ʱ��,����ʱ����Ч
#define TREALMTIMER_NUM     1//�ն�ʵ�߷ֶ�ʱ����
//1BYTE 0 0=����ʱ��δ����,1=����ʱ���Ѽ���
//1BYTE 1 ����
//2BYTE 2 TESAM��ԿʧЧ�ֶ�ʱ��


#define ADDR_2131     ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2)//����A���ѹ�ϸ���
//����2����ѹ�ϸ������ݣ���=structure
//{
//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
//  ���µ�ѹ�ϸ���         ��ѹ�ϸ���
//}
//��ѹ�ϸ��ʡ�=structure
//{
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
//��ѹ������   long-unsigned����λ��%�����㣺-2����
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
//}
#define LENmax_2131     48
#define ADDR_2132     ADDR_2131+LENmax_2131//����B���ѹ�ϸ���
//����2����ѹ�ϸ������ݣ���=structure
//{
//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
//  ���µ�ѹ�ϸ���         ��ѹ�ϸ���
//}
//��ѹ�ϸ��ʡ�=structure
//{
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
//��ѹ������   long-unsigned����λ��%�����㣺-2����
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
//}
#define LENmax_2132     48
#define ADDR_2133     ADDR_2132+LENmax_2132//����C���ѹ�ϸ���
//����2����ѹ�ϸ������ݣ���=structure
//{
//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
//  ���µ�ѹ�ϸ���         ��ѹ�ϸ���
//}
//��ѹ�ϸ��ʡ�=structure
//{
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
//��ѹ������   long-unsigned����λ��%�����㣺-2����
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
//}
#define LENmax_2133     48
#define ADDR_2140     ADDR_2133+LENmax_2133//������й����ʼ�����ʱ��
//����ʼ�����ʱ���=structure
//{
//  �����ֵ double-long-unsigned��
//  ����ʱ��   date_time_s
//}
//���ʵ�λ��kW�����㣺-4
#define LENmax_2140     15
#define ADDR_2141     ADDR_2140+LENmax_2140//������й����ʼ�����ʱ��
//����ʼ�����ʱ���=structure
//{
//  �����ֵ double-long-unsigned��
//  ����ʱ��   date_time_s
//}
//���ʵ�λ��kW�����㣺-4
#define LENmax_2141     15

#define ADDR_2200     ADDR_2141+LENmax_2141//ͨ������
//��ֵ��=structure
//{
//����ͨ������  double-long-unsigned��
//����ͨ������  double-long-unsigned
//}
//��λ��byte�����㣺0
#define LENmax_2200     12
#define ADDR_2200_EventRecordFags     ADDR_2200+LENmax_2200//ͨ�����������¼��Ѽ�¼��־,�¸���ʱ��0
//1byte 0=û��¼,1=�Ѽ�¼

#define ADDR_2203     ADDR_2200_EventRecordFags+1//����ʱ��
//��ֵ��=structure
//{
//�չ����ۼ�ʱ��  double-long-unsigned��
//�¹����ۼ�ʱ��  double-long-unsigned
//}
//��λ�����ӣ����㣺0
//1byte �ϴμ��㹩��ʱ���ʵʱ�ӷ�
#define LENmax_2203     13
#define ADDR_2204     ADDR_2203+LENmax_2203//��λ�ۼƴ���
//��ֵ��=structure
//{
//�ո�λ�ۼƴ���  long-unsigned��
//�¸�λ�ۼƴ���  long-unsigned
//}
#define LENmax_2204     8


//��������
#define ADDR_impEnergy_Start     ADDR_2204+LENmax_2204
#define ADDR_impEnergy_Remainder     ADDR_impEnergy_Start//�����������
//4BYTE �������1((������*10000)/��ȳ���)������hex
//...
//4BYTE �������NUMmax_IMPIN((������*10000)/��ȳ���)������hex
#define LEN_impEnergy_Remainder     (4*NUMmax_IMPIN)
#define ADDR_impEnergy     ADDR_impEnergy_Remainder+LEN_impEnergy_Remainder//�����������ʾֵ
//�������1��=array ����ʾֵ
//�������1��=double-long-unsigned 4λС��
//...
//�������NUMmax_IMPIN��=array ����ʾֵ
//�������NUMmax_IMPIN��=double-long-unsigned 4λС��
#define LENper_impEnergy     (2+(5*(TMaxTariff+1)))
#define ADDR_impEnergy_Day     ADDR_impEnergy+(LENper_impEnergy*NUMmax_IMPIN)//����������յ�����
//�������1��=array ������
//�������1��=double-long-unsigned 4λС��
//...
//�������NUMmax_IMPIN��=array ������
//�������NUMmax_IMPIN��=double-long-unsigned 4λС��
#define ADDR_impEnergy_Month     ADDR_impEnergy_Day+(LENper_impEnergy*NUMmax_IMPIN)//����������µ�����
//�������1��=array ������
//�������1��=double-long-unsigned 4λС��
//...
//�������NUMmax_IMPIN��=array ������
//�������NUMmax_IMPIN��=double-long-unsigned 4λС��
#define ADDR_impDemand     ADDR_impEnergy_Month+(LENper_impEnergy*NUMmax_IMPIN)//���嵱�����������ʱ��,�¸�����0
//�������1�ܼ�����������������=array �������������ʱ��
//�������������ʱ���=structure
//{
//  �������ֵ CHOICE��double-long [5]��double-long-unsigned  [6]
//  ����ʱ��   date_time_s
//}
//...
//�������NUMmax_IMPIN�ܼ�����������������=array �������������ʱ��
#define LENper_impDemand     (2+(15*(TMaxTariff+1)))
#define ADDR_impEnergy_End     ADDR_impDemand+(LENper_impDemand*NUMmax_IMPIN)



//�ܼ���
#define ADDR_230x_Sour     ADDR_impEnergy_End//�ܼӵ���������ԭʼ����ʾֵ
//1byte ���г�Ա���ݸ���
//6+4*9*(TMaxTariff+1) byte ���ܱ��ַ,ԭʼ����ʾֵ1 ���� ���� ���� ����
//...
//6+4*9*(TMaxTariff+1) byte ���ܱ��ַ,ԭʼ����ʾֵn ���� ���� ���� ����
//������ 0.1WH
#define LENsig_230x_sour	(6+4*9*(TMaxTariff+1))//186
#define LENper_230x_Sour    (1+LENsig_230x_sour*NUMmax_230x)//1489
#define LENmax_230x_Sour	(LENper_230x_Sour*NUMmax_TAG)//
#define ADDR_230x_7     ADDR_230x_Sour+LENmax_230x_Sour//�ܼ����й�����
//�ܼ����й�������=array ������
//��������=long64
//������ 0.1WH
#define LENper_230x_7     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_7     (LENper_230x_7*NUMmax_TAG)
#define ADDR_230x_8     ADDR_230x_7+LENmax_230x_7//�ܼ����޹�����
//�ܼ����޹�������=array ������
//��������=long64
#define LENper_230x_8     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_8     (LENper_230x_8*NUMmax_TAG)
#define ADDR_230x_9     ADDR_230x_8+LENmax_230x_8//�ܼ����й�����
//�ܼ����й�������=array ������
//��������=long64
//������ 0.1WH
#define LENper_230x_9     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_9     (LENper_230x_9*NUMmax_TAG)
#define ADDR_230x_10     ADDR_230x_9+LENmax_230x_9//�ܼ����޹�����
//�ܼ����޹�������=array ������
//��������=long64
#define LENper_230x_10     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_10     (LENper_230x_10*NUMmax_TAG)
#define ADDR_230x_11     ADDR_230x_10+LENmax_230x_10//�ܼ�ʣ��������ѣ�
//�ܼ�ʣ�����(��)��=long64
#define LENper_230x_11     9
#define LENmax_230x_11     (LENper_230x_11*NUMmax_TAG)


#define ADDR_AutoSearchMeter_Start     (ADDR_230x_11+LENmax_230x_11)//�Զ��ѱ��ַ
//6BYTE RS485-1��ǰ�ѱ��ַ
//1BYTE RS485-1��ǰ�ѱ������B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
//6BYTE RS485-2��ǰ�ѱ��ַ
//1BYTE RS485-2��ǰ�ѱ������B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
//6BYTE RS485-3��ǰ�ѱ��ַ
//1BYTE RS485-3��ǰ�ѱ������B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
#define LEN_PerPortAutoSearchMeter      7
#define ADDR_AutoSearchMeter_End     (ADDR_AutoSearchMeter_Start+(3*LEN_PerPortAutoSearchMeter))



#define ADDR_MeterAutoReport_3011_Count     ADDR_AutoSearchMeter_End//���ܱ�ͣ�������ϱ��¼���¼��ţ�˳�����ɣ�
//4BYTE
#define ADDR_EventCurrentvalue_3003     ADDR_MeterAutoReport_3011_Count+4//�¼���ǰֵ��¼��-���ܱ�����¼�
//��ǰֵ��¼���=array structure
//{
//	�¼��������� double-long-unsigned��
//	�¼��ۼ�ʱ�� double-long-unsigned 
//}
//����Ԫ�ذ�˳���Ӧ�¼���¼��1���¼���¼��2���¼���¼��3���¼���¼��4�����޸ü�¼�����Ԫ��ΪNULL


#define LENper_EventCurrentvalue     50//ÿ�¼���ǰֵ��¼���ֽ���
#define NUM_EventCurrentvalue     1//�¼���ǰֵ��¼�����(������0)


#if (USER/100)==15//����
	#define ADDR_2131_OneDay   		ADDR_EventCurrentvalue_3003+(LENper_EventCurrentvalue*NUM_EventCurrentvalue)//�洢ÿ���յ�A���ѹ�ϸ��ʴ洢������,����ÿ�������
	#define LENmax_2131_OneDay     (48*NUM_RMmax)
	#define ADDR_Terminal_SaveRam_End     ADDR_2131_OneDay+(LENmax_2131_OneDay*3)
#endif

#ifndef ADDR_Terminal_SaveRam_End
	#define ADDR_Terminal_SaveRam_End     ADDR_EventCurrentvalue_3003+(LENper_EventCurrentvalue*NUM_EventCurrentvalue)
#endif





#endif



























