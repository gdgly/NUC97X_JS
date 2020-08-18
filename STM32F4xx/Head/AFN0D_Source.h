
//集中抄抄表2类数据日月冻结原始数据(初始值为擦除值0xFF)
//(2类数据上1-24整点电表冻结的正反有功电能示值原始数据在RAM,不在此)

#ifndef AFN0D_Source_H
#define AFN0D_Source_H

//测量点1
#define	ADDR_AFN0D_Source_Date_Start     ADDR_AFN0D_Source_Start//(4K对齐)开始地址
	//类2数据日冻结原始数据(当日)
#define  ADDR_AFN0D_DateSource_Date     ADDR_AFN0D_Source_Date_Start//3byte 正反有功4个象限无功,终端抄表时间年月日,下紧接ADDR_AFN0DF161_Source用于寻址
#define  ADDR_AFN0DF161_Source      ADDR_AFN0D_DateSource_Date+3//测量点正向有功电能示值（总、费率1～M）
#define  ADDR_AFN0DF162_Source      ADDR_AFN0DF161_Source+(6+(5*(1+TMaxTariff)))//测量点正向无功（组合无功1）电能示值（总、费率1～M）
#define  ADDR_AFN0DF163_Source      ADDR_AFN0DF162_Source+(6+(4*(1+TMaxTariff)))//测量点反向有功电能示值（总、费率1～M）
#define  ADDR_AFN0DF164_Source      ADDR_AFN0DF163_Source+(6+(5*(1+TMaxTariff)))//测量点反向无功（组合无功1）电能示值（总、费率1～M）
#define  ADDR_AFN0DF165_Source      ADDR_AFN0DF164_Source+(6+(4*(1+TMaxTariff)))//测量点1象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF166_Source      ADDR_AFN0DF165_Source+(6+(4*(1+TMaxTariff)))//测量点2象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF167_Source      ADDR_AFN0DF166_Source+(6+(4*(1+TMaxTariff)))//测量点3象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF168_Source      ADDR_AFN0DF167_Source+(6+(4*(1+TMaxTariff)))//测量点4象限无功电能示值（总、费率1～M）

#define  ADDR_AFN0DF185_Source      ADDR_AFN0DF168_Source+(6+(4*(1+TMaxTariff)))//测量点正向有功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF186_Source      ADDR_AFN0DF185_Source+(6+(7*(1+TMaxTariff)))//测量点正向无功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF187_Source      ADDR_AFN0DF186_Source+(6+(7*(1+TMaxTariff)))//测量点反向有功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF188_Source      ADDR_AFN0DF187_Source+(6+(7*(1+TMaxTariff)))//测量点反向无功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF209_Source      ADDR_AFN0DF188_Source+(6+(7*(1+TMaxTariff)))//测量点电能表远程控制通断电状态及记录
#if(USER/100)==12//四川
#define  ADDR_AFN0DF210_Source      ADDR_AFN0DF209_Source+(5+13)//日冻结电能表购、用电信息
#define ADDR_AFN0D_Source_Date_End     (ADDR_AFN0DF210_Source+(5+12))
#endif

#ifndef ADDR_AFN0D_Source_Date_End
#define ADDR_AFN0D_Source_Date_End     (ADDR_AFN0DF209_Source+(5+13))
#endif

#define LEN_AFN0D_Source_Date     (ADDR_AFN0D_Source_Date_End-ADDR_AFN0D_Source_Date_Start)

	//类2数据月冻结原始数据
#define ADDR_AFN0D_Source_Month_Start     ADDR_AFN0D_Source_Date_End//开始地址
#define  ADDR_AFN0D_MonthSource_Date     ADDR_AFN0D_Source_Month_Start//2byte 正反有功4个象限无功,终端抄表时间年月,下紧接ADDR_AFN0DF177_Source用于寻址
#define  ADDR_AFN0DF177_Source      ADDR_AFN0D_MonthSource_Date+2//测量点正向有功电能示值（总、费率1～M）
#define  ADDR_AFN0DF178_Source      ADDR_AFN0DF177_Source+(6+(5*(1+TMaxTariff)))//测量点正向无功（组合无功1）电能示值（总、费率1～M）
#define  ADDR_AFN0DF179_Source      ADDR_AFN0DF178_Source+(6+(4*(1+TMaxTariff)))//测量点反向有功电能示值（总、费率1～M）
#define  ADDR_AFN0DF180_Source      ADDR_AFN0DF179_Source+(6+(5*(1+TMaxTariff)))//测量点反向无功（组合无功1）电能示值（总、费率1～M）
#define  ADDR_AFN0DF181_Source      ADDR_AFN0DF180_Source+(6+(4*(1+TMaxTariff)))//测量点1象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF182_Source      ADDR_AFN0DF181_Source+(6+(4*(1+TMaxTariff)))//测量点2象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF183_Source      ADDR_AFN0DF182_Source+(6+(4*(1+TMaxTariff)))//测量点3象限无功电能示值（总、费率1～M）
#define  ADDR_AFN0DF184_Source      ADDR_AFN0DF183_Source+(6+(4*(1+TMaxTariff)))//测量点4象限无功电能示值（总、费率1～M）

#define  ADDR_AFN0DF193_Source      ADDR_AFN0DF184_Source+(6+(4*(1+TMaxTariff)))//测量点正向有功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF194_Source      ADDR_AFN0DF193_Source+(6+(7*(1+TMaxTariff)))//测量点正向无功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF195_Source      ADDR_AFN0DF194_Source+(6+(7*(1+TMaxTariff)))//测量点反向有功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF196_Source      ADDR_AFN0DF195_Source+(6+(7*(1+TMaxTariff)))//测量点反向无功最大需量及发生时间（总、费率1～M）
#define  ADDR_AFN0DF213_Source      ADDR_AFN0DF196_Source+(6+(7*(1+TMaxTariff)))//测量点电能表开关操作次数及时间
#define  ADDR_AFN0DF214_Source      ADDR_AFN0DF213_Source+(5+18)//测量点电能表参数修改次数及时间
#define  ADDR_AFN0DF215_Source      ADDR_AFN0DF214_Source+(5+24)//测量点电能表购、用电信息
#define  ADDR_AFN0DF216_Source      ADDR_AFN0DF215_Source+(5+36)//测量点电能表结算信息

#define ADDR_AFN0D_Source_Month_End     (ADDR_AFN0DF216_Source+(6+(10*(1+TMaxTariff))))
#define LEN_AFN0D_Source_Month     (ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Month_Start)


#if ((ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Start)>=1024)
#define LEN_AFN0D_Source     (ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Start)
#else
#define LEN_AFN0D_Source     1024
#endif

//测量点2
	//类2数据日冻结原始数据
	//类2数据月冻结原始数据
//...
//测量点Max
	//类2数据日冻结原始数据
	//类2数据月冻结原始数据


#define  ADDR_LastDate_Source_Start     ((ADDR_AFN0D_Source_Date_Start+(LEN_AFN0D_Source*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//(4K对齐)补抄电表上1-3日日冻结原始数据地址
//补抄电表上2日日冻结原始数据
//测量点1
#define  Office_LastDate_Date     0
//3byte 电能表冻结时标年月日(发生在日更新后的0时0分冻结)
#define  Office_AFN0DF161_LastDate_Source     Office_LastDate_Date+3
//6+(5*(1+TMaxTariff))byte F161 抄表时间+费率数+正向有功电能示值（总、费率1～M）
#define  Office_AFN0DF162_LastDate_Source     Office_AFN0DF161_LastDate_Source+(6+(5*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F162 抄表时间+费率数+正向无功（组合无功1）电能示值（总、费率1～M）
#define  Office_AFN0DF163_LastDate_Source     Office_AFN0DF162_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(5*(1+TMaxTariff))byte F163 抄表时间+费率数+反向有功电能示值（总、费率1～M）
#define  Office_AFN0DF164_LastDate_Source     Office_AFN0DF163_LastDate_Source+(6+(5*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F164 抄表时间+费率数+反向无功（组合无功2）电能示值（总、费率1～M）
#define  Office_AFN0DF165_LastDate_Source     Office_AFN0DF164_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F165 抄表时间+费率数+1象限无功电能示值（总、费率1～M）
#define  Office_AFN0DF166_LastDate_Source     Office_AFN0DF165_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F166 抄表时间+费率数+2象限无功电能示值（总、费率1～M）
#define  Office_AFN0DF167_LastDate_Source     Office_AFN0DF166_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F167 抄表时间+费率数+3象限无功电能示值（总、费率1～M）
#define  Office_AFN0DF168_LastDate_Source     Office_AFN0DF167_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F168 抄表时间+费率数+4象限无功电能示值（总、费率1～M）

#if (((USER/100)==5)&&(Project==300))//上海专变
#define  Office_AFN0DF185_LastDate_Source     Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(7*(1+TMaxTariff))byte F168 抄表时间+费率数+正向有功最大需量及发生时间（总、费率1～M）
#define  Office_AFN0DF187_LastDate_Source     Office_AFN0DF185_LastDate_Source+(6+(7*(1+TMaxTariff)))
//6+(7*(1+TMaxTariff))byte F168 抄表时间+费率数+反向有功最大需量及发生时间（总、费率1～M）
#define  LEN_LastDate_Source_PerPn     (Office_AFN0DF187_LastDate_Source+(6+(7*(1+TMaxTariff))))
#endif

#if(USER/100)==12//四川
#define  Office_AFN0DF210_LastDate_Source      Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff)))//日冻结电能表购、用电信息
//5+12 抄表时间+购电次数+剩余金额+累计购电金额
#define LEN_LastDate_Source_PerPn     (Office_AFN0DF210_LastDate_Source+17)
#endif

#ifndef LEN_LastDate_Source_PerPn
#define  LEN_LastDate_Source_PerPn     (Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff))))
#endif

//测量点2
//...
//测量点Max
//...
//补抄电表上3日日冻结原始数据
//...
#define  ADDR_LastDate_Source_End     (ADDR_LastDate_Source_Start+(2*LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn))




#define ADDR_AFN0D_Source_End     ADDR_LastDate_Source_End
#if ((ADDR_AFN0D_Source_End-ADDR_AFN0D_Source_Date_Start)>LEN_Terminal_EFLASH_AFN0D_Source)
	#error "((ADDR_AFN0D_Source_End-ADDR_AFN0D_Source_Date_Start)>LEN_Terminal_EFLASH_AFN0D_Source)"
#endif


#endif

















