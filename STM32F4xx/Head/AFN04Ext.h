
//AFN04扩展
#ifndef AFN04Ext_H
#define AFN04Ext_H

#define ADDR_AFN04F104     ADDR_AFN04Ext_START//曲线冻结密度参数(测量点号)(同步const u8 CurveSetFnList[])
//测量点1
	//AFN0DF73 总加组有功功率曲线冻结密度参数 1
	//AFN0DF74 总加组无功功率曲线冻结密度参数 2
	//AFN0DF75 总加组有功电能量曲线冻结密度参数 3
	//AFN0DF76 总加组无功电能量曲线冻结密度参数 4
	//AFN0DF81 测量点有功功率曲线冻结密度参数 5
	//AFN0DF82 测量点A相有功功率曲线冻结密度参数 6
	//AFN0DF83 测量点B相有功功率曲线冻结密度参数 7
	//AFN0DF84 测量点C相有功功率曲线冻结密度参数 8
	//AFN0DF85 测量点无功功率曲线冻结密度参数 9
	//AFN0DF86 测量点A无功功率曲线冻结密度参数 10
	//AFN0DF87 测量点B无功功率曲线冻结密度参数 11
	//AFN0DF88 测量点C无功功率曲线冻结密度参数 12
	//AFN0DF89 测量点A相电压曲线冻结密度参数 13
	//AFN0DF90 测量点B相电压曲线冻结密度参数 14
	//AFN0DF91 测量点C相电压曲线冻结密度参数 15
	//AFN0DF92 测量点A相电流曲线冻结密度参数 16
	//AFN0DF93 测量点B相电流曲线冻结密度参数 17
	//AFN0DF94 测量点C相电流曲线冻结密度参数 18
	//AFN0DF95 测量点零序电流曲线冻结密度参数 19
	//AFN0DF97 测量点正向有功总电能量曲线冻结密度参数 20
	//AFN0DF98 测量点正向无功总电能量曲线冻结密度参数 21
	//AFN0DF99 测量点反向有功总电能量曲线冻结密度参数 22
	//AFN0DF100 测量点反向无功总电能量曲线冻结密度参数 23
	//AFN0DF101 测量点正向有功总电能示值冻结密度参数 24
	//AFN0DF102 测量点正向无功总电能示值冻结密度参数 25
	//AFN0DF103 测量点反向有功总电能示值冻结密度参数 26
	//AFN0DF104 测量点反向无功总电能示值冻结密度参数 27
	//AFN0DF105 测量点总功率因数冻结密度参数 28
	//AFN0DF106 测量点A相功率因数冻结密度参数 29
	//AFN0DF107 测量点B相功率因数冻结密度参数 30
	//AFN0DF108 测量点C相功率因数冻结密度参数 31
	//AFN0DF109 测量点电压相位角曲线冻结密度参数 32
	//AFN0DF110 测量点电流相位角曲线冻结密度参数 33
	//AFN0DF138 直流模拟量数据曲线冻结密度参数 34
	//AFN0DF145 测量点1象限无功总电能示值曲线冻结密度参数 35
	//AFN0DF146 测量点4象限无功总电能示值曲线冻结密度参数 36
	//AFN0DF147 测量点2象限无功总电能示值曲线冻结密度参数 37
	//AFN0DF148 测量点3象限无功总电能示值曲线冻结密度参数 38
//...
//测量点n
#define LEN_AFN04F104_PerPn     38
#define ADDR_AFN04F105     ADDR_AFN04F104+(LEN_AFN04F104_PerPn*AFN04F104MaxPn)//电能表数据分级归类参数(测量点号)
//测量点1
//数据分级类号BIN 1 (1-MaxMeterEventClass)
//...
//测量点n
#define ADDR_AFN04F106     ADDR_AFN04F105+(1*AFN04F105MaxPn)//电能表数据分级参数(数据分级类号)
//数据分级类号1
//第1 个电能表数据标识BIN 4 对应DL/T 645
//第1 个电能表数据等级BIN 1 (0表示删除)
//……
//第n 个电能表数据标识BIN 4 对应DL/T 645
//第n 个电能表数据等级BIN 1 (0表示删除)
//...
//数据分级类号N
//第1 个电能表数据标识BIN 4 对应DL/T 645
//第1 个电能表数据等级BIN 1 (0表示删除)
//……
//第n 个电能表数据标识BIN 4 对应DL/T 645
//第n 个电能表数据等级BIN 1 (0表示删除)
#define NUM_AFN04F106_PerC     96//每分级类存储的电能表数据标识个数(最大个数=LEN_DATABUFF/5)
#define ADDR_AFN04F107     ADDR_AFN04F106+((5*NUM_AFN04F106_PerC)*AFN04F106MaxPn)//电能表数据分级周期参数(数据等级号)
//数据等级号1
//周期数值BIN 1
//周期单位BIN 1
//..
//数据等级号N

#define ADDR_AutoReportWord     ((ADDR_AFN04F107+(2*AFN04F107MaxPn)+0xfff)&0xfffff000)//电能表主动上报状态字
//AFN04F10配置序号1
//96byte 主动上报事件新增次数hex(偏移对应状态字的位号,初始化值0)
//1BYTE 上96BYTE的数据或(初始化值0)
//12byte 主动上报事件的位标志寄存,用于发复位命令(初始化值0)
//...
//AFN04F10配置序号n
#define LEN_AutoReportWord     (96+1+12)
#define ADDR_MeterEventCount     ADDR_AutoReportWord+(LEN_AutoReportWord*MaxRS485AddCarrierPn)//寄存电能表各事件原发生次数
//测量点1
//3byte 第1个电能表事件数据标识原发生次数BCD(初始化值0xff)
//1byte 新增次数hex(初始化值0xff)
//...
//3byte 第n个电能表事件数据标识原发生次数BCD(初始化值0xff)
//1byte 新增次数hex(初始化值0xff)

//3byte 0x03060000//39 B68辅助电源失电次数后的3byte总累计时间
//3byte 0x03050000//45 B74全失压次数后的3byte总累计时间

//...
//测量点n
#define NUM_MeterEventDI     (60+6)//抄读的标识个数
#define LEN_MeterEventCount     ((4*NUM_MeterEventDI)+6)//每测量点长度

#define ADDR_MeterEvent          ((ADDR_MeterEventCount+(LEN_MeterEventCount*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//读取的仪表事件
//电能表事件1
//1byte 端口号(1,2,3,4或31)
//6byte 电能表通信地址(BCD)

//1byte 长度(值4+3或4+6或4+18,其他值无效;专用于发生次数存储)//上海:长度(值4+3或4+6或4+18或4+3+4+3其他值无效,发生次数和总累计时间,4+3+4+3有单独的总累计时间数据标识和数据)
//2byte 总长度(1-10次事件数据存储)

//22byte 数据标识和数据(FLASH空间固定保留22byte;专用于发生次数存储)//上海:发生次数和总累计时间
//256byte 总数据标识个数,数据长度数据标识和数据,...(256或1030)//上海:无总数据标识个数,数据长度单元
//...
//电能表事件256
#define LEN_MeterEvent     (10+22+256)//(10+22+1030)//不能大于LEN_UARTnTx否则主动上报会不能上报死循环

#define ADDR_ACSampleCurve     ((ADDR_MeterEvent+(LEN_MeterEvent*256)+0xfff)&0xfffff000)//内部交采1分钟电压曲线,按冻结时的年月日时分对00年1月1日0时0分计算存储位置(初始化时自动选择扇区或块擦除为0xff值)
//5byte 年月日时分
//2byte A相电压曲线
//2byte B相电压曲线
//2byte C相电压曲线
#define LEN_ACSampleCurve     11//每点字节数

	#if ((USER/100)==8)//用户标识：通用版辽宁
#define ADDR_AFN04Ext1_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
		#if ((ADDR_AFN04Ext1_END)-(ADDR_AFN04Ext_START))>0x000E0000
			#error "((ADDR_AFN04Ext1_END)-(ADDR_AFN04Ext_START))>0x000E0000"
		#endif
#define ADDR_AFN0DExt_Start     (ADDR_EFLASH_Start+0x007E0000)
//3byte 日冻结类数据时标 Td_d 日月年
#define MaxDate_AFN0DF321F322     (31)//AFN0DF321、F322最大存储天数
#define ADDR_AFN0DF321_Ext     (ADDR_AFN0DExt_Start+3)//电表对时后误差情况统计数量P0
//2byte 时间超差 5min 测量点数量
//2byte 超过时间阈值未超差5min测量点数量 
#define ADDR_AFN0DF322_Ext     (ADDR_AFN0DF321_Ext+4)//电表对时后误差情况统计明细Pn
//3byte 测量点 n 误差时间
		#if (3+4+MaxRS485AddCarrierPn*3)*MaxDate_AFN0DF321F322>0x00020000
			#error "(3+4+MaxRS485AddCarrierPn*3)*MaxDate_AFN0DF321F322>0x00020000"
		#endif
#define ADDR_AFN0DExt_END     (ADDR_AFN0DExt_Start+0x00020000)
#define ADDR_AFN04Ext_END     (ADDR_AFN0DExt_END)
	#else
#define ADDR_AFN04Ext_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
	#endif


#if ((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000
	#error "((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000"
#endif
//#define ADDR_AFN04Ext_END     ((ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal)+0xfff)&0xfffff000)
//
//
//#if ((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000
//	#error "((ADDR_AFN04Ext_END)-(ADDR_AFN04Ext_START))>0x00100000"
//#endif


#endif






















