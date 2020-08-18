

//��������
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"






#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

u32 kWhContrl1_Comp(u32 Pn,u8* p8s)//�ն˿���-���.�µ�ص������Ƚ�,���:Pn=�ܼ����(0-7),p8s=�ܼ����µ�����õ�Ԫ;����0û����,1����,2����
{
	u32 i;
//	u32 x;
	u64 a64;
	u64 b64;

	//ȡ�µ������ض�ֵ
	a64=MRR((u32)p8s+6,8);//ȡ�µ����ض�ֵ    long64����λ��kWh�����㣺-4��
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(a64>>63)
	{//��
		a64=(~a64)+1;
	}
	//ȡ�ն��µ������ض�ֵ����ϵ��
	i=p8s[17];
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
	//��ǰ��ֵ
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	MWR(a64,ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn)+8,8);//д�Ĵ������ݵ�������,Byte<=8
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	//ȡ��ǰ�µ���
	b64=MRR(ADDR_230x_9+(Pn*LENper_230x_9)+2+1,8);//�ܼ����й�����
	b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(b64>>63)
	{//��
		b64=(~b64)+1;
	}
	if(b64>=a64)
	{//��ǰ�µ���>=��ֵ
		return 2;//����
	}
	//��������ֵ
	a64=MRR((u32)p8s+6,8);//ȡ�µ����ض�ֵ    long64����λ��kWh�����㣺-4��
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(a64>>63)
	{//��
		a64=(~a64)+1;
	}
	//ȡ�������޸���ϵ��
	i=p8s[15];
	//i&=0x7f;
	a64*=i;
	a64/=100;
	if(b64>=a64)
	{//��ǰ�µ���>=��ֵ80%
		return 1;//����
	}
	return 0;//û����
}


void Terminal_Contrl_MonthEnergy(void)//�ն˿���-�µ��
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
//	u64 a64;
//	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
//	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8108_3+LENmax_8108_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//�µ��-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//�µ�����õ�Ԫ
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//����2�����Ʒ���������=array �µ�����õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//	�ܼ������      OI��
//	�µ����ض�ֵ    long64����λ��kWh�����㣺-4����
//	��������ֵϵ��  unsigned����λ��%����
//	�µ����ض�ֵ����ϵ��  integer����λ��%��
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
		p8s=(u8*)ADDR_DATABUFF+LENmax_8108_3;
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
		
		//����
		p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				break;
			case 3://����
				i=kWhContrl1_Comp(Pn,p8s);//�ն˿���-���.�µ�ص������Ƚ�,���:Pn=�ܼ����(0-7),p8s=�ܼ����µ�����õ�Ԫ;����0û����,1����,2����
				switch(i)
				{
					case 1:
						//��ظ澯�¼�
						//���:pOccurSour=�¼�����Դ    OI
						//OI=���ƶ���      OI
						//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
						Event_32030200(0x8108,0x2300+(Pn+1),p8d+8);//��ظ澯�¼���¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						//�ض�������
						MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//
						MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//
						p8d[0]=100;//����
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 100://����
				i=kWhContrl1_Comp(Pn,p8s);//�ն˿���-���.�µ�ص������Ƚ�,���:Pn=�ܼ����(0-7),p8s=�ܼ����µ�����õ�Ԫ;����0û����,1����,2����
				switch(i)
				{
					case 0:
						p8d[0]=3;
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 101://��բ׼��
				//��բ�ִ�
				x=0;//���ȫ���ִ�
				for(i=0;i<NUMmax_CR;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_230x_15+(LENper_230x_15*Pn)+2,1);//�ܼ������ִ�����
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2]=i;//��ǰ�����բ�ִ�B0-B7
				i&=x;
				if(i!=0x0)
				{//���ִ���բ
					p8d[1]=i;
					//�����բ�¼�
					//���:pOccurSour=�¼�����Դ    OI
					//OI=���ƶ���      OI
					//bit=��բ�ִ�      bit-string(SIZE(8))
					//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
					//pFirst=��բ����ʱ�ܼӵ�����  long64����λ��kwh/Ԫ������-4��
				//���¼�����Դ����Ϊ�µ��8108����բʱ�ܼӵ�����Ϊ�ܼ��µ�������
				//���¼�����Դ����Ϊ�����8107����բʱ�ܼӵ�����Ϊʣ�������/�ѡ�
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32010200(0x8108,0x2300+(Pn+1),i,p8d+8,(u8*)ADDR_230x_9+(Pn*LENper_230x_9)+2+1);//�����բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);//
					MR(ADDR_DATABUFF+LENmax_8108_3,ADDR_8108_2,LENmax_8108_2);//
				}
				p8d[0]=102;
				break;
			case 102://��բ
				i=kWhContrl1_Comp(Pn,p8s);//�ն˿���-���.�µ�ص������Ƚ�,���:Pn=�ܼ����(0-7),p8s=�ܼ����µ�����õ�Ԫ;����0û����,1����,2����
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//���ִ���բ
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
			p8d=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}
}



u32 kWhContrl2_Comp(u32 Pn,u8* p8s)//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7),p8s=�ܼ��鹺������õ�Ԫ;����0û����,1����,2����
{
	s64 a64;
	s64 b64;

	//ʣ���������
	a64=MRR(ADDR_230x_11+(Pn*LENper_230x_11)+1,8);//�ܼ�ʣ��������ѣ�
	a64=R_Inverse(a64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	//��բ����ֵ
	b64=MRR((u32)p8s+23+9+1,8);
	b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(a64<=b64)
	{//ʣ���������<=��բ����ֵ
		return 2;//����
	}
	//��������ֵ
	b64=MRR((u32)p8s+23+1,8);
	b64=R_Inverse(b64,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(a64<=b64)
	{//ʣ���������<=��������ֵ
		return 1;//����
	}
	return 0;//û����
}


void Terminal_Contrl_BuyEnergy(void)//�ն˿���-�����
{
	u32 i;
	u32 j;
	u32 x;
	u32 n;
	u32 OI;
	u32 Pn;
//	u64 a64;
//	u64 b64;
	u32 TAG;//��������ܼ���b0-b7��ʾ�ܼ���1-8
	u8* p8;
	u8* p8c;
	u8* p8s;
	u8* p8d;
//	u16* p16;
//	u16* p16timer;
	
	TAG=0;//��������ܼ���b0-b7��ʾ�ܼ���1-8
#if (LENmax_8107_3+LENmax_8107_2)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//�����-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
	MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//��������õ�Ԫ
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}

//����2�����Ʒ���������=array ��������õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//	�ܼ������      OI��
//���絥��        double-long-unsigned��
//׷��/ˢ�±�ʶ   enum{׷�ӣ�0����ˢ�£�1��}��
//��������        enum{������0������ѣ�1��}��
//���������ѣ�ֵ  long64����λ��kWh/Ԫ�� ���㣺-4����
//	��������ֵ      long64����λ��kWh/Ԫ�����㣺-4����
//	��բ����ֵ      long64����λ��kWh/Ԫ�����㣺-4��
//	�����ģʽ      enum{����ģʽ��0����Զ��ģʽ��1��}
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
//		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
//		if(p8[1]!=0x0)
//		{//����
//			break;
//		}
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
		p8s=(u8*)ADDR_DATABUFF+LENmax_8107_3;
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
		
		//����
		p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
		p8d[7]=p8s[13];//������� 0=������,1=�����
		MR((u32)p8d+8,(u32)p8s+23+1,8);//��������ֵ
		TAG|=1<<Pn;//��������ܼ���b0-b7��ʾ�ܼ���1-8
		switch(p8d[0])
		{
			case 0://�ϵ�򲻿���
				p8d[0]=3;
				break;
			case 3://����
				p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
				if(p8[1]!=0x0)
				{//����
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7),p8s=�ܼ��鹺������õ�Ԫ;����0û����,1����,2����
				switch(i)
				{
					case 1:
						//��ظ澯�¼�
						//���:pOccurSour=�¼�����Դ    OI
						//OI=���ƶ���      OI
						//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
						Event_32030200(0x8107,0x2300+(Pn+1),p8d+8);//��ظ澯�¼���¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
						//�ض�������
						MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//�����-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
						MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//��������õ�Ԫ
						p8d[0]=100;//����
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 100://����
				p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
				if(p8[1]!=0x0)
				{//����
					p8d[0]=3;
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//�ն˿���-���.����ص����ѱȽ�,���Pn=�ܼ����(0-7),p8s=�ܼ��鹺������õ�Ԫ;����0û����,1����,2����
				switch(i)
				{
					case 0:
						p8d[0]=3;
						break;
					case 2:
						p8d[0]=101;//��բ׼��
						break;
				}
				break;
			case 101://��բ׼��
				//��բ�ִ�
				x=0;//���ȫ���ִ�
				for(i=0;i<NUMmax_CR;i++)
				{
					x<<=1;
					x|=1;
				}
				i=MRR(ADDR_230x_15+(LENper_230x_15*Pn)+2,1);//�ܼ������ִ�����
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2]=i;//��ǰ�����բ�ִ�B0-B7
				i&=x;
				if(i!=0x0)
				{//���ִ���բ
					p8d[1]=i;
					//�����բ�¼�
					//���:pOccurSour=�¼�����Դ    OI
					//OI=���ƶ���      OI
					//bit=��բ�ִ�      bit-string(SIZE(8))
					//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
					//pFirst=��բ����ʱ�ܼӵ�����  long64����λ��kwh/Ԫ������-4��
				//���¼�����Դ����Ϊ�µ��8108����բʱ�ܼӵ�����Ϊ�ܼ��µ�������
				//���¼�����Դ����Ϊ�����8107����բʱ�ܼӵ�����Ϊʣ�������/�ѡ�
					i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					Event_32010200(0x8107,0x2300+(Pn+1),i,p8d+8,(u8*)ADDR_230x_11+(Pn*LENper_230x_11)+1);//�����բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					//�ض�������
					MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);//�����-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
					MR(ADDR_DATABUFF+LENmax_8107_3,ADDR_8107_2,LENmax_8107_2);//��������õ�Ԫ
				}
				p8d[0]=102;
				break;
			case 102://��բ
				p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
				if(p8[1]!=0x0)
				{//����
					p8d[0]=3;
					p8d[1]=0;//���ִ���բ
					break;
				}
				i=kWhContrl2_Comp(Pn,p8s);//�ն˿���-���.�µ�ص������Ƚ�,���:Pn=�ܼ����(0-7),p8s=�ܼ����µ�����õ�Ԫ;����0û����,1����,2����
				if(i!=2)
				{
					p8d[0]=3;
					p8d[1]=0;//���ִ���բ
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
			p8d=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len));
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
		}
	}

}

#endif

