
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"

#include "../OS/OS.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/LAN8720A.h"
#include "../Display/Display.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../NUC97x/NUC97x_AIC.h"


u32 Test_IP101GRI(u32 ADDR_BASE)
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	os_Type *os;
	
	__enable_irq();//�����ж�
	DisplayStringAutoUp(0,9,(u8*)"\x0""IP101GRI");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	switch(UARTCtrl->LinkTask)
	{
		case 0://0=��ʱ�ػ�
			while(p16timer[0]);
		case 1://1=�ص�Դ
			UnInit_NUC97x_MAC();
		case 2://����Դ,ETHϵͳ��ʼ��
			//ETHϵͳ��ʼ��
			OnE5V0();//��E5V0��Դ
			Init_NUC97x_MAC();
			Init_IP101();
			p16timer[0]=2000/10;//�ȴ�����RJ45����2s��ʱ
			while(p16timer[0]);
			os=Get_os();
			os->netInitialize();//ÿ������OS��ֻ�ܵ��ó�ʼ��1��,��������
		case 3://�ȴ�����RJ45����(ÿ2����)
			while(p16timer[0]);
		default:
			i=Read_ETH_PHYReg(PHYAddr_IP101,2);
			if(i==0x0243)//IP101GRI
			{
				DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
				MC(0,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
				return 0;
			}
			else
			{
				DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
				MC(1,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
				return 1;
			}
	}
}



