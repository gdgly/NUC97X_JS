
//�ն˿���-����������
#include "Project.h"
#include "Terminal_Contrl_Energy.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"







#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void Terminal_Contrl_MonthEnergy(void)//�ն˿���-�µ��
{
}
void Terminal_Contrl_BuyEnergy(void)//�ն˿���-�����
{
}
#else
u32 kWhContrl1_Comp(u32 Pn)//�ն˿���-���.�µ�ص������Ƚ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
{
	u32 i;
	u32 x;
	u64 a64;
	u64 b64;

	//ȡ�µ������ض�ֵ
	i=MRR(ADDR_AFN04F46+(5*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
	i&=0xefffffff;//ȥ����λ
	a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	//ȡ�ն��µ������ض�ֵ����ϵ��
	i=MRR(ADDR_AFN04F20,1);//�����������ݵ��Ĵ���,Byte<=8
	x=i;
	i&=0x7f;
	i=bcd_hex(i);
	b64=(a64*i)/100;
	if((x>>7)==0x0)
	{//S0=0����ʾ�ϸ�
		a64+=b64;
	}
	else
	{//S0=1����ʾ�¸�
		a64-=b64;
	}
	//��ǰ��ֵ
	i=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
	MWR(i,ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn)+2,4);//д�Ĵ������ݵ�������,Byte<=8
	//ȡ��ǰ�µ���
	i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//�����������ݵ��Ĵ���,Byte<=8
	i&=0xefffffff;//ȥ����λ
	b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	if(b64>=a64)
	{//��ǰ�µ���>=��ֵ
		return 2;//����
	}

	//��������ֵ(09��̶�Ϊ��ֵ��80%)
	i=MRR(ADDR_AFN04F46+(5*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
	i&=0xefffffff;//ȥ����λ
	a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	//ȡ�������޸���ϵ��
	i=MRR(ADDR_AFN04F46+(5*Pn)+4,1);//�����������ݵ��Ĵ���,Byte<=8
//	x=i;
//	i&=0x7f;
//	i=bcd_hex(i);
//	b64=(a64*i)/100;
//	if((x>>7)==0x0)
//	{//S0=0����ʾ�ϸ�
//		a64+=b64;
//	}
//	else
//	{//S0=1����ʾ�¸�
//		a64-=b64;
//	}
	i=bcd_hex(i);
	b64=i;
	a64*=b64;
	a64/=100;
	
	//ȡ��ǰ�µ���
	i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//�����������ݵ��Ĵ���,Byte<=8
	i&=0xefffffff;//ȥ����λ
	b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	if(b64>=a64)
	{//��ǰ�µ���>=��ֵ80%
		return 1;//����
	}
	return 0;//û����
}


void Terminal_Contrl_MonthEnergy(void)//�ն˿���-�µ��
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	p8s=(u8 *)(ADDR_AFN05F15);
	p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			goto NextPn;
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			p8d[0]=0;
			p8d[1]=0;
		}
		
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				break;
			case 3://����
				i=kWhContrl1_Comp(Pn);//�ն˿���-���.�µ�ص������Ƚ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				switch(i)
				{
					case 1:
						i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//��բʱ������
						x=MRR(((u32)p8d)+2,4);//��բʱ��������ֵ
						ERC23_Event(Pn+1,MRR(ADDR_AFN04F48+(1*Pn),1),1,i,x);//��ظ澯��¼;���Pn�ܼ����,Bin��բ�ִ�,Type������,kwh1�澯ʱ������,kwh2�澯ʱ��������ֵ
						p8d[0]=100;//����
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 100://����
				i=kWhContrl1_Comp(Pn);//�ն˿���-���.�µ�ص������Ƚ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				switch(i)
				{
					case 0:
						p8d[0]=3;//
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 101://��բ׼��
//				p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
//				if(p8[0]!=0x0)
//				{//����
//					i=kWhContrl1_Comp(Pn);//�ն˿���-���.�µ�ص������Ƚ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
//					if(i!=2)
//					{
//						p8d[0]=3;
//					}
//					break;
//				}

				//��բ�ִ�
				x=0;//���ȫ���ִ�
				for(i=0;i<MaxControlRound;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_AFN04F48+(1*Pn),1);//�����������ݵ��Ĵ���,Byte<=8
				i&=x;
				if(i!=0x0)
				{//���ִ���բ
					p8d[1]=i;
					i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);//��բʱ������
					x=MRR(((u32)p8d)+2,4);//��բʱ��������ֵ
					ERC7_Event(Pn+1,p8d[1],1,i,x);//�����բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type������,kwh1��բʱ������,kwh2��բʱ��������ֵ
				}
				p8d[0]=102;
				Terminal_Ctrl_Count(0);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				break;
			case 102://��բ
				i=kWhContrl1_Comp(Pn);//�ն˿���-���.�µ�ص������Ƚ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//���ִ���բ
				}
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}



u32 kWhContrl2_Comp(u32 Pn)//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
{
	u32 i;
	s64 a64;
	s64 b64;

	//ʣ���������
	i=MRR(ADDR_AFN0CF23+(4*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
	a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	//��բ����ֵ
	i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//�����������ݵ��Ĵ���,Byte<=8
	b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	if(a64<=b64)
	{//ʣ���������<=��բ����ֵ
		return 2;//����
	}
	//��������ֵ
	i=MRR(ADDR_AFN04F47+(17*Pn)+9,4);//�����������ݵ��Ĵ���,Byte<=8
	b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
	if(a64<=b64)
	{//ʣ���������<=��������ֵ
		return 1;//����
	}
	return 0;//û����
}


void Terminal_Contrl_BuyEnergy(void)//�ն˿���-�����
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	p8s=(u8 *)(ADDR_AFN05F16);
	p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			goto NextPn;
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			p8d[0]=0;
			p8d[1]=0;//���ִ���բ
		}
		
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				break;
			case 3://����
				i=kWhContrl2_Comp(Pn);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				switch(i)
				{
					case 1:
						i=MRR(ADDR_AFN0CF23+(4*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
						x=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//�����������ݵ��Ĵ���,Byte<=8
						ERC23_Event(Pn+1,MRR(ADDR_AFN04F48+(1*Pn),1),2,i,x);//��ظ澯��¼;���Pn�ܼ����,Bin��բ�ִ�,Type������,kwh1�澯ʱ������,kwh2�澯ʱ��������ֵ
						p8d[0]=100;//����
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 100://����
				i=kWhContrl2_Comp(Pn);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				switch(i)
				{
					case 0:
						p8d[0]=3;//
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 101://��բ׼��

//				p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
//				if(p8[0]!=0x0)
//				{//����
//					i=kWhContrl2_Comp(Pn);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
//					if(i!=2)
//					{
//						p8d[0]=3;
//					}
//					break;
//				}

				//��բ�ִ�
				x=0;//���ȫ���ִ�
				for(i=0;i<MaxControlRound;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_AFN04F48+(1*Pn),1);//�����������ݵ��Ĵ���,Byte<=8
				i&=x;
				if(i!=0x0)
				{//���ִ���բ
					p8d[1]=i;
					i=MRR(ADDR_AFN0CF23+(4*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
					x=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//�����������ݵ��Ĵ���,Byte<=8
					ERC7_Event(Pn+1,p8d[1],2,i,x);//�����բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type������,kwh1��բʱ������,kwh2��բʱ��������ֵ
				}
				p8d[0]=102;
				Terminal_Ctrl_Count(1);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				break;
			case 102://��բ
				i=kWhContrl2_Comp(Pn);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7);����0û����,1����,2����
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//���ִ���բ
				}
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

#endif//#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��












