
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_UART.h"

#include "../DL698/Contrl_Manage.h"

#include "../DL698/DL698_JSON.h"
//#include "../DL698/cJSON.h"
#include "../Calculate/ascii.h"
#include "../Calculate/Calculate.h"

#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#pragma  diag_suppress 870     //·ÀÖ¹KEIL±¨¾¯¸æ
//½­ËÕÉÏ±¨È«ÍøÊµÊ±Êı¾İµÄOAD
__align(4) const u16 fcstab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

const u32 RealDataOAD_JSON[]=//ÊµÊ±JSONÎÄ¼ş´ò°üµÄÊı¾İÏî
{
	0x40020200,	//²É¼¯´æ´¢Ê±±ê
	0x20000201,	//AÏàµçÑ¹
	0x20000202,	//BÏàµçÑ¹
	0x20000203,	//CÏàµçÑ¹

	0x20010201,	//AÏàµçÁ÷
	0x20010202,	//BÏàµçÁ÷
	0x20010203,	//CÏàµçÁ÷

	0x20010400,	//ÁãÏßµçÁ÷
	0x20010500,	//Ê£ÓàµçÁ÷

	0x20040201,	//×ÜÓĞ¹¦¹¦ÂÊ
	0x20040202,	//AÏàÓĞ¹¦¹¦ÂÊ
	0x20040203,	//BÏàÓĞ¹¦¹¦ÂÊ
	0x20040204,	//CÏàÓĞ¹¦¹¦ÂÊ
	
	0x43140506,	//(ÊÒÍâÎÂ¶È)
	0x43140505,	//(ÊÒÄÚÎÂ¶È)
	0x43140501,	//(¿Õµ÷ÉèÖÃÎÂ¶È)
	0x43140400,	//(¿Õµ÷¿ª¹Ø»ú×´Ì¬)
	0x43110502,	//(ÈÈË®Æ÷¼ÓÈÈÎÂ¶È)
	0x43110400,	//(ÈÈË®Æ÷¿ª¹Ø»ú×´Ì¬)

	0x00100201, //ÕıÓĞ×Ü
	0x00100203,	//ÕıÓĞ·æ
	0x00100205,	//ÕıÓĞ¹È

	0x00200201, //·´ÓĞ×Ü
	0x00200203,	//·´ÓĞ·æ
	0x00200205,	//·´ÓĞ¹È
};		
//½­ËÕÉÏ±¨È«ÍøÊµÊ±Êı¾İµÄOADÊıÁ¿
const u32 RealDataOAD_JSONSIZE=sizeof(RealDataOAD_JSON)/sizeof(u32);

typedef struct
{
	u16 OI;
	u16 POINTNUM;//Ğ¡ÊıµãÎ»Êı
}POINTNUM_type;

const POINTNUM_type OAD_PointNUM_list[]=//OAD¶ÔÓ¦Ğ¡ÊıµãÎ»Êı
{
	{0x2000,	1},//	µçÑ¹	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£ºV£¬»»Ëã£º-1
	{0x2001,	3},//	µçÁ÷	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºA »»Ëã£º-3
	{0x2002,	1},//	µçÑ¹Ïà½Ç	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£º¶È£¬»»Ëã£º-1
	{0x2003,	1},//	µçÑ¹µçÁ÷Ïà½Ç	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£º¶È£¬»»Ëã£º-1
	{0x2004,	1},//	ÓĞ¹¦¹¦ÂÊ	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºW£¬»»Ëã£º-1
	{0x2005,	1},//	ÎŞ¹¦¹¦ÂÊ	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºvar£¬»»Ëã£º-1
	{0x2006,	1},//	ÊÓÔÚ¹¦ÂÊ	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºVA£¬»»Ëã£º-1
	{0x2007,	1},//	Ò»·ÖÖÓÆ½¾ùÓĞ¹¦¹¦ÂÊ Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºW£¬»»Ëã£º-1
	{0x2008,	1},//	Ò»·ÖÖÓÆ½¾ùÎŞ¹¦¹¦ÂÊ Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºvar£¬»»Ëã£º-1
	{0x2009,	1},//	Ò»·ÖÖÓÆ½¾ùÊÓÔÚ¹¦ÂÊ Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºVA£¬»»Ëã£º-1
	{0x200A,	3},//	¹¦ÂÊÒòÊı	Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£ºÎŞ£¬»»Ëã£º-3
	{0x200B,	2},//	µçÑ¹²¨ĞÎÊ§Õæ¶È	Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x200C,	2},//	µçÁ÷²¨ĞÎÊ§Õæ¶È	Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x200D,	2},//	µçÑ¹Ğ³²¨º¬ÓĞÁ¿£¨×Ü¼° 2?n ´Î£© Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x200E,	2},//	µçÁ÷Ğ³²¨º¬ÓĞÁ¿£¨×Ü¼° 2?n ´Î£© Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x200F,	2},//	µçÍøÆµÂÊ	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£ºHz£¬»»Ëã£º-2
	 
	
	//¶ÔÏó±êÊ¶ OI ½Ó¿ÚÀà IC	¶ÔÏóÃû³Æ	ÊµÀıµÄ¶ÔÏóÊôĞÔ¼°·½·¨¶¨Òå
	{0x2010,	1},//	±íÄÚÎÂ¶È	Êı¾İÀàĞÍ£ºlong£¬µ¥Î»£º¡æ£¬»»Ëã£º-1
	{0x2011,	2},//	Ê±ÖÓµç³ØµçÑ¹	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£ºV£¬»»Ëã£º-2
	{0x2012,	2},//	Í£µç³­±íµç³ØµçÑ¹	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£ºV£¬»»Ëã£º-2
	//{0x2013,	0},//	Ê±ÖÓµç³Ø¹¤×÷Ê±¼ä	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£º·ÖÖÓ£¬ÎŞ»»Ëã
	//2014	6	µçÄÜ±íÔËĞĞ×´Ì¬×Ö	Êı¾İÀàĞÍ£ºarray bit-string£¬ÎŞµ¥Î»£¬ÎŞ»»Ëã£¬°üÀ¨µçÄÜ ±íÔËĞĞ×´Ì¬×Ö 1¡­7£¬¼û¸½  Â¼ G
	
	//2015	6	µçÄÜ±í¸úËæÉÏ±¨×´Ì¬×Ö	Êı¾İÀàĞÍ£ºbit-string(SIZE(32))£¬ÎŞµ¥Î»£¬ÎŞ»»Ëã£¬¼û¸½ Â¼  G
	//ÊôĞÔ 4£¨µçÄÜ±í¸úËæÉÏ±¨Ä£Ê½×Ö£©¡Ë= bit-string(SIZE(32)) ·½·¨ 127£¨È·ÈÏµçÄÜ±í¸úËæÉÏ±¨×´Ì¬×Ö£©¡Ë=
	//bit-string(SIZE(32))
	{0x2017,	4},//	µ±Ç°ÓĞ¹¦ĞèÁ¿	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºkW£¬»»Ëã£º-4
	{0x2018,	4},//	µ±Ç°ÎŞ¹¦ĞèÁ¿	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºkvar£¬»»Ëã£º-4
	{0x2019,	4},//	µ±Ç°ÊÓÔÚĞèÁ¿	Êı¾İÀàĞÍ£ºdouble-long£¬µ¥Î»£ºkVA£¬»»Ëã£º-4
	{0x201A,	4},//	µ±Ç°µç¼Û	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºÔª/kWh£¬»»Ëã£º-4
	{0x201B,	4},//	µ±Ç°·ÑÂÊµç¼Û	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºÔª/kWh£¬»»Ëã£º-4
	{0x201C,	4},//	µ±Ç°½×Ìİµç¼Û	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºÔª/kWh£¬»»Ëã£º-4
	//201E	8	ÊÂ¼ş·¢ÉúÊ±¼ä	Êı¾İÀàĞÍ£ºdate_time_s
	//2020	8	ÊÂ¼ş½áÊøÊ±¼ä	Êı¾İÀàĞÍ£ºdate_time_s
	//2021	8	Êı¾İ¶³½áÊ±¼ä	Êı¾İÀàĞÍ£ºdate_time_s
	//2022	8	ÊÂ¼ş¼ÇÂ¼ĞòºÅ	Êı¾İÀàĞÍ£ºdouble-long-unsigned
	//2023	8	¶³½á¼ÇÂ¼ĞòºÅ	Êı¾İÀàĞÍ£ºdouble-long-unsigned
	//2024	8	ÊÂ¼ş·¢ÉúÔ´ ¾ßÌå¶ÔÏó¶¨Òå¡£
	//2025	8	ÊÂ¼şµ±Ç°Öµ structure
	//{
	//ÊÂ¼ş·¢Éú´ÎÊı double-long-unsigned£¬
	//ÊÂ¼şÀÛ¼ÆÊ±¼ä double-long-unsigned£¨µ¥Î»£ºÃë£¬ÎŞ»» Ëã£©
	//}
	{0x2026,	2},//	µçÑ¹²»Æ½ºâÂÊ	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x2027,	2},//	µçÁ÷²»Æ½ºâÂÊ	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x2028,	2},//	¸ºÔØÂÊ	Êı¾İÀàĞÍ£ºlong-unsigned£¬µ¥Î»£º%£¬»»Ëã£º-2
	{0x2029,	2},//	°²Ê±Öµ	ÊôĞÔ 2 °²Ê±ÊıÖµ¡Ë=array Ïà°²Ê±ÖµÏà°²Ê±Öµ¡Ë=double-long-unsigned£¬µ¥Î»£ºAh£¬»»Ëã£º-2 Ïà°²Ê±Öµ°ü×Ü¡¢A¡¢B¡¢C Ïà¡£
	//202A	8	Ä¿±ê·şÎñÆ÷µØÖ·	ÊôĞÔ 2¡Ë=TSA
	 
	
	//¶ÔÏó±êÊ¶ OI ½Ó¿ÚÀà IC	¶ÔÏóÃû³Æ	ÊµÀıµÄ¶ÔÏóÊôĞÔ¼°·½·¨¶¨Òå
	{0x202C,	2},//	£¨µ±Ç°£©Ç®°üÎÄ¼ş	ÊıÖµ¡Ë=structure
	//{
	//Ê£Óà½ğ¶î	double-long-unsigned£¨µ¥Î»£ºÔª£¬»»Ëã£º-2£©£¬ ¹ºµç´ÎÊı	double-long-unsigned
	//}
	{0x202D,	2},//	£¨µ±Ç°£©Í¸Ö§½ğ¶î	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºÔª£¬»»Ëã£º-2
	{0x202E,	2},//	ÀÛ¼Æ¹ºµç½ğ¶î	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºÔª£¬»»Ëã£º-2
	{0x2031,	2},//	ÔÂ¶ÈÓÃµçÁ¿ ÊôĞÔ 2 ÓÃµçÁ¿¡Ë=double-long-unsigned£¬ µ¥Î»£ºkWh£¬»»Ëã£º-2
	{0x2032,	2},//	½×Ìİ½áËãÓÃµçÁ¿	ÊôĞÔ 2 ÓÃµçÁ¿¡Ë=double-long-unsigned£¬ µ¥Î»£ºkWh£¬»»Ëã£º-2
	//2040	6	¿ØÖÆÃüÁîÖ´ĞĞ×´Ì¬×Ö Êı¾İÀàĞÍ£ºbit-string(SIZE(16))£¬ÎŞµ¥Î»£¬ÎŞ»»Ëã
	//2041	6	¿ØÖÆÃüÁî´íÎó×´Ì¬×Ö Êı¾İÀàĞÍ£ºbit-string(SIZE(16))£¬ÎŞµ¥Î»£¬ÎŞ»»Ëã
	//2100	14	·ÖÖÓÇø¼äÍ³¼Æ	Í³¼ÆÖÜÆÚµ¥Î»Îª·ÖÖÓ
	//2101	14	Ğ¡Ê±Çø¼äÍ³¼Æ	Í³¼ÆÖÜÆÚµ¥Î»ÎªĞ¡Ê±
	//2102	14	ÈÕÇø¼äÍ³¼Æ Í³¼ÆÖÜÆÚµ¥Î»ÎªÈÕ
	//2103	14	ÔÂÇø¼äÍ³¼Æ Í³¼ÆÖÜÆÚµ¥Î»ÎªÔÂ
	//2104	14	ÄêÇø¼äÍ³¼Æ Í³¼ÆÖÜÆÚµ¥Î»ÎªÄê
	//2110	15	·ÖÖÓÆ½¾ù	Í³¼ÆÖÜÆÚµ¥Î»Îª·ÖÖÓ
	//2111	15	Ğ¡Ê±Æ½¾ù	Í³¼ÆÖÜÆÚµ¥Î»ÎªÊ±
	//2112	15	ÈÕÆ½¾ù	Í³¼ÆÖÜÆÚµ¥Î»ÎªÈÕ
	//2113	15	ÔÂÆ½¾ù	Í³¼ÆÖÜÆÚµ¥Î»ÎªÔÂ
	//2114	15	ÄêÆ½¾ù	Í³¼ÆÖÜÆÚµ¥Î»ÎªÄê
	//2120	16	·ÖÖÓ¼«Öµ	Í³¼ÆÖÜÆÚµ¥Î»Îª·ÖÖÓ
	//2121	16	Ğ¡Ê±¼«Öµ	Í³¼ÆÖÜÆÚµ¥Î»ÎªÊ±
	//2122	16	ÈÕ¼«Öµ	Í³¼ÆÖÜÆÚµ¥Î»ÎªÈÕ
	//2123	16	ÔÂ¼«Öµ	Í³¼ÆÖÜÆÚµ¥Î»ÎªÔÂ
	//2124	16	Äê¼«Öµ	Í³¼ÆÖÜÆÚµ¥Î»ÎªÄê
	//2131	6	µ±ÔÂ A ÏàµçÑ¹ºÏ¸ñÂÊ ÊôĞÔ 2£¨µçÑ¹ºÏ¸ñÂÊÊı¾İ£©¡Ë=structure
	//{
	//µ±ÈÕµçÑ¹ºÏ¸ñÂÊ	µçÑ¹ºÏ¸ñÂÊ£¬
	//µ±ÔÂµçÑ¹ºÏ¸ñÂÊ	µçÑ¹ºÏ¸ñÂÊ
	//}
	
	
	//µçÑ¹ºÏ¸ñÂÊ¡Ë=structure
	//{
	//µçÑ¹¼à²âÊ±¼ä double-long-unsigned£¨µ¥Î»£º·ÖÖÓ£¬ÎŞ»» Ëã£©£¬
	 
	
	//¶ÔÏó±êÊ¶ OI ½Ó¿ÚÀà IC	¶ÔÏóÃû³Æ	ÊµÀıµÄ¶ÔÏóÊôĞÔ¼°·½·¨¶¨Òå
	{0x2131,	2},//	µ±ÔÂ A ÏàµçÑ¹ºÏ¸ñÂÊ µçÑ¹ºÏ¸ñÂÊ long-unsigned£¨µ¥Î»£º%£¬»»Ëã£º-2£©£¬ µçÑ¹³¬ÏŞÂÊ	long-unsigned£¨µ¥Î»£º%£¬»»Ëã£º-2£©£¬ µçÑ¹³¬ÉÏÏŞÊ±¼ä  double-long-unsigned£¨µ¥Î»£º·ÖÖÓ£¬ÎŞ»»Ëã£©£¬µçÑ¹³¬ÏÂÏŞÊ±¼ä double-long-unsigned£¨µ¥Î»£º·ÖÖÓ£¬ ÎŞ»»Ëã£©}
	{0x2132,	2},//	µ±ÔÂ B ÏàµçÑ¹ºÏ¸ñÂÊ Í¬ 2131
	{0x2133,	2},//	µ±ÔÂ C ÏàµçÑ¹ºÏ¸ñÂÊ Í¬ 2131
	//2140	2	ÈÕ×î´óÓĞ¹¦¹¦ÂÊ¼°·¢ÉúÊ±¼ä	×î´ó¹¦ÂÊ¼°·¢ÉúÊ±¼ä¡Ë=structure
	//{
	//×î´ó¹¦ÂÊÖµ double-long-unsigned£¬ ·¢ÉúÊ±¼ä	date_time_s
	//}
	//¹¦ÂÊµ¥Î»£ºkW£¬»»Ëã£º-4
	{0x2141,	4},//	ÔÂ×î´óÓĞ¹¦¹¦ÂÊ¼°·¢ÉúÊ±¼ä	Í¬ 2140¡£
	//2200	6	Í¨ĞÅÁ÷Á¿	ÊıÖµ¡Ë=structure
	//{
	//µ±ÈÕÍ¨ĞÅÁ÷Á¿ double-long-unsigned£¬ µ±ÔÂÍ¨ĞÅÁ÷Á¿ double-long-unsigned
	//}
	//µ¥Î»£ºbyte£¬»»Ëã£º0
	//2203	6	¹©µçÊ±¼ä	ÊıÖµ¡Ë=structure
	//{
	//ÈÕ¹©µçÀÛ¼ÆÊ±¼ä double-long-unsigned£¬ ÔÂ¹©µçÀÛ¼ÆÊ±¼ä double-long-unsigned
	//}
	//µ¥Î»£º·ÖÖÓ£¬»»Ëã£º0
	//2204	6	¸´Î»´ÎÊı	ÊıÖµ¡Ë=structure
	//{
	//ÈÕ¸´Î»ÀÛ¼Æ´ÎÊı long-unsigned£¬ ÔÂ¸´Î»ÀÛ¼Æ´ÎÊı long-unsigned
	//}
	//2301	23	×Ü¼Ó×é 1 
	//2302	23	×Ü¼Ó×é 2 
	//2303	23	×Ü¼Ó×é 3 
	//2304	23	×Ü¼Ó×é 4 
	//2305	23	×Ü¼Ó×é 5 
	 
	
	//¶ÔÏó±êÊ¶ OI ½Ó¿ÚÀà IC	¶ÔÏóÃû³Æ	ÊµÀıµÄ¶ÔÏóÊôĞÔ¼°·½·¨¶¨Òå
	//2306	23	×Ü¼Ó×é 6 
	//2307	23	×Ü¼Ó×é 7 
	//2308	23	×Ü¼Ó×é 8 
	//2401	12	Âö³å¼ÆÁ¿ 1	
	//2402	12	Âö³å¼ÆÁ¿ 2	
	//2403	12	Âö³å¼ÆÁ¿ 3	
	//2404	12	Âö³å¼ÆÁ¿ 4	
	//2405	12	Âö³å¼ÆÁ¿ 5	
	//2406	12	Âö³å¼ÆÁ¿ 6	
	//2407	12	Âö³å¼ÆÁ¿ 7	
	//2408	12	Âö³å¼ÆÁ¿ 8	
	//2500	6	ÀÛ¼ÆË®£¨ÈÈ£©Á÷Á¿	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºm3£¬»»Ëã£º-4
	//£¨×¢£ºË®±íºÍÈÈ±í¹²ÓÃ£©
	{0x2501,	4},//	ÀÛ¼ÆÆøÁ÷Á¿ Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºm3£¬»»Ëã£º-4
	{0x2502,	2},//	ÀÛ¼ÆÈÈÁ¿	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºJ£¬»»Ëã£º-2
	{0x2503,	2},//	ÈÈ¹¦ÂÊ	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºJ/h£¬»»Ëã£º-2
	//2504	6	ÀÛ¼Æ¹¤×÷Ê±¼ä	Êı¾İÀàĞÍ£ºdouble-long-unsigned£¬µ¥Î»£ºĞ¡Ê±£¬»»Ëã£º0
	{0x2505,	2},//	Ë®ÎÂ	ÊıÖµ¡Ë=structure
	//{
	//¹©Ë®ÎÂ¶È double-long-unsigned£¬ »ØË®ÎÂ¶È double-long-unsigned
	//}
	//µ¥Î»£º¡æ£¬»»Ëã£º-2¡£
	//2506	6	£¨ÒÇ±í£©×´Ì¬ ST	ÊıÖµ¡Ë=structure
	///{
	//·§ÃÅ×´Ì¬  enum{¿ª£¨0£©£¬¹Ø£¨1£©£¬±£Áô£¨2£©£¬Òì³££¨3£©}£¬ µç³ØµçÑ¹  enum{Õı³££¨0£©£¬Ç·Ñ¹£¨0£©}
	//}
};
const u32 OAD_PointNUMSIZE=sizeof(OAD_PointNUM_list)/sizeof(POINTNUM_type);



u32 Str_CRandLF(u8 *p)//»Ø³µºÍ»»ĞĞ
{
#ifdef UNFORMAT_JSON
	*p=0x0D;
	p++;
	*p=0x0A;

	return 2;
#else
	return 0;
#endif
}

u32 Str_CR(u8 *p)//»Ø³µ
{
	*p=0x0D;
	return 1;
}

u32 Str_LF(u8 *p)//»»ĞĞ
{
	*p=0x0A;
	return 1;
}

u32 Str_SPASE(u8 *p)//¿Õ¸ñ
{
	*p=0x20;
	return 1;
}

u32 Str_nSPASE(u8 *p,u32 NUM)//¿Õ¸ñ
{
#ifdef UNFORMAT_JSON
	u32 i;
	for(i=0;i<NUM;i++)
	{
		*p=0x20;
		p++;
	}
	return NUM;
#else
	return 0;
#endif
}

u32 Str_COMMA(u8 *p)//¶ººÅ
{
	*p=0x2C;
	return 1;
}

u32 Str_QM(u8 *p)//ÒıºÅ
{
	*p=0x22;
	return 1;
}

u32 Str_COL(u8 *p)//Ã°ºÅ
{
	*p=0x3A;
	return 1;
}
u32 Str_L_SQB(u8 *p)//×ó·½À¨ºÅ
{
	*p=0x5B;
	return 1;
}

u32 Str_R_SQB(u8 *p)//ÓÒ·½À¨ºÅ
{
	*p=0x5D;
	return 1;
}
u32 Str_L_CUB(u8 *p)//×ó»¨À¨ºÅ
{
	*p=0x7B;
	return 1;
}

u32 Str_R_CUB(u8 *p)//ÓÒ»¨À¨ºÅ
{
	*p=0x7D;
	return 1;
}

void myprintf(char *pStr,u32 param1,u32 param2,u32 param3)
{
#ifdef DEBUG_JSON
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf((char *)pStr,param1,param2,param3);
#endif
}

u32 mystrlen(const char *p)//¼ÆËã×Ö·û´®³¤¶È
{
	u32 i;
	i=0;
	while((*p!='\0')||(*p!=0))
	{
		i++;
		p++;
	}
	return i;
}

void ByteInverte(u8 *ps,u8 *pc,u32 Len)//psÖĞLen×Ö½Úµ¹ĞğÊä³öµ½pcÖĞ
{
	u32 i;
	u32 j;
	j=0;
	for(i=Len;i>0;i--)
	{
		pc[j]=ps[i-1];
		j++;
	}
}
u32 mymemcmp(u8 *p,u32 val,u32 Len)//±È½ÏÁ½¸öÊı¾İ,1²»ÏàÍ¬,0ÏàÍ¬
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		if(*p!=val)
		{
			return 1;
		}
		p++;
	}
	return 0;
}
void mystrcpy(char *p,const char *pc)//pc×Ö½Ú¿½±´µ½pÀïÃæÈ¥
{
	u32 pclen;
	u32 i;
	pclen=mystrlen(pc);
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncpy(char *p,const char *pc,u32 Len)//pc×Ö½Ú¿½±´n¸öµ½pÀïÃæÈ¥
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncat(char *p,const char *pc,u32 Len)//pºóÃæÁ¬½ÓLen³¤¶Èpc×Ö·û
{
	u32 plen;
	//u32 pclen;
	u32 i;
	plen=mystrlen(p);
	//pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Data_Ascii(u64 ida,u8 *p,u32 NUM)//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
{
	u32 i;
	u64 data;
	u32 Len=0;
	u8 tm;
	data=hex_bcd16(ida);
	if(0==data)
	{	
		*p=0x30;
		p++;
		Len++;
		for(i=0;i<NUM;i++)
		{			
			if(0==i)
			{
				*p='.';	
				p++;
				Len++;
				*p=0x30;
				Len++;
			}else
			{
				*p=0x30;
				Len++;
			}
			p++;
		}
	}else
	{
		for(i=16;i>0;i--)
		{
			tm=(data>>((i-1)*4))&0xf;
			if(NUM==i)
			{
				*p='.';	
				p++;	
				Len++;
			}
			if(0!=tm)
			{
				*p=tm+0x30;
				p++;
				Len++;
			}else
			{
				if(Len!=0||NUM+1==i)
				{
					*p=tm+0x30;
					p++;
					Len++;
				}
			}
		}
	}
	*p=0;
	return Len;
}

void mystrcat(char *p,const char *pc)//p×Ö·ûÁ¬½Ópcµ½p
{
	u32 plen;
	u32 pclen;
	u32 i;
	plen=mystrlen(p);
	pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Cmp_Ascii_Inverte(u8 *p,u8 *pc,u32 Byte)//×Ö½Úpc×ª³ÉascÂëµ¹Ğò¸úp±È½Ï
{
	u32 i;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pc[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

u32 Cmp_Ascii(u8 *p,u8 *pc,u32 Byte)//×Ö½Úpc×ª³ÉascÂë¸úp±È½Ï
{
	u32 i;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Add_StrToASCII_Inverte(u8 *pStr,u8 *pbuf,u32 Byte)//pbufÀïÃæµÄ×Ö½Úµ¹ĞòÊä³öµ½pstr²¢×ª³ÉascÂë
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pbuf[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Add_StrToASCII(u8 *pStr,u8 *pbuf,u32 Byte)//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pbuf[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Get698CurTime_Ascii(u8 *p8,u32 subD)//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë,¼õÈ¥subD
{
	u32 i;
	u32 x;
	u8 data;
	u8 *p8s;
	//Ğ´Ê±¼ä
	//Äê
	u32 YYMD;
	YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
	if(subD!=0)
	{
		for(i=0;i<subD;i++)
			YYMD=YYMD_Sub1D_hex(YYMD);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ	
	}
	
	p8s=(u8*)ADDR_4KWRITE_BUFF;	
	p8s[0]=YYMD>>24;
	p8s[1]=YYMD>>16;
	p8s[2]=YYMD>>8;
	p8s[3]=YYMD>>0;
	p8s[4]=Comm_Ram->DL698YMDHMS[4];//Ê±¼ä:Ê±
	p8s[5]=Comm_Ram->DL698YMDHMS[5];//Ê±¼ä:·Ö
	p8s[6]=Comm_Ram->DL698YMDHMS[5];//Ê±¼ä:Ãë
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(p8s[0]<<8)|p8s[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p8s[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

void Get698Time_Ascii(u8 *p,u8 *p8)//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
{
	u32 i;
	u32 x;
	u8 data;
	//Ğ´Ê±¼ä
	//Äê
	x=(p[0]<<8)|p[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

u32 Cmp698Time_Ascii(u8 *p,u8 *pc,u32 Byte)//698Ê±¼ä±È½ÏpcÇ°Á½¸ö×Ö½Ú´ú±íÄê·İ
{
	u32 i;
	u8 data;
	u32 x;
	u8 ptim[4]={0};
	//Ğ´Ê±¼ä
	//Äê
	x=(pc[0]<<8)|pc[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(ptim,(u8 *)&x,2);
	//ÏÈ±È½ÏÄê·İ
	for(i=0;i<4;i++)
	{
		if(*p!=ptim[i])
		{
			return 1;
		}
	}
	for(i=2;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Get698Time_HMS_Ascii(u8 *pt,u8 *p8,u8 hour,u8 min,u8 sec)//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
{
	u32 i;
	u32 x;
	u8 data;
	//Ğ´Ê±¼ä
	//Äê
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(pt[0]<<8)|pt[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<4;i++)
	{
		//data=Comm_Ram->DL698YMDHMS[i];
		data=pt[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	data=hex_bcd(hour);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(min);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(sec);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	*p8=0;
}
void GetNullTime(u8 *pt)//µÃµ½Ò»¸öJSON¸ñÊ½ÒªÇóµÄ¿ÕÊ±¼ä¸ñÊ½
{
	u32 offset;
	offset=0;
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=' ';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=0;
}
void GetJsonTime(u8 *pt)//°Ñ´¦ÀíºÃµÄÊ±¼äbuf pt×ª»»³ÉJSONÎÄ¼şÀïÃæµÄÊ±¼ä¸ñÊ½£¨2020-05-01 20:00:00£©
{
	u8 tt[14];
	MW((u32)pt,(u32)tt,sizeof(tt));
	//Äê
	pt[0]=tt[0];
	pt[1]=tt[1];
	pt[2]=tt[2];
	pt[3]=tt[3];	
	//ÔÂ
	pt[4]='-';
	pt[5]=tt[4];
	pt[6]=tt[5];
	//ÈÕ
	pt[7]='-';
	pt[8]=tt[6];
	pt[9]=tt[7];

	pt[10]=' ';
	//Ê±
	pt[11]=tt[8];
	pt[12]=tt[9];
	pt[13]=':';
	//·Ö
	pt[14]=tt[10];
	pt[15]=tt[11];
	pt[16]=':';
	//Ãë
	pt[17]=tt[12];
	pt[18]=tt[13];
	pt[19]=0;
}

void WriteNullString(u8 *p8)//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
{
	p8[0]='n';
	p8[1]='u';
	p8[2]='l';
	p8[3]='l';
	p8[4]=0;	
}

u32 Get_DataPointNUM(u16 OI)//ÔÚOAD_PointNUM_list±í¸ñÖĞÕÒOAD²¢µÃµ½Õâ¸öOADµÄĞ¡ÊıÎ»Êı
{
	u32 i;
	u32 ret=0;
	for(i=0;i<OAD_PointNUMSIZE;i++)
	{
		if(OAD_PointNUM_list[i].OI==OI)
		{
			ret=OAD_PointNUM_list[i].POINTNUM;
			break;
		}
	}
	return ret;
}
u32 DealMetData_Ascii(u16 OI,u8 *pdata,u8 *pout,u32 isReal)//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
{
	u32 i;
	u32 datalen;
	u32 pointnum;
	u64 data;
	i=Get_DL698DataLen_S(pdata,0);
	if(0==i)
	{		
		return i;
	}
	if(pdata[0])
	{
		i--;
		data=0;
		if(i>8)
		{
			i=8;
		}
		datalen=i;
		while(i>0)
		{
			pdata++;
			i--;
			data|=((*pdata)<<(i*8));
		}
		if((data&0xffffffff)==0xffffffff || (data&0xffffffff)==0x7fffffff)
		{
			if(!isReal)
			{
				WriteNullString(pout);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
				i=4;
			}else
			{
				pout[0]=0;
				i=1;
			}
		}else
		{
			switch (OI>>8)
			{
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
					pointnum=2;
					break;
				case 0x10:
				case 0x11:
					pointnum=4;
					break;
				default:
					pointnum=Get_DataPointNUM(OI);
					break;		
			}
			i=0;
			i=(data>>((datalen-1)*8));
			if(i>>4)//Õâ¸öÊı¾İÊÇ¸ºÊı
			{
				*pout='-';
				pout++;
				i=(u32)data;
				data=(u64)(~(i)+1);
			}
			i=Data_Ascii(data,pout,pointnum);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı			
		}
	}else
	{
		if(!isReal)
		{
			WriteNullString(pout);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
			i=4;
		}else
		{
			pout[0]=0;
			i=1;
		}
	}
	return i;
}

u32 Createitem_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//´´½¨JSON¸ñÊ½: "XXXXX":"XXXXXXX",
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//Ã°ºÅ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemNum_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//´´½¨JSON¸ñÊ½: "XXXXX":XXXXXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//Ã°ºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemArrayHead_Json(u8 *pout,u8 *ph,u32 spacenum)//´´½¨JSON¸ñÊ½: "XXXXX":[
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//Ã°ºÅ
	CJSONLEN+=i;
	i=Str_L_SQB(pout+CJSONLEN);//×ó·½À¨ºÅ
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateitemArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//´´½¨JSON¸ñÊ½: ],
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_R_SQB(pout+Len+CJSONLEN);//ÓÒ·½À¨ºÅ
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateArrayHead_Json(u8 *pout,u32 spacenum)//´´½¨JSON¸ñÊ½: {
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_L_CUB(pout+CJSONLEN);//×ó»¨À¨ºÅ
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//´´½¨JSON¸ñÊ½: },
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_R_CUB(pout+Len+CJSONLEN);//ÓÒ»¨À¨ºÅ
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateDataHead_Json(u8 *pout,u8 *ph,u32 spacenum)//´´½¨JSON¸ñÊ½: "XXXXXXX","
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//»Ø³µºÍ»»ĞĞ
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//¿Õ¸ñ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//Ã°ºÅ
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateData_Json(u8 *pout,u8 *pd)//´´½¨JSON¸ñÊ½: XXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	if(pd[0]!=0)
	{
		mystrcpy((char *)pout+CJSONLEN, (const char *)pd);
		i=mystrlen((const char *)pout+CJSONLEN);
		CJSONLEN+=i;
	}
	i=Str_COMMA(pout+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateDataEnd_Json(u8 *pout,u32 Len)//´´½¨JSON¸ñÊ½: ",
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	//CJSONLEN=Len;
	i=Str_QM(pout+Len+CJSONLEN);//ÒıºÅ
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//¶ººÅ
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}
u32 Delet_COMMA(u8 *pout,u32 Len)//É¾³ı¶ººÅ,·µ»Ø×Ü³¤
{
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
		}
	}
	return Len;
}
u32 Get_OneFreDataLen_FromFlash(u8 *pIn)//¼ÆËãÒ»¸ö·ÖÊ±¶ÎÊı¾İ°üÊı¾İµÄ³¤¶È,pinµ¼ÈëµÄÊÇÍêÕûµÄ°ü°üº¬21¸öÊ±±ê×Ö½Ú
{
	u8 OADNUM;

	u32 i;
	u32 x;

	u32 Offset;
	Offset=21;

	Offset+=4;//ROAD

	Offset+=4;//OADNUM
	OADNUM=pIn[Offset];
	Offset++;
	Offset+=OADNUM*4;

	for(x=0;x<OADNUM;x++)
	{		
		i=Get_DL698DataLen_S(pIn+Offset,0);//Ëã³öµÚÒ»¸öÊı¾İµÄ³¤¶È
		Offset+=i;
	}
	return Offset;
}

u32 Day_Month_PackJson(u8 *pInbuf,u8 *pout)//ÈÕÔÂ¶³½áÊı¾İ´ò°üpINbufÇ°Á½¸ö×Ö½Ú±íÊ¾Ò»¹²¶àÉÙÊı¾İ°ü
{
	u8 *p8;
	u8 *p8s;
	u8 *p8Str;
	u8 *pdata;
	u16 *p16;
	
	u8 NUM;//Êı¾İ¸öÊı
	u32 ROAD;//ROADÆğÊ¼Î»ÖÃ
	u8 OADNUM;//OADµÄÊıÁ¿
	u8 OADPORT;//OADÆğÊ¼Î»ÖÃ
	u8 DataNUM;
	
	u16 TotalNUM;
	u32 OAD;

	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Offset;
	u32 CJSONLEN=0;

	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];//Ò»¹²µÄ²É¼¯ÊıÁ¿
	pInbuf+=2;
	while(TotalNUM>0)
	{
		TotalNUM--;	
		Offset=21;//7¸ö×Ö½Ú²É¼¯Æô¶¯Ê±±ê,²É¼¯³É¹¦Ê±±ê,²É¼¯½áÊøÊ±±ê
		if(pInbuf[Offset]!=1)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		p8=(u8 *)ADDR_DATABUFF; 
		Offset++;
		NUM=pInbuf[Offset];//Êı¾İ¸öÊı
		//myprintf("[%s:%d]:NUM=%d\n",(u32)&__func__,(u32)__LINE__,NUM);
		Offset++;
	for(x=0;x<NUM;x++)
	{
		p8Str=Get_Element(pInbuf+21,1+x,0,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
		if(!p8Str)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		Offset=0;
		if(p8Str[Offset]!=0x5b)//PROAD
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó p8Str[Offset]=%02x\n",(u32)&__func__,__LINE__,p8Str[Offset]);
			#endif
			goto JSON_ERROR;
		}
		Offset+=2;
		
		ROAD=(p8Str[Offset]<<24)|(p8Str[Offset+1]<<16)|(p8Str[Offset+2]<<8)|p8Str[Offset+3];
		//myprintf("[%s:%d]:ROAD=%08x\n",(u32)&__func__,__LINE__,ROAD);
		Offset+=4;
		OADNUM=p8Str[Offset];//OADµÄÊıÁ¿
		Offset++;
		OADPORT=Offset;//OADÆğÊ¼Î»ÖÃ
		Offset+=OADNUM*4;
		p8s=p8Str+Offset;
		//ÕÒ¶³½áÊ±±êµÄÎ»ÖÃ
		for(y=0;y<OADNUM;y++)
		{
			p8s=p8Str+Offset;
			p8s=Get_Element(p8s,y+1,0,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
			if(!p8s)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
				#endif
				goto JSON_ERROR;
			}
			OAD=(p8Str[OADPORT+y*4]<<24)|(p8Str[OADPORT+1+y*4]<<16)|(p8Str[OADPORT+2+y*4]<<8)|(p8Str[OADPORT+3+y*4]);
			if(0x20210200==OAD)//¶³½áÊ±±ê
			{
				p8s+=8;//Ò»¸ö±êÊ¶7¸öÊı¾İ
				continue;
			}
			if(1==p8s[0])//Êı×é
			{
				DataNUM=p8s[1];
				if(0==p8s[1])
				{
					DataNUM=1;
				}
			}else
			{
				DataNUM=1;
			}
			for(z=0;z<DataNUM;z++)
			{
				if(0!=p8s[1])
				{
					pdata=Get_Element(p8s,z+1,0,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
					if(!pdata)
					{
						#ifdef DEBUG_JSON
						myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
						#endif
						goto JSON_ERROR;
					}
				}
				i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=ROAD>>16;
				Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbufÀïÃæµÄ×Ö½Úµ¹ĞòÊä³öµ½pstr²¢×ª³ÉascÂë
				i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=OAD+z+1;
				Add_StrToASCII_Inverte(p8,(u8 *)&i,4);//pbufÀïÃæµÄ×Ö½Úµ¹ĞòÊä³öµ½pstr²¢×ª³ÉascÂë(p8, p8+4,4);
				i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+7,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+14,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
				i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
				i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				if(0==p8s[1])
				{//È«²¿Ğ´NULL
					if(((OAD>>28)&0xf) == 1)//ĞèÁ¿
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						
						GetNullTime(p8);
						i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;	
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}
				}else
				{
					if(((OAD>>28)&0xf) == 1)//ĞèÁ¿
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
		
						if(pdata[0]==0x02)
						{		
							pdata+=2;
							//´¦ÀíĞèÁ¿µÄÊı¾İ
							i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
							if(0==i)
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
								#endif
								goto JSON_ERROR;
							}
							i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
							//´¦ÀíĞèÁ¿µÄ×î´ó·¢ÉúÊ±¼ä
							i=Get_DL698DataLen_S(pdata,0);
							pdata+=i;
							if(pdata[0]==0x1c)//ĞèÁ¿µÄÊ±¼ä
							{
								pdata++;
								Get698Time_Ascii(pdata,p8);
								GetJsonTime(p8);
							}else
							{
								GetNullTime(p8);
							}
							i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
						}
						
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}

						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}	
				}
				i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json( pout, CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
			}
		}
	}
	i=Get_OneFreDataLen_FromFlash(pInbuf);
	pInbuf+=i;
}
	return CJSONLEN;
JSON_ERROR:
	return 0;
}

void Add_NewOADToBuff(u8 *pOAD,u8 *pBuff,u8 OADNUM)//pBuffÖĞµÄOADÌí¼Óµ½pPADÖĞ ÊıÁ¿ÎªOADNUM
{
	u8 x;	
	pOAD[0]=OADNUM;
	pOAD++;
	for(x=0;x<OADNUM;x++)
	{
		MW((u32)pBuff+x*4,(u32)pOAD+x*4,4);
	}
}

u32 IsFindOneOADFromBuff(u8 *pOAD,u8 *pBuff)//´Ó´¢´æºÃµÄPOAD bufÖĞÕÒPBUFF¶ÔÓ¦OAD,Èç¹ûÕÒµ½¾ÍËµÃ÷ÔÚpOADÖĞ´æÔÚpBuffÕâ¸öOAD
{

	u8 x;
	u8 NUM;
	u32 OAD;
	u32 OAD1;
	u32 ret=1;
	NUM=pOAD[0];
	pOAD++;

	OAD1=(pBuff[0]<<24)|(pBuff[1]<<16)|(pBuff[2]<<8)|pBuff[3];
	if(0x20210200==OAD1)
	{
		return 0;
	}
	for(x=0;x<NUM;x++)
	{
		OAD=(pOAD[x*4]<<24)|(pOAD[x*4+1]<<16)|(pOAD[x*4+2]<<8)|pOAD[x*4+3];	
		if(OAD==OAD1)
		{
			break;
		}
	}
	if(x>=NUM)
	{
		ret=0;
	}	
	return ret;
}

u8 *Find_DataHead(u8 *pIn,u8 *pOAD,u32 oadcnt)//¸ù¾İFlashÀïÃæµÄ´æ´¢¸ñÊ½ÓÃpInÀïÃæOADµÄÎ»ÖÃÈ¥ÕÒpInÀïÃæÊı¾İµÄÎ»ÖÃ,pOADµÚÒ»×Ö½ÚÊÇOADÊıÁ¿,oadcntÊÇpOADÀïÃæµÄÆ«ÒÆ,·µ»ØPinÀïÃæÊı¾İµÄÕıÈ·Î»ÖÃ
{
	u8 OADNUM;
	u32 i;
	u32 y;
	u32 Offset;
	pIn+=21;
	Offset=0;
	Offset+=4;
	Offset+=4;

	OADNUM=pIn[Offset];
	Offset++;
	Offset+=(OADNUM*4);
	
	if(oadcnt>OADNUM-1)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:ËÑË÷³öÀ´µÄOADÏÂ±ê´óÓÚÊµ¼ÊµÄOAD×ÜÊı\n",(u32)&__func__,__LINE__,0);
		#endif
		return 0;
	}
	pIn+=Offset;
	Offset=0;
	for(i=0;i<=oadcnt;i++)
	{
		if(i==oadcnt)
		{			
			return pIn+Offset;
		}
		y=Get_DL698DataLen_S(pIn+Offset,0);//Ëã³öµÚÒ»¸öÊı¾İµÄ³¤¶È
		Offset+=y;
	}
	return 0;
}

u32 JudegMin_FromTime(u8 *pch,u32 cnt,u32 Min)//´ÓflashÀïÃæµÄÊ±¼äÈ¥ÅĞ¶ÏÕâ¸öÊ±¼äµãµÄ·ÖÖÓ´æ²»´æÔÚ(ÈçÓĞ1440·ÖÖÓ´Ó0·ÖÖÓ¿ªÊ¼ÅĞ¶Ï)
{
	u8 hh;
	u8 mm;
	u32 i;
	pch+=14;
	//²É¼¯´æ´¢Ê±±ê
	//07 00 01 02 00 00 00
	pch+=4;
	hh=pch[0];
	mm=pch[1];
	i=hh*60+mm;
	
	if(i==(cnt*Min))
	{
		return 1;
	}
	return 0;
}

u32 Is_SingleOADNUM_FromData(u8 *pOAD,u8 *pData,u32 OADNUM)//±ÈÈçËµÕâ¸öOADÊÇ00100200 ËûÒ»¹²ÓĞ5Ïî ×Ü¼â·åÆ½¹È
{
	u32 i;
	u32 n;
	u32 Len;
	u32 OAD;
	OAD=(pOAD[0]<<24)|(pOAD[1]<<16)|(pOAD[2]<<8)|pOAD[3];

	if((OAD&0xff)==0)
	{
		Len=0;
		for(i=0;i<OADNUM;i++)
		{
			n=Get_DL698DataLen_S(pData+Len,0);//Ëã³öµÚÒ»¸öÊı¾İµÄ³¤¶È
			Len+=n;
		}
		if(1==pData[Len])//ÊÇ¸öÊı×é£¨Ò»°ã¶¼ÊÇÊı×é£©
		{
			i=pData[Len+1];
			if(i>1)
			{
				return i;
			}else
			{
				return 0;
			}
		}
	}
	return 0;
}

u32 Min_PackJson(u8 *pInbuf,u8 *pout,u32 Min)//·ÖÖÓ¶³½áÊı¾İµÄ´ò°üMiNÇø·ÖÊÇ15·ÖÖÓ»¹ÊÇ1·ÖÖÓ
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN;
	u8 *p8;
	u8 *pOAD;
	u8 *pbuf=pInbuf;
	u8 *pdata;//Ö¸ÏòÊı¾İÇøµÄÖ¸Õë

	u32 Offset;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//¼ÇÂ¼±¨ÎÄÖĞROAD
	u32 TIMESPOT;	//Ê±¼ä±êÇ©µÄÎ»ÖÃ
	u8 OADNUM;		//¼ÇÂ¼±¨ÎÄÖĞROADÊıÁ¿
	u8 OADNUM_S;
	u32 OADSPOT; 	//¼ÇÂ¼±¨ÎÄÖĞROADÊıÁ¿µÄÎ»ÖÃ//20210200
	u32 Fre_Dst;	//Êı¾İ¶³½áÃÜ¶È
	u16 *p16;
	CJSONLEN=0;
	p16=(u16 *)pbuf;
	TotalNUM=p16[0];
	if(0==TotalNUM)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:Ã»ÓĞÊı¾İÄÚÈİ %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	pbuf+=2;//Êı¾İ×ÜÊı³¤¶È
	for(TIMESPOT=0;TIMESPOT<1440;TIMESPOT++)
	{
		x=mymemcmp(pbuf+TIMESPOT*ONEDATASIZE, 0, 2);
		if(x)
		{
			break;
		}
	}
	if(TIMESPOT>=1440)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:Ã»ÓĞÊı¾İÄÚÈİ %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	Offset=TIMESPOT*ONEDATASIZE;//ÏÈÕÒµ½Ò»¸öÓĞĞ§,ÄÃÕâÒ»¸öÓĞĞ§µÄÍ·¸ãÊÂÇé
	p8=(u8 *)ADDR_DATABUFF;
	
	pOAD=(u8 *)ADDR_DATABUFF+1024;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);

	Offset+=21;

	Offset+=4;
	ROAD=(pbuf[Offset]<<24)|(pbuf[Offset+1]<<16)|(pbuf[Offset+2]<<8)|pbuf[Offset+3];
	if(ROAD!=0x50020200)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:²»ÊÇ·ÖÖÓ¶³½áÊı¾İ\n", (u32)&__func__, __LINE__, 0);
		#endif
		goto Min_JSON_ERROR;
	}
	Offset+=4;
	OADNUM=pbuf[Offset];
	Offset++;	
	Add_NewOADToBuff(pOAD,pbuf+Offset,OADNUM);//pBuffÖĞµÄOADÌí¼Óµ½pPADÖĞ ÊıÁ¿ÎªOADNUM
	OADSPOT=Offset;
	Offset+=(OADNUM*4);	
	for(y=0;y<OADNUM;y++)
	{
		if(!IsFindOneOADFromBuff(pOAD,pbuf+OADSPOT+y*4))//´Ó´¢´æºÃµÄPOAD bufÖĞÕÒPBUFF¶ÔÓ¦OAD,Èç¹ûÕÒµ½¾ÍËµÃ÷ÔÚpOADÖĞ´æÔÚpBuffÕâ¸öOAD
		{	
			continue;
		}
		
		i=Is_SingleOADNUM_FromData(pbuf+OADSPOT+y*4,pbuf+Offset,y);
		if(0==i)
		{
			i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=ROAD>>16;
			Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbufÀïÃæµÄ×Ö½Úµ¹ĞòÊä³öµ½pstr²¢×ª³ÉascÂë
			i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]<<0);
			Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
			i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
			//Get698Time_HMS_Ascii(p8,0,0,0);
			//p8[3]-=1;
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			switch(Min)
			{
				case 1:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//²É¼¯´æ´¢Ê±±ê(Êı¾İÊ±±ê)
					break;
				case 15:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//²É¼¯´æ´¢Ê±±ê(Êı¾İÊ±±ê)
					break;
			}
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=Data_Ascii((u64)Min,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
			i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Fre_Dst=(24*60)/Min;
			i=Data_Ascii((u64)Fre_Dst,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
			i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			//ÅĞ¶Ï·ÖÖÓµãÊı¾İ´æ²»´æÔÚ		

			for(z=0;z<Fre_Dst;z++)
			{	
				x=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
				if(x)
				{
					pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//¸ù¾İFlashÀïÃæµÄ´æ´¢¸ñÊ½ÓÃpInÀïÃæOADµÄÎ»ÖÃÈ¥ÕÒpInÀïÃæÊı¾İµÄÎ»ÖÃ,pOADµÚÒ»×Ö½ÚÊÇOADÊıÁ¿,oadcntÊÇpOADÀïÃæµÄÆ«ÒÆ,·µ»ØPinÀïÃæÊı¾İµÄÕıÈ·Î»ÖÃ
					if(pdata)
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
							#endif
							goto Min_JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}else
					{
						WriteNullString(p8);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}
				}else
				{
					WriteNullString(p8);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
					i=CreateData_Json(pout+CJSONLEN, p8);
					CJSONLEN+=i;
				}
			}

			i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
		}
		else//´¦Àí½»²ÉµÄOAD,ÓĞOADºÏ²¢µÄÏî
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:½»²É15·ÖÖÓÊı¾İ´¦Àí\n",(u32)&__func__,__LINE__,0);
			#endif
				OADNUM_S=i;//Êı×éµÄ¸öÊı¾ÍÊÇOADµÄ·ÖÁ¿µÄ¸öÊı
				OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]);
				for(x=1;x<=OADNUM_S;x++)//Ñ­»·¼ÇÂ¼Ã¿Ò»¸öOADÓĞ¶àÉÙ¸ö×ÓOAD
				{
					i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
					i=ROAD>>16;
					Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbufÀïÃæµÄ×Ö½Úµ¹ĞòÊä³öµ½pstr²¢×ª³ÉascÂë
					i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					OAD&=0xffffff00;
					OAD|=x;
					Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
					i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
					//Get698Time_HMS_Ascii(p8,0,0,0);
					//p8[3]-=1;
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					switch(Min)
					{
						case 1:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//²É¼¯´æ´¢Ê±±ê(Êı¾İÊ±±ê)
							break;
						case 15:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//²É¼¯´æ´¢Ê±±ê(Êı¾İÊ±±ê)
							break;
					}
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=Data_Ascii((u64)Min,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
					i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Fre_Dst=(24*60)/Min;
					i=Data_Ascii((u64)Fre_Dst,p8,0);//½«idaÖµ´æµ½pÖĞ,NUMÎªĞ¡ÊıµãµÄÎ»Êı
					i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;

					//ÅĞ¶Ï·ÖÖÓµãÊı¾İ´æ²»´æÔÚ	
					for(z=0;z<Fre_Dst;z++)
					{
						i=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
						if(i)
						{
							pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//¸ù¾İFlashÀïÃæµÄ´æ´¢¸ñÊ½ÓÃpInÀïÃæOADµÄÎ»ÖÃÈ¥ÕÒpInÀïÃæÊı¾İµÄÎ»ÖÃ,pOADµÚÒ»×Ö½ÚÊÇOADÊıÁ¿,oadcntÊÇpOADÀïÃæµÄÆ«ÒÆ,·µ»ØPinÀïÃæÊı¾İµÄÕıÈ·Î»ÖÃ
							pdata=Get_Element(pdata,x,0,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
							if(pdata)
							{	
								i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii
								if(0==i)
								{
									#ifdef DEBUG_JSON
									myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
									#endif
									goto Min_JSON_ERROR;
								}
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}else
							{
								WriteNullString(p8);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}
						}else
						{
							WriteNullString(p8);//Êı¾İBUFÀïÃæĞ´ASC µÄNULL
							i=CreateData_Json(pout+CJSONLEN, p8);
							CJSONLEN+=i;
						}
					}

					i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
				}
			}
	}
	
	return CJSONLEN;
Min_JSON_ERROR:
	return 0;
}

u32 RCSDtoFileID_JS(u16 *pAddr,u32 ROAD,u32 Min)//¼ÇÂ¼ÁĞÑ¡ÔñRCSDÏà¶ÔÓ¦µÄFileID,Èë¿Ú:pRCSDÖ¸ÏòÀàĞÍ×Ö½Ú;·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID,µ±ÓĞ2¸öÉÏÏàÍ¬·½°¸Ê±TaskNO=0xff£¬MIN£¬1·ÖÖÓ»¹ÊÇ15·ÖÖÓ×öÇø·Ö
{	
	if(0==ROAD)
	{	
		if(Min==15)
		{
			return 0x13f12000;
		}
		if(Min==1)
		{
			return 0x13f17000;
		}
	}else
	{
		switch(ROAD)
		{
			case 0x50040200:
				return 0x13f16000;
			case 0x50020200:
				if(Min==15)
				{
					return 0x13f19000;
				}
				if(Min==1)
				{
					return 0x13f22000;
				}
			case 0x50060200:
				return 0x13f18000;
		}
	}				
	return 0;//·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID
}

u32 Filter_Time(u8 *pfiletm,u32 ROAD,u32 isReal,u32 All)//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://·ÖÖÓ¶³½á
			//±È½ÏÊ±±êÊÇÉÏÒ»ÌìµÄ
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}		
		  	break;
		case 0x50040200://ÈÕ¶³½á
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}
			break;
		case 0x50060200://ÔÂ¶³½á
			//±È½ÏÊ±±êÊÇÉÏÒ»ÌìµÄ
			i=(JSON->Lastrealtime[0]<<8)|(JSON->Lastrealtime[1]<<16)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 1);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(pfiletm[0]<<8)|(pfiletm[1]<<16)|pfiletm[2];
			if(i==x)
			{
				if(JSON->Lastrealtime[3]==1)//Ã¿¸öÔÂµÄÒ»ºÅ²Å»ØÈ¥´ò°üÔÂ¶³½á
				{
					ret=1;
				}
			}			
			break;
		default://ÊµÊ±Êı¾İ
		break;
	}
	return ret;
}

u32 Min_FrzNUM(u8 *pIn)//·µ»Ø°ÑĞ¡Ê±·ÖÖÓ,×ª»»³É·ÖÖÓÊı
{
	u8 hh;
	u8 mm;
	u32 i;
	pIn+=14;
	pIn+=4;
	hh=pIn[0];
	mm=pIn[1];
	i=60*hh+mm;
	return i;
}

u32 Filter_Same_Data(u8 *pt,u32 datanum,u8 *pdata)//ÓÃ²É¼¯´æ´¢Ê±±êÈ¥ÅĞ¶ÏÊÇ²»ÊÇÖØ¸´µÄÊı¾İ,·µ»Ø1ÊÇÖØ¸´µÄÊı¾İ,·µ»Ø0²»ÊÇÖØ¸´µÄÊı¾İ
{
	u32 i;
	u32 x;
	if(0==datanum)//Ò»¸öÊı¾İ¶¼Ã»ÓĞµÄÊ±ºòÄ¬ÈÏÃ»ÓĞÖØ¸´µÄÊı¾İ
	{
		return 0;
	}
	pdata+=14;//Ö¸Ïò²É¼¯´æ´¢Ê±±ê
	for(i=0;i<datanum;i++)
	{
		//É¾Ñ¡µ½·ÖÖÓ
		for(x=0;x<6;x++)
		{
			if(pt[x]!=pdata[x])
			{
				break;
			}
		}
		if(x>=6)
		{
			return 1;
		}
		pt+=7;
	}
	return 0;
}

u32 Is_1MinData(u8 *pdata,u8 ACSAMPLE_FLAG)//ÅĞ¶ÏÕâ°üÊı¾İÊÇ²»ÊÇ1·ÖÖÓÊı¾İ,1ÊÇ.0²»ÊÇ,ACSAMPLE_FLAG½»²ÉÃ»ÓĞ1·ÖÖÓÊı¾İ
{
	u32 i;
	u32 OAD;
	u32 OADnum;
	if(ACSAMPLE_FLAG!=0)
	{
		return 0;
	}
	pdata+=21;
	pdata+=4;
	pdata+=4;
	OADnum=*pdata;
	pdata++;
	
	for(i=0;i<OADnum;i++)
	{
		OAD=(pdata[0]<<24)|(pdata[1]<<16)|(pdata[2]<<8)|(pdata[3]);
		if((OAD>>8)==0x200002)
		{
			return 1;
		}
		pdata+=4;
	}
	return 0;
}

u32 Filter_Realtime(u8 *ptime)//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
{
	u32 ret=0;
	u32 i;
	i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//Êı¾İ´®±È½Ï(¸ßÎ»ÔÚµÍ×Ö½Ú),p1ºÍp2ÒÑÔÚRAMÖĞ,·µ»Ø:0=ÏàÍ¬,1=²»ÏàÍ¬p1>p2,2=²»ÏàÍ¬p1<p2
	if(i==1)
	{
		ret=1;
	}	
	return ret;
}

u32 Is_ACSAMPLE(u16 *pAddr)
{
	u8 *p8;
	u8 *p8s;
	//u16 *p16;
	u16 *p16s;
	u32 i;
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p8s=Get_Element(p8,2,4,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
			if(0x08!=p8s[2])//½»²É
			{
				p8+=LENper_6000;
				continue;
			}
			p16s=(u16*)(p8+10);//È¡²É¼¯µµ°¸ÅäÖÃĞòºÅµÄÍ¨ĞÅµØÖ·
			if(pAddr[0]==p16s[0])
			{
				if(pAddr[1]==p16s[1])
				{
					if(pAddr[2]==p16s[2])
					{
						return 0;
					}
				}
			}
					
		}
		p8+=LENper_6000;
	}
	#ifdef DEBUG_JSON
	myprintf("%04x %04x \n",pAddr[0],p16s[0],0);
	myprintf("%04x %04x \n",pAddr[1],p16s[1],0);
	myprintf("%04x %04x \n",pAddr[2],p16s[2],0);
	#endif
	return 1;
}

u32 Get_RecordData(u32 ROAD,u32 FileID,u16 *pAddr,u8 *pstack,u32 *DataLen,u32 isReal,u32 All)//ÓÃFlashÖĞ»ñÈ¡Êı¾İ,·µ»ØÒ»¸öÊı¾İÖ¸Õë,¸ÃÖ¸ÕëĞèÒªFreeÊÍ·Å
{
	//u32 iii;
	u32 z;
	u8 *p8;
	u8 *p8time;
	u32 MetNUM;
	u32 i;
	u32 n;
	u32 offset;
	u32 DataNUM;
	u32 filename;
	u8 ACSAMPLE_FLAG;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//³­±í¼ÇÂ¼ÎÄ¼şÍ·ÎÄ¼ş
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//ÔÊĞí¿´ÃÅ¹·ºÍÎ¹¹·
	
	DataNUM=0;
	*DataLen=0;
	Comm_Ram->msFILESearchCount=0;	
	
	ACSAMPLE_FLAG=0;
	if(ROAD==0x50020200&&FileID==0x13f19000)//ÅĞ¶ÏÊÇ²»ÊÇ½»²Éµµ°¸,15·ÖÖÓÇúÏß
	{
		//ÊÇ²»ÊÇ½»²Éµµ°¸
		i=Is_ACSAMPLE(pAddr);
		if(i==0)
		{
			ACSAMPLE_FLAG=1;
		}
	}
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{		
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//ËÑË÷ÎÄ¼şÃû:Èë¿Ú:FileID=b31-b28·½°¸ÀàĞÍ,b27-b24²É¼¯ÀàĞÍ,b23-b20´æ´¢Ê±±êÀàĞÍ,b19-b12ÈÎÎñºÅ,b11-b8¶Ë¿ÚºÅ,b7-b0,ÏàÓ¦ÓòÖµÎª0xF±íÊ¾²»¹Ø×¢,pTimeÊı¾İ´æ´¢Ê±±ê=0±íÊ¾²»¹Ø×¢;SearchCount=¿ªÊ¼ËÑË÷µÄÎÄ¼şºÅ0xFFFF±íÊ¾´ÓÉÏ´Î½áÊøºÅ¿ªÊ¼ËÑË÷;·µ»Ø:0--(FILENAMEmax_FLASH-1)±íÊ¾ÎÄ¼şÔÚFLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)±íÊ¾ÎÄ¼şÔÚRAM,0xffffffff±íÊ¾Ã»ÕÒµ½
		if(filename==0xffffffff)
		{
			continue;
		}
		//Ö»ÔÚFlashÎÄ¼şÀïÃæ²éÕÒ RAMÎÄ¼şÀïÃæÃ»ÓĞÊı¾İ
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		if(0==ROAD)//ÊµÊ±ÔöÁ¿ÓÃ²É¼¯Æô¶¯Ê±±êÀ´ÅĞ¶Ï
		{
			i=Filter_Realtime(file->save_date_time_s);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
		}else
		{	
			i=Filter_Time(file->save_date_time_s,ROAD,isReal,All);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
		}
		if(0==i)
		{
			continue;
		}
		if(0==ROAD)//ÀïÃæ¿ÉÄÜÓĞÖØ¸´µÄµØÖ·µ«ÊÇÊı¾İÄÚÈİ²»Í¬Ò²ÒªÈ¡³öÀ´
		{
			p8time=(u8 *)ms->ms_malloc(7*1440);
			if(!p8time)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:ÉêÇë¿Õ¼äÊ§°Ü\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				return 0;
			}
			n=file->NUM_DATA;
			//myprintf("[%s:%d]:ĞÂÔö±íµÄÊıÁ¿ n=%d\n ",(u32)&__func__,(u32)__LINE__,n);	
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(MetNUM=0;MetNUM<n;MetNUM++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							i=RecordFileHead->LEN_DATA;
							if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:È¡µ½µÄÄÚÈİ³¤¶È³¬ÏŞ\n ",(u32)&__func__,(u32)__LINE__,0);	
								#endif
								continue;
							}
							ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
							p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
							if(0!=p8[0])//È¡µ½µÄÊı¾İÄÚÈİÓĞ¿ÉÄÜ¶¼ÊÇ0µÄÖ±½Ó¹ıÂËµô,ÎªÁË¼õÉÙÑ­»·´ÎÊı¾Í²éµÚÒ»¸ö×Ö½Ú
							{
								if(!Filter_Same_Data(p8time,DataNUM,p8))//ÓÃ²É¼¯´æ´¢Ê±±êÈ¥ÅĞ¶ÏÊÇ²»ÊÇÖØ¸´µÄÊı¾İ,·µ»Ø1ÊÇÖØ¸´µÄÊı¾İ,·µ»Ø0²»ÊÇÖØ¸´µÄÊı¾İ
								{
									MW((u32)p8,(u32)pstack+(*DataLen),i);
									MW((u32)p8+14,(u32)p8time+(DataNUM*7),7);//±£´æ²É¼¯´æ´¢Ê±±ê·½±ã²éÖØ
									(*DataLen)+=i;
									DataNUM++;//²éµ½Ò»¸öĞÂµÄÊı¾İ×Ô¶¯¼Ó1	
								}
							}	
						}
								
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			ms->ms_free(p8time);
			p8time=NULL;
		}else
		{
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i>=n)
			{//Ã»ÕÒµ½		
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:µØÖ·Ã»ÓĞÕÒµ½\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			i=RecordFileHead->LEN_DATA;
			if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:È¡µ½µÄÄÚÈİ³¤¶È³¬ÏŞ\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
			
			p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
			
			if(0x50020200==ROAD)//Èç¹ûÊÇ·ÖÖÓ,ÄÇ¾ÍÔÚÖ¡µÄÇ°Ãæ¶à¼ÓÁ½¸ö×Ö½Ú×÷ÎªĞòºÅ
			{
				z=Min_FrzNUM(p8);
				if(i<ONEDATASIZE&&z<1440)
				{
					if(FileID==0x13f22000)
					{
						if(Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//²éµ½Ò»¸öĞÂµÄÊı¾İ×Ô¶¯¼Ó1	
						}
					}else
					{
						if(!Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//²éµ½Ò»¸öĞÂµÄÊı¾İ×Ô¶¯¼Ó1	
						}
					}
					
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("¶³½áÊı¾İ´ÓFlashÀïÃæ¶ÁµÄÓĞÎÊÌâ,ĞèÒªĞŞ¸Ä i=%d,z=%d\n ",i,z,0);	
					#endif
				}
				
			}else
			{	
				MW((u32)p8,(u32)pstack+(*DataLen),i);
				(*DataLen)+=i;
				DataNUM++;//²éµ½Ò»¸öĞÂµÄÊı¾İ×Ô¶¯¼Ó1	
			}
		}
	}
	return DataNUM;
}

u8 *Get_MetData_ROAD(u16 *pAddr,u32 ROAD,u32 Min,u32 isReal,u32 All)//¸ù¾İROADÀ´È¡FlashÖĞµÄÊı¾İ,pAddr±íµØÖ·,MinÈ¡·ÖÖÓ¶³½áµÄÊ±ºòÓÃ,1±íÊ¾1·ÖÖÓ15±íÊ¾15·ÖÖÓ,·µ»Ø0ÎŞÊı¾İ,·ñÔò·µ»ØÒ»¸öÊı¾İbufÖ¸Õë,isRealÊÇ·ñÊÇÈ«ÍøÊµÊ±
{
	u16 *p16;
	u16 DataNUM;
	u32 Len;
	u32 i;
	u32 x;
	u8 *pstack=NULL;
	u32 FileID;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pstack=(u8 *)ms->ms_malloc(1024*1024);
	if(!pstack)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:ÉêÇë¿Õ¼äÊ§°Ü\n ",(u32)&__func__,(u32)__LINE__,0);	
		#endif
		return 0;
	}
	MC(0,(u32)pstack,1024*1024);
	Len=0;
	DataNUM=0;
	switch(ROAD)
	{
		case 0x50020200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//¼ÇÂ¼ÁĞÑ¡ÔñRCSDÏà¶ÔÓ¦µÄFileID,Èë¿Ú:pRCSDÖ¸ÏòÀàĞÍ×Ö½Ú;·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID,µ±ÓĞ2¸öÉÏÏàÍ¬·½°¸Ê±TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			break;
		case 0x50040200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//¼ÇÂ¼ÁĞÑ¡ÔñRCSDÏà¶ÔÓ¦µÄFileID,Èë¿Ú:pRCSDÖ¸ÏòÀàĞÍ×Ö½Ú;·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID,µ±ÓĞ2¸öÉÏÏàÍ¬·½°¸Ê±TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f11000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		case 0x50060200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//¼ÇÂ¼ÁĞÑ¡ÔñRCSDÏà¶ÔÓ¦µÄFileID,Èë¿Ú:pRCSDÖ¸ÏòÀàĞÍ×Ö½Ú;·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID,µ±ÓĞ2¸öÉÏÏàÍ¬·½°¸Ê±TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f13000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		default://ÊµÊ±
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//¼ÇÂ¼ÁĞÑ¡ÔñRCSDÏà¶ÔÓ¦µÄFileID,Èë¿Ú:pRCSDÖ¸ÏòÀàĞÍ×Ö½Ú;·µ»Ø:0±íÊ¾Ã»ÕÒµ½ÎŞĞ§FileID,µ±ÓĞ2¸öÉÏÏàÍ¬·½°¸Ê±TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
		break;
	}
	if(0==DataNUM)
	{
		ms->ms_free(pstack);
		pstack=NULL;
	}else
	{
		p16=(u16 *)pstack;
		p16[0]=DataNUM;	
	}
	return pstack;
}

u32 FreData_PackJson(u32 ROAD,u8 *pInbuf,u8 *pout,u32 Min)//¶³½áÊı¾İ´ò°üÓÃROADÇø·Ö ·ÖÖÓ¶³½áºÍÈÕÔÂ¶³½á
{
	u32 ret=0;
	switch(ROAD)
	{
		case 0x50020200:
			ret=Min_PackJson(pInbuf,pout,Min);
			break;
		case 0x50040200:
		case 0x50060200:
			ret=Day_Month_PackJson(pInbuf,pout);
			break;
		default:
			break;
	}
	return ret;
}

void GetMetAddr(void)//È¡µÃËùÓĞ±íµÄµØÖ·
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	u32 i;
	u32 AddrNUM;
//1.È¡µÃ±íµµ°¸µÄËùÓĞÓĞĞ§µØÖ·,¹ıÂË½»²ÉµØÖ·
	AddrNUM=0;
	p16=(u16*)(ADDR_AddrList+2);
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//È¡²É¼¯µµ°¸ÅäÖÃĞòºÅµÄÍ¨ĞÅµØÖ·
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			p16+=3;
			AddrNUM+=1;
		}
		p8+=LENper_6000;
	}
	p16=(u16*)(ADDR_AddrList);
	p16[0]=AddrNUM;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:È¡µÃµµ°¸ÖĞËùÓĞµÄ±íµØÖ· ÊıÁ¿:%d\n",(u32)&__func__, __LINE__, AddrNUM);
	#endif
}

u8 *CreateRealJsonFile(void)//´´½¨Ò»¸öĞÂµÄÊµÊ±jsonÏîÄ¿ÎÄ¼ş
{	
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//´´½¨ÎÄ¼ş¼Ğ
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë		
	Get698Time_Ascii(JSON->Lastrealtime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");	
	i=mystrlen((const char *)p8);
	if(sizeof(JSON->RealFileName)>=i+1)
	{
		mystrncpy((char *)JSON->RealFileName,(const char *)p8,i);
	}else
	{
		#ifdef DEBUG_JSON
		myprintf("\n[%s:%d]:JSON->RealFileName buf¿Õ¼ä´óĞ¡²»¹» ËùĞèi+1=%d\n", (u32)&__func__, __LINE__, i+1);
		#endif
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	myprintf("\n[%s:%d]:×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )JSON->RealFileName);
	#endif
	return p8;
}

u8 *CreateFreJsonFile(void)//´´½¨Ò»¸öĞÂµÄ¶³½ájsonÏîÄ¿ÎÄ¼ş
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//´´½¨ÎÄ¼ş¼Ğ
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë	
	mystrcpy((char *)p8+Len+8, ".json");	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	return p8;
}

u8 *CreateFreJsonFile_RAM(void)//ÔÚRAMÇø,´´½¨Ò»¸öĞÂµÄ¶³½áÏîÄ¿ÎÄ¼ş
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//´´½¨ÎÄ¼ş¼Ğ
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë	
	mystrcpy((char *)p8+Len+8, ".json");	
	
	return p8;
}

void Compress_JSONFile(u8 *fname,u32 fnamesiz,u32 isReal)//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½,isReal,1ÊµÊ±ÎÄ¼ş,0¶³½áÎÄ¼ş
{
	u8 *p8;
	u32 i;
	u32 SLen;
	ms_Type *ms;
	ms=Get_ms();
	p8=(u8 *)ADDR_DATABUFF+1024;
	SLen=0;
	//°ÑÎÄ¼ş¼ĞÀïÃæµÄÎÄ¼şÒÆ¶¯µ½µ±Ç°Ä¿Â¼ÏÂÀ´
	mystrcpy((char *)p8+SLen,"mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen,(const char *)fname);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen," .");
	ms->link_system((char *)p8);
	//´ò°üÕâ¸öÎÄ¼ş
	SLen=0;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, "tar jcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}else
	{
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:½âÑ¹ÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//°Ñ.JSONÎÄ¼şÉ¾³ıµô,È»ºóÔÙ°ÑÑ¹Ëõ°üÒÆ¶¯µ½¶ÔÓ¦µÄÎ»ÖÃ
	SLen=0;
	mystrcpy((char *)p8+SLen, "mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		i=mystrlen(JSONREALPATH);
	}else
	{
		i=mystrlen(JSONFREPATH);
	}
	mystrcpy((char *)p8+SLen, (const char *)fname+i);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	for(i=0;i<SLen;i++)
	{
		if('.'==p8[i])
		{
			break;
		}
	}
	if(isReal)
	{
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONREALPATH);
	}else
	{
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONFREPATH);
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:ÒÆ¶¯ÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//É¾³ıµ±Ç°µÄ.jsonÎÄ¼ş
	SLen=0;
	mystrcpy((char *)p8+SLen, "rm ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_REAL);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}else
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_FRE);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıµ±Ç°JSONÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	if(isReal)
	{
		Delet_JsonFile(JSON->RealFileName);
	}else
	{
		p8=(u8 *)ADDR_DATABUFF+1024;
		mystrcpy((char *)p8, (const char *)fname);
		Delet_JsonFile(p8);
	}
}

u32 HeadStart_JSON(u8 *pstc)//´´½¨JSON¸ñÊ½: [
{
	u32 Offset;
	u32 i;
	Offset=0;
	i=Str_L_SQB(pstc+Offset);//×ó·½À¨ºÅ
	Offset+=i;
	return Offset;
}	

u32 HeadEnd_JSON(u8 *pstc,u32 Len)//´´½¨JSON¸ñÊ½: ]
{
	u32 flag=0;
	u32 Offset;
	u32 i;
	Offset=0;
	if(Len>0)
	{
		if(','==pstc[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pstc+Len+Offset);//»Ø³µºÍ»»ĞĞ
	Offset+=i;
	i=Str_R_SQB(pstc+Len+Offset);//ÓÒ·½À¨ºÅ
	Offset+=i;
	if(1==flag)
	{
		Offset--;
	}
	return Offset;
}

u32 FreDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal)
{
	u32 i;
	u8 *pDATA;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,0);//¸ù¾İROADÀ´È¡FlashÖĞµÄÊı¾İ,pAddr±íµØÖ·,MinÈ¡·ÖÖÓ¶³½áµÄÊ±ºòÓÃ,1±íÊ¾1·ÖÖÓ15±íÊ¾15·ÖÖÓ,·µ»Ø0ÎŞÊı¾İ,·ñÔò·µ»ØÒ»¸öÊı¾İbufÖ¸Õë
	if(NULL==pDATA)
	{	
		return 0;	
	}
	i=FreData_PackJson(ROAD,pDATA,pd,Min);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 AllDataPack(u16 *pAddr,u8 *pd)
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	//»ñÈ¡¶³½áÊı¾İ²¢´ò°ü³ÉJSON¸ñÊ½,·µ»Ø0Ã»ÓĞÕÒµ½Êı¾İ
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50040200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50060200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,15,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,1,0);
	CJSONLEN+=i;	
	return CJSONLEN;
}

void Delet_FrzJsonFile(void)//É¾³ıFlash¿Õ¼äÀïÃæ,ËùÓĞµÄ¶³½áÎÄ¼ş
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

void Crl_AllJsonFileAndTar(void)//Çå¿ÕËùÓĞÓĞ¹ØµÄJSONÎÄ¼şÄÚÈİ
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.bz2");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);
}


void Delet_Yesterday_Data_Flash(void)
{
	//É¾³ı×òÌìµÄÈÕ¶³½á
	Delet_All_FromFlash(0x13f16000);
	Delet_All_FromFlash(0x13f11000);
	//É¾³ı×òÌìµÄÔÂ¶³½á
	Delet_All_FromFlash(0x13f18000);
	Delet_All_FromFlash(0x13f13000);
	//É¾³ı×òÌìµÄ·ÖÖÓ¶³½á
	Delet_All_FromFlash(0x13f19000);
	Delet_All_FromFlash(0x13f22000);	
}

u32 Get_OneRealDataLen_FromFlash(u8 *pIn)//¼ÆËãÒ»¸öÊµÊ±Êı¾İ°üÊı¾İµÄ³¤¶È,pinµ¼ÈëµÄÊÇÍêÕûµÄ°ü°üº¬21¸öÊ±±ê×Ö½Ú
{
	u8 *p8;
	u32 i;
	u32 x;
	u32 OADNUM;
	u32 Len;
	Len=0;
	Len+=21;//Ìø¹ı21¸öÊ±¼ä×Ö½Ú
	OADNUM=pIn[Len+1];//OADÊıÁ¿
	Len+=2;
	Len+=OADNUM*6;
	p8=pIn+Len;//Ö¸ÏòÊı¾İ²¿·Ö
	for(x=0;x<OADNUM;x++)
	{
		i=Get_DL698DataLen_S(p8,0);
		p8+=i;
		Len+=i;
	}
	return Len;
}

u32 RealData_PackJson(u8 *pInbuf,u8 *pout,u8 *pAddr,u32 All)//´¦ÀíÊµÊ±Êı¾İ
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN=0;
	u32 CJSONL;
	u8 *p8;
	u8 *pdata;//Ö¸ÏòÊı¾İÇøµÄÖ¸Õë
	u8 val;

	u32 Offset=0;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//¼ÇÂ¼±¨ÎÄÖĞROAD
	u8 OADNUM;		//¼ÇÂ¼±¨ÎÄÖĞROADÊıÁ¿
	u8 OADSPOT; 	//¼ÇÂ¼±¨ÎÄÖĞROADÊıÁ¿µÄÎ»ÖÃ//20210200
	u32 DataLen;	//Êı¾İ´óĞ¡

	u16 *p16;
	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];

	pInbuf+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);
	DataLen=0;
	val=0;
	WWDT_Disable();
	if(TotalNUM==0)
	{
		if(All!=0)
		{
			TotalNUM=1;
		}
	}
	for(z=0;z<TotalNUM;z++)
	{
		Offset=21;
		Offset+=4;
		ROAD=(pInbuf[Offset]<<24)|(pInbuf[Offset+1]<<16)|(pInbuf[Offset+2]<<8)|pInbuf[Offset+3];
		if(ROAD!=0x50020200)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:²»ÊÇ·ÖÖÓ¶³½áÊı¾İ:%08x\n", (u32)&__func__, __LINE__, ROAD);
			#endif
			goto Real_JSON_ERROR;
		}
		Offset+=4;
		OADNUM=pInbuf[Offset];
		Offset++;
		OADSPOT=Offset;
		Offset+=(OADNUM*4); 
		//=====================
		CJSONL=CJSONLEN;
		val=0;
		i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		Add_StrToASCII(p8,pAddr,6);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
		i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
		CJSONLEN+=i;
		
		//Êı¾İÊ±¼ä
		Get698Time_Ascii(pInbuf+14,p8);//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
		GetJsonTime(p8);
		i=CreateData_Json(pout+CJSONLEN, p8);
		CJSONLEN+=i;
		for(y=1;y<RealDataOAD_JSONSIZE;y++)//y´Ó1¿ªÊ¼Ëã ÒòÎªµÚÒ»¸öÊÇÊ±¼äÔÚÇ°ÃæÒÑ¾­Ğ´ºÃÁË
		{
			for(x=0;x<OADNUM;x++)
			{
				OAD=(pInbuf[OADSPOT+x*4]<<24)|(pInbuf[OADSPOT+x*4+1]<<16)|(pInbuf[OADSPOT+x*4+2]<<8)|pInbuf[OADSPOT+x*4+3];
				
				if((RealDataOAD_JSON[y])==(OAD))
				{
					break;
				}else
				{
					if((OAD&0xff)==0)//Èç¹ûOAD×îºóÒ»¸ö×Ö½ÚÊÇ0ÓĞ¿ÉÄÜ¾ÍÊÇµçÑ¹ µçÁ¿ÕâÖÖºÏÏîµÄ
					{
						if((RealDataOAD_JSON[y]>>8)==(OAD>>8))
						{
							break;
						}	
					}
				}
			}
			if(x<OADNUM)//ÕÒµ½ÁËÓĞĞ§OAD
			{	
				pdata=Find_DataHead(pInbuf,pInbuf+OADSPOT,x);//¸ù¾İFlashÀïÃæµÄ´æ´¢¸ñÊ½ÓÃpInÀïÃæOADµÄÎ»ÖÃÈ¥ÕÒpInÀïÃæÊı¾İµÄÎ»ÖÃ,pOADµÚÒ»×Ö½ÚÊÇOADÊıÁ¿,oadcntÊÇpOADÀïÃæµÄÆ«ÒÆ,·µ»ØPinÀïÃæÊı¾İµÄÕıÈ·Î»ÖÃ
				if((RealDataOAD_JSON[y]!=OAD)&&((OAD&0xff)==0))
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:ÊÇ½»²ÉÊı¾İ\n",(u32)&__func__,__LINE__,0);
					myprintf("[%s:%d]:ÊÇ½»²ÉÊı¾İOAD=%08x\n",(u32)&__func__,__LINE__,OAD);
					
					for(i=0;i<50;i++)
							myprintf("%02x ",pdata[i],0,0);
						myprintf("\n",0,0,0);
					#endif
					i=RealDataOAD_JSON[y]&0xff;
					pdata=Get_Element(pdata,i,0,0);//¼ÆËãÔªËØµØÖ·(ĞéÄâµØÖ·Ö¸Õë),Ê¹ÓÃADDR_128KDATABUFF;³ö¿Ú:µØÖ·Ö¸ÏòÔªËØµÄÀàĞÍ×Ö½Ú,0=´íÎó
				}
				if(!pdata)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:Ã»ÓĞÕÒµ½ÓĞĞ§µÄÊı¾İ\n",(u32)&__func__,__LINE__,0);
					#endif
					continue;
				}
				i=DealMetData_Ascii(OAD>>16,pdata,p8,1);//´¦Àí³­¶Á»Øµç±íµÄÊı¾İ,·µ»Ø´æÈëpoutµÄ×Ö½ÚÊıÁ¿,poutÒÑ¾­´¦ÀíºÃµÄASCii			
				if(0==i)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:²É¼¯µ½µÄÊı¾İ´æ´¢¸ñÊ½ÓĞ´íÎó\n",(u32)&__func__,__LINE__,0);
					#endif
					goto Real_JSON_ERROR;
				}
				if(All)
				{//È«Íø
					val=1;
				}else
				{//ÔöÁ¿  ¹ıÂËµô0
					if(0==p8[0])
					{
						continue;
					}else
					{
						val=1;
					}
				}
				i=CreateData_Json(pout+CJSONLEN, p8);
				CJSONLEN+=i;
			}else
			{
				i=Str_COMMA(pout+CJSONLEN);//¶ººÅ
				CJSONLEN+=i;
			}
		}
		i=CreateDataEnd_Json(pout,CJSONLEN);
		CJSONLEN+=i;
		i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		i=Str_LF(pout+CJSONLEN);
		CJSONLEN+=i;
		DataLen=Get_OneFreDataLen_FromFlash(pInbuf);//¼ÆËãÒ»¸ö·ÖÊ±¶ÎÊı¾İ°üÊı¾İµÄ³¤¶È,pinµ¼ÈëµÄÊÇÍêÕûµÄ°ü°üº¬21¸öÊ±±ê×Ö½Ú
		if(z<TotalNUM)
		{	
			pInbuf+=DataLen;
		}
		if(!All)
		{
			if(0==val)
			{
				CJSONLEN=CJSONL;
			}
		}
	}
	if(!All)
	{
		if(0==val)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:Êı¾İÎª¿Õ!!!!!!\n",(u32)&__func__,__LINE__,0);
			#endif
			return 0;
		}
	}
	return CJSONLEN;
Real_JSON_ERROR:
	return 0;
}

u32 RealDataPackNull(u8 *pout,u8 *pAddr)//È«ÍøÊµÊ±´´½¨¿ÕÊı¾İÁĞ±í
{
	u8 *p8;
	u32 i;
	u32 y;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	p8=(u8 *)ADDR_DATABUFF;
	Add_StrToASCII(p8,pAddr,6);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
	CJSONLEN+=i;
	
	//Êı¾İÊ±¼ä
	//Get698CurTime_Ascii(p8);
	Get698Time_Ascii(JSON->Lastrealtime,p8);//pÊ±¼äÇ°Á½¸ö×Ö½Ú±íÊ¾Äê·İp8Êä³öascÂë
	GetJsonTime(p8);
	i=CreateData_Json(pout+CJSONLEN, p8);
	CJSONLEN+=i;	
	for(y=1;y<RealDataOAD_JSONSIZE;y++)//y´Ó1¿ªÊ¼Ëã ÒòÎªµÚÒ»¸öÊÇÊ±¼äÔÚÇ°ÃæÒÑ¾­Ğ´ºÃÁË
	{
		i=Str_COMMA(pout+CJSONLEN);//¶ººÅ
		CJSONLEN+=i;
	}
	i=CreateDataEnd_Json(pout,CJSONLEN);
	CJSONLEN+=i;
	i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	i=Str_LF(pout+CJSONLEN);
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 RealDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal,u32 All)
{
	u32 i;
	u8 *pDATA;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,All);//¸ù¾İROADÀ´È¡FlashÖĞµÄÊı¾İ,pAddr±íµØÖ·,MinÈ¡·ÖÖÓ¶³½áµÄÊ±ºòÓÃ,1±íÊ¾1·ÖÖÓ15±íÊ¾15·ÖÖÓ,·µ»Ø0ÎŞÊı¾İ,·ñÔò·µ»ØÒ»¸öÊı¾İbufÖ¸Õë
	if(NULL==pDATA)
	{	
		if(All)
		{
			p8=(u8 *)pAddr;
			i=RealDataPackNull(pd,p8);
			ms->ms_free(pDATA);
			pDATA=NULL;	
			return i;
		}
		return 0;	
	}
	p8=(u8 *)pAddr;
	i=RealData_PackJson(pDATA,pd,p8,All);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 DeletFlashData_SelectTime(u32 ROAD,u8 *ptime)//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://·ÖÖÓ¶³½á
			//±È½ÏÊ±±êÊÇÉÏÒ»ÌìµÄ
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}		
			break;
		case 0x50040200://ÈÕ¶³½á
		//´ò°üµÄÊ±ºòÊÇµÚ¶şÌì
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
		//¼õÒ»ÌìÊÇ×òÌìµÄÈÕ¶³½á
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			i=YYMD_Sub1D_hex(i);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}
			break;
		case 0x50060200://ÔÂ¶³½á(±£´æÒ»¸öÔÂµÄÊ±¼ä)
			//±È½ÏÊ±±êÊÇÉÏÒ»ÌìµÄ
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 2);//ÄêÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÄêÔÂÈÕ
			x=(ptime[0]<<8)|(ptime[1]<<16)|ptime[2];
			if(i>=x)
			{
				ret=1;
			}			
			break;
		case 0x5002020E:
		case 0x50020201:
			i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//Êı¾İ´®±È½Ï(¸ßÎ»ÔÚµÍ×Ö½Ú),p1ºÍp2ÒÑÔÚRAMÖĞ,·µ»Ø:0=ÏàÍ¬,1=²»ÏàÍ¬p1>p2,2=²»ÏàÍ¬p1<p2
			if(i==1)
			{
				ret=1;
			}		
			break;	
		default:
		break;
	}
	return ret;
}

void Delet_All_FromFlash(u32 FileID)
{
	u32 i;
	u32	filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//ÔÊĞí¿´ÃÅ¹·ºÍÎ¹¹·
	Comm_Ram->msFILESearchCount=0;
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{	
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//ËÑË÷ÎÄ¼şÃû:Èë¿Ú:FileID=b31-b28·½°¸ÀàĞÍ,b27-b24²É¼¯ÀàĞÍ,b23-b20´æ´¢Ê±±êÀàĞÍ,b19-b12ÈÎÎñºÅ,b11-b8¶Ë¿ÚºÅ,b7-b0,ÏàÓ¦ÓòÖµÎª0xF±íÊ¾²»¹Ø×¢,pTimeÊı¾İ´æ´¢Ê±±ê=0±íÊ¾²»¹Ø×¢;SearchCount=¿ªÊ¼ËÑË÷µÄÎÄ¼şºÅ0xFFFF±íÊ¾´ÓÉÏ´Î½áÊøºÅ¿ªÊ¼ËÑË÷;·µ»Ø:0--(FILENAMEmax_FLASH-1)±íÊ¾ÎÄ¼şÔÚFLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)±íÊ¾ÎÄ¼şÔÚRAM,0xffffffff±íÊ¾Ã»ÕÒµ½
		if(filename==0xffffffff)
		{
			continue;
		}
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		switch (FileID)
		{
			case 0x13f11000://ÈÕ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f13000://ÔÂ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f16000://ÈÕ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f18000://ÔÂ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f19000://15·ÖÖÓ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f22000://1·ÖÖÓ¶³½á
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//È¡ÎÄ¼şµÄ´æ´¢Ê±¼ä½øĞĞ¹ıÂË,³É¹¦·µ»Ø1,Ê§°Ü0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f12000://15·ÖÖÓ¶³½á
				//i=1;//Ä¬ÈÏÈ«²¿É¾³ı
				i=DeletFlashData_SelectTime(0x5002020E,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			case 0x13f17000://1·ÖÖÓ¶³½á
				//i=1;//Ä¬ÈÏÈ«²¿É¾³ı
				i=DeletFlashData_SelectTime(0x50020201,file->save_date_time_s);//ÔÚÃ¿Ò»´Î´ò°üÖ®ºó´¦ÀíÉ¾³ıµÄÊı¾İÏî(´ÓNANDFLASH),³É¹¦¾Í·µ»Ø1,Ê§°Ü·µ»Ø0
				break;
			default:
				return;
		}
		if(0==i)
		{
			continue;
		}
		file->BLOCK_ENTRY=0;
		ms->msfile_Delete(filename);
	}
}

void Delet_FileName(void)
{
	Delet_All_FromFlash(0x13f12000);//É¾³ıÕâ¸öÊ±¼äµãµÄÎÄ¼ş
	Delet_All_FromFlash(0x13f17000);//É¾³ıÕâ¸öÊ±¼äµãµÄÎÄ¼ş
}

void Delet_JsonFile(u8 *fanme)//É¾³ıµ±Ç°µÄRealtime.jsonÎÄ¼ş,±£ÁôÑ¹Ëõ°ü
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)fanme);
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıtmpÀïÃæJSONÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_JsonFile_PATH(const char *path)//É¾³ıÂ·¾¶pathÏÂµÄjsonÎÄ¼ş
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıtmpÀïÃæJSONÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_AllData_PATH(const char *path)//É¾³ıÂ·¾¶pathÏÂµÄËùÓĞÎÄ¼ş
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıtmpÀïÃæJSONÎÄ¼ş×Ö·û:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}

void Delet_CurPath_JSON_And_TarGz(void)//É¾³ıµ±Ç°Ä¿Â¼ÏÂµÄjsonÎÄ¼şºÍtar.gzÑ¹ËõÎÄ¼ş
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıÃüÁî:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:É¾³ıÃüÁî:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	ms->link_system((char *)p8);
}


void Delet_RealTime_BZ2(void)//É¾³ıÊµÊ±µÄÑ¹ËõÎÄ¼ş,(ÉÏ±¨Ö®ºóÒªÉ¾³ı)
{
	u32 i;
	u32 Len;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	p8=(u8 *)ADDR_DATABUFF+1024;
	Len=0;	
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)pfilename);
	ms->link_system((char *)p8);
}

void Mkdir_JsonFile(const char *path)//´´½¨pathÂ·¾¶ÎÄ¼ş¼Ğ
{
	u32 i;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	mystrcpy((char *)p8, "mkdir ");
	i=mystrlen((const char *)p8);
	mystrcpy((char *)p8+i, (const char *)path);
	ms->link_system((char *)p8);
}

u32 Is_MinData_Flash(u32 FileID)//ÔÚFlash¿Õ¼äÀïÃæÕâ¸öÊı×ÖÊÇ²»ÊÇ·ÖÖÓÊı¾İ,0²»É¾³ı 1É¾³ı
{
	u32 ret=0;
	//¸ù¾İFileIDÀ´É¾³ı²»Í¬µÄÎÄ¼ş
	switch(FileID)
	{
		//case 0x13f12000:
		//case 0x13f17000:
		case 0x13f16000://ÈÕ¶³½á
		case 0x13f11000:
		case 0x13f18000://ÔÂ¶³½á
		case 0x13f13000:
		case 0x13f19000://15·ÖÖÓ¶³½á
		//case 0x13f22000://1·ÖÖÓ¶³½á
		case 3://ËÑ±í½á¹û²»É¾³ı
		case 4:
			break;
		default:
			ret=1;
		break;
	}
	return ret;
}

void Delet_MinData_Flash(u32 Delcnt)//ÔÚFlashÀïÃæÉ¾³ıDelcntµãµÄÊı¾İÁ¿
{
	u32 i;
	u32 FileCount;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=Get_ms();
	FileCount=0;	
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(Delcnt<=FileCount)//Èç¹ûÉ¾³ıµÄ´ÎÊıÒÑ¾­µ½ÁË,ÄÇ¾ÍÍË³ö
		{
			break;
		}
		if(file->BLOCK_ENTRY)
		{
			//i=file->FILECOUNT;
			FileID=file->FILEID;
			i=Is_MinData_Flash(FileID);//ÔÚFlash¿Õ¼äÀïÃæÕâ¸öÊı×ÖÊÇ²»ÊÇ·ÖÖÓÊı¾İ,0²»É¾³ı 1É¾³ı
			if(i)
			{
				ms->msfile_Delete(filename);
				FileCount++;
			}
		}
		file++;
	}
}

void Delet_AllFile(u8 *ptime)//É¾³ıÁÙÊ±ÎÄ¼şÏÂÃæµÄËùÓĞÎÄ¼ş ²»°üÀ¨ÎÄ¼ş¼Ğ
{
	u32  i;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	//u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return;
	}
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "mkdir ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+8, "* ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//ÎÄ¼şµÄÃû×Ö
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='/';
	p8[Len++]='*';
	p8[Len++]=' ';
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//ÎÄ¼şµÄÃû×Ö
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]=' ';
	p8[Len++]='-';
	p8[Len++]='r';
	p8[Len++]='f';
	p8[Len++]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
}

u32 Check_TarFtzFile(u8 *ptime)
{
	u32 i;
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return 0;
	}
	if(ptime[4]==0&&ptime[5]==0)
	{
		JSON->IsCreateFrzJson=0;
	}
	i=ptime[4]*60+ptime[5];
	if(i<70)//Õâ¸öÊ±¼ä¶ÎÄÚ²»»áÓĞÕâ¸öÑ¹ËõÎÄ¼şÉú³É
	{
		return 0;
	}
	if(JSON->IsCreateFrzJson!=0)
	{
		return 0;
	}
	Mkdir_JsonFile(JSONFREPATH);
	pname=GetFrzFileJsonName_Compress();//µÃµ½Ñ¹Ëõ°üµÄÃû×Ö´øÂ·¾¶
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);//²âÊÔÎÄ¼ş´æ²»´æÔÚ
	if(fd!=-1)//ËµÃ÷ÎÄ¼ş²»´æÔÚ
	{
		//ÄÇ¾ÍËµÃ÷½ñÌìÃ»ÓĞ´ò°ü,»òÕßËµÊÇ´ò°üÒÑ¾­Ê§°ÜÁË,ĞèÒªÖØĞÂÔÙ´ò°ü
		//1.É¾³ıµ±Ç°Ä¿Â¼ÏÂÃæËùÓĞµÄ.jsonºÍ.tar
		Delet_CurPath_JSON_And_TarGz();
		Delet_AllFile(ptime);//É¾³ıÉÏÒ»ÌìµÄÎÄ¼ş¼ĞÄÚÈİ
		ms->link_close(fd);
		return 1;
	}else
	{
		//ÎÄ¼ş´æÔÚ½ñÌì¾Í²»½øĞĞÈ«Á¿ÎÄ¼ş´ò°ü²Ù×÷
		JSON->IsCreateFrzJson=1;
	}
	return 0;
}

u32 IsTimeTask(u8 *pt)//ÓÃÊ±¼äÅĞ¶ÏÊÇ²»ÊÇ´ò°üÈÎÎñÊ±¼äµ½ÁË,ptÉÏÒ»´Î´ò°üµÄÊ±¼ä
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 ret=0;
	YYMDHM1=0;
	YYMDHM1=((u64)pt[0]<<32)|((u64)pt[1]<<40)|((u64)pt[2]<<24)|((u64)pt[3]<<16)|((u64)pt[4]<<8)|((u64)pt[5]);
	//´æÔÚFlashÎÄ¼şÀïÃæµÄÊ±±ê
	YYMDHM2=0;
	YYMDHM2=((u64)Comm_Ram->DL698YMDHMS[0]<<32)|((u64)Comm_Ram->DL698YMDHMS[1]<<40)|((u64)Comm_Ram->DL698YMDHMS[2]<<24)|((u64)Comm_Ram->DL698YMDHMS[3]<<16)|((u64)Comm_Ram->DL698YMDHMS[4]<<8)|((u64)Comm_Ram->DL698YMDHMS[5]);
	if(JSON->reportfiletime[4]==0&&0==JSON->reportfiletime[5])
	{
		Delet_AllFile(JSON->reportfiletime);
	}
	if(YYMDHM1<YYMDHM2)
	{
		if(0==pt[5])
		{
			return 3;
		}
		return 1;
	}
	if(YYMDHM1>YYMDHM2)
	{
		return 2;
	}
	return ret;
}

u32 Can_GetVaildMetAddr(void)
{
	u8 *p8;
	u16 *p16s;
	u32 i;
	for(i=0;i<NUMmax_6000;i++)
	{
		p8=(u8*)ADDR_6000_SDRAM+i*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//È¡²É¼¯µµ°¸ÅäÖÃĞòºÅµÄÍ¨ĞÅµØÖ·
			if(p16s[0]==0)
			{
				if(p16s[1]==0)
				{
					if(p16s[2]==0)
					{
						continue;
					}
				}
			}
			break;
		}
	}
	if(i>=NUMmax_6000)
	{
		return 0;
	}
	return 1;
}

void Terminal_FrzPack(void)
{
	u32 i;
	u32 CJSONLEN;//JSON×Ö·û´®¼ÆÊıÆ÷,³õÊ¼»¯0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//µØÖ·u8ÀàĞÍÖ¸Õë
	u8 *pname;
	u16 *p16;
	s32 fd;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch (JSON->PackFrzJsonTask)
	{
		case 0://³õÊ¼»¯Êı¾İ
			JSON->PackFrzJsonTask++;
			break;
		case 1://Ê±¼äµÄÅĞ¶Ï	
			//¼ì²éÊÇ²»ÊÇÒòÎªÍ£ÉÏµçÒıÆğµÄ´ò°üÊ§°Ü»òÕßÑ¹ËõÊ§°Ü
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=i=Check_TarFtzFile(JSON->Lastrealtime);//Ö»ÓĞ¹ıÁËÒ»·ÖÖÓ²Å»áÖ´ĞĞÏàÓ¦µÄÈÎÎñ
			if(i!=0)//ÈÕ¶³½á´ò°üÈÎÎñ
			{
				#ifdef DEBUG_JSON
				myprintf("==============================================================================\n", 0, 0, 0);
				myprintf("==========================×¼±¸¿ªÊ¼´´½¨¶³½áJSON¸ñÊ½ÎÄ¼ş========================\n", 0, 0, 0);
				myprintf("==============================================================================\n", 0, 0, 0);
				#endif
				JSON->PackFrzJsonTask++;
			}
			break;
		case 2://´ò°üÖ®Ç°Òª×öµÄ¹¤×÷
			//TEST_PACK();
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´ò°üÇ°Ê±¼ä\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackFrzJsonTask=0;
				return;
			}
			JSON->CurFrzMetCount=0;
			//´ò°üÖ®Ç°Çå¿ÕRAMÏÂÃæËùÓĞµÄ¿Õ¼ä
			Delet_RAM_FrzFile();
			JSON->PackFrzJsonTask++;
			break;
		case 3://Êı¾İÕıÔÚ´¦ÀíÖĞ
			//#ifndef DEBUG_JSON
			i=GetOneMetAddr((u16 *)&JSON->CurFrzMetCount);
			//#else
			//i=GetOneMetAddr_TEST(&JSON->CurFrzMetCount);
			//#endif
			if(i==0)//È«²¿±íµØÖ·ÒÑ¾­´ò°ü½áÊø
			{
				JSON->PackFrzJsonTask++;;//ÖØÖÃÃ¿Ìì´ò°üÈÕ¶³½áµÄ±êÖ¾
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:Êı¾İ´¦ÀíÍê±Ï¿ªÊ¼Ñ¹Ëõ\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurFrzMetCount++;//ÏÂÒ»´ÎÑ­»·¾ÍÕÒÏÂÒ»Ö»±í
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:JSON->CurFrzMetCount=%d\n",(u32)&__func__,(u32)__LINE__,JSON->CurFrzMetCount);
			#endif
			//µÃµ½ÎÄ¼şÃû×Ö
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=RAMTOFLASH_COPYMAXSIZE))
				{
					//´ÓRAM¿Õ¼äÒÆ¶¯µ½Flash¿Õ¼ä
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:ÉêÇë¿Õ¼äÊ§°Üi\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//Ç°ÃæÒ»¸öÊÇ±íµÄÊıÁ¿,ºóÃæÒ»¸öÊÇ±íµÄµØÖ·
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//ËµÃ÷ÊÇÓĞÊı¾İµÄ
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//É¾³ı¶ººÅ
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//ÕâÒ»Ö»±íÊ¾ÓĞÊı¾İµÄ
			{
				//ÏÂÃæ¿ªÊ¼ÍùRAM¿Õ¼äÀïÃæĞ´Êı¾İ
				i=CalCulate_Space(JSONFREPATH_RAM);//¼ÆËãÕâ¸ö¿Õ¼äÀïÃæÊ£ÓàµÄÈİÁ¿
				if(i>=CJSONLEN)//Èç¹ûÊ£ÓàµÄ¿Õ¼ä¹»ÓÃ
				{
					//µÃµ½ÎÄ¼şÃû×Ö
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//¹Ø±ÕÎÄ¼ş¾ä±ú
			}
			ms->ms_free(p8);
			break;
		case 4://Êı¾İ´¦Àí½áÊø,×¼±¸¿½±´ÎÄ¼şÑ¹Ëõ
			//µÃµ½ÎÄ¼şÃû×Ö
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´ò°ü´¦ÀíÍê¿ªÊ¼Ñ¹Ëõ\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//´ÓRAM¿Õ¼äÒÆ¶¯µ½Flash¿Õ¼ä
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				WWDT_Disable();
				Compress_JSONFile(pname,1024,0);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
				//Delet_Yesterday_Data_Flash();//É¾³ıflashÀïÃæ×òÌìËùÓĞµÄÊı¾İ
				Delet_JsonFile_PATH(JSONFREPATH_RAM);//É¾³ıJSONÎÄ¼ş
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				//ËµÃ÷´ò°üÊ§°ÜÁË
				if(JSON->IsCreateFrzJson>=1)//½ñÌìµÄÊı¾İ´ò°üÊ§°ÜÁË
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
					Delet_JsonFile_PATH(JSONFREPATH_RAM);//É¾³ıJSONÎÄ¼ş
				}
				else
				{
					JSON->IsCreateFrzJson++;//ÔÙ¸øÒ»´Î»ú»á´ò°ü
				}
			}
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´ò°üºóÊ±¼ä\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			break;
		default:
			JSON->PackFrzJsonTask=0;
		break;
	}
}

u32 GetOneMetAddr(u16 *MetCount)//ÕÒµ½Ò»¸ö±íµØÖ·
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	#ifdef DEBUG_JSON
	u32 i;
	#endif
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	for(;Met<NUMmax_6000;Met++)
	{
		p8=(u8*)ADDR_6000_SDRAM+(Met)*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//È¡²É¼¯µµ°¸ÅäÖÃĞòºÅµÄÍ¨ĞÅµØÖ·
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			break;
		}
	}
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;

	p8=(u8 *)(ADDR_AddrList+2);
	#ifdef DEBUG_JSON
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

void Terminal_RealPack(void)
{
	s8 *pc="dateTime(æ•°æ®æ—¶é—´),20000201(Aç›¸ç”µå‹),20000202(Bç›¸ç”µå‹),20000203(Cç›¸ç”µå‹),20010201(Aç›¸ç”µæµ),20010202(Bç›¸ç”µæµ),20010203(Cç›¸ç”µæµ),20010400(é›¶çº¿ç”µæµ),20010500(å‰©ä½™ç”µæµ),20040201(æ€»æœ‰åŠŸåŠŸç‡),20040202(Aç›¸æœ‰åŠŸåŠŸç‡),20040203(Bç›¸æœ‰åŠŸåŠŸç‡),20040204(Cç›¸æœ‰åŠŸåŠŸç‡),43140506(å®¤å¤–æ¸©åº¦),43140505(å®¤å†…æ¸©åº¦),43140501(ç©ºè°ƒè®¾ç½®æ¸©åº¦),43140400(ç©ºè°ƒå¼€å…³æœºçŠ¶æ€),43110502(çƒ­æ°´å™¨åŠ çƒ­æ¸©åº¦),43110400(çƒ­æ°´å™¨å¼€å…³æœºçŠ¶æ€),00100201(æ­£å‘æœ‰åŠŸæ€»ç”µèƒ½),00100203(æ­£å‘æœ‰åŠŸå³°ç”µèƒ½),00100205(æ­£å‘æœ‰åŠŸè°·ç”µèƒ½),00200201(åå‘æœ‰åŠŸæ€»ç”µèƒ½),00200203(åå‘æœ‰åŠŸå³°ç”µèƒ½),00200205(åå‘æœ‰åŠŸè°·ç”µèƒ½)";
	s8 *pc1="]";
	u32 i;
	//u32 x;
	u32 y;
	u32 MAXMETNUM=1;
	u32 CJSONLEN;
	u8 *p8;                               
	u8 *pname;
	u16 *p16;
	//u16 AddrNUM;
	s32 fd;
	ms_Type *ms;	
	ms=(ms_Type *)Get_ms();
	switch (JSON->PackRealJsonTask)
	{
		case 0://³õÊ¼»¯Êı¾İ
			Delet_FileName();//É¾³ıÕâ¸öÊ±¼äµãµÄÎÄ¼ş
			pYYMDHMS_AddS_hex(JSON->Lastrealtime,60);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó ·ÖÖÓ´ò°üµÄÊ±¼ä±êÖ¾¶¼¼Ó1·ÖÖÓ
			JSON->PackRealJsonTask++;
			break;
		case 1://Ê±¼äµÄÅĞ¶Ï	
			//¼ì²éÊÇ²»ÊÇÒòÎªÍ£ÉÏµçÒıÆğµÄ´ò°üÊ§°Ü»òÕßÑ¹ËõÊ§°Ü
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=IsTimeTask(JSON->Lastrealtime);//Ö»ÓĞ¹ıÁËÒ»·ÖÖÓ²Å»áÖ´ĞĞÏàÓ¦µÄÈÎÎñ
			switch (i)
			{
				case 1:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=0;
					break;
				case 2:
					MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
					break;
				case 3:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=1;
					break;
				default:
					break;
			}	
			break;
		case 2://ÊµÊ±ÎÄ¼şÏÈĞ´Ò»¸öÍ·
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:ÊµÊ±Ê±¼ä±êÖ¾\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",JSON->Lastrealtime[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
				return;
			}
			JSON->CurRealMetCount=0;
			//ÏÈ´¦ÀíÊµÊ±ÎÄ¼şµÄÍ·
			p8=(u8 *)ms->ms_calloc(4*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc¿Õ¼ä²»×ã\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			i=HeadStart_JSON(p8+CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=CreateDataHead_Json(p8+CJSONLEN, (u8 *)STR_COLOUMNHEAD, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateData_Json(p8+CJSONLEN, (u8 *)pc);
			CJSONLEN+=i;
			i=CreateDataEnd_Json(p8,CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN,DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;
			Mkdir_JsonFile(JSONREALPATH);//´´½¨Õâ¸öÎÄ¼ş¼Ğ
			pname=CreateRealJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1!=fd)//ÒÑ¾­´æÔÚ¸ÃÎÄ¼ş¾ÍÉ¾³ı
			{
				ms->link_remove((char *)pname);
				#ifdef DEBUG_JSON
				myprintf("\n\n[%s:%d]:É¾³ıÎÄ¼ş:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
				#endif
			}
			ms->link_close(fd);
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				ms->ms_free(p8);
				JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
				return;
			}
			y=ms->link_write(fd,(char *)p8,CJSONLEN);
			if(y==CJSONLEN)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
				#endif
				JSON->PackRealJsonTask++;
			}else
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
			}
			ms->link_close(fd);
			ms->ms_free(p8);
			break;
		case 3://Êı¾İ´¦Àí¹ı³Ì
			while(MAXMETNUM>0)
			{
			MAXMETNUM--;	
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//È«²¿±íµØÖ·ÒÑ¾­´ò°ü½áÊø
			{
				//²»ÊÇ´ò°üÈ«Á¿ÎÄ¼ş,ÄÇ¾ÍÊÇÃ»ÓĞ±íÓĞÊı¾İ
				Delet_JsonFile_PATH(JSONREALPATH);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:Ã»ÓĞÒ»Ö»±íÓĞÊı¾İ\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
				break;
			}
			JSON->CurRealMetCount++;//ÏÂÒ»´ÎÑ­»·¾ÍÕÒÏÂÒ»Ö»±í		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc¿Õ¼ä²»×ã\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
				return;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;

			if((CJSONLEN>0))
			{
				Mkdir_JsonFile(JSONREALPATH);//´´½¨Õâ¸öÎÄ¼ş¼Ğ
				pname=CreateRealJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//²»´æÔÚÕâ¸öÎÄ¼ş
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
					JSON->PackRealJsonTask=0xff;
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}else
			{
				//myprintf("[%s:%d]:Ã»ÓĞÊı¾İ\n",(u32)&__func__,(u32)__LINE__,0);
			}
			ms->ms_free(p8);
			if(JSON->PackRealJsonTask==0xff)
			{
				break;
			}
			}
			break;
		case 4://¿ªÊ¼Ñ¹ËõÁ÷³Ì
			Mkdir_JsonFile(JSONREALPATH);//´´½¨Õâ¸öÎÄ¼ş¼Ğ
			pname=CreateRealJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1==fd)//²»´æÔÚÕâ¸öÎÄ¼ş
			{
				JSON->PackRealJsonTask=2;
				return;
			}
			i=ms->link_lseek(fd,0,SEEK_END);
			if(i>1)
			{
				ms->link_lseek(fd,i-2,SEEK_SET);
			}
			y=ms->link_write(fd,pc1,1);
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
			#endif
			if(y==1)
			{
				Compress_JSONFile(pname,1024,1);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
				//É¾³ıFlashÀïÃæµÄfilename
				SetReportInfo();//ÉèÖÃÉÏ±¨ĞÅÏ¢
			}else
			{
				y=ms->link_write(fd,(char *)p8,1);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
				if(y==1)
				{
					Compress_JSONFile(pname,1024,1);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
					//É¾³ıFlashÀïÃæµÄfilename
					SetReportInfo();//ÉèÖÃÉÏ±¨ĞÅÏ¢
				}
			}
			JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
			ms->link_close(fd);
			break;
		case 0xff://Êı¾İ´¦Àí¹ı³Ì
			while (MAXMETNUM>0)
			{
			MAXMETNUM--;
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//È«²¿±íµØÖ·ÒÑ¾­´ò°ü½áÊø
			{
				JSON->PackRealJsonTask=4;;//¿ªÊ¼Ñ¹Ëõ
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:Êı¾İ´¦ÀíÍê±Ï¿ªÊ¼Ñ¹Ëõ\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurRealMetCount++;//ÏÂÒ»´ÎÑ­»·¾ÍÕÒÏÂÒ»Ö»±í		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc¿Õ¼ä²»×ã\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//ÖØĞÂ¿ªÊ¼×ßÁ÷³Ì
				return ;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;

			if(CJSONLEN>0)
			{
				Mkdir_JsonFile(JSONREALPATH);//´´½¨Õâ¸öÎÄ¼ş¼Ğ
				pname=CreateRealJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//²»´æÔÚÕâ¸öÎÄ¼ş
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}	
			ms->ms_free(p8);
			}
			break;
		default:
			JSON->PackRealJsonTask=0;
		break;
	}		
}

void Terminal_PackJSON(void)//ÖÕ¶Ë´ò°üJSONÎÄ¼şÈÎÎñ
{
	u32 i;
	if(FileDownload->New==0)//Ô¶³ÌÉı¼¶Íê³É,²»½øÀ´´ò°ü
	{
		return;
	}
	switch (JSON->PackJsonTask)
	{
		case 0://³õÊ¼»¯Êı¾İ
			JSON->IsCreateFrzJson=0;//¸ÕÉÏµçÅĞ¶ÏÕâ¸öÖµ³õÊ¼»¯Îª0
			i=Check_pYYMDHMS_hex(Comm_Ram->DL698YMDHMS);
			if(i)
			{
				return;
			}	
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->reportfiletime, sizeof(JSON->reportfiletime));
			//Mkdir_JsonFile();//´´½¨ÎÄ¼ş¼Ğ
			//TEST_PACK();
			//tar_thread();
			//tar_text();
			//Crl_AllJsonFileAndTar();
			#ifdef DEBUG_JSON
			//log_Flash();
			//test_saveMIN15();
			//test_saveMIN();
			//test_saveDAY();
			#endif
			//TEST_RAM() ;
			//return;
			JSON->PackRealJsonTask=1;//ÊµÊ±´Ó1¿ªÊ¼
			JSON->PackJsonTask++;
			break;
		case 1:
			Terminal_FrzPack();//ÈÕ¶³½áÊı¾İ´ò°ü
			Terminal_RealPack();//ÊµÊ±Êı¾İ´ò°ü
			break;
		default:
			JSON->PackJsonTask=0;
		break;
	}
	
}
		
//=======================================È«ÍøÊµÊ±Êı¾İÉÏ±¨=====================================
void SetReportInfo(void)//ÉÏ±¨ÈÎÎñ´Ó0¿ªÊ¼,¿ÉÒÔÉÏ±¨
{
	JSON->g_ReportFlag=1;
}
u8 GetReportInfo(void)//µÃµ½ÊÇ²»ÊÇĞèÒªÉÏ±¨ÓĞÃ»ÓĞÉú³ÉĞÂµÄÉÏ±¨ÎÄ¼ş
{
	return JSON->g_ReportFlag;	
}
void ClrReportInfo(void)//ÇåµôÉÏ±¨±êÖ¾,ºÍÉÏ±¨ÎÄ¼şÃû
{
	JSON->Reportjsonfile_Task=0;
	JSON->g_ReportFlag=0;
	MC(0,(u32)JSON->RealFileName,sizeof(JSON->RealFileName));
	pYYMDHMS_AddS_hex(JSON->reportfiletime,60);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó ·ÖÖÓ´ò°üµÄÊ±¼ä±êÖ¾¶¼¼Ó1·ÖÖÓ
}
u32 IsReportEnd(void)//ÊÇ²»ÊÇÉÏ±¨³É¹¦,×ÜÉÏ±¨Êı=ÒÑ¾­ÉÏ±¨Êı
{
	if(JSON->g_Block_Number==JSON->g_Need_report_Block)
	{
		return 1;
	}
	return 0;
}
void Next_DataBlock(void)//Ö¸ÏòÏÂÒ»¸öÉÏ±¨µÄÊı¾İ¿é
{
	JSON->g_Block_Number++;
}

u32 Merge_RealTimeData_JsonFile(u8 *ptime)//ºÏ²¢Á½¸öÉÏ±¨Ñ¹ËõÎÄ¼ş³ÉÎªÒ»¸öĞÂµÄÑ¹ËõÎÄ¼ş
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 i;
	u32 n;
	u8 isemptyfile;//Èç¹ûÕÒµ½µÄÊÇµÚÒ»¸öÎÄ¼ş
	u8 *pfilename;
	s32 fd1=-1;
	s32 fd_new=-1;
	u8 *p8;
	//u8 *paddr;
	u32 Len;
	u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	MW((u32)ptime,(u32)time,sizeof(time));
	i=Check_pYYMDHMS_hex(time);
	if(i)
	{
		return 0;
	}
	pYYMDHMS_SubS_hex(time,60);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó
	isemptyfile=1;
	pfilename=GetFileJsonName_Compress(time);//µÃµ½ÎÄ¼şµÄÂ·¾¶	
	fd_new=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd_new)
	{
		//´´½¨Ò»¸öĞÂµÄÎÄ¼ş
		p8=GetRealTime_FileJson(time);
		//´´½¨Ò»¸öĞÂµÄÎÄ¼ş,ÎÄ¼şµÄÃû×Ö¸üĞÂµ½×îĞÂµÄ´ò°üÊ±¼ä
		fd_new=ms->link_open((char *)p8,O_CREAT|O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		ms->link_lseek(fd_new,0,SEEK_SET);
		isemptyfile=0;
	}else
	{
		ms->link_close(fd_new);//ÂíÉÏÊÍ·ÅµôÕâ¸öÎÄ¼ş¾ä±ú
		//¿ªÊ¼½âÑ¹Ëõ
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jxvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:½âÑ¹ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);

		p8=GetRealTime_FileJson(time);

		fd_new=ms->link_open((char *)p8,O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		isemptyfile=1;
	}
	
	YYMDHM2=0;
	YYMDHM2=((u64)JSON->reportfiletime[0]<<32)|((u64)JSON->reportfiletime[1]<<40)|((u64)JSON->reportfiletime[2]<<24)|((u64)JSON->reportfiletime[3]<<16)|((u64)JSON->reportfiletime[4]<<8)|((u64)JSON->reportfiletime[5]);
	while(1){
		pYYMDHMS_SubS_hex(time,60);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó ·ÖÖÓ´ò°üµÄÊ±¼ä±êÖ¾¶¼¼Ó1·ÖÖÓ
		YYMDHM1=0;
		YYMDHM1=((u64)time[0]<<32)|((u64)time[1]<<40)|((u64)time[2]<<24)|((u64)time[3]<<16)|((u64)time[4]<<8)|((u64)time[5]);
		
		if(YYMDHM2>YYMDHM1)
		{
			break;
		}	
		//´Ó×î´óµÄÄÇ¸öÊ±¼äµãµÄÎÄ¼ş¿ªÊ¼ÕÒ
		pfilename=GetFileJsonName_Compress(time);//µÃµ½ÎÄ¼şµÄÂ·¾¶	
		fd1=ms->link_open((char *)pfilename,O_RDWR);
		if(-1!=fd1)
		{
			ms->link_close(fd1);//ÂíÉÏÊÍ·ÅµôÕâ¸öÎÄ¼ş¾ä±ú
			//¿ªÊ¼½âÑ¹Ëõ
			p8=(u8 *)ADDR_DATABUFF+1024;
			Len=0;
			mystrcpy((char *)p8+Len, "tar jxvf ");
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			mystrcpy((char *)p8+Len, (const char *)pfilename);
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:½âÑ¹ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
			#endif
			ms->link_system((char *)p8);
			
			//´ò¿ªÕâ¸öÎÄ¼ş
			p8=GetRealTime_FileJson(time);

			fd1=ms->link_open((char *)p8,O_RDWR);
			if(-1!=fd1)
			{
				Len=ms->link_lseek(fd1,0,SEEK_END);
				if(Len)
				{	
					p8=(u8 *)ms->ms_malloc(Len);
					if(p8)
					{	
						if(0==isemptyfile)//ÕÒµ½ÁËµÚÒ»¸öÎÄ¼ş,//ĞèÒª±£´æËùÓĞµÄÎÄ¼şĞÅÏ¢
						{
							isemptyfile=1;//½ÓÏÂÀ´ÕÒµÄ¶¼²»ÊÇµÚÒ»¸öÎÄ¼ş Òª×öÌØÊâ´¦Àí
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);
							if(i==Len)
							{
								i=ms->link_write(fd_new,(u8 *)p8,Len);
							}
						}else
						{
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);	
						
							if(i==Len&&Len>0)
							{
								for(i=0;i<Len-1;i++)
								{
									if(p8[i]==',')
									{
										if(p8[i+1]==0x0A)//»»ĞĞ·ûºÅ
										{
											break;
										}
									}
								}
								if(i<Len)
								{
									n=ms->link_lseek(fd_new,0,SEEK_END);
									if(n>0)
									{
										ms->link_lseek(fd_new,n-1,SEEK_SET);
										Len-=i;
										ms->link_write(fd_new,(u8 *)p8+i,Len);
									}	
								}
								
							}
						}
						ms->ms_free(p8);
						p8=NULL;
					}
				}
				ms->link_close(fd1);//ÂíÉÏÊÍ·ÅµôÕâ¸öÎÄ¼ş¾ä±ú
			}
		}
		
	}
	
	Len=ms->link_lseek(fd_new,0,SEEK_END);
	if(Len)
	{	
		MW((u32)ptime,(u32)JSON->reportfiletime,sizeof(JSON->reportfiletime));
		i=Check_pYYMDHMS_hex(JSON->reportfiletime);
		if(i)
		{
			ms->link_close(fd_new);
			return 0;
		}
		pYYMDHMS_SubS_hex(JSON->reportfiletime,60);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó
		//µÃµ½ĞÂÎÄ¼şµÄÎÄ¼şÃû×Ö
		pfilename=GetRealTime_FileJson_Pack(JSON->reportfiletime);
		
		//Ñ¹ËõĞÂµÄÎÄ¼ş
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jcvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " ");
		Len++;

		n=mystrlen((const char *)pfilename);
		for(i=0;i<n;i++)
		{
			if(pfilename[i]=='.')
			{
				break;
			}
		}
		if(i>=n)
		{
			ms->link_close(fd_new);
			return 0;
		}
		
		mystrcpy((char *)pfilename+i, ".json");
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:½âÑ¹ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);
		//ÒÆ¶¯ÎÄ¼şµ½ÏàÓ¦µÄÄ¿Â¼ÏÂÃæ
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "mv *.tar.bz2 ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, JSONREALPATH);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " -f");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		ms->link_system((char *)p8);
	}
	//É¾³ıµôµ±Ç°JsonÎÄ¼ş
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm *.json");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	ms->link_system((char *)p8);
	
	ms->link_close(fd_new);
	return 1;
}
   
u32 Selec_ReportTime(void)//ÉÏ±¨Ê±¼äµÄÉ¾Ñ¡
{
	u32 i;
	u8 time[7];
	i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//Êı¾İ´®±È½Ï(¸ßÎ»ÔÚµÍ×Ö½Ú),p1ºÍp2ÒÑÔÚRAMÖĞ,·µ»Ø:0=ÏàÍ¬,1=²»ÏàÍ¬p1>p2,2=²»ÏàÍ¬p1<p2
	if(i==0)
	{
		return 1;
	}
	if(i==1)
	{
		if(JSON->Lastrealtime[5]==0)
		{
			MR((u32)time,(u32)JSON->reportfiletime,7);
			pYYMDHMS_AddS_hex(time,300);//Ã¿Ö´ĞĞÒ»´Î´ò°üÈÎÎñÖ®ºó ·ÖÖÓ´ò°üµÄÊ±¼ä±êÖ¾¶¼¼Ó1·ÖÖÓ
			i=Compare_DL698DataString(JSON->Lastrealtime,time,6);//Êı¾İ´®±È½Ï(¸ßÎ»ÔÚµÍ×Ö½Ú),p1ºÍp2ÒÑÔÚRAMÖĞ,·µ»Ø:0=ÏàÍ¬,1=²»ÏàÍ¬p1>p2,2=²»ÏàÍ¬p1<p2
			if(i!=2)
			{
				//°ÑÕâ10·ÖÖÓÖ®ÄÚµÄ¶«Î÷Æ´ÔÚÒ»ÆğÉÏ±¨Ò»¸öÎÄ¼ş
				Merge_RealTimeData_JsonFile(JSON->Lastrealtime);
			}
		}
		return 1;
	}
	return 0;
}

u8 *GetFileJsonName(u8 *ptime)//µÃµ½ÉÏ±¨ÎÄ¼şµÄÑ¹Ëõ°üÃû×Ö (²»´øÂ·¾¶)
{
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	Len=0;
	p8=(u8 *)ADDR_DATABUFF;
	//´ò°üÍê³ÉÖ®ºó´¦ÀíÎÄ¼şµÄÃû×Ö,¹©ÉÏ´«µÄÊ±ºòµ÷ÓÃ
	Len=0;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:×éÖ¡ÀïÃæÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
	#endif
	return p8;
}

u8 *GetFileJsonName_Compress(u8 *ptime)//µÃµ½ÉÏ±¨ÎÄ¼şÑ¹Ëõ°üµÄÃû×Ö(´ø¾ø¶ÔÂ·¾¶)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÉÏ±¨ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetFrzFileJsonName_Compress(void)//µÃµ½ÉÏ±¨ÎÄ¼şÑ¹Ëõ°üµÄÃû×Ö(´ø¾ø¶ÔÂ·¾¶)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë		
	mystrcpy((char *)p8+Len+8, ".tar.gz");
	//myprintf("[%s:%d]:ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	return p8;
}

u8 *GetRealTime_FileJson(u8 *ptime)//µÃµ½ÉÏ±¨ÎÄ¼şÑ¹Ëõ°üµÄÃû×Ö(´ø¾ø¶ÔÂ·¾¶)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÉÏ±¨ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetRealTime_FileJson_Pack(u8 *ptime)//µÃµ½ÉÏ±¨ÎÄ¼şÑ¹Ëõ°üµÄÃû×Ö(´ø¾ø¶ÔÂ·¾¶)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´µØÖ·
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Ğ´Ê±¼ä
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//µÃµ½698µ±Ç°Ê±¼ä×ª»»³ÉascÂë		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:ÉÏ±¨ÎÄ¼şµÄÃû×Ö  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u32 AnalyzeReportFile(void)//·ÖÎöÉÏ±¨µÄÎÄ¼ş,(ÎÄ¼ş´óĞ¡,ÎÄ¼ş¿é×ÜÊı)
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	u32 filesize;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd)
	{
		i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//Êı¾İ´®±È½Ï(¸ßÎ»ÔÚµÍ×Ö½Ú),p1ºÍp2ÒÑÔÚRAMÖĞ,·µ»Ø:0=ÏàÍ¬,1=²»ÏàÍ¬p1>p2,2=²»ÏàÍ¬p1<p2
		if(i==1)
		{
			pYYMDHMS_AddS_hex(JSON->reportfiletime, 60);
		}
		return 0;
	}
	#ifdef DEBUG_JSON
	myprintf("===============¿ªÊ¼·ÖÎöÎÄ¼ş============= \n",0,0,0);
	#endif
	filesize=ms->link_lseek(fd,0,SEEK_END);
	if(0==filesize)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:ÎÄ¼şÎª¿Õ\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	JSON->g_Filesize=filesize;
	//ĞèÒªÉÏ´«¿éÊıÁ¿
	JSON->g_Need_report_Block=filesize/REPORT_FILEJSON_DATASIZE;
	//ÖÁÉÙÉÏ´«Ò»¿é
	if(0==JSON->g_Need_report_Block)
	{
		JSON->g_Need_report_Block=1;
	}else
	{
		i=filesize%REPORT_FILEJSON_DATASIZE;
		if(i!=0)
		{
			JSON->g_Need_report_Block++;//³¬³öÕû¿éÔÙ¼ÓÒ»¿é
		}
	}
	JSON->g_Block_Number=0;
	ms->link_close(fd);
	
	return 1;
}

u32 GetCurBlockSize(void)//·µ»ØÕâÒ»¿éÉÏ±¨Êı¾İ´óĞ¡
{
	u32 LEN;
	//07 01 08 f0 01 08 00 02 02 12 00 00 09
	LEN=0;
	if(JSON->g_Filesize>((JSON->g_Block_Number+1)*REPORT_FILEJSON_DATASIZE))
	{
		LEN=REPORT_FILEJSON_DATASIZE;
	}else
	{
		if(JSON->g_Filesize>(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE))
		{
			LEN=JSON->g_Filesize-(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE);
		}else
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:´«ÊäĞòºÅ´íÎó JSON->g_Block_Number=%d\n",(u32)&__func__,(u32)__LINE__,JSON->g_Block_Number);
			#endif
			return 0;
		}
	}
	return LEN;
}

u32 GetCurBlockData(u8 *pout,u32 Len)//·µ»ØÉÏ±¨ÕâÒ»¿é¾ßÌåÄÚÈİ
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	if(0==Len)
	{
		return 0;
	}
	//ms->link_system("cd /tmp/real_data");
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	//fd=ms->link_open((char *)JSON->RealFileName,O_RDONLY);
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		return 0;
	}
	ms->link_lseek(fd,JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE,SEEK_SET);
	i=ms->link_read(fd,(u8 *)pout,Len);
	if(i!=Len)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:¶ÁÉÏ±¨ÎÄ¼şÊ§°Ü i=%d\n",(u32)&__func__,(u32)__LINE__,i);
		#endif
		return 0;
	}
	ms->link_close(fd);
	return i;
}

u32 File_HCS(void)//Ëã³öÎÄ¼şµÄĞ£Ñé(Ëã²»³öÀ´ÎÊÌâÓ¦¸ÃÒ²²»ÊÇºÜ´ó)
{
	s32 fd;
	u32 i;
	u32 Len;
	u32 HCS;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:È¡ÎÄ¼şÃûÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:´ò¿ªÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	i=ms->link_lseek(fd,0,SEEK_END);
	p8=(u8 *)ms->ms_malloc(i);
	if(!p8)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:·ÖÅä¿Õ¼äÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	ms->link_lseek(fd,0,SEEK_SET);
	Len=ms->link_read(fd,(u8 *)p8,i);
	if(Len!=i)
	{
		ms->link_close(fd);
		ms->ms_free(p8);
		p8=NULL;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:¶ÁÊı¾İ³¤¶È²»¶Ôi=%d,",(u32)&__func__,(u32)__LINE__,i);
		myprintf("Len=%d\n",Len,0,0);
		#endif
		return 0;
	}
	HCS=0xffff;
	for(i=0;i<Len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; //complement
	
	ms->link_close(fd);
	ms->ms_free(p8);
	p8=NULL;
	return HCS;
}

u32 Report_FileInfo(u16 *pTime,u32 PORTn)//F0010700//Æô¶¯´«Êä
{
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//×î´ó»¹¿É·¢ËÍµÄ×Ö½ÚÊı
	//u8* p8;
	u8* p8tx;
	u8* pfilename;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//¿Í»§»úÁ¬½ÓĞÅÏ¢
	
	
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//×î´ó»¹¿É·¢ËÍµÄ×Ö½ÚÊı
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byteÉÏ±¨ĞÅÏ¢FollowReport OPTIONAL=0 ±íÊ¾Ã»ÓĞ
#if USE_ClientConnectInfo==0//Ê¹ÓÃ¿Í»§»úÁ¬½ÓĞÅÏ¢µÄ¿Í»§»ú×î´ó½ÓÊÕ×Ö½Ú:0=²»Ê¹ÓÃ,1=Ê¹ÓÃ
	ClientConnectInfo=ClientConnectInfo;
#else
	//¿Í»§»ú½ÓÊÕ»º³å³¤¶È
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//¿Í»§»úÁ¬½ÓĞÅÏ¢
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//¼Ä´æÆ÷Êı¾İµ¹Ğò(¸ßµÍ×Ö½Úµ÷»»),Èë¿Ú:Data=¼Ä´æÆ÷ÖĞÊı¾İ,Byte=Ğèµ¹ĞòµÄ×Ö½ÚÊı;·µ»Ø:µ¹ĞòºóµÄÊı¾İ
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//²»Îª0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif


UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

//APDU£º88 04 01 01 40 00 02 00 01 1C 07 E0 0B 0E 00 02 00 00 00	
		p8tx+=TxAPDUOffset;//TxAPDU´æ·Å¿ªÊ¼µØÖ·
		p8tx[0]=0x88;//ÉÏ±¨Í¨Öª REPORT-Notification
		p8tx[1]=0x04;//Í¨ÖªÇëÇó¿Í»§»ú·şÎñÉÏ±¨Í¸Ã÷Êı¾İ
		i=UARTCtrl->REPORT_PIID;
		i++;
		i&=0x3f;
		UARTCtrl->REPORT_PIID=i;
		p8tx[2]=i;//PIID
//--------------ÉÏ±¨Æô¶¯´«ÊäÃüÁîÖ¡£¬Ö¡²ÉÓÃ×Ö·û´®¸ñÊ½£¬ÓÉÓÚÆô¶¯´«ÊäÃüÁîÖ¡ÄÚÈİ¹Ì¶¨£¬ËùÓĞ×Ö·û´®µÄ×Ü³¤¶È¿ÉÒÔ¹Ì¶¨------//			
		p8tx[3]=0x81;
		p8tx[4]=0x89;//Æô¶¯´«ÊäÃüÁîÖ¡µÄ×Ö·û´®³¤¶È
		
		p8tx[5]=0x07;//·½·¨7£¬Æô¶¯ÎÄ¼ş´«Êä
		p8tx[6]=0x01;
		p8tx[7]=i;//PIID
		
		p8tx[8]=0xf0;
		p8tx[9]=0x01;
		p8tx[10]=0x07;
		p8tx[11]=0x00;//ÎÄ¼ş´«ÊäOMD=F0010700
		
		p8tx[12]=0x02;
		p8tx[13]=0x03;//²ÎÊı½á¹¹Ìå
		
		p8tx[14]=0x02;
		p8tx[15]=0x06;//ÎÄ¼şĞÅÏ¢½á¹¹Ìå
		
		p8tx[16]=DataType_visible_string;//Ô´ÎÄ¼ş
		p8tx[17]=0x2f;
		LEN_TxAPDU=18;
		LENmax_TxSPACE-=LEN_TxAPDU+2;//±£Áô2byte(1byteÊ±¼ä±êÇ©,1byteÉÏ±¨ĞÅÏ¢FollowReport OPTIONAL£©

		pfilename=GetFileJsonName(JSON->reportfiletime);
		if(!pfilename)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:ÎÄ¼ş´«ÊäµÄnameÓĞ´í\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;
		p8tx[LEN_TxAPDU]=DataType_visible_string;//Ä¿±êÎÄ¼ş
		p8tx[LEN_TxAPDU+1]=0x2f;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;

		p8tx[LEN_TxAPDU]=0x06;//ÎÄ¼ş´óĞ¡
		LEN_TxAPDU++;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>24)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>16)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>8)&0xff;
		p8tx[LEN_TxAPDU++]=JSON->g_Filesize&0xff;
		LENmax_TxSPACE-=5;

		p8tx[LEN_TxAPDU]=0x04;//ÎÄ¼şÊôĞÔ
		p8tx[LEN_TxAPDU+1]=0x08;//³¤¶È
		p8tx[LEN_TxAPDU+2]=0x80;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x0a;
		p8tx[LEN_TxAPDU+1]=0x04;
		p8tx[LEN_TxAPDU+2]=0x56;
		p8tx[LEN_TxAPDU+3]=0x31;
		p8tx[LEN_TxAPDU+4]=0x2e;
		p8tx[LEN_TxAPDU+5]=0x30;
		LEN_TxAPDU+=6;
		LENmax_TxSPACE-=6;
		//
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x12;//´«Êä¿é´óĞ¡
		p8tx[LEN_TxAPDU+1]=REPORT_FILEJSON_DATASIZE/256;
		p8tx[LEN_TxAPDU+2]=REPORT_FILEJSON_DATASIZE%256;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x02;//Ğ£Ñé
		p8tx[LEN_TxAPDU+1]=0x02;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		p8tx[LEN_TxAPDU+2]=0x09;
		p8tx[LEN_TxAPDU+3]=0x02;
		//¼ÆËãĞ£Ñé
		i=File_HCS();
		p8tx[LEN_TxAPDU+4]=(i>>8)&0xff;
		p8tx[LEN_TxAPDU+5]=i&0xff;
		p8tx[LEN_TxAPDU+6]=0x0;
		LEN_TxAPDU+=7;
		LENmax_TxSPACE-=7;
		
		p8tx[LEN_TxAPDU+0]=0;//Ê±¼ä±êÇ©
		p8tx[LEN_TxAPDU+1]=0;//ÉÏ±¨ĞÅÏ¢FollowReport OPTIONAL
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;

//----------------Æô¶¯´«ÊäEND------------------------// 		

			p8tx-=TxAPDUOffset;//TxAPDU´æ·Å¿ªÊ¼µØÖ·
			p8tx[0]=0x68;
			//2byte³¤¶ÈÓòL
			p8tx[3]=0x83;//¿ØÖÆÓòC
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//Êı¾İµ¹Ğò(¸ßµÍ×Ö½Úµ÷»»),Èë¿ÚLe>=2
			p8tx[11]=0;//¿Í»§»úµØÖ·CA
			//2byteÖ¡Í·Ğ£ÑéHCS
			MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			//2byteĞ£Ñé
			LEN_Tx=14+LEN_TxAPDU+2;
			p8tx[LEN_Tx]=0x16;
			LEN_Tx++;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=(LEN_Tx)&0xff;
			p8tx[2]=(LEN_Tx>>8)&0xff;

			Uart_698_HCSFCS(p8tx);//ÕûÖ¡Ğ£Ñé¼ÆËã(HCSºÍFCS),Í¬Ê±¼Ó½áÊø0x16;·µ»Ø0=ÕıÈ·£¬1=´íÎó

			UARTCtrl->Task=2;//2=µÈ´ıµ¥Ö¡Êı¾İ·¢ËÍ
			//·¢ËÍÖ¡»º´æÔÚDataBuff+LEN_UARTnTx,ÓÃÒÔÖØ·¢
			i=Get_ADDR_UARTnFnDataBuff(PORTn);//µÃµ½UART³­±íÊı¾İ»º³åµØÖ·
			i+=Get_LEN_UARTnTx(PORTn);
			MW((u32)p8tx,i,LEN_Tx+2);
			//ÉÏ±¨ÏìÓ¦³¬Ê±Ê±¼ä	TI
			pTime[0]=0;//Ä¬ÈÏ×îĞ¡Öµ
			UARTCtrl->REPORT_OVER_SECOND=0;//ÉÏ±¨ÏìÓ¦³¬Ê±Ê±¼ä¼Ä´æ
			UARTCtrl->REPORT_NUMmax=0;//ÈôÖ÷Õ¾Ã»Ó¦´ğ×î´ó»¹ĞèÖØ·¢ÉÏ±¨´ÎÊı,Ã¿ÉÏ±¨1´Î¼õ1
			UARTCtrl->HostACK=8;//Ö÷¶¯ÉÏ±¨ĞèÖ÷Õ¾È·ÈÏÖ¡±ê¼Ç:0=ÎŞ,1=µÇÂ¼Ö¡,2=ÍË³öµÇÂ¼Ö¡,3=ĞÄÌøÖ¡,4=ÖØÒªÊÂ¼ş,5=¶¨Ê±ÈÎÎñ,6=µçÄÜ±íÊÂ¼ş,7=ÖÕ¶ËÇëÇóÖ÷Õ¾Ê±ÖÓ
			//×î´óÉÏ±¨´ÎÊı
			#ifdef DEBUG_JSON
			myprintf("======================ÔöÁ¿ÉÏ±¨×Ö½ÚÊı:%d==========\n",UARTCtrl->TxByte,0,0);
			//create_file();
			for(i=0;i<UARTCtrl->TxByte;i++)
				myprintf("%02x ",p8tx[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			return 1;
}

u32 Report_FileData(u16 *pTime,u32 PORTn)//F0010800//ÉÏ±¨Êı¾İÄÚÈİ
{
	u8 *p8tx;
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u16 filesize;
	u32 LENmax_TxSPACE;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef* ClientConnectInfo;
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//×î´ó»¹¿É·¢ËÍµÄ×Ö½ÚÊı
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byteÉÏ±¨ĞÅÏ¢FollowReport OPTIONAL=0 ±íÊ¾Ã»ÓĞ
	#if USE_ClientConnectInfo==0//Ê¹ÓÃ¿Í»§»úÁ¬½ÓĞÅÏ¢µÄ¿Í»§»ú×î´ó½ÓÊÕ×Ö½Ú:0=²»Ê¹ÓÃ,1=Ê¹ÓÃ
	ClientConnectInfo=ClientConnectInfo;
	#else
		//¿Í»§»ú½ÓÊÕ»º³å³¤¶È
		ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//¿Í»§»úÁ¬½ÓĞÅÏ¢
		i=ClientConnectInfo->LEN_Rx;
		i=R_Inverse(i,2);//¼Ä´æÆ÷Êı¾İµ¹Ğò(¸ßµÍ×Ö½Úµ÷»»),Èë¿Ú:Data=¼Ä´æÆ÷ÖĞÊı¾İ,Byte=Ğèµ¹ĞòµÄ×Ö½ÚÊı;·µ»Ø:µ¹ĞòºóµÄÊı¾İ
		if(i<2048)
		{
			i=2048;
		}
		if(i>(TxAPDUOffset+1))
		{//²»Îª0
			i-=(TxAPDUOffset+1);
			if(LENmax_TxSPACE>i)
			{
				LENmax_TxSPACE=i;
			}
		}
	#endif
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	p8tx+=TxAPDUOffset;//TxAPDU´æ·Å¿ªÊ¼µØÖ·
	LEN_TxAPDU=0;
	p8tx[LEN_TxAPDU++]=0x88;
	p8tx[LEN_TxAPDU++]=0x04;
	i=UARTCtrl->REPORT_PIID;
	i++;
	i&=0x3f;
	UARTCtrl->REPORT_PIID=i;
	p8tx[LEN_TxAPDU++]=i;//PIID
	
	filesize=GetCurBlockSize();
	//³¤¶È
	if(256>16+filesize)
	{
		p8tx[LEN_TxAPDU++]=0x81;//
		p8tx[LEN_TxAPDU++]=16+filesize;//³¤¶È
	}else
	{		
		p8tx[LEN_TxAPDU++]=0x82;//
		p8tx[LEN_TxAPDU++]=(17+filesize)>>8;//³¤¶È
		p8tx[LEN_TxAPDU++]=(17+filesize)&0xff;//³¤¶È	
	}
	
	p8tx[LEN_TxAPDU++]=0x07;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=i;

	p8tx[LEN_TxAPDU++]=0xf0;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=0x08;
	p8tx[LEN_TxAPDU++]=0x00;

	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x12;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number>>8;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number&0xff;

	p8tx[LEN_TxAPDU++]=0x09;
	if(LENmax_TxSPACE<LEN_TxAPDU+REPORT_FILEJSON_DATASIZE)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:×Ö·û³¤¶È³¬\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	LENmax_TxSPACE-=(LEN_TxAPDU+REPORT_FILEJSON_DATASIZE);
	//´ÓÎÄ¼şÖĞÌáÈ¡Êı×Ö
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:×Ö·û³¤¶Èfilesize=%d\n",(u32)&__func__,(u32)__LINE__,filesize);
	#endif
	if(filesize<256)
	{
		p8tx[LEN_TxAPDU++]=0x81;
		p8tx[LEN_TxAPDU++]=filesize;
	}else
	{
		p8tx[LEN_TxAPDU++]=0x82;
		p8tx[LEN_TxAPDU++]=filesize/256;
		p8tx[LEN_TxAPDU++]=filesize%256;
	}
	GetCurBlockData(p8tx+LEN_TxAPDU,filesize);
	LEN_TxAPDU+=filesize;	
	p8tx[LEN_TxAPDU++]=0;
	
	p8tx[LEN_TxAPDU++]=0;//Ê±¼ä±êÇ©
	p8tx[LEN_TxAPDU++]=0;//ÉÏ±¨ĞÅÏ¢FollowReport OPTIONAL
				
	//----------------Æô¶¯´«ÊäEND------------------------//			

	p8tx-=TxAPDUOffset;//TxAPDU´æ·Å¿ªÊ¼µØÖ·
	p8tx[0]=0x68;
	//2byte³¤¶ÈÓòL
	p8tx[3]=0x83;//¿ØÖÆÓòC
	p8tx[4]=5;//TSA
	MR((u32)p8tx+5,ADDR_4001+2,6);
	Data_Inverse((u32)p8tx+5,6);//Êı¾İµ¹Ğò(¸ßµÍ×Ö½Úµ÷»»),Èë¿ÚLe>=2
	p8tx[11]=0;//¿Í»§»úµØÖ·CA
	//2byteÖ¡Í·Ğ£ÑéHCS
	MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
	//2byteĞ£Ñé
	LEN_Tx=14+LEN_TxAPDU+2;
	p8tx[LEN_Tx]=0x16;
	LEN_Tx++;
	UARTCtrl->TxByte=LEN_Tx;
	LEN_Tx-=2;
	p8tx[1]=(LEN_Tx)&0xff;
	p8tx[2]=(LEN_Tx>>8)&0xff;
	
	Uart_698_HCSFCS(p8tx);//ÕûÖ¡Ğ£Ñé¼ÆËã(HCSºÍFCS),Í¬Ê±¼Ó½áÊø0x16;·µ»Ø0=ÕıÈ·£¬1=´íÎó
	
	UARTCtrl->Task=2;//2=µÈ´ıµ¥Ö¡Êı¾İ·¢ËÍ
	//·¢ËÍÖ¡»º´æÔÚDataBuff+LEN_UARTnTx,ÓÃÒÔÖØ·¢
	i=Get_ADDR_UARTnFnDataBuff(PORTn);//µÃµ½UART³­±íÊı¾İ»º³åµØÖ·
	i+=Get_LEN_UARTnTx(PORTn);
	MW((u32)p8tx,i,LEN_Tx+2);
	//ÉÏ±¨ÏìÓ¦³¬Ê±Ê±¼ä  TI
	pTime[0]=0;//Ä¬ÈÏ×îĞ¡Öµ
	UARTCtrl->REPORT_OVER_SECOND=0;//ÉÏ±¨ÏìÓ¦³¬Ê±Ê±¼ä¼Ä´æ
	UARTCtrl->REPORT_NUMmax=0;//ÈôÖ÷Õ¾Ã»Ó¦´ğ×î´ó»¹ĞèÖØ·¢ÉÏ±¨´ÎÊı,Ã¿ÉÏ±¨1´Î¼õ1
	UARTCtrl->HostACK=8;//Ö÷¶¯ÉÏ±¨ĞèÖ÷Õ¾È·ÈÏÖ¡±ê¼Ç:0=ÎŞ,1=µÇÂ¼Ö¡,2=ÍË³öµÇÂ¼Ö¡,3=ĞÄÌøÖ¡,4=ÖØÒªÊÂ¼ş,5=¶¨Ê±ÈÎÎñ,6=µçÄÜ±íÊÂ¼ş,7=ÖÕ¶ËÇëÇóÖ÷Õ¾Ê±ÖÓ,8·ÖÖÓÉÏ±¨ÈÎÎñ
	#ifdef DEBUG_JSON
	myprintf("======================ÔöÁ¿ÉÏ±¨×Ö½ÚÊı:%d==========\n",LEN_Tx,0,0);
	for(i=0;i<UARTCtrl->TxByte;i++)
		myprintf("%02x ",p8tx[i],0,0);
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

u32 Report_JsonFile(u16 *pTime,u32 PORTn)//PORTn¶Ë¿ÚºÅ,pTime¶Ë¿Ú³¬Ê±Ê±¼ä
{
	//u16 *p16time;
	u32 i;
	u32 ret;
	ret=0;
	if(FileDownload->New==0)//Ô¶³ÌÉı¼¶Íê³É,ÉÏ±¨Í£Ö¹
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->reportfiletime);
	if(i)
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
	if(i)
	{
		return ret;
	}
	#ifdef DEBUG_JSON
	myprintf("===============½øÈëÉÏ±¨µÄ³ÌĞòJSON->Reportjsonfile_Task = %d============= \n",JSON->Reportjsonfile_Task,0,0);
	#endif
	switch(JSON->Reportjsonfile_Task)
	{
		case 0://·ÖÎöÉÏ±¨ÎÄ¼ş
		//´Ó½ñÌìµÄµÚÒ»¸öÉú³ÉµÄÎÄ¼ş¿ªÊ¼ÉÏ±¨	
			i=Selec_ReportTime();
			if(0==i)
			{
				return ret;
			}
			i=AnalyzeReportFile();
			if(i)
			{
				#ifdef DEBUG_JSON
				myprintf("===============·ÖÎöÎÄ¼ş³É¹¦,ÏÂÃæ¿ªÊ¼ÉÏ´«ÎÄ¼ş============= \n",0,0,0);
				#endif
				JSON->Reportjsonfile_Task++;
				ret=1;
			}
			break;
		case 1://Æô¶¯´«Êä
			i=Report_FileInfo(pTime,PORTn);
			if(i)
			{
				JSON->Reportjsonfile_Task++;
				ret=1;
			}else
			{
				//ÉÏ±¨Ê§°ÜµÄÊ±ºò,ÉÏ±¨Ê±¼äĞèÒª¼ÓÒ»Ìø¹ıÕâ¸öÎÄ¼ş,²»È»Ò»Ö±ËÀÑ­»·
				ClrReportInfo();
			}
			break;
		case 2://ÉÏ´«ÎÄ¼ş
			#ifdef DEBUG_JSON
			myprintf("===============¿ªÊ¼ÉÏ´«ÎÄ¼ş============= \n",0,0,0);
			#endif
			i=Report_FileData(pTime,PORTn);
			if(i)
			{
				Next_DataBlock();
				if(IsReportEnd())//½áÊøÁË
				{
					#ifdef DEBUG_JSON
					myprintf("===============ÉÏ´«ÎÄ¼ş³É¹¦============= \n",0,0,0);
					#endif
					ClrReportInfo();
				}
				ret=1;
			}else
			{
				ClrReportInfo();
			}
			break;
		default:
			JSON->Reportjsonfile_Task=0;
		break;
	}
	return ret;
}
/*
void tar_text(void)
{
	
	u32 i;
	u32 SLen;
	u32 size;
	u8 *p8;

	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	p8=(u8 *)ADDR_DATABUFF;
	//while(1)
	{
		SLen=0;
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "111.tar.gz ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "/usr/app/frz_data/");
		size=ms->ms_malloc_sizemax();
		//p8=(u8 *)ms->ms_malloc(13*1024*1024);
		//if(!p8)
		{
			//return;
		}
		myprintf("size=%d\n",size,0,0);
		
		myprintf("¿ªÊ¼Ñ¹Ëõ \n",0,0,0);
		myprintf("[%s:%d]:ÃüÁî %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
		ms->link_system((char *)p8);
		myprintf("Ñ¹Ëõ½áÊø \n",0,0,0);
		//ms->link_sleep(1);
	}
}

void save(u8 *pbuf,u32 FileID)
{
	u32 i;
	u32 x;
	u32 y;
	//u32 FileID;
	//u32 EventID;//ÊÂ¼ş¹ØÁªµÄID
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	//u8* p8;
	//UARTCtrl_TypeDef *UARTCtrl;
	Comm_Ram->msFILEchange=0;//ÎÄ¼ş¹ÜÀíÊı¾İ±ä»¯:0=±ä»¯,!=Ã»±ä»¯
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//FileID=Get_FileID(PORTn);//UART¸÷ÈÎÎñµÄFileID=b31-b28·½°¸ÀàĞÍ,b27-b24²É¼¯ÀàĞÍ,b23-b20´æ´¢Ê±±êÀàĞÍ,b19-b12ÈÎÎñºÅ,b11-b8¶Ë¿ÚºÅ,b7-b0ÎÄ¼ş·ÖÀà
	Comm_Ram->msFILESearchCount=0;

		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{	
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//Ì‘Ì·Ï„İ¾Ä»:É«à ?FileID=b31-b28×½Ğ¸`Ñ,b27-b24Ó‰Ü¯`Ñ,b23-b20Õ¦Ô¢Ê±Òª`Ñ,b19-b12ÉÏ±Û…,b11-b8×‹à šÛ?b7-b0,Ğ Ó¦Ô²ÖµÎª0xFÒ­Ê¾Ò»Ú˜×¢,pTimeË½ßÕ¦Ô¢Ê±Òª=0Ò­Ê¾Ò»Ú˜×¢;SearchCount=ßªÊ¼Ì‘Ì·Ö„Ï„İ¾Û…0xFFFFFFFFÒ­Ê¾Õ“ÊÕŞ¡Ë¸Û…ßªÊ¼Ì‘Ì·;×µÜ˜:0--(FILENAMEmax_FLASH-1)Ò­Ê¾Ï„İ¾ÕšFLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)Ò­Ê¾Ï„İ¾ÕšRAM,0xffffffffÒ­Ê¾Ã»Ö’Õ½
		}
		else
		{
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//Ì‘Ì·Ï„İ¾Ä»:É«à ?FileID=b31-b28×½Ğ¸`Ñ,b27-b24Ó‰Ü¯`Ñ,b23-b20Õ¦Ô¢Ê±Òª`Ñ,b19-b12ÉÏ±Û…,b11-b8×‹à šÛ?b7-b0,Ğ Ó¦Ô²ÖµÎª0xFÒ­Ê¾Ò»Ú˜×¢,pTimeË½ßÕ¦Ô¢Ê±Òª=0Ò­Ê¾Ò»Ú˜×¢;SearchCount=ßªÊ¼Ì‘Ì·Ö„Ï„İ¾Û…0xFFFFFFFFÒ­Ê¾Õ“ÊÕŞ¡Ë¸Û…ßªÊ¼Ì‘Ì·;×µÜ˜:0--(FILENAMEmax_FLASH-1)Ò­Ê¾Ï„İ¾ÕšFLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)Ò­Ê¾Ï„İ¾ÕšRAM,0xffffffffÒ­Ê¾Ã»Ö’Õ½
		}

		//log_Flash();

	
	if(filename!=0xffffffff)
	{
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		y=file->NUM_DATA;//ÎÄ¼ş´æÈëÊı¾İ¸öÊı¼ÆÊı(Í¬³­±í³É¹¦¸öÊı)
		if(y>NUM_RMmax)
		{
			y=NUM_RMmax;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
		RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
		Addr0=pbuf[0]|(pbuf[1]<<8);
		Addr1=pbuf[2]|(pbuf[3]<<8);
		Addr2=pbuf[4]|(pbuf[5]<<8);
		for(i=0;i<y;i++)
		{
			if(RecordFileHead->Addr0==Addr0)
			{
				if(RecordFileHead->Addr1==Addr1)
				{
					if(RecordFileHead->Addr2==Addr2)
					{
						break;
					}
				}
			}
			RecordFileHead++;
		}
		if(i<y)
		{//µØÖ·ÒÑ´æÔÚ
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//²¹³­ÇúÏßµÈÊı¾İÊ±¿ÉÄÜ»áÖØ¸´,¹Ê²»ÄÜÉ¾³ıÎÄ¼ş
			}
		}
	}
	if(filename==0xffffffff)
	{//Ã»ÕÒµ½

		{//FLASHÎÄ¼ş	
			while(1)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
				{
					if(file->BLOCK_ENTRY==0)
					{
						break;
					}
					file++;
				}
				if(filename>=recordFILENAMEmax_FLASH)
				{//Ã»¿Õ
					//myprintf("[%s:%d]:¿ªÊ¼Ö´ĞĞÉ¾³ıÃüÁî\n",(u32)&__func__,(u32)__LINE__,0);
					i=AutoDeleteFile(0);//×Ô¶¯É¾³ı¼ÇÂ¼ÎÄ¼ş(×Ô¶¯¼õÉÙ´æ´¢Éî¶È);Èë¿Ú:Ğ´ÈëÎÄ¼şÊ±¿Õ¼ä²»¹»µÄÎÄ¼şÃû,ÒÔÅĞ±ğÊÇRAM»¹ÊÇFLASH¿Õ¼ä²»¹»;·µ»Ø:0=Ã»É¾³ı,1=ÓĞÉ¾³ı
					if(i)
					{//ÓĞÉ¾³ı
						continue;
					}
					return;
				}
				break;
			}
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//¿ÕÎÄ¼ş
		ms->msfile_lenclear(filename);//Çå0ÎÄ¼şÏà¹ØµÄ³¤¶È´ÎÊı,0-(FILENAMEmax_FLASH-1)±íÊ¾ÎÄ¼şÔÚFLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)±íÊ¾ÎÄ¼şÔÚRAM
		file->FILEID=FileID;
		file->FILECOUNT=0;

			file->LEN_DATA=0;
			file->NUM_DATA=0;
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//²É¼¯´æ´¢Ê±±ê
				MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//²É¼¯Æô¶¯Ê±¼ä
			}else
			{
				MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//²É¼¯´æ´¢Ê±±ê
				MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//²É¼¯Æô¶¯Ê±¼ä
			}
	}
//Ğ´ÎÄ¼ş
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//´í
		ms->msfile_Delete(filename);
		return;
	}
	//Ğ´Êı¾İ
	if(filename>=FILENAMEmax_FLASH)
	{//RAMÖĞÎÄ¼ş
		i=ms->ms_malloc_sizemax();//×î´ó¿É·ÖÅäµÄ³ß´ç(×Ö½ÚÊı)
		if(i<(1024*1024))
		{//¿Õ¼ä<1M,±£ÁôÖÁÉÙ1M¿Õ¼äÓÃÓÚ¼ÇÂ¼Êı¾İ¶ÁÈ¡
			y=AutoDeleteFile(filename);//×Ô¶¯É¾³ı¼ÇÂ¼ÎÄ¼ş(×Ô¶¯¼õÉÙ´æ´¢Éî¶È);Èë¿Ú:Ğ´ÈëÎÄ¼şÊ±¿Õ¼ä²»¹»µÄÎÄ¼şÃû,ÒÔÅĞ±ğÊÇRAM»¹ÊÇFLASH¿Õ¼ä²»¹»;·µ»Ø:0=Ã»É¾³ı,1=ÓĞÉ¾³ı
			if(y==0)
			{//Ã»É¾³ı
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	//myprintf("[%s:%d]:RecordFileHead->Addr0=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr0);
	///myprintf("[%s:%d]:RecordFileHead->Addr1=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr1);
	//myprintf("[%s:%d]:RecordFileHead->Addr2=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr2);
	//myprintf("[%s:%d]:RecordFileHead->LEN_DATA=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->LEN_DATA);

	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//Ğ´´íÎó
			y=AutoDeleteFile(filename);//×Ô¶¯É¾³ı¼ÇÂ¼ÎÄ¼ş(×Ô¶¯¼õÉÙ´æ´¢Éî¶È);Èë¿Ú:Ğ´ÈëÎÄ¼şÊ±¿Õ¼ä²»¹»µÄÎÄ¼şÃû,ÒÔÅĞ±ğÊÇRAM»¹ÊÇFLASH¿Õ¼ä²»¹»;·µ»Ø:0=Ã»É¾³ı,1=ÓĞÉ¾³ı
			if(y)
			{//ÓĞÉ¾³ı
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//Ğ´µØÖ·ÁĞ±í
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//Êı¾İÊı+1
	x++;
	file->NUM_DATA=x;
	//myprintf("[%s:%d]:Ğ´ÈëFlash³É¹¦\n",(u32)&__func__,(u32)__LINE__,0);

}

u32 MoreThan_StackLen(u32 LENMAX)
{
	return 0;
}
u32 GetOneMetAddr_TEST(u16 *MetCount)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	for(;Met<TEST_METNUM;Met++)
	{	
		//i=bcd_hex((Met)+1);
		i=Met;
		//myprintf("i=%d \n",i,0,0);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;
		p16+=3;
		break;
	}
	if((Met)>=TEST_METNUM)
	{
		p16[0]=0;
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;
	p8=(u8 *)(ADDR_AddrList+2);
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	return 1;
}
 */
u32 CalCulate_Space(const char *path)
{
	return 8*1024*1024;
}

void Delet_RAM_FrzFile(void)
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

u32  Copy_RamToFlash(s32 fd)
{
	s32 fd1;
	u32 i;
	u32 filelen;
	u32 x;
	u32 y;
	u8 *p8;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	Mkdir_JsonFile(JSONFREPATH);
	pname=CreateFreJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
	fd1=ms->link_open((char *)pname,O_RDWR);
	if(fd1==-1)//ÄÜÕÒµ½Õâ¸öÎÄ¼ş
	{
		fd1=ms->link_open((char *)pname,O_CREAT|O_RDWR);
		if(fd1==-1)
		{
			return 1;
		}
	}
		ms->link_lseek(fd1,0,SEEK_END);
		p8=(u8 *)ms->ms_malloc(RAMTOFLASH_COPYONESIZE);
		if(!p8)
		{
			ms->link_close(fd1);
			return 1;
		}
		filelen=ms->link_lseek(fd,0,SEEK_END);//RAM¿Õ¼äÏÂµÄÎÄ¼ş´óĞ¡
		if(filelen<=(RAMTOFLASH_COPYONESIZE))
		{
			//¿ÉÒÔÖ±½Ó¿½±´¹ıÈ¥
			ms->link_lseek(fd,0,SEEK_SET);//RAM¿Õ¼äÏÂµÄÎÄ¼ş´óĞ¡
			i=ms->link_read(fd,(u8 *)p8,filelen);
			if(i==filelen)//¶Á³É¹¦
			{
				i=ms->link_write(fd1,(u8 *)p8,filelen);
				if(i==filelen)
				{
					ms->link_close(fd1);
					return 0;
				}else//Ğ´²Ù×÷Ê§°ÜÓĞ¿ÉÄÜÊÇ´æ´¢¿Õ¼äÒÑ¾­ÂúÁËFlash,ĞèÒªÉ¾³ıFilename
				{
					
				}
			}
		}else
		{
			y=RAMTOFLASH_COPYONESIZE;
			x=0;
			while(x<filelen)
			{
				ms->link_lseek(fd,x,SEEK_SET);//RAM¿Õ¼äÏÂµÄÎÄ¼ş´óĞ¡
				if(filelen>=(y+x))//×ÜÎÄ¼ş´óĞ¡=×î´óÒ»´ÎĞ´ÈëÁ¿+ÒÑ¾­Ğ´ÈëÁ¿
				{//ÄÇ¾ÍÓÃ×î´óĞ´ÈëÁ¿Ğ´Èë
					i=ms->link_read(fd,(u8 *)p8,y);
					if(y==i)
					{
						i=ms->link_write(fd1,(u8 *)p8,y);
					}else//Ğ´²Ù×÷Ê§°ÜÓĞ¿ÉÄÜÊÇ´æ´¢¿Õ¼äÒÑ¾­ÂúÁËFlash,ĞèÒªÉ¾³ıFilename
					{
						
					}
					x+=y;
				}else
				{
					if(filelen>x)
					{
						y=filelen-x;
						i=ms->link_read(fd,(u8 *)p8,y);
						if(y==i)
						{
							i=ms->link_write(fd1,(u8 *)p8,y);
							if(y==i)
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 0;
							}else
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 1;
							}
						}
					}else//ËµÃ÷ÒÑ¾­È«²¿Ğ´ÍêÁË
					{
						Delet_RAM_FrzFile();
						ms->link_close(fd1);
						return 0;
					}
				}
				
			}
		}
		ms->link_close(fd1);
		return 1;
	
}

u32 Find_TodayFrzFile()
{
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pname=GetFrzFileJsonName_Compress();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);
	if(fd!=-1)
	{
		ms->link_close(fd);
		return 0;
	}
	return 1;
}

/*
void TEST_PACK(void)
{
	u32 i;
	u32 x;
	u32 CJSONLEN;//JSON×Ö·û´®¼ÆÊıÆ÷,³õÊ¼»¯0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//µØÖ·u8ÀàĞÍÖ¸Õë
	u8 *pname;
	u16 *p16;
	u16 AddrNUM;//µµ°¸µÄÓĞĞ§ÊıÁ¿
	s32 fd;
	//u8 addr[6]={0};
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch(FrzPackTask)
	{
		case 0://´ò°üÖ®Ç°ĞèÒª×öµÄ×¼±¸
			CurMetCount=0;
			FrzPackTask++;
			//´ò°üÖ®Ç°Çå¿ÕRAMÏÂÃæËùÓĞµÄ¿Õ¼ä
			Delet_RAM_FrzFile();
			break;
		case 1:	
			//i=GetOneMetAddr();
			i=GetOneMetAddr_TEST();
			if(i==0)//È«²¿±íµØÖ·ÒÑ¾­´ò°ü½áÊø
			{
				FrzPackTask=2;//ÖØÖÃÃ¿Ìì´ò°üÈÕ¶³½áµÄ±êÖ¾
				myprintf("[%s:%d]:Êı¾İ´¦ÀíÍê±Ï¿ªÊ¼Ñ¹Ëõ\n",(u32)&__func__,(u32)__LINE__,0);
				break;
			}
			CurMetCount++;//ÏÂÒ»´ÎÑ­»·¾ÍÕÒÏÂÒ»Ö»±í
			//µÃµ½ÎÄ¼şÃû×Ö
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=8*1024*1024))
				//if(1024)
				{
					//´ÓRAM¿Õ¼äÒÆ¶¯µ½Flash¿Õ¼ä
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				myprintf("[%s:%d]:ÉêÇë¿Õ¼äÊ§°Üi\n",(u32)&__func__,(u32)__LINE__,0);
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//Ç°ÃæÒ»¸öÊÇ±íµÄÊıÁ¿,ºóÃæÒ»¸öÊÇ±íµÄµØÖ·
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//ËµÃ÷ÊÇÓĞÊı¾İµÄ
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//É¾³ı¶ººÅ
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//ÕâÒ»Ö»±íÊ¾ÓĞÊı¾İµÄ
			{
				//ÏÂÃæ¿ªÊ¼ÍùRAM¿Õ¼äÀïÃæĞ´Êı¾İ
				i=CalCulate_Space(JSONFREPATH_RAM);//¼ÆËãÕâ¸ö¿Õ¼äÀïÃæÊ£ÓàµÄÈİÁ¿
				if(i>=CJSONLEN)//Èç¹ûÊ£ÓàµÄ¿Õ¼ä¹»ÓÃ
				{
					//µÃµ½ÎÄ¼şÃû×Ö
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//¹Ø±ÕÎÄ¼ş¾ä±ú
			}
			ms->ms_free(p8);
			break;
		case 2:
			//µÃµ½ÎÄ¼şÃû×Ö
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//´ÓRAM¿Õ¼äÒÆ¶¯µ½Flash¿Õ¼ä
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				Compress_JSONFile(pname,1024,0);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				FrzPackTask=0;	
				//ËµÃ÷´ò°üÊ§°ÜÁË
				if(JSON->IsCreateFrzJson==2)
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
				}
				else
				{
					JSON->IsCreateFrzJson++;
				}
			}	
			break;
		default:
			FrzPackTask=0;
		break;
	}

//===============================1.È¡µµ°¸ÀïÃæËùÓĞµÄ±íµØÖ·=========================== 
	//GetMetAddr();//È¡µÃËùÓĞ±íµÄµØÖ·	
	p16=(u16*)(ADDR_AddrList);//È¡µÃµµ°¸µÄÊıÁ¿ÓÃunsigned short±íÊ¾
	p16[0]=2;
	p16++;	
	for(x=0;x<2;x++)
	{	
		i=hex_bcd(x+1);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;

		p8=(u8 *)p16;
		for(i=0;i<3;i++)
			myprintf("%04x ",p16[i],0,0);
		myprintf("\n",0,0,0);
		p16+=3;
	}
	AddrNUM=2;
	//p16++;		
//===============================2.Ïò¶ÑÉêÇë¿Õ¼ä=====================================
	p8=(u8 *)ms->ms_calloc(1024);
	if(!p8)
	{
		myprintf("[%s:%d]:malloc¿Õ¼ä²»×ã\n",(u32)&__func__,(u32)__LINE__,0);
		return ;
	}
//==============================3.¿ªÊ¼´¦ÀíÊı¾İ========================================
	CJSONLEN=0;//JSON×Ö·û´®¼ÆÊıÆ÷,³õÊ¼»¯0
	p8s=(u8 *)ADDR_DATABUFF;

	p16=(u16*)(ADDR_AddrList);//È¡µÃµµ°¸µÄÊıÁ¿ÓÃunsigned short±íÊ¾
	p16++;
	for(x=0;x<AddrNUM;x++)
	{
		if(0!=x)
		{
			p16+=3;
		}
		pAddr=(u8 *)p16;
		JSONDataLen=0;
		i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		Add_StrToASCII(p8s,pAddr,6);//pbufÀïÃæµÄ×Ö½ÚÊä³öµ½pstr²¢×ª³ÉascÂë
		i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=AllDataPack(p16,p8+CJSONLEN);
		CJSONLEN+=i;
		
		if(i>0)//ËµÃ÷ÊÇÓĞÊı¾İµÄ
		{
			i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			CJSONLEN=Delet_COMMA(p8,CJSONLEN);//É¾³ı¶ººÅ
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;			
		}else
		{		
			if(CJSONLEN>=JSONDataLen)
			{
				CJSONLEN-=JSONDataLen;
			}
		}	
	}
	if(CJSONLEN)
	{
		Delet_FrzJsonFile();//É¾³ıµôÔ­À´µÄJSONÎÄ¼ş
		pname=CreateFreJsonFile();//´´½¨Ò»¸öĞÂµÄÏîÄ¿ÎÄ¼ş,·µ»ØÎÄ¼ş¾ä±ú
		fd=ms->link_open((char *)pname,O_RDWR);
		if(-1!=fd)//ÒÑ¾­´æÔÚ¸ÃÎÄ¼ş¾ÍÉ¾³ı
		{
			//ms->link_remove((char *)pname);
			//myprintf("\n\n[%s:%d]:É¾³ıÎÄ¼ş:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
			}else
			{
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}else
		{
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				myprintf("[%s:%d]:´´½¨ÎÄ¼şÊ§°Ü\n",(u32)&__func__,(u32)__LINE__,0);
				return;
			}
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼ş³É¹¦  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//½«JSONÎÄ¼ş´ò°ü³Étar.gz¸ñÊ½
			}else
			{
				myprintf("[%s:%d]:Ğ´ÈëÎÄ¼şÊ§°Ü  ×Ö·ûÊıÁ¿i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}
		ms->link_close(fd);
	}
	ms->ms_free(p8);
	*/
	/*
u8 day[]={0x01,0x01,0x5b,0x01,0x50,0x04,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x00,0x00,0x20,0x02,0x00,0x1C,0x07,0xE4,0x07,0x0A,0x00,0x00,0x00,0x01,0x05,0x06,0x00,0x1C,0x34,0x6B,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x52,0x9D,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0C,0xE1,0xCD,0x01,0x05,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
//u8 min15[]={0x50,0x02,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x01,0x1C,0x06,0x06,0x0A,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1C,0x34,0x6B,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x2D,0x84,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x21,0xC8,0x48,0x00,0x00,0x00,0x0C,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x5D,0xF2,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,
	//0x09,0x16,0x00,0x00,0x00,0x1F,0x78,0xC6,0x00,0x00,0x00,0x34,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x26,0xC3,0xD3,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1E,0xFB,0xD1,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1F,0x91,0xAF,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1D,0x34,0xD5,0x00,0x00,0x00,0x08,
	//0x00,0x00
//};

u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x0B,0x20,0x00,0x02,0x01,0x20,0x00,0x02,0x02,0x20,0x00,0x02,0x03,0x20,0x01,0x02,0x01,0x20,0x01,0x02,0x02,0x20,0x01,0x02,0x03,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x20,0x04,0x02,0x03,0x20,0x04,0x02,0x04,0x01,0x12,0x12,0x12,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x06,0x08,0xF8,0x08,0xED,0x08,0xF1,0x00,0x00,0x08,0xEB,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC7,0x00,0x00,0x03,0x58,0x00,0x00,
	0x4C,0x5C,0x00,0x00,0x14,0x79,0x00,0x00,0x1A,0xA7,0x00,0x00,0x1D,0x3A,0x08,0xFB,0x08,0xF0,0x08,0xF5,0x00,0x00,0x08,0xF4,0x00,0x00,0x0B,0xAF,0x00,0x00,0x0C,0xCA,0x00,0x00,0x03,0x52,0x00,0x00,0x4C,0x9B,0x00,0x00,0x14,0x96,0x00,0x00,0x1A,0xB5,0x00,0x00,0x1D,0x4E,0x08,0xFA,0x08,0xF0,0x08,0xF4,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x50,0x00,0x00,0x14,0x70,0x00,0x00,0x1A,0xA8,0x00,
	0x00,0x1D,0x38,0x08,0xFC,0x08,0xF1,0x08,0xF6,0x00,0x00,0x08,0xF0,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0xA0,0x00,0x00,0x14,0x94,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xF0,0x08,0xF3,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAB,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x70,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xA8,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xEF,0x08,0xF3,0x00,0x00,
	0x08,0xED,0x00,0x00,0x0B,0xAA,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x90,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x00,0x00
};

//u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x05,0x20,0x00,0x02,0x01,0x20,0x01,0x02,0x01,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x01,0x12,0x07,0x05,0x05,0x05,0x0A,0x00,0x00,0x00};

u8 min15[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x06,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x00,0x50,0x02,0x01,0x00,0x60,0x02,0x01,0x00,0x70,0x02,0x01,0x00,0x80,0x02,0x01,0x01,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x00,0x07,0x72,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0xA8,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x72,0xD5,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x73,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x32,0xC9,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x32,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAD,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x07,0x71,0xF7,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x24,0x00,0x00,0x00,0x00,0x00,
	0x00,0x51,0xA7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

VOID test_saveMIN(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
		p8=(u8 *)ADDR_DATABUFF;
	myprintf("·ÖÖÓ´æ´¢ \n",0,0,0);

	for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min,sizeof(min));
			Offset+=sizeof(min);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f22000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("È«²¿¶¼Ğ´Íê³É \n",0,0,0);
}

VOID test_saveMIN15(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("·ÖÖÓ´æ´¢ \n",0,0,0);

	for(FILEID=0;FILEID<96;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min15);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min15,sizeof(min15));
			Offset+=sizeof(min15);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f19000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 900);
		//break;
	}
	myprintf("È«²¿¶¼Ğ´Íê³É \n",0,0,0);
}

VOID test_saveDAY(void)
{
	u32 i;
	u16 datalen;
	//u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1a,0x17,0x3b,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("½øÈëÌì´æ´¢ \n",0,0,0);
	//for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(day);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)day,sizeof(day));
			Offset+=sizeof(day);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f16000);
			//break;
		}
		//pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("È«²¿¶¼Ğ´Íê³É \n",0,0,0);
}
*/

