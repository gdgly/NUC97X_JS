

#ifndef Terminal_AFN0C_RealTimeData_Fn_H
#define Terminal_AFN0C_RealTimeData_Fn_H




typedef __packed struct
{
  vu32 Addr;//数据地址
	vu8 project;//支持方案:B7=,B6=通信模块,B5=采集器,B4=公变终端,B3=专变终端,B2=集中器,B1=网络表,B0=智能表
  vu8 PnType;//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
  vu16 PnLen;//每Pn数据长度
  vu16 PnMax;//最大Pn
  vu8 Init;//上电初始化0=初始化,1=不初始化
  vu8 InitValue;//初始值
}Terminal_Class1DataLib_TypeDef;

typedef __packed struct
{
  vu32 SourAddr;//原始数据地址
  vu32 DestAddr;//目的数据地址
	vu8 project;//支持方案:B7=,B6=通信模块,B5=采集器,B4=公变终端,B3=专变终端,B2=集中器,B1=网络表,B0=智能表
  vu8 PnType;//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
  vu16 SourPnLen;//原始每Pn数据长度
  vu16 DestPnLen;//目的每Pn数据长度
  vu16 PnMax;//最大Pn
}Terminal_HourCongealLib_TypeDef;

u32 Get_Class1Data_MaxFn(void);
u32 Get_InitClass1Data_MaxFn(void);
u32 Get_HourCongeal_MaxFn(void);

u32 Class1Data_FnOnly(u32 ClassNo,u32 Fn);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
u32 Class1Data_Fn(u32 ClassNo,u32 Fn);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
u32 GetClass1Data_PnMax(u32 Fn);//取类1数据各Fn的最大Pn
void SaveClass1Data(u32 Fn,u32 Pn,u32 SourAddr);//存贮类1数据

void Class1_F33_F34_F37_F38(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4);//抄表数据组合成F33_F34_F37_F38
void Class1_F35_F36_F39_F40(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2);//抄表数据组合成F35_F36_F39_F40
#endif
