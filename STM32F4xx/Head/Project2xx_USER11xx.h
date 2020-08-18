
//����ϵ


#ifndef Terminal_H
#define Terminal_H



//�汾-------------------------------------------------------------------------------------
#define UpDataReset     0//������º�����:0=��,1=��(ע:��MS������ǿ������)
#define MS_UpDataInit     3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define OS_UpDataInit     0//3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define Meter_UpDataInit     3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define Terminal_UpDataInit     3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define Menu_UpDataInit     0//3//��������Ǳ��ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��

#define EnProject     0//1//0=��,1=��;�������ʱ�Ƿ��б���Ŀ����
#define EnHV     1//0=��,1=��;�������ʱ�Ƿ��б�Ӳ���汾
#define EnUSER     0//1//0=��,1=��;�������ʱ�Ƿ��б��û���ʶ
#define EnSV     0//0=��,1=��;�������ʱ�Ƿ��б�����汾��,������ʱֻ��>=��ǰ�汾�Ų���Ч
#define EnMeter     1//0=��,1=��;I1���ܱ��������ʱ�Ƿ��б��������߷�ʽ�ȵ����
#define EnMainProtocol     1//0=��,1=��;�������ʱ�Ƿ��б���ͨ��Э��


#ifdef IC_STM32F4xx
#define SVER     0x100//����汾��:1.00
#define SVDATE     0x190425//0x180620//0x170220//0x160812//�����������:������
//#define HVER     0x400//Ӳ���汾��:4.00
#define HVER     0x410//Ӳ���汾��:4.10;nandbusbit=8,LCD���߷ֿ�
#define HVDATE     0x160701//Ӳ����������:������
#endif
#ifdef IC_NUC9xx
#define SVER     0x100//����汾��:1.00
#define SVDATE     0x190312//0x180620//0x170220//0x160812//�����������:������
#define HVER     0x500//NUC972Ӳ���汾��:5.00
#define HVDATE   0x190308//NUC972Ӳ����������:������
#endif

#define emWin      0//emWin GUI;0=û,1=��
#define Font_ttf   0//ttf����;0=û,1=��

#define SAME_MS       0//0=��,1=��;�����ͬʱ���±�־
#define SAME_OS        0//0=��,1=��;�����ͬʱ���±�־
#define SAME_Meter     0//0=��,1=��;�����ͬʱ���±�־
#define SAME_Terminal  0//0=��,1=��;�����ͬʱ���±�־
#define SAME_Menu      0//0=��,1=��;�����ͬʱ���±�־
#define SAME_Font      0//0=��,1=��;�����ͬʱ���±�־

#ifdef IC_STM32F4xx
//0x08000000	16K BOOT
//0x08004000	16K 
//0x08008000	16K �ڲ����������Լ�������
//0x0800C000	16K NAND FLASH�Ƿ����
#define ADDR_MS_ENTRY        0x08010000//64K
#define ADDR_OS_ENTRY        0x08020000//512K
#define ADDR_Font_ENTRY      0x080A0000//256K
#define ADDR_Meter_ENTRY     0x080E0000//128K
#define ADDR_Terminal_ENTRY  0x08100000//256*3K
#define ADDR_Menu_ENTRY      0x081C0000//256K
#define ADDR_Test_ENTRY      ADDR_Menu_ENTRY//256K(ͬ�˵���ںͳ���)

#define LENmax_MS     		(64*1024)//�ռ䳤��
#define LENmax_OS     		(512*1024)//�ռ䳤��
#define LENmax_Font       (256*1024)//�ռ䳤��
#define LENmax_Meter      (128*1024)//�ռ䳤��
#define LENmax_Terminal   (768*1024)//�ռ䳤��
#define LENmax_Menu       (256*1024)//�ռ䳤��
#define LENmax_Test       LENmax_Menu//�ռ䳤��(ͬ�˵���ںͳ���)
#endif
#ifdef IC_NUC9xx
	#ifndef LINUX
//      BOOT                 0x00000000	16K
#define ADDR_MS_ENTRY        0x00004000//128K
#define ADDR_OS_ENTRY        0x00024000//768K
#define ADDR_Font_ENTRY      0x000E4000//256K
#define ADDR_Meter_ENTRY     0x00124000//256K
#define ADDR_Terminal_ENTRY  0x00164000//768K
#define ADDR_Menu_ENTRY      0x00224000//256K
#define ADDR_Test_ENTRY      ADDR_Menu_ENTRY//256K(ͬ�˵���ںͳ���)
														//0x00264000//����624K
#define LENmax_BOOT       (16*1024)//�ռ䳤��
#define LENmax_MS     		(128*1024)//�ռ䳤��
#define LENmax_OS     		(768*1024)//�ռ䳤��
#define LENmax_Font       (256*1024)//�ռ䳤��
#define LENmax_Meter      (256*1024)//�ռ䳤��
#define LENmax_Terminal   (768*1024)//�ռ䳤��
#define LENmax_Menu       (256*1024)//�ռ䳤��
#define LENmax_Test       LENmax_Menu//�ռ䳤��(ͬ�˵���ںͳ���)
	#else//#ifndef LINUX
#define ADDR_BOOT_ENTRY      0x02000000//0k
#define ADDR_MS_ENTRY        0x02000000//128K
//OS
#define ADDR_Font_ENTRY      0x02020000//256K
#define ADDR_Meter_ENTRY     0x02060000//256K
#define ADDR_Terminal_ENTRY  0x020A0000//768K
#define ADDR_Menu_ENTRY      0x02160000//256K
#define ADDR_Test_ENTRY      ADDR_Menu_ENTRY//256K(ͬ�˵���ںͳ���)
														//0x00260000//����640K
#define LENmax_BOOT       0//�ռ䳤��												
#define LENmax_MS     		(128*1024)//�ռ䳤��
#define LENmax_OS     		0//�ռ䳤��
#define LENmax_Font       (256*1024)//�ռ䳤��
#define LENmax_Meter      (256*1024)//�ռ䳤��
#define LENmax_Terminal   (768*1024)//�ռ䳤��
#define LENmax_Menu       (256*1024)//�ռ䳤��
#define LENmax_Test       LENmax_Menu//�ռ䳤��(ͬ�˵���ںͳ���)
	#endif
#endif
//�汾����---------------------------------------------------------------------------------





#define LEN_DATABUFF     4096//��д���ݻ��峤��,��С4096

//UART------------------------------------------------------------------------------------------
#define RS232PORT     0//RS232ͨ�ſں�
#define GPRSPORT     1//GPRSͨ�ſں�
#define RS485_1PORT     2//RS485_1ͨ�ſں�
#define RS485_2PORT     3//RS485-2ͨ�ſں�
#define RS485_4PORT     4//RS485_4ͨ�ſں�(�������ز�,ר�����ģ��)
#define RS485_3PORT     5//RS485_3ͨ�ſں�(WI-FI)
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
#define LEN_UART0Ctrl     256//UART0�������ݳ���
#define LEN_UART0Rx     8192//UART0���ջ��峤��
#define LEN_UART0Tx     2048//UART0���ͻ��峤��
#define LEN_UART0TxMax     2048//UART0���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART0Tx<=LEN_UART0TxMax)
#define LEN_UART0FnDataBuff     0//UART0����Fn���ݻ��峤��
//UART1(GPRS)
#define LEN_UART1IntATRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��AT����
#define LEN_UART1IntRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��
#define LEN_UART1Ctrl     256//UART1�������ݳ���
#define LEN_UART1Rx     8192//UART1���ջ��峤��
#define LEN_ATTx     64//AT���ͻ��峤��
#define LEN_UART1Tx     2048//UART1���ͻ��峤��
#define LEN_UART1TxMax     2048//UART1���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART1Tx<=LEN_UART1TxMax)
#define LEN_UART1FnDataBuff     0//UART1����Fn���ݻ��峤��
//UART2(RS485-1)
#define LEN_UART2IntRx     8192//UART2�жϽ��ջ��峤��
#define LEN_UART2Ctrl     256//UART2�������ݳ���
#define LEN_UART2Rx     8192//UART2���ջ��峤��
#define LEN_UART2Tx     8192//UART2���ͻ��峤��
#define LEN_UART2TxMax     8192//UART2���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART2Tx<=LEN_UART2TxMax)
#define LEN_UART2FnDataBuff     256//UART2����Fn���ݻ��峤��
//UART3(RS485-2)
#define LEN_UART3IntRx     8192//UART3�жϽ��ջ��峤��
#define LEN_UART3Ctrl     256//UART3�������ݳ���
#define LEN_UART3Rx     8192//UART3���ջ��峤��
#define LEN_UART3Tx     8192//UART3���ͻ��峤��
#define LEN_UART3TxMax     8192//UART3���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART3Tx<=LEN_UART3TxMax)
#define LEN_UART3FnDataBuff     256//UART3����Fn���ݻ��峤��
//UART4(RS485-4)(�ز�)
#define LEN_UART4IntRx     256//UART4�жϽ��ջ��峤��
#define LEN_UART4Ctrl     256//UART4�������ݳ���
#define LEN_UART4Rx     2048//UART4���ջ��峤��
#define LEN_UART4Tx     256//UART4���ͻ��峤��
#define LEN_UART4TxMax     256//UART4���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART4Tx<=LEN_UART4TxMax)
#define LEN_UART4FnDataBuff     256//UART4����Fn���ݻ��峤��
//UART5(RS485-3)(WI-FI)
#define LEN_UART5IntRx     8192//UART5�жϽ��ջ��峤��
#define LEN_UART5Ctrl     256//UART5�������ݳ���
#define LEN_UART5Rx     8192//UART5���ջ��峤��
#define LEN_UART5Tx     8192//UART5���ͻ��峤��
#define LEN_UART5TxMax     8192//UART5���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART5Tx<=LEN_UART5TxMax)
#define LEN_UART5FnDataBuff     256//UART5����Fn���ݻ��峤��
//UART6(����)
#define LEN_UART6IntRx     2048//UART6�жϽ��ջ��峤��
#define LEN_UART6Ctrl     256//UART6�������ݳ���
#define LEN_UART6Rx     8192//UART6���ջ��峤��
#define LEN_UART6Tx     2048//UART6���ͻ��峤��
#define LEN_UART6TxMax     2048//UART6���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART6Tx<=LEN_UART6TxMax)
#define LEN_UART6FnDataBuff     0//UART6����Fn���ݻ��峤��
//UART7(�ն�ESAM)
#define LEN_UART7IntRx     8//UART7�жϽ��ջ��峤��
#define LEN_UART7Ctrl     256//UART7�������ݳ���
#define LEN_UART7Rx     2048//UART7���ջ��峤��
#define LEN_UART7Tx     2048//UART7���ͻ��峤��
#define LEN_UART7TxMax     2048//UART7���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART7Tx<=LEN_UART7TxMax)
#define LEN_UART7FnDataBuff     0//UART7����Fn���ݻ��峤��
//UART8(���ESAM)
#define LEN_UART8IntRx     8//UART8�жϽ��ջ��峤��
#define LEN_UART8Ctrl     0//UART8�������ݳ���
#define LEN_UART8Rx     0//UART8���ջ��峤��
#define LEN_UART8Tx     0//UART8���ͻ��峤��
#define LEN_UART8TxMax     0//UART8���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART8Tx<=LEN_UART8TxMax)
#define LEN_UART8FnDataBuff     0//UART8����Fn���ݻ��峤��
//UART9(���CPUCARD)
#define LEN_UART9IntRx     8//UART9�жϽ��ջ��峤��
#define LEN_UART9Ctrl     0//UART9�������ݳ���
#define LEN_UART9Rx     0//UART9���ջ��峤��
#define LEN_UART9Tx     0//UART9���ͻ��峤��
#define LEN_UART9TxMax     0//UART9���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART9Tx<=LEN_UART9TxMax)
#define LEN_UART9FnDataBuff     0//UART9����Fn���ݻ��峤��
//UART10(Ethernet)
#define LEN_UART10IntRx     8192//UART10�жϽ��ջ��峤��
#define LEN_UART10Ctrl     256//UART10�������ݳ���
#define LEN_UART10Rx     8192//UART10���ջ��峤��
#define LEN_UART10Tx     2048//UART10���ͻ��峤��
#define LEN_UART10TxMax     2048//UART10���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART10Tx<=LEN_UART10TxMax)
#define LEN_UART10FnDataBuff     0//UART10����Fn���ݻ��峤��
//UART11(SMS)
#define LEN_UART11IntRx     8//UART11�жϽ��ջ��峤��
#define LEN_UART11Ctrl     0//UART11�������ݳ���
#define LEN_UART11Rx     0//UART11���ջ��峤��
#define LEN_UART11Tx     0//UART11���ͻ��峤��
#define LEN_UART11TxMax     0//UART11���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART11Tx<=LEN_UART11TxMax)
#define LEN_UART11FnDataBuff     0//UART11����Fn���ݻ��峤��
//UART12(�����)
#define LEN_UART12IntRx     8//UART12�жϽ��ջ��峤��
#define LEN_UART12Ctrl     256//UART12�������ݳ���
#define LEN_UART12Rx     2048//UART12���ջ��峤��
#define LEN_UART12Tx     256//UART12���ͻ��峤��
#define LEN_UART12TxMax     256//UART12���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART12Tx<=LEN_UART12TxMax)
#define LEN_UART12FnDataBuff     256//UART12����Fn���ݻ��峤��

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART13IntRx     (10+1024+2)//UART13�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART13Ctrl     256//UART13�������ݳ���
#define LEN_UART13Rx     0//UART13���ջ��峤��
#define LEN_UART13Tx     1024//UART13���ͻ��峤��
#define LEN_UART13TxMax     1024//UART13���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART13Tx<=LEN_UART13TxMax)
#define LEN_UART13FnDataBuff     0//UART13����Fn���ݻ��峤��
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART14IntRx     (10+4096+2)//UART14�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART14Ctrl     256//UART14�������ݳ���
#define LEN_UART14Rx     0//UART14���ջ��峤��
#define LEN_UART14Tx     2048//UART14���ͻ��峤��
#define LEN_UART14TxMax     2048//UART14���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART14Tx<=LEN_UART14TxMax)
#define LEN_UART14FnDataBuff     0//UART14����Fn���ݻ��峤��
//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART15IntRx     8//UART15�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART15Ctrl     0//UART15�������ݳ���
#define LEN_UART15Rx     0//UART15���ջ��峤��
#define LEN_UART15Tx     0//UART15���ͻ��峤��
#define LEN_UART15TxMax     0//UART15���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART15Tx<=LEN_UART15TxMax)
#define LEN_UART15FnDataBuff     0//UART15����Fn���ݻ��峤��
//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART16IntRx     8//UART16�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART16Ctrl     0//UART16�������ݳ���
#define LEN_UART16Rx     0//UART16���ջ��峤��
#define LEN_UART16Tx     0//UART16���ͻ��峤��
#define LEN_UART16TxMax     0//UART16���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART16Tx<=LEN_UART16TxMax)
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
#define MeterSpec     0//���ܱ���
//���ܱ���
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A

//MeterSpec=16 0.5S 3P4W 50Hz 6400imp/kWh 220V 0.3(6)A
//MeterSpec=17 0.5S 3P3W 50Hz 20000imp/kWh 100V 0.3(6)A
//MeterSpec=18 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 0.3(6)A
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
#define ADC_Vu     0.31//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
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
#define ADC_Vu     0.14//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
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
#define ADC_Vu     0.33//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi    0.068//Inʱ����ADC������Чֵ(ATT7022��)
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
#define ADC_Vu     0.31//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
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
#define ADC_Vu     0.14//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
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
#define ADC_Vu     0.33//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
#endif


#if MeterSpec==16
//MeterSpec=16 0.5S 3P4W 50Hz 6400imp/kWh 220V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     6400//����й�����imp/kWh
#define MeterActiveConstbcd     0x6400//����й�����imp/kWh
#define MeterReactiveConst     6400//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x6400//����޹�����imp/kvarh
#define MeterUBhex     2200//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x2200//�αȵ�ѹxxx.x(V)
#define MeterIBhex     30//�αȵ���xx.xx(A)
#define MeterIBbcd     0x030//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#define ADC_Vu     0.31//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
#endif
#if MeterSpec==17
//MeterSpec=17 0.5S 3P3W 50Hz 20000imp/kWh 100V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     1//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     20000//����й�����imp/kWh
#define MeterActiveConstbcd     0x20000//����й�����imp/kWh
#define MeterReactiveConst     20000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x20000//����޹�����imp/kvarh
#define MeterUBhex     1000//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x1000//�αȵ�ѹxxx.x(V)
#define MeterIBhex     30//�αȵ���xx.xx(A)
#define MeterIBbcd     0x030//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#define ADC_Vu     0.14//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
#endif
#if MeterSpec==18
//MeterSpec=18 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 0.3(6)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     20000//����й�����imp/kWh
#define MeterActiveConstbcd     0x20000//����й�����imp/kWh
#define MeterReactiveConst     20000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x20000//����޹�����imp/kvarh
#define MeterUBhex     577//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x577//�αȵ�ѹxxx.x(V)
#define MeterIBhex     30//�αȵ���xx.xx(A)
#define MeterIBbcd     0x030//�αȵ���xx.xx(A)
#define MeterIMaxhex     600//������xx.xx(A)
#define MeterIMaxbcd     0x600//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)
#define ADC_Vu     0.33//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.068//Inʱ����ADC������Чֵ(ATT7022��)
#endif

//���ܱ����------------------------------------------------------------------------------------





//�ն�------------------------------------------------------------------------------------------
#define TMaxTariff     4//�ն���������
#define DefaultTariff     2//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
#define USERTEST     0//0=��������,1=��ʡ���û����Ժ�ͼ�
#define UartDateTI     1//�ն�����ٶ�ʱ��0=���յ�24ʱ����,1=���յ�0ʱ����
#define UartMonthTI     1//�¶�����ٶ�ʱ��0=����δ��24ʱ����,1=���³�1�յ�0ʱ����
#define RMM_RS485_PassageZero     0//RS485ͨ�������0��;0=����ͣ,1=��ͣ
#define RMM_StartHM     0x0005//�𶯳���ʱ��
#define RMM_EndHM     0x2359//��������ʱ��
#define RMM_CompCongealTime     0//�����ܱ������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
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
#define WeekRMTask        1//ÿ�ܳ�������;0=û��,1=��(�ز��޸�Ϊ������ʧ��ʱΪ�ӿ������˳���������������)
#define TimerRMTask        0//��ʱ����ͣ���¼����;0=û��,1=��
#define HourTask     1//�ص㻧���߳���(���������߶���ĵ����)
#define MeterEventRMTask     0//���ڳ������ܱ��¼�����;0=û��,1=��

#define RMM_RS485     0//RS485ͨ������ģʽ:0=ѭ��ȫ������,1=����С������ó�������
#define RMM_RS485_DateCongeal     1//RS485ͨ�������ն���;0=��,1=��
#define RMM_Carrier_DateCongeal     1//�ز�ͨ�������ն���;0=��,1=��
#define RMM_RS485_IntegralCongeal     0//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
#define RMM_Carrier_IntegralCongeal     1//�ز�ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
#define RMM_Carrier_CurveTime     60//�ز�ͨ����������ߵ���������ʱ���;15�ֻ�60��
#define RMM_RS485_Congeal     0//RS485ͨ����������2-3�ն���;0=��,1=��
#define RMM_Carrier_Congeal     0//�ز�ͨ����������2-3�ն���;0=��,1=��
#define RMM_RS485_AddMonth     0//RS485ͨ����������2�¶���;0=��,1=��
#define RMM_Carrier_AddMonth     0//�ز�ͨ����������2�¶���;0=��,1=��
	#if((RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0)
		#if(RMM_MonthCongeal_0xEE==0)
			#error (RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0,RMM_MonthCongeal_0xEE==0
		#endif
	#endif
#define RMDateCongealSour 1//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ

#define MaxRS485User     8//���RS485������(4��������)
#define MaxKeyUser     20//����г����ص��û���
#define MaxCarrierUser     1000//1500//����ز�������(4��������)
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
#define MaxRMDateCongeal     31//12//��󳭱��ն�����
#define MaxMonthCongeal     12//����¶�����
#define CurveCongealTime     15//60//���߶�����ʱ��(�ܶ�)0=������;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
#define MaxCurveCongeal     (96*10)//(24*10)//������ߵ���
#define MaxACSampleCurveCongeal     (1440*10)//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
#define MaxERC     64//����¼�ERC��
#define MaxClassEventNum     15//���ÿ�������¼���¼����
#define F246MaxPn     4//����ն�������¼���ݲ�������
#define NewERC14     1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
#define EventProject     1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
#define EnMeterAutoAutoReport     1//������ܱ������ϱ��¼�:0=������,1=����
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
#define MulRealRead     1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡

#define SoftBug1     0//0=��������,1=376.1���ܲ������BUG.Сʱ�����ܶ�
#define SoftBug3     1//���������������0=�Ƚ�������������,1=���Ƚ�������������
#define SoftBug4     0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
#define RMM_Type     0//���г�������:0=�ز���,1=RS485��ͨ���ɼ���,2=���
#define ErrorAutoDisplay     0//0=����,1=��ֹ;ϵͳ�����Զ�������ʾ
#define TESAMSPI     0//TESAM��SPI�ӿ�,0=��׼SPI1��,1=IO��
#define AFN04F150Dn     1//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
#define ReadF129Above    1//�ٶ�1��F129��������:0=��ԭ�洢����,1=ʵʱ��ȡ�����ϴ�
#define RMM_AFN0CF26     0//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
#define Uart3761Check0xEE   0//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
#define AFN0CF41F42F43F44     0//���յ�����AFN0CF41F42F43F44����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define AFN0CF45F46F47F48     0//���µ�����AFN0CF45F46F47F48����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define RS232DefaultBPS     (0xb+(6<<5))//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     (0xb+(3<<5))//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     (0xb+(2<<5))//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define DefaultClassNo     0x40//�Զ��ѱ�Ĭ�ϵ��û������С���

#define Extern_Uart_AFN255     0//�ⲿ����:0=I2,1=E1,2=E2,3=;��������(��չ)
#define Extern_Terminal_ReadMeter_DL645_1997     0//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳���DL645-1997
#define Extern_Terminal_ReadMeter_DL645_1997_Fn     0//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
#define Extern_BatchTask_RS485     0//�ⲿ����:0=I2,1=E1,2=E2,3=;����ת����������-RS485-ʱ�Ӷ�ʱ
#define Extern_BatchTask_3762     0//�ⲿ����:0=I2,1=E1,2=E2,3=;����ת����������-3762-ʱ�Ӷ�ʱ
#define Extern_W5200_FTP_RxTx     0//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 FTP�ļ�����
#define Extern_GPRS_FTP_RxTx     0//�ⲿ����:0=I2,1=E1,2=E2,3=;GPRS FTP�ļ�����
//#define Extern_CDMA_FTP_RxTx     1//�ⲿ����:0=I2,1=E1,2=E2,3=;CDMA FTP�ļ�����
#define Extern_Uart_AFN04     0//�ⲿ����:0=I2,1=E1,2=E2,3=;�������úͲ�ѯ
#define Extern_Uart_AFN05     0//�ⲿ����:0=I2,1=E1,2=E2,3=;��������
#define Extern_Uart_AFN09     0//�ⲿ����:0=I2,1=E1,2=E2,3=;�����ն�����
#define Extern_Terminal_ReadMeter_ShH     0//�ⲿ����:0=I2,1=E1,2=E2,3=;�ն˳��� �Ϻ����Լ
#define Extern_Terminal_ReadMeter_ShH_Fn     0//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
#define Extern_ReadMeter_Fn_YesNo     0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)



#define NAND128kBuff     1//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
//�ն˽���--------------------------------------------------------------------------------------








//Ӳ���汾--------------------------------------------------------------------------------------
#if(HVER==0x400)//Ӳ���汾��4.00
#define IC_STM32F42x     1//0=û,1=��
#define IC_STM32F46x     0//0=û,1=��
#define HCLK     168000000//32000000//84000000//168000000//ϵͳʱ��(Hz):168,84,32
#define LCDPixels     3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE

#define IC_MT29F1G     0//0=û,1=��(128 Mbyte)
#define IC_MT29F2G     1//0=û,1=��(256 Mbyte)
#define IC_MT29F4G     0//0=û,1=��(512 Mbyte)
#define IC_MT29F8G     0//0=û,1=��(1024 Mbyte)
#define IC_S34ML1G     0//0=û,1=��(128 Mbyte)
#define IC_S34ML2G     0//0=û,1=��(256 Mbyte)
#define IC_S34ML4G     0//0=û,1=��(512 Mbyte)
#define IC_S34ML8G     0//0=û,1=��(512 Mbyte)
#define NAND_BUS_WIDTH     16//8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
#define NAND_1G     1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
#define NAND_2G     1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
#define NAND_4G     1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
#define NAND_8G     1
#endif
#ifdef  NAND_1G
#define NAND_BLOCK_COUNT     1024//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_2G
#define NAND_BLOCK_COUNT     2048//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_4G
#define NAND_BLOCK_COUNT     4096//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_8G
#define NAND_BLOCK_COUNT     8192//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     250//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#if NAND_REPLACE_BLOCK_COUNT>250//1-250(0xFA)=���滻���滻�����滻���,0xFB=����,0xFC=����,0xFD=δ��(���滻),0xFE=����(���滻),(0x00,0xFF����Ƭ�ӳ�����ʼֵ����),����ֵ=��ʾδ��ʼ��
#error NAND_REPLACE_BLOCK_COUNT>250
#endif

#define IC_MT48LC4M16     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC8M16     1//0=û,1=��(16 Mbyte)
#define IC_MT48LC16M16     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC32M16     0//0=û,1=��(64 Mbyte)
#define IC_MT48LC8M8     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC16M8     0//0=û,1=��(16 Mbyte)
#define IC_MT48LC32M8     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC64M8     0//(A11���������ݲ�ѡ��)0=û,1=��(64 Mbyte)
#define IC_SDRAM     (IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      1//0=û,1=��
#define IC_DS3231     0//0=û,1=��

#define IC_LAN8720     1//0=û,1=��
#define IC_90E36     1//0=û,1=��
#define IC_ATT7022     0//0=û,1=��

#define IC_UC1698     1//0=û,1=��(������ʾ)
#define IC_ST75161     0//0=û,1=��(������ʾ)
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

#define MaxFileNum     8//��������ļ�����
//����Ŀ¼�ļ���
	#define FileFactory_PACK          "PACK.*"
	#define FileFactory_MS            "MS.*"
	#define FileFactory_OS            "OS.*"
	#define FileFactory_Meter         "Meter.*"
	#define FileFactory_Terminal      "Terminal.*"
	#define FileFactory_Menu          "Menu.*"
	#define FileFactory_Font          "Font.*"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test.*"
	#define FileFactory_Modele        "Module.*"
	#define FileFactory_PLC_50        "PLC_50.*"
	#define FileFactory_PLC_TC        "PLC_TC.*"
	#define FileFactory_PLC_ES        "PLC_ES.*"
	#define FileFactory_PLC_CX        "PLC_CX.*"
	#define FileFactory_PLC_01        "PLC_01.*"
	#define FileFactory_PLC_LH        "PLC_LH.*"
	#define FileFactory_PLC_NX        "PLC_NX.*"
	#define FileFactory_PLC_QL        "PLC_QL.*"
	#define FileFactory_PLC_MX        "PLC_MX.*"
	#define FileFactory_PLC_FC        "PLC_FC.*"
	#define FileFactory_PLC_NW        "PLC_NW.*"
	#define FileFactory_PLC_SR        "PLC_SR.*"

//�û�Ŀ¼ȫ��
#define FileUserDIR          "/J000"
#if MeterSpec==0//���ܱ���//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK_J900_000_V4.*"
	#define FileUser_MS        "MS_J900_000_V4.*"
	#define FileUser_OS        "OS_J900_000_V4.*"
	#define FileUser_Meter     "Meter_J900_000_V4.*"
	#define FileUser_Terminal  "Terminal_J900_000_V4.*"
	#define FileUser_Menu      "Menu_J900_000_V4.*"
	#define FileUser_Font      "Font_J900_000_V4.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module_J900_000_V4.*"
	#define FileUser_PLC_50    "PLC_J900_000_V4_50.*"
	#define FileUser_PLC_TC    "PLC_J900_000_V4_TC.*"
	#define FileUser_PLC_ES    "PLC_J900_000_V4_ES.*"
	#define FileUser_PLC_CX    "PLC_J900_000_V4_CX.*"
	#define FileUser_PLC_01    "PLC_J900_000_V4_01.*"
	#define FileUser_PLC_LH    "PLC_J900_000_V4_LH.*"
	#define FileUser_PLC_NX    "PLC_J900_000_V4_NX.*"
	#define FileUser_PLC_QL    "PLC_J900_000_V4_QL.*"
	#define FileUser_PLC_MX    "PLC_J900_000_V4_MX.*"
	#define FileUser_PLC_FC    "PLC_J900_000_V4_FC.*"
	#define FileUser_PLC_NW    "PLC_J900_000_V4_NW.*"
	#define FileUser_PLC_SR    "PLC_J900_000_V4_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK_J900_000_V4.zip"
	#define FileFTP_MS        "MS_J900_000_V4.zip"
	#define FileFTP_OS        "OS_J900_000_V4.zip"
	#define FileFTP_Meter     "Meter_J900_000_V4.zip"
	#define FileFTP_Terminal  "Terminal_J900_000_V4.zip"
	#define FileFTP_Menu      "Menu_J900_000_V4.zip"
	#define FileFTP_Font      "Font_J900_000_V4.zip"
	#define FileFTP_MODULE    "Module_J900_000_V4.zip"
	#define FileFTP_PLC_50    "PLC_J900_000_V4_50.zip"
	#define FileFTP_PLC_TC    "PLC_J900_000_V4_TC.zip"
	#define FileFTP_PLC_ES    "PLC_J900_000_V4_ES.zip"
	#define FileFTP_PLC_CX    "PLC_J900_000_V4_CX.zip"
	#define FileFTP_PLC_01    "PLC_J900_000_V4_01.zip"
	#define FileFTP_PLC_LH    "PLC_J900_000_V4_LH.zip"
	#define FileFTP_PLC_NX    "PLC_J900_000_V4_NX.zip"
	#define FileFTP_PLC_QL    "PLC_J900_000_V4_QL.zip"
	#define FileFTP_PLC_MX    "PLC_J900_000_V4_MX.zip"
	#define FileFTP_PLC_FC    "PLC_J900_000_V4_FC.zip"
	#define FileFTP_PLC_NW    "PLC_J900_000_V4_NW.zip"
	#define FileFTP_PLC_SR    "PLC_J900_000_V4_SR.zip"
#endif
#endif
//Ӳ���汾����----------------------------------------------------------------------------------


//Ӳ���汾--------------------------------------------------------------------------------------
#if (HVER==0x410)//Ӳ���汾��4.10
#define IC_STM32F42x     1//0=û,1=��
#define IC_STM32F46x     0//0=û,1=��
#define HCLK     168000000//32000000//84000000//168000000//ϵͳʱ��(Hz):168,84,32
#define LCDPixels     3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE

#define IC_MT29F1G     0//0=û,1=��(128 Mbyte)
#define IC_MT29F2G     1//0=û,1=��(256 Mbyte)
#define IC_MT29F4G     0//0=û,1=��(512 Mbyte)
#define IC_MT29F8G     0//0=û,1=��(1024 Mbyte)
#define IC_S34ML1G     0//0=û,1=��(128 Mbyte)
#define IC_S34ML2G     0//0=û,1=��(256 Mbyte)
#define IC_S34ML4G     0//0=û,1=��(512 Mbyte)
#define IC_S34ML8G     0//0=û,1=��(512 Mbyte)
#define NAND_BUS_WIDTH     8//��OS��TEST��Ҫ,���������Զ�ʶ��;8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
#define NAND_1G     1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
#define NAND_2G     1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
#define NAND_4G     1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
#define NAND_8G     1
#endif
#ifdef  NAND_1G
#define NAND_BLOCK_COUNT     1024//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_2G
#define NAND_BLOCK_COUNT     2048//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_4G
#define NAND_BLOCK_COUNT     4096//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_8G
#define NAND_BLOCK_COUNT     8192//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     250//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#if NAND_REPLACE_BLOCK_COUNT>250//1-250(0xFA)=���滻���滻�����滻���,0xFB=����,0xFC=����,0xFD=δ��(���滻),0xFE=����(���滻),(0x00,0xFF����Ƭ�ӳ�����ʼֵ����),����ֵ=��ʾδ��ʼ��
#error NAND_REPLACE_BLOCK_COUNT>250
#endif

#define IC_MT48LC4M16     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC8M16     1//0=û,1=��(16 Mbyte)
#define IC_MT48LC16M16     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC32M16     0//0=û,1=��(64 Mbyte)
#define IC_MT48LC8M8     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC16M8     0//0=û,1=��(16 Mbyte)
#define IC_MT48LC32M8     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC64M8     0//(A11���������ݲ�ѡ��)0=û,1=��(64 Mbyte)
#define IC_SDRAM     (IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      1//0=û,1=��
#define IC_DS3231     0//0=û,1=��

#define IC_LAN8720     1//0=û,1=��
#define IC_90E36     1//0=û,1=��
#define IC_ATT7022     0//0=û,1=��

#define IC_UC1698     1//0=û,1=��(������ʾ)
#define IC_ST75161     0//0=û,1=��(������ʾ)
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

#define MaxFileNum     8//��������ļ�����
//����Ŀ¼�ļ���
	#define FileFactory_PACK          "PACK.*"
	#define FileFactory_MS            "MS.*"
	#define FileFactory_OS            "OS.*"
	#define FileFactory_Meter         "Meter.*"
	#define FileFactory_Terminal      "Terminal.*"
	#define FileFactory_Menu          "Menu.*"
	#define FileFactory_Font          "Font.*"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test.*"
	#define FileFactory_Modele        "Module.*"
	#define FileFactory_PLC_50        "PLC_50.*"
	#define FileFactory_PLC_TC        "PLC_TC.*"
	#define FileFactory_PLC_ES        "PLC_ES.*"
	#define FileFactory_PLC_CX        "PLC_CX.*"
	#define FileFactory_PLC_01        "PLC_01.*"
	#define FileFactory_PLC_LH        "PLC_LH.*"
	#define FileFactory_PLC_NX        "PLC_NX.*"
	#define FileFactory_PLC_QL        "PLC_QL.*"
	#define FileFactory_PLC_MX        "PLC_MX.*"
	#define FileFactory_PLC_FC        "PLC_FC.*"
	#define FileFactory_PLC_NW        "PLC_NW.*"
	#define FileFactory_PLC_SR        "PLC_SR.*"

//�û�Ŀ¼ȫ��
#define FileUserDIR          "/J000"
#if MeterSpec==0//���ܱ���//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK_J900_000_V41.*"
	#define FileUser_MS        "MS_J900_000_V41.*"
	#define FileUser_OS        "OS_J900_000_V41.*"
	#define FileUser_Meter     "Meter_J900_000_V41.*"
	#define FileUser_Terminal  "Terminal_J900_000_V41.*"
	#define FileUser_Menu      "Menu_J900_000_V41.*"
	#define FileUser_Font      "Font_J900_000_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module_J900_000_V41.*"
	#define FileUser_PLC_50    "PLC_J900_000_V41_50.*"
	#define FileUser_PLC_TC    "PLC_J900_000_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_J900_000_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_J900_000_V41_CX.*"
	#define FileUser_PLC_01    "PLC_J900_000_V41_01.*"
	#define FileUser_PLC_LH    "PLC_J900_000_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_J900_000_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_J900_000_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_J900_000_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_J900_000_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_J900_000_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_J900_000_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK_J900_000_V41.zip"
	#define FileFTP_MS        "MS_J900_000_V41.zip"
	#define FileFTP_OS        "OS_J900_000_V41.zip"
	#define FileFTP_Meter     "Meter_J900_000_V41.zip"
	#define FileFTP_Terminal  "Terminal_J900_000_V41.zip"
	#define FileFTP_Menu      "Menu_J900_000_V41.zip"
	#define FileFTP_Font      "Font_J900_000_V41.zip"
	#define FileFTP_MODULE    "Module_J900_000_V41.zip"
	#define FileFTP_PLC_50    "PLC_J900_000_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_J900_000_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_J900_000_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_J900_000_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_J900_000_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_J900_000_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_J900_000_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_J900_000_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_J900_000_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_J900_000_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_J900_000_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_J900_000_V41_SR.zip"
#endif
#endif
//Ӳ���汾4.10����----------------------------------------------------------------------------------


//Ӳ���汾5.0--------------------------------------------------------------------------------------
#if (HVER==0x500)//Ӳ���汾��5.00
#define IC_STM32F42x     0//0=û,1=��
#define IC_STM32F46x     0//0=û,1=��
#define IC_NUC972     1//0=û,1=��
#define IC_NUC975     0//0=û,1=��
#define HCLK     			168000000//84000000//32000000//ϵͳʱ��(Hz):168,84,32
#define LCDPixels     3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
#define SuperCapacitorChargingTime     10//�������ݳ��ʱ��(��)(���ڵ���ʱ�Ƿ�������ݱ���)

#define IC_MT29F1G     0//0=û,1=��(128 Mbyte)
#define IC_MT29F2G     1//0=û,1=��(256 Mbyte)
#define IC_MT29F4G     0//0=û,1=��(512 Mbyte)
#define IC_MT29F8G     0//0=û,1=��(1024 Mbyte)
#define IC_S34ML1G     0//0=û,1=��(128 Mbyte)
#define IC_S34ML2G     0//0=û,1=��(256 Mbyte)
#define IC_S34ML4G     0//0=û,1=��(512 Mbyte)
#define IC_S34ML8G     0//0=û,1=��(512 Mbyte)
#define NAND_BUS_WIDTH     8//��OS��TEST��Ҫ,���������Զ�ʶ��;8=8bit,16=16bit

#if (IC_MT29F1G|IC_MT29F2G|IC_MT29F4G|IC_MT29F8G)
	#define IC_MT29FxG     1
#endif
#if (IC_S34ML1G|IC_S34ML2G|IC_S34ML4G|IC_S34ML8G)
	#define IC_S34MLxG     1
#endif
#if (IC_S34ML1G|IC_MT29F1G)
#define NAND_1G     1
#endif
#if (IC_S34ML2G|IC_MT29F2G)
#define NAND_2G     1
#endif
#if (IC_S34ML4G|IC_MT29F4G)
#define NAND_4G     1
#endif
#if (IC_S34ML8G|IC_MT29F8G)
#define NAND_8G     1
#endif
#ifdef  NAND_1G
#define NAND_BLOCK_COUNT     1024//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_2G
#define NAND_BLOCK_COUNT     2048//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_4G
#define NAND_BLOCK_COUNT     4096//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     ((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#ifdef  NAND_8G
#define NAND_BLOCK_COUNT     8192//NANDFLASH���п���(ÿ��128K,ÿҳ2048Byte)
#define NAND_FILE_BLOCK_COUNT     128//NANDFLASH�����ļ���ϵͳ�Ŀ���(�޸Ĵ�ֵͬʱҪ�޸��ļ�����ͷ�ļ��е�ֵ)
#define NAND_REPLACE_BLOCK_COUNT     250//((NAND_BLOCK_COUNT/1024)*32)//NANDFLASH�����滻�Ŀ���(�����ļ�ϵͳ��)
#endif
#if NAND_REPLACE_BLOCK_COUNT>250//1-250(0xFA)=���滻���滻�����滻���,0xFB=����,0xFC=����,0xFD=δ��(���滻),0xFE=����(���滻),(0x00,0xFF����Ƭ�ӳ�����ʼֵ����),����ֵ=��ʾδ��ʼ��
#error NAND_REPLACE_BLOCK_COUNT>250
#endif

#define IC_MT48LC4M16     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC8M16     0//0=û,1=��(16 Mbyte)
#define IC_MT48LC16M16     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC32M16     1//0=û,1=��(64 Mbyte)
#define IC_MT48LC8M8     0//0=û,1=��(8 Mbyte)
#define IC_MT48LC16M8     0//0=û,1=��(16 Mbyte)
#define IC_MT48LC32M8     0//0=û,1=��(32 Mbyte)
#define IC_MT48LC64M8     0//(A11���������ݲ�ѡ��)0=û,1=��(64 Mbyte)
#define IC_SDRAM     (IC_MT48LC4M16|IC_MT48LC8M16|IC_MT48LC16M16|IC_MT48LC32M16|IC_MT48LC8M8|IC_MT48LC16M8|IC_MT48LC32M8|IC_MT48LC64M8)

#define IC_RX8025T      1//0=û,1=��
#define IC_DS3231     0//0=û,1=��

#define IC_IP101     1//0=û,1=��(eth0)
#define IC_LAN8720     1//0=û,1=��(eth1)
#define RJ45SELECT     0//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
#define IC_90E36     1//0=û,1=��
#define IC_ATT7022     0//0=û,1=��

#define IC_UC1698     1//0=û,1=��(������ʾ)
#define IC_ST75161     0//0=û,1=��(������ʾ)
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

#define MaxFileNum     8//��������ļ�����
//����Ŀ¼�ļ���
	#define FileFactory_PACK          "PACK376N.zip"
	#define FileFactory_MS            "MS376N.zip"
	#define FileFactory_OS            "OS376N.zip"
	#define FileFactory_Meter         "Meter376N.zip"
	#define FileFactory_Terminal      "Terminal376N.zip"
	#define FileFactory_Menu          "Menu376N.zip"
	#define FileFactory_Font          "FontN.zip"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test376N.zip"
	#define FileFactory_Modele        "Module376N.zip"
	#define FileFactory_PLC_50        "PLC_50.zip"
	#define FileFactory_PLC_TC        "PLC_TC.zip"
	#define FileFactory_PLC_ES        "PLC_ES.zip"
	#define FileFactory_PLC_CX        "PLC_CX.zip"
	#define FileFactory_PLC_01        "PLC_01.zip"
	#define FileFactory_PLC_LH        "PLC_LH.zip"
	#define FileFactory_PLC_NX        "PLC_NX.zip"
	#define FileFactory_PLC_QL        "PLC_QL.zip"
	#define FileFactory_PLC_MX        "PLC_MX.zip"
	#define FileFactory_PLC_FC        "PLC_FC.zip"
	#define FileFactory_PLC_NW        "PLC_NW.zip"
	#define FileFactory_PLC_SR        "PLC_SR.zip"

//�û�Ŀ¼ȫ��
#define FileUserDIR          "/J1100"
#if MeterSpec==0//���ܱ���//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK376N_J1100_000_V50.zip"
	#define FileUser_MS        "MS376N_J1100_000_V50.zip"
	#define FileUser_OS        "OS376N_J1100_000_V50.zip"
	#define FileUser_Meter     "Meter376N_J1100_000_V50.zip"
	#define FileUser_Terminal  "Terminal376N_J1100_000_V50.zip"
	#define FileUser_Menu      "Menu376N_J1100_000_V50.zip"
	#define FileUser_Font      "FontN_J1100_000_V50.zip"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module376N_J1100_000_V50.zip"
	#define FileUser_PLC_50    "PLC_J1100_000_V50_50.zip"
	#define FileUser_PLC_TC    "PLC_J1100_000_V50_TC.zip"
	#define FileUser_PLC_ES    "PLC_J1100_000_V50_ES.zip"
	#define FileUser_PLC_CX    "PLC_J1100_000_V50_CX.zip"
	#define FileUser_PLC_01    "PLC_J1100_000_V50_01.zip"
	#define FileUser_PLC_LH    "PLC_J1100_000_V50_LH.zip"
	#define FileUser_PLC_NX    "PLC_J1100_000_V50_NX.zip"
	#define FileUser_PLC_QL    "PLC_J1100_000_V50_QL.zip"
	#define FileUser_PLC_MX    "PLC_J1100_000_V50_MX.zip"
	#define FileUser_PLC_FC    "PLC_J1100_000_V50_FC.zip"
	#define FileUser_PLC_NW    "PLC_J1100_000_V50_NW.zip"
	#define FileUser_PLC_SR    "PLC_J1100_000_V50_SR.zip"
//FTP���ļ���
	#define FileFTP_PACK      "PACK376N_J1100_000_V50.zip"
	#define FileFTP_MS        "MS376N_J1100_000_V50.zip"
	#define FileFTP_OS        "OS376N_J1100_000_V50.zip"
	#define FileFTP_Meter     "Meter376N_J1100_000_V50.zip"
	#define FileFTP_Terminal  "Terminal376N_J1100_000_V50.zip"
	#define FileFTP_Menu      "Menu376N_J1100_000_V50.zip"
	#define FileFTP_Font      "FontN_J1100_000_V50.zip"
	#define FileFTP_MODULE    "Module376N_J1100_000_V50.zip"
	#define FileFTP_PLC_50    "PLC_J1100_000_V50_50.zip"
	#define FileFTP_PLC_TC    "PLC_J1100_000_V50_TC.zip"
	#define FileFTP_PLC_ES    "PLC_J1100_000_V50_ES.zip"
	#define FileFTP_PLC_CX    "PLC_J1100_000_V50_CX.zip"
	#define FileFTP_PLC_01    "PLC_J1100_000_V50_01.zip"
	#define FileFTP_PLC_LH    "PLC_J1100_000_V50_LH.zip"
	#define FileFTP_PLC_NX    "PLC_J1100_000_V50_NX.zip"
	#define FileFTP_PLC_QL    "PLC_J1100_000_V50_QL.zip"
	#define FileFTP_PLC_MX    "PLC_J1100_000_V50_MX.zip"
	#define FileFTP_PLC_FC    "PLC_J1100_000_V50_FC.zip"
	#define FileFTP_PLC_NW    "PLC_J1100_000_V50_NW.zip"
	#define FileFTP_PLC_SR    "PLC_J1100_000_V50_SR.zip"
#endif


#if MeterSpec==19//���ܱ���//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK376N_J1100_1900_V50.zip"
	#define FileUser_MS        "MS376N_J1100_1900_V50.zip"
	#define FileUser_OS        "OS376N_J1100_1900_V50.zip"
	#define FileUser_Meter     "Meter376N_J1100_1900_V50.zip"
	#define FileUser_Terminal  "Terminal376N_J1100_1900_V50.zip"
	#define FileUser_Menu      "Menu376N_J1100_1900_V50.zip"
	#define FileUser_Font      "FontN_J1100_1900_V50.zip"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module376N_J1100_1900_V50.zip"
	#define FileUser_PLC_50    "PLC_J1100_1900_V50_50.zip"
	#define FileUser_PLC_TC    "PLC_J1100_1900_V50_TC.zip"
	#define FileUser_PLC_ES    "PLC_J1100_1900_V50_ES.zip"
	#define FileUser_PLC_CX    "PLC_J1100_1900_V50_CX.zip"
	#define FileUser_PLC_01    "PLC_J1100_1900_V50_01.zip"
	#define FileUser_PLC_LH    "PLC_J1100_1900_V50_LH.zip"
	#define FileUser_PLC_NX    "PLC_J1100_1900_V50_NX.zip"
	#define FileUser_PLC_QL    "PLC_J1100_1900_V50_QL.zip"
	#define FileUser_PLC_MX    "PLC_J1100_1900_V50_MX.zip"
	#define FileUser_PLC_FC    "PLC_J1100_1900_V50_FC.zip"
	#define FileUser_PLC_NW    "PLC_J1100_1900_V50_NW.zip"
	#define FileUser_PLC_SR    "PLC_J1100_1900_V50_SR.zip"
//FTP���ļ���
	#define FileFTP_PACK      "PACK376N_J1100_1900_V50.zip"
	#define FileFTP_MS        "MS376N_J1100_1900_V50.zip"
	#define FileFTP_OS        "OS376N_J1100_1900_V50.zip"
	#define FileFTP_Meter     "Meter376N_J1100_1900_V50.zip"
	#define FileFTP_Terminal  "Terminal376N_J1100_1900_V50.zip"
	#define FileFTP_Menu      "Menu376N_J1100_1900_V50.zip"
	#define FileFTP_Font      "FontN_J1100_1900_V50.zip"
	#define FileFTP_MODULE    "Module376N_J1100_1900_V50.zip"
	#define FileFTP_PLC_50    "PLC_J1100_1900_V50_50.zip"
	#define FileFTP_PLC_TC    "PLC_J1100_1900_V50_TC.zip"
	#define FileFTP_PLC_ES    "PLC_J1100_1900_V50_ES.zip"
	#define FileFTP_PLC_CX    "PLC_J1100_1900_V50_CX.zip"
	#define FileFTP_PLC_01    "PLC_J1100_1900_V50_01.zip"
	#define FileFTP_PLC_LH    "PLC_J1100_1900_V50_LH.zip"
	#define FileFTP_PLC_NX    "PLC_J1100_1900_V50_NX.zip"
	#define FileFTP_PLC_QL    "PLC_J1100_1900_V50_QL.zip"
	#define FileFTP_PLC_MX    "PLC_J1100_1900_V50_MX.zip"
	#define FileFTP_PLC_FC    "PLC_J1100_1900_V50_FC.zip"
	#define FileFTP_PLC_NW    "PLC_J1100_1900_V50_NW.zip"
	#define FileFTP_PLC_SR    "PLC_J1100_1900_V50_SR.zip"
#endif
#endif
//Ӳ���汾5.00����----------------------------------------------------------------------------------




#endif

