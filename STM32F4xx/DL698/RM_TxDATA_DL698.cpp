
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"
#if ((USER/100)==9)//����
//���Ķ�ȡ
u32 RM_SECURITY(u32 OI)//����ȫģʽ��,���:OI,����:(ͬ��ȫ����)b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
{
	return 0;
}
#else
//����+������֤���ȡ
u32 RM_SECURITY(u32 OI)//����ȫģʽ��,���:OI,����:(ͬ��ȫ����)b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
{
	switch(OI&0xf000)
	{
		case 0x0000://��ǰ����
			return 0;//����
		case 0x1000://�������
			return 0x81;//����+������֤��
		case 0x2000:
			if(OI==0x202c)
			{//��ǰǮ���ļ�
				return 0;//����
			}
			else
			{//����
				return 0x81;//����+������֤��
			}
		case 0x3000://�¼�
			return 0x81;//����+������֤��
		case 0x4000:
			switch(OI)
			{
				case 0x4000://����ʱ��
				case 0x4001://ͨ�ŵ�ַ
				case 0x4002://���
				case 0x4111://������
					return 0;//����
				default:
					return 0x81;//����+������֤��
			}
		case 0x5000://����
			return 0x81;//����+������֤��(���Ժ̨�����MAC��֤ʱ��ȡ��������)
		case 0x6000://�ɼ����
			return 0;//����
		case 0x7000://����
			return 0;//����
		case 0x8000://����
			return 0;//����
		case 0xf000://��֡����,ESAM,��ȫģʽ����,��������ӿ��豸���Զ���
			return 0;//����
		default:
			return 0;//����
	}
}
#endif


u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx)//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	u32 i;
	u32 x;
	u32 Byte;
	u32 YYMD;
	UARTCtrl_TypeDef * UARTCtrl;
#if (USER/100)==15//����
	u32 NUM_RCSD;
#endif	
//	u8* p8;
	u32 y;
	//u32 offset;
	u64 d64a;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//��ʼ�ַ���68H��  1Byte
	p8tx[0]=0x68;
//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
//������C  1Byte
	p8tx[3]=0x43;
//��ַ��A
	p8tx[4]=5;
	p8tx[5]=UARTCtrl->RMAddr[2]>>8;
	p8tx[6]=UARTCtrl->RMAddr[2];
	p8tx[7]=UARTCtrl->RMAddr[1]>>8;
	p8tx[8]=UARTCtrl->RMAddr[1];
	p8tx[9]=UARTCtrl->RMAddr[0]>>8;
	p8tx[10]=UARTCtrl->RMAddr[0];
//�ͻ�����ַCA  1Byte
	p8tx[11]=0;
//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
//�ճ�3�ֽڣ�����ȫģʽ��ȡʱ��
	p8tx+=3;
//APDU
	i=UARTCtrl->TaskID;

	switch((i>>8)&0xff)//�����
	{
	#if(USER/100)==17//����ϵ
		default:
			//if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			
			if(((UARTCtrl->TaskID>>20)&0xf)==5)
			{
				p8tx[1]=0x15;
#if 0
				x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
				d64a=MRR((u32)p8tx+2,6);
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList1->List1[y].Addr,6);
					if(d64a==d64b)
					{
						d64b=MRR((u32)&NoteList1->List1[y].CollectorAddr,6);
//						d64b=R_Inverse(d64b,6);
//						MWR(d64b,(u32)&UARTCtrl->RMAddr,6);
//						p8tx[2]=UARTCtrl->RMAddr[2]>>8;
//						p8tx[3]=UARTCtrl->RMAddr[2];
//						p8tx[4]=UARTCtrl->RMAddr[1]>>8;
//						p8tx[5]=UARTCtrl->RMAddr[1];
//						p8tx[6]=UARTCtrl->RMAddr[0]>>8;
//						p8tx[7]=UARTCtrl->RMAddr[0];
						MWR(d64b,(u32)p8tx+2,6);
						break;//ԭ�Ѵ���
					}
				}
				if(y==x)
				{
					myprintf("[%s:%d]:û������ӽڵ���Ϣ\n",(u32)&__func__,(u32)__LINE__,0);
					return 0;
				}
//				p8=(u8*)ADDR_Meter_Collector;
//				offset=0;
//				for(x=0;x<NUM_RMmax;x++)
//				{
//					for(y=0;y<6;y++)
//					{
//						if((p8[y+offset]!=p8tx[2+y])&&p8[y+offset]!=0)
//						{
//							break;
//						}
//					}
//					if(y!=6)//����ͬ���Ҳ���0
//					{
//						offset+=12;
//					}
//					else
//					{
//						MR((u32)p8tx+2,(u32)p8+offset+6,6);
//						MR((u32)&UARTCtrl->RMAddr,(u32)p8+offset+6,6);
//						Data_Inverse((u32)&UARTCtrl->RMAddr,6);
//						break;
//					}
//					
//				}
				myprintf("[%s:%d]:ϵͳ��ǰ���ڳ����ɼ�����ַ\n",(u32)&__func__,(u32)__LINE__,0);
				for(i=6;i>0;i--)
				{
					myprintf("%02x",NoteList1->List1[y].CollectorAddr[i-1],0,0);
				}
				myprintf("\n",0,0,0);
#else
				
				x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
				//myprintf("[%s:%d]:Terminal_Router->NoteNum=%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
				d64a=MRR((u32)p8tx+2,6);
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList->List[y].Addr,6);
					if(d64a==d64b)
					{
						d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
						MWR(d64b,(u32)p8tx+2,6);
						break;//ԭ�Ѵ���
					}
				}
				if(y==x)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:û������ӽڵ���Ϣ\n",(u32)&__func__,(u32)__LINE__,0);
					#endif
					return 0;
				}
				if(d64b==0)//�ɼ�����ַ����0��ʱ��,������ֻ,Ѱ����һֻ��Ч�ɼ�����ַ
				{
					if((y+1)>=NUM_RMmax)
					{
						i=0;
					}else
					{
						i=y+1;
					}
					for(;i<NUM_RMmax;i++)
					{
						d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
						if(d64b!=0)
						{
							MWR(d64b,(u32)p8tx+2,6);
							break;
						}
					}
					if(i>=NUM_RMmax)
					{
						for(i=0;i<y;i++)
						{
							d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
							if(d64b!=0)
							{
								MWR(d64b,(u32)p8tx+2,6);
								break;
							}
						}
						if(i>=y)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:û��һֻ�����������Ч�ɼ�����ַ\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							return 0;
						}
					}
				}
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:ϵͳ��ǰ���ڳ����ɼ�����ַ\n",(u32)&__func__,(u32)__LINE__,0);
				for(i=6;i>0;i--)
				{
					myprintf("%02x",NoteList->List[y].CollectorAddr[i-1],0,0);
				}
				myprintf("\n",0,0,0);
				#endif
#endif
				switch(UARTCtrl->res23)
				{
					case 0://��ʼ�㳭
						p8tx[14]=7;//����һ�����󷽷�
						p8tx[15]=1;//
						//UARTCtrl->PIID++;
						//p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						p8tx[16]=0;//PIIDд��
						Byte=17;
						
						p8tx[17]=0x60;
						p8tx[18]=0x43;
						p8tx[19]=0x7f;
						p8tx[20]=0;
						p8tx[21]=0x11;
						p8tx[22]=1;
						Byte+=6;
						break;
					case 1://�ϱ����ɸ���¼�Ͷ������ԣ��к���֡
					case 2://�ϱ����ɸ���¼�Ͷ������ԣ��޺���֡
					case 10:
					case 11:
						//08 02 00				// ReportResponseRecordList
						//01					// 1��OAD
						//60 12 03 00			// ��Ӧ�ϱ���OAD
						p8tx[0]=0x03;
						p8tx[14]=8;//�ϱ����ɸ���¼�Ͷ���������Ӧ֡
						p8tx[15]=2;//
						//p8tx[16]=UARTCtrl->PIID;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						
						p8tx[17]=1;
						p8tx[18]=0x60;
						p8tx[19]=0x12;
						p8tx[20]=0x03;
						p8tx[21]=0;
						Byte+=5;					
						break;
					case 3://ReportNotificationSimplifyRecord�ϱ���ʽ���к���֡
					case 8:
						//08 06 		// ReportResponseSimplifyRecord
						//02 
						p8tx[0]=0x03;
						p8tx[14]=8;//�ϱ����ɸ���¼�Ͷ���������Ӧ֡
						p8tx[15]=6;//
						//p8tx[16]=UARTCtrl->PIID;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						break;
					case 4:///ReportNotificationSimplifyRecord�ϱ���ʽ���޺���֡
					case 9:
						p8tx[14]=7;//����һ�����󷽷�
						p8tx[15]=1;//
						//UARTCtrl->PIID++;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						//p8tx[16]=UARTCtrl->PIID;
						Byte=17;
						
						p8tx[17]=0x60;
						p8tx[18]=0x43;
						p8tx[19]=0x7f;
						p8tx[20]=0;
						p8tx[21]=0x11;
						p8tx[22]=0;
						Byte+=6;
						break;
					case 5://�ɼ�����������Ӧ
						break;
					default:
						break;
				}
				
				
				p8tx[Byte]=0;
				Byte+=1;
			//��ȫģʽ��ȡ
				p8tx-=3;//�ճ���3�ֽ�
				x=Byte-14;//APDU����
				if((p8tx[4]&0x30)!=0)//�߼���ַ��Ϊ0����ʾ������Ӧ�߼��豸���ɼ�������STA
				{//���Ķ�ȡ
					for(i=0;i<x;i++)
					{
						p8tx[14+i]=p8tx[17+i];
					}
				}
				else
				{//����+������֤���ȡ
				#if RM_SECURITY_MeterNo==0//0=��,1=��;DL698��ȫģʽ����ʱ�ȳ������ܱ���(���ܱ������ڼ�У����)
					UARTCtrl->RM_Rx_date_40020200[0]=2;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
				#else
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
						UARTCtrl->RM_Rx_date_40020200[0]=2;
				#endif
					if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
					{//�������Ķ����ܱ���
						UARTCtrl->RM_Rx_date_40020200[0]=1;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
						p8tx[14]=5;//GET-Request
						p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						//OAD
						p8CSD[1]=0x00;
						p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//���ܱ��� �� ���ܱ�ESAM���к�
						p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
						p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
						p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
						for(i=0;i<4;i++)
						{
							p8tx[Byte+i]=p8CSD[2+i];
						}
						Byte+=4;
					//ʱ���ǩ
						p8tx[Byte]=0;
						Byte+=1;
					//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
					//�����ַ���16H��
						Byte+=3;
						Byte-=2;
						p8tx[1]=Byte;
						p8tx[2]=Byte>>8;
						Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
						return Byte+2;
					}
					p8tx[14]=0x10;
					p8tx[15]=0;//����Ӧ�����ݵ�Ԫ [0] ����Ӧ�����ݵ�Ԫ [1]
					p8tx[16]=x;//octet-string����
					Byte+=3;
					p8tx[Byte]=1;//����� [1]  RN
					p8tx[Byte+1]=16;//����
					Byte+=2;
					i=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
					if(i)
					{
						UART7Ctrl->Task=0;//�ص�Դ����
					}
					MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
					MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//����ȫģʽʱ���͸�����16�ֽ������
					Byte+=16;
				}
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				Byte+=3;
				Byte-=2;
				p8tx[1]=Byte;
				p8tx[2]=Byte>>8;
				Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����		
				return Byte+2;
				
			}

			break;
	#else
			default:
				break;
	#endif
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			p8tx[14]=5;//GET-Request
		#if (USER/100)==15//����
			p8tx[15]=3;
		#else
			p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
		#endif
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		#if (USER/100)==15//����
			p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
			p8tx[Byte+1]=1;//�ϵ�1�μ�¼  unsigned
			NUM_RCSD=UARTCtrl->RMCSD[6];
			p8tx[Byte+2]=NUM_RCSD;
			for(i=0;i<NUM_RCSD;i++)
			{
				p8tx[Byte+3+i*5]=0;
				p8tx[Byte+4+i*5]=p8CSD[7+i*4];
				p8tx[Byte+5+i*5]=p8CSD[8+i*4];
				p8tx[Byte+6+i*5]=p8CSD[9+i*4];
				p8tx[Byte+7+i*5]=p8CSD[10+i*4];
			}
			//p8tx[Byte+2]=0;//RCSD
			//Byte+=3;
			Byte+=3+NUM_RCSD*5;
		#endif
		//ʱ���ǩ
			p8tx[Byte]=0;
			Byte+=1;
		//��ȫģʽ��ȡ
			p8tx-=3;//�ճ���3�ֽ�
			x=Byte-14;//APDU����
			i=(p8CSD[2]<<8)|p8CSD[3];
			i=RM_SECURITY(i);//����ȫģʽ��,���:OI,����:(ͬ��ȫ����)b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
			if(i==0)
			{//���Ķ�ȡ
				for(i=0;i<x;i++)
				{
					p8tx[14+i]=p8tx[17+i];
				}
			}
			else
			{//����+������֤���ȡ
			#if RM_SECURITY_MeterNo==0//0=��,1=��;DL698��ȫģʽ����ʱ�ȳ������ܱ���(���ܱ������ڼ�У����)
				UARTCtrl->RM_Rx_date_40020200[0]=2;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
			#else
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
						UARTCtrl->RM_Rx_date_40020200[0]=2;
			#endif
				if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
				{//�������Ķ����ܱ���
					UARTCtrl->RM_Rx_date_40020200[0]=1;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
					p8tx[14]=5;//GET-Request
					p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
					p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
					Byte=17;
					//OAD
					p8CSD[1]=0x00;
					p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//���ܱ��� �� ���ܱ�ESAM���к�
					p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
					p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
					p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
					for(i=0;i<4;i++)
					{
						p8tx[Byte+i]=p8CSD[2+i];
					}
					Byte+=4;
				//ʱ���ǩ
					p8tx[Byte]=0;
					Byte+=1;
				//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
				//�����ַ���16H��
					Byte+=3;
					Byte-=2;
					p8tx[1]=Byte;
					p8tx[2]=Byte>>8;
					Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					return Byte+2;
				}
				p8tx[14]=0x10;
				p8tx[15]=0;//����Ӧ�����ݵ�Ԫ [0] ����Ӧ�����ݵ�Ԫ [1]
				p8tx[16]=x;//octet-string����
				Byte+=3;
				p8tx[Byte]=1;//����� [1]  RN
				p8tx[Byte+1]=16;//����
				Byte+=2;
				i=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					UART7Ctrl->Task=0;//�ص�Դ����
				}
				MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
				MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//����ȫģʽʱ���͸�����16�ֽ������
				Byte+=16;
			}
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			Byte+=3;
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
			return Byte+2;
			
//			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
//			p8tx[14]=5;//GET-Request
//			p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
//			UARTCtrl->PIID++;
//			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
//			Byte=17;
//			//OAD
//			for(i=0;i<4;i++)
//			{
//				p8tx[Byte+i]=p8CSD[2+i];
//			}
//			Byte+=4;
//			//ѡ�񷽷�
//			p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
//			p8tx[Byte+1]=UARTCtrl->RM_DI_N;//�ϵ�n�μ�¼  unsigned
//			Byte+=2;
//			//RCSD��SEQUENCE OF����
//			p8tx[Byte]=0;//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
//			Byte++;
//		//ʱ���ǩ
//			p8tx[Byte]=0;
//		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//		//�����ַ���16H��
//			Byte+=4;
//			
//			Byte-=2;
//			p8tx[1]=Byte;
//			p8tx[2]=Byte>>8;
//			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
//			return Byte+2;


//		default:
//			break;
	}

	switch((i>>24)&0xf)//��������
	{
		case 1://��ͨ�ɼ�����
		#if (USER/100)==17//����ϵ
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			//if(((UARTCtrl->TaskID>>20)&0xf)==5)
				p8CSD[1]=1;
		#endif
			p8tx[14]=5;//GET-Request
			if(p8CSD[1]==0)
			{//OAD
				p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
			}
			else
			{//ROAD
/*
				//���е��������
				i=UARTCtrl->RMCSD[2+0]<<24;
				i|=UARTCtrl->RMCSD[2+1]<<16;
				i|=UARTCtrl->RMCSD[2+2]<<8;
				i|=UARTCtrl->RMCSD[2+3];
				if(i==0x50020200)
				{//���Ӷ���
					i=UARTCtrl->TaskID;
					i=(i>>8)&0xff;//�����
					i=UARTCtrl->MeterCount[i];//ȡ������
					p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					p8=Get_Element(p8,2,8,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{
						i=p8[1];//ȡ���߷�ʽ
						if(i==1)//δ֪��0�������ࣨ1�����������ߣ�2�����������ߣ�3��
						{//���������,��ʵʱ����
							p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
							UARTCtrl->PIID++;
							p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
							Byte=17;
							i=UARTCtrl->RM_DI;
							p8tx[Byte+0]=i>>24;
							p8tx[Byte+1]=i>>16;
							p8tx[Byte+2]=i>>8;
							p8tx[Byte+2]=i>>0;
							Byte+=4;
							break;
						}
					}
				}
*/
				p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				//OAD
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
				switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
				{
					default:
					case 0://0=�ɼ���ǰ����
		//				p8tx[Byte]=0;//RSD�� ѡ�񷽷�0:��ѡ��       [0]  NULL
		//				Byte+=1;
		//				break;
					case 1://1=�ɼ��ϵ�N��
						p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
						p8tx[Byte+1]=UARTCtrl->RM_DI_N;//�ϵ�n�μ�¼  unsigned
						Byte+=2;
						break;
					case 2://2=������ʱ��ɼ�
						p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
						p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						for(i=0;i<4;i++)//ʱ��:��������
						{
							p8tx[Byte+i]=Comm_Ram->DL698YMDHMS[i];
						}
						p8tx[Byte+4]=0x00;//ʱ��:ʱ
						p8tx[Byte+5]=0x00;//ʱ��:��
						p8tx[Byte+6]=0x00;//ʱ��:��
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
						switch(i)
						{
							case 0x5004://�ն���
								break;
							case 0x5006://�¶���
								p8tx[Byte+3]=0x01;//�̶�Ϊ1��
								break;
						}
						Byte+=7;
						break;
					case 3://3=��ʱ�����ɼ�
						//����������
						p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
						p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						DateSaveTI(PORTn,p8tx+Byte);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
						Byte+=7;
						break;
					case 4://4=RetryMetering ����(��ͬ,2=������ʱ��ɼ�)
						x=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
						switch(x)
						{
							case 0x5002://���Ӷ���(������������)
								p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
								p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
								p8tx[Byte+2]=0x21;
								p8tx[Byte+3]=0x02;
								p8tx[Byte+4]=0x00;
								p8tx[Byte+5]=DataType_date_time_s;//date_time_s
								Byte+=6;
								DateSaveTI(PORTn,p8tx+Byte);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
								Byte+=7;
								break;
							case 0x5004://�����ն���
								p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
								p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
								p8tx[Byte+2]=0x21;
								p8tx[Byte+3]=0x02;
								p8tx[Byte+4]=0x00;
								p8tx[Byte+5]=DataType_date_time_s;//date_time_s
								Byte+=6;
								YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
								//i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
								#if (USER/100)==17
								myprintf("[%s:%d]:����\n",(u32)&__func__,(u32)__LINE__,0);
								if(UARTCtrl->RM_DI_TI_Count==0)
								{
									UARTCtrl->RM_DI_TI_Count=1;
								}
								i=(UARTCtrl->RM_DI_TI_Count-1);
								#else
								i=UARTCtrl->RM_DI_TI_Count;
								#endif
								while(i--)
								{
									YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
								}
								p8tx[Byte+0]=YYMD>>24;
								p8tx[Byte+1]=YYMD>>16;
								p8tx[Byte+2]=YYMD>>8;
								p8tx[Byte+3]=YYMD>>0;
								p8tx[Byte+4]=0x00;//ʱ��:ʱ
								p8tx[Byte+5]=0x00;//ʱ��:��
								p8tx[Byte+6]=0x00;//ʱ��:��
								Byte+=7;
								break;
							case 0x5006://�¶���
							default:
								return 0;
						}
						break;
				}
//			#if (USER/100)==17//����ϵ
//				x=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
//				switch(x)
//				{
//					case 0x5005://�����ն���
//						p8tx[15]=2;//��ȡһ�������б���������             [2] GetRequestNormalList
//						Byte-=7;
//						x=p8CSD[6];//RCSD��SEQUENCE OF����
//						p8tx[Byte]=x;
//						Byte++;
//						//Byte-=3;
//						for(i=0;i<x;i++)
//						{
//							p8tx[Byte]=p8CSD[7+(i*4)+0];//OAD
//							p8tx[Byte+1]=p8CSD[7+(i*4)+1];//OAD
//							p8tx[Byte+2]=p8CSD[7+(i*4)+2];//OAD
//							p8tx[Byte+3]=p8CSD[7+(i*4)+3];//OAD
//							Byte+=4;
//						}
//						break;
//					default:
//						if((((UARTCtrl->TaskID>>20)&0xf)!=5)||(Terminal_Router->MinuteCollect!=1))
//						{
//							x=p8CSD[6];//RCSD��SEQUENCE OF����
//							p8tx[Byte]=x;
//							Byte++;
//							for(i=0;i<x;i++)
//							{
//								p8tx[Byte+0]=0;//����=OAD
//								p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
//								p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
//								p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
//								p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
//								Byte+=5;
//							}	
//						}
//						break;
//						
//				}
//			#else
				x=p8CSD[6];//RCSD��SEQUENCE OF����
				p8tx[Byte]=x;
				Byte++;
				for(i=0;i<x;i++)
				{
					p8tx[Byte+0]=0;//����=OAD
					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
					Byte+=5;
				}
//			#endif
//				x=p8CSD[6];//RCSD��SEQUENCE OF����
//				p8tx[Byte]=x;
//				Byte++;
//				for(i=0;i<x;i++)
//				{
//					p8tx[Byte+0]=0;//����=OAD
//					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
//					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
//					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
//					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
//					Byte+=5;
//				}
			}
			break;
		case 2://�¼��ɼ�����
			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
			p8tx[14]=5;//GET-Request
			p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
			//ѡ�񷽷�
			p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
			p8tx[Byte+1]=1;//�ϵ�n�μ�¼  unsigned
			Byte+=2;
			//RCSD��SEQUENCE OF����
//			p8CSD[5]=0;//RCSD��SEQUENCE OF����//!!!����̨ģ���֧��SEQUENCE OF����=0����
//			p8tx[Byte]=0;
//			Byte++;
			x=p8CSD[6];//RCSD��SEQUENCE OF����
			p8tx[Byte]=x;
			Byte++;
			for(i=0;i<x;i++)
			{
				p8tx[Byte+0]=0;//����=OAD
				p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
				p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
				p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
				p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
				Byte+=5;
			}
			break;
		case 3://͸������
			
			return 0;
		default:
			return 0;
	}
	
//ʱ���ǩ
	p8tx[Byte]=0;
	Byte+=1;
//��ȫģʽ��ȡ
	p8tx-=3;//�ճ���3�ֽ�
	x=Byte-14;//APDU����
	i=(p8CSD[2]<<8)|p8CSD[3];
	i=RM_SECURITY(i);//����ȫģʽ��,���:OI,����:(ͬ��ȫ����)b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
	if(i==0)
	{//���Ķ�ȡ
		for(i=0;i<x;i++)
		{
			p8tx[14+i]=p8tx[17+i];
		}
	}
	else
	{//����+������֤���ȡ
	#if RM_SECURITY_MeterNo==0//0=��,1=��;DL698��ȫģʽ����ʱ�ȳ������ܱ���(���ܱ������ڼ�У����)
		UARTCtrl->RM_Rx_date_40020200[0]=2;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
	#else
		if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			UARTCtrl->RM_Rx_date_40020200[0]=2;
	#endif
		if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
		{//�������Ķ����ܱ���
			UARTCtrl->RM_Rx_date_40020200[0]=1;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
			p8tx[14]=5;//GET-Request
			p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			p8CSD[1]=0x00;
			p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//���ܱ��� �� ���ܱ�ESAM���к�
			p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
			p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
			p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		//ʱ���ǩ
			p8tx[Byte]=0;
			Byte+=1;
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			Byte+=3;
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
			return Byte+2;
		}
		p8tx[14]=0x10;
		p8tx[15]=0;//����Ӧ�����ݵ�Ԫ [0] ����Ӧ�����ݵ�Ԫ [1]
		p8tx[16]=x;//octet-string����
		Byte+=3;
		p8tx[Byte]=1;//����� [1]  RN
		p8tx[Byte+1]=16;//����
		Byte+=2;
		i=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
		if(i)
		{
			UART7Ctrl->Task=0;//�ص�Դ����
		}
		MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
		MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//����ȫģʽʱ���͸�����16�ֽ������
		Byte+=16;
	}
//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//�����ַ���16H��
	Byte+=3;
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	return Byte+2;
}

/*
u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx)//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	u32 i;
	u32 x;
	u32 Byte;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//��ʼ�ַ���68H��  1Byte
	p8tx[0]=0x68;
//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
//������C  1Byte
	p8tx[3]=0x43;
//��ַ��A
	p8tx[4]=5;
	p8tx[5]=UARTCtrl->RMAddr[2]>>8;
	p8tx[6]=UARTCtrl->RMAddr[2];
	p8tx[7]=UARTCtrl->RMAddr[1]>>8;
	p8tx[8]=UARTCtrl->RMAddr[1];
	p8tx[9]=UARTCtrl->RMAddr[0]>>8;
	p8tx[10]=UARTCtrl->RMAddr[0];
//�ͻ�����ַCA  1Byte
	p8tx[11]=0;
//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
//APDU
	i=UARTCtrl->TaskID;
	switch((i>>8)&0xff)//�����
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			p8tx[14]=5;//GET-Request
			p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		//ʱ���ǩ
			p8tx[Byte]=0;
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			Byte+=4;
			
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
			return Byte+2;
			
//			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
//			p8tx[14]=5;//GET-Request
//			p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
//			UARTCtrl->PIID++;
//			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
//			Byte=17;
//			//OAD
//			for(i=0;i<4;i++)
//			{
//				p8tx[Byte+i]=p8CSD[2+i];
//			}
//			Byte+=4;
//			//ѡ�񷽷�
//			p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
//			p8tx[Byte+1]=UARTCtrl->RM_DI_N;//�ϵ�n�μ�¼  unsigned
//			Byte+=2;
//			//RCSD��SEQUENCE OF����
//			p8tx[Byte]=0;//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
//			Byte++;
//		//ʱ���ǩ
//			p8tx[Byte]=0;
//		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//		//�����ַ���16H��
//			Byte+=4;
//			
//			Byte-=2;
//			p8tx[1]=Byte;
//			p8tx[2]=Byte>>8;
//			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
//			return Byte+2;
		default:
			break;
	}
	switch((i>>24)&0xf)//��������
	{
		case 1://��ͨ�ɼ�����
			p8tx[14]=5;//GET-Request
			if(p8CSD[1]==0)
			{//OAD
				p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
			}
			else
			{//ROAD
				p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				//OAD
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
				switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
				{
					default:
					case 0://0=�ɼ���ǰ����
		//				p8tx[Byte]=0;//RSD�� ѡ�񷽷�0:��ѡ��       [0]  NULL
		//				Byte+=1;
		//				break;
					case 1://1=�ɼ��ϵ�N��
						p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
						p8tx[Byte+1]=UARTCtrl->RM_DI_N;//�ϵ�n�μ�¼  unsigned
						Byte+=2;
						break;
					case 2://2=������ʱ��ɼ�
						p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
						p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						for(i=0;i<4;i++)//ʱ��:��������
						{
							p8tx[Byte+i]=Comm_Ram->DL698YMDHMS[i];
						}
						p8tx[Byte+4]=0x00;//ʱ��:ʱ
						p8tx[Byte+5]=0x00;//ʱ��:��
						p8tx[Byte+6]=0x00;//ʱ��:��
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
						switch(i)
						{
							case 0x5004://�ն���
								break;
							case 0x5006://�¶���
								p8tx[Byte+3]=0x01;//�̶�Ϊ1��
								break;
						}
						Byte+=7;
						break;
					case 3://3=��ʱ�����ɼ�
						//����������
						p8tx[Byte]=1;//RSD�� ѡ�񷽷�1
						p8tx[Byte+1]=0x20;//OAD�����ݶ���ʱ��
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						DateSaveTI(PORTn,p8tx+Byte);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
						Byte+=7;
						break;
					case 4://4=RetryMetering ����
						
						break;
				}
				x=p8CSD[6];//RCSD��SEQUENCE OF����
				p8tx[Byte]=x;
				Byte++;
				for(i=0;i<x;i++)
				{
					p8tx[Byte+0]=0;//����=OAD
					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
					Byte+=5;
				}
			}
			break;
		case 2://�¼��ɼ�����
			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
			p8tx[14]=5;//GET-Request
			p8tx[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
			//ѡ�񷽷�
			p8tx[Byte]=9;//RSD�� ѡ�񷽷�9:Selector9Ϊָ��ѡȡ�ϵ�n�μ�¼
			p8tx[Byte+1]=1;//�ϵ�n�μ�¼  unsigned
			Byte+=2;
			//RCSD��SEQUENCE OF����
//			p8CSD[5]=0;//RCSD��SEQUENCE OF����//!!!����̨ģ���֧��SEQUENCE OF����=0����
//			p8tx[Byte]=0;
//			Byte++;
			x=p8CSD[6];//RCSD��SEQUENCE OF����
			p8tx[Byte]=x;
			Byte++;
			for(i=0;i<x;i++)
			{
				p8tx[Byte+0]=0;//����=OAD
				p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
				p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
				p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
				p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
				Byte+=5;
			}
			break;
		case 3://͸������
			
			return 0;
		default:
			return 0;
	}
	
//ʱ���ǩ
	p8tx[Byte]=0;
//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//�����ַ���16H��
	Byte+=4;
	
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	return Byte+2;
}
*/



//698�㲥Уʱ
//68 1A 00 43 C0 AA 00 5E F7 07 01 00 40 00 7F 00 1C 07 E1 06 05 0A 18 0C 00 EC 67 16
//698�㲥����ַУʱ
//��ַΪ 01 00 10 56 93 58
//68 1F 00 43 05 58 93 56 10 00 01 00 17 B6 07 01 00 40 00 7F 00 1C 07 E1 06 05 0A 33 1A 00 30 8C 16

u32 BroadcastTime_TxDATA_DL698(u8 *p8tx)//DL698�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
{
	u32 Byte;
	
//��ʼ�ַ���68H��  1Byte
	p8tx[0]=0x68;
//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
//������C  1Byte
	p8tx[3]=0x43;
//��ַ��A
	p8tx[4]=0xC0;
	p8tx[5]=0xAA;
//�ͻ�����ַCA  1Byte
	p8tx[6]=0;
//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
//APDU
	p8tx[9]=0x07;
	p8tx[10]=1;
	p8tx[11]=0;//PIID
	p8tx[12]=0x40;//OAD
	p8tx[13]=0x00;
	p8tx[14]=0x7F;
	p8tx[15]=0;
	p8tx[16]=DataType_date_time_s;
	MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
	Byte=24;
	
//ʱ���ǩ
	p8tx[Byte]=0;
//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//�����ַ���16H��
	Byte+=4;
	
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	return Byte+2;
}








