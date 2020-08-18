
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "Test_Uart_3762_RxTx.h"
#include "Test_ReadMeter_3762.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Display/Display.h"
#include "../NUC97x/NUC97x_AIC.h"


#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
u32 Test_LOCAL_MODULE(u32 ADDR_BASE)//�����ز�ģ��
{
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""MODULE");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	__enable_irq();//�����ж�
	Comm_Ram->GPMSTimer=60000/10;
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�������Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		Uart_3762_RxTx(RS485_4PORT);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
		Terminal_ReadMeter_3762(RS485_4PORT);//�ն˳���Q/GDW376.2
		if(Terminal_Router->RouterInitTask>=10)
		{
			MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
			DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
			return 0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			MWR(0x010101,ADDR_BASE+(OFFSET_UART_Characteristics+5),3);
			DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
			return 1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		}
	}
}
#endif


