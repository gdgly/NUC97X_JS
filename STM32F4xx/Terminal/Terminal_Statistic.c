
//�ն�ͳ��
#include "Project.h"
#include "Terminal_Statistic.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Calculate/ASMCalculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"

#include "Terminal_AFN0E_Event.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"



void Init_StatisticData_Command(void)//�ն�ͳ�����ݳ�ʼ��(����,ȫ�����)
{
	MC(0,ADDR_Terminal_StatData_FM_Start,(ADDR_Terminal_StatData_FM_End)-(ADDR_Terminal_StatData_FM_Start));
	MC(0,ADDR_Terminal_StatData_ESRAM_Start,(ADDR_Terminal_StatData_ESRAM_End)-(ADDR_Terminal_StatData_ESRAM_Start));
}



void Terminal_Reset_Count(void)//�ն˸�λ����
{
	u32 i;
	
	i=MRR(ADDR_AFN0DF49_SOUR+2,2);
	i++;
	MWR(i,ADDR_AFN0DF49_SOUR+2,2);
	i=MRR(ADDR_AFN0DF51_SOUR+2,2);
	i++;
	MWR(i,ADDR_AFN0DF51_SOUR+2,2);

}


void AFN0DF25F33_Computer(void)//�ն�ͳ��.�����ܼ���������й����ʼ�����ʱ��,�й�����Ϊ��ʱ��
{
	u32 i;
	u32 Pn;
//	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 t;
	u32 f;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_1;
	if(Pn>=AFN0DF25MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF25F33_SOUR);
	t=p8[Pn];
	if(t!=0x0)
	{//�ָ���
		p8[Pn]=0;//��ָ���
		MR(ADDR_DATABUFF,ADDR_AFN0DF25_SOUR+(32*Pn),32);
		MR(ADDR_DATABUFF+32,ADDR_AFN0DF33_SOUR+(32*Pn),32);
		x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
		x+=5;
		p8=(u8 *)(x);
		f=0;//���ݸ��±�־
		for(i=0;i<4;i++)
		{
			if(p8[3*i]!=0xee)
			{
				y=MRR(x+(3*i),3);
				if(y==0xeeeeee)
				{
					continue;
				}
				y&=0x7fffff;
				//�����Ƚ�
				z=MRR(ADDR_DATABUFF+(6*i),3);
				if(y>z)
				{
					f=1;//���ݸ��±�־
					MWR(y,ADDR_DATABUFF+(6*i),3);
					MR(ADDR_DATABUFF+(6*i)+3,ADDR_TYMDHMS+1,3);
				}
				//�����Ƚ�
				z=MRR(ADDR_DATABUFF+32+(6*i),3);
				if(y>z)
				{
					f=1;//���ݸ��±�־
					MWR(y,ADDR_DATABUFF+32+(6*i),3);
					MR(ADDR_DATABUFF+32+(6*i)+3,ADDR_TYMDHMS+1,3);
				}
				//=0
				if(y==0x0)
				{
					f=1;//���ݸ��±�־
					z=MRR(ADDR_DATABUFF+24+(2*i),2);
					z+=t;
					MWR(z,ADDR_DATABUFF+24+(2*i),2);
					z=MRR(ADDR_DATABUFF+32+24+(2*i),2);
					z+=t;
					MWR(z,ADDR_DATABUFF+32+24+(2*i),2);
				}
			}
		}
		if(f!=0x0)
		{
			MW(ADDR_DATABUFF,ADDR_AFN0DF25_SOUR+(32*Pn),32);
			MW(ADDR_DATABUFF+32,ADDR_AFN0DF33_SOUR+(32*Pn),32);
		}
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_1=Pn;
}


void AFN0DF29F37_Computer(void)//�ն�ͳ��.����ͳ������
{
	u32 i;
	u32 Pn;
	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 gfs;//Խ�����޵�ABC�������־
	u32 gfx;//Խ���޵�ABC�������־
	u32 unfs;//Խ�����޵�ABC��ָ���־
	u32 unfx;//Խ���޵�ABC��ָ���־
	u32 t;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_4;
	if(Pn>=AFN0DF29MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF29F37_SOUR+(LEN_AFN0DF29F37_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF29_SOUR+(38*Pn),38);
	MR(ADDR_DATABUFF+38,ADDR_AFN0DF37_SOUR+(38*Pn),38);
	PnMap=(Pn+1);
	if(PnMap>AFN04F26MaxPn)
	{//PnMap>�������(��������ֵ����)
		Pn++;
		Terminal_Ram->I2SUBTask4_4=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
	x+=43;
	y=ADDR_AFN04F26+(57*(PnMap-1));
	gfs=0;//Խ�����޵�ABC�������־
	gfx=0;//Խ���޵�ABC�������־
	unfs=0;//Խ�����޵�ABC��ָ���־
	unfx=0;//Խ���޵�ABC��ָ���־
	//ABC��
	for(i=0;i<3;i++)
	{
		a=MRR(x+(3*i),3);//ȡ��ǰ����
		if(a==0xeeeeee)
		{
			continue;
		}
		a&=0x7fffff;
		switch(p8[1+(4*i)+3])//ԭԽ�ޱ�־:0=ûԽ��,1=Խ������,2=Խ����
		{
			case 0://ûԽ��
				b=MRR(y+16,3);//ȡ������
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ������
					if(p8[1+(4*i)+2]!=1)
					{//ԭûԽ������
						p8[1+(4*i)+2]=1;//��ǰԽ�����ޱ�־
						z=MRR(y+19,1)*60;//Խ�޳���ʱ��
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
							z=MRR(y+19,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(4*i),2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i),2);
								//Խ���������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+38+(4*i),2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i),2);
//							#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i)+2,2);
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
//							#endif
							}
						}
					}
					break;
				}
				b=MRR(y+22,3);//ȡ����
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ����
					if(p8[1+(4*i)+2]!=2)
					{//ԭûԽ����
						p8[1+(4*i)+2]=2;//��ǰԽ���ޱ�־
						z=MRR(y+25,1)*60;//Խ�޳���ʱ��
						p8[1+(4*i)]=z;
						p8[1+(4*i)+1]=z>>8;
					}
					else
					{//ԭԽ����
						z=p8[1+(4*i)];
						z|=p8[1+(4*i)+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+(4*i)+3]=2;//Խ���ȶ���־=2
							gfx|=1<<i;//Խ���޵�ABC�������־
							//Խ���������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(y+25,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+(4*i)+2,2);
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
							}
						}
					}
					break;
				}
				p8[1+(4*i)+2]=0;//ԭûԽ�ޱ�־(������,����)
				break;
			case 1://Խ������
				b=MRR(y+16,3);//ȡ������
				b&=0x7fffff;
				c=MRR(y+20,2);//ȡ�����޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a<b)
				{
					b=MRR(y+22,3);//ȡ����
					b&=0x7fffff;
					if(b==0)
					{
						b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
					}
					if(a>=b)
					{//Խ����
						if(p8[1+(4*i)+2]!=2)
						{//ԭûԽ����
							p8[1+(4*i)+2]=2;//��ǰԽ���ޱ�־
							z=MRR(y+19,1)*60;//Խ�޳���ʱ��
							p8[1+(4*i)]=z;
							p8[1+(4*i)+1]=z>>8;
						}
						else
						{//ԭԽ����
							z=p8[1+(4*i)];
							z|=p8[1+(4*i)+1];
							if(z==0x0)
							{//Խ�޳���ʱ�䶨ʱ��
								p8[1+(4*i)+3]=2;//Խ���ȶ���־=2
								unfs|=1<<i;//Խ�����޵�ABC��ָ���־
								gfx|=1<<i;//Խ���޵�ABC�������־
							}
						}
					}
					else
					{
						if(p8[1+(4*i)+2]!=0)
						{//��ǰԽ������
							p8[1+(4*i)+2]=0;//��ǰûԽ�ޱ�־
							z=MRR(y+19,1)*60;//Խ�޳���ʱ��
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
				}
				else
				{
					p8[1+(4*i)+2]=1;//ԭ��ǰԽ������
				}
				if(t!=0x0)
				{
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(4*i),2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i),2);
					//Խ���������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+38+(4*i),2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i),2);
//				#if (USER/100)!=5//�����Ϻ���;�Ϻ�Ҫ��:Խ������ʱ,Ҫ����ͳ�Ƶ�Խ������ȥ
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i)+2,2);
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
//				#endif
				}
				break;
			case 2://Խ����
				b=MRR(y+22,3);//ȡ����
				b&=0x7fffff;
				c=MRR(y+26,2);//ȡ���޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a<b)
				{
					if(p8[1+(4*i)+2]!=0)
					{//ԭ��ǰԽ����
						p8[1+(4*i)+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(y+25,1)*60;//Խ�޳���ʱ��
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
							unfx|=1<<i;//Խ���޵�ABC��ָ���־
						}
					}
				}
				else
				{
					b=MRR(y+16,3);//ȡ������
					b&=0x7fffff;
					if(b==0)
					{
						b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
					}
					if(a>=b)
					{//Խ������
						if(p8[1+(4*i)+2]!=1)
						{//ԭûԽ������
							p8[1+(4*i)+2]=1;//��ǰԽ�����ޱ�־
							z=MRR(y+25,1)*60;//Խ�޳���ʱ��
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
								unfx|=1<<i;//Խ���޵�ABC��ָ���־
								gfs|=1<<i;//Խ�����޵�ABC�������־
							}
						}
					}
					else
					{
						p8[1+(4*i)+2]=2;//ԭ��ǰԽ����
					}
				}
				if(t!=0x0)
				{
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+(4*i)+2,2);
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+38+(4*i)+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+(4*i)+2,2);
				}
				break;
			default:
				p8[1+(4*i)+3]=0;//Խ���ȶ���־=0
				break;
		}
		//�����ֵ
		b=MRR(ADDR_DATABUFF+14+(6*i),3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+(3*i),3),ADDR_DATABUFF+14+(6*i),3);
			MR(ADDR_DATABUFF+14+(6*i)+3,ADDR_TYMDHMS+1,3);
		}
		//�����ֵ
		b=MRR(ADDR_DATABUFF+38+14+(6*i),3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+(3*i),3),ADDR_DATABUFF+38+14+(6*i),3);
			MR(ADDR_DATABUFF+38+14+(6*i)+3,ADDR_TYMDHMS+1,3);
		}
	}	
	//�������	
	a=MRR(x+9,3);//ȡ��ǰ����
	if(a!=0xeeeeee)
	{
		a&=0x7fffff;
		switch(p8[1+12+3])//ԭԽ�ޱ�־:0=ûԽ��,1=Խ����
		{
			case 0://ûԽ��
				b=MRR(y+28,3);//ȡ����
				b&=0x7fffff;
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ����
					if(p8[1+12+2]!=1)
					{//ԭûԽ����
						p8[1+12+2]=1;//��ǰԽ���ޱ�־
						z=MRR(y+31,1)*60;//Խ�޳���ʱ��
						p8[1+12]=z;
						p8[1+12+1]=z>>8;
					}
					else
					{//ԭԽ����
						z=p8[1+12];
						z|=p8[1+12+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+12+3]=1;//Խ���ȶ���־=1
							//Խ���������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(y+31,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+12,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+12,2);
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+38+12,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+38+12,2);
							}
						}
					}
					break;
				}
				p8[1+12+2]=0;//ԭûԽ�ޱ�־
				break;
			case 1://Խ����
				b=MRR(y+28,3);//ȡ����
				b&=0x7fffff;
				c=MRR(y+32,2);//ȡ���޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a<b)
				{
					if(p8[1+12+2]==1)
					{//ԭ��ǰԽ����
						p8[1+12+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(y+31,1)*60;//Խ�޳���ʱ��
						p8[1+12]=z;
						p8[1+12+1]=z>>8;
					}
					else
					{
						z=p8[1+12];
						z|=p8[1+12+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+12+3]=0;//Խ���ȶ���־=0
						}
					}
				}
				else
				{
					p8[1+12+2]=1;//ԭ��ǰԽ����
				}
				if(t!=0x0)
				{
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+12,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+12,2);
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+38+12,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+38+12,2);
				}
				break;
			default:
				p8[1+12+3]=0;//Խ���ȶ���־=0
				break;
		}
		//�����ֵ
		b=MRR(ADDR_DATABUFF+32,3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+9,3),ADDR_DATABUFF+32,3);
			MR(ADDR_DATABUFF+32+3,ADDR_TYMDHMS+1,3);
		}
		//�����ֵ
		b=MRR(ADDR_DATABUFF+38+32,3);
		b&=0x7fffff;
		if(a>b)
		{
			MWR(MRR(x+9,3),ADDR_DATABUFF+38+32,3);
			MR(ADDR_DATABUFF+38+32+3,ADDR_TYMDHMS+1,3);
		}
	}	

	MW(ADDR_DATABUFF,ADDR_AFN0DF29_SOUR+(38*Pn),38);
	MW(ADDR_DATABUFF+38,ADDR_AFN0DF37_SOUR+(38*Pn),38);
	//�¼�
//	Pn++;
//	if(unfs!=0)//Խ�����޵�ABC��ָ���־
//	{
//		unfs|=1<<6;
//		ERC25_Event(PnMap,0,unfs);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(unfx!=0)//Խ���޵�ABC��ָ���־
//	{
//		unfx|=2<<6;
//		ERC25_Event(PnMap,0,unfx);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(gfs!=0)//Խ�����޵�ABC�������־
//	{
//		gfs|=1<<6;
//		ERC25_Event(PnMap,1,gfs);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	if(gfx!=0)//Խ���޵�ABC�������־
//	{
//		gfx|=2<<6;
//		ERC25_Event(PnMap,1,gfx);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
//	}
//	Pn--;
	if(ERC_Event_Set(25)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[17]=0;//B0-B2�ֱ�ABCԽ�������¼��Ѽ�¼,B4-B6�ֱ�ABCԽ�����¼��Ѽ�¼
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
		y=x^(p8[17]&0xf);
		if(y!=0)
		{
			//�ָ�
			z=y&(~x);
			if(z!=0)
			{
				p8[17]&=(~z);
				z|=1<<6;
			#if ERC25==1
				ERC25_Event(PnMap,0,z);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		//Խ���޻ָ�
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
		y=x^(p8[17]&0xf0);
		if(y!=0)
		{
			//�ָ�
			z=y&(~x);
			if(z!=0)
			{
				p8[17]&=(~z);
				z>>=4;
				z|=2<<6;
			#if ERC25==1
				ERC25_Event(PnMap,0,z);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
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
		y=x^(p8[17]&0xf);
		if(y!=0)
		{
			//����
			z=y&x;
			if(z!=0)
			{
				p8[17]|=z;
				z|=1<<6;
			#if ERC25==1
				ERC25_Event(PnMap,1,z);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		//Խ���޲���
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
		y=x^(p8[17]&0xf0);
		if(y!=0)
		{
			//����
			z=y&x;
			if(z!=0)
			{
				p8[17]|=z;
				z>>=4;
				z|=2<<6;
			#if ERC25==1
				ERC25_Event(PnMap,1,z);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
			#endif
			}
		}
		
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_4=Pn;
}


void AFN0DF28F36_Computer(void)//�ն�ͳ��.��ѹ������ƽ��
{
	u32 Pn;
	u32 pw;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 max;
	u32 min;
	u32 unu;//��ѹ��ƽ��
	u32 uni;//������ƽ��
	u32 t;
	u32 gf;//������־
	u32 unf;//�ָ���־
	
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_5;
	if(Pn>=AFN0DF28MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	if(Pn>=AFN04F25MaxPn)
	{//Pn>�������(�������������)
		Pn=0;
		Terminal_Ram->I2SUBTask4_5=Pn;
		return;
	}
	x=ADDR_AFN04F25+(11*Pn);
	pw=MRR(x+10,1);
	pw&=0x3;//D0��D1�����ʾ�������Դ���߷�ʽ����ֵ1��3���α�ʾ�������ߡ��������ߡ������
	x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
	//��ѹ��ƽ��
	a=MRR(x+37,2);
	b=MRR(x+37+2,2);
	c=MRR(x+37+4,2);

	gf=0;
	if(a!=0xeeee)
	{
		max=a;
		min=a;
		gf|=1;
	}
	if(b!=0xeeee)
	{
		if(pw==2)
		{
			if(b>max)
			{
				max=b;
			}
			if(b<min)
			{
				min=b;
			}
			gf|=2;
		}
	}
	if(c!=0xeeee)
	{
		if(c>max)
		{
			max=c;
		}
		if(c<min)
		{
			min=c;
		}
		gf|=4;
	}
	switch(pw)
	{
		case 1:
			if(gf!=0x5)
			{
				goto end;
			}
			break;
		case 2:
			if(gf!=0x7)
			{
				goto end;
			}
			break;
		default:
			goto end;
	}
	if(max!=0x0)
	{
		min=bcd_hex(min);
		max=bcd_hex(max);
		min=max-min;
		unu=(min*1000)/max;//��ѹ��ƽ��
		unu=hex_bcd(unu);
	}
	else
	{
		unu=0;
	}
	MWR(unu,ADDR_AFN0DF28F36_SOUR+(Pn*LEN_AFN0DF28F36_SOUR)+12,2);//�浱ǰ��ѹ��ƽ���
	
	//������ƽ��
	a=MRR(x+43,3);
	b=MRR(x+43+3,3);
	c=MRR(x+43+6,3);
	gf=0;
	if(a!=0xeeeeee)
	{
		a&=0x7fffff;
		max=a;
		min=a;
		gf|=1;
	}
	if(b!=0xeeee)
	{
		if(pw==2)
		{
			b&=0x7fffff;
			if(b>max)
			{
				max=b;
			}
			if(b<min)
			{
				min=b;
			}
			gf|=2;
		}
	}
	if(c!=0xeeee)
	{
		c&=0x7fffff;
		if(c>max)
		{
			max=c;
		}
		if(c<min)
		{
			min=c;
		}
		gf|=4;
	}
	switch(pw)
	{
		case 1:
			if(gf!=0x5)
			{
				goto end;
			}
			break;
		case 2:
			if(gf!=0x7)
			{
				goto end;
			}
			break;
		default:
			goto end;
	}
	if(max!=0x0)
	{
		min=bcd_hex(min);
		max=bcd_hex(max);
		min=max-min;
		uni=(min*1000)/max;//������ƽ��
		uni=hex_bcd(uni);
	}
	else
	{
		uni=0;
	}
	MWR(uni,ADDR_AFN0DF28F36_SOUR+(Pn*LEN_AFN0DF28F36_SOUR)+10,2);//�浱ǰ������ƽ���
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF28_SOUR+(14*Pn),14);
	MR(ADDR_DATABUFF+14,ADDR_AFN0DF36_SOUR+(16*Pn),16);
	x=ADDR_AFN04F26+(57*Pn);
	
	gf=0;//������־
	unf=0;//�ָ���־
	//������ƽ��
	switch(p8[1+3])//ԭ������ƽ���ȶ���־,0=ûԽ��,1=Խ��
	{
		case 0://ûԽ��
			a=MRR(x+51,2);//ȡ��ֵ
			a&=0x7fff;
			if(a==0)
			{
				a=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(uni>=a)
			{
					if(p8[1+2]!=1)
					{//ԭûԽ����
						p8[1+2]=1;//��ǰԽ���ޱ�־
						z=MRR(x+53,1)*60;//Խ�޳���ʱ��
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//ԭԽ����
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+3]=1;//Խ���ȶ���־=1
							gf|=2;//������־
							//Խ���������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(x+53,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF,2);
								c+=z;
								MWR(c,ADDR_DATABUFF,2);
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+14,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+14,2);
							}
						}
					}
					break;
			}
			p8[1+2]=0;//ԭûԽ���ޱ�־
			break;
		case 1://Խ����
				b=MRR(x+51,2);//ȡ����
				b&=0x7fff;
				c=MRR(x+54,2);//ȡ���޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(uni<b)
				{
					if(p8[1+2]==1)
					{//ԭ��ǰԽ����
						p8[1+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(x+53,1)*60;//Խ�޳���ʱ��
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+3]=0;//Խ���ȶ���־=0
							unf|=2;//�ָ���־
						}
					}
				}
				else
				{
					p8[1+2]=1;//ԭ��ǰԽ����
				}
				if(t!=0x0)
				{
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF,2);
					c+=t;
					MWR(c,ADDR_DATABUFF,2);
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+14,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+14,2);
				}
				break;
			default:
				p8[1+3]=0;//Խ���ȶ���־=0
				break;
	}
	//������ƽ�������
	a=MRR(ADDR_DATABUFF+4,2);
	if(uni>a)
	{
		MWR(uni,ADDR_DATABUFF+4,2);
		MR(ADDR_DATABUFF+4+2,ADDR_TYMDHMS+1,3);
	}
	//������ƽ�������
	a=MRR(ADDR_DATABUFF+14+4,2);
	if(uni>a)
	{
		MWR(uni,ADDR_DATABUFF+14+4,2);
		MR(ADDR_DATABUFF+14+4+2,ADDR_TYMDHMS+1,4);
	}
	//��ѹ��ƽ��
	switch(p8[1+4+3])//ԭ��ѹ��ƽ��Խ�ȶ���־,0=ûԽ��,1=Խ��
	{
		case 0://ûԽ��
			a=MRR(x+46,2);//ȡ��ֵ
			a&=0x7fff;
			if(a==0)
			{
				a=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(unu>=a)
			{
					if(p8[1+4+2]!=1)
					{//ԭûԽ����
						p8[1+4+2]=1;//��ǰԽ���ޱ�־
						z=MRR(x+48,1)*60;//Խ�޳���ʱ��
						p8[1+4]=z;
						p8[1+4+1]=z>>8;
					}
					else
					{//ԭԽ����
						z=p8[1+4];
						z|=p8[1+4+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+4+3]=1;//Խ���ȶ���־=1
							gf|=1;//������־
							//Խ���������ۼ�ʱ��+Խ�޳����ж�ʱ��
							z=MRR(x+53,1);//Խ�޳���ʱ��
							if(z!=0x0)
							{
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+2,2);
								//Խ�������ۼ�ʱ��
								c=MRR(ADDR_DATABUFF+14+2,2);
								c+=z;
								MWR(c,ADDR_DATABUFF+14+2,2);
							}
						}
					}
					break;
			}
			p8[1+4+2]=0;//ԭûԽ���ޱ�־
			break;
		case 1://Խ����
				b=MRR(x+46,2);//ȡ����
				b&=0x7fff;
				c=MRR(x+49,2);//ȡ���޻ָ�ϵ��
				b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
				if(b==0)
				{
					b=0xffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(unu<b)
				{
					if(p8[1+4+2]==1)
					{//ԭ��ǰԽ����
						p8[1+4+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(x+48,1)*60;//Խ�޳���ʱ��
						p8[1+4]=z;
						p8[1+4+1]=z>>8;
					}
					else
					{
						z=p8[1+4];
						z|=p8[1+4+1];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+4+3]=0;//Խ���ȶ���־=0
							unf|=1;//�ָ���־
						}
					}
				}
				else
				{
					p8[1+4+2]=1;//ԭԽ�ޱ�־
				}
				if(t!=0x0)
				{
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+2,2);
					//Խ�������ۼ�ʱ��
					c=MRR(ADDR_DATABUFF+14+2,2);
					c+=t;
					MWR(c,ADDR_DATABUFF+14+2,2);
				}
				break;
			default:
				p8[1+4+3]=0;//Խ���ȶ���־=0
				break;
	}
	//��ѹ��ƽ�������
	a=MRR(ADDR_DATABUFF+9,2);
	if(unu>a)
	{
		MWR(unu,ADDR_DATABUFF+9,2);
		MR(ADDR_DATABUFF+9+2,ADDR_TYMDHMS+1,3);
	}
	//��ѹ��ƽ�������
	a=MRR(ADDR_DATABUFF+14+10,2);
	if(unu>a)
	{
		MWR(unu,ADDR_DATABUFF+14+10,2);
		MR(ADDR_DATABUFF+14+10+2,ADDR_TYMDHMS+1,4);
	}
	
	MW(ADDR_DATABUFF,ADDR_AFN0DF28_SOUR+(14*Pn),14);
	MW(ADDR_DATABUFF+14,ADDR_AFN0DF36_SOUR+(16*Pn),16);
	//�¼�
//	if(unf!=0x0)
//	{
//		ERC17_Event(PnMap,0,unf,unu,uni);//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
//	}
//	if(gf!=0x0)
//	{
//		ERC17_Event(PnMap,1,gf,unu,uni);//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
//	}
	if(ERC_Event_Set(17)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[9]=0;//�¼��Ѽ�¼��־,B0=��ѹ��ƽ��Խ��,B1=������ƽ��Խ��
	}
	else
	{
		x=0;
		if(p8[8]!=0)
		{
			x|=1;
		}
		if(p8[4]!=0)
		{
			x|=2;
		}
		y=x^(p8[9]&0x3);
		if(y!=0)
		{
			//�ָ�
			z=y&(~x);
			if(z!=0)
			{
				p8[9]&=(~z);
			#if ERC17==1
				ERC17_Event(Pn+1,0,z,unu,uni);//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
			#endif
			}
			//����
			z=y&x;
			if(z!=0)
			{
				p8[9]|=z;
			#if ERC17==1
				ERC17_Event(Pn+1,1,z,unu,uni);//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
			#endif
			}
		}
	}
	
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_5=Pn;
}

void AFN0DF30F38_Computer(void)//�ն�ͳ��.���ڹ���Խ���ۼ�ʱ��
{
	u32 Pn;
	u32 PnMap;
	u32 x;
	u32 y;
	u32 z;
	u32 a;
	u32 b;
	u32 c;
	u32 t;
	u32 gfs;//Խ�����޲�����־
	u32 gfx;//Խ���޲�����־
	u32 unfs;//Խ�����޻ָ���־
	u32 unfx;//Խ���޻ָ���־
	
	u8 * p8;
	
	gfs=gfs;//Խ�����޲�����־
	gfx=gfx;//Խ���޲�����־
	unfs=unfs;//Խ�����޻ָ���־
	unfx=unfx;//Խ���޻ָ���־
	
	Pn=Terminal_Ram->I2SUBTask4_6;
	if(Pn>=AFN0DF30MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF30F38_SOUR+(LEN_AFN0DF30F38_SOUR*Pn));
	t=p8[0];
	p8[0]=0;
	PnMap=(Pn+1);
	if(PnMap>AFN04F26MaxPn)
	{//PnMap>�������(��������ֵ����)
		Pn++;
		Terminal_Ram->I2SUBTask4_6=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
	x+=55;
	a=MRR(x,3);//ȡ��ǰ���ڹ���
	if(a==0xeeeeee)
	{
		goto end;
	}
	a&=0x7fffff;

	MR(ADDR_DATABUFF,ADDR_AFN0DF30_SOUR+(4*Pn),4);
	MR(ADDR_DATABUFF+4,ADDR_AFN0DF38_SOUR+(4*Pn),4);

	y=ADDR_AFN04F26+(57*(PnMap-1));
	gfs=0;//Խ�����޲�����־
	gfx=0;//Խ���޲�����־
	unfs=0;//Խ�����޻ָ���־
	unfx=0;//Խ���޻ָ���־
	switch(p8[1+3])//ԭԽ�ޱ�־:0=ûԽ��,1=Խ������,2=Խ����
	{
		case 0://ûԽ��
			b=MRR(y+34,3);//ȡ������
			if(b==0)
			{
				b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(a>=b)
			{//Խ������
				if(p8[1+2]!=1)
				{//ԭûԽ������
					p8[1+2]=1;//��ǰԽ�����ޱ�־
					z=MRR(y+37,1)*60;//Խ�޳���ʱ��
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{//ԭԽ������
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//Խ�޳���ʱ�䶨ʱ��
						p8[1+3]=1;//Խ���ȶ���־=1
						gfs=1;//Խ�����޲�����־
						//Խ�����������ۼ�ʱ��+Խ�޳����ж�ʱ��
						z=MRR(y+37,1);//Խ�޳���ʱ��
						if(z!=0x0)
						{
							//Խ���������ۼ�ʱ��
							c=MRR(ADDR_DATABUFF,2);
							c+=z;
							MWR(c,ADDR_DATABUFF,2);
							//Խ���������ۼ�ʱ��
							c=MRR(ADDR_DATABUFF+4,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4,2);

							//Խ�������ۼ�ʱ��(���Ժ����Խ������ҲҪ��Խ�����ۼ�ʱ��)
							c=MRR(ADDR_DATABUFF+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+2,2);
							//Խ�������ۼ�ʱ��(���Ժ����Խ������ҲҪ��Խ�����ۼ�ʱ��)
							c=MRR(ADDR_DATABUFF+4+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4+2,2);
						}
					}
				}
				break;
			}
			b=MRR(y+40,3);//ȡ����
			if(b==0)
			{
				b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(a>=b)
			{//Խ����
				if(p8[1+2]!=2)
				{//ԭûԽ����
					p8[1+2]=2;//��ǰԽ���ޱ�־
					z=MRR(y+43,1)*60;//Խ�޳���ʱ��
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{//ԭԽ����
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//Խ�޳���ʱ�䶨ʱ��
						p8[1+3]=2;//Խ���ȶ���־=2
						gfx=1;//Խ���޲�����־
						//Խ���������ۼ�ʱ��+Խ�޳����ж�ʱ��
						z=MRR(y+43,1);//Խ�޳���ʱ��
						if(z!=0x0)
						{
							//Խ�������ۼ�ʱ��
							c=MRR(ADDR_DATABUFF+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+2,2);
							//Խ�������ۼ�ʱ��
							c=MRR(ADDR_DATABUFF+4+2,2);
							c+=z;
							MWR(c,ADDR_DATABUFF+4+2,2);
						}
					}
				}
				break;
			}
			p8[1+2]=0;//ԭûԽ�ޱ�־
			break;
		case 1://Խ������
			b=MRR(y+34,3);//ȡ������
			c=MRR(y+38,2);//ȡ�����޻ָ�ϵ��
			b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
			if(b==0)
			{
				b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(a<b)
			{
				b=MRR(y+40,3);//ȡ����
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ����
					if(p8[1+2]!=2)
					{//ԭûԽ����
						p8[1+2]=2;//��ǰԽ���ޱ�־
						z=MRR(y+37,1)*60;//Խ�޳���ʱ��
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//ԭԽ����
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+3]=2;//Խ���ȶ���־=2
							unfs=1;//Խ�����޻ָ���־
							gfx=1;//Խ���޲�����־
						}
					}
				}
				else
				{
					if(p8[1+2]!=0)
					{//ԭ��ǰԽ������
						p8[1+2]=0;//��ǰûԽ�ޱ�־
						z=MRR(y+37,1)*60;//Խ�޳���ʱ��
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+3]=0;//Խ���ȶ���־=0
							unfs=1;//Խ�����޻ָ���־
						}
					}
				}
			}
			else
			{
				p8[1+2]=1;//��ǰԽ�����ޱ�־
			}
			if(t!=0x0)
			{
				//Խ���������ۼ�ʱ��
				c=MRR(ADDR_DATABUFF,2);
				c+=t;
				MWR(c,ADDR_DATABUFF,2);
				//Խ���������ۼ�ʱ��
				c=MRR(ADDR_DATABUFF+4,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4,2);

				//Խ�������ۼ�ʱ��(���Ժ����Խ������ҲҪ��Խ�����ۼ�ʱ��)
				c=MRR(ADDR_DATABUFF+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+2,2);
				//Խ�������ۼ�ʱ��(���Ժ����Խ������ҲҪ��Խ�����ۼ�ʱ��)
				c=MRR(ADDR_DATABUFF+4+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4+2,2);
			}
			break;
		case 2://Խ����
			b=MRR(y+40,3);//ȡ����
			c=MRR(y+44,2);//ȡ���޻ָ�ϵ��
			b=ReNewValue(b,c);//�ָ�ֵ����,���b=��ֵ,c=�ָ�ϵ��
			if(b==0)
			{
				b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
			}
			if(a<b)
			{
				if(p8[1+2]!=0)
				{//ԭ��ǰԽ����
					p8[1+2]=0;//��ǰûԽ�ޱ�־
					z=MRR(y+43,1)*60;//Խ�޳���ʱ��
					p8[1]=z;
					p8[2]=z>>8;
				}
				else
				{
					z=p8[1];
					z|=p8[2];
					if(z==0x0)
					{//Խ�޳���ʱ�䶨ʱ��
						p8[1+3]=0;//Խ���ȶ���־=0
						unfx=1;//Խ���޻ָ���־
					}
				}
			}
			else
			{
				b=MRR(y+34,3);//ȡ������
				if(b==0)
				{
					b=0xffffff;//�¼��жϷ�ֵΪ0ʱ,�����ж�
				}
				if(a>=b)
				{//Խ������
					if(p8[1+2]!=1)
					{//ԭûԽ������
						p8[1+2]=1;//��ǰԽ�����ޱ�־
						z=MRR(y+43,1)*60;//Խ�޳���ʱ��
						p8[1]=z;
						p8[2]=z>>8;
					}
					else
					{//ԭԽ������
						z=p8[1];
						z|=p8[2];
						if(z==0x0)
						{//Խ�޳���ʱ�䶨ʱ��
							p8[1+3]=1;//Խ���ȶ���־=1
							unfx=1;//Խ���޻ָ���־
							gfs=1;//Խ�����޲�����־
						}
					}
				}
				else
				{
					p8[1+2]=2;//ԭ��ǰԽ����
				}
			}
			if(t!=0x0)
			{
				//Խ�������ۼ�ʱ��
				c=MRR(ADDR_DATABUFF+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+2,2);
				//Խ�������ۼ�ʱ��
				c=MRR(ADDR_DATABUFF+4+2,2);
				c+=t;
				MWR(c,ADDR_DATABUFF+4+2,2);
			}
			break;
		default:
			p8[1+3]=0;//Խ���ȶ���־=0
			break;
	}
	MW(ADDR_DATABUFF,ADDR_AFN0DF30_SOUR+(4*Pn),4);
	MW(ADDR_DATABUFF+4,ADDR_AFN0DF38_SOUR+(4*Pn),4);
	//�¼�
//	Pn++;
//	if(unfs!=0)//Խ�����޻ָ���־
//	{
//		b=MRR(y+34,3);//ȡ������
//		ERC26_Event(PnMap,0,1<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
//	}
//	if(unfx!=0)//Խ���޻ָ���־
//	{
//		b=MRR(y+40,3);//ȡ����
//		ERC26_Event(PnMap,0,2<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
//	}
//	if(gfs!=0)//Խ�����޲�����־
//	{
//		b=MRR(y+34,3);//ȡ������
//		ERC26_Event(PnMap,1,1<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
//	}
//	if(gfx!=0)//Խ���޲�����־
//	{
//		b=MRR(y+40,3);//ȡ����
//		ERC26_Event(PnMap,1,2<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
//	}
//	Pn--;
	if(ERC_Event_Set(26)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[5]=0;//���ڹ���Խ���¼���¼��־,0=û,B0=Խ������,B1=Խ����
	}
	else
	{
		x=(p8[4]&0x1)^(p8[5]&0x1);
		if(x!=0)
		{
			if((p8[4]&0x1)==0)
			{//Խ�����޻ָ�
				p8[5]&=0xfe;
				b=MRR(y+34,3);//ȡ������
			#if ERC26==1
				ERC26_Event(PnMap,0,1<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
			#endif
			}
		}
		x=(p8[4]&0x2)^(p8[5]&0x2);
		if(x!=0)
		{
			if((p8[4]&0x2)==0)
			{//Խ���޻ָ�
				p8[5]&=0xfd;
				b=MRR(y+40,3);//ȡ����
			#if ERC26==1
				ERC26_Event(PnMap,0,2<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
			#endif
			}
		}

		x=(p8[4]&0x1)^(p8[5]&0x1);
		if(x!=0)
		{
			if((p8[4]&0x1)!=0)
			{//Խ�����޲���
				p8[5]|=1;
				b=MRR(y+34,3);//ȡ������
			#if ERC26==1
				ERC26_Event(PnMap,1,1<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
			#endif
			}
		}
		x=(p8[4]&0x2)^(p8[5]&0x2);
		if(x!=0)
		{
			if((p8[4]&0x2)!=0)
			{//Խ���޲���
				p8[5]|=2;
				b=MRR(y+40,3);//ȡ����
			#if ERC26==1
				ERC26_Event(PnMap,1,2<<6,a,b);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
			#endif
			}
		}
		
	}
	
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_6=Pn;
}

void AFN0DF31F39_Computer(void)//�ն�ͳ��.������
{
	u32 i;
	u32 x;
	u32 Pn;
	u32 PnMap;
	u64 d64;
	
	Pn=Terminal_Ram->I2SUBTask4_7;
	if(Pn>=AFN0DF31MaxPn)
	{
		Pn=0;
	}
	PnMap=(Pn+1);
	if(PnMap>AFN04F25MaxPn)
	{//PnMap>�������(�������������)
		Pn++;
		Terminal_Ram->I2SUBTask4_7=Pn;
		return;
	}
	x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
	x+=55;
	x=MRR(x,3);//ȡ��ǰ���ڹ���
	if(x==0xeeeeee)
	{
		goto end;
	}
	x&=0x7fffff;
	x=bcd_hex(x);
	d64=x;
	d64*=1000;
	x=ADDR_AFN04F25+(11*(PnMap-1));
	x=MRR(x+7,3);//ȡ�����
	if(x==0x0)
	{
		goto end;
	}
	x=bcd_hex(x);
	x=d64/x;
	x=hex_bcd(x);
	
	MR(ADDR_DATABUFF,ADDR_AFN0DF31_SOUR+(10*Pn),10);
	MR(ADDR_DATABUFF+10,ADDR_AFN0DF39_SOUR+(12*Pn),12);
	//�����
	i=MRR(ADDR_DATABUFF,2);
	if(x>i)
	{
		MWR(x,ADDR_DATABUFF,2);
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,3);
	}
	//����С
	i=MRR(ADDR_DATABUFF+7,3);
	if(i==0x0)
	{//����С��ǰֵ��Ϊ��Сֵ
		MWR(x,ADDR_DATABUFF+5,2);
		MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
	}
	else
	{
		i=MRR(ADDR_DATABUFF+5,2);
		if(x<i)
		{
			MWR(x,ADDR_DATABUFF+5,2);
			MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
		}
	}
	//�����
	i=MRR(ADDR_DATABUFF+10,2);
	if(x>i)
	{
		MWR(x,ADDR_DATABUFF+10,2);
		MR(ADDR_DATABUFF+10+2,ADDR_TYMDHMS+1,4);
	}
	//����С
	i=MRR(ADDR_DATABUFF+10+8,4);
	if(i==0x0)
	{//����С��ǰֵ��Ϊ��Сֵ
		MWR(x,ADDR_DATABUFF+10+6,2);
		MR(ADDR_DATABUFF+10+8,ADDR_TYMDHMS+1,4);
	}
	else
	{
		i=MRR(ADDR_DATABUFF+10+6,2);
		if(x<i)
		{
			MWR(x,ADDR_DATABUFF+10+6,2);
			MR(ADDR_DATABUFF+10+8,ADDR_TYMDHMS+1,4);
		}
	}

	MW(ADDR_DATABUFF,ADDR_AFN0DF31_SOUR+(10*Pn),10);
	MW(ADDR_DATABUFF+10,ADDR_AFN0DF39_SOUR+(12*Pn),12);
end:
	Pn++;
	Terminal_Ram->I2SUBTask4_7=Pn;
}

void AFN0DF43F44_Computer(void)//�ն�ͳ��.�������������ۼ�ʱ��
{
	u32 i;
	u32 t;
	u32 Pn;
	u32 s;
	u32 s1;
	u32 s2;
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_9;
	if(Pn>=AFN0DF43MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF43F44_SOUR+(1*Pn));
	t=p8[0];
	if(t!=0x0)
	{//�ָ��¼Ӽ���!=0
		p8[0]=0;
		if(Pn>=AFN04F28MaxPn)
		{//Pn>=�������(�����㹦�������ֶ���ֵ)
			Pn++;
			Terminal_Ram->I2SUBTask4_9=Pn;
			return;
		}
		i=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
		i+=29;
		s=MRR(i,2);//ȡ��ǰ�ܹ�������
		if(s!=0xeeee)
		{
			s&=0x7fff;
			i=ADDR_AFN04F28+(4*Pn);//�����㹦�������ֶ���ֵ(�������)
			s1=MRR(i,2);//ȡ���������ֶ���ֵl
			s1&=0x7fff;
			s2=MRR(i+2,2);//ȡ���������ֶ���ֵ2
			s2&=0x7fff;
			if(s<s1)
			{//����1�ۼ�ʱ�䣨������������ֵ1��	BIN	min	2
				i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn),2);
				i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn),2);
			}
			else
			{
				if(s<s2)
				{//����2�ۼ�ʱ�䣨��ֵ1�ܹ�����������ֵ2�� 	BIN	min	2
					i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn)+2,2);
					i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn)+2,2);
				}
				else
				{//����3�ۼ�ʱ�䣨���������ݶ�ֵ2��  	BIN	min	2
					i=MRR(ADDR_AFN0DF43_SOUR+(6*Pn)+4,2);
					i+=t;
					MWR(i,ADDR_AFN0DF43_SOUR+(6*Pn)+4,2);
					i=MRR(ADDR_AFN0DF44_SOUR+(6*Pn)+4,2);
					i+=t;
					MWR(i,ADDR_AFN0DF44_SOUR+(6*Pn)+4,2);
				}
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_9=Pn;
}


void AFN0DF57F60_Computer(void)//�ն�ͳ��.�����ܼ��������С�й����ʼ��䷢��ʱ��,�й�����Ϊ���ۼ�ʱ��
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 Pn;
	u32 x;
	u32 y;
	u32 t;
	u64 a64;
	u64 b64;
	
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
	
	//����С1��ѭ�س���,����ʱ1�ֺ�������ۼ�0ֵʱ��
	p8=(u8 *)(ADDR_AFN0DF57F60_S_Timer);//36 �ܼ��������С�й������ڳ�����ɺ�����붨ʱ��
	#if (((Project/100)!=2)&&((Project/100)!=4))//���Ǽ�����,�����ն�
	if(Comm_Ram->Factory!=0x55)
	{//�ڹ���ģʽ,RS485-1������ά����
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			p8[0]=60;//�ڳ�����ɺ������ʱ60��
			return;
		}
	}
	//RS485-2
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			p8[0]=60;//�ڳ�����ɺ������ʱ60��
			return;
		}
	#endif
	#if (((Project/100)==2)||((Project/100)==4))//������,�����ն�
/*
	i=MRR(ADDR_AFN04F37,1);//�ն˼���ͨ�Ŷ˿ں� BIN  1 
	//RS485-1
	if(Comm_Ram->Factory!=0x55)
	{//�ڹ���ģʽ,RS485-1������ά����
		if(i!=2)
		{
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			if(UARTCtrl->RMComplete==0x0)
			{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
				p8[0]=60;//�ڳ�����ɺ������ʱ60��
				return;
			}
		}
	}
	//RS485-2
	if(i!=3)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			p8[0]=60;//�ڳ�����ɺ������ʱ60��
			return;
		}
	}
*/
	if(Comm_Ram->Factory!=0x55)
	{//�ڹ���ģʽ,RS485-1������ά����
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		if(UARTCtrl->RMComplete==0x0)
		{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
			p8[0]=60;//�ڳ�����ɺ������ʱ60��
			return;
		}
	}
	//RS485-2
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	if(UARTCtrl->RMComplete==0x0)
	{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
		p8[0]=60;//�ڳ�����ɺ������ʱ60��
		return;
	}
	#endif

	//�ڲ�����
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	if(UARTCtrl->RMComplete==0x0)
	{//1�ֽ� 16 D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
		p8[0]=60;//�ڳ�����ɺ������ʱ60��
		return;
	}
	if(p8[0]!=0x0)
	{//�������ʱ��ʱû��
		return;
	}
	
	Pn=Terminal_Ram->I2SUBTask4_2;
	if(Pn>=AFN0DF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF57F60_SOUR);
	t=p8[Pn];
	if(t!=0x0)
	{//�ָ���
		p8[Pn]=0;//��ָ���
		MR(ADDR_DATABUFF,ADDR_AFN0DF57_SOUR+(12*Pn),12);
		MR(ADDR_DATABUFF+12,ADDR_AFN0DF60_SOUR+(12*Pn),12);
		x=GetClass1DataAddr(17,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen
		x=MRR(x,2);
		a64=Data02_kW(x&0xefff);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		//�����Ƚ�
		y=MRR(ADDR_DATABUFF,2);
		b64=Data02_kW(y&0xefff);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		if(a64>b64)
		{
			MWR(x,ADDR_DATABUFF,2);
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,3);
		}
		//����С
		i=MRR(ADDR_DATABUFF+7,3);
		if(i==0x0)
		{//ԭû��Сֵ
			if(a64!=0x0)
			{//��ʼֵ��Ϊ��Сֵ
				MWR(x,ADDR_DATABUFF+5,2);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
			}
		}
		else
		{
			y=MRR(ADDR_DATABUFF+5,2);
			b64=Data02_kW(y&0xefff);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if(a64<b64)
			{
				MWR(x,ADDR_DATABUFF+5,2);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,3);
			}
		}
		//�����Ƚ�
		y=MRR(ADDR_DATABUFF+12,2);
		b64=Data02_kW(y&0xefff);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		if(a64>b64)
		{
			MWR(x,ADDR_DATABUFF+12,2);
			MR(ADDR_DATABUFF+12+2,ADDR_TYMDHMS+1,3);
		}
		//����С
		i=MRR(ADDR_DATABUFF+12+7,3);
		if(i==0x0)
		{//ԭû��Сֵ
			if(a64!=0x0)
			{//��ʼֵ��Ϊ��Сֵ
				MWR(x,ADDR_DATABUFF+12+5,2);
				MR(ADDR_DATABUFF+12+7,ADDR_TYMDHMS+1,3);
			}
		}
		else
		{
			y=MRR(ADDR_DATABUFF+12+5,2);
			b64=Data02_kW(y&0xefff);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if(a64<b64)
			{
				MWR(x,ADDR_DATABUFF+12+5,2);
				MR(ADDR_DATABUFF+12+7,ADDR_TYMDHMS+1,3);
			}
		}
		//=0ʱ��
		if(a64==0x0)
		{
			i=MRR(ADDR_DATABUFF+10,2);
			i+=t;
			MWR(i,ADDR_DATABUFF+10,2);
			i=MRR(ADDR_DATABUFF+12+10,2);
			i+=t;
			MWR(i,ADDR_DATABUFF+12+10,2);
		}
		MW(ADDR_DATABUFF,ADDR_AFN0DF57_SOUR+(12*Pn),12);
		MW(ADDR_DATABUFF+12,ADDR_AFN0DF60_SOUR+(12*Pn),12);
	}
	
	Pn++;
	Terminal_Ram->I2SUBTask4_2=Pn;
#endif
}

void AFN0DF65F66_Computer(void)//�ն�ͳ��.�ܼ��鳬���ʶ�ֵ�µ�������ֵ�����ۼ�ʱ���ۼƵ�����
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 a;
	u32 b;
	u64 a64;
	u64 b64;
	u32 Pn;
	u32 t;
	u8 * p8;
	

	Pn=Terminal_Ram->I2SUBTask4_8;
	if(Pn>=MaxTotalAddGroup)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF65F66_SOUR+(9*Pn));
	t=p8[0];
	if(t!=0x0)
	{//�ָ��¼Ӽ���1=0
		p8[0]=0;
		//�����ʶ�ֵ
		a=0;
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		for(i=0;i<4;i++)
		{
			if(p8[8]!=0x0)
			{//����ֵ��־0=û��,1=����ֵ
				a=1;
				break;
			}
			p8+=(Terminal_Ctrl_Pn_Len*MaxTotalAddGroup);
		}
		if(a==0x0)
		{//û����ֵ
			MC(0,ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);//�幦�س���ֵ�����ڼ����ԭ�ܼ��й�������
		}
		else
		{//����ֵ
			a=MRR(ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);
			b=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			b&=0xefffffff;
			MWR(b,ADDR_AFN0DF65F66_SOUR+(9*Pn)+1,4);
			if(a!=0x0)
			{
				a64=Data03_kWh(a);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				b64=Data03_kWh(b);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				if(b64>=a64)
				{
					b64-=a64;
				}
				else
				{
					b64=0;
				}
				a=MRR(ADDR_AFN0DF65_SOUR+(6*Pn)+2,4);
				a64=Data03_kWh(a);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				a64+=b64;
				a=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
				MWR(a,ADDR_AFN0DF65_SOUR+(6*Pn)+2,4);
				//ʱ��
				a=MRR(ADDR_AFN0DF65_SOUR+(6*Pn),2);
				a+=t;
				MWR(a,ADDR_AFN0DF65_SOUR+(6*Pn),2);
			}
		}
		//���µ�������ֵ
		p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8[0]>101)
		{//����״̬,0=�ϵ�򲻿���,1=,2=,3=����...,100=����,101=��բ׼��,102=��բ
			a=MRR(ADDR_AFN0DF65F66_SOUR+(9*Pn)+5,4);
			b=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			b&=0xefffffff;
			MWR(b,ADDR_AFN0DF65F66_SOUR+(9*Pn)+5,4);
			if(a!=0x0)
			{
				a64=Data03_kWh(a);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				b64=Data03_kWh(b);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				if(b64>=a64)
				{
					b64-=a64;
				}
				else
				{
					b64=0;
				}
				a=MRR(ADDR_AFN0DF66_SOUR+(6*Pn)+2,4);
				a64=Data03_kWh(a);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				a64+=b64;
				a=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
				MWR(a,ADDR_AFN0DF66_SOUR+(6*Pn)+2,4);
			}
			//ʱ��
			a=MRR(ADDR_AFN0DF66_SOUR+(6*Pn),2);
			a+=t;
			MWR(a,ADDR_AFN0DF66_SOUR+(6*Pn),2);
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_8=Pn;
#endif
}




void AFN0DF116F117F118_Computer(u32 ADDR_Current,u32 ADDR_Sour)//�ն�ͳ��.��ѹг�������������ֵ������ʱ��
{
#if MeterHarmonic==1//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	
	for(Pn=0;Pn<AFN0DF116MaxPn;Pn++)
	{
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_Current+2+(2*n),2);
			if(x!=0xeeee)
			{
				y=MRR(ADDR_Sour+(6*n),2);
				if(x>y)
				{
					MWR(x,ADDR_Sour+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
				}
			}
		}
		x=MRR(ADDR_Current,2);//�ܻ���
		if(x!=0xeeee)
		{
			y=MRR(ADDR_Sour+(6*n),2);
			if(x>y)
			{
				MWR(x,ADDR_Sour+(6*n),2);
				MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
			}
		}
		ADDR_Current+=1+222;
		ADDR_Sour+=114;
	}
#endif
#endif
}
void AFN0DF113F114F115_Computer(u32 ADDR_Current,u32 ADDR_Sour)//�ն�ͳ��.г�����������ֵ������ʱ��
{
#if MeterHarmonic==1//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	u32 T;
	
	for(Pn=0;Pn<AFN0DF113MaxPn;Pn++)
	{
		T=0;//�ܻ���
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_Current+(2*n),2);
			if(x!=0xeeee)
			{
				y=MRR(ADDR_Sour+(6*n),2);
				if(x>y)
				{
					MWR(x,ADDR_Sour+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
				}
				x=bcd_hex(x);
				T+=(x*x);
			}
		}
		T=sqrt4_2(T);//��ƽ��
		x=hex_bcd(T);
		y=MRR(ADDR_Sour+(6*n),2);
		if(x>y)
		{
			MWR(x,ADDR_Sour+(6*n),2);
			MW(ADDR_TYMDHMS+1,ADDR_Sour+(6*n)+2,4);
		}
		ADDR_Current+=1+216;
		ADDR_Sour+=114;
	}
#endif
#endif
}
void AFN0DF121F122F123(u32 ADDR_UH,u32 ADDR_IH,u32 ADDR_UHMax,u32 ADDR_IHMax,u32 ADDR_Time,u32 ABC)//�ն�ͳ��.��г��Խ����ͳ��
{
#if MeterHarmonic==1//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 i;
	u32 t;
	u32 Pn;
	u32 n;
	u32 x;
	u32 y;
	u32 TH;//�����ܻ���
	u32 ufs;//��ѹг��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
	u32 ifs;//����г��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
	u8 * p8;
	
	Pn=Terminal_Ram->I2SUBTask4_10;
	if(Pn>=AFN0CF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn));
	i=p8[0]+(p8[1]<<8);
	t=i/60;
	if(t!=0x0)
	{//�ּӼ���!=0
		ADDR_UH+=((1+222)*Pn);//��ǰ��ѹ�����ʵ�ַ
		ADDR_IH+=((1+216)*Pn);//��ǰг��������ַ
		ADDR_Time+=(77*Pn);//г��Խ����ͳ�����ݵ�ַ
		ADDR_UHMax+=(144*Pn);//��ѹ���������ֵ������ʱ���ַ
		ADDR_IHMax+=(144*Pn);//г���������ֵ������ʱ���ַ
		
		MWR(19,ADDR_Time,1);//г������
	//��ѹ������
		ufs=0;//��ѹг��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
		//2-19��
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_UH+2+(2*n),2);//ȡ��ǰ������
			if(x!=0xeeee)
			{
				y=MRR(ADDR_UHMax+(6*n),2);//ȡԭ���������ֵ
				if(x>y)
				{//��ǰ������>ԭ������
					MWR(x,ADDR_UHMax+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_UHMax+(6*n)+2,4);
				}
				//�ִ�����
				if((n%2)==0)
				{//ż��
					y=MRR(ADDR_AFN04F60+6+n,2);//ȡ�ִ�ż�κ���������
				}
				else
				{//���
					y=MRR(ADDR_AFN04F60+6+18+n-1,2);//ȡ�ִ���κ���������
				}
				//���ִ�����
				if((n%2)==0)
				{//ż��
					i=MRR(ADDR_AFN04F60+4,2);//ȡż�κ���������
				}
				else
				{//���
					i=MRR(ADDR_AFN04F60+2,2);//ȡ��κ���������
				}
				if(i>y)
				{//�����޽ϴ���
					y=i;
				}
				if(y==0)
				{//����=0û����
					continue;
				}
				if(x>=y)
				{//������
					ufs|=2<<n;//��ѹг��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
					i=MRR(ADDR_Time+3+(n*2),2);
					i+=t;
					MWR(i,ADDR_Time+3+(n*2),2);
				}
			}
		}
		//�ܻ���
		x=MRR(ADDR_UH,2);
		if(x!=0xeeee)
		{
			y=MRR(ADDR_UHMax+(6*n),2);
			if(x>y)
			{//��ǰ������>ԭ������
				MWR(x,ADDR_UHMax+(6*n),2);
				MW(ADDR_TYMDHMS+1,ADDR_UHMax+(6*n)+2,4);
			}
			y=MRR(ADDR_AFN04F60,2);//ȡ�ܻ��京��������
			if(y!=0)
			{
				if(x>=y)
				{//�ܻ��䳬����
					ufs|=1;//��ѹг��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
					i=MRR(ADDR_Time+1,2);
					i+=t;
					MWR(i,ADDR_Time+1,2);
				}
			}
		}
	//г������
		TH=0;//�����ܻ���
		ifs=0;//����г��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
		//2-19��
		for(n=0;n<18;n++)
		{
			x=MRR(ADDR_IH+(2*n),2);//ȡ��ǰг������
			if(x!=0xeeee)
			{
				i=x;
				i=bcd_hex(i);
				TH+=(i*i);
				y=MRR(ADDR_IHMax+(6*n),2);//ȡԭг���������ֵ
				if(x>y)
				{//��ǰг������>ԭг������
					MWR(x,ADDR_IHMax+(6*n),2);
					MW(ADDR_TYMDHMS+1,ADDR_IHMax+(6*n)+2,4);
				}
				if((n%2)==0)
				{//ż��
					y=MRR(ADDR_AFN04F60+42+2+n,2);//ȡż��г����������
				}
				else
				{//���
					y=MRR(ADDR_AFN04F60+42+2+18+n-1,2);//ȡ���г����������
				}
				if(y==0)
				{//����=0û����
					continue;
				}
				if(x>=y)
				{//������
					ifs|=2<<n;//����г��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
					i=MRR(ADDR_Time+39+2+(n*2),2);
					i+=t;
					MWR(i,ADDR_Time+39+2+(n*2),2);
				}
			}
		}
		//г�������ܻ���
		TH=sqrt4_2(TH);//��ƽ��
		TH=hex_bcd(TH);
		y=MRR(ADDR_IHMax+(6*n),2);
		if(TH>y)
		{//��ǰг�������ܻ���>ԭг�������ܻ���
			MWR(TH,ADDR_IHMax+(6*n),2);
			MW(ADDR_TYMDHMS+1,ADDR_IHMax+(6*n)+2,4);
		}
		y=MRR(ADDR_AFN04F60+42,2);//ȡг�������ܻ�������
		if(y!=0)
		{
			if(TH>=y)
			{//г�������ܻ��䳬����
				ifs|=1;//����г��Խ�ޱ�־;D0-D18�ֱ��ʾ�ܻ���,2��-19��
				i=MRR(ADDR_Time+39,2);
				i+=t;
				MWR(i,ADDR_Time+39,2);
			}
		}
	//�¼�
	#if ERC15==1//ERC15г��Խ�޸澯
		if(ERC_Event_Set(15)==0)//����¼���¼����,����0=����¼,1=��¼
		{
			MC(0,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2,18);//��г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
		}
		else
		{
			//��ѹԽ���¼��ָ�
			x=MRR(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2+(ABC*6),3);//ԭ��ѹг��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
			i=ufs^x;
			i&=x;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//����Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
				MWR(Pn+1,ADDR_DATABUFF+7,2);//D15����/ֹ��־,D11��D0��pn���������1��2048��
				p8[9]=1<<ABC;//�쳣��־
				MWR(i,ADDR_DATABUFF+10,3);//г��Խ�ޱ�־
				MR(ADDR_DATABUFF+13,ADDR_UH,38);//Խ��ʱֵ
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
			//����Խ���¼��ָ�
			y=MRR(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+5+(ABC*6),3);//ԭ����г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
			i=ifs^y;
			i&=y;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//����Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
				MWR(Pn+1,ADDR_DATABUFF+7,2);//D15����/ֹ��־,D11��D0��pn���������1��2048��
				p8[9]=(1<<ABC)+0x80;//�쳣��־
				MWR(i,ADDR_DATABUFF+10,3);//г��Խ�ޱ�־
				MWR(TH,ADDR_DATABUFF+13,2);//Խ��ʱ�����ܻ���ֵ
				MR(ADDR_DATABUFF+15,ADDR_IH,36);//Խ��ʱֵ
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
			//��ѹԽ���¼�����
			i=ufs^x;
			i&=ufs;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//����Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
				MWR((Pn+1)+0x8000,ADDR_DATABUFF+7,2);//D15����/ֹ��־,D11��D0��pn���������1��2048��
				p8[9]=1<<ABC;//�쳣��־
				MWR(i,ADDR_DATABUFF+10,3);//г��Խ�ޱ�־
				MR(ADDR_DATABUFF+13,ADDR_UH,38);//Խ��ʱֵ
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
			//����Խ���¼�����
			i=ifs^y;
			i&=ifs;
			if(i!=0)
			{
				p8=(u8*)(ADDR_DATABUFF);
				p8[0]=15;//ERC=15
				p8[1]=49;//����Le
				MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
				MWR((Pn+1)+0x8000,ADDR_DATABUFF+7,2);//D15����/ֹ��־,D11��D0��pn���������1��2048��
				p8[9]=(1<<ABC)+0x80;//�쳣��־
				MWR(i,ADDR_DATABUFF+10,3);//г��Խ�ޱ�־
				MWR(TH,ADDR_DATABUFF+13,2);//Խ��ʱ�����ܻ���ֵ
				MR(ADDR_DATABUFF+15,ADDR_IH,36);//Խ��ʱֵ
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			}
			
			//��Խ�ޱ�־
			MWR(ufs,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+2+(ABC*6),3);//��ѹг��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
			MWR(ifs,ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn)+5+(ABC*6),3);//����г��Խ�ޱ�־D0-D18�ֱ��ʾ�ܻ���,2��-19��
		}
	#endif//#if ERC15==1//ERC15г��Խ�޸澯	
	}
#endif
#endif
}
void AFN0DF121F122F123_Computer(void)//�ն�ͳ��.г��Խ��
{
#if MeterHarmonic==1//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 i;
	u32 Pn;
	u8 *p8;
	
	Pn=Terminal_Ram->I2SUBTask4_10;
	if(Pn>=AFN0CF57MaxPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_ERC15_SOUR+(LEN_ERC15_SOUR*Pn));
	i=p8[0]+(p8[1]<<8);
	if((i/60)!=0x0)
	{//�ּӼ���!=0
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(0*38),ADDR_AFN0CF57+1+(3*36),ADDR_AFN0DF116_SOUR,ADDR_AFN0DF113_SOUR,ADDR_AFN0DF121_SOUR,0);//�ն�ͳ��.A��г��Խ����ͳ��
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(1*38),ADDR_AFN0CF57+1+(4*36),ADDR_AFN0DF117_SOUR,ADDR_AFN0DF114_SOUR,ADDR_AFN0DF122_SOUR,1);//�ն�ͳ��.B��г��Խ����ͳ��
		AFN0DF121F122F123(ADDR_AFN0CF58+1+(2*38),ADDR_AFN0CF57+1+(5*36),ADDR_AFN0DF118_SOUR,ADDR_AFN0DF115_SOUR,ADDR_AFN0DF123_SOUR,2);//�ն�ͳ��.C��г��Խ����ͳ��
		i%=60;
		p8[0]=i;
		p8[1]=i>>8;
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_10=Pn;
#endif
#endif
}



void AFN0DF129F130_Computer(void)//�ն�ͳ��.ֱ��ģ����Խ��
{
#if MaxDCPn!=0//���ֱ��ģ������������
	u32 i;
	s64 x;
	s64 y;
	s64 z;
	u32 t;
	u32 Pn;
	u32 f;//��ǰԽ�ޱ�־,B0=Խ����,B1=Խ����
	u8 *p8;
	
	Pn=Terminal_Ram->I2SUBTask4_11;//�ն�ͳ�����ݼ�������.ֱ��ģ����Խ��
	if(Pn>=MaxDCPn)
	{
		Pn=0;
	}
	p8=(u8 *)(ADDR_AFN0DF129F130_SOUR+(LEN_AFN0DF129F130_SOUR*Pn));
	t=p8[0];
	if(t!=0x0)
	{//�ָ��¼Ӽ���!=0
		p8[0]=0;
		z=MRR(ADDR_AFN0CF73+(2*Pn),2);//��ǰֱ��ģ����	���ݸ�ʽ02	    2
		if(z!=0xeeee)
		{
			f=0;//��ǰԽ�ޱ�־,B0=Խ����,B1=Խ����
			x=MRR(ADDR_AFN04F82+(4*Pn),2);//ֱ��ģ��������	���ݸ�ʽ02	    2
			y=MRR(ADDR_AFN04F82+(4*Pn)+2,2);//ֱ��ģ��������	���ݸ�ʽ02	    2
			x=Data02_kW((u64)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			y=Data02_kW((u64)y);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			z=Data02_kW((u64)z);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if(z>=x)
			{//��ǰֱ��ģ����>=ֱ��ģ��������
				f=1;//��ǰԽ�ޱ�־,B0=Խ����,B1=Խ����
				//ֱ��ģ����Խ�������ۼ�ʱ��	BIN	min	2
				i=MRR(ADDR_AFN0DF129_SOUR+(14*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn),2);
				//ֱ��ģ����Խ�������ۼ�ʱ��	BIN	min	2
				i=MRR(ADDR_AFN0DF130_SOUR+(14*Pn),2);
				i+=t;
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn),2);
			}
			else
			{
				if(z<y)
				{//��ǰֱ��ģ����<ֱ��ģ��������
					f=2;//��ǰԽ�ޱ�־,B0=Խ����,B1=Խ����
					//ֱ��ģ����Խ�������ۼ�ʱ��	BIN	min	2
					i=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+2,2);
					//ֱ��ģ����Խ�������ۼ�ʱ��	BIN	min	2
					i=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+2,2);
					i+=t;
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+2,2);
				}
			}
			//
			i=MRR(ADDR_AFN0CF73+(2*Pn),2);//��ǰֱ��ģ����	���ݸ�ʽ02	    2
			//�����ֵ
			if(MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+6,3)==0)
			{//ԭ���ֵ����ʱ��=0,�������ֵ
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+6,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				x=Data02_kW((u64)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
				if(z>x)
				{//��ǰ>ԭ���ֵ
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+6,3);
				}
			}
			//����Сֵ
			if(MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+11,3)==0)
			{//ԭ��Сֵ����ʱ��=0,������Сֵ
				MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+11,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//ԭֱ��ģ������Сֵ	���ݸ�ʽ02	    2
				x=Data02_kW((u64)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
				if(z<x)
				{//��ǰ<ԭ��Сֵ
					MWR(i,ADDR_AFN0DF129_SOUR+(14*Pn)+9,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF129_SOUR+(14*Pn)+11,3);
				}
			}
			//�����ֵ
			if(MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+6,3)==0)
			{//ԭ���ֵ����ʱ��=0,�������ֵ
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+6,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				x=Data02_kW((u64)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
				if(z>x)
				{//��ǰ>ԭ���ֵ
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+4,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+6,3);
				}
			}
			//����Сֵ
			if(MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+11,3)==0)
			{//ԭ��Сֵ����ʱ��=0,������Сֵ
				MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
				MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+11,3);
			}
			else
			{
				x=MRR(ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//ԭֱ��ģ������Сֵ	���ݸ�ʽ02	    2
				x=Data02_kW((u64)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
				if(z<x)
				{//��ǰ<ԭ��Сֵ
					MWR(i,ADDR_AFN0DF130_SOUR+(14*Pn)+9,2);//ԭֱ��ģ�������ֵ	���ݸ�ʽ02	    2
					MW(ADDR_TYMDHMS+1,ADDR_AFN0DF130_SOUR+(14*Pn)+11,3);
				}
			}
			//�¼�
			if(ERC_Event_Set(16)==0)//����¼���¼����,����0=����¼,1=��¼
			{
				p8[2]=0;//�¼��Ѽ�¼��־,B0=Խ����,B1=Խ����
			}
			else
			{
				//�ָ�
				t=f^p8[2];
				t&=(~f);
				if(t!=0)
				{
					ERC16_Event(Pn+1,0,t,i);//ֱ��ģ����Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=Խ�ޱ�־,dc=��ǰֱ��ģ����
				}
				//����
				t=f^p8[2];
				t&=f;
				if(t!=0)
				{
					ERC16_Event(Pn+1,1,t,i);//ֱ��ģ����Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=Խ�ޱ�־,dc=��ǰֱ��ģ����
				}
				p8[2]=f;//��ǰ���ޱ�־
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_11=Pn;
#endif
}

void AFN0DF124F125F126(u32 Pn,u32 ADDR_SOUR,u32 ABC)//��ѹг����ѹ�����ʡ�г������95%����ֵ
{
#if MeterHarmonic!=0//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Count;
	u8 *p8;
	u16 *p16;
	
	//ȡԭʼֵ
	MR(ADDR_DATABUFF,ADDR_SOUR+(LEN_AFN0DF124_SOUR*Pn),LEN_AFN0DF124_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	if(p8[0]!=i)//ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96��;ÿСʱ���ݱ��浽FLASH
	{
		p8[0]=i;//�ϴμ����ʵʱ��0,15,30,45��
		Count=MRR(ADDR_DATABUFF+1,2);
		Count++;//�����������
		MWR(Count,ADDR_DATABUFF+1,2);
		p8[3]=19;//г������N=19
		//�ܻ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
		x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+1+(38*ABC),2);//ȡ��ǰ�ܻ����ѹ������
		if(x!=0xeeee)
		{
			p16=(u16*)(ADDR_DATABUFF+4+80+8);
			for(i=5;i!=0;i--)
			{
				if(x>p16[0])
				{
					MR(ADDR_DATABUFF+4+80,ADDR_DATABUFF+4+80+2,(2*i));
					p16[0]=x;
					break;
				}
				p16--;
			}
		}
		//��ż�λ����ѹ�����ʼ���
		y=0;//ż�λ����ѹ������=0
		z=0;//��λ����ѹ������=0
		for(i=0;i<9;i++)
		{
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(4*i),2);//ż�ε�ѹ������
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				y+=(x*x);
			}
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(4*i)+2,2);//��ε�ѹ������
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				z+=(x*x);
			}
		}
		y=sqrt4_2(y);//��ƽ��
		y=hex_bcd(y);
		z=sqrt4_2(z);//��ƽ��
		z=hex_bcd(z);
		//��λ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
			p16=(u16*)(ADDR_DATABUFF+4+90+8);
			for(i=5;i!=0;i--)
			{
				if(z>p16[0])
				{
					MR(ADDR_DATABUFF+4+90,ADDR_DATABUFF+4+90+2,(2*i));
					p16[0]=z;
					break;
				}
				p16--;
			}
		//ż�λ����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
			p16=(u16*)(ADDR_DATABUFF+4+100+8);
			for(i=5;i!=0;i--)
			{
				if(y>p16[0])
				{
					MR(ADDR_DATABUFF+4+100,ADDR_DATABUFF+4+100+2,(2*i));
					p16[0]=y;
					break;
				}
				p16--;
			}
		//2-19��г����ѹ������95%����ֵ(��5��ֵ-��1��ֵ)
		for(y=0;y<18;y++)
		{
			x=MRR(ADDR_AFN0CF58+(LEN_AFN0CF58*Pn)+3+(38*ABC)+(2*y),2);//ȡ��ѹ������
			if(x!=0xeeee)
			{
				p16=(u16*)(ADDR_DATABUFF+4+110+(10*y)+8);
				for(i=5;i!=0;i--)
				{
					if(x>p16[0])
					{
						MR(ADDR_DATABUFF+4+110+(10*y),ADDR_DATABUFF+4+110+(10*y)+2,(2*i));
						p16[0]=x;
						break;
					}
					p16--;
				}
			}
		}
		//�ܻ����������
		y=0;//�ܻ������=0
		for(i=0;i<18;i++)
		{
			x=MRR(ADDR_AFN0CF57+(LEN_AFN0CF57*Pn)+109+(36*ABC)+(2*i),2);//ȡ��г������
			if(x!=0xeeee)
			{
				x=bcd_hex(x);
				y+=(x*x);
			}
		}
		y=sqrt4_2(y);//��ƽ��
		y=hex_bcd(y);
		//�ܻ������95%����ֵ(��5��ֵ-��1��ֵ)
			p16=(u16*)(ADDR_DATABUFF+4+290+8);
			for(i=5;i!=0;i--)
			{
				if(y>p16[0])
				{
					MR(ADDR_DATABUFF+4+290,ADDR_DATABUFF+4+290+2,(2*i));
					p16[0]=y;
					break;
				}
				p16--;
			}
		//2-19��г������95%����ֵ(��5��ֵ-��1��ֵ)
		for(y=0;y<18;y++)
		{
			x=MRR(ADDR_AFN0CF57+(LEN_AFN0CF57*Pn)+109+(36*ABC)+(2*y),2);//ȡг������
			if(x!=0xeeee)
			{
				p16=(u16*)(ADDR_DATABUFF+4+300+(10*y)+8);
				for(i=5;i!=0;i--)
				{
					if(x>p16[0])
					{
						MR(ADDR_DATABUFF+4+300+(10*y),ADDR_DATABUFF+4+300+(10*y)+2,(2*i));
						p16[0]=x;
						break;
					}
					p16--;
				}
			}
		}
		//��ǰ����ֵ
		Count*=5;//ȥ5%��ֵ
		Count/=100;
		if(Count>4)
		{
			Count=4;
		}
		for(i=0;i<40;i++)
		{
			MR(ADDR_DATABUFF+4+(2*i),(ADDR_DATABUFF+84+(10*i)+8)-(Count*2),2);
		}
		//�洢
		MW(ADDR_DATABUFF,ADDR_SOUR+(LEN_AFN0DF124_SOUR*Pn),LEN_AFN0DF124_SOUR);
	}
#endif
#endif
}

void AFN0DF124F125F126_Computer(void)//�ն�ͳ��.�ն����ѹг����ѹ�����ʡ�г������95%����ֵ
{
#if MeterHarmonic==1//г������0=û,1=��
#if (IC_SDRAM!=0)
	u32 Pn;
	
	Pn=Terminal_Ram->I2SUBTask4_14;//�ն�ͳ�����ݼ�������.�ն����ѹг����ѹ�����ʡ�г������95%����ֵ
	if(Pn>=AFN0DF124MaxPn)
	{
		Pn=0;
	}
	
	AFN0DF124F125F126(Pn,ADDR_AFN0DF124_SOUR,0);//A��ѹг����ѹ�����ʡ�г������95%����ֵ
	AFN0DF124F125F126(Pn,ADDR_AFN0DF125_SOUR,1);//B��ѹг����ѹ�����ʡ�г������95%����ֵ
	AFN0DF124F125F126(Pn,ADDR_AFN0DF126_SOUR,2);//C��ѹг����ѹ�����ʡ�г������95%����ֵ
	
	Pn++;
	Terminal_Ram->I2SUBTask4_14=Pn;
#endif
#endif
}


void AFN0DF211_Computer(void)//�ն�ͳ��.�������ѹ��������ƽ���95%����ֵ
{
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 Count;
	u32 Pn;
	u8 *p8;
	u16 *p16;
	
	Pn=Terminal_Ram->I2SUBTask4_12;//�ն�ͳ�����ݼ�������.�������ѹ��������ƽ���95%����ֵ
	if(Pn>=AFN0DF211MaxPn)
	{
		Pn=0;
	}
	//ȡԭʼֵ
	MR(ADDR_DATABUFF,ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*Pn),LEN_AFN0DF211_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	if(p8[0]!=i)//ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96��;ÿСʱ���ݱ��浽FLASH
	{
		p8[0]=i;//�ϴμ����ʵʱ��0,15,30,45��
		p16=(u16*)(ADDR_DATABUFF+2);
		Count=p16[0];
		Count++;//�����������
		p16[0]=Count;
		//������ƽ������
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+10,2);//��ǰ������ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
		p16=(u16*)(ADDR_DATABUFF+8+8);
		for(i=5;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8,ADDR_DATABUFF+8+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//��ѹ��ƽ������
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+12,2);//��ǰ��ѹ��ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
		p16=(u16*)(ADDR_DATABUFF+8+10+8);
		for(i=5;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8+10,ADDR_DATABUFF+8+10+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//������ѹ��ǰ��ƽ�����ֵ
		Count*=5;//ȥ5%��ֵ
		Count/=100;
		if(Count>4)
		{
			Count=4;
		}
		MR(ADDR_DATABUFF+4,(ADDR_DATABUFF+8+8)-(Count*2),2);
		MR(ADDR_DATABUFF+6,(ADDR_DATABUFF+8+10+8)-(Count*2),2);
		//�洢
		MW(ADDR_DATABUFF,ADDR_AFN0DF211_SOUR+(LEN_AFN0DF211_SOUR*Pn),LEN_AFN0DF211_SOUR);
	}

	Pn++;
	Terminal_Ram->I2SUBTask4_12=Pn;
#endif
}

void AFN0DF212_Computer(void)//�ն�ͳ��.�������ѹ��������ƽ���95%����ֵ
{
#if (IC_SDRAM!=0)
	u32 i;
	u32 x;
	u32 Count;
	u32 Pn;
	u8 *p8;
	u16 *p16;
	
	Pn=Terminal_Ram->I2SUBTask4_13;//�ն�ͳ�����ݼ�������.�������ѹ��������ƽ���95%����ֵ
	if(Pn>=AFN0DF212MaxPn)
	{
		Pn=0;
	}

	//ȡԭʼֵ
	MR(ADDR_DATABUFF,ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*Pn),LEN_AFN0DF212_SOUR);
	p8=(u8*)(ADDR_DATABUFF);
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=15;
	i*=15;
	i=hex_bcd(i);
	if(p8[0]!=i)//ʵʱ�ӷ�;ÿ0,15,30,45�ּ���ÿ�����ȡ96��;ÿСʱ���ݱ��浽FLASH
	{
		p8[0]=i;//�ϴμ����ʵʱ��0,15,30,45��
		p16=(u16*)(ADDR_DATABUFF+2);
		Count=p16[0];
		Count++;//�����������
		p16[0]=Count;
		//������ƽ������
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+10,2);//��ǰ������ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
		p16=(u16*)(ADDR_DATABUFF+8+(149*2));
		for(i=150;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8,ADDR_DATABUFF+8+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//��ѹ��ƽ������
		x=MRR(ADDR_AFN0DF28F36_SOUR+(LEN_AFN0DF28F36_SOUR*Pn)+12,2);//��ǰ��ѹ��ƽ���(�ϵ�ͳ�ʼ��ʱ��Ϊ0)
		p16=(u16*)(ADDR_DATABUFF+8+300+(149*2));
		for(i=150;i!=0;i--)
		{
			if(x>p16[0])
			{
				MR(ADDR_DATABUFF+8+300,ADDR_DATABUFF+8+300+2,(2*i));
				p16[0]=x;
				break;
			}
			p16--;
		}
		//������ѹ��ǰ��ƽ�����ֵ
		Count*=5;//ȥ5%��ֵ
		Count/=100;
		if(Count>149)
		{
			Count=149;
		}
		MR(ADDR_DATABUFF+4,(ADDR_DATABUFF+8+(149*2))-(Count*2),2);
		MR(ADDR_DATABUFF+6,(ADDR_DATABUFF+8+300+(149*2))-(Count*2),2);

		MW(ADDR_DATABUFF,ADDR_AFN0DF212_SOUR+(LEN_AFN0DF212_SOUR*Pn),LEN_AFN0DF212_SOUR);
	}

	Pn++;
	Terminal_Ram->I2SUBTask4_13=Pn;
#endif
}

void HOURFLASH(void)//ÿСʱRAM���ݱ��浽FLASH
{
#if (IC_SDRAM!=0)
	u8 *p8;
	
	p8=(u8*)(ADDR_HOURFLASH_RAM_START);
	if(p8[0]!=Comm_Ram->TYMDHMS[2])
	{
		p8[0]=Comm_Ram->TYMDHMS[2];
		MW(ADDR_HOURFLASH_RAM_START,ADDR_HOURFLASH_START,LEN_HOURFLASH_RAM);
	}
#endif
}





void Terminal_Statistic(void)//�ն�ͳ�Ƽ���,����������
{

	switch(Terminal_Ram->I2SUBTask4)
	{
		case 0:
			AFN0DF25F33_Computer();//�ն�ͳ��.�����ܼ���������й����ʼ�����ʱ��,�й�����Ϊ��ʱ��
			break;
		case 1:
			AFN0DF27F35_Computer();//�ն�ͳ��.��ѹͳ������
			break;
		case 2:
			AFN0DF29F37_Computer();//�ն�ͳ��.����ͳ������
			break;
		case 3:
			AFN0DF28F36_Computer();//�ն�ͳ��.��ѹ������ƽ��
			break;
		case 4:
			AFN0DF30F38_Computer();//�ն�ͳ��.���ڹ���Խ���ۼ�ʱ��
			break;
		case 5:
			AFN0DF31F39_Computer();//�ն�ͳ��.������
			break;
		case 6:
			AFN0DF43F44_Computer();//�ն�ͳ��.�������������ۼ�ʱ��
			break;
		case 7:
		#if ERC22==1
			ERC22_Event();//�й��ܵ������Խ�޼�����¼���¼
		#endif
			break;
		case 8:
			AFN0DF57F60_Computer();//�ն�ͳ��.�����ܼ��������С�й����ʼ��䷢��ʱ��,�й�����Ϊ���ۼ�ʱ��		
			break;
		case 9:
			AFN0DF65F66_Computer();//�ն�ͳ��.�ܼ��鳬���ʶ�ֵ�µ�������ֵ�����ۼ�ʱ���ۼƵ�����
			break;
		case 10:
			AFN0DF121F122F123_Computer();//�ն�ͳ��.г��Խ��
			break;
		case 11:
			AFN0DF129F130_Computer();//�ն�ͳ��.ֱ��ģ����Խ��
			break;
		case 12:
			AFN0DF124F125F126_Computer();//�ն�ͳ��.�ն����ѹг����ѹ�����ʡ�г������95%����ֵ
			break;
		case 13:
			AFN0DF211_Computer();//�ն�ͳ��.�������ѹ��������ƽ���95%����ֵ
			break;
		case 14:
			AFN0DF212_Computer();//�ն�ͳ��.�������ѹ��������ƽ���95%����ֵ
			break;
		case 15:
			HOURFLASH();//ÿСʱRAM���ݱ��浽FLASH
			break;
		case 16:
			AFN0DF26F34_Computer();//F26F34�����ܼ������������������ʱ��
		default:
			Terminal_Ram->I2SUBTask4=0;
			return;
	}
	Terminal_Ram->I2SUBTask4++;
}

















