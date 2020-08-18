
//2类冻结集中抄表原始数据
#include "Project.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../Device/MEMRW.h"


#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];




/*
void SaveClass2SourceData(u32 Fn,u32 Pn,u32 ADDR_Buff)//存贮类2集中抄表原始数据
{
	u32 i;
	u32 *p32;
	
	i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(i!=0x0)
	{
		p32=(u32 *)(ADDR_Terminal_ClassDataLen);
		MW(ADDR_Buff,i,p32[0]);
//		if((Fn==161)||(Fn==(161+16)))
//		{//正反有无功,4个象限无功的日冻结和月冻结
//			MW(ADDR_Buff+5+3,i,LEN_LastDate_Source_PerPn);
//		}
//		else
//		{
//			p32=(u32 *)(ADDR_Terminal_ClassDataLen);
//			MW(ADDR_Buff,i,p32[0]);
//		}
	}
}


void SaveAddDateSourceData(u32 Fn,u32 Pn,u32 FnDataAddr)//存贮类2集中抄表补抄上2-3日电能示值原始数据;入口:Fn=0xee上2日,=0xef上3日
{
#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
	u32 i;
#endif
	u32 Addr;
	u8 *p8FnData;
	
	p8FnData=(u8*)FnDataAddr;
	if(p8FnData[0]==1)
	{//数据有效
		if(Pn!=0)
		{
			switch(Fn)
			{
				case 0xee://上2日
					Addr=ADDR_LastDate_Source_Start;
					break;
				case 0xef://上3日
					Addr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn);
					break;
				default:
					return;
			}
			Addr+=((Pn-1)*LEN_LastDate_Source_PerPn);
			MW(FnDataAddr+6,Addr,LEN_LastDate_Source_PerPn);
#if RMM_MonthCongealUserDate1==1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是
			//补抄到的月初1日数据转为月冻结原始数据
			if(p8FnData[6]==1)
			{//1日数据
				Addr=GetClass2SourceDataAddr(177,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(Addr!=0x0)
				{
					MR(ADDR_DATABUFF,Addr,(6*8)+((5+4+5+4+4+4+4+4)*(1+MaxTariff)));//读原原始数据
					i=ADDR_DATABUFF;
					//F177 测量点正向有功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+MaxTariff)));
					}
					i+=6+(5*(1+MaxTariff));
					//F178 测量点正向无功（组合无功1）电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F179 测量点反向有功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+MaxTariff)));
					}
					i+=6+(5*(1+MaxTariff));
					//F180 测量点反向无功（组合无功1）电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F181 测量点1象限无功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F182 测量点2象限无功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F183 测量点3象限无功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					i+=6+(4*(1+MaxTariff));
					//F184 测量点4象限无功电能示值（总、费率1～M）
					if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
					{
						MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+MaxTariff)));
					}
					MW(ADDR_DATABUFF,Addr,(6*8)+((5+4+5+4+4+4+4+4)*(1+MaxTariff)));//写原始数据
				}
			}
#endif
		}
	}
}
*/


