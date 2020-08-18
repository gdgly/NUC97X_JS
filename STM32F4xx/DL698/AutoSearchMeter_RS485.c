
//�Զ��ѱ�RS485
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/AutoSearchMeter.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/RM_TxDATA.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/SearchMeter_Record.h"
#include "../DL698/DL698_Uart.h"



void AutoSearchMeter_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x11;
	p8[9]=0x4;
	MWR(DI,BuffAddr+10,4);
	//��0x33
	for(i=0;i<4;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[14]=0;
	for(i=0;i<14;i++)
	{
		p8[14]+=p8[i];
	}
	p8[15]=0x16;
}

void AutoSearchMeter_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x01;
	p8[9]=0x2;
	MWR(DI,BuffAddr+10,2);
	//��0x33
	for(i=0;i<2;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[12]=0;
	for(i=0;i<12;i++)
	{
		p8[12]+=p8[i];
	}
	p8[13]=0x16;
}

void AutoSearchMeter_DL698_TxData(u32 BuffAddr,u32 OAD,u64 CommAddr)//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
{
	u32 i;
	u8* p8tx;
	
	p8tx=(u8*)BuffAddr;
//��ʼ�ַ���68H��  1Byte
	p8tx[0]=0x68;
//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
//������C  1Byte
	p8tx[3]=0x43;
//��ַ��A
	MWR(CommAddr,BuffAddr+5,6);
	for(i=0;i<6;i++)
	{
		if(p8tx[5+i]==0xaa)
		{
			break;
		}
	}
	if(i<6)
	{
		p8tx[4]=0x45;
	}
	else
	{
		p8tx[4]=0x05;
	}
//�ͻ�����ַCA  1Byte
	p8tx[11]=0;
//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
//APDU
	p8tx[14]=5;//GET-Request
	p8tx[15]=1;//��ȡһ��������������             [1] GetRequestNormal
	i=p8tx[16];//PIID
	i++;
	i&=0x3f;
	p8tx[16]=i;
	//OAD
	OAD=R_Inverse(OAD,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	for(i=0;i<4;i++)
	{
		p8tx[17+i]=OAD&0xff;
		OAD>>=8;
	}
//ʱ���ǩ
	p8tx[21]=0;
//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
//�����ַ���16H��
	p8tx[1]=23;
	p8tx[2]=0;
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	//��25byte
}


//����2�������ѱ�������=array һ���ѱ���
//һ���ѱ�����=structure
//{
//  ͨ�ŵ�ַ         TSA��
//  �����ɼ�����ַ   TSA��
//	��Լ����  enum
//	{
//  	δ֪          ��0����
//		DL/T645-1997  ��1����
//		DL/T645-2007  ��2����
//		DL/T698.45    ��3����
//		CJ/T 188-2004 ��4��
//	}��
//	��λ             enum{δ֪��0����A��1����B��2����C��3��}��
//		�ź�Ʒ��         unsigned��
//	�ѵ���ʱ��       date_time_s��
//	�ѵ��ĸ�����Ϣ   array������Ϣ
//}
//������Ϣ��=structure
//{
//	������������  OAD��
//	����ֵ        Data
//}

//����5����̨���ѱ�������=array һ����̨�����
//һ����̨�������=structure
//{
//	ͨ�ŵ�ַ       TSA��
//	���ڵ��ַ     TSA��
//	���ʱ��       date_time_s
//}

#if (USER/100)==0//���Ժ����
void AutoSearchMeter_RS485(u32 PORTn)//�Զ��ѱ�����-rs485:0=��,1=�ȴ�,2...����
{
	UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	UARTCtrl->TaskID=0;
	UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
}
#else
void AutoSearchMeter_RS485(u32 PORTn)//�Զ��ѱ�����-rs485:0=��,1=�ȴ�,2...����
{
	u32 i;
	u32 x;
	u32 RS485PORT;
	u8 *p8;
	u8 * p8tx;
	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16fifo;
	u16 *p16timer;
	u32 ProtocolBps;
	u64 MeterAddr;
	
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	if(Terminal_Ram->AutoSearch_M_Timer==0)//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
	{
		UARTCtrl->TaskID=0;
		UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
		return;
	}
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=0;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=1;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=2;
			break;
		default:
			return;
	}
	ProtocolBps=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT)+6,1);
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			if(p16timer[0]==0x0)
			{//��֡����������ʱ
				UARTCtrl->Task=3;//��������
			}
			break;
		case 3://3=��������
			MC(0xfe,(u32)p8tx,4);
			i=ProtocolBps>>4;//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
			switch(i)
			{
				case 0://DL645-2007
				case 3:
					AutoSearchMeter_DL645_2007_TxData(((u32)p8tx)+4,0x00010000,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
					UARTCtrl->TxByte=4+16;//TX����(�跢���ֽ���)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//Ĭ��2400
					break;
				case 1://DL698
				case 4:
					MeterAddr=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					//MeterAddr=R_Inverse(MeterAddr,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					AutoSearchMeter_DL698_TxData(((u32)p8tx)+4,0x40010200,MeterAddr);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
					UARTCtrl->TxByte=4+25;//TX����(�跢���ֽ���)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//Ĭ��9600
					break;
				case 2://DL645-1997
				case 5:
					AutoSearchMeter_DL645_1997_TxData(((u32)p8tx)+4,0x9010,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
					UARTCtrl->TxByte=4+14;//TX����(�跢���ֽ���)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//Ĭ��1200
					break;
				default:
					MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
					return;
			}
			//p16fifo[0]=0;
			//p16fifo[1]=0;
			//p16fifo[2]=0;
			UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			CopyString((u8 *)"485x�����Զ��ѱ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR(RS485PORT+1,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->Task=5;
				p16timer[0]=1000/10;//1000ms���ն�ʱ
			}
			break;
		case 5://5=���ڽ���
			DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
			if(p16fifo[0]!=p16fifo[2])
			{//���ֽ��յ�
				p16fifo[2]=p16fifo[0];
				p16timer[0]=500/10;//500ms��ʱ
				if(PORTn==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
				if(PORTn==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
				break;
			}
			if(p16timer[0]!=0)
			{//�ȴ�ȫ����Ӧ�������
				break;
			}
			i=ProtocolBps>>4;//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
			switch(i)
			{
				case 0://DL645-2007
				case 3:
					i=RM_Rx_DL645(PORTn);//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
					break;
				case 1://DL698
				case 4:
					i=Uart_698_Rx(PORTn);//UART_698����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
					break;
				case 2://DL645-1997
				case 5:
					i=RM_Rx_DL645(PORTn);//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
					break;
				default:
					MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
					UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
			}
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					UARTCtrl->Task=0;//����1��
					i=ProtocolBps>>4;//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
					switch(i)
					{
						case 0://DL645-2007
						case 3:
						case 2://DL645-1997
						case 5:
							if((p8rx[8]&0x80)==0x0)
							{//���ǴӶ�֡
								return;//�ط�
							}
							break;
						case 1://DL698
						case 4:
							if((p8rx[3]&0x80)==0x0)
							{//���ǴӶ�֡
								return;//�ط�
							}
							break;
						default:
							MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
							UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							return;
					}
					if(p16fifo[0]!=p16fifo[1])
					{
						goto Over;//�ж����ֽڽ�����Ϊ��ʱ
					}
					for(i=5;i<11;i++)
					{
						if(p8tx[i]==0xaa)
						{
							break;
						}
					}
					if(i<11)
					{//���͵�ַ�к���0xaa,�÷������ĵ�ַ��֤
						i=ProtocolBps>>4;//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
						switch(i)
						{
							case 0://DL645-2007
							case 3:
								AutoSearchMeter_DL645_2007_TxData(((u32)p8tx)+4,0x00010000,MRR(((u32)p8rx)+1,6));//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
								UARTCtrl->TxByte=4+16;//TX����(�跢���ֽ���)
								break;
							case 1://DL698
							case 4:
								MeterAddr=MRR(((u32)p8rx)+5,6);
								AutoSearchMeter_DL698_TxData(((u32)p8tx)+4,0x40010200,MeterAddr);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
								UARTCtrl->TxByte=4+25;//TX����(�跢���ֽ���)
								break;
							case 2://DL645-1997
							case 5:
								AutoSearchMeter_DL645_1997_TxData(((u32)p8tx)+4,0x9010,MRR(((u32)p8rx)+1,6));//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
								UARTCtrl->TxByte=4+14;//TX����(�跢���ֽ���)
								break;
							default:
								MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
								UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
								return;
						}
						UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
						Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
						Start_UARTn_Tx(PORTn);//����UART�ڷ���
						UARTCtrl->Task=4;
						RS485_TxLED(PORTn);//RS485 LED����ָʾ��
						return;
					}
					i=ProtocolBps>>4;//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
					switch(i)
					{
						case 0://DL645-2007
						case 3:
							i=DataComp(((u32)p8rx)+1,((u32)p8tx)+4+1,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//��ַ��ͬ
								MeterAddr=MRR(((u32)p8rx)+1,6);
								SearchMeter_Record(PORTn,3,MeterAddr,2,0x0,ProtocolBps&0xf,0);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
							}
							break;
						case 1://DL698
						case 4:
							i=DataComp(((u32)p8rx)+5,((u32)p8tx)+4+5,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//��ַ��ͬ
								MeterAddr=MRR(((u32)p8rx)+5,6);
								SearchMeter_Record(PORTn,3,MeterAddr,3,0x0,ProtocolBps&0xf,0);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
							}
							break;
						case 2://DL645-1997
						case 5:
							i=DataComp(((u32)p8rx)+1,((u32)p8tx)+4+1,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//��ַ��ͬ
								MeterAddr=MRR(((u32)p8rx)+1,6);
								SearchMeter_Record(PORTn,3,MeterAddr,1,0x0,ProtocolBps&0xf,0);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
							}
							break;
						default:
							MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
							UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							return;
					}
					if(i==0)
					{//��ַ��ͬ
						//���ѱ��ַ
						if((ProtocolBps>>4)<3)//B4-B7:0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
						{//����
							MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							p8=(u8*)(ADDR_DATABUFF);
							for(i=0;i<6;i++)
							{
								if(p8[i]==0xaa)
								{
									break;
								}
							}
							if(i!=0)
							{
								i--;
							}
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
										//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
											case 0://ԭDL645-2007����
												MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
											case 1://ԭDL698����
												MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
											default://ԭDL645-1997����
												MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
										}
										return;
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
							MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							break;
						}
					}
				default://2��ʱ
				Over://�ж����ֽڽ�����Ϊ��ʱ
					UARTCtrl->Task=0;//����1��
					MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<6;i++)
					{
						if(p8[i]==0xaa)
						{
							break;
						}
					}
					if(i!=0)
					{
						i--;
					}
					if(p16fifo[0]==0)
					{//û�յ��κ��ֽ�
						while(1)
						{
							x=p8[i];
							if(x==0x99)
							{
								if(i==0)
								{//��ַ��0
									switch(ProtocolBps>>4)
									{
										case 0://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
											MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 1://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
											MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 2://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
											MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 3://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
											MWR(0x46aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 4://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
											MWR(0x52aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 5://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
										default:
											if(UARTCtrl->AutoSearchTime==0)//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
											{
												UARTCtrl->TaskID=0;
												UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
												return;
											}
											MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
											return;
									}
								}
								else
								{
									p8[i]=0xaa;
									i--;
								}
							}
							else
							{
								break;
							}
						}
						x=bcd_hex(x);
						x++;
						x=hex_bcd(x);
						p8[i]=x;
					}
					else
					{//���յ��ֽ�
						if(i<5)
						{
							i++;
							p8[i]=0;
						}
						else
						{
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
											case 0://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
												MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 1://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
												MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 2://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
												MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 3://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
												MWR(0x46aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 4://ԭ0=DL645-2007����,1=DL698����,2=DL645-1997����,3=DL645-2007׼ȷ,4=DL698׼ȷ,5=DL645-1997׼ȷ
												MWR(0x52aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 5://ԭ0=DL645-2007����,1=DL645-1997����,2=DL698����,3=DL645-2007׼ȷ,4=DL645-1997׼ȷ,5=DL698׼ȷ
											default:
												if(UARTCtrl->AutoSearchTime==0)//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
												{
													UARTCtrl->TaskID=0;
													UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
													return;
												}
												MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
												return;
										}
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					return;
			}
//			break;
	}
}
#endif




