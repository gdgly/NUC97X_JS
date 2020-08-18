

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../Device/LAN8720A.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"



u32 Test_LAN8720A(u32 ADDR_BASE)//LAN8720A����
{
	u32 i;
	u32 SMI;
	u32 LINK;
	u32 REF_CLK;
	u32 tRESET;
//	u32 x;
//	u64 d64;
	u8 *p8;
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""LAN8720Ai");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	OnE5V0();//��E5V0��Դ
	SoftDelayMS(50);//
	Init_STM32F4xx_MAC();
	Init_LAN8720A();
//	SoftDelayMS(1);//
//Serial Management Interface (SMI)
	i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);//��ETH_PHY�Ĵ���
	if(i==7)
	{
		SMI=0;
	}
	else
	{
		SMI=1;
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
//Auto-Negotiation
	__enable_irq();//�����ж�
	LINK=0;
	Comm_Ram->GPMSTimer=5000/10;
	while(1)
	{
		SoftDelayMS(50);//���ms��ʱ
		i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);//��ETH_PHY�Ĵ���
		if((i&((1<<2)+(1<<5)))==((1<<2)+(1<<5)))//bit2 link is up,bit5 auto-negotiate process completed
		{
			i=i;
			break;
		}
		if(Comm_Ram->GPMSTimer==0)
		{
			LINK=1;
			TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
			break;
		}
	}
	
//REF_CLK(��ʱ������ʱ��Ƶ�ʽϵ͹��޷���׼50MHzʱ��,ֻ�ܲ�����ʱ������)
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	Pin_Configure(PIN_ETH_REF_CLK,
				2|//B3-B0:���ù���ѡ��AF(0-15)
	 (2<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
	 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
	 (2<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
	 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
	(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	START_STM32F4xx_TIM5_MEASURE_REF_CLK();//������ʱ��TIM5����LAN8720A�Ĳο�ʱ��
	Comm_Ram->GPMSTimer=5000/10;
	REF_CLK=0;//0=OK,1=ERR,0xff=û����
	while((TIM5->SR&(1<<2))==0)//λ 2 CC2IF������/�Ƚ� 2 �жϱ�־ (Capture/Compare 2 interrupt flag)
	{
		if(Comm_Ram->GPMSTimer==0)
		{
			REF_CLK=1;//0=OK,1=ERR,0xff=û����
			TestResult=1;
			break;
		}
	}
//tRESET
	for(tRESET=110;tRESET!=0;tRESET--)
	{
		Write_ETH_PHYReg(PHYAddr_LAN8720A_0,2,0x0);//дETH_PHY�Ĵ���
		Pin_OutL(PIN_ETH_RST);
		SoftDelayUS(tRESET);//���us��ʱ
		Pin_OutH(PIN_ETH_RST);
		i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);//��ETH_PHY�Ĵ���
		if(i!=7)
		{
			break;
		}
	}
	if(tRESET==0)
	{
		tRESET=1;//����1us
	}
	if(tRESET>=110)
	{
		TestResult=1;
	}
	if(TestResult==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	//�洢����
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=SMI;
	p8[1]=LINK;
	p8[2]=REF_CLK;
	p8[3]=tRESET;
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_LAN8720A_Characteristics),4);
	return TestResult;
}



