
//�ն˿��ƹ���
#include "Project.h"
#include "Terminal_Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_Contrl_Remote.h"
#include "Terminal_Contrl_Power.h"
#include "Terminal_Contrl_Energy.h"
#include "Terminal_Contrl_Out.h"







void Init_Contrl_Command(void)//�ն˿������ݳ�ʼ��(����,ȫ�����)
{
	MC(0,ADDR_GPRSTimerTaskNotes,6*MaxTimerTask*2);//GPRSͨ����ʱ�����ϱ���¼
	MC(0,ADDR_ENETTimerTaskNotes,6*MaxTimerTask*2);//ENETͨ����ʱ�����ϱ���¼
	MC(0,ADDR_RS232TimerTaskNote,6*MaxTimerTask*2);//RS232ͨ����ʱ�����ϱ���¼
}



void Terminal_Contrl_Protect(void)//�ն˿���-����
{
	u32 i;
	u32 x;
	u8 * p8;
	
	i=0;
	x=MRR(ADDR_AFN05F25,1);
	if(x==0x55)//�ն˱���Ͷ����
	{
		x=MRR(ADDR_AFN05F25+1,1);
		if(x!=0x0)
		{//��ʱ����
			i=x*30;//0.5h
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+0,2);//д�Ĵ������ݵ�������,Byte<=8
			MC(0x5A,ADDR_AFN05F25,1);//��������0
		}
		i=1;
	}
	else
	{
		if(x==0x5A)
		{//���뱣�綨ʱ
			if(MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+0,2)==0x0)
			{
				MC(0x0,ADDR_AFN05F25,1);//��������0
			}
			else
			{
				i=1;
			}
		}
	}
	if(i==0x0)
	{
		x=MRR(ADDR_AFN04F58,1);//�ն��Զ��������
		if(x!=0x0)
		{//�Զ�����
			if(Terminal_Ram->DisconAutoProtect_M_Timer==0x0)//�Զ�������������վ������ͨ��ʱ��ֶ�ʱ��
			{
				i=2;
			}
		}
	}
	p8=(u8 *)(ADDR_Terminal_Protect);
	p8[0]=i;
}

void Terminal_Time_Period(void)//�ն˷���ʱ�κ͹���ʱ��
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u8 * p8d;
	u8 * p8rtc;

//�ն˷���ʱ��
	p8rtc=(u8 *)(ADDR_TYMDHMS);
	i=p8rtc[2];
	i=bcd_hex(i);
	i*=2;
	if(p8rtc[1]>=0x30)
	{
		i++;
	}
	i=MRR(ADDR_AFN04F21+i,1);//ȡֵ0��47���α�ʾ����1������48������ֵ��Ч
	i++;
	if(i>TMaxTariff)
	{
		i=0;
	}
	p8d=(u8 *)(ADDR_Terminal_Period);//�ն˷���ʱ�κ͹���ʱ��
	p8d[0]=i;
//����ʱ��
	//��ǰRTC��ʱ�ְ�30�ּ���
	n=p8rtc[2];
	n=bcd_hex(n);
	n*=2;
	if(p8rtc[1]>=0x30)
	{
		n++;
	}
	n++;//0����1��30��
	x=MRR(ADDR_AFN04F18,1);//�ն˹���ʱ��
	x&=0x3;//ԭ���ƴ���
	y=1;//ʱ�κŴ�1��ʼ(�൱��Iֵ)
	for(i=0;i<n;i++)
	{
		z=MRR(ADDR_AFN04F18+(i/4),1);
		z>>=(i%4)*2;
		z&=0x3;
		if(z!=x)
		{//�����벻��ͬ
			x=z;
			y++;//ʱ�κ�+1
		}
	}
	if(y>8)
	{//ֻ��1-8,>9��Ϊ0
		y=0;
	}
	p8d[2]=y;
	if(p8d[3]!=x)
	{//���ƴ���仯
		p8d[3]=x;
		//����ʱ�α仯�澯
		BUZZ_Generate(3);//��������,��ڷ�����
		//��ԭʱ�ι���
		MC(0,ADDR_Terminal_kWCtrl4_State,Terminal_Ctrl_Pn_Len*MaxTotalAddGroup);
	}
}









void Terminal_Contrl_Manage(void)//�ն˿��ƹ���(����������)
{
	
	Terminal_Contrl_Protect();//�ն˿���-����
	switch(Terminal_Ram->I2SUBTask5)
	{
		case 0:
			Terminal_Time_Period();//�ն˷���ʱ�κ͹���ʱ��
			break;
		case 1:
			Terminal_Remote_Control();//�ն˿���-ң����բ
			break;
		case 2:
			Terminal_Contrl1();//�ն˿���-����.��ǰ�����¸���
			break;
		case 3:
			Terminal_Contrl2();//�ն˿���-����.Ӫҵ��ͣ����
			break;
		case 4:
			Terminal_Contrl3();//�ն˿���-����.���ݹ���
			break;
		case 5:
			Terminal_Contrl4();//�ն˿���-����.ʱ�ι���
			break;
		case 6:
			Terminal_Contrl_MonthEnergy();//�ն˿���-�µ��
			break;
		case 7:
			Terminal_Contrl_BuyEnergy();//�ն˿���-�����
			break;
		case 8:
			Terminal_Contrl_Out();//�ն˿���-���.�̵�����LED
			break;
	
		default:
			Terminal_Ram->I2SUBTask5=0;
			return;
	}
	Terminal_Ram->I2SUBTask5++;
}












