
//�û���ʶ:ɽ����
#include "Project.h"
#include "../terminal/Terminal_USER2xx.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../terminal/Terminal_AFN0E_Event.h"


#if (USER/100)==2//�û���ʶ:ɽ����

void DayAveragePower(u32 Pn,u32 Power,u32 Count)//��ƽ�����й����ʼ���;���Pn��1��ʼ,Power=�й�����(BCD),Count=���μ������
{
	u32 i;
	u32 x;
	u32 Addr;
	u64 d64;
	
	if((Pn==0)||(Pn>ERC29MaxPn))
	{
		return;
	}
	Pn--;
	if(Power&0x800000)
	{
		return;//������
	}
	Power&=0x7fffff;
	Power=bcd_hex(Power);
	Addr=ADDR_ERC29_SOUR+(Pn*LEN_ERC29_SOUR);
	i=MRR(ADDR_TYMDHMS+3,3);//��ǰ������
	x=MRR(Addr+7,3);//ԭ������
	if(i==x)
	{//ͬ��
		i=MRR(Addr+10,3);//����+1
		i+=Count;
		MWR(i,Addr+10,3);
		d64=MRR(Addr+13,6);//���ʼ�
		d64+=Power;
		MWR(d64,Addr+13,6);
	}
	else
	{//��ͬ��
		MWR(i,Addr+7,3);//��������
		x=YMD_Add1D(x);//�����ռ�1��,���ؼ�1���������
		if(i==x)
		{//����1��
			i=MRR(Addr+10,3);//����
			d64=MRR(Addr+13,6);//���ʼ�
			if(i==0)
			{
				MWR(0,Addr+19,3);
			}
			else
			{
				i=d64/i;
				i=hex_bcd(i);
				MWR(i,Addr+19,3);
			}
		}
		else
		{//������1��,ƽ��������Ч
			MWR(0xee,Addr+19,3);
		}
		MWR(Count,Addr+10,3);
		MWR(Power,Addr+13,6);
	}
}

void ERC29_Event_Day(u32 Pn,u64 LastDayEnergy,u64 TodayEnergy)//�ж�����Ϊ�ն�������ĵ��ܱ����,����;���Pn=�������(1��ʼ),LastDayEnergy=���Ƕ������,ToDayEnergy=���ն������
{
	u32 i;
	u32 x;
	u32 y;
	u32 Energy;
	u32 Errf;
	u32 f;
	double f64;
	u8 * p8;
	u8 * p8s;
	
	if((Pn==0)||(Pn>ERC29MaxPn))
	{
		return;
	}
	p8s=(u8 *)(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)));
	i=ERC_Event_Set(29);//����¼���¼����,����0=����¼,1=��¼
	if(i==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8s[5]=0;
	}
	x=ERC_Event_Set(28);//����¼���¼����,����0=����¼,1=��¼
	if(x==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8s[6]=0;
	}
	i|=x;
	if(i==0)
	{
		p8s[0]=0xee;
		return;
	}
	Energy=MRR(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1))+19,3);
	if((Energy&0xff)==0xee)
	{//ƽ������û����
		return;
	}
	i=MRR(ADDR_TYMDHMS+3,3);//��ǰ������
	x=MRR(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1))+7,3);//�����й�����ƽ���ӵ�������(BCD);��ʼ��ֵ0
	if(i!=x)
	{//ƽ�����ʼ������ڴ�
		return;
	}
	Energy=bcd_hex(Energy);
	f64=Energy;
	f64*=24;
	Energy=(u32)f64;//t=Ӧ�ߵ���kwh,4λС��
	if(Energy<=100)
	{//Ӧ�ߵ���kwh<=0.0100ʱ���Ƚ�
		return;
	}
	LastDayEnergy=bcd16_hex(LastDayEnergy);
	TodayEnergy=bcd16_hex(TodayEnergy);
	if(TodayEnergy>=LastDayEnergy)
	{//��ǰʾֵ>=ԭʾֵ
		i=TodayEnergy-LastDayEnergy;//i=ʾֵ����ĵ���
		y=i*10;
		y/=Energy;//ʾֵ����ĵ���/ʵ��Ӧ�ߵ���=x.x
		y=hex_bcd(y);
		
		f=0;//���߱�־
		Errf=0;//�����־
		x=MRR(ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
		if(x==0)
		{//��ֵ=0
			x=~x;
		}
		if(y>x)
		{//(ʾֵ����ĵ���/ʵ�ʷ�������)>���ܱ������ֵ,����Ϊ���ܱ����
			f=1;//���߱�־
			if(p8s[6]&0x01)
			{//ԭ�����Ѳ���,�����
				Errf=1;//�����־
			}
		}
		else
		{
			x=MRR(ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
			if(x==0)
			{//��ֵ=0
				x=~x;
			}
			if(y>x)
			{//(ʾֵ����ĵ���/ʵ�ʷ�������)>���ܱ�����ֵ,����Ϊ���ܱ���
				Errf=1;//�����־
			}
		}
	//��¼
		LastDayEnergy=hex_bcd16(LastDayEnergy);
		TodayEnergy=hex_bcd16(TodayEnergy);
		//����ָ���¼
		if(Errf==0)
		{
			x=p8s[6]&0x1;
			if(x!=0)
			{
				if((x^Errf)!=0x0)
				{//�仯
					p8s[6]&=0xfe;
					p8s[6]|=Errf;
					p8=(u8 *)(ADDR_DATABUFF+2048);
					p8[0]=28;//ERC
					p8[1]=18;//����Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(Errf<<7);
					MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//�����ʱ��Ӧ�����й�����ʾֵ
					MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//�����ʱ�����й�����ʾֵ
					MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
					ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				}
			}
		}
		//���߻ָ���¼
		if(f==0)
		{
			x=p8s[5]&0x1;
			if(x!=0)
			{
				if((x^f)!=0x0)
				{//�仯
					p8s[5]&=0xfe;
					p8s[5]|=f;
					p8=(u8 *)(ADDR_DATABUFF+2048);
					p8[0]=29;//ERC
					p8[1]=18;//����Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(f<<7);
					MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//���߷���ǰ�����й�����ʾֵ
					MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//���߷����������й�����ʾֵ
					MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
					ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				}
			}
		}
		//���������¼
		x=p8s[6]&0x1;
		if(x==0)
		{
			if((x^Errf)!=0x0)
			{//�仯
				p8s[6]&=0xfe;
				p8s[6]|=Errf;
				p8=(u8 *)(ADDR_DATABUFF+2048);
				p8[0]=28;//ERC
				p8[1]=18;//����Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(Errf<<7);
				MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//�����ʱ��Ӧ�����й�����ʾֵ
				MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//�����ʱ�����й�����ʾֵ
				MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
				ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
		}
		//���߲�����¼
		x=p8s[5]&0x1;
		if(x==0)
		{
			if((x^f)!=0x0)
			{//�仯
				p8s[5]&=0xfe;
				p8s[5]|=f;
				p8=(u8 *)(ADDR_DATABUFF+2048);
				p8[0]=29;//ERC
				p8[1]=18;//����Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(f<<7);
				MWR(LastDayEnergy,ADDR_DATABUFF+2048+9,5);//���߷���ǰ�����й�����ʾֵ
				MWR(TodayEnergy,ADDR_DATABUFF+2048+14,5);//���߷����������й�����ʾֵ
				MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
				ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
		}
	}
}


#endif
















