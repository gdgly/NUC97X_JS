
#include "Project.h"
#include "AutoSearchMeter_3762.h"
#include "AutoSearchMeter.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"



#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
void AutoSearchMeter_3762(u32 PORTn)//�Զ��ѱ�3762
{
	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢
	{
		case 2://����
			AutoSearchMeter_3762_TC(PORTn);//�Զ��ѱ�3762-����
			break;
		case 3://����
		case 9://9=����
			AutoSearchMeter_3762_ES(PORTn);//�Զ��ѱ�3762-����
			break;
		case 1://��̩
		case 5://��˹��
		default://����
			AutoSearchMeter_3762_01(PORTn);//�Զ��ѱ�3762-��˹��
			break;
	}
}


//�Զ��ѱ�3762-����
const u8 TC_Example1[]=
{0x68,0x1F,0x00,0x41,0x00,0x00,0x28,0x64,0x00,0x00,0x05,0x04,0x00,0x02,0x0E,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x02,0x36,0x33,0x4A,0x16,0x90,0x16,};

void AutoSearchMeter_3762_TC(u32 PORTn)//�Զ��ѱ�3762-����
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u8 * p8rxbuff;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
	{
		case 0://�ȴ�������
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			}
			break;
		case 1://��ͣ·��
			WarningOccur(PORTn,(u8 *)"�ز������Զ��ѱ�");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			Terminal_Router->StopFlags|=(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
	#if (USER/100)!=0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
			#if RMM_Type==0//���г�������:0=�ز���,1=RS485��ͨ���ɼ���,2=���
				Terminal_Router->RS485Time=0;//�ɼ�����485���ʱֵ��(BCD)
				Terminal_Ram->AutoSearch_S_Timer=0;//28 �ز��Զ��ѱ��붨ʱ��
				i=MRR(ADDR_AFN05F103+1,2);
				i++;
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
				UARTCtrl->AutoSearchTask=4;//�Զ��ѱ�����=4://����ӽڵ�����ע��
				return;
			#else
				Terminal_Router->RS485Time=0x03;//�ɼ�����485���ʱֵ��(BCD)
				Terminal_Router->KeepTime=10;//ע�����ʱ�䶨ʱֵ��
			#endif
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=0;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Router->RS485Time=0x60;//�ɼ�����485���ʱֵ��(BCD)
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
	#else
		//�ͼ�ģʽ
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
				Terminal_Router->KeepTime=120;//ע�����ʱ�䶨ʱֵ��
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask=4;//�Զ��ѱ�����
	#endif
			break;
		case 2://�����㲥(�ɼ�����485���)
			MR((u32)p8txbuff,(u32)TC_Example1,sizeof(TC_Example1));
			i=Terminal_Router->RS485Time;
			MWR(i,((u32)p8txbuff)+25,2);//�ɼ�����485����ʱ��
			i=bcd_hex(i);
			i*=60;
			i+=10;//��10��
			Terminal_Ram->AutoSearch_S_Timer=i;//28 �ز��Զ��ѱ��붨ʱ��
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+15);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
			UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
			p8txbuff[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=sizeof(TC_Example1);//�����ֽ���(���ֶ���)
#if(USER/100)==7//�㽭�û�
			/*
			�㽭Ҫ���Զ��ѱ��ڲ�ÿ��20:00-23��00�ѱ�ʱ��ż������������20:00������Һ����ʾ�ѱ������
			����ԭ��Ϊ���������㲥��ģ��û���ڳ�ʱʱ���ڻ�ȷ��֡�����ͺ���ճ�ʱֵUARTCtrl->TxToRxTimerOver����ʱ����̵���֪���ĸ���ֵ�����������¸�ֵ��
			�����ط������汾�ѱ�Ҳ�и����������û��Ҫ�����Զ��ѱ����Ըõط�ֻ�޸��㽭�������ط�����Ҫ���ѱ��ٸġ�
			*/
			UARTCtrl->TxToRxTimerOver=60000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
#endif
			UARTCtrl->Task=3;//�ȴ�����
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 3://����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
					break;
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 4://����ӽڵ�����ע��
			if(Terminal_Ram->AutoSearch_S_Timer!=0)//28 �ز��Զ��ѱ��붨ʱ��
			{//�����㲥(�ɼ�����485���)��ʱ���
				break;
			}
			MR(((u32)p8txbuff)+128,ADDR_TYMDHMS,6);//��ʼʱ��	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8txbuff)+128+6,2);//����ʱ��(min) ��
			MWR(0x0101,((u32)p8txbuff)+128+6+2,2);//�ӽڵ��ط�����	BIN	1,����ȴ�ʱ��Ƭ����(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 5://����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
					break;
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 6://�Զ��ѱ����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//��ʱ��ʱ
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 �ز��Զ��ѱ��붨ʱ��
			{
				UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
				break;
			}
			break;
		case 7://��·������״̬
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 8://��·������״̬����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
						UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
	#if (USER/100)!=0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
						if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢
						{//�Ƕ���
							i=MRR(((u32)p8rxbuff)+13+13,3);
							if(i==0x080808)
							{//·��������м����๤�������Ϊ08ʱ,�Զ��ѱ����
								i=0;
							}
							else
							{
								i=2;
							}
						}
						else
						{//��������
							i=p8rxbuff[13];
							i&=2;
						}
						if(i==0)
						{//����״̬��:D1=������־(0Ϊֹͣ����,1=���ڹ���),D0=·��ѧϰ(0=δ���,1=ѧϰ���)
							switch(Terminal_Router->n)
							{
								case 1://����ִ�е�1�ֽ���
									Terminal_Router->n=2;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
									Terminal_Router->RS485Time=0x10;//�ɼ�����485���ʱֵ��(BCD)
									Terminal_Router->KeepTime=20;//ע�����ʱ�䶨ʱֵ��
									UARTCtrl->AutoSearchTask=2;//�����㲥(�ɼ�����485���)
									return;
								case 2://����ִ�е�2�ֽ���
									Terminal_Router->n=3;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
									Terminal_Router->RS485Time=0x17;//�ɼ�����485���ʱֵ��(BCD)
									Terminal_Router->KeepTime=12*60;//ע�����ʱ�䶨ʱֵ��
									UARTCtrl->AutoSearchTask=2;//�����㲥(�ɼ�����485���)
									return;
								case 0://ÿ��ִ�н���
								default://����ִ�е�3�ֽ���
									goto AutoSearch_End;//�Զ��ѱ����
							}
						}
	#else
		//�ͼ�ģʽ
					i=p8rxbuff[13];
					i&=2;
					if(i==0)
					{//����״̬��:D1=������־(0Ϊֹͣ����,1=���ڹ���),D0=·��ѧϰ(0=δ���,1=ѧϰ���)
						goto AutoSearch_End;//�Զ��ѱ����
					}
	#endif
					Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchTask=6;//�Զ��ѱ�����
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 100://��ֹ����ע��
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 101://��ֹ����ע�����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//�Զ��ѱ����
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			
AutoSearch_End://�Զ��ѱ����
			WarningOccur(PORTn,(u8 *)"�ز��Զ��ѱ����");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
			UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
			UARTCtrl->RMCount=0;//�����������=0
			UARTCtrl->ReRMCount=0;//���������ط�����
			UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
			if(MRR(ADDR_AFN05F149,1)==0x55)
			{//��F149����
				if(MRR(ADDR_AFN05F149+2+3,1)==0x5a)//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				{//����ִ��
					MWR(0xaa,ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				}
			}
			//�������
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		#if (USER/100)==0//���Ժ�ͼ�
			Terminal_Router->StopFlags&=~(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
		#else
			//��λģ��
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}


//�Զ��ѱ�3762-����
const u8 ES_Example1[]=//�㲥�����ɼ����ѱ�
{0x68,35,0x00,0x41,0x00,0x00,0x28,0x64,0x00,0x00,0x05,0x04,0x00,0x80,0x20,0x01,16,0x68,0x99,0x99,0x99,0x99,0x99,0x99,0x68,0x1F,0x04,0xAB,0xAB,0xAB,0xAB,0x00,0x16,0x90,0x16,};


void AutoSearchMeter_3762_ES(u32 PORTn)//�Զ��ѱ�3762-����
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u8 * p8rxbuff;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
	{
		case 0://�ȴ�������
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			}
			break;
		case 1://��ͣ·��
			WarningOccur(PORTn,(u8 *)"�ز������Զ��ѱ�");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			Terminal_Router->StopFlags|=(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
	#if (USER/100)!=0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
			#if RMM_Type==0//���г�������:0=�ز���,1=RS485��ͨ���ɼ���,2=���
				Terminal_Router->RS485Time=0;//�ɼ�����485���ʱֵ��(BCD)
				Terminal_Ram->AutoSearch_S_Timer=0;//28 �ز��Զ��ѱ��붨ʱ��
				i=MRR(ADDR_AFN05F103+1,2);
				i++;
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
				UARTCtrl->AutoSearchTask=4;//�Զ��ѱ�����=4://����ӽڵ�����ע��
				return;
			#else
				Terminal_Router->RS485Time=15;//�ɼ�����485���ʱֵ��(HEX),��СΪ12����߽��鲻����120
				Terminal_Router->KeepTime=15;//ע�����ʱ�䶨ʱֵ��
			#endif
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=0;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Router->RS485Time=30;//�ɼ�����485���ʱֵ��(HEX),��СΪ12����߽��鲻����120
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x223000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��,����30���������ɼ�����485���ʱ
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
	#else
		//�ͼ�ģʽ
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
				Terminal_Router->KeepTime=120;//ע�����ʱ�䶨ʱֵ��
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=0;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Router->RS485Time=30;//�ɼ�����485���ʱֵ��(HEX),��СΪ12����߽��鲻����120
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x223000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��,����30���������ɼ�����485���ʱ
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask=4;//�Զ��ѱ�����
	#endif
			break;
		case 2://�����㲥(�ɼ�����485���)
			MR((u32)p8txbuff,(u32)ES_Example1,sizeof(ES_Example1));
			i=Terminal_Router->RS485Time;
			MWR(i,((u32)p8txbuff)+14,2);//�ɼ�����485����ʱ��
			i*=60;
			i+=10;//��10��
			Terminal_Ram->AutoSearch_S_Timer=i;//28 �ز��Զ��ѱ��붨ʱ��
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+17);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
			UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
			p8txbuff[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=sizeof(ES_Example1);//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//�ȴ�����
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 3://����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
	#if (USER/100)!=0//0=����,1=����==0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
					break;
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
	#else
		//�ͼ�ģʽ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
	#endif
		case 4://����ӽڵ�����ע��
			if(Terminal_Ram->AutoSearch_S_Timer!=0)//28 �ز��Զ��ѱ��붨ʱ��
			{//�����㲥(�ɼ�����485���)��ʱ���
				break;
			}
			MR(((u32)p8txbuff)+128,ADDR_TYMDHMS,6);//��ʼʱ��	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8txbuff)+128+6,2);//����ʱ��(min) ��
			MWR(0x0101,((u32)p8txbuff)+128+6+2,2);//�ӽڵ��ط�����	BIN	1,����ȴ�ʱ��Ƭ����(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 5://����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
					break;
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 6://�Զ��ѱ����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//��ʱ��ʱ
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 �ز��Զ��ѱ��붨ʱ��
			{
				UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
				break;
			}
			break;
		case 7://��·������״̬
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 8://��·������״̬����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
	#if (USER/100)!=0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
					i=p8rxbuff[13];
					i&=2;
					if(i==0)
					{//����״̬��:D1=������־(0Ϊֹͣ����,1=���ڹ���),D0=·��ѧϰ(0=δ���,1=ѧϰ���)
						switch(Terminal_Router->n)
						{
							case 1://����ִ�е�1�ֽ���
								Terminal_Router->n=2;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
								Terminal_Router->RS485Time=30;//�ɼ�����485���ʱֵ��(HEX),��СΪ12����߽��鲻����120
								Terminal_Router->KeepTime=30;//ע�����ʱ�䶨ʱֵ��
								UARTCtrl->AutoSearchTask=2;//�����㲥(�ɼ�����485���)
								return;
							case 2://����ִ�е�2�ֽ���
								Terminal_Router->n=3;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
								Terminal_Router->RS485Time=60;//�ɼ�����485���ʱֵ��(HEX),��СΪ12����߽��鲻����120
								Terminal_Router->KeepTime=12*60;//ע�����ʱ�䶨ʱֵ��
								UARTCtrl->AutoSearchTask=2;//�����㲥(�ɼ�����485���)
								return;
							case 0://ÿ��ִ�н���
							default://����ִ�е�3�ֽ���
								goto AutoSearch_End;//�Զ��ѱ����
						}
					}
	#else
		//�ͼ�ģʽ
					i=p8rxbuff[13];
					i&=2;
					if(i==0)
					{//����״̬��:D1=������־(0Ϊֹͣ����,1=���ڹ���),D0=·��ѧϰ(0=δ���,1=ѧϰ���)
						goto AutoSearch_End;//�Զ��ѱ����
					}
	#endif
					Terminal_Ram->AutoSearch_S_Timer=1*60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchTask=6;//�Զ��ѱ�����
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 100://��ֹ����ע��
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 101://��ֹ����ע�����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//�Զ��ѱ����
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			
AutoSearch_End://�Զ��ѱ����
			WarningOccur(PORTn,(u8 *)"�ز��Զ��ѱ����");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
			UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
			UARTCtrl->RMCount=0;//�����������=0
			UARTCtrl->ReRMCount=0;//���������ط�����
			UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
			if(MRR(ADDR_AFN05F149,1)==0x55)
			{//��F149����
				if(MRR(ADDR_AFN05F149+2+3,1)==0x5a)//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				{//����ִ��
					MWR(0xaa,ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				}
			}
			//�������
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		#if (USER/100)==0//���Ժ�ͼ�
			Terminal_Router->StopFlags&=~(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
		#else
			//��λģ��
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}

void AutoSearchMeter_3762_01(u32 PORTn)//�Զ��ѱ�3762-��˹��
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u8 * p8rxbuff;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//�Զ��ѱ�����:0=��,1=��ʼ,2...����
	{
		case 0://�ȴ�������
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			}
			break;
		case 1://��ͣ·��
			WarningOccur(PORTn,(u8 *)"�ز������Զ��ѱ�");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			Terminal_Router->StopFlags|=(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
	#if (USER/100)!=0//0=��������,1=���Ժ�ͼ�(���Ŷ���ģ���Զ��ѱ�Ϊ�ͼ�ģʽ)
		//��������
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Router->RS485Time=0x03;//�ɼ�����485���ʱֵ��(BCD)
			#if RMM_Type==0//���г�������:0=�ز���,1=RS485��ͨ���ɼ���,2=���
				i=MRR(ADDR_AFN05F103+1,2);
				i++;
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			#else
				Terminal_Router->KeepTime=12*60;//ע�����ʱ�䶨ʱֵ��
			#endif
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=0;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Router->RS485Time=0x60;//�ɼ�����485���ʱֵ��(BCD)
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
	#else
		//�ͼ�ģʽ
			i=MRR(ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
			if(i==0x5a)//����ִ��
			{//����ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
				Terminal_Router->KeepTime=120;//ע�����ʱ�䶨ʱֵ��
			}
			else
			{//ÿ��ִ��
				Terminal_Router->n=1;//�ִ�1-3=�����ѱ�,0=ÿ���ѱ�
				Terminal_Ram->AutoSearch_S_Timer=60;//28 �ز��Զ��ѱ��붨ʱ��
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//��23:00:00��ʱ��
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//�Զ��ѱ����
				}
				Terminal_Router->KeepTime=i;//ע�����ʱ�䶨ʱֵ��
			}
			UARTCtrl->AutoSearchTask=4;//�Զ��ѱ�����
	#endif
			break;
		case 2://�����㲥(�ɼ�����485���)
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 3://����
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 4://����ӽڵ�����ע��
			MW(ADDR_TYMDHMS,((u32)p8txbuff)+128,6);//��ʼʱ��	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8txbuff)+128+6,2);//����ʱ��(min) ��
			//�ӽڵ��ط�����	BIN	1,����ȴ�ʱ��Ƭ����(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 5://����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
					break;
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 6://�Զ��ѱ����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//��ʱ��ʱ
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 �ز��Զ��ѱ��붨ʱ��
			{
				UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
				break;
			}
			break;
		case 7://��·������״̬
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 8://��·������״̬����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrl->AutoSearchReCount=0;//�Զ��ѱ������ط�����
					i=p8rxbuff[13];
					i&=2;
					if(i==0)
					{//����״̬��:D1=������־(0Ϊֹͣ����,1=���ڹ���),D0=·��ѧϰ(0=δ���,1=ѧϰ���)
						goto AutoSearch_End;//�Զ��ѱ����
					}
					Terminal_Ram->AutoSearch_S_Timer=1*60;//28 �ز��Զ��ѱ��붨ʱ��
					UARTCtrl->AutoSearchTask=6;//�Զ��ѱ�����
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			goto AutoSearch_End;//�Զ��ѱ����
		case 100://��ֹ����ע��
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			UARTCtrl->AutoSearchTask++;//�Զ��ѱ�����
			break;
		case 101://��ֹ����ע�����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//�Զ��ѱ����
				}
				p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//�Զ��ѱ������ط�����
			{
				UARTCtrl->AutoSearchReCount++;//�Զ��ѱ������ط�����
				UARTCtrl->AutoSearchTask--;//�Զ��ѱ�����
				break;
			}
			
AutoSearch_End://�Զ��ѱ����
			WarningOccur(PORTn,(u8 *)"�ز��Զ��ѱ����");//�澯����,��ڸ澯����͸澯�ַ���
			UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=��ʼ,2...����
			UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B6=,B7=����
			UARTCtrl->RMCount=0;//�����������=0
			UARTCtrl->ReRMCount=0;//���������ط�����
			UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
			if(MRR(ADDR_AFN05F149,1)==0x55)
			{//��F149����
				if(MRR(ADDR_AFN05F149+2+3,1)==0x5a)//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				{//����ִ��
					MWR(0xaa,ADDR_AFN05F149+2+3,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
				}
			}
			//�������
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		#if (USER/100)==0//���Ժ�ͼ�
			Terminal_Router->StopFlags&=~(1<<13);//��ͣ·��
			Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
		#else
			//��λģ��
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}
#endif

//0x68
//�ܳ���L
//�ܳ���L
//������
//��Ϣ��R-1
//��Ϣ��R-2
//��Ϣ��R-3
//��Ϣ��R-4
//��Ϣ��R-5
//��Ϣ��R-6
//��ַ��A1,A2,A3
//Ӧ�ù�����AFN
//���ݵ�Ԫ��ʶDT1
//���ݵ�Ԫ��ʶDT2
//���ݵ�Ԫ
//CS
//0x16


