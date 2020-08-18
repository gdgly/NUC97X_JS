

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_LINK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"

#include "../DL698/DL698_Uart.h"
#include "../Device/RTC.h"
#include "../Device/ATNo.h"
#include "../Device/GPRS_3763.h"
#include "../MS/Ethernet.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"

//Ԥ����������������ͣ�LINK-Request��
//LINK-Request��=SEQUENCE
//{
//  �������-���ȼ�-ACD  PIID-ACD
//  ��������  ENUMERATED
//{
//    �������� ��0����
//    ����     ��1����
//    �Ͽ����� ��2��
//}��
//  ��������  long-unsigned��
//  ����ʱ��   date_time
//}

void LINK_request(u32 PORTn,u32 ADDR_HeartberatTimer)
{
	u32 i;
	u32 LEN_SA;//��������ַSA�ĳ���
	u32 LEN_Tx;//�ܷ��ͳ���
	u8 * p8tx;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 UARTxTimerAddr;
	u16 * p16timer;
	
	UARTxTimerAddr=Get_ADDR_UARTnTimer(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	p16=(u16 *)(ADDR_HeartberatTimer);//������������붨ʱ��
	if(p16[0]==0x0)
	{
		if(UARTCtrl->Task==0x0)
		{//����
			p16=(u16 *)(UARTxTimerAddr);//UART��ʱ��MS
			if(p16[0]==0x0)
			{
				if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{//����
						if((UARTCtrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
						{//������ģʽ,���趨(��������+60��)��û�յ���ȷ����֡Ϊ����
							if(PORTn==GPRSPORT)
							{
								i=Terminal_Ram->GPRSHeartBeat_S_Timer;//GPRS�����붨ʱ��
							}
							if(PORTn==ENETPORT)
							{
								i=Terminal_Ram->ENETHeartBeat_S_Timer;//��̫�������붨ʱ��
							}
							if(i==0)
							{
								UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							
							if(UARTCtrl->LinkTask==102)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							return;
						}
						
						if(UARTCtrl->HostACK>8)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
						{//ֵ����
							UARTCtrl->HostACK=0;
						}
						if(UARTCtrl->HostACK>=4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
						{//�����������ϱ��ȴ�ȷ�Ϸ���
							return;
						}
						UARTCtrl->HeartberatCount+=1;//������������,�յ�ȷ��֡��0
						if(UARTCtrl->HeartberatCount>3)
						{//����3��û�յ�ȷ��֡
							UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
							if(UARTCtrl->LinkTask==102)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							else
							{
								UARTCtrl->LinkTask=0;//���³�ʼ��
								if(PORTn==GPRSPORT)
								{
									WarningOccur(PORTn,(u8 *)"Զ��������վ����Ӧ");//�澯����,��ڸ澯����͸澯�ַ���
								}
								if(PORTn==ENETPORT)
								{
									WarningOccur(PORTn,(u8 *)"��̫����վ����Ӧ");//�澯����,��ڸ澯����͸澯�ַ���
								}
							}
						}
						else
						{
							UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
							p8tx=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							LEN_Tx=Uart_698_TxHead(p8tx,0x81,0);//Uart698֡ͷ���뷢�ͻ�����,���:C=������,CA=�ͻ�����ַ;����:֡ͷ����
							p8tx[LEN_Tx]=0x01;//[1] LINK-Request
							p8tx[LEN_Tx+1]=0x0;//PIID-ACD
							p8tx[LEN_Tx+2]=1,//�������ͣ�������1��
							LEN_Tx+=3;
						// ��������  long-unsigned��
							if(PORTn==GPRSPORT)
							{
								i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							}
							else
							{
								i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							}
							if(i==0)
							{
								i=120;
								i=(i>>8)+((i&0xff)<<8);
							}
							else
							{
								i=MRR(i+1,2);
							}
							p8tx[LEN_Tx]=i;
							p8tx[LEN_Tx+1]=i>>8;
							
							LEN_Tx+=2;
//����ʱ��  date_time
//date_time��=SEQUENCE
//{
//  year          long-unsigned��
//  month         unsigned��
//  day_of_month  unsigned��
							
//  day_of_week   unsigned��
							
//  hour          unsigned��
//  minute        unsigned��
//  second        unsigned��
//  milliseconds  long-unsigned
//}
							ReadRTCtoDataType();//��RTC��תΪ���������ͺ����뵽RAM����
							p8tx[LEN_Tx+0]=Comm_Ram->DL698YMDHMS[0];//��(2B),��,��,ʱ,��,��,�ٷ���,��
							p8tx[LEN_Tx+1]=Comm_Ram->DL698YMDHMS[1];
							p8tx[LEN_Tx+2]=Comm_Ram->DL698YMDHMS[2];
							p8tx[LEN_Tx+3]=Comm_Ram->DL698YMDHMS[3];
							p8tx[LEN_Tx+4]=Comm_Ram->DL698YMDHMS[9];
							p8tx[LEN_Tx+5]=Comm_Ram->DL698YMDHMS[4];
							p8tx[LEN_Tx+6]=Comm_Ram->DL698YMDHMS[5];
							p8tx[LEN_Tx+7]=Comm_Ram->DL698YMDHMS[6];
							i=Read_STM32F4xx_RTC_SS();//��STM32F4xx�ڲ�RTC������ֵ�����ms
							p8tx[LEN_Tx+8]=i>>8;
							p8tx[LEN_Tx+9]=i;
							LEN_Tx+=10;
							//2byte ֡У��
							LEN_Tx+=1;//2;
							p8tx[1]=LEN_Tx;
							p8tx[2]=LEN_Tx>>8;
							UARTCtrl->TxByte=LEN_Tx+2;
							LEN_SA=p8tx[4];//ȡ��ַ�ֽ���
							LEN_SA&=0x0f;
							LEN_SA++;
							switch(UARTCtrl->LinkTask)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							{
								default://����
									UARTCtrl->HostACK=3;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=1;//�������ͣ�������1��
									break;
								case 100://����
									UARTCtrl->HostACK=1;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=0;//�������ͣ��������ӣ�0��
									break;
								case 102://�˳�
									UARTCtrl->HostACK=2;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
									UARTCtrl->HeartberatCount=100;//�˳�ʱ��������������Ϊ100,�����ض�ʱ��ûȷ�ϻش����������³�ʼ���ر�ģ������
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=2;//�������ͣ��˳����ӣ�2��
									break;
							}
							Uart_698_HCSFCS(p8tx);//��֡У�����,ͬʱ�ӽ���0x16;����0=��ȷ��1=����
							p16=(u16 *)(ADDR_HeartberatTimer);//������������붨ʱ��
							if(PORTn==GPRSPORT)
							{
								i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							}
							else
							{
								i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							}
							if(i==0)
							{
								i=120;
							}
							else
							{
								i=MRR(i+1,2);
								i=(i>>8)+((i&0xff)<<8);
							}
							if(i<WaitHeartberatACKMinTimer)
							{
								i=WaitHeartberatACKMinTimer;//���������ȴ���վȷ����С��ʱ��
							}
							if(i>WaitHeartberatACKMaxTimer)
							{
								i=WaitHeartberatACKMaxTimer;//���������ȴ���վȷ�����ʱ��
							}
							p16[0]=i;//���������ض�ʱ
							
 							p16timer=(u16 *)(Get_ADDR_UARTnTimer(PORTn));
							if(PORTn==GPRSPORT)
							{
							 //����GPRS_3763�еĵȴ���֡���ݷ���
							#if LOG_698==1//0=û,1=��
								LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
							#endif
								Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//�����ֽ���
								UART1Ctrl->TxCount=0;//�ѷ��ͼ���
								Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
								Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2��,GPRS�������ݵȴ���ʱ�붨ʱ��
#if PPP_ENABLE==1
               //Do nothing
#else
								UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
								UART1Ctrl->ATCount=AT_MYNETWRITE;//��������
								UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
#endif
								UART1Ctrl->Task++;
							//Ϊ����ʱ��׼ȷ����������
								GPRS_3763_RxTx();//Զ������GPRSģ����շ���

							}
							if(PORTn==ENETPORT)
							{
							//����Ethernet�еĵȴ���֡���ݷ���
							#if LOG_698==1//0=û,1=��
								LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
							#endif
								UARTCtrl->TxCount=0;
								p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
								UARTCtrl->Task++;
							//Ϊ����ʱ��׼ȷ����������
								Ethernet_RxTx();
							}
						}

				}
			}
		}
	}
}

//Ԥ������Ӧ���������ͣ�LINK-Response��
//LINK-Response��=SEQUENCE
//{
//  �������-���ȼ�  PIID��
//  ���             Result��
//  ����ʱ��         date_time��
//  �յ�ʱ��         date_time��
//  ��Ӧʱ��         date_time
//}
//Result��=bit-string��SIZE��8����
//{
//  ʱ�ӿ��ű�־	��0����
//  ����bit6   		��1����
//  ����bit5     	��2����
//  ����bit4     	��3����
//  ����bit3     	��4����
//  ���bit2     	��5����
//  ���bit1     	��6����
//  ���bit0     	��7��
//}
//ʱ�ӿ��ű�־�������ڱ�ʾ��Ӧ����ʱ���Ƿ���ţ�׼ȷ����0�������ţ�1�����š����bit0�����bit2���������Ʊ����ʾ��0���ɹ���1����ַ�ظ���2���Ƿ��豸��3���������㣬����ֵ��������

void LINK_Response(u32 PORTn)
{
//	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	
	switch(UARTCtrl->LinkTask)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		default://����
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			break;
		case 100://����
			UARTCtrl->LinkTask=101;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			#if (USER/100)==9//����
			if(PORTn==GPRSPORT)
			{
				DOTMIXPMODE->Ln9Code=8;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
			}
			#endif
			break;
		case 102://�˳�
			UARTCtrl->LinkTask=103;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
			UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0			
			UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
			break;
	}
}






