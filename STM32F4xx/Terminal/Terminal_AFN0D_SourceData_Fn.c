
//2�ඳ�Ἧ�г���ԭʼ����
#include "Project.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../Device/MEMRW.h"


#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];




/*
void SaveClass2SourceData(u32 Fn,u32 Pn,u32 ADDR_Buff)//������2���г���ԭʼ����
{
	u32 i;
	u32 *p32;
	
	i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(i!=0x0)
	{
		p32=(u32 *)(ADDR_Terminal_ClassDataLen);
		MW(ADDR_Buff,i,p32[0]);
//		if((Fn==161)||(Fn==(161+16)))
//		{//�������޹�,4�������޹����ն�����¶���
//			MW(ADDR_Buff+5+3,i,LEN_LastDate_Source_PerPn);
//		}
//		else
//		{
//			p32=(u32 *)(ADDR_Terminal_ClassDataLen);
//			MW(ADDR_Buff,i,p32[0]);
//		}
	}
}


void SaveAddDateSourceData(u32 Fn,u32 Pn,u32 FnDataAddr)//������2���г�������2-3�յ���ʾֵԭʼ����;���:Fn=0xee��2��,=0xef��3��
{
#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
	u32 i;
#endif
	u32 Addr;
	u8 *p8FnData;
	
	p8FnData=(u8*)FnDataAddr;
	if(p8FnData[0]==1)
	{//������Ч
		if(Pn!=0)
		{
			switch(Fn)
			{
				case 0xee://��2��
					Addr=ADDR_LastDate_Source_Start;
					break;
				case 0xef://��3��
					Addr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn);
					break;
				default:
					return;
			}
			Addr+=((Pn-1)*LEN_LastDate_Source_PerPn);
			MW(FnDataAddr+6,Addr,LEN_LastDate_Source_PerPn);
#if RMM_MonthCongealUserDate1==1//2�����ݵ��¶���ʹ�õ��ÿ��1�ն��������;0=��,1=��
			//���������³�1������תΪ�¶���ԭʼ����
			if(p8FnData[6]==1)
			{//1������
				Addr=GetClass2SourceDataAddr(177,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
				if(Addr!=0x0)
				{
					MR(ADDR_DATABUFF,Addr,(6*8)+((5+4+5+4+4+4+4+4)*(1+MaxTariff)));//��ԭԭʼ����
					i=ADDR_DATABUFF;
					//F177 �����������й�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+MaxTariff)));
					}
					i+=6+(5*(1+MaxTariff));
					//F178 �����������޹�������޹�1������ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F179 �����㷴���й�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+MaxTariff)));
					}
					i+=6+(5*(1+MaxTariff));
					//F180 �����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F181 ������1�����޹�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F182 ������2�����޹�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F183 ������3�����޹�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F184 ������4�����޹�����ʾֵ���ܡ�����1��M��
					if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					MW(ADDR_DATABUFF,Addr,(6*8)+((5+4+5+4+4+4+4+4)*(1+MaxTariff)));//дԭʼ����
				}
			}
#endif
		}
	}
}
*/


