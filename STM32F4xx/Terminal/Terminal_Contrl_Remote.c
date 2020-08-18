
//�ն˿���-ң����բ
#include "Project.h"
#include "Terminal_Contrl_Remote.h"
#include "../device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"





#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void Terminal_Remote_Control(void)//�ն˿���-ң����բ
{
}
#else
void Terminal_Remote_Control(void)//�ն˿���-ң����բ
{
	u32 i;
	u32 Pn;
	u8 * p8;
	u8 *p8s;
	u8 * p8d;
	u16 * p16timer;
	
	
	p8=(u8 *)(ADDR_Terminal_Protect);//�ն˱���,0=������,1=�����,2=�Զ�����
	p8s=(u8 *)(ADDR_AFN05F1);
	p8d=(u8 *)(ADDR_Terminal_Remote_State);

	for(Pn=0;Pn<MaxControlRound;Pn++)
	{
		if(p8[0]!=0x0)
		{//����
NoCtrl://������
			p8d[0]=0;//����״̬=0
			goto NextPn;
		}
		if(MRR(((u32)p8s),1)==0x55)
		{//��Ͷ��
			i=MRR(((u32)p8s)+1,1)&0xf;//�޵�ʱ��0.5h
			i*=30;
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(Pn*2),2);//д�Ĵ������ݵ�������,Byte<=8
			MC(0x5a,ADDR_AFN05F1+(2*Pn),1);//0x55->0x5a
			p8d[0]=1;//����״̬=1
			goto NextPn;
		}
		if(MRR(((u32)p8s),1)!=0x5a)
		{//���
			goto NoCtrl;//������
		}
		if((MRR(((u32)p8s)+1,1)&0xf)!=0x0)
		{//���޵�ʱ��
			i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
			if(i==0x0)
			{//����ʱ�䶨ʱ��
				MC(0x0,ADDR_AFN05F1+(2*Pn),1);//��0x5a->0
				goto NoCtrl;//������
			}
		}
		p16timer=(u16 *)((u32)(&Terminal_Ram->Remote1_S_Timer));
		switch(p8d[0])
		{
			case 0://�޿��ƻ��ϵ�
				p8d[0]=1;
				break;
			case 1://Ͷ��
				i=(MRR(((u32)p8s)+1,1)>>4)&0xf;
				i*=60;
				if(i==0x0)
				{//������բ
					p8d[0]=101;
				}
				else
				{//�澯��ʱ
					p16timer[Pn]=i;
					p8d[0]=100;
				}
				break;
			case 100://����
				if(p16timer[Pn]==0x0)
				{//������ʱ��
					p8d[0]=101;
				}
				break;
			case 101://��բ׼��
				//��բʱ����
				i=MRR(ADDR_AFN0CF17,2);
				MWR(i,((u32)p8d)+2,2);//д�Ĵ������ݵ�������,Byte<=8
				//2���Ӷ�ʱ
				p16timer[Pn]=120;
				p8d[0]=102;
				Terminal_Ctrl_Count(3);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//�¼���¼
					i=MRR(ADDR_AFN0CF17,2);//��բ2�ֺ���
					ERC5_Event(Pn,MRR(((u32)p8d)+2,2),i);//ң����բ��¼;���n=��բ�ִ�(0-7),kw1=��բʱ����(�ܼӹ���),kw2=��բ��2���ӵĹ���(�ܼӹ���)
					p8d[0]=103;
				}
				break;
			case 103://��բ2���Ӻ�
				break;
			default:
				p8d[0]=0;//����״̬=0
				break;
		}
		
NextPn:
		p8s+=2;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}
#endif




