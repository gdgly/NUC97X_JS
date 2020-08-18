
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "Test_FLASH_Block_Err.h"
#include "../Display/Display.h"
#include "../Device/NAND.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"




#if IC_MT29FxG==1
#if NAND_BUS_WIDTH==8//8=8bit,16=16bit
//bus=8bit
u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest,u32 Data)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����;ʵ��HCLK=168Mʱ107ms
{
	u32 i;
	u32 Page;
	u8 *p8;
	u32 ReNum;
	
	p8=(u8*)0x80000000;
	ADDR_Dest&=0xfffe0000;
	WWDT_Enable_Feed(WDTTimerOutVal);//�������Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
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
					p8[0]=Data;
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
					if(Data!=p8[0])
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
u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest,u32 Data)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����;ʵ��HCLK=168Mʱ107ms
{
	u32 i;
	u32 Page;
	u16 *p16;
	u32 ReNum;
	
	p16=(u16*)0x80000000;
	ADDR_Dest&=0xfffe0000;
	WWDT_Enable_Feed(WDTTimerOutVal);//�������Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
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
					p16[0]=Data;
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
					if(Data!=p16[0])
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


u32 Test_FLASH_Block_Err(void)//����;0=��ȷ,1=����
{
	u32 i;
	u32 x;
	u32 ADDR_BASE;
	u32 BLOCK_COUNT;
	u32 ERROR_COUNT;
	
//	u32 *p32;
	u16 *pStrBuff;
	u8 *p8EEPROM;
	
	
	p8EEPROM=(u8*)ADDR_Characteristics_NO;//1BYTE ���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>45)
	i=p8EEPROM[0];
	if(i>2)
	{
		i=0;
	}
	ADDR_BASE=ADDR_Characteristics_BASE1+(i*LEN_BASE_Characteristics);
	
	__disable_irq();//���ж�
	DisplayClr();//��ȫ��
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH TEST");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//	i=ID_S34MLxxG2();//������ID(1Gb*8=0x01F1801D,1Gb*16=0x01C1805D)
//	if(i!=0x01F1801D)
//	{
//		DisplayStringAutoUp(0,9,(u8*)"\x0""Error!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//		return;
//	}
#if NAND_1G==1//0=û,1=��(128 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:1024");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if NAND_2G==1//0=û,1=��(256 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:2048");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if NAND_4G==1//0=û,1=��(512 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:4096");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if NAND_8G==1//0=û,1=��(512 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:8192");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
	DisplayStringAutoUp(0,9,(u8*)"\x0""Error Blocks:0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0"" ");//����������ʾ��
	
	MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE,NAND_BLOCK_COUNT);//EEPROM���
//	p32=(u32*)(ADDR_128KWRITE_BUFF);
	p8EEPROM=(u8*)ADDR_NANDFLASH_BLOCK_REPLACE;
	
	for(BLOCK_COUNT=NAND_FILE_BLOCK_COUNT;BLOCK_COUNT<NAND_BLOCK_COUNT;BLOCK_COUNT++)
	{
//		//����ԭ����
//		Read_NAND(ADDR_128K_DATABUFF,BLOCK_COUNT*128*1024,128*1024);//NAND_FLASH S34MLxxG2û�滻��
		//д0
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=0;
//		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024,0x0);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
//		//��д˳����
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=i;
//		}
//		i=BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
//		if(i!=0)
//		{
//			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
//		}
		//д0xff
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=0xffffffff;
//		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024,0xff);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
//		//�ָ�ԭ����
//		MW(ADDR_128K_DATABUFF,ADDR_128KWRITE_BUFF,128*1024);
//		BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����

		if((BLOCK_COUNT%8)==0)
		{
			//��ʾ�������
			ERROR_COUNT=0;
			for(i=0;i<NAND_BLOCK_COUNT;i++)
			{
				x=p8EEPROM[i];
				if(x==0xFE)
				{
					ERROR_COUNT++;
					continue;
				}
				if((x>=1)&&(x<=NAND_REPLACE_BLOCK_COUNT))
				{
					ERROR_COUNT++;
				}
			}
			pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(13*2));
			for(i=0;i<4;i++)
			{
				pStrBuff[i]=0;
			}
			x=hex_bcd(ERROR_COUNT);
			DisplayData_OffZero(x,4,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��ʾ��ʱ(S)

			//��ʾ����
			i=(BLOCK_COUNT*100)/NAND_BLOCK_COUNT;
			DisplayRATE(AutoDisplayRowCol->Row-1,i);//��ʾ����;���:Ln=��ʾ��,Rate=����(hex,0-100%)
		}
	}

	
	//���
	AutoDisplayRowCol->Row--;
	DisplayClrRow(AutoDisplayRowCol->Row,AutoDisplayRowCol->Row);//�������ʾ��
	MWR(ERROR_COUNT,ADDR_BASE+(OFFSET_NAND_ERROR_BLOCKS),2);
#if NAND_1G==1//0=û,1=��(128 Mbyte)
	if(ERROR_COUNT<=20)
#endif
#if NAND_2G==1//0=û,1=��(256 Mbyte)
	if(ERROR_COUNT<=40)
#endif
#if NAND_4G==1//0=û,1=��(512 Mbyte)
	if(ERROR_COUNT<=80)
#endif
#if NAND_8G==1//0=û,1=��(512 Mbyte)
	if(ERROR_COUNT<=160)
#endif
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	__enable_irq();//ԭ�жϻָ�
		return 0;//����;0=��ȷ,1=����
	}
	else
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		MWR(1,ADDR_BASE+(OFFSET_TestResult_Characteristics),1);//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__enable_irq();//ԭ�жϻָ�
		return 1;//����;0=��ȷ,1=����
	}

}
#endif


#if IC_S34MLxG==1
u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest,u32 Data)//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����;ʵ��HCLK=168Mʱ107ms
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
	ADDR_Dest&=0xfffe0000;
	WWDT_Enable_Feed(WDTTimerOutVal);//�������Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
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
		for(i=0;i<1000;i++)//�������ʱֵi=111;��ǰ���ֵ1000
		{
			SoftDelayUS(10);//����us��ʱ
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
			#if (NAND_2G|NAND_4G|NAND_8G)
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				p32ECC=(u32*)(ADDR_ECC_BUFF);
				for(x=0;x<8;x++)
				{
					FMC_Bank3->PCR&=~(1<<6);//��ֹ�͸�λ ECC �߼�
					FMC_Bank3->PCR|=(1<<6);//ECC �����߼�ʹ��λ
					for(i=0;i<256;i++)
					{
						p8[0]=Data;
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
				SoftDelayUS(10);//����us��ʱ
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
			#if (NAND_2G|NAND_4G|NAND_8G)
				p8[0x20000]=(i>>27);//Row Add. 3
			#endif
				p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
				
				while(!(FMC_Bank3->SR&(1<<6)));//0��FIFO �ǿ�
				for(i=0;i<100;i++)//�������ʱֵi=9;��ǰ���ֵ100
				{
					SoftDelayUS(1);//����us��ʱ
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
					if(Data!=p8RBUFF[i])
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



u32 Test_FLASH_Block_Err(void)//����;0=��ȷ,1=����
{
	u32 i;
	u32 x;
	u32 ADDR_BASE;
	u32 BLOCK_COUNT;
	u32 ERROR_COUNT;
	
//	u32 *p32;
	u16 *pStrBuff;
	u8 *p8EEPROM;
	
	
	p8EEPROM=(u8*)ADDR_Characteristics_NO;//1BYTE ���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>45)
	i=p8EEPROM[0];
	if(i>2)
	{
		i=0;
	}
	ADDR_BASE=ADDR_Characteristics_BASE1+(i*LEN_BASE_Characteristics);
	
	__disable_irq();//���ж�
	DisplayClr();//��ȫ��
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH TEST");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//	i=ID_S34MLxxG2();//������ID(1Gb*8=0x01F1801D,1Gb*16=0x01C1805D)
//	if(i!=0x01F1801D)
//	{
//		DisplayStringAutoUp(0,9,(u8*)"\x0""Error!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//		return;
//	}
#if NAND_1G==1//0=û,1=��(128 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:1024");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if NAND_2G==1//0=û,1=��(256 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:2048");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if NAND_4G==1//0=û,1=��(512 Mbyte)
	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:4096");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
	DisplayStringAutoUp(0,9,(u8*)"\x0""Error Blocks:0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0"" ");//����������ʾ��
	
	MC(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE,NAND_BLOCK_COUNT);//EEPROM���
//	p32=(u32*)(ADDR_128KWRITE_BUFF);
	p8EEPROM=(u8*)ADDR_NANDFLASH_BLOCK_REPLACE;
	
	for(BLOCK_COUNT=NAND_FILE_BLOCK_COUNT;BLOCK_COUNT<NAND_BLOCK_COUNT;BLOCK_COUNT++)
	{
//		//����ԭ����
//		Read_NAND(ADDR_128K_DATABUFF,BLOCK_COUNT*128*1024,128*1024);//NAND_FLASH S34MLxxG2û�滻��
		//д0
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=0;
//		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024,0x0);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
//		//��д˳����
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=i;
//		}
//		i=BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
//		if(i!=0)
//		{
//			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
//		}
		//д0xff
//		for(i=0;i<((128*1024)/4);i++)
//		{
//			p32[i]=0xffffffff;
//		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024,0xff);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
//		//�ָ�ԭ����
//		MW(ADDR_128K_DATABUFF,ADDR_128KWRITE_BUFF,128*1024);
//		BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����

		if((BLOCK_COUNT%8)==0)
		{
			//��ʾ�������
			ERROR_COUNT=0;
			for(i=0;i<NAND_BLOCK_COUNT;i++)
			{
				x=p8EEPROM[i];
				if(x==0xFE)
				{
					ERROR_COUNT++;
					continue;
				}
				if((x>=1)&&(x<=NAND_REPLACE_BLOCK_COUNT))
				{
					ERROR_COUNT++;
				}
			}
			pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(13*2));
			for(i=0;i<4;i++)
			{
				pStrBuff[i]=0;
			}
			x=hex_bcd(ERROR_COUNT);
			DisplayData_OffZero(x,4,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��ʾ��ʱ(S)

			//��ʾ����
			i=(BLOCK_COUNT*100)/NAND_BLOCK_COUNT;
			DisplayRATE(AutoDisplayRowCol->Row-1,i);//��ʾ����;���:Ln=��ʾ��,Rate=����(hex,0-100%)
		}
	}

	
	//���
	AutoDisplayRowCol->Row--;
	DisplayClrRow(AutoDisplayRowCol->Row,AutoDisplayRowCol->Row);//�������ʾ��
	MWR(ERROR_COUNT,ADDR_BASE+(OFFSET_NAND_ERROR_BLOCKS),2);
#if NAND_1G==1//0=û,1=��(128 Mbyte)
	if(ERROR_COUNT<=20)
#endif
#if NAND_2G==1//0=û,1=��(256 Mbyte)
	if(ERROR_COUNT<=40)
#endif
#if NAND_4G==1//0=û,1=��(512 Mbyte)
	if(ERROR_COUNT<=80)
#endif
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	__enable_irq();//ԭ�жϻָ�
		return 0;//����;0=��ȷ,1=����
	}
	else
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		MWR(1,ADDR_BASE+(OFFSET_TestResult_Characteristics),1);//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__enable_irq();//ԭ�жϻָ�
		return 1;//����;0=��ȷ,1=����
	}
}
#endif







