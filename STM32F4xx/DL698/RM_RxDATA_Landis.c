
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/RM_RxDATA.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"



const u8 LandisData_1[]=
{
	"/LGZ4    \r\n",
};
const u8 LandisData_2[]=
{
	02,0x46,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x29,0x0D,0x0A,
//0(84045764)
	0x30,0x28,0x38,0x34,0x30,0x34,0x35,0x37,0x36,0x34,0x29,0x0D,0x0A,
//11(16:03:55)
	0x31,0x31,0x28,0x31,0x36,0x3A,0x34,0x36,0x3A,0x31,0x39,0x29,0x0D,0x0A,
//12(18-08-20)
	0x31,0x32,0x28,0x31,0x38,0x2D,0x30,0x38,0x2D,0x32,0x30,0x29,0x0D,0x0A,
//20(000004.64*kWh)
	0x32,0x30,0x28,0x30,0x30,0x30,0x30,0x30,0x34,0x2E,0x36,0x34,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//20*7(000004.64)
	0x32,0x30,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x34,0x2E,0x36,0x34,0x29,0x0D,0x0A,
//22(000000.04*kWh)
	0x32,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x34,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//22*7(000000.04)
	0x32,0x32,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x34,0x29,0x0D,0x0A,
//21(000002.33*kvarh)
	0x32,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x32,0x2E,0x33,0x33,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//21*7(000002.33)
	0x32,0x31,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x32,0x2E,0x33,0x33,0x29,0x0D,0x0A,
//23(000000.19*kvarh)
	0x32,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x39,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//23*7(000000.19)
	0x32,0x33,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x39,0x29,0x0D,0x0A,
//8.1(000001.07*kWh)
	0x38,0x2E,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x31,0x2E,0x30,0x37,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//8.1*7(000001.07)
	0x38,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x31,0x2E,0x30,0x37,0x29,0x0D,0x0A,
//8.2(000003.44*kWh)
	0x38,0x2E,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x33,0x2E,0x34,0x34,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//8.2*7(000003.44)
	0x38,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x33,0x2E,0x34,0x34,0x29,0x0D,0x0A,
//8.3(000000.12*kWh)
	0x38,0x2E,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x32,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//8.3*7(000000.12)
	0x38,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x32,0x29,0x0D,0x0A,
//8.4(000000.00*kWh)
	0x38,0x2E,0x34,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//8.4*7(000000.00)
	0x38,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//38.1(000000.00*kWh)
	0x33,0x38,0x2E,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//38.1*7(000000.00)
	0x33,0x38,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//38.2(000000.01*kWh)
	0x33,0x38,0x2E,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x31,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//38.2*7(000000.01)
	0x33,0x38,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x31,0x29,0x0D,0x0A,
//38.3(000000.03*kWh)
	0x33,0x38,0x2E,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x33,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//38.3*7(000000.03)
	0x33,0x38,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x33,0x29,0x0D,0x0A,
//38.4(000000.00*kWh)
	0x33,0x38,0x2E,0x34,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x68,0x29,0x0D,0x0A,
//38.4*7(000000.00)
	0x33,0x38,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//9.1(000000.45*kvarh)
	0x39,0x2E,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x34,0x35,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//9.1*7(000000.45)
	0x39,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x34,0x35,0x29,0x0D,0x0A,
//9.2(000001.75*kvarh)
	0x39,0x2E,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x31,0x2E,0x37,0x35,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//9.2*7(000001.75)
	0x39,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x31,0x2E,0x37,0x35,0x29,0x0D,0x0A,
//9.3(000000.12*kvarh)
	0x39,0x2E,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x32,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//9.3*7(000000.12)
	0x39,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x32,0x29,0x0D,0x0A,
//9.4(000000.00*kvarh)
	0x39,0x2E,0x34,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//9.4*7(000000.00)
	0x39,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//39.1(000000.00*kvarh)
	0x33,0x39,0x2E,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//39.1*7(000000.00)
	0x33,0x39,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//39.2(000000.01*kvarh)
	0x33,0x39,0x2E,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x31,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//39.2*7(000000.01)
	0x33,0x39,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x31,0x29,0x0D,0x0A,
//39.3(000000.17*kvarh)
	0x33,0x39,0x2E,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x37,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//39.3*7(000000.17)
	0x33,0x39,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x31,0x37,0x29,0x0D,0x0A,
//39.4(000000.00*kvarh)
	0x33,0x39,0x2E,0x34,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x76,0x61,0x72,0x68,0x29,0x0D,0x0A,
//39.4*7(000000.00)
	0x33,0x39,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x2E,0x30,0x30,0x29,0x0D,0x0A,
//39.4*7(000000.00)
	0x36,0x2E,0x30,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.0(00.00*kW)(00-00-00 00:00)
	0x36,0x2E,0x30,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.0*7(00.00)(00-00-00 00:00)
	0x33,0x36,0x2E,0x30,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.0(00.00*kW)(00-00-00 00:00)
	0x33,0x36,0x2E,0x30,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.0*7(00.00)(00-00-00 00:00)
	0x37,0x2E,0x30,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x76,0x61,0x72,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//7.0(00.00*kvar)(00-00-00 00:00)
	0x37,0x2E,0x30,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//7.0*7(00.00)(00-00-00 00:00)
	0x33,0x37,0x2E,0x30,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x76,0x61,0x72,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//37.0(00.00*kvar)(00-00-00 00:00)
	0x33,0x37,0x2E,0x30,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//37.0*7(00.00)(00-00-00 00:00)
	0x36,0x2E,0x31,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.1(00.00*kW)(00-00-00 00:00)
	0x36,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.1*7(00.00)(00-00-00 00:00)
	0x36,0x2E,0x32,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.2(00.00*kW)(00-00-00 00:00)
	0x36,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.2*7(00.00)(00-00-00 00:00)
	0x36,0x2E,0x33,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.3(00.00*kW)(00-00-00 00:00)
	0x36,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.3*7(00.00)(00-00-00 00:00)
	0x36,0x2E,0x34,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.4(00.00*kW)(00-00-00 00:00)
	0x36,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//6.4*7(00.00)(00-00-00 00:00)
	0x33,0x36,0x2E,0x31,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.1(00.00*kW)(00-00-00 00:00)
	0x33,0x36,0x2E,0x31,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.1*7(00.00)(00-00-00 00:00)
	0x33,0x36,0x2E,0x32,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.2(00.00*kW)(00-00-00 00:00)
	0x33,0x36,0x2E,0x32,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.2*7(00.00)(00-00-00 00:00)
	0x33,0x36,0x2E,0x33,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.3(00.00*kW)(00-00-00 00:00)
	0x33,0x36,0x2E,0x33,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.3*7(00.00)(00-00-00 00:00)
	0x33,0x36,0x2E,0x34,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x6B,0x57,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.4(00.00*kW)(00-00-00 00:00)
	0x33,0x36,0x2E,0x34,0x2A,0x37,0x28,0x30,0x30,0x2E,0x30,0x30,0x29,0x28,0x30,0x30,0x2D,0x30,0x30,0x2D,0x30,0x30,0x20,0x30,0x30,0x3A,0x30,0x30,0x29,0x0D,0x0A,
//36.4*7(00.00)(00-00-00 00:00)
	0x4C,0x2E,0x31,0x28,0x2D,0x2D,0x2D,0x2E,0x2D,0x2A,0x56,0x29,0x0D,0x0A,
//L.1(---.-*V)
	0x4C,0x2E,0x32,0x28,0x2D,0x2D,0x2D,0x2E,0x2D,0x2A,0x56,0x29,0x0D,0x0A,
//L.2(---.-*V)
	0x4C,0x2E,0x33,0x28,0x32,0x32,0x36,0x2E,0x33,0x2A,0x56,0x29,0x0D,0x0A,
//L.3(225.9*V)
	0x41,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x41,0x29,0x0D,0x0A,
//A(00.00*A)
	0x42,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x41,0x29,0x0D,0x0A,
//B(00.00*A)
	0x43,0x28,0x30,0x30,0x2E,0x30,0x30,0x2A,0x41,0x29,0x0D,0x0A,
//C(00.00*A)
	0x37,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x36,0x30,0x32,0x29,0x0D,0x0A,
//71(00000602)
	0x37,0x32,0x2E,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x34,0x38,0x33,0x29,0x0D,0x0A,
//72.1(00000483)
	0x37,0x32,0x2E,0x32,0x28,0x30,0x30,0x30,0x30,0x30,0x31,0x33,0x30,0x29,0x0D,0x0A,
//72.2(00000130)
	0x37,0x32,0x2E,0x33,0x28,0x30,0x30,0x30,0x30,0x30,0x32,0x39,0x35,0x29,0x0D,0x0A,
//72.3(00000295)
	0x31,0x33,0x2E,0x37,0x28,0x20,0x2D,0x2E,0x2D,0x2D,0x29,0x0D,0x0A,
//13.7( -.--)
	0x33,0x31,0x2E,0x31,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//31.1(----*Deg)
	0x33,0x31,0x2E,0x32,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//31.2(----*Deg)
	0x33,0x31,0x2E,0x33,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//31.3(----*Deg)
	0x35,0x31,0x2E,0x31,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//51.1(----*Deg)
	0x35,0x31,0x2E,0x32,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//51.2(----*Deg)
	0x35,0x31,0x2E,0x33,0x28,0x2D,0x2D,0x2D,0x2D,0x2A,0x44,0x65,0x67,0x29,0x0D,0x0A,
//51.3(----*Deg)
	0x43,0x2E,0x32,0x31,0x28,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x33,0x29,0x0D,0x0A,
//C.21(00000003)
//95(05-04-08 11:55)
	0x39,0x35,0x28,0x30,0x35,0x2D,0x30,0x34,0x2D,0x30,0x38,0x20,0x31,0x31,0x3A,0x35,0x35,0x29,0x0D,0x0A,
//1(50)
	0x31,0x28,0x35,0x30,0x29,0x0D,0x0A,
//1*7(18-08-01 09:09)
	0x31,0x2A,0x37,0x28,0x31,0x38,0x2D,0x30,0x38,0x2D,0x30,0x31,0x20,0x30,0x39,0x3A,0x30,0x39,0x29,0x0D,0x0A,
//14(117191)
	0x31,0x34,0x28,0x31,0x31,0x37,0x31,0x39,0x32,0x29,0x0D,0x0A,
//90(00004584)
	0x39,0x30,0x28,0x30,0x30,0x30,0x30,0x34,0x35,0x38,0x34,0x29,0x0D,0x0A,
//91(06764)
	0x39,0x31,0x28,0x30,0x36,0x37,0x36,0x34,0x29,0x0D,0x0A,
//0.2.0(B22)
	0x30,0x2E,0x32,0x2E,0x30,0x28,0x42,0x32,0x32,0x29,0x0D,0x0A,
	
	0x21,0x0D,0x0A,0x03,0x43,
};





u32 RM_Rx_Landis(u32 PORTn)//Landis(IEC1107 模式C)接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
	u32 i;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rx;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 fEXT;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;
	}
	
//---RxTEST---
/*
	switch(UARTCtrl->CONNECT)
	{
		case 1:
			MR((u32)p8rx,(u32)LandisData_1,sizeof(LandisData_1));
			UARTCtrl->RxByte=sizeof(LandisData_1);
			return 1;//1=接收OK
		case 101:
			MR((u32)p8rx,(u32)LandisData_2,sizeof(LandisData_2));
			UARTCtrl->RxByte=sizeof(LandisData_2);
			return 1;//1=接收OK
		default:
			break;
	}
*/
//---RxTEST END---
	
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
		p16timer[0]=500/10;//字节间超时定时
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
	}
	fEXT=0;//收到EXT字节标志
	while(p0!=p1)
	{
		i=p8fifo[p1];
		i&=0x7f;//数据=7位
		switch(UARTCtrl->CONNECT)//连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
		{
			case 101://数据帧
				if(RxByte==0)
				{
					if((i!=1)&&(i!=2))//SOH,STX
					{
						p1++;
						if(p1>=(LEN_UARTnIntRx-10))
						{
							p1=0;
						}
						continue;
					}
				}
				if(RxByte>=LEN_UARTnRx)
				{
					RxByte=0;
				}
				p1++;
				if(p1>=(LEN_UARTnIntRx-10))
				{
					p1=0;
				}
				p8rx[RxByte]=i;
				RxByte++;
				if(fEXT)//收到EXT字节标志
				{
					p16fifo[1]=p1;
					UARTCtrl->RxByte=RxByte;
					return 1;//1=接收OK
				}
				if(i==3)//ETX
				{
					fEXT=1;//收到EXT字节标志
				}
				break;
			default://握手帧
				if(RxByte==0)
				{
					if(i!='/')//'/'
					{
						p1++;
						if(p1>=(LEN_UARTnIntRx-10))
						{
							p1=0;
						}
						continue;
					}
				}
				if(RxByte>=LEN_UARTnRx)
				{
					RxByte=0;
				}
				p1++;
				if(p1>=(LEN_UARTnIntRx-10))
				{
					p1=0;
				}
				p8rx[RxByte]=i;
				RxByte++;
				if(i==0x0a)//LF
				{
					p16fifo[1]=p1;
					UARTCtrl->RxByte=RxByte;
					return 1;//1=接收OK
				}
				break;
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}

u32 RM_RxDATA_Landis_CHECK(u32 PORTn)//Landis(IEC1107 模式C)帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
{

	return 0;
}


//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     512
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)

u32 RM_RxDATA_Landis(u32 PORTn)//Landis(IEC1107 模式C)抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
	u32 i;
	u32 n;
//	u32 offset;
	u32 OI;
	u32 Data;
//	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
//	u32 signedDI;
	u8* p8Array;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);

	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8Data+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//错
		return 3;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data+=LEN_RM_DI_CSD+4+LEN_Data1;
	p8Array=(u8*)p8Data;
	if(UARTCtrl->RMCSDSubCount==0)//抄表CSD子计数(非DL698抄表时)
	{
		p16Data[0]=0;//已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
	}
	LEN_Data0=p16Data[0];
	if(LEN_RxDataBuff<LEN_Data0)
	{//错
		return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data0;
	
	switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
	{
		case 1://300bps握手
			p8=SearchString(p8rx,(u8 *)"/LGZ",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
			if(p8)
			{
				i=p8[0];
				switch(i)
				{
					default:
					case '0'://300Bd
						i=0;//波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
						break;
					case '1'://600Bd
						i=1;
						break;
					case '2'://1200Bd
						i=2;
						break;
					case '3'://2400Bd
						i=3;
						break;
					case '4'://4800Bd
						i=4;
						break;
					case '5'://9600Bd
						i=6;
						break;
				}
				UARTCtrl->BpsCtrl=//通信控制字(同规约2007)(半字对齐)
					(i<<5)|//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
								//D4位表示停止位位数0/1分别表示1/2个停止位
					(1<<3)|//D3位表示有无校验位0/1分别表示无/有
								//D2位表示偶／奇校验位0/1分别表示偶／奇校验
						2;	//D1-D0 数据位数0～3分别表示5-8位
				UARTCtrl->CONNECT=100;//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
			}
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		case 101://101=抄表中
			break;
		case 102://关闭连接
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		default://698规约连接和关闭过程
			UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	
//---后续帧---
	
//---后续帧结束---	
	
	
//---曲线开始---
//用实时数据冻结为曲线
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
		if(OI==0x50020200)
		{//分钟冻结
			p8Data+=LEN_Data0;
			p8Array=(u8*)p8Data;
			n=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
				case 0x00100201://正向有功电能总
				case 0x00200201://反向有功电能总
				case 0x00300201://组合无功1电能总
				case 0x00400201://组合无功2电能总
				case 0x00500201://第1象限无功电能总
				case 0x00600201://第2象限无功电能总
				case 0x00700201://第3象限无功电能总
				case 0x00800201://第4象限无功电能总
				case 0x20000201://A相电压
				case 0x20010201://A相电流
				case 0x20040202://A相有功功率
				case 0x20050202://A相无功功率
				case 0x200A0202://A相功率因数
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			switch(OI)
			{
				case 0x20210200://冻结时标
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//得到date_time_s共7字节
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
				case 0x00100201://正向有功总电能
				case 0x00200201://反向有功总电能
				case 0x00500201://一象限无功总电能
				case 0x00600201://二象限无功总电能
				case 0x00700201://三象限无功总电能
				case 0x00800201://四象限无功总电能
					if(LEN_RxDataBuff<(2+(5*1)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					switch(OI)
					{
						case 0x00100201://正向有功总电能
							p8=SearchString(p8rx,(u8*)"20(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							break;
						case 0x00200201://反向有功总电能
							p8=SearchString(p8rx,(u8*)"22(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							break;
						case 0x00500201://一象限无功总电能
						case 0x00600201://二象限无功总电能
						case 0x00700201://三象限无功总电能
						case 0x00800201://四象限无功总电能
						default:
							p8=0;
							break;
					}
					if(p8)
					{
						Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
					}
					else
					{
						p8Data[0]=0;//空
						p8Data+=1;
						LEN_Data0+=1;
					}
					if(LEN_RxDataBuff<4)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[1]=5;//数组数
					p8Data[0]=0;//费率1
					p8Data[1]=0;//费率2
					p8Data[2]=0;//费率3
					p8Data[3]=0;//费率4
					p8Data+=4;
					LEN_Data0+=4;
					LEN_RxDataBuff-=4;
					break;
				case 0x00300201://组合无功1总电能
				case 0x00400201://组合无功2总电能
					if(LEN_RxDataBuff<(2+(5*1)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					switch(OI)
					{
						case 0x00300201://组合无功1总电能
							p8=SearchString(p8rx,(u8*)"21(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							break;
						case 0x00400201://组合无功2总电能
							p8=SearchString(p8rx,(u8*)"23(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							break;
						default:
							p8=0;
							break;
					}
					if(p8)
					{
						Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
					}
					else
					{
						p8Data[0]=0;//空
						p8Data+=1;
						LEN_Data0+=1;
					}
					if(LEN_RxDataBuff<4)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[1]=5;//数组数
					p8Data[0]=0;//费率1
					p8Data[1]=0;//费率2
					p8Data[2]=0;//费率3
					p8Data[3]=0;//费率4
					p8Data+=4;
					LEN_Data0+=4;
					LEN_RxDataBuff-=4;
					break;
//				case 0x20170200://当前有功需量
//				case 0x20180200://当前无功需量
//				//case 0x11100200://冻结周期内正向有功最大需量 
//					break;
				case 0x20000200://电压
					if(LEN_RxDataBuff<(2+(3*3)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;//数组数
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<3;i++)
					{
						switch(i)
						{
							case 0://A
								p8=SearchString(p8rx,(u8*)"L.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
							case 1://B
								p8=SearchString(p8rx,(u8*)"L.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
							case 2://C
								p8=SearchString(p8rx,(u8*)"L.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
						}
						//DL2007数据格式:DL2007返回数据格式:XXX.X
						//Landis数据格式:L.1(---.-*V)
						if(p8)
						{
							Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
							p8Data[0]=DataType_long_unsigned;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							p8Data+=3;
							LEN_Data0+=3;
							LEN_RxDataBuff-=3;
						}
						else
						{
							p8Data[0]=0;//空
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}						
					}
					break;
				case 0x20010200://电流
					if(LEN_RxDataBuff<(2+(5*3)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;//数组数
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<3;i++)
					{
						switch(i)
						{
							case 0://A
								p8=SearchString(p8rx,(u8*)"A(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
							case 1://B
								p8=SearchString(p8rx,(u8*)"B(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
							case 2://C
								p8=SearchString(p8rx,(u8*)"C(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
						}
						//DL2007数据格式:XXX.XXX
						//Landis数据格式:A(00.00*A)
						if(p8)
						{
							if(p8[0]=='-')
							{//负
								Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
								Data*=10;
								Data=~Data;
								Data+=1;
							}
							else
							{//正
								Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
								Data*=10;
							}
							p8Data[0]=DataType_double_long;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data+=5;
							LEN_Data0+=5;
							LEN_RxDataBuff-=5;
						}
						else
						{
							p8Data[0]=0;//空
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}
					}
					break;
//				case 0x20010400://零线电流
//					//零线电流无数组
//					break;
//				case 0x20040200://有功功率
//				case 0x20050200://无功功率
//					break;
				case 0x200A0200://功率因数
					if(LEN_RxDataBuff<(2+(3*4)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=4;//数组数
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<4;i++)
					{
						switch(i)
						{
							case 0://总
								p8=SearchString(p8rx,(u8*)"13.7(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
								break;
							case 1://A
								p8=0;
								break;
							case 2://B
								p8=0;
								break;
							case 3://C
								p8=0;
								break;
						}
						//数组数;DL2007返回数据格式:XX.XX
						//Dandis数据格式:13.7( -.--)
						if(p8)
						{
							if(p8[0]=='-')
							{//负
								Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
								Data=~Data;
								Data+=1;
							}
							else
							{//正
								Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
							}
							p8Data[0]=DataType_long;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							p8Data+=3;
							LEN_Data0+=3;
							LEN_RxDataBuff-=3;
						}
						else
						{
							p8Data[0]=0;//空
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}
					}					
					break;
//				case 0x20000201://A电压
//					break;
//				case 0x20010201://A电流
//					break;
//				case 0x20040202://A相有功功率
//				case 0x20050202://A相无功功率
//					break;
//				case 0x200A0202://A相功率因数
//					break;
				default:
					if(LEN_RxDataBuff<1)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_Data0+=1;
					LEN_RxDataBuff-=1;
					break;
			}
			p16Data[0]=LEN_Data0;
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
	}
//---曲线结束---
	
	
//---事件类开始---
	
	
//---事件类结束---

	i=UARTCtrl->RMCSD[1];//当前正在抄的CSD(有数据类型)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[3];
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	}
	i=OI_IC(OI);//得到接口类号;入口:对象标识OI
	switch(i)
	{
		case 1://电能量接口类,本接口类对象提供存储电能量类信息
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及费率电能量数组∷=array 电能量
				//电能量：
				//CHOICE
				//{
				//double-long-unsigned   [6]，
				//double-long            [5]
				//}
					if(LEN_RxDataBuff<(5*5))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					switch(OI)
					{
						case 0x0010://正有功
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"20(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"8.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"8.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"8.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"8.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									LEN_Data0+=5;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x0020://反有功
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"22(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"38.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"38.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"38.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"38.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									LEN_Data0+=5;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x0030://组合无功1(正无功)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"21(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"9.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"9.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"9.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"9.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									LEN_Data0+=5;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x0040://组合无功2(反无功)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"23(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"39.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"39.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"39.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"39.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									LEN_Data0+=5;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
//				case 4://高精度总及费率电能量数组∷=array 高精度电能量
//				//高精度电能量：
//				//CHOICE
//				//{
//				//long64-unsigned        [21]，
//				//long64                 [20]
//				//}

//					break;
				default:
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 2://最大需量接口类,本接口类对象提供存储最大需量类信息
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及费率最大需量数组∷=array 最大需量及发生时间
				//最大需量及发生时间∷=structure
				//{
				//  最大需量值 CHOICE，
				//  发生时间   date_time_s
				//}
				//最大需量值 ：
				//CHOICE
				//{
				//double-long            [5]，
				//double-long-unsigned   [6]
				//}
					if(LEN_RxDataBuff<(13*5))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					switch(OI)
					{
						case 0x1010://正有功
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"6.0(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"6.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"6.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"6.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"6.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
						//数组数;DL2007返回数据格式:XX.XXXX YYMMDDhhmm
						//Landis返回数据格式 6(0.083*kW)(03-06-11 10:48)<CR><LF>
								if(p8)
								{
									//值
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									Data*=10;
									p8Data[0]=DataType_structure;
									p8Data[1]=2;
									p8Data+=2;
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									//发生时间
									p8=SearchString(p8,(u8*)"(",17,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//年
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[3]=Data;//月
										Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[4]=Data;//日
										Data=ASCII_BCD((u32)p8+9,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[6]=Data;//分
										p8Data[7]=0;//秒
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//空
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x1020://反有功
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"36.0(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"36.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"36.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"36.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"36.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
						//数组数;DL2007返回数据格式:XX.XXXX YYMMDDhhmm
						//Landis返回数据格式 6(0.083*kW)(03-06-11 10:48)<CR><LF>
								if(p8)
								{
									//值
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									Data*=10;
									p8Data[0]=DataType_structure;
									p8Data[1]=2;
									p8Data+=2;
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									//发生时间
									p8=SearchString(p8,(u8*)"(",17,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//年
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[3]=Data;//月
										Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[4]=Data;//日
										Data=ASCII_BCD((u32)p8+9,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[6]=Data;//分
										p8Data[7]=0;//秒
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//空
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x1030://组合无功1(正无功)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"7.0(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"7.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"7.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"7.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"7.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
						//数组数;DL2007返回数据格式:XX.XXXX YYMMDDhhmm
						//Landis返回数据格式 7.0(00.00*kvar)(00-00-00 00:00)<CR><LF>
								if(p8)
								{
									//值
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									Data*=10;
									p8Data[0]=DataType_structure;
									p8Data[1]=2;
									p8Data+=2;
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									//发生时间
									p8=SearchString(p8,(u8*)"(",17,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//年
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[3]=Data;//月
										Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[4]=Data;//日
										Data=ASCII_BCD((u32)p8+9,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[6]=Data;//分
										p8Data[7]=0;//秒
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//空
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x1040://组合无功2(反无功)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"37.0(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://尖
										p8=SearchString(p8rx,(u8*)"37.4(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://峰
										p8=SearchString(p8rx,(u8*)"37.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 3://平
										p8=SearchString(p8rx,(u8*)"37.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 4://谷
										p8=SearchString(p8rx,(u8*)"37.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
						//数组数;DL2007返回数据格式:XX.XXXX YYMMDDhhmm
						//Landis返回数据格式 7.0(00.00*kvar)(00-00-00 00:00)<CR><LF>
								if(p8)
								{
									//值
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									Data*=10;
									p8Data[0]=DataType_structure;
									p8Data[1]=2;
									p8Data+=2;
									p8Data[0]=DataType_double_long_unsigned;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									//发生时间
									p8=SearchString(p8,(u8*)"(",17,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//年
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[3]=Data;//月
										Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[4]=Data;//日
										Data=ASCII_BCD((u32)p8+9,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
										Data=bcd_hex(Data);
										p8Data[6]=Data;//分
										p8Data[7]=0;//秒
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//空
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//数组数
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				default:
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 3://分相变量接口类,本接口类对象提供存储电压、电流、相角等分相变量类信息
		//分相数值组∷=array 分相数值
		//数值组按A相、B相、C相、N线顺序排列，其中：仅电流有N线，另外，当接线方式为单相时，A、B、C三相改为A相（某一相）。
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://分相数值组
					switch(OI)
					{
						case 0x2000://电压
							if(LEN_RxDataBuff<(3*3))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"L.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"L.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"L.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								//DL2007数据格式:DL2007返回数据格式:XXX.X
								//Landis数据格式:L.1(---.-*V)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x2001://电流
							if(LEN_RxDataBuff<(5*3))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"A(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"B(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"C(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								//DL2007数据格式:XXX.XXX
								//Landis数据格式:A(00.00*A)
								if(p8)
								{
									if(p8[0]=='-')
									{//负
										Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
										Data*=10;
										Data=~Data;
										Data+=1;
									}
									else
									{//正
										Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
										Data*=10;
									}
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									p8Data+=5;
									LEN_Data0+=5;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x2002://电压相角
							if(LEN_RxDataBuff<(3*3))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"31.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"31.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"31.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								//数组数;DL2007返回数据格式:XXX.X
								//Dandis数据格式:31.1(----*Deg)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x2003://电压电流相角
							if(LEN_RxDataBuff<(3*3))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"51.1(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"51.2(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"51.3(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
								}
								//数组数;DL2007返回数据格式:XXX.X
								//Dandis数据格式:51.1(----*Deg)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//数组数
							p16Data[0]=LEN_Data0;
							break;
						case 0x200B://电压波形失真度
						case 0x200C://电流波形失真度
						default:
							return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				case 4://零线电流
				default:
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 4://功率接口类,本接口类对象提供存储功率、功率因数等信息
			//总及分相数值组∷=array 总或分相数值
			//数值组按总、A相、B相、C相顺序排列，当接线方式为单相时，为总、A相（某一相）
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及分相数值组
					switch(OI)
					{
						case 0x2004://有功功率
						case 0x2005://无功功率
							
						case 0x2006://视在功率
						case 0x2007://一分钟平均有功功率
						case 0x2008://一分钟平均无功功率
						case 0x2009://一分钟平均视在功率
							break;
						case 0x200A://功率因数
							if(LEN_RxDataBuff<(3*4))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<4;i++)
							{
								switch(i)
								{
									case 0://总
										p8=SearchString(p8rx,(u8*)"13.7(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
										break;
									case 1://A
										p8=0;
										break;
									case 2://B
										p8=0;
										break;
									case 3://C
										p8=0;
										break;
								}
								//数组数;DL2007返回数据格式:XX.XX
								//Dandis数据格式:13.7( -.--)
								if(p8)
								{
									if(p8[0]=='-')
									{//负
										Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
										Data=~Data;
										Data+=1;
									}
									else
									{//正
										Data=ASCIIdp_HEX(p8+1);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
									}
									p8Data[0]=DataType_long;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//空
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=4;//数组数
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				default:
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
//		case 5://谐波变量接口类,本接口类对象提供存储谐波变量类信息
//			break;
		case 6://数据变量接口类,本接口类对象提供存储过程值或与过程值单元相关的状态值信息
//			switch(OI)
//			{
//				case 0x2014://电表运行状态字1-7
//				case 0x202D://（当前）透支金额  数据类型：double-long-unsigned，单位：元，换算：-2
//				default:
//					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//			}
//			break;
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//		case 7://事件对象接口类,本接口类对象提供配置、存储事件记录类信息
//			break;
		case 8://参数变量接口类,本接口类对象提供存储终端的各种参数类信息
			switch(OI)
			{
				case 0x2021://数据冻结时间
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//得到date_time_s共7字节
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
					break;
				case 0x4000://日期时间
					switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
					{
						case 0://电能表日历时钟Hhmmss
							if(LEN_RxDataBuff<8)
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							//11(15:28:42)
							p8Data[0]=DataType_date_time_s;
							p8Data[1]=0;//年
							p8Data[2]=0;
							p8Data[3]=0;//月
							p8Data[4]=0;//日
							p8Data[5]=0;//时
							p8Data[6]=0;//分
							p8Data[7]=0;//秒
							p8=SearchString(p8rx,(u8*)"11(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							if(p8)
							{
								Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								p8Data[5]=Data;//时
								Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								p8Data[6]=Data;//分
								Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								p8Data[7]=Data;//秒
							}
							LEN_Data0+=8;
							p16Data[0]=LEN_Data0;
							break;
						case 1://电能表日历时钟YYMMDDWW
							if(LEN_Data0<8)
							{
								break;//已填写入buff但还没完整一个CSD的数据长度
							}
							p8Data+=LEN_Data0;
							p8Data-=8;
							p8=SearchString(p8rx,(u8*)"12(",UARTCtrl->RxByte,0);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
							if(p8)
							{
								Data=ASCII_BCD((u32)p8,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								Data+=2000;
								p8Data[1]=Data>>8;//年
								p8Data[2]=Data;
								Data=ASCII_BCD((u32)p8+3,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								p8Data[3]=Data;//月
								Data=ASCII_BCD((u32)p8+6,2);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
								Data=bcd_hex(Data);
								p8Data[4]=Data;//日
							}
							break;
					}
					break;
				case 0x202C://（当前）钱包文件 剩余金额	double-long-unsigned（单位：元，换算：-2）， 购电次数	double-long-unsigned
					break;
			}
			break;
//		case 9://冻结数据接口类,本接口类对象提供配置、存储冻结数据及其相关信息
//			break;
//		case 10://采集监控接口类
//			break;
//		case 11://集合接口类,本接口类对象提供配置、存储终端采集数据及其相关信息
//			break;
//		case 12://脉冲计量接口类
//			break;
//		case 13://负荷控制对象接口类,本接口类对象提供负荷管理有关的控制功能
//			break;
//		case 14://区间统计接口类,本接口类对象提供依据配置参数判定、统计与越限相关的信息
//			break;
//		case 15://累加平均接口类,本接口类对象提供对相同物理属性的数值进行累加、平均的运算功能
//			break;
//		case 16://极值工具接口类,本接口类对象提供采集或生成最大、最小值及其发生时间
//			break;
//		case 17://显示接口类,本接口类对象提供与终端显示或对外打印相关的信息
//			break;
//		case 18://文件传输接口类,本接口类对象提供终端实现上传和下载文件的功能
//			break;
//		case 19://设备管理接口类,
//			break;
//		case 20://应用连接接口类
//			break;
//		case 21://ESAM接口类
//			break;
//		case 22://输入输出设备接口类
//			break;
//		case 23://总加组接口类
//			break;
//		case 24://分项事件对象接口类,本接口类对象提供配置、存储分项事件记录类信息
//			break;
//		case 25://无线公网通信接口类
//			break;
//		case 26://以太网通信接口类
//			break;
		default:
			//测试看未处理的OI值
			i=OI_IC(OI);//得到接口类号;入口:对象标识OI
			if(i)
			{
				i=i;
			}
			return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
}








































