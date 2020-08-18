
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"




u32 Test_POWER(u32 ADDR_BASE)
{
	u32 i;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	
//RTC_BATTERY 3.6V
	DisplayStringAutoUp(0,9,(u8*)"\x0""RTC3V6");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	RTC_Battery_Voltage();//RTC电池电压ADC测量
	i=Comm_Ram->URTC;
	if((i<0x360)||(i>0x380))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	
	
	return TestResult;
}












