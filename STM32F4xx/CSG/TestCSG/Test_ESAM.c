

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Device/IC_TESAM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "../Display/Display.h"
#include "../Device/MEMRW.h"





u32 TEST_TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
{
//ClaInsP1P2��APDU����4�ֽ��ױ�ֱ�ΪCla(ָ�����),Ins(ָ������е�ָ�����),P1(����1),P2(����2)
//Lc(P3)��APDU����������ֶ���(����)���ֵ��ֽ���
	u32 i;
	u32 x;
	u8 * p8;
  
	UART7Ctrl->Lock=0;
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM���շ���
  }
  p8=(u8 *)(ADDR_UART7Tx);
	p8[0]=0x55;
	p8[1]=ClaInsP1P2>>24;
	p8[2]=ClaInsP1P2>>16;
	p8[3]=ClaInsP1P2>>8;
	p8[4]=ClaInsP1P2>>0;
	p8[5]=Lc>>8;
	p8[6]=Lc;
	MR(ADDR_UART7Tx+7,ADDR_DATA,Lc);
	//LRC1�ļ��㷽������CLA INS P1 P2 Len1 Len2 DATA���ݣ�ÿ���ֽڵ����ֵ����ȡ��
	x=p8[1];
	for(i=0;i<(5+Lc);i++)
	{
		x^=p8[2+i];
	}
	x=(~x);
	p8[7+Lc]=x;
	
	UART7Ctrl->Task=10;//�ȴ����ͺ����
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM���շ���
  }
	
	p8=(u8 *)(ADDR_UART7Rx);
	i=(p8[1]<<8)+p8[2];
	if((UART7Ctrl->Task!=100)||(i!=0x9000))
	{
		return 1;
	}
	return 0;
}



u32 Test_ESAM(u32 ADDR_BASE)
{
	u32 i;
	u32 BR;
	u64 d64;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__enable_irq();//�����ж�
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	
//TESAM
	DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	while(UART7Ctrl->Task<100)
	{
		TESAM_RXTX();//TESAM���շ���(��׼SPI1��)
	}
	d64=0;
	BR=4;
	for(i=0;i<5;i++)
	{
		SPI1->CR1&=~(0x7<<3);
		SPI1->CR1|=(BR<<3);
		BR--;
	#if MainProtocol!=698
		TEST_TESAM_CMD_TxRx(0x800E0002,0,0);//��ȡоƬ���к�8�ֽ�
	#else
		TEST_TESAM_CMD_TxRx(0x803600FF,0,0);//��ȡоƬ���к�8�ֽ�
	#endif
		while(UART7Ctrl->Task<100)
		{
			TESAM_RXTX();//TESAM���շ���(��׼SPI1��)
		}
		if(UART7Ctrl->Task==100)
		{
			d64=MRR(ADDR_UART7Rx+5,8);//ȡ���к�
		}
		else
		{
			BR+=2;
			if(BR>7)
			{
				BR=7;
			}
			break;
		}
		if(BR==0)
		{
			break;
		}
	}
	SPI1->CR1&=~(0x7<<3);
	SPI1->CR1|=(3<<3);
	BR=RCC_GetPCLK2Freq()/(2<<BR);
	if(BR>2000000)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	//�洢����
	MWR(BR,ADDR_DATABUFF,4);
	MWR(d64,ADDR_DATABUFF+4,8);
	
//MESAM
	
	
	//�洢����
	MWR(0xffffffff,ADDR_DATABUFF+12,4);
	MWR(d64,ADDR_DATABUFF+16,8);
	//д
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_ESAM_Characteristics),24);
	
	return TestResult;
}








