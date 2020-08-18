
//�ն˳���698.45���Լ������������Fn���ݻ���
#include "Project.h"
#include "Terminal_ReadMeter_DL698_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Terminal/DataConversion_SH698.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "../terminal/Terminal_AFN0E_Event.h"

//#include <stdarg.h>


#if (USER/100)==5//�Ϻ�
extern const u32 DL698_RMLib[];


void WriteReportWord(u32 PORTn,u32 rxbuff)//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
{
#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
	u32 y;
	u32 MeterNo;
	u8 *p8s;
	u64 CommAddr;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+5,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
	if(MeterNo==0)
	{
		return;//û�ҵ�
	}
	MeterNo--;


	p8s=(u8*)(rxbuff);
	y=p8s[29];
	
	MWR(PORTn,ADDR_DATABUFF,1);//1byte �˿ں�
	MR(ADDR_DATABUFF+1,rxbuff+5,6);//6BYTE ͨ�ŵ�ַ
	MWR(0,ADDR_DATABUFF+7,1);//������������=0

	MWR(18,ADDR_DATABUFF+8,2);//2byte����
	
	MWR(0x040005FF,ADDR_DATABUFF+32,4);//���ݱ�ʶ
	MWR(y,ADDR_DATABUFF+36,1);//״̬��

	Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
#endif
}

void Terminal_ReadMeter_DL698_TxData(u32 PORTn,u8 *p8RMLib)//�ն�DL698���Լ����������
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u8* p8esamrxbuff;
	u32 index = 0;
	u32 fcs;
	//test_log_printf();
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8esamrxbuff=(u8 *)(ADDR_UART7Rx);
	p8txbuff[index++]=0x68;
	index+=2;//��2�ֽڷų���
	if(UARTCtrl->Last_Block_No!=0xffffffff)
		p8txbuff[index++]=0x63;//������  63  e3
	else
		p8txbuff[index++]=0x43;//������
	p8txbuff[index++]=0x05;//��F10��ı��ַ��Ĭ����6�ֽ�
	//ȡͨ�ŵ�ַ
	MR(((u32)p8txbuff)+index,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	index+=6;
	p8txbuff[index++]=0x10;//��ʾ����ע�ͻ�����ַ
	index+=2;//��2�ֽڷ�HCS  (�±�12 13)

	if(UARTCtrl->Last_Block_No!=0xffffffff)//����֡ȷ��֡
	{
		p8txbuff[index++]=(UARTCtrl->Last_Block_No+0x8000)&0xff;
		p8txbuff[index++]=((UARTCtrl->Last_Block_No+0x8000)>>8)&0xff;
	}
	else
	{
		p8txbuff[index++]=0x10;//��ȫ����
		p8txbuff[index++]=0x00;//����
		index++;//1�ֽڷų��� octet_string�ĳ���	[16  ] 
		p8txbuff[index++]=0x05;
		//������
		i=(u32)p8RMLib;
		i+=10+(5*UARTCtrl->RMCount);

		if(((MRR(i,4)&0xFF00FFFF)==0x30000200)||((MRR(i,4)&0xFF00FFFF)==0x30000700)||((MRR(i,4)&0xFF00FFFF)==0x30000800)||((MRR(i,4)&0xFF00FFFF)==0x30000900))//���¼���¼
		{
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x03;
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
			p8txbuff[index++]=0x09;//RSD ����һ
			p8txbuff[index++]=0x01;//��һ��
			p8txbuff[index++]=0x00; //RCSD�ĸ���,0 = ��ѡ = ȫѡ
			//p8txbuff[index++]=0x01; //RCSD�ĸ���
			//p8txbuff[index++]=0x00;//������OAD
			
			//p8txbuff[index++]=0x20;
			//p8txbuff[index++]=0x1e;
			//p8txbuff[index++]=0x02;
			//p8txbuff[index++]=0x00;
		}
		else if(((p8RMLib[4]&0x08)!=0)||((p8RMLib[4]&0x20)!=0))//���ݷ���:B5=�¶���,
		{
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x50;
			if((p8RMLib[4]&0x08)!=0)
				p8txbuff[index++]=0x04;
			else
				p8txbuff[index++]=0x06;
			p8txbuff[index++]=0x02;
			p8txbuff[index++]=0x00;
			p8txbuff[index++]=0x01;//RSD ����һ
			p8txbuff[index++]=0x20;//0x20210200  ���ݶ���ʱ���OAD
			p8txbuff[index++]=0x21;
			p8txbuff[index++]=0x02;
			p8txbuff[index++]=0x00;
			index+=Get_698DateTime_st(p8txbuff+index,p8RMLib[4]);
			p8txbuff[index++]=0x01; //RCSD�ĸ���
			p8txbuff[index++]=0x00;//������OAD
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
		}
		
		else //���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2
		{
			p8txbuff[index++]=0x01;
			p8txbuff[index++]=0x01;
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
		}
		//û��ʱ���ǩ
		p8txbuff[index++]=0x00;
		//�����ĳ���
		p8txbuff[16]=index-16-1;
		
		p8txbuff[index++]=0x01;//������֤��Ϣ	�����			[1]  RN��
		p8txbuff[index++]=0x10;//RN ����
		i=TESAM_CMD_TxRx(0x801A1000,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=���� �������
		//i=TESAM_CMD_TxRx(0x800E0002,0,0);//��ESAM�����к�
		//i=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
		//i=TESAM_CMD_TxRx(0x80360002,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����,����ESAM�����к�

		if(i)//��Ϊ������+RN�ķ�ʽ��ʵ�����ǲ���֤�ģ���������Բ��������
		{
			MC(0xee,(u32)(p8txbuff+index),16);//�洢����0
		}
		else
		{
			MR((u32)(p8txbuff+index),((u32)p8esamrxbuff)+5,16);
			Data_Inverse((u32)(p8txbuff+index),16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		}
		
		index+=16;

	}
	


//������
	p8txbuff[1]=(index+1)&0xff;
	p8txbuff[2]=((index+1)>>8)&0x3f;

	fcs=Get_16bitfcs_code(p8txbuff+1,11)^0xffff;
	p8txbuff[12]=fcs&0xff;
	p8txbuff[13]=(fcs>>8)&0xff;

	//HCS����
	fcs=Get_16bitfcs_code(p8txbuff+1,index-1)^0xffff;
	p8txbuff[index++]=fcs&0xff;
	p8txbuff[index++]=(fcs>>8)&0xff;
	p8txbuff[index++]=0x16;
	UARTCtrl->TxByte=index;//TX����(�跢���ֽ���)
}

 

void ReadMeter_DL698_FnData(u32 PORTn,u8 *p8RMLib)//698.45��Լ���������������Fn���ݻ���
{
	ReadMeter_DL698_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//698.45��Լ���������������Fn���ݻ���
}

void ReadMeter_DL698_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//698.45��Լ���������������Fn���ݻ���
{
		
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 YM=0;
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	u32 index;
	u32 Pn;
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	i=p8RMLib[0];
	//��ӡ����
	//Log_Printf_Level(LOG_LEVEL_DEBUG,"log test di=%x\n",0x05060101);
	//Log_Dumphex_Level(LOG_LEVEL_DEBUG,"dddd",ADDR_AFN04F10,10);
	//Add_Log_Date_Head(ADDR_DATABUFF);
	//sprintf((char*)(ADDR_DATABUFF+17),"i=%d\n",0x05060101);
	//Write_Log_To_File(ADDR_DATABUFF,strlen((u8*)ADDR_DATABUFF));
	//���Խ���
	switch(i)
	{
		case 3://F3 ���ڳ������ܱ��¼�
			if(UARTCtrl->RMCount==75)
			{
				if(p8rxbuff[24]!=1)//�صĲ�������
					break;
				if(p8rxbuff[25]!=1)//�ص��������Ͳ���ARRAY
					break;
				if(p8rxbuff[26]!=7)//��7��״̬��
					break;
				if(p8rxbuff[29]&0x0c)// ͣ�糭����Ƿѹ   ʱ�ӵ��Ƿѹ
				{
					y=0;//�ϱ���־��0=���ϱ���1=�ϱ�
					//�����ϱ�״̬��bit3ʱ�ӵ�ص�ѹ��
					//�������״̬��1 bit2
					i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
					if(p8rxbuff[29]&0x04)
					{
						if(i==0xff)
						{//�ǵ�1�ζ�״̬��
							y=1;//�ϱ���־��0=���ϱ���1=�ϱ�
						}
						else
						{
							if(i==0)
							{
								y=1;//�ϱ���־��0=���ϱ���1=�ϱ�
							}
						}

						MWR(1,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
					}
					else
					{
						MWR(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
					}

					//�����ϱ�״̬��bit8ͣ�糭����Ƿѹ
					//�������״̬��1 bit3
					i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
					if(p8rxbuff[29]&0x08)
					{
						if(i==0xff)
						{//�ǵ�1�ζ�״̬��
							y=1;//�ϱ���־��0=���ϱ���1=�ϱ�
						}
						else
						{
							if(i==0)
							{
								y=1;//�ϱ���־��0=���ϱ���1=�ϱ�
							}
						}
						MWR(1,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
					}
					else
					{
						MWR(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
					}
					if(y==1)
					{
						WriteReportWord(PORTn,((u32)p8rxbuff));
					}
				}
				else
				{
					break;
				}
			}
			
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if((UARTCtrl->RMCount%3)==0)//�����������
			{//������
				z=UARTCtrl->RMCount;
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				if(p8rxbuff[24]!=1)//�صĲ�������
					break;
				z=0;
				switch(UARTCtrl->RMCount)
				{
					case 6:
					case 15:
					case 24:
					case 33:
					case 42:
					case 51:	
						i=MRR(((u32)p8rxbuff)+30,4);//double_long-unsigned//��ǰ����
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+35,4);//double_long-unsigned//����ʱ��
						YM=Change_Data(YM,4);
						z=1;
						break;
					case 9:
					case 18:
					case 27:
					case 36:
					case 45:
					case 54:
						i=MRR(((u32)p8rxbuff)+42,4);//double_long-unsigned//��ǰ����
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+47,4);//double_long-unsigned//����ʱ��
						YM=Change_Data(YM,4);
						z=2;
						break;
					case 12:
					case 21:
					case 30:
					case 39:
					case 48:
					case 57:	
						i=MRR(((u32)p8rxbuff)+54,4);//double_long-unsigned//��ǰ����
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+59,4);//double_long-unsigned//����ʱ��
						YM=Change_Data(YM,4);
						z=3;
						break;
					default:
						i=MRR(((u32)p8rxbuff)+26,2);//long-unsigned//��ǰ����
						i=Change_Data(i,2);
						YM=0;
						z=0;
						break;
				}
				if(z!=0)////���ۼ�ʱ��
				{
					if(MRR((u32)p8FnData,2)==0xeeee)
					{//��1֡��־
						//����
						p8FnData[0]=7;//2byte �¼����ݳ���
						p8FnData[1]=0;
						//����
						y=MRR(((u32)p8rxbuff)+20,4);//���OAD
						y=Change_Data(y,4);
						y=Get_LastTimeDi_From_OAD(y,z);
						MWR(y,((u32)p8rxbuff)+512-7,4);//DI
						MWR(YM,((u32)p8rxbuff)+512-3,3);//����ʱ��
					}
				}
				x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);//ԭ����(��ʼ��ֵ0xff)
				if((x==0xffffff)||(i<x))
				{//��ʼ��ֵ,��ǰ����<ԭ�������ܻ���
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//��������=0
//�ն����ݳ�ʼ����,������������ǣ�������Ϊ0����ʱ�ӵ��Ƿѹ������״̬����ͣ�糭����Ƿѹ������״̬��ʱ���ն��������ϱ��¼����ܴ�������Ӧ���ݣ���
					//x=0;//-----------------------------------------------
					y=MRR(((u32)p8rxbuff)+20,4);//���OAD
					y=Change_Data(y,4);
					if(y==0x301B0400)
					{//����Ǵ���
						if(x==0xffffff)
						{//��ʼ��ֵ
							if(i==0)
							{
								break;
							}
							x=0;
						}
						else
						{//��ǰ����<ԭ�������ܻ���
							break;
						}
					}
					else
					{
						break;
					}
				}
				y=i-x;//��������
				x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//ԭ����(��ʼ��ֵ0xff)
				if(x==0xff)
				{
					x=0;
				}
				x+=y;
				if(x>10)
				{
					x=10;
				}
				//�Ϻ�Ҫ�������¼�ֻ�ϱ����һ��
				if(x>1)
				{
					x=1;
				}
				//x=1;//-----------------------------------------------
				i|=x<<24;
				MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//��������
			}
			else
			{//��1-10���¼�
				//���¼�
				if((UARTCtrl->RMCount%3)==2)
				{
					i=MRR((u32)p8FnData+2,2);
					if(i==0xeeee)
					{//��1֡��־
						//����
						if(p8rxbuff[16]&0x80)
						{
							if((p8rxbuff[16]&0x7f)==2)
							{
								YM=MRR(((u32)p8rxbuff)+22,4);
								YM=Change_Data(YM,4);
								i=Get_Di_From_OAD(YM);
								MWR(i,((u32)p8rxbuff)+512,4);
								i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+26);
								p8FnData[2]=i+4;//2byte �¼����ݳ���
								p8FnData[3]=(i+4)>>8;
							}
							else if((p8rxbuff[16]&0x7f)==1)							
							{
								YM=MRR(((u32)p8rxbuff)+21,4);
								YM=Change_Data(YM,4);
								i=Get_Di_From_OAD(YM);
								MWR(i,((u32)p8rxbuff)+512,4);
								i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+25);
								p8FnData[2]=i+4;//2byte �¼����ݳ���
								p8FnData[3]=(i+4)>>8;
							}
							else
							{
								break;
							}
						}
						else
						{
							YM=MRR(((u32)p8rxbuff)+20,4);
							YM=Change_Data(YM,4);
							i=Get_Di_From_OAD(YM);
							MWR(i,((u32)p8rxbuff)+512,4);
							i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+24);
							p8FnData[2]=i+4;//2byte �¼����ݳ���
							p8FnData[3]=(i+4)>>8;
						}
					}
				}

				if((UARTCtrl->RMCount%3)==2)
				{
					//д
					switch(PORTn)
					{
						case ACSAMPLEPORT:
							i=1;
							break;
						case RS485_1PORT://RS485-1
							i=2;//RS485PORT=1;
							break;
						case RS485_2PORT://RS485-2
							i=3;//RS485PORT=2;
							break;
						case RS485_3PORT://RS485-3
							i=4;//RS485PORT=3;
							break;
						case RS485_4PORT://RS485-4
							i=31;
							break;
						default:
							i=0;
							break;
					}
					MWR(i,ADDR_DATABUFF,1);//1byte �˿ں�
					MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+5,6);//6BYTE ͨ�ŵ�ַ
					i=p8FnData[0]+(p8FnData[1]<<8);
					if(i==0xeeee)
					{
						i=(u32)p8RMLib;
						i+=10+(5*(UARTCtrl->RMCount-2))+4;
						MR(ADDR_DATABUFF+7,i,1);
						p8=(u8 *)ADDR_DATABUFF+7;
						p8[0]+=4;
					}
					else
					{
						MWR(14,ADDR_DATABUFF+7,1);//������������
					}
					//ȡ��������DI	
					
					i=Get_EventNumDi_From_OAD(YM);
					MWR(i,ADDR_DATABUFF+10,4);
					
					//ȡ��ǰ����ֵ
					i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);
					i=hex_bcd(i);
					MWR(i,ADDR_DATABUFF+14,3);
					
					i=p8FnData[0]+(p8FnData[1]<<8);
					if(i!=0xeeee)
					{
						//�����ۼ�ʱ���DI
						//���ۼ�ʱ��ֵ
						MR(ADDR_DATABUFF+17,((u32)p8rxbuff)+512-7,7);//����
					}
					//
					i=p8FnData[2]+(p8FnData[3]<<8);
					MWR(i,ADDR_DATABUFF+8,2);//2byte����
					MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//����
					Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
					//�������-1
					i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//��������(��ʼ��ֵ0xff)
					if(i==0xff)
					{
						i=0;
					}
					if(i>10)
					{
						i=10;
					}
					if(i!=0)
					{
						i--;
					}
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//��������(��ʼ��ֵ0xff)
					//�建��
					p8FnData[0]=0xee;//��1֡���ݱ�־
					p8FnData[1]=0xee;
					p8FnData[2]=0xee;//��1֡���ݱ�־
					p8FnData[3]=0xee;
					if(i!=0)
					{//����û��0,��������֡
						UARTCtrl->RMCount--;//�����������-1
					}
				}
				return;
			}
		case 27://   F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��YYMMDDWWHhmmss
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					DateTimeStoDCB(p8rxbuff+25,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6,3);
					MW((u32)(p8+4),((u32)(p8FnData))+6+3,3);
					break;
				case 1://��ع���ʱ�� 4�ֽ�
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=6)//�������Ͳ���6
						break;
					YM=MRR(((u32)p8rxbuff)+26,4);//double-long-unsigned
					YM=Change_Data(YM,4);
					YM=hex_bcd(YM);
					MWR(YM,((u32)(p8FnData))+6+6,4);
					break;
				case 2://����ܴ���   3�ֽ�
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=0x12)//�������Ͳ���6
						break;
					p8FnData+=6+6+4;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 3://���һ�α�̷���ʱ�� 6�ֽ�
					p8FnData+=6+6+4+3;
					if(p8rxbuff[16]&0x80)
					{	
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 4://��������ܴ��� 3�ֽ�
					p8FnData+=6+6+4+3+6;
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=0x12)//�������Ͳ���6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 5://���һ�����㷢��ʱ�� 6�ֽ�
					p8FnData+=6+6+4+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 6://��������ܴ��� 3�ֽ�
					p8FnData+=6+6+4+3+6+3+6;
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=0x12)//�������Ͳ���6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 7://���һ���������㷢��ʱ��6�ֽ�
					p8FnData+=6+6+4+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 8://���������ܴ���	3�ֽ�
					p8FnData+=6+6+4+3+6+3+6+3+6;
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=0x12)//�������Ͳ���6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 9://���һ���¼����㷢��ʱ��6�ֽ�
					p8FnData+=6+6+4+3+6+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 10://Уʱ�ܴ���	3�ֽ�
					p8FnData+=6+6+4+3+6+3+6+3+6+3+6;
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					if(p8rxbuff[25]!=0x12)//�������Ͳ���6
						break;
					p8rxbuff+=25;
DEAL_LONG_UNSIGNED:
					YM=MRR((u32)(p8rxbuff+1),2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData),3);
					break;
				case 11://���һ��Уʱ����ʱ��(��Уʱ��ʱ��)
					p8FnData+=6+6+4+3+6+3+6+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
DEAL_DATE_TIME:
					DateTimeStoDCB(p8rxbuff,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData),3);
					MW((u32)(p8+4),(u32)(p8FnData+3),3);
					break;

			}
			break;
//16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
		case 129:
		case 131:
			UARTCtrl->RMError&=(~0x02);//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			if(p8rxbuff[24]==0)//�صĲ�������
				break;
			if((p8rxbuff[24]==1)&&(p8rxbuff[25]!=1))//�ص������ݣ������������Ͳ���ARRAY
				break;
			p8rxbuff+=26;
			p8FnData+=6;
MPDIRPOW://�����й�--5�ֽ�
			x=p8rxbuff[0];//������
			p8FnData[0]=x-1;
			p8FnData+=1;
			p8rxbuff+=1;
			for(i=0;i<x;i++)
			{
				YM=MRR((u32)(p8rxbuff+1+i*5),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);
				p8FnData[i*5]=0;
				MWR(YM,((u32)(p8FnData+1+i*5)),4);
			}

			break;			
		case 130:
		case 132:
		case 133:
		case 134:
		case 135:
		case 136:
			if(p8rxbuff[24]==0)//�صĲ�������
				break;
			if((p8rxbuff[24]==1)&&(p8rxbuff[25]!=1))//�ص������ݣ������������Ͳ���ARRAY
				break;
			p8rxbuff+=26;
			p8FnData+=6;
MPDIRVAR://�����޹�---4�ֽ�
			
			x=p8rxbuff[0];//������
			p8FnData[0]=x-1;
			p8FnData+=1;
			p8rxbuff+=1;
			for(i=0;i<x;i++)
			{
				YM=MRR((u32)(p8rxbuff+1+i*5),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);
				MWR(YM,((u32)(p8FnData+i*4)),4);
			}

			break;
		
			
//49 F162 ���ܱ�����ʱ��
		case 162:
			//MW(ADDR_TYMDHMS+1,(u32)(p8FnData+1),5);//���볭��ʱ��
			MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//���볭��ʱ��
			p8rxbuff+=25;
			DateTimeStoDCB(p8rxbuff,(u8 *)(ADDR_DATABUFF));
			p8=(u8 *)ADDR_DATABUFF;
			p8[3]=0;
			YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			p8[3]=i;
			p8[5]+=(p8[3]<<5);

			p8FnData+=7;
			MW(ADDR_DATABUFF,((u32)p8FnData),3);
			MW((u32)(p8+4),(u32)(p8FnData+3),3);


			break;
		case 165://����ǣ�����ť��
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
				break;
			}
			
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�β�Ǵ򿪴���
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					YM=MRR(((u32)p8rxbuff)+26,2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData)+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//��β�Ǵ򿪴����仯��־
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//ԭ����
						MWR(YM,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//����ԭ����
						if(i!=0xeeeeee)
						{
							if(i!=YM)
							{//�����仯
								MC(0x55,((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
							}
						}
					}
					break;
				case 1://���һ��β�Ǵ�ʱ��
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							index=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						index=24;
					}
					i=Get_OAD_Data_Position(p8rxbuff+index,0x201e0200,1);
					if(i==0)
						break;
					index+=i;
					
					DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6+3+6+3,3);
					MW((u32)(p8+4),((u32)p8FnData)+6+3+6+3+3,3);
					
					x=MRR(((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
					if(x==0x55)
					{//�򿪴����仯��־
					#if ERC38==1
						ERC38_Event(Pn,((u32)p8FnData)+6+3+6);//���ܱ���ť���¼���¼;���:Pn�������(��1��ʼ)
					#endif
					}
					break;
				case 2://���ܱ���Ǵ򿪴���
					if(p8rxbuff[24]!=1)//�صĲ�������
						break;
					YM=MRR(((u32)p8rxbuff)+26,2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData)+6+3+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//��β�Ǵ򿪴����仯��־
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//ԭ����
						MWR(YM,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//����ԭ����
						if(i!=0xeeeeee)
						{
							if(i!=YM)
							{//�����仯
								MC(0x55,((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
							}
						}
					}
					break;
				case 3:
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							index=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						index=24;
					}
					i=Get_OAD_Data_Position(p8rxbuff+index,0x201e0200,1);
					if(i==0)
						break;
					index+=i;
					DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6+3+6+3+6+3,3);
					MW((u32)(p8+4),((u32)p8FnData)+6+3+6+3+6+3+3,3);
					
					x=MRR(((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
					if(x==0x55)
					{//�򿪴����仯��־
					#if ERC37==1
						ERC37_Event(Pn,((u32)p8FnData)+6+3+6+3+6);//���ܱ���ť���¼���¼;���:Pn�������(��1��ʼ)
					#endif
					}
					break;
			}
			break;
//��չ3 ���г����ն������ʾֵ���ܡ�����1��M��
		case 0xE3:
			if(p8rxbuff[30]!=1)//�صĲ�������
				break;
			if(p8rxbuff[31]!=1)//������������1
				break;
			if(p8rxbuff[32]!=1)//�������Ͳ��� ARRAY
				break;
			p8rxbuff+=33;
			x=MRR(ADDR_TYMDHMS+3,3);
			MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
			p8FnData[0]=1;//�����������־
			switch(UARTCtrl->RMCount)
			{
				case 0://F161
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF161_LastDate_Source+5;
					goto MPDIRPOW;
				case 1://F162
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF162_LastDate_Source+5;
					goto MPDIRVAR;
				case 2://F163
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF163_LastDate_Source+5;
					goto MPDIRPOW;
				case 3://F164
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF164_LastDate_Source+5;
					goto MPDIRVAR;
				case 4://F165
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF165_LastDate_Source+5;
					goto MPDIRVAR;
				case 5://F166
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF166_LastDate_Source+5;
					goto MPDIRVAR;
				case 6://F167
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF167_LastDate_Source+5;
					goto MPDIRVAR;
				case 7://F168
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//����ʱ��
					p8FnData+=6+Office_AFN0DF168_LastDate_Source+5;
					goto MPDIRVAR;
				default:
					return;
			}
//			break;

//��չ11 ���г����ն��������й��������������ʱ�䣨�ܡ�����1��M��
//��չ12 ���г����ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
		case 149:
		case 151:
		case 0xEB:
		case 0xEC:
			if(p8rxbuff[30]!=1)//�صĲ�������
				break;
			if(p8rxbuff[31]!=1)//������������1
				break;
			if(p8rxbuff[32]!=1)//�������Ͳ��� ARRAY
				break;
			p8rxbuff+=33;

			p8FnData[0]=1;//�����������־
			MW(ADDR_TYMDHMS+1,((u32)p8FnData)+1,5);//����ʱ��
			p8FnData+=6;			

			index=0;
			p8FnData[0]=p8rxbuff[index];//����
			x=p8FnData[0];
			index++;
			p8FnData+=1;
			for(i=0;i<x;i++)
			{
				index+=3;//ȥ�� 02 02��2�ֽ� ��  ��������06
				YM=MRR((u32)(p8rxbuff+index),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);	
				index+=4;//������������
				
				DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
				p8=(u8 *)ADDR_DATABUFF;
				index+=8;//��������ʱ�䴦������

				MWR(YM,(u32)(p8FnData+7*i),3);
				MW((u32)(p8+1),(u32)(p8FnData+7*i+3),2);
				MW((u32)(p8+4),(u32)(p8FnData+7*i+5),2);
			}
			break;
		case 137:
		case 139:
			if(p8rxbuff[30]!=1)//�صĲ�������
				break;
			if(p8rxbuff[31]!=1)//������������1
				break;
			if(p8rxbuff[32]!=1)//�������Ͳ��� ARRAY
				break;
			p8rxbuff+=33;
			
			goto	MPDIRPOW;

		case 138:
		case 140:
		case 141:
		case 142:
		case 143:
		case 144:
			if(p8rxbuff[30]!=1)//�صĲ�������
				break;
			if(p8rxbuff[31]!=1)//������������1
				break;
			if(p8rxbuff[32]!=1)//�������Ͳ��� ARRAY
				break;
			p8rxbuff+=33;
			
			goto	MPDIRVAR;
		default:
			break;
	}
}
#endif





