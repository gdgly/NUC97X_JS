

//����
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"




#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

u32 NextTrip(u32 Bin,u32 Pn)//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
{
	u32 i;
	
	i=MRR(ADDR_230x_14+(LENper_230x_14*Pn)+2,1);//�����ִ��趨(�ܼ����)
	i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
	Bin^=i;
	Bin&=i;
	Bin=BitLoca(Bin);//�����1����λΪ1��λ��(0-32)
	if(Bin!=0x0)
	{
		if(Bin>NUMmax_CR)
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
	i=MRR(ADDR_230x_13+(LENper_230x_13*Pn)+1,1);//���ʿ��ƵĹ��ʼ��㻬��ʱ��(��Χ��1��60)	BIN	min	1
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

u32 kWContrl_Comp(u64 FixedValue,u32 Pn)//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
{
	u32 f;
	u32 i;
	u64 a64;
	u64 b64;
	u16 * p16timer;
	
	p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
	f=0;//���ޱ�־=0
	i=kWContrlSlipTime(Pn);//ȡ���ʿ��ƵĹ��ʼ��㻬��ʱ����(��Χ��1��60��*60=��),��ڣ��ܼ���Ŵ�0��ʼ
	i=0;//����ʱ�䲻��,�������빦�ʼ���̶�Ϊ4��15��(1��)
	p16timer[Pn]=i;//�´μ���ʱ��
	//��ǰ����
	a64=MRR(ADDR_230x_3+(LENper_230x_3*Pn)+1,8);//�ܼ��й�����
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if((a64>>63)!=0x0)
	{//��
		a64=(~a64)+1;
	}
	//������ֵ
	b64=MRR(ADDR_8100_2+1,8);//�ն˱�����ֵ��=long64����λ��W�����㣺-1��
	b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if((b64>>63)!=0x0)
	{//��
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//��ǰ����>=������ֵ
		if((FixedValue>>63)!=0x0)
		{//��
			FixedValue=(~FixedValue)+1;
		}
		if(a64>=FixedValue)
		{
			f=1;//����
		}
	}
	return f;
}

u32 kWContrl1_Comp(u32 Pn)//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
{
	u32 f;
	u32 i;
	u64 a64;
	u64 b64;
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
	i=0;//����ʱ�䲻��,�������빦�ʼ���̶�Ϊ4��15��(1��)
	p16timer[Pn]=i;//�´μ���ʱ��
	//��ǰ����
	a64=MRR(ADDR_230x_3+(LENper_230x_3*Pn)+1,8);//�ܼ��й�����
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if((a64>>63)!=0x0)
	{//��
		a64=(~a64)+1;
	}
	//������ֵ
	b64=MRR(ADDR_8100_2+1,8);//�ն˱�����ֵ��=long64����λ��W�����㣺-1��
	b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if((b64>>63)!=0x0)
	{//��
		b64=(~b64)+1;
	}
	if(a64>=b64)
	{//��ǰ����>=������ֵ
		b64=MRR(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);//ȡ���ʶ�ֵ
		b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
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
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
	u16* p16;
	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8106_3+LENmax_8106_127)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);//��ǰ�����¸���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8106_3,ADDR_8106_127,LENmax_8106_127);//��ǰ�����¸���-����127 Ͷ�루�ܼ�����󣬿��Ʒ�����(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//array
//structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//��ǰ�����¸��ض�ֵ����ʱ��    unsigned����λ�����ӣ���
//��ǰ�����¸��ض�ֵ����ϵ��    integer����λ��%����
//�غ��ܼ��й����ʶ�����ʱʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��صĿ���ʱ��      unsigned����λ��0.5Сʱ����
//��ǰ�����¸��ص�1�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�2�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�3�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�4�ָ澯ʱ��  unsigned����λ�����ӣ�
//}
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
		if(p8[1]!=0x0)
		{//����
			break;
		}
		if(p8c[6]!=1)
		{//ûͶ��
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//�ܼ�������=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//����ӦOI�Ŀ��Ʒ���
		p8s=(u8*)ADDR_DATABUFF+LENmax_8106_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
		}
		if(j>=x)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		//����ʱ��
		i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);
		if(i==0x0)
		{//����ʱ�䶨ʱ��
			p8c+=7;
			continue;
		}
		//����
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p16=(u16 *)(&Terminal_Ram->PowerCtrl1_Congeal_S_Timer);//25 �ն��ܼ���1��ǰ�����¸��ؿغ󶳽��붨ʱ��
				i=p8s[12];//�غ��ܼ��й����ʶ�����ʱʱ��,����
				i*=60;
				p16[Pn]=i;
				i=p8s[8];//��ǰ�����¸��ض�ֵ����ʱ�� unsigned����λ�����ӣ�;376.1�����ݷ�Χ��1��60
				if(i==0)
				{
					i=1;
				}
				if(i>60)
				{
					i=60;
				}
				i*=60;
				i=60;//����ʱ�䲻��,����4*15=60�������������ʼ�������
				p16timer[Pn]=i;
				MC(0,ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//��غ��ܼ��й����ʶ���ֵ
				p8d[0]=1;
				break;
			case 1://��ֵ����ʱ����ƽ�����ʼ���Ͷ�ֵ����
				if(p16timer[Pn]==0x0)
				{//����ʱ����ʱ��
					a64=MRR(ADDR_230x_3+(Pn*LENper_230x_3)+1,8);//�ܼ��й�����
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					if((a64>>63)!=0x0)
					{//��
						a64=(~a64)+1;
					}
					i=p8s[10];//��ǰ�����¸��ض�ֵ����ϵ��    integer����λ��%��
					p8d[3]=i;
					
					if((i&0x80)==0)
					{//S0=0����ʾ�ϸ�
						b64=(a64*i)/100;
						a64+=b64;
					}
					else
					{//S0=1����ʾ�¸�
						i=~i;
						i++;
						i&=0xff;
						b64=(a64*i)/100;
						if(a64>=b64)
						{
							a64-=b64;
						}
						else
						{
							a64=0;
						}
					}
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					MWR(a64,ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//�غ��ܼ��й����ʶ���
				}
				if(p16timer[Pn]==0x0)
				{//���ʿ��ƵĹ��ʼ��㻬��ʱ��(�ܼ����)����ʱ����ʱ��		
					x=kWContrl1_Comp(Pn);//�ն˿���-����.��ǰ�����¸��ع��ʱȽ�,���Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
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
							i=p8s[15+(2*i)+1];//��?�ָ澯ʱ��,��
							i*=60;
							p16timer[Pn]=i;
							p8d[0]=100;
						}
					}
					else
					{//û����
						
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//�غ��ܼ��й����ʶ���
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
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
					MW(ADDR_230x_3+(Pn*LENper_230x_3),ADDR_230x_12+(Pn*LENper_230x_12),LENper_230x_12);//�غ��ܼ��й����ʶ���
				}
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//��բ��2�����ܼ��й�����long64(��λ��W������-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
					//�¼���¼
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=���ض�ֵ      long64����λ��kW������-4��
	//pFirst=��բ����ǰ�ܼ��й�����    long64����λ��kW������-4��
	//pAfter=�¼�������2���ӹ���  long64(��λ��W������-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32000200(0x8106,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);//��ǰ�����¸���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8106_3,ADDR_8106_127,LENmax_8106_127);//��ǰ�����¸���-����127 Ͷ�루�ܼ�����󣬿��Ʒ�����(��ʼ��ֵ0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//��û�����ڿ��Ƶ��ܼ���
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

void Terminal_Contrl2(void)//�ն˿���-����.Ӫҵ��ͣ����
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
//	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8105_3+LENmax_8105_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8105_3,ADDR_8105_2,LENmax_8105_2);//���õ�Ԫ(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//����2�����Ʒ���������=array Ӫҵ��ͣ�����õ�Ԫ
//����2��=structure
//{
//�ܼ������      OI��
//��ͣ��ʼʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ����ʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ�ع��ʶ�ֵ  long64����λ��W�����㣺-1��
//}
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
		if(p8[1]!=0x0)
		{//����
			break;
		}
		if(p8c[6]!=1)
		{//ûͶ��
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//�ܼ�������=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//���ȼ�
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//����ӦOI�Ŀ��Ʒ���
		p8s=(u8*)ADDR_DATABUFF+LENmax_8105_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
		}
		if(j>=x)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		//Ӫҵ��ͣʱ��
		i=MRR(ADDR_DL698YMDHMS,4);//��ǰ������
		i=R_Inverse(i,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		x=MRR((u32)p8s+6,4);//��ͣ��ʼʱ��
		x=R_Inverse(x,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		if(i<x)
		{
			p8c+=7;
			continue;
		}
		x=MRR((u32)p8s+14,4);//��ͣ����ʱ��
		x=R_Inverse(x,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		if(i>=x)
		{
			p8c+=7;
			continue;
		}
		//����
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				MR((u32)p8d+8,(u32)p8s+22,8);//��ǰ��ֵ
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				p8d[0]=3;
				break;
			case 3://����
				if(p16timer[Pn]==0)//���ع��ʼ����붨ʱ��=0
				{
					MR((u32)p8d+8,(u32)p8s+22,8);//��ǰ��ֵ
					a64=MRR((u32)p8s+22,8);//��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//���ظ澯ʱ��
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8s+22,8);//��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//��բ��2�����ܼ��й�����long64(��λ��W������-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
					//�¼���¼
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=���ض�ֵ      long64����λ��kW������-4��
	//pFirst=��բ����ǰ�ܼ��й�����    long64����λ��kW������-4��
	//pAfter=�¼�������2���ӹ���  long64(��λ��W������-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32000200(0x8105,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8105_3,ADDR_8105_2,LENmax_8105_2);//���õ�Ԫ(��ʼ��ֵ0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//��û�����ڿ��Ƶ��ܼ���
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}

void Terminal_Contrl3(void)//�ն˿���-����.���ݹ���
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
//	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8104_3+LENmax_8104_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8104_3,ADDR_8104_2,LENmax_8104_2);//���õ�Ԫ(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//����2�����Ʒ���������=array ���ݿ����õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//�ܼ������    OI��
//���ݿض�ֵ    long64����λ��W�����㣺-1����
//�޵���ʼʱ��  date_time_s����=FFFFH����=FFH����=FFH����
//�޵�����ʱ��  long-unsigned����λ�����ӣ���
//ÿ���޵���    bit-string(SIZE(8)) 
//}
//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0��
	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
		if(p8[1]!=0x0)
		{//����
			break;
		}
		if(p8c[6]!=1)
		{//ûͶ��
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//�ܼ�������=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//���ȼ�
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//����ӦOI�Ŀ��Ʒ���
		p8s=(u8*)ADDR_DATABUFF+LENmax_8104_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
		}
		if(j>=x)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		//ÿ���޵���    bit-string(SIZE(8)) 
		i=p8s[27];//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0��
		i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
		x=Comm_Ram->MYMDHMS[6];//ȡ��ǰRTC����
		if(x==0x0)
		{
			x=7;//������=7
		}
		x=1<<x;
		i&=x;
		if(i==0x0)
		{//��ǰ�ܲ��ǳ���
			p8c+=7;
			continue;
		}
		//�޵���ʼʱ��
		i=(Comm_Ram->DL698YMDHMS[4]<<8)|Comm_Ram->DL698YMDHMS[5];//��ǰRTCʱ��
		x=(p8s[19]<<8)|p8s[20];//�޵���ʼʱ��ʱ��
		if(i<x)
		{//��ǰRTC��ʱ<�޵���ʼʱ���ʱ
			p8c+=7;
			continue;
		}
		//�޵����ʱ��ʱ�ּ���
		j=(p8s[23]<<8)|p8s[24];
		j+=x&0xff;
		x>>=8;
		x+=j/60;
		x<<=8;
		x|=j%60;
		if(i>=x)
		{//��ǰRTC��ʱ>=�޵����ʱ���ʱ
			p8c+=7;
			continue;
		}
		//����
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				MR((u32)p8d+8,(u32)p8s+6,8);//��ǰ��ֵ
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				p8d[0]=3;
				break;
			case 3://����
				if(p16timer[Pn]==0)//���ع��ʼ����붨ʱ��=0
				{
					MR((u32)p8d+8,(u32)p8s+6,8);//��ǰ��ֵ
					a64=MRR((u32)p8s+6,8);//��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//���ظ澯ʱ��
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8s+6,8);//��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//��բ��2�����ܼ��й�����long64(��λ��W������-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
					//�¼���¼
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=���ض�ֵ      long64����λ��kW������-4��
	//pFirst=��բ����ǰ�ܼ��й�����    long64����λ��kW������-4��
	//pAfter=�¼�������2���ӹ���  long64(��λ��W������-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32000200(0x8104,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8104_3,ADDR_8104_2,LENmax_8104_2);//���õ�Ԫ(��ʼ��ֵ0x00)
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//��û�����ڿ��Ƶ��ܼ���
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}

void Terminal_Contrl4(void)//�ն˿���-����.ʱ�ι���
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8p;
	u8* p8d;
//	u16* p16;
	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8103_3+LENmax_8103_2+LENmax_8103_127)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8103_3,ADDR_8103_2,LENmax_8103_2);//���õ�Ԫ(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2,ADDR_8103_127,LENmax_8103_127);
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//����2�����Ʒ���������=array ʱ�ι������õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure//
//{
//�ܼ������  OI��
//������ʶ    bit-string(SIZE(8))��
//��һ�׶�ֵ  PowerCtrlParam��
//�ڶ��׶�ֵ  PowerCtrlParam��
//�����׶�ֵ  PowerCtrlParam��
//ʱ�ι��ض�ֵ����ϵ��  integer����λ��%��
//}
//PowerCtrlParam��=structure
//{
//	ʱ�κ�          bit-string(SIZE(8))��
//	ʱ��1���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��2���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��3���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��4���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��5���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��6���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��7���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��8���ض�ֵ  long64����λ��W�����㣺-1��
//}

//ʱ�ι��ط����л���=structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//ʱ�ι���Ͷ���ʶ    bit-string(SIZE(8))��
//ʱ�ι��ض�ֵ������  unsigned
//}
//ʱ�ι���Ͷ���ʶ��D0��D7��˳���λ��ʾ��1����8ʱ�Σ��á�1������Ч���á�0������Ч��
//ʱ�ι��ض�ֵ�����ţ���ֵ��Χ��0��2���α�ʾ��1����3�׷���������ֵ��Ч��

	p8c=(u8*)ADDR_DATABUFF;
	n=p8c[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8c+=2;
	while(n--)
	{
		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
		if(p8[1]!=0x0)
		{//����
			break;
		}
		if(p8c[6]!=1)
		{//ûͶ��
			p8c+=7;
			continue;
		}
		OI=(p8c[3]<<8)+p8c[4];//�ܼ�������=OI
		if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
		{
			p8c+=7;
			continue;
		}
		Pn=OI&0xff;
		Pn--;
		//���ȼ�
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len));
		if(p8[0]!=0)
		{
			p8c+=7;
			continue;
		}
		
		//����ӦOI�Ŀ��Ʒ���
		p8s=(u8*)ADDR_DATABUFF+LENmax_8103_3;
		x=p8s[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8s+=2;
		for(j=0;j<x;j++)
		{
			i=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
			if(i==OI)
			{
				break;
			}
			p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
		}
		if(j>=x)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		//����ӦOI�ķ����л�
		p8p=(u8*)ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2;
		x=p8p[1];
		if(x>NUMmax_TAG)
		{
			x=NUMmax_TAG;
		}
		p8p+=2;
		for(j=0;j<x;j++)
		{
			i=(p8p[3]<<8)+p8p[4];//�ܼ�������=OI
			if(i==OI)
			{
				break;
			}
			p8p+=Get_DL698DataLen_S(p8p,0);//�õ�DL698�������ͳ���
		}
		if(j>=x)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		//
		p8=(u8 *)(ADDR_Terminal_Period);//�ն˷���ʱ�κ͹���ʱ��
		if(p8[3]==0x0)
		{//0=��ǰ����ʱ�κŲ�����,1��2��ǰ����ʱ�κſ���
			p8c+=7;
			continue;
		}
		x=p8[2];//��ǰ����ʱ�κ�1-8,=0��ʾʱ�κ�>9����
		if(x==0x0)
		{
			p8c+=7;
			continue;
		}
		x--;
		i=p8p[9];
//	#if (USER/100)==15//����
		i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
//	#endif	
		i>>=x;
		i&=0x1;
		if(i==0x0)
		{//��ǰʱ��ûͶ��
			p8c+=7;
			continue;
		}
		i=p8p[11];//ʱ�ι��ض�ֵ������
		if(i>2)
		{//0��2���α�ʾ��1����3�׷���������ֵ��Ч
			p8c+=7;
			continue;
		}
		j=p8s[7];//������ʶ    bit-string(SIZE(8))��
		j=Bit_Inverse(j,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
		if((j&(1<<i))==0)
		{//�޷�����ʶ
			p8c+=7;
			continue;
		}
		p8=Get_Element(p8s,3+i,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8==0)
		{
			p8c+=7;
			continue;
		}
		j=p8[4];
		j=Bit_Inverse(j,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
		if((j&(1<<x))==0)
		{//��ʱ�ι��ʶ�ֵ
			p8c+=7;
			continue;
		}
		p8+=2+3+(9*x);
		//ȡ��ǰʱ�ι��ʶ�ֵ
		a64=MRR((u32)p8+1,8);//ȡ��ǰʱ�ι��ʶ�ֵ
		a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		if((a64>>63)!=0x0)
		{//��
			a64=(~a64)+1;
		}
		//�ն�ʱ�ι��ض�ֵ����ϵ��
		i=p8s[1];//�ṹԪ�ظ���
		p8=Get_Element(p8s,i,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8==0)
		{//û�ҵ�
			p8c+=7;
			continue;
		}
		
		i=p8[1];//ʱ�ι��ض�ֵ����ϵ��  integer����λ��%��
		if((i>>7)==0x0)
		{//S0=0����ʾ�ϸ�
			b64=(a64*i)/100;
			a64+=b64;
		}
		else
		{//S0=1����ʾ�¸�
			i=~i;
			i++;
			i&=0xff;
			b64=(a64*i)/100;
			if(a64>=b64)
			{
				a64-=b64;
			}
			else
			{
				a64=0;
			}
		}
		a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		
		//����
		p16timer=(u16 *)(&Terminal_Ram->PowerCtrl1_S_Timer);
		p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len));
		MWR(a64,(u32)p8d+8,8);//�浱ǰ��ֵ
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p16timer[Pn]=0;//���ع��ʼ����붨ʱ��=0
				p8d[0]=3;
				break;
			case 3://����
				if(p16timer[Pn]==0)//���ع��ʼ����붨ʱ��=0
				{
					a64=MRR((u32)p8d+8,8);//ȡ��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
					if(x!=0x0)
					{//����
						//����բ�ִμ���
						i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
						if(i==0x0)
						{//������,���ٸ澯
						}
						else
						{
							i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
							i--;
							i=MRR(ADDR_8102_2+2+(2*i)+1,1);//���ظ澯ʱ��
							i*=60;
							p16timer=(u16 *)(&Terminal_Ram->PowerAttr1_S_Timer);
							p16timer[Pn]=i;//��?�ָ澯ʱ��
							p8d[0]=100;
						}
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
					a64=MRR((u32)p8d+8,8);//ȡ��ǰ��ֵ
					a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					x=kWContrl_Comp(a64,Pn);//�ն˿���-���ع��ʱȽ�,���FixedValue=��ֵ,Pn=�ܼ����(0-7);����0û����,1����
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
				MR((u32)p8d+16,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
				//����բ�ִμ���
				i=NextTrip(p8d[1],Pn);//����բ�ִμ���,���Bin=����λ��־,Pn=�ܼ����(0-7),����0=�����ִ�,!=0���ִε�λ��־
				if(i!=0x0)
				{
					p8d[1]|=i;
					p8d[2]=i;//��ǰ������բ�ִ�B0-B7
					//2���Ӷ�ʱ
					p16timer[Pn]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(2);//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
				}
				else
				{//�ִ���
					p8d[0]=3;
				}
				break;
			case 102://��բ
				if(p16timer[Pn]==0x0)
				{//2����ʱ�䵽
					//��բ��2�����ܼ��й�����long64(��λ��W������-1)
					MR((u32)p8d+24,ADDR_230x_3+1+(9*Pn),8);//�ܼ���Pn
					//�¼���¼
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=���ض�ֵ      long64����λ��kW������-4��
	//pFirst=��բ����ǰ�ܼ��й�����    long64����λ��kW������-4��
	//pAfter=�¼�������2���ӹ���  long64(��λ��W������-1)
					i=p8d[1];
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32000200(0x8103,0x2300+(Pn+1),i,p8d+8,p8d+16,p8d+24);//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);//�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8103_3,ADDR_8103_2,LENmax_8103_2);//���õ�Ԫ(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8103_3+LENmax_8103_2,ADDR_8103_127,LENmax_8103_127);
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
				p8d[0]=0;
				p8d[1]=0;
				p8d[2]=0;
				break;
		}
		p8c+=7;
	}
//��û�����ڿ��Ƶ��ܼ���
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		if((TAG&(1<<Pn))==0)
		{
			p8d=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

#endif

