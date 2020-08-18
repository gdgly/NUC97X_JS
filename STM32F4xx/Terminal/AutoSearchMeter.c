
//�Զ��ѱ�
#include "Project.h"
#include "AutoSearchMeter.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "../Terminal/Terminal_Init.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0C_RealTimeData_Init.h"
#include "Terminal_AFN0D_CongealData_Init.h"


void AutoUpdataAFN04F10ERC3(void)//�Զ�AFN04F10�ĵ��ܱ����õ�ERC3�¼�
{
	u8 *p8;
	
	if(Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer==0)//26 �Զ�����F10�����ϱ�ERC3�¼��붨ʱ��;������ͨ�����ص��Զ����֣���ɲ����������������ɾ����Ӧ�����ò�������������־��F150��������ӦλΪ1���������ϱ���������¼���ERC3������վ,���ǽ����˲�������������־�ĸ�λ������ÿ���ϱ���������¼����m���ӣ�m=0~30��һ��ȡ10��֮�ڣ����۲����Ƿ�����������ٴ��ϱ�
	{
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0;//0=��,1=��AFN04F10���µ���10���ڲ����ϱ����
		Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer=10*60;//��10���ڲ����ϱ�
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=3;//ERC
		p8[1]=14;//����Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
		p8[7]=0;//����վ��ַ
		p8[8]=0;//Pn=0
		p8[9]=0;
		p8[10]=0x2;//Fn=10
		p8[11]=1;
		p8[12]=0;//Pn=0
		p8[13]=0;
		p8[14]=0x20;//Fn=150
		p8[15]=18;
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��
	}
	else
	{
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0x55;//0=��,1=��AFN04F10���µ���10���ڲ����ϱ����
	}
}
/*
void AutoDelAFN04F10(void)//�Զ�ɾ��AFN04F10�ĵ��ܱ�����;��Terminal_ReadMeter_3762.c�еĴ���void DateRMFlags(u32 Pn,u32 f)//�ճ����־;���:f=0�ɹ�,=1ʧ��
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	
	i=MRR(ADDR_AFN05F149+1,1);//�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
	if(i==0x1)
	{	
		if(Terminal_Ram->AutoUpdataAFN04F10_S_Timer==0)//25 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
		{
			for(Pn=0;Pn<AFN04F10MaxPn;Pn++)
			{
				i=MRR(ADDR_RMDateFlags+(4*Pn),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
				i=~i;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				if(i<=2)
				{//����30�쳭��ʧ��
					MC(0,ADDR_AFN04F10+(LEN_AFN04F10_Pn*Pn)+2,2);//�����������=0
					//��������Ч��־
				#if AFN04F150Dn==1//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
					Pn++;
					if(Pn>=2048)
					{
						return;
					}
				#endif
					x=Pn/8;
					i=Pn%8;
					y=MRR(ADDR_AFN04F150+x,1);
					y&=(~(1<<i));
					MWR(y,ADDR_AFN04F150+x,1);
					//��������������־
					y=MRR(ADDR_AFN04F150+256+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+256+x,1);
					//ERC3��������¼�
					AutoUpdataAFN04F10ERC3();//�Զ�AFN04F10�ĵ��ܱ����õ�ERC3�¼�
				}
			}
		}
	}
}
*/
u32 Scand_AFN04F10(u32 PORTn,u64 MeterAddr,u64 CollectorAddr)//����������ĵ����Ϣ��ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
{
	u32 i;
	u32 Addr;
	u32 Meter;
	u64 d64;

	Addr=ADDR_AFN04F10;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr+2,2);
		if(i!=0)
		{//���������Ч
			i=MRR(Addr+4,1);
			i&=0x1f;
			if(i==PORTn)
			{//�˿ں���ͬ
					d64=MRR(Addr+6,6);
					if(d64==MeterAddr)
					{//ͨ�ŵ�ַ��ͬ
						if(CollectorAddr==0xeeeeeeeeeeee)
						{//�ղɼ�����ַ
							return Meter;//��ͬ���������
						}
						d64=MRR(Addr+20,6);
						if(d64==CollectorAddr)
						{//�ɼ�����ַ��ͬ
							return Meter;//��ͬ���������
						}
					}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;

}

u32 Scand_AFN0CF13(u32 PORTn,u64 MeterAddr,u64 CollectorAddr)//����������ĵ����Ϣ��ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
{
	u32 i;
	u32 x;
	u32 Addr;
	u32 Meter;
	u64 d64;
	
	x=MRR(ADDR_AFN0CF13,2);
	if(x>MaxRS485AddCarrierPn)
	{
		x=MaxRS485AddCarrierPn;
	}
	Addr=ADDR_AFN0CF13;
	for(Meter=1;Meter<=x;Meter++)
	{
		i=MRR(Addr+13,1);
		i&=0x1f;
		if(i==PORTn)
		{//�˿ں���ͬ
				d64=MRR(Addr,6);
				if(d64==MeterAddr)
				{//ͨ�ŵ�ַ��ͬ
					if(CollectorAddr==0xeeeeeeeeeeee)
					{//�ղɼ�����ַ
						return Meter;//��ͬ���������
					}
					d64=MRR(Addr+7,6);
					if(d64==CollectorAddr)
					{//�ɼ�����ַ��ͬ
						return Meter;//��ͬ���������
					}
				}
		}
		Addr+=LEN_PerMeterAFN0CF13;
	}
	return 0;
}

void AutoSetAFN04F10(u32 PORTn,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase)//�Զ�����AFN04F10�ĵ��ܱ�����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u32 F10;
	u32 F13;
	
	switch(PORTn)
	{
		case RS485_1PORT:
			PORTn=2;
			break;
		case RS485_2PORT:
			PORTn=3;
			break;
		case RS485_3PORT:
			PORTn=4;
			break;
		case RS485_4PORT://�ز�
			PORTn=31;
			break;
		default:
			return;
	}
	F10=Scand_AFN04F10(PORTn,MeterAddr,CollectorAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
	F13=Scand_AFN0CF13(PORTn,MeterAddr,CollectorAddr);//����������ĵ����Ϣ��ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
	if((F10|F13)==0)
	{
		MWR(MeterAddr,ADDR_DATABUFF+LEN_TERMINAL_EVENT,6);//���ͨ�ŵ�ַ
		i=Phase;//0Ϊ��ȷ����1��3���α�ʾ���Ϊ��1�ࡢ��2�ࡢ��3��
		if((i!=0)&&(i<=3))
		{
			i--;
			i=1<<i;
		}
		else
		{
			i=0;
		}
		i<<=5;//D7��D5��λ���α�ʾ���ʵ����λC�ࡢB�ࡢA�ࣺ��"1"��Ч����"0"��Ч;D3��D0�����ʾ���յ����ز��ź�Ʒ�ʣ���ֵ��Χ1��15
		MWR(i,ADDR_DATABUFF+LEN_TERMINAL_EVENT+6,1);//���������𼰷����߽��յ����ź�Ʒ��
		MWR(1,ADDR_DATABUFF+LEN_TERMINAL_EVENT+7,1);//���ͨ��Э��,b)	D1��D0�����ʾ���ͨ��Э�飬��ֵ��Χ0��3��0��ʾDL/T 645-1997��1��ʾDL/T 645-2007��2��3����
		ERC35_Event(PORTn,1,ADDR_DATABUFF+LEN_TERMINAL_EVENT);//����δ֪����¼�;���Port=�˿ں�,n=δ֪�����,DataAddr=δ֪���ͨ�ŵ�ַ��ÿ��8�ֽ����ݴ�ŵĵ�ַ
	}
	i=MRR(ADDR_AFN05F149+1,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
	switch(i)
	{
		case 1://�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
			if(F10==0)
			{//����ͬ
//				if(Terminal_Ram->AutoUpdataAFN04F10_S_Timer==0)//25 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
//				{
					//ֻע��ɼ���
					if(MeterAddr==0xeeeeeeeeeeee)
					{
						//����ͬ�ɼ���ַ
						x=ADDR_AFN04F10+2;
						for(i=0;i<AFN04F10MaxPn;i++)
						{
							y=MRR(x,2);
							if(y!=0)
							{//���������Ч
								y=MRR(x+2,1);
								y&=0x1f;
								if(y==PORTn)
								{//�˿ں���ͬ
									if(MRR(x+18,6)==CollectorAddr)
									{
										return;//ԭ�Ѵ���
									}
								}
							}
							x+=LEN_AFN04F10_Pn;
						}
					}
					//��ͨ�ŵ�ַΪ0xeeeeeeeeeeee,�ɼ�����ַ��ͬ������(�ҵ���1ֻ����滻ԭ����Ĳ�����)
					i=Scand_AFN04F10(PORTn,0xeeeeeeeeeeee,CollectorAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
					if(i!=0)
					{
						i--;
						x=ADDR_AFN04F10+2+(i*LEN_AFN04F10_Pn);
					}
					else
					{
						//��ͨ�ŵ�ַ��ͬ��ԭλ��(���ܸ����ɼ���)
						i=AFN04F10MaxPn;
						if(MeterAddr!=0xeeeeeeeeeeee)
						{
							x=ADDR_AFN04F10+2;
							for(i=0;i<AFN04F10MaxPn;i++)
							{
								y=MRR(x,2);
								if(y!=0)
								{//���������Ч
									y=MRR(x+2,1);
									y&=0x1f;
									if(y==PORTn)
									{//�˿ں���ͬ
										if(MRR(x+4,6)==MeterAddr)
										{
											break;//�ҵ�ͨ�ŵ�ַ��ͬ��ԭλ��
										}
									}
								}
								x+=LEN_AFN04F10_Pn;
							}
						}
						//�Ҳ������Ϊ0�Ŀ�λ��
						if(i>=AFN04F10MaxPn)
						{
							x=ADDR_AFN04F10+2;
						#if (USER/100)==7//�㽭�û�
							if(PORTn==31)
							{
								x=ADDR_AFN04F10+2+10*LEN_AFN04F10_Pn;
								for(i=10;i<AFN04F10MaxPn;i++)
								{
									if(MRR(x,2)==0)
									{
										break;//�ҵ��������Ϊ0�Ŀ�λ��
									}
									x+=LEN_AFN04F10_Pn;
								}
							}
							else
							{
								for(i=0;i<AFN04F10MaxPn;i++)
								{
									if(MRR(x,2)==0)
									{
										break;//�ҵ��������Ϊ0�Ŀ�λ��
									}
									x+=LEN_AFN04F10_Pn;
								}
							}
						#else
							for(i=0;i<AFN04F10MaxPn;i++)
							{
								if(MRR(x,2)==0)
								{
									break;//�ҵ��������Ϊ0�Ŀ�λ��
								}
								x+=LEN_AFN04F10_Pn;
							}
						#endif
						}
					}
					if(i>=AFN04F10MaxPn)
					{//û����
						return;
					}
					Pn=i+1;
					MC(0,ADDR_DATABUFF,LEN_AFN04F10_Pn);
					MWR(Pn,ADDR_DATABUFF,2);//���ܱ�/��������װ����� BIN 2 
					MWR(Pn,ADDR_DATABUFF+2,2);//����������� BIN 2 
				#if (USER/100)==7//�㽭�û�
					if(Bps==0)
					{
						if(Protocol==1)
						{//DLT645-1997
							Bps=2;//1200
						}
						else
						{//DLT645-2007
							Bps=3;//2400
						} 
					}
				#endif
					MWR(PORTn+(Bps<<5),ADDR_DATABUFF+4,1);//ͨ�����ʼ��˿ں� BIN 1 
					MWR(Protocol,ADDR_DATABUFF+5,1);//ͨ��Э������ BIN 1 
					MWR(MeterAddr,ADDR_DATABUFF+6,6);//ͨ�ŵ�ַ ����¼A.12 6 
						//ͨ������ BIN 6 
					MWR(4,ADDR_DATABUFF+18,1);//���ܷ��ʸ��� BS8 1
					MWR(0x09,ADDR_DATABUFF+19,1);//�й�����ʾֵ����λ��С��λ���� BS8 1 
					MWR(CollectorAddr,ADDR_DATABUFF+20,6);//�����ɼ���ͨ�ŵ�ַ ����¼A.12 6 
					MWR(DefaultClassNo,ADDR_DATABUFF+26,1);//�û�����ż��û�С��� BS8 1;(�����û�E��5)
					x-=2;
					MW(ADDR_DATABUFF,x,LEN_AFN04F10_Pn);
					MC(0,ADDR_RMDateFlags+(4*i),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
				
					//��������Ч��־
			#if AFN04F150Dn==1//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
					i++;
					if(i>=2048)
					{
						return;
					}
			#endif
					x=i/8;
					i%=8;
					y=MRR(ADDR_AFN04F150+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+x,1);
					PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
					//��������������־
					y=MRR(ADDR_AFN04F150+256+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+256+x,1);
					//ERC3��������¼�
					AutoUpdataAFN04F10ERC3();//�Զ�AFN04F10�ĵ��ܱ����õ�ERC3�¼�
					
					//���������ݳ�ʼ��(ͬUART����ʱ�Ĳ�����ű仯)
					Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
					Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
					Init_Class1Data(pnType,Pn);//��1���ݳ�ʼ��(ָ��Pn����)
					Init_Class2Data(Pn);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
					Init_Class3Data(Pn);//��3����(�¼�)��ʼ��(ָ���Ĳ���������,Pn=1~n)
					Init_EnergyInc_Souser_Pn(Pn);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
					Init_Energy_Pn(Pn);//���������µ�������Ϊ0(ָ����Pn����)
		#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==31)
					{
						Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
						Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
					}
		#endif
//				}
			}
			//break;//ͬʱд��AFN04F10��AFN0CF13
		case 2://�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
			if(F13==0)
			{
				MC(0,ADDR_DATABUFF,LEN_PerMeterAFN0CF13);
				MWR(MeterAddr,ADDR_DATABUFF,6);//6BYTE ���ܱ��ַ
				MWR(Protocol,ADDR_DATABUFF+6,1);//1BYTE ͨ��Э������
				MWR(CollectorAddr,ADDR_DATABUFF+7,6);//6BYTE �����ɼ���ͨ�ŵ�ַ
				MWR(PORTn,ADDR_DATABUFF+13,1);//1BYTE �˿ں�
				MWR(Phase,ADDR_DATABUFF+14,1);//1BYTE �����λ
				
				z=MRR(ADDR_AFN0CF13,2);
				if(z>=MaxRS485AddCarrierPn)
				{
					z=0;//��¼��������ȫ����¼��¼���Բ�������ڵı�
				}
				//ֻע��ɼ���
				if(MeterAddr==0xeeeeeeeeeeee)
				{
					//����ͬ�ɼ���ַ
					x=ADDR_AFN0CF13+2;
					for(i=0;i<z;i++)
					{
						y=MRR(x+13,1);
						y&=0x1f;
						if(y==PORTn)
						{//�˿ں���ͬ
							if(MRR(x+7,6)==CollectorAddr)
							{
								return;//ԭ�Ѵ���
							}
						}
						x+=LEN_PerMeterAFN0CF13;
					}
				}
				//��ͨ�ŵ�ַΪ0xeeeeeeeeeeee,�ɼ�����ַ��ͬ������(�ҵ���1ֻ����滻ԭ����Ĳ�����)
				i=Scand_AFN0CF13(PORTn,0xeeeeeeeeeeee,CollectorAddr);//����������ĵ����Ϣ��ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(i!=0)
				{
					i--;
					x=ADDR_AFN0CF13+2+(i*LEN_PerMeterAFN0CF13);
					MW(ADDR_DATABUFF,x,LEN_PerMeterAFN0CF13);
				}
				else
				{
					//��ͨ�ŵ�ַ��ͬ��ԭλ��(���ܸ����ɼ���)
					i=MaxRS485AddCarrierPn;
					if(MeterAddr!=0xeeeeeeeeeeee)
					{
						x=ADDR_AFN0CF13+2;
						for(i=0;i<z;i++)
						{
							y=MRR(x+13,1);
							y&=0x1f;
							if(y==PORTn)
							{//�˿ں���ͬ
								if(MRR(x,6)==MeterAddr)
								{
									//�����ɼ���
									MW(ADDR_DATABUFF,ADDR_AFN0CF13+2+(i*LEN_PerMeterAFN0CF13),LEN_PerMeterAFN0CF13);
									return;//ԭ�Ѵ���
								}
							}
							x+=LEN_PerMeterAFN0CF13;
						}
					}
					//ԭû����
					MW(ADDR_DATABUFF,ADDR_AFN0CF13+2+(z*LEN_PerMeterAFN0CF13),LEN_PerMeterAFN0CF13);
					z++;
					MWR(z,ADDR_AFN0CF13,2);
				}
			}
			break;
	}
}


