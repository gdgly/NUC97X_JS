
//硬件和软件模块错误显示
#include "../Hcsg/ProjectCSG.h"

#include "../Display/DisplayError.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"


const u8 TEXT_ERRORIC0[]={"IC_STM32F4xx RAM"};
const u8 TEXT_ERRORIC1[]={"IC_MT48LCxxxx"};
#if IC_MT29FxG==1
const u8 TEXT_ERRORIC2[]={"IC_MT29FxG"};
#endif
#if IC_S34MLxG==1
const u8 TEXT_ERRORIC2[]={"IC_S34MLxG"};
#endif
const u8 TEXT_ERRORIC3[]={"IC_S29GLxxx"};
#if IC_RX8025T==1//0=没,1=有
const u8 TEXT_ERRORIC4[]={"IC_RX8025T"};
#endif
#if IC_DS3231==1//0=没,1=有
const u8 TEXT_ERRORIC4[]={"IC_DS3231"};
#endif
#if (IC_RX8025T|IC_DS3231)==0
const u8 TEXT_ERRORIC4[]={""};
#endif
#if IC_ATT7022E==1//0=没,1=有
const u8 TEXT_ERRORIC5[]={"IC_ATT7022E"};
#endif
#if IC_90E36==1//0=没,1=有
const u8 TEXT_ERRORIC5[]={"IC_90E36"};
#else
const u8 TEXT_ERRORIC5[]={""};
#endif
const u8 TEXT_ERRORIC6[]={"IC_PCF8562"};
const u8 TEXT_ERRORIC7[]={"IC_LAN8720"};
const u8 TEXT_ERRORIC8[]={"IC_MESAM"};
const u8 TEXT_ERRORIC9[]={"IC_TESAM"};
const u8 TEXT_ERRORIC10[]={"IC_24AAxxx"};
const u8 TEXT_ERRORIC11[]={"IC_xxxx"};
const u8 TEXT_ERRORIC12[]={"IC_xxxx"};
const u8 TEXT_ERRORIC13[]={"STM32F4xx ADC"};
const u8 TEXT_ERRORIC14[]={"IC_16MHz"};
const u8 TEXT_ERRORIC15[]={"IC_32768Hz"};
const u8 TEXT_ERRORIC16[]={"DATA_METER"};
const u8 TEXT_ERRORIC17[]={"DATA_TERMINAL"};
const u8 TEXT_ERRORIC18[]={"抄表Fn缓冲不够"};
const u8 TEXT_ERRORIC19[]={"堆栈长度不够"};
const u8 TEXT_ERRORIC20[]={"UART0"};
const u8 TEXT_ERRORIC21[]={"UART1"};
const u8 TEXT_ERRORIC22[]={"UART2"};
const u8 TEXT_ERRORIC23[]={"UART3"};
const u8 TEXT_ERRORIC24[]={"UART4"};
const u8 TEXT_ERRORIC25[]={"UART5"};
const u8 TEXT_ERRORIC26[]={"UART6"};
const u8 TEXT_ERRORIC27[]={"STM32F4xx PLL0"};
const u8 TEXT_ERRORIC28[]={"STM32F4xx PLL1"};
const u8 TEXT_ERRORIC29[]={"P_BATTERY"};
const u8 TEXT_ERRORIC30[]={"P_GPRS"};
const u8 TEXT_ERRORIC31[]={"LPC11xx"};


const u32 pTEXT_ERRORIC[]=
{
	(u32)TEXT_ERRORIC0,
	(u32)TEXT_ERRORIC1,
	(u32)TEXT_ERRORIC2,
	(u32)TEXT_ERRORIC3,
	(u32)TEXT_ERRORIC4,
	(u32)TEXT_ERRORIC5,
	(u32)TEXT_ERRORIC6,
	(u32)TEXT_ERRORIC7,
	(u32)TEXT_ERRORIC8,
	(u32)TEXT_ERRORIC9,
	(u32)TEXT_ERRORIC10,
	(u32)TEXT_ERRORIC11,
	(u32)TEXT_ERRORIC12,
	(u32)TEXT_ERRORIC13,
	(u32)TEXT_ERRORIC14,
	(u32)TEXT_ERRORIC15,
	(u32)TEXT_ERRORIC16,
	(u32)TEXT_ERRORIC17,
	(u32)TEXT_ERRORIC18,
	(u32)TEXT_ERRORIC19,
	(u32)TEXT_ERRORIC20,
	(u32)TEXT_ERRORIC21,
	(u32)TEXT_ERRORIC22,
	(u32)TEXT_ERRORIC23,
	(u32)TEXT_ERRORIC24,
	(u32)TEXT_ERRORIC25,
	(u32)TEXT_ERRORIC26,
	(u32)TEXT_ERRORIC27,
	(u32)TEXT_ERRORIC28,
	(u32)TEXT_ERRORIC29,
	(u32)TEXT_ERRORIC30,
	(u32)TEXT_ERRORIC31,
};


void DisplayError(void)//硬件和软件模块错误显示
{
	u32 i;
	u32 Row;
	u32 Err;
//	u16 *p16;

  if(Comm_Ram->DisplayScanTimer!=0x0)
  {
    return;
  }
	DisplayClr();//清屏
	DisplayString(0,7,0,(u8 *)"ERROR");//显示字符串
	
	Row=1;
//硬件错误
	Err=Comm_Ram->ErrorIC;
	for(i=0;i<32;i++)
	{
		if(Err&0x1)
		{
			DisplayString(Row,0,0,(u8*)pTEXT_ERRORIC[i]);//显示字符串
			Row++;
		}
		Err>>=1;
	}


}






