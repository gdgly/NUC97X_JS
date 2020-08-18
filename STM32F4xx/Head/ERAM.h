


#ifndef ERAM_H
#define ERAM_H


	#if IC_SDRAM
#ifdef IC_STM32F4xx
#define ADDR_ERAM_START     0xC0000000
#endif

#ifdef IC_NUC9xx
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
#endif
	#else
#define ADDR_ERAM_START     ADDR_Terminal_Ram_Start
	#endif

#define ADDR_DataSave_SDRAM     ADDR_ERAM_START
//LEN_SaveRam BYTE ��SRAM���Ƶĵ��籣������,�ϵ�ʱ����0�ʷ�����ǰ�Ա��ϵ�ʱ����������0��ַ����
#define ADDR_msFILEmanage     (ADDR_DataSave_SDRAM+(128*1024))
#ifdef  NAND_1G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((128*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#ifdef  NAND_2G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((256*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#ifdef  NAND_4G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((512*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#ifdef  NAND_8G
	#define LEN_msFILEmanage     (128*1024)
	#define LEN_msFILEblock     (32*1024)//msFILE�ļ��鳤��:16K,32K,64K,128K
	#define LEN_msFILEunltFlags     (((1024*1024*1024)/LEN_msFILEblock)*2)//�洢��FLASH�е��ļ��Ĺ���Ԫ�ֽ���
#endif
#if LEN_msFILEunltFlags>=LEN_msFILEmanage
	#error
#endif
/*
typedef __packed struct
{
	u32 ADDR_ENTRY;//�ļ����nand flash��ַ,0=��
	u32 ADDR_END;//�ļ�����nand flash��ַ,0=��
	u32 LEN_FILE;//�ļ���Ч�����ܳ���
	
}msFILE_TypeDef;
*/
typedef __packed struct
{
	u16 BLOCK_ENTRY;//�ļ����nand flash���,0=��
	u16 BLOCK_END;//�ļ�����nand flash���,0=��
	u32 LEN_DATA;//�ļ���Ч�����ܳ���
	u32 LEN_FILE;//�ļ�Ԥ��ռ�д洢�ռ��ܳ���
}msFILE_TypeDef;
#define ADDR_BKPSDRAM     ADDR_msFILEmanage+(2*LEN_msFILEmanage)//128K(BKPSDRAM) NAND FLASHд����(�������ݼ�����ʱ��0)
typedef __packed struct
{
	u32 ADDR_Dest;//4BYTE д��NAND FLASH��Ŀ�ĵ�ַ
	u32 LEN_Data;//4BYTE �����ֽ���
	u32 LEN_Write;//4BYTE �Ѳ���д����ֽ���
	u8 Data[1];//nBYTE ����
}BKPSDRAM_BLOCK_TypeDef;
typedef __packed struct
{
	u16 NUM_BLOCK;//2BYTE �ܿ���
	u32 LEN_TOTAL;//4BYTE ���ֽ���(ȫ���ֽ�,Ҳ���¿�Ŀ�ʼ��ַƫ��)
	//���ݿ�
}BKPSDRAM_TypeDef;
#define ADDR_SDRAM_CLR_START     (ADDR_BKPSDRAM+(128*1024))//SDRAM������0��ʼ��ַ

#define ADDR_RAMmsFILEmanage     ADDR_SDRAM_CLR_START//�洢��RAM�е��ļ�����
#define LEN_RAMmsFILEmanage     32768
#define LEN_RAMmsFILEblock     (32*1024)//RAMmsFILE�ļ��鳤��

#define ADDR_emWin     ADDR_RAMmsFILEmanage+LEN_RAMmsFILEmanage
//128K emWin������
#define ADDR_128KDATABUFF     (ADDR_emWin+(128*1024))
//128K DATABUFF
#define LEN_128KDATABUFF     (128*1024)
#define ADDR_128KWRITE_BUFF     (ADDR_128KDATABUFF+LEN_128KDATABUFF)
//128K�ֽ�NAND FLASH��д����
#define ADDR_NAND_PAGEREAD_BUFF     (ADDR_128KWRITE_BUFF+(128*1024))
//2048�ֽ�NAND FLASHҳ������
#define ADDR_ECC_BUFF     (ADDR_NAND_PAGEREAD_BUFF+2048)
//128+128�ֽ�ECC����

#define ADDR_MACADDR_ascii     (ADDR_ECC_BUFF+256)//����MAC��ַ��ascii��
//18BYTE ��ʽ"1E-30-6C-A2-45-5E"
#define ADDR_IP4ADDR_ascii     (ADDR_MACADDR_ascii+18)//����IP4��ַ��ascii��
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


#define ADDR_UARTStart    ((ADDR_IP6SECONDARY_DNS_ascii+40+3)&0xfffffffc)//ERAM���ֶ���//UART
//UART0
#define ADDR_UART0     (ADDR_UARTStart)
#define ADDR_UART0IntRx     (ADDR_UART0)
#define ADDR_UART0Ctrl     (ADDR_UART0IntRx+LEN_UART0IntRx)
#define ADDR_UART0Rx     (ADDR_UART0Ctrl+LEN_UART0Ctrl)
#define ADDR_UART0Tx     (ADDR_UART0Rx+LEN_UART0Rx)
#define ADDR_UART0FnDataBuff     (ADDR_UART0Tx+LEN_UART0TxMax)
//UART1
#define ADDR_UART1     (ADDR_UART0FnDataBuff+LEN_UART0FnDataBuff)
#define ADDR_UART1IntATRx     (ADDR_UART1)
#define ADDR_UART1IntRx     (ADDR_UART1IntATRx+LEN_UART1IntATRx)
#define ADDR_UART1Ctrl     (ADDR_UART1IntRx+LEN_UART1IntRx)
#define ADDR_UART1Rx     (ADDR_UART1Ctrl+LEN_UART1Ctrl)
#define ADDR_ATTx     (ADDR_UART1Rx+LEN_UART1Rx)
#define ADDR_UART1Tx     (ADDR_ATTx+LEN_ATTx)
#define ADDR_UART1FnDataBuff     (ADDR_UART1Tx+LEN_UART1TxMax)
//UART2
#if IC_SDRAM
#define ADDR_UART2     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#else
#define ADDR_UART2     0x20002000//(Ethernetר��RAM��)
#endif
#define ADDR_UART2IntRx     (ADDR_UART2)
#define ADDR_UART2Ctrl     (ADDR_UART2IntRx+LEN_UART2IntRx)
#define ADDR_UART2Rx     (ADDR_UART2Ctrl+LEN_UART2Ctrl)
#define ADDR_UART2Tx     (ADDR_UART2Rx+LEN_UART2Rx)
#define ADDR_UART2FnDataBuff     (ADDR_UART2Tx+LEN_UART2TxMax)
//UART3
#define ADDR_UART3     (ADDR_UART2FnDataBuff+LEN_UART2FnDataBuff)
#define ADDR_UART3IntRx     (ADDR_UART3)
#define ADDR_UART3Ctrl     (ADDR_UART3IntRx+LEN_UART3IntRx)
#define ADDR_UART3Rx     (ADDR_UART3Ctrl+LEN_UART3Ctrl)
#define ADDR_UART3Tx     (ADDR_UART3Rx+LEN_UART3Rx)
#define ADDR_UART3FnDataBuff     (ADDR_UART3Tx+LEN_UART3TxMax)

#if !IC_SDRAM
#if ((ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)-ADDR_UART2)>(7*1024)
	#error ((ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)-ADDR_UART2)>(7*1024)
#endif
#endif

//UART4
#if IC_SDRAM
#define ADDR_UART4     (ADDR_UART3FnDataBuff+LEN_UART3FnDataBuff)
#else
#define ADDR_UART4     (ADDR_UART1FnDataBuff+LEN_UART1FnDataBuff)
#endif
#define ADDR_UART4IntRx     (ADDR_UART4)
#define ADDR_UART4Ctrl     (ADDR_UART4IntRx+LEN_UART4IntRx)
#define ADDR_UART4Rx     (ADDR_UART4Ctrl+LEN_UART4Ctrl)
#define ADDR_UART4Tx     (ADDR_UART4Rx+LEN_UART4Rx)
#define ADDR_UART4FnDataBuff     (ADDR_UART4Tx+LEN_UART4TxMax)
//UART5
#define ADDR_UART5     (ADDR_UART4FnDataBuff+LEN_UART4FnDataBuff)
#define ADDR_UART5IntRx     (ADDR_UART5)
#define ADDR_UART5Ctrl     (ADDR_UART5IntRx+LEN_UART5IntRx)
#define ADDR_UART5Rx     (ADDR_UART5Ctrl+LEN_UART5Ctrl)
#define ADDR_UART5Tx     (ADDR_UART5Rx+LEN_UART5Rx)
#define ADDR_UART5FnDataBuff     (ADDR_UART5Tx+LEN_UART5TxMax)
//UART6
#define ADDR_UART6     (ADDR_UART5FnDataBuff+LEN_UART5FnDataBuff)
#define ADDR_UART6IntRx     (ADDR_UART6)
#define ADDR_UART6Ctrl     (ADDR_UART6IntRx+LEN_UART6IntRx)
#define ADDR_UART6Rx     (ADDR_UART6Ctrl+LEN_UART6Ctrl)
#define ADDR_UART6Tx     (ADDR_UART6Rx+LEN_UART6Rx)
#define ADDR_UART6FnDataBuff     (ADDR_UART6Tx+LEN_UART6TxMax)
//UART7(�ն�ESAM)
#define ADDR_UART7     (ADDR_UART6FnDataBuff+LEN_UART6FnDataBuff)
#define ADDR_UART7IntRx     (ADDR_UART7)
#define ADDR_UART7Ctrl     (ADDR_UART7IntRx+LEN_UART7IntRx)
#define ADDR_UART7Rx     (ADDR_UART7Ctrl+LEN_UART7Ctrl)
#define ADDR_UART7Tx     ADDR_UART7Rx
#define ADDR_UART7FnDataBuff     (ADDR_UART7Tx+LEN_UART7TxMax)
//UART8(���ESAM)
#define ADDR_UART8     (ADDR_UART7FnDataBuff+LEN_UART7FnDataBuff)
#define ADDR_UART8IntRx     (ADDR_UART8)
#define ADDR_UART8Ctrl     (ADDR_UART8IntRx+LEN_UART8IntRx)
#define ADDR_UART8Rx     (ADDR_UART8Ctrl+LEN_UART8Ctrl)
#define ADDR_UART8Tx     (ADDR_UART8Rx+LEN_UART8Rx)
#define ADDR_UART8FnDataBuff     (ADDR_UART8Tx+LEN_UART8TxMax)
//UART9(���CARD)
#define ADDR_UART9     (ADDR_UART8FnDataBuff+LEN_UART8FnDataBuff)
#define ADDR_UART9IntRx     (ADDR_UART9)
#define ADDR_UART9Ctrl     (ADDR_UART9IntRx+LEN_UART9IntRx)
#define ADDR_UART9Rx     (ADDR_UART9Ctrl+LEN_UART9Ctrl)
#define ADDR_UART9Tx     (ADDR_UART9Rx+LEN_UART9Rx)
#define ADDR_UART9FnDataBuff     (ADDR_UART9Tx+LEN_UART9TxMax)
//UART10(��̫��)
#define ADDR_UART10     (ADDR_UART9FnDataBuff+LEN_UART9FnDataBuff)
#define ADDR_UART10IntRx     (ADDR_UART10)
#define ADDR_UART10Ctrl     (ADDR_UART10IntRx+LEN_UART10IntRx)
#define ADDR_UART10Rx     (ADDR_UART10Ctrl+LEN_UART10Ctrl)
#define ADDR_UART10Tx     (ADDR_UART10Rx+LEN_UART10Rx)
#define ADDR_UART10FnDataBuff     (ADDR_UART10Tx+LEN_UART10TxMax)
//UART11(SMS)
#define ADDR_UART11     (ADDR_UART10FnDataBuff+LEN_UART10FnDataBuff)
#define ADDR_UART11IntRx     (ADDR_UART11)
#define ADDR_UART11Ctrl     (ADDR_UART11IntRx+LEN_UART11IntRx)
#define ADDR_UART11Rx     (ADDR_UART11Ctrl+LEN_UART11Ctrl)
#define ADDR_UART11Tx     (ADDR_UART11Rx+LEN_UART11Rx)
#define ADDR_UART11FnDataBuff     (ADDR_UART11Tx+LEN_UART11TxMax)
//UART12(�����)
#define ADDR_UART12     (ADDR_UART11FnDataBuff+LEN_UART11FnDataBuff)
#define ADDR_UART12IntRx     (ADDR_UART12)
#define ADDR_UART12Ctrl     (ADDR_UART12IntRx+LEN_UART12IntRx)
#define ADDR_UART12Rx     (ADDR_UART12Ctrl+LEN_UART12Ctrl)
#define ADDR_UART12Tx     (ADDR_UART12Rx+LEN_UART12Rx)
#define ADDR_UART12FnDataBuff     (ADDR_UART12Tx+LEN_UART12TxMax)

//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART13     (ADDR_DATABUFF)
#define ADDR_UART13IntRx     (ADDR_UART13)
#define ADDR_UART13Ctrl     (ADDR_UART13IntRx+LEN_UART13IntRx)
#define ADDR_UART13Rx     (ADDR_UART13Ctrl+LEN_UART13Ctrl)
#define ADDR_UART13Tx     (ADDR_UART13Rx+LEN_UART13Rx)
#define ADDR_UART13FnDataBuff     (ADDR_UART13Tx+LEN_UART13TxMax)
//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART14     (ADDR_DATABUFF)
#define ADDR_UART14IntRx     (ADDR_UART14)
#define ADDR_UART14Ctrl     (ADDR_UART14IntRx+LEN_UART14IntRx)
#define ADDR_UART14Rx     (ADDR_UART14Ctrl+LEN_UART14Ctrl)
#define ADDR_UART14Tx     (ADDR_UART14Rx+LEN_UART14Rx)
#define ADDR_UART14FnDataBuff     (ADDR_UART14Tx+LEN_UART14TxMax)
//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART15     (ADDR_DATABUFF)
#define ADDR_UART15IntRx     (ADDR_UART15)
#define ADDR_UART15Ctrl     (ADDR_UART15IntRx+LEN_UART15IntRx)
#define ADDR_UART15Rx     (ADDR_UART15Ctrl+LEN_UART15Ctrl)
#define ADDR_UART15Tx     (ADDR_UART15Rx+LEN_UART15Rx)
#define ADDR_UART15FnDataBuff     (ADDR_UART15Tx+LEN_UART15TxMax)
//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
#define ADDR_UART16     (ADDR_DATABUFF)
#define ADDR_UART16IntRx     (ADDR_UART16)
#define ADDR_UART16Ctrl     (ADDR_UART16IntRx+LEN_UART16IntRx)
#define ADDR_UART16Rx     (ADDR_UART16Ctrl+LEN_UART16Ctrl)
#define ADDR_UART16Tx     (ADDR_UART16Rx+LEN_UART16Rx)
#define ADDR_UART16FnDataBuff     (ADDR_UART16Tx+LEN_UART16TxMax)


#define ADDR_UARTnCtrl_End     (ADDR_UART12FnDataBuff+LEN_UART12FnDataBuff)
#define ADDR_TESAM_User     ADDR_UARTnCtrl_End
//8BYTE TESAMоƬ���к�(�ߵ��ֽ�ͬTESAM���)
//8BYTE 8�ֽ������8��4(��TESAM�ж�ȡ)(�ߵ��ֽ�ͬTESAM���)
#define ADDR_MESAM_User     ADDR_TESAM_User+16
//8BYTE MESAMоƬ���к�
//8BYTE 8�ֽ������8��4(��MESAM�ж�ȡ)
//8BYTE 8�ֽ�Զ�������֤ʱ��վ�·��ķ�ɢ����
//8BYTE 8�ֽ�Զ�������֤ʱ��վ�·��������
#define ADDR_CARD_User     ADDR_MESAM_User+32
//8BYTE CARD��λ��Ϣ��8�ֽ���ɢ
//8BYTE
#define ADDR_AFN09F9     ADDR_CARD_User+16//Զ��ͨ��ģ��汾��Ϣ
//���̴��� ASCII 4
//ģ���ͺ� ASCII 8
//����汾�� ASCII 4
//����������ڣ������� ����¼A.20 3
//Ӳ���汾�� ASCII 4
//Ӳ���������ڣ������� ����¼A.20 3
//�ӣɣͿ�ICCID ASCII 20



#define ADDR_UARTEnd     (ADDR_AFN09F9+46)
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
  u8 Safety;//��ȫ��֤
  u8 RMProtocol;//����ͨ�Ź�Լ:0=��,1=DL/T645-1997,2=��������װ��,3=...,30=DL/T645-2007,31=խ����ѹ�ز�
  u16 BpsCtrl;//ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
			//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
			//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
			//D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
			//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
  u8 Protocol;//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
  u8 NewBps;//�����ʸ��±�־;��ΪMEASMͨ��ʱΪMESAM�ڲ����ݸ��±�־0=����,����ֵû����
  u8 SubTask;//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
  u8 AddrType;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
	//��16Byte
//�ն�ר�ò���
  u32 SourDI;//ԭʼDA,DT
  u32 DestDI;//�ֽ�����е�DA,DT
	u32 RxTxByte;//ͨ������
	u16 PORT;//�����˿ں�(���ֶ���),ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
	u8 TCPUDP;//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
	u8 LinkCtrl;//���ӿ���0=������,1=����
	u8 LinkCount;//���Ӽ���(2����IP,1�α���IP)
	u8 ReLinkNum;//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
	u8 LinkTask;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	u8 LinkSubTask;//����������
	u8 HeartberatCount;//������������,�յ�ȷ��֡��0
	u8 HostACK;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7-�������������ϱ�
	u8 HostCount;//�����ϱ��ط�����(���������������)
	u8 TimerTaskCount;//�����ϱ���ʱ�������
  u8 RxSEQ;//����֡������SEQ
  u8 TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	
	u16 MeterNo;//485������ܱ�/��������װ�����(���ֶ���)
	u16 MeterNoBackup;//485������ܱ�/��������װ�����(���ֶ���)(���볭��ʵʱ����ʱ����)
	u8 FnEnd;//485����Fn����������־,0=����,1=û����
	u8 FnCount;//485����Fn����
	u8 FnCountBackup;//485����Fn����(���볭��ʵʱ����ʱ����)
	u8 RMCount;//485�����������
	u8 ReRMCount;//485���������ط�����
	u8 RMError;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
	u8 RMComplete;//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
	u8 RMFlag;//0=��������ͣ,1=���������
	u8 CascadingAddrType;//�������ݼ���ַ����0=�޼�������,1=����ַ,2=���ַ,3=�㲥��ַ
	u8 CascadingReTxCount;//�����ط�����(��Ϊ��������Ѳ������ʱ��ֵ��Ϊ���0xff��ֻ1��)
	u8 CascadingBroadcast;//0=��,�������㲥��ַת���Ķ˿ں�(RS485_1PORT,RS485_2PORT,RS485_3PORT)
	u8 ATTask;//AT����:0=����,1=����,2=���ڷ���,3=����
	u8 ATWriteTask;//AT���ݷ�������:0=������,1=������
	u8 ATCount;//AT����
	
	u8 LinkTask2;//ͨ��2��������(FTP����)
	u8 FTPLinkCount;//FTP�������Ӽ���
	u8 Task2;//ͨ��2����(FTP����)
	u8 FTPErr;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
	u32 FTPByte;//FTP��ǰ�ļ�����������ֽ���
	u32 FTPFileLen;//FTP��ǰ�ļ��ܳ���
	u16 DIOffset;//�Ĵ��֡����ʱ��ǰ���ڴ������֡�����ݱ�ʶDI�ĵ�ַƫ��(���ֶ���)
	u16 TxFrameCount;//�Ĵ��֡����ʱ�ѷ���֡����(���ֶ���)
	u16 MulFrameGPCR;//��֡ʱͨ�ü����Ĵ���
	u16 DIlen;//���ݱ�ʶ���ȼĴ���
	//��80Byte
//���г���ר�ò���
	u16 TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
	u8 LockFlags;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7=����
	u8 RTCS;//ʵʱ�Ӹ��±�־ԭʼ��
	u8 RTCM;//ʵʱ�Ӹ��±�־ԭʼ��
	u8 RTCH;//ʵʱ�Ӹ��±�־ԭʼʱ
	u8 DateReRMCount;//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
	u8 BroadcastTime;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	u16 MeterNoBackupOfKeyUser;//485������ܱ�/��������װ�����(���ֶ���)(���볭���ص㻧��������ʱ����)
	u8 FnCountBackupOfKeyUser;//485����Fn����(���볭���ص㻧��������ʱ����)
	u8 RMTaskCOfKeyUser;//�����ص㻧��������ǰԭ����
	u8 RMTask5;//�������ܱ��¼��������:B0=�ȼ�1,B1=�ȼ�2,...,B7=�ȼ�8
	u8 SubsequentFrames;//0=�޶�����֡,1-255=DL645����֡��֡��
	u8 F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
	u8 LINKCHECK_RTCS;//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
	u8 TI[7];//�Ĵ�ȫ��������ʱ���ߵ�ʱ��(������ʱ��),�ܶ�,����
	u8 RMCountErr;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
	//��104Byte
	u8 BatchTask;//ת����������:0=��,1=�ȴ��ڿ�������,2=...����
	u8 BatchTaskLock;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
	u16 BatchMeterCount;//ת���������������
	u32 Random;//��������֤ʱ���ص������4byte
	u8 BatchMeterNo[6];//ת��������ʱ����ӵ���ȡ�ı��	
	u8 AutoSearchTask;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
	u8 AutoSearchReCount;//�Զ��ѱ������ط�����
	u16 TransferDelay;//��·�ϴ�����ʱʱ����
	u8 NextRMTime[6];//�����������´γ���ʱ�����ʱ������
	//��128Byte
	u8 NETOPENCount;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
	u8 RMTaskC;//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=ͣ���¼����,4=ÿСʱ��������,5=,(MaxRMTask-2)=���ڳ������ܱ��¼�,(MaxRMTask-1)=ѭ������(��ÿСʱ����������,ֻ��ѭ������ſɲ�������ȼ�����)
	u8 RMTaskN;//������b0-b7�ֱ��ʾ����0-7��������
	u8 FnCountS;//485����ԭѭ�������Fn����
	u16 MeterNoS;//485����ԭѭ�������������ż���(���ֶ���)
	u8 RTCD[2];//ʵʱ���ո��±�־��ԭʼ����
	u8 RTCW[2];//ʵʱ�����ڸ��±�־��ԭʼ����
	u16 SMSRxSEQ;//���Ž��սӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	u16 SMSTxSEQ;//���ŷ��ͽӿ�;B0-B11����֡����,B12-B14֡�ڶ�����������,B15֡��δ�����ű�־
	u8 NextTimerRMTime[6];//��ʱ��ͣ���¼��������´γ���ʱ�����ʱ������
	u16 MeterNoBackupOfAutoReport;//485������ܱ�/��������װ�����(���ֶ���)(������������ϱ�ǰԭ���泭���װ����żĴ�)
	u8 FnCountBackupOfAutoReport;//485����Fn����(������������ϱ�ǰԭ���泭���Fn�����Ĵ�)
	u8 RMMeterEvent;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
	
	u8 RMEventTaskC;//��ǰ�������ܱ��¼�����:0=��,1-3=1-3��
	u8 RMEventTaskN;//�����ڳ������ܱ��¼�����b0-b2�ֱ�Ϊ�¼��ּ�1-3,;b6=�¸���(�Ϻ�ÿ�³��¼���)
	u8 RMCountBackup;//485�����������(���볭��ʵʱ����ʱ����)
	u8 ReTxSEQ;//�ط�ʱ����֡������SEQ(�ظ�����ʱ֡������SEQ����Ĵ�)
	u32 res0;
	//��160Byte
	u8 RMTaskFlags;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	u8 RMCommandYMDHM[5];//����������ߵ�ʱ��������ʱ��(ȫ0��ʾ��Ч)

//	
	u8 SearchDICount;//�����¼�����AFN04F106������������
	u8 RMDICount;//�����¼������ѳ�DI����
	u8 TotalDICount;//�����¼�����AFN04F106��������������DI����(��SearchDICount=0ʱ����)
	u8 res2;
	u8 res3;
	u8 DHCPTask;//DHCP����
	u32 RMDI;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
	s32 TCPSocket;
	s32 UDPSocket;
	u32 Last_Block_No;//(�Ϻ�ר����)����698��֡��ţ���ʼ����0xffffffff
	u32 Addr_Split_Frame;//(�Ϻ�ר����)��ŷ�֡����ʱ����Ķ�̬�ڴ��ַ��Ϊ0��ʾδ����
	u32 Save_Split_Frame_len;//(�Ϻ�ר����)����Ѿ���ŵķ�֡�ĳ���,��ʼ����0
	u32 res10;
	u8 NET_ADDR[20];//NET_ADDR
#if (USER/100)==14//����ϵ	
	u32 CongealStartTime;//���ն�����ʼʱ��(ͣ����ʼ:������)
	u32 CongealEndTime;//���ն������ʱ��(ͣ�����:������)
	u16 CongealSymbol;//���ն���Fn��־  D0-D7:0CF129-0CF136ת0DF161-0DF168	D8-D16����
	u8  CongealDays;//���ն�������	
	u8 FuJBroadCastTask;//�����㲥����
	u8 FuJBroadCastReCount;//���Դ���
	u8 FuJAutoSearchTask;//�����ӽڵ�ע������
	u8 FuJAutoSearchReCount;//���Դ���
	u8 TaskType;//��������
	u16 TaskID;//�����
	u8 Prio;//�������ȼ�
	u8 res11;//����
#else
	u8 res11[20];
#endif	
	u32 S_num;//�������д�������
	u32 res7;//uart0��uart4�ز���������ִ���쳣������
	u32 res13;
#if (((USER/100)==6)||((USER/100)==12))//���ϣ��Ĵ�
	u8 Jump;//���ϣ��Ĵ��ڶ���ʱ�겻��ȷ������£���Ҫ������� =1 ��ʾ��  =0 ��ʾ����
#else
	u8 res8;
#endif	
	u8 AT_NETINFO;//AT������ ����4G��Ƶ(1���Զ�  5:2G+3G    6:2G+4G)
	u8 AT_NETINFO_NUM;//(���Ӽ���)	
	u8 AT_CIMI_NAME;//��ǰSIM����������Ϣ(1���ƶ�  2����ͨ  3������   4������)
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

//ϵͳ����
typedef __packed struct
{
#define ADDR_Terminal_ERAM_Start     ((ADDR_UARTEnd+3)&0xfffffffc)//ERAM����ֶ���//ERAM���ֶ���
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
	u16 I2SUBTask4_17;//
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
	
#if ((USER/100)==14)//����
	u16 FuJ_GPRSTick_MS_Timer;//17 ����GPRSά�������붨ʱ��
#define Terminal_MS_Timer_Num     17//�ն�10MS��ʱ����
#endif
#ifndef Terminal_MS_Timer_Num
#define Terminal_MS_Timer_Num     16//�ն�10MS��ʱ����
#endif

#define ADDR_Terminal_S_Timer     (ADDR_Terminal_MS_Timer+(2*Terminal_MS_Timer_Num))//s��ʱ��
	u16 S_PR;//1 60���Ƶ������
  u16 ENETHeartBeat_S_Timer;//2 ��̫�������붨ʱ��
	u16 IPLeaseTimer;//3 ��̫���Զ����IP����ʱ���붨ʱ��
	u16 ENETCheck_S_Timer;//4 ��̫��IP��ͻ�������޸����붨ʱ��
  u16 GPRSHeartBeat_S_Timer;//5 GPRS�����붨ʱ��
	u16 GPRSAutoDiscon_S_Timer;//6 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
	u16 GPRSReConnection_S_Timer;//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
	u16 GPRSWaitConnection_S_Timer;//8 GPRS�������ӷ���ȴ��붨ʱ��
	u16 GPRSWaitTx_S_Timer;//9 GPRS�������ݵȴ���ʱ�붨ʱ��
	u16 GPRSCheck_S_Timer;//10 GPRS���ߺ��źż���붨ʱ��
	u16 ResetCommand_S_Timer;//11 �ն��յ���λ������ʱִ���붨ʱ��
	u16 GPRS_WaitHostACK_S_Timer;//12 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
	u16 ENET_WaitHostACK_S_Timer;//13 ENET�����ϱ��ȴ���վȷ���붨ʱ��
	u16 RS232_WaitHostACK_S_Timer;//14 RS232�����ϱ��ȴ���վȷ���붨ʱ��
	u16 RMDelay_S_Timer;//15 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
	u16 ACSampleRM_S_Timer;//16 �ڲ��������������붨ʱ��
	u16 RS4851RM_S_Timer;//17 RS485-1�����붨ʱ��
	u16 RS4852RM_S_Timer;//18 RS485-2�����붨ʱ��
	u16 RS4853RM_S_Timer;//19 RS485-3�����붨ʱ��
	u16 RS4854RM_S_Timer;//20 RS485-4�����붨ʱ��(�ز�)
	u16 RS4851RMDelay_S_Timer;//21 RS485-1�ϵ���ʱ�����붨ʱ��(�����ϳ���ʱ�����ֿ�)
	u16 RS4852RMDelay_S_Timer;//22 RS485-2�ϵ���ʱ�����붨ʱ��
	u16 RS4853RMDelay_S_Timer;//23 RS485-3�ϵ���ʱ�����붨ʱ��
	u16 RS4854RMDelay_S_Timer;//24 RS485-4�ϵ���ʱ�����붨ʱ��(�ز�)
	u16 RS4851RMF246_S_Timer;//25 RS485-1ͣ���¼�����ʱ����ֵ�붨ʱ��(�����ϳ���ʱ�����ֿ�)
	u16 RS4852RMF246_S_Timer;//26 RS485-2ͣ���¼�����ʱ����ֵ�붨ʱ��
	u16 RS4853RMF246_S_Timer;//27 RS485-3ͣ���¼�����ʱ����ֵ�붨ʱ��
	u16 RS4854RMF246_S_Timer;//28 RS485-4ͣ���¼�����ʱ����ֵ�붨ʱ��(�ز�)
	u16 Cascading_LoopChack_S_Timer;//29 ����Ѳ�������붨ʱ��
	u16 SlipCYC_S_Timer;//30 �ն��������幦�ʼ�����������廬���붨ʱ��
	u16 DemandCYC_S_Timer;//31 �ն������������������붨ʱ��
	u16 RouterCheck_S_Timer;//32 �ز�·��������붨ʱ��
	u16 AutoUpdataAFN04F10_S_Timer;//33 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
	u16 AutoUpdataAFN04F10ERC3_S_Timer;//34 �Զ�����F10�����ϱ�ERC3�¼��붨ʱ��;������ͨ�����ص��Զ����֣���ɲ����������������ɾ����Ӧ�����ò�������������־��F150��������ӦλΪ1���������ϱ���������¼���ERC3������վ,���ǽ����˲�������������־�ĸ�λ������ÿ���ϱ���������¼����m���ӣ�m=0~30��һ��ȡ10��֮�ڣ����۲����Ƿ�����������ٴ��ϱ�
	u16 AutoDelAFN04F10_S_Timer;//35 �Զ�ɾ��AFN04F10�ĵ��ܱ������붨ʱ��,���⾭������Ӱ��CPU�ٶ�
	u16 AutoSearch_S_Timer;//36 �ز��Զ��ѱ��붨ʱ��
	u16 RouterSynDelay_S_Timer;//37 ·�ɵ���ͬ����ʱִ���붨ʱ��
	u16 Power_S_Timer;//38 ͣ���¼��붨ʱ��
//  u16 DisplayLoopTimer;// ѭ���붨ʱ��
//  u16 BackLightTimer;// �������ʾ�붨ʱ��

#if ((USER/100)==14)//����
	u16 FuJTask_S_Timer;//39 ������ʱ�����붨ʱ��
	u16 GPRS_Error_S_Timer;//40 GPRS���󷵻ض�ʱ��
	u16 AFN04HF222_Task_S_Timer;//41 ����4������ִ����ͣ�붨ʱ��	  ���������׼�޸�
	u16 AFN04HF222_AFN57_S_Timer;//42 ����4�������ϱ���ͣ�붨ʱ��	  ���������׼�޸�	
#define BASE_S_Timer_Num     42//�붨ʱ����
#endif
#ifndef BASE_S_Timer_Num
#define BASE_S_Timer_Num     38//�붨ʱ����
#endif

#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
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
#endif//#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

	
#define Terminal_S_Timer_Num     (BASE_S_Timer_Num+Contrl_S_Timer_Num)//�ն��붨ʱ����
#define ADDR_Terminal_M_Timer     (ADDR_Terminal_S_Timer+(2*Terminal_S_Timer_Num))//m��ʱ��
	u16 DisconAutoProtect_M_Timer;//1 �Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
	u16 AutoSearchMeterTimer;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��(��ʼ�Ǻ���Ҫ��)
	u16 RS4851RMF246_M_Timer;//3 RS485-1ͣ���¼���������ֶ�ʱ��
	u16 RS4852RMF246_M_Timer;//4 RS485-2ͣ���¼���������ֶ�ʱ��
	u16 RS4853RMF246_M_Timer;//5 RS485-3ͣ���¼���������ֶ�ʱ��
	u16 RS4854RMF246_M_Timer;//6 RS485-4ͣ���¼���������ֶ�ʱ��(�ز�)

#if ((USER/100)==14)//����
	u16 FuJBroadCastTimer;//7 �����㲥����ֶ�ʱ��
	u16 FuJAutoSearchTimer;//8 ��������ע������ֶ�ʱ��
	u16 FuJCollectTaskTimer;//9 �������񷽰���ʱ��
#define Terminal_M_Timer_Num     9//�ն˷ֶ�ʱ����
#endif
#ifndef Terminal_M_Timer_Num 
#define Terminal_M_Timer_Num     6//�ն˷ֶ�ʱ����
#endif

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
	u8 AFN04F10F11F13F35UpDate;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����;����ʵ����ʹ��
	u8 AFN04F150_Computer_RTCS;//��������Ч��־����ԭʵʱ����
	u8 AutoUpdataAFN04F10ERC3;//0=��,0x55=��AFN04F10���µ���10���ڲ����ϱ����
	u8 AFN04F10F35Update;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
	u8 RoundTripFlags;//�ն��ִ���բλ��־,B0-B7=0ûѭ��,=1��ѭ��
	u8 MODULSEL;//����ģ��ѡ��0=,1=��ΪCDMA MC323,2=GPRS��׼3763,3=ģ���û����
	u8 ENETSEL;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
	u8 Signal;//GPRS�ź�ǿ��0-4
	u8 SignaldBm;//�ź�ǿ��0-31��99(BCD��)
	u8 F246PnListUpDate;//Ҫ�����ĵ����¼����Pn�б��Ѹ��±�־,0=Ҫ����,!=0�Ѹ���
	u8 PowerDownTask;//��Դ�����������
	u8 PowerDownSubTask;//��Դ�������������
	u16 CopyTxByte;//�Ĵ�GPRS�跢���ֽ���(���ֶ���)
  u16 CopyTxCount;//�Ĵ�GPRS�ѷ��ͼ���(���ֶ���)
	u16 ReqTxByte;//�Ĵ�GPRS�������ֽ���(���ֶ���)
	u8 PowerNote;//�ϵ��¼���¼:0=û��¼,!=0�Ѽ�¼;
	u8 PowerTask;//ͣ���¼�����
	u32 res13;//�˵�����ʹ����(�ƴӺ�)	
	u32 res4;
	u32 res5;
	u16 res6;
#if (USER/100)==14//����ϵ		4G,SIM7100ģ��
#define ADDR_AFN0CF251     ADDR_Other+42
	u16 PowerState;//��Դ����״̬  2BYTE  Bit5ͣ�糬������(0������1����)   Bit4ʱ�ӵ�� (0������1Ƿѹ)
								//Bit3ͣ�繤�����(0������1����)    Bit2Bit1���緽ʽ(00����Դ��01������Դ��10��ع���)
								
#define ADDR_AFN0CF252     ADDR_AFN0CF251+2								
	u16 BatterCapacity;//ͣ�繤�������Ч����  2BYTE  ���ݸ�ʽ07  0-100
	u16 BatterV;//ͣ�繤����ص�ѹ  2BYTE  ���ݸ�ʽ07  V

#else
	u16 res7;
	u16 rse8;
	u16 rse9;	
#endif
	u32 res10;
	u32 res11;
	u32 res12;
	u16 res3;
//#define ADDR_AFN0CF253    ADDR_Other+42+6
//	u32 Lac;//��վ��  BCD  4BYTE
//	u32 CELL_ID;//С����   BCD  3BYTE
//	u16 ARFCN;//Ƶ���     BCD   2BYTE
//	u16 RxLevel;//�����ź�ǿ��   2BYTE   dBm  BCD�����ݸ�ʽ02��   һ����-10~-140֮��
//	u16 TxPower;//��ǰ���͹���   2BYTE   dBm  BCD�����ݸ�ʽ02��	
	u16 ALIGN_FALG;//������Ա�־  0��δ����   0xffff���������ݶ�����
}Terminal_Ram_TypeDef;
#define Terminal_Ram     ((Terminal_Ram_TypeDef *)(ADDR_Terminal_ERAM_Start))


//��3(�¼�)�����ʱ��
#define ADDR_Class3Data_ESRAM_Start     (((ADDR_Other+64)+3)&0xfffffffc)//ERAM����ֶ���
#define ADDR_ERC10_Timer     ADDR_Class3Data_ESRAM_Start//ERC10����ʧѹ�¼���ʱ�붨ʱ��
#define ERC10_TNum     AFN04F25MaxPn//ERC10����ʧѹ�¼��붨ʱ������
#define ADDR_ERC22_Timer     ADDR_ERC10_Timer+(2*ERC10_TNum)//ERC22�Խ���¼��붨ʱ��
#define ERC22_TNum     MaxDiffGroup//ERC22�Խ���¼��붨ʱ������
#define ADDR_ERC29_Timer     ADDR_ERC22_Timer+(2*ERC22_TNum)//ERC29���ܱ�����붨ʱ��
#define ERC29_TNum     ERC29MaxPn//ERC29���ܱ�����붨ʱ������
#define ADDR_ERC30_Timer     ADDR_ERC29_Timer+(2*ERC29_TNum)//ERC30���ܱ�ͣ���붨ʱ��
#define ERC30_TNum     ERC30MaxPn//ERC30���ܱ�ͣ���붨ʱ������

#define EVENT_S_TNum     (ERC10_TNum+ERC22_TNum+ERC29_TNum+ERC30_TNum)//���붨ʱ������

//��3(�¼�)ԭʼ����
#define ADDR_ERC8_SOUR     ADDR_ERC10_Timer+(2*EVENT_S_TNum)//ERC8���ܱ��������¼�ԭʼ����
//1BYTE B0-B7�ֱ��ʾ��1�׵�1-8��ʱ�α�0=û��1=��ȷ��(�ϵ��ʼ��0xee)
//4BYTE ��1�׵�1-8��ʱ�α����ȷ�����ݵ�CRC(�ϵ��ʼ��0xee)
//1BYTE B0-B7�ֱ��ʾ��2�׵�1-8��ʱ�α�0=û��1=��ȷ��(�ϵ��ʼ��0xee)
//4BYTE ��2�׵�1-8��ʱ�α����ȷ�����ݵ�CRC(�ϵ��ʼ��0xee)
//6BYTE ÿ�µ�1-3������(�ϵ��ʼ��0xee)
//3BYTE ����й�����(�ϵ��ʼ��0xee)
//3BYTE ����޹�����(�ϵ��ʼ��0xee)
//3BYTE �������������(�ϵ��ʼ��0xee)
//3BYTE ��ѹ���������(�ϵ��ʼ��0xee)
//6BYTE ���һ�α�̷���ʱ��(�ϵ��ʼ��0xee)
//3BYTE ��������������(�ϵ��ʼ��0xee)
#define ERC8_SOUR_Pn_LEN     37//ERC8���ܱ��������¼�ԭʼ����ÿPn����
#define ADDR_ERC9_SOUR     ADDR_ERC8_SOUR+(ERC8_SOUR_Pn_LEN*ERC8MaxPn)//ERC9������·�쳣�¼�
//1BYTE B0-B5ÿ2λ�ֱ��ʾA,B,C��,ֵ0-3:0=�ա�1=��·��2=��·��3=����(��ʼ��ֵ0)
//3BYTE A������ܴ���(��ʼ��0xee)
//3BYTE B������ܴ���(��ʼ��0xee)
//3BYTE C������ܴ���(��ʼ��0xee)
#define LEN_ERC9_SOUR_Pn     10//ERC9������·�쳣�¼�ԭʼ����ÿPn����
#define ADDR_ERC10_SOUR     ADDR_ERC9_SOUR+(LEN_ERC9_SOUR_Pn*ERC9MaxPn)//ERC10��ѹ��·�쳣�¼�
//1BYTE 0 B0-B5ÿ2λ�ֱ��ʾA,B,C��ԭ�¼���¼,ֵ0-3:0=�ָ���1=���ࡢ2=ʧѹ��3=(��ʼ��ֵ0)
//1BYTE 1 ˲ʱ����ʧѹ״̬(��ʼ��ֵ0)
//3BYTE 2 ȫʧѹ�ܴ���(��ʼ��0xee)
//3BYTE 5 ʧѹ�ܴ���,DL645-1997��ʱΪ�ܶ������(��ʼ��0xee)
//3BYTE 8 A�������(��ʼ��0xee)
//3BYTE 11 B�������(��ʼ��0xee)
//3BYTE 14 C�������(��ʼ��0xee)
//3BYTE 17 A��ʧѹ����(��ʼ��0xee)
//3BYTE 20 B��ʧѹ����(��ʼ��0xee)
//3BYTE 23 C��ʧѹ����(��ʼ��0xee)
#define LEN_ERC10_SOUR_Pn     26//ERC10��ѹ��·�쳣�¼�ԭʼ����ÿPn����
#define ADDR_ERC11_SOUR     ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*ERC10MaxPn)//ERC11��λ�쳣�¼�
#if (AFN0CF49MaxPn%8)==0x0
#define ERC11_SOUR_LEN     (AFN0CF49MaxPn/8)
#else
#define ERC11_SOUR_LEN     ((AFN0CF49MaxPn/8)+1)
#endif
//ÿ��������1��λ��ʾԭ״̬,0=�ָ�,1=����
#define ADDR_ERC12_SOUR     ADDR_ERC11_SOUR+ERC11_SOUR_LEN//ERC12���ܱ�ʱ�䳬���¼�
#if (AFN0CF27MaxPn%8)==0x0
#define ERC12_SOUR_LEN     (AFN0CF27MaxPn/8)
#else
#define ERC12_SOUR_LEN     ((AFN0CF27MaxPn/8)+1)
#endif
//ÿ��������1��λ��ʾԭ״̬,0=�ָ�,1=����
#define ADDR_ERC13_SOUR     ADDR_ERC12_SOUR+ERC12_SOUR_LEN//ERC13��������Ϣ�¼�ԭʼ����
//3BYTE ���ܱ��̴���(�ϵ��ʼ��0xee)
//3BYTE ��������������(�ϵ��ʼ��0xee)
//1BYTE B0-B2�ֱ��ʾA,B,C������������־
//3BYTE ���ܱ�������,A,B,C���������(�ϵ��ʼ��0xee)
//3BYTE ���ܱ�ʧѹ����(�ϵ��ʼ��0xee)
//3BYTE ���ܱ�ͣ�����(�ϵ��ʼ��0xee)
//1BYTE ���ܱ���Ƿѹ,0=ûǷ,1=Ƿѹ(�ϵ��ʼ��0xee)
#define ERC13_SOUR_Pn_LEN     17
#define ADDR_ERC14SOUR     ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*ERC13MaxPn)//ERC14�¼���¼��־
//1BYTE 0 �ϵ��¼���¼:0=û��¼,!=0�Ѽ�¼;
//1BYTE 1 
//1BYTE 2 ERC
//1BYTE 3 �¼�����
//1BYTE 4 ���Ա�־
//5BYTE 5 �����ͣ��ʱ��
//5BYTE 10 ������ϵ�ʱ��
//1BYTE 15 ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
//6BYTE 16 �����ͣ��ʱ��YMDHMS
//6BYTE 22 ������ϵ�ʱ��YMDHMS
//1byte 28 
#define ADDR_ERC22_TotalEnergy     ADDR_ERC14SOUR+29//ERC22���䶨ʱ�ܼӵ���
//���1
//4BYTE �ܼ���1�ܵ���,���ݸ�ʽ03
//...
//4BYTE �ܼ���N�ܵ���,���ݸ�ʽ03
//���2
//4BYTE �ܼ���1�ܵ���,���ݸ�ʽ03
//...
//4BYTE �ܼ���N�ܵ���,���ݸ�ʽ03
//...
//���N
#define ADDR_ERC22_Remainder     ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*MaxDiffGroup)//ERC22���䶨ʱ�ܼӵ���������С��λ
//�ܼ���1
//4BYTE ��,HEX����4λС���㵥λΪkWh;��ʼ��ʱ��0
//4BYTE ����l,HEX����4λС���㵥λΪkWh;��ʼ��ʱ��0
//...
//4BYTE ����M,HEX����4λС���㵥λΪkWh;��ʼ��ʱ��0
//......
//�ܼ���N
#define ADDR_ERC22_SOUR     ADDR_ERC22_Remainder+(4*(TMaxTariff+1)*MaxTotalAddGroup)//ԭ��鳬�ޱ�־,0=û��,1=����
//1BYTE ԭ��鳬�ޱ�־,0=û��,1=����
//..
//1BYTE
#define ADDR_ERC27_SOUR     ADDR_ERC22_SOUR+MaxDiffGroup//ERC27���ܱ�ʾ���½��¼�
//1BYTE 0=û��¼,!=0�Ѽ�¼
#define LEN_ERC27_SOUR     1
#define ADDR_ERC29_SOUR     ADDR_ERC27_SOUR+(LEN_ERC27_SOUR*ERC27MaxPn)//ERC29���ܱ�����¼�,ERC28���ܱ����¼�
//5BYTE 0 �ϴγ���ʱ�����ʱ���»���������й�����ʾֵ;��ʼ��ֵ0xee
//1BYTE 5 �����¼�:B0=0�ָ�=1����;B4-B7=�ָ���ʱ����(376.1�����������ָ��¼�����ʱ2���������ں����);��ʼ��ֵ0
//1BYTE 6 �����¼�:B0=0�ָ�=1����;��ʼ��ֵ0
//3BYTE 7 �����й�����ƽ���ӵ�������(BCD);��ʼ��ֵ0
//3BYTE 10 �����й�����ƽ���Ӵ���(HEX);��ʼ��ֵ0
//6BYTE 13 �����й����ʼ�(HEX);��ʼ��ֵ0
//3BYTE 19 �������й�ƽ������(BCD);��ʼ��ֵ0xee(��1�ֽ�)
#define LEN_ERC29_SOUR     22
#define ADDR_ERC30_SOUR     ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*ERC29MaxPn)//ERC30���ܱ�ͣ���¼���ʱ��ʼʱ����ֵ
//������1-N
//5BYTE ���������й�����ʾֵ;��ʼ��ֵ0xee
//1BYTE �ָ�������־0=�ָ�,1=����;��ʼ��ֵ0
#define LEN_ERC30_SOUR     6
#define ADDR_ERC37_38_SOUR     ADDR_ERC30_SOUR+(LEN_ERC30_SOUR*ERC30MaxPn)//ERC37ERC38���ܱ���Ƕ˸��¼���¼ԭʼ����
//������1-N
//3BYTE ԭ����Ǵ���;��ʼ��ֵ0xeeeeee
//3BYTE ԭ���˸Ǵ���;��ʼ��ֵ0xeeeeee
#define LEN_ERC37_38_SOUR     6
#define ADDR_ERC39_SOUR     ADDR_ERC37_38_SOUR+(LEN_ERC37_38_SOUR*ERC37MaxPn)
//1byte ERC39����ʧ���¼���¼��־:0=��(�ϵ���ո���),1=ʧ�ܼ�¼,2=�ɹ���¼
#define ADDR_ERC40_SOUR     ADDR_ERC39_SOUR+MaxRS485AddCarrierPn//ERC40�ų��쳣�¼���¼
//1BYTE �豸����0(�ն���Ӵ�����),B0-B7λ��ʾ�����ָ�:b0=��Ƶ�ų��쳣,b1=��Ƶ�ų��쳣,b2=ǿ�ų��쳣;��ʼ��ֵ0
//������1-N
//1BYTE �豸����1(�ɼ���),B0-B7λ��ʾ�����ָ�:b0=��Ƶ�ų��쳣,b1=��Ƶ�ų��쳣,b2=ǿ�ų��쳣;��ʼ��ֵ0
//1BYTE �豸����2(���ܱ�),B0-B7λ��ʾ�����ָ�:b0=��Ƶ�ų��쳣,b1=��Ƶ�ų��쳣,b2=ǿ�ų��쳣;��ʼ��ֵ0
//3BYTE ԭ���ܱ�㶨�ų����ŷ����ܴ���;��ʼ��ֵ0xee
#define LEN_ERC40_SOUR     5
#define ADDR_ERC45_SOUR     ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*ERC40MaxPn)//ERC45�ն˵��ʧЧ�¼�
//1byte �ָ�������־0=�ָ�,1=����;��ʼ��ֵ0

	#if (USER/100)==5//�Ϻ�
#define ADDR_ERC41_SOUR     ADDR_ERC45_SOUR+1//ERC41ֱ�Ӷ����ܱ��ʱ�¼���¼
//������1-N
//2byte ԭ����ʱ�¼�������;��ʼ��ֵ0xee
//3byte ԭ���ܱ�ʱ���޸Ĵ���;��ʼ��ֵ0xee
#define LEN_ERC41_SOUR     5
#define ADDR_ERC12_SOUR_Ext     ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*MaxRS485AddCarrierPn)
//3BYTE �ϴ�У��ʱ�ӵ������ջ���;��ʼ��ֵ0
#define LEN_ERC12_SOUR_Ext     3
#define ADDR_ERC68_SOUR     ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*MaxRS485AddCarrierPn)//ERC68���ܱ��ɿ��������¼�(�Ϻ���չ)
//������1-N
//3byte ԭ��������;��ʼ��ֵ0xeeeeee
#define ADDR_Class3Data_ESRAM_End     (ADDR_ERC68_SOUR+(3*MaxRS485AddCarrierPn))
	#else
#define ADDR_Class3Data_ESRAM_End     (ADDR_ERC45_SOUR+1)
	#endif


//ͳ����ԭʼ����(��ʼ������ȫ�����)
#define ADDR_Terminal_StatData_ESRAM_Start     ADDR_Class3Data_ESRAM_End
#define ADDR_AFN0DF25F33_SOUR     ADDR_Terminal_StatData_ESRAM_Start//�й�����Ϊ��ʱ��ָ��¼Ӽ���
//1BYTE �ָ��¼Ӽ���
#define ADDR_AFN0DF57F60_SOUR     ADDR_AFN0DF25F33_SOUR+(1*AFN0DF25MaxPn)//�ܼ����й�����Ϊ��ʱ��ָ��¼Ӽ���
//1BYTE �ָ��¼Ӽ���
#define ADDR_AFN0DF57F60_S_Timer     ADDR_AFN0DF57F60_SOUR+(1*AFN0DF57MaxPn)//�ܼ��������С�й������ڳ�����ɺ�����붨ʱ��
//1BYTE	�����ʱ��
#define ADDR_AFN0DF27F35_SOUR     ADDR_AFN0DF57F60_S_Timer+1//���µ�ѹͳ��
//1BYTE �ָ��¼Ӽ���
//2BYTE A��ѹԽ�޳���ʱ���붨ʱ
//1BYTE ��ǰA��ѹԽ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ������
//1BYTE A��ѹԽ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ������
//2BYTE B��ѹԽ�޳���ʱ���붨ʱ
//1BYTE ��ǰB��ѹԽ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ������
//1BYTE B��ѹԽ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ������
//2BYTE C��ѹԽ�޳���ʱ���붨ʱ
//1BYTE ��ǰC��ѹԽ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ������
//1BYTE C��ѹԽ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ������
//1BYTE Խ���¼���¼��־,B0-B2�ֱ�ABCԽ������,B4-B6�ֱ�ABCԽ������
#define LEN_AFN0DF27F35_SOUR     14
#define ADDR_AFN0DF29F37_SOUR     ADDR_AFN0DF27F35_SOUR+(LEN_AFN0DF27F35_SOUR*AFN0DF27MaxPn)//���µ���ͳ��
//1BYTE �ָ��¼Ӽ���
//2BYTE A����Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰA����Խ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ����
//1BYTE A����Խ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ����
//2BYTE B����Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰB����Խ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ����
//1BYTE B����Խ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ����
//2BYTE C����Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰC����Խ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ����
//1BYTE C����Խ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ����
//2BYTE �������Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰ�������Խ�ޱ�־,0=ûԽ��,1=Խ����
//1BYTE �������Խ���ȶ���־,0=ûԽ��,1=Խ����
//1BYTE B0-B2�ֱ�ABCԽ�������¼��Ѽ�¼,B4-B6�ֱ�ABCԽ�����¼��Ѽ�¼
#define LEN_AFN0DF29F37_SOUR     18
#define ADDR_AFN0DF28F36_SOUR      ADDR_AFN0DF29F37_SOUR+(LEN_AFN0DF29F37_SOUR*AFN0DF29MaxPn)//��ѹ������ƽ��
//1BYTE �ָ��¼Ӽ���
//2BYTE ������ƽ��Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰ������ƽ��Խ�ޱ�־,0=ûԽ��,1=Խ��
//1BYTE ������ƽ��Խ���ȶ���־,0=ûԽ��,1=Խ��
//2BYTE ��ѹ��ƽ��Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰ��ѹ��ƽ��Խ�ޱ�־,0=ûԽ��,1=Խ��
//1BYTE ��ѹ��ƽ��Խ���ȶ���־,0=ûԽ��,1=Խ��
//1BYTE �¼��Ѽ�¼��־,B0=��ѹ��ƽ��Խ��,B1=������ƽ��Խ��
//2BYTE ��ǰ������ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
//2BYTE ��ǰ��ѹ��ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
#define LEN_AFN0DF28F36_SOUR     14
#define ADDR_AFN0DF30F38_SOUR     ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*AFN0DF28MaxPn)//���ڹ���Խ��
//1BYTE �ָ��¼Ӽ���
//2BYTE ���ڹ���Խ�޳���ʱ���붨ʱ
//1BYTE ��ǰ���ڹ���Խ�ޱ�־,0=ûԽ��,1=Խ������,2=Խ����
//1BYTE ���ڹ���Խ���ȶ���־,0=ûԽ��,1=Խ������,2=Խ����
//1BYTE ���ڹ���Խ���¼���¼��־,0=û,B0=Խ������,B1=Խ����
#define LEN_AFN0DF30F38_SOUR     6
#define ADDR_AFN0DF65F66_SOUR     ADDR_AFN0DF30F38_SOUR+(LEN_AFN0DF30F38_SOUR*AFN0DF30MaxPn)//�ܼ��鳬���ʶ�ֵ�µ�������ֵ
//1BYTE �ָ��¼Ӽ���
//4BYTE ���س���ֵ�����ڼ����ԭ�ܼ��й�������
//4BYTE ���µ�������ֵ�����ڼ����ԭ�ܼ��й�������
#define ADDR_AFN0DF43F44_SOUR     ADDR_AFN0DF65F66_SOUR+(9*AFN0DF65MaxPn)//�������������ۼ�ʱ��
//1BYTE �ָ��¼Ӽ���
#define ADDR_AFN0DF129F130_SOUR     ADDR_AFN0DF43F44_SOUR+(1*AFN0DF43MaxPn)//������ֱ��ģ����Խ���ۼ�ʱ��
//1BYTE �ָ��¼Ӽ���
//1BYTE ��ǰԽ�ޱ�־,B0=Խ����,B1=Խ����
//1BYTE �¼��Ѽ�¼��־,B0=Խ����,B1=Խ����
#define LEN_AFN0DF129F130_SOUR     3



//2�ඳ����ԭʼ����
#if (IC_SDRAM==0)
#define ADDR_CLASS2DATASOUR_ESRAM_Start     ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*AFN0DF129MaxPn)
#define ADDR_CLASS2DATASOUR_ESRAM_End     ADDR_CLASS2DATASOUR_ESRAM_Start
#else//#if (IC_SDRAM==0)
#define ADDR_CLASS2DATASOUR_ESRAM_Start     ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*AFN0DF129MaxPn)
#define  ADDR_AFN0DF113_SOUR      ADDR_CLASS2DATASOUR_ESRAM_Start//������A��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF114_SOUR      ADDR_AFN0DF113_SOUR+(114*AFN0DF113MaxPn)//������B��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF115_SOUR      ADDR_AFN0DF114_SOUR+(114*AFN0DF114MaxPn)//������C��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF116_SOUR      ADDR_AFN0DF115_SOUR+(114*AFN0DF115MaxPn)//������A��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF117_SOUR      ADDR_AFN0DF116_SOUR+(114*AFN0DF116MaxPn)//������B��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF118_SOUR      ADDR_AFN0DF117_SOUR+(114*AFN0DF117MaxPn)//������C��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF121_SOUR      ADDR_AFN0DF118_SOUR+(114*AFN0DF118MaxPn)//������A��г��Խ����ͳ������
//�ն���
#define  ADDR_AFN0DF122_SOUR      ADDR_AFN0DF121_SOUR+(77*AFN0DF121MaxPn)//������B��г��Խ����ͳ������
//�ն���
#define  ADDR_AFN0DF123_SOUR      ADDR_AFN0DF122_SOUR+(77*AFN0DF122MaxPn)//������C��г��Խ����ͳ������
//�ն���


#define ADDR_HOURFLASH_RAM_START     ADDR_AFN0DF123_SOUR+(77*AFN0DF123MaxPn)//ÿСʱRAM���ݱ��浽FLASH��ʼ��ַ
//1byte �ϴ�RAM���浽FLASH��ʵʱ��ʱ(�ϵ���Ϊ0,���¶������Ϊ0xffʹ�������б���)
#define ADDR_AFN0DF124_SOUR     ADDR_HOURFLASH_RAM_START+1//�ն���A ���ѹг����ѹ�����ʡ�г������95%����ֵ
//1BYTE �ϴμ����ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96��,ÿСʱ���ݱ��浽FLASH
//2BYTE �����������
//1BYTE г������N����19��
//2BYTE �ܻ����ѹ������95%����ֵ(���ֶ���)
//2byte ��λ����ѹ������95%����ֵ
//2byte ż�λ����ѹ������95%����ֵ
//2*18byte 2-19��г����ѹ������95%����ֵ
//2byte �ܻ������95%����ֵ
//2*18byte 2-19��г������95%����ֵ

//2*5BYTE �ܻ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
//2*5BYTE ��λ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
//2*5BYTE ż�λ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
//2*5*18BYTE 2-19��г����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
//2*5byte �ܻ������95%����ֵ(��5��ֵ-��1��ֵ)
//2*5*18byte 2-19��г������95%����ֵ(��5��ֵ-��1��ֵ)
#define LEN_AFN0DF124_SOUR     (4+80+(80*5))
#define ADDR_AFN0DF125_SOUR     ADDR_AFN0DF124_SOUR+(LEN_AFN0DF124_SOUR*AFN0DF124MaxPn)//�ն���B ���ѹг����ѹ�����ʡ�г������95%����ֵ
//ͬADDR_AFN0DF124_SOUR
#define LEN_AFN0DF125_SOUR     LEN_AFN0DF124_SOUR
#define ADDR_AFN0DF126_SOUR     ADDR_AFN0DF125_SOUR+(LEN_AFN0DF125_SOUR*AFN0DF125MaxPn)//�ն���C ���ѹг����ѹ�����ʡ�г������95%����ֵ
//ͬADDR_AFN0DF124_SOUR
#define LEN_AFN0DF126_SOUR     LEN_AFN0DF124_SOUR
#define ADDR_AFN0DF211_SOUR     ADDR_AFN0DF126_SOUR+(LEN_AFN0DF126_SOUR*AFN0DF126MaxPn)//�ն��������ѹ��������ƽ���95%����ֵ
//1BYTE �ϴμ����ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96��;ÿСʱ���ݱ��浽FLASH
//1BYTE ����,Ϊ���ֶ���
//2BYTE �����������
//2BYTE ������ƽ���95%����ֵ(���ֶ���)
//2BYTE ��ѹ��ƽ���95%����ֵ
//2*5BYTE ������ƽ���95%����ֵ(��5��ֵ-��1��ֵ)
//2*5BYTE ��ѹ��ƽ���95%����ֵ(��5��ֵ-��1��ֵ)
#define LEN_AFN0DF211_SOUR     28
#define ADDR_AFN0DF212_SOUR     ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*AFN0DF211MaxPn)//�¶��������ѹ��������ƽ���95%����ֵ
//1BYTE �ϴμ����ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96*31��;ÿСʱ���ݱ��浽FLASH
//1BYTE ����,Ϊ���ֶ���
//2BYTE �����������
//2BYTE ������ƽ���95%����ֵ(���ֶ���)
//2BYTE ��ѹ��ƽ���95%����ֵ
//2*150BYTE ������ƽ���95%����ֵ(��150��ֵ-��1��ֵ);96*31*0.05=148.8
//2*150BYTE ��ѹ��ƽ���95%����ֵ(��150��ֵ-��1��ֵ);96*31*0.05=148.8
#define LEN_AFN0DF212_SOUR     608
#define ADDR_HOURFLASH_RAM_END     ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*AFN0DF212MaxPn)//ÿСʱRAM���ݱ��浽FLASH������ַ
#define LEN_HOURFLASH_RAM     ((ADDR_HOURFLASH_RAM_END)-(ADDR_HOURFLASH_RAM_START))//ÿСʱRAM���ݱ��浽FLASH�ܳ���
#if (LEN_HOURFLASH_RAM>LEN_HOURFLASH)
#error LEN_HOURFLASH_RAM>LEN_HOURFLASH
#endif


#define ADDR_ERC15_SOUR     ADDR_HOURFLASH_RAM_END//ERC15г��Խ���¼�ԭʼ����
//2BYTE ��Ӽ��������
//3BYTE A��ѹг��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
//3BYTE A����г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
//3BYTE B��ѹг��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
//3BYTE B����г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
//3BYTE C��ѹг��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
//3BYTE C����г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
#define LEN_ERC15_SOUR     20
#define ADDR_CLASS2DATASOUR_ESRAM_End     ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*AFN0CF57MaxPn)

#endif//#if (IC_SDRAM==0)



#define ADDR_Terminal_StatData_ESRAM_End     (ADDR_CLASS2DATASOUR_ESRAM_End)//ͳ����ԭʼ����(��ʼ������ȫ�����)����







//�ն˿���
#define ADDR_Terminal_CTRL_ESRAM_START     ADDR_Terminal_StatData_ESRAM_End//�ն˿���ESRAM��ʼ��ַ

#define ADDR_Terminal_Period     ADDR_Terminal_CTRL_ESRAM_START//�ն˷���ʱ�κ͹���ʱ��
//1BYTE �ն˵�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ��,��,ƽ,�ȵȷ���),0=��Ч
//1BYTE 
//1BYTE ��ǰ����ʱ�κ�1-8,=0��ʾʱ�κ�>9����
//1BYTE 0=��ǰ����ʱ�κŲ�����,1��2��ǰ����ʱ�κſ���

#define ADDR_Terminal_Protect     ADDR_Terminal_Period+4//�ն˱���,0=������,1=�����,2=�Զ�����
//1BYTE 0=������,1=�����,2=�Զ�����
#define ADDR_Terminal_Attr_State     ADDR_Terminal_Protect+1//�߷Ѹ澯����״̬
//1BYTE 0=û���,1=���
#define ADDR_Terminal_Total_State     ADDR_Terminal_Attr_State+1//�ն�����բ
//1BYTE λ��־(�ִ�1-8)��ǰ����բ״̬(1��բ,0�����բ)
//1BYTE λ��־(�ִ�1-8)�����բ֪ͨ(����բ״̬��ӦλתΪ��բʱ��λ,֪ͨ��ʱ������0)
//2BYTE ����
#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//����ģ�������������
//1BYTE	Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
//1BYTE Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��,b2-b6,b7=�ִμ̵���������ʽ(0=����1=�ڲ��Զ�����ʱ��ƽ)
//1BYTE	�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
//1BYTE �ӿ���ģ������Ķ��߱�־�ֽ�1

#define Terminal_Ctrl_Pn_Len     12//�ն˿���ÿPn���ݳ���
#define ADDR_Terminal_Remote_State     ADDR_ContrlUARTData+4//�ն˿���-ң����բ
//�����ִ�1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE 
//2BYTE ��բʱ����(���ݸ�ʽ2)
//1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
//1BYTE 
//2BYTE  
//4BYTE 
#define ADDR_Terminal_kWCtrl1_State     ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*MaxControlRound)//�ն˿���-����.��ǰ�����¸���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=���㶨ֵ,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//2BYTE ��բʱ����(���ݸ�ʽ2)
//1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
//1BYTE 
//2BYTE
//1BYTE ����ֵ��־0=û��,1=����ֵ
//1BYTE ��ǰ������բ�ִ�B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl2_State     ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���-����.Ӫҵ��ͣ����
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//2BYTE ��բʱ����(���ݸ�ʽ2)
//1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
//1BYTE 
//2BYTE
//1BYTE ����ֵ��־0=û��,1=����ֵ
//1BYTE ��ǰ������բ�ִ�B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl3_State     ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���-����.���ݹ���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//2BYTE ��բʱ����(���ݸ�ʽ2)
//1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
//1BYTE 
//2BYTE
//1BYTE ����ֵ��־0=û��,1=����ֵ
//1BYTE ��ǰ������բ�ִ�B0-B7
//2BYTE
#define ADDR_Terminal_kWCtrl4_State     ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���-����.ʱ�ι���
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
//1BYTE ����բ�ִ�B0-B7
//2BYTE ��բʱ����(���ݸ�ʽ2)
//1BYTE !=0ֵ��ʾͶ�뵫û��ԭ���1=...
//1BYTE ������0-2��ʾ1-3
//2BYTE ��ǰʱ�ζ�ֵ
//1BYTE ����ֵ��־0=û��,1=����ֵ
//1BYTE ��ǰ������բ�ִ�B0-B7
//2BYTE
#define ADDR_Terminal_kWhCtrl1_State     ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���-���.�µ��
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
//1BYTE ����բ�ִ�B0-B7
//4BYTE �µ�����ֵ(���ݸ�ʽ3)
//2BYTE 
//1BYTE	
//3BYTE 
#define ADDR_Terminal_kWhCtrl2_State     ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���-���.�����
//�ܼ���1-N
//1BYTE ����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
//1BYTE ����բ�ִ�B0-B7
//4BYTE 
//1BYTE ���������ʣ�����(��)����ʱʶ��,0=������,1=�����
//1BYTE 
//4BYTE
#define ADDR_Terminal_CTRL_ESRAM_END     ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup)//�ն˿���ESRAM������ַ


//����ɹ�ʧ�ܼ����ͱ�־
#define ADDR_ERC21_Flag     ADDR_Terminal_CTRL_ESRAM_END//ERC21 485��������¼���¼��־
//1byte 0=û��¼,1=�Ѽ�¼
#define ADDR_RM_CountFlags     (((ADDR_ERC21_Flag+1)+3)&0xfffffffc)//����ɹ�ʧ�ܼ����ͱ�־(ERAM���ֶ���)
//2BYTE RS485-1	���ֳ���ɹ�����
//2BYTE RS485-1 ���ֳ���ʧ�ܼ���
//2BYTE RS485-2	���ֳ���ɹ�����
//2BYTE RS485-2 ���ֳ���ʧ�ܼ���
//2BYTE RS485-3	���ֳ���ɹ�����
//2BYTE RS485-3 ���ֳ���ʧ�ܼ���
//2BYTE �ز���  ���ֳ���ɹ�����
//2BYTE �ز���  ���ֳ���ʧ�ܼ���

//2BYTE RS485-1	���ֳ���ɹ�����
//2BYTE RS485-1 ���ֳ���ʧ�ܼ���
//2BYTE RS485-2	���ֳ���ɹ�����
//2BYTE RS485-2 ���ֳ���ʧ�ܼ���
//2BYTE RS485-3	���ֳ���ɹ�����
//2BYTE RS485-3 ���ֳ���ʧ�ܼ���
//2BYTE �ز���  ���ֳ���ɹ�����
//2BYTE �ز���  ���ֳ���ʧ�ܼ���
//(MaxRS485AddCarrierPn/4)�ֽ�,�����ܱ��������,ÿ����2λԭ���ܱ����־(0=�ɹ�,1=ʧ��->�ɹ�,2=�ɹ�->ʧ��,3=ʧ��)
#define ADDR_RM_CountFlags_End     (ADDR_RM_CountFlags+32+(MaxRS485AddCarrierPn/4))


#define ADDR_AFN0DF26F34_SOUR     ADDR_RM_CountFlags_End//�ն������ܼ������������������ʱ��
//������1
//5BYTE �ϴμ���ȷ�����������ʵʱ��������ʱ��
//1byte �ϴμ���ƽ���ӵ�ʵʱ�ӷ�

//1BYTE ���й����ʼ�ƽ���Ӵ���(hex)
//1BYTE A�й����ʼ�ƽ���Ӵ���(hex)
//1BYTE B�й����ʼ�ƽ���Ӵ���(hex)
//1BYTE C�й����ʼ�ƽ���Ӵ���(hex)

//4byte ���й����ʼ�(hex)
//4byte A�й����ʼ�(hex)
//4byte B�й����ʼ�(hex)
//4byte C�й����ʼ�(hex)
//������n
#define LEN_AFN0DF26F34_SOUR     26
#define ADDR_AFN0DF26_SOUR     ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*AFN0DF26MaxPn)//�ն������ܼ������������������ʱ��
//������1 (��ʼֵ0,�ն������0)
//�������й��������	����¼A.23	kW	3
//�������й������������ʱ��	����¼A.18	��ʱ��	3
//A���й��������	����¼A.23	kW	3
//A���й������������ʱ��	����¼A.18	��ʱ��	3
//B���й��������	����¼A.23	kW	3
//B���й������������ʱ��	����¼A.18	��ʱ��	3
//C���й��������	����¼A.23	kW	3
//C���й������������ʱ��	����¼A.18	��ʱ��	3
//...
//������n (��ʼֵ0,�ն������0)
#define ADDR_AFN0DF34_SOUR     ADDR_AFN0DF26_SOUR+(24*AFN0DF26MaxPn)//�¶������ܼ������������������ʱ��
//������1 (��ʼֵ0,�¶������0)
//�������й��������	����¼A.23	kW	3
//�������й������������ʱ��	����¼A.18	��ʱ��	3
//A���й��������	����¼A.23	kW	3
//A���й������������ʱ��	����¼A.18	��ʱ��	3
//B���й��������	����¼A.23	kW	3
//B���й������������ʱ��	����¼A.18	��ʱ��	3
//C���й��������	����¼A.23	kW	3
//C���й������������ʱ��	����¼A.18	��ʱ��	3
//...
//������n (��ʼֵ0,�¶������0)

#define ADDR_KeyUserPnList     ((ADDR_AFN0DF34_SOUR+(24*AFN0DF34MaxPn)+3)&0xfffffffc)//�ص㻧���߶���Pn�б�,���ֶ���
//2BYTE ������1��������߶�Ӧ��ʵ�ʲ������,0Ϊ��Ч
//...
//2BYTE ������MaxKeyUser��������߶�Ӧ��ʵ�ʲ������,0Ϊ��Ч
#define ADDR_F246PnList     ADDR_KeyUserPnList+(2*MaxKeyUser)//Ҫ�����ĵ����¼����Pn�б�,���ֶ���
//2BYTE Pn=1�����Ӧ��ʵ�ʲ������,0Ϊ��Ч
//...
//2BYTE Pn=F246MaxPn�����Ӧ��ʵ�ʲ������,0Ϊ��Ч
#define ADDR_PnVSAFN04F10SetNo     ADDR_F246PnList+(2*F246MaxPn)//�������Pn��ӦAFN04F10�������,0Ϊ��Ч
//2byte 
//2byte p1��ӦAFN04F10�������,0Ϊ��Ч
//...
//2byte pn��ӦAFN04F10�������,0Ϊ��Ч
#define ADDR_PnVSSetList     ADDR_PnVSAFN04F10SetNo+2+(2*MaxRS485AddCarrierPn)//�������Pn��ӦAFN04F10�����б�
//6byte Pn=0
//6byte Pn=1��ӦAFN04F10����
//6byte ...
//6byte Pn=n��ӦAFN04F10����
#define ADDR_CurveCongealList     ADDR_PnVSSetList+6+(6*MaxRS485AddCarrierPn)//�������߶���ʱ���б�
//2byte B0-B14���߶������,b15=1��ʾ���߶��������
//5byte YMDHM
//1byte 
//...
#define ADDR_AllRMList     ((ADDR_CurveCongealList+2+(6*MaxCurveCongeal)+3)&0xfffffffc)//485�ڳ�����Ϣ��
typedef __packed struct
{
	u8 res1;//
	u8 res2;//
	u8 RMCompleteS[MaxRMTask];//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
}RMList_TypeDef;
#define LEN_AllRMList     (2+MaxRMTask)//ÿ�б��ֽ���
typedef __packed struct
{
  RMList_TypeDef     Pn[1+MaxRS485AddCarrierPn];
}AllRMList_TypeDef;
#define AllRMList     ((AllRMList_TypeDef *)(ADDR_AllRMList))//485�ڳ�����Ϣ��(��Pn=0-n����)



#if ((Project/100)!=2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#define ADDR_ERAM1_End     ((ADDR_AllRMList+(LEN_AllRMList*(1+MaxRS485AddCarrierPn))+3)&0xfffffffc)//�ն���eram���ݽ�����ַ
#else
//�ز�������������
#define ADDR_Router     ((ADDR_AllRMList+(LEN_AllRMList*(1+MaxRS485AddCarrierPn))+3)&0xfffffffc)//·����
typedef __packed struct
{
	u8 RouterAddr[6];//·�����ڵ��ַ(���Ƿ�����ǰ,ͨ�ſ��ַ��λ��)//0xC014AD24
	u8 RouterCODE[4];//���̴��� ASCII 2,оƬ���� ASCII 2
	u8 RouterVer[5];//·�����汾��,�ֱ�Ϊ:��,��,��,2BYTE�汾

	u8 RouterCtrl;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
	u8 RouterCtrlTask;//·���������������;��ɺ��Զ��ص�0
	u8 ReRouterCtrl;//·���������ط�����
	u8 RUNState;//��ģ���ȡ�ĵ�ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
	u8 UpDateTask;//�����������
	u8 UpDateReCount;//����������Դ���
	u8 res1;

	u8 RouterInitTask;//·������ʼ������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	u8 RouterInitSubTask;//·������ʼ��������
	u8 RouterInitReCount;//·������ʼ�������ط�����
	u8 AFN04F10;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
	u8 MaxOverTime;//�ӽڵ������ʱʱ����
	
	u8 RouterSynTask;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
	u8 RouterRealTimeTask;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
	u8 RouterKeyUserTask;//�ز��ص㻧��������

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
	u16 res5;
	u16 res6;
	u32 StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
		//b0=��������ʱ,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
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
#if (USER/100)==14//����ϵ		
	u8 Protocol;//Э��:2-��������  ����-����
	u8 FuJTNum;//����ÿ��Ԥ�泭���������� BIN 1	
	u16 FuJTSum;//����ÿ�ִ�֧�ֵ���Ԥ�������������  BIN 2
	u64 FuJAutoSerTime;//��������ע������ʼʱ��
	u8 res7[30];
#else	
	u8 res7[42];
#endif
    u8 MeterCourts;//̨��ʶ���־
    u8 MaxTxNum;//��󲢷���
    u8 HPLC_S_Sum;//����ز�ģ�鲢���������
	u8 HPLC_Over;//0=��,!=0���;����ز��������
	u16 HPLC_MeterNum;//��ǰ�������������ţ�ģ������ʱ����ʱ����
    u8 TimerS;//������ʱʱ�䶨ʱs
    u8 TxNum;//���ɷ���֡��
    u8 HPLC_CurveTaSK;//�ز�ģ�鳭��HPLC���� TASK  0:����һСʱ����    1:������0�㵽���ڵ�����    2:����һ��0�㵽���ڵ�����
#if ((USER/100)==6)//���ϼ�����
    u8 AFN04F251_FLAG;//������־
#else
    u8 res9;
#endif
}Terminal_Router_TypeDef;
#define LEN_Router     128//·�������������ݳ���
#define Terminal_Router     ((Terminal_Router_TypeDef *)(ADDR_Router))

#define ADDR_NoteList     (ADDR_Router+LEN_Router)//·�����нڵ��
typedef __packed struct
{
	u8 Addr[6];//�ӽڵ��ַ(�ֶ���)
	u16 MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
	u16 Pn;//��Ӧ�������
	u8 FnCount;//����Fn����
	u8 RMCount;//�������ݱ�ʶ����
	u8 ReRMCount;//�ط�����
	u8 RMCountErr;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
	u8 RMErr;//��ǰ����ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
	u8 RMErr_0x901F;//�ز�DL645-1997�ն���0xE3��0x901F����ʱ��0x9010����ı��;0=��,1=��0x901f,2=0x901F�����ر�ʶ����
	u8 FnCountSOfMeterEvent;//�������ڳ������ܱ��¼�ǰԭ�����Fn����
	u8 FnCountBackupOfAutoReport;//���������ϱ�ǰԭ�����Fn����
	u8 FnCountSOfKeyUser;//����ÿСʱ������������ǰԭѭ������ĳ���Fn����
	u8 FnCountS;//��������ǰԭѭ������ĳ���Fn����
	u16 res1;
	u8 CollectorAddr[6];//�ɼ�����ַ
	u8 RMCompleteS[MaxRMTask];//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
	u8 FnDataBuff[LEN_UART4FnDataBuff];//Fn���ݻ���
#if HPLC==1
		u8 TxSEQ;//����������
		u8 EndRMCount;//�����������       u8 TASK;//��������  //0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		u16 res2;
#endif	
}NoteList_TypeDef;
#if HPLC==1
	#define LEN_NoteList     (28+MaxRMTask+LEN_UART4FnDataBuff+4)//ÿ�б��ֽ���  �ܳ�������4�ı���
#else
	#define LEN_NoteList     (28+MaxRMTask+LEN_UART4FnDataBuff)//ÿ�б��ֽ���  �ܳ�������4�ı���
#endif

	
typedef __packed struct
{
  NoteList_TypeDef     List[MaxRS485AddCarrierPn];
}Terminal_NoteList_TypeDef;
#define Terminal_Note     ((Terminal_NoteList_TypeDef *)(ADDR_NoteList))//�ز�·�����нڵ��(����·�ɺ����˳������)


#define ADDR_ERAM1_End     (ADDR_NoteList+(LEN_NoteList*MaxRS485AddCarrierPn))//�ն���eram���ݽ�����ַ
#endif//#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=




//�ն�1��AFN0C����
#define ADDR_AFN0C_Start     ADDR_ERAM1_End
#include "AFN0C.h"
#define LEN_AFN0C     (ADDR_AFN0C_End-ADDR_AFN0C_Start)//ʵʱ1������RAM�����ܳ���



#define ADDR_HOURCONGEAL_BUFF_START     ADDR_AFN0C_End//Сʱ���Ỻ�忪ʼ��ַ
#include "AFN0C_HourCongeal_Buff.h"
#define ADDR_AFNOD_SOUR_Start     ADDR_HOURCONGEAL_BUFF_END//��2ԭʼ���ݺ���2���ݶ��Ỻ�忪ʼ��ַ
#include "AFN0D_Congeal_Buff.h"



#define ADDR_AFN0D_UART_Buff_Start     ADDR_AFN0D_Congeal_Buff_End//������UART376.1�ٶ�2�൱������ʱ��ʱ������

#define ADDR_DATABUFF_ERAM     ADDR_AFN0D_Congeal_Buff_End+LEN_AFN0D_Congeal_Buff
#define LEN_DATABUFF_ERAM     (8*1024)//ERAM���ݻ�����С4096(��1�ζ��������ڷ����¼��洢)

//��չ
#define ADDR_ERAM_EXT_START     ((ADDR_DATABUFF_ERAM+LEN_DATABUFF_ERAM+3)&0xfffffffc)//�ֶ���
typedef __packed struct
{
	u16 MeterNo;//�Ĵ��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
	u8 FnCount;//�Ĵ泭��Fn����
	u8 RMCount;//�Ĵ泭���������
	u8 RMTaskC;//�Ĵ�ԭ��������
	u8 RMEventTaskC;//�Ĵ泭�����ܱ��¼�����:0=��,1-3=1-3��
	u16 res2;
	u32 rse3;
	u32 res4;
}RMEventStack_TypeDef;//�������ܱ��¼�ʱ�ļĴ�ջ����
#define LEN_RMEventStack     16
typedef __packed struct
{
	u8 YMDHMS[6];//6byte YMDHMS �ϴ����ڶ�ʱ�����������ʱ����(ȫ0��ʾ��Ч)
	u8 YMDHMS1[6];//6byte YMDHMS �ּ�1�¼��´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
	u8 YMDHMS2[6];//6byte YMDHMS �ּ�2�¼��´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
	u8 YMDHMS3[6];//6byte YMDHMS �ּ�3�¼��´ζ�ʱ��ʼ��������ʱ����(ȫ0��ʾ��Ч)
	u32 res2;
	u8 res3;
	u8 res4;
	u8 TaskStore;//�ݴ�����ܱ��¼���־:0=û,1=��
	u8 StackCount;//ջ����(ֵ1-3,4�̶�Ϊ��ʱ��)
	//ջ����=0 ջ��
	//ջ1 �¼����������ȼ���ջ
	//ջ2 �¼����������ȼ���ջ
	//ջ3 �¼����������ȼ���ջ
	//ջ4 �̶��¼�������ʱ�˳���ջ
	RMEventStack_TypeDef     RMEventStack[MaxMeterEventLevel];//
}UARTRMEvent_TypeDef;//UART���¼������ṹ����
#define LEN_UARTRMEvent     (32+(LEN_RMEventStack*4))

#define ADDR_UARTRMEvent_4851     ADDR_ERAM_EXT_START//485-1
#define UARTRMEvent_4851     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4851))
#define ADDR_UARTRMEvent_4852     ADDR_UARTRMEvent_4851+LEN_UARTRMEvent//485-2
#define UARTRMEvent_4852     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4852))
#define ADDR_UARTRMEvent_4853     ADDR_UARTRMEvent_4852+LEN_UARTRMEvent//485-3
#define UARTRMEvent_4853     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4853))
#define ADDR_UARTRMEvent_4854     ADDR_UARTRMEvent_4853+LEN_UARTRMEvent//485-4(�ز�)
#define UARTRMEvent_4854     ((UARTRMEvent_TypeDef *)(ADDR_UARTRMEvent_4854))



#define ADDR_ERAM_END     ((ADDR_UARTRMEvent_4854+LEN_UARTRMEvent+3)&0xfffffffc)

#if (((Project/100)==2)&&((USER/100)==14))//����������-����ϵ
#define MAX_HOSTACK_NUM 8//���ACK����
#define FUJ_RIni_S	150//��ʼ�����
#define FUJ_RIni_E	159//��ͣ������
#define FUJ_RRun_S	160//�����ز�����
#define FUJ_RRun_E	170//���������
#define FUJ_ROUTINIT(N)  ((FUJ_RIni_S<=N)&&(N<=FUJ_RIni_E))
#define FUJ_ROUTRUN(N)  ((FUJ_RRun_S<=N)&&(N<=FUJ_RRun_E))
#define FUJTASK_DISABLE_FLAG 	0x00//��Ч����
#define FUJTASK_ENABLE_FLAG 	0x01//��Ч����-�ȴ��ж�
#define FUJTASK_RUNNING_FLAG	0x02//����ִ��--��ѹ���������
#define FUJTASK_END_FLAG			0x03//����ɣ���������
#define FUJTASK_ACT_FLAG			0x04//����������
#define FUJTASK_PAUSE_FLAG		0x05//��ͣ����
typedef __packed struct
{
	u8 State;//����״̬
	u8 Prio;//���ȼ�
//	u8 TaskPerio;//����ִ��������ֵ
//	u8 TaskUnit;//����ִ�����ڵ�λ
//	u8 RetryPerio;//ʧ������������ֵ
//	u8 RetryUnit;//ʧ���������ڵ�λ
	u8 RetryTimes;//���Դ���,�״ζ�ȡʧ��ʱ����FLASH��ȡ������Դ�����ÿ��ִ�У���ֵ��1��Ϊ0ʱ����ʧ�ܣ���ֹͣ
	u8 ReportFlag;//�ϱ�״̬  0-�����ϱ�  1-�ȴ��ϱ�  2-�ȴ������ٲ� 3-�����ٲ������
	u8 LastRTime[6];//��һ��ִ�е�ʱ��--��������ʱ��Ϊ0
	u8 NextSTime[6];//��һ������ʱ��
}FuJTask_TypeDef;//������ʱ���񷽰��жϱ���
//#define ADDR_FUJTASK_BUF	(ADDR_UARTRMEvent_4854+LEN_UARTRMEvent)
#define ADDR_FUJTASK_BUF	(ADDR_ERAM_END)
#define FuJClloectTask     ((FuJTask_TypeDef *)(ADDR_FUJTASK_BUF))
#define FUJ_CTL_LEN	16//sizeof(FuJTask_TypeDef)
#define LEN_FUJTASK_BUF		FUJ_CTL_LEN*MAX_FUJ_TASK//16byte*60000(1byte state + 4byte BaseTime + 2byte Per + 2byte RetryPer + 4byte CurRound)
//״̬ ��׼ʱ��  ִ������  ��������  ��ǰ�ִ�((LastEndTime-BaseTime) / Per)--��ֹͬһ����ִͣ��
typedef __packed struct
{
	u16 TaskID;//�����1-60000
//	u32 StartTime;//��ʼʱ��
}FuJTaskStack_TypeDef;//�����������,���������ȼ���С������Ŵ�С����,˳���ɵ͵��ߣ�����ά��
#define ADDR_FUJTASK_STACK	(ADDR_FUJTASK_BUF+LEN_FUJTASK_BUF)
#define LEN_Per_StackObj		2
#define LEN_FUJTASK_STACK		LEN_Per_StackObj*MAX_FUJ_TASK
#define FuJTaskStack     ((FuJTaskStack_TypeDef *)(ADDR_FUJTASK_STACK))

#define ADDR_FUJTASK_PRIO_BUF	(ADDR_FUJTASK_STACK+LEN_FUJTASK_STACK)//����������������ȼ���Ӧ���������
#define LEN_FUJTASK_PRIO_BUF	2*254//2byte*���ȼ�������ÿ�����ȼ������񷽰������� 0-253

typedef __packed struct
{
//�����ܿ���  4�ֽ�
	u8 TaskCtrl;//�����ܿ���
	u8 TaskFlag;//D0-D1�ն������ϱ�״̬ 1����2��ֹ D2-D3������������ִ��״̬ 1���� 2��ֹ	
	u16 Num;//�Զ������Ѵ����������
	
//��ǰ����ִ�е��������  1+ 31�ֽ�
	u8 Res1;//����	����
	u8 TaskState;//����״̬;0-99�����ʼ��;100-199����ִ�н׶�;200-219��������׶�
	u8 ReTimes;//�ظ�������                  ���ִ�,��ǰʱ�����������ڵı�ֵ
	u8 MaxCount;//������Դ���
	u8 TaskReCount;//��֡���Լ���
	u8 Port;//ִ�ж˿�
	u16 TaskID;//�����1-60000
	u8  STime[6];//��ʼʱ��-�����������ʼʱ��
	u8  ETime[6];//���ֽ���ʱ��
	u32 Round;//�ִ�
	u16 SubTaskID;//��ǰ���ڳ����ľ��������
	u16 Total;//�ɼ�������
	u16 Success;//�ɼ��ɹ�����
	u8  ReportFlag;//�ϱ���־
	u8 FrameCheck;//���ĸ�ʽ��
	//Ԥ��������  1+  15�ֽ�	
	u16 MeterIndex;//����ȡ��Ԥ���������--���ݾ�������������,ͨ�ŵ�ַ���ظ�
	u16 LastIndex;//��һ����ȡλ��
	u16 MeterSum;//��Ԥ���������--
	u16 EndReadSum;//�ѳ���������������
	u16 CurNum;//��ǰ����Ԥ��Ķ�������
	u8  Delay;//ͨ����ʱ����
	u8  Times;//Ԥ���ܴ���
	u8  TimeOutNum;//Ԥ����޻ظ���ʱ�ִ�
	u8 Res2;//����	����	
	u16  S_Timer;//��ʱʱ��
//������п���    35�ֽ�
	u16 Head;//����λ��
	u16 Depth;//��������ܳ���
	u16 SubTask[10];
	u8  Recount[10];//ʧ�����Դ�������ֵ
	u8 LogFlag;
	u8 Res[7];//����
}FuJCurTask_TypeDef;//������ǰ����ִ�е�����
#define ADDR_FuJCurTask_CTRL	(ADDR_FUJTASK_PRIO_BUF+LEN_FUJTASK_PRIO_BUF)
#define LEN_FuJCurTask_CTRL		94//���ֽڳ���94
#define FuJCurTask     ((FuJCurTask_TypeDef *)(ADDR_FuJCurTask_CTRL))

typedef __packed struct
{
	u16 SubTaskID;//�����
	u8 Type;//ͨ�Ŷ�������
	u8 Addr[6];//ͨ�Ŷ����ַ
	u8 State;//״̬  00-�ȴ�Ԥ��  1-���ڽ���Ԥ�� 2-��Ԥ�� 10-ִ��ʧ��  11-ִ�гɹ�	
}FuJTaskObj_TypeDef;//����Ԥ�������
#define ADDR_FuJTask_OBJ	(ADDR_FuJCurTask_CTRL+LEN_FuJCurTask_CTRL)//��ǰԤ�����
#define LEN_FuJTask_OBJ_Per 10//sizeof(FuJTaskObj_TypeDef)
#define MAX_FuJTask_OBJ   2040//���֧�ֵĶ�����
#define LEN_FuJTask_OBJ		LEN_FuJTask_OBJ_Per*MAX_FuJTask_OBJ//���ֽڳ���2040*10
#define FuJTaskObj     ((FuJTaskObj_TypeDef *)(ADDR_FuJTask_OBJ))

#define ADDR_FuJTask_Cur_OBJ	(ADDR_FuJTask_OBJ+LEN_FuJTask_OBJ)//��ǰԤ�����
#define MAX_FuJTask_OBJ_Per		256//����Ԥ�����Ķ�����
#define LEN_FuJTask_Cur_OBJ 	MAX_FuJTask_OBJ_Per*9//



//485�������
#define MAX_FuJTask_485_Num  100//485ͨ��֧�ֵ����������
#define ADDR_FUJTASK_485_STACK	(ADDR_FuJTask_Cur_OBJ+LEN_FuJTask_Cur_OBJ)
#define LEN_FUJTASK_485_STACK		LEN_Per_StackObj*MAX_FuJTask_485_Num

//485������������б�
#define ADDR_FuJTask_485_OBJ	(ADDR_FUJTASK_485_STACK+LEN_FUJTASK_485_STACK)//��ǰԤ�����
#define LEN_FuJTask_485_OBJ		(LEN_FuJTask_OBJ_Per*MAX_FuJTask_OBJ)//���ֽڳ���2040*10

typedef __packed struct
{
//�����ܿ���    2�ֽ�
	u8 TaskCtrl;//�����ܿ���
	u8 TaskFlag;//D0-D1�ն������ϱ�״̬ 1����2��ֹ D2-D3������������ִ��״̬ 1���� 2��ֹ
	
//��ǰ����ִ�е��������   3+ 31�ֽ�
	u8 Res1[3];//����	����	
	u8 TaskState;//����״̬;0-99�����ʼ��;100-199����ִ�н׶�;200-219��������׶�
	u8 ReTimes;//�ظ�������
	u8 MaxCount;//������Դ���
	u8 TaskReCount;//��֡���Լ���
	u8 Port;//ִ�ж˿�
	u16 TaskID;//�����1-60000
	u8  STime[6];//��ʼʱ��-�����������ʼʱ��
	u8  ETime[6];//���ֽ���ʱ��
	u32 Round;//�ִ�
	u16 SubTaskID;//��ǰ���ڳ����ľ��������
	u16 Total;//�ɼ�������
	u16 Success;//�ɼ��ɹ�����
	u8  ReportFlag;//�ϱ���־
	u8 FrameCheck;//���ĸ�ʽ��
	//Ԥ��������     1+ 7�ֽ�
	u16 MeterIndex;//����ȡ��Ԥ���������--���ݾ�������������,ͨ�ŵ�ַ���ظ�
	u16 CurNum;//��ǰ����Ԥ��Ķ�������
	u8  Delay;//ͨ����ʱ����
	u8 Res2;//����	����
	u16  S_Timer;//��ʱʱ��
//������п���      2�ֽ�
	u16 Depth;//��������ܳ���
	u8 Res[18];//����
}FuJCur485Task_TypeDef;//������ǰ����ִ�е�����
#define ADDR_FuJCurTask_485_CTRL	((ADDR_FuJTask_485_OBJ+LEN_FuJTask_485_OBJ+3)&0xFFFFFFFC)
#define LEN_FuJCurTask_485_CTRL		64//���ֽڳ���64
#define FuJCur485Task     ((FuJCur485Task_TypeDef *)(ADDR_FuJCurTask_485_CTRL))


typedef __packed struct
{
	u8 ReportFlag;//54F22
	u8 AutoPackFlag;//�����ٲ��־
	u8 Num;//��ǰ�ϱ�������
	u8 Res;//����	����	
	u16 Task;//��ǰ�����ϱ��������
	u16 SubTaskID;//���������Ӧ�����
	u32 CurRound;//��ǰ��ȡ���ִ�
	u32 MaxRound;//����ִ�
	u32 EventIndex;//��һ���ϱ����¼����	
	u16 SubTaskIDList[15];//�ϱ���������洢����б�
	u16 EventNum[7];//�����ϱ����¼����
}FuJTaskReport_TypeDef;//���������ϱ�����
#define ADDR_FuJTaskReportCtrl	(ADDR_FuJCurTask_485_CTRL+LEN_FuJCurTask_485_CTRL)
#define LEN_FuJTaskReportCtrl 64
#define FuJTaskReportCtrl  ((FuJTaskReport_TypeDef *)(ADDR_FuJTaskReportCtrl))
//LOG
#define ADDR_LOGOUTDATA		(ADDR_FuJTaskReportCtrl+LEN_FuJTaskReportCtrl)
#define LEN_LOGOUTDATA	(4+256*1024)//2byteLen+8190byteData
#define ADDR_ERAM_EXT_END     (ADDR_LOGOUTDATA+LEN_LOGOUTDATA)

//#define ADDR_ERAM_EXT_END     (ADDR_FuJTaskReportCtrl+LEN_FuJTaskReportCtrl)
#else
#define ADDR_TEMP_TIMER		ADDR_UARTRMEvent_4854+LEN_UARTRMEvent
#define ADDR_LOGOUTDATA   ADDR_TEMP_TIMER+1//��ӡLOG����
#define LEN_LOGOUTDATA	(4+256*1024)//2byteLen+8190byteData
#define ADDR_ERAM_EXT_END     (ADDR_LOGOUTDATA+LEN_LOGOUTDATA)//ADDR_UARTRMEvent_4854+LEN_UARTRMEvent
#endif  
#if ((ADDR_ERAM_EXT_END)-ADDR_ERAM_START)>0x01000000
    #error ADDR_FuJTask_END>32M-16M
#endif


//ʣ��ERAM�ռ�ȫ�����Զ�����(alloc())
#if (IC_MT48LC4M16|IC_MT48LC8M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00600000
    #error ADDR_ERAM_END>8M-2M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00600000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00200000//ʣ��ERAM�ռ䳤��
#endif
#if (IC_MT48LC8M16|IC_MT48LC16M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00800000
    #error ADDR_ERAM_END>16M-8M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00800000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x00800000//ʣ��ERAM�ռ䳤��
#endif
#if (IC_MT48LC16M16|IC_MT48LC32M8)//0=û,1=��
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x01000000
    #error ADDR_ERAM_END>32M-16M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x01000000)//��Ϊ8�ֽڶ���
	#define LEN_MyHeap     0x01000000//ʣ��ERAM�ռ䳤��
#endif

#if (IC_MT48LC32M16|IC_MT48LC64M8)//0=û,1=��(64M)
  #if ((ADDR_ERAM_END)-ADDR_ERAM_START)>0x00A00000
    #error ADDR_ERAM_END>10M//64M
  #endif
	#define ADDR_MyHeap     (ADDR_ERAM_START+0x00A00000)//��Ϊ8�ֽڶ���
	#ifndef LINUX
	#define LEN_MyHeap     0x02600000//ʣ��ERAM�ռ䳤��
	#else//#ifndef LINUX
	
	#define LEN_MyHeap     0//ʣ��ERAM�ռ䳤��
	
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

