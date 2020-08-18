

#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../TEST/Test_FLASH_Life.h"
#include "../Display/Display.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Device/MEMRW.h"
#include "../Device/NAND.h"






u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest);//ûECC,��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//����;0=��ȷ,1=����
{
	return Test_BLOCK_Write_NAND(ADDR_Dest);//ûECC,��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
}


void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
//	__disable_irq();//���ж�
	DisplayClr();//��ȫ��
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	MC(0,ADDR_128KWRITE_BUFF,128*1024);
	sysDisableCache();
	while(1)
	{
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//������,�������ʹ����;����0��1
		{
			return;
		}
		#endif
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=MRR_RTC((u32*)&RTC97x->BKP3R);
		i=hex_bcd(i&0x7fffffff);
		DisplayData_OffZero(i,8,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		LCDDataOut();//�������������LCD��
		
		if(MRR_RTC((u32*)&RTC97x->BKP3R)&0x80000000)
		{
			DisplayString(AutoDisplayRowCol->Row+1,0,0,(u8 *)"End");//��ʾ�ַ���
			LCDDataOut();//�������������LCD��
			break;//����
		}
		x=8;
		while(x--)
		{
			i=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//����;0=��ȷ,1=����
			if(i==0)
			{
				i=MRR_RTC((u32*)&RTC97x->BKP3R);
				i++;
				MWR_RTC(i,(u32*)&RTC97x->BKP3R);
			}
			else
			{
				i=MRR_RTC((u32*)&RTC97x->BKP3R);
				i|=0x80000000;
				MWR_RTC(i,(u32*)&RTC97x->BKP3R);
				break;
			}
		}
	}
//	__enable_irq();//ԭ�жϻָ�
}


