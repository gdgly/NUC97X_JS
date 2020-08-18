
//�ն˿���-����;���Ƶ����ȼ��ɸߵ����ǵ�ǰ�����¸��ء�Ӫҵ��ͣ���ء����ݹ��ء�ʱ�ι���
#include "Project.h"
#include "Terminal_Contrl_Power.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"








#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void Terminal_Contrl1(void)//�ն˿���-����.��ǰ�����¸���
{
}
void Terminal_Contrl2(void)//�ն˿���-����.Ӫҵ��ͣ����
{
}
void Terminal_Contrl3(void)//�ն˿���-����.���ݹ���
{
}
void Terminal_Contrl4(void)//�ն˿���-����.ʱ�ι���
{
}
#else
u32 NextTrip(u32 Bin,u32 Pn)//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
{
	u32 i;
	
	i=MRR(ADDR_AFN04F45+Pn,1);//�����ִ��趨(�ܼ����)
	Bin^=i;
	Bin&=i;
	Bin=BitLoca(Bin);//�����1����λΪ1��λ��(0-32)
	if(Bin!=0x0)
	{
		if(Bin>MaxControlRound)
		{
			Bin=0;
		}
		else
		{
			Bin--;
			Bin=1<<Bin;
		}
	}
	return Bin;
}

u32 kWContrlSlipTime(u32 Pn)//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
{
	u32 i;
	i=MRR(ADDR_AFN04F43+Pn,1);//���ʿ��ƵĹ��ʼ��㻬��ʱ��(��Χ��1��60)	BIN	min	1
	if(i==0)
	{
		i=1;
	}
	if(i>60)
	{
		i=60;
	}
	i*=60;
	return i;
}

u32 kWContrl_Comp(u32 FixedValue,u32 Pn)//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
{
	u32 i;
	u64 a64;
	u64 b64;
	u32 f;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//���ޱ�־=0
	i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
	p16timer[Pn]=i;//�´μ���ʱ��
	//��ǰ����
	i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
	a64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
	if((a64>>63)!=0x0)
	{//��
		a64=(~a64)+1;
	}
	//������ֵ
	i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
	b64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
	if((b64>>63)!=0x0)
	{//��
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//��ǰ����>=������ֵ
		b64=Data02_kW(FixedValue);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		if((b64>>63)!=0x0)
		{//��
			b64=(~b64)+1;
		}
		if(a64>=b64)
		{
			f=1;//����
		}
	}
	return f;
}

u32 kWContrl1_Comp(u32 Pn)//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
{
	u32 i;
	u64 a64;
	u64 b64;
	u32 f;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//���ޱ�־=0
//	i=MRR(ADDR_AFN05F12+1,1);//��ǰ�����¸��ض�ֵ����ʱ�䣺���ݷ�Χ��1��60
//	if(i==0)
//	{
//		i=1;
//	}
//	if(i>60)
//	{
//		i=60;
//	}
//	i*=60;
	i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
	p16timer[Pn]=i;//�´μ���ʱ��
	//��ǰ����
	i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
	a64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
	if((a64>>63)!=0x0)
	{//��
		a64=(~a64)+1;
	}
	//������ֵ
	i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
	b64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
	if((b64>>63)!=0x0)
	{//��
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//��ǰ����>=������ֵ
		i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
		b64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		if(a64>=b64)
		{
			f=1;//����
		}
	}
	return f;
}


void Terminal_Contrl1(void)//�ն˿���-����.��ǰ�����¸���
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F12);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0ֵ��ʾͶ�뵫û��ԭ���1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		if(MRR(((u32)p8s),1)==0x55)
		{//��Ͷ��
				i=MRR(((u32)p8s)+4,1)*30;//��ǰ�����¸��صĿ���ʱ��0.5h
				MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);//д�Ĵ������ݵ�������,Byte<=8
				MC(0x5a,(u32)p8s,1);//0x55->0x5a
				MC(0xee,ADDR_kWCtrl1_FixedValue+(Pn*2),2);//����ƶ�ֵ
				p8d[0]=0;
				p8d[1]=0;//��բ�ִ�=0
				p8d[8]=0;//����ֵ��־=0
				goto NextPn;
		}
		if(MRR(((u32)p8s),1)!=0x5a)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			p8d[8]=0;//����ֵ��־=0
			goto NextPn;
		}
		i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
		if(i==0x0)
		{//����ʱ�䶨ʱ��
			p8d[4]=1;//!=0����ʱ�䶨ʱ��
			goto NoCtrl;//������
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
				p16[Pn]=MRR(((u32)p8s)+3,1)*60;//�غ��ܼ��й����ʶ�����ʱʱ��
				
				i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
				if(i!=0xeeee)
				{//��ֵ�Ѽ���
					p8d[0]=2;
				}
				else
				{
					p8d[0]=1;
					i=MRR(((u32)p8s)+1,1);//��ǰ�����¸��ض�ֵ����ʱ�䣺���ݷ�Χ��1��60
					if(i==0)
					{
						i=1;
					}
					if(i>60)
					{
						i=60;
					}
					i*=60;
					p16timer[Pn]=i;
					MC(0xee,ADDR_AFN0CF24+(Pn*2),2);//��غ��ܼ��й����ʶ���ֵ
				}
				break;
			case 1://��ֵ����ʱ����ƽ�����ʼ���Ͷ�ֵ����
				if(p16timer[Pn]==0x0)
				{//����ʱ����ʱ��
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
					a64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
					if((a64>>63)!=0x0)
					{//��
						a64=(~a64)+1;
					}
					i=MRR(((u32)p8s)+2,1);//��ǰ�����¸��ض�ֵ����ϵ��(��ֵ����ϵ���� S0	BCD��ʮλ��0-7��	BCD���λ;S0���壺S0=0����ʾ�ϸ���S0=1����ʾ�¸�)
					i&=0x7f;
					i=bcd_hex(i);
					b64=(a64*i)/100;
					if((MRR(((u32)p8s)+2,1)>>7)==0x0)
					{//S0=0����ʾ�ϸ�
						a64+=b64;
					}
					else
					{//S0=1����ʾ�¸�
						a64-=b64;
						if((a64>>63)!=0x0)
						{//��ֵ
							a64=0;
						}
					}
					i=kW_Data02(a64);//��λͳһΪkw��3λС����hex����תΪ���ݸ�ʽ02
					MWR(i,ADDR_kWCtrl1_FixedValue+(Pn*2),2);//д�Ĵ������ݵ�������,Byte<=8
					p8d[0]=2;
				}
				break;
			case 2:
				p8d[0]=3;
				break;
			case 3://����
				//�ȼ��غ��ܼ��й����ʶ���
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
				if(p16[Pn]==0x0)
				{//�غ��ܼ��й����ʶ�����ʱʱ�䵽
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//�غ��ܼ��й����ʶ���
					}
				}
				if(p16timer[Pn]==0x0)
				{//���ʿ��ƵĹ��ʼ��㻬��ʱ��(�ܼ����)����ʱ����ʱ��		
					x=kWContrl1_Comp(Pn);//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						p8d[8]=1;//����ֵ��־=0
						p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
						if(p8[0]!=0x0)
						{//����
							break;
						}
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							if(i>4)
							{//��ǰ�����¸���ֻ��4�ָ澯ʱ��
								i=4;
							}
							i--;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=MRR(((u32)p8s)+5+i,1)*60;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//����ֵ��־=0
					}
				}
				break;
			case 100://����
				//��������բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i==0x0)
				{//������,���ٸ澯
					p8d[0]=3;
					break;
				}
				//�ȼ��غ��ܼ��й����ʶ���
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
				if(p16[Pn]==0x0)
				{//�غ��ܼ��й����ʶ�����ʱʱ�䵽
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//�غ��ܼ��й����ʶ���
					}
				}
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=kWContrl1_Comp(Pn);//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
					if(x==0x0)
					{//û����
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//������ʱ��
					p8d[0]=101;
				}
				break;
			case 101://��բ׼��
				//��բʱ����
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//д�Ĵ������ݵ�������,Byte<=8
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				//�ȼ��غ��ܼ��й����ʶ���
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
				if(p16[Pn]==0x0)
				{//�غ��ܼ��й����ʶ�����ʱʱ�䵽
					i=MRR(ADDR_AFN0CF24+(Pn*2),2);
					if(i==0xeeee)
					{
						i=MRR(ADDR_AFN0CF17+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
						MWR(i,ADDR_AFN0CF24+(Pn*2),2);//�غ��ܼ��й����ʶ���
					}
				}
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//�¼���¼
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//��բ2�ֺ���
					ERC6_Event(Pn+1,p8d[9],8,MRR(((u32)p8d)+2,2),i,MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2));//������բ��¼;���Fn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
					p8d[0]=103;
				}
				break;
			case 103://��բ2���Ӻ�
				p8d[0]=3;
				//���ػ���ʱ��
				kWContrl1_Comp(Pn);//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
				if(p16timer[Pn]>=120)
				{
					p16timer[Pn]-=120;//��բ�¼���¼����ʱ2����
				}
				else
				{
					p16timer[Pn]=0;
				}
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=9;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl2(void)//�ն˿���-����.Ӫҵ��ͣ����
{
	u32 i;
	u32 x;
	u32 Pn;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F11);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0ֵ��ʾͶ�뵫û��ԭ���1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			p8d[8]=0;//����ֵ��־=0
			goto NextPn;
		}

//#define ADDR_AFN04F44     ADDR_AFN04F43+(1*MaxTotalAddGroup)//Ӫҵ��ͣ�ز���(�ܼ����)
//��ͣ��ʼʱ��	���ݸ�ʽ20	������	3
//��ͣ����ʱ��	���ݸ�ʽ20	������	3
//��ͣ�ع��ʶ�ֵ	���ݸ�ʽ02	kW	2

		i=MRR(ADDR_TYMDHMS+3,3);//��ǰRTC������
		x=MRR(ADDR_AFN04F44+(8*Pn),3);//��ͣ��ʼʱ��������
		if(i<x)
		{//��ǰRTC������<��ͣ��ʼʱ��������
			p8d[4]=1;//!=0ֵ��ʾͶ�뵫û��ԭ���1=û����ʼʱ��
			goto NoCtrl;//������
		}
		x=MRR(ADDR_AFN04F44+(8*Pn)+3,3);//��ͣ����ʱ��ʱ��������
		if(i>=x)
		{//��ǰRTC������>=��ͣ����ʱ��������
			p8d[4]=2;//!=0ֵ��ʾͶ�뵫û��ԭ���2=�ѵ�����ʱ��
			goto NoCtrl;//������
		}
		//�������ȼ�1=��ǰ�����¸���
		i=MRR(ADDR_AFN05F12+(9*Pn),1);//��ǰ�����¸���
		if(i!=0x0)
		{
			p8d[4]=3;//!=0ֵ��ʾͶ�뵫û��ԭ���3=�����ȼ����¸���
			goto NoCtrl;//������
		}
		
		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				break;
			case 3://����
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(ADDR_AFN04F44+(8*Pn)+6,2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						p8d[8]=1;//����ֵ��־=0
						p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
						if(p8[0]!=0x0)
						{//����
							break;
						}
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//����ֵ��־=0
					}
				}
				break;
			case 100://����
				//��������բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i==0x0)
				{//������,���ٸ澯
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(ADDR_AFN04F44+(8*Pn)+6,2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x==0x0)
					{//û����
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//������ʱ��
					p8d[0]=101;
				}
				break;
			case 101://��բ׼��
				//��բʱ����
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//д�Ĵ������ݵ�������,Byte<=8
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//�¼���¼
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//��բ2�ֺ���
					ERC6_Event(Pn+1,p8d[9],4,MRR(((u32)p8d)+2,2),i,MRR(ADDR_AFN04F44+(8*Pn)+6,2));//������բ��¼;���Fn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
					p8d[0]=103;
				}
				break;
			case 103://��բ2���Ӻ�
				p8d[0]=3;
				//���ػ���ʱ��
				i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
				if(i>=120)
				{
					i-=120;//��բ�¼���¼����ʱ2����
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl3(void)//�ն˿���-����.���ݹ���
{
	u32 i;
	u32 x;
	u32 Pn;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F10);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0ֵ��ʾͶ�뵫û��ԭ���1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			p8d[8]=0;//����ֵ��־=0
			goto NextPn;
		}

//		p8=(u8 *)(Terminal_Period);//�ն˷���ʱ�κ͹���ʱ��
//		if(p8[3]==0x0)
//		{//0=��ǰ����ʱ�κŲ�����,1=��ǰ����ʱ�κſ���
//			p8d[4]=1;//1=ʱ��?������
//			goto NoCtrl;//������
//		}


//#define ADDR_AFN04F42     ADDR_AFN04F41+((1+(17*3))*MaxTotalAddGroup)//���ݹ��ز���(�ܼ����)
//���ݿض�ֵ	����¼A.2	kW	2
//�޵���ʼʱ��	����¼A.19	��ʱ	2
//�޵�����ʱ�䣨��Χ1��48��	BIN	0.5h	1
//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0	BS8		1

		i=MRR(ADDR_AFN04F42+(6*Pn)+5,1);//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0	BS8		1
		p8=(u8 *)(ADDR_TYMDHMS+6);
		x=p8[0];//ȡ��ǰRTC����
		if(x==0x0)
		{
			x=7;//������=7
		}
		x=1<<x;
		i&=x;
		if(i==0x0)
		{//��ǰ�ܲ��ǳ���
			p8d[4]=2;//2=���ǳ�����
			goto NoCtrl;//������
		}
		i=MRR(ADDR_TYMDHMS+1,2);//��ǰRTC��ʱ
		x=MRR(ADDR_AFN04F42+(6*Pn)+2,2);//�޵���ʼʱ���ʱ
		if(i<x)
		{//��ǰRTC��ʱ<�޵���ʼʱ���ʱ
			p8d[4]=3;//3=�����޵�ʱ��
			goto NoCtrl;//������
		}
		//�޵����ʱ���ʱ����
		i=x&0xff;
		x=x>>8;
		x=bcd_hex(x);
		x*=60;
		i=bcd_hex(i);
		i+=x;
		x=MRR(ADDR_AFN04F42+(6*Pn)+4,1);//�޵�����ʱ��0.5h
		x*=30;
		i+=x;
		x=i/60;
		i%=60;
		x=hex_bcd(x);
		x<<=8;
		i=hex_bcd(i);
		x|=i;
		i=MRR(ADDR_TYMDHMS+1,2);//��ǰRTC��ʱ
		if(i>=x)
		{//��ǰRTC��ʱ>=�޵����ʱ���ʱ
			p8d[4]=3;//3=�����޵�ʱ��
			goto NoCtrl;//������
		}
		
		//�������ȼ�1=��ǰ�����¸���
		i=MRR(ADDR_AFN05F12+(9*Pn),1);//��ǰ�����¸���
		if(i!=0x0)
		{
			p8d[4]=4;//4=�����ȼ����¸���
			goto NoCtrl;//������
		}
		//�������ȼ�2=Ӫҵ��ͣ����
		i=MRR(ADDR_AFN05F11+Pn,1);//Ӫҵ��ͣ����
		if(i!=0x0)
		{
			p8d[4]=5;//5=�����ȼ��߱�ͣ��
			goto NoCtrl;//������
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				break;
			case 3://����
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(ADDR_AFN04F42+(6*Pn),2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						p8d[8]=1;//����ֵ��־=0
						p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
						if(p8[0]!=0x0)
						{//����
							break;
						}
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//����ֵ��־=0
					}
				}
				break;
			case 100://����
				//��������բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i==0x0)
				{//������,���ٸ澯
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(ADDR_AFN04F42+(6*Pn),2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x==0x0)
					{//û����
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//������ʱ��
					p8d[0]=101;
				}
				break;
			case 101://��բ׼��
				//��բʱ����
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//д�Ĵ������ݵ�������,Byte<=8
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//�¼���¼
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//��բ2�ֺ���
					ERC6_Event(Pn+1,p8d[9],2,MRR(((u32)p8d)+2,2),i,MRR(ADDR_AFN04F42+(6*Pn),2));//������բ��¼;���Fn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
					p8d[0]=103;
				}
				break;
			case 103://��բ2���Ӻ�
				p8d[0]=3;
				//���ػ���ʱ��
				i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
				if(i>=120)
				{
					i-=120;//��բ�¼���¼����ʱ2����
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=1;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

void Terminal_Contrl4(void)//�ն˿���-����.ʱ�ι���
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8s=(u8 *)(ADDR_AFN05F9);
	p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[4]=0;//!=0ֵ��ʾͶ�뵫û��ԭ���1=...
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		i=MRR(((u32)p8s),1);
		if(i!=0x55)
		{//���
NoCtrl://������
			p8d[0]=0;//����״̬=0
			p8d[1]=0;//��բ�ִ�=0
			p8d[8]=0;//����ֵ��־=0
			goto NextPn;
		}

//ʱ�ι���Ͷ���־��D0��D7��˳���λ��ʾ��1����8ʱ�Σ���"1"����Ч����"0"����Ч
//ʱ�ι��ض�ֵ�����ţ���ֵ��Χ��0��2���α�ʾ��1����3�׷���������ֵ��Ч

		p8=(u8 *)(ADDR_Terminal_Period);//�ն˷���ʱ�κ͹���ʱ��
		if(p8[3]==0x0)
		{//0=��ǰ����ʱ�κŲ�����,1��2��ǰ����ʱ�κſ���
			p8d[4]=1;//û��ԭ���2=��ǰʱ�β�����
			goto NoCtrl;//������
		}
		x=p8[2];//��ǰ����ʱ�κ�1-8,=0��ʾʱ�κ�>9����
		if(x==0x0)
		{
			p8d[4]=2;//û��ԭ���3=����ʱ������8
			goto NoCtrl;//������
		}
		x--;
		i=MRR(((u32)p8s)+1,1);
		i>>=x;
		i&=0x1;
		if(i==0x0)
		{//��ǰʱ��ûͶ��
			p8d[4]=3;//û��ԭ���4=��ǰʱ��ûͶ��
			goto NoCtrl;//������
		}
		i=MRR(((u32)p8s)+2,1);//ʱ�ι��ض�ֵ������
		if(i>2)
		{//0��2���α�ʾ��1����3�׷���������ֵ��Ч
			p8d[4]=4;//û��ԭ���5=��Ч������
			goto NoCtrl;//������
		}
		p8d[5]=i;
		p8=(u8 *)(ADDR_AFN04F41+((1+(17*3))*Pn));//ʱ�ι��ض�ֵ(�ܼ����)(����ʱ3��������ʱ�ι̶��ռ����)
		if((MRR(((u32)p8),1)&(1<<i))==0x0)
		{//�޷�����־
			p8d[4]=5;//û��ԭ���6=�ޱ�������ֵ
			goto NoCtrl;//������
		}
		p8+=1+(17*i);
		if((MRR(((u32)p8),1)&(1<<x))==0x0)
		{//��ʱ�ι��ʶ�ֵ
			p8d[4]=6;//û��ԭ���7=������ʱ���޶�ֵ
			goto NoCtrl;//������
		}
		p8+=1+(2*x);
		i=MRR((u32)p8,2);//ȡ��ǰʱ�ι��ʶ�ֵ
		//�ն�ʱ�ι��ض�ֵ����ϵ��
		a64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		if((a64>>63)!=0x0)
		{//��
			a64=(~a64)+1;
		}
		p8=(u8 *)(ADDR_AFN04F19);//�ն�ʱ�ι��ض�ֵ����ϵ��
		i=MRR(((u32)p8),1);//��ǰ�����¸��ض�ֵ����ϵ��(��ֵ����ϵ���� S0	BCD��ʮλ��0-7��	BCD���λ;S0���壺S0=0����ʾ�ϸ���S0=1����ʾ�¸�)
		i&=0x7f;
		i=bcd_hex(i);
		b64=(a64*i)/100;
		if((MRR(((u32)p8),1)>>7)==0x0)
		{//S0=0����ʾ�ϸ�
			a64+=b64;
		}
		else
		{//S0=1����ʾ�¸�
			a64-=b64;
		}
		i=kW_Data02(a64);//��λͳһΪkw��3λС����hex����תΪ���ݸ�ʽ02
		MWR(i,((u32)p8d)+6,2);//д�Ĵ������ݵ�������,Byte<=8
		
		//�������ȼ�1=��ǰ�����¸���
		p8=(u8 *)(ADDR_AFN05F12+(9*Pn));//��ǰ�����¸���
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=7;//û��ԭ���1=�����ȼ����¸���
			goto NoCtrl;//������
		}
		//�������ȼ�2=Ӫҵ��ͣ����
		p8=(u8 *)(ADDR_AFN05F11+Pn);//Ӫҵ��ͣ����
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=8;//û��ԭ���1=�����ȼ��߱�ͣ��
			goto NoCtrl;//������
		}
		//�������ȼ�3=���ݹ���
		p8=(u8 *)(ADDR_AFN05F10+Pn);//���ݹ���
		if(MRR(((u32)p8),1)!=0x0)
		{
			p8d[4]=9;//û��ԭ���1=�����ȼ��߳��ݿ�
			goto NoCtrl;//������
		}

		p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
		if(p8[0]!=0x0)
		{//����
			if(p8d[0]>3)
			{
				p8d[0]=3;
			}
			p8d[1]=0;
		}
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				break;
			case 3://����
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(((u32)p8d)+6,2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						p8d[8]=1;//����ֵ��־=0
						p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
						if(p8[0]!=0x0)
						{//����
							break;
						}
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR((ADDR_AFN04F49+i),1);
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
					}
					else
					{
						p8d[8]=0;//����ֵ��־=0
					}
				}
				break;
			case 100://����
				//��������բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i==0x0)
				{//������,���ٸ澯
					p8d[0]=3;
					break;
				}
				if(p16timer[Pn]==0x0)
				{//����ʱ�䵽
					x=MRR(((u32)p8d)+6,2);//ȡ��ֵ
					x=kWContrl_Comp(x,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x==0x0)
					{//û����
						p8d[0]=3;
						break;
					}
				}
				p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
				if(p16timer[Pn]==0x0)
				{//������ʱ��
					p8d[0]=101;
				}
				break;
			case 101://��բ׼��
				//��բʱ����
				i=MRR(ADDR_AFN0CF17+(Pn*2),2);
				MWR(i,((u32)p8d)+2,2);//д�Ĵ������ݵ�������,Byte<=8
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[9]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//�¼���¼
					i=MRR(ADDR_AFN0CF17+(Pn*2),2);//��բ2�ֺ���
					ERC6_Event(Pn+1,p8d[9],1,MRR(((u32)p8d)+2,2),i,MRR(((u32)p8d)+6,2));//������բ��¼;���Fn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
					p8d[0]=103;
				}
				break;
			case 103://��բ2���Ӻ�
				p8d[0]=3;
				//���ػ���ʱ��
				i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
				if(i>=120)
				{
					i-=120;//��բ�¼���¼����ʱ2����
				}
				else
				{
					i=0;
				}
				p16timer[Pn]=i;
				break;
			default:
				goto NoCtrl;//������
		}
NextPn:
		p8s+=3;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}

#endif//#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��




