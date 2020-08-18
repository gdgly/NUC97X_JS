
//�ն�Uart����
#include "Project.h"
#include "Terminal_Uart_Manage.h"
#include "UartAuto_645.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#include "Terminal_AFN0E_Event.h"
#include "Terminal_Contrl_Uart.h"
#include "Terminal_Uart_3761.h"
#include "Terminal_Uart_AFN02_Heartbeat.h"
#include "Terminal_Uart_AFN0F_File.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_Uart_3762_Manage.h"

#include "../Device/GPRS_3763.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/Ethernet.h"


	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif

void Link_Contrl(void)//����ͨ��ģ�����߿���
{
//ע:���߿��Ʊ仯ʱ:������������붨ʱ��=0,������������=0,ʹ�˳����¼֡���Ϸ���
	u32 i;
	u8 * p8;
	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;

//�����ն˶Ͽ�����
	i=MRR(ADDR_AFN05F39,1);
	if(i==0xaa)
	{
		i=0;
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
		if(Get_LEN_UARTnCtrl(ENETPORT))//�õ�UART�������ݳ���
		{
			UARTCtrl->LinkCtrl=0;//���ӿ���0=������,1=����
			if(UARTCtrl->LinkTask<101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				i=1;
			}
		}
		else
		{
			i=1;
		}
		if(Get_LEN_UARTnCtrl(GPRSPORT))//�õ�UART�������ݳ���
		{
			UART1Ctrl->LinkCtrl=0;//���ӿ���0=������,1=����
			if(UART1Ctrl->LinkTask<101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				i|=2;
			}
		}
		else
		{
			i|=2;
		}
		if(i==3)
		{
			MC(0,ADDR_AFN05F39,1);
		}
		return;
	}

#if IC_LAN8720==1
//��̫��������������,���������������ģ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
	if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
	{
		UART1Ctrl->LinkCtrl=0;//���ӿ���0=������,1=����
		WarningOccur(GPRSPORT,(u8 *)"��̫���������߹ر�");//�澯����,��ڸ澯����͸澯�ַ���
		return;
	}
#endif

//GPRS����ģ������,��������ģʽ����������ģʽ��ʱ������ģʽ
	if(Get_LEN_UARTnCtrl(GPRSPORT))//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
		//��ͨ������
		MR(ADDR_DATABUFF,ADDR_AFN0CF10,9);
		p32=(u32 *)(ADDR_DATABUFF);
		i=MRR((u32)&UARTCtrl->RxTxByte,4);//ͨ������
		if(i!=0x0)
		{
			MWR(0,(u32)&UARTCtrl->RxTxByte,4);//ͨ������
			p32[0]+=i;
			p32[1]+=i;
			MW(ADDR_DATABUFF,ADDR_AFN0CF10,8);
		}
		i=MRR(ADDR_AFN04F36,4);
		if(i!=0x0)
		{//��ͨ����������Ϊ0����ʾϵͳ����Ҫ�ն˽�����������
			p8=(u8 *)(ADDR_DATABUFF);
			if(p32[1]<i)
			{//������û����
				//WarningCancel(GPRSPORT);//�澯ȡ��,��ڸ澯����
				if(p8[8]!=0x0)
				{
					MC(0,ADDR_AFN0CF10+8,1);
				}
			}
			else
			{//����������
				//WarningOccur(GPRSPORT,(u8 *)"������ͨ����������");//�澯����,��ڸ澯����͸澯�ַ���
				if(p8[8]==0x0)
				{//�¼���¼��־,0û��¼,1=�Ѽ�¼
					if(ERC_Event_Set(32)!=0)//����¼���¼����,����0=����¼,1=��¼
					{
						MC(1,ADDR_AFN0CF10+8,1);
					}
					ERC32_Event(p32[1],i);//ͨ�������������¼�,n1=�����ѷ�����ͨ������,n2=��ͨ����������
				}
				
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
				{
					MR(ADDR_DATABUFF,ADDR_AFN0CF7_FM+2,1);//����ǰ��û�ϴ���ȷ����Ҫ�¼�����
					if(p8[0]==0x0)
					{//����Ҫ�¼�û�ϱ�,���ϱ���ɺ�Ͽ�
						UARTCtrl->LinkCtrl=0;//���ӿ���0=������,1=����
//						UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
//						Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
					}
				}
				else
				{
					UARTCtrl->LinkCtrl=0;//���ӿ���0=������,1=����
//					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
//					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
				}
				return;
			}
		}
		//��������
		i=MRR(ADDR_AFN04F8,1);
		i&=0x3;
		if(i==2)
		{//1��3���α�ʾ��������ģʽ����������ģʽ��ʱ������ģʽ
			i=MRR(ADDR_AFN05F38,1);
			if(i==0x55)
			{//�յ�������������,�ó�ֵ�ز������������Ϊ0x5a
				MC(0x5A,ADDR_AFN05F38,1);
				i=MRR(ADDR_AFN04F8+3,1);
				if(i==0)
				{
					i=1;//��������ģʽ�ز�������СΪ1��
				}
				UARTCtrl->ReLinkNum=i;//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
				return;
			}
			if(i==0x5A)
			{//0x55=�����ն�������վ,0xAA=�����ն˶Ͽ�����,ִ�к���0
				WarningCancel(GPRSPORT);//�澯ȡ��,��ڸ澯����
				UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
				{
					if(Terminal_Ram->GPRSAutoDiscon_S_Timer==0)//4 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
					{
						MC(0,ADDR_AFN05F38,1);//������ͨ���Զ����������������0x55(0x5a)
					}
				}
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)"Զ�����ߵȴ���������");//�澯����,��ڸ澯����͸澯�ַ���
				UARTCtrl->LinkCtrl=0;//���ӿ���0=������,1=����
//				UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
//				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
			}
			return;
		}
		//ʱ������
		if(i==3)
		{//1��3���α�ʾ��������ģʽ����������ģʽ��ʱ������ģʽ
			p8=(u8 *)(ADDR_TYMDHMS+2);
			i=MRR(ADDR_AFN04F8+5,3);
			i>>=bcd_hex(p8[0]);//8λѹ��BCD��תΪHEX
			i&=0x1;
			if(i==0x0)
			{//��ǰʱ�β�����
				WarningOccur(GPRSPORT,(u8 *)"���ߵ�ǰʱ�β�����");//�澯����,��ڸ澯����͸澯�ַ���
				UARTCtrl->LinkCtrl=0;//���ӿ���0=������,1=����
//				UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
//				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
			}
			else
			{//��ǰʱ������
				UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
			}
			return;
		}
		//��������
		UARTCtrl->LinkCtrl=1;//���ӿ���0=������,1=����
	}
	
}



//��ʼ��
#if (((Project/100)==0)||((Project/100)==1))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ�� 
void Terminal_UartInit_Manage(void)//Terminal_Uart��ʼ������
{
}
#endif
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ�� 
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=����ģ��
//UART5=
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
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
//����
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
//����ģ��(ר�ó�ʼ��)
	
}
#endif
#if (((Project/100)==2)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ�� 
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
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
#endif
#if ((Project/100)==5)||((Project/100)==6)//�ɼ���,ͨ��ģ��
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=����
//UART5=
//UART6=
//UART7=�ն�ESAMͨ�ſں�
//UART8=
//UART9=
//UART10=��̫��ͨ�ſں�
//UART11=����ͨ�ſں�
//UART12=
void Terminal_UartInit_Manage(void)//Terminal_Uart��ʼ������
{
  UARTCtrl_TypeDef * UARTCtrl;
//RS232
	if(Get_LEN_UARTnCtrl(RS232PORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
		UARTCtrl->BpsCtrl=RS232DefaultBPS;//RS232��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
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
//����
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//�õ�UART�������ݳ���
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl=IRDefaultBPS;//�����Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	  Init_UARTn(IRPORT);//��ʼ��UART��,���ͨ�ſ�����������
	}
}
#endif






//���շ���
//���ܱ�,�����----------------------------------------------------------------------------------
#if ((Project/100)==0)//���ܱ�
void Terminal_Uart_Manage(void)//Terminal_Uart����
{
}
#endif
#if ((Project/100)==1)//�����
void Terminal_Uart_Manage(void)//Terminal_Uart����
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
	//�ļ�����
		Request_File();//�����ļ�����
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761���շ���
			UartAuto_645(RS232PORT);//�Զ�ʶ��DL_645֡
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761���շ���
			UartAuto_645(IRPORT);//�Զ�ʶ��DL_645֡
		}
	//RS485-1
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
	  if(Comm_Ram->Factory==0x55)//0x55=����ģʽ
	  {
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
				UartAuto_645(RS485_1PORT);//�Զ�ʶ��DL_645֡
			}
		}
		UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
//	//RS485-2����
		UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
		UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
//		ReadMeter_RS485(RS485_2PORT);//RS485-2����
	//��������ͨ�ſں�(����)����
		ReadMeter_RS485(ACSAMPLEPORT);
	}
	
//����ͨ��ģ�����߿���
	Link_Contrl();//����ͨ��ģ�����߿���
//�����ϱ���Ҫ�¼�
	Terminal_Uart_AutoEvent();//�����ϱ���Ҫ�¼�
//�����ϱ���ʱ����
	Terminal_Uart_AutoTask();//�����ϱ���ʱ����
//ENET
#if IC_W5200==1
	W5200_3761_RxTx(ENETPORT);//W5200���շ���
	W5200_FTP(ENETPORT);//W5200 FTP�ļ�����
	Uart_3761_RxTx(ENETPORT);//UART_3761���շ���
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//��������(��·�ӿڼ��)
#endif
//GPRS
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	i=Pin_Read(PIN_MODULSEL0);//������,�������ʹ����;����0��1
	i|=Pin_Read(PIN_MODULSEL1)<<1;//������,�������ʹ����;����0��1
	if(i==3)
	{//GPRSģ���û����
		//��ģ���Դ��IO����
		All_GPRS_3763_PinIn();//ȫ��GPRSģ������Ϊ����������
		Terminal_Ram->MODULSEL=i;//GPRSģ��ѡ��0=BGS2,1=,2=,3=��
		Terminal_Ram->Signal=0;//GPRS�ź�ǿ��0-4
		UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�ȴ��˳���¼
		WarningOccur(GPRSPORT,(u8 *)"Զ������ģ��û����");//�澯����,��ڸ澯����͸澯�ַ���
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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//��������(��·�ӿڼ��)
		}
	}
//

}
#endif
//���ܱ�,��������------------------------------------------------------------------------------



//ר���ն�,�ɼ���,ͨ��ģ��---------------------------------------------------------
#if (((Project/100)==3)||((Project/100)==5)||((Project/100)==6))//ר���ն�,�ɼ���,ͨ��ģ��
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=����ģ��
//UART5=
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

	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
	//�ļ�����
		Request_File();//�����ļ�����
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761���շ���
			UartAuto_645(RS232PORT);//�Զ�ʶ��DL_645֡
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761���շ���
			UartAuto_645(IRPORT);//�Զ�ʶ��DL_645֡
		}
	//RS485-1����
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
	  if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//0x55=����ģʽ
	  {
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
				Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
			}
			UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			WarningOccur(RS485_1PORT,(u8 *)"485\x1�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
				UartAuto_645(RS485_1PORT);//�Զ�ʶ��DL_645֡
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_1PORT);//RS485-1����
		}
	//RS485-2����
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
	  if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)//�ն�ά��
	  {
		#if (USER/100)==5//�û���ʶ:�Ϻ���
			if(MRR(ADDR_TerminalFMTimer+42,2)==0)//2 RS485-2����ת���ֶ�ʱ��
			{
				MWR(0xff,ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1);
			}
		#endif
		#if (USER/100)==10//�ӱ�ϵ
			//�ӱ�ά��������Ϊ9600
			if(Comm_Ram->SelfTestCommand!=0x55555555)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
			{
				if(UARTCtrl->BpsCtrl!=(0xb+(6<<5)))//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
				{
					UARTCtrl->BpsCtrl=(0xb+(6<<5));//RS485��Ĭ�ϵ����ʿ�����
					Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
				}
			}
		#else
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
				Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
			}
		#endif
			UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			WarningOccur(RS485_2PORT,(u8 *)"485\x2�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
				UartAuto_645(RS485_2PORT);//�Զ�ʶ��DL_645֡
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_2PORT);//RS485-2����
		}
	//��������ͨ�ſں�(����)����
		ReadMeter_RS485(ACSAMPLEPORT);
#if ((Project/100)==3)//ר���ն�,�ɼ���,ͨ��ģ��
	//����ģ��
		Terminal_Contrl_Uart_RxTx(RS485_4PORT);//����ģ��ͨ��RxTx
#endif
	}
	
//����ͨ��ģ�����߿���
	Link_Contrl();//����ͨ��ģ�����߿���
//�����ϱ���Ҫ�¼�
	Terminal_Uart_AutoEvent();//�����ϱ���Ҫ�¼�
//�����ϱ���ʱ����
	Terminal_Uart_AutoTask();//�����ϱ���ʱ����
//ENET
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP�ļ�����
	Uart_3761_RxTx(ENETPORT);//UART_3761���շ���
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//��������(��·�ӿڼ��)
	//AutoCascadingData(ENETPORT);//�Զ��ϴ�����Ѳ�⵽������
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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//��������(��·�ӿڼ��)
			//AutoCascadingData(GPRSPORT);//�Զ��ϴ�����Ѳ�⵽������
		}
	}
//

}
#endif
//ר���ն�,�ɼ���,ͨ��ģ��---------------------------------------------------------------




//������,�����ն�-------------------------------------------------------------------------------
#if (((Project/100)==2)||((Project/100)==4))//������,�����ն�
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

	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
	//�ļ�����
		Request_File();//�����ļ�����
	//RS232
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(RS232PORT);//UART_3761���շ���
			UartAuto_645(RS232PORT);//�Զ�ʶ��DL_645֡
		}
	//IR
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(IRPORT));
		if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
		{
			Uart_3761_RxTx(IRPORT);//UART_3761���շ���
			UartAuto_645(IRPORT);//�Զ�ʶ��DL_645֡
		}
	//RS485-1����
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_1PORT));
		i=MRR(ADDR_AFN04F37,1);//�ն˼���ͨ�Ŷ˿ں� BIN  1 
		i|=MRR(ADDR_AFN04F37+6,1)<<8;//����/��������־
	  if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//�ն�ά��
	  {
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
				Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
			}
			UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			WarningOccur(RS485_1PORT,(u8 *)"485\x1�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->RMComplete|=0X80;//
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
				UartAuto_645(RS485_1PORT);//�Զ�ʶ��DL_645֡
			}
		}
		else
		{
//			if((i&0xff)==2)
//			{//������
//				if((i&0x8000)!=0)
//				{//��������
//					x=MRR(ADDR_AFN04F37+1,1);
//					if(UARTCtrl->BpsCtrl!=x)
//					{
//						UARTCtrl->BpsCtrl=x;
//						Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
//					}
//					Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
//					if(UARTCtrl->Task==1)
//					{//1=�������
//						Terminal_Ram->CD485RxLED_MS_Timer=100/10;//����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//					}
//					if(UARTCtrl->Task==2)
//					{//2=�ȴ���֡���ݷ���
//						Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//					}
//				}
//				else
//				{//��������
//					Cascading_LoopChack(RS485_1PORT);//��������Ѳ��
//				}
//			}
//			else
//			{//�Ǽ�����
				ReadMeter_RS485(RS485_1PORT);//RS485-1����
//			}
		}
	//RS485-2����
	  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
	  if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)//�ն�ά��
	  {
	#if (USER/100)==5//�û���ʶ:�Ϻ���
			if(MRR(ADDR_TerminalFMTimer+42,2)==0)//2 RS485-2����ת���ֶ�ʱ��
			{
				MWR(0xff,ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1);
			}
	#endif
			if(UARTCtrl->BpsCtrl!=RS485DefaultBPS)
			{
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����
				Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
			}
			UARTCtrl->RMComplete=0x80;//D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			WarningOccur(RS485_2PORT,(u8 *)"485\x2�ն�ά��");//�澯����,��ڸ澯����͸澯�ַ���
			if(UARTCtrl->Protocol==0)//ͨ�Ź�Լ�Զ�ʶ��:0=376.1,1=DL/T645-2007
			{
				Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
				UartAuto_645(RS485_2PORT);//�Զ�ʶ��DL_645֡
			}
		}
		else
	  {
			ReadMeter_RS485(RS485_2PORT);//RS485-2����
		}
//		if((i&0xff)==3)
//		{//������
//			if((i&0x8000)!=0)
//			{//��������
//				x=MRR(ADDR_AFN04F37+1,1);
//				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_2PORT));
//				if(UARTCtrl->BpsCtrl!=x)
//				{
//					UARTCtrl->BpsCtrl=x;
//					Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
//				}
//				Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
//				if(UARTCtrl->Task==1)
//				{//1=�������
//					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//				}
//				if(UARTCtrl->Task==2)
//				{//2=�ȴ���֡���ݷ���
//					Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//				}
//			}
//			else
//			{//��������
//				Cascading_LoopChack(RS485_2PORT);//��������Ѳ��
//			}
//		}
//		else
//		{//�Ǽ�����
//			ReadMeter_RS485(RS485_2PORT);//RS485-2����
//		}
	//RS485-3
//		if((i&0xff)==4)
//		{//������
//			if((i&0x8000)!=0)
//			{//��������
//				x=MRR(ADDR_AFN04F37+1,1);
//				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS485_3PORT));
//				if(UARTCtrl->BpsCtrl!=x)
//				{
//					UARTCtrl->BpsCtrl=x;
//					Init_UARTn(RS485_3PORT);//��ʼ��UART��,���ͨ�ſ�����������
//				}
//
//				Uart_3761_RxTx(RS485_3PORT);//UART_3761���շ���
//				if(UARTCtrl->Task==1)
//				{//1=�������
//					Terminal_Ram->CD485RxLED_MS_Timer=100/10;//����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//				}
//				if(UARTCtrl->Task==2)
//				{//2=�ȴ���֡���ݷ���
//					Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 ����485����LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
//				}
//			}
//			else
//			{//��������
//				Cascading_LoopChack(RS485_3PORT);//��������Ѳ��
//			}
//		}
//		else
//		{//�Ǽ�����
//			ReadMeter_RS485(RS485_3PORT);//RS485-3����
//		}
	//�ز�����
#if ((Project/100)==2)//������
		Terminal_uart_3762_Manage();//376.2·��ģ�����
#endif
	//��������ͨ�ſں�(����)����
		ReadMeter_RS485(ACSAMPLEPORT);
	}
	
//����ͨ��ģ�����߿���
	Link_Contrl();//����ͨ��ģ�����߿���
//�����ϱ���Ҫ�¼�
	Terminal_Uart_AutoEvent();//�����ϱ���Ҫ�¼�
//�����ϱ���ʱ����
	Terminal_Uart_AutoTask();//�����ϱ���ʱ����
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
	//��ȷ��ʱ
	if(MRR(ADDR_AFN04F297_Ext,1)!=0)
	{//�ն˾�ȷ��ʱ���ؿ���
		i=MRR(ADDR_AFN04F297_Ext+2,2);
		i<<=8;
		i=Check_HMS(i);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
		if(i==0)
		{//ʱ�ӺϷ�
			i=DataComp(ADDR_AFN04F297_Ext+2,ADDR_TYMDHMS+1,2);
			if((i==0)||(i==2))//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{
				MWR(0,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
				if(MRR(ADDR_AFN04F297_Ext+7,1))
				{//�����ʱ������Ϊ0
					i=MRR(ADDR_AFN04F299_Ext+28,1);
					if(i==0)
					{
						if(MRR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,6)==0)
						{
							//��ȷ��ʱ��ʼ
							i=0x80;
							MWR(i,ADDR_AFN04F299_Ext+28,1);
						}
						else
						{
							MC(0,ADDR_DATABUFF,16);
							MR(ADDR_DATABUFF+3,ADDR_AFN04F299_Ext+28+1+4*25+4+1+3,3);
							MR(ADDR_DATABUFF+9,ADDR_TYMDHMS+3,3);
							p8=(u8 *)(ADDR_DATABUFF);
							p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
							p8[10]&=0x1f;//ȥ�������ֽ��ϵ�����
							i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							if(i==0)
							{//ʱ�ӺϷ�
								i=TimeDifference((u32)ADDR_DATABUFF,(u32)(ADDR_DATABUFF+6));//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
								//�뻯Ϊ��
								i/=60;
								i/=60;
								i/=24;
								if(i>=MRR(ADDR_AFN04F297_Ext+1,1))
								{
									//��ȷ��ʱ��ʼ
									i=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28,1);
								}
							}
						}
					}
				}
			}
		}
		i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
		if(i&0x7f)
		{//�Թ�ʱ
			if((i&0x80)==0)
			{//ʧ��
				if(MRR(ADDR_AFN04F297_Ext+7,1)>(i&0x7f))
				{//�����ʱ����
					MR(ADDR_DATABUFF,(u32)(ADDR_AFN04F299_Ext+28+1+4*25+4+1),6);
					MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
					p8=(u8 *)ADDR_DATABUFF;
					p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
					p8[10]&=0x1f;//ȥ�������ֽ��ϵ�����
					i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
					if(i==0)
					{//ʱ�ӺϷ�
						i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
						if(i>=(MRR(ADDR_AFN04F297_Ext+6,1)*60))
						{
							i=MRR(ADDR_AFN04F297_Ext+4,2);
							i<<=8;
							i=Check_HMS(i);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							if(i==0)
							{//ʱ�ӺϷ�
								if(DataComp(ADDR_AFN04F297_Ext+4,ADDR_TYMDHMS+1,2)==1)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
								{
									//��ȷ��ʱ��ʼ
									i=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28,1);
									i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
									i|=0x80;
									MWR(i,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
								}
							}
						}
					}
				}
			}
		}
		i=MRR(ADDR_AFN04F299_Ext+28,1);
		if(i&0x80)
		{
			if(Terminal_Ram->PowerTask)//ͣ���¼�����
			{
				if((i&0x7f)<=24)
				{
					Terminal_AutoAFN0AF299(GPRSPORT);//�������㽭��չAFN0AF299�ն�������վʱ��
					Terminal_AutoAFN0AF299(ENETPORT);//�������㽭��չAFN0AF299�ն�������վʱ��
				}
				else
				{
					MWR(i,ADDR_AFN04F299_Ext+28,1);
				}
			}
		}
	}
	else
	{
		MWR(0,ADDR_AFN04F299_Ext+28,1);
		MWR(0,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
	}
#endif
	
//ENET
	Ethernet_RxTx();
	//W5200_FTP(ENETPORT);//W5200 FTP�ļ�����
	Uart_3761_RxTx(ENETPORT);//UART_3761���շ���
	Heartberat(ENETPORT,(u32)&Terminal_Ram->ENETHeartBeat_S_Timer);//��������(��·�ӿڼ��)
	//AutoCascadingData(ENETPORT);//�Զ��ϴ�����Ѳ�⵽������

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
			Heartberat(GPRSPORT,(u32)&Terminal_Ram->GPRSHeartBeat_S_Timer);//��������(��·�ӿڼ��)
			//AutoCascadingData(GPRSPORT);//�Զ��ϴ�����Ѳ�⵽������
		}
	}
//

}
#endif
//������,�����ն˽���---------------------------------------------------------------------------















