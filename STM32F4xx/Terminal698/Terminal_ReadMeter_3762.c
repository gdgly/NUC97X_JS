
#include "Project.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_ReadMeter_3762_Std.h"
#include "Terminal_ReadMeter_3762_NoRouter.h"
#include "AutoSearchMeter.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_RouterUpDate.h"




#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void DisplayRouterVer(u32 PORTn)//��ʾ·�ɳ��ҺͰ汾��
{
	u32 i;
	u8*p8;
	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
	{
		case 1:
			CopyString((u8 *)"��̩�ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 2:
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 3:
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 4:
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 5:
			CopyString((u8 *)"��˹��  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 6:
			CopyString((u8 *)"����΢  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 7:
			CopyString((u8 *)"Ѷ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 8:
			CopyString((u8 *)"Ǯ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 9:
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
			
		case 101:
			CopyString((u8 *)"��ϣ�ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 102:
			CopyString((u8 *)"��Ѷ��  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 103:
			CopyString((u8 *)"�з��ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case 104:
			CopyString((u8 *)"ɣ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
			
		default:
			WarningOccur(PORTn,(u8 *)"δ֪�ز�");//�澯����,��ڸ澯����͸澯�ַ���
			return;
	}
	p8=(u8 *)(ADDR_DATABUFF+8);
	p8[0]=(Terminal_Router->RouterVer[2]>>4)+0x30;
	p8[1]=(Terminal_Router->RouterVer[2]&0xf)+0x30;
	p8[2]=(Terminal_Router->RouterVer[1]>>4)+0x30;
	p8[3]=(Terminal_Router->RouterVer[1]&0xf)+0x30;
	p8[4]=(Terminal_Router->RouterVer[0]>>4)+0x30;
	p8[5]=(Terminal_Router->RouterVer[0]&0xf)+0x30;
	p8[6]='V';
	i=(Terminal_Router->RouterVer[4]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[7]=i;
	i=(Terminal_Router->RouterVer[4]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[8]=i;
	i=(Terminal_Router->RouterVer[3]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[9]=i;
	i=(Terminal_Router->RouterVer[3]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[10]=i;
	p8[11]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
}


void Terminal_ReadMeter_3762(u32 PORTn)//�ն˳���Q/GDW376.2
{
	u32 i;
//	u32 x;
//	u32 y;
//	u32 z;
#if (USER/100)!=11//���Ǽ���ϵ
	u64 d64a;
#endif
//	u64 d64b;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
//	u32 * p32;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	if(UARTCtrl->Task>5)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{//�ݴ�
		UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	}
	switch(Terminal_Router->RouterInitTask)
	{//ģ��״̬
		case 0://�ϵ��λ(�ص�Դ)
			Pin_Configure(PIN_UART4_RX,//RXD����,����
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			Pin_Configure(PIN_UART4_TX,//TXD����,����
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			Pin_Configure(PIN_PLC_RST,//��λ�������0
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//ʱ��
			if(i<0x0010)
			{//��С��0ʱ10��,�ص�Դ
				Pin_Configure(PIN_PLC_PWR,//��λ�������0
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				p16timer[0]=2000/10;//��ʱ(�ص�Դ)
			}
			else
			{//���ص�Դ
				p16timer[0]=0;//�رյ�Դ��ʱ��
			}
			Pin_Configure(PIN_PLC_ID,//ģ���������,��������
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			if(Pin_Read(PIN_PLC_ID)==0)
			{
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			else
			{
				WarningOccur(PORTn,(u8 *)"�ز�ģ��û����");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 1://����Դ
			if(p16timer[0]==0)
			{
				Pin_Configure(PIN_PLC_RST,//��λ�������1
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1		
				Pin_Configure(PIN_PLC_PWR,//��λ�������0
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				p16timer[0]=1000/10;//ģ�����г�ʼ����ʱ
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 2://��λ
			if(p16timer[0]==0)
			{
				Pin_Configure(PIN_PLC_RST,//��λ�������0
										0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				p16timer[0]=500/10;//ģ�����г�ʼ����ʱ
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 3://��λ���
			if(p16timer[0]==0)
			{
				p16timer=(u16 *)Get_ADDR_UARTn(PORTn);
				p16timer[0]=0;//�жϽ���ָ��
				p16timer[1]=0;//��ǰ�Ѷ�ָ��
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
				UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				Terminal_Router->RUNState=0;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
				Terminal_Router->StopFlags=0;//��ͣ·�ɱ�־
				Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
				Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				Terminal_Router->AFN04F10=0;//AFN04F10���ñ仯��־0=û,1=�仯
				UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
				Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
				Pin_OutH(PIN_PLC_RST);//���������,�������ʹ����
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 4://ģ�����г�ʼ����ʱ
		#if (USER/100)==0
			Terminal_Ram->RouterCheck_S_Timer=60;//60S,�ȴ�·�ɿ����Զ��ϱ�֡
		#else
			Terminal_Ram->RouterCheck_S_Timer=10;//10S,�ȴ�·�ɿ����Զ��ϱ�֡
		#endif
			Terminal_Router->RouterInitSubTask=0;//������
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 5://�ȴ�·�ɿ����Զ��ϱ�֡
			switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 1://1=���1֡����
					Terminal_Router->RouterInitSubTask=0;//������
					Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
				case 0://0=����
				case 2://2=���ճ�ʱ(�����)
					if(Terminal_Ram->RouterCheck_S_Timer!=0)//24 �ز�·���������붨ʱ��
					{
						UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
						break;
					}
					Terminal_Router->RouterInitSubTask=0;//������
					Terminal_Router->RouterInitTask+=2;//����ȷ��֡
					break;
			}
			break;
		case 6://��ȷ��֡(�����ϱ����̴������Ӧ,��ȷ��ֹ֡ͣ)
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					i=p8rxbuff[13];
					Terminal_Router->RouterRunMode_1=i&0x0f;//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
					MWR(0xffffffff,((u32)p8txbuff)+128,6);
					Uart_3762_TxData(PORTn,0,1,6);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://�ȴ����
					if((UARTCtrl->Task==3)||(UARTCtrl->Task==4))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						break;
					}
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x030A)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//��ȷ�յ�AFN3F10�ϱ�֡
						Terminal_Router->RouterInitTask+=2;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						Terminal_Router->RouterInitSubTask=3;//������
						UARTCtrl->Task=1;//1=���1֡����
						break;
					}
					Terminal_Router->RouterInitSubTask=0;//������
					Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 7://
			//����ģ�鸴λ����Ӧ��1֡��3������,�ʼ���3����ʱ
			p16timer[0]=3000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 8://������ģ������ģʽ
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					if(p16timer[0]==0)//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					{
						Terminal_Router->RouterInitSubTask++;
					}
					break;
				case 1://
					if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						if(UARTCtrl->Task!=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{//�ϵ������ѽӵ���֡�ϱ�,�����֡
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						else
						{
							Terminal_Router->RouterInitSubTask++;
						}
					}
					break;
				case 2:
					Uart_3762_TxData(PORTn,3,10,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;
					break;
				case 3:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x030A)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								break;
							}
//							Comm_Ram->SelfTestFlags|=(1<<16);//B16=�ز�ģ��(�Զ�)
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							i=p8rxbuff[13+30]+(p8rxbuff[13+31]<<8);
							switch(i)
							{
							//������
								case (('5'<<8)+'0')://��̩
									Comm_Ram->RouterType=1;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('T'<<8)+'C')://����
									Comm_Ram->RouterType=2;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('E'<<8)+'S')://����
									Comm_Ram->RouterType=3;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('C'<<8)+'X')://����
									Comm_Ram->RouterType=4;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('0'<<8)+'1')://��˹��
									Comm_Ram->RouterType=5;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('L'<<8)+'H')://����΢
									Comm_Ram->RouterType=6;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
								case (('N'<<8)+'X')://Ѷ��
									Comm_Ram->RouterType=7;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��
									break;
								case (('Q'<<8)+'L')://Ǯ��
									Comm_Ram->RouterType=8;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��
									break;
								case (('R'<<8)+'H')://����
									Comm_Ram->RouterType=9;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��
									break;
									
							//С����
								case (('M'<<8)+'X')://��ϣ
									Comm_Ram->RouterType=101;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
									break;
								case (('F'<<8)+'C')://��Ѷ��
									Comm_Ram->RouterType=102;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
									break;
								case (('N'<<8)+'W')://�з�
									Comm_Ram->RouterType=103;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
									break;
								case (('S'<<8)+'R')://ɣ��
									Comm_Ram->RouterType=104;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
									break;
									
								default:
									Comm_Ram->RouterType=0;//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
									break;
							}
							
							i=p8rxbuff[13];
							Terminal_Router->RouterRunMode_1=i&0x0f;//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							Terminal_Router->RouterRunMode_2=(i>>4)&0x1;//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
							if(Comm_Ram->RouterType==7)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��
							{//Ѷ�ܴ�������
								Terminal_Router->RouterRunMode_2=0;//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
							}
							Terminal_Router->RouterRunMode_3=(i>>5)&0x1;//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
							Terminal_Router->RouterRunMode_4=(i>>6)&0x3;//·������ģʽ_���ڳ���ģʽ��01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
							i=p8rxbuff[13+1];
							Terminal_Router->RouterRunMode_5=i&0x07;//·������ģʽ_������ʱ����֧�֣���D2��D1��D0������λ��ʾ���ֱ����㲥���ӽڵ��ء�·�������������������֧���������ṩ������ʱ�����������1��ʾ֧���ṩ������ʱ������0��ʾ��֧��
							Terminal_Router->RouterRunMode_6=(i>>3)&0x3;//·������ģʽ_ʧ�ܽڵ��л�����ʽ��D0��1��ʾͨ��ģ�������л������ڵ㣬D1��1��ʾ����������֪ͨͨ��ģ���л������ڵ�
							Terminal_Router->RouterRunMode_7=(i>>5)&0x1;//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
							Terminal_Router->RouterRunMode_8=(i>>6)&0x3;//·������ģʽ_�㲥�����ŵ�ִ�з�ʽ��0��ʾִ�й㲥�����Ҫ�ŵ���ʶ��1��ʾ�㲥����Ҫ���ݱ����е��ŵ���ʶ��������ͣ�����ȡֵ����
							i=p8rxbuff[13+6];
							Terminal_Router->MaxOverTime=i;//�ӽڵ������ʱʱ����
							
							i=p8rxbuff[13+7]+(p8rxbuff[13+8]<<8);
							Terminal_Router->MaxBroadcastOverTime=i;//���㲥��ʱʱ����
							i=p8rxbuff[13+9]+(p8rxbuff[13+10]<<8);
							Terminal_Router->MaxTxByte=i;//���֧�ֱ��ĳ���(���ֶ���)
							i=p8rxbuff[13+11]+(p8rxbuff[13+12]<<8);
							Terminal_Router->MaxFileByte=i;//�ļ�����֧�ֵ���󵥸����ݰ�����(���ֶ���)
							i=p8rxbuff[13+13];
							Terminal_Router->MaxUpDateTime=i;//�����������ȴ�ʱ����(���ֶ���)
							i=p8rxbuff[13+20]+(p8rxbuff[13+21]<<8);
							if(Comm_Ram->RouterType==102)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
							{//��Ѷ��֧�ֵ����ӽڵ���-1
								i--;
							}
							Terminal_Router->MaxNoteNum=i;//֧�ֵ����ӽڵ���(���ֶ���)
							MR((u32)&Terminal_Router->RouterCODE,((u32)p8rxbuff)+13+30,4);//���̴��� ASCII 2,оƬ���� ASCII 2
							Data_Inverse((u32)&Terminal_Router->RouterCODE,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
							Data_Inverse(((u32)&Terminal_Router->RouterCODE)+2,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
							MR((u32)&Terminal_Router->RouterVer,((u32)p8rxbuff)+13+34,5);//·�����汾��,�ֱ�Ϊ:3BYTE������,2BYTE�汾

							DisplayRouterVer(PORTn);//��ʾ·�ɳ��ҺͰ汾��
							Terminal_Router->RouterInitReCount=0;//�����ط�����
							Terminal_Router->RouterInitSubTask=0;//������
							Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Uart_3762_TxData(PORTn,3,10,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
								return;
							}
							WarningOccur(PORTn,(u8 *)"û��⵽�ز�ģ��");//�澯����,��ڸ澯����͸澯�ַ���
//							Comm_Ram->SelfTestFlags&=~(1<<16);////B16=�ز�ģ��,(��λ��ʾOK)
							Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 9://�����ڵ��ַ
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://����
					Uart_3762_TxData(PORTn,0x03,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1://����
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x0304)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=03H,Fn=4
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								break;
							}
						#if (USER/100)==11//����ϵ
							MR((u32)&Terminal_Router->RouterAddr,ADDR_AREACODE,2);//������������
							MR(((u32)&Terminal_Router->RouterAddr)+2,ADDR_04000401,4);//�����ַ��Ϊ�ն˵�ַ
						#else
							d64a=MRR(ADDR_TERMINAL,2);
							d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
							MWR(d64a,(u32)&Terminal_Router->RouterAddr,4);
							MR(((u32)&Terminal_Router->RouterAddr)+4,ADDR_AREACODE,2);
						#endif
							i=DataComp((u32)&Terminal_Router->RouterAddr,((u32)p8rxbuff)+13,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//��ַ��ͬ��������
								Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							Terminal_Router->RouterInitReCount=0;//�����ط�����
							Terminal_Router->RouterInitSubTask=0;//������
							Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Uart_3762_TxData(PORTn,0x03,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
								return;
							}
							Terminal_Router->RouterInitReCount=0;//�����ط�����
							Terminal_Router->RouterInitSubTask=0;//������
							Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 10://�������ڵ��ַ(����������+�ն˵�ַ)
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
				#if (USER/100)==11//����ϵ
					MR((u32)&Terminal_Router->RouterAddr,ADDR_AREACODE,2);//������������
					MR(((u32)&Terminal_Router->RouterAddr)+2,ADDR_04000401,4);//�����ַ��Ϊ�ն˵�ַ
				#else
					d64a=MRR(ADDR_TERMINAL,2);
					d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
					MWR(d64a,(u32)&Terminal_Router->RouterAddr,4);
					MR(((u32)&Terminal_Router->RouterAddr)+4,ADDR_AREACODE,2);
				#endif
					MR(((u32)p8txbuff)+128,(u32)&Terminal_Router->RouterAddr,6);
					Uart_3762_TxData(PORTn,5,1,6);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1:
					i=Uart_3762_RxData_AFN00_F1(PORTn,5,1,6);//UART_3762����AFN00F1ȷ������,���AFN,Fn,LEN_Data����ʧ���ط��ò���;����0δȷ��1ȷ��2ʧ��
					if(i==2)
					{//�������ڵ��ַʧ��
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 11://
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 12://������ʼ��(�嵵����Ϣ,�ָ�����������)
			i=MRR(ADDR_AFN01Fx+5,1);//��λ�����Fnֵ:1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
			if(i>=3)
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0://����
						Uart_3762_TxData(PORTn,0x01,2,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
						Terminal_Router->RouterInitSubTask=1;
						break;
					case 1://����
						Uart_3762_RxData_AFN00_F1(PORTn,0x01,2,0);//UART_3762����AFN00F1ȷ������,���AFN,Fn,LEN_Data����ʧ���ط��ò���;����0δȷ��1ȷ��2ʧ��
						//ֻ��3����ʧ�������ת����1��
						break;
					default:
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						break;
				}
			}
			else
			{
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 13://���ݳ�ʼ��(�ڵ�������Ϣ)
			i=MRR(ADDR_AFN01Fx+5,1);//��λ�����Fnֵ:1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,5=������ȫ����������ʼ��(���ָ�����������)��Ӳ����λ
			if(i>=2)
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0:
						Uart_3762_TxData(PORTn,0x01,3,0);
						Terminal_Router->RouterInitSubTask=1;
						break;
					case 1:
						Uart_3762_RxData_AFN00_F1(PORTn,0x01,3,0);//UART_3762����AFN00F1ȷ������,���AFN,Fn,LEN_Data����ʧ���ط��ò���;����0δȷ��1ȷ��2ʧ��
						//ֻ��3����ʧ�������ת����1��
						break;
					default:
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						break;
				}
			}
			else
			{
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 14://���ʼ����ͬ����־
			MC(0,ADDR_AFN01Fx+5,1);//�帴λ�����Fnֵ
		
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			Terminal_Router->RouterSynTask=1;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 15://��ʼ����ͬ������
			Terminal_RouterSyn(PORTn);//·��������ͬ��
			//��ʾ�������ͬ������
			i=CopyString((u8 *)"����ͬ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			i+=HEX_ASCII(MRR((u32)&Terminal_Router->NoteNum,2),ADDR_DATABUFF+i);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			MWR(0,ADDR_DATABUFF+i,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			if(Terminal_Router->RouterSynTask==0)//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			{
				Terminal_Router->RouterInitReCount=0;//�����ط�����
				Terminal_Router->RouterInitSubTask=0;//������
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
		case 16://�������ŵ���
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		/*
			if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			else
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0:
						Uart_3762_TxData(PORTn,0x03,8,0);
						Terminal_Router->RouterInitSubTask++;
						break;
					case 1:
						switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							case 1://1=���1֡����
								if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x0308)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
								{
									UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
									UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
									break;
								}
								UARTCtrl->Task=0;//0=����,����ת���Ȳ������
								i=p8rxbuff[13];
								Terminal_Router->RadioChannel=i;//�����ŵ���:0��63��64��253��������254���Զ�ѡ��255�����ֲ���
								i=p8rxbuff[14];
								Terminal_Router->RadioPower=i;//�������ڵ㷢�书��:00����߷��书�ʣ�01���θ߷��书�ʣ�02���εͷ��书�ʣ�03����ͷ��书�ʣ�04��254��������255�����ֲ���
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								break;
							case 0://0=����
							case 2://2=���ճ�ʱ(�����)
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								break;
						}
						break;
					default:
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						break;
				}
			}
		*/
			break;
		case 17://
			if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 �ϵ����ʱ�����붨ʱ��
			{
				CopyString((u8 *)"�ز��ȴ��ϵ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				MWR(0,ADDR_DATABUFF+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
			Terminal_Router->RouterInitTask=100;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;

			
		case 100://����·��
			Terminal_Router->RouterCtrl=1;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 101:
			Terminal_Router->RouterInitTask=200;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;

		
		case 200://>=200����
		//·���������
/*
			p32=(u32*)(ADDR_E4_ENTRY);
			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//�����������
			{
				UpDateRouter(PORTn);//·���������
				break;
			}
*/
		//·��������ͬ��
			Terminal_RouterSyn(PORTn);//·��������ͬ��
			if(Terminal_Router->RouterSynTask>=2)//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			{//����ͬ����ͬ����������־��Ϊ�˳��ָ�·��ʱ
				//������ͣ���ʱ����0���б����ʱ��������
				if(UARTCtrl->RMFlag==0)//0=��������ͣ,1=���������
				{
					p16timer=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
					p16timer[0]=0;
				}
				//��ʾ�������ͬ������
				i=CopyString((u8 *)"����ͬ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				i+=HEX_ASCII(MRR((u32)&Terminal_Router->NoteNum,2),ADDR_DATABUFF+i);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				MWR(0,ADDR_DATABUFF+i,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
		//��ͣ����
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//0=����,2=��ʱ
			{
				if((Terminal_Router->AFN04F10!=0)&&(Terminal_Ram->RouterSynDelay_S_Timer==0))//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
				{
					Terminal_Router->AFN04F10=0;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
					Terminal_Router->RouterSynTask=1;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
					break;
				}
				
				if((MRR(ADDR_AFN05F49+30,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//����ָ��ͨ�Ŷ˿���ͣ����
				{
					WarningOccur(PORTn,(u8 *)"�ز�������ͣ����");//�澯����,��ڸ澯����͸澯�ַ���
					if((Terminal_Router->StopFlags&(1<<10))==0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags|=(1<<10);//��ͣ·��
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
					}
					break;
				}
				else
				{
					if((Terminal_Router->StopFlags&(1<<10))!=0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags&=~(1<<10);//�ָ�·��
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
				}
				
				i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
//			#if (USER/100)==0
//				//���Ժ�����Բ���������õĲ������Զ�����
//				i=0;
//			#endif
				if(((i&0x01)!=0x0)&&(UARTCtrl->RMTaskC!=0))
				{//D0��"1"�������Զ�������"0" Ҫ���ն˸��ݳ���ʱ���Զ�����
					WarningOccur(PORTn,(u8 *)"�ز��������Զ�����");//�澯����,��ڸ澯����͸澯�ַ���
					if((Terminal_Router->StopFlags&(1<<11))==0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags|=(1<<11);//��ͣ·��
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
					}
					break;
				}
				else
				{
					if((Terminal_Router->StopFlags&(1<<11))!=0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags&=~(1<<11);//�ָ�·��
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
				}
				
				if((Terminal_Router->RMTD==0)&&(UARTCtrl->RMTaskC!=0))//0=�ǳ���ʱ��,1=����ʱ��
				{
//					WarningOccur(PORTn,(u8 *)"�ز��ǳ���ʱ��");//�澯����,��ڸ澯����͸澯�ַ���
//					if((Terminal_Router->StopFlags&(1<<12))==0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
//					{
//						Terminal_Router->StopFlags|=(1<<12);//��ͣ·��
//						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
//					}
//					break;
//xxx					
					WarningOccur(PORTn,(u8 *)"�ز��ǳ���ʱ��");//�澯����,��ڸ澯����͸澯�ַ���
					if((Terminal_Router->StopFlags&(1<<12))==0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags|=(1<<12);//��ͣ·��
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					else
					{
						if((Terminal_Router->StopFlags&(1<<17))==0)//��ͣ·��;b17=�������ϱ��¼�;Ҳ��Ϊ�����ͣʱ���������ϱ��¼�������־
						{
							//���������ϱ�����
							switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							{
								case 0://0=����
									UARTCtrl->Task=5;//����
									UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
									p16timer[0]=100/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
									break;
								case 1://1=���1֡����
								case 2://2=���ճ�ʱ(�����)
									UARTCtrl->Task=0;
									break;
							}
							return;
						}
					}
				}
				else
				{
					if((Terminal_Router->StopFlags&(1<<12))!=0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=ʵʱ����,8=����ͬ��,9=������ʱ,10=
					{
						Terminal_Router->StopFlags&=~(1<<12);//�ָ�·��
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
				}
		/*
				//�����ڶ������
				if((Terminal_Ram->CongealFlags&0x1f)!=0x1f)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
				{//�����ڶ�������
					if(Terminal_Router->StopFlags==0)//��ͣ·�ɱ�ʶ:b15=���ڶ���
					{
						Terminal_Router->StopFlags|=(1<<15);//��ͣ·��
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
					}
					break;
				}
				else
				{
					if((Terminal_Router->StopFlags&(1<<15))!=0)//��ͣ·�ɱ�ʶ:b15=���ڶ���
					{
						Terminal_Router->StopFlags&=~(1<<15);//�ָ�·��
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
				}
		*/
				//·���������
				if(Terminal_Router->RouterRunMode_4==1)//·������ģʽ_���ڳ���ģʽ��01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
				{//���������������ڳ���ģʽ
					if(UARTCtrl->RMFlag==0)//0=��������ͣ,1=���������
					{
						if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{
							UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
						}
						WarningOccur(PORTn,(u8 *)"�ز���������ͣ");//�澯����,��ڸ澯����͸澯�ַ���
					#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����								
						switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							case 0://0=����
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								p16timer[0]=100/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->Task=5;//����
								break;
							case 1://1=���1֡����
							case 2://2=���ճ�ʱ(�����)
								UARTCtrl->Task=0;
								break;
						}
					#endif
					#endif
						break;
					}
				}
				else
				{//·�����������ڳ���ģʽ
					if(Terminal_Router->NoteNum!=0)
					{//�ڵ����
					#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
						if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					#else
						if(((Terminal_Router->RUNState&2)==0)&&((Terminal_Router->StopFlags&(1<<17))==0))//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					#endif
						{
							if((Terminal_Router->StopFlags&1)!=0)//��ͣ·�ɱ�־:b0=��������ʱ
							{
								if(UARTCtrl->RMFlag!=0)//0=��������ͣ,1=���������
								{
									Terminal_Router->RouterCtrl=1;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
									break;
								}
								WarningOccur(PORTn,(u8 *)"�ز���������ͣ");//�澯����,��ڸ澯����͸澯�ַ���
							#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
							#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����								
								switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								{
									case 0://0=����
										UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
										p16timer[0]=100/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
										UARTCtrl->Task=5;//����
										break;
									case 1://1=���1֡����
									case 2://2=���ճ�ʱ(�����)
										UARTCtrl->Task=0;
										break;
								}
							#endif
							#endif
								break;
							}
						}
					}
				}
			}
		//����
			if(Terminal_Router->NoteNum==0)
			{//�ڵ���
				UARTCtrl->Task=0;//0=����,�Ա���������
				DisplayRouterVer(PORTn);//��ʾ·�ɳ��ҺͰ汾��
				return;
			}
		#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			if(Terminal_Router->RouterRunMode_4!=1)//·������ģʽ_���ڳ���ģʽ:01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
		#else
			if((Terminal_Router->RouterRunMode_4!=1)&&((Terminal_Router->StopFlags&(1<<17))==0))//·������ģʽ_���ڳ���ģʽ:01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
		#endif	
			{//֧��·�����������ڳ���ģʽ
				//�ϵ�����,�㳭
				if(UARTCtrl->RMTaskC==0)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
				{//�ϵ�����,�����ü�������������
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
					return;
				}
				//ʱ����,�㳭
				if(UARTCtrl->RMTaskC==2)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
				{//ÿ�ܳ�������;0=û��,1=��(�ز��޸�Ϊ������ʧ��ʱΪ�ӿ������˳���������������)(���ÿ�յ�ʱ����:������û����������û��͹���ҵ�û���ǰ��ѹ�������������ۼ�ʱ�����ݵĲɼ�)
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
					return;
				}
				//��ʱ����ͣ���¼����,�㳭
				if(UARTCtrl->RMTaskC==3)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
				{//��ʱ����ͣ���¼����
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
					return;
				}
				//�ص㻧���߳���(���������߶���ĵ����)
				if(UARTCtrl->RMTaskC==4)//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
				{//
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
					return;
				}
				//���ڳ������ܱ��¼�
				if(UARTCtrl->RMTaskC==(MaxRMTask-2))//��ǰ���������:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ʱ����,(MaxRMTask-1)=ѭ������(ֻ��ѭ������ſɲ�������ȼ�����)
				{//
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						return;
					}
					Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
					return;
				}
				
				//·�ɳ���
				if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
				{
					if(Terminal_Router->StopFlags==0)//��ͣ·�ɱ�־
					{
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						Terminal_Router->RouterInitTask=201;//��ʱ·�ɻָ�
						break;
					}
				}
				GeneralReadMeter_3762_Std(PORTn);//��׼�ز�ģ��,���泭��
			}
			else
			{//01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ;�������ϱ�
			#if (USER/100)!=0//���ǵ��Ժ�ͼ�
				if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
				{//΢��������,��·��ѧϰ��ɺ���ܳ���,����ֱ�ӷ��ط���
					if((Terminal_Router->RUNState&1)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						if(Terminal_Router->NoteNumOver==0)//AFN04F10���õĴӽڵ���>ģ��֧�ֵ����ӽڵ���;0=��,1=��
						{
							WarningOccur(PORTn,(u8 *)"������������");//�澯����,��ڸ澯����͸澯�ַ���
						}
						else
						{
							WarningOccur(PORTn,(u8 *)"�ӽڵ�������!");//�澯����,��ڸ澯����͸澯�ַ���
						}
						p16timer=(u16 *)&Terminal_Ram->Trip3_MS_Timer;//�ն���բ3 10MS��ʱ��
						if(p16timer[0]==0)
						{
							p16timer[0]=10000/10;//�´ζ���10���
							Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						}
						break;
					}
				}
			#endif
				Terminal_ReadMeter_3762_NoRouter(PORTn);//�ز�����(���������������ڳ���ģʽ)
			}
			break;
		case 201://��ʱ·�ɻָ�
			if(Terminal_Ram->RouterCtrl_MS_Timer==0)//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
			{
				Terminal_Router->RouterCtrl=5;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
				Terminal_Router->RouterInitTask=202;//·�ɻָ����
			}
			WarningOccur(PORTn,(u8 *)"�ز�·�ɻָ���ʱ");//�澯����,��ڸ澯����͸澯�ַ���
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
		#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����								
			switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 0://0=����
					UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
					p16timer[0]=100/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					UARTCtrl->Task=5;//����
					break;
				case 1://1=���1֡����
				case 2://2=���ճ�ʱ(�����)
					UARTCtrl->Task=0;
					break;
			}
		#endif
		#endif
			break;
		case 202://·�ɻָ����
			if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
			{
				Terminal_Router->RouterInitTask=0;//�ָ�ʧ��
			}
			else
			{
				Terminal_Router->RouterInitTask=200;//�ָ��ɹ�
			}
			break;
			
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}



void Terminal_RouterSyn(u32 PORTn)//·��������ͬ��
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u64 d64a;
	u64 d64b;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 *p16timer;
	u64 d64c;
	
	d64c=d64c;
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(Terminal_Router->RouterSynTask)//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
	{
		default:
			Terminal_Router->RouterSynTask=0;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			break;
		case 1://1=�ȴ�����
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				{
					Terminal_Router->StopFlags|=(1<<8);//��ͣ·��
					Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				}
				if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
				{//С����,ͬ����������ѧϰ��ɱ�־=0
					Terminal_Router->RUNState=0;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
				}
			#if (USER/100)==0//���Ժ����
				Terminal_Router->NoteNum=0;//���Ժ�������Ǵ�0��ʼͬ��
			#endif
				Terminal_Router->RouterInitSubTask=0;//������
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			}
			break;
		case 2://��ѯ�ӽڵ�����
		#if (USER/100)==0//���Ժ����
			if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
			{
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
			{//0��ʾ����Ҫ�·���������Ϣ
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://����
					Uart_3762_TxData(PORTn,0x10,1,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://����
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=1
								x=p8rxbuff[13]+(p8rxbuff[14]<<8);
								if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
								{//΢��������,����֪�ڵ���=0
									Terminal_Router->NoteNum=0;//����֪�ڵ������Ǵ�0��ʼ
								}
								else
								{//�������ز�
									if(Terminal_Router->NoteNum!=x)
									{
										Terminal_Router->NoteNum=0;//��֪�ڵ���
									}
								}
								Terminal_Router->ModeNoteNum=x;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)//�����ط�����
							{
								Terminal_Router->RouterInitReCount++;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������=����
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 3://��ȡ�ӽڵ���Ϣ
		#if (USER/100)==0//���Ժ����
			if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
			{
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
			{//0��ʾ����Ҫ�·���������Ϣ
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://�嵱ǰ·�����дӽڵ���
					if(Terminal_Router->ModeNoteNum==0)//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
					{
						Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
						return;
					}
					Terminal_Router->TempCount=1;//ɾ������ӽڵ�������(��Ŵ�1��ʼ)
					Terminal_Router->ReadNoteNum=0;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://����
					x=Terminal_Router->TempCount;
					MWR(x,((u32)p8txbuff)+128,2);//�ӽڵ���ʼ���
					p8txbuff[128+2]=10;//���ζ�10��
					Uart_3762_TxData(PORTn,0x10,2,3);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 2://����
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							y=0;
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1002)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=2
								x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
								y=MRR(((u32)p8rxbuff)+13+2,1);//���ζ���ӽڵ���
								Terminal_Router->TempCount+=y;//�¿�ʼ���
								Terminal_Router->ReadNoteNum+=y;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
								for(z=0;z<y;z++)
								{
									if((x+1)<=MaxRS485AddCarrierPn)
									{
										d64a=MRR(((u32)p8rxbuff)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&Terminal_Note->List[i].Addr,6);
											if(d64a==d64b)
											{
												break;//ԭ�Ѵ���
											}
										}
										if(i==x)
										{//ԭû����
											if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
											{//�������ز���
												MC(0,(u32)&Terminal_Note->List[x],LEN_NoteList);
												MWR(d64a,(u32)&Terminal_Note->List[x].Addr,6);
												x++;
												Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
											}
											else
											{//΢��������,������ַ�òɼ�����ַ
										#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&Terminal_Note->List[i].CollectorAddr,6);
													if(d64a==d64b)
													{
														break;//ԭ�Ѵ���
													}
												}
												if(i==x)
												{//ԭû����
													MC(0,(u32)&Terminal_Note->List[x],LEN_NoteList);
													MWR(d64a,(u32)&Terminal_Note->List[x].Addr,6);
													MWR(d64a,(u32)&Terminal_Note->List[x].CollectorAddr,6);//�ɼ�����ַ
													x++;
													Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
												}
										#endif
											}
										}
									}
								}
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								return;
							}
							if((Terminal_Router->TempCount<=MaxRS485AddCarrierPn)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
							{//������
								if(y==0)
								{
									Terminal_Router->TempCount+=10;//�¿�ʼ��Ŵ���10����ʼ
								}
								Terminal_Router->RouterInitSubTask=1;
							}
							else
							{//������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)//�����ط�����
							{
								Terminal_Router->RouterInitReCount++;//�����ط�����
								Terminal_Router->RouterInitSubTask=1;//������=����
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 4://ɾ���ӽڵ�
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://��ɾ������ӽڵ�������
					Terminal_Router->TempCount=0;//ɾ������ӽڵ�������
					p8txbuff[128]=0;//��Ҫɾ���Ľڵ���
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://��ɾ��
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=Terminal_Router->NoteNum)
						{
							if(p8txbuff[128]==0)//��Ҫɾ���Ľڵ���
							{//ûҪɾ��
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
								return;
							}
							else
							{//��ɾ��
								//������,����������Ӱ��p8txbuff+128�������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=2;//������
								return;
							}
						}
						d64a=MRR((u32)&Terminal_Note->List[Terminal_Router->TempCount].Addr,6);
						z=ADDR_AFN04F10;
						for(x=0;x<AFN04F10MaxPn;x++)
						{
							y=MRR(z+2,2);
							if(y!=0x0)
							{//��������Ч
								if((MRR(z+4,1)&0x1f)==31)
								{//�˿ں�31
									if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{//�������ز�
										d64b=MRR(z+6,6);
										if(d64a==d64b)
										{//��ַ��ͬ
											MWR(x+1,(u32)&Terminal_Note->List[Terminal_Router->TempCount].MeterNo,2);//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
											y=MRR(z+2,2);
											Terminal_Note->List[Terminal_Router->TempCount].Pn=y;//��Ӧ�������
											break;
										}
									}
									else
									{//΢��������,������ַ�òɼ�����ַ
									#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
										d64b=MRR(z+6,6);
										if(d64a==d64b)
										{//��ͨ�ŵ�ַ��ͬ
											d64b=MRR(z+20,6);//ȡ���òɼ�����ַ
											d64c=MRR((u32)&Terminal_Note->List[Terminal_Router->TempCount].CollectorAddr,6);
											if(((d64b==0xeeeeeeeeeeee)||(d64b==0))||(d64b==d64c))
											{//�޲ɼ�����ɼ�����ַҲ��ͬ
												MWR(x+1,(u32)&Terminal_Note->List[Terminal_Router->TempCount].MeterNo,2);//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
												y=MRR(z+2,2);
												Terminal_Note->List[Terminal_Router->TempCount].Pn=y;//��Ӧ�������
												break;
											}
										}
										else
										{
											d64c=MRR(z+20,6);//ȡ���õĲɼ�����ַ
											if(d64a==d64c)
											{//�ɼ�����ַ��ͬ
												for(y=0;y<Terminal_Router->NoteNum;y++)
												{
													d64c=MRR((u32)&Terminal_Note->List[y].Addr,6);//�б���ͨ�ŵ�ַ
													if(d64b==d64c)
													{
														break;
													}
												}
												if(y>=Terminal_Router->NoteNum)
												{//��ͬ�ɼ�����ַ�ĵ��ͨ�ŵ�ַ�����б�
													MWR(d64b,(u32)&Terminal_Note->List[Terminal_Router->TempCount].Addr,6);//ͨ�ŵ�ַ
													MWR(x+1,(u32)&Terminal_Note->List[Terminal_Router->TempCount].MeterNo,2);//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
													y=MRR(z+2,2);
													Terminal_Note->List[Terminal_Router->TempCount].Pn=y;//��Ӧ�������
													break;
												}
											}
										}
									#endif
									}
								}
							}
							z+=LEN_AFN04F10_Pn;
						}
						if(x==AFN04F10MaxPn)
						{//ɾ��
							if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{//�������ز�
								x=p8txbuff[128];//��Ҫɾ���Ľڵ���
								MR(((u32)p8txbuff)+128+1+(x*6),(u32)&Terminal_Note->List[Terminal_Router->TempCount].Addr,6);
								x++;//��Ҫɾ���Ľڵ���+1
								p8txbuff[128]=x;//��Ҫɾ���Ľڵ���
								if(x>=10)
								{
									//������,����������Ӱ��p8txbuff+128�������
									Terminal_Router->RouterInitReCount=0;//�����ط�����
									Terminal_Router->RouterInitSubTask=2;//������
									return;
								}
							}
							else
							{
							#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
								d64b=MRR((u32)&Terminal_Note->List[Terminal_Router->TempCount].CollectorAddr,6);//ȡҪɾ���Ĳɼ�����ַ
								if((d64b==0xeeeeeeeeeeee)||(d64b==0))
								{//�޲ɼ�����ַ
									x=p8txbuff[128];//��Ҫɾ���Ľڵ���
									MR(((u32)p8txbuff)+128+1+(x*6),(u32)&Terminal_Note->List[Terminal_Router->TempCount].Addr,6);
									x++;//��Ҫɾ���Ľڵ���+1
									p8txbuff[128]=x;//��Ҫɾ���Ľڵ���
									if(x>=10)
									{
										//������,����������Ӱ��p8txbuff+128�������
										Terminal_Router->RouterInitReCount=0;//�����ط�����
										Terminal_Router->RouterInitSubTask=2;//������
										return;
									}
								}
								else
								{//�ɼ�����ַ��Ч
									z=0;//����ͬ�ɼ�����ַ����
									for(y=0;y<Terminal_Router->NoteNum;y++)
									{
										d64c=MRR((u32)&Terminal_Note->List[y].CollectorAddr,6);//�б���ͨ�ŵ�ַ
										if(d64b==d64c)
										{
											z++;
											if(z>=2)
											{
												break;
											}
										}
									}
									if(z>=2)
									{//��ɾ���б�,�����͵�ģ��
										if(Terminal_Router->NoteNum!=0)
										{
											z=Terminal_Router->TempCount;
											y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//�ӵ�ַ����ɾȥ
											MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
											Terminal_Router->NoteNum--;//��ǰ·�������ܴӽڵ���
											Terminal_Router->TempCount=0;//��0��ʼ��������
											return;
										}
									}
									else
									{
										x=p8txbuff[128];//��Ҫɾ���Ľڵ���
										MR(((u32)p8txbuff)+128+1+(x*6),(u32)&Terminal_Note->List[Terminal_Router->TempCount].CollectorAddr,6);
										x++;//��Ҫɾ���Ľڵ���+1
										p8txbuff[128]=x;//��Ҫɾ���Ľڵ���
										if(x>=10)
										{
											//������,����������Ӱ��p8txbuff+128�������
											Terminal_Router->RouterInitReCount=0;//�����ط�����
											Terminal_Router->RouterInitSubTask=2;//������
											return;
										}
									}
								}
							#endif
							}
						}
						Terminal_Router->TempCount++;
					}
					break;
				case 2://ɾ������
				#if (USER/100)==0//���Ժ����
					if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
					{
						UARTCtrl->Task=0;//0=����,����ת���Ȳ������
						goto DELACK;
					}
				#else
					if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
					{//0��ʾ����Ҫ�·���������Ϣ
						UARTCtrl->Task=0;//0=����,����ת���Ȳ������
						goto DELACK;
					}
				#endif
					x=p8txbuff[128];//��Ҫɾ���Ľڵ���
					Uart_3762_TxData(PORTn,0x11,2,1+(x*6));//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 3://ɾ������
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//�ش�ȷ��,ɾ��
						DELACK:
								x=p8txbuff[128];//��Ҫɾ���Ľڵ���
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8txbuff)+128+1+(i*6),6);
									y=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
									for(z=0;z<y;z++)
									{
										d64b=MRR((u32)&Terminal_Note->List[z].Addr,6);
										if(d64a==d64b)
										{//�ҵ�Ҫɾȥ�ĵ�ַ
											if(Terminal_Router->NoteNum!=0)
											{
												y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//�ӵ�ַ����ɾȥ
												MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
												Terminal_Router->NoteNum--;//��ǰ·�������ܴӽڵ���
												Terminal_Router->ModeNoteNum--;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
											}
											break;
										}
										if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
										{//С����
										#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
											d64b=MRR((u32)&Terminal_Note->List[z].CollectorAddr,6);
											if(d64a==d64b)
											{//�ҵ�Ҫɾȥ�ĵ�ַ
												if(Terminal_Router->NoteNum!=0)
												{
													y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//�ӵ�ַ����ɾȥ
													MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
													Terminal_Router->NoteNum--;//��ǰ·�������ܴӽڵ���
													Terminal_Router->ModeNoteNum--;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
												}
												break;
											}
										#endif
										}
									}
								}
								p8txbuff[128]=0;//Ҫɾ����
								Terminal_Router->TempCount=0;//��0��ʼ��������
								Terminal_Router->RouterInitSubTask=1;//������
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							return;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Terminal_Router->RouterInitSubTask=2;//������
							}
							else
							{
								Terminal_Router->NoteNum=0;//��ǰ·��������֪�ܴӽڵ���
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;	
		case 5://��Ӵӽڵ�
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://��ɾ������ӽڵ�������
					Terminal_Router->NoteNumOver=0;//AFN04F10���õĴӽڵ���>ģ��֧�ֵ����ӽڵ���;0=��,1=��
					Terminal_Router->TempCount=0;//ɾ������ӽڵ�������
					p8txbuff[128]=0;//Ҫ�����
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://�����
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=AFN04F10MaxPn)
						{
							if(p8txbuff[128]==0)//��Ҫɾ���Ľڵ���
							{//ûҪ���
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
								return;
							}
							else
							{//�����
								//������,����������Ӱ��p8txbuff+128�������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=2;//������
								return;
							}
						}
						z=ADDR_AFN04F10+(LEN_AFN04F10_Pn*Terminal_Router->TempCount);
						x=MRR(z+2,2);
						if(x!=0x0)
						{//��������Ч
							if((MRR(z+4,1)&0x1f)==31)
							{//�˿ں�31
								d64a=MRR(z+6,6);
								d64b=MRR((u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ
								if(d64a!=d64b)
								{//����ַ!=���ڵ��ַ;��Ч
									if(d64a!=0xeeeeeeeeeeee)
									{//����ַ!=0xee;��Ч
										for(y=0;y<Terminal_Router->NoteNum;y++)
										{
											d64c=MRR((u32)&Terminal_Note->List[y].Addr,6);
											if(d64a==d64c)
											{
												break;
											}
										}
										if(y==Terminal_Router->NoteNum)
										{//���
										#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
											if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
											{//΢��������,������ַ�òɼ�����ַ
												d64b=MRR(z+20,6);//ȡ�ɼ�����ַ
												if((d64b!=0xeeeeeeeeeeee)&&(d64b!=0))
												{//�ɼ�����ַ��Ч
													for(y=0;y<Terminal_Router->NoteNum;y++)
													{
														d64c=MRR((u32)&Terminal_Note->List[y].CollectorAddr,6);
														if(d64c==d64b)
														{
															break;
														}
													}
													if(y!=Terminal_Router->NoteNum)
													{//�ɼ�����ַԭ�Ѵ���;�������б�,��д��ģ��
													AddNoTx:
														if(Terminal_Router->NoteNum<MaxRS485AddCarrierPn)
														{
															MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
															MWR(d64a,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].Addr,6);
															MWR(d64b,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].CollectorAddr,6);
															Terminal_Note->List[Terminal_Router->NoteNum].MeterNo=Terminal_Router->TempCount+1;//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
															Terminal_Note->List[Terminal_Router->NoteNum].Pn=x;//��Ӧ�������
															Terminal_Router->NoteNum++;
														}
														Terminal_Router->TempCount++;
														continue;
													}
													d64a=d64b;//�òɼ�����ַ����ģ��
												}
											}
										#endif
											x=p8txbuff[128];//��Ҫ��ӵĽڵ���
											//MR(((u32)p8txbuff)+128+1+(x*7),ADDR_AFN04F10+(LEN_AFN04F10_Pn*Terminal_Router->TempCount)+6,6);
											MWR(d64a,((u32)p8txbuff)+128+1+(x*7),6);
											y=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*Terminal_Router->TempCount)+5,1);
											switch(y)
											{
												case 1://dl645-1997
													p8txbuff[128+1+(x*7)+6]=1;//ͨ��Э������
													break;
											#if ((USER/100)==5)
												case 21://�Ϻ���Լ
													//����Ҫ������Ϻ�Э����ʱ��Ҫ���Լ���ͱ���Ϊ��00��
													p8txbuff[128+1+(x*7)+6]=0;//ͨ��Э������
													break;
											#endif
												default://dl645-2007
													p8txbuff[128+1+(x*7)+6]=2;//ͨ��Э������
													break;
											}
											y=10;
										#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
											if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
											{//΢��������,������ַ�òɼ�����ַ
												y=1;
												if(Terminal_Router->ModeNoteNum>=Terminal_Router->MaxNoteNum)
												{//ģ�������дӽڵ���>=ģ��֧�ֵ����ӽڵ���
													//�������б�,��д��ģ��
													Terminal_Router->NoteNumOver=1;//AFN04F10���õĴӽڵ���>ģ��֧�ֵ����ӽڵ���;0=��,1=��
													goto AddNoTx;
												}
											}
										#endif
											x++;//��Ҫ��ӵĽڵ���+1
											p8txbuff[128]=x;//��Ҫ��ӵĽڵ���
											if(x>=y)
											{
												//������,����������Ӱ��p8txbuff+128�������
												Terminal_Router->RouterInitReCount=0;//�����ط�����
												Terminal_Router->RouterInitSubTask=2;//������
												return;
											}
										}
									}
								}
							}
						}
						Terminal_Router->TempCount++;
					}
					break;
				case 2://��ӷ���
				#if (USER/100)==0//���Ժ����
					if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
					{
						UARTCtrl->Task=0;//0=����,����ת���Ȳ������
						goto ADDACK;
					}
				#else
					if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
					{//0��ʾ����Ҫ�·���������Ϣ
						UARTCtrl->Task=0;//0=����,����ת���Ȳ������
						goto ADDACK;
					}
				#endif
					x=p8txbuff[128];//��Ҫ��ӵĽڵ���
					Uart_3762_TxData(PORTn,0x11,1,1+(7*x));//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 3://��ӽ���
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==((0<<8)+1))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//�ش�ȷ��,���
						ADDACK:
								x=p8txbuff[128];//��Ҫ��ӵĽڵ���
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8txbuff)+128+1+(i*7),6);
									for(z=0;z<AFN04F10MaxPn;z++)
									{
										if(MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+2,2)!=0)
										{//��������Ч
											d64b=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+4,8);
											if((d64b&0x1f)==31)
											{//�˿ں�31
												d64b>>=16;
												if(d64a==d64b)
												{//�ҵ�Ҫ��ӵ�04�������õ�ַ
													if(Terminal_Router->NoteNum<MaxRS485AddCarrierPn)
													{
														MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
														MWR(d64a,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].Addr,6);
														if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
														{//΢��������,������ַ�òɼ�����ַ
														#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
															d64b=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+20,6);
															MWR(d64b,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].CollectorAddr,6);
														#endif
														}
														Terminal_Note->List[Terminal_Router->NoteNum].MeterNo=z+1;//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
														z=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+2,2);//��Ӧ�������
														Terminal_Note->List[Terminal_Router->NoteNum].Pn=z;
														Terminal_Router->NoteNum++;
														Terminal_Router->ModeNoteNum++;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
													}
													break;
												}
												else
												{
													if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
													{//΢��������,������ַ�òɼ�����ַ
													#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
														d64b=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+20,6);
														if(d64a==d64b)
														{//�ҵ�Ҫ��ӵ�04�������õ�ַ
															if(Terminal_Router->NoteNum<MaxRS485AddCarrierPn)
															{
																MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
																d64a=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+6,6);
																MWR(d64a,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].Addr,6);
																MWR(d64b,(u32)&Terminal_Note->List[Terminal_Router->NoteNum].CollectorAddr,6);
																Terminal_Note->List[Terminal_Router->NoteNum].MeterNo=z+1;//��Ӧ���ܱ�/��������װ�����(��1��ʼ)
																z=MRR(ADDR_AFN04F10+(z*LEN_AFN04F10_Pn)+2,2);//��Ӧ�������
																Terminal_Note->List[Terminal_Router->NoteNum].Pn=z;
																Terminal_Router->NoteNum++;
																Terminal_Router->ModeNoteNum++;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
															}
															break;
														}
													#endif
													}
												}
											}
										}
									}
								}
								p8txbuff[128]=0;//Ҫ�����
								Terminal_Router->RouterInitSubTask=1;//������
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							return;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Terminal_Router->RouterInitSubTask=2;//������
							}
							else
							{
								Terminal_Router->NoteNum=0;//��ǰ·��������֪�ܴӽڵ���
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 6://��ѯ�ӽڵ�����
		#if (USER/100)==0//���Ժ����
			if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
			{
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
			{//0��ʾ����Ҫ�·���������Ϣ
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://����
					Uart_3762_TxData(PORTn,0x10,1,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://����
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=1
								x=p8rxbuff[13]+(p8rxbuff[14]<<8);
								if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
								{//΢��������
									y=Terminal_Router->ModeNoteNum;
									if(y!=x)
									{//ģ��ڵ���!=��ȡ�Ľڵ���
										Terminal_Router->NoteNum=0;//ͬ������
										Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
										return;
									}
								}
								else
								{//�������ز�
									if(Terminal_Router->NoteNum!=x)
									{//����֪�ڵ���!=ģ���ȡ�Ľڵ���
										Terminal_Router->NoteNum=0;//ͬ������
										Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
										return;
									}
								}
								Terminal_Router->ModeNoteNum=x;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)//�����ط�����
							{
								Terminal_Router->RouterInitReCount++;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������=����
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 7://��ȡ�ӽڵ���Ϣ
		#if (USER/100)==0//���Ժ����
			if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
			{
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
			{//0��ʾ����Ҫ�·���������Ϣ
				Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://�嵱ǰ·�����дӽڵ���
					if(Terminal_Router->ModeNoteNum==0)//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
					{
						Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
						return;
					}
					Terminal_Router->TempCount=1;//ɾ������ӽڵ�������(��Ŵ�1��ʼ)
					Terminal_Router->ReadNoteNum=0;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://����
					x=Terminal_Router->TempCount;
					MWR(x,((u32)p8txbuff)+128,2);//�ӽڵ���ʼ���
					p8txbuff[128+2]=10;//���ζ�10��
					Uart_3762_TxData(PORTn,0x10,2,3);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 2://����
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							y=0;
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1002)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=2
								x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
								y=MRR(((u32)p8rxbuff)+13+2,1);//���ζ���ӽڵ���
								Terminal_Router->TempCount+=y;
								Terminal_Router->ReadNoteNum+=y;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
								for(z=0;z<y;z++)
								{
									if((x+1)<=MaxRS485AddCarrierPn)
									{
										d64a=MRR(((u32)p8rxbuff)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&Terminal_Note->List[i].Addr,6);
											if(d64a==d64b)
											{
												break;//ԭ�Ѵ���
											}
										}
										if(i==x)
										{//ԭû����
											if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
											{
												//��Ӳ������
												Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
												return;
											}
											else
											{//΢��������,������ַ�òɼ�����ַ
											#if (((USER/100)!=6)||(SVER>=0x200))//���ϼ�����V1.xx����ԭ����
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&Terminal_Note->List[i].CollectorAddr,6);
													if(d64a==d64b)
													{
														break;//ԭ�Ѵ���
													}
												}
												if(i==x)
												{//ԭû����
													//��Ӳ������
													Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
													return;
												}
											#else
												//��Ӳ������
												Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
												return;
											#endif
											}
										}
									}
								}
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								return;
							}
							if((Terminal_Router->TempCount<=MaxRS485AddCarrierPn)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
							{//������
								if(y==0)
								{
									Terminal_Router->TempCount+=10;//�¿�ʼ��Ŵ���10����ʼ
								}
								Terminal_Router->RouterInitSubTask=1;
							}
							else
							{//������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->RouterInitReCount<2)//�����ط�����
							{
								Terminal_Router->RouterInitReCount++;//�����ط�����
								Terminal_Router->RouterInitSubTask=1;//������=����
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 8://�ָ�·��
			if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				Terminal_Router->StopFlags&=~(1<<8);//�ָ�·��
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			}
			Terminal_Router->RouterSynTask=0;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
			break;
	}
}




void Terminal_RouterCtrl(u32 PORTn)//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
{
	u32 i;
	u8 *p8rxbuff;
	u16 *p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterCtrl==0)//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((UARTCtrl->Task==3)||(UARTCtrl->Task==4))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		return;//���ڷ���(�����ϱ�ʱ�������������,������ȷ��,������˵)
	}
	//if(Terminal_Router->RouterRunMode_2==0)//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
	if(Terminal_Router->RouterRunMode_4==1)//·������ģʽ_���ڳ���ģʽ��01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
	{
		if((Terminal_Router->RouterCtrl!=4)&&(Terminal_Router->RouterCtrl!=6))//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
		{
			if(Terminal_Router->RouterCtrl==1)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			{
				Terminal_Router->StopFlags=0;//��ͣ·�ɱ�־
			}
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			Terminal_Ram->RouterCheck_S_Timer=(10*60);//�ز�·���������붨ʱֵ
			Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
			Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			return;
		}
	}

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	switch(Terminal_Router->RouterCtrl)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
	{//·�ɿ���
		case 0://��
			break;
		case 1://����
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
			switch(Terminal_Router->RouterCtrlTask)//·���������������;��ɺ��Զ��ص�0
			{
				case 0:
					//�����ڹ����㶳������,�����Ժ������Ҫ,������������·����������ʱҪ��6������Ӧ,����ǰ���㶳��ʱ��Լ��3��
					i=MRR(ADDR_TYMDHMS,2);
					if((i<0x0003)||((Terminal_Ram->CongealFlags&0x1f)!=0x1f))
					{//�������ʱ�ӷ���<00��03������ڶ���
						break;
					}
					Terminal_Router->ReRouterCtrl=0;//·���������ط�����
					Uart_3762_TxData(PORTn,0x12,1,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterCtrlTask++;//·���������������;��ɺ��Զ��ص�0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rxbuff[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->StopFlags=0;//��ͣ·�ɱ�־
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								//������Ϊ���Ժ����ȥ�˶�״̬�����ӵ�
								Terminal_Router->RUNState|=2;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,1,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								//����·��������=0ʱ�����ͻָ�����
								if(Terminal_Router->NoteNum!=0)
								{//�ڵ��!=0
									Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								}
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 2://��ͣ
			switch(Terminal_Router->RouterCtrlTask)//·���������������;��ɺ��Զ��ص�0
			{
				case 0:
					if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					{
						if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
						{//��������ͣ
							Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
							Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
							break;
						}
					}
					Terminal_Router->ReRouterCtrl=0;//·���������ط�����
					Uart_3762_TxData(PORTn,0x12,2,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterCtrlTask++;//·���������������;��ɺ��Զ��ص�0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rxbuff[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								//������Ϊ���Ժ����ȥ�˶�״̬�����ӵ�
								Terminal_Router->RUNState&=0xfd;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,2,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							}
							else
							{
								Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 3://��ʱ�ָ�
			if(Terminal_Router->StopFlags!=0)//��ͣ·�ɱ�־
			{//������������ͣ
				Terminal_Ram->RouterCtrl_MS_Timer=0;//5000/100;//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
			}
			else
			{
			#if (USER/100)==0
				Terminal_Ram->RouterCtrl_MS_Timer=0;//5000/100;//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
			#else
				if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
				{//����100s
					Terminal_Ram->RouterCtrl_MS_Timer=100000/10;//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
				}
				else
				{//����30s
					Terminal_Ram->RouterCtrl_MS_Timer=30000/10;//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
				}
			#endif
			}
			UARTCtrl->Task=0;//��Ϊ0=�ձ�,�Ա���������ת����
			Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
			//�����õ�ǰ·������״̬=0��Ϊ��ϣ·��ʱ��Ҫ,����ͣ·�ɺ��״̬��Ϊ������ָ������ٴζ�ȡ״̬���ж�ѧϰ״̬
			Terminal_Router->RUNState=0;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
			break;
		case 5://�����ָ�
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
			switch(Terminal_Router->RouterCtrlTask)//·���������������;��ɺ��Զ��ص�0
			{
				case 0:
					if(Terminal_Router->StopFlags!=0)//��ͣ·�ɱ�־
					{//������������ͣ
						Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
					Terminal_Router->ReRouterCtrl=0;//·���������ط�����
					Uart_3762_TxData(PORTn,0x12,3,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterCtrlTask++;//·���������������;��ɺ��Զ��ص�0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rxbuff[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								//������Ϊ���Ժ����ȥ�˶�״̬�����ӵ�
								Terminal_Router->RUNState|=2;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,3,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								//����·��������=0ʱ�����ͻָ�����
								if(Terminal_Router->NoteNum!=0)
								{//�ڵ��!=0
									Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								}
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 4://��״̬
			if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//������
				Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
				Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
			}
			else
			{//С����
				switch(Terminal_Router->RouterCtrlTask)//·���������������;��ɺ��Զ��ص�0
				{
					case 0:
						Terminal_Router->ReRouterCtrl=0;//·���������ط�����
						Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
						Terminal_Router->RouterCtrlTask++;//·���������������;��ɺ��Զ��ص�0
						break;
					case 1:
						switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							case 1://1=���1֡����
								UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)&&((p8rxbuff[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
								{
									//ȡ��ǰ����״̬
									i=p8rxbuff[13];
									Terminal_Router->RUNState=i;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
									//
									Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
									Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
									break;
								}
								else
								{
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
									p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
									UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
									UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								}
								break;
							case 0://0=����
							case 2://2=���ճ�ʱ(�����)
								i=2;
								if(Comm_Ram->RouterType==102)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
								{//��Ѷ��������ʱ�᲻��Ӧ
									i=20;
								}
								if(Terminal_Router->ReRouterCtrl<i)//·���������ط�����
								{
									Terminal_Router->ReRouterCtrl++;
									Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
								}
								else
								{
									Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
									Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								}
								break;
						}
						break;
					default:
						Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
						break;
				}
			}
			break;
		case 6://��������
			switch(Terminal_Router->RouterCtrlTask)//·���������������;��ɺ��Զ��ص�0
			{
				case 0:
					Terminal_Router->ReRouterCtrl=0;//·���������ط�����
					Uart_3762_TxData(PORTn,0x11,102,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterCtrlTask++;//·���������������;��ɺ��Զ��ص�0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rxbuff[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
								UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
								UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x11,102,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
								Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}

void RouterBroadcastTime(u32 PORTn)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
{
	u32 i;
	u32 x;
	u64 d64;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
		return;
	}
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->BroadcastTime)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	{
		default:
			break;
		case 3://3=�����
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Terminal_Router->StopFlags|=(1<<4);//��ͣ·��
				Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				WarningOccur(PORTn,(u8 *)"�ز�׼���㲥Уʱ");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 4://�ȴ�1��
			p16timer[0]=60000/10;
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 5://ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
			if(p16timer[0]==0)
			{
				p8txbuff+=128;
				p8txbuff[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
				p8txbuff[1]=22;//���ĳ���
				MC(0xfe,((u32)p8txbuff)+2,4);
				p8txbuff[6]=0x68;
				MC(0x99,((u32)p8txbuff)+7,6);
				p8txbuff[13]=0x68;
				p8txbuff[14]=0x08;
				p8txbuff[15]=0x06;
			#if ((USER/100)==5)//�Ϻ�
				i=MRR(ADDR_AFN05F201,1);
				if(i&(1<<3))
				{
					d64=MRR(ADDR_TYMDHMS,6);
					x=MRR(ADDR_AFN05F201+7,2);
					if(x&0x1000)
					{//��ֵ
						x&=0xfff;
						x=bcd_hex(x);
						d64=YMDHMS_SubS(d64,x);//������ʱ�����S��,���ؼ�S����������ʱ����
					}
					else
					{//��ֵ
						x=bcd_hex(x);
						d64=YMDHMS_AddS(d64,x);//������ʱ�����S��,���ؼ�S����������ʱ����
					}
					MWR(d64,((u32)p8txbuff)+16,6);
				}
				else
				{
					MR(((u32)p8txbuff)+16,ADDR_TYMDHMS,6);
				}
			#else
				MR(((u32)p8txbuff)+16,ADDR_TYMDHMS,6);
			#endif
				for(i=0;i<6;i++)
				{
					p8txbuff[16+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8txbuff[6+i];
				}
				p8txbuff[22]=x;
				p8txbuff[23]=0x16;
				Uart_3762_TxData(PORTn,0x03,9,24);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0	
				WarningOccur(PORTn,(u8 *)"�ز����ڹ㲥Уʱ");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 6://���㲥Уʱ
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rxbuff)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTime--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{
					UARTCtrl->BroadcastTime=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				break;
			}
			i=MRR(((u32)p8rxbuff)+13,2);//�㲥ͨ���ӳ�ʱ����
			p8txbuff+=128;
			p8txbuff[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
			p8txbuff[1]=22;//���ĳ���
			MC(0xfe,((u32)p8txbuff)+2,4);
			p8txbuff[6]=0x68;
			MC(0x99,((u32)p8txbuff)+7,6);
			p8txbuff[13]=0x68;
			p8txbuff[14]=0x08;
			p8txbuff[15]=0x06;
			d64=MRR(ADDR_TYMDHMS,6);
			d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
		#if ((USER/100)==5)//�Ϻ�
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<3))
			{
				x=MRR(ADDR_AFN05F201+7,2);
				if(x&0x1000)
				{//��ֵ
					x&=0xfff;
					x=bcd_hex(x);
					d64=YMDHMS_SubS(d64,x);//������ʱ�����S��,���ؼ�S����������ʱ����
				}
				else
				{//��ֵ
					x=bcd_hex(x);
					d64=YMDHMS_AddS(d64,x);//������ʱ�����S��,���ؼ�S����������ʱ����
				}
			}
		#endif
			MWR(d64,((u32)p8txbuff)+16,6);
			for(i=0;i<6;i++)
			{
				p8txbuff[16+i]+=0x33;
			}
			x=0;
			for(i=0;i<16;i++)
			{
				x+=p8txbuff[6+i];
			}
			p8txbuff[22]=x;
			p8txbuff[23]=0x16;
			Uart_3762_TxData(PORTn,0x05,3,24);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
			}
			UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0	
			break;
		case 7://����ȷ��֡
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{//0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ���
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//���㲥��ʱʱ����(���ֶ���)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
				}
			}
			else
			{//1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//ȡ�ȴ�ʱ��
							i=p8rxbuff[17];
							i+=p8rxbuff[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTime++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTime--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
							else
							{
								UARTCtrl->BroadcastTime=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						UARTCtrl->BroadcastTime=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						break;
				}
			}
			break;
		case 8://�ȴ��㲥��ʱ
			if(p16timer[0]==0)
			{
				UARTCtrl->BroadcastTime=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
//		case 9:
//			break;
		case 10:
			Terminal_Router->StopFlags&=~(1<<4);//�ָ�·��
			Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
		#if ((USER/100)==5)//�Ϻ�
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<3))
			{
				i&=~(1<<3);
				MWR(i,ADDR_AFN05F201,1);
			}
		#endif
			break;
	}
}



#endif

