
#include "Project.h"
#include "Terminal_AFN0C_RealTimeData_Init.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"



extern u8 ADDR_AFN09F1[];//�ն˰汾��Ϣ
extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];
extern Terminal_HourCongealLib_TypeDef	Terminal_HourCongealLib[];


void Init_Class1Data(u32 PnType,u32 Pn)//��1���ݳ�ʼ��(ָ��Pn����)
{
	u32 i;
	u32 Fn;
	
	if(Pn==0)
	{
		return;
	}
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
				if(Terminal_Class1DataLib[Fn].PnType==PnType)
				{
					if(Terminal_Class1DataLib[Fn].PnMax!=0)
					{
						MC(Terminal_Class1DataLib[Fn].InitValue,i+(Terminal_Class1DataLib[Fn].PnLen*(Pn-1)),Terminal_Class1DataLib[Fn].PnLen);
					}
				}
			}
		}
	}
}







