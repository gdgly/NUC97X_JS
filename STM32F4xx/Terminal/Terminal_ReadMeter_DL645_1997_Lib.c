
//Terminal_ReadMeter_DL645_1997_Lib
#include "Project.h"
#include "Terminal_ReadMeter_DL645_1997_Lib.h"
#include "Terminal_AFN0E_Event.h"

//F0 通用
__asm void ADDR_RM_DL645_1997_F0_Lib(void)
{
	dcb 0//实时Fn
	dcb 0//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000000//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}

//F1 重点户曲线(仅对无曲线冻结的DL645-2007单相表)
//F2 电能表主动上报事件
#if (USER/100)==7//浙江用户
__asm void ADDR_RM_DL645_1997_F2_Lib(void)
{
	dcb 2//实时Fn
	dcb 4*2//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
	dcb 0//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
#else
	dcb 2_00000100//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
#endif
	dcb 2_00000000//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	
	dcwu 0xB212//编程次数
	dcb 2//返回长度
	dcwu 0//
	dcb 0//返回长度

	dcwu 0xB213//最大需量清零次数
	dcb 2//返回长度
	dcwu 0//
	dcb 0//返回长度

	dcwu 0xB310//总断相次数
	dcb 2//返回长度
	dcwu 0//
	dcb 0//返回长度

	dcwu 0xC021//电网状态字
	dcb 1//返回长度
	dcwu 0//
	dcb 0//返回长度
	ALIGN
}
#endif

//F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
__asm void ADDR_RM_DL645_1997_F25_Lib(void)
{
	dcb 25//实时Fn
	dcb 18//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xb630//总有功功率
	dcb 3//返回长度
	dcwu 0xb631//A有功功率
	dcb 3//返回长度
	dcwu 0xb632//B有功功率
	dcb 3//返回长度
	dcwu 0xb633//C有功功率
	dcb 3//返回长度
	dcwu 0xb640//总无功功率
	dcb 2//返回长度
	dcwu 0xb641//A无功功率
	dcb 2//返回长度
	dcwu 0xb642//B无功功率
	dcb 2//返回长度
	dcwu 0xb643//C无功功率
	dcb 2//返回长度
	dcwu 0xb650//总功率因数
	dcb 2//返回长度
	dcwu 0xb651//A功率因数
	dcb 2//返回长度
	dcwu 0xb652//B功率因数
	dcb 2//返回长度
	dcwu 0xb653//C功率因数
	dcb 2//返回长度
	dcwu 0xb611//A相电压
	dcb 2//返回长度
	dcwu 0xb612//B相电压
	dcb 2//返回长度
	dcwu 0xb613//C相电压
	dcb 2//返回长度
	dcwu 0xB621//A相电流
	dcb 2//返回长度
	dcwu 0xB622//B相电流
	dcb 2//返回长度
	dcwu 0xB623//C相电流
	dcb 2//返回长度
	//零序电流
	//总,A,B,C视在功率
	ALIGN
}
//F26 A、B、C三相断相统计数据及最近一次断相记录
__asm void ADDR_RM_DL645_1997_F26_Lib(void)
{
	dcb 26//实时Fn
	dcb 16//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xb310//总断相次数
	dcb 2//返回长度
	dcwu 0xb311//A断相次数
	dcb 2//返回长度
	dcwu 0xb312//B断相次数
	dcb 2//返回长度
	dcwu 0xb313//C断相次数
	dcb 2//返回长度
	dcwu 0xb320//总断相时间累计值
	dcb 3//返回长度
	dcwu 0xb321//A断相时间累计值
	dcb 3//返回长度
	dcwu 0xb322//B断相时间累计值
	dcb 3//返回长度
	dcwu 0xb323//C断相时间累计值
	dcb 3//返回长度
	dcwu 0xb330//总断相起始时刻
	dcb 4//返回长度
	dcwu 0xb331//A断相起始时刻
	dcb 4//返回长度
	dcwu 0xb332//B断相起始时刻
	dcb 4//返回长度
	dcwu 0xb333//C断相起始时刻
	dcb 4//返回长度
	dcwu 0xb340//总断相结束时刻
	dcb 4//返回长度
	dcwu 0xb341//A断相结束时刻
	dcb 4//返回长度
	dcwu 0xb342//B断相结束时刻
	dcb 4//返回长度
	dcwu 0xb343//C断相结束时刻
	dcb 4//返回长度
	ALIGN
}

//F27 电能表日历时钟、编程次数及其最近一次操作时间
__asm void ADDR_RM_DL645_1997_F27_Lib(void)
{
	dcb 27//实时Fn
	dcb 7//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
  dcwu 0xC011//电能表日历时钟(时分秒)
  dcb 3//通信返回数据总长度,0=可变
  dcwu 0xC010//电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
  dcwu 0xB214//电池工作时间
  dcb 3//通信返回数据总长度
  dcwu 0xB212//编程次数
  dcb 2//通信返回数据总长度
  dcwu 0xB210//最近一次编程时间
  dcb 4//通信返回数据总长度
  dcwu 0xB213//最大需量清零次数
  dcb 2//通信返回数据总长度
  dcwu 0xB211//最近一次最大需量清零时间
  dcb 4//通信返回数据总长度
	ALIGN
}
//F28 电表运行状态字及其变位标志
__asm void ADDR_RM_DL645_1997_F28_Lib(void)
{
	dcb 28//实时Fn
	dcb 2//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
  dcwu 0xC020//电表运行状态字
  dcb 1//通信返回数据总长度,0=可变
  dcwu 0xC021//电网状态字
  dcb 1//通信返回数据总长度,0=可变
	ALIGN
}
//F29 当前铜损、铁损有功总电能示值
__asm void ADDR_RM_DL645_1997_F29_Lib(void)
{
	dcb 29//实时Fn
	dcb 0//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000010//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F30 上一结算日铜损、铁损有功总电能示值
__asm void ADDR_RM_DL645_1997_F30_Lib(void)
{
	dcb 30//实时Fn
	dcb 0//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000010//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
__asm void ADDR_RM_DL645_1997_F31_Lib(void)
{
	dcb 31//实时Fn
	dcb 0//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
__asm void ADDR_RM_DL645_1997_F32_Lib(void)
{
	dcb 32//实时Fn
	dcb 0//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F33_Lib(void)
{
	dcb 33//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F34_Lib(void)
{
	dcb 34//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F35_Lib(void)
{
	dcb 35//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F36_Lib(void)
{
	dcb 36//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F37_Lib(void)
{
	dcb 37//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F38_Lib(void)
{
	dcb 38//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F39_Lib(void)
{
	dcb 39//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_DL645_1997_F40_Lib(void)
{
	dcb 40//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#else
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F49 当前电压、电流相位角

//F57 当前A、B、C三相电压、电流2～N次谐波有效值

//F58 当前A、B、C三相电压、电流2～N次谐波含有率

//F65 当前电容器投切状态
//F66 当前电容器累计补偿投入时间和次数
//F67 当日、当月电容器累计补偿的无功电能量
//F73 直流模拟量当前数据


//F129 当前正向有功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F129_Lib(void)
{
#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
	dcb 129//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 161//日冻结Fn
	dcb 169//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x901f
	dcb 0//返回长度
	dcwu 0x902f
	dcb 0//返回长度	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x901f
	dcb 20//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x902f
	dcb 20//返回长度
#else
	dcb 129//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 161//日冻结Fn
	dcb 169//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x901f
	dcb 0//返回长度
	dcwu 0x9010//当无块命令时只抄总电量
	dcb 4//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x901f
	dcb 20//返回长度
//	dcwu 0x9010
	dcb 4//返回长度
#endif
	ALIGN
}	
//F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F130_Lib(void)
{
	dcb 130//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 162//日冻结Fn
	dcb 170//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x911f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x911f
	dcb 20//返回长度
	ALIGN
}
//F131 当前反向有功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F131_Lib(void)
{
	dcb 131//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 163//日冻结Fn
	dcb 171//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x902f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x902f
	dcb 20//返回长度
	ALIGN
}
//F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F132_Lib(void)
{
	dcb 132//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 164//日冻结Fn
	dcb 172//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x912f
	dcb 0//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x912f
	dcb 20//返回长度
	ALIGN
}
//F133 当前一象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F133_Lib(void)
{
	dcb 133//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 165//日冻结Fn
	dcb 173//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x913f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x913f
	dcb 20//返回长度
	ALIGN
}
//F134 当前二象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F134_Lib(void)
{
	dcb 134//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 166//日冻结Fn
	dcb 174//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x915f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x915f
	dcb 20//返回长度
	ALIGN
}
//F135 当前三象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F135_Lib(void)
{
	dcb 135//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 167//日冻结Fn
	dcb 175//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x916f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x916f
	dcb 20//返回长度
	ALIGN
}
//F136 当前四象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F136_Lib(void)
{
	dcb 136//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 168//日冻结Fn
	dcb 176//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0x914f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x914f
	dcb 20//返回长度
	ALIGN
}
//F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F137_Lib(void)
{
#if (((USER/100)==5)&&((Project/100)==2))//上海集中器	
	dcb 137//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 177//月冻结Fn
	dcwu 0x941f
	dcb 0//返回长度
	dcwu 0x942f
	dcb 0//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x941f
	dcb 20//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x942f
	dcb 20//返回长度
#else
	dcb 137//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 177//月冻结Fn
	dcwu 0x941f
	dcb 0//返回长度
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x941f
	dcb 20//返回长度
#endif
	ALIGN
}
//F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F138_Lib(void)
{
	dcb 138//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 178//月冻结Fn
	dcwu 0x951f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x951f
	dcb 20//返回长度
	ALIGN
}
//F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F139_Lib(void)
{
	dcb 139//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 179//月冻结Fn
	dcwu 0x942f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x942f
	dcb 20//返回长度
	ALIGN
}
//F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F140_Lib(void)
{
	dcb 140//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 180//月冻结Fn
	dcwu 0x952f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x952f
	dcb 20//返回长度
	ALIGN
}
//F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F141_Lib(void)
{
	dcb 141//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 181//月冻结Fn
	dcwu 0x953f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x953f
	dcb 20//返回长度
	ALIGN
}
//F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F142_Lib(void)
{
	dcb 142//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 182//月冻结Fn
	dcwu 0x955f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x955f
	dcb 20//返回长度
	ALIGN
}
//F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F143_Lib(void)
{
	dcb 143//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 183//月冻结Fn
	dcwu 0x956f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x956f
	dcb 20//返回长度
	ALIGN
}
//F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F144_Lib(void)
{
	dcb 144//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 184//月冻结Fn
	dcwu 0x954f
	dcb 0//返回长度
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0x954f
	dcb 20//返回长度
	ALIGN
}
//F145 当月正向有功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F145_Lib(void)
{
	dcb 145//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00010001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 189//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xA01F//当月正向有功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB01F//当月正向有功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA01F//当月正向有功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB01F//当月正向有功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F146 当月正向无功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F146_Lib(void)
{
	dcb 146//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00010001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 190//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xA11F//当月正向无功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB11F//当月正向无功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA11F//当月正向无功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB11F//当月正向无功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F147 当月反向有功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F147_Lib(void)
{
	dcb 147//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00010001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 191//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xA02F//当月反向有功最大需量
 	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB02F//当月反向有功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA02F//当月反向有功最大需量
 	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB02F//当月反向有功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F148 当月反向无功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F148_Lib(void)
{
	dcb 148//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00010001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 192//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xA12F//当月反向无功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB12F//当月反向无功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA12F//当月反向无功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB12F//当月反向无功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F149_Lib(void)
{
	dcb 149//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 193//月冻结Fn
	dcwu 0xA41F//上月正向有功最大需量
 	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB41F//上月正向有功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA41F//上月正向有功最大需量
 	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB41F//上月正向有功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F150_Lib(void)
{
	dcb 150//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 194//月冻结Fn
	dcwu 0xA51F//上月正向无功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB51F//上月正向无功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA51F//上月正向无功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB51F//上月正向无功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F151_Lib(void)
{
	dcb 151//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 195//月冻结Fn
	dcwu 0xA42F//上月反向有功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB42F//上月反向有功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA42F//上月反向有功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB42F//上月反向有功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}
//F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
__asm void ADDR_RM_DL645_1997_F152_Lib(void)
{
	dcb 152//实时Fn
	dcb 2//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00100001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 196//月冻结Fn
	dcwu 0xA52F//上月反向无功最大需量
	dcb 0//通信返回数据总长度,0=可变
	dcwu 0xB52F//上月反向无功最大需量发生时间
	dcb 0//通信返回数据总长度,0=可变
	
//当正确返回长度=0时的预计返回字节数
//	dcwu 0xA52F//上月反向无功最大需量
	dcb 15//通信返回数据总长度,0=可变
//	dcwu 0xB52F//上月反向无功最大需量发生时间
	dcb 20//通信返回数据总长度,0=可变
	ALIGN
}

//F153 第一时区冻结正向有功电能示值（总、费率1～M）

//F154 第二时区冻结正向有功电能示值（总、费率1～M）

//F155 第三时区冻结正向有功电能示值（总、费率1～M）

//F156 第四时区冻结正向有功电能示值（总、费率1～M）

//F157 第五时区冻结正向有功电能示值（总、费率1～M）

//F158 第六时区冻结正向有功电能示值（总、费率1～M）

//F159 第七时区冻结正向有功电能示值（总、费率1～M）

//F160 第八时区冻结正向有功电能示值（总、费率1～M）


//F161 电能表远程控制通断电状态及记录
//F162 电能表日历时钟
__asm void ADDR_RM_DL645_1997_F162_Lib(void)
{
	dcb 162//实时Fn
	dcb 2//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
  dcwu 0xC011//电能表日历时钟(时分秒)
  dcb 3//通信返回数据总长度,0=可变
  dcwu 0xC010//电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
	ALIGN
}

//F165 电能表开关操作次数及时间
__asm void ADDR_RM_DL645_1997_F165_Lib(void)
{
	dcb 165//实时Fn
	dcb 2//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
  dcwu 0xB212//编程总次数
  dcb 2//通信返回数据总长度,0=可变
  dcwu 0xB210//最近一次编程发生时刻
  dcb 4//通信返回数据总长度,0=可变
	//电能表尾盖打开次数
	//最近一次尾盖打开时间
	ALIGN
}
//F166 电能表参数修改次数及时间

//F167 电能表购、用电信息

//F168 电能表结算信息

#if ERC8==1
//扩展1 ERC8电能表参数变更事件用数据
__asm void ADDR_RM_DL645_1997_Ext1_Lib(void)
{
	dcb 0xE1//实时Fn
	dcb 19//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	dcwu 0xc331//第1日时段表1
	dcb 3//返回长度
	dcwu 0xc332//第1日时段表2
	dcb 3//返回长度
	dcwu 0xc333//第1日时段表3
	dcb 3//返回长度
	dcwu 0xc334//第1日时段表4
	dcb 3//返回长度
	dcwu 0xc335//第1日时段表5
	dcb 3//返回长度
	dcwu 0xc336//第1日时段表6
	dcb 3//返回长度
	dcwu 0xc337//第1日时段表7
	dcb 3//返回长度
	dcwu 0xc338//第1日时段表8
	dcb 3//返回长度
	dcwu 0xc339//第1日时段表9
	dcb 3//返回长度
	dcwu 0xc33a//第1日时段表10
	dcb 3//返回长度
	dcwu 0xc33b//第1日时段表11
	dcb 3//返回长度
	dcwu 0xc33c//第1日时段表12
	dcb 3//返回长度
	dcwu 0xc33d//第1日时段表13
	dcb 3//返回长度
	dcwu 0xc33e//第1日时段表14
	dcb 3//返回长度
	
	dcwu 0xc030//电表有功常数
	dcb 3//返回长度
	dcwu 0xc031//电表无功常数
	dcb 3//返回长度
	dcwu 0xc117//结算日(无功常数有些表可能没有，为正确跳过下2项，故读结算日在此)
	dcb 2//返回长度
  dcwu 0xB210//最近一次编程时间(当Fn=27不抄时)
  dcb 4//通信返回数据总长度
  dcwu 0xB213//最大需量清零次数(当Fn=27不抄时)
  dcb 2//通信返回数据总长度
	ALIGN
}
#endif
#if ERC13==1
//扩展2 ERC13电表故障信息事件用数据
__asm void ADDR_RM_DL645_1997_Ext2_Lib(void)
{
	dcb 0xE2//实时Fn
	dcb 4//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn

	dcwu 0xB310//断相总次数
	dcb 2//返回长度
  dcwu 0xB212//编程次数(当Fn=27不抄时)
  dcb 2//通信返回数据总长度
  dcwu 0xB213//最大需量清零次数(当Fn=27不抄时)
  dcb 2//通信返回数据总长度
  dcwu 0xC020//电表运行状态字(当Fn=28不抄时)
  dcb 1//通信返回数据总长度,0=可变
	ALIGN
}
#endif

//扩展3 日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_DL645_1997_Ext3_Lib(void)
{
	dcb 0xE3//实时Fn
	dcb 10//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb (0x2*(RMM_RS485_DateCongeal&1))+((RMM_Carrier_DateCongeal&1)<<2)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00001000//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
  dcwu 0xC010//电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
	dcwu 0x901f//(当前)上1次日正向有功电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x911f//上1次(当前)日正向无功（组合无功1）电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x902f//上1次(当前)日反向有功电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x912f//上1次(当前)日反向无功（组合无功2）电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x913f//上1次(当前)日1象限无功电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x915f//上1次(当前)日2象限无功电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x916f//上1次(当前)日3象限无功电能示值（总、费率1～M）
	dcb 0//返回长度
	dcwu 0x914f//上1次(当前)日4象限无功电能示值（总、费率1～M）
	dcb 0//返回长度
  dcwu 0xC010//再次确认电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
//当正确返回长度=0时的预计返回字节数
//  dcwu 0xC010//电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
//	dcwu 0x901f//上1次日正向有功电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x911f//上1次日正向无功（组合无功1）电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x902f//上1次日反向有功电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x912f//上1次日反向无功（组合无功2）电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x913f//上1次日1象限无功电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x915f//上1次日2象限无功电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x916f//上1次日3象限无功电能示值（总、费率1～M）
	dcb 20//返回长度
//	dcwu 0x914f//上1次日4象限无功电能示值（总、费率1～M）
	dcb 20//返回长度
//  dcwu 0xC010//再次确认电能表日历时钟(年月日周)
  dcb 4//通信返回数据总长度,0=可变
	ALIGN
}



#if (USER/100)==2//用户标识:山东版
//扩展 ERC10电压回路异常事件用数据
__asm void ADDR_RM_DL645_1997_FF3_Lib(void)
{
	dcb 0xF3//实时Fn
	dcb 8//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn

	dcwu 0xB310//断相总次数
	dcb 2//返回长度
	dcwu 0xb611//A相电压(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0xb612//B相电压(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0xb613//C相电压(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0xB621//A相电流(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0xB622//B相电流(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0xB623//C相电流(当事件产生时实时读)
	dcb 2//返回长度
	dcwu 0x9010//当前正向有功示值(当事件产生时实时读)
	dcb 4//返回长度
	ALIGN
}
#endif

const u32 DL645_1997_RMLib[]=
{
//F0 通用
	(u32)ADDR_RM_DL645_1997_F0_Lib,
#if (USER/100)==7//浙江用户
	0,
//F2 电能表主动上报事件(Fn计数固定值=2)
	(u32)ADDR_RM_DL645_1997_F2_Lib,
	0,
#endif

//电量数据总第1个抄,用以判断通信是否正常
//0 F129 当前正向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F129_Lib,
//1 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F130_Lib,
//2 F131 当前反向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F131_Lib,
//3 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F132_Lib,
//4 F133 当前一象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F133_Lib,
//5 F134 当前二象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F134_Lib,
//6 F135 当前三象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F135_Lib,
//7 F136 当前四象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F136_Lib,
//8 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F137_Lib,
//9 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F138_Lib,
//10 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F139_Lib,
//11 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F140_Lib,
//12 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F141_Lib,
//13 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F142_Lib,
//14 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F143_Lib,
//15 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F144_Lib,
//16 F145 当月正向有功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F145_Lib,
//17 F146 当月正向无功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F146_Lib,
//18 F147 当月反向有功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F147_Lib,
//19 F148 当月反向无功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F148_Lib,
//20 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F149_Lib,
//21 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F150_Lib,
//22 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F151_Lib,
//23 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_F152_Lib,

//24 F28 电表运行状态字及其变位标志
	(u32)ADDR_RM_DL645_1997_F28_Lib,
//25 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
	(u32)ADDR_RM_DL645_1997_F25_Lib,
//26 F26 A、B、C三相断相统计数据及最近一次断相记录
	(u32)ADDR_RM_DL645_1997_F26_Lib,
//27 F27 电能表日历时钟、编程次数及其最近一次操作时间
	(u32)ADDR_RM_DL645_1997_F27_Lib,
//28 F29 当前铜损、铁损有功总电能示值
	(u32)ADDR_RM_DL645_1997_F29_Lib,
//29 F30 上一结算日铜损、铁损有功总电能示值
	(u32)ADDR_RM_DL645_1997_F30_Lib,
//30 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
	(u32)ADDR_RM_DL645_1997_F31_Lib,
//31 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
	(u32)ADDR_RM_DL645_1997_F32_Lib,

//32 F49 当前电压、电流相位角
	0,
//33 F57 当前A、B、C三相电压、电流2～N次谐波有效值
	0,
//34 F58 当前A、B、C三相电压、电流2～N次谐波含有率
	0,
//35 F65 当前电容器投切状态
	0,
//36 F66 当前电容器累计补偿投入时间和次数
	0,
//37 F67 当日、当月电容器累计补偿的无功电能量
	0,
//38 F73 直流模拟量当前数据
	0,

//39 F153 第一时区冻结正向有功电能示值（总、费率1～M）
	0,
//40 F154 第二时区冻结正向有功电能示值（总、费率1～M）
	0,
//41 F155 第三时区冻结正向有功电能示值（总、费率1～M）
	0,
//42 F156 第四时区冻结正向有功电能示值（总、费率1～M）
	0,
//43 F157 第五时区冻结正向有功电能示值（总、费率1～M）
	0,
//44 F158 第六时区冻结正向有功电能示值（总、费率1～M）
	0,
//45 F159 第七时区冻结正向有功电能示值（总、费率1～M）
	0,
//46 F160 第八时区冻结正向有功电能示值（总、费率1～M）
	0,
//47 F161 电能表远程控制通断电状态及记录
	0,
//48 F162 电能表日历时钟
	(u32)ADDR_RM_DL645_1997_F162_Lib,
	
//49 F165 电能表开关操作次数及时间
	(u32)ADDR_RM_DL645_1997_F165_Lib,
//50 F166 电能表参数修改次数及时间
	0,
//51 F167 电能表购、用电信息
	0,
//52 F168 电能表结算信息
	0,

//F33～F40抄表数据组合总是放在最后
//53 F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F33_Lib,
//54 F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F34_Lib,
//55 F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F35_Lib,
//56 F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F36_Lib,
//57 F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F37_Lib,
//58 F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F38_Lib,
//59 F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F39_Lib,
//60 F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_DL645_1997_F40_Lib,

#if ERC8==1
//61 0xE1 扩展1 ERC8电能表参数变更事件用数据
	(u32)ADDR_RM_DL645_1997_Ext1_Lib,
#endif
#if ERC13==1
//62 扩展2 ERC13电表故障信息事件用数据
	(u32)ADDR_RM_DL645_1997_Ext2_Lib,
#endif
//63 扩展3 日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_DL645_1997_Ext3_Lib,
#if (USER/100)==2//用户标识:山东版
//扩展 ERC10电压回路异常事件用数据
	(u32)ADDR_RM_DL645_1997_FF3_Lib,
#endif	
	
};


u32 Get_RM_DL645_1997_MaxFn(void)
{
	return (sizeof(DL645_1997_RMLib)/4);
}

u32 Get_RM_DL645_1997_FnCount(u32 Fn)//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
{
	u32 i;
	u32 MaxFnCount;
	u8 * p8RMLib;

	MaxFnCount=Get_RM_DL645_1997_MaxFn();
	for(i=0;i<MaxFnCount;i++)
	{
		p8RMLib=(u8 *)DL645_1997_RMLib[i];
		if(Fn==p8RMLib[0])
		{
			return i;
		}
	}
	return 0xffffffff;
}

