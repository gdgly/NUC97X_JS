
//ȷ��/����
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"




void Uart_AFN00(u32 PORTn)//ȷ��/����
{
#if (USER/100)==5//�Ϻ�
	u32 i;
#endif
  UARTCtrl_TypeDef * UARTCtrl;
//	u8 * p8rxbuff;
//	u8 * p8txbuff;


  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	switch(UARTCtrl->HostACK)//������������վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
	{
		case 1://1=��¼֡
			UARTCtrl->LinkTask=101;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
//			if(PORTn==GPRSPORT)
//			{
//				Comm_Ram->SelfTestFlags|=1<<11;//B11=GPRS��¼(�Զ�,��������IP�Ȳ���)
//			}
//			if(PORTn==ENETPORT)
//			{
//				Comm_Ram->SelfTestFlags|=1<<17;//B17=��̫����¼(�Զ�,��������IP�Ȳ���)
//			}
		#if (USER/100)==5//�Ϻ�
			//F241	��ǰ��¼��վ��Ϣ�����һ�Σ�
			MW(ADDR_AFN0CF2,ADDR_AFN0CF241,6);
			//�¶���F242	�¶����¼��վ����
			i=MRR(ADDR_AFN0CF242+4,4);
			i++;
			MWR(i,ADDR_AFN0CF242+4,4);
			//�¶���F241	�ն����¼��վ����
			i=MRR(ADDR_AFN0CF242,4);
			i++;
			if(i>AFN0CF242MaxCount)//���յ�¼��վ��Ϣ����¼����
			{
				i=AFN0CF242MaxCount;
			}
			//F242	���յ�¼��վ��Ϣ
			DMW(ADDR_AFN0CF2,ADDR_AFN0CF242_Next+(6*(i-1)),6);
			MWR(i,ADDR_AFN0CF242,4);
		#endif
			break;
		case 2://2=�˳���¼֡
			UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			break;
		case 3://3=����֡
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			break;
		case 4://4=��Ҫ�¼�
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			UARTCtrl->HostCount=0;//�����ϱ��ط�����(��������������)
			if(PORTn==GPRSPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�			
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+5,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#else
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#endif			
			if(PORTn==RS232PORT)
			{
				MC(0,ADDR_AFN0CF7_FM+7,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
			break;
		case 5://5=��ʱ����
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			UARTCtrl->HostCount=0;//�����ϱ��ط�����(��������������)
			break;
		case 6://6=���ܱ��¼�
//		  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
//		  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
//			if((p8txbuff[13]&0xf)!=(p8rxbuff[13]&0xf))
//			{//֡������SEQ��ͬ
//				break;
//			}
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			UARTCtrl->HostCount=0;//�����ϱ��ط�����(��������������)
			if(PORTn==GPRSPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�			
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+5,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#else
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+3,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
#endif			
			if(PORTn==RS232PORT)
			{
				MC(0,ADDR_MeterEventCount_FM+7,1);//�嵱ǰ��?����Ҫ�¼������ϴ���ȷ��
			}
			break;
	}
}




