
//5xx=�Ϻ�ϵ


#ifndef Terminal_H
#define Terminal_H




//�汾-------------------------------------------------------------------------------------
#define I0_UpDataInit     0//3//I0������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
#define I1_UpDataInit     0//3//I1������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
#define I2_UpDataInit     0//3//I2������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
#define E1_UpDataMeterInit     0//3//E1��������Ǳ��ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
#define E1_UpDataTerminalInit     0//3//E1��������ն˳�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
//#define E2_UpDataInit     0//E2������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��
//#define E3_UpDataInit     0//E3������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ,6=�����ݲ�����ʼ��


#define EnProject     1//0=��,1=��;�������ʱ�Ƿ��б���Ŀ����
#define EnHV     1//0=��,1=��;�������ʱ�Ƿ��б�Ӳ���汾
#define EnUSER     0//1//0=��,1=��;�������ʱ�Ƿ��б��û���ʶ
#define EnSV     0//0=��,1=��;�������ʱ�Ƿ��б�����汾��,������ʱֻ��>=��ǰ�汾�Ų���Ч
#define EnMeter     0//0=��,1=��;I1���ܱ��������ʱ�Ƿ��б��������߷�ʽ�ȵ����

#define SVER     0x0002//0x0001//����汾��:ZZ01
#define SVDATE     0x150906//0x150211//�����������:������
#define HVER     0x200//Ӳ���汾��:2.00
#define HVDATE     0x130301//Ӳ����������:������

//�汾����---------------------------------------------------------------------------------





#define LEN_DATABUFF     4096//��д���ݻ��峤��,��С4096




//UART------------------------------------------------------------------------------------------
#define RS232PORT     0//RS232ͨ�ſں�
#define GPRSPORT     1//GPRSͨ�ſں�
#define RS485_1PORT     2//RS485_1ͨ�ſں�
#define RS485_2PORT     3//RS485-2ͨ�ſں�
#define RS485_4PORT     4//RS485_4ͨ�ſں�(�ز�)
#define RS485_3PORT     5//RS485_3ͨ�ſں�
#define IRPORT     6//����ͨ�ſں�
#define TESAMPORT     7//�ն�ESAMͨ�ſں�
#define MESAMPORT     8//���ESAMͨ�ſں�
#define CARDPORT     9//���CPUCARDͨ�ſں�
#define ENETPORT     10//��̫��ͨ�ſں�
#define SMSPORT     11//����ͨ�ſں�
#define ACSAMPLEPORT     12//��������ͨ�ſں�(����)
#define EFTPCMDPORT     13//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define EFTPDATAPORT     14//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define GFTPCMDPORT     15//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define GFTPDATAPORT     16//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)

//UART0(RS232)
#define LEN_UART0IntRx     8192//UART0�жϽ��ջ��峤��
#define LEN_UART0Ctrl     200//UART0�������ݳ���
#define LEN_UART0Rx     8192//UART0���ջ��峤��
#define LEN_UART0Tx     2048//UART0���ͻ��峤��
#define LEN_UART0FnDataBuff     0//UART0����Fn���ݻ��峤��
//UART1(GPRS)
#define LEN_UART1IntATRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��AT����
#define LEN_UART1IntRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��
#define LEN_UART1Ctrl     200//UART1�������ݳ���
#define LEN_UART1Rx     8192//UART1���ջ��峤��
#define LEN_ATTx     64//AT���ͻ��峤��
#define LEN_UART1Tx     2048//UART1���ͻ��峤��
#define LEN_UART1FnDataBuff     0//UART1����Fn���ݻ��峤��
//UART2(RS485-1)
#define LEN_UART2IntRx     8192//UART2�жϽ��ջ��峤��
#define LEN_UART2Ctrl     200//UART2�������ݳ���
#define LEN_UART2Rx     8192//UART2���ջ��峤��
#define LEN_UART2Tx     8192//UART2���ͻ��峤��
#define LEN_UART2FnDataBuff     256//UART2����Fn���ݻ��峤��
//UART3(RS485-2)
#define LEN_UART3IntRx     8192//UART3�жϽ��ջ��峤��
#define LEN_UART3Ctrl     200//UART3�������ݳ���
#define LEN_UART3Rx     8192//UART3���ջ��峤��
#define LEN_UART3Tx     8192//UART3���ͻ��峤��
#define LEN_UART3FnDataBuff     256//UART3����Fn���ݻ��峤��
//UART4(RS485-4)(�ز�)
#define LEN_UART4IntRx     256//UART4�жϽ��ջ��峤��
#define LEN_UART4Ctrl     200//UART4�������ݳ���
#define LEN_UART4Rx     2048//UART4���ջ��峤��
#define LEN_UART4Tx     256//UART4���ͻ��峤��
#define LEN_UART4FnDataBuff     256//UART4����Fn���ݻ��峤��
//UART5(RS485-3)
#define LEN_UART5IntRx     8192//UART5�жϽ��ջ��峤��
#define LEN_UART5Ctrl     200//UART5�������ݳ���
#define LEN_UART5Rx     8192//UART5���ջ��峤��
#define LEN_UART5Tx     8192//UART5���ͻ��峤��
#define LEN_UART5FnDataBuff     256//UART5����Fn���ݻ��峤��
//UART6(����)
#define LEN_UART6IntRx     2048//UART6�жϽ��ջ��峤��
#define LEN_UART6Ctrl     200//UART6�������ݳ���
#define LEN_UART6Rx     8192//UART6���ջ��峤��
#define LEN_UART6Tx     2048//UART6���ͻ��峤��
#define LEN_UART6FnDataBuff     0//UART6����Fn���ݻ��峤��
//UART7(�ն�ESAM)
#define LEN_UART7IntRx     8//UART7�жϽ��ջ��峤��
#define LEN_UART7Ctrl     200//UART7�������ݳ���
#define LEN_UART7Rx     2048//UART7���ջ��峤��
#define LEN_UART7Tx     2048//UART7���ͻ��峤��
#define LEN_UART7FnDataBuff     0//UART7����Fn���ݻ��峤��
//UART8(���ESAM)
#define LEN_UART8IntRx     8//UART8�жϽ��ջ��峤��
#define LEN_UART8Ctrl     0//UART8�������ݳ���
#define LEN_UART8Rx     0//UART8���ջ��峤��
#define LEN_UART8Tx     0//UART8���ͻ��峤��
#define LEN_UART8FnDataBuff     0//UART8����Fn���ݻ��峤��
//UART9(���CPUCARD)
#define LEN_UART9IntRx     8//UART9�жϽ��ջ��峤��
#define LEN_UART9Ctrl     0//UART9�������ݳ���
#define LEN_UART9Rx     0//UART9���ջ��峤��
#define LEN_UART9Tx     0//UART9���ͻ��峤��
#define LEN_UART9FnDataBuff     0//UART9����Fn���ݻ��峤��
//UART10(Ethernet)
#define LEN_UART10IntRx     8192//UART10�жϽ��ջ��峤��
#define LEN_UART10Ctrl     200//UART10�������ݳ���
#define LEN_UART10Rx     8192//UART10���ջ��峤��
#define LEN_UART10Tx     2048//UART10���ͻ��峤��
#define LEN_UART10FnDataBuff     0//UART10����Fn���ݻ��峤��
//UART11(SMS)
#define LEN_UART11IntRx     1024//UART11�жϽ��ջ��峤��
#define LEN_UART11Ctrl     200//UART11�������ݳ���
#define LEN_UART11Rx     1024//UART11���ջ��峤��
#define LEN_UART11Tx     1024//UART11���ͻ��峤��
#define LEN_UART11FnDataBuff     0//UART11����Fn���ݻ��峤��
//UART12(�����)
#define LEN_UART12IntRx     8//UART12�жϽ��ջ��峤��
#define LEN_UART12Ctrl     200//UART12�������ݳ���
#define LEN_UART12Rx     2048//UART12���ջ��峤��
#define LEN_UART12Tx     256//UART12���ͻ��峤��
#define LEN_UART12FnDataBuff     256//UART12����Fn���ݻ��峤��

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART13IntRx     (6+1024+2)//UART13�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART13Ctrl     200//UART13�������ݳ���
#define LEN_UART13Rx     0//UART13���ջ��峤��
#define LEN_UART13Tx     1024//UART13���ͻ��峤��
#define LEN_UART13FnDataBuff     0//UART13����Fn���ݻ��峤��
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART14IntRx     (6+4096+2)//UART14�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART14Ctrl     200//UART14�������ݳ���
#define LEN_UART14Rx     0//UART14���ջ��峤��
#define LEN_UART14Tx     2048//UART14���ͻ��峤��
#define LEN_UART14FnDataBuff     0//UART14����Fn���ݻ��峤��
//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART15IntRx     8//UART15�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART15Ctrl     0//UART15�������ݳ���
#define LEN_UART15Rx     0//UART15���ջ��峤��
#define LEN_UART15Tx     0//UART15���ͻ��峤��
#define LEN_UART15FnDataBuff     0//UART15����Fn���ݻ��峤��
//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART16IntRx     8//UART16�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART16Ctrl     0//UART16�������ݳ���
#define LEN_UART16Rx     0//UART16���ջ��峤��
#define LEN_UART16Tx     0//UART16���ͻ��峤��
#define LEN_UART16FnDataBuff     0//UART16����Fn���ݻ��峤��

#define SocketPortnMin     1025//�����˿ں���Сֵ
#define SocketPortnMax     65535//�����˿ں����ֵ
//UARTEND---------------------------------------------------------------------------------------




//���ܱ�----------------------------------------------------------------------------------------
#define MMaxTariff     4//������
#define MeterHarmonic     0//г������0=û,1=��
#define MeterMaxLadder     6//������
#define MaxMonth     12//�¶�����
#define MaxMeterCurveCongeal     (96*10)//������ߵ���,60��������(����ʱ��Խ��)
#define MeterMaxDemandCalculate     1//����������㣺0=���巨,1=����ƽ����

#define MeterCtrl     0//�Ʒѿ���:0�޿���,1��Զ��,2���غ�Զ��
#define MeterType     0//�������:0��Ԥ���ѱ�,1������Ԥ���ѱ�,2�����Ԥ���ѱ�
#define MeterSpec     3//���ܱ���
//���ܱ���
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#if MeterSpec==0
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     6400//����й�����imp/kWh
#define MeterActiveConstbcd     0x6400//����й�����imp/kWh
#define MeterReactiveConst     6400//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x6400//����޹�����imp/kvarh
#define MeterUBhex     2200//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x2200//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif
#if MeterSpec==1
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     1//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     6400//����й�����imp/kWh
#define MeterActiveConstbcd     0x6400//����й�����imp/kWh
#define MeterReactiveConst     6400//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x6400//����޹�����imp/kvarh
#define MeterUBhex     1000//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x1000//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif
#if MeterSpec==2
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     20000//����й�����imp/kWh
#define MeterActiveConstbcd     0x20000//����й�����imp/kWh
#define MeterReactiveConst     20000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x20000//����޹�����imp/kvarh
#define MeterUBhex     577//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x577//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif
#if MeterSpec==3
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     6400//����й�����imp/kWh
#define MeterActiveConstbcd     0x6400//����й�����imp/kWh
#define MeterReactiveConst     6400//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x6400//����޹�����imp/kvarh
#define MeterUBhex     2200//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x2200//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif
#if MeterSpec==4
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     1//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     20000//����й�����imp/kWh
#define MeterActiveConstbcd     0x20000//����й�����imp/kWh
#define MeterReactiveConst     20000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x20000//����޹�����imp/kvarh
#define MeterUBhex     1000//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x1000//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif
#if MeterSpec==5
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     20000//����й�����imp/kWh
#define MeterActiveConstbcd     0x20000//����й�����imp/kWh
#define MeterReactiveConst     20000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x20000//����޹�����imp/kvarh
#define MeterUBhex     577//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x577//�αȵ�ѹxxx.x(V)
#define MeterIBhex     150//�αȵ���xx.xx(A)
#define MeterIBbcd     0x150//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#endif

//���ܱ����------------------------------------------------------------------------------------





//�ն�------------------------------------------------------------------------------------------
#define TMaxTariff     4//�ն���������
#define DefaultTariff     1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
#define USERTEST     0//0=��������,1=��ʡ���û����Ժ�ͼ�
#define UartDateTI     0//�ն�����ٶ�ʱ��0=���յ�24ʱ����,1=���յ�0ʱ����
#define UartMonthTI     0//�¶�����ٶ�ʱ��0=����δ��24ʱ����,1=���³�1�յ�0ʱ����
#define RMM_RS485_PassageZero     1//RS485ͨ�������0��;0=����ͣ,1=��ͣ
#define RMM_StartHM     0x0010//�𶯳���ʱ��
#define RMM_EndHM     0x2359//��������ʱ��
#define RMM_CompCongealTime     1//�����ܱ������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
#define RMM_RS485_FailData_0xEE     0//RS485����ʧ��������0xee;0=��,1=��
#define RMM_DateCongeal_0xEE     1//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
#define RMM_MonthCongeal_0xEE     1//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	#if RMM_CompCongealTime==1//�����ܱ������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			#error RMM_CompCongealTime==1,RMM_DateCongeal_0xEE==0
		#endif
	#endif
#define RMM_MonthCongealUserDate1     1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��;
	#if(RMM_MonthCongealUserDate1==1)
		#if(RMM_MonthCongeal_0xEE==0)
			#error RMM_MonthCongealUserDate1==1,RMM_MonthCongeal_0xEE==0
		#endif
	#endif
	
#define MaxRMTask     8//��󳭱�������(>=2)
#define PowerOnRMTask     1//��Դ����������;0=û��,1=��
#define DateRMTask        1//ÿ�ճ�������;0=û��,1=��
#define WeekRMTask        1//ÿ�ܳ�������;0=û��,1=��(��� ���ڳ����ز���07�������)(�ز��޸�Ϊ������ʧ��ʱΪ�ӿ������˳���������������)
#define TimerRMTask        1//��ʱ����ͣ���¼����;0=û��,1=��
#define HourTask     1//�ص㻧���߳���(���������߶���ĵ����)
#define MeterEventRMTask     1//���ڳ������ܱ��¼�����;0=û��,1=��

#define RMM_RS485     1//RS485ͨ������ģʽ:0=ѭ��ȫ������,1=����С������ó�������
#define RMM_RS485_DateCongeal     1//RS485ͨ�������ն���;0=��,1=��
#define RMM_Carrier_DateCongeal     1//�ز�ͨ�������ն���;0=��,1=��
#define RMM_RS485_IntegralCongeal     2//RS485ͨ������,�������1-24��������߶���;0=��,1=��1-24����,2=����
#define RMM_Carrier_IntegralCongeal     2//�ز�ͨ������,�������1-24��������߶���;0=��,1=��1-24����,2=����
#define RMM_Carrier_CurveTime     60//�ز�ͨ����������ߵ���������ʱ���;15�ֻ�60��
#define RMM_RS485_Congeal     1//RS485ͨ����������2-3�ն���;0=��,1=��
#define RMM_Carrier_Congeal     1//�ز�ͨ����������2-3�ն���;0=��,1=��
#define RMM_RS485_AddMonth     0//RS485ͨ����������2�¶���;0=��,1=��
#define RMM_Carrier_AddMonth     0//�ز�ͨ����������2�¶���;0=��,1=��
	#if((RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0)
		#if(RMM_MonthCongeal_0xEE==0)
			#error (RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0,RMM_MonthCongeal_0xEE==0
		#endif
	#endif
#define RMDateCongealSour 0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ

#define MaxRS485User     8//���RS485������(4��������)
#define MaxKeyUser     10//����г����ص��û���
#define MaxCarrierUser     1492//����ز�������(4��������)
#define MaxRS485Pn     MaxRS485User//���RS485�����������
#define MaxRS485AddKeyUserPn     (MaxRS485User+MaxKeyUser)//���RS485����+�ص��û���
#define MaxRS485AddCarrierPn     (MaxRS485User+MaxCarrierUser)//���RS485����+�ز�������
#define MaxDCPn     0//���ֱ��ģ������������
#define MaxCPn     0//�������������ò�������
#define MaxCCR     0//�����Ƶ�����Ͷ����
#define MaxTotalAddGroup     8//����ܼ�����
#define MaxControlRound     0//4//�������ִ�
#define MaxTimerTask     64//���ʱ������
#define MaxTimerTaskDI     8//���ʱ�������ݱ�ʶ����
#define MaxImpInPort     0//�����������˿���(<=8)
#define MaxSwitchInPort     5//��󿪹�������˿���(<=8,����������˿���+�Žӵ�)
#define MaxAnalogPort     6//����ѹ/����ģ������������(��ѹ���������Ŀǰ���ֻ��6)
#define MaxDiffGroup     8//����й��ܵ������������
#define MaxHourCongeal     4//���Сʱ�������ݸ���(�ܶ�)0=������;4=15,30,45,0;2=30,0;1=0;12=5,10,15...0;60=1,2,3...0
#define MaxDateCongeal     62//31//����ն�����
#define MaxRMDateCongeal     12//31//��󳭱��ն�����
#define MaxMonthCongeal     12//����¶�����
#define CurveCongealTime    60//15//���߶�����ʱ��(�ܶ�)0=������;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
#define MaxCurveCongeal     (96*10)//������ߵ���
#define MaxACSampleCurveCongeal     0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
#define MaxCurveCongeal_Ext     (288*11)//����ߵ�ѹ���ߵ���(�Ϻ���չ)
#define MaxERC     64//����¼�ERC��
#define MaxClassEventNum     15//���ÿ�������¼���¼����
#define F246MaxPn     4//����ն�������¼���ݲ�������
#define NewERC14     1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
#define EventProject     1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
#define EnMeterAutoAutoReport     0//������ܱ������ϱ��¼�:0=������,1=����
#define MaxMeterEventClass     8//�����ܱ��¼��ּ����
#define MaxMeterEventLevel     4//�����ܱ��¼��ּ���

#define GPRSENETAutoRePort     1//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
#define RS232AutoRePort     0//RS232�������ϱ�0=��ֹ,1=����
#define WaitHeartberatACKMinTimer     30//���������ȴ���վȷ����С��ʱ��
#define WaitHeartberatACKMaxTimer     180//600//���������ȴ���վȷ�����ʱ��
#define WaitHostACKMinTimer     5//�����ϱ��ȴ���վȷ����С��ʱ��
#define WaitHostACKMaxTimer     60//�����ϱ��ȴ���վȷ�����ʱ��
#define RouterCheckTimerVal     (120*60)//�ز�·���������붨ʱֵ
#define ReadDL645OverTimer     3000//DL645����ʱ��ʱֵms
#define SMSEQ2Byte     1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
#define SM_CMGF     0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
#define MulRealRead     0//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡

#define SoftBug1     0//0=��������,1=376.1���ܲ������BUG.Сʱ�����ܶ�
#define SoftBug3     1//���������������0=�Ƚ�������������,1=���Ƚ�������������
#define SoftBug4     0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
#define RMM_Type     0//���г�������:0=�ز���,1=RS485��ͨ���ɼ���,2=���
#define ErrorAutoDisplay     0//0=����,1=��ֹ;ϵͳ�����Զ�������ʾ
#define TESAMSPI     0//TESAM��SPI�ӿ�,0=��׼SPI0��,1=IO��
#define AFN04F150Dn     1//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
#define ReadF129Above    1//�ٶ�1��F129��������:0=��ԭ�洢����,1=ʵʱ��ȡ�����ϴ�
#define RMM_AFN0CF26     0//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
#define Uart3761Check0xEE   0//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
#define AFN0CF41F42F43F44     0//���յ�����AFN0CF41F42F43F44����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define AFN0CF45F46F47F48     0//���µ�����AFN0CF45F46F47F48����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define RS232DefaultBPS     (0xb+(6<<5))//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     (0xb+(4<<5))//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     (0xb+(3<<5))//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define DefaultClassNo     0x50//�Զ��ѱ�Ĭ�ϵ��û������С���

#define Extern_Uart_AFN255     1//�ⲿ����:0=I2,1=E1,2=E2,3=;��������(��չ)
#define Extern_Terminal_ReadMeter_DL645_1997     1//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳���DL645-1997
#define Extern_Terminal_ReadMeter_DL645_1997_Fn     1//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
#define Extern_BatchTask_RS485     1//�ⲿ����:0=I2,1=E1,2=E2,3=;����ת����������-RS485-ʱ�Ӷ�ʱ
#define Extern_BatchTask_3762     1//�ⲿ����:0=I2,1=E1,2=E2,3=;����ת����������-3762-ʱ�Ӷ�ʱ
#define Extern_W5200_FTP_RxTx     1//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 FTP�ļ�����
#define Extern_GPRS_FTP_RxTx     1//�ⲿ����:0=I2,1=E1,2=E2,3=;GPRS FTP�ļ�����
//#define Extern_CDMA_FTP_RxTx     1//�ⲿ����:0=I2,1=E1,2=E2,3=;CDMA FTP�ļ�����
#define Extern_Uart_AFN04     1//�ⲿ����:0=I2,1=E1,2=E2,3=;�������úͲ�ѯ
#define Extern_Uart_AFN05     1//�ⲿ����:0=I2,1=E1,2=E2,3=;��������
#define Extern_Uart_AFN09     1//�ⲿ����:0=I2,1=E1,2=E2,3=;�����ն�����
#define Extern_Terminal_ReadMeter_ShH     1//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳��� �Ϻ����Լ
#define Extern_Terminal_ReadMeter_ShH_Fn     1//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
#define Extern_ReadMeter_Fn_YesNo     1//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)

//�ն˽���--------------------------------------------------------------------------------------








//Ӳ���汾--------------------------------------------------------------------------------------
#if(HVER==0x100)//Ӳ���汾��1.00
#define CPUCLK     96//CPUʱ��cclk:12,24,48,96(MHz)
#define LCDPixels     3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
#define IC_LPC177x8x     1//0=û,1=��
#define IC_LPC176x     0//0=û,1=��
#define IC_SST25VF64C     0//0=û,1=��
#define IC_MX25L128     0//0=û,1=��
#define IC_MX25L256     0//0=û,1=��
#define IC_SerialFlash     (IC_SST25VF64C||IC_MX25L128||IC_MX25L256)//0=û,1=��
#define IC_SST39VF6401B     1//0=û,1=��
#define IC_S29GL01G     0//0=û,1=��
#define IC_S29GL512     1//0=û,1=��
#define IC_S29GL256     0//0=û,1=��
#define IC_S29GL128     0//0=û,1=��
#define IC_S29GLxxx     (IC_S29GL01G||IC_S29GL512||IC_S29GL256||IC_S29GL128)
#define IC_IS61LV25616     0//0=û,1=��
#define IC_MT48LC4M16     1//0=û,1=��
#define IC_DS3231     1//0=û,1=��
#define IC_W5200     1//0=û,1=��
#define IC_90E36     1//0=û,1=��
#define IC_ATT7022E     0//0=û,1=��
#define IC_UC1698     1//0=û,1=��(������ʾ)
#define IC_PCF8562     0//0=û,1=��(��ʽ��ʾ)
#define IC_CPUCARD     0//0=û,1=��
#define IC_MESAM     0//0=û,1=��
#define IC_TESAM     1//0=û,1=��
#define IC_24AAxxx     0//0=û,1=��
#define LOADSWITCH     0//���ܱ���Ŀ�ĸ��ɿ���0=�ڲ�,1=�ⲿ
#define IC_BGS2     1//GPRSģ��BGS2;0=û,1=��
#define IC_GPRS3763     0//GPRSģ��376.3��׼;0=û,1=��
#define IC_MC323     0//CDMA��ΪMC323ģ��;0=û,1=��
#define KEY_SET     0//���ð���0=û,1=��
#endif
#if(HVER==0x200)//Ӳ���汾��2.00
#define CPUCLK     96//CPUʱ��cclk:12,24,48,96(MHz)
#define LCDPixels     3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
#define IC_LPC177x8x     1//0=û,1=��
#define IC_LPC176x     0//0=û,1=��
#define IC_SST25VF64C     0//0=û,1=��
#define IC_MX25L128     0//0=û,1=��
#define IC_MX25L256     0//0=û,1=��
#define IC_SerialFlash     (IC_SST25VF64C||IC_MX25L128||IC_MX25L256)//0=û,1=��
#define IC_SST39VF6401B     1//0=û,1=��
#define IC_S29GL01G     0//0=û,1=��
#define IC_S29GL512     1//0=û,1=��
#define IC_S29GL256     0//0=û,1=��
#define IC_S29GL128     0//0=û,1=��
#define IC_S29GLxxx     (IC_S29GL01G||IC_S29GL512||IC_S29GL256||IC_S29GL128)
#define IC_IS61LV25616     0//0=û,1=��
#define IC_MT48LC4M16     1//0=û,1=��
#define IC_DS3231     1//0=û,1=��
#define IC_W5200     1//0=û,1=��
#define IC_90E36     1//0=û,1=��
#define IC_ATT7022E     0//0=û,1=��
#define IC_UC1698     1//0=û,1=��(������ʾ)
#define IC_PCF8562     0//0=û,1=��(��ʽ��ʾ)
#define IC_CPUCARD     0//0=û,1=��
#define IC_MESAM     0//0=û,1=��
#define IC_TESAM     1//0=û,1=��
#define IC_24AAxxx     1//0=û,1=��
#define LOADSWITCH     0//���ܱ���Ŀ�ĸ��ɿ���0=�ڲ�,1=�ⲿ
#define IC_BGS2     0//GPRSģ��BGS2;0=û,1=��
#define IC_GPRS3763     1//GPRSģ��376.3��׼;0=û,1=��
#define IC_MC323     1//CDMA��ΪMC323ģ��;0=û,1=��
#define KEY_SET     0//���ð���0=û,1=��
#endif
//Ӳ���汾����----------------------------------------------------------------------------------










#endif
