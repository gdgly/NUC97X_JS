
//#if MainProtocol==376
//#include "../Head/Project.h"
//#endif
//#if MainProtocol==698
//#include "../H698/Project698.h"
//#include "../DL698/DL698_DataDef.h"
//#endif
#include "../Hcsg/ProjectCSG.h"
#include "../GPRS/GPRS_FTP_RxTx.h"
#include "../GPRS/ATString.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../OS/My_Heap.h"


u32 GPRS_FTP_Tx(void)		//GPRS_FTP_Tx发送;返回发送字节数

{
	return 0;
}


void GPRS_FTP_Rx(void)		//GPRS_FTP_Rx接收
{

}


