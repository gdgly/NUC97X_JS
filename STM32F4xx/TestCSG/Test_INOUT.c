

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"




u32 Test_INOUT(u32 ADDR_BASE)//�����������
{
#if MainProtocol==CSG
	return 0;
	
#else
	u32 i;
	u32 x;
	u8* p8;
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""INOUT");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	__enable_irq();//�����ж�
	Comm_Ram->GPMSTimer=2000/10;//14 ͨ��(������UART11) ÿ10MS��1��ʱ��
	while(Comm_Ram->GPMSTimer!=0)
	{
		i=Pin_Read(PIN_SWIN1);//������,�������ʹ����;����0��1
		i|=Pin_Read(PIN_SWIN2)<<1;
		i|=Pin_Read(PIN_SWIN3)<<2;
		i|=Pin_Read(PIN_SWIN4)<<3;
		i|=Pin_Read(PIN_DOOR_IN)<<4;
		if(i==0)
		{
			break;
		}
	}
	if(i)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		p8=(u8*)ADDR_DATABUFF;
		for(x=0;x<5;x++)
		{
			if(i&1)
			{
				p8[x]=1;//0=OK,1=ERR,0xff=û����
			}
			else
			{
				p8[x]=0;//0=OK,1=ERR,0xff=û����
			}
		}
		MW(ADDR_DATABUFF,ADDR_BASE+OFFSET_SWIN_Characteristics,5);//0=OK,1=ERR,0xff=û����
		return 1;
	}
	i=0;
	while(Comm_Ram->GPMSTimer!=0)
	{
		i=Pin_Read(PIN_SWIN1);//������,�������ʹ����;����0��1
		i|=Pin_Read(PIN_SWIN2)<<1;
		i|=Pin_Read(PIN_SWIN3)<<2;
		i|=Pin_Read(PIN_SWIN4)<<3;
		i|=Pin_Read(PIN_DOOR_IN)<<4;
		if(i==0x1f)
		{
			break;
		}
	}
	if(i!=0x1f)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		p8=(u8*)ADDR_DATABUFF;
		for(x=0;x<5;x++)
		{
			if(i&1)
			{
				p8[x]=0;//0=OK,1=ERR,0xff=û����
			}
			else
			{
				p8[x]=1;//0=OK,1=ERR,0xff=û����
			}
		}
		MW(ADDR_DATABUFF,ADDR_BASE+OFFSET_SWIN_Characteristics,5);//0=OK,1=ERR,0xff=û����
		return 1;
	}
	DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	MC(0,ADDR_BASE+OFFSET_SWIN_Characteristics,5);//0=OK,1=ERR,0xff=û����
	return 0;
#endif
}



