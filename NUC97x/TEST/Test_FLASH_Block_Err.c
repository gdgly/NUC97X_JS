
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../TEST/Test_FLASH_Block_Err.h"
#include "../Display/Display.h"
#include "../Device/NAND.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"




void Test_Read_Page(u32 ADDR_Dest,u32 ADDR_Sour)//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
{
	u32 i;
	u32 x;
	u32 *p32NANDCTL;
	u32 *p32DMA;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32* p32s;
//	u32* p32d;
//	u32 *p32DATA;
	
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	
//�ȶ�ҳУ����(64byte)
	i=ADDR_Sour;
	*p32CMD=0x00;//1st Cycle Page Read Command
	*p32ADDR=0;//i;//Col.Add.1
	*p32ADDR=0x08;//(i>>8)&0x07;//Col.Add.2
	*p32ADDR=(i>>11);//Row Add. 1
#if (NAND_2G|NAND_4G|NAND_8G)
	*p32ADDR=(i>>19);//Row Add. 2
	*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
#else
	*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
#endif
	*p32CMD=0x30;//2nd Cycle Cycle Page Read Command
	Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
	p32NANDCTL=(u32*)(REG_NANDCTL);
	i=p32NANDCTL[0];
	i|=
	(0<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
	(1<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
	(0<<2)|//[2]DWR_EN DMA Write Data Enable
	(0<<1);//[1]DRD_EN DMA Read Data Enable
	p32NANDCTL[0]=i;
	while(p32NANDCTL[0]&(1<<3));
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//��ҳ����
	i=ADDR_Sour;
	*p32CMD=0x00;//1st Cycle Page Read Command
	*p32ADDR=0;//i;//Col.Add.1
	*p32ADDR=0;//(i>>8)&0x07;//Col.Add.2
	*p32ADDR=(i>>11);//Row Add. 1
#if (NAND_2G|NAND_4G|NAND_8G)
	*p32ADDR=(i>>19);//Row Add. 2
	*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
#else
	*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
#endif
	*p32CMD=0x30;//2nd Cycle Cycle Page Read Command
	
	Wait_NAND(70*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
	//while(!p32s[0]&0x400);
	p32s=(u32*)(REG_NANDINTSTS);
	p32s[0]|=0x405;//��[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IFλ
	
	p32DMA=(u32*)(REG_FMI_DMASA);
	p32DMA[0]=ADDR_Dest;
	p32NANDCTL=(u32*)(REG_NANDCTL);
	i=p32NANDCTL[0];
	i|=
	(0<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
	(0<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
	(0<<2)|//[2]DWR_EN DMA Write Data Enable
	(1<<1);//[1]DRD_EN DMA Read Data Enable
	p32NANDCTL[0]=i;
	while(1)
	{
		p32s=(u32*)(REG_NANDINTSTS);
		x=p32s[0];
		if(x&4)
		{//[2]ECC_FLD_IF,����0����DMA��ֹͣ���ٴ���
			p32s[0]=4;//��ECC_FLD_IF
		}
		if(x&1)
		{//[0]DMA_IFλ
			break;
		}
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
}


u32 Test_BLOCK_Write_NAND(u32 ADDR_Dest)//ûECC,��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
{
	u32 i;
	u32 Page;
	u32 *p32CMD;
	u32 *p32ADDR;
	u32 *p32NANDCTL;
	u32* p32DMA;
//	u32 *p32DATA;
	u32* p32s;
	u32* p32d;
	
	Init_NAND();//��λnand
	p32CMD=(u32*)(REG_NANDCMD);
	p32ADDR=(u32*)(REG_NANDADDR);
//	p32DATA=(u32*)(REG_NANDDATA);
	
//�����
		*p32CMD=0x60;
		*p32ADDR=(ADDR_Dest>>11);//Row Add. 1
	#if (NAND_2G|NAND_4G|NAND_8G)
		*p32ADDR=(ADDR_Dest>>19);//Row Add. 2
		*p32ADDR=(ADDR_Dest>>27)|0x80000000;//Row Add. 3
	#else
		*p32ADDR=(ADDR_Dest>>19)|0x80000000;//Row Add. 2
	#endif
		*p32CMD=0xD0;//Command
		Wait_NAND(3000*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��

//��д��(ÿ��ҳ2048�ֽ�)
		sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
		for(Page=0;Page<64;Page++)
		{
			i=ADDR_Dest+(Page*2048);
			*p32CMD=0x80;//Command
			*p32ADDR=i;//Col.Add.1
			*p32ADDR=(i>>8)&0x7;//Col.Add.2
			*p32ADDR=(i>>11);//Row Add. 1
		#if (NAND_2G|NAND_4G|NAND_8G)
			*p32ADDR=(i>>19);//Row Add. 2
			*p32ADDR=(i>>27)|0x80000000;//Row Add. 3
		#else
			*p32ADDR=(i>>19)|0x80000000;//Row Add. 2
		#endif
			
			p32s=(u32*)(REG_NANDINTSTS);
			p32s[0]|=0x405;//��[10]RB0_IF,[2]ECC_FLD_IF,[0]DMA_IFλ
			p32DMA=(u32*)(REG_FMI_DMASA);
			p32DMA[0]=ADDR_128KWRITE_BUFF+(Page*2048);
			p32NANDCTL=(u32*)(REG_NANDCTL);
			i=p32NANDCTL[0];
			i|=
			(1<<4)|//[4]REDUN_AUTO_WEN Redundant Area Auto Write Enable
			(0<<3)|//[3]REDUN_REN NOTE: When transfer completed, this bit will be cleared automatically.
			(1<<2)|//[2]DWR_EN DMA Write Data Enable
			(0<<1);//[1]DRD_EN DMA Read Data Enable
			p32NANDCTL[0]=i;
			while(1)
			{
				p32s=(u32*)(REG_NANDINTSTS);
				i=p32s[0];
				if(i&1)
				{//[0]DMA_IFλ
					break;
				}
			}
			*p32CMD=0x10;//Command
			Wait_NAND(600*3);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
		}
//���Ƚ�
		for(Page=0;Page<64;Page++)
		{
			Test_Read_Page(ADDR_NAND_PAGEREAD_BUFF,ADDR_Dest+(Page*2048));//NAND_FLASH û�滻��ҳ����;����:0=û����,!=0����
			p32s=(u32*)(ADDR_128KWRITE_BUFF+(Page*2048));
			p32d=(u32*)(ADDR_NAND_PAGEREAD_BUFF);
			for(i=0;i<(2048/4);i++)
			{
				if(p32s[i]!=p32d[i])
				{
					break;
				}
			}
			if(i<(2048/4))
			{
				break;
			}
		}
		if(Page>=64)
		{
			return 0;//����;0=��ȷ,1=����
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
	
	u32 *p32;
	u16 *pStrBuff;
	u8 *p8EEPROM;
	
	
	p8EEPROM=(u8*)ADDR_Characteristics_NO;//1BYTE ���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>45)
	i=p8EEPROM[0];
	if(i>2)
	{
		i=0;
	}
	ADDR_BASE=ADDR_Characteristics_BASE1+(i*LEN_BASE_Characteristics);
	
//	__disable_irq();//���ж�
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
	p32=(u32*)(ADDR_128KWRITE_BUFF);
	p8EEPROM=(u8*)(ADDR_NANDFLASH_BLOCK_REPLACE-ADDR_IEEPROM_START)+ADDR_SPI_MAP;
	
	for(BLOCK_COUNT=0;BLOCK_COUNT<NAND_BLOCK_COUNT;BLOCK_COUNT++)
	{
		WWDT_Enable_Feed(0);
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//������,�������ʹ����;����0��1
		{
			return 0;
		}
		#endif
		//����ԭ����
		Read_NAND(ADDR_128KDATABUFF,BLOCK_COUNT*128*1024,128*1024);//NAND_FLASH S34MLxxG2û�滻��
		//д0
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=0;
		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
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
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=0xffffffff;
		}
		i=Test_BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			MC(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		//�ָ�ԭ����
		MW(ADDR_128KDATABUFF,ADDR_128KWRITE_BUFF,128*1024);
		BLOCK_Write_NAND(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����

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
//	__enable_irq();//ԭ�жϻָ�
		return 0;//����;0=��ȷ,1=����
	}
	else
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		MWR(1,ADDR_BASE+(OFFSET_TestResult_Characteristics),1);//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
//	__enable_irq();//ԭ�жϻָ�
		return 1;//����;0=��ȷ,1=����
	}
}









