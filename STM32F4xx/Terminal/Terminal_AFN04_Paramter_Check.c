
#include "Project.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


void PnVSAFN04F10SetNoList(void)//�������Pn��ӦAFN04F10��������б�
{
	u32 Pn;
	u32 Meter;
	u8 *p8;
	u16 *p16;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������
	for(Meter=0;Meter<=AFN04F10MaxPn;Meter++)
	{
		p16[Meter]=0;//���б�
	}
	p8=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8[0]+(p8[1]<<8);
		if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
		{
			p16[Pn]=Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
}

u32 Check_AFN04F10_Pn(u32 Pn)//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
{
	u16 *p16;
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return 0;
	}
	return p16[Pn];
/*
#if IC_SST39VF6401B==0//0=û,1=��
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F10+2;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr,2);
		if(i==Pn)
		{
			return Meter;
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;
#else
	u32 i;
	u32 Meter;
	u8 *p8;
	if(Pn==0)
	{
		return 0;
	}
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i==Pn)
		{
			return Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
*/
}

u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
{
	u32 i;
	u32 Meter;
	u64 d64;
	u8 *p8;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i!=0)
		{//���������Ч
			i=p8[2];
			i&=0x1f;
			if(i==port)
			{//�˿ں���ͬ
				d64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
				d64<<=16;
				d64+=(p8[4+1]<<8)+p8[4];
				if(d64==CommAddr)
				{//��ַ��ͬ
					return Meter;//��ͬ���������
				}
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
}

u32 Check_AFN04F11_Pn(u32 Pn)//����ն��������ò���ָ���Ĳ������;����0��,1��
{
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F11+1;
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i==Pn)
		{
			return 1;
		}
		Addr+=5;
	}
	return 0;
}

u32 Check_AFN04F13_Pn(u32 Pn)//����ն˵�ѹ/����ģ�������ò���ָ���Ĳ������;����0��,1��
{
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F13+1;
	for(Meter=1;Meter<=AFN04F13MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i==Pn)
		{
			return 1;
		}
		Addr+=3;
	}
	return 0;
}

void AFN04F150_Computer(void)//��������Ч��־����
{
	u32 i;
	u32 Meter;
	u8 *p8;
	u8 *p8s;
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u32 Addr;
#endif
	
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8=(u8 *)(ADDR_DATABUFF);
	MC(0,ADDR_DATABUFF,256);
	//�ն˵��ܱ���������װ�����ò���
	/*
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������,0Ϊ��Ч
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=Meter;
		if(p16[i]!=0)
		{
	#if AFN04F150Dn==0//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
	}
	*/
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8s[0]+(p8s[1]<<8);
		if((i!=0)&&(i<(256*8)))
		{
	#if AFN04F150Dn==0//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	//�ն��������ò���
	Addr=ADDR_AFN04F11+1;
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		i=MRR(Addr,1);
		if(i!=0)
		{
	#if AFN04F150Dn==0//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
			i--;
	#endif
			p8[i/8]|=(1<<(i%8));
		}
		Addr+=5;
	}
#endif
	MW(ADDR_DATABUFF,ADDR_AFN04F150,256);
}

u32 PnSet(u32 Pn)//��Ч������ż��;����0=��Ч,1=��Ч
{
	u32 i;
	u32 x;
	
	if((Pn==0)||(Pn>=(256*8)))
	{
		return 0;
	}
#if AFN04F150Dn==0//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
	Pn-=1;
#endif
	i=Pn/8;
	x=MRR(ADDR_AFN04F150+i,1);
	i=Pn%8;
	x>>=i;
	x&=1;
	return x;
}

u32 Check_ACSamplePn(u32 Pn)//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
{
	u32 i;
	u16 *p16;
	
	p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return 0;
	}
	i=p16[Pn];
	if(i!=0)
	{
		i--;
		i=(ADDR_AFN04F10+(LEN_AFN04F10_Pn*i));
		i=MRR(i+4,2);//ͨ�����ʼ��˿ں�
		if((i&0x1f)==1)
		{//�˿ں�1
			return 1;
		}
		if((i>>8)==2)//ͨ��Э������
		{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
			return 1;
		}
	}
	return 0;
}

u32 ACSampleVoltageCurvePn(u32 Fn,u32 Pn)//��Ч���ɵ�ѹ���߲�����ż��;����0=��Ч,1=��Ч
{
	if((Fn<89)||(Fn>91))
	{
		return 0;
	}
	Fn=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	return Fn;
}

u32 GroupTotalSet(u32 Pn)//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
{
	if((Pn==0)||(Pn>AFN04F14MaxPn))
	{
		return 0;
	}
	Pn-=1;
	Pn=ADDR_AFN04F14+1+(LEN_AFN04F14_Pn*Pn);
	Pn=MRR(Pn,1);
	if(Pn!=0)
	{
		Pn=1;
	}
	return Pn;
}

void Terminal_AFN0CF3_Computer(u32 Fn)//�ն���1����F3,�ն˲���״̬
{
	u32 i;
	u32 x;

	if(Fn!=0x0)
	{
		if(Fn<=248)
		{
			Fn-=1;
			i=Fn/8;
			x=MRR(ADDR_AFN0CF3+i,1);
			x|=1<<(Fn%8);
			MWR(x,ADDR_AFN0CF3+i,1);
		}
	}
}

void PnVSProtocolList(u32 ADDR_BUFF)//�������Pn��ӦAFN04F10����ͨ��Э���б�,�������ҷ������ͨ��Э��Ϊ30(DL645-2007)
{
	u32 Pn;
	u32 Meter;
	u8 *p8s;
	u8 *p8d;

#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	//���б�
	p8d=(u8*)(ADDR_BUFF);//�������Pn��ӦAFN04F10�����б�
	for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
	{
		p8d[Pn]=30;//ͨ��Э������:1=DL/T645-1997,30=DL645-2007
	}
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8s[0]+(p8s[1]<<8);
		if(Pn<=MaxRS485AddCarrierPn)
		{//���������Ч
			p8d[Pn]=p8s[3];//ȡͨ��Э��
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	//�����ͨ��Э��Ϊ0
	p8s=(u8*)(ADDR_AFN04F11+1);
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		//Pn=p8s[0];
		Pn=MRR((u32)p8s,1);
		if(Pn<=MaxRS485AddCarrierPn)
		{//���������Ч
			p8d[Pn]=0;//�˿ںź�ͨ��Э��
		}
		p8s+=5;
	}
}

void PnVSPORTProtocolList(u32 ADDR_BUFF)//�������Pn��ӦAFN04F10���ö˿ںź�ͨ��Э���б�,�������ҷ������Ĭ�϶˿ں�0ͨ��Э��Ϊ30(DL645-2007)
{
	u32 Pn;
	u32 Meter;
	u8 *p8s;
	u16 *p16;
	
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	//���б�
	p16=(u16*)(ADDR_BUFF);//�������Pn��ӦAFN04F10�����б�
	for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
	{
		p16[Pn]=0+(30<<8);//�˿ںź�ͨ��Э������:1=DL/T645-1997,30=DL645-2007
	}
	p8s=(u8*)(ADDR_128KDATABUFF+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		Pn=p8s[0]+(p8s[1]<<8);
		if(Pn<=MaxRS485AddCarrierPn)
		{//���������Ч
			p16[Pn]=(p8s[2]&0x1f)+(p8s[3]<<8);//ȡ�˿ںź�ͨ��Э��
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	//�����ͨ��Э��Ϊ0
	p8s=(u8*)(ADDR_AFN04F11+1);
	for(Meter=1;Meter<=AFN04F11MaxPn;Meter++)
	{
		//Pn=p8s[0];
		Pn=MRR((u32)p8s,1);
		if(Pn<=MaxRS485AddCarrierPn)
		{//���������Ч
			p16[Pn]=0;//�˿ںź�ͨ��Э��
		}
		p8s+=5;
	}
}

u32 Get_SetTariff(u32 Pn)//ȡAFN04F10�����õķ�����
{
	u32 i;
	i=Check_AFN04F10_Pn(Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
	if(i!=0)
	{
		i--;
		i=MRR(ADDR_AFN04F10+(i*LEN_AFN04F10_Pn)+18,1);
	}
	else
	{//������������ն�04�������õķ�����(��������ȡֵ��Χ1��12��������Ч)
		i=Check_AFN04F11_Pn(Pn);//����ն��������ò���ָ���Ĳ������;����0��,1��
		if(i!=0)
		{
			i=MRR(ADDR_AFN04F21+48,1);
			if(i==0)
			{
				i=1;
			}
			else
			{
				if(i>12)
				{
					i=12;
				}
			}
		}
		else
		{
			return 4;//û�ҵ�
		}
	}
	if(i>TMaxTariff)
	{
		i=TMaxTariff;
	}
	return i;
}


u32 F246MapPn(u32 Pn)//F246�����¼����ӳ��Pn;����0��ʾ��Ч
{
	u32 i;
	u16 *p16;
	p16=(u16 *)(ADDR_F246PnList);//Ҫ�����ĵ����¼����Pn�б�,���ֶ���
	for(i=0;i<F246MaxPn;i++)
	{
		if(p16[i]==Pn)
		{
			return (i+1);
		}
	}
	return 0;
}

void F246PnList(void)//Ҫ������F246�����¼����Pn�б�
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u16 *pn16;
	u32 YMD;
	
	p16=(u16 *)(ADDR_F246PnList);//Ҫ�����ĵ����¼����Pn�б�,���ֶ���
	for(i=0;i<F246MaxPn;i++)
	{//���б�
		p16[i]=0;
	}
	i=MRR(ADDR_AFN04F97,1);//ͣ�����ݲɼ����ò���
	if((i&1)==0)
	{//ͣ�����ݲɼ���־D0����"1"��Ч����"0"��Ч
		return;
	}
#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
	#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	if((i&2)==0)
	{//D1����"0"ֻ�ɼ����õĲ�����
		y=MRR(ADDR_AFN04F97+3,2);//��Ҫ��ȡͣ���¼����ܱ����n	BIN	2	
		if(y>F246MaxPn)
		{
			y=F246MaxPn;
		}
		MR(ADDR_F246PnList,ADDR_AFN04F97+5,y*2);
		//ѡ1ֻ���俼�˵�
		if(y<F246MaxPn)
		{
			p8=(u8*)(ADDR_128KDATABUFF);
			for(i=0;i<AFN04F10MaxPn;i++)
			{
				Pn=p8[2]+(p8[3]<<8);
				if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
				{
					x=p8[4]&0x1f;
					if((x==2)||(x==3)||(x==31))
					{//�Ƕ˿�2,3��31
						x=p8[26];//�û�����ż��û�С���
					#if (USER/100)==11//����
						if(((x>>4)<=2)||(x==0x42))//���ֹ��俼�˵�Ϊ�����5�ڲ�����,��������������
					#else
						x>>=4;
						if(x==6)
					#endif
						{//
							for(z=0;z<y;z++)
							{
								if(Pn==p16[z])
								{
									break;;//ԭ�Ѽ���
								}
							}
							if(z==y)
							{
								p16[y]=Pn;
								y++;
								break;
							}
						}
					}
				}
				p8+=LEN_AFN04F10_Pn;
			}
		}
	}
	else
	{//D1����"1"���ѡ�������
		y=0;//��ѡ����
		//�����1���С���(1-16),����ר���û�(A��)
		//�����2���С���(1-16),��С��ר���û�(B��)
		//�����3���С���(1-16),����һ�㹤��ҵ�û�(C��)
		//�����4���С���(1-16),����һ�㹤��ҵ�û�(D��)
		//�����5���С���(1-16),�����û�(E��)
		//�����6���С���(1-16),���ÿ��˼�����(F��)
		//��ѡ1ֻ���俼�˵�
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//���Ժ����,�ز���������
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//�Ƕ˿�2,3��31
					x=p8[26];//�û�����ż��û�С���
				#if (USER/100)==11//����
					if(((x>>4)<=2)||(x==0x42))//���ֹ��俼�˵�Ϊ�����5�ڲ�����,��������������
				#else
					x>>=4;
					if(x==6)
				#endif
					{//
						p16[y]=Pn;
						y++;
						break;
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
		//��ѡ���ն�����������������
		YMD=MRR(ADDR_TYMDHMS+3,3);
		YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
		YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
		i=GetDateCongealAddr(YMD,161,1);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
		if(i!=0)
		{
			YMD=YMD_Add1D(YMD);//�����ռ�1��,���ؼ�1���������
			YMD&=0xffff;
			p8=(u8*)i;
			pn16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������,0Ϊ��Ч
			for(Pn=1;Pn<=MaxRS485AddCarrierPn;Pn++)
			{
				x=p8[2]+(p8[3]<<8);//ԭ��������
				if(x==YMD)
				{//���ճ�������
					x=pn16[Pn];
					if((x!=0)&&(x<=MaxRS485AddCarrierPn))
					{
						x--;
						p8d=(u8*)(ADDR_128KDATABUFF+(x*LEN_AFN04F10_Pn));
						x=p8d[4];
						x&=0x1f;
					#if (USER/100)==0//���Ժ����,�ز���������
						if((x==2)||(x==3))
					#else
						if((x==2)||(x==3)||(x==31))
					#endif
						{//�Ƕ˿�2,3��31
							x=p8d[26];//�û�����ż��û�С���
						#if (USER/100)==11//����
							if(((x>>4)<=2)||(x==0x42))//���ֹ��俼�˵�Ϊ�����5�ڲ�����,��������������
						#else
							x>>=4;
							if((x!=4)&&(x!=6))
						#endif
							{//
								for(z=0;z<y;z++)
								{
									if(Pn==p16[z])
									{
										break;;//ԭ�Ѽ���
									}
								}
								if(z==y)
								{
									p16[y]=Pn;
									y++;
									x=MRR(ADDR_AFN04F97+3,2);//��Ҫ��ȡͣ���¼����ܱ����n	BIN	2
									x++;//��Ҫ��ȡͣ���¼����ܱ����nΪ��̨�����˱���ɼ�ͣ���¼�����Ч����������������ɼ�n+1�����ܱ�ͣ���¼�
									if(x>F246MaxPn)
									{
										x=F246MaxPn;
									}
									if(y>=x)
									{
										return;
									}
								}
							}
						}
					}
				}
				p8+=6+(5*(1+TMaxTariff));
			}
		}
		//ѡ���������
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//���Ժ����,�ز���������
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//�Ƕ˿�2,3��31
					x=p8[26];//�û�����ż��û�С���
				#if (USER/100)==11//����
					if(((x>>4)<=2)||(x==0x42))//���ֹ��俼�˵�Ϊ�����5�ڲ�����,��������������
				#else
					x>>=4;
					if((x!=4)&&(x!=6))
				#endif
					{//
						for(z=0;z<y;z++)
						{
							if(Pn==p16[z])
							{
								break;//ԭ�Ѽ���
							}
						}
						if(z==y)
						{
							p16[y]=Pn;
							y++;
							x=MRR(ADDR_AFN04F97+3,2);//��Ҫ��ȡͣ���¼����ܱ����n	BIN	2
							x++;//��Ҫ��ȡͣ���¼����ܱ����nΪ��̨�����˱���ɼ�ͣ���¼�����Ч����������������ɼ�n+1�����ܱ�ͣ���¼�
							if(x>F246MaxPn)
							{
								x=F246MaxPn;
							}
							if(y>=x)
							{
								return;
							}
						}
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
		//��������
		p8=(u8*)(ADDR_128KDATABUFF);
		for(i=0;i<AFN04F10MaxPn;i++)
		{
			Pn=p8[2]+(p8[3]<<8);
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				x=p8[4]&0x1f;
			#if (USER/100)==0//���Ժ����,�ز���������
				if((x==2)||(x==3))
			#else
				if((x==2)||(x==3)||(x==31))
			#endif
				{//�Ƕ˿�2,3��31
					for(z=0;z<y;z++)
					{
						if(Pn==p16[z])
						{
							break;//ԭ�Ѽ���
						}
					}
					if(z==y)
					{
						p16[y]=Pn;
						y++;
						x=MRR(ADDR_AFN04F97+3,2);//��Ҫ��ȡͣ���¼����ܱ����n	BIN	2
						x++;//��Ҫ��ȡͣ���¼����ܱ����nΪ��̨�����˱���ɼ�ͣ���¼�����Ч����������������ɼ�n+1�����ܱ�ͣ���¼�
						if(x>F246MaxPn)
						{
							x=F246MaxPn;
						}
						if(y>=x)
						{
							return;
						}
					}
				}
			}
			p8+=LEN_AFN04F10_Pn;
		}
	}
}


const u8 CurveSetFnList[]=//2���������߶����ܶȲ���Fn�б�(ͬ��ADDR_AFN04F104��
{
	73,//1 AFN0DF73 �ܼ����й���������
	74,//2 AFN0DF74 �ܼ����޹���������
	75,//3 AFN0DF75 �ܼ����й�����������
	76,//4 AFN0DF76 �ܼ����޹�����������
	81,//5 AFN0DF81 �������й���������
	82,//6 AFN0DF82 ������A���й���������
	83,//7 AFN0DF83 ������B���й���������
	84,//8 AFN0DF84 ������C���й���������
	85,//9 AFN0DF85 �������޹���������
	86,//10 AFN0DF86 ������A�޹���������
	87,//11 AFN0DF87 ������B�޹���������
	88,//12 AFN0DF88 ������C�޹���������
	89,//13 AFN0DF89 ������A���ѹ����
	90,//14 AFN0DF90 ������B���ѹ����
	91,//15 AFN0DF91 ������C���ѹ����
	92,//16 AFN0DF92 ������A���������
	93,//17 AFN0DF93 ������B���������
	94,//18 AFN0DF94 ������C���������
	95,//19 AFN0DF95 �����������������
	97,//20 AFN0DF97 �����������й��ܵ���������
	98,//21 AFN0DF98 �����������޹��ܵ���������
	99,//22 AFN0DF99 �����㷴���й��ܵ���������
	100,//23 AFN0DF100 �����㷴���޹��ܵ���������
	101,//24 AFN0DF101 �����������й��ܵ���ʾֵ
	102,//25 AFN0DF102 �����������޹��ܵ���ʾֵ
	103,//26 AFN0DF103 �����㷴���й��ܵ���ʾֵ
	104,//27 AFN0DF104 �����㷴���޹��ܵ���ʾֵ
	105,//28 AFN0DF105 �������ܹ�������
	106,//29 AFN0DF106 ������A�๦������
	107,//30 AFN0DF107 ������B�๦������
	108,//31 AFN0DF108 ������C�๦������
	109,//32 AFN0DF109 �������ѹ��λ������
	110,//33 AFN0DF110 �����������λ������
	138,//34 AFN0DF138 ֱ��ģ������������
	145,//35 AFN0DF145 ������1�����޹��ܵ���ʾֵ����
	146,//36 AFN0DF146 ������4�����޹��ܵ���ʾֵ����
	147,//37 AFN0DF147 ������2�����޹��ܵ���ʾֵ����
	148,//38 AFN0DF148 ������3�����޹��ܵ���ʾֵ����

#if LEN_AFN04F104_PerPn!=38
	#error "LEN_AFN04F104_PerPn!=38"
#endif
};

u32 Get_CurveSetFnListNo(u32 Fn)//�õ����߶����ܶȲ���Fn�б��;����:�б��0-n,0xffffffff��ʾ��
{
	u32 i;
	
	for(i=0;i<sizeof(CurveSetFnList);i++)
	{
		if(CurveSetFnList[i]==Fn)
		{
			return i;
		}
	}
	return 0xffffffff;
}



