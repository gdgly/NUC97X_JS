
#ifndef EFLASH_Terminal_H
#define EFLASH_Terminal_H







#define ADDR_AFN04_Start     (ADDR_Terminal_EFLASH_Start)
#include "AFN04.h"
#ifdef IC_STM32F4xx
	#if ((Project/100)==2)//������
		#if (USER/100)==14//����ϵ
			#define ADDR_AFN54_Start      (ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04)
			#include "AFN54.h"
			#define ADDR_AFN05_Start      (ADDR_AFN54_Start+LEN_Terminal_EFLASH_AFN54)
		#endif
	#endif
#endif
#ifndef	ADDR_AFN05_Start
#define ADDR_AFN05_Start      (ADDR_AFN04_Start+LEN_Terminal_EFLASH_AFN04)
#endif
#include "AFN05.h"
#define ADDR_AFN0E_Start      (ADDR_AFN05_Start+LEN_Terminal_EFLASH_AFN05)
#include "AFN0E.h"
#define ADDR_AFN0C_HourCongeal_Start      (ADDR_AFN0E_Start+LEN_Terminal_EFLASH_AFN0E)
#include "AFN0C_HourCongeal.h"

	#define	ADDR_AFN0D_Source_Start     (ADDR_AFN0C_HourCongeal_Start+LEN_Terminal_EFLASH_AFN0C_HourCongeal)//��ʼ��ַ
	#include "AFN0D_Source.h"//2���������¶���ԭʼ����

#define ADDR_AFN0D_Start      (ADDR_AFN0D_Source_Start+LEN_Terminal_EFLASH_AFN0D_Source)
#include "AFN0D.h"


//�ն�ÿСʱRAM���ݱ��浽FLASH--------------------------------------------------------------------------------
#if (((Project/100)==2)&&((USER/100)==14))//����ϵ
	#ifdef IC_STM32F4xx
		#define ADDR_HOURFLASH_START     (ADDR_AFN0D_Start+LEN_Terminal_EFLASH_AFN0D+LEN_Terminal_EFLASH_AFN56)
	#endif
	#ifdef IC_NUC9xx
		#define ADDR_HOURFLASH_START     (ADDR_AFN0D_Start+LEN_Terminal_EFLASH_AFN0D+LEN_Terminal_EFLASH_AFN54+LEN_Terminal_EFLASH_AFN56)
	#endif
#endif
#ifndef ADDR_HOURFLASH_START
	#define ADDR_HOURFLASH_START     (ADDR_AFN0D_Start+LEN_Terminal_EFLASH_AFN0D)
#endif
#define LEN_HOURFLASH     (24*1024)
#define ADDR_HOURFLASH_END     (ADDR_HOURFLASH_START+LEN_HOURFLASH)
//--------------------------------------------------------------------------------------------------------


#define ADDR_FLASH_FILE_START     ((ADDR_HOURFLASH_END+0x1ffff)&0xfffe0000)//����䵥Ԫ�ֽ�������!!!
#define msFILE//����ms�ļ�ϵͳ
#define LEN_msFILEunit     (128*1024)//�ļ����䵥Ԫ



//����
#define ADDR_Terminal_EFLASH_End     (ADDR_FLASH_FILE_START+LEN_FLASH_FILE)




#endif

