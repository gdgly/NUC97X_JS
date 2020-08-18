
//�ն˳���DL645_2007������������Fn���ݻ���
#include "Project.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"

#include "../terminal/Terminal_USER2xx.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"



extern const u32 DL645_2007_RMLib[];
void Terminal_ReadMeter_DL645_2007_TxData(u32 PORTn,u8 *p8RMLib)//�ն�DL645-2007����������
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//ȡͨ�ŵ�ַ
	if(PORTn==ACSAMPLEPORT)
	{//���ڲ����ɿ�
		MC(0xaa,((u32)p8txbuff)+4+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x11;
	p8txbuff[4+9]=0x4;
	//ȡDI	
	//i=DL645_2007_RMLib[UARTCtrl->FnCount];
	i=(u32)p8RMLib;
	i+=10+(5*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+4+10,i,4);
	//��0x33
	for(i=0;i<4;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	p8txbuff[4+15]=0x16;
	//CS����
	p8txbuff[4+14]=0;
	for(i=0;i<14;i++)
	{
		p8txbuff[4+14]+=p8txbuff[4+i];
	}
	UARTCtrl->TxByte=4+16;//TX����(�跢���ֽ���)
}

void PORTn_ReadMeter_DL645_2007_TxCurveData(u32 PORTn,u32 DI,u64 YMDHM)//DL645-2007��������������֡
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//ȡͨ�ŵ�ַ
	if(PORTn==ACSAMPLEPORT)
	{//���ڲ����ɿ�
		MC(0xaa,((u32)p8txbuff)+4+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x11;
	p8txbuff[4+9]=10;
	MWR(DI,((u32)p8txbuff)+4+10,4);
	MWR(1,((u32)p8txbuff)+4+10+4,1);//��1����
	MWR(YMDHM,((u32)p8txbuff)+4+10+4+1,5);
	//��0x33
	for(i=0;i<10;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	//CS����
	p8txbuff[4+20]=0;
	for(i=0;i<20;i++)
	{
		p8txbuff[4+20]+=p8txbuff[4+i];
	}
	p8txbuff[4+21]=0x16;
	UARTCtrl->TxByte=4+22;//TX����(�跢���ֽ���)
}

void Change0xFFTo0xEE(u32 Addr,u32 Byte)//����0xFF��Ϊ0xee
{
	u32 i;
	u8 *p8;
	
	if(Byte>(256-16))
	{
		return;
	}
	p8=(u8*)Addr;
	for(i=0;i<Byte;i++)
	{
		if(p8[i]==0xff)
		{
			p8[i]=0xee;
		}
	}
}

void ReadMeter_DL645_2007_FnData(u32 PORTn,u8 *p8RMLib)//DL645_2007���������������Fn���ݻ���
{
	ReadMeter_DL645_2007_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//DL645_2007���������������Fn���ݻ���
}

void ReadMeter_DL645_2007_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//DL645_2007���������������Fn���ݻ���
{
	u32 i;
	u32 x;
	u32 y;
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	#if ((USER/100)!=5)//���Ϻ��û�
	u32 z;
	#endif
#endif
#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
	u32 RMCount;
#endif
	u32 Pn;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	u64 d64a;
	u64 d64b;
#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
	u32 j;
#endif
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;
	
	//i=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
		case 0://ͨ��
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0:
					x=p8rxbuff[9];//ȡ���ݳ���
					if(x<=4)
					{
						break;
					}
					x-=4;
					i=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART����Fn���ݻ��峤��
					i-=6;
					if(x>i)
					{
						x=i;
					}
					MR(((u32)p8FnData)+6,((u32)p8rxbuff)+14,x);
					p8FnData[0]=x;//���г���
					break;
				default:
					x=p8rxbuff[9];//ȡ���ݳ���
					if(x<=4)
					{
						break;
					}
					x-=4;
					i=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART����Fn���ݻ��峤��
					i-=6;
					if((x+p8FnData[0])>i)
					{
						x=i-p8FnData[0];
					}
					MR(((u32)p8FnData)+6+p8FnData[0],((u32)p8rxbuff)+14,x);
					p8FnData[0]+=x;//���г���
					break;
			}
			break;
#if HourTask==1//�ص㻧���߳���(���������߶���ĵ����)
		case 1://�ص㻧����(���������߶���ĵ����)
//���ɼ�¼��ʼ�룺A0H��A0H���� E0H��E0H�������ݿ鲻��ȷ����2 �ֽڣ� 
//���ɼ�¼�ֽ�����1 �ֽڣ�ʮ�����ƣ��� 
//���ɼ�¼�洢ʱ�䣺�ꡢ�¡��ա�ʱ����,  5 �ֽڣ�
//��ѹ��������Ƶ�ʣ�17 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�С��޹����ʣ�24 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//����������8 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�С��޹��ܵ��ܣ�16 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�������޹��ܵ��ܣ�16 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//��ǰ������6 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ�
//���ɼ�¼�ۼ�У���룺1 �ֽڣ��ӵ�һ�� A0H ��ʼ�����һ�����ݿ�����룩��
//���ɼ�¼�����룺E5H,1 �ֽڡ�
			if(p8FnData[6]==0xee)
			{
				MC(0xff,((u32)p8FnData)+7+4,103);//û������������0xff�Ա����ɲ���
			}
			p8FnData[6]=107;
			p8FnData[7+4+0]=0xa0;
			p8FnData[7+4+1]=0xa0;
			p8FnData[7+4+3]=0;//������Ϊ0
			MR(((u32)p8FnData)+7+4+4,ADDR_TYMDHMS+2,4);//���ߵ�ʱ��
			p8FnData[7+4+8+17]=0xaa;
			p8FnData[7+4+8+18+24]=0xaa;
			p8FnData[7+4+8+18+25+8]=0xaa;
			p8FnData[7+4+8+18+25+9+16]=0xaa;
			p8FnData[7+4+8+18+25+9+17+16]=0xaa;
			p8FnData[7+4+8+18+25+9+17+17+6]=0xaa;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://A���ѹ
					MR(((u32)p8FnData)+7+4+8,((u32)p8rxbuff)+14,2);
					break;
				case 1://��ǰ�����й��ܵ���ʾֵ
					MR(((u32)p8FnData)+7+4+8+18+25+9,((u32)p8rxbuff)+14,4);
					break;
				case 2://��ǰ�����й��ܵ���ʾֵ
					MR(((u32)p8FnData)+7+4+8+18+25+9+4,((u32)p8rxbuff)+14,4);
					break;
				case 3://A�����
					MR(((u32)p8FnData)+7+4+8+6,((u32)p8rxbuff)+14,3);
					break;
			}
			break;
#endif

#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
//F2 ���ܱ������ϱ��¼�
		case 2:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0:
					break;
				case (2+(96*2))://��λ�����ϱ�״̬��
					//��������֡
					UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
					//��ȷ��Ӧ�����踴λ��λ
					MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//��λ�����ϱ�״̬�ֵ�λ��־
					break;
				case 1://�������ϱ�״̬��(��������ģ��ʱ)
				case (2+(96*2)+1)://�������ϱ�״̬��
					//��������֡
					UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
					WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
					break;
				default://�������͸��¼�
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
					{
						break;
					}
					Pn--;
					i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
					RMCount=DL645_2007_DI_To_RMCount(i);//�����ϱ��������ܱ���¼�������1-10�μ�¼�����ݱ�ʶתΪ�����ڳ�����ͬ��ʶ�ĵĳ������;����0xffff(65535)��ʾû�ҵ�
					if((UARTCtrl->RMCount&1)==0)//�����������
					{//������
						//��������֡
						UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
						if(i==0x04001501)
						{//���յ������ϱ�֡
							//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
						}
						else
						{
							if(RMCount==0xffff)
							{
								//���¼���־,����ѭ��
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
								return;
							}
							x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
							if(x==0xffffff)
							{//��Щ������ݷ���0xff
								x=0;
							}
							x=bcd_hex(x);
							//�����������ϱ�,���ڳ�������Ϊ0
							MWR(x,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)),4);//��������=0
							//�������3�ֽڵ����ݱ���
							i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
							switch(i)
							{
								case 0x03120000://41 B70�������޴����ķ����й���4�����޹�����
									for(i=1;i<6;i++)
									{
										z=MRR(((u32)p8rxbuff)+14+(i*3),3);//��ǰ����
										if(z==0xffffff)
										{//��Щ������ݷ���0xff
											z=0;
										}
										z=bcd_hex(z);
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),3);//ԭ����(��ʼ��ֵ0xff)
										if((x==0xffffff)||(z<x))
										{//��ʼ��ֵ,��ǰ����<ԭ�������ܻ���
											MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),4);//��������=0
											continue;
										}
										y=z-x;//��������
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4)+3,1);//ԭ����(��ʼ��ֵ0xff)
										if(x==0xff)
										{
											x=0;
										}
										x+=y;
										if(x>10)
										{
											x=10;
										}
										z|=x<<24;
										MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),4);//����
									}
									return;
								case 0x03060000://39 B68������Դʧ�����������ۼ�ʱ��
									MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
									break;
								case 0x03050000://45 B74ȫʧѹ����������ۼ�ʱ��
									MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
									break;
							}
						}	
					}
					else
					{//��1-10���¼�
						UARTCtrl->RMCount--;//�����������-1,��1����������ʱ��鱾�¼�����������������
		#if ((USER/100)==5)//�Ϻ��û�
					//���¼�
						i=MRR((u32)p8FnData,2);
						if(i==0xeeee)
						{//��1֡��־�򳭱��������ͬ
							//����
							i=p8rxbuff[9];
							p8FnData[0]=i;//2byte �¼����ݳ���
							p8FnData[1]=0;
							//����
							MR(((u32)p8rxbuff)+512,((u32)p8rxbuff)+10,i);//���ݱ�ʶ������
						}
						else
						{//�Ǻ���֡
							if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
							{
								i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
								if(i==0x04001501)
								{//���յ������ϱ�֡
									i=MRR(((u32)p8rxbuff)+512,4);//ȡ�Ĵ�����ݱ�ʶ
									RMCount=DL645_2007_DI_To_RMCount(i);//�����ϱ��������ܱ���¼�������1-10�μ�¼�����ݱ�ʶתΪ�����ڳ�����ͬ��ʶ�ĵĳ������;����0xffff(65535)��ʾû�ҵ�
									//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
								}
								else
								{
									//����
									x=p8FnData[0]+(p8FnData[1]<<8);//���г���
									i=p8rxbuff[9];//��ǰ����
									if(i>=5)
									{
										i-=5;//��֡��1�ֽ�seq
									}
									else
									{
										i=0;
									}
									//�������볤��,��UART4Rx��С2048�ֽ����Ƚ�
									y=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
									if(y>=512)
									{
										y-=512;
										if(y>=x)
										{
											y-=x;
										}
										else
										{
											y=0;
										}
									}
									else
									{
										y=0;
									}
									if(i>y)
									{
										i=y;
									}
									i+=x;
									p8FnData[0]=i;//2byte �¼����ݳ���
									p8FnData[1]=i>>8;
									//����
									i-=x;
									MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//256-8byte ���ݱ�ʶ������
								}
							}
						}
		#else//#if ((USER/100)==5)//�Ϻ��û�
		//�����Ϻ��ĵ��ܱ������ϱ��¼�
					//���¼�
						i=MRR((u32)p8FnData,2);
						if(i==0xeeee)
						{//��1֡��־�򳭱��������ͬ
							//����
							i=p8rxbuff[9];
							i++;
							p8FnData[0]=i+1;//2byte ���¼����ݳ���
							p8FnData[1]=0;
							//����
							p8rxbuff[512]=1;//�����ݱ�ʶ����
							MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+9,i);//1Byte����,nByte���ݱ�ʶ������
						}
						else
						{//�Ǻ���֡
							if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
							{
								i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
								if(i==0x04001501)
								{//���յ������ϱ�֡
									i=MRR(((u32)p8rxbuff)+512+2,4);//ȡ�Ĵ�����ݱ�ʶ
									RMCount=DL645_2007_DI_To_RMCount(i);//�����ϱ��������ܱ���¼�������1-10�μ�¼�����ݱ�ʶתΪ�����ڳ�����ͬ��ʶ�ĵĳ������;����0xffff(65535)��ʾû�ҵ�
									//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
								}
								else
								{
									//����
									x=p8FnData[0]+(p8FnData[1]<<8);//���г���
									i=p8rxbuff[9];//��ǰ����
									if(i>=5)
									{
										i-=5;//��֡��1�ֽ�seq
									}
									else
									{
										i=0;
									}
									//�������볤��,��UART4Rx��С2048�ֽ����Ƚ�
									y=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
									if(y>=512)
									{
										y-=512;
										if(y>=x)
										{
											y-=x;
										}
										else
										{
											y=0;
										}
									}
									else
									{
										y=0;
									}
									if(i>y)
									{
										i=y;
									}
									i+=x;
									p8FnData[0]=i;//2byte �¼����ݳ���
									p8FnData[1]=i>>8;
									//����
									i-=x;
									MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//256-8byte ���ݱ�ʶ������
									//�޸�������ݱ�ʶ֡�ĳ���
									if(p8rxbuff[512]>=1)
									{//�������ݱ�ʶ����>1
										//����ǰ�����������ݵ��ܳ���
										p8rxbuff[512]--;
										x=1;//1�ֽ��ܱ�ʶ����
										for(y=0;y<p8rxbuff[512];y++)
										{
											x+=p8rxbuff[512+x]+1;
										}
										p8rxbuff[512]++;
										p8rxbuff[512+x]+=i;
									}
								}
							}
						}
		#endif
						
						if((p8rxbuff[8]&0x20)==0)
						{//�޺���֡
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
							MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE ͨ�ŵ�ַ
							MWR(7,ADDR_DATABUFF+7,1);//������������
							//ȡ��������DI	
							i=(u32)p8RMLib;
							i+=10+(5*UARTCtrl->RMCount);//UARTCtrl->RMCount����-1
							MR(ADDR_DATABUFF+10,i,4);
							//ȡ��ǰ����ֵ
							if(RMCount==0xffff)
							{
								//���¼���־,����ѭ��
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
								return;
							}
							i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)),3);
							i=hex_bcd(i);
							MWR(i,ADDR_DATABUFF+14,3);
							//�������ݱ�ʶ����3�ֽڵ�
							i=MRR(((u32)p8rxbuff)+512+2,4);//ȡ�Ĵ�����ݱ�ʶ
							switch(i)
							{
								case 0x03120101://B70 ��������
									MWR(22,ADDR_DATABUFF+7,1);//������������
									for(i=1;i<6;i++)
									{
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)+(i*4)),3);
										x=hex_bcd(x);
										MWR(x,ADDR_DATABUFF+14+(i*3),3);
									}
									break;
								case 0x03060001://B68������Դʧ��
									MWR(10,ADDR_DATABUFF+7,1);//������������
									MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
									break;
								case 0x03050001://B74ȫʧѹ
									MWR(10,ADDR_DATABUFF+7,1);//������������
									MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
									break;
							}
							//
							i=p8FnData[0]+(p8FnData[1]<<8);
							MWR(i,ADDR_DATABUFF+8,2);//2byte����
							MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//����
						#if ((USER/100)==10)//�ӱ��û�
						//����Ŧ���¼��ӱ��ն�ֻ���ϱ���һ���������ݿ��Ǵ������ڶ��������¼���¼����Ҫ��
							i=MRR(((u32)p8rxbuff)+512+2,4);//ȡ�Ĵ�����ݱ�ʶ
							switch(i)
							{
								default:
									Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
									break;
								//����ť���¼�������ֻ�ϱ�һ��
								case 0x03300E02:
								case 0x03300E03:
								case 0x03300E04:
								case 0x03300E05:
								case 0x03300E06:
								case 0x03300E07:
								case 0x03300E08:
								case 0x03300E09:
								case 0x03300E0A:
									break;
							}
						#else
							Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
						#endif
							//�������-1
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-1)/2),1);//UARTCtrl->RMCount����-1
							if(i>10)
							{
								i=10;
							}
							if(i!=0)
							{
								i--;
							}
							MWR(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-1)/2),1);//UARTCtrl->RMCount����-1
							//�建��
							p8FnData[0]=0xee;//��1֡���ݱ�־
							p8FnData[1]=0xee;
						}
					}
					break;
			}
			break;
	#endif//#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
//F3 ���ڳ������ܱ��¼�
		case 3:
	#if ((USER/100)==5)//�Ϻ��û�
		#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
			if(UARTCtrl->RMCount==((60)*3))
			{//�������ϱ�״̬��bit5,bit7
				//��������֡
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
				break;
			}
			if(UARTCtrl->RMCount==((60)*3+1))
			{//��λ�����ϱ�״̬��bit5,bit7
				//��������֡
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				//��ȷ��Ӧ�����踴λ��λ
//				MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//��λ�����ϱ�״̬�ֵ�λ��־
				i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,1);
				i&=0x5f;
				MC(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,1);
				break;
			}

			if(UARTCtrl->RMCount==((60)*3+3))
			{//���������״̬��
				//��������֡
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��

				if((p8rxbuff[9]!=(14+4))&&(p8rxbuff[9]!=(18+4)))
				{//0x040005ff��07��Լ������14����07��Լ������18
					break;
				}
				x=MRR(((u32)p8rxbuff)+14,1);
				y=0;//�ϱ���־��0=���ϱ���1=�ϱ�

				//�����ϱ�״̬��bit3ʱ�ӵ�ص�ѹ��
				//�������״̬��1 bit2
				i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
				if(x&0x04)
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
				if(x&0x08)
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
				{//�ϱ�
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
							return;
					}
					MWR(i,ADDR_DATABUFF,1);//1byte �˿ں�
					MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE ͨ�ŵ�ַ
					MWR(0,ADDR_DATABUFF+7,1);//������������=0
			
					x=p8rxbuff[9];
					MWR(x,ADDR_DATABUFF+8,2);//2byte����
					MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+10,x);//����
					Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
				}
				break;
			}
		#endif//#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if((UARTCtrl->RMCount%3)==0)//�����������
			{//������
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
				if(i==0x04001501)
				{//���յ������ϱ�֡
					//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
				}
				else
				{
					i=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
					if(i==0xffffff)
					{//��Щ������ݷ���0xff
						i=0;
					}
					i=bcd_hex(i);
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);//ԭ����(��ʼ��ֵ0xff)
					if((x==0xffffff)||(i<x))
					{//��ʼ��ֵ,��ǰ����<ԭ�������ܻ���
						MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//��������=0
//�ն����ݳ�ʼ����,������������ǣ�������Ϊ0����ʱ�ӵ��Ƿѹ������״̬����ͣ�糭����Ƿѹ������״̬��ʱ���ն��������ϱ��¼����ܴ�������Ӧ���ݣ���
						y=MRR(((u32)p8rxbuff)+10,4);
						if(y==0x03300D00)
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
					i|=x<<24;
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//��������
					//�������3�ֽڵ����ݱ���
					i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
					switch(i)
					{
						case 0x03060000://39 B68������Դʧ�����������ۼ�ʱ��
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
							break;
						case 0x03050000://44 B74ȫʧѹ����������ۼ�ʱ��
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
							break;
					}
				}
			}
			else
			{//��1-10���¼�
				//���¼�
				if((UARTCtrl->RMCount%3)==1)
				{//���ۼ�ʱ��
					i=MRR((u32)p8FnData,2);
					if(i==0xeeee)
					{//��1֡��־
						//����
						i=p8rxbuff[9];
						p8FnData[0]=i;//2byte �¼����ݳ���
						p8FnData[1]=0;
						//����
						MR(((u32)p8rxbuff)+512-7,((u32)p8rxbuff)+10,i);//���ݱ�ʶ������
					}
					
				}
				if((UARTCtrl->RMCount%3)==2)
				{
					i=MRR((u32)p8FnData+2,2);
					if(i==0xeeee)
					{//��1֡��־
						//����
						i=p8rxbuff[9];
						p8FnData[2]=i;//2byte �¼����ݳ���
						p8FnData[3]=0;
						//����
						MR(((u32)p8rxbuff)+512,((u32)p8rxbuff)+10,i);//���ݱ�ʶ������
					}
					else
					{//�Ǻ���֡
						if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
						{
							i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
							if(i==0x04001501)
							{//���յ������ϱ�֡
								//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
							}
							else
							{
								//����
								x=p8FnData[2]+(p8FnData[3]<<8);//���г���
								i=p8rxbuff[9];//��ǰ����
								if(i>=5)
								{
									i-=5;//��֡��1�ֽ�seq
								}
								else
								{
									i=0;
								}
								//�������볤��,��UART4Rx��С2048�ֽ����Ƚ�
								y=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
								if(y>=512)
								{
									y-=512;
									if(y>=x)
									{
										y-=x;
									}
									else
									{
										y=0;
									}
								}
								else
								{
									y=0;
								}
								if(i>y)
								{
									i=y;
								}
								i+=x;
								p8FnData[2]=i;//2byte �¼����ݳ���
								p8FnData[3]=i>>8;
								//����
								i-=x;
								MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//���ݱ�ʶ������
							}
						}
					}
				}

				if((p8rxbuff[8]&0x20)==0)
				{//�޺���֡
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
						MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE ͨ�ŵ�ַ
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
						i=(u32)p8RMLib;
						i+=10+(5*(UARTCtrl->RMCount-2));
						MR(ADDR_DATABUFF+10,i,4);
						//ȡ��ǰ����ֵ
						i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);
						i=hex_bcd(i);
						MWR(i,ADDR_DATABUFF+14,3);
						//�������3�ֽڵ�����
						i=MRR(ADDR_DATABUFF+10,4);
						switch(i)
						{
							case 0x03060000://39 B68������Դʧ�����������ۼ�ʱ��
								MR(ADDR_DATABUFF+17,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
								break;
							case 0x03050000://44 B74ȫʧѹ����������ۼ�ʱ��
								MR(ADDR_DATABUFF+17,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
								break;
						}
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
						if(i!=0)
						{//����û��0,��������֡
							UARTCtrl->RMCount--;//�����������-1
						}
					}
					return;
				}
				if((p8rxbuff[8]&0x20)!=0)
				{//�к���֡
					UARTCtrl->RMCount--;//�����������-1
				}
			}
	#else//#if ((USER/100)==5)//�Ϻ��û�
	//�����Ϻ��û��ĵ��ܱ��¼��ɼ�
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if(((UARTCtrl->RMCount&1)==0)&&(UARTCtrl->RMDI==0xffffffff))
			{//������
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
				i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
				if(i==0x04001501)
				{//���յ������ϱ�֡
					//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
				}
				else
				{
					i=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
					if(i==0xffffff)
					{//��Щ������ݷ���0xff
						i=0;
					}
					i=bcd_hex(i);
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),3);//ԭ����(��ʼ��ֵ0xff)
					if((x==0xffffff)||(i<x))
					{//��ʼ��ֵ,��ǰ����<ԭ�������ܻ���
						MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),4);//��������=0
						break;
					}
					y=i-x;//��������
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//ԭ����(��ʼ��ֵ0xff)
					if(x==0xff)
					{
						x=0;
					}
					x+=y;
					if(x>10)
					{
						x=10;
					}
					i|=x<<24;
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),4);//��������
					//�������3�ֽڵ����ݱ���
					i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
					switch(i)
					{
						case 0x03120000://41 B70�������޴����ķ����й���4�����޹�����
							for(i=1;i<6;i++)
							{
								z=MRR(((u32)p8rxbuff)+14+(i*3),3);//��ǰ����
								if(z==0xffffff)
								{//��Щ������ݷ���0xff
									z=0;
								}
								z=bcd_hex(z);
								x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),3);//ԭ����(��ʼ��ֵ0xff)
								if((x==0xffffff)||(z<x))
								{//��ʼ��ֵ,��ǰ����<ԭ�������ܻ���
									MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),4);//��������=0
									continue;
								}
								y=z-x;//��������
								x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4)+3,1);//ԭ����(��ʼ��ֵ0xff)
								if(x==0xff)
								{
									x=0;
								}
								x+=y;
								if(x>10)
								{
									x=10;
								}
								z|=x<<24;
								MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),4);//����
							}
							return;
						case 0x03060000://39 B68������Դʧ�����������ۼ�ʱ��
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
							break;
						case 0x03050000://45 B74ȫʧѹ����������ۼ�ʱ��
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
							break;
					}
				}
			}
			else
			{//��1-10���¼�,����¼�������
				i=MRR((u32)p8FnData,2);
				if(i==0xeeee)
				{//��1֡��־
					//����
					i=p8rxbuff[9];
					i++;
					p8FnData[0]=i+1;//2byte ���¼����ݳ���
					p8FnData[1]=0;
					//����
					p8rxbuff[512]=1;//�����ݱ�ʶ����
					MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+9,i);//1Byte����,nByte���ݱ�ʶ������
				}
				else
				{//�Ǻ���֡���n��ʼ֡
					i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
					if(i==0x04001501)
					{//���յ������ϱ�֡
						//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
					}
					else
					{//�Ǻ���֡���n��ʼ֡
						if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
						{//�Ǻ���֡
							//����
							x=p8FnData[0]+(p8FnData[1]<<8);//���г���
							i=p8rxbuff[9];//��ǰ����
							if((x+i)>(LEN_MeterEvent-10-22))
							{//�ܳ���>FLASH�ܴ洢�ĳ���,�ȴ���ǰ��ȡ��
								if(p8rxbuff[512]>1)
								{//�������ݱ�ʶ����>1
									//����ǰ�����������ݵ��ܳ���
									p8rxbuff[512]--;
									x=1;//1�ֽ��ܱ�ʶ����
									for(y=0;y<p8rxbuff[512];y++)
									{
										x+=p8rxbuff[512+x]+1;
									}
									p8FnData[0]=x;//2byte ���¼����ݳ���
									p8FnData[1]=x>>8;
									Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//��ȡ�ĵ��ܱ��¼�д�뻺��ADDR_DATABUFF
									Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
									//�ƶ����δ����֡����ʼ
									y=p8rxbuff[512+x]+1;
									MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+x,y);
									x=y+1;//���г���
									p8rxbuff[512]=1;//�����ݱ�ʶ����
								}
							}
							if(i>=5)
							{
								i-=5;//��֡��1�ֽ�seq
							}
							else
							{
								i=0;
							}
							//�������볤��,��UART4Rx��С2048�ֽ����Ƚ�
							y=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
							if(y>=512)
							{
								y-=512;
								if(y>=x)
								{
									y-=x;
								}
								else
								{
									y=0;
								}
							}
							else
							{
								y=0;
							}
							if(i>y)
							{
								i=y;
							}
							i+=x;
							p8FnData[0]=i;//2byte �¼����ݳ���
							p8FnData[1]=i>>8;
							//����
							i-=x;
							MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//���ݱ�ʶ������
							//�޸�������ݱ�ʶ֡�ĳ���
							if(p8rxbuff[512]>=1)
							{//�������ݱ�ʶ����>1
								//����ǰ�����������ݵ��ܳ���
								p8rxbuff[512]--;
								x=1;//1�ֽڱ�ʶ����
								for(y=0;y<p8rxbuff[512];y++)
								{
									x+=p8rxbuff[512+x]+1;
								}
								p8rxbuff[512]++;
								p8rxbuff[512+x]+=i;
							}
						}
						else
						{//��n��ʼ֡
							//����
							x=p8FnData[0]+(p8FnData[1]<<8);//���г���
							i=p8rxbuff[9];//��ǰ����
							i++;
							if((x+i)>(LEN_MeterEvent-10-22))
							{//�ܳ���>FLASH�ܴ洢�ĳ���,�ȴ���ǰ��ȡ��
								Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//��ȡ�ĵ��ܱ��¼�д�뻺��ADDR_DATABUFF
								//�¼�����FLASH�ܴ洢�ĳ��Ȳ����ĵ�ǰֻ�з��¼���ĺܶ����ʶ���õ����
								Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
								p8rxbuff[512]=0;//�����ݱ�ʶ����
								x=1;//���г���(1�ֽ������ݱ�ʶ����)
							}
							//�������볤��,��UART4Rx��С2048�ֽ����Ƚ�
							y=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
							if(y>=512)
							{
								y-=512;
								if(y>=x)
								{
									y-=x;
								}
								else
								{
									y=0;
								}
							}
							else
							{
								y=0;
							}
							if(i>y)
							{
								i=y;
							}
							i+=x;
							p8FnData[0]=i;//2byte �¼����ݳ���
							p8FnData[1]=i>>8;
							//����
							i-=x;
							p8rxbuff[512]+=1;//�����ݱ�ʶ����
							MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+9,i);//1Byte����,nByte���ݱ�ʶ������
						}
					}
				}
				if((p8rxbuff[8]&0x20)!=0)
				{//�к���֡
					UARTCtrl->RMCount--;//�����������-1
					return;
				}
				//�޺���֡
				
				//�ѳ�DI����
				UARTCtrl->SearchDICount++;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount++;//�����¼������ѳ�DI����
				if(UARTCtrl->RMDICount<UARTCtrl->TotalDICount)
				{//�ѳ�DI����<���賭��
					UARTCtrl->RMCount--;//�����������-1
					return;
				}
				
				//���¼�
				Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//��ȡ�ĵ��ܱ��¼�д�뻺��ADDR_DATABUFF
			#if ((USER/100)==10)//�ӱ��û�
			//�ӱ��ն�ֻ���ϱ���һ���������ݿ��Ǵ������ڶ��������¼���¼����Ҫ��
				i=MRR(((u32)p8rxbuff)+512+2,4);//ȡ�Ĵ�����ݱ�ʶ
				switch(i)
				{
					default:
						Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
						break;
					//����ť���¼�������ֻ�ϱ�һ��
					case 0x03300E02:
					case 0x03300E03:
					case 0x03300E04:
					case 0x03300E05:
					case 0x03300E06:
					case 0x03300E07:
					case 0x03300E08:
					case 0x03300E09:
					case 0x03300E0A:
						break;
				}
			#else//#if ((USER/100)==10)//�ӱ��û�
				Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
			#endif
				//�����������-1
				i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//��������(��ʼ��ֵ0xff)
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
				MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//��������(��ʼ��ֵ0xff)
				//�建��
				p8FnData[0]=0xee;//��1֡���ݱ�־
				p8FnData[1]=0xee;
				
				UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
				UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
			}
	#endif//#if ((USER/100)==5)//�Ϻ��û�
			break;
#endif//#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����


//1 F25 ��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//ȡ��Դ���߷�ʽ BS8  1
			y&=0x3;//D0��D1�����ʾ�������Դ���߷�ʽ����ֵ1��3���α�ʾ�������ߡ��������ߡ������
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��,A,B,C�й�����
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+6,3);
					}
#if (USER/100)==2//�û���ʶ:ɽ����
					i=MRR(((u32)p8rxbuff)+14,3);
					DayAveragePower(Pn,i,1);//��ƽ�����й����ʼ���;���Pn��1��ʼ,Power=�й�����(BCD),Count=���μ������
#endif
					break;
				case 1://��,A,B,C�޹�����
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+12+6,3);
					}
					break;
				case 2://��,A,B,C��������
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+4,4);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>8)
					{
						x=8;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+12+12+4,2);
					}
					break;
				case 3://A,B,C���ѹ
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+2,4);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>6)
					{
						x=6;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+2,2);
					}
					break;
				case 4://A,B,C�����
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+3,6);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>9)
					{
						x=9;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+6+3,3);
					}
					break;
				case 5://�������			
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6+9,3);
					break;
				case 6://��,A,B,C���ڹ���
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//����0xFF��Ϊ0xee
					x=p8rxbuff[9];//ȡ���ݳ���
					x-=4;//4�ֽ�DI��ʶ
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6+9+3,x);
					if(y==1)
					{//��������
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+6+9+3+6,3);
					}
					break;
			}
			break;
/*
//1 F25 ��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
		case 25:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���й�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://A�й�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://B�й�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://C�й�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://���޹�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://A�޹�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+3,3);
					break;
				case 6://B�޹�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+6,3);
					break;
				case 7://C�޹�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+9,3);
					break;
				case 8://�ܹ�������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24,2);
					break;
				case 9://A��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+2,2);
					break;
				case 10://B��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+4,2);
					break;
				case 11://C��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+6,2);
					break;
				case 12://A���ѹ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32,2);
					break;
				case 13://B���ѹ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32+2,2);
					break;
				case 14://C���ѹ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32+4,2);
					break;
				case 15://A�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38,3);
					break;
				case 16://B�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38+3,3);
					break;
				case 17://C�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38+6,3);
					break;
				case 18://�������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+47,3);
					break;
				case 19://�����ڹ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50,3);
					break;
				case 20://A���ڹ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+3,3);
					break;
				case 21://B���ڹ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+6,3);
					break;
				case 22://C���ڹ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+9,3);
					break;
			}
			break;
*/
//2 F26 A��B��C�������ͳ�����ݼ����һ�ζ����¼
#if RMM_AFN0CF26==0//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
		//���������¼����
		case 26:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://A��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 1://A�����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 2://B��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://B�����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 4://C��������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 5://C�����ʱ���ۼ�ֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					//�ܶ����������
					x=MRR(((u32)p8FnData)+6+3,3);
					if(x!=0xeeeeee)
					{
						i=bcd_hex(x);
						x=MRR(((u32)p8FnData)+6+6,3);
						if(x!=0xeeeeee)
						{
							i+=bcd_hex(x);
							x=MRR(((u32)p8FnData)+6+9,3);
							if(x!=0xeeeeee)
							{
								i+=bcd_hex(x);
								i=hex_bcd(i);
								MWR(i,((u32)p8FnData)+6,3);
							}
						}
					}
					//ʱ���ۼ�ֵ����
					x=MRR(((u32)p8FnData)+6+15,3);
					if(x!=0xeeeeee)
					{
						i=bcd_hex(x);
						x=MRR(((u32)p8FnData)+6+18,3);
						if(x!=0xeeeeee)
						{
							i+=bcd_hex(x);
							x=MRR(((u32)p8FnData)+6+21,3);
							if(x!=0xeeeeee)
							{
								i+=bcd_hex(x);
								i=hex_bcd(i);
								MWR(i,((u32)p8FnData)+6+12,3);
							}
						}
					}
					break;
				case 6://A�����������ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56,6);//�ݴ�A���������ʼʱ��YMDHMS
					break;
				case 7://A������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+12,6);//�ݴ�A����������ʱ��YMDHMS
					break;
				case 8://B�����������ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+6,6);//�ݴ�B���������ʼʱ��YMDHMS
					break;
				case 9://B������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+18,6);//�ݴ�B����������ʱ��YMDHMS
					break;
				case 10://C�����������ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					//���������ʼʱ�̼���
					d64a=MRR(((u32)p8rxbuff)+14,6);
					if(d64a!=0xeeeeeeeeeeee)
					{
						d64b=MRR(((u32)p8FnData)+6+56,6);
						if(d64b!=0xeeeeeeeeeeee)
						{
							if(d64b>d64a)
							{
								d64a=d64b;
							}
							d64b=MRR(((u32)p8FnData)+6+56+6,6);
							if(d64b!=0xeeeeeeeeeeee)
							{
								if(d64b>d64a)
								{
									d64a=d64b;
								}
								d64a>>=8;
								MWR(d64a,((u32)p8FnData)+6+24,4);
							}
						}
					}
					break;
				case 11://C������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					//����������ʱ�̼���
					d64a=MRR(((u32)p8rxbuff)+14,6);
					if(d64a!=0xeeeeeeeeeeee)
					{
						d64b=MRR(((u32)p8FnData)+6+56+12,6);
						if(d64b!=0xeeeeeeeeeeee)
						{
							if(d64b>d64a)
							{
								d64a=d64b;
							}
							d64b=MRR(((u32)p8FnData)+6+56+18,6);
							if(d64b!=0xeeeeeeeeeeee)
							{
								if(d64b>d64a)
								{
									d64a=d64b;
								}
								d64a>>=8;
								MWR(d64a,((u32)p8FnData)+6+40,4);
							}
						}
					}
					break;
				//����������ɹ�ʱΪ���������������
				case (12+0)://A,B,C��������,�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					MW(((u32)p8rxbuff)+14+3,((u32)p8FnData)+6+15,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6+6,3);
					MW(((u32)p8rxbuff)+14+9,((u32)p8FnData)+6+18,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+6+9,3);
					MW(((u32)p8rxbuff)+14+15,((u32)p8FnData)+6+21,3);
					break;
				case (12+1)://A�����������ʼʱ��,A������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case (12+2)://B�����������ʼʱ��,A������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case (12+3)://C�����������ʼʱ��,A������������ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
#endif
#if RMM_AFN0CF26==1//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
//	#if (USER/100)!=6//���Ǻ���ϵ
		//�����ʧѹ��¼����
		case 26:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://Aʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://Bʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://Cʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://��ʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://Aʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 6://Bʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 7://Cʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 8://�����ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+24,4);
					break;
				case 9://A���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					break;
				case 10://B���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					break;
				case 11://C���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					break;
				case 12://�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+40,4);
					break;
				case 13://A���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					break;
				case 14://B���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					break;
				case 15://C���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					break;
				//����������ɹ�ʱΪ���������������
				case 16://A,B,C��ʧѹ����,�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					MW(((u32)p8rxbuff)+14+3,((u32)p8FnData)+6+15,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6+6,3);
					MW(((u32)p8rxbuff)+14+9,((u32)p8FnData)+6+18,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+6+9,3);
					MW(((u32)p8rxbuff)+14+15,((u32)p8FnData)+6+21,3);
					break;
				case 17://A�����ʧѹ��ʼʱ��,A�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case 18://B�����ʧѹ��ʼʱ��,B�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case 19://C�����ʧѹ��ʼʱ��,c�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
/*
	#endif
	#if (USER/100)==6//�Ǻ���ϵ
		//�����ʧѹ��¼����
		case 26:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://Aʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://Bʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://Cʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://��ʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://Aʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 6://Bʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 7://Cʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 8://�����ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+24,4);
					break;
				case 9://A���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					break;
				case 10://B���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					break;
				case 11://C���ʧѹ��ʼʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					break;
				case 12://�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+40,4);
					break;
				case 13://A���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					break;
				case 14://B���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					break;
				case 15://C���ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					break;
				case 16://Aʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 17://Bʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 18://Cʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 19://Aʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 20://Bʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 21://Cʧѹ�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 22://A�����ʧѹ��ʼʱ��,A�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case 23://B�����ʧѹ��ʼʱ��,B�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case 24://C�����ʧѹ��ʼʱ��,c�����ʧѹ����ʱ��
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
	#endif
*/
#endif
		
//3 F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		case 27:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��Hhmmss
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5,3);
					break;
				case 1://���ܱ�����ʱ��YYMMDDWW
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+1+5+3,3);
					//����
					i=p8rxbuff[14];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[1+5+3+1]|=i;
					break;
				case 2://��ع���ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3,4);
					break;
				case 3://����ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4,3);
					break;
				case 4://���һ�α�̷���ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3,6);
					break;
				case 5://��������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6,3);
					break;
				case 6://���һ�����㷢��ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3,6);
					break;
				case 7://���������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6,3);
					break;
				case 8://���һ�����㷢��ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3,6);
					break;
				case 9://�¼������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6,3);
					break;
				case 10://���һ�����㷢��ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3,6);
					break;
				case 11://Уʱ�ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3+6,3);
					break;
				case 12://���һ��Уʱ����ʱ��(��Уʱ��ʱ��)
					MW(((u32)p8rxbuff)+14+4+6,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3+6+3,6);
					break;
			}
			break;
//4 F28 �������״̬�ּ����λ��־
		case 28:
			MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+14,14);
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
				break;
			}
			x=p8FnData[1+5+14+12];
			x&=0x3;
			if(x!=0x0)
			{//��ѹ�����������
				x=1;
			}
			ERC11_Event(Pn,x);//�����쳣,���:Pn�������(��1��ʼ),f=0�ָ�=1����
			
			break;
//5 F29 ��ǰͭ�������й��ܵ���ʾֵ
//6 F30 ��һ������ͭ�������й��ܵ���ʾֵ
		case 29:
		case 30:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0:
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1,4);
					break;
				case 1:
					p8FnData[6+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,4);
					break;
			}
			break;
//7 F31 ��ǰA��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
//8 F32 ��һ������A��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
		case 31:
		case 32:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ǰA�������й�����ʾֵ
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1,4);
					break;
				case 1://��ǰA�෴���й�����ʾֵ
					p8FnData[6+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+5+1,4);
					break;
				case 2://��ǰA������޹�1����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+10,4);
					break;
				case 3://��ǰA������޹�2����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+14,4);
					break;
				case 4://��ǰB�������й�����ʾֵ
					p8FnData[6+18]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18+1,4);
					break;
				case 5://��ǰB�෴���й�����ʾֵ
					p8FnData[6+23]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+23+1,4);
					break;
				case 6://��ǰB������޹�1����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+28,4);
					break;
				case 7://��ǰB������޹�2����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32,4);
					break;
				case 8://��ǰC�������й�����ʾֵ
					p8FnData[6+36]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+36+1,4);
					break;
				case 9://��ǰC�෴���й�����ʾֵ
					p8FnData[6+41]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+41+1,4);
					break;
				case 10://��ǰC������޹�1����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+46,4);
					break;
				case 11://��ǰC������޹�2����ʾֵ
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50,4);
					break;
			}
			break;

//9 F49 ��ǰ��ѹ��������λ��
		case 49:
			//Uab/Ua��λ��	����¼A.5	��	2
			MWR(0,((u32)p8FnData)+1,2);
			//Ub��λ��	����¼A.5	��	2
			MWR(0x1200,((u32)p8FnData)+1+2,2);
			//Ucb/Uc��λ��	����¼A.5	��	2
			MWR(0x2400,((u32)p8FnData)+1+4,2);
			//Ia��λ��	����¼A.5	��	2
			MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+6,2);
			//Ib��λ��	����¼A.5	��	2
			i=MRR(((u32)p8rxbuff)+14+2,2);
			i=bcd_hex(i);
			i+=1200;
			if(i>=3600)
			{
				i-=3600;
			}
			i=hex_bcd(i);
			MWR(i,((u32)p8FnData)+1+8,2);
			//Ic��λ��	����¼A.5	��	2
			i=MRR(((u32)p8rxbuff)+14+4,2);
			i=bcd_hex(i);
			i+=2400;
			if(i>=3600)
			{
				i-=3600;
			}
			i=hex_bcd(i);
			MWR(i,((u32)p8FnData)+1+10,2);
			break;
//��չ17 ��ǰ��ѹ��������λ��(��չ�����ն��ڲ�����)
		case 0xF1:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ǰ��ѹ��������λ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1,12);
					break;
			}
			break;
//10 F57 ��ǰA��B��C�����ѹ������2��N��г����Чֵ
		case 57:
			i=UARTCtrl->RMCount;//�����������
			if(i<6)
			{
				p8FnData[1]=19;
				MW(((u32)p8rxbuff)+14+2,((u32)p8FnData)+2+(36*i),36);
			}
			break;
//11 F58 ��ǰA��B��C�����ѹ������2��N��г��������
		case 58:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ѹ����ʧ������ݿ�
					p8FnData[1]=19;
					for(i=0;i<3;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+(38*i),2);
					}
					break;
				case 1://A ���ѹг���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+2+(2*i),2);
					}
					break;
				case 2://B ���ѹг���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+38+2+(2*i),2);
					}
					break;
				case 3://C ���ѹг���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+38+38+2+(2*i),2);
					}
					break;
				case 4://A �����г���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+38+38+38+(2*i),2);
					}
					break;
				case 5://B �����г���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+38+38+38+36+(2*i),2);
					}
					break;
				case 6://C �����г���������ݿ�
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2λС��ɾȥ1λ
						MWR(x,((u32)p8FnData)+2+38+38+38+36+36+(2*i),2);
					}
					break;
			}
			break;
//12 F65 ��ǰ������Ͷ��״̬
//13 F66 ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//14 F67 ���ա����µ������ۼƲ������޹�������
//15 F73 ֱ��ģ������ǰ����


//16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
//24 F137 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
//��չ0xD1 ��2�������й�����ʾֵ���ܡ�����1��M��
		case 129:
		case 137:
		case 0xd1:
			//if(i==129)
			//{
				UARTCtrl->RMError&=(~0x02);//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			//}
			#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
				switch(UARTCtrl->RMCount)//�����������
				{

					case 0://����й� ,��С��ţ�5,1����5,2����������й�
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
						switch(i)
						{
							case 0x51:
							case 0x52:
								goto F129DATA;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://�����������5С���1����
								goto F129DATA;
						}
						break;
					case 1://�����й�����С��ţ�0,2����6,2����7,1����7,2�����������й�
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
						switch(i)
						{
							case 0x51:
							case 0x52:
								break;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								goto F129DATA;
							default://�����������5С���1����
								break;
						}
						break;
				}
			#else
				switch(UARTCtrl->RMCount)//�����������
				{
					case 0://�����
						//�п����������²�ֻ���ܵ���
						UARTCtrl->RMCount=1;//�����������+1
						goto F129DATA;
					case 1://ֻ���ܵ���
					/*
						p8FnData[6]=1;//������=1
						p8FnData+=7;
						p8rxbuff+=14;
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						MW((u32)p8FnData,((u32)p8FnData)+5,5);//����1����=��
						break;
					*/
						goto F129DATA;
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
// F177 ��ǰ����й�����ʾֵ���ܡ�����1~M��
// F178 ��1 ����������й�����ʾֵ���ܡ�����1~M��
//��չ0xD3 ��2�·����й�����ʾֵ���ܡ�����1��M��
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
		case 177:
		case 178:
		case 0xd3:
F129DATA:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=14;
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
//25 F138 ���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
//27 F140 ���£���һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
//��չ0xD2 ��2�������޹�������޹�1������ʾֵ���ܡ�����1��M��
//��չ0xD4 ��2�·����޹�������޹�2������ʾֵ���ܡ�����1��M��
		case 130:
		case 132:
		case 138:
		case 140:
		case 0xd2:
		case 0xd4:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<Pn;i++)
				{
					p8FnData[i]=p8rxbuff[i];
				}
//				//ȥ����޹����λ����
//				for(i=0;i<(x+1);i++)
//				{
//					p8FnData[(4*i)+3]&=0x7f;
//				}
				if(x==0)
				{//������=0��Ϊ1
					p8FnData-=7;
					p8FnData[6]=1;//������=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+4,4);//����1����=��
				}
			}
			break;
//20 F133 ��ǰһ�����޹�����ʾֵ���ܡ�����1��M��
//21 F134 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//22 F135 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//23 F136 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//28 F141 ���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
//29 F142 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//30 F143 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//31 F144 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//��չ0xD5 ��2��һ�����޹�����ʾֵ���ܡ�����1��M��
//��չ0xD6 ��2�¶������޹�����ʾֵ���ܡ�����1��M��
//��չ0xD7 ��2���������޹�����ʾֵ���ܡ�����1��M��
//��չ0xD8 ��2���������޹�����ʾֵ���ܡ�����1��M��
		case 133:
		case 134:
		case 135:
		case 136:
		case 141:
		case 142:
		case 143:
		case 144:
		case 0xd5:
		case 0xd6:
		case 0xd7:
		case 0xd8:
//F130DATA:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=14;
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
//��չ0xD9 ��2�������й��������������ʱ�䣨�ܡ�����1��M��
//��չ0xDA ��2�������޹��������������ʱ�䣨�ܡ�����1��M��
//��չ0xDB ��2�·����й��������������ʱ�䣨�ܡ�����1��M��
//��չ0xDC ��2�·����޹��������������ʱ�䣨�ܡ�����1��M��
		case 145:
		case 146:
		case 147:
		case 148:
		case 149:
		case 150:
		case 151:
		case 152:
		case 0xd9:
		case 0xda:
		case 0xdb:
		case 0xdc:
F145DATA:
			x=p8rxbuff[9];
			if(x>=4)
			{
				Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//����0xFF��Ϊ0xee
			}
			if(x>=12)
			{
				x-=12;
				x/=8;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<=x;i++)
				{
					for(Pn=0;Pn<7;Pn++)
					{
						p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
					}
					p8rxbuff+=8;
				}
				if(x==0)
				{//������=0��Ϊ1
					p8FnData-=7;
					p8FnData[6]=1;//������=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+7,7);//����1����=��
				}
			}
			break;
//48 F161 ���ܱ�Զ�̿���ͨ�ϵ�״̬����¼
		case 161:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�ͨ�ϵ�״̬
					i=MRR(((u32)p8rxbuff)+14,1);
					i&=0x10;//B4=�̵���״̬(0ͨ��1��)
					if(i==0)
					{
						p8FnData[6]=0x11;//0x11��ʾ"ͨ��"
					}
					else
					{
						p8FnData[6]=0;//0��ʾ"�ϵ�"
					}
					break;
				case 1://��1-10�κ�բ�����ߴ���(�����Զ�����բʱ��������բ��¼�еĲ����ߴ���ΪFFFFFFFF)
					for(i=0;i<10;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(4*i),4);
						if(x!=0xffffffff)
						{
							break;
						}
					}
					if(i<10)
					{//������κ�բ����
						p8FnData[32]=i;
					}
					break;
				case 2://��1-10�κ�բ����ʱ��
					i=p8FnData[32];
					if(i!=0xee)
					{//������κ�բ����
						MW(((u32)p8rxbuff)+14+(6*i),((u32)p8FnData)+6+1,6);
					}
					break;
				case 3://��1-10����բ�����ߴ���
					for(i=0;i<10;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(4*i),4);
						if(x!=0xffffffff)
						{
							break;
						}
					}
					if(i<10)
					{//������κ�բ����
						p8FnData[33]=i;
					}
					break;
				case 4://��1-10����բ����ʱ��
					i=p8FnData[33];
					if(i!=0xee)
					{//������κ�բ����
						MW(((u32)p8rxbuff)+14+(6*i),((u32)p8FnData)+6+1+6,6);
					}
					break;
			}
			break;
//49 F162 ���ܱ�����ʱ��
		case 162:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��Hhmmss				
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+6,3);
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
							i=7;//��������=7
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//���볭��ʱ��
					}
					break;
				case 1://���ܱ�����ʱ��YYMMDDWW
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+1+6+3,3);
					//����
					i=p8rxbuff[14];
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
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
				break;
			}
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://����ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://���һ�α�̷���ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,6);
					break;
				case 2://���ܱ�β�Ǵ򿪴���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//��β�Ǵ򿪴����仯��־
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//ԭ����
						x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
						MWR(x,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//����ԭ����
						if(i!=0xeeeeee)
						{
							if(i!=x)
							{//�����仯
								MC(0x55,((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
							}
						}
					}
					break;
				case 3://���һ��β�Ǵ�ʱ��
					x=MRR(((u32)p8FnData)+64,1);//β�Ǵ򿪴����仯��־
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3,60);
					if(x==0x55)
					{//�򿪴����仯��־
					#if ERC38==1
						ERC38_Event(Pn,((u32)p8FnData)+6+3+6);//���ܱ���ť���¼���¼;���:Pn�������(��1��ʼ)
					#endif
					}
					break;
				case 4://���ܱ���Ǵ򿪴���(�������¼���¼)
					if(Pn<=ERC37MaxPn)
					{
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3+6,3);
						MC(0,((u32)p8FnData)+64,1);//�忪��Ǵ򿪴����仯��־
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//ԭ����
						x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
						MWR(x,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//����ԭ����
						if(i!=0xeeeeee)
						{
							if(i!=x)
							{//�����仯
								MC(0x55,((u32)p8FnData)+64,1);//����Ǵ򿪴����仯��־
								break;
							}
						}
					}
					UARTCtrl->RMCount++;//�����²�
					break;
				case 5://���һ�ο���Ǽ�¼(�������¼���¼)
					if(Pn<=ERC37MaxPn)
					{
						x=MRR(((u32)p8FnData)+64,1);//����Ǵ򿪴����仯��־
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3+6+3,60);
						if(x==0x55)
						{//�򿪴����仯��־
						#if ERC37==1
							ERC37_Event(Pn,((u32)p8FnData)+6+3+6+3+6);//���ܱ�����¼���¼;���:Pn�������(��1��ʼ)
						#endif
						}
					}
					break;
			}
			break;
//50 F166 ���ܱ�����޸Ĵ�����ʱ��
		case 166:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://Уʱ�ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://���ܱ����һ��ʱ���޸�ǰ�ͺ�ʱ��
					MW(((u32)p8rxbuff)+14+4,((u32)p8FnData)+6+3,12);
					break;
				case 2://���ܱ�ʱ�β����޸Ĵ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+12,3);
					break;
				case 3://���һ�ε��ܱ�ʱ�β����޸�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+12+3,6);
					break;
			}
			break;
//51 F167 ���ܱ����õ���Ϣ
		case 167:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��1�ι�����ܹ���Ѵ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,2);
					break;
				case 1://����ǰ��ʣ����
					p8FnData[6+2]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+1,4);
					break;
				case 2://��1�ι�����ۼƹ�����
					p8FnData[6+2+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+1,4);
					break;
				case 3://��1�ι�����ܹ���������
					i=MRR(((u32)p8FnData)+6,2);
					if((i==0)||(i==0xeeee))
					{//������Ѵ���=0���
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,2);
					}
					break;
				case 4://����ǰ��ʣ�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5,4);
					break;
				case 5://����ǰ��͸֧����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4,4);
					break;
				case 6://��1�ι�����ۼƹ�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4,4);
					break;
				case 7://��Ƿ���޵���(͸֧������ֵ)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4+4,4);
					break;
				case 8://��������(��������1)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4+4+4,4);
					break;
			}
			break;
//52 F168 ���ܱ������Ϣ
		case 168:
			break;
			
#if ERC8==1		
//0xE1 ��չ1 ERC8���ܱ��������¼�������
		case 0xE1:
			if(p8FnData[ERC8_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData),10);//��ǰ10�ֽڵ�1,2�׵�1-8��ʱ�α���־��CRCֵ
				p8FnData[ERC8_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<4)//�����������
			{//��1,2�׵�1-2��ʱ�α�
				p8FnData[0]|=1<<i;
				x=MRR(((u32)p8FnData)+1,4);//�����������ݵ��Ĵ���,Byte<=8
				Pn=p8rxbuff[9];
				if(Pn>4)
				{
					Pn-=4;
					Pn/=3;
					for(i=0;i<Pn;i++)
					{
						if(i<8)
						{
							x+=MRR(((u32)p8rxbuff)+14+(3*i),3)<<i;//�����������ݵ��Ĵ���,Byte<=8
						}
						else
						{
							x+=(~MRR(((u32)p8rxbuff)+14+(3*i),3))<<(i-8);//�����������ݵ��Ĵ���,Byte<=8
						}
					}
				}
				MWR(x,((u32)p8FnData)+1,4);//д�Ĵ������ݵ�������,Byte<=8
				break;
			}
			switch(i)//�����������
			{
				case 4://ÿ�µ�1������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,2);
					break;
				case 5://ÿ�µ�2������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+2,2);
					break;
				case 6://ÿ�µ�3������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+4,2);
					break;
				case 7://����޹�����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+3,3);
					break;
				case 8://�������������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6,3);
					break;
				case 9://��ѹ���������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+3,3);
					break;
				case 10://����й�����(�����������Щ�����û�У�Ϊ��ȷ������2��ʶ��й������ڴ�)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6,3);
					if(PORTn==ACSAMPLEPORT)
					{//�ڲ����ɿ�
						goto Fn0xE1_END;
					}
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
				Fn0xE1_END:
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
				case 11://���һ�α�̷���ʱ��(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+6,6);
					break;
				case 12://���������ܴ���(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+6+6,3);
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
			if(p8FnData[ERC13_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData)+6,4);//��������
				p8FnData[ERC13_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<3)//�����������
			{//�������
				p8FnData[6]|=1<<i;
				x=MRR(((u32)p8FnData)+7,3);//�����������ݵ��Ĵ���,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3);//�����������ݵ��Ĵ���,Byte<=8
				MWR(x,((u32)p8FnData)+7,3);//д�Ĵ������ݵ�������,Byte<=8
				break;
			}
			switch(i)
			{
				case 3://ʧѹ�ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,3);
					break;
				case 4://�����ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+3,3);
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
				case 5://����ܴ���(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
					break;
				case 6://���������ܴ���(��Fn=27����ʱ)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
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
				case 7://�������״̬��1(��Fn=28����ʱ)
					i=p8rxbuff[14];
					i&=0x0c;//b3=ͣ�糭����,b2=ʱ�ӵ��
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
			break;
#endif	
//��չ3 ���г����ն������ʾֵ���ܡ�����1��M��
		case 0xE3:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ն���ʱ��YYMMDD
					//ע:���ȶ�ʱ��ʱ���ڶ����ʱ����Terminal_AFN0D_CongealData_Fn.c����ʱ��ȷ��
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//�Ϻ�
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					i=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD��ͬ
						UARTCtrl->RMCount=100;//����������
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
					return;
				case 9://�ٴ�ȷ���ն���ʱ��YYMMDD(���޸�Ϊ����)
				/*
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD��ͬ
						p8FnData[0]=0xee;//������Ч��־
					}
				#endif
				*/
					return;
			}
			goto DateAddCom;
//��չ RS485�ö�ȫ��6�ฺ������
//��չ �ز��ö�1�ฺ������(��ѹ��������Ƶ��)
//��չ �ز��ö�2�ฺ������(���޹�����)
//��չ �ز��ö�3�ฺ������(��������)
//��չ �ز��ö�4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
//��չ �ز��ö�5�ฺ������(1-4�����޹��ܵ���)
//��չ �ز��ö�6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
		case 0xE4://��չ RS485�ö�ȫ��6�ฺ������
		case 0xE5://��չ �ز��ö�1�ฺ������(��ѹ��������Ƶ��)
		case 0xE6://��չ �ز��ö�2�ฺ������(���޹�����)
		case 0xE7://��չ �ز��ö�3�ฺ������(��������)
		case 0xE8://��չ �ز��ö�4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
		case 0xE9://��չ �ز��ö�5�ฺ������(1-4�����޹��ܵ���)
		case 0xEA://��չ �ز��ö�6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
			i=p8rxbuff[9];
			i+=1;
			x=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART����Fn���ݻ��峤��
			if(x>=(6+i))
			{
				MR(((u32)p8FnData)+6,((u32)p8rxbuff)+9,i);
			}
			p8FnData[0]=1;//������Ч��־
			switch(p8RMLib[0])
			{
				case 0xE6://��չ �ز��ö�2�ฺ������(���޹�����)
					if(p8rxbuff[14+8]!=0xAA)
					{//ǰ���޷ָ���AA
						p8FnData[6]+=1;
						p8FnData[7+4+8]=0xAA;
						MR(((u32)p8FnData)+7+4+8+1,((u32)p8rxbuff)+14+8,25);
					}
					break;
				case 0xE7://��չ �ز��ö�3�ฺ������(��������)
					if(p8rxbuff[14+8]!=0xAA)
					{//ǰ���޷ָ���AA,AA
						p8FnData[6]+=2;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						MR(((u32)p8FnData)+7+4+8+2,((u32)p8rxbuff)+14+8,9);
					}
					break;
				case 0xE8://��չ �ز��ö�4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
					if(p8rxbuff[14+8]!=0xAA)
					{//ǰ���޷ָ���AA,AA,AA
						p8FnData[6]+=3;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						MR(((u32)p8FnData)+7+4+8+3,((u32)p8rxbuff)+14+8,17);
					#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
						switch(i)
						{
							case 0x51:
							case 0x52:
						D51E8://�����5С���1�����������й�����ȡ���ܱ������й�����������й���ȡ����+�������ݣ�
								d64b=MRR(((u32)p8FnData)+7+4+8+3+4,4);
								for(y=0;y<8;y++)
								{
									x=(d64b>>(y*4));
									x&=0xf;
									if(x>9)
									{
										break;
									}
								}
								if(y!=8)
								{
									d64b=0;
								}
								else
								{
									d64b=bcd16_hex(d64b);
								}
								d64a=MRR(((u32)p8FnData)+7+4+8+3,4);
								for(y=0;y<8;y++)
								{
									x=(d64a>>(y*4));
									x&=0xf;
									if(x>9)
									{
										break;
									}
								}
								if(y!=8)
								{
									d64a=0;
								}
								else
								{
									d64a=bcd16_hex(d64a);
								}
								d64a+=d64b;
								d64a=hex_bcd16(d64a);
								MWR(d64a,((u32)p8FnData)+7+4+8+3,4);
								break;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://�����������5С���1����
								goto D51E8;
						}
					#endif

					}
					break;
				case 0xE9://��չ �ز��ö�5�ฺ������(1-4�����޹��ܵ���)
					if(p8rxbuff[14+8]!=0xAA)
					{//ǰ���޷ָ���AA,AA,AA,AA
						p8FnData[6]+=4;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						p8FnData[7+4+11]=0xAA;
						MR(((u32)p8FnData)+7+4+8+4,((u32)p8rxbuff)+14+8,17);
					}
					break;
				case 0xEA://��չ �ز��ö�6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
					if(p8rxbuff[14+8]!=0xAA)
					{//ǰ���޷ָ���AA,AA,AA,AA,AA
						p8FnData[6]+=5;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						p8FnData[7+4+11]=0xAA;
						p8FnData[7+4+12]=0xAA;
						MR(((u32)p8FnData)+7+4+8+5,((u32)p8rxbuff)+14+8,7);
					}
					break;
			}
			break;
//��չ11 ���г����ն��������й��������������ʱ�䣨�ܡ�����1��M��
//��չ12 ���г����ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
		case 0xEB:
		case 0xEC:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ն������ʾֵ
		#if (RMM_CompCongealTime==0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					p8FnData[0]=1;//������Ч��־
					UARTCtrl->RMCount=1;//�����������+1
		#else
					p8FnData[0]=0;//�����ݱ�־
		#endif
					goto F145DATA;
				case 1://����1�Σ��ն���ʱ��YYMMDDhhmm
					i=MRR(((u32)p8rxbuff)+14+2,3);
					x=MRR(ADDR_TYMDHMS+3,3);
					if(i==x)
					{//YMDͬ
						if(p8FnData[0]!=0xee)
						{
							p8FnData[0]=1;//������Ч��־
						}
					}
					break;
			}
			break;
//75 ��չ13 ��1-24��������������й�����ʾֵ
		case 0xED:
			//FnDataAddr+6=�������(0-23����1-24��),FnDataAddr+7��ʼ12�ֽ�����
			x=p8rxbuff[9];
			if(x>=14)
			{
				p8FnData[6]=UARTCtrl->RMCount;//�������(0-23����1-24��)
				MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+1,4);
				MW(((u32)p8rxbuff)+14+5+1,((u32)p8FnData)+6+1+4,4);
				if(x>=19)
				{//�з���
					MW(((u32)p8rxbuff)+14+5+1+4+1,((u32)p8FnData)+6+1+4+4,4);
				}
			}
			break;
//76 ��չ14 ������2���ն�������
		case 0xEE:
	#if(LEN_UART2FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-1 LEN_UART2FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
	#if(LEN_UART3FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-2 LEN_UART3FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#if(LEN_UART4FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "�ز� LEN_UART4FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#endif
#if (Project/100)==5//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#if(LEN_UART5FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-3 LEN_UART5FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#endif
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ն���ʱ��YYMMDD
					//ע:���ȶ�ʱ��ʱ���ڶ����ʱ����Terminal_AFN0D_CongealData_Fn.c����ʱ��ȷ��
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//�Ϻ�
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					if(i!=x)
					{//YMD��ͬ
						UARTCtrl->RMCount=100;//��,��������
						UARTCtrl->RMError|=2;//B0=ʧ��,B1=F129ʧ��;�Ϻ�Ҫ���ն���ʱ�����ʱAFN0CF11���г�����Ϣ���ܼƳ���ɹ�
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
					return;
				case 9://�ٴ�ȷ���ն���ʱ��YYMMDD(���޸�Ϊ����)
				/*
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					if(i!=x)
					{//YMD��ͬ
						p8FnData[0]=0xee;//������Ч��־
					}
				#endif
				*/
					return;
			}
	DateAddCom:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 1://F161 �����й�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							p8FnData[0+(5*i)]=0;
							p8FnData[1+(5*i)]=p8rxbuff[0];
							p8FnData[2+(5*i)]=p8rxbuff[1];
							p8FnData[3+(5*i)]=p8rxbuff[2];
							p8FnData[4+(5*i)]=p8rxbuff[3];
							p8rxbuff+=4;
						}
					#if (USER/100)==4//�û���ʶ:����
						//ũ����ֻ���ܵ���,��������4
						if(x==0)
						{
							x=TMaxTariff;
							p8FnData-=6+Office_AFN0DF161_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=TMaxTariff;//������=4
						}
					#endif
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
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF162_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source+6,4*(x+1));
					}
					break;
				case 3://F163 �����й�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
						p8rxbuff+=14;
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
				#if (((USER/100)==5)&&((Project/100)==2))//�Ϻ�������
	 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
					switch(i)
					{
						case 0x51:
						case 0x52:
					D51F161://�����5С���1�����������й�����ȡ���ܱ������й�����������й���ȡ����+�������ݣ�
							p8FnData=(u8 *)Addr_FnDataBuff;
							for(i=0;i<=x;i++)
							{
								d64b=MRR(((u32)p8FnData)+(6+Office_AFN0DF161_LastDate_Source+6)+(5*i),5);
								for(y=0;y<10;y++)
								{
									j=(d64b>>(y*4));
									j&=0xf;
									if(j>9)
									{
										break;
									}
								}
								if(y!=10)
								{
									d64b=0;
								}
								else
								{
									d64b=bcd16_hex(d64b);
								}
								d64a=MRR(((u32)p8FnData)+(6+Office_AFN0DF163_LastDate_Source+6)+(5*i),5);
								for(y=0;y<10;y++)
								{
									j=(d64a>>(y*4));
									j&=0xf;
									if(j>9)
									{
										break;
									}
								}
								if(y!=10)
								{
									d64a=0;
								}
								else
								{
									d64a=bcd16_hex(d64a);
								}
								d64a+=d64b;
								d64a=hex_bcd16(d64a);
								MWR(d64a,((u32)p8FnData)+(6+Office_AFN0DF161_LastDate_Source+6)+(5*i),5);
							}
							break;
						case 0x02:
						case 0x62:
						case 0x71:
						case 0x72:
							break;
						default://�����������5С���1����
							goto D51F161;
					}
				#endif
					break;
				case 4://F164 �����޹�������޹�2������ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF164_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source+6,4*(x+1));
					}
					break;
				case 5://F165 1�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF165_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source+6,4*(x+1));
					}
					break;
				case 6://F166 2�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF166_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source+6,4*(x+1));
					}
					break;
				case 7://F167 3�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF167_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source+6,4*(x+1));
					}
					break;
				case 8://F168 4�����޹�����ʾֵ���ܡ�����1��M��
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//�����������־
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF168_LastDate_Source+5]=x;//������
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source+6,4*(x+1));
					}
					break;
			#if (((USER/100)==5)&&(Project==300))//�Ϻ�ר��
				case 10://F185 �����й��������������ʱ��
					x=p8rxbuff[9];
					if(x>=4)
					{
						Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//����0xFF��Ϊ0xee
					}
					if(x>=(4+8))
					{
						p8FnData[0]=1;//�����������־
						x-=(4+8);
						x/=8;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF185_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF185_LastDate_Source+5]=x;//������
						p8FnData+=6+Office_AFN0DF185_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<7;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=8;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=7;
							p8FnData[6]=1;//������=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,7);//����1����=��
						}
					}
					break;
				case 11://F187 �����й��������������ʱ��
					x=p8rxbuff[9];
					if(x>=4)
					{
						Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//����0xFF��Ϊ0xee
					}
					if(x>=(4+8))
					{
						p8FnData[0]=1;//�����������־
						x-=(4+8);
						x/=8;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF187_LastDate_Source,5);//����ʱ��
						p8FnData[6+Office_AFN0DF187_LastDate_Source+5]=x;//������
						p8FnData+=6+Office_AFN0DF187_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<7;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=8;
						}
						if(x==0)
						{//������=0��Ϊ1
							p8FnData-=7;
							p8FnData[6]=1;//������=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,7);//����1����=��
						}
					}
					break;
			#endif
			}
			break;
//77 ��չ15 ������3���ն�������
		case 0xEF:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ն���ʱ��YYMMDD
					//ע:���ȶ�ʱ��ʱ���ڶ����ʱ����Terminal_AFN0D_CongealData_Fn.c����ʱ��ȷ��
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//�Ϻ�
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					if(i!=x)
					{//YMD��ͬ
						UARTCtrl->RMCount=100;//��,��������
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
					return;
				case 9://�ٴ�ȷ���ն���ʱ��YYMMDDhhmm(���޸�Ϊ����)
				/*
				#if (RMM_CompCongealTime!=0)//�����ܱ��ն������ݱȽ϶���ʱ��:0=���Ƚ�,1=Ҫ�Ƚ�
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					if(i!=x)
					{//YMD��ͬ
						p8FnData[0]=0xee;//������Ч��־
					}
				#endif
				*/
					return;
			}
			goto DateAddCom;
			
#if ERC40==1
//��չ16 �㶨�ų������ܴ���
		case 0xF0:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>ERC40MaxPn))
			{//Pn��
				UARTCtrl->RMCount++;//������1���
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://
					i=MRR(((u32)p8rxbuff)+14,3);//��ǰ�ܴ���
					x=MRR(ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,3);
					if(i!=0xeeeeee)
					{
						MWR(i,ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//������Ч��־
							break;//����¼
						}
					}
					UARTCtrl->RMCount++;//������1���
					break;
				case 1://
					if(p8FnData[0]!=1)//������Ч��־
					{
						break;
					}
					MW(((u32)p8rxbuff)+1,((u32)p8FnData)+6,6);//6�ֽ��豸��ַ
					MW(ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,((u32)p8FnData)+12,3);//�ܴ���
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+15,5);//����ʱ��YMDHM
					ERC40_Meter_Event((u32)p8FnData,Pn+1);//ERC40ֱ�Ӷ����ܱ�ų��쳣�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)
					break;
			}
			break;
#endif
	#if (USER/100)==2//�û���ʶ:ɽ����
		case 0xF2://��չ ERC9������·�쳣�¼�������
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://A������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
					break;
				case 1://B������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
					break;
				case 2://C������ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC9MaxPn))
					{//Pn��
						UARTCtrl->RMCount+=3;//������3���
						break;
					}
					x=0;//b0-b2�ֱ��ʾA,B,C������仯��־
					for(i=0;i<3;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						d64b=MRR(ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							if(d64b==0xeeeeee)
							{
								MWR(d64a,ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
							}
							else
							{
								if(d64a!=d64b)
								{
									x|=1<<i;
								}
							}
						}
					}
					if(x==0)
					{//û�仯,�������¼���¼
						UARTCtrl->RMCount+=3;//������3���
					}
					x|=1<<6;//D6��D7�����ʾ������·���쳣���ͣ�ȡֵ1��3���α�ʾ��·����·������
					p8FnData[9]=x;
					p8FnData[0]=1;//������ҪЧ��־
					break;
				case 3://A,B,C���ѹ(���¼�����ʱʵʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,6);
					break;
				case 4://A,B,C�����(���¼�����ʱʵʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6,9);
					break;
				case 5://(��ǰ)�����й��ܵ���(���¼�����ʱʵʱ��)
					if(p8FnData[0]!=1)//������ҪЧ��־
					{
						break;
					}
					p8FnData[10+6+9]=0;//5�ֽڵ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+9+1,4);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC9MaxPn))
					{//Pn��
						break;
					}
					//���³ɹ���ȡ�Ĵ���
					for(i=0;i<3;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							MWR(d64a,ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
						}
					}
					p8FnData[0]=9;//ERC
					p8FnData[1]=28;//����Le
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					ERC_Event_Save(((u32)p8FnData));//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					break;
			}
			break;
		#endif
		#if (((USER/100)==2)||((USER/100)==5))//�û���ʶ:ɽ��,�Ϻ�
		case 0xF3://��չ ERC10��ѹ��·�쳣�¼�������
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>ERC10MaxPn))
			{//Pn��
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://ȫʧѹ�ܴ���,�ۼ�ʱ��
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
				#if (USER/100)==5//�û���ʶ:�Ϻ�
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+401,6);
				#endif
					break;
				case 1://A��ʧѹ����
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
				#if (USER/100)==5//�û���ʶ:�Ϻ�
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+8,3);
				#endif
					break;
				case 2://B��ʧѹ����	����¼A.10	��	3
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
				#if (USER/100)==5//�û���ʶ:�Ϻ�
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+11,3);
				#endif
					break;
				case 3://C��ʧѹ����	����¼A.10	��	3
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+9,3);
				#if (USER/100)==5//�û���ʶ:�Ϻ�
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+14,3);
				#endif
					UARTCtrl->RMCount++;//�ܳ����������������
					break;
				case 4://A,B,Cʧѹ�ܴ����ۼ�ʱ��(��������������ʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+9,3);
				#if (USER/100)==5//�û���ʶ:�Ϻ�
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+8,3);
					MW(((u32)p8rxbuff)+14+6,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+11,3);
					MW(((u32)p8rxbuff)+14+12,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+14,3);
				#endif
					break;
				case 5://A�������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+12,3);
					break;
				case 6://B�������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+15,3);
					break;
				case 7://c�������
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+18,3);
					UARTCtrl->RMCount++;//�ܳ����������������
					goto Fn0xF3_NEXT;
				case 8://A,B,C�����ܴ����ۼ�ʱ��(��������������ʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+12,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+15,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+18,3);
				Fn0xF3_NEXT:
					x=0;//b0-b2�ֱ��ʾA,B,C������仯��־
					for(i=0;i<7;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						d64b=MRR(ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							#if (USER/100)==5//�û���ʶ:�Ϻ�  ֻҪ������1��ERC10
								if(Pn==0)
								{
									MWR(d64a,ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
								}
							#else
								MWR(d64a,ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
							#endif
						}
						if((d64a!=0xeeeeee)&&(d64b!=0xeeeeee))
						{
							if(d64a!=d64b)
							{
								if(i==0)
								{//ȫʧѹ
									x|=0x07;
									if((x&0xf0)==0)
									{
										x|=2<<6;//D6��D7�����ʾ��ѹ��·���쳣���ͣ�ȡֵ1��2���α�ʾ���ࡢʧѹ��ֵ0��3Ϊ����
									}
								}
								else
								{
									if(i<4)
									{//ʧѹ
										x|=1<<(i-1);
										if((x&0xf0)==0)
										{
											x|=2<<6;//D6��D7�����ʾ��ѹ��·���쳣���ͣ�ȡֵ1��2���α�ʾ���ࡢʧѹ��ֵ0��3Ϊ����
										}
									}
									else
									{//����
										x|=1<<(i-4);
										if((x&0xf0)==0)
										{
											x|=1<<6;//D6��D7�����ʾ��ѹ��·���쳣���ͣ�ȡֵ1��2���α�ʾ���ࡢʧѹ��ֵ0��3Ϊ����
										}
									}
								}
							}
						}
					}
					if(x==0)
					{//û�仯,�������¼���¼
						UARTCtrl->RMCount+=3;//������3���
					}
					p8FnData[15]=x;
					p8FnData[0]=1;//������ҪЧ��־
					break;
				case 9://A,B,C���ѹ(���¼�����ʱʵʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16,6);
					break;
				case 10://A,B,C�����(���¼�����ʱʵʱ��)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16+6,9);
					break;
				case 11://(��ǰ)�����й��ܵ���(���¼�����ʱʵʱ��)
					if(p8FnData[0]!=1)//������ҪЧ��־
					{
						break;
					}
					p8FnData[16+6+9]=0;//5�ֽڵ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16+6+9+1,4);
					p8FnData[6]=10;//ERC
					p8FnData[7]=28;//����Le
					MR(((u32)p8FnData)+8,ADDR_TYMDHMS+1,5);//��ʱ������
					Pn++;
					p8FnData[13]=Pn;
					p8FnData[14]=(Pn>>8)|(1<<7);
					ERC_Event_Save(((u32)p8FnData)+6);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					break;
			}
			break;
	#endif
	#if (USER/100)==2//�û���ʶ:ɽ����
		case 0xF4://��չ ERC29���ܱ�����¼�,ERC28���ܱ����¼�������
			i=MRR(((u32)p8rxbuff)+14,3);
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			DayAveragePower(Pn,i,1);//��ƽ�����й����ʼ���;���Pn��1��ʼ,Power=�й�����(BCD),Count=���μ������
			break;
	#endif
		case 0xF6://��չ F246��ǰ�����¼����
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			Pn=F246MapPn(Pn);//F246�����¼����ӳ��Pn;����0��ʾ��Ч
			if(Pn==0)
			{
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ܴ���
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1,3);
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF246_Sec+(15*Pn),3);
					break;
				default://��2-10��
					i=UARTCtrl->RMCount;
					if((i>=1)&&(i<=10))
					{
						i--;
						for(x=0;x<12;x++)
						{//0xffתΪ0xee
							if(p8rxbuff[14+x]==0xff)
							{
								p8rxbuff[14+x]=0xee;
							}
						}
						MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+4+(10*i),5);
						MW(((u32)p8rxbuff)+14+7,((u32)p8FnData)+4+(10*i)+5,5);
						if(i==0)
						{//�ǵ�1��
							MW(((u32)p8rxbuff)+14,ADDR_AFN0CF246_Sec+3+(15*Pn),12);
						}
					}
					break;
			}
			break;
	#if (USER/100)==5//�û���ʶ:�Ϻ�
		case 0xF7://ERC68���ܱ��ɿ��������¼���¼����
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//Pn��
				break;
			}
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://�ܴ���
					i=MRR(ADDR_ERC68_SOUR+(3*(Pn-1)),3);//ԭ����
					x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_ERC68_SOUR+(3*(Pn-1)),3);//����ԭ����
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{//�����仯
							p8FnData[0]=1;//��Ч�Ƚ��ܴ�����־
							break;
						}
					}
					UARTCtrl->RMCount++;//�����������
					break;
				case 1://(��1��)���ɿ���������¼
					if(p8FnData[0]!=1)//��Ч�Ƚ��ܴ�����־
					{
						break;
					}
					p8FnData[0]=68;//ERC=68	BIN	1
					p8FnData[1]=39;//����Le	BIN	1
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//��ʱ������//����ʱ�䣺��ʱ������	����¼A.15	5
					p8FnData[7]=Pn;//D15��D12������	D11��D0��pn���������1��2048��	BIN	2
					p8FnData[8]=Pn>>8;
					MR(((u32)p8FnData)+9,ADDR_ERC68_SOUR+(3*(Pn-1)),3);//���ɿ��������ܴ���	����¼A.10	3
					//����ʱ��	����¼A.1	6
					//����ʱ��	����¼A.1	6
					//���ɿ���������״̬	BIN	1
					//���ɿ�����������ʱ�������й��ܵ���	����¼A.11	4
					//���ɿ�����������ʱ�̷����й��ܵ���	����¼A.11	4
					//���ɿ�����������ʱ�������й��ܵ���	����¼A.11	4
					//���ɿ�����������ʱ�̷����й��ܵ���	����¼A.11	4
					MR(((u32)p8FnData)+12,((u32)p8rxbuff)+14,29);
					ERC_Event_Save((u32)p8FnData);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					break;
			}
			break;
	
		case 0xF8://��ʾ��ʧѹȫʧѹ����
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//Pn��
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://��ʧѹ����	����¼A.10	��	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+5,3);
					break;
				case 1://ʧѹʱ���ۼ�ֵ	����¼A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+17,3);
					break;
				case 2://A��ʧѹʱ���ۼ�ֵ	����¼A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+20,3);
					break;
				case 3://B��ʧѹʱ���ۼ�ֵ	����¼A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+23,3);
					break;
				case 4://C��ʧѹʱ���ۼ�ֵ	����¼A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+26,3);
					break;
				case 5://���1��ʧѹ��ʼʱ��	���ʱ������	6
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+29,6);
					break;
				case 6://���1��ʧѹ����ʱ��	���ʱ������	6
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+35,6);
					break;
				case 7://A�����1-10��ʧѹ��ʼʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+41,60);
					break;
				case 8://A�����1-10��ʧѹ����ʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+101,60);
					break;
				case 9://B�����1-10��ʧѹ��ʼʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+161,60);
					break;
				case 10://B�����1-10��ʧѹ����ʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+221,60);
					break;
				case 11://C�����1-10��ʧѹ��ʼʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+281,60);
					break;
				case 12://C�����1-10��ʧѹ����ʱ��	���ʱ������	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+341,60);
					break;
				case 13://����1�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+407,15);
					break;
				case 14://����2�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+422,15);
					break;
				case 15://����3�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+437,15);
					break;
				case 16://����4�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+452,15);
					break;
				case 17://����5�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+467,15);
					break;
				case 18://����6�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+482,15);
					break;
				case 19://����7�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+497,15);
					break;
				case 20://����8�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+512,15);
					break;
				case 21://����9�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+527,15);
					break;
				case 22://����10�Σ�ȫʧѹ����ʱ�̣�����ֵ������ʱ��
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+542,15);
					break;
				case 23://��ѹ��ƽ���ܴ��������ۼ�ʱ��(ERC17�û�)					
					i=MRR(ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);//ԭ����
					x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//��Ч�Ƚ��ܴ�����־
							break;//���¼���¼
						}
					}
					UARTCtrl->RMCount+=3;//����������
					break;
				case 24://��ѹ���ƽ����(ERC17��)
					if(p8FnData[0]!=1)//������Ч
					{
						break;
					}
					x=MRR(((u32)p8rxbuff)+14,3);//���ƽ����
					x>>=4;
					p8FnData=(u8 *)(ADDR_DATABUFF);
					p8FnData[0]=17;//ERC
					p8FnData[1]=27;//����Le
					MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
					Pn++;
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x1;
					p8FnData[10]=x;
					p8FnData[11]=x>>8;
					p8FnData[12]=0xee;
					p8FnData[13]=0xee;
					MC(0xee,ADDR_DATABUFF+14,15);
					ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					UARTCtrl->RMCount+=2;//����������
					break;
				case 25://(������)��ѹ��ƽ���ܴ��������ۼ�ʱ��(ERC17��)
					i=MRR(ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);//ԭ����
					x=MRR(((u32)p8rxbuff)+14,3);//��ǰ����
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//��Ч�Ƚ��ܴ�����־
							break;//���¼���¼
						}
					}
					UARTCtrl->RMCount++;//����������
					break;
				case 26://��ѹ���ƽ����(ERC17��)
					if(p8FnData[0]!=1)//��Ч�Ƚ��ܴ�����־
					{
						break;
					}
					x=MRR(((u32)p8rxbuff)+14+12,2);//���ƽ����
					x>>=4;
					p8FnData=(u8 *)(ADDR_DATABUFF);
					p8FnData[0]=17;//ERC
					p8FnData[1]=27;//����Le
					MR(ADDR_DATABUFF+2,((u32)p8rxbuff)+14+1,5);//��ʱ������
					Pn++;
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x1;
					p8FnData[10]=x;
					p8FnData[11]=x>>8;
					p8FnData[12]=0xee;
					p8FnData[13]=0xee;
					MC(0xee,ADDR_DATABUFF+14,15);
					ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
					break;
			}
			break;
	#endif
	#if (USER/100)==11//����ϵ
		case 0xF9://����ȡ���յ�ѹ�ϸ���ͳ������
		case 0xFA://����ȡ���µ�ѹ�ϸ���ͳ������
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0)
			{
				break;
			}
			Pn--;
			if(p8RMLib[0]==0xf9)
			{
				x=ADDR_AFN0DF27_SOUR_ReadMeter+((66+27)*Pn);
			}
			else
			{
				x=((u32)p8FnData)+6+1;
				if(UARTCtrl->RMCount==0)
				{
					MC(0,x,93);
				}
			}
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://A���ѹ�ϸ���ͳ������
				//���ѹ���ʱ��
					
				//���ѹ�ϸ��� 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+6,3);
				//���ѹ������
				
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+4,2);
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+6,2);
				//����ߵ�ѹ
				//����ߵ�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+13,x+30,5);
				//����͵�ѹ
				//����͵�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+19,x+35,5);

				//���ѹ�ϸ�ʱ�� �����ѹ���ʱ��- ���ѹ������ʱ��-���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+8,2);
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
					    MWR(d64b,x+66,3);		
					}
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+3,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+3,3);					
					}
					break;
				case 1://B���ѹ�ϸ���ͳ������
				//���ѹ���ʱ��
					
				//���ѹ�ϸ��� 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+9+6,3);
				//���ѹ������
				
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+14,2);
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+16,2);
				//����ߵ�ѹ
				//����ߵ�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+13,x+40,5);
				//����͵�ѹ
				//����͵�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+19,x+45,5);

				//���ѹ�ϸ�ʱ�� �����ѹ���ʱ��- ���ѹ������ʱ��-���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+18,2);
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+9,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+9,3);				
					}
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+12,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+12,3);					
					}
					break;
				case 2://C���ѹ�ϸ���ͳ������
				//���ѹ���ʱ��
					
				//���ѹ�ϸ��� 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+18+6,3);
				//���ѹ������
				
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+24,2);
				//���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+26,2);
				//����ߵ�ѹ
				//����ߵ�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+13,x+50,5);
				//����͵�ѹ
				//����͵�ѹ����ʱ��
					MW(((u32)p8rxbuff)+14+19,x+55,5);
				//���ѹ�ϸ�ʱ�� �����ѹ���ʱ��- ���ѹ������ʱ��-���ѹ������ʱ��
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+28,2);
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+18,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+18,3);
					}
				//��Խ������
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+18,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+18+3,3);
					}
					break;
			}
			break;
		case 0xFB://A���ѹ����
			x=p8rxbuff[9];
			if((x==(4+8))||(x==(4+24)))
			{
				i=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰʱ��������
				i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
				i<<=8;
				i=YMDH_AddH(i,UARTCtrl->RMCount);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
				MWR(i,((u32)p8FnData)+6+1,4);
				MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1+4,8);
			}
			break;
	#endif
	}
}













