
//�ն˵�����������
#include "Project.h"
#include "Terminal_impIn.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"









void Init_impInData_Command(void)//�ն˵��������������ݳ�ʼ��(����,ȫ�����)
{
	MC(0,ADDR_Terminal_impIn_FM_START,(ADDR_Terminal_impIn_FM_END)-(ADDR_Terminal_impIn_FM_START));//�������������������
	MC(0,ADDR_Terminal_impIn_ESRAM_START,(ADDR_Terminal_impIn_ESRAM_END)-(ADDR_Terminal_impIn_ESRAM_START));//������������ESARM
}


void Terminal_impIn_Energy(void)//�ն���������-����
{
#if MaxImpInPort!=0//�����������˿���
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 Port;
	u32 imp;
	u64 d64;
	u8 * p8;
	u16 * p16;
	
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		//ȡ�ж��м�������
		p16=(u16 *)(ADDR_Terminal_impIn_Count+(2*Port));
		__disable_irq();
		imp=p16[0];
		p16[0]=0;
		__enable_irq();
		
//		//״̬�������־
//		i=MRR(ADDR_AFN04F12,1);//�ն�״̬���������
//		i>>=Port;
//		i&=1;
//		if(i!=0x0)
//		{//����,���������������
//			imp=0;//�������=0
//		}
		//�������������
		p16=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));
		p16[0]+=imp;
		
		//��������
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//�����������!=0
			if(Pn<=AFN0CF129MaxPn)
			{//��������Ч
				MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*Port),(4+(5*(TMaxTariff+1))));//��ԭ����ֵ
				if(imp!=0x0)
				{//����������
					i=MRR(ADDR_AFN04F11+(5*Port)+3,2);//ȡ�����
					if(i!=0x0)
					{//�����!=0
						imp*=10000;
						imp+=MRR(ADDR_DATABUFF+256,4);
						MWR(imp%i,ADDR_DATABUFF+256,4);//д�Ĵ������ݵ�������,Byte<=8
						imp/=i;
						//�ܵ���
						d64=MRR(ADDR_DATABUFF+256+4,5);
						d64+=imp;
						d64%=10000000000;
						MWR(d64,ADDR_DATABUFF+256+4,5);
						//���ʵ���
						p8=(u8 *)(ADDR_Terminal_Period);
						i=p8[0];
						if((i==0x0)||(i>TMaxTariff))
						{
							i=TMaxTariff;
						}
						d64=MRR(ADDR_DATABUFF+256+4+(5*i),5);
						d64+=imp;
						d64%=10000000000;
						MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);
						MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*Port),(4+(5*(TMaxTariff+1))));
					}
				}
				//����������1����
				MW(ADDR_DATABUFF+256+4,ADDR_DATABUFF+512,5*(TMaxTariff+1));
				p8=(u8 *)(ADDR_DATABUFF+(256+4-1));
				p8[0]=TMaxTariff;//������
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//��������
				{
					case 0://�����й�
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);//д�Ĵ������ݵ�������,Byte<=8
						}
						Fn=129;
						break;
					case 1://�����޹�
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							d64>>=8;//2λС��
							MWR(d64,ADDR_DATABUFF+256+4+(4*i),4);//д�Ĵ������ݵ�������,Byte<=8
						}
						//ͬ������������й���������ΪTMaxTariff,����F33,F34,F35,F36����޷�����
						i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						p8=(u8 *)(i);
						p8[5]=TMaxTariff;
						Fn=130;
						break;
					case 2://�����й�
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							MWR(d64,ADDR_DATABUFF+256+4+(5*i),5);//д�Ĵ������ݵ�������,Byte<=8
						}
						Fn=131;
						break;
					case 3://�����޹�	
						for(i=0;i<=TMaxTariff;i++)
						{
							d64=MRR(ADDR_DATABUFF+512+(5*i),5);
							d64=hex_bcd16(d64);
							d64>>=8;//2λС��
							MWR(d64,ADDR_DATABUFF+256+4+(4*i),4);//д�Ĵ������ݵ�������,Byte<=8
						}
						//ͬ������ķ����й���������ΪTMaxTariff,����F33,F34,F35,F36����޷�����
						i=GetClass1DataAddr(131,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						p8=(u8 *)(i);
						p8[5]=TMaxTariff;
						Fn=132;
						break;
					default:
						continue;
				}
				ReadMeter_SaveFnData(0,Fn,Pn,ADDR_DATABUFF+(256+4-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				//��ǰ����F33,F34���
				ReadMeter_SaveFnData(0,33,Pn,ADDR_DATABUFF+(256+4-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				ReadMeter_SaveFnData(0,34,Pn,ADDR_DATABUFF+(256+4-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
			}
		}
	}
#endif//#if MaxImpInPort!=0//�����������˿���(����״̬������˿���)
}

void Terminal_impIn_Demand(void)//�ն���������-���ʺ��������
{
#if MaxImpInPort!=0//�����������˿���
	u32 i;
	u32 x;
	u32 Fn;
	u32 Pn;
	u32 Port;
	u32 imp;
	u64 a64;
	u64 b64;
	u64 ActivePointFlags;//�й����ʲ������־
	u64 ReactivePointFlags;//�޹����ʲ������־
	u8 * p8;
	u16 * p16s;

	if(Terminal_Ram->SlipCYC_S_Timer!=0x0)
	{
		return;
	}
	Terminal_Ram->SlipCYC_S_Timer=15;//�´�15�붨ʱ
//ÿ15�빦�ʼ���ͬʱ������1����
//����=(1������������*60)/(�����)
	ActivePointFlags=0;//�й����ʲ������־
	ReactivePointFlags=0;//�޹����ʲ������־
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//�����������!=0
			if(Pn<=AFN0CF25MaxPn)
			{//��������Ч
				i=MRR(ADDR_AFN04F11+(5*Port)+3,2);//ȡ�����
				if(i!=0x0)
				{//�����!=0
					p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
					a64=p16s[0];//1����Ϊ4��15��
					a64+=p16s[1];
					a64+=p16s[2];
					a64+=p16s[3];
					b64=60*10000;//4λС��
					a64*=b64;
					i=a64/i;
					if(i>=800000)
					{
						i=0;//���
					}
					i=hex_bcd(i);
				}
				x=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
				MR(x,ADDR_TYMDHMS+1,5);//�ն˳���ʱ��
				a64=1;
				a64<<=(Pn-1);
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//��������
				{
					case 0://�����й�
						if((ActivePointFlags&a64)==0)
						{
							ActivePointFlags|=a64;//�й����ʲ������־
							MWR(i,x+5,3);//д�Ĵ������ݵ�������,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								MWR(i,x+5,3);//д�Ĵ������ݵ�������,Byte<=8
							}
						}
						break;
					case 1://�����޹�
						if((ReactivePointFlags&a64)==0)
						{
							ReactivePointFlags|=a64;//�޹����ʲ������־
							MWR(i,x+5+12,3);//д�Ĵ������ݵ�������,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								MWR(i,x+5+12,3);//д�Ĵ������ݵ�������,Byte<=8
							}
						}
						break;
					case 2://�����й�
						if((ActivePointFlags&a64)==0)
						{
							ActivePointFlags|=a64;//�й����ʲ������־
							i|=0x800000;
							MWR(i,x+5,3);//д�Ĵ������ݵ�������,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								i|=0x800000;
								MWR(i,x+5,3);//д�Ĵ������ݵ�������,Byte<=8
							}
						}
						break;
					case 3://�����޹�
						if((ReactivePointFlags&a64)==0)
						{
							ReactivePointFlags|=a64;//�޹����ʲ������־
							i|=0x800000;
							MWR(i,x+5+12,3);//д�Ĵ������ݵ�������,Byte<=8
						}
						else
						{
							if(i!=0)
							{
								i|=0x800000;
								MWR(i,x+5+12,3);//д�Ĵ������ݵ�������,Byte<=8
							}
						}
						break;
					default:
						continue;
				}
			}
		}
	}

//����=(����������������*60)/(�����*��������)
	if(Terminal_Ram->DemandCYC_S_Timer==0x0)//7 �ն������������������붨ʱ��
	{
		for(Port=0;Port<MaxImpInPort;Port++)
		{
			Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
			if(Pn!=0x0)
			{//�����������!=0
				if(Pn<=AFN0CF145MaxPn)
				{//��������Ч
					MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//��ԭ�������
					p8=(u8 *)(ADDR_DATABUFF+256);
					i=p8[6];//ԭ���������������
					p8=(u8 *)(ADDR_TYMDHMS);
					x=p8[4];//��ǰʵʱ����
					if(i!=x)
					{//�¸���
						if((p8[1]|p8[2])!=0x0)
						{//ʵʱ��ʱ��!=0x0,Լ���¶������
							MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//�¸�����0
						}
						else
						{
							if((Terminal_Ram->CongealFlags&0x10)==0x10)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
							{//�¶������
								MC(0,ADDR_DATABUFF+256,7*(TMaxTariff+1));//�¸�����0
							}
						}
					}
					i=MRR(ADDR_AFN04F11+(5*Port)+3,2);
					if(i!=0x0)
					{//�����!=0
						p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
						imp=0;
						for(x=0;x<(4*15);x++)
						{
							imp+=p16s[x];
						}
						a64=imp;
						b64=60*10000;//4λС��
						a64*=b64;//����������������*60*10000(4λС��)
						i*=15;//�����*��������
						i=a64/i;
						i=hex_bcd(i);
						if(i>0x799999)
						{
							i=0;//���
						}
					}
					else
					{//�����=0
						i=0;
					}
					//����������Ƚ�
					x=MRR(ADDR_DATABUFF+256,3);//�����������ݵ��Ĵ���,Byte<=8
					if(i>=x)
					{//�Ƚ���>=,��Ϊ0ʱҲ��д��1����ʱ���ֵ,������ֵ��ͬ�޷�������1����
						MWR(i,ADDR_DATABUFF+256,3);//д�Ĵ������ݵ�������,Byte<=8
						MR(ADDR_DATABUFF+256+3,ADDR_TYMDHMS+1,4);
					}
					//������������Ƚ�
					p8=(u8 *)(ADDR_Terminal_Period);
					x=p8[0];
					if(x!=0x0)
					{
						if(x<=TMaxTariff)
						{
							if(i>=MRR(ADDR_DATABUFF+256+(7*x),3))
							{//�Ƚ���>=,��Ϊ0ʱҲ��д��1����ʱ���ֵ,������ֵ��ͬ�޷�������1����
								MWR(i,ADDR_DATABUFF+256+(7*x),3);//�����������ݵ��Ĵ���,Byte<=8
								MR(ADDR_DATABUFF+256+3+(7*x),ADDR_TYMDHMS+1,4);
							}
						}
					}
					MW(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//дԭ�������
				}
			}
		}
	}
	
//���廬��
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		p16s=(u16 *)(ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*Port));//�ն���������,�������廬��(�ֶ���)
		for(i=((4*15)-2);i!=0x0;i--)
		{
			p16s[i+1]=p16s[i];
		}
		p16s[1]=p16s[0];
		p16s[0]=0;
	}


//ÿ1�����������������1����
	if((Terminal_Ram->CongealFlags&0x1f)!=0x1f)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	{//�����ڶ�������
		return;
	}
	for(Port=0;Port<MaxImpInPort;Port++)
	{
		Pn=MRR(ADDR_AFN04F11+(5*Port)+1,1);
		if(Pn!=0x0)
		{//�����������!=0
			if(Pn<=AFN0CF145MaxPn)
			{//��������Ч
				MR(ADDR_DATABUFF+256,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*Port),7*(TMaxTariff+1));//��ԭ�������
				p8=(u8 *)(ADDR_DATABUFF+(256-1));
				p8[0]=TMaxTariff;//������
				switch(MRR(ADDR_AFN04F11+(5*Port)+2,1))//��������
				{
					case 0://�����й�
						Fn=145;
						break;
					case 1://�����޹�
						//ͬ������������й���������ΪTMaxTariff,����F33,F34,F35,F36��Ϸ�����=0
						i=GetClass1DataAddr(145,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							p8=(u8 *)(i);
							p8[5]=TMaxTariff;
						}
						Fn=146;
						break;
					case 2://�����й�
						Fn=147;
						break;
					case 3://�����޹�	
						//ͬ������ķ����й���������ΪTMaxTariff,����F33,F34,F35,F36��Ϸ�����=0
						i=GetClass1DataAddr(147,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							p8=(u8 *)(i);
							p8[5]=TMaxTariff;
						}
						Fn=148;
						break;
					default:
						continue;
				}
				ReadMeter_SaveFnData(0,Fn,Pn,ADDR_DATABUFF+(256-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				//��ǰ����F35,F36���
				ReadMeter_SaveFnData(0,35,Pn,ADDR_DATABUFF+(256-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				ReadMeter_SaveFnData(0,36,Pn,ADDR_DATABUFF+(256-6-1),0);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
			}
		}
	}
#endif//#if MaxImpInPort!=0//�����������˿���(����״̬������˿���)
}












