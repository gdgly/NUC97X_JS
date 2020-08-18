
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"

#include "../OS/OS.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/LAN8720A.h"
#include "../Display/Display.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../NUC97x/NUC97x_AIC.h"


u32 Test_IP101GRI(u32 ADDR_BASE)
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	os_Type *os;
	
	__enable_irq();//允许中断
	DisplayStringAutoUp(0,9,(u8*)"\x0""IP101GRI");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	switch(UARTCtrl->LinkTask)
	{
		case 0://0=延时关机
			while(p16timer[0]);
		case 1://1=关电源
			UnInit_NUC97x_MAC();
		case 2://开电源,ETH系统初始化
			//ETH系统初始化
			OnE5V0();//打开E5V0电源
			Init_NUC97x_MAC();
			Init_IP101();
			p16timer[0]=2000/10;//等待电缆RJ45插入2s延时
			while(p16timer[0]);
			os=Get_os();
			os->netInitialize();//每次重启OS后只能调用初始化1次,否则死机
		case 3://等待电缆RJ45插入(每2秒检测)
			while(p16timer[0]);
		default:
			i=Read_ETH_PHYReg(PHYAddr_IP101,2);
			if(i==0x0243)//IP101GRI
			{
				DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
				MC(0,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
				return 0;
			}
			else
			{
				DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
				MC(1,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
				return 1;
			}
	}
}



