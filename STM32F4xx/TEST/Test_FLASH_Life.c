

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
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
{
	u32 i;
	u32 Page;
	u8 *p8;
	u32 ReNum;
	
	p8=(u8*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
//��д
	ReNum=1;//д������ظ�����
	while(ReNum--)
	{
		//�����
		p8[0x10000]=0x60;//Command
		p8[0x20000]=(ADDR_Dest>>11);//Row Add. 1
		p8[0x20000]=(ADDR_Dest>>19);//Row Add. 2
	#if (NAND_2G|NAND_4G|NAND_8G)
		p8[0x20000]=(ADDR_Dest>>27);//Row Add. 3
	#endif
		p8[0x10000]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
		//�ȴ���������
		Wait_NAND(3000*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		//��д��(ÿ��ҳ2048�ֽ�)
		for(Page=0;Page<64;Page++)
		{
			while(1)//�����жϳ�ͻ��д
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
//				if(Comm_Ram->IntFlags&0x08)//�жϷ�����ʹ�ó�ͻ��־
//				{
//					Comm_Ram->IntFlags=0;//�жϷ�����ʹ�ó�ͻ��־
//					p8WBUFF-=8*256;
//					continue;
//				}
				p8[0x10000]=0x10;//Command
				break;
			}
			Wait_NAND(600*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		}
	//���Ƚ�
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
				Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
				
				p8[0x10000]=0x00;//1st Cycle Page Read Command
				while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
				i=4;//������ʱtWHR=60ns
				while(i--);
	
				for(i=0;i<2048;i++)
				{
					if((i&0xff)!=p8[0])
					{
						break;
					}
				}
				if(i<2048)
				{//����ͬ
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//��ͬ
			return 0;//����;0=��ȷ,1=����
		}
	}
	return 1;//����;0=��ȷ,1=����
}
#else
//bus=16bit
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
{
	u32 i;
	u32 Page;
	u16 *p16;
	u32 ReNum;
	
	p16=(u16*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
//��д
	ReNum=1;//д������ظ�����
	while(ReNum--)
	{
		//�����
		i=ADDR_Dest;
		i>>=1;
		p16[0x10000>>1]=0x60;//Command
		p16[0x20000>>1]=(i>>10);//Row Add. 1
		p16[0x20000>>1]=(i>>18);//Row Add. 2
	#if (NAND_2G|NAND_4G|NAND_8G)
		p16[0x20000>>1]=(i>>26);//Row Add. 3
	#endif
		p16[0x10000>>1]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
		//�ȴ���������
		Wait_NAND(3000*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		//��д��(ÿ��ҳ2048�ֽ�)
		for(Page=0;Page<64;Page++)
		{
			while(1)//�����жϳ�ͻ��д
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
//				if(Comm_Ram->IntFlags&0x08)//�жϷ�����ʹ�ó�ͻ��־
//				{
//					Comm_Ram->IntFlags=0;//�жϷ�����ʹ�ó�ͻ��־
//					p8WBUFF-=8*256;
//					continue;
//				}
				p16[0x10000>>1]=0x10;//Command
				break;
			}
			Wait_NAND(600*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		}
	//���Ƚ�
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
				Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
				
				p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
				while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
				i=4;//������ʱtWHR=60ns
				while(i--);
	
				for(i=0;i<1024;i++)
				{
					if((i&0xffff)!=p16[0])
					{
						break;
					}
				}
				if(i<1024)
				{//����ͬ
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//��ͬ
			return 0;//����;0=��ȷ,1=����
		}
	}
	return 1;//����;0=��ȷ,1=����
}
#endif


void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
	__disable_irq();//���ж�
	DisplayClr();//��ȫ��
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	AutoDisplayRowCol->Row=2;
	DisplayStringAutoUp(0,9,(u8*)"\x0""CHARGE BATTERY");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	Pin_Configure(PIN_UBACKUP,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	while(1)
	{
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+(1*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=hex_bcd(RTC->BKP3R&0x7fffffff);
		DisplayData_OffZero(i,8,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+(3*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=ADCx(9,5,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
		pStrBuff=DisplayData_OffZero(i,4,2,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		pStrBuff[0]='V'+0x2000;
		LCDDataOut();//�������������LCD��
		
		if(RTC->BKP3R&0x80000000)
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
				RTC->BKP3R++;
			}
			else
			{
				RTC->BKP3R|=0x80000000;
				break;
			}
		}
	}
	__enable_irq();//ԭ�жϻָ�
}
#endif


#if IC_S34MLxG==1
u32 Life_BLOCK_Write_NAND(u32 ADDR_Dest)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
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
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
//��д
	ReNum=1;//д������ظ�����
	while(ReNum--)
	{
		//�����
		p8[0x10000]=0x60;//Command
		p8[0x20000]=(ADDR_Dest>>11);//Row Add. 1
		p8[0x20000]=(ADDR_Dest>>19);//Row Add. 2
	#if (IC_S34ML02G2|IC_S34ML04G2)!=0
		p8[0x20000]=(ADDR_Dest>>27);//Row Add. 3
	#endif
		p8[0x10000]=0xD0;//Command
		while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
		//�ȴ���������
		for(i=0;i<1000;i++)//�������ʱֵi=111;��ǰ���ֵ1000
		{
			SoftDelayUS(10);//���us��ʱ
			if(Pin_Read(PIN_NAND_WAIT)!=0)//������,�������ʹ����;����0��1
			{
				break;
			}
		}
		//��д��(ÿ��ҳ2048�ֽ�)
//		p8WBUFF=(u8*)(ADDR_128KWRITE_BUFF);
		for(Page=0;Page<64;Page++)
		{
			while(1)//�����жϳ�ͻ��д
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
					FMC_Bank3->PCR&=~(1<<6);//��ֹ�͸�λ ECC �߼�
					FMC_Bank3->PCR|=(1<<6);//ECC �����߼�ʹ��λ
					for(i=0;i<256;i++)
					{
						p8[0]=i;
					}
					while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
					for(i=0;i<2;i++);//�ȴ�����ֽ�ECC���
					i=FMC_Bank3->ECCR;
					p32ECC[0]=i;
					p32ECC++;
				}
				//дECC
				p8ECC=(u8*)(ADDR_ECC_BUFF);
				for(i=0;i<64;i++)
				{
					p8[0]=p8ECC[i];
				}
//				if(Comm_Ram->IntFlags&0x08)//�жϷ�����ʹ�ó�ͻ��־
//				{
//					Comm_Ram->IntFlags=0;//�жϷ�����ʹ�ó�ͻ��־
//					p8WBUFF-=8*256;
//					continue;
//				}
				p8[0x10000]=0x10;//Command
				while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
				break;
			}
			for(i=0;i<230;i++)//�������ʱֵi=23;��ǰ���ֵ230
			{
				SoftDelayUS(10);//���us��ʱ
				if(Pin_Read(PIN_NAND_WAIT)!=0)//������,�������ʹ����;����0��1
				{
					break;
				}
			}
		}
	//���Ƚ�
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
				
				while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
				for(i=0;i<100;i++)//�������ʱֵi=9;��ǰ���ֵ100
				{
					SoftDelayUS(1);//���us��ʱ
					if(Pin_Read(PIN_NAND_WAIT)!=0)//������,�������ʹ����;����0��1
					{
						break;
					}
				}
				p8RBUFF=(u8*)(ADDR_NAND_PAGEREAD_BUFF);
				p32ECC=(u32*)(ADDR_ECC_BUFF+128);
				for(x=0;x<8;x++)
				{
					FMC_Bank3->PCR&=~(1<<6);//��ֹ�͸�λ ECC �߼�
					FMC_Bank3->PCR|=(1<<6);//ECC �����߼�ʹ��λ
					for(i=0;i<256;i++)
					{
						*p8RBUFF=p8[0];
						p8RBUFF++;
					}
					while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
					for(i=0;i<2;i++);//�ȴ�����ֽ�ECC���
					i=FMC_Bank3->ECCR;
					p32ECC[0]=i;
					p32ECC++;
				}
				//��ECC
				p8ECC=(u8*)(ADDR_ECC_BUFF);
				for(i=0;i<64;i++)
				{
					*p8ECC=p8[0];
					p8ECC++;
				}
				//ECC����
//				for(i=0;i<8;i++)
//				{
//					STM32F4xx_ECC_Correct_Data((u8 *)ADDR_NAND_PAGEREAD_BUFF+(i*256),(u32 *)(ADDR_ECC_BUFF+(i*4)),(u32 *)(ADDR_ECC_BUFF+128+(i*4)));//NAND������������
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
				{//����ͬ
					break;
				}
//				p8WBUFF+=2048;
			}
			break;
		}
		if(Page==64)
		{//��ͬ
			return 0;//����;0=��ȷ,1=����
		}
	}
	return 1;//����;0=��ȷ,1=����
}

void Test_FLASH_Life(void)
{
	u32 i;
	u32 x;
	u16 *pStrBuff;
	
	__disable_irq();//���ж�
	DisplayClr();//��ȫ��
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH LIFES");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	while(1)
	{
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=hex_bcd(RTC->BKP3R&0x7fffffff);
		DisplayData_OffZero(i,8,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		LCDDataOut();//�������������LCD��
		
		if(RTC->BKP3R&0x80000000)
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
				RTC->BKP3R++;
			}
			else
			{
				RTC->BKP3R|=0x80000000;
				break;
			}
		}
	}
	__enable_irq();//ԭ�жϻָ�
}

#endif
