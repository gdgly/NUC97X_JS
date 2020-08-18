
//�ն�ͳ��2�� �����ⲿ����E1
#include "Project.h"
#include "Terminal_Statistic.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0E_Event.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"




u32 ReNewValue(u32 b,u32 c)//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
{
	u64 d64a;
	u64 d64b;
	b=bcd_hex(b);
#if (USER/100)==5//�Ϻ�
	//�Ϻ���������F26��Ļָ�ϵ����Ϊ�������Ϻ����Ļָ�ֵ��1-�ָ�ϵ��������
	c&=0x7fff;
	c=bcd_hex(c);
	d64a=b;
	d64b=c;
	d64a*=d64b;
	c=d64a/1000;
	b-=c;
	if((b>>31)!=0x0)
	{//���Ϊ��
		b=0;
	}
#else
	if((c>>15)==0x0)
	{//ϵ����
		c&=0x7fff;
		c=bcd_hex(c);
		d64a=b;
		d64b=c;
		d64a*=d64b;
		c=d64a/1000;
		b+=c;
	}
	else
	{//ϵ����
		c&=0x7fff;
		c=bcd_hex(c);
		d64a=b;
		d64b=c;
		d64a*=d64b;
		c=d64a/1000;
		b-=c;
		if((b>>31)!=0x0)
		{//���Ϊ��
			b=0;
		}
	}
#endif
	b=hex_bcd(b);//�ָ�ֵ
	return b;
}

void Terminal_Ctrl_Count(u32 ci)//�ն���բͳ�Ƽ���;���ci=0�µ����բ,=1�������բ,=2������բ,3=ң����բ
{
	u32 i;
	i=MRR(ADDR_AFN0DF50_SOUR+ci,1);
	i++;
	MWR(i,ADDR_AFN0DF50_SOUR+ci,1);
	i=MRR(ADDR_AFN0DF52_SOUR+ci,1);
	i++;
	MWR(i,ADDR_AFN0DF52_SOUR+ci,1);
}

void AFN0DF27F35_Computer(void)//�ն�ͳ��.��ѹͳ������
{
	u32 i;
	u32 Pn;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 gfs;//Խ�����޵�ABC�������־
	u32 gfx;//Խ�����޵�ABC�������־
	u32 unfs;//Խ�����޵�ABC��ָ���־
	u32 unfx;//Խ�����޵�ABC��ָ���־
	u32 t;
	u8 * p8;
	u64 d64;
	
	Pn=Terminal_Ram->I2SUBTask4_3;
	if(Pn>=AFN0DF27MaxPn)
	{
		Pn=0;
	}

	p8=(u8 *)(ADDR_AFN0DF27F35_SOUR+(LEN_AFN0DF27F35_SOUR*Pn));
	t=p8[0];//�ָ��¼Ӽ���
	p8[0]=0;

	MR(ADDR_DATABUFF,ADDR_AFN0DF27_SOUR+((66+27)*Pn),66+27);
	MR(ADDR_DATABUFF+(66+27),ADDR_AFN0DF27_PAR+(18*Pn),18);
	MR(ADDR_DATABUFF+(66+27)+18,ADDR_AFN0DF35_SOUR+((66+27)*Pn),66+27);
	MR(ADDR_DATABUFF+(66+27)+18+(66+27),ADDR_AFN0DF35_PAR+(18*Pn),18);
	if(Pn>=AFN04F26MaxPn)
	{//Pn>�������(��������ֵ����)
		Pn++;
		Terminal_Ram->I2SUBTask4_3=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
	x+=37;
	y=ADDR_AFN04F26+(57*Pn);
	gfs=0;//Խ�����޵�ABC�������־
	gfx=0;//Խ�����޵�ABC�������־
	unfs=0;//Խ�����޵�ABC��ָ���־
	unfx=0;//Խ�����޵�ABC��ָ���־
	for(i=0;i<3;i++)
	{
		#if (((Project/100)==3)&&(MeterPW==1))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		if(i==1)
		{//B���ѹͳ��Ϊ�ϸ�
			MWR(0x100000,ADDR_DATABUFF+66+(9*i)+6,3);
			MWR(0x100000,ADDR_DATABUFF+(66+27)+18+66+(9*i)+6,3);
			continue;
		}
		#endif
		if(Check_ACSamplePn(Pn+1)==0)//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
		{
			a=MRR(x+(2*i),2);//ȡ��ǰ��ѹ
		}
		else
		{
		#if (((USER/100)==13) || ((USER/100)==11))//������������
			//��������̨��ʱ��̨����Դ���Լ1�ְ��Ӻ��Ҫ�ٶ��¼������ý���1����ƽ����ѹ�Ļ����¼������������ʸĳɵ�ǰ��ѹ
			a=MRR(x+(2*i),2);//ȡ��ǰ��ѹ
		#else
			a=MRR(ADDR_Average_Voltage+(2*i),2);//ȡ�ڲ�����A,B,C��1����ƽ����ѹ(�ϵ��ʼΪ0xee)
		#endif	
		}
		if(a==0xeeee)
		{
			continue;
		}
		switch(p8[1+(4*i)+3])//ԭԽ���ȶ���־:0=ûԽ��,1=Խ������,2=Խ������
		{
			case 0://ûԽ��
				b=MRR(y+6,2);//ȡ��ѹ������
				if(b==0)
				{
					b=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ������
					if(p8[1+(4*i)+2]!=1)
					{//ԭûԽ������
						p8[1+(4*i)+2]=1;//��ǰԽ�����ޱ�־
						z=MRR(y+8,1)*60;//Խ�޳���ʱ��
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//ԭԽ������
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+(4*i)+3]=1;//Խ���ȶ���־=1
							gfs|=1<<i;//Խ�����޵�ABC�������־
							//Խ�����������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(y+8,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(10*i),2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i),2);
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i),2);
//							#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
								//��Խ����ʱ��ӷ�
								c=MRR(ADDR_DATABUFF+(10*i)+4,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+4,2);
								//��Խ����ʱ��ӷ�
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
//							#endif
							}
						}
					}
					break;
				}
				b=MRR(y+11,2);//ȡ������
				if(a<b)
				{//Խ������
					if(p8[1+(4*i)+2]!=2)
					{//ԭûԽ������
						p8[1+(4*i)+2]=2;//��ǰԽ�����ޱ�־
						z=MRR(y+13,1)*60;//Խ�޳���ʱ��
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//ԭԽ������
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+(4*i)+3]=2;//Խ���ȶ���־=2
							gfx|=1<<i;//Խ�����޵�ABC�������־
							//Խ�����������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(y+13,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(10*i)+2,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+2,2);
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
//							#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
								//��Խ����ʱ��ӷ�
								c=MRR(ADDR_DATABUFF+(10*i)+6,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(10*i)+6,2);
								//��Խ����ʱ��ӷ�
								c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
								c+=z;
								if(c>0xffff)
								{
									c=0xffff;
								}
								MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
//							#endif
							}
						}
					}
					break;
				}
				p8[1+(4*i)+2]=0;//ԭûԽ�ޱ�־(������,������)
				if(t!=0x0)
				{
					b=MRR(y,2);//ȡ��ѹ����
					if(b==0)
					{
						b=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
					}
					if(a>=b)
					{//Խ����
						//��Խ����ʱ��ӷ�
						c=MRR(ADDR_DATABUFF+(10*i)+4,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(10*i)+4,2);
						//��Խ����ʱ��ӷ�
						c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
						break;
					}
					b=MRR(y+2,2);//ȡ��ѹ����
					if(a<b)
					{//Խ����
						//��Խ����ʱ��ӷ�
						c=MRR(ADDR_DATABUFF+(10*i)+6,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(10*i)+6,2);
						//��Խ����ʱ��ӷ�
						c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
						c+=t;
						if(c>0xffff)
						{
							c=0xffff;
						}
						MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
						break;
					}
					//�պϸ��ѹʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(10*i)+8,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+8,2);
					//�ºϸ��ѹʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+8,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+8,2);
				}
				break;
			case 1://Խ������
				b=MRR(y+6,2);//ȡ��ѹ������
				c=MRR(y+9,2);//ȡ��ѹ�����޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a<b)
				{
					if(p8[1+(4*i)+2]==1)
					{//ԭ��ǰԽ������
						p8[1+(4*i)+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(y+8,1)*60;//Խ�޳���ʱ��
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+(4*i)+3]=0;//Խ���ȶ���־=0
							unfs|=1<<i;//Խ�����޵�ABC��ָ���־
						}
					}
				}
				else
				{
					p8[1+(4*i)+2]=1;//ԭ��ǰԽ������
				}
				if(t!=0x0)
				{
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(10*i),2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i),2);
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i),2);
//				#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
					//��Խ����ʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(10*i)+4,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+4,2);
					//��Խ����ʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);
//				#endif	
				}
				break;
			case 2://Խ������
				b=MRR(y+11,2);//ȡ��ѹ������
				c=MRR(y+14,2);//ȡ��ѹ�����޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(a>=b)
				{
					if(p8[1+(4*i)+2]==2)
					{//ԭ��ǰԽ������
						p8[1+(4*i)+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(y+13,1)*60;//Խ�޳���ʱ��
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+(4*i)+3]=0;//Խ���ȶ���־=0
							unfx|=1<<i;//Խ�����޵�ABC��ָ���־
						}
					}
				}
				else
				{
					p8[1+(4*i)+2]=2;//ԭ��ǰԽ������
				}
				if(t!=0x0)
				{
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(10*i)+2,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+2,2);
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);
//				#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
					//��Խ����ʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(10*i)+6,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(10*i)+6,2);
					//��Խ����ʱ��ӷ�
					c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
					c+=t;
					if(c>0xffff)
					{
						c=0xffff;
					}
					MWR(c,ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);
//				#endif
				}
				break;
			default:
				p8[1+(4*i)+3]=0;//Խ���ȶ���־=0
				break;
		}
		//������ѹ
		b=MRR(ADDR_DATABUFF+30+(10*i),2);
		if(a>b)
		{
			MWR(a,ADDR_DATABUFF+30+(10*i),2);
			MR(ADDR_DATABUFF+30+(10*i)+2,ADDR_TYMDHMS+1,3);
		}
		//������ѹ
		b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i),2);
		if(a>b)
		{
			MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i),2);
			MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+2,ADDR_TYMDHMS+1,3);
		}
		//����С��ѹ
		b=MRR(ADDR_DATABUFF+30+(10*i)+5+2,3);
		if(b==0x0)
		{//ԭ��С��ѹ=��,��ǰ��ѹ��Ϊ��С��ѹ
			MWR(a,ADDR_DATABUFF+30+(10*i)+5,2);
			MR(ADDR_DATABUFF+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
		}
		else
		{
			b=MRR(ADDR_DATABUFF+30+(10*i)+5,2);
			if(a<b)
			{
				MWR(a,ADDR_DATABUFF+30+(10*i)+5,2);
				MR(ADDR_DATABUFF+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
			}
		}
		//����С��ѹ
		b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,3);
		if(b==0x0)
		{//ԭ��С��ѹ=��,��ǰ��ѹ��Ϊ��С��ѹ
			MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
			MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
		}
		else
		{
			b=MRR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
			if(a<b)
			{
				MWR(a,ADDR_DATABUFF+(66+27)+18+30+(10*i)+5,2);
				MR(ADDR_DATABUFF+(66+27)+18+30+(10*i)+5+2,ADDR_TYMDHMS+1,3);
			}
		}
		//��ƽ����ѹ
		a=bcd_hex(a);
		b=MRR(ADDR_DATABUFF+(66+27)+(6*i),2);
		b++;//����+1
		MWR(b,ADDR_DATABUFF+(66+27)+(6*i),2);
		c=MRR(ADDR_DATABUFF+(66+27)+(6*i)+2,4);
		c+=a;
		MWR(c,ADDR_DATABUFF+(66+27)+(6*i)+2,4);
		c/=b;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+60+(2*i),2);
		//��ƽ����ѹ
		b=MRR(ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i),2);
		b++;//����+1
		MWR(b,ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i),2);
		c=MRR(ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i)+2,4);
		c+=a;
		MWR(c,ADDR_DATABUFF+(66+27)+18+(66+27)+(6*i)+2,4);
		c/=b;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+(66+27)+18+60+(2*i),2);
		
		//�յ�ѹԽ������ ����¼A.25	%	3
		a=MRR(ADDR_DATABUFF+(10*i)+4,2);//Խ�������ۼ�ʱ��
		b=MRR(ADDR_DATABUFF+(10*i)+4+2,2);//Խ�������ۼ�ʱ��
//	#if (USER/100)==5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
//		c=MRR(ADDR_DATABUFF+(10*i),2);	//Խ���������ۼ�ʱ��
//		a+=c;
//		c=MRR(ADDR_DATABUFF+(10*i)+2,2);//Խ���������ۼ�ʱ��		
//		b+=c;						
//	#endif
		c=MRR(ADDR_DATABUFF+(10*i)+4+4,2);//�ϸ����ۼ�ʱ��
		z=a+b+c;
		if(z==0)
		{
			a=0;
			b=0;
			c=0;
			z=1;
		}
		d64=a;
		d64*=100000;//3λС����%
		a=d64/z;
		a=hex_bcd(a);
		MWR(a,ADDR_DATABUFF+66+(9*i),3);
		//�յ�ѹԽ������ ����¼A.25	%	3
		d64=b;
		d64*=100000;//3λС����%
		b=d64/z;
		b=hex_bcd(b);
		MWR(b,ADDR_DATABUFF+66+(9*i)+3,3);
		//�յ�ѹ�ϸ��� ����¼A.25	%	3
		d64=c;
		d64*=100000;//3λС����%
		c=d64/z;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+66+(9*i)+6,3);
		//�µ�ѹԽ������ ����¼A.25	%	3
		a=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4,2);//Խ�������ۼ�ʱ��
		b=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+6,2);//Խ�������ۼ�ʱ��
//	#if (USER/100)==5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
//		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i),2);//Խ���������ۼ�ʱ��
//		a+=c;
//		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+2,2);//Խ���������ۼ�ʱ��
//		b+=c;
//	#endif
		c=MRR(ADDR_DATABUFF+(66+27)+18+(10*i)+4+4,2);//�ϸ����ۼ�ʱ��
		z=a+b+c;
		if(z==0)
		{
			a=0;
			b=0;
			c=0;
			z=1;
		}
		d64=a;
		d64*=100000;//3λС����%
		a=d64/z;
		a=hex_bcd(a);
		MWR(a,ADDR_DATABUFF+(66+27)+18+66+(9*i),3);
		//�µ�ѹԽ������ ����¼A.25	%	3
		d64=b;
		d64*=100000;//3λС����%
		b=d64/z;
		b=hex_bcd(b);
		MWR(b,ADDR_DATABUFF+(66+27)+18+66+(9*i)+3,3);
		//�µ�ѹ�ϸ��� ����¼A.25	%	3
		d64=c;
		d64*=100000;//3λС����%
		c=d64/z;
		c=hex_bcd(c);
		MWR(c,ADDR_DATABUFF+(66+27)+18+66+(9*i)+6,3);
	}
	MW(ADDR_DATABUFF,ADDR_AFN0DF27_SOUR+((66+27)*Pn),(66+27));
	MW(ADDR_DATABUFF+(66+27),ADDR_AFN0DF27_PAR+(18*Pn),18);
	MW(ADDR_DATABUFF+(66+27)+18,ADDR_AFN0DF35_SOUR+((66+27)*Pn),(66+27));
	MW(ADDR_DATABUFF+(66+27)+18+(66+27),ADDR_AFN0DF35_PAR+(18*Pn),18);
	//�¼�
//	if(unfs!=0)//Խ�����޵�ABC��ָ���־
//	{
//		unfs|=1<<6;
//		ERC24_Event(Pn+1,0,unfs);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(unfx!=0)//Խ�����޵�ABC��ָ���־
//	{
//		unfx|=2<<6;
//		ERC24_Event(Pn+1,0,unfx);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(gfs!=0)//Խ�����޵�ABC�������־
//	{
//		gfs|=1<<6;
//		ERC24_Event(Pn+1,1,gfs);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(gfx!=0)//Խ�����޵�ABC�������־
//	{
//		gfx|=2<<6;
//		ERC24_Event(Pn+1,1,gfx);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
	if(ERC_Event_Set(24)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[13]=0;//B0-B2�ֱ�ABCԽ�������¼��Ѽ�¼,B4-B6�ֱ�ABCԽ�����¼��Ѽ�¼
	}
	else
	{
		//Խ�����޻ָ�
		x=0;
		if(p8[4]==1)
		{
			x|=1;
		}
		if(p8[8]==1)
		{
			x|=2;
		}
		if(p8[12]==1)
		{
			x|=4;
		}
		y=x^(p8[13]&0xf);
		if(y!=0)
		{
			//�ָ�
			z=y&(~x);
			if(z!=0)
			{
				p8[13]&=(~z);
				z|=1<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,0,z);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		//Խ�����޻ָ�
		x=0;
		if(p8[4]==2)
		{
			x|=0x10;
		}
		if(p8[8]==2)
		{
			x|=0x20;
		}
		if(p8[12]==2)
		{
			x|=0x40;
		}
		y=x^(p8[13]&0xf0);
		if(y!=0)
		{
			//�ָ�
			z=y&(~x);
			if(z!=0)
			{
				p8[13]&=(~z);
				z>>=4;
				z|=2<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,0,z);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		//Խ�����޲���
		x=0;
		if(p8[4]==1)
		{
			x|=1;
		}
		if(p8[8]==1)
		{
			x|=2;
		}
		if(p8[12]==1)
		{
			x|=4;
		}
		y=x^(p8[13]&0xf);
		if(y!=0)
		{
			//����
			z=y&x;
			if(z!=0)
			{
				p8[13]|=z;
				z|=1<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,1,z);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		//Խ�����޲���
		x=0;
		if(p8[4]==2)
		{
			x|=0x10;
		}
		if(p8[8]==2)
		{
			x|=0x20;
		}
		if(p8[12]==2)
		{
			x|=0x40;
		}
		y=x^(p8[13]&0xf0);
		if(y!=0)
		{
			//����
			z=y&x;
			if(z!=0)
			{
				p8[13]|=z;
				z>>=4;
				z|=2<<6;
			#if ERC24==1
				ERC24_Event(Pn+1,1,z);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
	
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_3=Pn;
}

void AFN0DF26F34_Computer(void)//F26F34�����ܼ������������������ʱ��
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u32 Addr;
	u64 d64;
	u8 *p8;
	
	for(Pn=0;Pn<AFN0DF26MaxPn;Pn++)
	{
		d64=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		if(d64==0)
		{//û�ϴ������������ʱ��
			MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		}
		p8=(u8*)(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+5);
		if(p8[0]==Comm_Ram->TYMDHMS[1])
		{
			continue;
		}
		p8[0]=Comm_Ram->TYMDHMS[1];
		p8++;
		
		Addr=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		if(Addr!=0)
		{
			for(i=0;i<4;i++)
			{
				x=MRR(Addr+5+(i*3),3);
				if(x!=0xeeeeee)
				{
					x&=0x7fffff;//ȥ����
					x=bcd_hex(x);
					y=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
					y+=x;
					MWR(y,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
					p8[i]++;//�Ӵ�����1
				}
			}
		}
		MC(0,ADDR_DATABUFF,1);
		MR(ADDR_DATABUFF+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);
		i=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
		i/=60;
		if(i>=15)
		{//15������ʱ�䵽
			MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn),5);//�ϴ������������ʱ��
			for(i=0;i<4;i++)
			{
				x=MRR(ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+10+(i*4),4);
				y=p8[i];//�Ӵ���
				if(y!=0)
				{
					x/=y;
				}
				x=hex_bcd(x);
				//�����
				y=MRR(ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6),3);
				if(x>y)
				{
					MWR(x,ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6),3);
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF26_SOUR+(24*Pn)+(i*6)+3,3);
				}
				//�����
				y=MRR(ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6),3);
				if(x>y)
				{
					MWR(x,ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6),3);
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF34_SOUR+(24*Pn)+(i*6)+3,3);
				}
			}
			//��ƽ������
			MC(0,ADDR_AFN0DF26F34_SOUR+(LEN_AFN0DF26F34_SOUR*Pn)+6,20);
		}
	}
}





