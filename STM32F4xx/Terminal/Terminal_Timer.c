
#include "Project.h"
#include "Terminal_Timer.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../device/MEMRW.h"

void Terminal_FMTimer(void)//���籣����ͷֶ�ʱ��
{
	u32 i;
	u32 s;
	u32 m;
	u8 *p8;
	u16 *p16;
	
	MR(ADDR_DATABUFF,ADDR_TerminalFMTimer,6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2));
	i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
	if(i)
	{//ԭʱ�Ӵ���
		MW(ADDR_TYMDHMS,ADDR_TerminalFMTimer,6);//����ʱ��
		return;
	}
	s=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
	if(s>>31)
	{//ʱ��2<ʱ��1��ֵΪ��
		MW(ADDR_TYMDHMS,ADDR_TerminalFMTimer,6);//����ʱ��
		return;
	}
	if(s==0)
	{
		return;
	}
	
	MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
	p16=(u16*)(ADDR_DATABUFF+6);
//��Ӽ��������
	m=p16[0];
	m+=s;
	p16[0]=m%60;
	m/=60;
//�������
	for(i=1;i<TFMSTIMER_NUM;i++)
	{
		if(p16[i]>=s)
		{
			p16[i]-=s;
		}
		else
		{
			p16[i]=0;
		}
	}
//�ֶ�ʱ��
	p16=(u16*)(ADDR_DATABUFF+6+(TFMSTIMER_NUM*2));
	for(i=0;i<TFMMTIMER_NUM;i++)
	{
		if(p16[i]>=m)
		{
			p16[i]-=m;
		}
		else
		{
			p16[i]=0;
		}
	}
	MW(ADDR_DATABUFF,ADDR_TerminalFMTimer,6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2));

//ʵ�߶�ʱ��,����ʱ����Ч
	if(m!=0)
	{
		MR(ADDR_DATABUFF,ADDR_TerminalRealTimer,2+(TREALMTIMER_NUM*2));
		p8=(u8*)(ADDR_DATABUFF);
		p16=(u16*)(ADDR_DATABUFF+2);
		if(p8[0])
		{//0=����ʱ��δ����,1=����ʱ���Ѽ���
			if(m>1)
			{
				m=1;
			}
		}
		p8[0]=1;//1=����ʱ���Ѽ���
		for(i=0;i<TREALMTIMER_NUM;i++)
		{
			if(p16[i]>=m)
			{
				p16[i]-=m;
			}
			else
			{
				p16[i]=0;
			}
		}
		MW(ADDR_DATABUFF,ADDR_TerminalRealTimer,2+(TREALMTIMER_NUM*2));
	}
}


void Terminal_Timer(void)//�ն˷��ж���ͷֶ�ʱ��
{
	u32 i;
	u32 x;
	u32 t;
	u8 *p8;
	u16 *p16;

//�붨ʱ��
	t=Terminal_Ram->Terminal_S_Count;//�ն���Ӽ���,���������붨ʱ��
	if(t!=0x0)
	{
		//��3(�¼�)�����ʱ��
	  p16=(u16*)(ADDR_ERC10_Timer);
		for(i=0;i<EVENT_S_TNum;i++)
		{
			x=p16[i];
			if(x>t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p16[i]=x;
		}
		//ͳ����ԭʼ����
		//�ܼ��������С�й������ڳ�����ɺ�����붨ʱ��
		p8=(u8 *)ADDR_AFN0DF57F60_S_Timer;
		if(p8[0]>t)
		{
			p8[0]-=t;
		}
		else
		{
			p8[0]=0;
		}
		//�ն˵�ѹԽ���¼���S����ʱ��
		p8=(u8 *)(ADDR_AFN0DF27F35_SOUR+1);
		for(i=0;i<AFN0DF27MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			x=p8[8]+(p8[9]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[8]=x;
			p8[9]=x>>8;
			p8+=LEN_AFN0DF27F35_SOUR;
		}
		//�ն˵���Խ���¼���S����ʱ��
		p8=(u8 *)(ADDR_AFN0DF29F37_SOUR+1);
		for(i=0;i<AFN0DF29MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			x=p8[8]+(p8[9]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[8]=x;
			p8[9]=x>>8;
			x=p8[12]+(p8[13]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[12]=x;
			p8[13]=x>>8;
			p8+=LEN_AFN0DF29F37_SOUR;
		}
		//�ն˵�ѹ������ƽ���¼���S����ʱ��
		p8=(u8 *)(ADDR_AFN0DF28F36_SOUR+1);
		for(i=0;i<AFN0DF28MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			x=p8[4]+(p8[5]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[4]=x;
			p8[5]=x>>8;
			p8+=LEN_AFN0DF28F36_SOUR;
		}
		//�ն����ڹ���Խ���¼���S����ʱ��
		p8=(u8 *)(ADDR_AFN0DF30F38_SOUR+1);
		for(i=0;i<AFN0DF30MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			if(x>=t)
			{
				x-=t;
			}
			else
			{
				x=0;
			}
			p8[0]=x;
			p8[1]=x>>8;
			p8+=LEN_AFN0DF30F38_SOUR;
		}
		//г��Խ����Ӽ��������
#if MeterHarmonic!=0//г������0=û,1=��
#if IC_SDRAM
	  p8=(u8*)(ADDR_ERC15_SOUR);
		for(i=0;i<AFN0CF57MaxPn;i++)
		{
			x=p8[0]+(p8[1]<<8);
			x+=t;
			p8[0]=x;
			p8[1]=x>>8;
			p8+=LEN_ERC15_SOUR;
		}
#endif
#endif	
		
	  __disable_irq();
		Terminal_Ram->Terminal_S_Count-=t;
	  __enable_irq();
		
	}
	
//�ֶ�ʱ��
	t=Terminal_Ram->Terminal_M_Count;//�ն˷ּӼ���,���������ֶ�ʱ��
	if(t!=0x0)
	{
		//�ն����¹���ʱ��
		i=MRR(ADDR_AFN0DF49_SOUR,2);
		i+=t;
		MWR(i,ADDR_AFN0DF49_SOUR,2);//д�Ĵ������ݵ�������,Byte<=8
		i=MRR(ADDR_AFN0DF51_SOUR,2);
		i+=t;
		MWR(i,ADDR_AFN0DF51_SOUR,2);//д�Ĵ������ݵ�������,Byte<=8
	
		//ͳ����ԭʼ����
		//�й�����Ϊ��ʱ��ָ��¼Ӽ���
	  p8=(u8*)(ADDR_AFN0DF25F33_SOUR);
		for(i=0;i<AFN0DF25MaxPn;i++)
		{
			p8[i]+=t;
		}
		//�ܼ����й�����Ϊ��ʱ��ָ��¼Ӽ���
#if AFN0DF57MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF57F60_SOUR);
		for(i=0;i<AFN0DF57MaxPn;i++)
		{
			p8[i]+=t;
		}
#endif
		//���µ�ѹͳ��
	  p8=(u8*)(ADDR_AFN0DF27F35_SOUR);
		for(i=0;i<AFN0DF27MaxPn;i++)
		{
			p8[i*LEN_AFN0DF27F35_SOUR]+=t;
		}
		//���µ���ͳ��
	  p8=(u8*)(ADDR_AFN0DF29F37_SOUR);
		for(i=0;i<AFN0DF29MaxPn;i++)
		{
			p8[i*LEN_AFN0DF29F37_SOUR]+=t;
		}
		//��ѹ������ƽ��
	  p8=(u8*)(ADDR_AFN0DF28F36_SOUR);
		for(i=0;i<AFN0DF28MaxPn;i++)
		{
			p8[i*LEN_AFN0DF28F36_SOUR]+=t;
		}
		//���ڹ���Խ��
	  p8=(u8*)(ADDR_AFN0DF30F38_SOUR);
		for(i=0;i<AFN0DF30MaxPn;i++)
		{
			p8[i*LEN_AFN0DF30F38_SOUR]+=t;
		}
		//�ܼ��鳬���ʶ�ֵ�µ�������ֵ
#if AFN0DF65MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF65F66_SOUR);
		for(i=0;i<AFN0DF65MaxPn;i++)
		{
			p8[i*9]+=t;
		}
#endif
		//�������������ۼ�ʱ��
	  p8=(u8*)(ADDR_AFN0DF43F44_SOUR);
		for(i=0;i<AFN0DF43MaxPn;i++)
		{
			p8[i]+=t;
		}
		//ֱ��ģ����Խ���ۼ�ʱ��
#if AFN0DF129MaxPn!=0
	  p8=(u8*)(ADDR_AFN0DF129F130_SOUR);
		for(i=0;i<AFN0DF129MaxPn;i++)
		{
			p8[i*LEN_AFN0DF129F130_SOUR]+=t;
		}
#endif	
		
		
	  __disable_irq();
		Terminal_Ram->Terminal_M_Count-=t;
	  __enable_irq();
	}
}









