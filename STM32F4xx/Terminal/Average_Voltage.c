
//�ڲ�����1����ƽ����ѹ����
#include "Project.h"
#include "Average_Voltage.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Display/DisplayLoop_ACSample.h"



void Average_Voltage(void)//�ڲ�����1����ƽ����ѹ��������߶���
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	
	if(Terminal_Ram->RMDelay_S_Timer!=0)//�ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
	{
		return;
	}
	p8=(u8*)(ADDR_Average_Count);//�ڲ�����1����ƽ����ѹֵ�������
	if(p8[0]!=Comm_Ram->TYMDHMS[0])
	{//�����
		p8[0]=Comm_Ram->TYMDHMS[0];
		GetACSample(0x0201ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
		for(i=0;i<3;i++)
		{
			x=MRR(Get_ADDR_UARTnTx(ACSAMPLEPORT)+14+(2*i),2);
			x=bcd_hex(x);
			y=MRR(ADDR_Average_Count+3+(3*i),3);
			x+=y;
			MWR(x,ADDR_Average_Count+3+(3*i),3);
		}
		p8[2]++;//ƽ��ֵ����Ӵ�������+1
		if(p8[1]!=Comm_Ram->TYMDHMS[1])
		{//�ָ���
			p8[1]=Comm_Ram->TYMDHMS[1];
			i=p8[2];//ƽ��ֵ����Ӵ�������
			if(i!=0)
			{//ƽ���Ӵ�����Ϊ0
				for(i=0;i<3;i++)
				{
					x=MRR(ADDR_Average_Count+3+(3*i),3);
					x/=p8[2];
					x=hex_bcd(x);
					MWR(x,ADDR_Average_Voltage+(2*i),2);
				}
			#if MeterPW==1//��ʼ���߷�ʽ;0=��������,1=��������
				MC(0xee,ADDR_Average_Voltage+2,2);//��������B��Ϊ0xeeֵ
			#endif
			}
			MC(0,ADDR_Average_Count+2,10);//��Ӽ���
	#if MaxACSampleCurveCongeal!=0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
			//1�������߶���
			MC(0,ADDR_DATABUFF,7);
			p8=(u8*)(ADDR_DATABUFF);
			p8[3]=1;//00��01��01��00ʱ00��00��
			p8[4]=1;
			MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,5);//������ʱ��0��
			i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
			i/=60;
			i%=MaxACSampleCurveCongeal;//ƫ�Ƶ���
			i*=LEN_ACSampleCurve;//ƫ�Ƶ�ַ
			i+=ADDR_ACSampleCurve;//��ַ
			x=(i+11)&0xfff;
			if((x!=0)&&(x<=11))
			{//�������տ�ʼд��
				//EFLASH_Sector_Erase(i+11);//��������,���R0=Ҫ�����������ڱ������е������ַ(ÿ����2KWORD)
			}
			MR(ADDR_DATABUFF,i,11);
			for(x=0;x<11;x++)
			{
				if(p8[x]!=0xff)
				{
					break;
				}
			}
			if(x!=11)
			{//û����
				x=DataComp(ADDR_DATABUFF,ADDR_TYMDHMS+1,5);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				if(x==0)
				{//��ͬʱ���ظ�д
					MR(ADDR_DATABUFF+5,ADDR_Average_Voltage,6);
					MW(ADDR_DATABUFF,i,11);
					return;
				}
				//EFLASH_Sector_Erase(i);//��������,���R0=Ҫ�����������ڱ������е������ַ(ÿ����2KWORD)
			}
			MR(ADDR_DATABUFF,ADDR_TYMDHMS+1,5);
			MR(ADDR_DATABUFF+5,ADDR_Average_Voltage,6);
			DMW(ADDR_DATABUFF,i,11);
	#endif
		}
	}
}








