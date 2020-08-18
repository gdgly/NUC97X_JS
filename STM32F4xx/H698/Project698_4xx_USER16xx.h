
//16xx=����ϵ
#ifndef Terminal_H
#define Terminal_H



//�汾-------------------------------------------------------------------------------------
#define UpDataReset          1//������º�����:0=��,1=��(ע:��MS������ǿ������)
#define MS_UpDataInit        3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define OS_UpDataInit        0//3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define Meter_UpDataInit     3//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
#define Terminal_UpDataInit  3//7//������³�ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��,7=������ʼ��(������nand�ļ�����ת�Ƶ��µ�ַ)
#define Menu_UpDataInit      0//��������Ǳ��ʼ��:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��

#define EnProject       0//0=��,1=��;�������ʱ�Ƿ��б���Ŀ����
#define EnHV            0//0=��,1=��;�������ʱ�Ƿ��б�Ӳ���汾
#define EnUSER          0//1//0=��,1=��;�������ʱ�Ƿ��б��û���ʶ
#define EnSV            0//0=��,1=��;�������ʱ�Ƿ��б�����汾��,������ʱֻ��>=��ǰ�汾�Ų���Ч
#define EnMeter         0//0=��,1=��;���ܱ��������ʱ�Ƿ��б��������߷�ʽ�ȵ����
#define EnMainProtocol  1//0=��,1=��;�������ʱ�Ƿ��б���ͨ��Э��

#ifdef IC_STM32F4xx
#define SVER     0x100//����汾��:���Ժ����ΪV1.0���µ�V1.1
#define SVDATE   0x181210//�����������:������
//#define HVER     0x400//Ӳ���汾��:4.00
#define HVER     0x410//Ӳ���汾��:4.10;nandbusbit=8,LCD���߷ֿ�
//#define HVER    	 0x420//Ӳ���汾��:4.20;���ӳ�������
#define HVDATE   0x180109//0x160701//Ӳ����������:������
#endif
#ifdef IC_NUC9xx
#define SVER     0x100//����汾��:���Ժ����ΪV1.0���µ�V1.1
#define SVDATE   0x181210//�����������:������
	#ifndef LINUX
#define HVER     0x500//NUC97xӲ���汾��:5.00 BOOT_SPI
	#else
#define HVER     0x510//NUC97xӲ���汾��:5.10 BOOT_NAND linux
	#endif
#define HVDATE   0x190501//0x160701//Ӳ����������:������
#endif

#define FactoryCode      'Z','J','Z','T'//'0','0','0','0'//���̴���       visible-string(SIZE (4))
#define FactoryInfoExt   'D','J','1','7', 'G','2','0','0'//'C','H','I','N','T',' ',' ',' '//������չ��Ϣ   visible-string(SIZE (8))
#define ProduceDate_year        2018//����������(hex)
#define ProduceDate_year_BCD    0x2018//����������(BCD)
#define ProduceDate_month       4//5//6//����������(hex) 
#define ProduceDate_month_BCD   0x04//0x05//0x06//����������(BCD) 
#define ProduceDate_day     2//����������(hex) 
#define ProduceDate_hour    8//��������ʱ(hex)
#define ProduceDate_minute  0//�������ڷ�(hex)
#define ProduceDate_second  0//����������(hex)

#define emWin      0//emWin GUI;0=û,1=��
#define Font_ttf   0//ttf����;0=û,1=��

#define SAME_MS        0//0=��,1=��;�����ͬʱ���±�־
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

	#endif
#endif
//�汾����---------------------------------------------------------------------------------


#define LEN_DATABUFF     4096//��д���ݻ��峤��,��С4096
#define LEN_128KDATABUFF     (128*1024)



//---�ն�DL698---------------------------
#define TMaxTariff      4//������
#define NAND128kBuff    0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
#define FileID_10x      1//0=��,1=��;��������_�ɼ�����_�洢ʱ������=10x(��ͨ�ɼ�����,�ɼ���ǰ����)�ļ���ʶ��������ID��
#define FileID_12xD     0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=12x(��ͨ�ɼ�����,������ʱ��ɼ��ն���)�ļ���ʶ��������ID��
#define FileID_12xM     0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=12x(��ͨ�ɼ�����,������ʱ��ɼ��¶���)�ļ���ʶ��������ID��
#define FileID_136      0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=136(��ͨ�ɼ�����,��ʱ�����ɼ�����)�ļ���ʶ��������ID��
#define FileID_2xx      1//0=��,1=��;��������_�ɼ�����_�洢ʱ������=2xx(�¼��ɼ�����)�ļ���ʶ��������ID��
#define RecordFILEtoPORT       0//�����¼�ļ����˿ںż�¼:0=��,1=��
#define RecordFILEallinFLASH   0//ȫ����ͨ���������ݱ�����nand flash:0=��(ֻ��OADʱ����RAM),1=��
#define NUM_RMmax    					 1024//��󳭱���
#define NUM_RMLISTmax  		   16//NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
#define RM_HPLC_Concurrency    1//0=��,1=��;����ز�������������������
	#if (NUM_RMmax*8)>(128*1024)
		#error //��¼�ļ��ĵ�ַ�����ݿ鳤���б���,�б������ȹ̶�8*NUM_RMmax byteͨ����1�ζ���ADDR_128KDATABUFF�����־�ж���
	#endif


#define RMTASKmax 			64//���������
#define NUMmax_6012_2   	RMTASKmax//��󳭱�������
#define NUMmax_6014     	64//�����ͨ�ɼ�������
#define NUMmax_6016     	64//����¼��ɼ�������
#define NUMmax_6018     	64//���͸��������
#define NUMmax_601C     	64//����ϱ�������(����Ӱ���ļ����������ļ�����)(��ǰV1.10����)
#define GPRS_ENET_REPORT 	1//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�(��ǰV1.10����)


#define ExtRMTASKmax 			0//��չ������
#define RMTASKnoPOWER    	0//ͣ�ϵ��¼��̶������
#define RMovertimeACKfail 0//1//�ز�·���������󳭱����������ʧ��ʱ:0=����������֡,1=���س���ʧ��(����)
#define RMSOMETASK    		1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
#define MeterAutoReportFileID_DL645     0x00000005//DL645��������ϱ��ļ�ID:b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
#define MeterAutoReportFileID_DL698     0x21000000//DL698��������ϱ��ļ�ID:b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
#define MeterAutoReportFileID_3011      0x23000000//���ܱ�ͣ�������ϱ��ļ�ID:b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
#define EnRS485AutoSearchMeter 		 0//0=��,1=��;����RS485���Զ��ѱ�
#define FORCEiACTIVE  				 		 0//0=��,1=��;ǿ�Ƽ�����Ϊ��������(��·�ɳ���ģʽ�޹�)

#define LENmax_APDU     		2048//���APDU�ߴ�
#define LENmax_TxAPDU     	2048//1400//��·���֡����ÿ֡���APDU�ֽ���(��������·���֡���ֵ��ΪͬLEN_UARTnTx)
#define REPORT_NUMFILEmax   1//16//1֡�����ϱ��������Բ�ͬ�ļ���������
#define CongealTime_5002    15//�ֶ���(����)���ʱ���

#define NUMmax_Event  			   64//15//����¼���¼��
#define NUMmax_Event3106RM     4//ͣ/�ϵ��¼���󳭱����
#define NUMmax_SWITCHIN    		 4//��󿪹�������·��(�Žӵ�)(<=8)
#define NUMmax_IMPIN    		   2//�����������·��
#define NUMmax_230x    			   8//����ܼ����Ա����
#define NUMmax_TAG    			   8//����ܼ�����<=8
#define NUMmax_CR     			   2//�������ִ���
#define NUMmax_DC  				     0//���ֱ��ģ����·��
#define NUMmax_RS485  			   2//���RS485����
#define TimerS_UARTRAMFREE     600//600��,UART������ڴ��Զ��ͷ���ʱ
#define PARAMETER_601E   	   	0//���ò���601E�ɼ������ʹ��:0=�ڲ�Ĭ��,1=������
#define ALLDATANULLSEL     	   0//ȫ�����ݿ�ʱ��0=�޷���,1=�ÿ�����0
#define RM_SECURITY_MeterNo     0//0=��,1=��;DL698��ȫģʽ����ʱ�ȳ�������� �� ESAM���к�
#define RM_SECURITY_MeterNo_OAD 0xF1000200//DL698��ȫģʽ����ʱ�����ȳ�����OAD,���ܱ���=0x40020200,���ܱ�ESAM���к�=0xF1000200
#define RM_SECURITY_CompMAC     1//0=��,1=��,��ȫģʽ����ʱ����MACУ����Ƚ�
#define CompareTime_5002    	   0//�������Ӷ���(����������)�Ƿ�Ƚ϶���ʱ��:0=���Ƚ�,1=�Ƚ�
//#define CompareTime_5004     	   0//�����ն����Ƿ�Ƚ϶���ʱ��:0=���Ƚ�,1=�Ƚ�
#define CompareMeterEventCount     1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
#define CurveTaskTImin     (60*60)//�ز������߳�������ִ��Ƶ����Сֵ(��)
#define CurveTaskTImax     (24*60*60)//�ز������߳�������ִ��Ƶ�����ֵ(��),��ֵ�������ɼ������ɼ����ʱ�����ǰ���������ߵ���
#define NUMmax_TIDOT     (3*96)//�����ж�ʱ���TI��������ݵ���(����������������ʱ��)
#define EnMeterAutoReportEvent     1//������ܱ������ϱ��¼�:0=������,1=����
#define NUM_UART4RxCopyMax     0//32//���ܱ������ϱ�������󻺴�֡��
#define NUMmax_50xx     127//��󶳽����OI�����ø�����(<=127)
#define SEL_RouterRunMode3ADDR     1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
#define USE_ClientConnectInfo     0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��


#define WaitHeartberatACKMinTimer     30//���������ȴ���վȷ����С��ʱ��
#define WaitHeartberatACKMaxTimer     180//600//���������ȴ���վȷ�����ʱ��
#define WaitHostACKMinTimer     5//�����ϱ��ȴ���վȷ����С��ʱ��
#define WaitHostACKMaxTimer     120//60//�����ϱ��ȴ���վȷ�����ʱ��
#define RouterCheckTimerVal     (120*60)//�ز�·���������붨ʱֵ
#define RMRS485OverTimer     3000//RS485����ʱ��ʱֵms
#define RS232DefaultBPS     (0xb+(6<<5))//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     (0xb+(6<<5))//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     (0xb+(2<<5))//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...

#define SMSEQ2Byte     1//0=û,1=��;���Ŵ���ͨ�������ݵ�Ԫ��2�ֽڶ��Ŵ���ӿ�(��׼376.1����)
#define SM_CMGF     0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
#define LCDLn9Code     1//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬(0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�)

#define ProtocolNO_ABB     	5//ABB��ͨ��Э����
#define ProtocolNO_Landis   6//Landis��ͨ��Э����(IEC1107 ģʽC)
//---�ն˽���--------------------------------



//UART------------------------------------------------------------------------------------------
#define RS232PORT     0//RS232ͨ�ſں�
#define GPRSPORT     1//GPRSͨ�ſں�
#define RS485_1PORT     2//RS485_1ͨ�ſں�
#define RS485_2PORT     3//RS485-2ͨ�ſں�
#define RS485_4PORT     4//RS485_4ͨ�ſں�(�������ز�,ר�����ģ��)
#define RS485_3PORT     5//RS485_3ͨ�ſں�(WI-FI)
#define IRPORT        6//����ͨ�ſں�
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
#define LEN_UART0IntRx     2048//UART0�жϽ��ջ��峤��
#define LEN_UART0Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART0�������ݳ���
#define LEN_UART0Rx     2048//UART0���ջ��峤��
#define LEN_UART0Tx     2048//UART0���ͻ��峤��
#define LEN_UART0TxMax     (2*LEN_UART0Tx)//UART0���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART0Tx<=LEN_UART0TxMax)
#define LEN_UART0FnDataBuff     8192//UART0����ת�����ݻ���+�����ϱ�֡���壬����>=2*LEN_UARTnTx
//UART1(GPRS)
#define LEN_UART1IntATRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��AT����
#define LEN_UART1IntRx     (LEN_DATABUFF+1024)//UART1�жϽ��ջ��峤��
#define LEN_UART1Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART1�������ݳ���
#define LEN_UART1Rx     8192//UART1���ջ��峤��
#define LEN_ATTx     64//AT���ͻ��峤��
#define LEN_UART1Tx     2048//UART1���ͻ��峤��
#define LEN_UART1TxMax     (2*LEN_UART1Tx)//UART1���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART1Tx<=LEN_UART1TxMax)
#define LEN_UART1FnDataBuff     8192//UART1����ת�����ݻ���+�����ϱ�֡���壬����>=2*LEN_UARTnTx
//UART2(RS485-1)
#define LEN_UART2IntRx     2048//UART2�жϽ��ջ��峤��
#define LEN_UART2Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART2�������ݳ���
#define LEN_UART2Rx     2048//UART2���ջ��峤��
#define LEN_UART2Tx     2048//UART2���ͻ��峤��
#define LEN_UART2TxMax     (2*LEN_UART2Tx)//UART2���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART2Tx<=LEN_UART2TxMax)
#define LEN_UART2FnDataBuff     8192//UART2����Fn���ݻ��峤��(ע:ȷ��>LEN_RM_DI_CSD+4)
//UART3(RS485-2)
#define LEN_UART3IntRx     2048//UART3�жϽ��ջ��峤��
#define LEN_UART3Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART3�������ݳ���
#define LEN_UART3Rx     2048//UART3���ջ��峤��
#define LEN_UART3Tx     2048//UART3���ͻ��峤��
#define LEN_UART3TxMax     (2*LEN_UART3Tx)//UART3���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART3Tx<=LEN_UART3TxMax)
#define LEN_UART3FnDataBuff     8192//UART3����Fn���ݻ��峤��(ע:ȷ��>LEN_RM_DI_CSD+4)
//UART4(RS485-4)(�ز�)
#define LEN_UART4IntRx     2048//UART4�жϽ��ջ��峤��
#define LEN_UART4Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART4�������ݳ���
#define LEN_UART4Rx     2048//UART4���ջ��峤��
#define LEN_UART4Tx     2048//UART4���ͻ��峤��
#define LEN_UART4TxMax     2048//UART4���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART4Tx<=LEN_UART4TxMax)
	#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
#define LEN_UART4FnDataBuff     8192//UART4����Fn���ݻ��峤��(ע:ȷ��>LEN_RM_DI_CSD+4)
	#else
#define LEN_UART4FnDataBuff     2560//UART4����Fn���ݻ��峤��(ע:ȷ��>LEN_RM_DI_CSD+4)
	#endif
//UART5(RS485-3)(WI-FI)
#define LEN_UART5IntRx     8//UART5�жϽ��ջ��峤��
#define LEN_UART5Ctrl     0//UART5�������ݳ���
#define LEN_UART5Rx     0//UART5���ջ��峤��
#define LEN_UART5Tx     0//UART5���ͻ��峤��
#define LEN_UART5TxMax     0//(2048+64)//UART5���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART5Tx<=LEN_UART5TxMax)
#define LEN_UART5FnDataBuff     0//UART5����Fn���ݻ��峤��
//UART6(����)
#define LEN_UART6IntRx     2048//UART6�жϽ��ջ��峤��
#define LEN_UART6Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART6�������ݳ���
#define LEN_UART6Rx     2048//UART6���ջ��峤��
#define LEN_UART6Tx     2048//UART6���ͻ��峤��
#define LEN_UART6TxMax     (2*LEN_UART6Tx)//UART6���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART6Tx<=LEN_UART6TxMax)
#define LEN_UART6FnDataBuff     2048//UART6����ת�����ݻ���+�����ϱ�֡���壬����>=2*LEN_UARTnTx
//UART7(�ն�ESAM)
#define LEN_UART7IntRx     8//UART7�жϽ��ջ��峤��
#define LEN_UART7Ctrl     1024//UART7�������ݳ���
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
#define LEN_UART10Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128)+3)&0xfffffffc)//UART10�������ݳ���
#define LEN_UART10Rx     8192//UART10���ջ��峤��
#define LEN_UART10Tx     2048//UART10���ͻ��峤��
#define LEN_UART10TxMax     (2*LEN_UART10Tx)//UART10���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART10Tx<=LEN_UART10TxMax)
#define LEN_UART10FnDataBuff     8192//UART10����ת�����ݻ���+�����ϱ�֡���壬����>=2*LEN_UARTnTx
//UART11(SMS)
#define LEN_UART11IntRx     8//UART11�жϽ��ջ��峤��
#define LEN_UART11Ctrl     0//UART11�������ݳ���
#define LEN_UART11Rx     0//UART11���ջ��峤��
#define LEN_UART11Tx     0//UART11���ͻ��峤��
#define LEN_UART11TxMax     0//UART11���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART11Tx<=LEN_UART11TxMax)
#define LEN_UART11FnDataBuff     0//UART11����Fn���ݻ��峤��
//UART12(�����)
#define LEN_UART12IntRx     8//UART12�жϽ��ջ��峤��
#define LEN_UART12Ctrl     (((1024+((15+LEN_UARTnTask)*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax))+3)&0xfffffffc)//UART12�������ݳ���
#define LEN_UART12Rx     2048//UART12���ջ��峤��
#define LEN_UART12Tx     2048//UART12���ͻ��峤��
#define LEN_UART12TxMax     2048//UART12���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART12Tx<=LEN_UART12TxMax)
#define LEN_UART12FnDataBuff     8192//UART12����Fn���ݻ��峤��

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART13IntRx     (10+1024+2)//UART13�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART13Ctrl     1024//UART13�������ݳ���
#define LEN_UART13Rx     0//UART13���ջ��峤��
#define LEN_UART13Tx     1024//UART13���ͻ��峤��
#define LEN_UART13TxMax     1024//UART13���ͻ���Ԥ������󳤶�(ע:ȷ��LEN_UART13Tx<=LEN_UART13TxMax)
#define LEN_UART13FnDataBuff     0//UART13����Fn���ݻ��峤��
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define LEN_UART14IntRx     (10+4096+2)//UART14�жϽ��ջ��峤��(���ջ����W5200�ڽ��ջ������ٶ�1,���0)
#define LEN_UART14Ctrl     1024//UART14�������ݳ���
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
#define iMeterProtocol  2//�ڲ����ɳ����Լ:2=DL/T645-2007��3=DL/T698.45
#define MMaxTariff     	4//������
#define MeterHarmonic   0//г������0=û,1=��
#define MeterMaxLadder  6//������
#define MaxMonth     		12//�¶�����
#define MaxMeterCurveCongeal     (96*10)//������ߵ���,60��������(����ʱ��Խ��)
#define MeterMaxDemandCalculate  1//����������㣺0=���巨,1=����ƽ����

#define MeterCtrl     0//�Ʒѿ���:0�޿���,1��Զ��,2���غ�Զ��
#define MeterType     0//�������:0��Ԥ���ѱ�,1������Ԥ���ѱ�,2�����Ԥ���ѱ�
#define MeterSpec     20//���ܱ���
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
//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
//MeterSpec=20 ����Ӧ:0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh

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
#if MeterSpec==19
//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     3200//����й�����imp/kWh
#define MeterActiveConstbcd     0x3200//����й�����imp/kWh
#define MeterReactiveConst     3200//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x3200//����޹�����imp/kvarh
#define MeterUBhex     2200//�αȵ�ѹxxx.x(V)
#define MeterUBbcd     0x2200//�αȵ�ѹxxx.x(V)
#define MeterIBhex     100//�αȵ���xx.xx(A)
#define MeterIBbcd     0x100//�αȵ���xx.xx(A)
#define MeterIMaxhex     8000//������xx.xx(A)
#define MeterIMaxbcd     0x8000//������xx.xx(A)
#define MeterPStartupHex     ((MeterUBhex*MeterIBhex*0.00066)/2)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     ((MeterUBhex*MeterIBhex*0.00066)/2)//�޹��𶯹���xxxxx.xxx(var)
#define ADC_Vu     0.14//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.033//Inʱ����ADC������Чֵ(ATT7022��)
#endif
#if MeterSpec==20//�Զ���Ӧ���ܱ���
//MeterSpec=20 0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh
#define MeterClass     0x05//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
//#define MeterPW     0//��ʼ���߷�ʽ;0=��������,1=��������
#define MeterFreq     0//�Ƶ��0=50Hz,1=60Hz
#define MeterActiveConst     10000//����й�����imp/kWh
#define MeterActiveConstbcd     0x10000//����й�����imp/kWh
#define MeterReactiveConst     10000//����޹�����imp/kvarh
#define MeterReactiveConstbcd     0x10000//����޹�����imp/kvarh
#define MeterUBhex     577//�αȵ�ѹxxx.x(V)(����С��ѹ���ֵ��Ҫ��ʧѹ�¼���,��У��ԭ��)
#define MeterUBbcd     0x577//�αȵ�ѹxxx.x(V)
#define MeterIBhex     100//�αȵ���xx.xx(A)(����С��ѹ���ֵ��Ҫ��ʧ���¼���,��У��ԭ��)
#define MeterIBbcd     0x100//�αȵ���xx.xx(A)
#define MeterIMaxhex     8000//������xx.xx(A)(�����������ֵ��У���������ԭ��)
#define MeterIMaxbcd     0x8000//������xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�й��𶯹���xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//�޹��𶯹���xxxxx.xxx(var)

#define ADC_Vu     0.14//Unʱ��ѹADC������Чֵ(ATT7022��)
#define ADC_Vi     0.033//Inʱ����ADC������Чֵ(ATT7022��)
#define MeterActiveConst220V     3200//220V����й�����imp/kWh
#define MeterActiveConst100V     10000//100V����й�����imp/kWh
#define MeterActiveConst57V     10000//57.7V����й�����imp/kWh
#endif
//���ܱ����------------------------------------------------------------------------------------












//Ӳ���汾--------------------------------------------------------------------------------------
#if (HVER==0x410)//Ӳ���汾��4.10
#define IC_STM32F42x     1//0=û,1=��
#define IC_STM32F46x     0//0=û,1=��
#define IC_NUC972     0//0=û,1=��
#define IC_NUC975     0//0=û,1=��
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
	#define FileFactory_PACK          "PACK698.*"
	#define FileFactory_MS            "MS698.*"
	#define FileFactory_OS            "OS698.*"
	#define FileFactory_Meter         "Meter698.*"
	#define FileFactory_Terminal      "Terminal698.*"
	#define FileFactory_Menu          "Menu698.*"
	#define FileFactory_Font          "Font.*"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test698.*"
	#define FileFactory_Modele        "Module698.*"
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
#define FileUserDIR          "/JK1600"
#if MeterSpec==0//���ܱ���//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
	#define FileUser_PACK      "PACK698_JK1600_000_V41.*"
	#define FileUser_MS        "MS698_JK1600_000_V41.*"
	#define FileUser_OS        "OS698_JK1600_000_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_000_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_000_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_000_V41.*"
	#define FileUser_Font      "Font698_JK1600_000_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_000_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_000_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_000_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_000_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_000_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_000_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_000_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_000_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_000_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_000_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_000_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_000_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_000_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_000_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_000_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_000_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_000_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_000_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_000_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_000_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_000_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_000_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_000_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_000_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_000_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_000_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_000_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_000_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_000_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_000_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_000_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_000_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_000_V41_SR.zip"
#endif

#if MeterSpec==13//���ܱ���//MeterSpec=13 0.5S 3P4W 50Hz 6400imp/kWh 220V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1300_V41.*"
	#define FileUser_MS        "MS698_JK1600_1300_V41.*"
	#define FileUser_OS        "OS698_JK1600_1300_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1300_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1300_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1300_V41.*"
	#define FileUser_Font      "Font698_JK1600_1300_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1300_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1300_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1300_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1300_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1300_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1300_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1300_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1300_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1300_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1300_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1300_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1300_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1300_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_1300_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1300_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1300_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1300_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1300_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1300_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1300_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1300_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1300_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1300_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1300_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1300_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1300_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1300_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1300_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1300_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1300_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1300_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1300_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1300_V41_SR.zip"
#endif
#if MeterSpec==14//���ܱ���//MeterSpec=14 0.5S 3P3W 50Hz 20000imp/kWh 100V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1400_V41.*"
	#define FileUser_MS        "MS698_JK1600_1400_V41.*"
	#define FileUser_OS        "OS698_JK1600_1400_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1400_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1400_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1400_V41.*"
	#define FileUser_Font      "Font698_JK1600_1400_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1400_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1400_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1400_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1400_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1400_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1400_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1400_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1400_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1400_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1400_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1400_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1400_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1400_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_1400_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1400_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1400_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1400_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1400_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1400_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1400_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1400_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1400_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1400_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1400_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1400_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1400_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1400_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1400_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1400_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1400_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1400_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1400_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1400_V41_SR.zip"
#endif
#if MeterSpec==15//���ܱ���//MeterSpec=15 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1(10)A
	#define FileUser_PACK      "PACK698_JK1600_1500_V41.*"
	#define FileUser_MS        "MS698_JK1600_1500_V41.*"
	#define FileUser_OS        "OS698_JK1600_1500_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1500_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1500_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1500_V41.*"
	#define FileUser_Font      "Font698_JK1600_1500_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1500_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1500_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1500_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1500_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1500_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1500_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1500_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1500_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1500_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1500_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1500_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1500_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1500_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_1500_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1500_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1500_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1500_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1500_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1500_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1500_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1500_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1500_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1500_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1500_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1500_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1500_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1500_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1500_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1500_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1500_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1500_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1500_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1500_V41_SR.zip"
#endif
#if MeterSpec==19//���ܱ���//MeterSpec=19 0.5S 3P4W 50Hz 3200imp/kWh 220V 1(80)A
	#define FileUser_PACK      "PACK698_JK1600_1900_V41.*"
	#define FileUser_MS        "MS698_JK1600_1900_V41.*"
	#define FileUser_OS        "OS698_JK1600_1900_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_1900_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_1900_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_1900_V41.*"
	#define FileUser_Font      "Font698_JK1600_1900_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_1900_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_1900_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_1900_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_1900_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_1900_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_1900_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_1900_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_1900_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_1900_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_1900_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_1900_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_1900_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_1900_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_1900_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_1900_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_1900_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_1900_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_1900_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_1900_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_1900_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_1900_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_1900_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_1900_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_1900_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_1900_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_1900_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_1900_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_1900_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_1900_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_1900_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_1900_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_1900_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_1900_V41_SR.zip"
#endif
#if MeterSpec==20//���ܱ���///MeterSpec=20 0.5S 3P4W 57.7V 1.5(6)A 10000imp/kWh;3P3W 100V 1.5(6)A 10000imp/kWh;3P4W 220V 1(80)A 3200imp/kWh
	#define FileUser_PACK      "PACK698_JK1600_2000_V41.*"
	#define FileUser_MS        "MS698_JK1600_2000_V41.*"
	#define FileUser_OS        "OS698_JK1600_2000_V41.*"
	#define FileUser_Meter     "Meter698_JK1600_2000_V41.*"
	#define FileUser_Terminal  "Terminal698_JK1600_2000_V41.*"
	#define FileUser_Menu      "Menu698_JK1600_2000_V41.*"
	#define FileUser_Font      "Font698_JK1600_2000_V41.*"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698_JK1600_2000_V41.*"
	#define FileUser_PLC_50    "PLC_JK1600_2000_V41_50.*"
	#define FileUser_PLC_TC    "PLC_JK1600_2000_V41_TC.*"
	#define FileUser_PLC_ES    "PLC_JK1600_2000_V41_ES.*"
	#define FileUser_PLC_CX    "PLC_JK1600_2000_V41_CX.*"
	#define FileUser_PLC_01    "PLC_JK1600_2000_V41_01.*"
	#define FileUser_PLC_LH    "PLC_JK1600_2000_V41_LH.*"
	#define FileUser_PLC_NX    "PLC_JK1600_2000_V41_NX.*"
	#define FileUser_PLC_QL    "PLC_JK1600_2000_V41_QL.*"
	#define FileUser_PLC_MX    "PLC_JK1600_2000_V41_MX.*"
	#define FileUser_PLC_FC    "PLC_JK1600_2000_V41_FC.*"
	#define FileUser_PLC_NW    "PLC_JK1600_2000_V41_NW.*"
	#define FileUser_PLC_SR    "PLC_JK1600_2000_V41_SR.*"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698_JK1600_2000_V41.zip"
	#define FileFTP_MS        "MS698_JK1600_2000_V41.zip"
	#define FileFTP_OS        "OS698_JK1600_2000_V41.zip"
	#define FileFTP_Meter     "Meter698_JK1600_2000_V41.zip"
	#define FileFTP_Terminal  "Terminal698_JK1600_2000_V41.zip"
	#define FileFTP_Menu      "Menu698_JK1600_2000_V41.zip"
	#define FileFTP_Font      "Font698_JK1600_2000_V41.zip"
	#define FileFTP_MODULE    "Module698_JK1600_2000_V41.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_2000_V41_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_2000_V41_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_2000_V41_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_2000_V41_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_2000_V41_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_2000_V41_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_2000_V41_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_2000_V41_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_2000_V41_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_2000_V41_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_2000_V41_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_2000_V41_SR.zip"
#endif
#endif
//Ӳ���汾4.10����----------------------------------------------------------------------------------

//Ӳ���汾--------------------------------------------------------------------------------------
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
	#define FileFactory_PACK          "PACK698N.zip"
	#define FileFactory_MS            "MS698N.zip"
	#define FileFactory_OS            "OS698N.zip"
	#define FileFactory_Meter         "Meter698N.zip"
	#define FileFactory_Terminal      "Terminal698N.zip"
	#define FileFactory_Menu          "Menu698N.zip"
	#define FileFactory_Font          "FontN.zip"
	#define FileFactory_Font_ttf      "*.ttf"
	#define FileFactory_Test          "Test698N.zip"
	#define FileFactory_Modele        "Module698N.zip"
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
#define FileUserDIR          "/JK1600"
#if MeterSpec==20//�Զ���Ӧ���ܱ���
	#define FileUser_PACK      "PACK698N_JK1600_2000_V50.zip"
	#define FileUser_MS        "MS698N_JK1600_2000_V50.zip"
	#define FileUser_OS        "OS698N_JK1600_2000_V50.zip"
	#define FileUser_Meter     "Meter698N_JK1600_2000_V50.zip"
	#define FileUser_Terminal  "Terminal698N_JK1600_2000_V50.zip"
	#define FileUser_Menu      "Menu698N_JK1600_2000_V50.zip"
	#define FileUser_Font      "FontN_JK1600_2000_V50.zip"
	#define FileUser_Font_ttf  "*.ttf"
	#define FileUser_MODULE    "Module698N_JK1600_2000_V50.zip"
	#define FileUser_PLC_50    "PLC_JK1600_2000_V50_50.zip"
	#define FileUser_PLC_TC    "PLC_JK1600_2000_V50_TC.zip"
	#define FileUser_PLC_ES    "PLC_JK1600_2000_V50_ES.zip"
	#define FileUser_PLC_CX    "PLC_JK1600_2000_V50_CX.zip"
	#define FileUser_PLC_01    "PLC_JK1600_2000_V50_01.zip"
	#define FileUser_PLC_LH    "PLC_JK1600_2000_V50_LH.zip"
	#define FileUser_PLC_NX    "PLC_JK1600_2000_V50_NX.zip"
	#define FileUser_PLC_QL    "PLC_JK1600_2000_V50_QL.zip"
	#define FileUser_PLC_MX    "PLC_JK1600_2000_V50_MX.zip"
	#define FileUser_PLC_FC    "PLC_JK1600_2000_V50_FC.zip"
	#define FileUser_PLC_NW    "PLC_JK1600_2000_V50_NW.zip"
	#define FileUser_PLC_SR    "PLC_JK1600_2000_V50_SR.zip"
//FTP���ļ���
	#define FileFTP_PACK      "PACK698N_JK1600_2000_V50.zip"
	#define FileFTP_MS        "MS698N_JK1600_2000_V50.zip"
	#define FileFTP_OS        "OS698N_JK1600_2000_V50.zip"
	#define FileFTP_Meter     "Meter698N_JK1600_2000_V50.zip"
	#define FileFTP_Terminal  "Terminal698N_JK1600_2000_V50.zip"
	#define FileFTP_Menu      "Menu698N_JK1600_2000_V50.zip"
	#define FileFTP_Font      "FontN_JK1600_2000_V50.zip"
	#define FileFTP_MODULE    "Module698N_JK1600_2000_V50.zip"
	#define FileFTP_PLC_50    "PLC_JK1600_2000_V50_50.zip"
	#define FileFTP_PLC_TC    "PLC_JK1600_2000_V50_TC.zip"
	#define FileFTP_PLC_ES    "PLC_JK1600_2000_V50_ES.zip"
	#define FileFTP_PLC_CX    "PLC_JK1600_2000_V50_CX.zip"
	#define FileFTP_PLC_01    "PLC_JK1600_2000_V50_01.zip"
	#define FileFTP_PLC_LH    "PLC_JK1600_2000_V50_LH.zip"
	#define FileFTP_PLC_NX    "PLC_JK1600_2000_V50_NX.zip"
	#define FileFTP_PLC_QL    "PLC_JK1600_2000_V50_QL.zip"
	#define FileFTP_PLC_MX    "PLC_JK1600_2000_V50_MX.zip"
	#define FileFTP_PLC_FC    "PLC_JK1600_2000_V50_FC.zip"
	#define FileFTP_PLC_NW    "PLC_JK1600_2000_V50_NW.zip"
	#define FileFTP_PLC_SR    "PLC_JK1600_2000_V50_SR.zip"
#endif
#endif
//Ӳ���汾5.00����----------------------------------------------------------------------------------









#endif






























