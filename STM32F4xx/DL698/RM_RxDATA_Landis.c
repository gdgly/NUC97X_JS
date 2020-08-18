
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





u32 RM_Rx_Landis(u32 PORTn)//Landis(IEC1107 ģʽC)����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
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

	DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
	p0=p16fifo[0];//�жϽ���ָ��
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//�жϽ���ָ��
		p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		p16fifo[2]=0;//��Լ���ָ��
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
			return 1;//1=����OK
		case 101:
			MR((u32)p8rx,(u32)LandisData_2,sizeof(LandisData_2));
			UARTCtrl->RxByte=sizeof(LandisData_2);
			return 1;//1=����OK
		default:
			break;
	}
*/
//---RxTEST END---
	
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//�����ݽ������ó�ʱ��ʱ
		p16timer[0]=500/10;//�ֽڼ䳬ʱ��ʱ
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
	}
	fEXT=0;//�յ�EXT�ֽڱ�־
	while(p0!=p1)
	{
		i=p8fifo[p1];
		i&=0x7f;//����=7λ
		switch(UARTCtrl->CONNECT)//����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
		{
			case 101://����֡
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
				if(fEXT)//�յ�EXT�ֽڱ�־
				{
					p16fifo[1]=p1;
					UARTCtrl->RxByte=RxByte;
					return 1;//1=����OK
				}
				if(i==3)//ETX
				{
					fEXT=1;//�յ�EXT�ֽڱ�־
				}
				break;
			default://����֡
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
					return 1;//1=����OK
				}
				break;
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		return 2;//2=���ճ�ʱ
	}
	return 0;
}

u32 RM_RxDATA_Landis_CHECK(u32 PORTn)//Landis(IEC1107 ģʽC)֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
{

	return 0;
}


//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)

u32 RM_RxDATA_Landis(u32 PORTn)//Landis(IEC1107 ģʽC)����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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

	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8Data+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//��
		return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data+=LEN_RM_DI_CSD+4+LEN_Data1;
	p8Array=(u8*)p8Data;
	if(UARTCtrl->RMCSDSubCount==0)//����CSD�Ӽ���(��DL698����ʱ)
	{
		p16Data[0]=0;//����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
	}
	LEN_Data0=p16Data[0];
	if(LEN_RxDataBuff<LEN_Data0)
	{//��
		return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data0;
	
	switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
	{
		case 1://300bps����
			p8=SearchString(p8rx,(u8 *)"/LGZ",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(p8)
			{
				i=p8[0];
				switch(i)
				{
					default:
					case '0'://300Bd
						i=0;//������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
				UARTCtrl->BpsCtrl=//ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
					(i<<5)|//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
								//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
					(1<<3)|//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
								//D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
						2;	//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
				UARTCtrl->CONNECT=100;//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
			}
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		case 101://101=������
			break;
		case 102://�ر�����
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		default://698��Լ���Ӻ͹رչ���
			UARTCtrl->CONNECT=0;//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	
//---����֡---
	
//---����֡����---	
	
	
//---���߿�ʼ---
//��ʵʱ���ݶ���Ϊ����
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
		if(OI==0x50020200)
		{//���Ӷ���
			p8Data+=LEN_Data0;
			p8Array=(u8*)p8Data;
			n=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
				case 0x00100201://�����й�������
				case 0x00200201://�����й�������
				case 0x00300201://����޹�1������
				case 0x00400201://����޹�2������
				case 0x00500201://��1�����޹�������
				case 0x00600201://��2�����޹�������
				case 0x00700201://��3�����޹�������
				case 0x00800201://��4�����޹�������
				case 0x20000201://A���ѹ
				case 0x20010201://A�����
				case 0x20040202://A���й�����
				case 0x20050202://A���޹�����
				case 0x200A0202://A�๦������
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			switch(OI)
			{
				case 0x20210200://����ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//�õ�date_time_s��7�ֽ�
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
				case 0x00100201://�����й��ܵ���
				case 0x00200201://�����й��ܵ���
				case 0x00500201://һ�����޹��ܵ���
				case 0x00600201://�������޹��ܵ���
				case 0x00700201://�������޹��ܵ���
				case 0x00800201://�������޹��ܵ���
					if(LEN_RxDataBuff<(2+(5*1)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					switch(OI)
					{
						case 0x00100201://�����й��ܵ���
							p8=SearchString(p8rx,(u8*)"20(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							break;
						case 0x00200201://�����й��ܵ���
							p8=SearchString(p8rx,(u8*)"22(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							break;
						case 0x00500201://һ�����޹��ܵ���
						case 0x00600201://�������޹��ܵ���
						case 0x00700201://�������޹��ܵ���
						case 0x00800201://�������޹��ܵ���
						default:
							p8=0;
							break;
					}
					if(p8)
					{
						Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
						p8Data[0]=0;//��
						p8Data+=1;
						LEN_Data0+=1;
					}
					if(LEN_RxDataBuff<4)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[1]=5;//������
					p8Data[0]=0;//����1
					p8Data[1]=0;//����2
					p8Data[2]=0;//����3
					p8Data[3]=0;//����4
					p8Data+=4;
					LEN_Data0+=4;
					LEN_RxDataBuff-=4;
					break;
				case 0x00300201://����޹�1�ܵ���
				case 0x00400201://����޹�2�ܵ���
					if(LEN_RxDataBuff<(2+(5*1)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					switch(OI)
					{
						case 0x00300201://����޹�1�ܵ���
							p8=SearchString(p8rx,(u8*)"21(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							break;
						case 0x00400201://����޹�2�ܵ���
							p8=SearchString(p8rx,(u8*)"23(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							break;
						default:
							p8=0;
							break;
					}
					if(p8)
					{
						Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
						p8Data[0]=0;//��
						p8Data+=1;
						LEN_Data0+=1;
					}
					if(LEN_RxDataBuff<4)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[1]=5;//������
					p8Data[0]=0;//����1
					p8Data[1]=0;//����2
					p8Data[2]=0;//����3
					p8Data[3]=0;//����4
					p8Data+=4;
					LEN_Data0+=4;
					LEN_RxDataBuff-=4;
					break;
//				case 0x20170200://��ǰ�й�����
//				case 0x20180200://��ǰ�޹�����
//				//case 0x11100200://���������������й�������� 
//					break;
				case 0x20000200://��ѹ
					if(LEN_RxDataBuff<(2+(3*3)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;//������
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<3;i++)
					{
						switch(i)
						{
							case 0://A
								p8=SearchString(p8rx,(u8*)"L.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
							case 1://B
								p8=SearchString(p8rx,(u8*)"L.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
							case 2://C
								p8=SearchString(p8rx,(u8*)"L.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
						}
						//DL2007���ݸ�ʽ:DL2007�������ݸ�ʽ:XXX.X
						//Landis���ݸ�ʽ:L.1(---.-*V)
						if(p8)
						{
							Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
							p8Data[0]=DataType_long_unsigned;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							p8Data+=3;
							LEN_Data0+=3;
							LEN_RxDataBuff-=3;
						}
						else
						{
							p8Data[0]=0;//��
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}						
					}
					break;
				case 0x20010200://����
					if(LEN_RxDataBuff<(2+(5*3)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;//������
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<3;i++)
					{
						switch(i)
						{
							case 0://A
								p8=SearchString(p8rx,(u8*)"A(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
							case 1://B
								p8=SearchString(p8rx,(u8*)"B(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
							case 2://C
								p8=SearchString(p8rx,(u8*)"C(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
								break;
						}
						//DL2007���ݸ�ʽ:XXX.XXX
						//Landis���ݸ�ʽ:A(00.00*A)
						if(p8)
						{
							if(p8[0]=='-')
							{//��
								Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
								Data*=10;
								Data=~Data;
								Data+=1;
							}
							else
							{//��
								Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
							p8Data[0]=0;//��
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}
					}
					break;
//				case 0x20010400://���ߵ���
//					//���ߵ���������
//					break;
//				case 0x20040200://�й�����
//				case 0x20050200://�޹�����
//					break;
				case 0x200A0200://��������
					if(LEN_RxDataBuff<(2+(3*4)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=4;//������
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<4;i++)
					{
						switch(i)
						{
							case 0://��
								p8=SearchString(p8rx,(u8*)"13.7(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
						//������;DL2007�������ݸ�ʽ:XX.XX
						//Dandis���ݸ�ʽ:13.7( -.--)
						if(p8)
						{
							if(p8[0]=='-')
							{//��
								Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
								Data=~Data;
								Data+=1;
							}
							else
							{//��
								Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
							p8Data[0]=0;//��
							p8Data+=1;
							LEN_Data0+=1;
							LEN_RxDataBuff-=1;
						}
					}					
					break;
//				case 0x20000201://A��ѹ
//					break;
//				case 0x20010201://A����
//					break;
//				case 0x20040202://A���й�����
//				case 0x20050202://A���޹�����
//					break;
//				case 0x200A0202://A�๦������
//					break;
				default:
					if(LEN_RxDataBuff<1)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_Data0+=1;
					LEN_RxDataBuff-=1;
					break;
			}
			p16Data[0]=LEN_Data0;
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
	}
//---���߽���---
	
	
//---�¼��࿪ʼ---
	
	
//---�¼������---

	i=UARTCtrl->RMCSD[1];//��ǰ���ڳ���CSD(����������)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[3];
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	}
	i=OI_IC(OI);//�õ��ӿ����;���:�����ʶOI
	switch(i)
	{
		case 1://�������ӿ���,���ӿ�������ṩ�洢����������Ϣ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ����ʵ����������=array ������
				//��������
				//CHOICE
				//{
				//double-long-unsigned   [6]��
				//double-long            [5]
				//}
					if(LEN_RxDataBuff<(5*5))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					switch(OI)
					{
						case 0x0010://���й�
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"20(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"8.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"8.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"8.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"8.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x0020://���й�
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"22(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"38.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"38.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"38.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"38.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x0030://����޹�1(���޹�)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"21(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"9.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"9.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"9.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"9.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x0040://����޹�2(���޹�)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"23(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"39.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"39.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"39.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"39.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
//				case 4://�߾����ܼ����ʵ����������=array �߾��ȵ�����
//				//�߾��ȵ�������
//				//CHOICE
//				//{
//				//long64-unsigned        [21]��
//				//long64                 [20]
//				//}

//					break;
				default:
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 2://��������ӿ���,���ӿ�������ṩ�洢�����������Ϣ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ�����������������=array �������������ʱ��
				//�������������ʱ���=structure
				//{
				//  �������ֵ CHOICE��
				//  ����ʱ��   date_time_s
				//}
				//�������ֵ ��
				//CHOICE
				//{
				//double-long            [5]��
				//double-long-unsigned   [6]
				//}
					if(LEN_RxDataBuff<(13*5))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					switch(OI)
					{
						case 0x1010://���й�
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"6.0(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"6.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"6.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"6.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"6.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
						//������;DL2007�������ݸ�ʽ:XX.XXXX YYMMDDhhmm
						//Landis�������ݸ�ʽ 6(0.083*kW)(03-06-11 10:48)<CR><LF>
								if(p8)
								{
									//ֵ
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									//����ʱ��
									p8=SearchString(p8,(u8*)"(",17,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//��
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[3]=Data;//��
										Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[4]=Data;//��
										Data=ASCII_BCD((u32)p8+9,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[6]=Data;//��
										p8Data[7]=0;//��
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//��
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x1020://���й�
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"36.0(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"36.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"36.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"36.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"36.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
						//������;DL2007�������ݸ�ʽ:XX.XXXX YYMMDDhhmm
						//Landis�������ݸ�ʽ 6(0.083*kW)(03-06-11 10:48)<CR><LF>
								if(p8)
								{
									//ֵ
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									//����ʱ��
									p8=SearchString(p8,(u8*)"(",17,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//��
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[3]=Data;//��
										Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[4]=Data;//��
										Data=ASCII_BCD((u32)p8+9,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[6]=Data;//��
										p8Data[7]=0;//��
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//��
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x1030://����޹�1(���޹�)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"7.0(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"7.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"7.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"7.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"7.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
						//������;DL2007�������ݸ�ʽ:XX.XXXX YYMMDDhhmm
						//Landis�������ݸ�ʽ 7.0(00.00*kvar)(00-00-00 00:00)<CR><LF>
								if(p8)
								{
									//ֵ
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									//����ʱ��
									p8=SearchString(p8,(u8*)"(",17,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//��
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[3]=Data;//��
										Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[4]=Data;//��
										Data=ASCII_BCD((u32)p8+9,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[6]=Data;//��
										p8Data[7]=0;//��
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//��
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x1040://����޹�2(���޹�)
							for(i=0;i<5;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"37.0(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://��
										p8=SearchString(p8rx,(u8*)"37.4(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://��
										p8=SearchString(p8rx,(u8*)"37.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 3://ƽ
										p8=SearchString(p8rx,(u8*)"37.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 4://��
										p8=SearchString(p8rx,(u8*)"37.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
						//������;DL2007�������ݸ�ʽ:XX.XXXX YYMMDDhhmm
						//Landis�������ݸ�ʽ 7.0(00.00*kvar)(00-00-00 00:00)<CR><LF>
								if(p8)
								{
									//ֵ
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									//����ʱ��
									p8=SearchString(p8,(u8*)"(",17,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
									if(p8)
									{
										p8Data[0]=DataType_date_time_s;
										Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data+=2000;
										p8Data[1]=Data>>8;//��
										p8Data[2]=Data;
										Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[3]=Data;//��
										Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[4]=Data;//��
										Data=ASCII_BCD((u32)p8+9,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										Data=ASCII_BCD((u32)p8+12,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
										Data=bcd_hex(Data);
										p8Data[6]=Data;//��
										p8Data[7]=0;//��
										p8rx+=5;
										p8Data+=8;
										LEN_Data0+=2+5+8;
									}
									else
									{
										p8Data[0]=0;//��
										p8Data+=1;
										LEN_Data0+=1;
									}
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=5;//������
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				default:
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 3://��������ӿ���,���ӿ�������ṩ�洢��ѹ����������ǵȷ����������Ϣ
		//������ֵ���=array ������ֵ
		//��ֵ�鰴A�ࡢB�ࡢC�ࡢN��˳�����У����У���������N�ߣ����⣬�����߷�ʽΪ����ʱ��A��B��C�����ΪA�ࣨĳһ�ࣩ��
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://������ֵ��
					switch(OI)
					{
						case 0x2000://��ѹ
							if(LEN_RxDataBuff<(3*3))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"L.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"L.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"L.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								//DL2007���ݸ�ʽ:DL2007�������ݸ�ʽ:XXX.X
								//Landis���ݸ�ʽ:L.1(---.-*V)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x2001://����
							if(LEN_RxDataBuff<(5*3))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"A(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"B(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"C(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								//DL2007���ݸ�ʽ:XXX.XXX
								//Landis���ݸ�ʽ:A(00.00*A)
								if(p8)
								{
									if(p8[0]=='-')
									{//��
										Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
										Data*=10;
										Data=~Data;
										Data+=1;
									}
									else
									{//��
										Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
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
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x2002://��ѹ���
							if(LEN_RxDataBuff<(3*3))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"31.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"31.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"31.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								//������;DL2007�������ݸ�ʽ:XXX.X
								//Dandis���ݸ�ʽ:31.1(----*Deg)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x2003://��ѹ�������
							if(LEN_RxDataBuff<(3*3))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<3;i++)
							{
								switch(i)
								{
									case 0://A
										p8=SearchString(p8rx,(u8*)"51.1(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 1://B
										p8=SearchString(p8rx,(u8*)"51.2(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
									case 2://C
										p8=SearchString(p8rx,(u8*)"51.3(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
										break;
								}
								//������;DL2007�������ݸ�ʽ:XXX.X
								//Dandis���ݸ�ʽ:51.1(----*Deg)
								if(p8)
								{
									Data=ASCIIdp_HEX(p8);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=3;//������
							p16Data[0]=LEN_Data0;
							break;
						case 0x200B://��ѹ����ʧ���
						case 0x200C://��������ʧ���
						default:
							return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				case 4://���ߵ���
				default:
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 4://���ʽӿ���,���ӿ�������ṩ�洢���ʡ�������������Ϣ
			//�ܼ�������ֵ���=array �ܻ������ֵ
			//��ֵ�鰴�ܡ�A�ࡢB�ࡢC��˳�����У������߷�ʽΪ����ʱ��Ϊ�ܡ�A�ࣨĳһ�ࣩ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ�������ֵ��
					switch(OI)
					{
						case 0x2004://�й�����
						case 0x2005://�޹�����
							
						case 0x2006://���ڹ���
						case 0x2007://һ����ƽ���й�����
						case 0x2008://һ����ƽ���޹�����
						case 0x2009://һ����ƽ�����ڹ���
							break;
						case 0x200A://��������
							if(LEN_RxDataBuff<(3*4))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<4;i++)
							{
								switch(i)
								{
									case 0://��
										p8=SearchString(p8rx,(u8*)"13.7(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
								//������;DL2007�������ݸ�ʽ:XX.XX
								//Dandis���ݸ�ʽ:13.7( -.--)
								if(p8)
								{
									if(p8[0]=='-')
									{//��
										Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
										Data=~Data;
										Data+=1;
									}
									else
									{//��
										Data=ASCIIdp_HEX(p8+1);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
									}
									p8Data[0]=DataType_long;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									p8Data+=3;
									LEN_Data0+=3;
								}
								else
								{
									p8Data[0]=0;//��
									p8Data+=1;
									LEN_Data0+=1;
								}
							}
							p8Array[1]=4;//������
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				default:
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
//		case 5://г�������ӿ���,���ӿ�������ṩ�洢г����������Ϣ
//			break;
		case 6://���ݱ����ӿ���,���ӿ�������ṩ�洢����ֵ�������ֵ��Ԫ��ص�״ֵ̬��Ϣ
//			switch(OI)
//			{
//				case 0x2014://�������״̬��1-7
//				case 0x202D://����ǰ��͸֧���  �������ͣ�double-long-unsigned����λ��Ԫ�����㣺-2
//				default:
//					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//			}
//			break;
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//		case 7://�¼�����ӿ���,���ӿ�������ṩ���á��洢�¼���¼����Ϣ
//			break;
		case 8://���������ӿ���,���ӿ�������ṩ�洢�ն˵ĸ��ֲ�������Ϣ
			switch(OI)
			{
				case 0x2021://���ݶ���ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//�õ�date_time_s��7�ֽ�
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
					break;
				case 0x4000://����ʱ��
					switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
					{
						case 0://���ܱ�����ʱ��Hhmmss
							if(LEN_RxDataBuff<8)
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							//11(15:28:42)
							p8Data[0]=DataType_date_time_s;
							p8Data[1]=0;//��
							p8Data[2]=0;
							p8Data[3]=0;//��
							p8Data[4]=0;//��
							p8Data[5]=0;//ʱ
							p8Data[6]=0;//��
							p8Data[7]=0;//��
							p8=SearchString(p8rx,(u8*)"11(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(p8)
							{
								Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								p8Data[5]=Data;//ʱ
								Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								p8Data[6]=Data;//��
								Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								p8Data[7]=Data;//��
							}
							LEN_Data0+=8;
							p16Data[0]=LEN_Data0;
							break;
						case 1://���ܱ�����ʱ��YYMMDDWW
							if(LEN_Data0<8)
							{
								break;//����д��buff����û����һ��CSD�����ݳ���
							}
							p8Data+=LEN_Data0;
							p8Data-=8;
							p8=SearchString(p8rx,(u8*)"12(",UARTCtrl->RxByte,0);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(p8)
							{
								Data=ASCII_BCD((u32)p8,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								Data+=2000;
								p8Data[1]=Data>>8;//��
								p8Data[2]=Data;
								Data=ASCII_BCD((u32)p8+3,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								p8Data[3]=Data;//��
								Data=ASCII_BCD((u32)p8+6,2);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
								Data=bcd_hex(Data);
								p8Data[4]=Data;//��
							}
							break;
					}
					break;
				case 0x202C://����ǰ��Ǯ���ļ� ʣ����	double-long-unsigned����λ��Ԫ�����㣺-2���� �������	double-long-unsigned
					break;
			}
			break;
//		case 9://�������ݽӿ���,���ӿ�������ṩ���á��洢�������ݼ��������Ϣ
//			break;
//		case 10://�ɼ���ؽӿ���
//			break;
//		case 11://���Ͻӿ���,���ӿ�������ṩ���á��洢�ն˲ɼ����ݼ��������Ϣ
//			break;
//		case 12://��������ӿ���
//			break;
//		case 13://���ɿ��ƶ���ӿ���,���ӿ�������ṩ���ɹ����йصĿ��ƹ���
//			break;
//		case 14://����ͳ�ƽӿ���,���ӿ�������ṩ�������ò����ж���ͳ����Խ����ص���Ϣ
//			break;
//		case 15://�ۼ�ƽ���ӿ���,���ӿ�������ṩ����ͬ�������Ե���ֵ�����ۼӡ�ƽ�������㹦��
//			break;
//		case 16://��ֵ���߽ӿ���,���ӿ�������ṩ�ɼ������������Сֵ���䷢��ʱ��
//			break;
//		case 17://��ʾ�ӿ���,���ӿ�������ṩ���ն���ʾ������ӡ��ص���Ϣ
//			break;
//		case 18://�ļ�����ӿ���,���ӿ�������ṩ�ն�ʵ���ϴ��������ļ��Ĺ���
//			break;
//		case 19://�豸����ӿ���,
//			break;
//		case 20://Ӧ�����ӽӿ���
//			break;
//		case 21://ESAM�ӿ���
//			break;
//		case 22://��������豸�ӿ���
//			break;
//		case 23://�ܼ���ӿ���
//			break;
//		case 24://�����¼�����ӿ���,���ӿ�������ṩ���á��洢�����¼���¼����Ϣ
//			break;
//		case 25://���߹���ͨ�Žӿ���
//			break;
//		case 26://��̫��ͨ�Žӿ���
//			break;
		default:
			//���Կ�δ�����OIֵ
			i=OI_IC(OI);//�õ��ӿ����;���:�����ʶOI
			if(i)
			{
				i=i;
			}
			return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
}








































