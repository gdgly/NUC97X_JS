


#ifndef EFLASH_H
#define EFLASH_H

#define ADDR_EFLASH_Start     0x30000000
// NAND_FILE_BLOCK_COUNT*128*1024�ֽڳ����ļ�ϵͳ
// NAND_REPLACE_BLOCK_COUNT*128*1024�ֽڳ��ȴ�����滻
#define ADDR_Download_ENTRY     ADDR_EFLASH_Start+((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024)//���ַ����
#define LENmax_Download         (1024*1024)
// 512K MS,OS,Router����ʱ����


// ����ʱ���ݱ�����--------------------------------------------------------------------------------
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
// ����ʱ���ݱ���������----------------------------------------------------------------------------


//���ܱ�����----------------------------------------------------------------------------------------------
#define ADDR_Meter_EFLASH_Start     ADDR_DISK_End 		// (ADDR_Com_EFLASH_Start+LEN_Com_EFLASH)
#if ((Project/100)==5)||((Project/100)==6)				// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Meter_EFLASH_End     (ADDR_Meter_EFLASH_Start+0)
	#define LEN_Meter_EFLASH     0						// �޶��ܳ����ֽ���
#else
	#include "EFLASH_MeterCSG.h"
	#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))		// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		#define LEN_Meter_EFLASH     (512*1024)			// �޶��ܳ����ֽ���
	#else
		#define LEN_Meter_EFLASH     (6*1024*1024)							// �޶��ܳ����ֽ���
	#endif
#endif

#if (ADDR_Meter_EFLASH_End-ADDR_Meter_EFLASH_Start)>LEN_Meter_EFLASH
#error ADDR_Meter_EFLASH_End Over
#endif
//���ܱ����ݽ���------------------------------------------------------------------------------------------





//�ն�����-----------------------------------------------------------------------------------------------
#define ADDR_Terminal_EFLASH_Start     (ADDR_Meter_EFLASH_Start+LEN_Meter_EFLASH)
#if ((Project/100)==0)					// ����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#define ADDR_Terminal_EFLASH_End     (ADDR_Terminal_EFLASH_Start+0)
	#define LEN_Terminal_EFLASH     0	// �޶��ܳ����ֽ���
#else
	#if NAND_1G==1//0=û,1=��
		#define LEN_Terminal_EFLASH     (((128-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))	// �޶��ܳ����ֽ���
		// NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		// 2M ��ȥDownload,����,COM
		// 6M ��ȥMeter
		// 1M ��ȥLOG
		// ����ȥ33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (91*1024*1024)
	#endif
	#if NAND_2G==1	//0=û,1=��
		#define LEN_Terminal_EFLASH     (((256-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))	// �޶��ܳ����ֽ���
		// NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		// 2M ��ȥDownload,����,COM
		// 6M ��ȥMeter
		// 1M ��ȥLOG
		// ����ȥ33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (220*1024*1024)
	#endif
	#if NAND_4G==1	//0=û,1=��
		#define LEN_Terminal_EFLASH     (((512-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		// NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		// 2M ��ȥDownload,����,COM
		// 6M ��ȥMeter
		// 1M ��ȥLOG
		// ����ȥ33M
		#define LEN_FLASH_QCSG_SET    	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (476*1024*1024)
	#endif
	#if NAND_8G==1	//0=û,1=��
		#define LEN_Terminal_EFLASH     (((1024-2-6-1)*1024*1024)-((NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT)*128*1024))//�޶��ܳ����ֽ���
		// NAND_FILE_BLOCK_COUNT*128*1024  ��ȥ�ļ�ϵͳ16M
		// NAND_REPLACE_BLOCK_COUNT*128*1024 ��ȥ���滻8M
		// 2M ��ȥDownload,����,COM
		// 6M ��ȥMeter
		// 1M ��ȥLOG
		// ����ȥ33M
		#define LEN_FLASH_QCSG_SET     	(1*1024*1024)
		#define LEN_FLASH_QCSG_DATA     (1*1024*1024)
		#define LEN_FLASH_FILE          (732*1024*1024)
	#endif
	#include "EFLASH_TerminalCSG.h"
#endif

#if (ADDR_Terminal_EFLASH_End-ADDR_Terminal_EFLASH_Start)>LEN_Terminal_EFLASH
#error ADDR_Terminal_EFLASH_End Over
#endif
// �ն����ݽ���--------------------------------------------------------------------------------------------


#define ADDR_EFLASH_End     (ADDR_Terminal_EFLASH_Start+LEN_Terminal_EFLASH)

#if NAND_1G==1		//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x08000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_2G==1		//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x10000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_4G==1		//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x20000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif
#if NAND_8G==1		//0=û,1=��
	#if ((ADDR_EFLASH_End-ADDR_EFLASH_Start)>(0x40000000-0x00100000))
	#error ADDR_EFLASH_End Over
	#endif
#endif

// LOG����(���ǹ̶���nand�����1M�ռ�)----------------------------------------------------------------------------

#if NAND_1G==1					// 0=û,1=��
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x08000000-0x00100000)
#endif
#if NAND_2G==1					// 0=û,1=��
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x10000000-0x00100000)
	#define LENmax_LOG     (3*1024*1024)//ÿ���ļ����ߴ�����6M
#endif
#if NAND_4G==1					// 0=û,1=��
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x20000000-0x00100000)
#endif
#if NAND_8G==1					// 0=û,1=��
	#define ADDR_LOG_START     	ADDR_EFLASH_Start+(0x40000000-0x00100000)
#endif

#define ADDR_LOG_698        ADDR_LOG_START
// 128K space
// 128K space
// 128K space
#define ADDR_LOG_1376_1     	(ADDR_LOG_START+(3*128*1024))
// 128K
#define ADDR_LOG_1376_2     	(ADDR_LOG_1376_1+(128*1024))
// 128K
#define ADDR_LOG_1376_3     	(ADDR_LOG_1376_2+(128*1024))
// 128K
#define ADDR_LOG_645_1997     	(ADDR_LOG_1376_3+(128*1024))
// 128K
#define ADDR_LOG_645_2007     	(ADDR_LOG_645_1997+(128*1024))
// 128K
#define ADDR_LOG_END     		(ADDR_LOG_645_2007+(128*1024))

//LOG���ݽ���-----------------------------------------------------------------------------------------------









#endif





















