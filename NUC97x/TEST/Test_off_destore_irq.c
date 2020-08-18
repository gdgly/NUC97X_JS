
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../Calculate/Calculate.h"
#include "../NUC97x/NUC97x_AIC.h"



void Test_off_destore_irq(void)//���Դ򿪺ͻָ�IRQ�ж�
{
	u32 i;
	u32 Count;
	u16* p16;
	u32 irq;
	
	
	Count=0;
	DisplayClr();//����
	DisplayStringAutoUp(0,9,(u8*)"\x0""IRQ TEST");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//������,�������ʹ����;����0��1
		{
			return;
		}
		#endif
		//��ȫ���ж�
		irq=off_irq();//��IRQ�ж�,����:�ر�ǰ����״̬
		Comm_Ram->GPMSTimer=1000/10;
		for(i=0;i<100;i++)
		{
			SoftDelayMS(10);//���ms��ʱ
			if(Comm_Ram->GPMSTimer!=(1000/10))
			{
				DisplayStringAutoUp(0,9,(u8*)"off_irq      ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			}
		}
		//�ָ�ȫ���ж�
		destore_irq(irq);//�ָ�IRQ�ж�,���:�ر�ǰ����״̬
		while(Comm_Ram->GPMSTimer)
		{
		}
		Count++;
		p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			p16[i]=0;
		}
		i=hex_bcd(Count);
		DisplayData_OffZero(i,8,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		LCDDataOut();//�������������LCD��
	}
}






