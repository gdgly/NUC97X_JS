
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/ATM90E36.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"




#if IC_90E36==1
u32 Test_90E36(u32 ADDR_BASE)
{
	u32 x;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""ATM90E36");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	
	TestResult=0;
	Write_90E36((0x30<<16)|0x8765);//д90E36
	x=Read_90E36(0x30|0x8000);
	if(x!=0x8765)
	{
		Write_90E36((0x30<<16)|0x8765);//д90E36
		x=Read_90E36(0x30|0x8000);
		if(x!=0x8765)
		{
			TestResult=1;
		}
	}

	if(TestResult==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	MC(TestResult,ADDR_BASE+OFFSET_90E36_Characteristics,1);
	return TestResult;
}
#endif




