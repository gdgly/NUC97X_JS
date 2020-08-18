


#ifndef ERAM_H
#define ERAM_H

#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
#define LEN_ERAM_BUFF_UART_TIMER     ((4*1024*1024)+(2*LEN_msFILEmanage_Ext))//���ݻ�������UART�Ͷ�ʱ����ϵͳ�����ܳ���
#else
#define LEN_ERAM_BUFF_UART_TIMER     ((6*1024*1024)+(2*LEN_msFILEmanage_Ext))//���ݻ�������UART�Ͷ�ʱ����ϵͳ�����ܳ���
#endif

#ifdef IC_STM32F4xx
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
	#ifndef LINUX
//0x00000000-0x003FFFFF 4M-512-16byte Program
#define ADDR_ETHMAC     (0x00400000-512-16)//��̫��MAC��ַ
//6byte MACADDR0
//6byte MACADDR1
#define ADDR_REG_SYS_RSTSTS     (0x00400000-512-4)
//4byte �ϵ�ʱ��ȡ��REG_SYS_RSTSTSֵ
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

	#else
	
//1M ISRAM
//ADDR_ERAM_linux+0x00001000   256K   Meter
//ADDR_ERAM_linux+0x00040000   768K     Terminal
//ADDR_ERAM_linux+0x00100000   256K     Menu or Test

#define ADDR_SPI_MAP     (ADDR_ERAM_linux+0x00240000)
#define LEN_SPI_MAP     0x00080000
//512K IEEPROM(SPIFLASH) ӳ��
#define ADDR_NAND_MAP     (ADDR_ERAM_linux+0x002C0000)
#define LEN_NAND_MAP     0x00280000
//0x00C80000-0x00EFFFFF 2.5M NAND_FLASH ӳ��
#define ADDR_ERAM_START     (ADDR_ERAM_linux+0x00540000)
	#endif
#endif



#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
//LEN_SaveRam BYTE ��SRAM���Ƶĵ��籣������,�ϵ�ʱ����0�ʷ�����ǰ�Ա��ϵ�ʱ����������0��ַ����

#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#if  NAND_1G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (16*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#if  NAND_2G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (16*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#if  NAND_4G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#if  NAND_8G==1
	#define LEN_msFILEmanage     ((256*1024)+LEN_msFILEmanage_Ext)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#if LEN_msFILEunltFlags>=LEN_msFILEmanage
	#error
#endif
//LEN_msFILEunltFlags ���:0=δʹ��,1=ռ�õ����¿���,2-8191�¿�Ŀ��
//LEN_msFILEmanage-��־��-12�ֽ� �ļ���ڱ�
	//�ļ���ڱ�1
	//...
	//�ļ���ڱ�n
//12BYTE
	//4byte �ļ����䵥Ԫ����
	//4byte��Ч��ʶ0x12345678
	//4byte CRC32


//256K��768K ����(��ǰ�������ֶ�ǿ�Ƹ�λʱ���ݻָ���)

typedef __packed struct
{
	u16 BLOCK_ENTRY;//�ļ����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�(ֵ��Чʱ!=0)
	u16 BLOCK_END;//�ļ�����:nand flash���,0=��;RAM ��ʼ��ַ��2�ֽ�
	u32 LEN_FILE;//�ļ�Ԥ��ռ�д洢�ռ��ܳ���
	u32 LEN_DATA;//�ļ���Ч�����ܳ���
	
	u32 FILEID;//FILEID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	//�ļ�����:
	//0=��ͨ�ɼ��������ݼ�¼
	//1=
	//2=��OI������¼���¼
	//
	//
	u16 FILECOUNT;//��ͬFILEID���ļ�������
	u16 NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
	u16 NUM_FAIL;//�ļ�ͷ��д�볭��ʧ�ܵ�ַ����
	u8 save_date_time_s[7];//�ɼ��洢ʱ��
	u8 start_date_time_s[7];//�ɼ�����ʱ��
#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
//GPRS,ENET�����ϱ�
	u16 NUM_GPRSreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
	u16 NUM_ENETreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
#else
//GPRS,ENET�ϲ��ϱ�
	u16 NUM_GPRSorENETreport[NUMmax_601C];//���ϱ������ݸ���(����ϱ�������)
#endif
#ifndef LINUX_CROSS_GCC
}msFILE_TypeDef;
#else
}__attribute__ ((packed)) msFILE_TypeDef;
#endif
#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)//128K(BKPSDRAM) NAND FLASHд����(�������ݼ�����ʱ��0)
//2BYTE �ܿ���
//4BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
//���ݿ�1
//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
//4BYTE �����ֽ���
//4BYTE �Ѳ���д����ֽ���
//nBYTE ����
//...
//���ݿ�n
typedef __packed struct
{
	u32 ADDR_Dest;//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
	u32 LEN_Data;//4BYTE �����ֽ���
	u32 LEN_Write;//4BYTE �Ѳ���д����ֽ���
	u8 Data[1];//nBYTE ����
#ifndef LINUX_CROSS_GCC
}BKPSDRAM_BLOCK_TypeDef;
#else
}__attribute__ ((packed)) BKPSDRAM_BLOCK_TypeDef;
#endif
typedef __packed struct
{
	u16 NUM_BLOCK;//2BYTE �ܿ���
	u32 LEN_TOTAL;//4BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
	//���ݿ�
#ifndef LINUX_CROSS_GCC
}BKPSDRAM_TypeDef;
#else
}__attribute__ ((packed)) BKPSDRAM_TypeDef;
#endif
#define ADDR_SDRAM_CLR_START     (ADDR_BKPSDRAM+(128*1024))//SDRAM������0��ʼ��ַ


#define ADDR_RAMmsFILEmanage     ADDR_SDRAM_CLR_START//�洢��RAM�е��ļ�����
#define LEN_RAMmsFILEmanage     (8*1024)//(4*1024)
#define LEN_RAMmsFILEblock     (32*1024)//RAMmsFILE�ļ��鳤��


#define ADDR_emWin     ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
//128K emWin������
#define ADDR_128KDATABUFF     (ADDR_emWin+(128*1024))
//128K DATABUFF
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
//128K�ֽ�NAND FLASH��д����
#define ADDR_NAND_PAGEREAD_BUFF     (ADDR_128KWRITE_BUFF+(128*1024))
//2048�ֽ�NAND FLASHҳ������
#define ADDR_ECC_BUFF     (ADDR_NAND_PAGEREAD_BUFF+2048)
//128+128�ֽ�ECC����

#define ADDR_MACADDR_ascii     (ADDR_ECC_BUFF+256)//����MAC��ַ��ascii��
//20BYTE ��ʽ"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     (ADDR_MACADDR_ascii+20)//����IP4��ַ��ascii��
//16BYTE ��ʽ"192.168.0.100"
#define ADDR_IP4MASK_ascii     (ADDR_IP4ADDR_ascii+16)//����IP4 MASK��ַ��ascii��
//16BYTE ��ʽ"255.255.255.0"
#define ADDR_IP4GATEWAY_ascii     (ADDR_IP4MASK_ascii+16)//����IP4 GATEWAY��ַ��ascii��
//16BYTE ��ʽ"192.168.0.254"
#define ADDR_IP4PRIMARY_DNS_ascii     (ADDR_IP4GATEWAY_ascii+16)//����IP4 PRIMARY_DNS��ascii��
//16BYTE ��ʽ"8.8.8.8"
#define ADDR_IP4SECONDARY_DNS_ascii     (ADDR_IP4PRIMARY_DNS_ascii+16)//����IP4 SECONDARY_DNS��ascii��
//16BYTE ��ʽ"8.8.4.4"
#define ADDR_IP6ADDR_ascii     (ADDR_IP4SECONDARY_DNS_ascii+16)//����IP6��ַ��ascii��
//40BYTE
#define ADDR_IP6GATEWAY_ascii     (ADDR_IP6ADDR_ascii+40)//����IP6 GATEWAY��ַ��ascii��
//40BYTE
#define ADDR_IP6PRIMARY_DNS_ascii     (ADDR_IP6GATEWAY_ascii+40)//����IP6 PRIMARY_DNS��ascii��
//40BYTE
#define ADDR_IP6SECONDARY_DNS_ascii     (ADDR_IP6PRIMARY_DNS_ascii+40)//����IP6 SECONDARY_DNS��ascii��
//40BYTE


#define ADDR_UARTnCtrl_Start     (ADDR_IP6SECONDARY_DNS_ascii+40)//UART
//UART0
#define ADDR_UART0     (ADDR_UARTnCtrl_Start)
#define ADDR_UART0IntRx     ADDR_UART0
#define ADDR_UART0Ctrl     (ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     (ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     (ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     (ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     (ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     ADDR_UART1
#define ADDR_UART1IntRx     (ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     (ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     (ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     (ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     (ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     (ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#define ADDR_UART2     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#define ADDR_UART2IntRx     ADDR_UART2
#define ADDR_UART2Ctrl     (ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     (ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     (ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     (ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     (ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     ADDR_UART3
#define ADDR_UART3Ctrl     (ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     (ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     (ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     (ADDR_UART3Tx+LEN_UART3TxMax)
//UART4(�������ز�,ר�����ģ��)
#define ADDR_UART4     (ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#define ADDR_UART4IntRx     ADDR_UART4
#define ADDR_UART4Ctrl     (ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     (ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     (ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     (ADDR_UART4Tx+LEN_UART4TxMax)

#define ADDR_UART4RxCopy     (ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)//�������ز�ͨ�������ϱ�֡����
#define LEN_UART4RxCopy     128

//UART5
#define ADDR_UART5     (ADDR_UART4RxCopy+(LEN_UART4RxCopy*NUM_UART4RxCopyMax))
#define ADDR_UART5IntRx     ADDR_UART5
#define ADDR_UART5Ctrl     (ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     (ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     (ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     (ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     (ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     ADDR_UART6
#define ADDR_UART6Ctrl     (ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     (ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx     (ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     (ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(�ն�ESAM)
#define ADDR_UART7     (ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     ADDR_UART7
#define ADDR_UART7Ctrl     (ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     (ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     (ADDR_UART7Rx+LEN_UART7Rx)
#define ADDR_UART7FnDataBuff     (ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(���ESAM)
#define ADDR_UART8     (ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     ADDR_UART8
#define ADDR_UART8Ctrl     (ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx     (ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     (ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     (ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(���CARD)
#define ADDR_UART9     (ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     ADDR_UART9
#define ADDR_UART9Ctrl     (ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     (ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     (ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff     (ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(��̫��)
#define ADDR_UART10     (ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     ADDR_UART10
#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     (ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     (ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     ADDR_UART11
#define ADDR_UART11Ctrl     (ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     (ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     (ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     (ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(�����)
#define ADDR_UART12     (ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     ADDR_UART12
#define ADDR_UART12Ctrl     (ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     (ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     (ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     (ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART13     (ADDR_DATABUFF)
#define ADDR_UART13IntRx     ADDR_UART13
#define ADDR_UART13Ctrl     (ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     (ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     (ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     (ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART14     (ADDR_DATABUFF)
#define ADDR_UART14IntRx     ADDR_UART14
#define ADDR_UART14Ctrl     (ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     (ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     (ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     (ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART15     (ADDR_DATABUFF)
#define ADDR_UART15IntRx     ADDR_UART15
#define ADDR_UART15Ctrl     (ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     (ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     (ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     (ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART16     (ADDR_DATABUFF)
#define ADDR_UART16IntRx     ADDR_UART16
#define ADDR_UART16Ctrl     (ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     (ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     (ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     (ADDR_UART16Tx+LEN_UART16TxMax)

#define ADDR_UARTnCtrl_End     (ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)//(ADDR_UART19FnDataBuff+LEN_UART19FnDataBuff)

typedef __packed struct
{
	u8 Run;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
	u8 res2;//
	u8 StdTime[7];//����Ƶ�ʵ��׼ʱ��YYMDHMS,
	u8 StartTime[7];//����ִ�п�ʼʱ��YYMDHMS,
	u8 EndTime[7];//����ִ�н���ʱ��YYMDHMS
	u8 res1;
#ifndef LINUX_CROSS_GCC
}UARTnTaskTime_TypeDef;
#else
}__attribute__ ((packed)) UARTnTaskTime_TypeDef;
#endif
#define LEN_UARTnTask     24//����ֶ���,��0ʱ��16bitд
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
#ifndef LINUX_CROSS_GCC
}RMAddrList_TypeDef;//UART�����ַ�б�
#else
}__attribute__ ((packed)) RMAddrList_TypeDef;
#endif
#define LEN_RMAddrList     6
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 ReRMCount;//�����ط���������
	u8 SpaceData;//0=ʧ��,1=,2=��������������Ϊ�ձ�־ʧ��(̨����Բɼ���ؼ�Ϊ����ʧ��)
#ifndef LINUX_CROSS_GCC
}RMokfailLIST_TypeDef;//����ɹ�ʧ�ܵ�ַ�б�
#else
}__attribute__ ((packed)) RMokfailLIST_TypeDef;
#endif
#define LEN_RMokfailLIST     8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u16 LEN_DATA;
#ifndef LINUX_CROSS_GCC
}RecordFileHead_TypeDef;//�����¼�ļ�ͷ
#else
}__attribute__ ((packed)) RecordFileHead_TypeDef;
#endif
#define LEN_RecordFileHead     8
typedef __packed struct
{
	u8 OAD[4];
	u8 LEN_data[2];
	u8 REPORT;//�ϱ���־:0=�����ϱ�,1=���ϱ���2=���ϱ�
	u8 res1;
#ifndef LINUX_CROSS_GCC
}CongealFileHead_TypeDef;//�����ļ�ͷ
#else
}__attribute__ ((packed)) CongealFileHead_TypeDef;
#endif
#define LEN_CongealFileHead     8
typedef __packed struct
{
	u8 LEN_data[2];//�ܳ���,�¼�����+OAD˳���(array OAD)
	u8 LEN_event[2];//�¼����ݳ���
	u8 REPORT;//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
	u8 OCCUR;//�����ָ�:0=�ָ�,1=����,2=��
	u8 ACK;//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
	u8 res1;
#ifndef LINUX_CROSS_GCC
}EventFileHead_TypeDef;//�¼���¼�ļ�ͷ
#else
}__attribute__ ((packed)) EventFileHead_TypeDef;
#endif
#define LEN_EventFileHead     8
typedef __packed struct
{
	u16 Addr0;
	u16 Addr1;
	u16 Addr2;
	u8 AddrType;//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	u8 LEN_DATA;
#ifndef LINUX_CROSS_GCC
}SearchMeterFileHead_TypeDef;//�ѱ��¼�ļ�ͷ
#else
}__attribute__ ((packed)) SearchMeterFileHead_TypeDef;
#endif
#define LEN_SearchMeterFileHead     8









typedef __packed struct
{
	u8 APPVER[2];//  ������Ӧ�ò�Э��汾��    long-unsigned��
	u8 Protocol[8];// 	������Э��һ���Կ�        ProtocolConformance��
	u8 Function[16];//  �����Ĺ���һ���Կ�        FunctionConformance��
	u8 Len_Tx[2];//  �ͻ�������֡���ߴ�      long-unsigned��
	u8 Len_Rx[2];//  �ͻ�������֡���ߴ�      long-unsigned��
	u8 NUM_RxFrame;//  �ͻ�������֡��󴰿ڳߴ�  unsigned��
	u8 Len_APDU[2];//  �ͻ������ɴ���APDU�ߴ� long-unsigned��
	u8 ConnectOVERTime[4];//  ������Ӧ�����ӳ�ʱʱ��    double-long-unsigned��
	u8 ConnectMechanismInfo;//  ��֤�������              ConnectMechanismInfo
	//ConnectMechanismInfo��=CHOICE
	//{
	//  ��������    [0] NullSecurity��
	//  һ������    [1] PasswordSecurity��
	//  �ԳƼ���    [2] SymmetrySecurity��
	//  ����ǩ��    [3] SignatureSecurity
	//}
#ifndef LINUX_CROSS_GCC
}ClientConnectInfo_TypeDef;//�ͻ���������Ϣ
#else
}__attribute__ ((packed)) ClientConnectInfo_TypeDef;
#endif
#define LEN_ClientConnectInfo     38

typedef __packed struct
{
//	���������̰汾��Ϣ        FactoryVersion��
	u8 code[4];//	���̴���         visible-string(SIZE (4))
	u8 sver[4];//	����汾��      visible-string(SIZE (4))
	u8 svdate[6];//	����汾����   visible-string(SIZE (6))
	u8 hver[4];//	Ӳ���汾��      visible-string(SIZE (4))
	u8 hvdate[6];//	Ӳ���汾����   visible-string(SIZE (6))
	u8 Info[8];//	������չ��Ϣ   visible-string(SIZE (8))
	u8 APPVER[2];//  �̶���Ӧ�ò�Э��汾��    long-unsigned��
	u8 Protocol[8];//  �̶���Э��һ���Կ�        ProtocolConformance��
	u8 Function[16];//  �̶��Ĺ���һ���Կ�        FunctionConformance��
	u8 Len_Tx[2];//  ����������֡���ߴ�      long-unsigned��
	u8 Len_Rx[2];//  ����������֡���ߴ�      long-unsigned��
	u8 NUM_RxFrame;//  ����������֡��󴰿ڳߴ�  unsigned��
	u8 Len_APDU[2];//  ���������ɴ���APDU�ߴ� long-unsigned��
	u8 ConnectOVERTime[4];//  �̶���Ӧ�����ӳ�ʱʱ��    double-long-unsigned��
	u8 ConnectResult;//Ӧ������������֤�Ľ��
	//ConnectResult��=ENUMERATED
	//{
	//  ������Ӧ������     ��0����
	//�������             ��1����
	//�Գƽ��ܴ���         ��2����
	//�ǶԳƽ��ܴ���       ��3����
	//ǩ������             ��4����
	//Э��汾��ƥ��       ��5����
	//��������             ��255��
	//}
#ifndef LINUX_CROSS_GCC
}ServerConnectInfo_TypeDef;//�����������Ϣ
#else
}__attribute__ ((packed)) ServerConnectInfo_TypeDef;
#endif
#define LEN_ServerConnectInfo     70




typedef __packed struct
{
//ͨ�ò���
  u8 Task;//������:
					//0=���ڽ���,1=�������,2=�ȴ���֡���ݷ���,3=��֡�������ڷ���
					//4=��֡�������������,5=�ȴ���֡���ݷ���,6=��֡�������ڷ���
					//7=�ȴ���������תΪ����
          //8=�ȴ�ͨ������������ת��,9=�ȴ�����������ʱ,10=����ͨ�������ڷ�������,11=����ͨ�������ڽ�������
          //20=�����������,21=����ת����������,22=����ת����������,23=���ռ�������
 					//
	u8 Lock;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
  u16 RxByte;//�����ֽ���(���ֶ���)
  u16 TxByte;//�����ֽ���(���ֶ���)
  u16 TxCount;//�ѷ��ͼ���(���ֶ���)
  u8 Safety;//��ȫ��֤(������)
	u8 SECURITY_Request;//��ȫ����:b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
  u16 BpsCtrl;//ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
			//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
			//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
			//D2λ��ʾż(Even)����(Odd)У��λ0/1�ֱ��ʾż����У��
			//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
  u8 Protocol;//ͨ�Ź�Լ�Զ�ʶ��:0=698.45,1=DL/T645-2007
  u8 NewBps;//�����ʸ��±�־;��ΪMESAMͨ��ʱΪMESAM�ڲ����ݸ��±�־0=����,����ֵû����
  u8 SubTask;//������
  u8 AddrType;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
	//��16Byte
//�ն�ר�ò���
	u32 RxTxByte;//ͨ������
	u16 PORT;//�����˿ں�(���ֶ���),ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
	u8 TCPUDP;//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
	u8 LinkCtrl;//���ӿ���0=������,1=����
	u8 LinkCount;//���Ӽ���(2����IP,1�α���IP)
	u8 ReLinkNum;//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
	u8 LinkTask;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	u8 LinkSubTask;//����������
	u8 HeartberatCount;//������������,�յ�ȷ��֡��0
	u8 HostACK;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
	u8 HostCount;//�����ϱ��ط�����(���������������)
	u8 TimerTaskCount;//�����ϱ���ʱ�������
  u8 RxSEQ;//����֡������SEQ
  u8 TxSEQ;//����֡������SEQ
	
	u8 ReNewLink;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
	u8 res3;//
	u8 res4;//
	u8 ATTask;//AT����:0=����,1=����,2=���ڷ���,3=����
	u8 ATWriteTask;//AT���ݷ�������:0=������,1=������
	u8 ATCount;//AT����
	
	u8 LinkTask2;//ͨ��2��������(FTP����)
	u8 FTPLinkCount;//FTP�������Ӽ���
	u8 Task2;//ͨ��2����(FTP����)
	u8 FTPErr;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
	u32 FTPByte;//FTP��ǰ�ļ�����������ֽ���
	u32 FTPFileLen;//FTP��ǰ�ļ��ܳ���
	u16 TxAPDUSEQ;//��·���֡��ż���0-4095
	u16 TxAPDUByte;//��·���֡�ѷ���APDU�ֽ���
	//��40Byte
//���г���ר�ò���
	u8 LINKCHECK_RTCS;//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
	u8 RTCS_TASK;//ʵʱ�Ӹ��±�־ԭʼ��(��������)
	u8 RTCM;//ʵʱ�Ӹ��±�־ԭʼ��
	u8 RTCh;//ʵʱ�Ӹ��±�־ԭʼʱ
	u8 RTCD[2];//ʵʱ���ո��±�־��ԭʼ����
	u8 RTCW[2];//ʵʱ�����ڸ��±�־��ԭʼ����
	u8 res7;
	u8 Lock_InfoTask;//��·������������Ϣ������:0x55=����
	u8 InfoTask;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	u8 LastAutoSearchRTCD;//�ϴ��Զ��ѱ�ʵʱ����
	u8 AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
	u8 AutoSearchReCount;//�Զ��ѱ������ط�����
	u16 AutoSearchTime;//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
	u16 TransferDelay;//��·�ϴ�����ʱʱ����
	u8 NETOPENCount;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
	u8 ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
	u16 SMSRxSEQ;//���Ž��սӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	u16 SMSTxSEQ;//���ŷ��ͽӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	//��24Byte
//DL698
	u8 Function;//�ڹ��� 0=�ն�ά����,!=0�ն˳����
	u8 FunctionTimer;//�ϴοڹ��ܼ���ʵʱ����
	u8 PowerUpDealy;//�ϵ���ʱ:0=û��ʱ,!=0�ѵ���ʱ
	u8 DHCPTask;//DHCP����
	s32 TCPSocket;
	s32 UDPSocket;
	u8 NET_ADDR[20];//NET_ADDR
	u8 GET_Reques_CHOICE;//ԭ�õ�����ѡ��
	u8 PIID;
	u8 DAR;//DL698���ݷ��ʽ��
	u8 NEXT;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	u32 pRxBuff;//DL698��֡������ջ���ָ��Ĵ�	
	u32 NEXTOADNum;//DL698��֡���贫���OAD����
	u32 NEXTOADSubNum;//DL698��֡����OAD���Ӽ���

	u8* pTxData;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���(��������2byte)
	u32 LEN_TxData;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	u32 LEN_ReadData;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2byte�γ���)
	u16 NEXTBlock;//DL698��֡������̵�֡��ţ�ȡֵ��Χ0~4095
	
	u8 CONNECT;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	u8 RMMeterCountAuto;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
  u8 RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��,128=DL376.2
	u8 RMCSDCount;//����CSD����
	u8 RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
	u8 RMCSDSubCount;//����CSD�Ӽ���(��DL698����ʱ)
	u8 RMCSDCountMax;//����CSD������
	u8 RMROADCountMax;//����CSD��ROAD���й�����OAD������(��DL698�����Լ���趨)
	u8 RMCSDSubCountMax;//����CSD���ӳ������(��DL698�����Լ���趨)
	u8 RMROADCountOK;//����д�ļ����ݻ�����ROADʱ�����������OAD���ݸ�������
	u8 ReRMCount;//�����ط�����
	u8 RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
	u8 RM_DI_CHOICE;//����DIѡ��:0=��DI,1=����DI
	u8 SetChange6000;//�ɼ��������ñ��б仯��־:0=û�仯,0x55=�仯
	u32 TaskID;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	u32 RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	u16 res8;//
	u16 RM_DI_N;//�ɼ��ϵ�N��
	u8 res9;
	u8 RM_DI_TI[3];//��ʱ�����ɼ�(��λ���ڵ��ֽ�ͬ698)
	u16 RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
	u16 RM_SubsequentFrames;//0=�޶�����֡,1-n=����֡��֡��
	u16 TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
  u8 RM_Rx_date_time_s[8];//������յ�������ʱ��(����������),7byte,��Ϊ���붨��8byte
  u8 RM_Rx_date_40020200[10];//������յ��������¼������ݴ漰����MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
	
	u8 ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
	u8 ProxyPORTn;//�������Ķ˿ں�
	u8 ProxyBpsCtrl;//����ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
	u8 ProxyRMprotocol;//�������Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	u16 ProxyTxRxByte;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
	u16 ProxyOverTime;//����1���������ĳ�ʱʱ����
	u16 REPORT_OVER_SECOND;//�ϱ���Ӧ��ʱʱ��Ĵ�
	u8 REPORT_NUMmax;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
	u8 REPORT_RTCS;//�ϱ�����ʱ�����RTC��
	u8 REPORT_PIID;//�ϱ�����֡PIID
	u8 REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	u16 REPORT_filename[REPORT_NUMFILEmax];//1֡�����ϱ����ݲ�ͬ�ļ����б�
	u16	REPORT_NUM_Data[REPORT_NUMFILEmax];//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
#define LEN_REPORT_file   (4*REPORT_NUMFILEmax)
	//122+LEN_REPORT_file byte
	u8 EVENT_FAIL;//�豸�����¼���¼(485���ز�ͨ��),0=�ջ�ָ�,1=����
	u8 BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
	u8 LastBroadcastRTCD2;//�ϴ�ִ�й㲥Уʱ-2��ʵʱ����
	u8 LastBroadcastRTCD3;//�ϴ�ִ�й㲥Уʱ-3��ʵʱ����
	u8 BroadcastTimeProxyTask;//����͸��ת���㲥Уʱ����:0=��,1=�д���
	u8 RM_Rx_date_NULL;//������շ���:0=������,!=0������(������)
	
	u16 AutoReportCopyNUM;//�����ϱ������Ѿ�����֡��
	u16 SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
	u8 NUM_SomeTaskID;//��ǰͬ��ɼ������id�ŵ��б����
	u8 NUM_LastSomeTaskID;//�������ͬ��ɼ������id�ŵ��б����
	u16 NUM_SetMeter;//���˿��ڵ��������õ��Ǳ�����
	u16 res23;//
	u32 res24;//
	u32 res35;//
	u32 res36;//
	u8 Rand[16];//����ȫģʽʱ���͸�����16�ֽ������
	u8 OAD_44000400[2];//Ӧ������:��ǰ���ӵĿͻ�����ַ��=unsigned
	//54 byte
	u8 ConnectInfo[80];//������Ϣ
#if ((LEN_ClientConnectInfo>80)||(LEN_ServerConnectInfo>80))
	#error
#endif
#if (LEN_REPORT_file+80)>512
	#error
#endif
	u8 res104[512-(LEN_REPORT_file+80)];
	
	u8 AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
	u8 AutoReportRMprotocol;//���ܱ������ϱ���Լ=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	u8 AutoReportRMAddr[6];//���ܱ������ϱ���ַ(ֱ�Ӷ�����֡��645��698���ֵ�ַ˳��)
	u8 AutoReportData[248];//���ܱ������ϱ����ݼĴ�
		//DL645:12byte״̬�ּĴ����ڷ���λ����+96byte��ӦB0-B95�����ϱ��¼���������hex
		//DL698:
	
//���½�698��������ר��,�ǳ���ڲ���Ҫ(����,�����ϱ��������õ�)
	//byte=LEN_UARTnTask*(RMTASKmax+ExtRMTASKmax)
	UARTnTaskTime_TypeDef TaskTime[RMTASKmax+ExtRMTASKmax];
	//byte=(15*(RMTASKmax+ExtRMTASKmax))+128    (�ǳ����)
	//byte=(15*(RMTASKmax+ExtRMTASKmax))+128+(LEN_RMokfailLIST*(RMTASKmax+ExtRMTASKmax)*NUM_RMmax)   (�����)
	u16 MeterCount[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
	u8 RMLOOP[RMTASKmax+ExtRMTASKmax];//RMTASKmax byte ȫ�����ܱ���һ��ı�������,���������
	u16 NUMrmTOTAL[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte��������ɼ����ܱ�����,�����ϱ�ʱ�����ļ���������
	u16 NUMrmOK[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte�������Ѳɼ���ɵ��ܱ�����
	u16 NUMrmFAIL[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte�������Ѳɼ�ʧ�ܵ��ܱ�����
	u16 NUMrmBAD[RMTASKmax+ExtRMTASKmax];//2*RMTASKmax byte�������Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������
	u8 date_time_s[7];//��ǰ��¼�ļ����ѳ�����ַ�б�ʱ��!!!���ݴ洢ʱ��(����������),7byte,��Ϊ���붨��8byte
	u8 TSA[3];//TSA���������ͳ��ȵȿ�ʼ3�ֽ�,�����¼�����Դ
	u16 RMAddr[3];//6 byte ��ǰ���ڲɼ�ͨ�ŵ�ַ(��λ���ڵ��ֽ�)(����������),ȫ0��ʾ��Ч��ַ
	u8 RMOADROAD;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
#define LEN_RMCSD     (128-17)
	u8 RMCSD[LEN_RMCSD];//128-x byte ��ǰ���ڳ���CSD(����������)
	RMokfailLIST_TypeDef rmOKFAILlist[RMTASKmax+ExtRMTASKmax][NUM_RMmax];//����ɹ�ʧ�ܵ�ַ�б�
	u16 SomeTaskid[RMTASKmax+ExtRMTASKmax];//RMTASKmax*2 byte ��ǰͬ��ɼ������b15-b8�����,b7-b0�������
	u16 LastSomeTaskid[RMTASKmax+ExtRMTASKmax];//RMTASKmax*2 byte �������ͬ��ɼ������b15-b8�����,b7-b0�������
#ifndef LINUX_CROSS_GCC
}UARTCtrl_TypeDef;
#else
}__attribute__ ((packed)) UARTCtrl_TypeDef;
#endif

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





#define ADDR_RMTaskID     ADDR_UARTnCtrl_End//��ǰ��ִ�еĳ��������б�
//4byte ��ЧRMTaskID����
//4*NUMmax_6012_2 byte TaskID
#define ADDR_REPORTTaskID     ADDR_RMTaskID+(4+(4*NUMmax_6012_2))//��ǰ��ִ�е��ϱ������б�
//4byte ��ЧREPORTTaskID����
//4*NUMmax_6012_2 byte TaskID
#define ADDR_AddrList     ADDR_REPORTTaskID+(4+(4*NUMmax_6012_2))//��ַ�б�
//2byte �б���
//typedef __packed struct
//{
//	u16 Addr0;
//	u16 Addr1;
//	u16 Addr2;
//	u16 LEN_DATA;
//}RecordFileHead_TypeDef;//�����¼�ļ�ͷ
#define ADDR_1kDATABUFF     ADDR_AddrList+4+(NUM_RMmax*LEN_RecordFileHead)//ADDR_AddrList+2+(NUM_RMmax*LEN_RecordFileHead)
//1k byte ��ҪΪRCSDΪȫѡʱ�������¼�ʱ��OAD�б�
#define LEN_1kDATABUFF     1024

#define ADDR_Router     ADDR_1kDATABUFF+LEN_1kDATABUFF//·����(�������ز�)
typedef __packed struct
{
	u8 RouterAddr[6];//·�����ڵ��ַ(���Ƿ�����ǰ,ͨ�ſ��ַ��λ��)
	u8 RouterCODE[4];//���̴��� ASCII 2,оƬ���� ASCII 2
	u8 RouterVer[5];//·�����汾��,�ֱ�Ϊ:��,��,��,2BYTE�汾

	u8 RouterCtrl;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
	u8 RouterCtrlTask;//·���������������;��ɺ��Զ��ص�0
	u8 ReRouterCtrl;//·���������ط�����
	u8 RUNState;//��ģ���ȡ�ĵ�ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
	u8 UpDateTask;//�����������
	u8 UpDateReCount;//����������Դ���
	u8 MinuteCollect;

	u8 RouterInitTask;//·������ʼ������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	u8 RouterInitSubTask;//·������ʼ��������
	u8 RouterInitReCount;//·������ʼ�������ط�����
	u8 NUM_Enable_Concurrency;//����ز���ǰ��������֡��
	u8 MaxOverTime;//�ӽڵ������ʱʱ����
	//#if (USER/100)==17//����ϵ
//	u8 MinuteCollect;//�Ƿ�֧�ַ��Ӳɼ�
//	u16 NoteNum1;
 //#endif
	
	u8 RouterSynTask;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
	u16 RMListCount;//�����ַ���б���Զ��������0-(NUM_RMLISTmax-1)

	u16 ModeNoteNum;//����ͬ��ʱ��ģ���ȡ��ģ�鷵�ص�ǰ�ӽڵ���(���ֶ���)
	u16 NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
	u16 TempCount;//ɾ������ӽڵ����ʱ����(���ֶ���)
	u16 UpDateCount;//�������֡����(���ֶ���)
	u16 ReadNoteNum;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
	u16 RS485Time;//�ɼ�����485���ʱֵ��(BCD)(���ֶ���)
	u16 KeepTime;//ע�����ʱ�䶨ʱֵ��(���ֶ���)
	u8 n;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
	u8 RMTD;//0=�ǳ���ʱ��,1=����ʱ��
	u8 NoteNumOver;//AFN04F10���õĴӽڵ���>ģ��֧�ֵ����ӽڵ���;0=��,1=��;����������ģ������ʾ��������ʱ��ʾ������Ϣ
	u8 res4;
	u16 NoteNum1;
	u16 BandRate;
	u32 StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
		//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
		//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
		//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	u8 RouterRunMode_1;//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	u8 RouterRunMode_2;//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
	u8 RouterRunMode_3;//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
	u8 RouterRunMode_4;//·������ģʽ_���ڳ���ģʽ�� 01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
	u8 RouterRunMode_5;//·������ģʽ_������ʱ����֧�֣���D2��D1��D0������λ��ʾ���ֱ����㲥���ӽڵ��ء�·�������������������֧���������ṩ������ʱ�����������1��ʾ֧���ṩ������ʱ������0��ʾ��֧��
	u8 RouterRunMode_6;//·������ģʽ_ʧ�ܽڵ��л�����ʽ��D3��1��ʾͨ��ģ�������л������ڵ㣬D4��1��ʾ����������֪ͨͨ��ģ���л������ڵ�
	u8 RouterRunMode_7;//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
	u8 RouterRunMode_8;//·������ģʽ_�㲥�����ŵ�ִ�з�ʽ��0��ʾִ�й㲥�����Ҫ�ŵ���ʶ��1��ʾ�㲥����Ҫ���ݱ����е��ŵ���ʶ��������ͣ�����ȡֵ����
	u16 MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
	u16 MaxTxByte;//���֧�ֱ��ĳ���(���ֶ���)
	u16 MaxFileByte;//�ļ�����֧�ֵ���󵥸����ݰ�����(���ֶ���)
	u16 MaxUpDateTime;//�����������ȴ�ʱ����(���ֶ���)
	u16 MaxNoteNum;//֧�ֵ����ӽڵ���(���ֶ���)
	u8 RadioChannel;//�����ŵ���:0��63��64��253��������254���Զ�ѡ��255�����ֲ���
	u8 RadioPower;//�������ڵ㷢�书��:00����߷��书�ʣ�01���θ߷��书�ʣ�02���εͷ��书�ʣ�03����ͷ��书�ʣ�04��254��������255�����ֲ���
	
	u8 res7[52];
#ifndef LINUX_CROSS_GCC
}Terminal_Router_TypeDef;
#else
}__attribute__ ((packed)) Terminal_Router_TypeDef;
#endif
#define LEN_Router     128//·�������������ݳ���
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

#define ADDR_NoteList     (ADDR_Router+LEN_Router)//·�����нڵ��
typedef __packed struct
{
	u8 Addr[6];//�ӽڵ��ַ(�ֶ���)
	u8 CollectorAddr[6];//�ɼ�����ַ
#ifndef LINUX_CROSS_GCC
}Note_TypeDef;
#else
}__attribute__ ((packed)) Note_TypeDef;
#endif
#define LEN_NoteList     12//ÿ�б��ֽ���

typedef __packed struct
{
  Note_TypeDef     List[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}NoteList_TypeDef;
#else
}__attribute__ ((packed)) NoteList_TypeDef;
#endif
#define NoteList     ((NoteList_TypeDef *)(ADDR_NoteList))//�ز�·�����нڵ��(����·�ɺ����˳������)


#define ADDR_RMList     (ADDR_NoteList+(LEN_NoteList*NUM_RMmax))//��������б�(��ǰ:������������Ŵ�0��ʼ)
typedef __packed struct
{
 u16 RMAddr[3];//原地址
 u8 CONNECT;//DL698连接:0=�?1=启动连接�?=�?00=连接,101=抄表�?102=关闭连接
  u8 RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997�?），DL/T645-2007�?），DL/T698.45�?），CJ/T 188-2004�?�?
 u8 RMCSDCount;//抄表CSD计数
 u8 RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表�?
 u8 RMCSDSubCount;//抄表CSD子计�?非DL698抄表�?
 u8 RMCSDCountMax;//抄表CSD最大个�?
 u8 RMROADCountMax;//抄表CSD的ROAD内有关联的OAD最大个�?非DL698抄表规约库设�?
 u8 RMCSDSubCountMax;//抄表CSD有子抄表个数(非DL698抄表规约库设�?
 u8 RMROADCountOK;//抄表写文件数据缓冲中ROAD时已填入关联的OAD数据个数计数
 u8 TASK;//并发任务:0=空闲,1=完成1帧发�?2=接收超时(或错�?,3=启动发�?4=正在发�?5=正在接收,6=抄表发�?同RS485�?=启动发�?,7=停用(为了能执行下个列表号)
 u8 TIMER_S;//并发启动发送后等待接收秒定�?
 u8 TxSEQ;//并发启动发送帧序号
 u8 RM_OKFAIL;//抄表成功失败标志;=0失败�?1成功,2=成功但无数据
 u8 RM_DI_CHOICE;//抄表DI选择:0=主DI,1=备用DI
 u32 RM_DI;//DL/T645-2007/1997当前抄表数据标识
 u16 RM_DI_TI_Count;//按时标间隔采集时的间隔计�?
 u16 RM_SubsequentFrames;//0=无读后续�?1-n=读续帧的帧号
  u8 RM_Rx_date_time_s[8];//抄表接收到的数据时标(无数据类�?,7byte,仅为对齐定义8byte
  u8 RM_Rx_date_40020200[10];//抄表接收到的数据事件次暂存及用于MAC验证的电能表表号;0th=状�?0=�?1=正在�?2=完成),1th=表号长度(<=6),2th-7th表号�?
 u8 RMOADROAD;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
 u8 Rand[16];//抄表安全模式时发送给电表�?6字节随机�?
 u8 RMCSD[LEN_RMCSD];//128-x byte 当前正在抄的CSD(有数据类�?
 u8 FnDataBuff[LEN_UART4FnDataBuff];//Fn数据缓冲
 #if (USER/100)==17//江苏�?
 u16 collectoraddr[4];
 u8 PIID;
 u8 res23;
 u16 res24;
 #endif
#ifndef LINUX_CROSS_GCC
}RMCount_TypeDef;
#else
}__attribute__ ((packed)) RMCount_TypeDef;
#endif
#if (USER/100)!=17//不是江苏�?
#define LEN_RMLIST     (63+LEN_RMCSD+LEN_UART4FnDataBuff)//每列表字节数
#else
#define LEN_RMLIST     (63+LEN_RMCSD+LEN_UART4FnDataBuff+12)//每列表字节数
#endif

typedef __packed struct
{
  RMCount_TypeDef     List[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}RMList_TypeDef;
#else
}__attribute__ ((packed)) RMList_TypeDef;
#endif
#define RMLIST     ((RMList_TypeDef *)(ADDR_RMList))//�ز�·�����нڵ��(����·�ɺ����˳������)


//ϵͳ����
typedef __packed struct
{
#define ADDR_Terminal_ERAM_Start     ((ADDR_RMList+(LEN_RMLIST*NUM_RMLISTmax))&0xfffffffc)//ERAM����ֶ���//ERAM���ֶ���
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
	u16 RequestFileTimer;//1 �����ļ�����MS��ʱ��,ÿ10MS��1��ʱ��
  	u16 BUZZMSTimer;//2 ������ÿ10MS��1��ʱ��
	u16 Trip1_MS_Timer;//3 �ն���բ1 10MS��ʱ��;1200ms,>=1000��,<1000��
	u16 Trip2_MS_Timer;//4 �ն���բ2 10MS��ʱ��;1200ms,>=1000��,<1000��
	u16 Trip3_MS_Timer;//5 �ն���բ3 10MS��ʱ��;1200ms,>=1000��,<1000��;΢��������ѧϰ��ɶ�·��״̬��ʱ��
	u16 Trip4_MS_Timer;//6 �ն���բ4 10MS��ʱ��;1200ms,>=1000��,<1000��;������ʱ��ʱ�ȴ��������Уʱ��¼��ʱ��
	u16 AT_MS_Timer;//7 GPRS AT������10MS��ʱ��
	u16 GPRS_RxLED_MS_Timer;//8 GPRS ����LED 10MS��ʱ��
	u16 GPRS_TxLED_MS_Timer;//9 GPRS ����LED 10MS��ʱ��
	u16 RS485_1_TxLED_MS_Timer;//10 RS485_1������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_1_RxLED_MS_Timer;//11 RS485_1�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_2_TxLED_MS_Timer;//12 RS485_2������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 RS485_2_RxLED_MS_Timer;//13 RS485_2�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 ENETLED_MS_Timer;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	u16 GPRS_ONOFF_MS_Timer;//15 GPRS ONOFF�ſ��� 10MS��ʱ��
	u16 RouterCtrl_MS_Timer;//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
	//u16 CD485TxLED_MS_Timer;// ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	//u16 CD485RxLED_MS_Timer;// ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	//u16 DisplayScanTimer;// ��ʾɨ�� ÿ10MS��1��ʱ��
	
#define Terminal_MS_Timer_Num     16//�ն�10MS��ʱ����
#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s��ʱ��
	u16 S_PR;//1 60���Ƶ������
	u16 PORT0_S_Timer;//2 �붨ʱ��
	u16 PORT1_S_Timer;//3 �붨ʱ��
	u16 PORT2_S_Timer;//4 �붨ʱ��
	u16 PORT3_S_Timer;//5 �붨ʱ��
	u16 PORT4_S_Timer;//6 �붨ʱ��
	u16 PORT5_S_Timer;//7 �붨ʱ��
	u16 PORT6_S_Timer;//8 �붨ʱ��
	u16 PORT7_S_Timer;//9 �붨ʱ��
	u16 PORT8_S_Timer;//10 �붨ʱ��
	u16 PORT9_S_Timer;//11 �붨ʱ��
	u16 PORT10_S_Timer;//12 �붨ʱ��
	u16 PORT11_S_Timer;//13 �붨ʱ��
	u16 PORT12_S_Timer;//14 �붨ʱ��
	u16 PORT13_S_Timer;//15 �붨ʱ��
	u16 PORT14_S_Timer;//16 �붨ʱ��
	u16 PORT15_S_Timer;//17 �붨ʱ��
	u16 PORT16_S_Timer;//18 �붨ʱ��

  u16 ENETHeartBeat_S_Timer;//19 ��̫�������붨ʱ��
	u16 IPLeaseTimer;//20 ��̫���Զ����IP����ʱ���붨ʱ��
	u16 ENETCheck_S_Timer;//21 ��̫��IP��ͻ�������޸����붨ʱ��
  u16 GPRSHeartBeat_S_Timer;//22 GPRS�����붨ʱ��
	u16 GPRSAutoDiscon_S_Timer;//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
	u16 GPRSReConnection_S_Timer;//24 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
	u16 GPRSWaitConnection_S_Timer;//25 GPRS�������ӷ���ȴ��붨ʱ��
	u16 GPRSWaitTx_S_Timer;//26 GPRS�������ݵȴ���ʱ�붨ʱ��
	u16 GPRSCheck_S_Timer;//27 GPRS���ߺ��źż���붨ʱ��
	u16 ResetCommand_S_Timer;//28 �ն��յ���λ������ʱִ���붨ʱ��
	u16 GPRS_WaitHostACK_S_Timer;//29 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
	u16 ENET_WaitHostACK_S_Timer;//30 ENET�����ϱ��ȴ���վȷ���붨ʱ��
	u16 RS232_WaitHostACK_S_Timer;//31 RS232�����ϱ��ȴ���վȷ���붨ʱ��
	
	u16 Cascading_LoopChack_S_Timer;//32 ����Ѳ�������붨ʱ��
	u16 SlipCYC_S_Timer;//33 �ն��������幦�ʼ�����������廬���붨ʱ��
	u16 DemandCYC_S_Timer;//34 �ն������������������붨ʱ��
	u16 RouterCheck_S_Timer;//35 �ز�·��������붨ʱ��
	u16 RouterTASK_S_Timer;//36 �ز�·�����������ͺ󵽽��ճ�ʱʱ����TASK()��������붨ʱ��
	u16 res21;//37
	u16 res22;//38 
	u16 AutoSearch_S_Timer;//39 �ز��Զ��ѱ��붨ʱ��
	u16 RouterSynDelay_S_Timer;//40 ·�ɵ���ͬ����ʱִ���붨ʱ��
	u16 Power_S_Timer;//41 ͣ���¼��붨ʱ��
	u16 res23;//42
//  u16 DisplayLoopTimer;// ѭ���붨ʱ��
//  u16 BackLightTimer;// �������ʾ�붨ʱ��

#define BASE_S_Timer_Num     42//�붨ʱ����

#if (((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
	u16 ReTrip_S_Timer;//1 �ն˲���բ�붨ʱ��
	u16 EnSwitchON_S_Timer;//2 �ն˺�բ����֪ͨ�붨ʱ��
	u16 Remote1_S_Timer;//3 ң����բ�ִ�1������ʱ�붨ʱ��
	u16 Remote2_S_Timer;//4 ң����բ�ִ�2������ʱ�붨ʱ��
	u16 Remote3_S_Timer;//5 ң����բ�ִ�3������ʱ�붨ʱ��
	u16 Remote4_S_Timer;//6 ң����բ�ִ�4������ʱ�붨ʱ��
	u16 Remote5_S_Timer;//7 ң����բ�ִ�5������ʱ�붨ʱ��
	u16 Remote6_S_Timer;//8 ң����բ�ִ�6������ʱ�붨ʱ��
	u16 Remote7_S_Timer;//9 ң����բ�ִ�7������ʱ�붨ʱ��
	u16 Remote8_S_Timer;//10 ң����բ�ִ�8������ʱ�붨ʱ��
	u16 PowerCtrl1_S_Timer;//11 �ն��ܼ���1���ع��ʼ����붨ʱ��
	u16 PowerCtrl2_S_Timer;//12 �ն��ܼ���2���ع��ʼ����붨ʱ��
	u16 PowerCtrl3_S_Timer;//13 �ն��ܼ���3���ع��ʼ����붨ʱ��
	u16 PowerCtrl4_S_Timer;//14 �ն��ܼ���4���ع��ʼ����붨ʱ��
	u16 PowerCtrl5_S_Timer;//15 �ն��ܼ���5���ع��ʼ����붨ʱ��
	u16 PowerCtrl6_S_Timer;//16 �ն��ܼ���6���ع��ʼ����붨ʱ��
	u16 PowerCtrl7_S_Timer;//17 �ն��ܼ���7���ع��ʼ����붨ʱ��
	u16 PowerCtrl8_S_Timer;//18 �ն��ܼ���8���ع��ʼ����붨ʱ��
	u16 PowerAttr1_S_Timer;//19 �ն��ܼ���1���ر����붨ʱ��
	u16 PowerAttr2_S_Timer;//20 �ն��ܼ���2���ر����붨ʱ��
	u16 PowerAttr3_S_Timer;//21 �ն��ܼ���3���ر����붨ʱ��
	u16 PowerAttr4_S_Timer;//22 �ն��ܼ���4���ر����붨ʱ��
	u16 PowerAttr5_S_Timer;//23 �ն��ܼ���5���ر����붨ʱ��
	u16 PowerAttr6_S_Timer;//24 �ն��ܼ���6���ر����붨ʱ��
	u16 PowerAttr7_S_Timer;//25 �ն��ܼ���7���ر����붨ʱ��
	u16 PowerAttr8_S_Timer;//26 �ն��ܼ���8���ر����붨ʱ��
	u16 PowerCtrl1_Congeal_S_Timer;//27 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl2_Congeal_S_Timer;//28 �ն��ܼ���2��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl3_Congeal_S_Timer;//29 �ն��ܼ���3��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl4_Congeal_S_Timer;//30 �ն��ܼ���4��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl5_Congeal_S_Timer;//31 �ն��ܼ���5��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl6_Congeal_S_Timer;//32 �ն��ܼ���6��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl7_Congeal_S_Timer;//33 �ն��ܼ���7��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 PowerCtrl8_Congeal_S_Timer;//34 �ն��ܼ���8��ǰ�����¸��ؿغ󶳽��붨ʱ��
	u16 UpPower_Protect_S_Timer;//35 �ն��ϵ籣���붨ʱ��
	u16 ContrlLoopCheck_S_Timer;//36 ���������·�����Ч�ȴ��붨ʱ��
#define Contrl_S_Timer_Num     36
#else
#define Contrl_S_Timer_Num     0
#endif//#if (((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//�ն��붨ʱ����
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m��ʱ��
	u16 DisconAutoProtect_M_Timer;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
	u16 AutoSearch_M_Timer;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��

#define Terminal_M_Timer_Num     2//�ն˷ֶ�ʱ����
#define ADDR_SwitchIn     ADDR_Terminal_M_Timer+(2*Terminal_M_Timer_Num)//����������
	u8 SwitchIn1;//����������1ԭ״̬
	u8 SwitchInFilter1;//����������1�˲�����
	u8 SwitchIn2;//����������2ԭ״̬
	u8 SwitchInFilter2;//����������2�˲�����
	u8 SwitchIn3;//����������3ԭ״̬
	u8 SwitchInFilter3;//����������3�˲�����
	u8 SwitchIn4;//����������4ԭ״̬
	u8 SwitchInFilter4;//����������4�˲�����
	u8 SwitchIn5;//����������5ԭ״̬
	u8 SwitchInFilter5;//����������5�˲�����
	u8 SwitchIn6;//����������6ԭ״̬
	u8 SwitchInFilter6;//����������6�˲�����
	u8 SwitchIn7;//����������7ԭ״̬
	u8 SwitchInFilter7;//����������7�˲�����
	u8 SwitchIn8;//����������8ԭ״̬
	u8 SwitchInFilter8;//����������8�˲�����
#define ADDR_Other      ADDR_SwitchIn+(2*8)
  u8 CongealFlags;//��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
 	u8 LastCongealTime[5];//�ϴζ���ʱ��ʵʱ��������ʱ�ּĴ�
  u8 Terminal_S_Count;//�ն���Ӽ���,���������붨ʱ��
  u8 Terminal_M_Count;//�ն˷ּӼ���,���������ֶ�ʱ��
	u8 SET6000;//�ɼ��������ñ������ñ�־:0=û����,0x55=����
	u8 SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
	u8 FLASHtoSDRAM;//����FLASHtoSDRAM�ϴν��е�ʵʱ��ʱ(������)
	u8 res4;//SETtime;//ʱ�����ñ�־:0=û����,0xff=����,���ڵ��Ժ�ز��ڻ����Բ���
	u8 res5;
	u8 RoundTripFlags;//�ն��ִ���բλ��־,B0-B7=0ûѭ��,=1��ѭ��
	u8 MODULSEL;//����ģ��ѡ��0=,1=��ΪCDMA MC323,2=GPRS��׼3763,3=ģ���û����
	u8 ENETSEL;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
	u8 Signal;//GPRS�ź�ǿ��0-4
	u8 SignaldBm;//�ź�ǿ��0-31��99(BCD��)
	u8 PowerDownTask;//��Դ�����������
	u8 PowerDownSubTask;//��Դ�������������
	u16 CopyTxByte;//�Ĵ�GPRS�跢���ֽ���(���ֶ���)
  u16 CopyTxCount;//�Ĵ�GPRS�ѷ��ͼ���(���ֶ���)
	u16 ReqTxByte;//�Ĵ�GPRS�������ֽ���(���ֶ���)
	u8 res6;
	u8 res7;
	
	u32 res8;
	u32 res9;
	u32 res10;
	u32 res11;
	u32 res12;

	u32 res13;//�˵�����ʹ����(�ƴӺ�)
	u32 res14;//�˵�����ʹ����(�ƴӺ�)
	u32 res15;//�˵�����ʹ����(�ƴӺ�)
	u32 rse16;//�˵�����ʹ����(�ƴӺ�)
#ifndef LINUX_CROSS_GCC
}Terminal_Ram_TypeDef;
#else
}__attribute__ ((packed)) Terminal_Ram_TypeDef;
#endif
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


#define ADDR_BUFF_UART_TIMER_END     (((ADDR_Other+64)+3)&0xfffffffc)
#if (ADDR_BUFF_UART_TIMER_END-ADDR_ERAM_START)>LEN_ERAM_BUFF_UART_TIMER
	#error
#endif

#define ADDR_DL698_ERAM_START     (ADDR_ERAM_START+LEN_ERAM_BUFF_UART_TIMER)

#define ADDR_MobilePhoneNumber     ADDR_DL698_ERAM_START//ͨ�ſ����ֻ������IMSI����(������ʾ)
//20byte �ֻ�����(ASCII)
//20byte IMSI����(ASCII)
//16�ֽ�����ģ�鱾��IP��ַ,ASCII
//6�ֽ�����ģ�鱾�ض˿ں�,ASCII
//6�ֽ�����ģ���źŷֱ�ֵ,ASCII,��λdB

#define ADDR_4022     ADDR_MobilePhoneNumber+68//�忨״̬��
//����2��=bit-string(SIZE(16))���޵�λ���޻���
#define LENmax_4022     4
#define ADDR_4500_5     ADDR_4022+LENmax_4022//����ͨ��ģ��1;5�汾��Ϣ
//�汾��Ϣ��=structure
//{
//���̴���       visible-string(SIZE (4))��
//����汾��     visible-string(SIZE (4))��
//����汾����   visible-string(SIZE (6))��
//Ӳ���汾��     visible-string(SIZE (4))��
//Ӳ���汾����   visible-string(SIZE (6))��
//������չ��Ϣ   visible-string(SIZE (8))
//}
#define LENmax_4500_5     48
#define ADDR_4500_6     ADDR_4500_5+LENmax_4500_5//����ͨ��ģ��1;6֧�ֹ�Լ�б�
//���ݸ�ʽ array visible-string
#define LENmax_4500_6     16
#define ADDR_4500_7     ADDR_4500_6+LENmax_4500_6//����ͨ��ģ��1;7 SIM����ICCID
//visible-string(SIZE(20))
#define LENmax_4500_7     22
#define ADDR_4500_8     ADDR_4500_7+LENmax_4500_7//����ͨ��ģ��1;8 IMSI
//visible-string(SIZE(20))
#define LENmax_4500_8     22
#define ADDR_4500_9     ADDR_4500_8+LENmax_4500_8//����ͨ��ģ��1;9 �ź�ǿ��
//long����λ��dBm
#define LENmax_4500_9     3
#define ADDR_4500_10     ADDR_4500_9+LENmax_4500_9//����ͨ��ģ��1;10 SIM������
//visible-string(SIZE(16))
#define LENmax_4500_10     18


#define ADDR_6000_SDRAM     (((ADDR_4500_10+LENmax_4500_10)+3)&0xfffffffc)//�ɼ��������ñ�
//��FLASH��COPY
#define ADDR_6012_2_SDRAM     ADDR_6000_SDRAM+LENmax_6000//�������ñ�
//��FLASH��COPY
#define ADDR_6014_SDRAM     ADDR_6012_2_SDRAM+LENmax_6012_2//��ͨ�ɼ�����
//��FLASH��COPY
#define ADDR_6016_SDRAM     ADDR_6014_SDRAM+LENmax_6014//�¼��ɼ�������
//��FLASH��COPY
#define ADDR_601C_SDRAM     ADDR_6016_SDRAM+LENmax_6016///�ϱ�������
//��FLASH��COPY


#define ADDR_6002_10     ADDR_601C_SDRAM+LENmax_601C//�ѱ�״̬
//����10��=enum
//{
//���У�0�����ѱ��У�1�� 
//}
#define LENmax_6002_10     2
#define ADDR_6032     ADDR_6002_10+LENmax_6002_10//�ɼ�״̬��
//һ���ɼ�״̬��=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//ͨ�ŵ�ַ              TSA��
//�м̼���              unsigned��
//�м̵�ַ              TSA��
//�˿�                  OAD��
//���һ�βɼ��ɹ�ʱ��  date_time_s��
//�ɼ�ʧ�ܴ���          unsigned��
//��λ       enum{δ֪��0����A�ࣨ1����B�ࣨ2����C�ࣨ3��}��
//�����쳣   enum{������0����LN���ף�1����������2��}
//}
#define LENper_6032     64
#define NUMmax_6032     NUM_RMmax
#define LENmax_6032     (LENper_6032*NUMmax_6032)
#define ADDR_6034     ADDR_6032+LENmax_6032//�ɼ������ؼ�
//�ɼ������ص�Ԫ��=structure (�ڶ�ͨ��ʱ��д)
//{
//����ID       unsigned
//����ִ��״̬  enum
//{
//	δִ�У�0����
//	ִ���У�1����
//	��ִ�У�2��
//}��
//����ִ�п�ʼʱ��   date_time_s��
//����ִ�н���ʱ��   date_time_s��
//�ɼ�������         long-unsigned��
//�ɼ��ɹ�����       long-unsigned��
//�ѷ��ͱ�������     long-unsigned��
//�ѽ��ձ�������     long-unsigned
//}
//˵�����ɼ��ɹ��������ѷ��ͱ����������ѽ��ձ���������ÿ������ִ�������㡣
#define LENper_6034     34
#define NUMmax_6034     RMTASKmax
#define LENmax_6034     (LENper_6034*NUMmax_6034)


#define ADDR_240x_5     ADDR_6034+LENmax_6034//����ڹ���
//���ʡ�=double-long
#define LENper_240x_5     5
#define LENmax_240x_5     (LENper_240x_5*NUMmax_IMPIN)

#define ADDR_230x_3     ADDR_240x_5+LENmax_240x_5//�ܼ��й�����
//long64 
#define LENper_230x_3     9
#define LENmax_230x_3     (LENper_230x_3*NUMmax_TAG)
#define ADDR_230x_4     ADDR_230x_3+LENmax_230x_3//�ܼ��޹�����
//long64
#define LENper_230x_4     9
#define LENmax_230x_4     (LENper_230x_4*NUMmax_TAG)
#define ADDR_230x_5     ADDR_230x_4+LENmax_230x_4//�ܼӻ���ʱ����ƽ���й�����
//long64
#define LENper_230x_5     9
#define LENmax_230x_5     (LENper_230x_5*NUMmax_TAG)
#define ADDR_230x_6     ADDR_230x_5+LENmax_230x_5//�ܼӻ���ʱ����ƽ���޹�����
//long64
#define LENper_230x_6     9
#define LENmax_230x_6     (LENper_230x_6*NUMmax_TAG)
#define ADDR_230x_12     ADDR_230x_6+LENmax_230x_6//��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
//long64
#define LENper_230x_12     9
#define LENmax_230x_12     (LENper_230x_12*NUMmax_TAG)
#define ADDR_kWCtrl1_FixedValue     ADDR_230x_12+LENmax_230x_12//��ǰ�����¸��ض�ֵ


//�ն˿���
#define ADDR_Terminal_CTRL_ESRAM_START      ADDR_230x_12+LENmax_230x_12//�ն˿���ESRAM��ʼ��ַ

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START//�ն˷���ʱ�κ͹���ʱ��
//1BYTE �ն˵�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ��,��,ƽ,�ȵȷ���),0=��Ч
//1BYTE 
//1BYTE ��ǰ����ʱ�κ�1-8,=0��ʾʱ�κ�>9����
//1BYTE 0=��ǰ����ʱ�κŲ�����,1��2��ǰ����ʱ�κſ���

#define ADDR_Terminal_Total_State     ADDR_Terminal_Period+4//�ն�����բ
//1BYTE λ��־(�ִ�1-8)��ǰ����բ״̬(1��բ,0�����բ)
//1BYTE λ��־(�ִ�1-8)�����բ֪ͨ(����բ״̬��ӦλתΪ��բʱ��λ,֪ͨ��ʱ������0)
//2BYTE ����
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//����ģ�������������
//1BYTE	Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
//1BYTE Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��,b2-b6,b7=�ִμ̵���������ʽ(0=����1=�ڲ��Զ�����ʱ��ƽ)
//1BYTE	�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
//1BYTE �ӿ���ģ������Ķ��߱�־�ֽ�1

#define Terminal_Ctrl_Pn_Len     32//�ն˿���ÿPn���ݳ���
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4//�ն˿���-ң����բ
//�����ִ�1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//7BYTE 
//8BYTE	��բ����ǰ�ܼ��й�����long64����λ��kW������-4��
//8BYTE ��բ��2�����ܼ��й�����long64(��λ��W������-1)��
//8BYTE
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*NUMmax_CR)//�ն˿���-����.��ǰ�����¸���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=���㶨ֵ,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//1BYTE ��ǰ������բ�ִ�B0-B7
//1byte ��ǰ�����¸��ض�ֵ����ϵ��
//4byte 
//8BYTE ���ʶ�ֵlong64����λ��kW������-4��
//8BYTE	��բ����ǰ�ܼ��й�����long64����λ��kW������-4��
//8BYTE ��բ��2�����ܼ��й�����long64(��λ��W������-1)��
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-����.Ӫҵ��ͣ����
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//1byte ��ǰ������բ�ִ�B0-B7
//5byte 
//8BYTE ���ʶ�ֵlong64����λ��kW������-4��
//8BYTE	��բ����ǰ�ܼ��й�����long64����λ��kW������-4��
//8BYTE ��բ��2�����ܼ��й�����long64(��λ��W������-1)��
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-����.���ݹ���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//1byte ��ǰ������բ�ִ�B0-B7
//5byte 
//8BYTE ���ʶ�ֵlong64����λ��kW������-4��
//8BYTE	��բ����ǰ�ܼ��й�����long64����λ��kW������-4��
//8BYTE ��բ��2�����ܼ��й�����long64(��λ��W������-1)��
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-����.ʱ�ι���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//1byte ��ǰ������բ�ִ�B0-B7
//5byte 
//8BYTE ���ʶ�ֵlong64����λ��kW������-4��
//8BYTE	��բ����ǰ�ܼ��й�����long64����λ��kW������-4��
//8BYTE ��բ��2�����ܼ��й�����long64(��λ��W������-1)��
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-���.�µ��
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
//1BYTE ����բ�ִ�B0-B7
//1byte 
//5byte 
//8BYTE �µ�����ֵ long64����λ��kWh�����㣺-4��
//8BYTE ��բ����ʱ�ܼӵ�����  long64����λ��kwh/Ԫ������-4��
//8BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���-���.�����
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
//1BYTE ����բ�ִ�B0-B7
//1byte 
//4byte
//1byte �������0=������,1=�����
//8BYTE ��������ֵ  long64����λ��kWh/Ԫ�����㣺-4��
//8BYTE ��բ����ʱʣ�������/��  long64����λ��kwh/Ԫ������-4��
//8BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*NUMmax_TAG)//�ն˿���ESRAM������ַ


#define ADDR_230x_16     ADDR_Terminal_CTRL_ESRAM_END//�ܼ����������״̬
//�ܼ����������״̬��=structure//�����ڶ�ͨ��ʱ����
//{
//ʱ�οض�ֵ������	   unsigned��
//����ʱ����Ч��־λ  bit-string(SIZE(8))��
//����״̬            PCState��
//���״̬            ECState��
//�����ִ�״̬        TrunState��
//����ִ�״̬        TrunState
//}
//ʱ�οض�ֵ�����ţ���ʾ��Ͷ��Ĺ��ض�ֵ�����š�
//����ʱ����Ч��־λ��bit0��bit7��˳���λ��ʾ1��8ʱ�ο�Ͷ�����Чʱ�Σ��á�1������Ч���á�0������Ч
//PCState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
//bit0��ʱ�ο�
//bit1�����ݿ�
//bit2��Ӫҵ��ͣ��
//bit3����ǰ�����¸���
//bit4��bit7�����á�
//ECState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
//bit0���µ��
//bit1�������
//bit2��bit7������
//TrunState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ1��8�ִο��ص��ܿ�״̬���á�1�����ܿأ��á�0�������ܿء�
#define LENper_230x_16     19
#define LENmax_230x_16     (LENper_230x_16*NUMmax_TAG)
#define ADDR_230x_17     ADDR_230x_16+LENmax_230x_16//�ܼ��鵱ǰ����״̬
//��ǰ����״̬��=structure
//{
//	��ǰ���ض�ֵ             long64����λ��W ���㣺-1����     
//	��ǰ�����¸��ظ���ϵ��   integer����λ��%����
//	������բ���״̬         OutputState��
//	�µ����բ���״̬       OutputState��
//	�������բ���״̬       OutputState��
//	����Խ�޸澯״̬         PCAlarmState��
//	���Խ�޸澯״̬         ECAlarmState
//}
//OutputState��=bit-string(SIZE(8))
//bit0��bit7�ֱ��ʾ�ն�1��8�ִ���բ���״̬���á�1����������բ״̬���á�0����δ������բ״̬��
//PCAlarmState��=bit-string(SIZE(8))
//��˳���λ��ʾ���á�1��������ĳ�ֹ���Խ�޸澯״̬���á�0����δ������Ӧ״̬��
//bit0��ʱ�ο�
//bit1�����ݿ�
//bit2��Ӫҵ��ͣ��
//bit3����ǰ�����¸���
//bit4��bit7�����á�
//ECAlarmState��=bit-string(SIZE(8))
//�á�1��������ĳ�ֵ��Խ�޸澯״̬���á�0����δ������Ӧ״̬��
//bit0���µ��
//bit1�������
//bit2��bit7�����á�
#define LENper_230x_17     28
#define LENmax_230x_17     (LENper_230x_17*NUMmax_TAG)

#define ADDR_8000_3     ADDR_230x_17+LENmax_230x_17//ң��-����3���̵������״̬��ֻ��)
//����3���̵������״̬��ֻ��)��=bit-string(SIZE(8))
//�̵������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ���״̬���á�1������բ״̬���á�0������բ״̬��
#define LENmax_8000_3     3
#define ADDR_8000_4     ADDR_8000_3+LENmax_8000_3//ң��-����4(�澯״̬��ֻ��)
//����4(�澯״̬��ֻ��)��=bit-string(SIZE(8))
//�澯״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң�ظ澯���״̬���á�1�������ڸ澯״̬���á�0����δ���ڸ澯״̬��
#define LENmax_8000_4     3
#define ADDR_8000_5     ADDR_8000_4+LENmax_8000_4//ң��-����4����5������״̬��ֻ��)
//����5������״̬��ֻ��)��=bit-string(SIZE(8))
//�̵�������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ�������״̬���á�1������բ����á�0������բ���
#define LENmax_8000_5     3
#define ADDR_8001_2     ADDR_8000_5+LENmax_8000_5//����-����2������״̬��ֻ����
//����2������״̬��ֻ������=enum{�����0�������磨1�����Զ����磨2��}
#define LENmax_8001_2     2
#define ADDR_8002_2     ADDR_8001_2+LENmax_8001_2//�߷Ѹ澯״̬��ֻ��
//����2���߷Ѹ澯״̬��ֻ������=enum{δ�澯(0)���澯(1)}
#define LENmax_8002_2     2

#define ADDR_8103_4     ADDR_8002_2+LENmax_8002_2//ʱ�ι���-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENper_8103_4     8
#define LENmax_8103_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8103_5     ADDR_8103_4+LENmax_8103_4//ʱ�ι���-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENper_8103_5     7
#define LENmax_8103_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8104_4     ADDR_8103_5+LENmax_8103_5//���ݿ�-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENmax_8104_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8104_5     ADDR_8104_4+LENmax_8104_4//���ݿ�-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENmax_8104_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8105_4     ADDR_8104_5+LENmax_8104_5//Ӫҵ��ͣ��-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENmax_8105_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8105_5     ADDR_8105_4+LENmax_8105_4//Ӫҵ��ͣ��-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENmax_8105_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8106_4     ADDR_8105_5+LENmax_8105_5//��ǰ�����¸���-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENmax_8106_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8106_5     ADDR_8106_4+LENmax_8106_4//��ǰ�����¸���-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENmax_8106_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8107_4     ADDR_8106_5+LENmax_8106_5//�����-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENmax_8107_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8107_5     ADDR_8107_4+LENmax_8107_4//�����-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENmax_8107_5     (2+(LENper_8103_5*NUMmax_TAG))
#define ADDR_8108_4     ADDR_8107_5+LENmax_8107_5//�µ��-�ܼ���������״̬
//�������״̬��=array  һ���ܼ���������״̬
//һ���ܼ���������״̬��=structure
//{
//  �ܼ������    OI��
//  �������״̬  bit-string(SIZE(8))
//}
//���ڱ�ʾn���ܼ���Ŀ������״̬��
//�������״̬��
//bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1���������
#define LENmax_8108_4     (2+(LENper_8103_4*NUMmax_TAG))
#define ADDR_8108_5     ADDR_8108_4+LENmax_8108_4//�µ��-�ܼ���澯���״̬
//Խ�޸澯״̬��=array һ���ܼ���澯���״̬
//һ���ܼ���澯���״̬��=structure
//{
//  �ܼ������    OI��
//  �澯���״̬  enum
//{
//δ�澯��0�����澯��1��
//}
//}
#define LENmax_8108_5     (2+(LENper_8103_5*NUMmax_TAG))


#define ADDR_F100_2     ADDR_8108_5+LENmax_8108_5
//2��ESAM���к�	��static��	octet-string
#define LENmax_F100_2     16
#define ADDR_F100_3     ADDR_F100_2+LENmax_F100_2
//3��ESAM�汾��	��static��	octet-string
#define LENmax_F100_3     16
#define ADDR_F100_4     ADDR_F100_3+LENmax_F100_3
//4���Գ���Կ�汾	��static��	octet-string
#define LENmax_F100_4     16
#define ADDR_F100_5     ADDR_F100_4+LENmax_F100_4
//5���ỰʱЧ����	��static��	double-long-unsigned ��λ������
#define LENmax_F100_5     16
#define ADDR_F100_6     ADDR_F100_5+LENmax_F100_5
//6���ỰʱЧʣ��ʱ��	��dyn.��	double-long-unsigned ��λ������
#define LENmax_F100_6     16
#define ADDR_F100_7     ADDR_F100_6+LENmax_F100_6
//7����ǰ������	��static��
//structure
//{
//����ַӦ��Э�̼����� double-long-unsigned��
//�����ϱ�������       double-long-unsigned��
//Ӧ�ù㲥ͨ�����к�   double-long-unsigned
//}
#define LENmax_F100_7     16
#define ADDR_F100_8     ADDR_F100_7+LENmax_F100_7
//8��֤��汾	��static��
//structure
//{
//�ն�֤��汾  octet-string��
//��վ֤��汾  octet-string
//}
#define LENmax_F100_8     16
#define ADDR_F100_9     ADDR_F100_8+LENmax_F100_8
//9���ն�֤�����к�	��static��	octet-string
#define LENmax_F100_9     16
#define ADDR_F100_10     ADDR_F100_9+LENmax_F100_9
//10���ն�֤��	��static��	octet-string
#define LENmax_F100_10     16
#define ADDR_F100_11     ADDR_F100_10+LENmax_F100_10
//11����վ֤�����к�	��static��	octet-string
#define LENmax_F100_11     16
#define ADDR_F100_12     ADDR_F100_11+LENmax_F100_11
//12. ��վ֤��	��static��	octet-string
//��վ��MAC�·���ͬ��վ֤��һ���·�����4���ֽ�Ϊ mac��
#define LENmax_F100_12     16
#define ADDR_F100_13     ADDR_F100_12+LENmax_F100_12
//13. ESAM��ȫ�洢�����б�	��static��	array
//��Ҫ�洢��ESAM�еĶ���
//ESAM��ȫ�洢�����б��=array OAD
#define LENmax_F100_13     16


#define ADDR_F203_2     ADDR_F100_13+LENmax_F100_13
//����2���豸�����б�ֻ������=array ��������Ԫ
//��������Ԫ��=structure
//{
//״̬ST  unsigned��
//��λCD  unsigned
//}
//״̬ST����0�����֡�״̬��1�����ϡ�״̬��
//��λCD����0����ǰ��ң�Ŵ��ͺ���״̬�仯��1����ǰ��ң�Ŵ��ͺ�������һ��״̬�仯��
#define LENper_F203_2     6
#define NUMmax_F203_2     NUMmax_SWITCHIN
#define LENmax_F203_2     (2+(LENper_F203_2*NUMmax_F203_2))
#define ADDR_F204_2     ADDR_F203_2+LENmax_F203_2
//����2���豸�����б�ֻ������=array ֱ��ģ����
//ֱ��ģ������=double-long
#define LENper_F204_2     3
#define NUMmax_F204_2     NUMmax_DC
#define LENmax_F204_2     (2+(LENper_F203_2*NUMmax_F203_2))
#define ADDR_F205_2     ADDR_F204_2+LENmax_F204_2
//����2���豸�����б�ֻ������=array �̵�����Ԫ(�ڶ�ͨ��ʱ��д)
//�̵�����Ԫ��=structure
//{
//  ������    visible-string��
//��ǰ״̬  enum{δ�����0���������1��}��
//��������  enum{����ʽ��0��������ʽ��1��}��
//����״̬  enum{���루0����δ���루1) }
//}
#define LENper_F205_2     16
#define LENmax_F205_2     (2+(LENper_F205_2*NUMmax_CR))
#define ADDR_F206_2     ADDR_F205_2+LENmax_F205_2//�澯���
//����2���豸�����б�ֻ������=array �澯���(�ڶ�ͨ��ʱ��д)
//�澯�����=enum
//{
//δ�����0���������1��
//}
#define LENper_F206_2     2
#define LENmax_F206_2     (2+(LENper_F206_2*1))
#define ADDR_F207_2     ADDR_F206_2+LENmax_F206_2
//����2���豸�����б�ֻ������=array ���ӹ���
//���ӹ��ܡ�=enum
//{
//     �����������0����
//     ��������  ��1����
//     ʱ��Ͷ��  ��2��
//}
#define LENper_F207_2     2
#define NUMmax_F207_2     1
#define LENmax_F207_2     (2+(LENper_F207_2*NUMmax_F207_2))
#define ADDR_F209_2     ADDR_F207_2+LENmax_F207_2
//����2���豸�����б�ֻ������=array ����ͨ��ģ�鵥Ԫ
//����ͨ��ģ�鵥Ԫ��=structure
//{
//�˿�������   visible-string��
//ͨ�Ų���     COMDCB��
//�汾��Ϣ     VersionInfo
//}
//VersionInfo��=structure
//{
//  ���̴���    visible-string(SIZE(2))��
//  оƬ����    visible-string(SIZE(2))��
//  �汾����     Date��
//  ����汾     long-unsigned
//}
#define LENper_F209_2     64
#define NUMmax_F209_2     1
#define LENmax_F209_2     (2+(LENper_F209_2*NUMmax_F209_2))
#define ADDR_F20A_2     ADDR_F209_2+LENmax_F209_2
//����2���豸�����б�ֻ������=array ��������˿�������
//��������˿���������=visible-string
#define LENper_F20A_2     8
#define NUMmax_F20A_2     NUMmax_IMPIN
#define LENmax_F20A_2     (2+(LENper_F20A_2*NUMmax_F20A_2))
#define ADDR_F20B_2     ADDR_F20A_2+LENmax_F20A_2
//����2���豸�����б�ֻ������=array ����ģ��
//����ģ���=structure
//{
//�˿�������   visible-string��
//ͨ�Ų���     COMDCB
//}
#define LENper_F20B_2     8
#define NUMmax_F20B_2     0
#define LENmax_F20B_2     (2+(LENper_F20B_2*NUMmax_F20B_2))




//�ն�ͳ����ʵʱ����
#define ADDR_213x_REAL     (((ADDR_F20B_2+LENmax_F20B_2)+3)&0xfffffffc)//�������ѹ�ϸ���
//1BYTE 0 �ϴμ���RTC��
//1BYTE 1 �ϵ���ʱ��Ӷ�ʱ
//1BYTE 2 �����
//1BYTE 3 ��ѹ��ƽ������

//4BYTE 4 ��ѹ��ƽ��
//1BYTE 8 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
//1BYTE 9 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������

//4BYTE 10 ��ѹ��ƽ��
//1BYTE 14 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
//1BYTE 15 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������

//4BYTE 16 ��ѹ��ƽ��
//1BYTE 20 ��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
//1BYTE 21 Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
#define LEN_2131_REAL     22

#define ADDR_PowerEvent     ADDR_213x_REAL+LEN_2131_REAL//�ն�ͣ/�ϵ��¼�ԭʼ����
typedef __packed struct
{
	u8 Tstoptime[7];//�ն�ͣ��ʱ��hexYYMDHMS
	u8 Tstarttime[7];//�ն��ϵ�ʱ��hexYYMDHMS
	u8 RTCS;//�ϴμ���ʱRTC����
	u8 task;//����
	u8 bit;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
	u8 TimeErr;//�г����ĵ��ܱ�ͣ�ϵ��¼�δ��������
	u8 res2;
	u8 NUMrm;//ͣ�ϵ��¼��賭��������
	u16 setno[NUMmax_Event3106RM];//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
#ifndef LINUX_CROSS_GCC
}PowerEvent_typedef;
#else
}__attribute__ ((packed)) PowerEvent_typedef;
#endif
#define LEN_PowerEvent     (20+(2*NUMmax_Event3106RM))
#define PowerEvent     ((PowerEvent_typedef*)(ADDR_PowerEvent))

#define ADDR_Event_Sour     ((ADDR_PowerEvent+LEN_PowerEvent+3)&0xfffffffc)//����������,����,ͣ���¼�ԭʼ����(�����������������,��0��ʼ)
typedef __packed struct
{
	u16 RMAddr[3];//���ܱ��ַ
	u8 res;
	u8 Re;//����ʧ��(310F)�����ִ�
	u32 Occur;//�¼������ָ�,bit=0�ָ�=1����;b0=����ʧ��(310F),b1=,...,b7=
	u32 TimeCount;//���ܱ�ͣ��ʱ���ʱ(��)
#ifndef LINUX_CROSS_GCC
}EventSour_typedef;
#else
}__attribute__ ((packed)) EventSour_typedef;
#endif
#define LEN_Event_Sour     16

#define ADDR_Read_Info_Time     (ADDR_Event_Sour+(NUM_RMmax*LEN_Event_Sour))
//7byte YYMDHMS
#define ADDR_AFN10F21     ADDR_Read_Info_Time+7//��·�ɶ�ȡ������������Ϣ
//2byte �ڵ�������
//6byte �ڵ�1��ַ
//5byte �ڵ�1����������Ϣ
//...
//6byte �ڵ�n��ַ
//5byte �ڵ�n����������Ϣ
#define ADDR_AFN10F111     ADDR_AFN10F21+2+(11*NUM_RMmax)//��·�ɶ�ȡ�Ķ�������Ϣ
//������ڵ�������n BIN 1
//���ڵ������ʶ�� BIN 3
//���ڵ����ڵ��ַ BIN 6
//�ھӽڵ�1�����ʶ�� BIN 3
//����
//�ھӽڵ�n�����ʶ�� BIN 3
#define ADDR_AFN10F112     ADDR_AFN10F111+10+(3*256)//��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
//�ڵ������� BIN 2
//�ڵ�1��ַ BIN 6
//�ڵ�1�豸���� BIN 1
//�ڵ�1оƬID��Ϣ BIN 24
//�ڵ�1оƬ����汾��Ϣ BIN 2
//...
//�ڵ�n��ַ BIN 6
//�ڵ�n�豸���� BIN 1
//�ڵ�nоƬID��Ϣ BIN 24
//�ڵ�nоƬ����汾��Ϣ BIN 2

#if (PPP_ENABLE==0)//1ʹ��PPP����,0��ʹ��PPP����
#define ADDR_TEST     		ADDR_AFN10F112+2+(33*NUM_RMmax)//���ܲ���,���Ƿ������
#else
#define SERVER_LINK_ADDR 	((ADDR_AFN10F112+2+(33*NUM_RMmax)+3)&0xfffffffc)
typedef __packed struct
{
 u32 ppp0_subtask;
 s32 ppp0_listen;//
 s32 ppp0_clint;//
 u32 eth0_subtask;
 s32 eth0_listen;//
 s32 eth0_clint;//
#ifndef LINUX_CROSS_GCC
}SERVER_TypeDef;
#else
}__attribute__ ((packed)) SERVER_TypeDef;
#endif
#define SERVER_LINK_LEN     24//
#define Svr_Skt     ((SERVER_TypeDef*)(SERVER_LINK_ADDR))

//#define ADDR_UART10IntRx     ADDR_UART10
//#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
//#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
//#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define  ADDR_PPP0_IP4ADDR_ASCII   SERVER_LINK_ADDR+SERVER_LINK_LEN
//"192.168.247.236"
#define  ADDR_TEST     	       ADDR_PPP0_IP4ADDR_ASCII+20//���ܲ���,���Ƿ������
#endif

#if (USER/100)==17//����ϵ
#define ADDR_NoteList1     (((ADDR_TEST+8+3)+3)&0xfffffffc)//·�����нڵ��
typedef __packed struct
{
	u8 Addr[6];//�ӽڵ��ַ(�ֶ���)
	u8 CollectorAddr[6];//�ɼ�����ַ
	u16 user;
#ifndef LINUX_CROSS_GCC
}Note_TypeDef1;
#else
}__attribute__ ((packed)) Note_TypeDef1;
#endif
#define LEN_NoteList1     14//ÿ�б��ֽ���

typedef __packed struct
{
  Note_TypeDef1     List1[NUM_RMmax];
#ifndef LINUX_CROSS_GCC
}NoteList_TypeDef1;
#else
}__attribute__ ((packed)) NoteList_TypeDef1;
#endif
#define NoteList1     ((NoteList_TypeDef1 *)(ADDR_NoteList1))//�ز�·�����нڵ��(����·�ɺ����˳������)

#define ADDR_AFN10F31     (((ADDR_NoteList1+(LEN_NoteList1*NUM_RMmax))+3)&0xfffffffc)//��λ��Ϣ  ((ADDR_NoteList1+(LEN_NoteList1*NUM_RMmax))&0xfffffffc)

#define ADDR_BatteryEvent 	ADDR_AFN10F31+2+(11*NUM_RMmax)//����豸���

#define ADDR_NetTopFlag 	ADDR_BatteryEvent+4//�������˿���flag

#define ADDR_AFN10F104     	ADDR_NetTopFlag+1//��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
//�ڵ������� BIN 2
//�ڵ�1оƬ����汾���� BIN 15
//...
//�ڵ�nоƬ����汾���� BIN 15

#define ADDR_AFN10F112_old     (ADDR_AFN10F104 + 2 + (15 * NUM_RMmax))//��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
//�ڵ������� BIN 2
//�ڵ�1��ַ BIN 6
//�ڵ�1�豸���� BIN 1
//�ڵ�1оƬID��Ϣ BIN 24
//�ڵ�1оƬ����汾��Ϣ BIN 2
//...
//�ڵ�n��ַ BIN 6
//�ڵ�n�豸���� BIN 1
//�ڵ�nоƬID��Ϣ BIN 24
//�ڵ�nоƬ����汾��Ϣ BIN 2

#define ADDR_AFN10F104_old 			(ADDR_AFN10F112_old + 2 + (33 * NUM_RMmax))
//�ڵ������� BIN 2
//�ڵ�1оƬ����汾���� BIN 15
//...
//�ڵ�nоƬ����汾���� BIN 15

#define ADDR_AFN3F12			(ADDR_AFN10F104_old + 2 + (15 * NUM_RMmax))
//ģ�鳧�̴��� ASCII 2
//ģ�� ID ���� BIN 1
//ģ�� ID ��ʽ BIN 1
//ģ�� ID ��Ӧģ�� ID ��ʽ M ���50

#define ADDR_AFN10F7     	(ADDR_AFN3F12 + 54) //��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
//�ڵ������� BIN					 2

//�ӽڵ�1��ַ	BCD				 6 
//�ӽڵ�1�ڵ�����BS				 1 
//�ӽڵ�1ģ�鳧�̴���ASCII 			 2 
//�ӽڵ�1�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�1�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�1�ڵ�ģ��ID��				 50
//...
//�ӽڵ�N��ַ	BCD 			 6 
//�ӽڵ�N�ڵ�����BS				 1 
//�ӽڵ�Nģ�鳧�̴���ASCII 			 2 
//�ӽڵ�N�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�N�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�N�ڵ�ģ��ID��				 50

#define ADDR_AFN10F7_old    (ADDR_AFN10F7 + 2 + (61 * NUM_RMmax)) //��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
//�ڵ������� BIN					 2

//�ӽڵ�1��ַ	BCD				 6 
//�ӽڵ�1�ڵ�����BS				 1 
//�ӽڵ�1ģ�鳧�̴���ASCII 			 2 
//�ӽڵ�1�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�1�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�1�ڵ�ģ��ID��				 50
//...
//�ӽڵ�N��ַ	BCD 			 6 
//�ӽڵ�N�ڵ�����BS				 1 
//�ӽڵ�Nģ�鳧�̴���ASCII 			 2 
//�ӽڵ�N�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�N�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�N�ڵ�ģ��ID��				 50
//#define ADDR_TEST     		(ADDR_AFN10F7 + 2 + (61 * NUM_RMmax))	//���ܲ���,���Ƿ������




#define ADDR_45000C00    	ADDR_AFN10F7_old+2+(33*NUM_RMmax) //��·�ɶ�ȡ�Ŀ���ز�оƬ��Ϣ
// Զ��ģ��ID��Ϣ������15�ֽ�


//JSON��ز���
#define ADDR_JSON	(((ADDR_45000C00+15)+3)&0xfffffffc)
typedef __packed struct
{
	//JSON���������״̬
	u8 RealFileName[84];//ʵʱ���ݵ��ļ���
	u8 PackFrzJsonTask;//����JSON�������
	u8 PackRealJsonTask;//��ǰJSON�������
	u8 PackJsonTask;//�ն��������
	u8 Lastrealtime[7];//���ǰ��ʱ����Ҫ����,
	u8 reportfiletime[7];//�ϱ���ʱ���
	u8 IsCreateFrzJson;//�����ǲ��Ǵ�����ն���,0û��,��0����û�д����
	u8 Is_AllRealData;//�ǲ��Ǵ���ȫ���ĵ������(����û�е�������,ֻ���ڵ�ǰ����ļ�����)
	u8 res;//�ֽڶ��벹��
	u16 CurFrzMetCount;//����������
	u16 CurRealMetCount;//��ǰ�������
	//�ϱ�
	u16 g_Block_Number;//�ļ��ϱ��ܿ�
	u16 g_Need_report_Block;//��ǰ�ļ��ϱ��Ŀ�
	u32 g_Filesize;//��ǰ��Ҫ�ϴ����ļ���С
	u8 Reportjsonfile_Task;//�ϱ��������
	u8 g_ReportFlag;//�Ƿ���Ҫ�ϱ��ı�־	
	u16 res2;//�ֽڶ��벹��
#ifndef LINUX_CROSS_GCC
}ADDR_JSON_typedef;
#else
}__attribute__ ((packed)) ADDR_JSON_typedef;
#endif

#define JSON ((ADDR_JSON_typedef*)(ADDR_JSON))//JSON���ݴ����ز���

//#define ADDR_ERAM_END  (((ADDR_45000C00+15)+3)&0xfffffffc)
#define ADDR_ERAM_END  (((ADDR_JSON+120)+3)&0xfffffffc)

#endif


//#endif
//?�ֽ� ���ܲ���1
//?�ֽ� ���ܲ���2
//...
//?�ֽ� ���ܲ���n

#ifndef ADDR_ERAM_END
	#define ADDR_ERAM_END     ((ADDR_TEST+8+3)&0xfffffffc)
#endif






//ʣ��ERAM�ռ�ȫ�����Զ�����(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=û,1=��(8M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00400000
    #error ADDR_ERAM_END>4//8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00400000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00400000//ʣ��ERAM�ռ䳤��
#endif

#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=û,1=��(16M)
#if NUM_RMLISTmax<NUM_RMmax//�ز�·����������ʱ�õ���󳭱��ܻ������(����������ѳ����������)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>6//16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00A00000//ʣ��ERAM�ռ䳤��
#else
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8//16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00800000//ʣ��ERAM�ռ䳤��
#endif
#endif

#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=û,1=��(32M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>8M//32M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x01800000//ʣ��ERAM�ռ䳤��
#endif

#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=û,1=��(64M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00A00000
    #error ADDR_ERAM_END>10M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00A00000)//��Ϊ8�ֽڶ���
	#ifndef LINUX
	#define LEN_MyHeap     0x02600000//ʣ��ERAM�ռ䳤��
	#else//#ifndef LINUX
	
	#define LEN_MyHeap     (0x02000000-0x00540000-0x00A00000)//ʣ��ERAM�ռ䳤��
	
	#endif
#endif

#if IC_SDRAM==0
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>LEN_Terminal_Ram
    #error ADDR_ERAM_END>LEN_Terminal_Ram
  #endif
	#define ADDR_MyHeap     0
	#define LEN_MyHeap     0//ʣ��ERAM�ռ䳤��
#endif



#endif





