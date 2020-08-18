
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"



#if IC_ATT7022==1//0=û,1=��
u32 Test_ATT7022(u32 ADDR_BASE)
{
	u32 x;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""ATT7022");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	
	TestResult=0;
	Write_ATT7022(0xC9000000);//У������д�ر�
	Write_ATT7022(0xC6000000);//����������ʹ��(��ͨѶ���ݱ��ݼĴ�����Ч)
	x=Read_ATT7022(0);
	if((x!=0x7122A0)&&(x!=0x7126A0))
	{
		x=Read_ATT7022(0);
		if((x!=0x7122A0)&&(x!=0x7126A0))
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


















