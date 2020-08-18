


#ifndef ERAM_H
#define ERAM_H


#ifdef IC_STM32F4xx
#define LEN_ERAM_BUFF_UART_TIMER     (2*1024*1024)		// ���ݻ�������UART�Ͷ�ʱ����ϵͳ�����ܳ���
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
#define LEN_ERAM_BUFF_UART_TIMER     ((4*1024*1024)+(2*LEN_msFILEmanage_Ext))//���ݻ�������UART�Ͷ�ʱ����ϵͳ�����ܳ���

//0x00000000-0x003FFFFF 4M-512-16byte Program
#define ADDR_ETHMAC     (0x00400000-512-16)//��̫��MAC��ַ
//6byte MACADDR0
//6byte MACADDR1
//4byte 
#define ADDR_IrqHandlerTable     (0x00400000-512)//ע:����OS LINK
//256byte IRQ�жϱ�
#define ADDR_FiqHandlerTable     (0x00400000-256)//ע:����OS LINK
//256byte FIQ�жϱ�

//0x00400000-0x0043FFFF //256K MS Stack and Heap
//0x00440000-0x0047FFFF //256K ETH1
//0x00480000-0x00AFFFFF //7M-512K OS Heap
//0x00B00000-0x00BFFFFF //1M OS Stack

#define ADDR_SPI_MAP     0x00C00000
#define LEN_SPI_MAP     0x00080000
//0x00C00000-0x00C7FFFF 512K IEEPROM(SPIFLASH) ӳ��
#define ADDR_NAND_MAP     0x00C80000
#define LEN_NAND_MAP     0x00280000
//0x00C80000-0x00EFFFFF 2.5M NAND_FLASH ӳ��
//0x00F00000-0x00FFFFFF 1M ISRAM
#define ADDR_ERAM_START     0x01000000
#endif


#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
// LEN_SaveRam BYTE ��SRAM���Ƶĵ��籣������,�ϵ�ʱ����0�ʷ�����ǰ�Ա��ϵ�ʱ����������0��ַ����

#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#ifdef  NAND_1G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(16*1024)									// msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)		// �洢��FLASH�е��ļ��Ĺ�����Ԫ�ֽ���
#endif
#ifdef  NAND_2G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(16*1024)									// msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)		//�洢��FLASH�е��ļ��Ĺ�����Ԫ�ֽ��� (256-32)*1024/16 = 1433.6 ����ļ���Ԫ��1433
#endif
#ifdef  NAND_4G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(32*1024)  									// msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)		//�洢��FLASH�е��ļ��Ĺ�����Ԫ�ֽ���
#endif
#ifdef  NAND_8G
	#define LEN_msFILEmanage     	(256*1024)
	#define LEN_msFILEblock     	(32*1024)									// msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)	// �洢��FLASH�е��ļ��Ĺ�����Ԫ�ֽ���
#endif
#if LEN_msFILEunltFlags >= LEN_msFILEmanage
	#error
#endif
// 8192*2byte ���:0=δʹ��,1=ռ�õ����¿���,2-8191�¿�Ŀ��
// LEN_msFILEmanage-��־��-12�ֽ� �ļ���ڱ�
	// �ļ���ڱ�1
	// ...
	// �ļ���ڱ�n
// 12BYTE
	// 4byte �ļ����䵥Ԫ����
	// 4byte��Ч��ʶ0x12345678
	// 4byte CRC32


// 256K ����(��ǰ�������ֶ�ǿ�Ƹ�λʱ���ݻָ���)

typedef __packed struct
{
	u16 BLOCK_ENTRY;		//�ļ����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�(ֵ��Чʱ!=0)
	u16 BLOCK_END;			//�ļ�����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�
	u32 LEN_FILE;			//�ļ�Ԥ��ռ�д洢�ռ��ܳ���
	u32 LEN_DATA;			//�ļ���Ч�����ܳ���
	
	u32 FILEID;				//FILEID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	// �ļ�����:
	// 0=��ͨ�ɼ��������ݼ�¼
	// 1=
	// 2=��OI������¼���¼
	//
	//
	u32 FILECOUNT;			//��ͬFILEID���ļ�������
	u16 NUM_DATA;			//�ļ��������ݸ�������(ͬ�����ɹ�����)	//�洢�˶��ٸ���
	u16 NUM_FAIL;			//�ļ�ͷ��д�볭��ʧ�ܵ�ַ����
	u8 date_time_s[7];		//�ɼ��洢ʱ��
	u8 res1;				//��Ϊ����
	
	u16 NUM_GPRSreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
	u16 NUM_ENETreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
}msFILE_TypeDef;

#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)	// 128K(BKPSDRAM) NAND FLASHд����(�������ݼ�����ʱ��0)
// 2BYTE �ܿ���
// 4BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
// ���ݿ�1
// 4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
// 4BYTE �����ֽ���
// 4BYTE �Ѳ���д����ֽ���
// nBYTE ����
// ...
// ���ݿ�n
typedef __packed struct
{
	u32 ADDR_Dest;		// 4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
	u32 LEN_Data;		// 4BYTE �����ֽ���
	u32 LEN_Write;		// 4BYTE �Ѳ���д����ֽ���
	u8 Data[1];			// nBYTE ����
}BKPSDRAM_BLOCK_TypeDef;
typedef __packed struct
{
	u16 NUM_BLOCK;		// 2BYTE �ܿ���
	u32 LEN_TOTAL;		// 4BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
	//���ݿ�
}BKPSDRAM_TypeDef;
#define ADDR_SDRAM_CLR_START	(ADDR_BKPSDRAM+(128*1024))	// SDRAM������0��ʼ��ַ


#define ADDR_RAMmsFILEmanage    ADDR_SDRAM_CLR_START		// �洢��RAM�е��ļ�����
#define LEN_RAMmsFILEmanage     (4*1024)
#define LEN_RAMmsFILEblock     	(32*1024)					// RAMmsFILE�ļ��鳤��


#define ADDR_emWin     			ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
// 128K emWin������
#define ADDR_128KDATABUFF     	(ADDR_emWin+(128*1024))
// 128K DATABUFF
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
// 128K�ֽ�NAND FLASH��д����
#define ADDR_NAND_PAGEREAD_BUFF (ADDR_128KWRITE_BUFF+(128*1024))
// 2048�ֽ�NAND FLASHҳ������
#define ADDR_ECC_BUFF     		(ADDR_NAND_PAGEREAD_BUFF+2048)
// 128+128�ֽ�ECC����

#define ADDR_MACADDR_ascii     	(ADDR_ECC_BUFF+256)						//����MAC��ַ��ascii��
// 18BYTE ��ʽ"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     	(ADDR_MACADDR_ascii+18)					//����IP4��ַ��ascii��
// 16BYTE ��ʽ"192.168.0.100"
#define ADDR_IP4MASK_ascii     	(ADDR_IP4ADDR_ascii+16)					//����IP4 MASK��ַ��ascii��
// 16BYTE ��ʽ"255.255.255.0"
#define ADDR_IP4GATEWAY_ascii   (ADDR_IP4MASK_ascii+16)					//����IP4 GATEWAY��ַ��ascii��
// 16BYTE ��ʽ"192.168.0.254"
#define ADDR_IP4PRIMARY_DNS_ascii     	(ADDR_IP4GATEWAY_ascii+16)		//����IP4 PRIMARY_DNS��ascii��
// 16BYTE ��ʽ"8.8.8.8"
#define ADDR_IP4SECONDARY_DNS_ascii   	(ADDR_IP4PRIMARY_DNS_ascii+16)	//����IP4 SECONDARY_DNS��ascii��
// 16BYTE ��ʽ"8.8.4.4"
#define ADDR_IP6ADDR_ascii     			(ADDR_IP4SECONDARY_DNS_ascii+16)	//����IP6��ַ��ascii��
// 40BYTE
#define ADDR_IP6GATEWAY_ascii     		(ADDR_IP6ADDR_ascii+40)				//����IP6 GATEWAY��ַ��ascii��
// 40BYTE
#define ADDR_IP6PRIMARY_DNS_ascii     	(ADDR_IP6GATEWAY_ascii+40)			//����IP6 PRIMARY_DNS��ascii��
// 40BYTE
#define ADDR_IP6SECONDARY_DNS_ascii     (ADDR_IP6PRIMARY_DNS_ascii+40)		//����IP6 SECONDARY_DNS��ascii��
// 40BYTE


//#define ADDR_UARTnCtrl_Start     	(ADDR_IP6SECONDARY_DNS_ascii+40)//UART
//20190513 �˴�ʹ��������ݵ�ַ4�ֽڶ���
#define ADDR_UARTnCtrl_Start		((ADDR_IP6SECONDARY_DNS_ascii + 40 + 3) & 0xfffffffc)
//UART0
#define ADDR_UART0     				(ADDR_UARTnCtrl_Start)
#define ADDR_UART0IntRx     		ADDR_UART0
#define ADDR_UART0Ctrl     			(ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     			(ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     			(ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     	(ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     				(ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     		ADDR_UART1
#define ADDR_UART1IntRx     		(ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     			(ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     			(ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     				(ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     			(ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     	(ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#define ADDR_UART2     				(ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#define ADDR_UART2IntRx     		ADDR_UART2
#define ADDR_UART2Ctrl     			(ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     			(ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     			(ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     	(ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     				(ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     		ADDR_UART3
#define ADDR_UART3Ctrl     			(ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     			(ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     			(ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     	(ADDR_UART3Tx+LEN_UART3TxMax)
//UART4
#define ADDR_UART4     				(ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#define ADDR_UART4IntRx     		ADDR_UART4
#define ADDR_UART4Ctrl     			(ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     			(ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     			(ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     	(ADDR_UART4Tx+LEN_UART4TxMax)
//UART5
#define ADDR_UART5     				(ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)
#define ADDR_UART5IntRx     		ADDR_UART5
#define ADDR_UART5Ctrl     			(ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     			(ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     			(ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     	(ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     				(ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     		ADDR_UART6
#define ADDR_UART6Ctrl     			(ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     			(ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx    	 		(ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     	(ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(�ն�ESAM)
#define ADDR_UART7     				(ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     		ADDR_UART7
#define ADDR_UART7Ctrl     			(ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     			(ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     			(ADDR_UART7Rx+LEN_UART7Rx)
#define ADDR_UART7FnDataBuff     	(ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(���ESAM)
#define ADDR_UART8     				(ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     		ADDR_UART8
#define ADDR_UART8Ctrl     			(ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx    	 		(ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     			(ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     	(ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(���CARD)
#define ADDR_UART9     				(ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     		ADDR_UART9
#define ADDR_UART9Ctrl     			(ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     			(ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     			(ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff    	(ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(��̫��)
#define ADDR_UART10     			(ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     		ADDR_UART10
#define ADDR_UART10Ctrl     		(ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     			(ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     			(ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     	(ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     			(ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     		ADDR_UART11
#define ADDR_UART11Ctrl     		(ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     			(ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     			(ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     	(ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(�����)
#define ADDR_UART12     			(ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     		ADDR_UART12
#define ADDR_UART12Ctrl     		(ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     			(ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     			(ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     	(ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART13     			(ADDR_DATABUFF)
#define ADDR_UART13IntRx     		ADDR_UART13
#define ADDR_UART13Ctrl     		(ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     			(ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     			(ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     	(ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART14     			(ADDR_DATABUFF)
#define ADDR_UART14IntRx     		ADDR_UART14
#define ADDR_UART14Ctrl     		(ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     			(ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     			(ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     	(ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART15     			(ADDR_DATABUFF)
#define ADDR_UART15IntRx     		ADDR_UART15
#define ADDR_UART15Ctrl     		(ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     			(ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     			(ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     	(ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART16     			(ADDR_DATABUFF)
#define ADDR_UART16IntRx     		ADDR_UART16
#define ADDR_UART16Ctrl     		(ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     			(ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     			(ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     	(ADDR_UART16Tx+LEN_UART16TxMax)

#define ADDR_UARTnCtrl_End     		(ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)

typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
}RMAddrList_TypeDef;			//UART������ַ�б�
#define LEN_RMAddrList     		6
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u16 LEN_DATA;
}RecordFileHead_TypeDef;		//������¼�ļ�ͷ
#define LEN_RecordFileHead     	8
typedef __packed struct
{
	u8 OAD[4];
	u16 LEN_DATA;
	u8 REPORT;					//�ϱ���־:0=�����ϱ�,1=���ϱ���2=���ϱ�
	u8 res1;
}CongealFileHead_TypeDef;		//�����ļ�ͷ
#define LEN_CongealFileHead     8
typedef __packed struct
{
	u16 LEN_DATA;				//�ܳ���,�¼�����+OAD˳���(array OAD)
	u16 LEN_EVENT;				//�¼����ݳ���
	u8 REPORT;					//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
	u8 OCCUR;					//�����ָ�:0=�ָ�,1=����,2=��
	u8 ACK;						//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
	u8 res1;
}EventFileHead_TypeDef;	//�¼���¼�ļ�ͷ
#define LEN_EventFileHead     	8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 AddrType;						//ͷ�ļ��м�¼�ĵ�ַ��:0=�����ַ,1=�ɼ�����ַ
	u8 LEN_DATA;
}SearchMeterFileHead_TypeDef;	//�ѱ���¼�ļ�ͷ
#define LEN_SearchMeterFileHead     8


#define UART_TASK_RX				(0)
#define UART_TASK_RX_DONE			(1)
#define UART_TASK_RX_TIMEOUT		(2)

#define UART_TASK_TX				(3)
#define UART_TASK_TX_TIMEOUT		(5)
#define UART_TASK_TXING				(6)
#define UART_TASK_TX_DONE			(7)

#define UART_LOCK					(0x55)
#define UART_UNLOCK					(0)

typedef __packed struct
{
//ͨ�ò���
	u8 Task;		// ���ڵ�ǰ�����ʶ
					// 0=���ڽ���,1=�������,2=�ȴ���֡���ݷ���,3=��֡�������ڷ���
					// 4=��֡�������������,5=�ȴ���֡���ݷ���,6=��֡�������ڷ���
					// 7=�ȴ���������תΪ����
	u8 Lock;		// 0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	u16 RxByte;		//�����ֽ���(���ֶ���)
	u16 TxByte;		//�����ֽ���(���ֶ���)
	u16 TxCount;	//�ѷ��ͼ���(���ֶ���)
	u8 Safety;		//��ȫ��֤(������)
	//u8 res1;		//
	u8 BpsCtrlBack;	//̨�������ʱ����  0:115200  1:38400  2:9600  GPRS������
	u16 BpsCtrl;	//ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
					//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
					//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
					//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
					//D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
					//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
	u8 Protocol;	//ͨ�Ź�Լ�Զ�ʶ��:0=698.45,1=DL/T645-2007
	u8 NewBps;		//�����ʸ��±�־;��ΪMESAMͨ��ʱΪMESAM�ڲ����ݸ��±�־0=����,����ֵû����
	u8 SubTask;		//������:���ĸ�������ȥ����
	u8 AddrType;	//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
					//��16Byte
	//�ն�ר�ò���
	u32 RxTxByte;			//ͨ������
	u16 PORT;				//�����˿ں�(���ֶ���),ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
	u8 TCPUDP;				//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
	u8 LinkCtrl;			//���ӿ���0=������,1=����
	u8 LinkCount;			//���Ӽ��������ڱ�ʾ��ǰ���ӵ����ĸ���վ����(0=��վ������1=����1��վ��2=����2��վ)
	//u8 LinkCount_SelfAdaption; //err20181210 GPRS��������Ӧ ����˴�ϵͳ�������⣡�����޸� ���Ӽ��������ڱ�ʾ����Ӧ���̵�ǰ���ӵ����ĸ���վ����(0=��ͨ����1=������)
	//u8 SelfAdaptionChangeFlag;//err20181210 GPRS��������Ӧ ����˴�ϵͳ�������⣡�����޸� ����Ӧ�����л���ʶ����������ȥ���� 
	u8 ReLinkNum;			//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
	u8 LinkTask;			//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	u8 LinkSubTask;			//����������
	u8 HeartberatCount;		//������������,�յ�ȷ��֡��0
	u8 HostACK;				//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
	u8 HostCount;			//�����ϱ��ط�����(����������������)
	u8 TimerTaskCount;		//�����ϱ���ʱ�������
	
	u8 ATTask;				//AT����:0=����,1=����,2=���ڷ���,3=����
	u8 ATWriteTask;			//AT���ݷ�������:0=������,1=������,2=�ȴ�������ʱ
	u8 ATCount;				//AT����
	u8 State;
	u8 LinkTask2;			//ͨ��2��������(FTP����)
	u8 FTPLinkCount;		//FTP�������Ӽ���
	u8 Task2;				//ͨ��2����(FTP����)
	u8 FTPErr;				//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
	u32 FTPByte;			//FTP��ǰ�ļ�����������ֽ���
	u32 FTPFileLen;			//FTP��ǰ�ļ��ܳ���

	//���г���ר�ò���
	u8 LINKCHECK_RTCS;		//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
	u8 RTCS;				//ʵʱ�Ӹ��±�־ԭʼ��
	u8 RTCM;				//ʵʱ�Ӹ��±�־ԭʼ��
	u8 RTCH;				//ʵʱ�Ӹ��±�־ԭʼʱ
	u8 RTCD[2];				//ʵʱ���ո��±�־��ԭʼ����
	u8 RTCW[2];				//ʵʱ�����ڸ��±�־��ԭʼ����
	//u16 res7;				//��ָ���������EDMI����
	u8 res7;
	u8 Jump_Flag;			//���ϸ���EDMI�������ݽ����ã�0��ʾ����ת�� 1��ʾ0x10��0x40������
	//u8 LinkTask_Temp;			//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	//u8 LinkSubTask_Temp;			//����������
	u8 LINKCHECK_RTCD;		//���Ӽ�飬ÿ��һ�Σ���Ҫ��������ʧ��ʱ�������Ƿ������������ֵ�뵱ǰ����ͬʱ������ʧ�ܺ��첻������
	u8 LastAutoSearchRTCD;	//�ϴ��Զ��ѱ�ʵʱ����
	u8 AutoSearchTask;		//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
	u8 AutoSearchReCount;	//�Զ��ѱ������ط�����
	u16 AutoSearchTime;		//�Զ��ѱ�����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ�����
	u16 TransferDelay;		//��·�ϴ�����ʱʱ����
	u8 NETOPENCount;		//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
	u8 ReTxSEQ;				//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
	u16 SMSRxSEQ;			//���Ž��սӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	u16 SMSTxSEQ;			//���ŷ��ͽӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	//��24Byte
	u8 Function;			//�ڹ��� 0=�ն�ά����,!=0�ն˳�����
	u8 FunctionTimer;		//�ϴοڹ��ܼ���ʵʱ����
	u8 PowerUpDealy;		//�ϵ���ʱ:0=û��ʱ,!=0�ѵ���ʱ
	u8 DHCPTask;			//DHCP����
	s32 TCPSocket;
	s32 UDPSocket;
	u8 NET_ADDR[20];		//NET_ADDR

	u32 TaskID;				//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	u8 FrameType;
	u8 ReceivedHostAck;		//�յ���վȷ��֡
	u16 Round;
}UARTCtrl_TypeDef;

#define UART0Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART0Ctrl))
#define UART1Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART1Ctrl))
#define UART2Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART2Ctrl))
#define UART3Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART3Ctrl))
#define UART4Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART4Ctrl))
#define UART5Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART5Ctrl))
#define UART6Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART6Ctrl))
#define UART7Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART7Ctrl))
#define UART8Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART8Ctrl))
#define UART9Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART9Ctrl))
#define UART10Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART10Ctrl))
#define UART11Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART11Ctrl))
#define UART12Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART12Ctrl))
#define UART13Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART13Ctrl))
#define UART14Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART14Ctrl))
#define UART15Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART15Ctrl))
#define UART16Ctrl     ((UARTCtrl_TypeDef *)(ADDR_UART16Ctrl))


typedef __packed struct
{
	u8 RouterAddr[6];		// ·�����ڵ��ַ(���Ƿ�����ǰ,ͨ�ſ��ַ��λ��)
	u8 RouterDate[3];		// ͨ��ģ��ӿ�Э�鷢������
	u8 RouterMFCode[2];		// ���̴��� ASCII 2
	u8 RouterICCode[2];		// оƬ���� ASCII 2 
	u8 RouterVerDate[3];	// ·�����汾��,�ֱ�Ϊ:��,��,��
	u8 RouterVer[2];		// ·�����汾��,2BYTE�汾

	u8 RouterCtrl;			// ·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
	u8 RouterCtrlTask;		// ·���������������;��ɺ��Զ��ص�0
	u8 RetryCnt;			// ·���������ط�����
	u8 RUNState;			// ��ģ���ȡ�ĵ�ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
	u8 UpDateTask;			// ������������
	u8 UpDateReCount;		// �����������Դ���
	
	u8 RouterInitTask;		// ·������ʼ������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	u8 RouterInitSubTask;	// ·������ʼ��������
	u8 RouterInitReCount;	// ·������ʼ�������ط�����
	u8 RouterResetCnt;		// ·������λ��������
	u8 MaxOverTime;			// �ӽڵ������ʱʱ����
	
	u8 RouterSynTask;		// ����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
	u8 RouterRealTimeTask;	// �ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
	u8 RouterKeyUserTask;	// �ز��ص㻧��������

	u16 AddNodeNum;			// ����ͬ��ʱ�����ӵĴӽڵ�����
	u16 TotalNodeNum;		// ��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	u16 CurReadNodeNum;		// ��ѯ�ӽڵ���Ϣʱ����ǰ֡���صĴӽڵ�����
	u16 CurAddNodeNum;		// ���Ӵӽڵ�ʱ����ǰ֡���ӵĴӽڵ�����
	u16 TempCount;			// ɾ�������ӽڵ����ʱ����(���ֶ���)
	u16 UpDateCount;		// ��������֡����(���ֶ���)
	u16 ReadNodeNum;		// ����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
	u16 RS485Time;			// �ɼ�����485�����ʱֵ��(BCD)(���ֶ���)
	u16 KeepTime;			// ע�����ʱ�䶨ʱֵ��(���ֶ���)
	u8 n;					// �ִ�1-3=�����ѱ�,0=ÿ���ѱ�
	u8 RMTD;				// 0=�ǳ���ʱ��,1=����ʱ��
	u8 NodeNumOver;			// AFN04F10���õĴӽڵ���>ģ��֧�ֵ����ӽڵ���;0=��,1=��;����������ģ������ʾ��������ʱ��ʾ������Ϣ
	u8 Old_RouterInitTask;	// res5; //̨�� ����ԭ������״̬
	u16 UnfinishTaskNum;	//res6;	//̨�� δ���������
	u32 StopFlags;			//��ͣ·�ɱ�־:(�ֶ���)
		//b0=�������1,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
		//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
		//b16=·����������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	u8 RouterRunMode_1;		//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"�����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	u8 RouterRunMode_2;		//·������ģʽ_·�ɹ�����ʽ��0��ʾ����ͨ��ģ����·�ɹ������ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�������
	u8 RouterRunMode_3;		//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
	u8 RouterRunMode_4;		//·������ģʽ_���ڳ���ģʽ�� 01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
	u8 RouterRunMode_5;		//·������ģʽ_������ʱ����֧�֣���D2��D1��D0������λ��ʾ���ֱ�����㲥���ӽڵ��ء�·��������������������֧���������ṩ������ʱ�����������1��ʾ֧���ṩ������ʱ������0��ʾ��֧��
	u8 RouterRunMode_6;		//·������ģʽ_ʧ�ܽڵ��л�����ʽ��D3��1��ʾͨ��ģ�������л������ڵ㣬D4��1��ʾ����������֪ͨͨ��ģ���л������ڵ�
	u8 RouterRunMode_7;		//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
	u8 RouterRunMode_8;		//·������ģʽ_�㲥�����ŵ�ִ�з�ʽ��0��ʾִ�й㲥�����Ҫ�ŵ���ʶ��1��ʾ�㲥����Ҫ���ݱ����е��ŵ���ʶ��������ͣ�����ȡֵ����
	u16 MaxBCastOverTime;	//���㲥��ʱʱ����(���ֶ���)
	u16 MaxTxByte;			//���֧�ֱ��ĳ���(���ֶ���)
	u16 MaxFileByte;		//�ļ�����֧�ֵ���󵥸����ݰ�����(���ֶ���)
	u16 MaxUpDateTime;		//�����������ȴ�ʱ����(���ֶ���)
	u16 MaxNodeNum;			//֧�ֵ����ӽڵ���(���ֶ���)
	u16 MaxRWNodeNum;		//���ζ�д�ڵ���������
	u8 RadioChannel;		//�����ŵ���:0��63��64��253��������254���Զ�ѡ��255�����ֲ���
	u8 RadioPower;			//�������ڵ㷢�书��:00����߷��书�ʣ�01���θ߷��书�ʣ�02���εͷ��书�ʣ�03����ͷ��书�ʣ�04��254��������255�����ֲ���

	u16 ResDistribTaskNum;  //̨�� ʣ��ɷ���������
	u8 res7[48];
}Terminal_Router_TypeDef;
#define LEN_Router     128								//·�������������ݳ���
#define ADDR_Router     	ADDR_UARTnCtrl_End			//·����(�������ز�)
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

//#define ADDR_NoteList     (ADDR_Router+LEN_Router)	//·�����нڵ��
typedef __packed struct
{
	u8 Addr[6];				//�ӽڵ��ַ(�ֶ���)
	u8 CollectorAddr[6];	//�ɼ�����ַ
//	u16 MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
//	u16 Pn;//��Ӧ�������
//	u8 FnCount;//����Fn����
//	u8 RMCount;//�������ݱ�ʶ����
//	u8 ReRMCount;//�ط�����
//	u8 RMCountErr;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
//	u8 RMErr;//��ǰ�����ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
//	u8 RMErr_0x901F;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010�����ı��;0=��,1=��0x901f,2=0x901F�������ر�ʶ����
//	u8 FnCountSOfMeterEvent;//�������ڳ������ܱ��¼�ǰԭ������Fn����
//	u8 FnCountBackupOfAutoReport;//���������ϱ�ǰԭ������Fn����
//	u8 FnCountSOfKeyUser;//����ÿСʱ������������ǰԭѭ������ĳ���Fn����
//	u8 FnCountS;//��������ǰԭѭ������ĳ���Fn����
//	u16 res1;
}NoteList_TypeDef;
#define LEN_NoteList     12				//ÿ�б��ֽ���
typedef __packed struct
{
  NoteList_TypeDef     List[NUM_RMmax];
}Terminal_NoteList_TypeDef;
//#define Terminal_Note     ((Terminal_NoteList_TypeDef *)(ADDR_NoteList))//�ز�·�����нڵ��(����·�ɺ�����˳������)


#define ADDR_Terminal_ERAM_Start     ((ADDR_Router + LEN_Router) & 0xfffffffc)		//ERAM����ֶ���

//�ն�ϵͳ����
typedef __packed struct
{
	u8 res0;
	u8 I2SUBTask1;//�ն���1���ݼ�������
	u8 I2SUBTask2;//�ն���2���ݼ�������
	u8 I2SUBTask3;//�ն���3���ݼ�������
	u8 I2SUBTask4;//�ն�ͳ�����ݼ�������
	u8 I2SUBTask5;//�ն˿�������
	u8 ERC22Task;//ERC22�й��ܵ������Խ�޼���
	u8 Cascading_LoopChack_Count;//��������Ѳ���ն˼���
	u8 CurveCongealTask;//���߶�������(��ʱ,��1-24ʱ)
	u8 DateCongealCheckCount;//�ն����������
	u8 MonthCongealCheckCount;//�¶����������
	u8 CurveCongealCheckCount;//���߶��������
	u8 ExtTask1;//�ն���1���ݼ�����չ����
	u8 res1;
	u16 I2SUBTask4_1;//�ն�ͳ�����ݼ�������.�����ܼ���������й����ʼ�����ʱ��,�й�����Ϊ��ʱ��(���ֶ���)
	u16 I2SUBTask4_2;//�ն�ͳ�����ݼ�������.�����ܼ��������С�й����ʼ��䷢��ʱ��,�й�����Ϊ���ۼ�ʱ��(���ֶ���)
	u16 I2SUBTask4_3;//�ն�ͳ�����ݼ�������.��ѹͳ�����ݷֲ�����(���ֶ���)
	u16 I2SUBTask4_4;//�ն�ͳ�����ݼ�������.����ͳ������(���ֶ���)
	u16 I2SUBTask4_5;//�ն�ͳ�����ݼ�������.��ѹ������ƽ��ֲ�����(���ֶ���)
	u16 I2SUBTask4_6;//�ն�ͳ�����ݼ�������.���ڹ���Խ���ۼ�ʱ��(���ֶ���)
	u16 I2SUBTask4_7;//�ն�ͳ�����ݼ�������.������
	u16 I2SUBTask4_8;//�ն�ͳ�����ݼ�������.�ܼ��鳬���ʶ�ֵ�µ�������ֵ�����ۼ�ʱ���ۼƵ�����
	u16 I2SUBTask4_9;//�ն�ͳ�����ݼ�������.�������������ۼ�ʱ��
	u16 I2SUBTask4_10;//�ն�ͳ�����ݼ�������.г��Խ��
	u16 I2SUBTask4_11;//�ն�ͳ�����ݼ�������.ֱ��ģ����Խ��
	u16 I2SUBTask4_12;//�ն�ͳ�����ݼ�������.�������ѹ��������ƽ���95%����ֵ
	u16 I2SUBTask4_13;//�ն�ͳ�����ݼ�������.�������ѹ��������ƽ���95%����ֵ
	u16 I2SUBTask4_14;//�ն�ͳ�����ݼ�������.�ն����ѹг����ѹ�����ʡ�г������95%����ֵ
	u16 I2SUBTask4_15;//ERC10_Event();//��ѹ��·�쳣(���ࡢʧѹ)
	u16 I2SUBTask4_16;//
	u16 res2;
#define ADDR_Terminal_MS_Timer     (ADDR_Terminal_ERAM_Start+48)//ms��ʱ��
	u16 RequestFileTimer;			// 1 �����ļ�����MS��ʱ��,ÿ10MS��1��ʱ��
  	u16 BUZZMSTimer;				// 2 ������ÿ10MS��1��ʱ��
	u16 Trip1_MS_Timer;				// 3 �ն���բ1 10MS��ʱ��;1200ms,>=1000��,<1000��
	u16 Trip2_MS_Timer;				// 4 �ն���բ2 10MS��ʱ��;1200ms,>=1000��,<1000��
	u16 Trip3_MS_Timer;				// 5 �ն���բ3 10MS��ʱ��;1200ms,>=1000��,<1000��;΢��������ѧϰ��ɶ�·��״̬��ʱ��
	u16 Trip4_MS_Timer;				// 6 �ն���բ4 10MS��ʱ��;1200ms,>=1000��,<1000��;������ʱ��ʱ�ȴ��������Уʱ��¼��ʱ��
	u16 AT_MS_Timer;				// 7 GPRS AT������10MS��ʱ��
	u16 GPRS_RxLED_MS_Timer;		// 8 GPRS ����LED 10MS��ʱ��
	u16 GPRS_TxLED_MS_Timer;		// 9 GPRS ����LED 10MS��ʱ��
	u16 RS485_1_TxLED_MS_Timer;		// 10 RS485_1��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_1_RxLED_MS_Timer;		// 11 RS485_1��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_2_TxLED_MS_Timer;		// 12 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_2_RxLED_MS_Timer;		// 13 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_3_TxLED_MS_Timer;		// 14 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_3_RxLED_MS_Timer;		// 15 RS485_2��������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 ENETLED_MS_Timer;			// 16 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 GPRS_ONOFF_MS_Timer;		// 17 GPRS ONOFF�ſ��� 10MS��ʱ��
	u16 RouterCtrl_MS_Timer;		// 18 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
	//u16 CD485TxLED_MS_Timer;		// ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	//u16 CD485RxLED_MS_Timer;		// ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	//u16 DisplayScanTimer;			// ��ʾɨ�� ÿ10MS��1��ʱ��
	u16 GPRS_ONLINELED_MS_Timer;//19 GPRS����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 GPRS_WEAK_SINGLE_LED_MS_Timer;//20 GPRS�ź�����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 GPRS_STRONG_SINGLE_LED_MS_Timer;//21 GPRS�ź���ǿLEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
#define Terminal_MS_Timer_Num     21//�ն�10MS��ʱ����
#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s��ʱ��
	u16 S_PR;						// 1 60���Ƶ������
	u16 PORT0_S_Timer;				// 2 �붨ʱ��
	u16 PORT1_S_Timer;				// 3 �붨ʱ��
	u16 PORT2_S_Timer;				// 4 �붨ʱ��
	u16 PORT3_S_Timer;				// 5 �붨ʱ��
	u16 PORT4_S_Timer;				// 6 �붨ʱ��
	u16 PORT5_S_Timer;				// 7 �붨ʱ��
	u16 PORT6_S_Timer;				// 8 �붨ʱ��
	u16 PORT7_S_Timer;				// 9 �붨ʱ��
	u16 PORT8_S_Timer;				// 10 �붨ʱ��
	u16 PORT9_S_Timer;				// 11 �붨ʱ��
	u16 PORT10_S_Timer;				// 12 �붨ʱ��
	u16 PORT11_S_Timer;				// 13 �붨ʱ��
	u16 PORT12_S_Timer;				// 14 �붨ʱ��
	u16 PORT13_S_Timer;				// 15 �붨ʱ��
	u16 PORT14_S_Timer;				// 16 �붨ʱ��
	u16 PORT15_S_Timer;				// 17 �붨ʱ��
	u16 PORT16_S_Timer;				// 18 �붨ʱ��

    u16 ENETHeartBeat_S_Timer;		// 19 ��̫�������붨ʱ��
	u16 IPLeaseTimer;				// 20 ��̫���Զ����IP����ʱ���붨ʱ��
	u16 ENETCheck_S_Timer;			// 21 ��̫��IP��ͻ�������޸����붨ʱ��
    u16 GPRSHeartBeat_S_Timer;		// 22 GPRS�����붨ʱ��
	u16 GPRSAutoDiscon_S_Timer;		// 23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
	u16 GPRSReConnection_S_Timer;	// 24 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
	u16 GPRSWaitConnection_S_Timer;	// 25 GPRS�������ӷ���ȴ��붨ʱ��
	u16 GPRSWaitTx_S_Timer;			// 26 GPRS�������ݵȴ���ʱ�붨ʱ��
	u16 GPRSCheck_S_Timer;			// 27 GPRS���ߺ��źż���붨ʱ��
	u16 ResetCommand_S_Timer;		// 28 �ն��յ���λ������ʱִ���붨ʱ��
	u16 GPRS_WaitHostACK_S_Timer;	// 29 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
	u16 ENET_WaitHostACK_S_Timer;	// 30 ENET�����ϱ��ȴ���վȷ���붨ʱ��
	u16 RS232_WaitHostACK_S_Timer;	// 31 RS232�����ϱ��ȴ���վȷ���붨ʱ��
	
	u16 Cascading_LoopChack_S_Timer;// 32 ����Ѳ�������붨ʱ��
	u16 SlipCYC_S_Timer;			// 33 �ն��������幦�ʼ�����������廬���붨ʱ��
	u16 DemandCYC_S_Timer;			// 34 �ն������������������붨ʱ��
	u16 RouterCheck_S_Timer;		// 35 �ز�·��������붨ʱ��
	u16 RouterAck_S_Timer;			// 36 �ز�·�ɵȴ� ACK Ӧ���붨ʱ��
	u16 AutoUpdataAFN04F10_S_Timer;	// 37 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
	u16 AutoUpdataAFN04F10ERC3_S_Timer;	// 38 �Զ�����F10�����ϱ�ERC3�¼��붨ʱ��;������ͨ�����ص��Զ����֣���ɲ����������������ɾ����Ӧ�����ò�������������־��F150��������ӦλΪ1���������ϱ���������¼���ERC3������վ,���ǽ����˲�������������־�ĸ�λ������ÿ���ϱ���������¼����m���ӣ�m=0~30��һ��ȡ10��֮�ڣ����۲����Ƿ�����������ٴ��ϱ�
	u16 AutoDelAFN04F10_S_Timer;		// 39 �Զ�ɾ��AFN04F10�ĵ��ܱ������붨ʱ��,���⾭������Ӱ��CPU�ٶ�
	u16 AutoSearch_S_Timer;				// 40 �ز��Զ��ѱ��붨ʱ��
	u16 RouterSynDelay_S_Timer;			// 41 ·�ɵ���ͬ����ʱִ���붨ʱ��
	u16 Power_S_Timer;					// 42 ͣ���¼��붨ʱ��
	u16 MeterSyncToRam_S_Timer;			// 43 �����㵵��ͬ���� SDRAM �붨ʱ��
	u16 RS485_1_Ack_S_Timer;			// 44 RS485_1�ȴ� ACK Ӧ���붨ʱ��
	u16 RS485_2_Ack_S_Timer;			// 45 RS485_2�ȴ� ACK Ӧ���붨ʱ��
	u16 RS485_3_Ack_S_Timer;			// 46 RS485_3�ȴ� ACK Ӧ���붨ʱ��
	u16 IR_WaitHostAck_S_Timer;			// 47 IR ����ȴ� ACK Ӧ���붨ʱ��
//  u16 DisplayLoopTimer;				// ѭ���붨ʱ��
//  u16 BackLightTimer;					// �������ʾ�붨ʱ��

#define BASE_S_Timer_Num     47			// �붨ʱ����

#if ((Project/100) == 3)				// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u16 ReTrip_S_Timer;				// 1 �ն˲���բ�붨ʱ��
	u16 EnSwitchON_S_Timer;			// 2 �ն˺�բ����֪ͨ�붨ʱ��
	u16 Remote1_S_Timer;			// 3 ң����բ�ִ�1������ʱ�붨ʱ��
	u16 Remote2_S_Timer;			// 4 ң����բ�ִ�2������ʱ�붨ʱ��
	u16 Remote3_S_Timer;			// 5 ң����բ�ִ�3������ʱ�붨ʱ��
	u16 Remote4_S_Timer;			// 6 ң����բ�ִ�4������ʱ�붨ʱ��
	u16 Remote5_S_Timer;			// 7 ң����բ�ִ�5������ʱ�붨ʱ��
	u16 Remote6_S_Timer;			// 8 ң����բ�ִ�6������ʱ�붨ʱ��
	u16 Remote7_S_Timer;			// 9 ң����բ�ִ�7������ʱ�붨ʱ��
	u16 Remote8_S_Timer;			// 10 ң����բ�ִ�8������ʱ�붨ʱ��
	u16 PowerCtrl1_S_Timer;			// 11 �ն��ܼ���1���ع��ʼ����붨ʱ��
	u16 PowerCtrl2_S_Timer;			// 12 �ն��ܼ���2���ع��ʼ����붨ʱ��
	u16 PowerCtrl3_S_Timer;			// 13 �ն��ܼ���3���ع��ʼ����붨ʱ��
	u16 PowerCtrl4_S_Timer;			// 14 �ն��ܼ���4���ع��ʼ����붨ʱ��
	u16 PowerCtrl5_S_Timer;			// 15 �ն��ܼ���5���ع��ʼ����붨ʱ��
	u16 PowerCtrl6_S_Timer;			// 16 �ն��ܼ���6���ع��ʼ����붨ʱ��
	u16 PowerCtrl7_S_Timer;			// 17 �ն��ܼ���7���ع��ʼ����붨ʱ��
	u16 PowerCtrl8_S_Timer;			// 18 �ն��ܼ���8���ع��ʼ����붨ʱ��
	u16 PowerAttr1_S_Timer;			// 19 �ն��ܼ���1���ر����붨ʱ��
	u16 PowerAttr2_S_Timer;			// 20 �ն��ܼ���2���ر����붨ʱ��
	u16 PowerAttr3_S_Timer;			// 21 �ն��ܼ���3���ر����붨ʱ��
	u16 PowerAttr4_S_Timer;			// 22 �ն��ܼ���4���ر����붨ʱ��
	u16 PowerAttr5_S_Timer;			// 23 �ն��ܼ���5���ر����붨ʱ��
	u16 PowerAttr6_S_Timer;			// 24 �ն��ܼ���6���ر����붨ʱ��
	u16 PowerAttr7_S_Timer;			// 25 �ն��ܼ���7���ر����붨ʱ��
	u16 PowerAttr8_S_Timer;			// 26 �ն��ܼ���8���ر����붨ʱ��
	u16 PowerCtrl1_Congeal_S_Timer;	// 27 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl2_Congeal_S_Timer;	// 28 �ն��ܼ���2��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl3_Congeal_S_Timer;	// 29 �ն��ܼ���3��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl4_Congeal_S_Timer;	// 30 �ն��ܼ���4��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl5_Congeal_S_Timer;	// 31 �ն��ܼ���5��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl6_Congeal_S_Timer;	// 32 �ն��ܼ���6��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl7_Congeal_S_Timer;	// 33 �ն��ܼ���7��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl8_Congeal_S_Timer;	// 34 �ն��ܼ���8��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 UpPower_Protect_S_Timer;	// 35 �ն��ϵ籣���붨ʱ��
	u16 ContrlLoopCheck_S_Timer;	// 36 ���������·�����Ч�ȴ��붨ʱ��
	#define Contrl_S_Timer_Num     36
#else
	#define Contrl_S_Timer_Num     0
#endif	//#if ((Project/100)==3)	// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//�ն��붨ʱ����
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m��ʱ��
	u16 DisconAutoProtect_M_Timer;		// 1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
	u16 AutoSearchMeterTimer;			// 2 �Զ��ѱ�����ʱ��ֶ�ʱ��(��ʼ�Ǻ���Ҫ��)
	u16 RS4851RMF246_M_Timer;			// 25 RS485-1ͣ���¼���������ֶ�ʱ��
	u16 RS4852RMF246_M_Timer;			// 26 RS485-2ͣ���¼���������ֶ�ʱ��
	u16 RS4853RMF246_M_Timer;			// 27 RS485-3ͣ���¼���������ֶ�ʱ��
	u16 RS4854RMF246_M_Timer;			// 28 RS485-4ͣ���¼���������ֶ�ʱ��(�ز�)

#define Terminal_M_Timer_Num     6		// �ն˷ֶ�ʱ����
#define ADDR_SwitchIn     ADDR_Terminal_M_Timer+(2*Terminal_M_Timer_Num)//����������
	u8 SwitchIn1;						// ����������1ԭ״̬
	u8 SwitchInFilter1;					// ����������1�˲�����
	u8 SwitchIn2;						// ����������2ԭ״̬
	u8 SwitchInFilter2;					// ����������2�˲�����
	u8 SwitchIn3;						// ����������3ԭ״̬
	u8 SwitchInFilter3;					// ����������3�˲�����
	u8 SwitchIn4;						// ����������4ԭ״̬
	u8 SwitchInFilter4;					// ����������4�˲�����
	u8 SwitchIn5;						// ����������5ԭ״̬
	u8 SwitchInFilter5;					// ����������5�˲�����
	u8 SwitchIn6;						// ����������6ԭ״̬
	u8 SwitchInFilter6;					// ����������6�˲�����
	u8 SwitchIn7;						// ����������7ԭ״̬
	u8 SwitchInFilter7;					// ����������7�˲�����
	u8 SwitchIn8;						// ����������8ԭ״̬
	u8 SwitchInFilter8;					// ����������8�˲�����
#define ADDR_Other      ADDR_SwitchIn+(2*8)
	u8 CongealFlags;			// ��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
 	u8 LastCongealTime[5];		// �ϴζ���ʱ��ʵʱ��������ʱ�ּĴ�
	u8 Terminal_S_Count;		// �ն���Ӽ���,���������붨ʱ��
	u8 Terminal_M_Count;		// �ն˷ּӼ���,���������ֶ�ʱ��
	u8 SET6000;					// �ɼ��������ñ������ñ�־:0=û����,0x55=����
	u8 SETTask;					// �������ñ��ɼ����������ñ�־:0=û����,0xff=����
	u8 FLASHtoSDRAM;			// ����FLASHtoSDRAM�ϴν��е�ʵʱ��ʱ(������)
	u8 res4;
	u8 res5;
	u8 RoundTripFlags;			// �ն��ִ���բλ��־,B0-B7=0ûѭ��,=1��ѭ��
	u8 MODULSEL;				// ����ģ��ѡ��0=,1=��ΪCDMA MC323,2=GPRS��׼3763,3=ģ���û����
	u8 ENETSEL;					// ��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
	u8 Signal;					// GPRS�ź�ǿ��0-4
	u8 SignaldBm;				// �ź�ǿ��0-31��99(BCD��)
	u8 PowerDownTask;			// ��Դ�����������
	u8 PowerDownSubTask;		// ��Դ�������������
	u16 CopyTxByte;				// �Ĵ�GPRS�跢���ֽ���(���ֶ���)
	u16 CopyTxCount;			// �Ĵ�GPRS�ѷ��ͼ���(���ֶ���)
	u16 ReqTxByte;				// �Ĵ�GPRS�������ֽ���(���ֶ���)
	u16 RT_AMRTimeOut;			// �ն�ʵʱ������ʱʱ�����
	
	
	u32 Ticks;
	u32 Seconds;
	u32 LCD_RT_AMR_REQ_TEST_START_TIME;//res10; //err20180730 ̨�� ΢��������ģ��ʵʱ�ٲ������ʱ�л������ʣ��·�һ��GUIʵʱ�ٲ�֡��Ӧ�����ʡ�
	u8 LinkCount_SelfAdaption; //err20181210 GPRS��������Ӧ ���Ӽ��������ڱ�ʾ����Ӧ���̵�ǰ���ӵ����ĸ���վ����(0=��ͨ����1=������)(2=��վIP1��3=����IP1��4=����IP2)err20190528 ���ϸ�������Ӧ
	u8 SelfAdaptionChangeFlag;	//err20181210 GPRS��������Ӧ ����Ӧ�����л���ʶ����������ȥ����
	u8 GPRS_Para_Reset_Flag;		//err20190313 GPRS����ʧ�ܺ���������ͨ�Ų��� 1:�������ò���
	u8 GPRS_Para_Reset_Count;		//err20190313 GPRS�������ô����������ﵽһ����������Ӳ����λ
	//u8 res11;
	//u32 res11;
	u16 AT_AMSTimeOut;          // RS485�Զ��ѱ���ʱʱ�����
	u16 res16;
	//u32 res12;
	
	u32 res13;
	u32 res14;
	u32 res15;
	u32 rse16;
}Terminal_Ram_TypeDef;
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


#define ADDR_BUFF_UART_TIMER_END     (((ADDR_Other + 64) + 3) & 0xfffffffc)
#if (ADDR_BUFF_UART_TIMER_END - ADDR_ERAM_START) > LEN_ERAM_BUFF_UART_TIMER
#error
#endif

#define ADDR_QCSG_ERAM_START     	ADDR_BUFF_UART_TIMER_END
//#define ADDR_QCSG_ERAM_START     	(ADDR_ERAM_START + LEN_ERAM_BUFF_UART_TIMER)
#define LEN_Terminal_ERAM			(2*1024*1024)
#define ADDR_MobilePhoneNumber     	ADDR_QCSG_ERAM_START					//ͨ�ſ����ֻ������IMSI����(������ʾ)
// 20byte �ֻ�����(ASCII)
// 20byte IMSI����(ASCII)
// 16�ֽ�����ģ�鱾��IP��ַ,ASCII
// 6�ֽ�����ģ�鱾�ض˿ں�,ASCII
// 6�ֽ�����ģ���źŷֱ�ֵ,ASCII,��λdB

//�ն˿���
#define ADDR_Terminal_CTRL_ESRAM_START     ADDR_MobilePhoneNumber+68	//�ն˿���ESRAM��ʼ��ַ

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START			//�ն˷���ʱ�κ͹���ʱ��
// 1BYTE �ն˵�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ��,��,ƽ,�ȵȷ���),0=��Ч
// 1BYTE 
// 1BYTE ��ǰ����ʱ�κ�1-8,=0��ʾʱ�κ�>9����
// 1BYTE 0=��ǰ����ʱ�κŲ�����,1��2��ǰ����ʱ�κſ���

#define ADDR_Terminal_Protect     ADDR_Terminal_Period+4				// �ն˱���,0=������,1=�����,2=�Զ�����
// 1BYTE 0=������,1=�����,2=�Զ�����
#define ADDR_Terminal_Attr_State     ADDR_Terminal_Protect+1			// �߷Ѹ澯����״̬
// 1BYTE 0=û���,1=���
#define ADDR_Terminal_Total_State     ADDR_Terminal_Attr_State+1		// �ն�����բ
// 1BYTE λ��־(�ִ�1-8)��ǰ����բ״̬(1��բ,0������բ)
// 1BYTE λ��־(�ִ�1-8)������բ֪ͨ(����բ״̬��ӦλתΪ��բʱ��λ,֪ͨ��ʱ������0)
// 2BYTE ����
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//����ģ�������������
// 1BYTE Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
// 1BYTE Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��,b2-b6,b7=�ִμ̵���������ʽ(0=����1=�ڲ��Զ�����ʱ��ƽ)
// 1BYTE �ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
// 1BYTE �ӿ���ģ������Ķ��߱�־�ֽ�1

#define Terminal_Ctrl_Pn_Len     12										// �ն˿���ÿPn���ݳ���
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4			// �ն˿���-ң����բ
// �����ִ�1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
// 1BYTE 
// 2BYTE ��բʱ����(���ݸ�ʽ2)
// 1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
// 1BYTE 
// 2BYTE  
// 4BYTE 
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*NUMmax_CR)		//�ն˿���-����.��ǰ�����¸���
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=���㶨ֵ,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
// 1BYTE ����բ�ִ�B0-B7
// 2BYTE ��բʱ����(���ݸ�ʽ2)
// 1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
// 1BYTE 
// 2BYTE
// 1BYTE ����ֵ��־0=û��,1=����ֵ
// 1BYTE ��ǰ������բ�ִ�B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)	//�ն˿���-����.Ӫҵ��ͣ����
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
// 1BYTE ����բ�ִ�B0-B7
// 2BYTE ��բʱ����(���ݸ�ʽ2)
// 1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
// 1BYTE 
// 2BYTE
// 1BYTE ����ֵ��־0=û��,1=����ֵ
// 1BYTE ��ǰ������բ�ִ�B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-����.���ݹ���
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
// 1BYTE ����բ�ִ�B0-B7
// 2BYTE ��բʱ����(���ݸ�ʽ2)
// 1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
// 1BYTE 
// 2BYTE
// 1BYTE ����ֵ��־0=û��,1=����ֵ
// 1BYTE ��ǰ������բ�ִ�B0-B7
// 2BYTE
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-����.ʱ�ι���
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
// 1BYTE ����բ�ִ�B0-B7
// 2BYTE ��բʱ����(���ݸ�ʽ2)
// 1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
// 1BYTE ������0-2��ʾ1-3
// 2BYTE ��ǰʱ�ζ�ֵ
// 1BYTE ����ֵ��־0=û��,1=����ֵ
// 1BYTE ��ǰ������բ�ִ�B0-B7
// 2BYTE
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-���.�µ��
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
// 1BYTE ����բ�ִ�B0-B7
// 4BYTE �µ�����ֵ(���ݸ�ʽ3)
// 2BYTE 
// 1BYTE	
// 3BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)		//�ն˿���-���.�����
// �ܼ���1-N
// 1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
// 1BYTE ����բ�ִ�B0-B7
// 4BYTE 
// 1BYTE ���������ʣ�����(��)����ʱʶ��,0=������,1=�����
// 1BYTE 
// 4BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)		//�ն˿���ESRAM������ַ


// �ն�ͳ����ʵʱ����
//#define ADDR_213x_REAL     ADDR_F20B_2+LENmax_F20B_2//�������ѹ�ϸ���
// 1BYTE 0 �ϴμ���RTC��
// 1BYTE 1 �ϵ���ʱ��Ӷ�ʱ
// 1BYTE 2 �����
// 1BYTE 3 ��ѹ��ƽ������

// 4BYTE 4 ��ѹ��ƽ��
// 1BYTE 8 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
// 1BYTE 9 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������

// 4BYTE 10 ��ѹ��ƽ��
// 1BYTE 14 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
// 1BYTE 15 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������

// 4BYTE 16 ��ѹ��ƽ��
// 1BYTE 20 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
// 1BYTE 21 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
//#define LEN_2131_REAL     22

//#define ADDR_PowerEvent     ADDR_213x_REAL+LEN_2131_REAL//�ն�ͣ/�ϵ��¼�ԭʼ����
typedef __packed struct
{
	u8 Tstoptime[7];		//�ն�ͣ��ʱ��hexYYMDHMS
	u8 Tstarttime[7];		//�ն��ϵ�ʱ��hexYYMDHMS
	u8 RTCS;				//�ϴμ���ʱRTC����
	u8 task;				//����
	u8 bit;					//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
	u8 res1;
	u8 res2;
	u8 NUMrm;				//ͣ�ϵ��¼��賭���������
	u16 setno[NUMmax_Event3106RM];//ͣ/�ϵ��¼��賭��������������(��0��ʼ)
}PowerEvent_typedef;
//#define LEN_PowerEvent     	(20+(2*NUMmax_Event3106RM))
//#define PowerEvent     		((PowerEvent_typedef*)(ADDR_PowerEvent))

//#define ADDR_Event_Sour     	((ADDR_PowerEvent+LEN_PowerEvent+3)&0xfffffffc)//����������,����,ͣ���¼�ԭʼ����(����������Ŵ�0��ʼ)
typedef __packed struct
{
	u16 RMAddr[3];			//���ܱ���ַ
	u16 res;
	u32 Occur;				//�¼������ָ�,bit=0�ָ�=1����;b0=����ʧ��(310F),b1=
	u32 TimeCount;			//���ܱ�ͣ��ʱ���ʱ(��)
}EventSour_typedef;
#define LEN_Event_Sour     16


#define ADDR_RealTimeAMRTaskCtrl			((ADDR_Terminal_CTRL_ESRAM_END + 3) & 0xfffffffc)
#define LEN_RealTimeAMRTaskCtrl				(8192 + 2048)

#define ADDR_RS232_TxBuf					(ADDR_RealTimeAMRTaskCtrl + LEN_RealTimeAMRTaskCtrl)
#define LEN_RS232_TxBuf						(2048)

#define ADDR_IR_TxBuf						(ADDR_RS232_TxBuf + LEN_RS232_TxBuf)
#define LEN_IR_TxBuf						(2048)

#define ADDR_GPRS_TxBuf						(ADDR_IR_TxBuf + LEN_IR_TxBuf)
#define LEN_GPRS_TxBuf						(2048)

#define ADDR_ETHE_TxBuf						(ADDR_GPRS_TxBuf + LEN_GPRS_TxBuf)
#define LEN_ETHE_TxBuf						(2048)

#define ADDR_RS485_CH1_TxBuf				(ADDR_ETHE_TxBuf + LEN_ETHE_TxBuf)
#define LEN_RS485_CH1_TxBuf					(2048)

#define ADDR_RS485_CH2_TxBuf				(ADDR_RS485_CH1_TxBuf + LEN_RS485_CH1_TxBuf)
#define LEN_RS485_CH2_TxBuf					(2048)

#define ADDR_RS485_CH3_TxBuf				(ADDR_RS485_CH2_TxBuf + LEN_RS485_CH2_TxBuf)
#define LEN_RS485_CH3_TxBuf					(2048)

#define ADDR_PLC_YX_TxBuf					(ADDR_RS485_CH3_TxBuf + LEN_RS485_CH3_TxBuf)
#define LEN_PLC_YX_TxBuf					(2048)

#define ADDR_AC_SAMPLE_TxBuf				(ADDR_PLC_YX_TxBuf + LEN_PLC_YX_TxBuf)
#define LEN_AC_SAMPLE_TxBuf					(256)

#define ADDR_RS485_CH1_NormalTaskCtrl		(ADDR_AC_SAMPLE_TxBuf + LEN_AC_SAMPLE_TxBuf)
#define LEN_AMRNormalTaskCtrl				(20*1024 + 512)
#define ADDR_RS485_CH2_NormalTaskCtrl		(ADDR_RS485_CH1_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define ADDR_RS485_CH3_NormalTaskCtrl		(ADDR_RS485_CH2_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define ADDR_PLC_NormalTaskCtrl				(ADDR_RS485_CH3_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)

#define ADDR_LCD_MeterAMRDataBuf			(ADDR_PLC_NormalTaskCtrl + LEN_AMRNormalTaskCtrl)
#define LEN_LCD_MeterAMRDataBuf				(1024)

#define ADDR_NodeList						(ADDR_LCD_MeterAMRDataBuf + LEN_LCD_MeterAMRDataBuf)
#define LEN_NodeList						(32*1024)

#define ADDR_PnDataBuf						(ADDR_NodeList + LEN_NodeList)
#define LEN_PnDataBuf						(64*1024)

#define ADDR_ReportDataBuf					(ADDR_PnDataBuf + LEN_PnDataBuf)
#define LEN_ReportDataBuf					(64*1024)

/*
#define ADDR_AFN_0A_DataBuf					(ADDR_ReportDataBuf + LEN_ReportDataBuf)
#define LEN_AFN_0A_DataBuf					(256*1024)				

#define ADDR_AFN_0E_DataBuf					(ADDR_AFN_0A_DataBuf + LEN_AFN_0A_DataBuf)			// ���¼���¼���ݻ���
#define LEN_AFN_0E_DataBuf					(32*1024)				// ����� 64 �ֽ�Ϊ��ָֹ��������

#define ADDR_AFN_12_DataBuf					(ADDR_AFN_0E_DataBuf + LEN_AFN_0E_DataBuf + 64)	// ���������ݻ���
#define LEN_AFN_12_DataBuf					(64*1024)				// ����� 64 �ֽ�Ϊ��ָֹ��������

#define ADDR_AFN_13_DataBuf					(ADDR_AFN_12_DataBuf + LEN_AFN_12_DataBuf + 64)	// ���澯���ݻ���
#define LEN_AFN_13_DataBuf					(32*1024)				// ����� 64 �ֽ�Ϊ��ָֹ��������

#define ADDR_AFN_0D_DataBuf					(ADDR_AFN_13_DataBuf + LEN_AFN_13_DataBuf + 64)	// ����ʷ���ݻ���
#define LEN_AFN_0D_DataBuf					(2*1024*1024)			// ����� 64 �ֽ�Ϊ��ָֹ��������
*/

#define ADDR_RS232_QCSG_DataBuf				(ADDR_ReportDataBuf + LEN_ReportDataBuf)
#define LEN_RS232_QCSG_DataBuf				(512*1024)

#define ADDR_GPRS_QCSG_DataBuf				(ADDR_RS232_QCSG_DataBuf + LEN_RS232_QCSG_DataBuf)
#define LEN_GPRS_QCSG_DataBuf				(512*1024)

#define ADDR_ENET_QCSG_DataBuf				(ADDR_GPRS_QCSG_DataBuf + LEN_GPRS_QCSG_DataBuf)
#define LEN_ENET_QCSG_DataBuf				(512*1024)

#define ADDR_IR_QCSG_DataBuf				(ADDR_ENET_QCSG_DataBuf + LEN_ENET_QCSG_DataBuf)
#define LEN_IR_QCSG_DataBuf					(64*1024)

#define ADDR_RS485_1_QCSG_DataBuf			(ADDR_IR_QCSG_DataBuf + LEN_IR_QCSG_DataBuf)
#define LEN_RS485_1_QCSG_DataBuf			(256*1024)

#define ADDR_RS485_2_QCSG_DataBuf			(ADDR_RS485_1_QCSG_DataBuf + LEN_RS485_1_QCSG_DataBuf)
#define LEN_RS485_2_QCSG_DataBuf			(256*1024)

#define ADDR_RS485_3_QCSG_DataBuf			(ADDR_RS485_2_QCSG_DataBuf + LEN_RS485_2_QCSG_DataBuf)
#define LEN_RS485_3_QCSG_DataBuf			(256*1024)

#define ADDR_QCSG_ERAM_END					(ADDR_RS485_3_QCSG_DataBuf + LEN_RS485_3_QCSG_DataBuf)

#if (ADDR_QCSG_ERAM_END - ADDR_ERAM_START) > (LEN_Terminal_ERAM + LEN_ERAM_BUFF_UART_TIMER)
	#error ADDR_QCSG_ERAM_END Over
#endif

#define ADDR_LIN_YUN_ERAM_START			(ADDR_ERAM_START + LEN_ERAM_BUFF_UART_TIMER + LEN_Terminal_ERAM)
#if(((Project/100) == 2) || ((Project/100) == 3) || ((Project/100) == 5) || ((Project/100) == 9))			//����200=������,300=ר���ն�
	#define LEN_LIN_YUN_ERAM     		(1024*1024)			// �޶��ܳ����ֽ���
	#include "../Hcsg/ERAM_For_LinYun.h"
#else	
	#define ADDR_LIN_YUN_ERAM_END     	(ADDR_LIN_YUN_ERAM_START+0)
	#define LEN_LIN_YUN_ERAM     		0						// �޶��ܳ����ֽ���	
#endif

#if (ADDR_LIN_YUN_ERAM_END - ADDR_LIN_YUN_ERAM_START) > LEN_LIN_YUN_ERAM
	#error ADDR_LIN_YUN_ERAM_END Over
#endif


#define ADDR_MXN_ERAM_START				(ADDR_LIN_YUN_ERAM_START + LEN_LIN_YUN_ERAM)
#if(((Project/100) == 2) || ((Project/100) == 3) || ((Project/100) == 5) || ((Project/100) == 9))			//����200=������,300=ר���ն�
	#define LEN_MXN_ERAM     			(1024*1024)			// �޶��ܳ����ֽ���
	#include "../Hcsg/ERAM_For_Mxn.h"
#else	
	#define ADDR_MXN_ERAM_END     		(ADDR_MXN_ERAM_START+0)
	#define LEN_MXN_ERAM     			0						// �޶��ܳ����ֽ���	
#endif

#if (ADDR_MXN_ERAM_END - ADDR_MXN_ERAM_START) > LEN_MXN_ERAM
	#error ADDR_MXN_ERAM_END Over
#endif

#define ADDR_ERAM_END 					(ADDR_MXN_ERAM_END & 0xfffffffc)		

//ʣ��ERAM�ռ�ȫ�����Զ�����(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00400000
    #error ADDR_ERAM_END>4//4M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00400000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00400000//ʣ��ERAM�ռ䳤��
#endif

#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=û,1=��
#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(�����������ѳ����������)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>6//6M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00A00000//ʣ��ERAM�ռ䳤��
#else
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8//8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00800000//ʣ��ERAM�ռ䳤��
#endif
#endif

#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00700000
    #error ADDR_ERAM_END>7M//32M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00700000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x01900000//ʣ��ERAM�ռ䳤��
#endif
#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00900000
    #error ADDR_ERAM_END>9M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00900000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x02700000//ʣ��ERAM�ռ䳤��
#endif

#if IC_SDRAM==0
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>LEN_Terminal_Ram
    #error ADDR_ERAM_END>LEN_Terminal_Ram
  #endif
	#define ADDR_MyHeap     0
	#define LEN_MyHeap     0//ʣ��ERAM�ռ䳤��
#endif



#endif
















