

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
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



#if IC_MT29FxG==1
#if NAND_BUS_WIDTH==8//8=8bit,16=16bit
//bus=8bit
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
{
	u32 i;
	u32 Page;
	u8 *p8;
	u32 ReNum;
	
	p8=(u8*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
//块写
	ReNum=1;//写入错误重复次数
	while(ReNum--)
	{
		//块擦除
		p8[0x10000]=0x60;//Command
		p8[0x20000]=(ADDR_Dest>>11);//Row Add. 1
		p8[0x20000]=(ADDR_Dest>>19);//Row Add. 2
	#if (NAND_2G|NAND_4G|NAND_8G)
		p8[0x20000]=(ADDR_Dest>>27);//Row Add. 3
	#endif
		p8[0x10000]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
		//等待块擦除完成
		Wait_NAND(3000*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		//块写回(每分页2048字节)
		for(Page=0;Page<64;Page++)
		{
			while(1)//掉电中断冲突重写
			{
				i=ADDR_Dest+(Page*2048);
				p8[0x10000]=0x80;//Command
				p8[0x20000]=i;//Col.Add.1
				p8[0x20000]=(i>>8)&0x7;//Col.Add.2
				p8[0x20000]=(i>>11);//Row Add. 1
				p8[0x20000]=(i>>19);//Row Add. 2
			#if (NAND_2G|NAND_4G|NAND_8G)
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				for(i=0;i<2048;i++)
				{
					p8[0]=i;
				}
//				if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
//				{
//					Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
//					p8WBUFF-=8*256;
//					continue;
//				}
				p8[0x10000]=0x10;//Command
				break;
			}
			Wait_NAND(600*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		}
	//读比较
		while(1)//
		{
			for(Page=0;Page<64;Page++)
			{
				i=(ADDR_Dest&0xfffe0000)+(Page*2048);
				p8[0x10000]=0x00;//1st Cycle Page Read Command
				p8[0x20000]=i;//Col.Add.1
				p8[0x20000]=(i>>8)&0x7;//Col.Add.2
				p8[0x20000]=(i>>11);//Row Add. 1
				p8[0x20000]=(i>>19);//Row Add. 2
			#if (NAND_2G|NAND_4G|NAND_8G)
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
				Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
				
				p8[0x10000]=0x00;//1st Cycle Page Read Command
				while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
				i=4;//至少延时tWHR=60ns
				while(i--);
	
				for(i=0;i<2048;i++)
				{
					if((i&0xff)!=p8[0])
					{
						break;
					}
				}
				if(i<2048)
				{//不相同
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//相同
			return 0;//返回;0=正确,1=错误
		}
	}
	return 1;//返回;0=正确,1=错误
}
#else
//bus=16bit
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
{
	u32 i;
	u32 Page;
	u16 *p16;
	u32 ReNum;
	
	p16=(u16*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
//块写
	ReNum=1;//写入错误重复次数
	while(ReNum--)
	{
		//块擦除
		i=ADDR_Dest;
		i>>=1;
		p16[0x10000>>1]=0x60;//Command
		p16[0x20000>>1]=(i>>10);//Row Add. 1
		p16[0x20000>>1]=(i>>18);//Row Add. 2
	#if (NAND_2G|NAND_4G|NAND_8G)
		p16[0x20000>>1]=(i>>26);//Row Add. 3
	#endif
		p16[0x10000>>1]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
		//等待块擦除完成
		Wait_NAND(3000*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		//块写回(每分页2048字节)
		for(Page=0;Page<64;Page++)
		{
			while(1)//掉电中断冲突重写
			{
				i=ADDR_Dest+(Page*2048);
				i>>=1;
				p16[0x10000>>1]=0x80;//Command
				p16[0x20000>>1]=i;//Col.Add.1
				p16[0x20000>>1]=(i>>8)&0x3;//Col.Add.2
				p16[0x20000>>1]=(i>>10);//Row Add. 1
				p16[0x20000>>1]=(i>>18);//Row Add. 2
			#if (NAND_2G|NAND_4G|NAND_8G)
				p16[0x20000>>1]=(i>>26);//Row Add. 3
			#endif
				for(i=0;i<1024;i++)
				{
					p16[0]=i;
				}
//				if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
//				{
//					Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
//					p8WBUFF-=8*256;
//					continue;
//				}
				p16[0x10000>>1]=0x10;//Command
				break;
			}
			Wait_NAND(600*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
		}
	//读比较
		while(1)//
		{
			for(Page=0;Page<64;Page++)
			{
				i=(ADDR_Dest&0xfffe0000)+(Page*2048);
				i>>=1;
				p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
				p16[0x20000>>1]=i;//Col.Add.1
				p16[0x20000>>1]=(i>>8)&0x3;//Col.Add.2
				p16[0x20000>>1]=(i>>10);//Row Add. 1
				p16[0x20000>>1]=(i>>18);//Row Add. 2
			#if (NAND_2G|NAND_4G|NAND_8G)
				p16[0x20000>>1]=(i>>26);//Row Add. 3
			#endif
				p16[0x10000>>1]=0x30;//2nd Cycle Cycle Page Read Command
				Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
				
				p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
				while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
				i=4;//至少延时tWHR=60ns
				while(i--);
	
				for(i=0;i<1024;i++)
				{
					if((i&0xffff)!=p16[0])
					{
						break;
					}
				}
				if(i<1024)
				{//不相同
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//相同
			return 0;//返回;0=正确,1=错误
		}
	}
	return 1;//返回;0=正确,1=错误
}
#endif


void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
	__disable_irq();//关中断
	DisplayClr();//清全屏
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	AutoDisplayRowCol->Row=2;
	DisplayStringAutoUp(0,9,(u8*)"\x0""CHARGE BATTERY");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Pin_Configure(PIN_UBACKUP,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	while(1)
	{
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+(1*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=hex_bcd(RTC->BKP3R&0x7fffffff);
		DisplayData_OffZero(i,8,0,pStrBuff);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+(3*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=ADCx(9,5,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
		pStrBuff=DisplayData_OffZero(i,4,2,pStrBuff);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		pStrBuff[0]='V'+0x2000;
		LCDDataOut();//点阵数据输出到LCD屏
		
		if(RTC->BKP3R&0x80000000)
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
				RTC->BKP3R++;
			}
			else
			{
				RTC->BKP3R|=0x80000000;
				break;
			}
		}
	}
	__enable_irq();//原中断恢复
}
#endif


#if IC_S34MLxG==1
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误
{
	u32 i;
	u32 x;
	u32 Page;
	u8 *p8;
//	u8 *p8WBUFF;
	u8 *p8RBUFF;
	u8 *p8ECC;
	u32 *p32ECC;
	u32 ReNum;
	
	p8=(u8*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
//块写
	ReNum=1;//写入错误重复次数
	while(ReNum--)
	{
		//块擦除
		p8[0x10000]=0x60;//Command
		p8[0x20000]=(ADDR_Dest>>11);//Row Add. 1
		p8[0x20000]=(ADDR_Dest>>19);//Row Add. 2
	#if (IC_S34ML02G2|IC_S34ML04G2)!=0
		p8[0x20000]=(ADDR_Dest>>27);//Row Add. 3
	#endif
		p8[0x10000]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
		//等待块擦除完成
		for(i=0;i<1000;i++)//常规测试时值i=111;当前最大值1000
		{
			SoftDelayUS(10);//软件us延时
			if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
			{
				break;
			}
		}
		//块写回(每分页2048字节)
//		p8WBUFF=(u8*)(ADDR_128KWRITE_BUFF);
		for(Page=0;Page<64;Page++)
		{
			while(1)//掉电中断冲突重写
			{
				i=ADDR_Dest+(Page*2048);
				p8[0x10000]=0x80;//Command
				p8[0x20000]=i;//Col.Add.1
				p8[0x20000]=(i>>8)&0x7;//Col.Add.2
				p8[0x20000]=(i>>11);//Row Add. 1
				p8[0x20000]=(i>>19);//Row Add. 2
			#if (IC_S34ML02G2|IC_S34ML04G2)!=0
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				p32ECC=(u32*)(ADDR_ECC_BUFF);
				for(x=0;x<8;x++)
				{
					FMC_Bank3->PCR&=~(1<<6);//禁止和复位 ECC 逻辑
					FMC_Bank3->PCR|=(1<<6);//ECC 计算逻辑使能位
					for(i=0;i<256;i++)
					{
						p8[0]=i;
					}
					while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
					for(i=0;i<2;i++);//等待最后字节ECC完成
					i=FMC_Bank3->ECCR;
					p32ECC[0]=i;
					p32ECC++;
				}
				//写ECC
				p8ECC=(u8*)(ADDR_ECC_BUFF);
				for(i=0;i<64;i++)
				{
					p8[0]=p8ECC[i];
				}
//				if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
//				{
//					Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
//					p8WBUFF-=8*256;
//					continue;
//				}
				p8[0x10000]=0x10;//Command
				while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
				break;
			}
			for(i=0;i<230;i++)//常规测试时值i=23;当前最大值230
			{
				SoftDelayUS(10);//软件us延时
				if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
				{
					break;
				}
			}
		}
	//读比较
		while(1)//
		{
//			p8WBUFF=(u8*)(ADDR_128KWRITE_BUFF);
			for(Page=0;Page<64;Page++)
			{
				i=(ADDR_Dest&0xfffe0000)+(Page*2048);
				p8[0x10000]=0x00;//1st Cycle Page Read Command
				p8[0x20000]=i;//Col.Add.1
				p8[0x20000]=(i>>8)&0x7;//Col.Add.2
				p8[0x20000]=(i>>11);//Row Add. 1
				p8[0x20000]=(i>>19);//Row Add. 2
			#if (IC_S34ML02G2|IC_S34ML04G2)!=0
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
				
				while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
				for(i=0;i<100;i++)//常规测试时值i=9;当前最大值100
				{
					SoftDelayUS(1);//软件us延时
					if(Pin_Read(PIN_NAND_WAIT)!=0)//读引脚,入口引脚使用名;返回0或1
					{
						break;
					}
				}
				p8RBUFF=(u8*)(ADDR_NAND_PAGEREAD_BUFF);
				p32ECC=(u32*)(ADDR_ECC_BUFF+128);
				for(x=0;x<8;x++)
				{
					FMC_Bank3->PCR&=~(1<<6);//禁止和复位 ECC 逻辑
					FMC_Bank3->PCR|=(1<<6);//ECC 计算逻辑使能位
					for(i=0;i<256;i++)
					{
						*p8RBUFF=p8[0];
						p8RBUFF++;
					}
					while(!(FMC_Bank3->SR&(1<<6)));//0：FIFO 非空
					for(i=0;i<2;i++);//等待最后字节ECC完成
					i=FMC_Bank3->ECCR;
					p32ECC[0]=i;
					p32ECC++;
				}
				//读ECC
				p8ECC=(u8*)(ADDR_ECC_BUFF);
				for(i=0;i<64;i++)
				{
					*p8ECC=p8[0];
					p8ECC++;
				}
				//ECC修正
//				for(i=0;i<8;i++)
//				{
//					STM32F4xx_ECC_Correct_Data((u8 *)ADDR_NAND_PAGEREAD_BUFF+(i*256),(u32 *)(ADDR_ECC_BUFF+(i*4)),(u32 *)(ADDR_ECC_BUFF+128+(i*4)));//NAND错误数据修正
//				}
				p8RBUFF=(u8*)(ADDR_NAND_PAGEREAD_BUFF);
				for(i=0;i<2048;i++)
				{
					if((i&0xff)!=p8RBUFF[i])
					{
						break;
					}
				}
				if(i<2048)
				{//不相同
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//相同
			return 0;//返回;0=正确,1=错误
		}
	}
	return 1;//返回;0=正确,1=错误
}

void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
	__disable_irq();//关中断
	DisplayClr();//清全屏
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	while(1)
	{
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=hex_bcd(RTC->BKP3R&0x7fffffff);
		DisplayData_OffZero(i,8,0,pStrBuff);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		LCDDataOut();//点阵数据输出到LCD屏
		
		if(RTC->BKP3R&0x80000000)
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
				RTC->BKP3R++;
			}
			else
			{
				RTC->BKP3R|=0x80000000;
				break;
			}
		}
	}
	__enable_irq();//原中断恢复
}

#endif
