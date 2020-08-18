


#ifndef EFLASH_H
#define EFLASH_H

#define ADDR_EFLASH_Start     0x30000000
//NAND_FILE_BLOCK_COUNT*128*1024�ֽڳ����ļ�ϵͳ
//NAND_REPLACE_BLOCK_COUNT*128*1024�ֽڳ��ȴ�����滻
#define ADDR_Download_ENTRY     ADDR_EFLASH_Start+((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024)//���ַ����
#define LENmax_Download         (1024*1024)
//1024K MS,OS,Router����ʱ���ݻ����ش����ݴ�
	

//����ʱ���ݱ�����--------------------------------------------------------------------------------
#define ADDR_DISK_Start     (ADDR_Download_ENTRY+LENmax_Download)
//128K ��Դ����ʱ���ݱ���1//iMeter,Terminal
//128K ��Դ����ʱ���ݱ���2//msFILE
//128K ��Դ����ʱ���ݱ���3
//128K ��Դ����ʱ���ݱ���4//128k NANDBuff

//128K ��ع����ػ�ǰ���ݱ���1//iMeter,Terminal
//128K ��ع����ػ�ǰ���ݱ���2//msFILE
//128K ��ع����ػ�ǰ���ݱ���3
//128K ��ع����ػ�ǰ���ݱ���4//128k NANDBuff

#define LEN_msFILEmanage_Ext     (256*1024)//msFILE��չ;0��128K��������
//LEN_msFILEmanage_Ext�ֽ� ��Դ����ʱ���ݱ���//msFILE��չ
//LEN_msFILEmanage_Ext�ֽ� ��ع����ػ�ǰ���ݱ���//msFILE��չ

#define LEN_DISK     ((8*128*1024)+(2*LEN_msFILEmanage_Ext))//����ʱ���ݱ���������
#define ADDR_DISK_End     (ADDR_DISK_Start+LEN_DISK)
//����ʱ���ݱ���������----------------------------------------------------------------------------


//��������-----------------------------------------------------------------------------------------------
#define ADDR_Com_EFLASH_Start     ADDR_DISK_End
#define ADDR_RTCOffice     ADDR_Com_EFLASH_Start//�ն�ʱ������Ǳ�ʱ�Ӳ�ֵ(�������ʱ��)
//4BYTE HEX����,��
#define ADDR_Language     ADDR_RTCOffice+4//��ʾ����ͬ��RAM�е�Comm_Ram->Language
//1BYTE 0=����,1=Ӣ��,2=
//3BYTE ����
#define ADDR_FileDown     ADDR_Language+4//�Զ����������
//�������1
//1BYTE �ŵ�0=RS232,1=GPRS��RJ45
//1BYTE ����
//1BYTE �ļ��������	BIN	1
//1BYTE �ļ����(0-255)	BIN	1
//16BYTE �ļ�����	ASCII	16
//2BYTE �ļ��ܶ���n(n>=1)	BIN	2
//2BYTE �ļ����ݵ�ǰ�����ţ�i=1~n��	BIN	2
//2BYTE �ļ�ÿ���ֽڳ���(L)	BIN	2
//4BYTE ���������ֽ���
//2BYTE ����
//�������2
//....
//�������8
//...
#define ADDR_LEN_AFN0FF1    ADDR_FileDown+(32*8)//376.1 F1�����ļ����ܳ��ȼ����Ĵ�
//4byte
#define ADDR_AFN0FF1     ADDR_LEN_AFN0FF1+4//376.1 F1����
//1BYTE �ļ���ʶ:00H�����װ�ļ�;01H-06H�����ļ�
//1BYTE �ļ����ԣ���ʼ���м�֡��00H������֡��01H��
//1BYTE �ļ�ָ�00H�����ķ�ʽ��װ��01H��FTP ��ʽ��װ��02H���������ַ������
//2BYTE �ܶ���n
//4BYTE ��i �α�ʶ��ƫ�ƣ�i=0��n��
//2BYTE ��i �����ݳ���Lf
//xBYTE �ļ�����
#define LEN_AFN0FF1     256
#define ADDR_AFN0CF14     ADDR_AFN0FF1+LEN_AFN0FF1//�ļ���װδ�յ����ݶ�
//4BYTE ��� BIN 4
//128BYTE ���ڸ����ݶ�δ�յ���־ BS8 128
#define ADDR_FTPFileFlags     ADDR_AFN0CF14+4+128//FTP�ļ����ر�־
//4BYTE ��
//1BYTE �����ļ�����;0-x,=MaxFileNum��FTP,=(MaxFileNum+1)����֡,=(MaxFileNum+2)���������ʾ,=(MaxFileNum+3)����������ʾ
//1BYTE MS�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE OS�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE Meter�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE Terminal�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE Menu�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE Font�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE Module�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//1BYTE ?�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
//8BYTE 
#define ADDR_AFN0FF1_Rx     ADDR_FTPFileFlags+5+16//FTP����֡��ǰ256�ֽڱ���
//256BYTE
#define ADDR_AFN0FF1_Tx     ADDR_AFN0FF1_Rx+256//FTP����֡��ǰ18�ֽڱ���
//18BYTE
#define ADDR_UART_PORT_SET     ADDR_AFN0FF1_Tx+18//ͨ�ſ�����
//UART0 (RS232)
//1BYTE ���� 0=�ն˼��ӿ�,!=0�ն�ά����
//1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE ����
//1BYTE ����
//UART2(RS485-1)
//1BYTE ���� 0=�ն�ά����,!=0�ն˳����
//1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE ����
//1BYTE ����
//UART3(RS485-2)
//1BYTE���� 0=�ն�ά����,!=0�ն˳����
//1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE ����
//1BYTE ����
//UART4(RS485-3)
//1BYTE ���� 0=�ն�ά����,!=0�ն˳����
//1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE ����
//1BYTE ����
//UART5(����)
//1BYTE ���� 0=�ն�ά����,!=0�ն˳����
//1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
//1BYTE ����
//1BYTE ����
#define LEN_UART_PORT_SET     4


#define ADDR_Com_EFLASH_BASEEnd     (ADDR_UART_PORT_SET+(LEN_UART_PORT_SET*5))


#if (USER/100)==5//�Ϻ�
#define ADDR_AFN0FF97     ADDR_Com_EFLASH_BASEEnd
//1byte ������־:0=����F97����,1=��F97�����
//FTP������IP��ַ	BIN	6
//FTP������IP��ַ�˿ں�	BIN	2
//FTP������IP���õ�ַ	BIN	6
//FTP������IP���õ�ַ�˿ں�	BIN	2
//�û���	BIN	10
//�û���¼����	BIN	10
//�����������ļ�·��	ASCII	20
//���÷������ļ�·��	ASCII	20
//�����ļ���	ASCII	10
//�����ļ���1	ASCII	10
//�����ļ���2	ASCII	10
//�����ļ���3	ASCII	10
//�����ļ���4	ASCII	10
#define ADDR_AFN0FF98     ADDR_AFN0FF97+127
//����ǰ�ն�����汾��	ASCII	10
//�������ն�����汾��	ASCII	10
//����ʱ�䣺��ʱ������	����¼A.15	5
#define ADDR_Com_EFLASH_End     (ADDR_AFN0FF98+25)
#else
#define ADDR_Com_EFLASH_End     ADDR_Com_EFLASH_BASEEnd
#endif


#define LEN_Com_EFLASH     (128*1024)//�޶��ܳ����ֽ���(1������)
#if (ADDR_Com_EFLASH_End-ADDR_Com_EFLASH_Start)>LEN_Com_EFLASH
#error ADDR_Com_EFLASH_End Over
#endif
//�������ݽ���--------------------------------------------------------------------------------------------





//���ܱ�����----------------------------------------------------------------------------------------------
#define ADDR_Meter_EFLASH_Start     (ADDR_Com_EFLASH_Start+LEN_Com_EFLASH)
#if ((Project/100)==5)||((Project/100)==6)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
#define ADDR_Meter_EFLASH_End     (ADDR_Meter_EFLASH_Start+0)
#define LEN_Meter_EFLASH     0//�޶��ܳ����ֽ���
#else
#include "EFLASH_Meter698.h"
	#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
		#define LEN_Meter_EFLASH     (512*1024)//�޶��ܳ����ֽ���
	#else
		#define LEN_Meter_EFLASH     (6*1024*1024)//�޶��ܳ����ֽ���
	#endif
#endif

#if (ADDR_Meter_EFLASH_End-ADDR_Meter_EFLASH_Start)>LEN_Meter_EFLASH
#error ADDR_Meter_EFLASH_End Over
#endif
//���ܱ����ݽ���------------------------------------------------------------------------------------------





//�ն�����-----------------------------------------------------------------------------------------------
#define ADDR_Terminal_EFLASH_Start     (ADDR_Meter_EFLASH_Start+LEN_Meter_EFLASH)
#if ((Project/100)==0)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Terminal_EFLASH_End     (ADDR_Terminal_EFLASH_Start+0)
	#define LEN_Terminal_EFLASH     0//�޶��ܳ����ֽ���
#else
	#if NAND_1G==1//0=û,1=��
		#define LEN_Terminal_EFLASH     (((128-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		//NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		//2M ��ȥDownload,����,COM
		//6M ��ȥMeter
		//1M ��ȥLOG
		//����ȥ33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (91*1024*1024)
	#endif
	#if NAND_2G==1//0=û,1=��
	 #ifndef LINUX
		#define LEN_Terminal_EFLASH     (((256-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		//NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		//2M ��ȥDownload,����,COM
		//6M ��ȥMeter
		//1M ��ȥLOG
		//����ȥ33M
			#if (USER/100)==14//����
		#define LEN_FLASH_DL698_SET     (2*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (219*1024*1024)
			#else
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (220*1024*1024)
			#endif
			
	 #else//#ifndef LINUX
		#define LEN_Terminal_EFLASH     (((256-13)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		//NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ32M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		//2M nand_program
		//4M ��ȥDownload,����,COM
		//6M ��ȥMeter
		//1M ��ȥLOG
		//����ȥ53M
			#if (USER/100)==14//����
		#define LEN_FLASH_DL698_SET     (2*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (199*1024*1024)
			#else
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (200*1024*1024)
			#endif
	 #endif
	#endif
	#if NAND_4G==1//0=û,1=��
		#define LEN_Terminal_EFLASH     (((512-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		//NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		//2M ��ȥDownload,����,COM
		//6M ��ȥMeter
		//1M ��ȥLOG
		//����ȥ33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (476*1024*1024)
	#endif
	#if NAND_8G==1//0=û,1=��
		#define LEN_Terminal_EFLASH     (((1024-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		//NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		//NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		//2M ��ȥDownload,����,COM
		//6M ��ȥMeter
		//1M ��ȥLOG
		//����ȥ33M
		#define LEN_FLASH_DL698_SET     (1*1024*1024)
		#define LEN_FLASH_DL698_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (732*1024*1024)
	#endif
	#include "EFLASH_Terminal698.h"
#endif

#if (ADDR_Terminal_EFLASH_End-ADDR_Terminal_EFLASH_Start)>LEN_Terminal_EFLASH
#error ADDR_Terminal_EFLASH_End Over
#endif
//�ն����ݽ���--------------------------------------------------------------------------------------------


#define ADDR_EFLASH_End     (ADDR_Terminal_EFLASH_Start+LEN_Terminal_EFLASH)

#if NAND_1G==1//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x08000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_2G==1//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x10000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_4G==1//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x20000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_8G==1//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x40000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif

//LOG����(���ǹ̶���nand�����1M�ռ�)----------------------------------------------------------------------------

#if NAND_1G==1//0=û,1=��
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x08000000-0x00100000)
#endif
#if NAND_2G==1//0=û,1=��
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x10000000-0x00100000)
	#define LENmax_LOG     (3*1024*1024)//ÿ���ļ����ߴ�����6M
#endif
#if NAND_4G==1//0=û,1=��
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x20000000-0x00100000)
#endif
#if NAND_8G==1//0=û,1=��
	#define ADDR_LOG_START     ADDR_EFLASH_Start+(0x40000000-0x00100000)
#endif

#define ADDR_LOG_698        ADDR_LOG_START
//128K space

//128K space
//128K space
#define ADDR_LOG_1376_1     ADDR_LOG_START+(3*128*1024)
//128K
#define ADDR_LOG_1376_2     (ADDR_LOG_1376_1+(128*1024))
//128K
#define ADDR_LOG_1376_3     (ADDR_LOG_1376_2+(128*1024))
//128K
#define ADDR_LOG_645_1997     (ADDR_LOG_1376_3+(128*1024))
//128K
#define ADDR_LOG_645_2007     (ADDR_LOG_645_1997+(128*1024))
//128K
#define ADDR_LOG_END     (ADDR_LOG_645_2007+(128*1024))

//LOG���ݽ���-----------------------------------------------------------------------------------------------









#endif





















