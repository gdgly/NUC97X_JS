
//Terminal_ReadMeter_ShH_Lib
#include "Project.h"
#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_AFN0E_Event.h"
#include "../Device/MEMRW.h"


#if (USER/100)==5//上海版

//F0 通用
__asm void ADDR_RM_ShH_F0_Lib(void)
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




//F27 电能表日历时钟、编程次数及其最近一次操作时间等
__asm void ADDR_RM_ShH_F27_Lib(void)
{
	dcb 27//实时Fn
	dcb 3//4//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
/*
	0x17:	读日期时钟	
			SSmmHHDDMMYY	秒分时日月年	日期（6字节BCD码）
	0x2A:	读需量手动清零数据
			X.XXX	kW	手动清零最大需量（2字节BCD码）
			mmHHDDMM 	分时日月	最大需量出现时间（4字节BCD码）
			mmHHDDMM 	分时日月	需量手动清零时间（4字节BCD码）
	0x34:	读清事件编程操作记录
			MmhhDDMMYY	……	MmhhDDMMYY	年月日时分	编程时间、编程内容
			前1次	前n次	前3次		编程内容依据命令来确认
			传输时前1次在前，前3次在后
			传输格式：分时日月年
			MmhhDDMMYY：分时日月年（5字节）
			传输顺序：MmhhDDMMYY		
	0x3C:	读对时编程操作记录
			snmnhnDnMnYnsomohoDoMoYo	……	snmnhnDnMnYnsomohoDoMoYo	年月日时分秒	前1次至前12次对时前后的日期时间
			前1次	前n次	前12次		
			传输时前1次在前，前12次在后
			传输格式：秒分时日月年（对时后）秒分时日月年（对时前）
			snmnhnDnMnYn：对时后日期时间（6字节）
			somohoDoMoYo：对时前日期时间（6字节）		
*/
	dcb 0x17//电能表日历时钟YYMMDDWWHhmmss
	dcb 6//返回长度
//	dcb 0x2A//最近一次需量清零发生时刻
//	dcb 10//返回长度
	dcb 0x34//最近一次事件清零发生时刻
	dcb 15//返回长度
	dcb 0x3C//最近一次校时发生时刻
	dcb 144//返回长度
	ALIGN
}
//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_ShH_F33_Lib(void)
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
//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
__asm void ADDR_RM_ShH_F35_Lib(void)
{
	dcb 35//实时Fn
	dcb 0//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
#if AFN0CF33MaxPn==0
	dcb 2_00000000//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
#else
	dcb 2_00000010//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
#endif
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
	ALIGN
}
//F129 当前正向有功电能示值（总、费率1～M）
__asm void ADDR_RM_ShH_F129_Lib(void)
{
	dcb 129//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00011001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 161//日冻结Fn
	dcb 169//抄表日冻结Fn
	dcb 0//月冻结Fn
/*
	0x1C:	读当前总平谷电量
			XXXXXX.XX	kWh	总电量（4字节BCD码）
			0XXXXX.XX	kWh	平电量（4字节BCD码）
			0XXXXX.XX	kWh	谷电量（4字节BCD码）
*/
	dcb 0x1C//读当前累计总平谷电量
	dcb 12//返回长度             
	ALIGN
}
//F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
__asm void ADDR_RM_ShH_F137_Lib(void)
{
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
/*
	0x37:	读前3个月电量（总平谷）
			MmhhDDMMYYZ1P1G1	……	MmhhDDMMYYZ3P3G3	年月日时分kWh	前3个月总平谷电量自动转存数据
			前1月	前2月	前3月		
			传输时前1月在前，前3月在后
			传输格式：分时日月年总平谷电量
			mmhhDDMMYY：电量自动转存日期时间（5字节）
			Z1P1G1：前1月总平谷电量（12字节）
			Z3P3G3：前3月总平谷电量（12字节）		
*/
	dcb 0x37//读前3个月电量（总平谷），分时日月年总平谷
	dcb 51//返回长度       
	ALIGN
}

//F145 当月正向有功最大需量及发生时间（总、费率1～M）
//__asm void ADDR_RM_ShH_F145_Lib(void)
//{
//	dcb 145//实时Fn
//	dcb 1//通信标识个数
//	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
//	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
//	dcb 2_00010001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
//	dcb 0//小时冻结Fn
//	dcb 0//曲线冻结Fn
//	dcb 0//日冻结Fn
//	dcb 189//抄表日冻结Fn
//	dcb 0//月冻结Fn
///*
//	0x27:	读当前最大需量
//			X.XXX	kW	当前最大需量（2字节BCD码）
//			mmHHDDMM 	分时日月	最大需量出现时间（4字节BCD码）
//*/
//	dcb 0x27//当月正向有功最大需量
//	dcb 6//返回长度
//	ALIGN
//}

//F162 电能表日历时钟
__asm void ADDR_RM_ShH_F162_Lib(void)
{
	dcb 162//实时Fn
	dcb 1//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
/*
	0x17:	读日期时钟	
			SSmmHHDDMMYY	秒分时日月年	日期（6字节BCD码）
*/
	dcb 0x17//电能表日历时钟YYMMDDHhmmss	秒分时日月年
	dcb 6//返回长度
	ALIGN
}
//F166 电能表参数修改次数及时间
__asm void ADDR_RM_ShH_F166_Lib(void)
{
	dcb 166//实时Fn
	dcb 2//通信标识个数
	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
/*
	0x3C:	读对时编程操作记录
			snmnhnDnMnYnsomohoDoMoYo	……	snmnhnDnMnYnsomohoDoMoYo	年月日时分秒	前1次至前12次对时前后的日期时间
			前1次	前n次	前12次		
			传输时前1次在前，前12次在后
			传输格式：秒分时日月年（对时后）秒分时日月年（对时前）
			snmnhnDnMnYn：对时后日期时间（6字节）
			somohoDoMoYo：对时前日期时间（6字节）		
	0x36:	读写时段编程操作记录
			mmhhDDMMYYSDnSDo	……	mmhhDDMMYYSDnSDo	年月日时分时段表	前1次至前3次设置时段前后的数据
			前1次	前n次	前3次		
			传输时前1次在前，前3次在后
			传输格式：分时日月年时段表（后）时段表（前）
			mmhhDDMMYY：设置时段日期时间（5字节）
			SDn：设置时段后数据，共8时段（16字节）
			SDo：设置时段前数据，共8时段（16字节）		
*/
	dcb 0x3C//电能表最近一次时钟修改前和后时间
	dcb 144//返回长度
	dcb 0x36//最近一次电能表时段参数修改时间
	dcb 111//返回长度
}


//#if ERC8==1
////扩展1 ERC8电能表参数变更事件用数据
//__asm void ADDR_RM_ShH_FE1_Lib(void)
//{
//	dcb 0xE1//实时Fn
//	dcb 4//通信标识个数
//	dcb 0//数据头,0=终端抄表时间,1=终端抄表时间+费率数
//	dcb 2_00000110//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
//	dcb 2_00000001//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
//	dcb 0//小时冻结Fn
//	dcb 0//曲线冻结Fn
//	dcb 0//日冻结Fn
//	dcb 0//抄表日冻结Fn
//	dcb 0//月冻结Fn
///*
//	0x1B:	读时段
//			当实际使用时段数不足8个时，主站发送的时段数据将以最后一个实际使用的时段复制到剩余的各个时段，
//			若某一费率装置实际允许的时段数不足8个时，可忽略多余的时段数据。
//			费率装置在返回自身的时段数据时可以按照实际允许的时段数返回。
//			
//			字节顺序	 D7D6  	 D5D4	  D3D2D1D0	   字节数	说明
//			第1字节			  	 分钟（00～59）			 1个	时段的起始分钟
//			第2字节		 费率	时十位	   时个位		 1个	费率及时段的起始时
//			                  （0、1、2） （0～9）
//					图5  时段数据项格式
//
//			D7	D6	费率
//			1	0	尖费率
//			1	1	峰费率
//			0	0	平费率
//			0	1	谷费率
//			图6  费率代码格式
//	0x25:	读自动转存日
//			hhDD	时日	自动转存日（2字节BCD码）
//			传输时hh在前，DD在后		
//	0x24:	读常数（含电机常数）
//			XXXX	imp/kWh	电表常数（2字节二进制）
//			XXXX	imp/kWh	电机常数（2字节二进制）
//	0x2A:	读需量手动清零数据
//			X.XXX	kW	手动清零最大需量（2字节BCD码）
//			mmHHDDMM 	分时日月	最大需量出现时间（4字节BCD码）
//			mmHHDDMM 	分时日月	需量手动清零时间（4字节BCD码）
//*/
//	dcb 0x1B//
//	dcb 0//返回长度 不固定 可以根据时段数1-8返回长度2、4、6、8、10、12、14、16
//	dcb 0x25//读自动转存日  时日
//	dcb 2//返回长度
//	dcb 0x24//电表有功常数(互感器变比有些表可能没有，为正确跳过下2项，故读有功常数在此)
//	dcb 4//返回长度
//	dcb 0x2A//读需量手动清零数据
//	dcb 10//返回长度
//	ALIGN
//}
//#endif

//扩展3 日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
__asm void ADDR_RM_ShH_FE3_Lib(void)
{
	dcb 0xE3//实时Fn
	dcb 1//通信标识个数
	dcb 1//数据头,0=终端抄表时间,1=终端抄表时间+费率数
	dcb (0x2*(RMM_RS485_DateCongeal&1))+((RMM_Carrier_DateCongeal&1)<<2)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
	dcb 2_00001000//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	dcb 0//小时冻结Fn
	dcb 0//曲线冻结Fn
	dcb 0//日冻结Fn
	dcb 0//抄表日冻结Fn
	dcb 0//月冻结Fn
/*
	0x1C:	读当前总平谷电量
			XXXXXX.XX	kWh	总电量（4字节BCD码）
			0XXXXX.XX	kWh	平电量（4字节BCD码）
			0XXXXX.XX	kWh	谷电量（4字节BCD码）
*/
	dcb 0x1C//读当前累计总平谷电量
	dcb 12//返回长度             
	ALIGN
}

const u32 ShH_RMLib[]=
{
//F0 通用
	(u32)ADDR_RM_ShH_F0_Lib,
//F129第1个抄,用以判断通信是否正常
//0 F129 当前正向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_ShH_F129_Lib,
//1 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
	0,
//2 F131 当前反向有功电能示值（总、费率1～M）
	0,
//3 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
	0,
//4 F133 当前一象限无功电能示值（总、费率1～M）
	0,
//5 F134 当前二象限无功电能示值（总、费率1～M）
	0,
//6 F135 当前三象限无功电能示值（总、费率1～M）
	0,
//7 F136 当前四象限无功电能示值（总、费率1～M）
	0,
//8 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
	(u32)ADDR_RM_ShH_F137_Lib,
//9 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
	0,
//10 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
	0,
//11 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
	0,
//12 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
	0,
//13 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
	0,
//14 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
	0,
//15 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
	0,
//16 F145 当月正向有功最大需量及发生时间（总、费率1～M）
	0,//(u32)ADDR_RM_ShH_F145_Lib,
//17 F146 当月正向无功最大需量及发生时间（总、费率1～M）
	0,
//18 F147 当月反向有功最大需量及发生时间（总、费率1～M）
	0,
//19 F148 当月反向无功最大需量及发生时间（总、费率1～M）
	0,
//20 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
	0,
//21 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
	0,
//22 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
	0,
//23 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
	0,  

//24 F28 电表运行状态字及其变位标志
	0,
//25 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
	0,
//26 F26 A、B、C三相断相统计数据及最近一次断相记录
	0,
//27 F27 电能表日历时钟、编程次数及其最近一次操作时间
	(u32)ADDR_RM_ShH_F27_Lib,
//28 F29 当前铜损、铁损有功总电能示值
	0,
//29 F30 上一结算日铜损、铁损有功总电能示值
	0,
//30 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
	0,
//31 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
	0,

//32 F49 当前电压、电流相位角(外部458表)
	0,
//扩展17 当前电压、电流相位角(扩展用于终端内部交采)
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
	(u32)ADDR_RM_ShH_F162_Lib,
//49 F165 电能表开关操作次数及时间
	0,
//50 F166 电能表参数修改次数及时间
	(u32)ADDR_RM_ShH_F166_Lib,
//51 F167 电能表购、用电信息
	0,
//52 F168 电能表结算信息
	0,
//F33～F40抄表数据组合总是放在最后
//53 F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_ShH_F33_Lib,
//54 F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
	0,
//55 F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	(u32)ADDR_RM_ShH_F35_Lib,
//56 F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	0,
//57 F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
	0,
//58 F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
	0,
//59 F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	0,
//60 F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	0,
//扩展3 日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
	(u32)ADDR_RM_ShH_FE3_Lib,
};


u32 Get_RM_ShH_MaxFn(void)
{
	return (sizeof(ShH_RMLib)/4);
}


u32 Get_RM_ShH_FnCount(u32 Fn)//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
{
	u32 i;
	u32 MaxFnCount;
	u8 * p8RMLib;

	MaxFnCount=Get_RM_ShH_MaxFn();
	for(i=0;i<MaxFnCount;i++)
	{
		p8RMLib=(u8 *)ShH_RMLib[i];
		if(Fn==p8RMLib[0])
		{
			return i;
		}
	}
	return 0xffffffff;
}

void Read_ShH_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//上海规约读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	//鼎信要求上海规约表通信地址A5必须为0xff
	p8[6]=0xff;
	p8[7]=0x68;
	p8[8]=DI;
	p8[8]|=0x80;
	p8[9]=0;
	//cs
	p8[10]=0;
	for(i=0;i<10;i++)
	{
		p8[10]+=p8[i];
	}
	p8[11]=0x16;
}
#endif













