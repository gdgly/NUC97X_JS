
//�ն�Uart����
#include "Project698.h"
#include "Terminal698_Uart_Manage.h"
#include "UartAuto_645.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#include "Terminal_Contrl_Uart.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_LINK.h"

#include "../DL698/RM_RS485_Manage.h"
#include "../DL698/RM_3762_Manage.h"

#include "../Device/GPRS_3763.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/Ethernet.h"
#include "../DL698/DL698TEST.h"
//#include "../MS/MS.h"


	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif

void Link_Contrl(void)//����ͨ��ģ�����߿���
{
//ע:���߿��Ʊ仯ʱ:������������붨ʱ��=0,������������=0,ʹ�˳����¼֡���Ϸ���
//	u32 i;
//	u8 * p8;
//	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;
	
	

#if ((IC_LAN8720==1)||(IC_IP101==1))
//��̫��������������,���������������ģ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
//	if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
//	{
//		UART1Ctrl->LinkCtrl=0;//���ӿ���0=������,1=����
//		WarningOccur(GPRSPORT,(u8 *)"��̫���������߹ر�");//�澯����,��ڸ澯����͸澯�ַ���
//		return;
//	}
#endif
//GPRS����ģ������,��������ģʽ����������ģʽ��ʱ������ģʽ
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
	//��������
	UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
	
}



//��ʼ��
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=�ز�
//UART5=RS485-3
//UART6=����
//UART7=�ն�ESAMͨ�ſں�
//UART8=
//UART9=
//UART10=��̫��ͨ�ſں�
//UART11=����ͨ�ſں�
//UART12=��������ͨ�ſں�(����),ʹ��DATABUFF
void Terminal_UartInit_Manage(void)//Terminal_Uart��ʼ������
{
  UARTCtrl_TypeDef * UARTCtrl;
//RS232
	if(Get_LEN_UARTnCtrl(RS232PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
	#if DEBUG_PORTtoRS232==1
		UARTCtrl->BpsCtrl=0xb+(10<<5);//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	#else
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	#endif
	  Init_UARTn(RS232PORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//GPRS

//RS485-1
	if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//RS485-2
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//RS485-3
	if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
		UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(RS485_3PORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//RS485-4(�ز�)
	if(Get_LEN_UARTnCtrl(RS485_4PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
		UARTCtrl->BpsCtrl=0xb+(6<<5);
	  Init_UARTn(RS485_4PORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//����
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
}


//���շ���
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=�ز�
//UART5=RS485-3
//UART6=����
//UART7=�ն�ESAMͨ�ſں�
//UART8=
//UART9=
//UART10=��̫��ͨ�ſں�
//UART11=����ͨ�ſں�
//UART12=��������ͨ�ſں�(����),ʹ��DATABUFF
void Terminal_Uart_Manage(void)//Terminal_Uart����
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
	u8 *p8;
#endif
// #ifdef LINUX
// ms_Type * ms;
// ms=Get_ms();
//#endif
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_698_RxTx(RS232PORT);//UART_698���շ���
			UartAuto_645(RS232PORT);//�Զ�ʶ��DL_645֡
		}
//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_698_RxTx(IRPORT);//UART_698���շ���
			UartAuto_645(IRPORT);//�Զ�ʶ��DL_645֡
		}
		
//��������ͨ�ſں�(����)����
		RM_RS485_Manage(ACSAMPLEPORT);

#if RUN_DL698TEST_1==0//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
//RS485-1
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
		if(UARTCtrl->FunctionTimer!=Comm_Ram->RTCBuff[0])
		{
			UARTCtrl->FunctionTimer=Comm_Ram->RTCBuff[0];
			if(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0)//�ն�ά��
			{
				UARTCtrl->Function=0;//�ڹ��� 0=�ն�ά����,!=0�ն˳����
				if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
				{
					UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
					Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
				}
			}
			else
			{
				UARTCtrl->Function=1;//�ڹ��� 0=�ն�ά����,!=0�ն˳����
			}
		}
		if(UARTCtrl->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
		{
			WarningOccur(RS485_1PORT,(u8 *)"485\x1�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_698_RxTx(RS485_1PORT);//UART_698���շ���
				UartAuto_645(RS485_1PORT);//�Զ�ʶ��DL_645֡
			}
		}
		else
		{
			RM_RS485_Manage(RS485_1PORT);//RS485-1����
		}

//RS485-2
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
		if(UARTCtrl->FunctionTimer!=Comm_Ram->RTCBuff[0])
		{
			if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0))//�ն�ά��
			{
				UARTCtrl->Function=0;//�ڹ��� 0=�ն�ά����,!=0�ն˳����
				if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)
				{
					UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
					Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
				}
			}
			else
			{
				UARTCtrl->Function=1;//�ڹ��� 0=�ն�ά����,!=0�ն˳����
			}
		}
		if(UARTCtrl->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
		{
				WarningOccur(RS485_2PORT,(u8 *)"485\x2�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
				if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
				{
					Uart_698_RxTx(RS485_2PORT);//UART_698���շ���
					UartAuto_645(RS485_2PORT);//�Զ�ʶ��DL_645֡
				}
		}
		else
		{
			RM_RS485_Manage(RS485_2PORT);//RS485-2����
		}
//�ز�����
	#if ((Project/100)==2)//������
		RM_3762_Manage();//376.2·��ģ�����
	#endif
	#if ((Project/100)==3)//ר��
//����ģ��
		Terminal_Contrl_Uart_RxTx(RS485_4PORT);//����ģ��ͨ��RxTx
	#endif
	#if ((Project/100)==4)//����һ�ն�
		Pin_Configure(PIN_PLC_ID,//ģ���������,��������
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
		if(Pin_Read(PIN_PLC_ID)==0)
		{
			if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{//�ز�����
				RM_3762_Manage();//376.2·��ģ�����
			}
			else
			{//����ģ��
				Terminal_Contrl_Uart_RxTx(RS485_4PORT);//����ģ��ͨ��RxTx
			}
		}
		else
		{
			Comm_Ram->MODE3in1=0;//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			UARTCtrl->LinkTask=0;
			WarningOccur(RS485_4PORT,(u8 *)"���ƻ��ز�ģ��û����");//�澯����,��ڸ澯����͸澯�ַ���
		}
	#endif
#endif//#if RUN_DL698TEST_1==0//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������

	}

//����ͨ��ģ�����߿���
	Link_Contrl();//����ͨ��ģ�����߿���
//�����ϱ���Ҫ�¼�
//	Terminal_Uart_AutoEvent();//�����ϱ���Ҫ�¼�
//�����ϱ���ʱ����
//	Terminal_Uart_AutoTask();//�����ϱ���ʱ����


//ENET

//#warning "too much time start"
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP�ļ�����
	Uart_698_RxTx(ENETPORT);//UART_698���շ���
	LINK_request(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);
	//AutoCascadingData(ENETPORT);//�Զ��ϴ�����Ѳ�⵽������
//#warning "too much time end"


//GPRS
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	i=Pin_Read(PIN_GPRS_ID0);//������,�������ʹ����;����0��1
	i|=Pin_Read(PIN_GPRS_ID1)<<1;//������,�������ʹ����;����0��1
	if(i==3)
	{//GPRSģ���û����
		//��ģ���Դ��IO����
		UnInit_GPRS_3763();//GPRSģ������Ϊ����������
		Terminal_Ram->MODULSEL=i;//GPRSģ��ѡ��0=,1=,2=,3=��
		Terminal_Ram->Signal=0;//GPRS�ź�ǿ��0-4
		UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
		WarningOccur(GPRSPORT,(u8 *)"Զ������ģ��û����");//�澯����,��ڸ澯����͸澯�ַ���
	
//#ifdef LINUX
//		#undef  RJ45SELECT
//		#define RJ45SELECT     0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立�?eth0用于以太网RJ45通道
//	 #if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立�?eth0用于以太网RJ45通道
//		 ms->link_system("/sbin/ifconfig eth0 down");
//	 #elif RJ45SELECT==1
//		 ms->link_system("/sbin/ifconfig eth1 down");
//	 #elif RJ45SELECT==2
//		 ms->link_system("/sbin/ifconfig eth0 down");
//		 ms->link_system("/sbin/ifconfig eth1 down");
//	 #endif
//#endif
	}
	else
	{//GPRSģ��в���
		if(i!=Terminal_Ram->MODULSEL)
		{//GPRSģ��仯
			WarningCancel(GPRSPORT);//�澯ȡ��,��ڸ澯����
			Terminal_Ram->MODULSEL=i;//GPRSģ��ѡ��0=BGS2,1=,2=,3=��
			UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
		}
		else
		{
			switch(i)
			{
				case 0://����GPRSģ��BGS2
					GPRS_3763_RxTx();//GPRS_3763ģ����շ���;����ӦATָ��
					break;
				case 1://
					//CDMA_MC323_RxTx();//CDMA_MC323ģ����շ���
					GPRS_3763_RxTx();//GPRS_3763ģ����շ���;��ӦATָ��
					break;
				case 2://
					GPRS_3763_RxTx();//GPRS_3763ģ����շ���;��ӦATָ��
					break;
			}
			LINK_request(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);
			//AutoCascadingData(GPRSPORT);//�Զ��ϴ�����Ѳ�⵽������
			//����PPP����
			
		}
	}
//

}
















