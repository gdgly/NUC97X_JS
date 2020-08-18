

#ifndef IRAM_H
#define IRAM_H


#ifdef IC_STM32F4xx
//0x10000000 64k(CCM DATA RAM)
//0x20000000 112k
//0x2001c000 16k (Ethernet or the USB OTG HS)
//0x20020000 64k(STM32F42xxx)
//0x40024000 4K(BKPSRAM)
//RTC ���ݼĴ��� 20��
#define ADDR_IRAM_Start     0x10000000//64KB(CCM DATA RAM)
#endif
#ifdef IC_NUC9xx
#define ADDR_IRAM_Start     0x00F00000//256K
#endif


#define ADDR_Comm_Ram_Start     ADDR_IRAM_Start
// ���������ϵ��ʼ����0--------------------------------------------------------------------------------------------
// 4 �ֽ�������ʾ�ַ�ת��Ϊ����LCDDot��������(��Ϊ�̶���ַ0x10000000Ҫ��)
#define ADDR_MRWBUFF     (ADDR_IRAM_Start+4)
#define LEN_MRWBUFF     12
// 12�ֽڴ洢����д���Ĵ�������,8+4�ֽ�,����4�ֽڽ�Ϊ��ʾ����
// 4byte �洢����д��ADDR_Sour��Data(����д��־=MCʱ)
// 4byte �洢����д��ADDR_Dest
// 4byte �洢����д��Byte
// 1byte �洢����д�ı�־:0=��,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
// 1byte NAND��д��־
// 1byte nand�ڲ�ECC:0=������,1=����
// 1byte os״̬:0=û��,1=��,2=��ͣ
typedef __packed struct
{
	u32 ADDR_Sour_or_Data;			// �洢����д��ADDR_Sour��Data(����д��־=MCʱ)
	u32 ADDR_Dest;					// �洢����д��ADDR_Dest
	u32 Byte;						// �洢����д��Byte
	u8 RWC;							// �洢����д�ı�־:0=��,1=MR,2=MW,3=MC,4=ME,5=DMW,6=DMC,7=DMWR
	u8 NAND;						// NAND��д��־
	u8 NAND_ECC;					// nand�ڲ�ECC,0x55=����,����ֵ=������
	u8 osSTATE;						// os״̬:0=û��,1=��,2=��ͣ
}MRW_TypeDef;
#define MRW     ((MRW_TypeDef *)(ADDR_MRWBUFF+LEN_MRWBUFF))
#define ADDR_DATABUFF     (ADDR_MRWBUFF+LEN_MRWBUFF+16)
									// 4K�ֽ� LEN_DATABUFF�ֽڶ�д���ݻ���
#define ADDR_4KWRITE_BUFF     (ADDR_DATABUFF+LEN_DATABUFF)
									// 4K�ֽ�FLASHд����(�Ͻ���ADDR_DATABUFF,����ʹ��ʱLEN_DATABUFF���ٴﵽ8K)

#define ADDR_FileDownload     (ADDR_4KWRITE_BUFF+4096)
typedef __packed struct
{
	u32 File;				// �ļ���־(��λ��ʾOK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,B8=Test,...B31=
	u8 USBH_Initialize;		// USBH:0=û��ʼ��,1=��ʼ��
	u8 DownloadCount;		// �ļ����ؼ���
	u8 UpdateCount;			// �ļ����¼���
	u8 New;					// �����ļ������־:0=��,1=û
	u8 USB;					// ����״̬�˲�:b0=��������,b1=�豸����
	u8 PORTn;				// ������صĶ˿ں�
	u8 NAND_BUSBIT;//nand����λ����Զ�ʶ��:8=8bit,16=16bit
	u8 USB0Task;
}FileDownload_TypeDef;
#define FileDownload     ((FileDownload_TypeDef *)(ADDR_FileDownload))

#define ADDR_FilePlay     (ADDR_FileDownload+12)
typedef __packed struct
{
	u32 LEN_FILE;			// �ļ��ܳ���
	u32 LEN_FILEREAD;		// �Ѷ��ļ�����
	u32 ADDR_Myalloc_Play;	// ��ǰ���ŵ��ļ�������ڴ�ָ��Ĵ�,������ʱռ���ڴ潫���ͷ�,ͬʱ��!=0��ʾ���ڲ���
    u32 ADDR_WAVDATA;		// WAV���ݿ�ʼ��ַ
	u32 Hz;					// ����Ƶ��
	u8  Bits;				// λ��
	u8  CHs;				// ͨ����
	u8  res1;
	u8  res2;
  	u32 LEN_WAVDATA;		// WAV���ݳ���
  	u32 LEN_WAVDATAOUT;		// �Ѳ���WAV���ݳ���
	u8 FileName[32];		// ��ǰ���ŵ��ļ���
}FilePlay_TypeDef;
#define FilePlay     ((FilePlay_TypeDef *)(ADDR_FilePlay))

typedef __packed struct
{
#define ADDR_SYSTEM     (ADDR_FilePlay+64)
	u32 IRQ0;				// ����OSʱԭ�ж�������
	u32 IRQ1;				// ����OSʱԭ�ж�������
	u32 IRQ2;				// ����OSʱԭ�ж�������
	u32 res0;
	u32 res1;
	u32 ADDR_Myalloc_TTF;	// ttf�ļ�������ڴ�ָ��Ĵ�
	u8 Init_LOG;			// �ϵ��LOG�ѳ�ʼ����־:0=û��!=0�ѳ�ʼ��
	u8 Init_emWin;			// �ϵ��emWin�ѳ�ʼ����־:=0û��ʼ��,=1��ʼ��ʧ��,>=2��ʼ���ɹ�
	u8 RealDataSave;		// �ϴ�ʵʱ���ݱ�������
	u8 TimerDataSave;		// �ϴζ�ʱ���ݱ��������ֻ�ʱ
	u8 outclk;				// ���ʱ��:B0=TEASMCLK,B1=MESAMCLK,B2=CARDCLK
	u8 POWER;				// ��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	u8 outpower;			// �����ԴB0=A5V0,B1=BACKLIGHT,B2=LED3V3,B3=E5V0,B4=R5V0,B5=USB5V0,B6=,B7=�жϹص�Դ��־
	u8 Program;				// (���ȡ��ַ)��ǰ���еĳ����(����HardFault�ж�ʱ����OSʶ��):0=��,1=FS,2=USB0,3=USB1,4=ETH,5=...
	u8 Init_OS;				// OS������ԭ���־:B0=�������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	u8 After_Init_OS;		// OS������ʹ�ñ�־:B0=�������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=TEST
	u8 MS_Task;				// MS����
	u8 IntFlags;			// �жϷ����ͻ��־
							// B0��ʾ�ж���ʹ��CRC�����ͻ��־
							// B1=�ж���дIEEPROM��ͻ��־
							// B2=�ж���ʹ��ADC��ͻ��־
							// B3=�ж��ж�дEFLASH��ͻ��־
							//
	u8 RESET;				// 0=���Ź���λ,1=�ϵ縴λ,0x55=������(����ʹ�ÿ��Ź���λ)
	u8 ACTION_RESET_PORTn;	// ������λ��ͨ�Ŷ˿ں�
	u16 msFILESearchCount;	// �ļ���������
	u8 msFILEchange;		// �ļ��������ݱ仯:0=�仯,!=û�仯(1=�������ݱ���,2=���������ϱ�����)
	u8 DataSave;			// ���ݱ���:0=��,1=�����е糬��10�������豣��,2=�����ѱ���
	u16 TESTKEY;
							// B0=���ư���(�ֶ�)
							// B1=���ư���(�ֶ�)
							// B2=���ư���(�ֶ�)
							// B3=���ư���(�ֶ�)
							// B4=ȷ�ϰ���(�ֶ�)
							// B5=�˳�����(�ֶ�)
							// B6=���ð���(�ֶ�)
							// B7=��������(�ֶ�)
							// B8=��ǰ���(�ֶ�)
							// B9=�˸ǰ���(�ֶ�)
	u32 ErrorIC;			// ʵʱ����ʱ���ֵĴ���
							// B0=STM32F4xx SRAM
							// B1=ERAM �ⲿRAMоƬ(MT48LC4M16��MT48LC8M16)
							// B2=EFLASH1 S25FLxxx
							// B3=EFLASH2 S34MLxxG1
							// B4=RTC �ⲿRTCʱ��оƬ(DS3231��RX8025T)
							// B5=EMU ����оƬ(ATT7022E,IDC90E36)
							// B6=PCF8562 ��ʽLCD����оƬ
							// B7=LAN8720A	��̫��PHYоƬ
							// B8=MESAM ���ܱ���ESAMоƬ
							// B9=TESAM �ն���ESAMоƬ
							// B10=24AAxxx ��̫����MAC��ַоƬ
							// B11=
							// B12=STM32F4xx ECC
							// B13=STM32F4xx RTC
							// B14=����ͨ��(�Զ�,�Ƿ�������Է����ռ��,IO��������ж��м��)
							// B15=
							// B16=�����籣�����ݴ���
							// B17=�ն˵��籣�����ݴ���
							// B18=�������ݻ��峤�Ȳ���
							// B19=ϵͳ��ջ���Ȳ���
							// B20=UART0�շ�����
							// B21=UART1�շ�����
							// B22=UART2�շ�����
							// B23=UART3�շ�����
							// B24=UART4�շ�����
							// B25=UART5�շ�����
							// B26=UART6�շ�����
							// B27=
							// B28=
							// B29=��س���Դ
							// B30=GPRSģ���Դ
							// B31=LPC11xxר���ÿ���ģ��
#define ADDR_RTCBUFF      	(ADDR_SYSTEM+48)		// ʱ�ӻ���(�ֶ���)
  u8 RTCBuff[8];									// ��,��,ʱ,��,��,��,��,NC
#define ADDR_MYMDHMS     	(ADDR_RTCBUFF+8)		// �Ǳ�ʱ��
  u8 MYMDHMS[8];									// ��,��,ʱ,��,��,��,��,NC
#define ADDR_DL698YMDHMS    (ADDR_MYMDHMS+8)		// DL698ʱ��(hex)
  u8 DL698YMDHMS[10];								// ����(�ȸ��ֽں���ֽ�),��,��,ʱ,��,��,�ٷ���,��
#define ADDR_URTC     		(ADDR_DL698YMDHMS+10)	// ʱ�ӵ�ص�ѹ(�ڲ�)(���ֶ���)
  u16 URTC;											// BCD 2λС��
#define ADDR_ULCD     		(ADDR_URTC+2)			// ͣ�糭���ص�ѹ(�ⲿ)(���ֶ���)
  u16 ULCD;											// BCD 2λС��
#define ADDR_UBACKUP     	(ADDR_ULCD+2)			// �ɳ�籸�õ�ص�ѹ(�ⲿ)(���ֶ���)
  u16 UBACKUP;										// BCD 2λС��
#define ADDR_UGPRS     		(ADDR_UBACKUP+2)		// GPRS��Դ��ѹ(���ֶ���)
  u16 UGPRS;										// BCD 2λС��
#define ADDR_UDC     		(ADDR_UGPRS+2)			// ֱ��ģ����(���ֶ���)
  u16 UDC;											// HEX
#define ADDR_TEMP     		(ADDR_UDC+2)			// �����¶�(���ֶ���)
  u16 TEMP;											// BCD 1λС��(b15����λ)
#define ADDR_BUTTON     	(ADDR_TEMP+2)
													// λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
  u16 ButtonVal;									// ����״̬(ʹ�ú���0)
  u16 ButtonKeep;									// ���±���3��״̬(ʹ�ú���0)
  u16 ButtonCurrent;								// �м����
  u8 Factory;										// 0x55=����״̬
  u8 msFILEcheck;									// �ϴ�msFILE���ʱ��ʵʱ��
#define ADDR_Time_Int     	(ADDR_BUTTON+8)
	u8 MS1_PR; 										// 1mS��10��Ƶ����10mS
	u8 MS10_PR; 									// 10mS��100��Ƶ����1S
	u8 RS485_CTRL1_Timer;							// RS485-1��1ms��ʱ�ж��е���ʱֵ��Ϊ0ʱתΪ����
	u8 RS485_CTRL2_Timer;							// RS485-2��1ms��ʱ�ж��е���ʱֵ��Ϊ0ʱתΪ����
#define ADDR_Comm_MS_Timer     (ADDR_Time_Int+4)
#define ADDR_UART0Timer     	ADDR_Comm_MS_Timer
#define ADDR_UART1Timer     	ADDR_UART0Timer+2
#define ADDR_UART2Timer     	ADDR_UART1Timer+2
#define ADDR_UART3Timer     	ADDR_UART2Timer+2
#define ADDR_UART4Timer     	ADDR_UART3Timer+2
#define ADDR_UART5Timer     	ADDR_UART4Timer+2
#define ADDR_UART6Timer     	ADDR_UART5Timer+2
#define ADDR_UART7Timer     	ADDR_UART6Timer+2
#define ADDR_UART8Timer     	ADDR_UART7Timer+2
#define ADDR_UART9Timer     	ADDR_UART8Timer+2
#define ADDR_UART10Timer     	ADDR_UART9Timer+2
#define ADDR_UART11Timer     	ADDR_UART10Timer+2
#define ADDR_UART12Timer     	ADDR_UART11Timer+2
#define ADDR_UART13Timer     	ADDR_UART12Timer+2
#define ADDR_UART14Timer     	ADDR_UART13Timer+2
#define ADDR_UART15Timer     	ADDR_UART14Timer+2
#define ADDR_UART16Timer     	ADDR_UART15Timer+2
#define ADDR_GPMSTimer     		ADDR_UART16Timer+2
#define ADDR_SPI1Timer     		ADDR_GPMSTimer+2
#define ADDR_SPI2Timer     		ADDR_SPI1Timer+2
#define ADDR_SPI3Timer     		ADDR_SPI2Timer+2
  u16 UART0Timer;			// 1 UART0 ÿ10MS��1��ʱ��
  u16 UART1Timer;			// 2 UART1 ÿ10MS��1��ʱ��
  u16 UART2Timer;			// 3 UART2 ÿ10MS��1��ʱ��
  u16 UART3Timer;			// 4 UART3 ÿ10MS��1��ʱ��
  u16 UART4Timer;			// 5 UART4 ÿ10MS��1��ʱ��
  u16 UART5Timer;			// 6 UART5 ÿ10MS��1��ʱ��
  u16 UART6Timer;			// 7 UART6 ÿ10MS��1��ʱ��
  u16 UART7Timer;			// 8 UART7 ÿ10MS��1��ʱ��
  u16 UART8Timer;			// 9 UART8 ÿ10MS��1��ʱ��
  u16 UART9Timer;			// 10 UART9 ÿ10MS��1��ʱ��
  u16 UART10Timer;			// 11 UART10 ÿ10MS��1��ʱ��
  u16 UART11Timer;			// 12 UART11 ÿ10MS��1��ʱ��
  u16 UART12Timer;			// 13 UART12 ÿ10MS��1��ʱ��
  u16 UART13Timer;			// 14 UART13 ÿ10MS��1��ʱ��
  u16 UART14Timer;			// 15 UART14 ÿ10MS��1��ʱ��
  u16 UART15Timer;			// 16 UART15 ÿ10MS��1��ʱ��
  u16 UART16Timer;			// 17 UART16 ÿ10MS��1��ʱ��
  u16 GPMSTimer;			// 18 ͨ��(������UART11) ÿ10MS��1��ʱ��
  u16 SPI1Timer;			// 19 SSP0 ÿ10MS��1��ʱ��
  u16 SPI2Timer;			// 20 SSP1 ÿ10MS��1��ʱ��
  u16 EMCTimer;				// 21 ��⵽��EMC����ÿ10MS��1��ʱ��   SPI3Timer;//21 SSP2 ÿ10MS��1��ʱ��
  u16 RUNTimer;				// 22 ����LEDָʾ�� ÿ10MS��1��ʱ��
  u16 ButtonTimer;			// 23 ����������ʱ ÿ10MS��1��ʱ��
  u16 ButtonKeepTimer;		// 24 �������� ÿ10MS��1��ʱ��
  u16 MenuOutTimer;			// 25 �˵��˳� ÿ10MS��1��ʱ��(�ն�)
  u16 USBMSTimer;			// 26 USB ÿ10MS��1��ʱ��(�ն�)
  u16 DisplayScanTimer;		// 27 ��ʾɨ�� ÿ10MS��1��ʱ��(�ն�)
  u16 CheckLEDTimer;		// 28 �ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
  u16 ReadRTCTimer;			// 29 ��ERTCÿ10MS��1��ʱ��
  u16 ChargeBatteryTimer;	// 30 �޳���ؽ���ʱ>6V���е�ؽ��붨ʱ,��Դ�½����г�����������Ϊ������ص�ѹ,��ʱһ��ʱ����ݷŵ��ADCֵ����Ϊ��ѹֵ
  u16 DataSaveTimer;		// 31 ���ݱ��涨ʱ�� ÿ10ms��1��ʱ��


#ifdef IC_NUC9xx
 u16 WaitUartCompleteMSTimer;//32 �ȴ�UART֡���ض�ʱ�� ÿ10ms��1��ʱ��
#define Comm_MS_Timer_Num     32//����10MS��ʱ����
#endif 

#ifndef Comm_MS_Timer_Num
	#define Comm_MS_Timer_Num     31//����10MS��ʱ����
#endif


#define ADDR_Comm_S_Timer     (ADDR_Comm_MS_Timer+(2*Comm_MS_Timer_Num))
	u16 DisplayAllErrorTimer;		// 1 �ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
	u16 DisplayLoopTimer;			// 2 ѭ���붨ʱ��(�ն�)
	u16 BackLightTimer;				// 3 �������ʾ�붨ʱ��(�ն�)
	u16 BattWork_S_timer;			// 4 ��ع����ػ��붨ʱ��
	
#define Comm_S_Timer_Num     4		//�����붨ʱ����

#define ADDR_BUZZCTRL     (ADDR_Comm_S_Timer+(2*Comm_S_Timer_Num))//���������ƻ���
  u8 BUZZCTRL[9];
		// 1BYTE 0 0=���е�ǰ,1=���в���
		// 1BYTE 1 ��ǰ������0=������,!=0������
		// 1BYTE 2 ��ǰ���в����0-3
		// 1BYTE 3 ���м�������
		// 1byte 4 �ȴ�����0������0=������,!=0������
		// 1byte 5 �ȴ�����1������0=������,!=0������
		// 1byte 6 �ȴ�����2������0=������,!=0������
		// 1byte 7 �ȴ�����3������0=������,!=0������
		// 1BYTE 8 ��������������/��ֹ,=0��ֹ,!=0����
#define ADDR_WaitFLASHErase     ADDR_BUZZCTRL+9
  u8 WaitFLASHErase;								// 0x55=�ȴ�FLASH�������������־
#define ADDR_RouterType     ADDR_WaitFLASHErase+1
	u8 RouterType;									// ·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��
													// 100=����,101=��ϣ,102=��Ѷ��,103=�з�,104=ɣ��
#define ADDR_RemoteWirelessModuleType     ADDR_RouterType+1
	u8 RemoteWirelessModuleType;					// Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
#define ADDR_RamLanguage     ADDR_RemoteWirelessModuleType+1//��ʾ����(ͬ��FLASH�е�ADDR_Language)
	u8 Language;									// 0=����,1=Ӣ��,2=
#define ADDR_CSPN     ADDR_RamLanguage+1			//��ȡSIM��������Ӫ������
	u8 CSPN;										// 0=��
													// 1=�й��ƶ�ͨ��-----china mobile  CMCC
													// 2=�й���ͨͨѶ-----china unicom  CUCC
													// 3=�й�����  ------CHINA TELECOM  CTCC
#define ADDR_UARText     (((ADDR_CSPN+1)+1)&0xfffffffe)
	u8 RS485_CTRL3_Timer;							// RS485-3��1ms��ʱ�ж��е���ʱֵ��Ϊ0ʱתΪ����
	u8 UART6TR;										// 0=����,1=����
	u8 UART6TIM;									// 0.5BIT��ʱ�жϼ���
	u8 UART6BITCOUNT;								// ���շ�����λ����
	u16 UART6DATA;									// ���ݼĴ���
#define ADDR_RUNSPEEDTEST     (ADDR_UARText+6)		// ѭ�������ٶȲ���(���ֶ���)
//2BYTE ÿѭ������
	u16 LOOPCOUNT;
//2BYTE ÿѭ�������������ʱ�Ĵ�
	u16 LOOPCOUNT_COPY;
#define ADDR_VDD5V0     (ADDR_RUNSPEEDTEST+4)//VDD5V0��ѹ
	u16 VDD5V0;//BCD 2λС��
#define ADDR_MODE3in1     ADDR_VDD5V0+2//����һ�ն˵�ǰ����ģʽ
	u8 MODE3in1;//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	u8 res2;//��Ϊ�ϵ���0����4byte
	u8 res3;//��Ϊ�ϵ���0����4byte
	u8 res4;//��Ϊ�ϵ���0����4byte

}Comm_Ram_TypeDef;

#define Comm_Ram     			((Comm_Ram_TypeDef *)(ADDR_SYSTEM))

////ybwhz20190221  ���� �� ���� 
#define ADDR_YBWHZ_DATABUFF   (ADDR_MODE3in1 + 4) 	//ybwhz20190221  ���� �� ���� ռ��64�ֽ� ��ȷ���Ƿ���Ӱ��?
////ybwhz20190221  ���� �� ���� 

#define ADDR_Comm_Ram_End     (ADDR_YBWHZ_DATABUFF + 64) 	//(ADDR_RUNSPEEDTEST + 4) 

#define LEN_Comm_Ram     		(9*1024)	// �޶��ܳ����ֽ���

#if (ADDR_Comm_Ram_End-ADDR_Comm_Ram_Start) > LEN_Comm_Ram
	#error ADDR_Comm_Ram_End Over
#endif
//���������ϵ��ʼ����0-------------------------------------------------------------------------------------------------


//���ܱ�����(���粻����)---------------------------------------------------------------------------------------
#define ADDR_Meter_Ram_Start     	(ADDR_IRAM_Start+LEN_Comm_Ram)
#if((Project/100) >= 5) && ((Project/100)!=9) 						// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Meter_Ram_End     	(ADDR_Meter_Ram_Start+0)
	#define LEN_Meter_Ram     0					// �޶��ܳ����ֽ���
#else
	#define LEN_Meter_Ram     		(2*1024)	//�޶��ܳ����ֽ���
	#include "IRAM_MeterCSG.h"
#endif

#if (ADDR_Meter_Ram_End - ADDR_Meter_Ram_Start) > LEN_Meter_Ram
	#error ADDR_Meter_Ram_End Over
#endif
// ���ܱ�����(���粻����)����------------------------------------------------------------------------------------


// �ն�����(���粻����)-----------------------------------------------------------------------------------------
#define ADDR_Terminal_IRAM_Start     	(ADDR_Meter_Ram_Start+LEN_Meter_Ram)
#if((Project/100) == 0)								//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Terminal_IRAM_End     	(ADDR_Terminal_IRAM_Start+0)
	#define LEN_Terminal_IRAM     		0			// �޶��ܳ����ֽ���
#else
	#define LEN_Terminal_IRAM     		(53*1024)	// �޶��ܳ����ֽ���
	#include "IRAM_TerminalCSG.h"
#endif

#if (ADDR_Terminal_IRAM_End - ADDR_Terminal_IRAM_Start) > LEN_Terminal_IRAM
	#error ADDR_Terminal_IRAM_End Over
#endif
// �ն�����(���粻����)����-------------------------------------------------------------------------------------



#define ADDR_IRAM_End     (ADDR_Terminal_IRAM_Start+LEN_Terminal_IRAM)


//#if (LEN_Comm_Ram+LEN_Meter_Ram + LEN_Terminal_Ram) > (64*1024)
//#error LEN_Comm_Ram+LEN_Meter_Ram + LEN_Terminal_Ram > 64K
#if (LEN_Comm_Ram + LEN_Meter_Ram + LEN_Terminal_IRAM) > (64*1024)
#error LEN_Comm_Ram + LEN_Meter_Ram + LEN_Terminal_IRAM > 64K
#endif
#if (ADDR_IRAM_End-ADDR_IRAM_Start) > (64*1024)
	#error ADDR_IRAM_End-ADDR_IRAM_Start > 64K
#endif

//-------------------------------------------------------------------------------------------------



	
// 0x20020000 64k(STM32F42xxx)
// ���ܱ�����(���籣��)------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
//0x20020000 64k(STM32F42xxx)
#define ADDR_Meter_SaveRam_Start     0x20020000
#endif
#ifdef IC_NUC9xx
#define ADDR_Meter_SaveRam_Start     0x00F40000//256K
#endif
#if((Project/100)>=5) && ((Project/100)!=9)						// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Meter_SaveRam_End     (ADDR_Meter_SaveRam_Start+0)
	#define LEN_Meter_SaveRam     0			// �޶��ܳ����ֽ���
#else
	#define LEN_Meter_SaveRam     (1*1024)	// �޶��ܳ����ֽ���
	#include "IRAM_MeterSaveCSG.h"
#endif

#if (ADDR_Meter_SaveRam_End-ADDR_Meter_SaveRam_Start)>LEN_Meter_SaveRam
	#error ADDR_Meter_SaveRam_End Over
#endif
//���ܱ�����(���籣��)����-------------------------------------------------------------------------------------------------
//�ն�����(���籣��)-----------------------------------------------------------------------------------------
#define ADDR_Terminal_SaveRam_Start     	(ADDR_Meter_SaveRam_Start+LEN_Meter_SaveRam)
#if((Project/100)==0)								// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Terminal_SaveRam_End     	(ADDR_Terminal_SaveRam_Start+0)
	#define LEN_Terminal_SaveRam     		0		// �޶��ܳ����ֽ���
#else
	#define LEN_Terminal_SaveRam     		((63*1024) - 8)	// �޶��ܳ����ֽ���
	#include "IRAM_TerminalSaveCSG.h"
#endif

#if (ADDR_Terminal_SaveRam_End-ADDR_Terminal_SaveRam_Start)>LEN_Terminal_SaveRam
	#error ADDR_Terminal_SaveRam_End Over
#endif
// �ն�����(���籣��)����-------------------------------------------------------------------------------------
// 4BYTE �̶���ʶ0x12345678
// 4BYTE ��ȫ������CRC32
// ���籣������-------------------------------------------------------------------------------------


#define LEN_SaveRam     (LEN_Meter_SaveRam + LEN_Terminal_SaveRam + 8)	//ע:��nand 2048ҳд����



#if LEN_SaveRam > (64*1024)
	#error LEN_SaveRam>64k
#endif
#if (ADDR_Terminal_SaveRam_End-ADDR_Meter_SaveRam_Start)>((64*1024)-8)
	#error ADDR_Terminal_SaveRam_End-ADDR_Meter_SaveRam_Start>64k
#endif



//-------------------------------------------------------------------------------------------------

#ifdef IC_STM32F4xx
//0x40024000 4K(BKPSRAM) NAND FLASHд����(�������ݼ�����ʱ��0)
#define ADDR_BKPSRAM     0x40024000
#endif
#ifdef IC_NUC9xx
#define ADDR_BKPSRAM     0x00F80000//256k
#endif
// 2BYTE �ܿ���
// 2BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
// ���ݿ�1
// 4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
// 2BYTE �����ֽ���
// 2BYTE �Ѳ���д����ֽ���
// nBYTE ����
// ...
// ���ݿ�n


//-------------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
//RTC ���ݼĴ���(20��) RTC_BASE+Address offset: 0x50
#define ADDR_RTCBKPSRAM_START     (RTC_BASE+0x50)
#endif
#ifdef IC_NUC9xx
//RTC ���ݼĴ���(16��)
#define ADDR_RTCBKPSRAM_START     RTC_BA

#endif
// RTC->BKP0R
// B0-B7 USB0����:0=��,1=�ȴ�����,2=����������ʱ,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
// B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
// B16-B23 
// B24-B31

// RTC->BKP1R
// 4BYTE �Ѹ��µ���ͬʱ�����ļ�:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router

// RTC->BKP2R
// �������籣����FLASH����Ϊ0xffffffff;��ʾ:1.�����ж�ʱ�������.2.��1mS�ж���ֹͣ��ʱ
// 1BYTE ��Դ����ʱά�ֵ�ص�ѹ(BCD x.x)
// 1BYTE ��Դ����ʱ���ݱ���ʱ��ms
// 2BYTE ��Դ����ά�ֹ���ʱ��ms

// RTC->BKP3R
// 4byte nand��������,b0-b30����,b31=1��ʾ����ֹͣ

// RTC->BKP4R
// 4byte LOG_1376_1 nand����дָ��

// RTC->BKP5R
// 4byte LOG_1376_2 nand����дָ��

// RTC->BKP6R
// 4byte LOG_1376_3 nand����дָ��

// RTC->BKP7R
// 4byte LOG_645_1997 nand����дָ��

// RTC->BKP8R
// 4byte LOG_645_2007 nand����дָ��

// RTC->BKP9R

// ...
// RTC->BKP19R
// 4byte ͣ�����¼��ж����ϵ��־;���Ź���λ�Զ�����=0x12345678,����ֵΪ�ϵ�
#define LEN_RTCBKPSRAM_START     80

#endif
























