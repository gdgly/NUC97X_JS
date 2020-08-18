
#include "Project.h"
#include "Terminal_ReadMeter_Note.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "AutoSearchMeter.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"





void ReadMeterNote_OK(u32 PORTn,u32 Pn,u32 Info)//���г���������Ϣ.�ɹ�
{
	u32 i;
	u8 *p8;

	if((Pn==0)||(Pn>AFN0CF170MaxPn))
	{
		return;
	}
	Pn--;
	p8=(u8 *)(ADDR_AFN0CF170+(18*Pn));
//�����ն�ͨ�Ŷ˿ں�	BIN	1
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			p8[0]=2;
			break;
		case RS485_1PORT:
			p8[0]=2;
			break;
		case RS485_2PORT:
			p8[0]=3;
			break;
		case RS485_3PORT:
			p8[0]=4;
			break;
		case RS485_4PORT:
			p8[0]=31;
			break;
	}
	if(PORTn==RS485_4PORT)
	{
//�м�·�ɼ���	BIN	1
		i=Info;
		i&=0xff;
		p8[1]=i>>4;
//�ز�����ͨ����λ	BS8	1
		i=Info>>16;
		i=i&0xf;
		if((i!=0)&&(i<=3))
		{
			i--;
			i=1<<i;
			p8[2]=i;
		}
		else
		{
			p8[2]=0;
		}
//�ز��ź�Ʒ��	BS8	1
		p8[3]=Info>>24;
	}
	else
	{
//�м�·�ɼ���	BIN	1
		p8[1]=0;
//�ز�����ͨ����λ	BS8	1
		p8[2]=0;
//�ز��ź�Ʒ��	BS8	1
		p8[3]=0;
	}
//���һ�γ���ɹ�/ʧ�ܱ�־	BIN	1
	p8[4]=1;
//���һ�γ���ɹ�ʱ��	����¼A.1	6
	MW(ADDR_TYMDHMS,((u32)p8)+5,6);
//���һ�γ���ʧ��ʱ��	����¼A.1	6
//�������ʧ���ۼƴ���	BIN	1	
	p8[17]=0;
}

void ReadMeterNote_Fail(u32 PORTn,u32 Pn)//���г���������Ϣ.ʧ��
{
	u8 *p8;

	if((Pn==0)||(Pn>AFN0CF170MaxPn))
	{
		return;
	}
	Pn--;
	p8=(u8 *)(ADDR_AFN0CF170+(18*Pn));
//�����ն�ͨ�Ŷ˿ں�	BIN	1
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			p8[0]=2;
			break;
		case RS485_1PORT:
			p8[0]=2;
			break;
		case RS485_2PORT:
			p8[0]=3;
			break;
		case RS485_3PORT:
			p8[0]=4;
			break;
		case RS485_4PORT:
			p8[0]=31;
			break;
	}
	if(PORTn==RS485_4PORT)
	{
//�м�·�ɼ���	BIN	1
//�ز�����ͨ����λ	BS8	1
//�ز��ź�Ʒ��	BS8	1
	}
	else
	{
//�м�·�ɼ���	BIN	1
		p8[1]=0;
//�ز�����ͨ����λ	BS8	1
		p8[2]=0;
//�ز��ź�Ʒ��	BS8	1
		p8[3]=0;
	}
//���һ�γ���ɹ�/ʧ�ܱ�־	BIN	1
	p8[4]=0;
//���һ�γ���ɹ�ʱ��	����¼A.1	6
//���һ�γ���ʧ��ʱ��	����¼A.1	6
	MW(ADDR_TYMDHMS,((u32)p8)+11,6);
//�������ʧ���ۼƴ���	BIN	1	
	if(p8[17]!=0xff)
	{
		p8[17]++;
	}
}

void DateRMFlags(u32 Pn,u32 f)//�ճ����־;���:f=0�ɹ�,=1ʧ��
{
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#if (USER/100)==7//�û���ʶ:�㽭��������Զ�ɾ��
	u32 i;
	u32 x;
	u32 y;
	
	i=MRR(ADDR_AFN05F149+1,1);//�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
	if(i!=0x1)
	{	
		return;
	}
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	Pn--;
	//1-31�ճ���ɹ�ʧ�ܱ�־,�����Զ�ɾ��
	//i=MRR(ADDR_AFN05F149+1,1);//�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
	//if(i==0x1)
	//{	
		i=Comm_Ram->TYMDHMS[3];//��,��,ʱ,��,��,��
		i=bcd_hex(i);
		if((i!=0)&&(i<=31))
		{
			i=(1<<i);
			x=MRR(ADDR_RMDateFlags+(4*Pn),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
			if(f==0)
			{//�ɹ�
				i=(~i);
				x&=i;
				MWR(x,ADDR_RMDateFlags+(4*Pn),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
			}
			else
			{//ʧ��
				x|=i;
				MWR(x,ADDR_RMDateFlags+(4*Pn),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
				//�Զ�ɾ��AFN04F10�ĵ��ܱ�����
				i=~x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				if(i<=2)
				{//����30�쳭��ʧ��
					i=Check_AFN04F10_Pn(Pn+1);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
					if(i!=0)
					{
						i--;
						MC(0,ADDR_AFN04F10+(LEN_AFN04F10_Pn*i)+2,2);//�����������=0
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
						Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
						y=MRR(ADDR_AFN04F150+256+x,1);
						y|=(1<<i);
						MWR(y,ADDR_AFN04F150+256+x,1);
						//ERC3��������¼�
						AutoUpdataAFN04F10ERC3();//�Զ�AFN04F10�ĵ��ܱ����õ�ERC3�¼�
						//
						PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
						AFN04F150_Computer();//��������Ч��־����
					}
				}
			}
		}
	//}
#endif
#endif
}




