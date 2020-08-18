

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../TEST/LOG.h"


#include "../DL698/DL698_JSON.h"


void RS485_TxLED(u32 PORTn)//RS485 LED����ָʾ��
{
	u32 i;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//LED����ָʾ��
	x=UARTCtrl->TxByte;
	x*=(11*1000);//ms
	i=UARTCtrl->BpsCtrl;
	i>>=5;
	i&=0xf;
	switch(i)
	{
		case 0://0=300,
			x/=300;
			break;
		case 1://1=600,
			x/=600;
			break;
		case 2://2=1200,
			x/=1200;
			break;
		case 3://3=2400,
			x/=2400;
			break;
		case 4://4=4800,
			x/=4800;
			break;
		case 5://5=7200,
			x/=7200;
			break;
		case 6://6=9600,
			x/=9600;
			break;
		case 7://7=19200
		default:
			x/=19200;
			break;
	}
	x/=10;
	if(x<2)
	{
		x=2;
	}
	if(PORTn==RS485_1PORT)
	{
		Terminal_Ram->RS485_1_TxLED_MS_Timer=x;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	}
	if(PORTn==RS485_2PORT)
	{
		Terminal_Ram->RS485_2_TxLED_MS_Timer=x;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
	}
}


void RS485_RxTx(u32 PORTn)//RS485���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
{
	u32 i;
  UARTCtrl_TypeDef* UARTCtrl;
	u16* p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			break;
		case 1://1=���1֡����
			break;
		case 2://2=���ճ�ʱ(�����)
			break;
		case 3://3=��������
			if(p16timer[0]!=0)
			{
				break;
			}
		#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				//p16timer[0]=UARTCtrl->TxToRxTimerOver;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
				p16timer[0]=RMRS485OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
				UARTCtrl->Task=5;
			}
			break;
		case 5://5=���ڽ���
			i=RM_Rx(PORTn);//�������֡;���:PORTn;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
			switch(i)
			{
				case 0://���ڽ���
					return;
				case 1://���1֡����
					UARTCtrl->Task=1;
					p16timer[0]=30/10;//��֡������ʱ
				
		#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
					break;
				default://���ճ�ʱ
					UARTCtrl->Task=2;
					return;
			}
			break;
	}
}


void RM_RS485(u32 PORTn)//RS485����
{
	u32 i;
	u32 x;
	//u8* p8tx;
	//u8* p8rx;
	u16* p16timer;
	u8* pCSD;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	#if(USER/100)==15//����
	u8* p8;
	#endif

	x=x;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	if(PORTn!=ACSAMPLEPORT)
	{//���ǽ���
		UARTCtrl->PowerUpDealy=1;
		if(UARTCtrl->PowerUpDealy==0)//�ϵ���ʱ:0=û��ʱ,!=0�ѵ���ʱ
		{
			p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
			if(p16timer[0]!=0x0)//10 �ϵ����ʱ�����붨ʱ��
			{
				CopyString((u8 *)"485 �ȴ��ϵ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				pCSD=(u8*)ADDR_DATABUFF;
				if(PORTn==RS485_1PORT)
				{
					i=1;
				}
				else
				{
					i=2;
				}
				pCSD[3]=i;
				pCSD[12]=0;
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
			UARTCtrl->PowerUpDealy=1;//�ϵ���ʱ:0=û��ʱ,!=0�ѵ���ʱ
		}
	}

	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			if(PORTn!=ACSAMPLEPORT)
			{//���ǽ���
				//����ʱ������������
				i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				if(i)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				{
					PROXY_TASK_RS485(PORTn);//��������RS485
					return;
				}
				i=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
				if((i&0x7f)==1)
				{
					i++;
					UARTCtrl->BroadcastTimeTask=i;
					return;
				}
				i=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
				if(i==1)
				{
					i++;
					UARTCtrl->AutoSearchTask=i;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
					return;
				}
				i=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
				if(i==1)
				{
					i++;
					UARTCtrl->AutoReportTask=i;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
					return;
				}
			}
			UARTCtrl->Task=3;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			break;
		case 3://3=��������
			if(p16timer[0]!=0)//��֡����������ʱ
			{
				return;
			}
			//p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			//p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			if(UARTCtrl->ReRMCount>=3)//�����ط�����
			{
				RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
				UARTCtrl->ReRMCount=0;//�����ط�����
				UARTCtrl->RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
				UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
				//�µ��ܱ�
				RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
			}
			UARTCtrl->RMMeterCountAuto=0;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
			
			if(UARTCtrl->CONNECT==0)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
			{
				TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������;����:0��ʾ�޲ɼ�����
				i=UARTCtrl->TaskID;
				if(i==0)
				{
					p16timer[0]=2000/10;//�´μ���TASK()��1���,�����Ӱ��˵��ٶ�
					UARTCtrl->Task=0;//����1��
					if(PORTn!=ACSAMPLEPORT)
					{//���ǽ���
						RM_NUM_Display(PORTn);//����������ʾ
						if(PORTn==RS485_4PORT)
						{
							WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
						}
					}
			#if DEBUG==1//0=����,1=����
					else
					{//�ǽ���
						RM_NUM_Display(PORTn);//����������ʾ
					}
			#endif
					return;
				}
			}
			i=RM_TxDATA_Manage(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			if(UARTCtrl->TxByte==0)
			{
				UARTCtrl->ReRMCount=0;//�����ط�����
				return;
			}
		#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			
			UARTCtrl->ReRMCount++;//�����ط�����
			if(PORTn==ACSAMPLEPORT)
			{//����
			#if iMeterProtocol==2//�ڲ����ɳ����Լ:2=DL/T645-2007��3=DL/T698.45
			//�ڲ����ɳ����Լ:2=DL/T645-2007
				//	DataType_CSD,1,//0=OAD,1=ROAD
				//	0x50,0x02,0x02,0x00,//OAD
				//	2,//��������
				//	0x20,0x21,0x02,0x00,//1 ����ʱ��
				//	0x20,0x00,0x02,0x00,//2 ��ѹ
					i=UARTCtrl->RMCSD[1];
					if(i==1)
					{//1=ROAD
						i=UARTCtrl->RMCSD[2];
						i<<=8;
						i|=UARTCtrl->RMCSD[3];
						i<<=8;
						i|=UARTCtrl->RMCSD[4];
						i<<=8;
						i|=UARTCtrl->RMCSD[5];
						if(i==0x50020200)
						{//���Ӷ���(�ڲ���������)
						#if (USER/100)==9//����,5���ӵ�ѹ����
							if(UARTCtrl->RM_DI_TI_Count!=0)//��ʱ�����ɼ�ʱ�ļ������
							{//����ֻ����ǰһ����
								UARTCtrl->ReRMCount=0;//�����ط�����
								UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
								UARTCtrl->Task=0;//����1��
								return;
							}
							UARTCtrl->RM_DI_TI_Count=0xF0;//����ֻ����ǰһ����
							p8rx[8]=0x91;//������
							p8rx[9]=36;//���ݳ���
							UARTCtrl->RM_DI=0x06010001;
							p8rx[10+0]=0x01;//DI
							p8rx[10+1]=0x00;
							p8rx[10+2]=0x01;
							p8rx[10+3]=0x06;
						//���ɼ�¼��ʼ�룺A0H��A0H���� E0H��E0H�������ݿ鲻��ȷ����2 �ֽڣ� 
							p8rx[10+4]=0xA0;
							p8rx[10+5]=0xA0;
						//���ɼ�¼�ֽ�����1 �ֽڣ�ʮ�����ƣ��� 
							p8rx[10+6]=36-7-2;
						//���ɼ�¼�洢ʱ�䣺�ꡢ�¡��ա�ʱ����,  5 �ֽڣ�
							i=Comm_Ram->DL698YMDHMS[5];
							i/=5;
							i*=5;
							i=hex_bcd(i);
							p8rx[10+7]=i;
							p8rx[10+8]=Comm_Ram->MYMDHMS[2];
							p8rx[10+9]=Comm_Ram->MYMDHMS[3];
							p8rx[10+10]=Comm_Ram->MYMDHMS[4];
							p8rx[10+11]=Comm_Ram->MYMDHMS[5];
							
						//��ѹ��������Ƶ�ʣ�17 �ֽڣ� 
							p8tx=(u8*)ADDR_UA+2;//A��ѹ
							p8rx[10+12]=p8tx[0];
							p8rx[10+13]=p8tx[1];
							p8tx=(u8*)ADDR_UB+2;//B��ѹ
							p8rx[10+14]=p8tx[0];
							p8rx[10+15]=p8tx[1];
							p8tx=(u8*)ADDR_UC+2;//C��ѹ
							p8rx[10+16]=p8tx[0];
							p8rx[10+17]=p8tx[1];
							p8tx=(u8*)ADDR_IA+1;//A����
							p8rx[10+18]=p8tx[0];
							p8rx[10+19]=p8tx[1];
							p8rx[10+20]=p8tx[2];
							p8tx=(u8*)ADDR_IB+1;//B����
							p8rx[10+21]=p8tx[0];
							p8rx[10+22]=p8tx[1];
							p8rx[10+23]=p8tx[2];
							p8tx=(u8*)ADDR_IC+1;//C����
							p8rx[10+24]=p8tx[0];
							p8rx[10+25]=p8tx[1];
							p8rx[10+26]=p8tx[2];
							p8tx=(u8*)ADDR_FREQUENCY;//Ƶ��
							p8rx[10+27]=p8tx[0];
							p8rx[10+28]=p8tx[1];
						//��ָ��룺AAH��1 �ֽڣ� 
							p8rx[10+29]=0xaa;
						//�С��޹����ʣ�24 �ֽڣ� 
						//��ָ��룺AAH��1 �ֽڣ� 
							p8rx[10+30]=0xaa;
						//����������8 �ֽڣ� 
						//��ָ��룺AAH��1 �ֽڣ� 
							p8rx[10+31]=0xaa;
						//�С��޹��ܵ��ܣ�16 �ֽڣ� 
						//��ָ��룺AAH��1 �ֽڣ� 
							p8rx[10+32]=0xaa;
						//�������޹��ܵ��ܣ�16 �ֽڣ�
						//��ָ��룺AAH��1 �ֽڣ� 
							p8rx[10+33]=0xaa;
						//��ǰ������6 �ֽڣ� 
						//��ָ��룺AAH��1 �ֽڣ�
							p8rx[10+34]=0xaa;
						//���ɼ�¼�ۼ�У���룺1 �ֽڣ��ӵ�һ�� A0H ��ʼ�����һ�����ݿ�����룩��
							p8rx[10+35]=0;
						//���ɼ�¼�����룺E5H,1 �ֽڡ�
							p8rx[10+35]=0xE5;
							
						//���ݼ�0x33
							for(i=0;i<36;i++)
							{
								p8rx[10+i]+=0x33;
							}
							UARTCtrl->Task=1;//1=�������
						#if DEBUG==1//0=����,1=����
							RM_ADDR_Display(ACSAMPLEPORT);//�����ַ��ʾ
						#endif
							return;
						#endif
							
						}
					}
					
					UARTCtrl->Task=1;//1=�������
					UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					p32=(u32 *)(ADDR_Meter_ENTRY);
					/*
					#ifdef DEBUG_JSON
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					myprintf("[%s:%d]:���ɷ���\n",(u32)__func__,(u32)__LINE__,0);
					for(i=0;i<50;i++)
					{	
						myprintf("%02x ",p8rx[i],0,0);
					}
					myprintf("\n",0,0,0);
					#endif
					*/
					(*(void(*)())p32[130])((u32)PORTn);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
					/*
					#ifdef DEBUG_JSON
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					myprintf("[%s:%d]���ɽ���\n",(u32)__func__,(u32)__LINE__,0);
					for(i=0;i<50;i++)
					{	
						myprintf("%02x ",p8tx[i],0,0);
					}
					myprintf("\n",0,0,0);
					#endif
					*/
					UARTCtrl->Task=1;//1=�������
			#else
			//�ڲ����ɳ����Լ:3=DL/T698.45
				UARTCtrl->Task=0;//0=����
				UARTCtrl->TxByte=0;
				//Uart_698_Data(PORTn);//UART_698����
					Uart_698_Data(PORTn,0);//UART_698����
				if(UARTCtrl->Task==2)
				{//2=�ȴ���֡���ݷ���
					MR((u32)p8rx,(u32)p8tx,UARTCtrl->TxByte);
					UARTCtrl->Task=1;//1=�������
				}
			#endif
			}
			else
			{//RS485
				Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
				Start_UARTn_Tx(PORTn);//����UART�ڷ���
				UARTCtrl->Task=4;
			}
			
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//RS485
				RS485_TxLED(PORTn);//RS485 LED����ָʾ��
				RM_ADDR_Display(PORTn);//�����ַ��ʾ
				i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
				i>>=8;
				i&=0xff;
				RM_Monitor_TxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Tx=��������
			}
	#if DEBUG==1//0=����,1=����
			if(PORTn==ACSAMPLEPORT)
			{
				RM_ADDR_Display(PORTn);//�����ַ��ʾ
			}
	#endif
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->Task=5;
				p16timer[0]=RMRS485OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			}
			break;
		case 5://5=���ڽ���
			i=RM_Rx(PORTn);//�������֡;���:PORTn;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					UARTCtrl->Task=0;
					break;
				default://2��ʱ
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					return;
			}
			p16timer[0]=30/10;//��֡����������ʱ20MS��
		case 1://1=�������
		#if DEBUG_RS485_1toRS232==1//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//RS485
				i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
				i>>=8;
				i&=0xff;
				RM_Monitor_RxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Rx=��������
			}
			UARTCtrl->Task=0;//����1��
			i=RM_RxDATA_CHECK(PORTn);//֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����:0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			if((i==1)||(i==3))
			{//1��ַ��,3���ݱ�ʶ��
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
				return;
			}
			if(i==4)
			{//4�����ϱ�(�Ѵ���)
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//������շ���:0=������,!=0������(������)
			if(i==2)
			{//2�������(������)
			#if(USER/100)==17
				//���ߵȶ������ݵ�ָ��ʱ��������ݶ���ʱд������ݱ��ⲹ��ʱ���³���
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//�ɼ�����
				if((i==0x5002)&&(x==2))//������ʱ��ɼ��ķ�������
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:���յ�̨�屨���ǿ����ݵ�ʱ��\n",(u32)__func__,(u32)__LINE__,0);
					#endif
					UARTCtrl->ReRMCount=0;//�����ط�����
					UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
					return;
				}
			#endif
			#if (USER/100)==15//����
				//����:����������Ҫ֧��06000001��061000FF�ȿ鳭��
				if(UARTCtrl->RM_DI==0x06000001)
				{
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					return;
				}
			#endif
			#if (USER&100)==0//���Ժ��ʡ���Ժ����
//---
				//̨����Բɼ���ز����㳭��ɹ�����
				UARTCtrl->ReRMCount=0;//�����ط�����
				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
				return;
//---
			#else
//---
				//���ߵȶ������ݵ�ָ��ʱ��������ݶ���ʱд������ݱ��ⲹ��ʱ���³���
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//�ɼ�����
				if(!((i==0x5002)||(x==4)))
				{//���ǳ����Ӷ���(����������)�Ͳ���(2=�¼��ɼ�����ʱ����������ݶ��ϱ�)
					UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
					UARTCtrl->ReRMCount=0;//�����ط�����
					return;
				}
			#if (USER/100)==15//����
				if((i==0x5002)&&(x==2))
				{
					i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
					i>>=8;
					i&=0xff;
					p8=(u8*)ADDR_DATABUFF;
					Get_date_time_s(p8);
					i=YYMDHMS_Sub_hex(UARTCtrl->TaskTime[i].StdTime,p8);
					if(i<=600)//ʮ����������صĶ��ǿ����ݣ���һֱ�ٶ�������ʮ�����Ժ󣬽������ݱ��档
					{
						UARTCtrl->ReRMCount=0;//�����ط�����
						UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
						return;
					}				
				}
			#endif
				
//---
			#endif
			}
			RM_RxDATA(PORTn);//����������ݴ���

			UARTCtrl->ReRMCount=0;//�����ط�����
			if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
			{//�޺���֡
				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
			}
			return;
	
	}
}






