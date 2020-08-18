
#include "Project.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Calculate/Calculate.h"
#include "KeyUser.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../MS/MS.h"





extern Terminal_Class2SourceDataLib_TypeDef Terminal_Class2SourceDataLib[];
u32 Get_Class2SourceData_MaxLib(void);//�õ���2ԭʼ�����б����
extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);





void Init_Class2SourceData_Command(void)//��2ԭʼ���ݳ�ʼ��(����,ȫ�����Ϊ0xff)
{
	u32 i;
	u32 Addr;
	//��1��ԭʼ
	//��2-3��ԭʼ
	Addr=ADDR_AFN0D_Source_Date_Start;
	i=ADDR_LastDate_Source_End-ADDR_AFN0D_Source_Date_Start;
	MC(0xff,Addr,i);
//	while(i>=0x1000)
//	{
//		if(((Addr&0xffff)==0)&&(i>=0x10000))
//		{
//			EFLASH_Block_Erase(Addr);//�����,���R0=Ҫ�����Ŀ��ڱ����е������ַ(uses AMS-A15 address lines,ÿ��32KWORD)
//			Addr+=0x10000;
//			i-=0x10000;
//		}
//		else
//		{
//			EFLASH_Sector_Erase(Addr);//��������,���R0=Ҫ�����������ڱ������е������ַ(ÿ����2KWORD)
//			Addr+=0x1000;
//			i-=0x1000;
//		}
//	}
}


u32 ValidRMDate(u32 CongealType,u32 ValidDate,u32 RMDate)//���������ͱȽϳ�������,���:ValidDate=��Ч������(ֵ0��ʾ���Ƚ�);����:0=��ͬ��Ч,1=����ͬ��Ч
{
	if(ValidDate==0)
	{
		return 0;
	}
	switch(CongealType)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
	{
		case 0://0=�ն���
			if(ValidDate!=RMDate)
			{//������Ч����ʱ��
				return 1;
			}
			return 0;
		case 1://1=�����ն���
			return 0;
		case 2://2=�¶���
			if((ValidDate>>8)!=(RMDate>>8))
			{//������Ч����ʱ��
				return 1;
			}
			return 0;
		case 3://3=����
			return 0;
	}
	return 0;
}

/*
void AFN0DEnergyGroup(u32 CongealType,u32 YMD,u32 Fn,u32 Pn,u32 Addr_CongealBuff,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4,u32 Full0xee)//2�����ݵ���ʾֵ���
{
	u32 i;
	u32 DestAddr;
	u32 SourAddr;
	u64 DestYMDHM;
	u64 SourYMDHM;
	u8 *p8;
	u32 Fullf;
	u8 *pn8;
	
	Fullf=0;//b0-b3�ֱ�ΪFn1~Fn4�����������־
	pn8=(u8*)(ADDR_PnVSSetList);
	DestAddr=Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	p8=(u8 *)DestAddr;
//�й�����ʾֵ
	SourAddr=GetClass2SourceDataAddr(Fn1,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=1;//b0-b3�ֱ�ΪFn1~Fn4�����������־
				//��������ʾֵ
				MR(DestAddr+5,SourAddr+5,1+(5*(MaxTariff+1)));
				//����ʱ��
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&1)==0)//b0-b3�ֱ�ΪFn1~Fn4�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6,5*(MaxTariff+1));
			}
		}
	}
	//������
	if(p8[5]>MaxTariff)
	{
		p8[5]=MaxTariff;
	}
//�޹�����ʾֵ
	SourAddr=GetClass2SourceDataAddr(Fn2,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=2;//b0-b3�ֱ�ΪFn1~Fn4�����������־
				//ʾֵ
				MR(DestAddr+6+(5*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//����ʱ��
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&2)==0)//b0-b3�ֱ�ΪFn1~Fn4�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6+(5*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//�����޹�����ʾֵ
	SourAddr=GetClass2SourceDataAddr(Fn3,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=4;//b0-b3�ֱ�ΪFn1~Fn4�����������־
				//ʾֵ
				MR(DestAddr+6+(9*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//����ʱ��
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&4)==0)//b0-b3�ֱ�ΪFn1~Fn4�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6+(9*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//�����޹�����ʾֵ
	SourAddr=GetClass2SourceDataAddr(Fn4,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=8;//b0-b3�ֱ�ΪFn1~Fn4�����������־
				//ʾֵ
				MR(DestAddr+6+(13*(p8[5]+1)),SourAddr+6,(4*(MaxTariff+1)));
				//����ʱ��
				if(p8[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&8)==0)//b0-b3�ֱ�ΪFn1~Fn4�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6+(13*(p8[5]+1)),(4*(MaxTariff+1)));
			}
		}
	}
//����ʱ��
	if(Fullf==0)//b0-b3�ֱ�ΪFn1~Fn4�����������־
	{//ȫû����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr,6);
			}
		}
	}
}
void AFN0DDemandGroup(u32 CongealType,u32 YMD,u32 Fn,u32 Pn,u32 Addr_CongealBuff,u32 Fn1,u32 Fn2,u32 Full0xee)//2����������ֵ���
{
	u32 i;
	u32 x;
	u32 Tariff;
	u32 DestAddr;
	u32 SourAddr;
	u64 DestYMDHM;
	u64 SourYMDHM;
	u8 * p8s;
	u8 * p8d;
	u32 Fullf;
	u8 *pn8;
	
	Fullf=0;//b0-b1�ֱ�ΪFn1~Fn2�����������־
	pn8=(u8*)(ADDR_PnVSSetList);
	DestAddr=Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	p8d=(u8 *)DestAddr;
//�й�����
	SourAddr=GetClass2SourceDataAddr(Fn1,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=1;//b0-b1�ֱ�ΪFn1~Fn2�����������־
				p8s=(u8 *)SourAddr;
				//������
				Tariff=p8s[5];
				if(Tariff>MaxTariff)
				{
					Tariff=MaxTariff;
				}
				p8d[5]=Tariff;
				//����ֵ
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(3*i)+x]=p8s[6+(7*i)+x];
					}
				}
				//����ʱ��
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(3*(Tariff+1))+(4*i)+x]=p8s[6+(7*i)+3+x];
					}
				}
				//����ʱ��
				if(p8d[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&1)==0)//b0-b1�ֱ�ΪFn1~Fn2�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6,7*(MaxTariff+1));
			}
		}
	}
	//������
	Tariff=p8d[5];
	if(Tariff>MaxTariff)
	{
		Tariff=MaxTariff;
	}
	p8d[5]=Tariff;
//�޹�����
	SourAddr=GetClass2SourceDataAddr(Fn2,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(SourAddr!=0)
	{//��ԭʼ���ݿ�
		i=MRR(SourAddr+2,3);
		if(i!=0xffffff)
		{//ԭʼ������Ч(��0ʱΪ0xff)
			i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
			if(i==0)
			{//��ͬ��Ч
				Fullf|=2;//b0-b1�ֱ�ΪFn1~Fn2�����������־
				p8s=(u8 *)SourAddr;
				//����ֵ
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(7*(Tariff+1))+(3*i)+x]=p8s[6+(7*i)+x];
					}
				}
				//����ʱ��
				for(i=0;i<(Tariff+1);i++)
				{
					for(x=0;x<3;x++)
					{
						p8d[6+(7*(Tariff+1))+(3*(Tariff+1))+(4*i)+x]=p8s[6+(7*i)+3+x];
					}
				}
				//����ʱ��
				if(p8d[0]==0xee)
				{
					MR(DestAddr,SourAddr,5);//ԭ�޳���ʱ��
				}
				else
				{
					DestYMDHM=MRR(DestAddr,5);
					SourYMDHM=MRR(SourAddr,5);
					if(SourYMDHM>DestYMDHM)
					{
						MR(DestAddr,SourAddr,5);//��ǰ����ʱ��>ԭ����ʱ��
					}
				}
			}
		}
	}
	if((Fullf&2)==0)//b0-b1�ֱ�ΪFn1~Fn2�����������־
	{//û����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr+6+(7*(p8d[5]+1)),7*(MaxTariff+1));
			}
		}
	}
//����ʱ��
	if(Fullf==0)//b0-b1�ֱ�ΪFn1~Fn2�����������־
	{//ȫû����
		if(Full0xee)
		{//��ԭʼ������0xee
			i=pn8[Pn];
			if((i==2)||(i==30)||(i==31))
			{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
				MC(0xee,DestAddr,6);
			}
		}
	}
}
*/

void Source_Full_CongealBuff(u32 Addr_CongealBuff,u32 CongealType,u32 ADDR_CongealFlags,u32 YMD,u32 Full0xee)//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 MaxFn;
	u32 MaxPn;
	u32 SourAddr;
	u8 * p8f;
	u8 *pn8;
	ms_Type* ms;
	
//Ϊ��������nandдbuff
	ms=Get_ms();
#if NAND128kBuff==0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
	ms->NAND4kBuff_Write_NAND(0,0,0);
#else
	ms->NAND128kBuff_Write_NAND(0,0,0);
#endif
	p8f=(u8*)ADDR_CongealFlags;//������ֶ˿��足���־
	MaxFn=Get_Class2Data_MaxFn();
	if(Full0xee)
	{//��ԭʼ������0xee
		PnVSPORTProtocolList(ADDR_PnVSSetList);//�������Pn��ӦAFN04F10���ö˿ںź�ͨ��Э���б�,�������ҷ������Ĭ�϶˿ں�0ͨ��Э��Ϊ30(DL645-2007)
	}
	pn8=(u8*)(ADDR_PnVSSetList);
	
	for(Fn=1;Fn<MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==CongealType)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				MaxPn=Terminal_Class2DataLib[Fn].PnMax;
				for(Pn=1;Pn<=MaxPn;Pn++)
				{
					if(CongealType==1)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
					{//�����ն���,���˿ںŽ���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//���������
						{
							i=p8f[(Pn-1)/8];
							i&=1<<((Pn-1)%8);
							if(i==0)
							{//�ֶ˿ڱ������㶳���־=0
								continue;
							}
						}
					}
					SourAddr=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
					if(SourAddr!=0)
					{//��ԭʼ���ݿ�
						i=MRR(SourAddr,1);
						if(i==0xff)
						{//ԭʼ������Ч(��0ʱΪ0xff)
							if(Full0xee)
							{//��ԭʼ������0xFF
								if(CongealType==0)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
								{//���ն���
									if(pn8[(2*Pn)+1]==1)
									{//��DL/T 645-1997
										if((Fn>=185)&&(Fn<=188))
										{//���ն���97����������,����
											continue;
										}
									}
								#if RMM_RS485_DateCongeal==0//RS485ͨ�������ն���;0=��,1=��
									if(((pn8[(2*Pn)]==0)||(pn8[(2*Pn)]==31))&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//����,485��,����ڲ���(ֻ���ز���)
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#else
									#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									//97��Լ���Ϻ���Լʵʱ���ܹ��մ洢Ϊ��һ���ն���
									//if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]>=30)))//����,�����,1997��Լ,�Ϻ���Լ����
									//97��Լ���Ϻ���Լ���ղɼ�ʵʱ���ܴ洢Ϊ��һ���ն���
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]==21)||(pn8[(2*Pn)+1]>=30)))//����,����ڲ���
									#else//#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//����,����ڲ���
									#endif
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#endif
								}
								else
								{//��1=�����ն���,2=�¶���
								#if RMM_RS485_DateCongeal==0//RS485ͨ�������ն���;0=��,1=��
									if(((pn8[(2*Pn)]==0)||(pn8[(2*Pn)]==31))&&(pn8[(2*Pn)+1]>=30))//����,485��,�����,1997��Լ����
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#else
									#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									if(CongealType==2)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
									{
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==1))
										{//��DL/T 645-1997
											if((Fn>=193)&&(Fn<=196))
											{//���¶���97����������,���
												MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
											}
										}
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==21))
										{//���Ϻ���Լ
											//�Ϻ���ɼ�������3�������й�������Ϊ�¶��������й�����
											MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
										}
									}
									#endif
									if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]>=30))//����,�����,1997��Լ����
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								#endif
								}
							}
							continue;
						}
				#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
						if((Fn==209)||(Fn==213)||(Fn==214)||(Fn==215)||(Fn==216))
						{//����Fn���Ƚϳ���ʱ��,���������γ������ݶ���
							//Fn=209�ն�����ܱ�Զ�̿���ͨ�ϵ�״̬����¼
							//Fn=213�¶�����ܱ��ز���������ʱ��
							//Fn=214�¶�����ܱ�����޸Ĵ�����ʱ��
							//Fn=215�¶�����ܱ����õ���Ϣ
							//Fn=216�¶�����ܱ������Ϣ
							MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							continue;
						}
				#endif
						i=MRR(SourAddr+2,3);//ȡ����ʱ��������
						i=ValidRMDate(CongealType,YMD,i);//���������ͱȽϳ�������,����:0=��ͬ��Ч,1=����ͬ��Ч
						if(i!=0)
						{//����ͬ��Ч
							if(Full0xee)
							{//��ԭʼ������0xFF
								if(CongealType==0)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
								{//���ն���
									if(pn8[(2*Pn)+1]==1)
									{//��DL/T 645-1997
										if((Fn>=185)&&(Fn<=188))
										{//���ն���97����������,����
											continue;
										}
									}
								#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]>=30)))//����,�����,1997��Լ,�Ϻ���Լ����
								#else//#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									if((pn8[(2*Pn)]!=1)&&((pn8[(2*Pn)+1]==1)||(pn8[(2*Pn)+1]>=30)))//����,����ڲ���
								#endif
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								}
								else
								{//��1=�����ն���,2=�¶���
								#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
									if(CongealType==2)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
									{
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==1))
										{//��DL/T 645-1997
											if((Fn>=193)&&(Fn<=196))
											{//���¶���97����������,���
												MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
											}
										}
										if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]==21))
										{//���Ϻ���Լ
											//�Ϻ���ɼ�������3�������й�������Ϊ�¶��������й�����
											MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
										}
									}
								#endif
									if((pn8[(2*Pn)]!=1)&&(pn8[(2*Pn)+1]>=30))//����,�����,1997��Լ����
									{//1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��				
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),Terminal_Class2DataLib[Fn].PnLen);
									}
								}
							}
							continue;
						}
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{//��ԭʼ���ݿ�
						/*
						switch(Fn)
						{
							case 1://�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 9://�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
								//F161 �ն��������й�����ʾֵ���ܡ�����1��M��
								//F162 �ն��������޹�������޹�1������ʾֵ���ܡ�����1��M��
								//F165 �ն���һ�����޹�����ʾֵ���ܡ�����1��M��
								//F168 �ն����������޹�����ʾֵ���ܡ�����1��M��
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,161,162,165,168,Full0xee);//2�����ݵ���ʾֵ���
								break;
							case 2://�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 10://�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
								//F163 �ն��ᷴ���й�����ʾֵ���ܡ�����1��M��
								//F164 �ն��ᷴ���޹�������޹�1������ʾֵ���ܡ�����1��M��
								//F166 �ն���2�����޹�����ʾֵ���ܡ�����1��M��
								//F167 �ն���3�����޹�����ʾֵ���ܡ�����1��M��
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,163,164,166,167,Full0xee);//2�����ݵ���ʾֵ���
								break;
							case 3://�ն���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 11://�����ն���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								//F185 �ն��������й��������������ʱ�䣨�ܡ�����1��M��
								//F186 �ն��������޹��������������ʱ�䣨�ܡ�����1��M��
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,185,186,Full0xee);//2����������ֵ���
								break;
							case 4://�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 12://�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								//F187 �ն��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
								//F188 �ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,187,188,Full0xee);//2����������ֵ���
								break;
							case 17://�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
								//F177 �¶��������й�����ʾֵ���ܡ�����1��M��
								//F178 �¶��������޹�������޹�1������ʾֵ���ܡ�����1��M��
								//F181 �¶���1�����޹�����ʾֵ���ܡ�����1��M��
								//F184 �¶���4�����޹�����ʾֵ���ܡ�����1��M��
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,177,178,181,184,Full0xee);//2�����ݵ���ʾֵ���
								break;
							case 18://�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
								//F179 �¶��ᷴ���й�����ʾֵ���ܡ�����1��M��
								//F180 �¶��ᷴ���޹�������޹�1������ʾֵ���ܡ�����1��M��
								//F182 �¶���2�����޹�����ʾֵ���ܡ�����1��M��
								//F183 �¶���3�����޹�����ʾֵ���ܡ�����1��M��
								AFN0DEnergyGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,179,180,182,183,Full0xee);//2�����ݵ���ʾֵ���
								break;
							case 19://�¶���������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								//F193 �¶��������й��������������ʱ�䣨�ܡ�����1��M��
								//F194 �¶��������޹��������������ʱ�䣨�ܡ�����1��M��
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,193,194,Full0xee);//2����������ֵ���
								break;
							case 20://�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								//F195 �¶��ᷴ���й��������������ʱ�䣨�ܡ�����1��M��
								//F196 �¶��ᷴ���޹��������������ʱ�䣨�ܡ�����1��M��
								AFN0DDemandGroup(CongealType,YMD,Fn,Pn,Addr_CongealBuff,195,196,Full0xee);//2����������ֵ���
								break;
						}
					*/
					}

				}
			}
		}
	}
}


u32 GetLastSourceDataOffice(u32 Fn)//ȡ��2��2-3��ԭʼ����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
{
	switch(Fn)
	{
		case 161://�����й�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF161_LastDate_Source;
		case 162://�����޹�������޹�1������ʾֵ���ܡ�����1��M��
			return Office_AFN0DF162_LastDate_Source;
		case 163://�����й�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF163_LastDate_Source;
		case 164://�����޹�������޹�2������ʾֵ���ܡ�����1��M��
			return Office_AFN0DF164_LastDate_Source;
		case 165://1�����޹�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF165_LastDate_Source;
		case 166://2�����޹�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF166_LastDate_Source;
		case 167://3�����޹�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF167_LastDate_Source;
		case 168://4�����޹�����ʾֵ���ܡ�����1��M��
			return Office_AFN0DF168_LastDate_Source;
		default:
			return 0;
	}
}

u32 LastSource_Full_CongealBuff(u32 Addr_CongealBuff,u32 LastYMD)//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u32 MaxPn;
	u32 SourAddr;
	u32 Full;//�������־
	ms_Type* ms;
	
//Ϊ��������nandдbuff
	ms=Get_ms();
#if NAND128kBuff==0//����128k nandд����;0=4k BKPSRAM,1=128k SDRAM
	ms->NAND4kBuff_Write_NAND(0,0,0);
#else
	ms->NAND128kBuff_Write_NAND(0,0,0);
#endif
	
	Full=0;//�������־
	for(Fn=161;Fn<168;Fn++)
	{
		MaxPn=Terminal_Class2DataLib[Fn].PnMax;
		for(Pn=1;Pn<=MaxPn;Pn++)
		{
			SourAddr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
			i=MRR(SourAddr,3);//ȡԭʼ����ʱ��
			if(i==LastYMD)
			{//����ʱ����Ч
				SourAddr=GetLastSourceDataOffice(Fn);//ȡ��2��2-3��ԭʼ����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
				SourAddr+=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
			}
			else
			{
				SourAddr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
				i=MRR(SourAddr,3);//ȡԭʼ����ʱ��
				if(i==LastYMD)
				{//����ʱ����Ч
					SourAddr=GetLastSourceDataOffice(Fn);//ȡ��2��2-3��ԭʼ����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
					SourAddr+=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
				}
				else
				{
					continue;
				}
			}
			i=MRR(SourAddr,1);
			if(i!=0xee)
			{//������Ч
				Full=1;//�������־
				MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1)),SourAddr,Terminal_Class2DataLib[Fn].PnLen);
			}
		}
	}
	return Full;
}







