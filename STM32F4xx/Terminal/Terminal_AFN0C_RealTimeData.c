
//�ն���1���ݼ���
#include "Project.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_impIn.h"
#include "Terminal_SwitchIn.h"
#include "Terminal_AFN0D_CongealData_Init.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "../Terminal/Terminal_USER5xx.h"


extern u8 ADDR_AFN09F1[];//�ն˰汾��Ϣ
extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];
extern Terminal_HourCongealLib_TypeDef	Terminal_HourCongealLib[];


void Init_AFN0CF1(void)//��ʼ���ն���1����F1,�汾��Ϣ
{
	u32 * p32;
	
	MR(ADDR_DATABUFF,((u32)ADDR_AFN09F1)+12,4);
	MR(ADDR_DATABUFF+4,ADDR_AFN0CF1+12,4);
	p32=(u32 *)(ADDR_DATABUFF);
	if(p32[0]!=p32[1])
	{//�汾���
		ERC1_Event(2);//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
	}
	MR(ADDR_DATABUFF,(u32)ADDR_AFN09F1,30);
	MR(ADDR_DATABUFF+4,ADDR_TERMINAL+2,8);//8�ֽ��豸���ASCII
	MW(ADDR_DATABUFF,ADDR_AFN0CF1,30);
}

void Init_Class1Data_UpPower(void)//��1���ݳ�ʼ��(�ϵ�)
{
	u32 i;
	u32 Fn;
	u8 * p8;

	for(Fn=1;Fn<Get_InitClass1Data_MaxFn();Fn++)
	{
		i=Terminal_Class1DataLib[Fn].Addr;
		if(i!=0x0)
		{
			if((Fn>=81)&&(Fn<=121))
			{//Сʱ��������
				i-=ADDR_HOURCONGEAL_START;
				i+=ADDR_HOURCONGEAL_BUFF_START;
			}
			if(Terminal_Class1DataLib[Fn].Init==0x0)
			{
				if(Terminal_Class1DataLib[Fn].PnType==0x0)
				{
					MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen);
				}
				else
				{
					MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen*Terminal_Class1DataLib[Fn].PnMax);
				}
			}
		}
	}
	p8=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
	p8[0]=0xee;//Сʱ����ʱ��=0xee
	p8[1]=0;//Сʱ�����ܶ�=0(������)
	Init_AFN0CF1();//��ʼ���ն���1����F1�汾��Ϣ

//���г�����Ϣ
	p8=(u8 *)(ADDR_AFN0CF11);//�ն˼��г���״̬��Ϣ
#if ((Project/100)==2)||((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	p8[0]=3;//�������ݿ����n	BIN	1(1-31)
	p8[1]=2;//�˿ں�2
	p8[19+1]=3;//�˿ں�3
	p8[38+1]=31;//�ն�ͨ�Ŷ˿ں�	BIN	1
	MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
#else
	p8[0]=2;//�������ݿ����n	BIN	1(1-31)
	p8[1]=2;//�˿ں�2
	p8[19+1]=3;//�˿ں�3
	MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
	p8+=19;
	MC(0xee,((u32)p8)+8,12);//��ʼʱ��,����ʱ��
#endif

}


void Init_Class1Data_Command(void)//��1���ݳ�ʼ��(����,ȫ�����)
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;

//��ԭ����
	p32=(u32 *)(ADDR_HourCongeal_AutoRW);//��Сʱ�����Զ�IOW
	p32[3]=0;
	MC(0,ADDR_HOURCONGEAL_START+1,1);//��Ч����
//�嶳�Ỻ��
	for(Fn=1;Fn<Get_InitClass1Data_MaxFn();Fn++)
	{
		i=Terminal_Class1DataLib[Fn].Addr;
		if(i!=0x0)
		{
			if((Fn>=81)&&(Fn<=121))
			{//Сʱ��������
				i-=ADDR_HOURCONGEAL_START;
				i+=ADDR_HOURCONGEAL_BUFF_START;
			}
			if(Terminal_Class1DataLib[Fn].PnType==0x0)
			{
				MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen);
			}
			else
			{
				MC(Terminal_Class1DataLib[Fn].InitValue,i,Terminal_Class1DataLib[Fn].PnLen*Terminal_Class1DataLib[Fn].PnMax);
			}
		}
	}
	p8=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
	p8[0]=0xee;//Сʱ����ʱ��=0xee
	p8[1]=0;//Сʱ�����ܶ�=0(������)
	Init_AFN0CF1();//��ʼ���ն���1����F1�汾��Ϣ
	Init_SwitchIn();//��ʼ���ն˿���������
	
	//�ճ�ʼ�����ʱ��������ն�ʱ�ӻ���0xee
	Terminal_AFN0CF2_Computer();//�ն���1����F2,�ն�����ʱ��
}


void Terminal_AFN0CF2_Computer(void)//�ն���1����F2,�ն�����ʱ��
{
	u32 i;
	u8 * p8;
	MR(ADDR_AFN0CF2,ADDR_TYMDHMS,6);
	p8=(u8 *)(ADDR_TYMDHMS+6);
	i=p8[0];
	if(i==0x0)
	{
		i=7;
	}
	i<<=5;
	p8=(u8 *)(ADDR_AFN0CF2+4);
	p8[0]|=i;
}

void Terminal_AFN0CF4_Computer(void)//�ն���1����F4,�ն�����ͨ��״̬
{
	u32 i;
	if(MRR(ADDR_AFN05F29,1)==0x55)//�����ֹ�ն������ϱ�
	{
		i=1;
	}
	else
	{
		i=2;
	}
	if(MRR(ADDR_AFN05F27,1)==0x55)//�����ֹ�ն�����վͨ��
	{
		i|=0x4;
	}
	else
	{
		i|=0x08;
	}
  MWR(i,ADDR_AFN0CF4,1);//�ն�ͨ��״̬
}


void Terminal_EnergyInc_Computer(u32 f,u32 SourAddr,u32 SourLen,u32 Fn,u32 DateIncAddr,u32 MonthIncAddr)//���������͵��յ��µ���������(������)
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	u8 * p8s;
	u8 * p8d;
#if ((AFN0CF41F42F43F44!=0)||(AFN0CF45F46F47F48!=0))
	u32 Class1DataAddr;
	u32 DateCongealAddr;
#endif

	MR(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);//�������־
	p8d=(u8 *)(ADDR_DATABUFF);
	if((p8d[0]&f)!=0x0)
	{//�Ѽ���
		return;
	}
	p8d[0]|=f;//��������������Ѽ����־
	p8d[1]=0;//�ܼ�������־=0
	MW(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);
	for(Pn=0;Pn<MaxRS485Pn;Pn++)
	{
		//������
		MC(0,ADDR_DATABUFF,1+(4*(TMaxTariff+1)));
		p8d[0]=TMaxTariff;//��������Ϊ���
		//��ԭʼʾֵ
		MR(ADDR_DATABUFF+1+(4*(TMaxTariff+1)),SourAddr+((1+((4+SourLen)*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),SourLen*(TMaxTariff+1));
		i=GetClass1DataAddr(Fn,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
		if(i!=0x0)
		{
			p8s=(u8 *)(i);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//��������
					break;
				}
				if(p8s[6+(SourLen*x)]==0xee)
				{//��ǰֵ=0xee
					continue;
				}
				//ȡԭʼֵ
				a64=MRR(ADDR_DATABUFF+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				//ȡ��ǰֵ
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				//����ԭʼֵ
				MWR(b64,ADDR_DATABUFF+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if((a64&0xff)==0xee)
				{//ԭʼֵ=��
					continue;
				}
				//��������
				a64=bcd16_hex(a64);
				b64=bcd16_hex(b64);
				if(b64>=a64)
				{//��ǰֵ>=ԭʼֵ
					b64-=a64;
				}
				else
				{
					if(SourLen==5)
					{
						if(a64>9000000000)
						{//��ǰֵ��0
							b64+=10000000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
					else
					{
						if(a64>90000000)
						{//��ǰֵ��0
							b64+=100000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
				}
				//��������
				if(SourLen==4)
				{
					b64*=100;//С�����4λ
				}
				MWR(b64,ADDR_DATABUFF+1+(4*x),4);
				//���������
				p8d[0]=p8s[5];
			}
		}
		//д����
		MW(ADDR_DATABUFF,SourAddr+((1+((4+SourLen)*(TMaxTariff+1)))*Pn),1+((4+SourLen)*(TMaxTariff+1)));
		//�����㵱�յ�����
#if AFN0CF41F42F43F44==0//���յ�����AFN0CF41F42F43F44����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
			//0=ʾֵʵʱ����
		MR(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		p8s=(u8 *)(ADDR_DATABUFF+512);
		p8d=(u8 *)(ADDR_DATABUFF);
		p8s[0]=p8d[0];//������
		for(x=0;x<=TMaxTariff;x++)
		{
			i=MRR(ADDR_DATABUFF+512+1+(4*x),4);//
			i=bcd_hex(i);
			i+=MRR(ADDR_DATABUFF+1+(4*x),4);//
			i=hex_bcd(i);
			MWR(i,ADDR_DATABUFF+512+1+(4*x),4);//
		}
		MW(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
#else
			//1=��ǰʾֵ-����0�㶳��
		i=MRR(ADDR_TYMDHMS+3,3);
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
		switch(Fn)
		{
			case 129://�����й�����
				Class1DataAddr=GetClass1DataAddr(129,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetDateCongealAddr(i,161,Pn+1);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 130://�����޹�����
				Class1DataAddr=GetClass1DataAddr(130,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetDateCongealAddr(i,162,Pn+1);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 131://�����й�����
				Class1DataAddr=GetClass1DataAddr(131,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetDateCongealAddr(i,163,Pn+1);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 132://�����޹�����
				Class1DataAddr=GetClass1DataAddr(132,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetDateCongealAddr(i,164,Pn+1);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
			default:
				Class1DataAddr=0;//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=0;//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
		}
		if((Class1DataAddr==0)||(DateCongealAddr==0))
		{
			MC(0,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			MC(TMaxTariff,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1);
		}
		else
		{
			p8d=(u8 *)(ADDR_DATABUFF+512);
			//��ԭ������
			MR(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			p8d[0]=TMaxTariff;//��������Ϊ���
			//���ն���ֵ
			MR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1)),DateCongealAddr+6,SourLen*(TMaxTariff+1));
			p8s=(u8 *)(Class1DataAddr);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//��������
					break;
				}
				//ȡԭʼֵ
				a64=MRR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if(((a64&0xff)==0xee)||((a64&0xff)==0xFF))
				{//ԭʼֵ=��
					continue;
				}
				//ȡ��ǰֵ
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				if(p8s[6+(SourLen*x)]==0xee)
				{//��ǰֵ=0xee
					continue;
				}
				//��������
				a64=bcd16_hex(a64);
				b64=bcd16_hex(b64);
				if(b64>=a64)
				{//��ǰֵ>=ԭʼֵ
					b64-=a64;
				}
				else
				{
					if(SourLen==5)
					{
						if(a64>9000000000)
						{//��ǰֵ��0
							b64+=10000000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
					else
					{
						if(a64>90000000)
						{//��ǰֵ��0
							b64+=100000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
				}
				//��������
				if(SourLen==4)
				{
					b64*=100;//С�����4λ
				}
				i=b64;
				i=hex_bcd(i);
				MWR(i,ADDR_DATABUFF+512+1+(4*x),4);
				//���������
				p8d[0]=p8s[5];
			}
			//д
			MW(ADDR_DATABUFF+512,DateIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		}
#endif
		//�����㵱�µ�����
#if AFN0CF45F46F47F48==0//���µ�����AFN0CF45F46F47F48����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
			//0=ʾֵʵʱ����
		MR(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		p8s=(u8 *)(ADDR_DATABUFF+512);
		p8d=(u8 *)(ADDR_DATABUFF);
		p8s[0]=p8d[0];//������
		for(x=0;x<=TMaxTariff;x++)
		{
			i=MRR(ADDR_DATABUFF+512+1+(4*x),4);//
			i=bcd_hex(i);
			i+=MRR(ADDR_DATABUFF+1+(4*x),4);//
			i=hex_bcd(i);
			MWR(i,ADDR_DATABUFF+512+1+(4*x),4);//
		}
		MW(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
#else
			//1=��ǰʾֵ-����0�㶳��
		i=MRR(ADDR_TYMDHMS+4,2);
		i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
		switch(Fn)
		{
			case 129://�����й�����
				Class1DataAddr=GetClass1DataAddr(129,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetMonthCongealAddr(i,177,Pn+1);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 130://�����޹�����
				Class1DataAddr=GetClass1DataAddr(130,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetMonthCongealAddr(i,178,Pn+1);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 131://�����й�����
				Class1DataAddr=GetClass1DataAddr(131,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetMonthCongealAddr(i,179,Pn+1);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
				break;
			case 132://�����޹�����
				Class1DataAddr=GetClass1DataAddr(132,Pn+1);//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=GetMonthCongealAddr(i,180,Pn+1);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
				break;
			default:
				Class1DataAddr=0;//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				DateCongealAddr=0;//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
				break;
		}
		if((Class1DataAddr==0)||(DateCongealAddr==0))
		{
			MC(0,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			MC(TMaxTariff,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1);
		}
		else
		{
			p8d=(u8 *)(ADDR_DATABUFF+512);
			//��ԭ������
			MR(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
			p8d[0]=TMaxTariff;//��������Ϊ���
			//���¶���ֵ
			MR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1)),DateCongealAddr+6,SourLen*(TMaxTariff+1));
			p8s=(u8 *)(Class1DataAddr);
			for(x=0;x<=TMaxTariff;x++)
			{
				if(x>p8s[5])
				{//��������
					break;
				}
				//ȡԭʼֵ
				a64=MRR(ADDR_DATABUFF+512+1+(4*(TMaxTariff+1))+(SourLen*x),SourLen);
				if(((a64&0xff)==0xee)||((a64&0xff)==0xFF))
				{//ԭʼֵ=��
					continue;
				}
				//ȡ��ǰֵ
				b64=MRR(((u32)p8s)+6+(SourLen*x),SourLen);
				if(p8s[6+(SourLen*x)]==0xee)
				{//��ǰֵ=0xee
					continue;
				}
				//��������
				a64=bcd16_hex(a64);
				b64=bcd16_hex(b64);
				if(b64>=a64)
				{//��ǰֵ>=ԭʼֵ
					b64-=a64;
				}
				else
				{
					if(SourLen==5)
					{
						if(a64>9000000000)
						{//��ǰֵ��0
							b64+=10000000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
					else
					{
						if(a64>90000000)
						{//��ǰֵ��0
							b64+=100000000;
							b64-=a64;
						}
						else
						{//����
							b64=0;
						}
					}
				}
				//��������
				if(SourLen==4)
				{
					b64*=100;//С�����4λ
				}
				i=b64;
				i=hex_bcd(i);
				MWR(i,ADDR_DATABUFF+512+1+(4*x),4);
				//���������
				p8d[0]=p8s[5];
			}
			//д
			MW(ADDR_DATABUFF+512,MonthIncAddr+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
		}
#endif
	}
}


void TotalAddEnergyInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//���������ܼӵ���������,���ֵ��DATABUFF
{
	u32 * p32;
	s64 d64a;
	s64 d64b;
	
	//��ÿ�ζ��Ὺʼʱֵ
	MR(ADDR_DATABUFF,SourAddr+(4*Pn),4);
	//����ǰֵ
	MR(ADDR_DATABUFF+4,CurrAddr+1+((1+(4*(1+TMaxTariff)))*Pn),4);
	//�浱ǰֵΪ��ʼʱֵ
	MW(ADDR_DATABUFF+4,SourAddr+(4*Pn),4);
	p32=(u32 *)(ADDR_DATABUFF);
	if(p32[0]==0xeeeeeeee)
	{//ÿ�ζ��Ὺʼʱֵ=0xee
		return;	
	}
	d64a=Data03_kWh(p32[0]);
	d64b=Data03_kWh(p32[1]);
	d64b-=d64a;
	p32[0]=kWh_Data03(d64b);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
}

void PointkWhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//��������������й�����������,���ֵ��DATABUFF
{
	u64 d64a;
	u64 d64b;
	u8 * p8;
	//��ÿ�ζ��Ὺʼʱֵ
	MR(ADDR_DATABUFF,SourAddr+(5*Pn),5);
	//����ǰֵ
	MR(ADDR_DATABUFF+5,CurrAddr+6+((6+(5*(1+TMaxTariff)))*Pn),5);
	//�浱ǰֵΪ��ʼʱֵ
	MW(ADDR_DATABUFF+5,SourAddr+(5*Pn),5);

	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0xee)
	{
		return;
	}
	if(p8[5]==0xee)
	{
		MC(0xee,ADDR_DATABUFF,5);
		return;
	}
	d64a=MRR(ADDR_DATABUFF,5);//
	d64b=MRR(ADDR_DATABUFF+5,5);//
	d64a=bcd16_hex(d64a);
	d64b=bcd16_hex(d64b);
	if(d64b<d64a)
	{//���ܲ�����ֵ��0
		d64b+=10000000000;
	}
	d64b-=d64a;
	d64b=hex_bcd16(d64b);
	MWR(d64b,ADDR_DATABUFF,4);//
}

void PointkvarhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn)//���������޹�����������,���ֵ��DATABUFF
{
	u32 i;
	u32 * p32;
	//��ÿ�ζ��Ὺʼʱֵ
	MR(ADDR_DATABUFF,SourAddr+(4*Pn),4);
	//����ǰֵ
	MR(ADDR_DATABUFF+4,CurrAddr+6+((6+(4*(1+TMaxTariff)))*Pn),4);
	//�浱ǰֵΪ��ʼʱֵ
	MW(ADDR_DATABUFF+4,SourAddr+(4*Pn),4);

	p32=(u32 *)(ADDR_DATABUFF);
	i=0xeeeeeeee;
	if(p32[0]==i)
	{
		return;
	}
	if(p32[1]==i)
	{
		p32[0]=i;
		return;
	}
	p32[0]=bcd_hex(p32[0]);
	p32[1]=bcd_hex(p32[1]);
	if(p32[1]<p32[0])
	{//���ܲ�����ֵ��0
		p32[1]+=100000000;
	}
	p32[1]-=p32[0];
	p32[1]=hex_bcd(p32[1]);
	p32[0]=p32[1]<<8;//2λС����4λС��
}



void Terminal_HourCongeal(void)//Сʱ����
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 n;
	u32 m;

	u8 * p8;
	u8 * p8d;
	u32 * p32;

	if(Terminal_Ram->CongealFlags&1)
	{
		return;
	}
	Terminal_Ram->CongealFlags|=1;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	p8=(u8 *)(ADDR_TYMDHMS);
	i=bcd_hex(p8[1]);
	switch(MaxHourCongeal)//���Сʱ�������ݸ���(�ܶ�)0=������;4=15,30,45,0;2=30,0;1=0;12=5,10,15...0;60=1,2,3...0
	{
		case 0://������
			return;
		case 1://00�ֶ���
			m=3;
			if(i==0x0)
			{
				n=0;
				break;
			}
			return;
		case 2://30,0�ֶ���
			m=2;
			if(i==30)
			{
				n=0;
				break;
			}
			if(i==0x0)
			{
				n=1;
				break;
			}
			return;
		case 4://15,30,45,0�ֶ���
			m=1;
			if((i%15)==0x0)
			{
				if(i==0x0)
				{
					n=3;
					break;
				}
				else
				{
					n=i/15;
					n-=1;
					break;
				}
			}
			return;
		case 12://5,10,15...0�ֶ���
			m=254;
			if((i%5)==0x0)
			{
				if(i==0x0)
				{
					n=11;
					break;
				}
				else
				{
					n=i/5;
					n-=1;
					break;
				}
			}
			return;
		case 60://1,2,3...0�ֶ���
			m=255;
			if(i==0x0)
			{
				n=59;
			}
			else
			{
				n=(i-1);
			}
			break;
	}
	if(MRR(ADDR_TYMDHMS+1,5)==MRR(ADDR_TCongealCount+2,5))
	{//�Ѷ���
		return;
	}
	//����
	MW(ADDR_TYMDHMS+1,ADDR_TCongealCount+2,5);//д�ϴζ���ʱ��
//	MR(ADDR_DATABUFF,ADDR_TCongealCount,1);
//	p8=(u8 *)(ADDR_DATABUFF);
//	p8[0]|=0x1;//�ո��º󶳽�״̬B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���
//	MW(ADDR_DATABUFF,ADDR_TerminalCongealCount,1);
	
	for(Fn=0;Fn<Get_HourCongeal_MaxFn();Fn++)
	{
		if(Terminal_HourCongealLib[Fn].PnType==0x0)
		{
			i=Terminal_HourCongealLib[Fn].DestAddr+(Terminal_HourCongealLib[Fn].DestPnLen*n);
			MR(i,Terminal_HourCongealLib[Fn].SourAddr,Terminal_HourCongealLib[Fn].DestPnLen);
		}
		else
		{
			if(Terminal_HourCongealLib[Fn].PnMax!=0x0)
			{
				for(Pn=0;Pn<Terminal_HourCongealLib[Fn].PnMax;Pn++)
				{
					switch(Fn)
					{
						case (3-1)://83//�ܼ��й��ܵ�����
							TotalAddEnergyInc_Computer(ADDR_AFN0CF83_SOUR,ADDR_AFN0CF19,Pn);//Сʱ�����ܼӵ���������,���ֵ��DATABUFF
							break;
						case (4-1)://84//�ܼ��޹��ܵ�����
							TotalAddEnergyInc_Computer(ADDR_AFN0CF84_SOUR,ADDR_AFN0CF20,Pn);//Сʱ�����ܼӵ���������,���ֵ��DATABUFF
							break;
						case (20-1)://105//�����й��ܵ�����
							PointkWhInc_Computer(ADDR_AFN0CF105_SOUR,ADDR_AFN0CF129,Pn);//Сʱ����������й�����������,���ֵ��DATABUFF
							break;
						case (21-1)://106//�����޹��ܵ�����
							PointkvarhInc_Computer(ADDR_AFN0CF106_SOUR,ADDR_AFN0CF130,Pn);//Сʱ����������޹�����������,���ֵ��DATABUFF
							break;
						case (22-1)://107//�����й��ܵ�����
							PointkWhInc_Computer(ADDR_AFN0CF107_SOUR,ADDR_AFN0CF131,Pn);//Сʱ����������й�����������,���ֵ��DATABUFF
							break;
						case (23-1)://108//�����޹��ܵ�����
							PointkvarhInc_Computer(ADDR_AFN0CF108_SOUR,ADDR_AFN0CF132,Pn);//Сʱ����������޹�����������,���ֵ��DATABUFF
							break;
					}
					i=Terminal_HourCongealLib[Fn].DestAddr+(Terminal_HourCongealLib[Fn].DestPnLen*MaxHourCongeal*Pn)+(Terminal_HourCongealLib[Fn].DestPnLen*n);
					if(Terminal_HourCongealLib[Fn].SourAddr==ADDR_DATABUFF)
					{
						MR(i,ADDR_DATABUFF,Terminal_HourCongealLib[Fn].DestPnLen);
					}
					else
					{
						MR(i,Terminal_HourCongealLib[Fn].SourAddr+(Terminal_HourCongealLib[Fn].SourPnLen*Pn),Terminal_HourCongealLib[Fn].DestPnLen);
					}
				}
			}
		}
	}
	p8=(u8 *)(ADDR_TYMDHMS);
	if(p8[1]==0x0)
	{//0�ֶ���,ת��EFLASH
		p8d=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
		i=p8[2];//����ʱ��ʱ(��ʾ��һ�����Сʱʱ�䣬��ֵ��Χ0��23)
		i=bcd_hex(i);
		if(i==0)
		{
			i=23;
		}
		else
		{
			i--;
		}
		i=hex_bcd(i);
		p8d[0]=i;
		p8d[1]=m;//�����ܶ�
		p32=(u32 *)(ADDR_HourCongeal_AutoRW);
		p32[0]=2;//2=IOW
		p32[1]=ADDR_HOURCONGEAL_BUFF_START;
		p32[2]=ADDR_HOURCONGEAL_START;
		p32[3]=(ADDR_HOURCONGEAL_BUFF_END)-(ADDR_HOURCONGEAL_BUFF_START);
		Terminal_Ram->CongealFlags&=0xfe;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	}

}

void HourCongeal_AutoRW(void)//Сʱ�����Զ�IO��д����0,��ɺ�������
{
	u32 i;
	u32 x;

	u8 * p8;
	u8 * p8d;
	u32 * p32;
	
	p32=(u32 *)(ADDR_HourCongeal_AutoRW);
	if(p32[3]!=0x0)
	{
    //i=Get_LEN_FLASHSectorWrite(p32[2]);//�õ�FLASH��ǰ��ַ������д�ֽڳ���
		i=(128*1024)-(p32[2]&0x1ffff);//��ǰ����д�ֽ�
		if(i>p32[3])
		{
			i=p32[3];
		}
		p8=(u8 *)(ADDR_HourCongeal_AutoRW);
		switch(p8[0])
		{
			case 0://IOCLR
				MC(p8[1],p32[2],i);
				break;
			case 1://IOR
				MR(p32[1],p32[2],i);
				break;
			case 2://IOW
				p8d=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);//д������ܶ���=0������Ч
				x=p8d[1];
				MW(p32[1],p32[2],i);
				p8d[1]=x;//�ָ��ܶ�ֵ
				break;
			default:
				p32[3]=0;
				return;
		}
		p32[1]+=i;
		p32[2]+=i;
		p32[3]-=i;
		if(p32[3]==0x0)
		{//����=0,��ɺ���
			PnHourCongealCount();//�������Сʱ�������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
			MW(ADDR_HOURCONGEAL_BUFF_START+1,ADDR_HOURCONGEAL_START+1,1);//������Ч(д�����ܶ�)
			Terminal_Ram->CongealFlags|=1;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
		}
	}
}






void Terminal_AFN0CF17_F18_Computer(u32 f)//�ն���1����F17F18,��ǰ�ܼӹ���,���=0�й�=1�޹�
{
#if MaxTotalAddGroup!=0	
	u32 i;
	u32 x;
	u32 y;
	u32 AddPn;
	u32 Pn;
	u64 d64;
	u64 x64;
	u64 y64;
	u8 * p8;

	for(i=0;i<MaxTotalAddGroup;i++)
	{	
		d64=0;
		AddPn=MRR(ADDR_AFN04F14+(66*i)+1,1);
		if(AddPn<=MaxRS485User)//�ܼ���Ĳ���������<=��������������
		{
			for(x=0;x<AddPn;x++)
			{
        Pn=MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x3f;
				if(Pn>=AFN04F25MaxPn)
				{//������ų�
					continue;
				}
        x64=MRR(ADDR_AFN04F25+(11*Pn),2);
				y64=MRR(ADDR_AFN04F25+(11*Pn)+2,2);
				x64*=y64;
				y=GetClass1DataAddr(25,(Pn+1));//ȡ��1�����㳭�����ݴ�����ַ,����0��ʾû�ҵ�
				if(y!=0x0)
				{
					p8=(u8 *)(y+5);
					if(f!=0x0)
					{//�޹��ܼ�
						p8+=12;
					}
					if((MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x40)==0x0)
					{//����
						if((p8[2]&0x80)!=0x0)
						{//�����
							continue;
						}
					}
					else
					{//����
						if((p8[2]&0x80)==0x0)
						{//�����
							continue;
						}
					}
					if(p8[0]==0xee)
					{
						continue;//û������,����Ϊ��С����ֵ
					}
					else
					{
						y=p8[0]+(p8[1]<<8)+((p8[2]&0x7f)<<16);
						y=bcd_hex(y);
						y64=y;
					}
					x64*=y64;
					if((MRR(ADDR_AFN04F14+(66*i)+2+x,1)&0x80)==0x0)
					{//��
						d64+=x64;
					}
					else
					{//��
						d64-=x64;
					}
				}
			}
		}
		y=d64>>63;
		if(y==0x0)
		{//���Ϊ��
			d64/=10;//4λС��ȥ1λΪ3λС��
		}
		else
		{//���Ϊ��
			d64=(~d64)+1;
			d64/=10;//4λС��ȥ1λΪ3λС��
			d64=(~d64)+1;
		}
		x=kW_Data02(d64);//��λͳһΪkw��3λС����hex����תΪ���ݸ�ʽ02
		
		if(f==0x0)
		{
			p8=(u8 *)(ADDR_AFN0CF17+(2*i));
		}
		else
		{
			p8=(u8 *)(ADDR_AFN0CF18+(2*i));
		}
		p8[0]=x;
		p8[1]=x>>8;
	}
#endif
}

void Terminal_TotalEnergy_Computer(u32 f,u32 PSourAddr,u32 NSourAddr,u32 RemainderAddr,u32 TotalAddr)//�ܼӵ���������
{
#if MaxTotalAddGroup==0
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x0f;//�ܼ����������ʹ�ñ�־,D0-D5�ֱ��ʾ�����й�,�����޹�,�����й�,�����޹�,ERC22��¼��������,ʣ�����
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64;
	u8 * p8;
	u32 * p32s;
	u32 * p32d;
	u64 * p64;
	u32 TariffMax;
	
	TariffMax=4;//�ܼ���������̶�Ϊ4
	MR(ADDR_DATABUFF,ADDR_Inc_User_Flag,2);//�������־
	p8=(u8 *)(ADDR_DATABUFF);
	if((p8[0]&0x0f)!=0x0f)
	{//����������û����
		return;
	}
	if((p8[1]&0x3f)==0x3f)
	{//ȫ�Ѽ���
		MC(0,ADDR_Inc_User_Flag,2);
		return;
	}
	if((p8[1]&f)!=0x0)
	{//�Ѽ���
		return;
	}
	p8[1]|=f;
	MW(ADDR_DATABUFF+1,ADDR_Inc_User_Flag+1,1);
	p64=(u64 *)(ADDR_DATABUFF);

	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)//�ܼ������
	{
		//��ԭ�ܼӵ���
		MR(ADDR_DATABUFF+512,TotalAddr+1+((1+(4*(1+TMaxTariff)))*Pn),(4*(1+TMaxTariff)));
		//ԭ�ܼӵ���תΪ8�ֽڵ�HEX,ͳһΪKWH
		p32s=(u32 *)(ADDR_DATABUFF+512);
		for(i=0;i<=TariffMax;i++)
		{
			d64=bcd_hex(p32s[i]&0x0fffffff);
			if((p32s[i]&0x40000000)!=0x0)
			{//mwh
				d64*=1000;
			}
			if((p32s[i]&0x10000000)!=0x0)
			{//��
				d64=(~d64)+1;
			}
			p64[i]=d64;
		}
		//��ԭ�ܼӵ�����С��λ(hex����)
		MR(ADDR_DATABUFF+512,RemainderAddr+(4*(1+TMaxTariff)*Pn),4*(1+TMaxTariff));
		//���������ܼ�
		p32d=(u32 *)(ADDR_DATABUFF+1024);
		for(x=0;x<MRR(ADDR_AFN04F14+((2+64)*Pn)+1,1);x++)//�ܼ���Ĳ���������
		{
			i=MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x3f;//�������0-63��ʾ1-64
			if(i>=MaxRS485Pn)
			{
				continue;
			}
			if((MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x40)==0x0)
			{//����
				if(PSourAddr==ADDR_PKWH_INC)
				{
					MR(ADDR_DATABUFF+1024,PSourAddr+1+((1+(9*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
				else
				{
					MR(ADDR_DATABUFF+1024,PSourAddr+1+((1+(8*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
			}
			else
			{//����
				if(NSourAddr==ADDR_NKWH_INC)
				{
					MR(ADDR_DATABUFF+1024,NSourAddr+1+((1+(9*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
				else
				{
					MR(ADDR_DATABUFF+1024,NSourAddr+1+((1+(8*(1+TMaxTariff)))*i),(4*(1+TMaxTariff)));
				}
			}
      y=MRR(ADDR_AFN04F25+(11*i),2);
			i=MRR(ADDR_AFN04F25+(11*i)+2,2);
			i*=y;//��ѹ����������*��������������
			for(y=0;y<=TariffMax;y++)
			{
				d64=p32d[y];//ȡ����
				d64*=i;//����*����
				if((MRR(ADDR_AFN04F14+((2+64)*Pn)+2+x,1)&0x80)!=0x0)
				{//��������ֵ
					d64=(~d64)+1;
				}
				if((p32s[y]&0x80000000)==0x0)
				{//ԭ������
					d64+=p32s[y];
				}
				else
				{//ԭ������
					d64-=((~p32s[y])+1);
				}	
				if((d64>>63)==0x0)
				{//����Ϊ��
					p64[y]+=d64/10000;
					p32s[y]=d64%10000;
				}
				else
				{//����Ϊ��
					d64=(~d64)+1;
					p64[y]-=d64/10000;
					p32s[y]=(~(d64%10000))+1;
				}
			}
		}
		//�ܼӵ���תΪ4�ֽ�BCD��ʽ
		for(y=0;y<=TariffMax;y++)
		{
			d64=p64[y];
			if((d64>>63)==0x0)
			{//��
				f=0;
			}
			else
			{//��
				f=1;
				d64=(~d64)+1;
			}
			if(d64>=10000000)
			{//MWH
				i=d64%1000;
				d64/=1000;
				if(f==0x0)
				{
					p32s[y]+=(i*10000);
				}
				else
				{
					p32s[y]-=(i*10000);
				}
				f|=0x4;
			}
			i=hex_bcd(d64);
			i|=f<<28;
			p32d[y]=i;
		}
		p8=(u8 *)(ADDR_DATABUFF+(1024-1));
		p8[0]=TariffMax;//������
		//д�ܼӵ���
		MW(ADDR_DATABUFF+(1024-1),TotalAddr+((1+(4*(1+TMaxTariff)))*Pn),1+(4*(1+TMaxTariff)));
		//дԭ�ܼӵ�����С��λ(hex����)
		MW(ADDR_DATABUFF+512,RemainderAddr+(4*(1+TMaxTariff)*Pn),4*(1+TMaxTariff));
	}
#endif
}

void ERC22_TotalEnergy_Computer(void)//ERC22��¼���ʱ�����ܼӵ�������
{
#if (MaxTotalAddGroup==0)||(MaxDiffGroup==0)
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x10;//�ܼ����������ʹ�ñ�־,D0-D5�ֱ��ʾ�����й�,�����޹�,�����й�,�����޹�,ERC22��¼��������,ʣ�����
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 Fn;
	u32 Pn;
	u64 a64;
	u64 b64;
	u32 * p32;
	
	//���ܼӵ������������
	p32=(u32 *)(ADDR_DATABUFF+2048);
	for(i=0;i<(2048/4);i++)
	{
		p32[i]=0;
	}
	Terminal_TotalEnergy_Computer(0x10,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_ERC22_Remainder,ADDR_DATABUFF+2048);//�ܼӵ�����������

	for(Fn=0;Fn<MaxDiffGroup;Fn++)
	{
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Fn)+(4*Pn),4);//��ԭ�ܼӵ��� ���ݸ�ʽ03
			a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			i=MRR(ADDR_DATABUFF+2048+1+((1+4+(4*TMaxTariff))*Pn),4);
			i&=0xefffffff;//ȥ����,��������ֵ
			b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			a64+=b64;
			a64=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
			MWR(a64,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Fn)+(4*Pn),4);//д�Ĵ������ݵ�������,Byte<=8
		}
	}
#endif
}


//(ADC20MA-ADC4MA)*Gain=(ֱ��ģ����������ֵֹ-ֱ��ģ����������ʼֵ)
//Gain=(ֱ��ģ����������ֵֹ-ֱ��ģ����������ʼֵ)/(ADC20MA-ADC4MA)
void Terminal_AFN0CF73_Computer(void)//ֱ��ģ��������
{
#if (AFN0CF73MaxPn!=0)
	u32 i;
	u32 ADC4MA;//������4MA����ʱADCת��ֵ
	u32 ADC20MA;//������20MA����ʱADCת��ֵ
	s64 x;
	s64 y;
	double Gain;
	
//	ADC4MA=0;//������4MA����ʱADCת��ֵ
//	ADC20MA=0xfff;//������20MA����ʱADCת��ֵ
	ADC4MA=MRR(ADDR_ADC4MA,2);
	ADC20MA=MRR(ADDR_ADC20MA,2);
	
	i=MRR(ADDR_AFN04F61,1);//ֱ��ģ�����������
	i&=1;
	if(i==0)
	{
		MC(0xee,ADDR_AFN0CF73,2);
	}
	else
	{//����
		x=MRR(ADDR_AFN04F81,2);//ֱ��ģ����������ʼֵ
		y=MRR(ADDR_AFN04F81+2,2);//ֱ��ģ����������ֵֹ
		x=Data02_kW((u32)x);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		y=Data02_kW((u32)y);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
		Gain=y-x;
		i=ADC20MA-ADC4MA;
		if(i==0)
		{//��
			MC(0xee,ADDR_AFN0CF73,2);
			return;
		}
		else
		{
			if(ADC20MA<ADC4MA)
			{//ADC20MA<ADC4MA
				MC(0xee,ADDR_AFN0CF73,2);
				return;
			}
			else
			{
				Gain/=i;
			}
		}
		i=MRR(ADDR_UDC,2);
		if(i<ADC4MA)
		{
			i=ADC4MA;
		}
		if(i>ADC20MA)
		{
			i=ADC20MA;
		}
		i-=ADC4MA;
		Gain*=i;
		y=(s64)Gain;
		y+=x;
		i=kW_Data02((u64)y);//��λͳһΪkw��3λС����hex����תΪ���ݸ�ʽ02
		MWR(i,ADDR_AFN0CF73,2);
	}
#endif
}



void Terminal_AFN0CF5_Computer(void)//�ն���1����F5,�ն˿�������״̬
{
	u32 i;
	u8 * p8s;
	u8 * p8d;
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=	
	u32 x;
#endif

	i=0;
	p8s=(u8 *)(ADDR_Terminal_Protect);//�ն˱���
	if(p8s[0]!=0x0)
	{
		i|=1;
	}
	if(MRR(ADDR_AFN05F28,1)==0x55)//�ն��޳�Ͷ����
	{
		i|=2;
	}
	if(MRR(ADDR_AFN05F26,1)==0x55)//�߷Ѹ澯Ͷ����
	{
		i|=4;
	}
	p8d=(u8 *)(ADDR_AFN0CF5);//�ն˿�������״̬
	p8d[0]=i;
	p8d[1]=0;//�ܼ�����Ч��־λ
	
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=	
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		p8d[1]=(p8d[1]<<1)+0x1;
	}
	p8d+=2;
	for(i=0;i<MaxTotalAddGroup;i++)
	{
		p8d[0]=MRR(ADDR_AFN05F9+(3*i)+2,1);//���ض�ֵ������
		p8d[1]=MRR(ADDR_AFN05F9+(3*i)+1,1);//����ʱ����Ч��־λ
		p8d[2]=0;//����״̬:D0ʱ�ο�,D1���ݿ�,D2Ӫҵ��ͣ��,D3��ǰ�����¸���
		if(MRR(ADDR_AFN05F9+(3*i),1)==0x55)
		{//ʱ�ο�
			p8d[2]|=1;
		}
		if(MRR(ADDR_AFN05F10+(1*i),1)==0x55)
		{//���ݿ�
			p8d[2]|=2;
		}
		if(MRR(ADDR_AFN05F11+(1*i),1)==0x55)
		{//Ӫҵ��ͣ��
			p8d[2]|=4;
		}
		x=MRR(ADDR_AFN05F12+(9*i),1);
		if((x==0x55)||(x==0x5a))
		{//��ǰ�����¸���
			p8d[2]|=8;
		}
		p8d[3]=0;//���״̬:D0�µ��,D1�����
		if(MRR(ADDR_AFN05F15+(1*i),1)==0x55)
		{//�µ��
			p8d[3]|=1;
		}
		if(MRR(ADDR_AFN05F16+(1*i),1)==0x55)
		{//�����
			p8d[3]|=2;
		}
		p8d[4]=MRR(ADDR_AFN04F45+(1*i),1);//�����ִ�״̬
		p8d[5]=MRR(ADDR_AFN04F48+(1*i),1);//����ִ�״̬		
		p8d+=6;
	}
#endif
}

void Terminal_AFN0CF6_Computer(void)//�ն���1����F6,�ն˵�ǰ����״̬
{
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=	
	u32 i;
	u32 Pn;
	u8 * p8s;
	u8 * p8d;

	p8d=(u8 *)(ADDR_AFN0CF6);
//ң����բ���״̬
	p8s=(u8 *)(ADDR_Terminal_Remote_State);
	p8d[0]=0;
	for(i=0;i<MaxControlRound;i++)
	{
		if(p8s[0]>=102)
		{
			p8d[0]|=1<<i;
		}
		p8s+=Terminal_Ctrl_Pn_Len;
	}
//��ǰ�߷Ѹ澯״̬
	p8s=(u8 *)(ADDR_Terminal_Attr_State);
	p8d[1]=p8s[0];
//�ܼ�����Ч��־λ
	p8d[2]=0;
	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		p8d[2]=(p8d[2]<<1)+0x1;
	}
	p8d+=3;
	for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
	{
		//��ǰ���ض�ֵ
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]!=0x0)
		{
			MR((u32)p8d,ADDR_kWCtrl1_FixedValue+(Pn*2),2);
		}
		else
		{
			p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			if(p8s[0]!=0x0)
			{
				MR((u32)p8d,ADDR_AFN04F44+(8*Pn)+6,2);
			}
			else
			{
				p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
				if(p8s[0]!=0x0)
				{
					MR((u32)p8d,ADDR_AFN04F42+(6*Pn),2);
				}
				else
				{
					p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
					if(p8s[0]!=0x0)
					{
						MR((u32)p8d,((u32)p8s)+6,2);
					}
					else
					{
						p8d[0]=0xee;
						p8d[1]=0xee;
					}
				}
			}
		}
		p8d+=2;
		//��ǰ�����¸��ظ���ϵ��
		p8d[0]=MRR(ADDR_AFN05F12+(9*Pn)+2,1);
		p8d++;
		//������բ���״̬
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]|=p8s[1];
		p8d++;
		//�µ����բ���״̬
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]=p8s[1];
		p8d++;
		//�������բ���״̬
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		p8d[0]=p8s[1];
		p8d++;
		//����Խ�޸澯״̬
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=8;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=4;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=2;
		}
		p8s=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=1;
		}
		p8d++;
		//���Խ�޸澯״̬
		p8d[0]=0;
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=1;
		}
		p8s=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		if(p8s[0]==100)
		{
			p8d[0]|=2;
		}
		p8d++;
	}
#endif
}

void Terminal_ResidualEnergy_Computer(void)//ʣ�����(��)����
{
#if (Project/100)!=3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=	
	u32 i;
	i=MRR(ADDR_Inc_User_Flag+1,1);
	i|=0x20;//�ܼ����������ʹ�ñ�־,D0-D5�ֱ��ʾ�����й�,�����޹�,�����й�,�����޹�,ERC22��¼��������,ʣ�����
	MWR(i,ADDR_Inc_User_Flag+1,1);
#else
	u32 i;
	u32 x;
	u32 Pn;
	u64 a64;
	u64 b64;
	u64 c64;
	u8 * p8;
	u32 * p32;
	
	//���ܼӵ������������
	p32=(u32 *)(ADDR_DATABUFF+2048);
	for(i=0;i<(2048/4);i++)
	{
		p32[i]=0;
	}
	Terminal_TotalEnergy_Computer(0x20,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF23_Remainder,ADDR_DATABUFF+2048);//�ܼӵ�����������

	//�������򹺵���ж�(�ն˵����������Ƿ�ȫ��ͬ)
	Pn=ADDR_AFN04F22+1;
	x=MRR(Pn,4);
	Pn+=4;
	for(i=1;i<TMaxTariff;i++)
	{
		if(MRR(Pn,4)!=x)
		{
			break;
		}
		Pn+=4;
	}
	if(i==TMaxTariff)
	{//�ն˵���������ȫ��ͬ,������
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_AFN05F16+Pn,1);
			if(i!=0x55)
			{//���;�����δͶ��ʱ��ʣ���������
				continue;
			}
			p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			p8[6]=0;//���������ʣ�����(��)����ʱʶ��,0=������,1=�����
			i=MRR(ADDR_AFN0CF23+(4*Pn),4);//����ǰʣ�����(��)���ݸ�ʽ03
			a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			b64=MRR(ADDR_AFN0CF23_DP+(2*Pn),2);//����ǰʣ�������λG=1ʱ(MWH)С���������,��ʽhex=0.000
			if((i&0x10000000)!=0x0)
			{//���ݸ�ʽ03Ϊ��ֵ
				b64=(~b64)+1;
			}
			a64+=b64;
			i=MRR(ADDR_DATABUFF+2048+1+((1+4+(4*TMaxTariff))*Pn),4);
			i&=0xefffffff;//ȥ����,��������ֵ
			b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			a64-=b64;
			a64=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
			MWR(a64,ADDR_AFN0CF23+(4*Pn),4);//д�Ĵ������ݵ�������,Byte<=8
			a64>>=32;
			MWR(a64,ADDR_AFN0CF23_DP+(2*Pn),2);//д�Ĵ������ݵ�������,Byte<=8
		}
	}
	else
	{//�ն˵��������ʲ�ȫΪ1,�����
		for(Pn=0;Pn<MaxTotalAddGroup;Pn++)
		{
			i=MRR(ADDR_AFN05F16+Pn,1);
			if(i!=0x55)
			{//���;�����δͶ��ʱ��ʣ���Ѳ���
				continue;
			}
			p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
			p8[6]=1;//���������ʣ�����(��)����ʱʶ��,0=������,1=�����
			i=MRR(ADDR_AFN0CF23+(4*Pn),4);//����ǰʣ�����(��)���ݸ�ʽ03
			a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			b64=MRR(ADDR_AFN0CF23_DP+(2*Pn),2);//����ǰʣ�������λG=1ʱ(MWH)С���������,��ʽhex=0.000
			if((i&0x10000000)!=0x0)
			{//���ݸ�ʽ03Ϊ��ֵ
				b64=(~b64)+1;
			}
			a64+=b64;
			for(x=0;x<TMaxTariff;x++)
			{
				i=MRR(ADDR_DATABUFF+2048+5+((1+4+(4*TMaxTariff))*Pn)+(4*x),4);//ȡ���ʵ�������
				i&=0xefffffff;//ȥ����,��������ֵ
				b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				i=MRR(ADDR_AFN04F22+1+(4*x),4);//ȡ�ն˵���������
				i&=0xefffffff;//ȥ����,��������ֵ
				c64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				b64*=c64;
				a64-=b64;
			}
			a64=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
			MWR(a64,ADDR_AFN0CF23+(4*Pn),4);//д�Ĵ������ݵ�������,Byte<=8
			a64>>=32;
			MWR(a64,ADDR_AFN0CF23_DP+(2*Pn),2);//д�Ĵ������ݵ�������,Byte<=8
		}
	}
#endif
}

void Terminal_AFN0CF12_Computer(void)//���������·���ؽ���״̬������λ��־
{
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=	
	u32 i;
	u32 x;
	u8 *p8s;
	u8 *p8d;
	if(Terminal_Ram->ContrlLoopCheck_S_Timer==0)//36 ���������·�����Ч�ȴ��붨ʱ��
	{
//1BYTE ״̬ST BIN 1 D0��D7 ��˳���λ��2 λһ�飬��Ӧ1��4 ·��������ĳ��������մ�����뿪����բ��·״̬����״̬ST���á�0������δ���롱״̬���á�1���������롱״̬��
//1BYTE ��λCD BIN 1 D0��D7 ��˳���λ��2 Ϊһ�飬��Ӧ1��4 ·��������ĳ��������մ�����뿪����բ��·״̬���ı�λCD���á�0������ǰ��ң�Ŵ��ͺ���״̬�仯���á�1������ǰ��ң�Ŵ��ͺ�������һ��״̬�仯��
		MR(ADDR_DATABUFF,ADDR_AFN0CF12,2);//���������·���ؽ���״̬������λ��־
		p8s=(u8*)(ADDR_ContrlUARTData+2);//����ģ�������������
		p8d=(u8*)(ADDR_DATABUFF);
//1BYTE	�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
//1BYTE �ӿ���ģ������Ķ��߱�־�ֽ�1
		i=p8s[0];
		i=(~i);
		x=i&0xf;
		i^=p8d[0];
		i&=0xf;
		p8d[0]=x;
		p8d[1]|=i;
		p8d[1]&=0xf;
		MW(ADDR_DATABUFF,ADDR_AFN0CF12,2);//���������·���ؽ���״̬������λ��־
		if(i!=0x0)
		{
			ERC36_Event(i,x);//���������·���ؽ���״̬����λ��¼;��ڣ�CD=��λ,ST=״̬
		}
	}
#endif
}



void Terminal_Class1Data_Computer_Ext(void)//�ն���1���ݼ�����չ����
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	u16 *p16;
	u32 *p32;
	u64 YMDHM;
	u64 YMDHMsour;
	u32 CongealNo;
#endif

	switch(Terminal_Ram->ExtTask1)//��չ����1
	{
		case 0://�ն��¼�����
			p8=(u8*)(ADDR_AFN0CF245);//�ն��¼�����
			x=0;//�ն˵�ǰ�¼���������n
			for(i=0;i<MaxERC;i++)
			{
				y=ADDR_AFN0EF3+(i*(2+(15*LEN_TERMINAL_EVENT)));
				y=MRR(y,2);
				if(y>15)
				{
					y=15;
				}
				if(y!=0)
				{
					p8[1+(2*x)+0]=i+1;//�¼�����ERC
					p8[1+(2*x)+1]=y;//�¼�����
					x++;//�ն˵�ǰ�¼���������n
				}
			}
			p8[0]=x;//����n
			break;
		case 1://��ǰ�ն˵��״̬
			p8=(u8*)ADDR_AFN0CF247;//��ǰ�ն˵��״̬
//		#if (	((USER/100)==7)||((USER/100)==5) )//�㽭,�Ϻ�
			if(Comm_Ram->UBACKUP<0x480)//�ɳ�������������С��ѹ4.80V
			{
				p8[0]=0;//D0����"1"���δʧЧ����"0"���ʧЧ
				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45�ն˵��ʧЧ�¼�(�Ϻ���չ)
				if(p8[0]==0)
				{
					p8[0]=1;
					ERC45_Event();//�ն˵��ʧЧ�¼�
				}
			}
			if(Comm_Ram->UBACKUP>=0x500)//�ɳ�������������С��ѹ5.00V
			{
				p8[0]=1;//D0����"1"���δʧЧ����"0"���ʧЧ
				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45�ն˵��ʧЧ�¼�(�Ϻ���չ)
				p8[0]=0;
			}
//		#else
//			//ʱ�ӵ�ص�ѹ(�ڲ�)(���ֶ���)
//			if(Comm_Ram->URTC<0x200)//ʱ�ӵ�ص�ѹ��С��ѹ2.00V
//			{
//				p8[0]=0;//D0����"1"���δʧЧ����"0"���ʧЧ
//				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45�ն˵��ʧЧ�¼�(�Ϻ���չ)
//				if(p8[0]==0)
//				{
//					p8[0]=1;
//					ERC45_Event();//�ն˵��ʧЧ�¼�
//				}
//			}
//			if(Comm_Ram->URTC>=0x220)//2.2v
//			{
//				p8[0]=1;//D0����"1"���δʧЧ����"0"���ʧЧ
//				p8=(u8*)(ADDR_ERC45_SOUR);//ERC45�ն˵��ʧЧ�¼�(�Ϻ���չ)
//				p8[0]=0;
//			}
//		#endif
			break;
	#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
		case 2://�����ź�ǿ��
			if(UART1Ctrl->LinkTask<8)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{//8=��ʼ��AT����
				i=0x99;//0x99δ֪���޷���ȡ
			}
			else
			{
				i=Terminal_Ram->SignaldBm;//�ź�ǿ��0-31��99
			}
			if(i==0x99)
			{
				i=0xee;//0x99δ֪���޷���ȡ
			}
			else
			{
				i=bcd_hex(i);//8λѹ��BCD��תΪHEX
				if(i>31)
				{
					i=31;
				}
				i*=2;
				i=113-i;
				i=~i;//16���ƵĲ���
				i++;
			}
			x=Comm_Ram->TYMDHMS[2];//ȡʱ
			x=bcd_hex(x);
			if(x>23)
			{
				x=23;
			}
			DMWR(i,ADDR_AFN0CF243+x,1);
			//��ƽ���ź�ǿ��
			x=0;
			y=0;
//			p8=(u8*)(ADDR_AFN0CF243);
			p8=(u8*)(ADDR_DATABUFF);
			MR(ADDR_DATABUFF,ADDR_AFN0CF243,24);
			for(i=0;i<24;i++)
			{
				if((p8[i]!=0xee)&&(p8[i]!=0xff))
				{
					y++;
					x+=(0xffffff00+p8[i]);
				}
			}
			if(x==0)
			{
				x=0xee;
			}
			else
			{
				if(y==0)
				{
					x=0xee;
				}
				else
				{
					x=~x;
					x++;
					x/=y;
					x|=1;//����ֵ
					x=~x;
					x++;
				}
			}
			MWR(x,ADDR_AFN0CF243_Per,1);
			break;
		case 3://�ն��ߵ�ѹ
			p8=(u8*)ADDR_AFN0CF191_Per;//1����ƽ���ߵ�ѹ
			if(p8[0]!=Comm_Ram->TYMDHMS[0])
			{//�ϴβ���ƽ����ʵʱ����
				//ȡʵʱֵ
				p8[0]=Comm_Ram->TYMDHMS[0];
				GetACSample(0x020Dff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
				i=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
				MR(ADDR_AFN0CF191,ADDR_TYMDHMS+1,5);//�ߵ�ѹ����ʱ��
				MR(ADDR_AFN0CF191+5,i,6);//�ߵ�ѹ
				//1����ƽ��
				p16=(u16*)(ADDR_AFN0CF191+5);
				p32=(u32*)(ADDR_AFN0CF191_Per+2);
				for(i=0;i<3;i++)
				{
					x=p16[i];
					x=bcd_hex(x);
					y=p32[i];
					y+=x;
					p32[i]=y;
				}
				i=p8[1];
				i++;
				p8[1]=i;
				if(i>=60)
				{
					p8[1]=0;
					MR(ADDR_AFN0CF191_Per+14,ADDR_TYMDHMS+1,5);//����ʱ��
					p16=(u16*)(ADDR_AFN0CF191_Per+19);
					for(x=0;x<3;x++)
					{
						y=p32[x];
						y/=i;
						y=hex_bcd(y);
						p16[x]=y;
						p32[x]=0;
					}
				}
			}
			//ÿ5�����߶���
			i=Comm_Ram->TYMDHMS[1];//��ǰRTC��
			i=bcd_hex(i);
			i/=5;//ÿ5��
			i*=5;
			i=hex_bcd(i);
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM|=i;
			YMDHMsour=MRR(ADDR_CURVECONGEAL_ExtCount+2,5);
			if(YMDHM!=YMDHMsour)
			{//û����
				CongealNo=GetCurveCongealNo_Ext(YMDHM);//�õ���չ���߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				if(CongealNo!=0xffffffff)
				{//�ҵ�ԭ����
					//����
					MWR(YMDHM,ADDR_DATABUFF,6);
					MR(ADDR_DATABUFF+6,ADDR_AFN0CF191_Per+19,6);
					MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					//�������ʱ��
					MWR(YMDHM,ADDR_CURVECONGEAL_ExtCount+2,5);
				}
				else
				{//û�ҵ�
					//��ǰ�����
					CongealNo=MRR(ADDR_CURVECONGEAL_ExtCount,2);//B0-B14���߶������,b15=1��ʾ���߶��������
					CongealNo&=0x7fff;
					if(CongealNo>=MaxCurveCongeal_Ext)
					{
						CongealNo=0;
					}
					//����Ƿ����ֵ��0xff
					MR(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<LEN_CURVECONGEAL_Ext;i++)
					{
						if(p8[i]!=0xff)
						{
							break;
						}
					}
					if(i!=LEN_CURVECONGEAL_Ext)
					{
						//�Ȳ���1��288�����д��
						if((CongealNo+288)<=MaxCurveCongeal_Ext)
						{
							MC(0xff,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext*288);
						}
						else
						{
							x=MaxCurveCongeal_Ext-CongealNo;
							MC(0xff,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext*x);
							x=288-x;
							MC(0xff,ADDR_CURVECONGEAL_ExtStart,LEN_CURVECONGEAL_Ext*x);
						}
					}
					//д��
					MWR(YMDHM,ADDR_DATABUFF,6);
					MR(ADDR_DATABUFF+6,ADDR_AFN0CF191_Per+19,6);
					DMW(ADDR_DATABUFF,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*CongealNo),LEN_CURVECONGEAL_Ext);
					//�������ʱ��
					MWR(YMDHM,ADDR_CURVECONGEAL_ExtCount+2,5);
					//�������+1
					CongealNo++;
					if(CongealNo>=MaxCurveCongeal_Ext)
					{
						CongealNo=0x8000;
					}
					i=MRR(ADDR_CURVECONGEAL_ExtCount,2);//B0-B14���߶������,b15=1��ʾ���߶��������
					i&=0x8000;
					CongealNo|=i;
					MWR(CongealNo,ADDR_CURVECONGEAL_ExtCount,2);//B0-B14���߶������,b15=1��ʾ���߶��������
				}
			}
			break;
	#endif
		default:
			Terminal_Ram->ExtTask1=0;//�û���չ����1
			return;
	}
	Terminal_Ram->ExtTask1++;//�û���չ����1
}



void Terminal_Class1Data_Computer(void)//�ն���1���ݼ���
{

	Terminal_impIn_Energy();//�ն���������-����
	Terminal_impIn_Demand();//�ն���������-���ʺ��������

	switch(Terminal_Ram->I2SUBTask1)
	{
		case 0:
			Terminal_AFN0CF2_Computer();//�ն���1����F2,�ն�����ʱ��
			break;
		case 1:
			Terminal_AFN0CF4_Computer();//�ն���1����F4,�ն�����ͨ��״̬
			break;
		case 2:
			Terminal_HourCongeal();//Сʱ����	
			break;
		case 3:
			Terminal_SwitchIn();//�ն˿���������
			break;

		case 4:
			Terminal_EnergyInc_Computer(0x1,ADDR_PKWH_INC,5,129,ADDR_AFN0CF41,ADDR_AFN0CF45);//�����й����������͵��յ��µ���������(������)
			break;
		case 5:
			Terminal_EnergyInc_Computer(0x2,ADDR_PKVARH_INC,4,130,ADDR_AFN0CF42,ADDR_AFN0CF46);//�����޹����������͵��յ��µ���������(������)
			break;
		case 6:
			Terminal_EnergyInc_Computer(0x4,ADDR_NKWH_INC,5,131,ADDR_AFN0CF43,ADDR_AFN0CF47);//�����й����������͵��յ��µ���������(������)
			break;
		case 7:
			Terminal_EnergyInc_Computer(0x8,ADDR_NKVARH_INC,4,132,ADDR_AFN0CF44,ADDR_AFN0CF48);//�����޹����������͵��յ��µ���������(������)
			break;

		case 8:
			Terminal_AFN0CF17_F18_Computer(0);//�ն���1����F17F18,��ǰ�ܼӹ���,���=0�й�=1�޹�
			break;
		case 9:
			Terminal_AFN0CF17_F18_Computer(1);//�ն���1����F17F18,��ǰ�ܼӹ���,���=0�й�=1�޹�
			break;
		case 10:
			Terminal_TotalEnergy_Computer(0x1,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF19_Remainder,ADDR_AFN0CF19);//�����й��ܼӵ���������
			break;
		case 11:
			Terminal_TotalEnergy_Computer(0x2,ADDR_PKVARH_INC,ADDR_NKVARH_INC,ADDR_AFN0CF20_Remainder,ADDR_AFN0CF20);//�����޹��ܼӵ���������
			break;
		case 12:
			Terminal_TotalEnergy_Computer(0x4,ADDR_PKWH_INC,ADDR_NKWH_INC,ADDR_AFN0CF21_Remainder,ADDR_AFN0CF21);//�����й��ܼӵ���������
			break;
		case 13:
			Terminal_TotalEnergy_Computer(0x8,ADDR_PKVARH_INC,ADDR_NKVARH_INC,ADDR_AFN0CF22_Remainder,ADDR_AFN0CF22);//�����޹��ܼӵ���������
			break;
		case 14:
			ERC22_TotalEnergy_Computer();//ERC22��¼���ʱ�����ܼӵ�������
			break;
		case 15:
			Terminal_AFN0CF73_Computer();//ֱ��ģ��������
			break;
		case 16:
			Terminal_AFN0CF5_Computer();//�ն���1����F5,�ն˿�������״̬
			break;
		case 17:
			Terminal_AFN0CF6_Computer();//�ն���1����F6,�ն˵�ǰ����״̬
			break;
		case 18:
			Terminal_ResidualEnergy_Computer();//ʣ�����(��)����
			break;
		case 19:
			Terminal_AFN0CF12_Computer();//���������·���ؽ���״̬������λ��־
			break;
		case 20:
			Terminal_Class1Data_Computer_Ext();//�ն���1���ݼ�����չ����
			break;
		default:
			Terminal_Ram->I2SUBTask1=0;
			return;
	}
	Terminal_Ram->I2SUBTask1++;
}





