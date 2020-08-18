

#ifndef Terminal_AFN0D_SourceData_H
#define Terminal_AFN0D_SourceData_H



void Init_Class2SourceData(u32 Pn);//类2原始数据初始化(指定Pn数据)
void Init_Class2SourceData_Command(void);//类2原始数据初始化(命令,全部清除为0xff)


void Source_Full_CongealBuff(u32 Addr_CongealBuff,u32 CongealType,u32 ADDR_CongealFlags,u32 YMD,u32 Full0xee);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
u32 LastSource_Full_CongealBuff(u32 Addr_CongealBuff,u32 LastYMD);//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有

#endif




