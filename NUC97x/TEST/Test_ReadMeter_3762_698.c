
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "../DL698/RM_3762_RouterActive.h"
#include "../DL698/RM_3762_iActive.h"





#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

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
			//WarningOccur(PORTn,(u8 *)"δ֪�ز�");//�澯����,��ڸ澯����͸澯�ַ���
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
	//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
}


void Terminal_ReadMeter_3762(u32 PORTn)//�ն˳���Q/GDW376.2
{
	u32 i;
	u64 d64a;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
//	u32 * p32;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	if(UARTCtrl->Task>6)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������
	{//�ݴ�
		UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������
	}
	switch(Terminal_Router->RouterInitTask)
	{//ģ��״̬
		case 0://�ϵ��λ(�ص�Դ)
//			Pin_Configure(PIN_UART4_RX,//RXD����,����
//									0|//B3-B0:���ù���ѡ��AF(0-15)
//						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
//						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
//						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
//						 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
//						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
//			Pin_Configure(PIN_UART4_TX,//TXD����,����
//									0|//B3-B0:���ù���ѡ��AF(0-15)
//						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
//						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
//						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
//						 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
//						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			Pin_Configure(PIN_PLC_RST,//��λ�������0
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			i=Comm_Ram->MYMDHMS[1]+(Comm_Ram->MYMDHMS[2]<<8);//ʱ��
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
				//WarningOccur(PORTn,(u8 *)"�ز�ģ��û����");//�澯����,��ڸ澯����͸澯�ַ���
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
//				UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
//				UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
				UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				Terminal_Router->RUNState=0;//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
				Terminal_Router->StopFlags=0;//��ͣ·�ɱ�־
				Terminal_Router->RouterCtrlTask=0;//·���������������;��ɺ��Զ��ص�0
				Terminal_Router->RouterCtrl=0;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
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
					i=p8rx[13];
					Terminal_Router->RouterRunMode_1=i&0x0f;//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
					MWR(0xffffffff,((u32)p8tx)+128,6);
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
							i=p8rx[13+30]+(p8rx[13+31]<<8);
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
							
							i=p8rx[13];
							Terminal_Router->RouterRunMode_1=i&0x0f;//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							Terminal_Router->RouterRunMode_2=(i>>4)&0x1;//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
							if(Comm_Ram->RouterType==7)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��
							{//Ѷ�ܴ�������
								Terminal_Router->RouterRunMode_2=0;//·������ģʽ_·�ɹ���ʽ��0��ʾ����ͨ��ģ����·�ɹ����ܣ���ʵ��ͨ�ŵĵ��ƽ�����ܣ�1��ʾ����ͨ��ģ�����·�ɹ�����
							}
							Terminal_Router->RouterRunMode_3=(i>>5)&0x1;//·������ģʽ_��������Ϣģʽ��0��ʾ����Ҫ�·���������Ϣ��1��ʾ��Ҫ�·���������Ϣ
							Terminal_Router->RouterRunMode_4=(i>>6)&0x3;//·������ģʽ_���ڳ���ģʽ��01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
							i=p8rx[13+1];
							Terminal_Router->RouterRunMode_5=i&0x07;//·������ģʽ_������ʱ����֧�֣���D2��D1��D0������λ��ʾ���ֱ����㲥���ӽڵ��ء�·�������������������֧���������ṩ������ʱ�����������1��ʾ֧���ṩ������ʱ������0��ʾ��֧��
							Terminal_Router->RouterRunMode_6=(i>>3)&0x3;//·������ģʽ_ʧ�ܽڵ��л�����ʽ��D0��1��ʾͨ��ģ�������л������ڵ㣬D1��1��ʾ����������֪ͨͨ��ģ���л������ڵ�
							Terminal_Router->RouterRunMode_7=(i>>5)&0x1;//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
							Terminal_Router->RouterRunMode_8=(i>>6)&0x3;//·������ģʽ_�㲥�����ŵ�ִ�з�ʽ��0��ʾִ�й㲥�����Ҫ�ŵ���ʶ��1��ʾ�㲥����Ҫ���ݱ����е��ŵ���ʶ��������ͣ�����ȡֵ����
							i=p8rx[13+6];
							Terminal_Router->MaxOverTime=i;//�ӽڵ������ʱʱ����
							
							i=p8rx[13+7]+(p8rx[13+8]<<8);
							Terminal_Router->MaxBroadcastOverTime=i;//���㲥��ʱʱ����
							i=p8rx[13+9]+(p8rx[13+10]<<8);
							Terminal_Router->MaxTxByte=i;//���֧�ֱ��ĳ���(���ֶ���)
							i=p8rx[13+11]+(p8rx[13+12]<<8);
							Terminal_Router->MaxFileByte=i;//�ļ�����֧�ֵ���󵥸����ݰ�����(���ֶ���)
							i=p8rx[13+13];
							Terminal_Router->MaxUpDateTime=i;//�����������ȴ�ʱ����(���ֶ���)
							i=p8rx[13+20]+(p8rx[13+21]<<8);
							if(Comm_Ram->RouterType==102)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=;100=����,101=��ϣ,102=��Ѷ��,103=�з�
							{//��Ѷ��֧�ֵ����ӽڵ���-1
								i--;
							}
							Terminal_Router->MaxNoteNum=i;//֧�ֵ����ӽڵ���(���ֶ���)
							MR((u32)&Terminal_Router->RouterCODE,((u32)p8rx)+13+30,4);//���̴��� ASCII 2,оƬ���� ASCII 2
							Data_Inverse((u32)&Terminal_Router->RouterCODE,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
							Data_Inverse(((u32)&Terminal_Router->RouterCODE)+2,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
							MR((u32)&Terminal_Router->RouterVer,((u32)p8rx)+13+34,5);//·�����汾��,�ֱ�Ϊ:3BYTE������,2BYTE�汾

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
							//Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							
							Comm_Ram->GPMSTimer=0;//���Զ�ʱ��
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
							d64a=MRR(ADDR_4001+2,6);
							d64a=R_Inverse(d64a,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							MWR(d64a,(u32)&Terminal_Router->RouterAddr,6);
							i=DataComp((u32)&Terminal_Router->RouterAddr,((u32)p8rx)+13,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
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
					d64a=MRR(ADDR_4001+2,6);
					d64a=R_Inverse(d64a,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					MWR(d64a,(u32)&Terminal_Router->RouterAddr,6);
				
					MR(((u32)p8tx)+128,(u32)&Terminal_Router->RouterAddr,6);
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
/*
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
*/
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 13://���ݳ�ʼ��(�ڵ�������Ϣ)
/*
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
*/
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 14://���ʼ����ͬ����־
			//MC(0,ADDR_AFN01Fx+5,1);//�帴λ�����Fnֵ
		
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
			//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
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
								i=p8rx[13];
								Terminal_Router->RadioChannel=i;//�����ŵ���:0��63��64��253��������254���Զ�ѡ��255�����ֲ���
								i=p8rx[14];
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
//			if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 �ϵ����ʱ�����붨ʱ��
//			{
//				CopyString((u8 *)"�ز��ȴ��ϵ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
//				MWR(0,ADDR_DATABUFF+12,1);
//				//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
//				return;
//			}
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
				if(UARTCtrl->TaskID==0)//0=��������ͣ,1=���������
				{
//					p16timer=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
//					p16timer[0]=0;
				}
				//��ʾ�������ͬ������
				i=CopyString((u8 *)"����ͬ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				i+=HEX_ASCII(MRR((u32)&Terminal_Router->NoteNum,2),ADDR_DATABUFF+i);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				MWR(0,ADDR_DATABUFF+i,1);
				//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//0=����,2=��ʱ
			{
				if((UARTCtrl->SetChange6000==0x55)&&(Terminal_Ram->RouterSynDelay_S_Timer==0))//�ɼ��������ñ��б仯��־:0=û�仯,0x55=�仯
				{
					UARTCtrl->SetChange6000=0;//�ɼ��������ñ��б仯��־:0=û�仯,0x55=�仯
					Terminal_Router->RouterSynTask=1;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
					break;
				}
			}
		//����
			if(Terminal_Router->NoteNum==0)
			{//�ڵ���
				UARTCtrl->Task=0;//0=����,�Ա���������
				DisplayRouterVer(PORTn);//��ʾ·�ɳ��ҺͰ汾��
				return;
			}
			if((Terminal_Router->RouterRunMode_4!=1)&&((Terminal_Router->StopFlags&(1<<17))==0))//·������ģʽ_���ڳ���ģʽ:01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
			{//֧��·�����������ڳ���ģʽ
				i=UARTCtrl->TaskID;
				i>>=8;
				i&=0xff;
				if(i==1)
				{//Լ�������1·��������,��������Ϊ�㳭
					//·�ɳ���
					if((Terminal_Router->RUNState&2)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						
//						if(Terminal_Router->StopFlags==0)//��ͣ·�ɱ�־
//						{
							Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
							Terminal_Router->RouterInitTask=201;//��ʱ·�ɻָ�
							break;
//						}
					}
					//RM_3762_RouterActive(PORTn);//�ز�����(·��ģ�����������ڳ���ģʽ)
				}
				else
				{//Լ�������1·��������,��������Ϊ�㳭
					if((Terminal_Router->RUNState&2)!=0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
					{
						Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
						break;
					}
					//RM_3762_iActive(PORTn);//�ز�����(���������������ڳ���ģʽ)
				}
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
							////WarningOccur(PORTn,(u8 *)"������������");//�澯����,��ڸ澯����͸澯�ַ���
						}
						else
						{
							////WarningOccur(PORTn,(u8 *)"�ӽڵ�������!");//�澯����,��ڸ澯����͸澯�ַ���
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
				//RM_3762_iActive(PORTn);//�ز�����(���������������ڳ���ģʽ)
			}
			break;
		case 201://��ʱ·�ɻָ�
			if(Terminal_Ram->RouterCtrl_MS_Timer==0)//16 ·�ɿ��ƻָ���ʱ 10MS��ʱ��
			{
				Terminal_Router->RouterCtrl=5;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
				Terminal_Router->RouterInitTask=202;//·�ɻָ����
			}
			//WarningOccur(PORTn,(u8 *)"�ز�·�ɻָ���ʱ");//�澯����,��ڸ澯����͸澯�ַ���
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
	u8* p8rx;
	u8* p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16* p16timer;
	u64 d64c;
	u8 *p8s;
	u8* p8d;
	
	d64c=d64c;
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
								x=p8rx[13]+(p8rx[14]<<8);
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
					MWR(x,((u32)p8tx)+128,2);//�ӽڵ���ʼ���
					p8tx[128+2]=10;//���ζ�10��
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
								y=MRR(((u32)p8rx)+13+2,1);//���ζ���ӽڵ���
								Terminal_Router->TempCount+=y;//�¿�ʼ���
								Terminal_Router->ReadNoteNum+=y;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
								for(z=0;z<y;z++)
								{
									if((x+1)<=NUM_RMmax)
									{
										d64a=MRR(((u32)p8rx)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&NoteList->List[i].Addr,6);
											if(d64a==d64b)
											{
												break;//ԭ�Ѵ���
											}
										}
										if(i==x)
										{//ԭû����
											if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
											{//�������ز���
												MC(0,(u32)&NoteList->List[x],LEN_NoteList);
												MWR(d64a,(u32)&NoteList->List[x].Addr,6);
												x++;
												Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
											}
											else
											{//΢��������,������ַ�òɼ�����ַ
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
													if(d64a==d64b)
													{
														break;//ԭ�Ѵ���
													}
												}
												if(i==x)
												{//ԭû����
													MC(0,(u32)&NoteList->List[x],LEN_NoteList);
													MWR(d64a,(u32)&NoteList->List[x].Addr,6);
													MWR(d64a,(u32)&NoteList->List[x].CollectorAddr,6);//�ɼ�����ַ
													x++;
													Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
												}
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
							if((Terminal_Router->TempCount<=NUM_RMmax)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
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
					p8tx[128]=0;//��Ҫɾ���Ľڵ���
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://��ɾ��
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=Terminal_Router->NoteNum)
						{
							if(p8tx[128]==0)//��Ҫɾ���Ľڵ���
							{//ûҪɾ��
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
								return;
							}
							else
							{//��ɾ��
								//������,����������Ӱ��p8tx+128�������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=2;//������
								return;
							}
						}
						d64a=MRR((u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
						p8s=(u8*)ADDR_6000_SDRAM;
						for(x=0;x<NUM_RMmax;x++)
						{
							if(p8s[0]==DataType_structure)
							{//��Ч
								p8d=p8s+5;
								p8d=Get_Element(p8d,4,0,0);//����Ԫ�ص�ַ
								if(p8d)
								{
									z=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
									z=OADtoPORTn(z);
									if(z==PORTn)
									{//�˿ں���ͬ
										if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
										{//�������ز�
											d64b=MRR((u32)p8s+9,6);//ȡ����ͨ�ŵ�ַ
											d64b=R_Inverse(d64b,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
											if(d64a==d64b)
											{//��ַ��ͬ
												break;
											}
										}
										else
										{//΢��������,������ַ�òɼ�����ַ
											d64b=MRR((u32)p8s+9,6);
											d64b=R_Inverse(d64b,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
											if(d64a==d64b)
											{//��ͨ�ŵ�ַ��ͬ
												p8d=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ
												if(p8d==0)
												{//�޲ɼ�����ȡ
													break;
												}
												d64b=MRR((u32)p8d+4,6);//ȡ���òɼ�����ַ
												d64b=R_Inverse(d64b,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
												d64c=MRR((u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);
												if(((d64b==0xeeeeeeeeeeee)||(d64b==0))||(d64b==d64c))
												{//�޲ɼ�����ɼ�����ַҲ��ͬ
													break;
												}
											}
											else
											{
												p8d=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ
												if(p8d)
												{
													d64c=MRR((u32)p8d+4,6);//ȡ���òɼ�����ַ
													d64c=R_Inverse(d64c,6);
													if(d64a==d64c)
													{//�ɼ�����ַ��ͬ
														for(y=0;y<Terminal_Router->NoteNum;y++)
														{
															d64c=MRR((u32)&NoteList->List[y].Addr,6);//�б���ͨ�ŵ�ַ
															if(d64b==d64c)
															{
																break;
															}
														}
														if(y>=Terminal_Router->NoteNum)
														{//��ͬ�ɼ�����ַ�ĵ��ͨ�ŵ�ַ�����б�
															MWR(d64b,(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);//ͨ�ŵ�ַ
															break;
														}
													}
												}
											}
										}
									}
								}
							}
							p8s+=LENper_6000;
						}
						if(x==NUM_RMmax)
						{//ɾ��
							if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
							{//�������ز�
								x=p8tx[128];//��Ҫɾ���Ľڵ���
								MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
								x++;//��Ҫɾ���Ľڵ���+1
								p8tx[128]=x;//��Ҫɾ���Ľڵ���
								if(x>=10)
								{
									//������,����������Ӱ��p8tx+128�������
									Terminal_Router->RouterInitReCount=0;//�����ط�����
									Terminal_Router->RouterInitSubTask=2;//������
									return;
								}
							}
							else
							{
								d64b=MRR((u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);//ȡҪɾ���Ĳɼ�����ַ
								if((d64b==0xeeeeeeeeeeee)||(d64b==0))
								{//�޲ɼ�����ַ
									x=p8tx[128];//��Ҫɾ���Ľڵ���
									MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
									x++;//��Ҫɾ���Ľڵ���+1
									p8tx[128]=x;//��Ҫɾ���Ľڵ���
									if(x>=10)
									{
										//������,����������Ӱ��p8tx+128�������
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
										d64c=MRR((u32)&NoteList->List[y].CollectorAddr,6);//�б���ͨ�ŵ�ַ
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
										x=p8tx[128];//��Ҫɾ���Ľڵ���
										MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);
										x++;//��Ҫɾ���Ľڵ���+1
										p8tx[128]=x;//��Ҫɾ���Ľڵ���
										if(x>=10)
										{
											//������,����������Ӱ��p8tx+128�������
											Terminal_Router->RouterInitReCount=0;//�����ط�����
											Terminal_Router->RouterInitSubTask=2;//������
											return;
										}
									}
								}
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
					x=p8tx[128];//��Ҫɾ���Ľڵ���
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
								x=p8tx[128];//��Ҫɾ���Ľڵ���
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8tx)+128+1+(i*6),6);
									y=Terminal_Router->NoteNum;//��ǰ·�������ܴӽڵ���
									for(z=0;z<y;z++)
									{
										d64b=MRR((u32)&NoteList->List[z].Addr,6);
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
											d64b=MRR((u32)&NoteList->List[z].CollectorAddr,6);
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
										}
									}
								}
								p8tx[128]=0;//Ҫɾ����
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
					p8tx[128]=0;//Ҫ�����
					Terminal_Router->RouterInitSubTask++;//������
					break;
				case 1://�����
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=NUM_RMmax)
						{
							if(p8tx[128]==0)//��Ҫɾ���Ľڵ���
							{//ûҪ���
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterSynTask++;//����ͬ������;0=����,1=�ȴ�����,2-nͬ��������ɺ��0
								return;
							}
							else
							{//�����
								//������,����������Ӱ��p8tx+128�������
								Terminal_Router->RouterInitReCount=0;//�����ط�����
								Terminal_Router->RouterInitSubTask=2;//������
								return;
							}
						}
						p8s=(u8*)ADDR_6000_SDRAM+(LENper_6000*Terminal_Router->TempCount);
						if(p8s[0]==DataType_structure)
						{//��Ч
							p8d=p8s+5;
							p8d=Get_Element(p8d,4,0,0);//����Ԫ�ص�ַ
							if(p8d)
							{
								z=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
								z=OADtoPORTn(z);
								if(z==PORTn)
								{//�˿ں���ͬ
									d64a=MRR((u32)p8s+9,6);//ȡ����ͨ�ŵ�ַ
									d64a=R_Inverse(d64a,6);
									d64b=MRR((u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ
									if(d64a!=d64b)
									{//����ַ!=���ڵ��ַ;��Ч
										if(d64a!=0xeeeeeeeeeeee)
										{//����ַ!=0xee;��Ч
											for(y=0;y<Terminal_Router->NoteNum;y++)
											{
												d64c=MRR((u32)&NoteList->List[y].Addr,6);
												if(d64a==d64c)
												{
													break;
												}
											}
											if(y==Terminal_Router->NoteNum)
											{//���
												if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
												{//΢��������,������ַ�òɼ�����ַ
													p8d=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ
													if(p8d)
													{
														d64b=MRR((u32)p8d+4,6);//ȡ���òɼ�����ַ
														d64b=R_Inverse(d64b,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
														if((d64b!=0xeeeeeeeeeeee)&&(d64b!=0))
														{//�ɼ�����ַ��Ч
															for(y=0;y<Terminal_Router->NoteNum;y++)
															{
																d64c=MRR((u32)&NoteList->List[y].CollectorAddr,6);
																if(d64c==d64b)
																{
																	break;
																}
															}
															if(y!=Terminal_Router->NoteNum)
															{//�ɼ�����ַԭ�Ѵ���;�������б�,��д��ģ��
															AddNoTx:
																if(Terminal_Router->NoteNum<NUM_RMmax)
																{
																	MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
																	MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
																	MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																	Terminal_Router->NoteNum++;
																}
																Terminal_Router->TempCount++;
																continue;
															}
															d64a=d64b;//�òɼ�����ַ����ģ��
														}
													}
												}
												x=p8tx[128];//��Ҫ��ӵĽڵ���
												MWR(d64a,((u32)p8tx)+128+1+(x*7),6);
												
//	��Լ����  enum
//	{
//  	δ֪ (0)��
//		DL/T645-1997��1����
//		DL/T645-2007��2����
//		DL/T698.45��3����
//		CJ/T 188-2004��4��
//	}
												p8d=p8s+5;
												p8d=Get_Element(p8d,3,0,0);//����Ԫ�ص�ַ
												if(p8d==0)
												{
													y=0;
												}
												else
												{
													y=p8d[1];
												}
												switch(y)
												{
													case 1://DL/T645-1997��1��
														p8tx[128+1+(x*7)+6]=1;//ͨ��Э������
														break;
													case 2://DL/T645-2007��2��
														p8tx[128+1+(x*7)+6]=2;//ͨ��Э������
														break;
													case 3://DL/T698.45��3��
														if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��
														{//������698����͸��
															p8tx[128+1+(x*7)+6]=0;//ͨ��Э������
														}
														else
														{
															p8tx[128+1+(x*7)+6]=3;//ͨ��Э������
														}
														break;
													case 4://CJ/T 188-2004��4��
													default://
														p8tx[128+1+(x*7)+6]=2;//ͨ��Э������
														break;
												}
												y=10;
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
												x++;//��Ҫ��ӵĽڵ���+1
												p8tx[128]=x;//��Ҫ��ӵĽڵ���
												if(x>=y)
												{
													//������,����������Ӱ��p8tx+128�������
													Terminal_Router->RouterInitReCount=0;//�����ط�����
													Terminal_Router->RouterInitSubTask=2;//������
													return;
												}
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
					x=p8tx[128];//��Ҫ��ӵĽڵ���
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
								x=p8tx[128];//��Ҫ��ӵĽڵ���
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8tx)+128+1+(i*7),6);
									for(z=0;z<NUM_RMmax;z++)
									{
										p8s=(u8*)ADDR_6000_SDRAM+(z*LENper_6000);
										if(p8s[0]==DataType_structure)
										{//��������Ч
											p8d=p8s+5;
											p8d=Get_Element(p8d,4,0,0);//����Ԫ�ص�ַ
											if(p8d)
											{
												y=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
												y=OADtoPORTn(y);
												if(y==PORTn)
												{//�˿ں���ͬ
													d64b=MRR((u32)p8s+9,6);//ȡ����ͨ�ŵ�ַ
													d64b=R_Inverse(d64b,6);
													if(d64a==d64b)
													{//�ҵ�Ҫ��ӵ�04�������õ�ַ
														if(Terminal_Router->NoteNum<NUM_RMmax)
														{
															MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
															MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
															if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
															{//΢��������,������ַ�òɼ�����ַ
																p8d=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ
																if(p8d)
																{
																	d64b=MRR((u32)p8d+4,6);//ȡ���òɼ�����ַ
																	d64b=R_Inverse(d64b,6);
																	MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																}
															}
															Terminal_Router->NoteNum++;
															Terminal_Router->ModeNoteNum++;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
														}
														break;
													}
													else
													{
														if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
														{//΢��������,������ַ�òɼ�����ַ
															p8d=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ
															if(p8d)
															{
																d64b=MRR((u32)p8d+4,6);//ȡ���òɼ�����ַ
																d64b=R_Inverse(d64b,6);
																if(d64a==d64b)
																{//�ҵ�Ҫ��ӵ�04�������õ�ַ
																	if(Terminal_Router->NoteNum<NUM_RMmax)
																	{
																		MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
																		d64a=MRR((u32)p8s+9,6);//ȡ����ͨ�ŵ�ַ
																		d64a=R_Inverse(d64a,6);
																		MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
																		MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																		Terminal_Router->NoteNum++;
																		Terminal_Router->ModeNoteNum++;//��ģ���ȡ��ģ�鷵�شӽڵ���(���ֶ���)
																	}
																	break;
																}
															}
														}
													}
												}
											}
										}
									}
								}
								p8tx[128]=0;//Ҫ�����
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
								x=p8rx[13]+(p8rx[14]<<8);
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
					MWR(x,((u32)p8tx)+128,2);//�ӽڵ���ʼ���
					p8tx[128+2]=10;//���ζ�10��
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
								y=MRR(((u32)p8rx)+13+2,1);//���ζ���ӽڵ���
								Terminal_Router->TempCount+=y;
								Terminal_Router->ReadNoteNum+=y;//����ͬ��ʱ��ģ���Ѷ�ȡ�Ľڵ���(���ֶ���)
								for(z=0;z<y;z++)
								{
									if((x+1)<=NUM_RMmax)
									{
										d64a=MRR(((u32)p8rx)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&NoteList->List[i].Addr,6);
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
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
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
							if((Terminal_Router->TempCount<=NUM_RMmax)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
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
	u8 *p8rx;
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

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
					i=MRR(ADDR_MYMDHMS,2);
					if(i<0x0003)
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
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
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
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
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
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
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
								if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)&&((p8rx[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
								{
									//ȡ��ǰ����״̬
									i=p8rx[13];
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
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
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
	u8 *p8rx;
	u8 *p8tx;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
		return;
	}
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->BroadcastTimeTask)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
	{
		default:
			break;
		case 3://3=�����
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Terminal_Router->StopFlags|=(1<<4);//��ͣ·��
				Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				//WarningOccur(PORTn,(u8 *)"�ز�׼���㲥Уʱ");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 4://�ȴ�1��
			p16timer[0]=60000/10;
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 5://ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
			if(p16timer[0]==0)
			{
				p8tx+=128;
				p8tx[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
				p8tx[1]=22;//���ĳ���
				MC(0xfe,((u32)p8tx)+2,4);
				p8tx[6]=0x68;
				MC(0x99,((u32)p8tx)+7,6);
				p8tx[13]=0x68;
				p8tx[14]=0x08;
				p8tx[15]=0x06;
				MR(((u32)p8tx)+16,ADDR_MYMDHMS,6);
				for(i=0;i<6;i++)
				{
					p8tx[16+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8tx[6+i];
				}
				p8tx[22]=x;
				p8tx[23]=0x16;
				Uart_3762_TxData(PORTn,0x03,9,24);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0	
				//WarningOccur(PORTn,(u8 *)"�ز����ڹ㲥Уʱ");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 6://���㲥Уʱ
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				break;
			}
			i=MRR(((u32)p8rx)+13,2);//�㲥ͨ���ӳ�ʱ����
			p8tx+=128;
			p8tx[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
			p8tx[1]=22;//���ĳ���
			MC(0xfe,((u32)p8tx)+2,4);
			p8tx[6]=0x68;
			MC(0x99,((u32)p8tx)+7,6);
			p8tx[13]=0x68;
			p8tx[14]=0x08;
			p8tx[15]=0x06;
			d64=MRR(ADDR_MYMDHMS,6);
			d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
			MWR(d64,((u32)p8tx)+16,6);
			for(i=0;i<6;i++)
			{
				p8tx[16+i]+=0x33;
			}
			x=0;
			for(i=0;i<16;i++)
			{
				x+=p8tx[6+i];
			}
			p8tx[22]=x;
			p8tx[23]=0x16;
			Uart_3762_TxData(PORTn,0x05,3,24);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
			}
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0	
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
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
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
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
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
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						break;
				}
			}
			break;
		case 8://�ȴ��㲥��ʱ
			if(p16timer[0]==0)
			{
				UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
//		case 9:
//			break;
		case 10:
			Terminal_Router->StopFlags&=~(1<<4);//�ָ�·��
			Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			UARTCtrl->BroadcastTimeTask=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
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
#endif
