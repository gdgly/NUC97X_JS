
//�ն˳���DL645_1997������������Fn���ݻ���
#include "Project.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"




extern const u32 DL645_1997_RMLib[];
void Terminal_ReadMeter_DL645_1997_TxData(u32 PORTn,u8 *p8RMLib)//�ն�DL645-1997����������
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//ȡͨ�ŵ�ַ
	MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x01;
	p8txbuff[4+9]=0x2;
	//ȡDI
	//i=DL645_1997_RMLib[UARTCtrl->FnCount];
	i=(u32)p8RMLib;
	i+=10+(3*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+4+10,i,2);
	//��0x33
	for(i=0;i<2;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	//CS����
	p8txbuff[4+12]=0;
	for(i=0;i<12;i++)
	{
		p8txbuff[4+12]+=p8txbuff[4+i];
	}
	p8txbuff[4+13]=0x16;
	UARTCtrl->TxByte=4+14;//TX����(�跢���ֽ���)
}

void ReadMeter_DL645_1997_FnData(u32 PORTn,u8 *p8RMLib)//DL645_1997���������������Fn���ݻ���
{
	ReadMeter_DL645_1997_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//DL645_1997���������������Fn���ݻ���
}

void ReadMeter_DL645_1997_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//DL645_1997���������������Fn���ݻ���
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

		
	//i=MRR(DL645_1997_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
#if (USER/100)==7//�㽭�û�
	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
		#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
//F2 ���ܱ������ϱ��¼�
		case 2:
		//��������֡
			UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
			//���¼���־,����ѭ��
			MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(UARTCtrl->RMCount/2),1);
			i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),4);
			if(i==0)
			{
				MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
			}
		//���¼�
			i=MRR((u32)p8FnData,2);
			if(i==0xeeee)
			{//��1֡��־�򳭱��������ͬ
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
				MC(0,ADDR_DATABUFF,512);
				p8=(u8 *)ADDR_DATABUFF;
				MWR(i,ADDR_DATABUFF,1);//1byte �˿ں�
				MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE ͨ�ŵ�ַ

				i=(u32)p8RMLib;
				i+=10+(3*UARTCtrl->RMCount)+2;
				MR(ADDR_DATABUFF+7,i,1);
				MR(ADDR_DATABUFF+10,((u32)p8rxbuff)+10,2);//���ݱ�ʶ
				p8[12]=0;
				p8[13]=0;
				MR(ADDR_DATABUFF+14,((u32)p8rxbuff)+12,p8[7]);//����
				p8[7]+=4;//�ܳ���

				Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
			}
			break;
		#endif
	#endif
#endif
//1 F25 ��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//ȡ��Դ���߷�ʽ BS8  1
			y&=0x3;//D0��D1�����ʾ�������Դ���߷�ʽ����ֵ1��3���α�ʾ�������ߡ��������ߡ������
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���й�����
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,3);
					i=GetClass1DataAddr(28,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
					if(i!=0x0)
					{
						p8=(u8 *)(i);
						if(p8[6]!=0xee)
						{
							if((p8[6]&0x10)!=0x0)
							{//�й����ʷ���
								p8FnData[6+2]|=0x80;
							}
						}
					}
					break;
				case 1://A�й�����
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+3,3);
					break;
				case 2://B�й�����
					if(y==1)
					{//��������
						break;
					}
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,3);
					break;
				case 3://C�й�����
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+9,3);
					break;
				case 4://���޹�����
					p8FnData[6+12]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+13,2);
					i=GetClass1DataAddr(28,Pn);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
					if(i!=0x0)
					{
						p8=(u8 *)(i);
						if(p8[6]!=0xee)
						{
							if((p8[6]&0x20)!=0x0)
							{//�޹����ʷ���
								p8FnData[6+14]|=0x80;
							}
						}
					}
					break;
				case 5://A�޹�����
					p8FnData[6+15]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+16,2);
					break;
				case 6://B�޹�����
					if(y==1)
					{//��������
						break;
					}
					p8FnData[6+18]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+19,2);
					break;
				case 7://C�޹�����
					p8FnData[6+21]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+22,2);
					break;
				case 8://�ܹ�������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+24,2);
					break;
				case 9://A��������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+26,2);
					break;
				case 10://B��������
					if(y==1)
					{//��������
						break;
					}
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,2);
					break;
				case 11://C��������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+30,2);
					//��,A,B,C���ڹ���(���й�����/������������)
					for(i=0;i<4;i++)
					{
						x=MRR(((u32)p8FnData)+6+(3*i),3);//ȡ�й�����
						if(x!=0xeeeeee)
						{
							x&=0x7fffff;//ȥ����λ
							x=bcd_hex(x);
							d64a=x;
							d64a*=1000;//����������3λС��
							x=MRR(((u32)p8FnData)+6+24+(2*i),2);//ȡ��������
							if(x!=0xeeee)
							{
								x&=0x7fff;//ȥ����λ
								x=bcd_hex(x);
								d64a/=x;
								x=d64a;
								x=hex_bcd(x);
								MWR(x,((u32)p8FnData)+6+50+(3*i),3);
							}
						}
					}
					break;
				case 12://A���ѹ
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+32]=i;
					p8FnData[6+33]=i>>8;
					break;
				case 13://B���ѹ
					if(y==1)
					{//��������
						break;
					}
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+34]=i;
					p8FnData[6+35]=i>>8;
					break;
				case 14://C���ѹ
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+36]=i;
					p8FnData[6+37]=i>>8;
					break;
				case 15://A�����
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+38]=i;
					p8FnData[6+39]=i>>8;
					p8FnData[6+40]=i>>16;
					break;
				case 16://B�����
					if(y==1)
					{//��������
						break;
					}
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+41]=i;
					p8FnData[6+42]=i>>8;
					p8FnData[6+43]=i>>16;
					break;
				case 17://C�����
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+44]=i;
					p8FnData[6+45]=i>>8;
					p8FnData[6+46]=i>>16;
					break;
				//�������
			}
			break;
//2 F26 A��B��C�������ͳ�����ݼ����һ�ζ����¼
		case 26:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ܶ������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,2);
					p8FnData[6+2]=0;
					break;
				case 1://A�������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+3,2);
					p8FnData[6+3+2]=0;
					break;
				case 2://B�������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,2);
					p8FnData[6+6+2]=0;
					break;
				case 3://C�������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+9,2);
					p8FnData[6+9+2]=0;
					break;
				case 4://�ܶ���ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+12,3);
					break;
				case 5://A����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+15,3);
					break;
				case 6://B����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+18,3);
					break;
				case 7://C����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+21,3);
					break;
				case 8://�ܶ�����ʼʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+24,4);
					break;
				case 9://A������ʼʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,4);
					break;
				case 10://B������ʼʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+32,4);
					break;
				case 11://C������ʼʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+36,4);
					break;
				case 12://�ܶ������ʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+40,4);
					break;
				case 13://A�������ʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+44,4);
					break;
				case 14://B�������ʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+48,4);
					break;
				case 15://C�������ʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
//3 F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		case 27:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��Hhmmss
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,3);
					break;
				case 1://���ܱ�����ʱ��YYMMDDWW
					MW(((u32)p8rxbuff)+12+1,((u32)p8FnData)+6+3,3);
					//����
					i=p8rxbuff[12];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[6+3+1]|=i;
					break;
				case 2://��ع���ʱ��
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,3);
					p8FnData[6+9]=0;
					break;
				case 3://����ܴ���
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+10,2);
					p8FnData[6+12]=0;
					break;
				case 4://���һ�α�̷���ʱ��
					p8FnData[6+13]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+14,4);
					p8FnData[6+18]=0;
					break;
					//��������ܴ���
					//���һ�ε�����㷢��ʱ��
				case 5://��������������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,2);
					p8FnData[6+30]=0;
					break;
				case 6://���һ������������㷢��ʱ��
					p8FnData[6+31]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+32,4);
					p8FnData[6+36]=0;
					break;
					//�¼������ܴ���
					//���һ���¼����㷢��ʱ��
					//Уʱ�ܴ���
					//���һ��Уʱ����ʱ��
			}
			break;
//4 F28 �������״̬�ּ����λ��־
		case 28:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�������״̬��
					for(i=0;i<14;i++)
					{
						p8FnData[6+14+i]=0;
					}
					p8FnData[6+14]=p8rxbuff[12];
					break;
				case 1://����״̬��
					if((p8rxbuff[12]&0x1)!=0x0)
					{//A�ϵ�
						p8FnData[6+14+6]|=0x80;
					}
					if((p8rxbuff[12]&0x10)!=0x0)
					{//A��ѹ
						p8FnData[6+14+6]|=0x04;
					}
					if((p8rxbuff[12]&0x2)!=0x0)
					{//B�ϵ�
						p8FnData[6+14+8]|=0x80;
					}
					if((p8rxbuff[12]&0x20)!=0x0)
					{//B��ѹ
						p8FnData[6+14+8]|=0x04;
					}
					if((p8rxbuff[12]&0x4)!=0x0)
					{//C�ϵ�
						p8FnData[6+14+10]|=0x80;
					}
					if((p8rxbuff[12]&0x40)!=0x0)
					{//C��ѹ
						p8FnData[6+14+10]|=0x04;
					}
					break;
			}
			break;
//5 F29 ��ǰͭ�������й��ܵ���ʾֵ
//6 F30 ��һ������ͭ�������й��ܵ���ʾֵ
		case 29:
		case 30:
			break;
//7 F31 ��ǰA��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
//8 F32 ��һ������A��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
		case 31:
		case 32:
			break;

//9 F49 ��ǰ��ѹ��������λ��
//10 F57 ��ǰA��B��C�����ѹ������2��N��г����Чֵ
//11 F58 ��ǰA��B��C�����ѹ������2��N��г��������
//12 F65 ��ǰ������Ͷ��״̬
//13 F66 ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//14 F67 ���ա����µ������ۼƲ������޹�������
//15 F73 ֱ��ģ������ǰ����


//16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
//24 F137 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
		case 129:
		case 137:
			//if(i==129)
			//{
				UARTCtrl->RMError&=(~0x02);//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			//}
			#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
				switch(UARTCtrl->RMCount)//�����������
				{
					case 0://�����й�
						x=p8rxbuff[9];
						if(x>=6)
						{
							x-=6;
							x/=4;
							if(x>TMaxTariff)
							{
								x=TMaxTariff;
							}
							p8FnData[6]=x;
							p8FnData+=7;
							p8rxbuff+=12;
							for(i=0;i<=x;i++)
							{
								p8FnData[0+(5*i)]=0;
								p8FnData[1+(5*i)]=p8rxbuff[0];
								p8FnData[2+(5*i)]=p8rxbuff[1];
								p8FnData[3+(5*i)]=p8rxbuff[2];
								p8FnData[4+(5*i)]=p8rxbuff[3];
								p8rxbuff+=4;
							}
						}
						break;
					case 1://�����й��������й�
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
						switch(i)
						{
							case 0x51:
							case 0x52:
						C51F129://�����5С���1�����������й�����ȡ���ܱ������й�����������й���ȡ����+�������ݣ�
								x=p8rxbuff[9];
								if(x>=6)
								{
									x-=6;
									x/=4;
									if(x>TMaxTariff)
									{
										x=TMaxTariff;
									}
									p8FnData[6]=x;
									p8FnData+=7;
									p8rxbuff+=12;
									for(i=0;i<=x;i++)
									{
										p8FnData[60+(5*i)]=0;
										p8FnData[61+(5*i)]=p8rxbuff[0];
										p8FnData[62+(5*i)]=p8rxbuff[1];
										p8FnData[63+(5*i)]=p8rxbuff[2];
										p8FnData[64+(5*i)]=p8rxbuff[3];
										p8rxbuff+=4;
									}
									for(i=0;i<=x;i++)
									{
										d64a=MRR(((u32)p8FnData)+(5*i),5);
										d64a=bcd16_hex(d64a);
										d64b=MRR(((u32)p8FnData)+60+(5*i),5);
	 									d64b=bcd16_hex(d64b);
										d64a+=d64b;
										d64a=hex_bcd16(d64a);
										MWR(d64a,((u32)p8FnData)+(5*i),5);
									}
								}
								break;														
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://�����������5С���1����
								goto C51F129;
						}
						break;
				}										
			#else
				switch(UARTCtrl->RMCount)//�����������
				{
					case 0://�����
						//�п���������ֻ���ܵ���
						UARTCtrl->RMCount=1;//�����������+1
						goto F129DATA;
					case 1://ֻ���ܵ���
					
						p8FnData[6]=1;//������=1
						p8FnData+=7;
						p8rxbuff+=12;
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						MW((u32)p8FnData,((u32)p8FnData)+5,5);//����1����=��
						break;
					
						//goto F129DATA;			
				}
			#endif
			break;
//18 F131 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
//26 F139 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
//40 F153 ��һʱ�����������й�����ʾֵ���ܡ�����1��M��
//41 F154 �ڶ�ʱ�����������й�����ʾֵ���ܡ�����1��M��
//42 F155 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//43 F156 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//44 F157 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//45 F158 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//46 F159 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//47 F160 �ڰ�ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 131:
		case 139:
		case 153:
		case 154:
		case 155:
		case 156:
		case 157:
		case 158:
		case 159:
		case 160:
	#if (((USER/100)!=5)||((Project/100)!=2))//���Ϻ�������
F129DATA:
	#endif
			x=p8rxbuff[9];
			if(x>=6)
			{
				x-=6;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=12;
				for(i=0;i<=x;i++)
				{
					p8FnData[0+(5*i)]=0;
					p8FnData[1+(5*i)]=p8rxbuff[0];
					p8FnData[2+(5*i)]=p8rxbuff[1];
					p8FnData[3+(5*i)]=p8rxbuff[2];
					p8FnData[4+(5*i)]=p8rxbuff[3];
					p8rxbuff+=4;
				}
				if(x==0)
				{//������=0��Ϊ1
					p8FnData-=7;
					p8FnData[6]=1;//������=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+5,5);//����1����=��
				}
			}
			break;
//17 F130 ��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��
//19 F132 ��ǰ�����޹�������޹�2������ʾֵ���ܡ�����1��M��
//20 F133 ��ǰһ�����޹�����ʾֵ���ܡ�����1��M��
//21 F134 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//22 F135 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//23 F136 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//25 F138 ���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
//27 F140 ���£���һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
//28 F141 ���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
//29 F142 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//30 F143 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//31 F144 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
		case 130:
		case 132:
		case 133:
		case 134:
		case 135:
		case 136:
		case 138:
		case 140:
		case 141:
		case 142:
		case 143:
		case 144:
			x=p8rxbuff[9];
			if(x>=6)
			{
				x-=6;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=12;
				for(i=0;i<Pn;i++)
				{
					p8FnData[i]=p8rxbuff[i];
				}
				if(x==0)
				{//������=0��Ϊ1
					p8FnData-=7;
					p8FnData[6]=1;//������=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+4,4);//����1����=��
				}
			}
			break;
//32 F145 ���������й��������������ʱ�䣨�ܡ�����1��M��
//33 F146 ���������޹��������������ʱ�䣨�ܡ�����1��M��
//34 F147 ���·����й��������������ʱ�䣨�ܡ�����1��M��
//35 F148 ���·����޹��������������ʱ�䣨�ܡ�����1��M��
//36 F149 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
//37 F150 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
//38 F151 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
//39 F152 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
		case 145:
		case 146:
		case 147:
		case 148:
		case 149:
		case 150:
		case 151:
		case 152:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�������
					x=p8rxbuff[9];
					if(x>=5)
					{
						x-=5;
						x/=3;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						p8FnData[6]=x;
						p8FnData+=7;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<3;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=3;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=7;
							p8FnData[6]=1;//������=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,3);//����1����=��
						}
					}
					break;
				case 1://����ʱ��
					x=p8rxbuff[9];
					if(x>=6)
					{
						x-=6;
						x/=4;
						if(x>p8FnData[6])
						{
							break;
						}
						p8FnData+=7;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<4;Pn++)
							{
								p8FnData[3+Pn+(i*7)]=p8rxbuff[Pn];
							}
							p8rxbuff+=4;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=7;
							p8FnData[6]=1;//������=1
							p8FnData+=7;
							MW(((u32)p8FnData)+3,((u32)p8FnData)+7+3,4);//����1����=��
						}
					}
					break;
			}
			break;
//48 F161 ���ܱ�Զ�̿���ͨ�ϵ�״̬����¼
		case 161:
			break;
//49 F162 ���ܱ�����ʱ��
		case 162:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��Hhmmss
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+1+6,3);
					//��ȷ�ĳ���ʱ��,������ʱ����(����)
					if(UARTCtrl->TransferDelay==0)
					{//��·�ϴ�����ʱʱ����
						MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//���볭��ʱ��
					}
					else
					{
						d64a=MRR(ADDR_TYMDHMS,6);
						d64a=YMDHMS_SubS(d64a,UARTCtrl->TransferDelay);//������ʱ�����S��,���ؼ�S����������ʱ����
						//���ڼ���
						MWR(d64a,((u32)p8rxbuff)+14,3);
						MWR(d64a>>24,((u32)p8rxbuff)+14+4,3);
						YMDCalculateW(((u32)p8rxbuff)+14);//ָ����ַ�����е���������ʱ���������
						i=p8rxbuff[14+3];
						if(i==0x0)
						{
							i=7;
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//���볭��ʱ��
					}
					break;
				case 1://���ܱ�����ʱ��YYMMDDWW
					MW(((u32)p8rxbuff)+12+1,((u32)p8FnData)+1+6+3,3);
					//����
					i=p8rxbuff[12];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[1+6+3+1]|=i;
					break;
			}
			break;
//49 F165 ���ܱ��ز���������ʱ��
		case 165:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://����ܴ���
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,2);
					p8FnData[8]=0;
					break;
				case 1://���һ�α�̷���ʱ��
					p8FnData[6+3]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+4,4);
					p8FnData[6+8]=0;
					break;
			}
			break;
			
//50 F166 ���ܱ�����޸Ĵ�����ʱ��
		case 166:
			
//51 F167 ���ܱ����õ���Ϣ
		case 167:
			
//52 F168 ���ܱ������Ϣ
		case 168:
			
			break;
		
#if ERC8==1		
		case 0xE1:
//0xE1 ��չ1 ERC8���ܱ��������¼�������
			if(p8FnData[ERC8_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData),10);//��ǰ10�ֽڵ�1,2�׵�1-8��ʱ�α���־��CRCֵ
				p8FnData[ERC8_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<8)//�����������
			{//��1��ʱ�α�1-8
				p8FnData[0]|=1<<i;
				x=MRR(((u32)p8FnData)+1,4);//�����������ݵ��Ĵ���,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3)<<i;
				MWR(x,((u32)p8FnData)+1,4);//д�Ĵ������ݵ�������,Byte<=8
				break;
			}
			if(i<14)//�����������
			{//��1��ʱ�α�9-14
				i-=8;
				p8FnData[5]|=1<<i;
				x=MRR(((u32)p8FnData)+6,4);//�����������ݵ��Ĵ���,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3)<<i;
				MWR(x,((u32)p8FnData)+6,4);//д�Ĵ������ݵ�������,Byte<=8
				break;
			}
			switch(i)//�����������
			{
				case 14://����й�����
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6,3);
					break;
				case 15://����޹�����
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+3,3);
					break;
				case 16://ÿ�µ�1������(�޹�������Щ�����û�У�Ϊ��ȷ������2��ʶ��������ڴ�)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10,2);
					if(PORTn!=RS485_4PORT)
					{//RS485��
	#if RMM_RS485==0//RS485ͨ������ģʽ:0=ѭ��ȫ������,1=����С������ó�������
						goto Fn0xE1_END;
	#endif
					}
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
					i=Class1Data_Fn(i,27);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					if(i==0)
					{//Fn=27û����
						break;
					}
	#if RMM_RS485==0//RS485ͨ������ģʽ:0=ѭ��ȫ������,1=����С������ó�������
				Fn0xE1_END:
	#endif
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					i=GetClass1DataAddr(27,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(i==0x0)
					{
						break;
					}
					//ȡ���һ�α�̷���ʱ��
					MR(((u32)p8FnData)+10+6+6+6,i+18,6);
					//ȡ���������ܴ���
					MR(((u32)p8FnData)+10+6+6+6+6,i+33,3);
					if(Pn==0x0)
					{//����ŵĵ��ܱ�/��������װ��ɾ��
						break;
					}
					ERC8_Event(Pn-1,(u32)p8FnData);//���ܱ�������,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
					UARTCtrl->RMCount+=2;//������2���
					break;
				case 17://���һ�α��ʱ��(��Fn=27����ʱ)
					MC(0,((u32)p8FnData)+10+6+6+6,6);
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+6+6+1,4);
					break;
				case 18://��������������(��Fn=27����ʱ)
					MC(0,((u32)p8FnData)+10+6+6+6+6,3);
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+6+6+6,2);
					
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//����ŵĵ��ܱ�/��������װ��ɾ��
						break;
					}
					ERC8_Event(Pn-1,(u32)p8FnData);//���ܱ�������,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
					break;
			}
			break;
#endif
#if ERC13==1
//0xE2 ��չ2 ERC13��������Ϣ�¼�������
		case 0xE2:
			switch(UARTCtrl->RMCount)
			{
				case 0://�ܶ������
					p8FnData[6]=1;//
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+7,2);
					p8FnData[9]=0;
					
					if(PORTn==ACSAMPLEPORT)
					{//�ڲ����ɿ�
						goto Fn0xE2_END;
					}
					if(PORTn!=RS485_4PORT)
					{//RS485��
	#if RMM_RS485==0//RS485ͨ������ģʽ:0=ѭ��ȫ������,1=����С������ó�������
						goto Fn0xE2_END;
	#endif
					}
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
					x=Class1Data_Fn(i,27);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					if(x==0)
					{//Fn=27û����
						break;
					}
					x=Class1Data_Fn(i,28);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					if(x==0)
					{//Fn=28û����
						Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
						if(Pn==0x0)
						{//����ŵĵ��ܱ�/��������װ��ɾ��
							break;
						}
						i=GetClass1DataAddr(27,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							//���ܱ��̴���							
							MR(((u32)p8FnData),i+15,3);
							//��������������
							MR(((u32)p8FnData)+3,i+33,3);	
						}
						UARTCtrl->RMCount+=2;//����2���
						break;
					}
				Fn0xE2_END:
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//����ŵĵ��ܱ�/��������װ��ɾ��
						break;
					}
					i=GetClass1DataAddr(27,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//���ܱ��̴���							
						MR(((u32)p8FnData),i+15,3);
						//��������������
						MR(((u32)p8FnData)+3,i+33,3);	
					}
					i=GetClass1DataAddr(28,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//�������״̬��1���Ƿѹ
						p8=(u8 *)(i+19);
						i=p8[0];
					#if (USER/100)==5//�Ϻ�;ֻҪ��ʱ�ӵ��
						i&=0x04;//b3=ͣ�糭����,b2=ʱ�ӵ��
					#else
						i&=0x0c;//b3=ͣ�糭����,b2=ʱ�ӵ��
					#endif
						if(i!=0)
						{
							i=1;
						}
						p8FnData[16]=i;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//��������Ϣ,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
					UARTCtrl->RMCount+=3;//������3���
					break;
				case 1://��̴���(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData),2);
					p8FnData[2]=0;
					break;
				case 2://��������������(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+3,2);
					p8FnData[5]=0;
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
					x=Class1Data_Fn(i,28);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					if(x==0)
					{//Fn=28û����
						break;
					}
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//����ŵĵ��ܱ�/��������װ��ɾ��
						break;
					}
					i=GetClass1DataAddr(28,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//�������״̬��1���Ƿѹ
						p8=(u8 *)(i+19);
						i=p8[0];
						i&=0x0c;//b3=ͣ�糭����,b2=ʱ�ӵ��
						if(i!=0)
						{
							i=1;
						}
						p8FnData[16]=i;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//��������Ϣ,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
					UARTCtrl->RMCount+=1;//������1���
					break;
				case 3://�������״̬��(��Fn=28����ʱ)
					i=p8rxbuff[12];
					i&=0x04;//b2=���Ƿѹ
					if(i!=0)
					{
						i=1;
					}
					p8FnData[16]=i;
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//����ŵĵ��ܱ�/��������װ��ɾ��
						break;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//��������Ϣ,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
					break;
			}
#endif
//��չ3 ���г����ն������ʾֵ���ܡ�����1��M��
		case 0xE3:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��(��������)
					//ע:���ȶ�ʱ��ʱ���ڶ����ʱ����Terminal_AFN0D_CongealData_Fn.c����ʱ��ȷ��
					x=MRR(ADDR_TYMDHMS+3,3);
			/*
			#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					i=MRR(((u32)p8rxbuff)+12+1,3);
					if(i!=x)
					{//YMD��ͬ
						UARTCtrl->RMCount=100;//����������
						return;
					}
			#endif
			*/
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
					return;
				case 1://F161 �����й�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							p8FnData[0+(5*i)]=0;
							p8FnData[1+(5*i)]=p8rxbuff[0];
							p8FnData[2+(5*i)]=p8rxbuff[1];
							p8FnData[3+(5*i)]=p8rxbuff[2];
							p8FnData[4+(5*i)]=p8rxbuff[3];
							p8rxbuff+=4;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=6+Office_AFN0DF161_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=1;//������=1
							p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
							MW((u32)p8FnData,((u32)p8FnData)+5,5);//����1����=��
						}
					}
					break;
				case 2://F162 �����޹�������޹�1������ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF162_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source+6,4*(x+1));
					}
					break;
				case 3://F163 �����й�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							p8FnData[0+(5*i)]=0;
							p8FnData[1+(5*i)]=p8rxbuff[0];
							p8FnData[2+(5*i)]=p8rxbuff[1];
							p8FnData[3+(5*i)]=p8rxbuff[2];
							p8FnData[4+(5*i)]=p8rxbuff[3];
							p8rxbuff+=4;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=6+Office_AFN0DF163_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=1;//������=1
							p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
							MW((u32)p8FnData,((u32)p8FnData)+5,5);//����1����=��
						}
					}
					break;
				case 4://F164 �����޹�������޹�2������ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF164_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source+6,4*(x+1));
					}
					break;
				case 5://F165 1�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF165_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source+6,4*(x+1));
					}
					break;
				case 6://F166 2�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF166_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source+6,4*(x+1));
					}
					break;
				case 7://F167 3�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF167_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source+6,4*(x+1));
					}
					break;
				case 8://F168 4�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//�����������־
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF168_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source+6,4*(x+1));
					}
					break;
				case 9://�ٴ�ȷ�ϵ��ܱ�����ʱ��(��������)(���޸�Ϊ����)
			#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					i=MRR(((u32)p8rxbuff)+12+1,3);
					x=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD��ͬ
						p8FnData[0]=0xee;//������Ч��־
					}
			#endif
					return;
			}

#if (USER/100)==2//�û���ʶ:ɽ����
//��չ ERC10��ѹ��·�쳣�¼�������
		case 0xF3:
			switch(UARTCtrl->RMCount)
			{
				case 0://�ܶ������
					MW(((u32)p8rxbuff)+12,((u32)p8FnData),2);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC10MaxPn))
					{//Pn��
						UARTCtrl->RMCount+=7;//������7���
						break;
					}
					i=MRR(((u32)p8FnData),2);
					x=MRR(ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
					if((x==0xeeee)||(i==x))
					{
						MWR(i,ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
						UARTCtrl->RMCount+=7;//������7���
						break;
					}
					break;
				case 1://A���ѹ(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+10,2);
					break;
				case 2://B���ѹ(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+12,2);
					break;
				case 3://C���ѹ(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+14,2);
					break;
				case 4://A�����(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+16,3);
					break;
				case 5://B�����(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+19,3);
					break;
				case 6://C�����(���¼�����ʱʵʱ��)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+22,3);
					break;
				case 7://��ǰ�����й�ʾֵ(���¼�����ʱʵʱ��)
					p8FnData[10+6+9]=0;//5�ֽڵ���
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+9+1,4);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC10MaxPn))
					{//Pn��
						break;
					}
					i=MRR(((u32)p8FnData),2);//���³ɹ���ȡ�Ĵ���
					MWR(i,ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
					
					p8FnData[0]=10;//ERC
					p8FnData[1]=28;//����Le
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x07+(1<<6);//D6��D7�����ʾ��ѹ��·���쳣���ͣ�ȡֵ1��2���α�ʾ���ࡢʧѹ��ֵ0��3Ϊ����
					ERC_Event_Save(((u32)p8FnData));//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					break;
			}
			break;
#endif
	}
}















