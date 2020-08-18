

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






u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest);//没ECC,块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//返回;0=正确,1=错误
{
	return Test_BLOCK_Write_NAND(ADDR_Dest);//没ECC,块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
}


void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
//	__disable_irq();//关中断
	DisplayClr();//清全屏
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	MC(0,ADDR_128KWRITE_BUFF,128*1024);
	sysDisableCache();
	while(1)
	{
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//读引脚,入口引脚使用名;返回0或1
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
		DisplayData_OffZero(i,8,0,pStrBuff);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		LCDDataOut();//点阵数据输出到LCD屏
		
		if(MRR_RTC((u32*)&RTC97x->BKP3R)&0x80000000)
		{
			DisplayString(AutoDisplayRowCol->Row+1,0,0,(u8 *)"End");//显示字符串
			LCDDataOut();//点阵数据输出到LCD屏
			break;//错误
		}
		x=8;
		while(x--)
		{
			i=Life_BLOCK_Write_NAND((NAND_BLOCK_COUNT-1)*128*1024);//返回;0=正确,1=错误
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
//	__enable_irq();//原中断恢复
}


