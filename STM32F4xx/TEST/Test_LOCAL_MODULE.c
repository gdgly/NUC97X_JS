

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "Test_Uart_3762_RxTx.h"
#include "Test_ReadMeter_3762.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Display/Display.h"



#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
u32 Test_LOCAL_MODULE(u32 ADDR_BASE)//本地载波模块
{
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""MODULE");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	__enable_irq();//允许中断
	Comm_Ram->GPMSTimer=60000/10;
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		Uart_3762_RxTx(RS485_4PORT);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
		Terminal_ReadMeter_3762(RS485_4PORT);//终端抄表Q/GDW376.2
		if(Terminal_Router->RouterInitTask>=10)
		{
			MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
			DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
			return 0;//总测试结果0=合格,1=不合格,0xff=无结论
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			MWR(0x010101,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
			DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
			return 1;//总测试结果0=合格,1=不合格,0xff=无结论
		}
	}
}
#endif



