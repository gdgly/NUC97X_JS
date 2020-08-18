
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Test/Test_RAM.h"
#include "../STM32F4xx/STM32F4xx_ECC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"




u32  GetCRC32(u32 initcrc,u8 * DataPtr,u32 DataLen,u32 Poly)
{
	u32 *Table_CRC;
	u32 i, j;
	u32 nData;
	u32 nAccum;

	Table_CRC=(u32 *)(ADDR_DATABUFF);
	for ( i = 0; i < 256; i++ )
	{
		nData =( i << 24 );
		nAccum = 0;
		for ( j = 0; j < 8; j++ )
		{
			if ( ( nData ^ nAccum ) & 0x80000000 )
			{
				nAccum = ( nAccum << 1 ) ^ Poly;
			}
			else
			{
				nAccum <<= 1;
			}
			nData <<= 1;
		}
		Table_CRC[i] = nAccum;
	}
	nAccum=initcrc;
  for (i = 0; i < DataLen/4; i++)
	{
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[3]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[2]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[1]];
		nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ DataPtr[0]];
		DataPtr+=4;
	}
	return nAccum;//^0xffffffff;
}


u32 Test_STM32F4xx(u32 ADDR_BASE)
{
	u32 i;
	u32 x;
	u32 y;
	u64 d64;
	u32 ISRAMErr;
	u32 ECCErr;
	u32 CRCErr;
	u32 fLSI;
	u32 fLSE;//0=OK,1=ERR,0xff=û����
	u32 fHSE;
	u8 *p8;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__disable_irq();//���ж�
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""STM32F4xx");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
//isram��64k���籣��������
	ISRAMErr=RAM_Test(ADDR_Meter_SaveRam_Start,64*1024);//RAM����;����:0=��ȷ,1=����
//isram��4K(BKPSRAM)����
	ISRAMErr|=RAM_Test(ADDR_BKPSRAM,4*1024);//RAM����;����:0=��ȷ,1=����
	ISRAMErr|=Comm_Ram->ErrorIC&1;
//isram��RTC->BKP0R-RTC->BKP19R
	ISRAMErr|=RAM_Test((u32)&RTC->BKP0R,4*20);//RAM����;����:0=��ȷ,1=����
	TestResult|=ISRAMErr;
//ECC
	ECCErr=0;//ECC_TEST();//ECC�����У�����(�����е�ÿ�ֽڰ�λȡ������,ѭ������=����*8);���أ�0=��ȷ,1=����
	TestResult|=ECCErr;
//CRC
	i=RAM_CRC32(ADDR_DATABUFF+1024,256);
	CRCErr=GetCRC32(0xffffffff,(u8 *)ADDR_DATABUFF+1024,256,0x04C11DB7);
	if(i==CRCErr)
	{
		CRCErr=0;
	}
	else
	{
		CRCErr=1;
		TestResult=1;
	}

	
//����LSEʱ��
	fLSE=0;//0=OK,1=ERR,0xff=û����
	RCC->BDCR=//λ 31:17 ���������뱣�ָ�λֵ
				(0<<16)|//λ 16 BDRST�������������λ (Backup domain software reset)������� 1 �����㡣ע�⣺ BKPSRAM ���ܴ˸�λӰ�죬ֻ���� Flash ��������Ӽ��� 1 ����Ϊ���� 0 ʱ��λBKPSRAM��
					//0����λδ����
					//1����λ����������
				(0<<15)|//λ 15 RTCEN��RTC ʱ��ʹ�� (RTC clock enable)������� 1 �����㡣
					//0��RTC ʱ�ӽ�ֹ
					//1��RTC ʱ��ʹ��
				//λ 14:10 ���������뱣�ָ�λֵ��
				(1<<8)|//λ 9:8 RTCSEL[1:0]��RTC ʱ��Դѡ�� (RTC clock source selection)������� 1������ѡ�� RTC ��ʱ��Դ��ѡ�� RTC ʱ��Դ�󣬳��Ǳ�����λ�������䲻���ٸ��ġ���ʹ�� BDRST λ������и�λ��
					//00����ʱ��
					//01��LSE ����ʱ������ RTC ʱ��
					//10��LSI ����ʱ������ RTC ʱ��
					//11���ɿɱ��Ԥ��Ƶ����Ƶ�� HSE ����ʱ�ӣ�ͨ�� RCC ʱ�����üĴ��� (RCC_CFGR)�е� RTCPRE[4:0] λѡ������ RTC ʱ��
				//λ 7:3 ���������뱣�ָ�λֵ��
				(1<<2)|//λ 2 LSEBYP���ⲿ����������· (External low-speed oscillator bypass)������� 1 �����㣬������·����ģʽ�µ�������ֻ���ڽ�ֹ LSE ʱ�Ӻ����д���λ��
					//0������· LSE ����
					//1����· LSE ����
				//λ 1 LSERDY���ⲿ������������ (External low-speed oscillator ready)��Ӳ���� 1 �����㣬����ָʾ�ⲿ 32 kHz �������ȶ����� LSEON λ�������LSERDY���� 6 ���ⲿ��������ʱ�����ں�תΪ�͵�ƽ��
					//0��LSE ʱ��δ����
					//1��LSE ʱ�Ӿ���
				1;//λ 0 LSEON���ⲿ��������ʹ�� (External low-speed oscillator enable)������� 1 �����㡣
					//0��LSE ʱ�ӹر�
					//1��LSE ʱ�ӿ���
					//�� LSE �������ڹر�״̬ʱ���ɷֱ� LSE �������� OSC32_IN �� OSC32_OUT �� ��ͨ�� PC14 I/O �� PC15 I/O��
					//�� LSE �������ڿ���״̬ʱ��PC14 I/O �� PC15 I/O ֻ�� ������Ϊ LSE �������� OSC32_IN �� OSC32_OUT��
					//��ͨ���� RCC_BDCR �Ĵ����н� LSEON λ�� 1 ����ɴ˲�����LSE �����ȼ����� GPIO ����
	for(i=0;i<10;i++)
	{
		SoftDelayMS(100);//���ms��ʱ
		if(RCC->BDCR&(1<<1))
		{
			break;//LSERDY���ⲿ������������
		}
	}
	if(i>=10)
	{
		fLSE=1;//0=OK,1=ERR,0xff=û����
		TestResult=1;
	}
	else
	{
//HSE(16MHz)
		START_STM32F4xx_TIM5_MEASURE_LSI(2);//������ʱ��TIM5Ϊ�ڲ�(LSI,LSE)ʱ�Ӳ���;���:TI4_RMP=1(LSI),=2(LSE)
		while((TIM5->SR&(1<<4))==0)//λ 4 CC4IF������/�Ƚ� 4 �жϱ�־ (Capture/Compare 4 interrupt flag)
		{
		}
		i=TIM5->CCR4;
		for(x=0;x<(32768/8);x++)
		{
			TIM5->SR=0;
			while((TIM5->SR&(1<<12))==0)//λ 12 CC4OF������/�Ƚ� 4 �ظ������־
			{
			}
			y=TIM5->CCR4;
		}
		y-=i;
		d64=RCC_GetPCLK1Freq()*2;
		d64*=32768*2;
		
		y=d64/y;
		if(y==32768)
		{
			fHSE=16000000;
		}
		else
		{
			if(y>32768)
			{
				y-=32768;
				fHSE=16000000-((y*16000000)/32768);
			}
			else
			{//y<32768
				y=32768-y;
				fHSE=16000000+((y*16000000)/32768);
			}
			y*=1000000;
			y/=32768;
			if(y>200)//200ppm
			{//�������� ?ppm
				TestResult=1;
			}
		}
	}
//LSI(32k)
	RCC->CSR|=1;//λ 0 LSION���ڲ���������ʹ��
	while((RCC->CSR&2)==0)
	{//�ȴ�LSI RC ��������
	}
	START_STM32F4xx_TIM5_MEASURE_LSI(1);//������ʱ��TIM5Ϊ�ڲ�(LSI,LSE)ʱ�Ӳ���;���:TI4_RMP=1(LSI),=2(LSE)
	while((TIM5->SR&(1<<4))==0)//λ 4 CC4IF������/�Ƚ� 4 �жϱ�־ (Capture/Compare 4 interrupt flag)
	{
	}
	i=TIM5->CCR4;
	for(x=0;x<(16384/8);x++)
	{
		TIM5->SR=0;
		while((TIM5->SR&(1<<12))==0)//λ 12 CC4OF������/�Ƚ� 4 �ظ������־
		{
		}
		fLSI=TIM5->CCR4;
	}
	
	fLSI-=i;
	d64=RCC_GetPCLK1Freq()*2;
	d64*=16384*2;
	
	fLSI=d64/fLSI;
	if((fLSI<17000)||(fLSI>47000))
	{
		TestResult=1;
	}
//

//�洢����
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=ISRAMErr;
	p8[1]=ECCErr;
	p8[2]=CRCErr;
	p8[3]=fLSE;
	MWR(fHSE,ADDR_DATABUFF+4,4);
	MWR(fLSI,ADDR_DATABUFF+8,4);

	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_STM32F4xx_Characteristics),12);
	if(TestResult)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	return TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
}

