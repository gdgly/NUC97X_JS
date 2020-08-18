
//ң��
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"



#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void Terminal_Remote_Control(void)//�ն˿���-ң����բ
{
	u32 i;
	u32 x;
	u32 n;
	u32 OAD;
	u8 * p8;
	u8 *p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	if(p8[1]!=0x0)
	{//����
		for(i=0;i<NUMmax_CR;i++)
		{
			p8d[0]=0;//����״̬=0
			p8d+=Terminal_Ctrl_Pn_Len;
		}
		return;
	}
	MR(ADDR_128KDATABUFF+(64*1024),ADDR_8000_129,LENmax_8000_129);//ң��-����129����բ��������
	//MR(ADDR_128KDATABUFF+(64*1024)+1024,ADDR_8000_130 ,LENmax_8000_130);//ң��-����130����բ��������
//#define ADDR_8000_5     ADDR_8000_4+LENmax_8000_4//ң��-����4����5������״̬��ֻ��)
//����5������״̬��ֻ��)��=bit-string(SIZE(8))
//�̵�������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ�������״̬���á�1������բ����á�0������բ���
//#define LENmax_8000_5     3
	p8=(u8*)ADDR_8000_5;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	p8s=(u8 *)(ADDR_128KDATABUFF+(64*1024));
	n=p8s[1];
	if(n>NUMmax_CR)
	{
		n=NUMmax_CR;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OAD=(p8s[3]<<24)|(p8s[4]<<16)|(p8s[5]<<8)|p8s[6];//�̵���OAD
		if(OAD!=0)
		{//��բ����
			x|=1<<(7-i);
		}
		p8s+=LENper_8000_129;
	}
	p8[2]=x;
//
	p16timer=(u16*)((u32)(&Terminal_Ram->Remote1_S_Timer));
	for(i=0;i<NUMmax_CR;i++)
	{
		p8s=(u8 *)(ADDR_128KDATABUFF+(64*1024));
		n=p8s[1];
		if(n>NUMmax_CR)
		{
			n=NUMmax_CR;
		}
		p8s+=2;
		for(x=0;x<n;x++)
		{
			OAD=(p8s[3]<<24)|(p8s[4]<<16)|(p8s[5]<<8)|p8s[6];//�̵���OAD
			if((OAD&0xff)==(i+1))
			{
				break;
			}
			p8s+=LENper_8000_129;
		}
		if(x<n)
		{//�ҵ���Ӧ�̵���OAD
			x=(p8s[10]<<8)|p8s[11];//�޵�ʱ��    long-unsigned����λ�����ӣ����㣺0��ֵΪ0��ʾ�����޵磩
			if(x!=0x0)
			{//���޵�ʱ��
				x=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(i*2),2);//�����������ݵ��Ĵ���,Byte<=8
				if(x==0x0)
				{//����ʱ�䶨ʱ��
					p8d[0]=0;
					p8d+=Terminal_Ctrl_Pn_Len;
					continue;
				}
			}
			switch(p8d[0])
			{
				case 0://�޿��ƻ��ϵ�
					p8d[0]=1;
					break;
				case 1://Ͷ��
					x=p8s[8];//�澯��ʱunsigned����λ�����ӣ����㣺0��
					x*=60;
					if(x==0x0)
					{//������բ
						p8d[0]=101;
					}
					else
					{//�澯��ʱ
						p16timer[i]=x;
						p8d[0]=100;
					}
					break;
				case 100://����
					if(p16timer[i]==0x0)
					{//������ʱ��
						p8d[0]=101;
					}
					break;
				case 101://��բ׼��
					//��բʱ����
					MR((u32)p8d+8,ADDR_230x_3+1,8);//�ܼ���1
					//2���Ӷ�ʱ
					p16timer[i]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(3);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
					break;
				case 102://��բ
					if(p16timer[i]==0x0)
					{//2����ʱ�䵽
						//��բ��2�����ܼ��й�����long64(��λ��W������-1)
						MR((u32)p8d+16,ADDR_230x_3+1,8);//�ܼ���1
						//�¼���¼
						Event_31150200(p8s+2,1);//ң���¼���¼(�غ�2����),���:pOAD=�¼�����ԴOAD���̵�����Ԫ��,occur=0�ָ�=1����=2��	 
						p8d[0]=103;
					}
					break;
				case 103://��բ2���Ӻ�
					break;
				default:
					p8d[0]=0;//����״̬=0
					break;
			}
		}
		else
		{//û�ҵ���Ӧ�̵���OAD
			p8d[0]=0;
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}

//#define ADDR_8000_3     ADDR_230x_17+LENmax_230x_17//ң��-����3���̵������״̬��ֻ��)
//����3���̵������״̬��ֻ��)��=bit-string(SIZE(8))
//�̵������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ���״̬���á�1������բ״̬���á�0������բ״̬��
//#define LENmax_8000_3     3
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	p8=(u8*)ADDR_8000_3;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	for(i=0;i<NUMmax_CR;i++)
	{
		if(p8d[0]>=102)
		{//��բ
			x|=1<<(7-i);
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}
	p8[2]=x;
//#define ADDR_8000_4     ADDR_8000_3+LENmax_8000_3//ң��-����4(�澯״̬��ֻ��)
//����4(�澯״̬��ֻ��)��=bit-string(SIZE(8))
//�澯״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң�ظ澯���״̬���á�1�������ڸ澯״̬���á�0����δ���ڸ澯״̬��
//#define LENmax_8000_4     3
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	p8=(u8*)ADDR_8000_4;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	for(i=0;i<NUMmax_CR;i++)
	{
		if(p8d[0]==100)
		{//����
			x|=1<<(7-i);
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}
	p8[2]=x;
	
}

#endif






























