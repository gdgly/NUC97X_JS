

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_Voltage_Qualification_Rate.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_GET_4100.h"


void CALC_Voltage_Qualification_Rate(void)//��ѹ�ϸ��ʼ���
{
	u32 i;
	u32 a;
	u32 b;
	u32 c;
	u32 f;//��ǰԽ�ޱ�־
	u32 t;
	u32 n;
	u8 *p8;
	u16 *p16;
	u32 limitH;//��ѹ��������
	u32 limitL;//��ѹ��������
	u32 standardH;//��ѹ�ϸ�����
	u32 standardL;//��ѹ�ϸ�����

	p8=(u8*)ADDR_213x_REAL;//�������ѹ�ϸ���
	t=Comm_Ram->DL698YMDHMS[6];
	i=p8[0];//�ϴμ���RTC��
	if(t==i)
	{
		return;
	}
	p8[0]=t;//�ϴμ���RTC��
	a=p8[1];//hex�ϵ�����ʱ��ʱ
	if(a<5)//5����ʱ
	{
		a++;
		p8[1]=a;
		return;
	}
	if(t<i)
	{
		t+=60;
	}
	t-=i;//��ֵ
	t+=p8[2];//�����
	n=p8[3];//��ѹ��ƽ������
	n++;
	if(t<60)
	{
		p8[2]=t;
		p8[3]=n;//��ѹ��ƽ������
	}
	else
	{
		p8[2]=0;//�����
		p8[3]=0;//��ѹ��ƽ������
//#define ADDR_4030     ADDR_4024+LENmax_4024//��ѹ�ϸ��ʲ���
////����2��=structure
////{
////	��ѹ��������  long-unsigned����λ��V�����㣺-1����
////  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
////	��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
////  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
////}
//#define LENmax_4030     14
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
		{
			case 0:
				limitH=2640;//��ѹ��������
				limitL=1540;//��ѹ��������
				standardH=2354;//��ѹ�ϸ�����
				standardL=2046;//��ѹ�ϸ�����
				break;
			case 1:
				limitH=1200;//��ѹ��������
				limitL=780;//��ѹ��������
				standardH=1070;//��ѹ�ϸ�����
				standardL=900;//��ѹ�ϸ�����
				break;
			default:
				limitH=692;//��ѹ��������
				limitL=450;//��ѹ��������
				standardH=617;//��ѹ�ϸ�����
				standardL=519;//��ѹ�ϸ�����
				break;
		}
	#else
		MR(ADDR_DATABUFF,ADDR_4030,LENmax_4030);
		p8=(u8*)ADDR_DATABUFF;
		limitH=(p8[3]<<8)|p8[4];//��ѹ��������
		limitL=(p8[6]<<8)|p8[7];//��ѹ��������
		standardH=(p8[9]<<8)|p8[10];//��ѹ�ϸ�����
		standardL=(p8[12]<<8)|p8[13];//��ѹ�ϸ�����
	#endif
	}
	
	for(i=0;i<3;i++)
	{
//��ʼ�����ݽṹ
		p8=(u8*)ADDR_2131+(i*LENmax_2131);
//����2����ѹ�ϸ������ݣ���=structure
		p8[0]=DataType_structure;
		p8[1]=2;
//{
//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
//��ѹ�ϸ��ʡ�=structure
		p8[2]=DataType_structure;
		p8[3]=5;
//{
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[4]=DataType_double_long_unsigned;
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
		p8[9]=DataType_long_unsigned;
//��ѹ������   long-unsigned����λ��%�����㣺-2����
		p8[12]=DataType_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[15]=DataType_double_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
		p8[20]=DataType_double_long_unsigned;
//}
//  ���µ�ѹ�ϸ���         ��ѹ�ϸ���
//��ѹ�ϸ��ʡ�=structure
		p8[25]=DataType_structure;
		p8[26]=5;
//{
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[27]=DataType_double_long_unsigned;
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
		p8[32]=DataType_long_unsigned;
//��ѹ������   long-unsigned����λ��%�����㣺-2����
		p8[35]=DataType_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[38]=DataType_double_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
		p8[43]=DataType_double_long_unsigned;
		
		p16=(u16*)(ADDR_UA+2+(i*4));//ȡ���ɵ�ѹ
		a=p16[0];
		a=bcd_hex(a);
		p16=(u16*)(ADDR_213x_REAL+4+(i*6));
		b=p16[0]|(p16[1]<<16);
		a+=b;//��ѹ��
		if(t<60)
		{//û��60��
			p16[0]=a;
			p16[1]=a>>16;
			f=3;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
			//continue;
		}
		else
		{//��60��
			p16[0]=0;
			p16[1]=0;
			if((Meter_Ram->PW==1)&&(i==1))//��ʼ���߷�ʽ;0=��������,1=��������
			{//��������B��
				f=3;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
			}
			else
			{
				a/=n;
				if(a>=limitH)//��ѹ��������
				{
					f=1;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
				}
				else
				{
					if(a>=standardH)//��ѹ�ϸ�����
					{
						f=2;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
					}
					else
					{
						if(a<=limitL)//��ѹ��������
						{
							f=5;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
						}
						else
						{
							if(a<=standardL)//��ѹ�ϸ�����
							{
								f=4;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
							}
							else
							{
								f=3;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
							}
						}
					}
				}
			}
		}
/*
		//���յ�ѹ���ʱ��
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+3,4);
		a=R_Inverse(a,4);
		a++;
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131+(i*LENmax_2131)+2+3,4);
		//���µ�ѹ���ʱ��
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+23+3,4);
		a=R_Inverse(a,4);
		a++;
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131+(i*LENmax_2131)+2+23+3,4);
*/
		a=Comm_Ram->DL698YMDHMS[4];//ʱ
		a*=60;
		a+=Comm_Ram->DL698YMDHMS[5];//��
		a++;//��1��
		//���յ�ѹ���ʱ��
		b=R_Inverse(a,4);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+3,4);
		//���µ�ѹ���ʱ��
		b=Comm_Ram->DL698YMDHMS[3];//��
		if(b)
		{
			b--;
		}
		b*=24*60;
		a+=b;
		b=R_Inverse(a,4);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+23+3,4);
		
		switch(f)
		{
			case 1://1=Խ��������
			case 2://2=Խ�ϸ�����,
				//���յ�ѹ������ʱ��
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+14,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+14,4);
				//���µ�ѹ������ʱ��
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+14,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+14,4);
				break;
			case 3://3=�ϸ�
				break;
			case 4://4=Խ�ϸ�����,
			case 5://5=Խ��������
				//���յ�ѹ������ʱ��
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+19,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+19,4);
				//���µ�ѹ������ʱ��
				c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+19,4);
				c=R_Inverse(c,4);
				c++;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+19,4);
				break;
		}
		//���յ�ѹ������
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+3,4);//��ѹ���ʱ��
		b=MRR(ADDR_2131+(i*LENmax_2131)+2+14,4);//��ѹ������ʱ��
		c=MRR(ADDR_2131+(i*LENmax_2131)+2+19,4);//��ѹ������ʱ��
		a=R_Inverse(a,4);
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%��λС��
		c=b/a;//������
		b=10000-c;//�ϸ���
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+8,2);//��ѹ�ϸ���
		MWR(c,ADDR_2131+(i*LENmax_2131)+2+11,2);//��ѹ������
		//���µ�ѹ�ϸ���
		a=MRR(ADDR_2131+(i*LENmax_2131)+2+23+3,4);//��ѹ���ʱ��
		b=MRR(ADDR_2131+(i*LENmax_2131)+2+23+14,4);//��ѹ������ʱ��
		c=MRR(ADDR_2131+(i*LENmax_2131)+2+23+19,4);//��ѹ������ʱ��
		a=R_Inverse(a,4);
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%��λС��
		c=b/a;//������
		b=10000-c;//�ϸ���
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131+(i*LENmax_2131)+2+23+8,2);//��ѹ�ϸ���
		MWR(c,ADDR_2131+(i*LENmax_2131)+2+23+11,2);//��ѹ������
		//�¼�
		p8=(u8*)ADDR_213x_REAL+4+(i*6);//�������ѹ�ϸ���
		if(f!=p8[5])
		{
			
			p8[5]=f;//Խ���¼���¼��־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
		}
	}
}


















