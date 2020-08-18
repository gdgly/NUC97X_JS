
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
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
//RTC_BATTERY 3.6V
	DisplayStringAutoUp(0,9,(u8*)"\x0""RTC3V6");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	RTC_Battery_Voltage();//RTC��ص�ѹADC����
	i=Comm_Ram->URTC;
	if((i<0x360)||(i>0x380))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	
	
	return TestResult;
}












