

#ifndef Terminal_AFN0D_CongealData_Fn_H
#define Terminal_AFN0D_CongealData_Fn_H



typedef __packed struct
{
  vu32 OfficeAddr;//数据偏移地址
  vu32 SourAddr;//冻结时原始数据地址
	vu8 project;//支持方案:B7=,B6=通信模块,B5=采集器,B4=公变终端,B3=专变终端,B2=集中器,B1=网络表,B0=智能表
  vu8 PnType;//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
  vu16 PnLen;//每Pn数据长度
  vu16 SourPnLen;//原始数据每Pn数据长度
  vu16 PnMax;//最大Pn
  vu8 CongealType;//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
  vu8 SourInit;//冻结后原始数据处理:0=不变,1=清为0,2=清为0xee,3=置初值
}Terminal_Class2DataLib_TypeDef;


u32 Class2Data_FnOnly(u32 ClassNo,u32 Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
u32 Class2Data_Fn(u32 ClassNo,u32 Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
u32 GetClass2DataOfficeAddr(u32 Fn,u32 Pn);//取类2数据存贮偏移地址,返回0表示没找到
u32 GetClass2DataF246OfficeAddr(u32 CongealNo,u32 Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
u32 GetClass2DataOfficeAddr_MapPn(u32 Fn,u32 Pn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
u32 GetClass2DataCongealType(u32 Fn);//取类2数据冻结类型
u32 GetClass2DataLen(u32 Fn,u32 Pn,u32 DataAddr);//取类2数据长度
u32 GetDateCongealNo(u32 YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
u32 GetDateCongealAddr(u32 YMD,u32 Fn,u32 Pn);//得到日冻结存储地址,返回=0表示没找到
u32 GetRMDateCongealNo(u32 YMD);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
u32 GetRMDateCongealAddr(u32 YMD,u32 Fn,u32 Pn);//得到抄表日冻结存储地址,返回=0表示没找到
u32 GetMonthCongealNo(u32 YM);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
u32 GetMonthCongealAddr(u32 YM,u32 Fn,u32 Pn);//得到月冻结存储地址,返回=0表示没找到
u32 GetCurveCongealNo(u64 YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
u32 GetCurveCongealAddr(u64 YMDHM,u32 Fn,u32 Pn);//得到曲线冻结存储地址,返回=0表示没找到
u32 GetACSampleCurveCongealAddr(u64 YMDHM,u32 Fn);//得到交采测量点曲线冻结存储地址,返回=0表示没找到

u32 CheckCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn);//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
void AddCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//补曲线冻结点数据
u64 CurveCongeal_YMDHM(void);//当前标准曲线冻结时间YMDHM
u32 CheckDateCongeal(u32 YMD,u32 Fn,u32 Pn);//检查日冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
void AddDateCongeal(u32 YMD,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//补日冻结点数据
u32 CheckMonthCongeal(u32 YM,u32 Fn,u32 Pn);//检查月冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
void AddMonthCongeal(u32 YM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//补月冻结点数据


void Class2_Group(u32 CongealAddr,u32 Fn,u32 Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
void UART_AFN0DF97F98F99F100_Computer(u32 ADDR_FullData,u64 YMDHM,u32 m,u32 Fn,u32 Pn,u32 CongealNo);//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
void UART_AFN0DF5F6F7F8_Computer(u32 ADDR_FullData,u32 YMD,u32 Fn,u32 Pn,u32 Len);//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
void UART_AFN0DF21F22F23F24_Computer(u32 ADDR_FullData,u32 YM,u32 Fn,u32 Pn,u32 Len);//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn


void SaveClass2Data(u32 PORTn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib);//存贮类2测量点抄表数据

u32 CreateCurveCongealNo(u64 YMDHM);//创建曲线冻结号;返回:原已存在或新创建的冻结号
u32 CreateDateCongealNo(u32 YMD);//创建日冻结号;返回:原已存在或新创建的冻结号
u32 CreateMonthCongealNo(u32 YM);//创建月冻结号;返回:原已存在或新创建的冻结号

u32 LastRMDateCongealYMD(u32 PORTn);//按端口号计算最后次抄表日冻结的时标年月日;入口:通信口号;返回:0表示没
u32 GetLastRMDateCongealAddr(u32 PORTn,u32 Fn,u32 Pn);//按端口号得到最后次抄表日冻结存储地址,返回=0表示没找到


#endif


