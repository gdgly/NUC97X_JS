

#ifndef AFN0C_H
#define AFN0C_H



//终端1类实时数据
#define ADDR_AFN0CF2     ADDR_AFN0C_Start//终端日历时钟
//秒分时日月(周)年
#define ADDR_AFN0CF4     ADDR_AFN0CF2+6//终端通信状态
//1字节

//16字节无线模块本地IP地址,ASCII
//6字节无线模块本地端口号,ASCII
//6字节无线模块信号分贝值,ASCII,单位dB

//4字节以太网自动获得IP地址(字段1-字段4 HEX)
//4字节以太网自动获得子掩码IP地址(字段1-字段4 HEX)
//4字节以太网自动获得网关IP地址(字段1-字段4 HEX)
//4字节以太网自动获得服务器IP地址(字段1-字段4 HEX)
//4字节以太网自动获得租借时间HEX秒(低到高与接收是高到低相反)
#define ADDR_AFN0CF5     ADDR_AFN0CF4+1+28+20//终端控制设置状态
//总加组有效标志位相关
#define ADDR_AFN0CF6     ADDR_AFN0CF5+2+(6*AFN0CF5MaxPn)//终端当前控制状态
//总加组有效标志位相关

#define ADDR_AFN0CF11     ADDR_AFN0CF6+3+(8*AFN0CF6MaxPn)//终端集中抄表状态信息
//本项数据块个数n=4	BIN	1(1-31) 
//终端通信端口号(2)	BIN	1
//要抄电表总数	BIN	2;(当值=0时本块无效)
//当前抄表工作状态标志	BS8	1
//抄表成功块数	BIN	2
//抄重点表成功块数	BIN	1
//抄表开始时间	见附录A.1	6
//抄表结束时间	见附录A.1	6
//终端通信端口号(3)	BIN	1
//...
//终端通信端口号(4)	BIN	1
//...
//终端通信端口号(31)	BIN	1
//...
//终端通信端口号(未用)	BIN	1
//...
#define ADDR_AFN0CF17     ADDR_AFN0CF11+1+(19*5)//当前总加有功功率(总加组号)
//2BYTE 数据格式2
#define ADDR_AFN0CF18     ADDR_AFN0CF17+(2*AFN0CF17MaxPn)//当前总加无功功率(总加组号)
//2BYTE 数据格式2


#define ADDR_AFN0CF25     ADDR_AFN0CF18+(2*AFN0CF18MaxPn)//F25：当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率(测量点号)
//终端抄表时间	见附录A.15	分时日月年	5
//当前总有功功率	见附录A.9	kW	3
//当前A相有功功率	见附录A.9	kW	3
//当前B相有功功率	见附录A.9	kW	3
//当前C相有功功率	见附录A.9	kW	3
//当前总无功功率	见附录A.9	kW	3
//当前A相无功功率	见附录A.9	kW	3
//当前B相无功功率	见附录A.9	kW	3
//当前C相无功功率	见附录A.9	kW	3
//当前总功率因数	见附录A.5	％	2
//当前A相功率因数	见附录A.5	％	2
//当前B相功率因数	见附录A.5	％	2
//当前C相功率因数	见附录A.5	％	2
//当前A相电压	见附录A.7	V	2
//当前B相电压	见附录A.7	V	2
//当前C相电压	见附录A.7	V	2
//当前A相电流	见附录A.25	A	3
//当前B相电流	见附录A.25	A	3
//当前C相电流	见附录A.25	A	3
//当前零序电流	见附录A.25	A	3
//当前总视在功率	见附录A.9	kVA	3
//当前A相视在功率	见附录A.9	kVA	3
//当前B相视在功率	见附录A.9	kVA	3
//当前C相视在功率	见附录A.9	kVA	3
#define ADDR_AFN0CF26     ADDR_AFN0CF25+(67*AFN0CF25MaxPn)//A、B、C三相断相统计数据及最近一次断相记录
//终端抄表时间	见附录A.15	分时日月年	5
//总断相次数	见附录A.10	次	3
//A相断相次数	见附录A.10	次	3
//B相断相次数	见附录A.10	次	3
//C相断相次数	见附录A.10	次	3
//断相时间累计值	见附录A.10	min	3
//A相断相时间累计值	见附录A.10	min	3
//B相断相时间累计值	见附录A.10	min	3
//C相断相时间累计值	见附录A.10	min	3
//最近一次断相起始时刻	见附录A.17	分时日月	4
//A相最近断相起始时刻	见附录A.17	分时日月	4
//B相最近断相起始时刻	见附录A.17	分时日月	4
//C相最近断相起始时刻	见附录A.17	分时日月	4
//最近一次断相结束时刻	见附录A.17	分时日月	4
//A相最近断相结束时刻	见附录A.17	分时日月	4
//B相最近断相结束时刻	见附录A.17	分时日月	4
//C相最近断相结束时刻	见附录A.17	分时日月	4
#define ADDR_AFN0CF27     ADDR_AFN0CF26+(61*AFN0CF26MaxPn)//电能表日历时钟、编程次数及其最近一次操作时间
//终端抄表时间	见附录A.15	分时日月年	5
//电能表日历时钟	见附录A.1	秒分时日月年	6
//电池工作时间	见附录A.27	分钟	4
//编程总次数	见附录A.10	次	3
//最近一次编程发生时刻	见附录A.1	秒分时日月年	6
//电表清零总次数	见附录A.10	次	3
//最近一次清零发生时刻	见附录A.1	秒分时日月年	6
//需量清零总次数	见附录A.10	次	3
//最近一次清零发生时刻	见附录A.1	秒分时日月年	6
//事件清零总次数	见附录A.10	次	3
//最近一次清零发生时刻	见附录A.1	秒分时日月年	6
//校时总次数	见附录A.10	次	3
//最近一次校时发生时刻	见附录A.1	秒分时日月年	6
#define ADDR_AFN0CF28     ADDR_AFN0CF27+(60*AFN0CF27MaxPn)//电表运行状态字及其变位标志
//终端抄表时间	见附录A.15	分时日月年	5
//电表运行状态字变位标志1	BS16		2
//……	……	……	……
//电表运行状态字变位标志7	BS16		2

//电表运行状态字1	BS16		2
//……	……	……	……
//电表运行状态字7	BS16		2
#define ADDR_AFN0CF29     ADDR_AFN0CF28+(33*AFN0CF28MaxPn)//当前铜损、铁损有功总电能示值
//终端抄表时间	见附录A.15	分时日月年	5
//当前铜损有功总电能示值	见附录A.14	kWh	5
//当前铁损有功总电能示值	见附录A.14	kWh	5
#define ADDR_AFN0CF30     ADDR_AFN0CF29+(15*AFN0CF29MaxPn)//上一结算日铜损、铁损有功总电能示值
//终端抄表时间	见附录A.15	分时日月年	5
//当前铜损有功总电能示值	见附录A.14	kWh	5
//当前铁损有功总电能示值	见附录A.14	kWh	5
#define ADDR_AFN0CF31     ADDR_AFN0CF30+(15*AFN0CF30MaxPn)//当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//终端抄表时间	见附录A.15	分时日月年	5
//当前A相正向有功电能示值	见附录A.14	kWh	5
//当前A相反向有功电能示值	见附录A.14	kWh	5
//当前A相组合无功1电能示值	见附录A.11	Kvarh	4
//当前A相组合无功2电能示值	见附录A.11	Kvarh	4
//当前B相正向有功电能示值	见附录A.14	kWh	5
//当前B相反向有功电能示值	见附录A.14	kWh	5
//当前B相组合无功1电能示值	见附录A.11	Kvarh	4
//当前B相组合无功2电能示值	见附录A.11	Kvarh	4
//当前C相正向有功电能示值	见附录A.14	kWh	5
//当前C相反向有功电能示值	见附录A.14	kWh	5
//当前C相组合无功1电能示值	见附录A.11	Kvarh	4
//当前C相组合无功2电能示值	见附录A.11	Kvarh	4
#define ADDR_AFN0CF32     ADDR_AFN0CF31+(59*AFN0CF31MaxPn)//上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//终端抄表时间	见附录A.15	分时日月年	5
//当前A相正向有功电能示值	见附录A.14	kWh	5
//当前A相反向有功电能示值	见附录A.14	kWh	5
//当前A相组合无功1电能示值	见附录A.11	Kvarh	4
//当前A相组合无功2电能示值	见附录A.11	Kvarh	4
//当前B相正向有功电能示值	见附录A.14	kWh	5
//当前B相反向有功电能示值	见附录A.14	kWh	5
//当前B相组合无功1电能示值	见附录A.11	Kvarh	4
//当前B相组合无功2电能示值	见附录A.11	Kvarh	4
//当前C相正向有功电能示值	见附录A.14	kWh	5
//当前C相反向有功电能示值	见附录A.14	kWh	5
//当前C相组合无功1电能示值	见附录A.11	Kvarh	4
//当前C相组合无功2电能示值	见附录A.11	Kvarh	4
#define ADDR_AFN0CF33     ADDR_AFN0CF32+(59*AFN0CF32MaxPn)//当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//当前正向有功总电能示值	见附录A.14	kWh	5
//当前费率1正向有功总电能示值	见附录A.14	kWh	5
//……	……	……	……
//当前费率M正向有功总电能示值	见附录A.14	kWh	5
//当前正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//当前费率1正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前费率M正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//当前一象限无功总电能示值	见附录A.11	kvarh	4
//当前一象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前一象限费率M无功电能示值	见附录A.11	kvarh	4
//当前四象限无功总电能示值	见附录A.11	kvarh	4
//当前四象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前四象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF34     ADDR_AFN0CF33+((6+(17*(1+TMaxTariff)))*AFN0CF33MaxPn)//当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//当前正向有功总电能示值	见附录A.14	kWh	5
//当前费率1正向有功总电能示值	见附录A.14	kWh	5
//……	……	……	……
//当前费率M正向有功总电能示值	见附录A.14	kWh	5
//当前正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//当前费率1正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前费率M正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//当前一象限无功总电能示值	见附录A.11	kvarh	4
//当前一象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前一象限费率M无功电能示值	见附录A.11	kvarh	4
//当前四象限无功总电能示值	见附录A.11	kvarh	4
//当前四象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//当前四象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF35     ADDR_AFN0CF34+((6+(17*(1+TMaxTariff)))*AFN0CF34MaxPn)//当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//当月正向有功总最大需量	见附录A.23	kW	3
//当月正向有功费率1最大需量	见附录A.23	kW	3
//……	……	……	……
//当月正向有功费率M最大需量	见附录A.23	kW	3
//当月正向有功总最大需量发生时间	见附录A.17	分时日月	4
//当月正向有功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//当月正向有功费率M最大需量发生时间	见附录A.17	分时日月	4
//当月正向无功总最大需量	见附录A.23	kvar	3
//当月正向无功费率1最大需量	见附录A.23	kvar	3
//……	……	……	……
//当月正向无功费率M最大需量	见附录A.23	kvar	3
//当月正向无功总最大需量发生时间	见附录A.17	分时日月	4
//当月正向无功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//当月正向无功费率M最大需量发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0CF36     ADDR_AFN0CF35+((6+(14*(1+TMaxTariff)))*AFN0CF35MaxPn)//当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//当月正向有功总最大需量	见附录A.23	kW	3
//当月正向有功费率1最大需量	见附录A.23	kW	3
//……	……	……	……
//当月正向有功费率M最大需量	见附录A.23	kW	3
//当月正向有功总最大需量发生时间	见附录A.17	分时日月	4
//当月正向有功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//当月正向有功费率M最大需量发生时间	见附录A.17	分时日月	4
//当月正向无功总最大需量	见附录A.23	kvar	3
//当月正向无功费率1最大需量	见附录A.23	kvar	3
//……	……	……	……
//当月正向无功费率M最大需量	见附录A.23	kvar	3
//当月正向无功总最大需量发生时间	见附录A.17	分时日月	4
//当月正向无功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//当月正向无功费率M最大需量发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0CF37     ADDR_AFN0CF36+((6+(14*(1+TMaxTariff)))*AFN0CF36MaxPn)//上月（上一结算日）正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//上月（上一结算日）正向有功总电能示值	见附录A.14	kWh	5
//上月（上一结算日）费率1正向有功总电能示值	见附录A.14	kWh	5
//……	……		……
//上月（上一结算日）费率M正向有功总电能示值	见附录A.14	kWh	5
//上月（上一结算日）正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）费率1正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//……	……		……
//上月（上一结算日）费率M正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）一象限无功总电能示值	见附录A.11	kvarh	4
//表136（续）
//数据内容	数据格式	单位	字节数
//上月（上一结算日）一象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……		……
//上月（上一结算日）一象限费率M无功电能示值	见附录A.11	kvarh	4
//上月（上一结算日）四象限无功总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）四象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//上月（上一结算日）四象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF38     ADDR_AFN0CF37+((6+(17*(1+TMaxTariff)))*AFN0CF37MaxPn)//上月（上一结算日）反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//上月（上一结算日）反向有功总电能示值	见附录A.14	kWh	5
//上月（上一结算日）费率1反向有功总电能示值	见附录A.14	kWh	5
//……	……	……	……
//上月（上一结算日）费率M反向有功总电能示值	见附录A.14	kWh	5
//上月（上一结算日）反向无功（组合无功2）总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）费率1反向无功（组合无功2）总电能示值	见附录A.11	kvarh	4
//……	……	……	……
//上月（上一结算日）费率M反向无功（组合无功2）总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）二象限无功总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）二象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//上月（上一结算日）二象限费率M无功电能示值	见附录A.11	kvarh	4
//上月（上一结算日）三象限无功总电能示值	见附录A.11	kvarh	4
//上月（上一结算日）三象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//上月（上一结算日）三象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF39     ADDR_AFN0CF38+((6+(17*(1+TMaxTariff)))*AFN0CF38MaxPn)//上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//上月（上一结算日）正向有功总最大需量	见附录A.23	kW	3
//上月（上一结算日）正向有功费率1最大需量	见附录A.23	kW	3
//……	……	……	……
//上月（上一结算日）正向有功费率M最大需量	见附录A.23	kW	3
//上月（上一结算日）正向有功总最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）正向有功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//上月（上一结算日）正向有功费率M最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）正向无功总最大需量	见附录A.23	kvar	3
//上月（上一结算日）正向无功费率1最大需量	见附录A.23	kvar	3
//……	……	……	……
//上月（上一结算日）正向无功费率M最大需量	见附录A.23	kvar	3
//上月（上一结算日）正向无功总最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）正向无功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//上月（上一结算日）正向无功费率M最大需量发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0CF40     ADDR_AFN0CF39+((6+(14*(1+TMaxTariff)))*AFN0CF39MaxPn)//上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M（1≤M≤12）	BIN	个	1
//上月（上一结算日）反向有功总最大需量	见附录A.23	kW	3
//上月（上一结算日）反向有功费率1最大需量	见附录A.23	kW	3
//……	……	……	……
//上月（上一结算日）反向有功费率M最大需量	见附录A.23	kW	3
//上月（上一结算日）反向有功总最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）反向有功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//上月（上一结算日）反向有功费率M最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）反向无功总最大需量	见附录A.23	kvar	3
//表139（续）
//数据内容	数据格式	单位	字节数
//上月（上一结算日）反向无功费率1最大需量	见附录A.23	kvar	3
//……	……	……	……
//上月（上一结算日）反向无功费率M最大需量	见附录A.23	kvar	3
//上月（上一结算日）反向无功总最大需量发生时间	见附录A.17	分时日月	4
//上月（上一结算日）反向无功费率1最大需量发生时间	见附录A.17	分时日月	4
//……	……	……	……
//上月（上一结算日）反向无功费率M最大需量发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0CF49     ADDR_AFN0CF40+((6+(14*(1+TMaxTariff)))*AFN0CF40MaxPn)//当前电压、电流相位角
//Uab/Ua相位角	见附录A.5	度	2
//Ub相位角	见附录A.5	度	2
//Ucb/Uc相位角	见附录A.5	度	2
//Ia相位角	见附录A.5	度	2
//Ib相位角	见附录A.5	度	2
//Ic相位角	见附录A.5	度	2
#define ADDR_AFN0CF57     ADDR_AFN0CF49+(12*AFN0CF49MaxPn)//当前A、B、C三相电压、电流2～N次谐波有效值
//谐波次数N（N≤19）	BIN	次	1
//A相2次谐波电压	见附录A.7	V	2
//……	……	……	……
//A相N次谐波电压	见附录A.7	V	2
//B相2次谐波电压	见附录A.7	V	2
//……	……	……	……
//B相N次谐波电压	见附录A.7	V	2
//C相2次谐波电压	见附录A.7	V	2
//……	……	……	……
//C相N次谐波电压	见附录A.7	V	2
//A相2次谐波电流	见附录A.6	A	2
//……	……	……	……
//A相N次谐波电流	见附录A.6	A	2
//B相2次谐波电流	见附录A.6	A	2
//……	……	……	……
//B相N次谐波电流	见附录A.6	A	2
//C相2次谐波电流	见附录A.6	A	2
//……	……	……	……
//C相N次谐波电流	见附录A.6	A	2
#define LEN_AFN0CF57     (1+216)
#define ADDR_AFN0CF58     ADDR_AFN0CF57+(LEN_AFN0CF57*AFN0CF57MaxPn)//当前A、B、C三相电压、电流2～N次谐波含有率
//谐波次数N（N≤19）	BIN	次	1
//A相总谐波电压含有率	见附录A.5	％	2
//A相2次谐波电压含有率	见附录A.5	％	2
//……	……	……	……
//A相N次谐波电压含有率	见附录A.5	％	2
//B相总谐波电压含有率	见附录A.5	％	2
//B相2次电压谐波含有率	见附录A.5	％	2
//……	……	……	……
//B相N次电压谐波含有率	见附录A.5	％	2
//C相总谐波电压含有率	见附录A.5	％	2
//C相2次电压谐波含有率	见附录A.5	％	2
//……	……	……	……
//C相N次谐波电压含有率	见附录A.5	％	2
//A相2次谐波电流含有率	见附录A.5	％	2
//……	……	……	……
//A相N次谐波电流含有率	见附录A.5	％	2
//B相2次谐波电流含有率	见附录A.5	％	2
//……	……	……	……
//B相N次谐波电流含有率	见附录A.5	％	2
//C相2次谐波电流含有率	见附录A.5	％	2
//……	……	……	……
//C相N次谐波电流含有率	见附录A.5	％	2
#define LEN_AFN0CF58     (1+222)
#define ADDR_AFN0CF65     ADDR_AFN0CF58+(LEN_AFN0CF58*AFN0CF58MaxPn)//当前电容器投切状态
//1BYTE 运行方式	BS8	1
//2BYTE 电容器的投切状态	BS16	2
#define ADDR_AFN0CF73     ADDR_AFN0CF65+(3*AFN0CF65MaxPn)//直流模拟量当前数据
//2BYTE 直流模拟量当前数据	数据格式02	2





#define ADDR_AFN0CF129     ADDR_AFN0CF73+(2*AFN0CF73MaxPn)//当前正向有功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//正向有功总电能示值	见附录A.14	kWh	5
//费率1正向有功总电能示值	见附录A.14	kWh	5
//……	……	……	……
//费率M正向有功总电能示值	见附录A.14	kWh	5
#define ADDR_AFN0CF130     ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*AFN0CF129MaxPn)//当前正向无功（组合无功1）电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//费率1正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//……	……	……	……
//费率M正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF131     ADDR_AFN0CF130+((6+(4*(1+TMaxTariff)))*AFN0CF130MaxPn)//当前反向有功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//反向有功总电能示值	见附录A.14	kWh	5
//费率1反向有功总电能示值	见附录A.14	kWh	5
//……	……	……	……
//费率M反向有功总电能示值	见附录A.14	kWh	5
#define ADDR_AFN0CF132     ADDR_AFN0CF131+((6+(5*(1+TMaxTariff)))*AFN0CF131MaxPn)//当前反向无功（组合无功1）电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//反向无功总电能示值	见附录A.11	kvarh	4
//费率1反向无功总电能示值	见附录A.11	kvarh	4
//……	……	……	……
//费率M反向无功总电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF133     ADDR_AFN0CF132+((6+(4*(1+TMaxTariff)))*AFN0CF132MaxPn)//当前1象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//1象限无功总电能示值	见附录A.11	kvarh	4
//1象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//1象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF134     ADDR_AFN0CF133+((6+(4*(1+TMaxTariff)))*AFN0CF133MaxPn)//当前2象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//2象限无功总电能示值	见附录A.11	kvarh	4
//2象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//2象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF135     ADDR_AFN0CF134+((6+(4*(1+TMaxTariff)))*AFN0CF134MaxPn)//当前3象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//3象限无功总电能示值	见附录A.11	kvarh	4
//3象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//3象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF136     ADDR_AFN0CF135+((6+(4*(1+TMaxTariff)))*AFN0CF135MaxPn)//当前4象限无功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//4象限无功总电能示值	见附录A.11	kvarh	4
//4象限费率1无功电能示值	见附录A.11	kvarh	4
//……	……	……	……
//4象限费率M无功电能示值	见附录A.11	kvarh	4
#define ADDR_AFN0CF137     ADDR_AFN0CF136+((6+(4*(1+TMaxTariff)))*AFN0CF136MaxPn)//上月（上一结算日）正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF138     ADDR_AFN0CF137+((6+(5*(1+TMaxTariff)))*AFN0CF137MaxPn)//上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）

#define ADDR_AFN0CF139     ADDR_AFN0CF138+((6+(4*(1+TMaxTariff)))*AFN0CF138MaxPn)//上月（上一结算日）反向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF140     ADDR_AFN0CF139+((6+(5*(1+TMaxTariff)))*AFN0CF139MaxPn)//上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）

#define ADDR_AFN0CF141     ADDR_AFN0CF140+((6+(4*(1+TMaxTariff)))*AFN0CF140MaxPn)//上月（上一结算日）1象限无功电能示值（总、费率1～M）

#define ADDR_AFN0CF142     ADDR_AFN0CF141+((6+(4*(1+TMaxTariff)))*AFN0CF141MaxPn)//上月（上一结算日）2象限无功电能示值（总、费率1～M）

#define ADDR_AFN0CF143     ADDR_AFN0CF142+((6+(4*(1+TMaxTariff)))*AFN0CF142MaxPn)//上月（上一结算日）3象限无功电能示值（总、费率1～M）

#define ADDR_AFN0CF144     ADDR_AFN0CF143+((6+(4*(1+TMaxTariff)))*AFN0CF143MaxPn)//上月（上一结算日）4象限无功电能示值（总、费率1～M）

#define ADDR_AFN0CF145     ADDR_AFN0CF144+((6+(4*(1+TMaxTariff)))*AFN0CF144MaxPn)//当月正向有功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF146     ADDR_AFN0CF145+((6+(7*(1+TMaxTariff)))*AFN0CF145MaxPn)//当月正向无功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF147     ADDR_AFN0CF146+((6+(7*(1+TMaxTariff)))*AFN0CF146MaxPn)//当月反向有功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF148     ADDR_AFN0CF147+((6+(7*(1+TMaxTariff)))*AFN0CF147MaxPn)//当月反向无功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF149     ADDR_AFN0CF148+((6+(7*(1+TMaxTariff)))*AFN0CF148MaxPn)//上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF150     ADDR_AFN0CF149+((6+(7*(1+TMaxTariff)))*AFN0CF149MaxPn)//上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF151     ADDR_AFN0CF150+((6+(7*(1+TMaxTariff)))*AFN0CF150MaxPn)//上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF152     ADDR_AFN0CF151+((6+(7*(1+TMaxTariff)))*AFN0CF151MaxPn)//上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）

#define ADDR_AFN0CF153     ADDR_AFN0CF152+((6+(7*(1+TMaxTariff)))*AFN0CF152MaxPn)//第1时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF154     ADDR_AFN0CF153+((6+(5*(1+TMaxTariff)))*AFN0CF153MaxPn)//第2时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF155     ADDR_AFN0CF154+((6+(5*(1+TMaxTariff)))*AFN0CF154MaxPn)//第3时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF156     ADDR_AFN0CF155+((6+(5*(1+TMaxTariff)))*AFN0CF155MaxPn)//第4时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF157     ADDR_AFN0CF156+((6+(5*(1+TMaxTariff)))*AFN0CF156MaxPn)//第5时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF158     ADDR_AFN0CF157+((6+(5*(1+TMaxTariff)))*AFN0CF157MaxPn)//第6时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF159     ADDR_AFN0CF158+((6+(5*(1+TMaxTariff)))*AFN0CF158MaxPn)//第7时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF160     ADDR_AFN0CF159+((6+(5*(1+TMaxTariff)))*AFN0CF159MaxPn)//第8时区冻结正向有功电能示值（总、费率1～M）

#define ADDR_AFN0CF161     ADDR_AFN0CF160+((6+(5*(1+TMaxTariff)))*AFN0CF160MaxPn)//电能表远程控制通断电状态及记录
//终端抄表时间	见附录A.15	分时日月年	5
//电能表通断电状态	BIN		1
//最近一次合闸发生时刻	见附录A.1	秒分时日月年	6
//最近一次跳闸发生时刻	见附录A.1	秒分时日月年	6
#define ADDR_AFN0CF162     ADDR_AFN0CF161+((5+13)*AFN0CF161MaxPn)//电能表日历时钟
//终端抄表时间	秒分时日月年	6
//电能表日历时钟见附录A.1 秒分时日月年  6
#define ADDR_AFN0CF165     ADDR_AFN0CF162+((6+6)*AFN0CF162MaxPn)//电能表开关操作次数及时间
//终端抄表时间	见附录A.15	分时日月年	5
//编程开关操作次数	见附录A.10	次	3
//最近一次编程开关操作时间	见附录A.1	秒分时日月年	6
//电能表尾盖打开次数	见附录A.10	次	3
//最近一次尾盖打开时间	见附录A.1	秒分时日月年	6
#define ADDR_AFN0CF166     ADDR_AFN0CF165+((5+18)*AFN0CF165MaxPn)//电能表参数修改次数及时间
//终端抄表时间	见附录A.15	分时日月年	5
//电能表时钟修改次数	见附录A.10	次	3
//电能表最近一次时钟修改前时间	见附录A.1	秒分时日月年	6
//电能表最近一次时钟修改后时间	见附录A.1	秒分时日月年	6
//电能表时段参数修改次数	见附录A.10	次	3
//最近一次电能表时段参数修改时间	见附录A.1	秒分时日月年	6
#define ADDR_AFN0CF167     ADDR_AFN0CF166+((5+24)*AFN0CF166MaxPn)//电能表购、用电信息
//终端抄表时间	见附录A.15	分时日月年	5
//购电次数	见附录A.8	次	2
//剩余金额	见附录A.14	元	5
//累计购电金额	见附录A.14	元	5
//剩余电量	见附录A.11	kWh	4
//透支电量	见附录A.11	kWh	4
//累计购电量	见附录A.11	kWh	4
//赊欠门限电量	见附录A.11	kWh	4
//报警电量	见附录A.11	kWh	4
//故障电量	见附录A.11	kWh	4
#define ADDR_AFN0CF168     ADDR_AFN0CF167+((5+36)*AFN0CF167MaxPn)//电能表结算信息
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	见附录A.8	BIN	2
//已结有功总电能	见附录A.14	kWh	5
//已结费率1正向有功总电能	见附录A.14	kWh	5
//……	见附录A.14	kWh	5
//已结费率M正向有功总电能	见附录A.14	kWh	5
//未结有功总电能	见附录A.14	kWh	5
//未结费率1正向有功总电能	见附录A.14	kWh	5
//……	见附录A.14	kWh	5
//未结费率M正向有功总电能	见附录A.14	kWh	5
#define ADDR_AFN0CF169     ADDR_AFN0CF168+((6+(10*(1+TMaxTariff)))*AFN0CF168MaxPn)//集中抄表中继路由信息,测量点号
//仅用于菜单，UART口读取不在此
//前96字节读临时缓冲定义
//2BYTE Pn
//1BYTE 
//1BYTE 中继路由级数
//90BYTE(6*15) 中继1-15地址
//2BYTE 数据长度(字节)

//所属终端通信端口号	BIN	1
//中继路由个数n	BIN	1
//第1个中继路由级数m1	BIN	1
//第1个中继路由的第1级中继地址	见附录A.12	6
//……	……	……
//第1个中继路由的第m1级中继地址	见附录A.12	6
//……	……	……
//第n个中继路由级数mn	BIN	1
//第n个中继路由的第1级中继地址	见附录A.12	6
//……	……	……
//第n个中继路由的第mn级中继地址	见附录A.12	6
//#define ADDR_AFN0CF170     ADDR_AFN0CF169+((96+2+((1+(6*15))*15)*AFN0CF169MaxPn))//集中抄表电表抄读信息,测量点号
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
#define ADDR_AFN0CF170     ADDR_AFN0CF169+(96+2+(((1+(6*15))*15)*1))//集中抄表电表抄读信息,测量点号
#else
#define ADDR_AFN0CF170     ADDR_AFN0CF169+(96+2+(((1+(6*15))*15)*0))//集中抄表电表抄读信息,测量点号
#endif
//所属终端通信端口号	BIN	1
//中继路由级数	BIN	1
//载波抄读通信相位	BS8	1
//载波信号品质	BS8	1
//最近一次抄表成功/失败标志	BIN	1
//最近一次抄表成功时间	见附录A.1	6
//最近一次抄表失败时间	见附录A.1	6
//最近连续失败累计次数	BIN	1
#define ADDR_AFN0CF177     ADDR_AFN0CF170+(18*AFN0CF170MaxPn)//当前组合有功电能示值（总、费率1~M）测量点号
//终端抄表时间 见附录A.15 分时日月年
//费率数M BIN 个
//组合有功总电能示值 见附录A.14 kWh
//组合有功费率1 电能示值 见附录A.14 kWh
//...
//组合有功费率M 电能示值 见附录A.14 kWh
#define ADDR_AFN0CF178     ADDR_AFN0CF177+((6+(5*(1+TMaxTariff)))*AFN0CF177MaxPn)//上1结算日组合有功电能示值（总、费率1~M）测量点号
//终端抄表时间 见附录A.15 分时日月年
//费率数M BIN 个
//组合有功总电能示值 见附录A.14 kWh
//组合有功费率1 电能示值 见附录A.14 kWh
//...
//组合有功费率M 电能示值 见附录A.14 kWh
#define ADDR_AFN0CF245     ADDR_AFN0CF178+((6+(5*(1+TMaxTariff)))*AFN0CF178MaxPn)//终端事件分类
//终端当前事件类型总数n	BIN	1
//终端当前第1类事件代码ERC	BIN	1
//终端当前第1类事件数量	BIN	1
//终端当前第2类事件代码ERC	BIN	1
//终端当前第2类事件数量	BIN	1
//……		
//终端当前第n类事件代码ERC	BIN	1
//终端当前第n类事件数量	BIN	1
#define ADDR_AFN0CF246     ADDR_AFN0CF245+1+(2*MaxERC)//当前掉电记录数据
//掉电总次数	BCD	3
//上1次停上电事件停电时间	见附录A.15	5
//上1次停上电事件上电时间	见附录A.15	5
//...
//上10次停上电事件停电时间	见附录A.15	5
//上10次停上电事件上电时间	见附录A.15	5
#define ADDR_AFN0CF246_Sec          ADDR_AFN0CF246+(103*F246MaxPn)//最近1次掉电记录(精确到秒)
//掉电总次数	BCD	3(在判断掉电事件中自已清为0xee)
//上1次停上电事件停电时间	YMDHMS(在判断掉电事件中自已清为0xee)
//上1次停上电事件上电时间	YMDHMS(在判断掉电事件中自已清为0xee)
#define ADDR_AFN0CF247     ADDR_AFN0CF246_Sec+(15*F246MaxPn)//当前终端电池状态
//1byte D0：置"1"电池未失效，置"0"电池失效


#define ADDR_AFN0CF253     ADDR_AFN0CF247+1//现场信号信息(福建扩展)
//基站号LAC	BCD	4	
//小区号CELL_ID	BCD	3	
//频点号ARFCN	BCD	2	MHz
//信号强度Rx level	BCD（表4）	2	dBm
//当前发射功率	BCD（表4）	2	dBm
#define ADDR_AFN0CF254     ADDR_AFN0CF253+13//本地通信模块信息(福建扩展)
//芯片厂商代码	ASCII	2
//软件版本日期	BCD（表2）	3
//软件版本号	ASCII	4
//硬件版本日期	BCD（表2）	3
//硬件版本号	ASCII	4
#define ADDR_MobilePhoneNumber     ADDR_AFN0CF254+16//通信卡的手机号码和IMSI号码(用于显示)
//20byte 手机号码(ASCII)
//20byte IMSI号码(ASCII)




#define ADDR_AFN0CF83_SOUR     ADDR_MobilePhoneNumber+40//小时冻结总加有功总电量原始
//总加有功总电量原始,初始化值0xee,在日冻结清零AFN0CF19时同时清0
#define ADDR_AFN0CF84_SOUR     ADDR_AFN0CF83_SOUR+(4*AFN0CF83MaxPn)//小时冻结总加无功总电量原始
//总加无功总电量原始,初始化值0xee,在日冻结清零AFN0CF20时同时清0
#define ADDR_AFN0CF105_SOUR     ADDR_AFN0CF84_SOUR+(4*AFN0CF84MaxPn)//小时冻结正向有功总电量原始
//正向有功总电量原始,初始化值0xee
#define ADDR_AFN0CF106_SOUR     ADDR_AFN0CF105_SOUR+(5*AFN0CF105MaxPn)//小时冻结正向无功总电量原始
//正向无功总电量原始,初始化值0xee
#define ADDR_AFN0CF107_SOUR     ADDR_AFN0CF106_SOUR+(4*AFN0CF106MaxPn)//小时冻结反向有功总电量原始
//反向有功总电量原始,初始化值0xee
#define ADDR_AFN0CF108_SOUR     ADDR_AFN0CF107_SOUR+(5*AFN0CF107MaxPn)//小时冻结反向无功总电量原始
//反向无功总电量原始,初始化值0xee


#define ADDR_AFN0C_BaseEnd     (ADDR_AFN0CF108_SOUR+(4*AFN0CF108MaxPn))


#if (USER/100)==5//上海
#define ADDR_AFN0CF26_Ext     ADDR_AFN0C_BaseEnd//显示用失压全失压数据(上电时清为0xee)
//测量点1
//0 终端抄表时间	见附录A.15	分时日月年	5
//5 总失压次数	见附录A.10	次	3
//8 A相失压次数	见附录A.10	次	3
//11 B相失压次数	见附录A.10	次	3
//14 C相失压次数	见附录A.10	次	3
//17 失压时间累计值	见附录A.10	min	3
//20 A相失压时间累计值	见附录A.10	min	3
//23 B相失压时间累计值	见附录A.10	min	3
//26 C相失压时间累计值	见附录A.10	min	3

//29 最近1次失压起始时刻	秒分时日月年	6
//35 最近1次失压结束时刻	秒分时日月年	6
//41 A相最近1-10次失压起始时刻	秒分时日月年	6*10
//101 A相最近1-10次失压结束时刻	秒分时日月年	6*10
//161 B相最近1-10次失压起始时刻	秒分时日月年	6*10
//221 B相最近1-10次失压结束时刻	秒分时日月年	6*10
//281 C相最近1-10次失压起始时刻	秒分时日月年	6*10
//341 C相最近1-10次失压结束时刻	秒分时日月年	6*10

//401 总全失压次数	见附录A.10	次	3
//404 全失压时间累计值	见附录A.10	min	3
//407 最近1次全失压起始时刻	秒分时日月年	6
//413 最近1次全失压电流值  3
//416 最近1次全失压结束时刻	秒分时日月年	6
//...
//最近10次全失压起始时刻	秒分时日月年	6
//最近10次全失压电流值  3
//最近10次全失压结束时刻	秒分时日月年	6

//557 电压不平衡总次数 3
//...
//测量点n
#define LEN_AFN0CF26_Ext     560
#define ADDR_AFN0CF243_Per     ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*MaxRS485AddCarrierPn)
//1byte 日平均信号强度(仅用于日冻结)
#define ADDR_AFN0CF191     ADDR_AFN0CF243_Per+1//终端当前线电压
//终端抄表时间	见附录A.15	分时日月年	5
//当前AB线电压	见附录A.7	V	2
//当前BC线电压	见附录A.7	V	2
//当前AC线电压	见附录A.7	V	2
#define ADDR_AFN0CF191_Per     ADDR_AFN0CF191+11//1分钟平均线电压
//1byte 上次参与平均的实时钟秒
//1byte 平均次数计数
//4byte AB线电压加
//4byte BC线电压加
//4byte AC线电压加
//终端计算时间	见附录A.15	分时日月年	5
//平均AB线电压	见附录A.7	V	2
//平均BC线电压	见附录A.7	V	2
//平均AC线电压	见附录A.7	V	2
#define ADDR_AFN0CF241     ADDR_AFN0CF191_Per+25//当前登录主站信息
//登录时间	见附录A.1	6

#define ADDR_AFN0C_ExtEnd     ADDR_AFN0CF241+6
#else

	#if (USER/100)==11//吉林
#define ADDR_AFN0CF201     ADDR_AFN0C_BaseEnd//F201：终端网络信号品质
//2byte
#define ADDR_AFN0DF27_SOUR_ReadMeter     ADDR_AFN0CF201+2//从电表读取的日电压统计数据
//0 A相电压越上上限日累计时间	BIN	min	2
//2 A相电压越下下限日累计时间	BIN	min	2
//4 A相电压越上限日累计时间	BIN	min	2
//6 A相电压越下限日累计时间	BIN	min	2
//8 A相电压合格日累计时间	BIN	min	2
//10 B相电压越上上限日累计时间	BIN	min	2
//12 B相电压越下下限日累计时间	BIN	min	2
//14 B相电压越上限日累计时间	BIN	min	2
//16 B相电压越下限日累计时间	BIN	min	2
//18 B相电压合格日累计时间	BIN	min	2
//20 C相电压越上上限日累计时间	BIN	min	2
//22 C相电压越下下限日累计时间	BIN	min	2
//24 C相电压越上限日累计时间	BIN	min	2
//26 C相电压越下限日累计时间	BIN	min	2
//28 C相电压合格日累计时间	BIN	min	2
//30 A相电压最大值	见附录A.7	V	2
//32 A相电压最大值发生时间	见附录A.18	分时日	3
//35 A相电压最小值	见附录A.7	V	2
//37 A相电压最小值发生时间	见附录A.18	分时日	3
//40 B相电压最大值	见附录A.7	V	2
//42 B相电压最大值发生时间	见附录A.18	分时日	3
//45 B相电压最小值	见附录A.7	V	2
//47 B相电压最小值发生时间	见附录A.18	分时日	3
//50 C相电压最大值	见附录A.7	V	2
//52 C相电压最大值发生时间	见附录A.18	分时日	3
//55 C相电压最小值	见附录A.7	V	2
//57 C相电压最小值发生时间	见附录A.18	分时日	3
//60 A相平均电压	见附录A.7	V	2
//62 B相平均电压	见附录A.7	V	2
//64 C相平均电压	见附录A.7	V	2

//66 A相电压越上限率 见附录A.25	%	3
//69 A相电压越下限率 见附录A.25	%	3
//72 A相电压合格率 见附录A.25	%	3
//75 B相电压越上限率 见附录A.25	%	3
//78 B相电压越下限率 见附录A.25	%	3
//81 B相电压合格率 见附录A.25	%	3
//84 C相电压越上限率 见附录A.25	%	3
//87 C相电压越下限率 见附录A.25	%	3
//90 C相电压合格率 见附录A.25	%	3
#define ADDR_AFN0C_ExtEnd     ADDR_AFN0DF27_SOUR_ReadMeter+((66+27)*AFN0DF27MaxPn_ReadMeter)
	#else
#define ADDR_AFN0C_ExtEnd     ADDR_AFN0C_BaseEnd
	#endif//#if (USER/100)==11//吉林

#endif






#define ADDR_AFN0C_End     (ADDR_AFN0C_ExtEnd)

#endif














