
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/TEST/FastTest.h"
#include "../../NUC97x/TEST/Test_ALIGN.h"

#include "../../NUC97x/Driver/NUC97x_SPIFLASH.h"
#include "../../STM32F4xx/Device/NAND.h"
#include "../../STM32F4xx/Device/IC_TESAM.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_CRC.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_RCC.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Device/MEMRW.h"






void FastTest(void)//��Χ�������ٲ���
{
	u32 i;
	u64 d64;
	
//DATA ALIGN
	i=Test_ALIGN();//���ݶ������;����:0=��ȷ,1=����
	if(i)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""DATA ALIGN  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
//CPU CLK
	i=GetClockMHz(SYS_SYSTEM);//NUC97x�õ�ʱ��Ƶ��MHz
	if(i!=300)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""300MHz  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol

	}
//SPI FLASH
	i=ID_SPIFLASH();//��SPI FLASH����ID
	if(i!=0xEF15)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""W25Q32JV  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
//NAND FLASH
	d64=ID_NAND();//������ID
#ifdef  NAND_1G
	if(d64!=0xEFF1809500)
#endif
#ifdef  NAND_2G	
	if(d64!=0xEFDA909504)
#endif
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""W29N02GV  ERROR1");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
//RTC
	if(Comm_Ram->ErrorIC&(1<<4))//Ӳ�������־(��λ):B4=RTC
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		DisplayStringAutoUp(0,9,(u8*)"\x0""RX8025T  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
//
	
}


u32 TESAM_CMD_TxRx_FastTest(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
{
//ClaInsP1P2��APDU����4�ֽ��ױ�ֱ�ΪCla(ָ�����),Ins(ָ������е�ָ�����),P1(����1),P2(����2)
//Lc(P3)��APDU����������ֶ���(����)���ֵ��ֽ���
	u32 i;
	u32 x;
	u8 * p8;
  
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

void FastTest_AfterStartTimer(void)//��Χ�������ٲ���������1ms��ʱ�жϺ�
{
	u32 i;
	u32 MAC;
	u64 d64;
	u8* p8;
	
	i=i;
//��̫��MAC��ַ����
//0xDE684896 C7641C33
//0xDE684896 C7516F21
//0xDE6874a3 47826733
	
	d64=Unique_ID_SPIFLASH();//��SPI FLASH����Unique ID
	MWR(d64,ADDR_ETHMAC,8);
	MAC=RAM_CRC32(ADDR_ETHMAC,8);//RAM������CRC32����
	p8=(u8*)(ADDR_ETHMAC);
	//MAC0
	p8[0]=0xac;
	p8[1]=0xe0;
	p8[2]=MAC>>24;
	p8[3]=MAC>>16;
	p8[4]=MAC>>8;
	p8[5]=MAC;
#if MainProtocol==698
	MW(ADDR_ETHMAC,ADDR_4510_5+2,6);//��̫��ͨ��ģ��1;5 MAC��ַ
#endif
	//MAC1
	p8=(u8*)(ADDR_ETHMAC+6);
	p8[0]=0xac;
	p8[1]=0xe2;
	p8[2]=MAC>>24;
	p8[3]=MAC>>16;
	p8[4]=MAC>>8;
	p8[5]=MAC;

//TESAM
//---TESAM LOOP TEST----
/*
	while(1)
	{
		UART7Ctrl->Task=0;//��0��ʼ,���ϵ缴���˵�Դ,��Դ�����ٶ���
		while(TESAM_CMD_TxRx_FastTest(0x803600FF,0,0))
		{
			UART7Ctrl->Task=0;//��0��ʼ,���ϵ缴���˵�Դ,��Դ�����ٶ���
			Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
			DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		}
	}
*/
//---TESAM LOOP TEST END----
	
#if IC_TESAM==1//�ն���ESAM,0==û,1==��
	i=2;//��������ٲ�1��
	while(i)
	{
		i--;
		UART7Ctrl->Task=0;//��0��ʼ,���ϵ缴���˵�Դ,��Դ�����ٶ���
	#if MainProtocol!=698
		if(TESAM_CMD_TxRx_FastTest(0x800E0002,0,0))//��ȡоƬ���к�8�ֽ�
	#else
		if(TESAM_CMD_TxRx_FastTest(0x803600FF,0,0))//��ȡ�ն˰�ȫоƬ��Ϣ
	#endif
		{
			UART7Ctrl->Task=0;//��0��ʼ,���ϵ缴���˵�Դ,��Դ�����ٶ���
			if(i==0)
			{
				Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM  ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			}
		}
		else
		{
			break;
		}
	}
#endif

}







