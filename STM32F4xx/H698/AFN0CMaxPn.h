

//1类数据(实时)各Fn最大Pn
#ifndef AFN0CMaxPn_H
#define AFN0CMaxPn_H




#define AFN0CF5MaxPn     MaxTotalAddGroup//终端控制设置状态
#define AFN0CF6MaxPn     MaxTotalAddGroup//终端当前控制状态

#define AFN0CF17MaxPn     MaxTotalAddGroup//当前总加有功功率;总加组号
#define AFN0CF18MaxPn     MaxTotalAddGroup//当前总加无功功率;总加组号
#define AFN0CF19MaxPn     MaxTotalAddGroup//当日总加有功电能量（总、费率1～M）;总加组号
#define AFN0CF20MaxPn     MaxTotalAddGroup//当日总加无功电能量（总、费率1～M）;总加组号
#define AFN0CF21MaxPn     MaxTotalAddGroup//当月总加有功电能量（总、费率1～M）;总加组号
#define AFN0CF22MaxPn     MaxTotalAddGroup//当月总加无功电能量（总、费率1～M）;总加组号
#define AFN0CF23MaxPn     MaxTotalAddGroup//终端当前剩余电量（费）;总加组号
#define AFN0CF24MaxPn     MaxTotalAddGroup//当前功率下浮控控后总加有功功率冻结值;总加组号

#define AFN0CF25MaxPn     MaxRS485AddCarrierPn//当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率(测量点号)
#define AFN0CF26MaxPn     MaxRS485AddCarrierPn//A、B、C三相断相统计数据及最近一次断相记录
#define AFN0CF27MaxPn     MaxRS485AddCarrierPn//电能表日历时钟、编程次数及其最近一次操作时间
#define AFN0CF28MaxPn     MaxRS485AddCarrierPn//电表运行状态字及其变位标志
#define AFN0CF29MaxPn     MaxRS485AddCarrierPn//当前铜损、铁损有功总电能示值
#define AFN0CF30MaxPn     MaxRS485AddCarrierPn//上一结算日铜损、铁损有功总电能示值
#define AFN0CF31MaxPn     MaxRS485AddCarrierPn//当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
#define AFN0CF32MaxPn     MaxRS485AddCarrierPn//上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
#define AFN0CF33MaxPn     0//MaxRS485AddCarrierPn//当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0CF34MaxPn     0//MaxRS485AddCarrierPn//当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0CF35MaxPn     0//MaxRS485AddCarrierPn//当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0CF36MaxPn     0//MaxRS485AddCarrierPn//当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0CF37MaxPn     0//MaxRS485AddCarrierPn//上月（上一结算日）正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M）
#define AFN0CF38MaxPn     0//MaxRS485AddCarrierPn//上月（上一结算日）反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M）
#define AFN0CF39MaxPn     0//MaxRS485AddCarrierPn//上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M）
#define AFN0CF40MaxPn     0//MaxRS485AddCarrierPn//上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M）

#define AFN0CF41MaxPn     MaxRS485Pn//当日正向有功电能量(总、费率l～M)(测量点号)
#define AFN0CF42MaxPn     MaxRS485Pn//当日正向无功电能量(总、费率1～M)(测量点号)
#define AFN0CF43MaxPn     MaxRS485Pn//当日反向有功电能量(总、费率l～M)(测量点号)
#define AFN0CF44MaxPn     MaxRS485Pn//当日反向无功电能量(总、费率l～M)(测量点号)
#define AFN0CF45MaxPn     MaxRS485Pn//当月正向有功电能量(总、费率l～M)(测量点号)
#define AFN0CF46MaxPn     MaxRS485Pn//当月正向无功电能量(总、费率1～M)(测量点号)
#define AFN0CF47MaxPn     MaxRS485Pn//当月反向有功电能量(总、费率1～M)(测量点号)
#define AFN0CF48MaxPn     MaxRS485Pn//当月反向无功电能量(总、费率l～M)(测量点号)

#define AFN0CF49MaxPn     MaxRS485AddCarrierPn//当前电压、电流相位角
	#if MeterHarmonic==0//谐波计算0=没,1=有
#define AFN0CF57MaxPn     0//MaxRS485Pn//当前A、B、C三相电压、电流2～N次谐波有效值
#define AFN0CF58MaxPn     0//MaxRS485Pn//当前A、B、C三相电压、电流2～N次谐波含有率
	#else
#define AFN0CF57MaxPn     MaxRS485Pn//当前A、B、C三相电压、电流2～N次谐波有效值
#define AFN0CF58MaxPn     MaxRS485Pn//当前A、B、C三相电压、电流2～N次谐波含有率
	#endif
#define AFN0CF65MaxPn     MaxCPn//当前电容器投切状态
#define AFN0CF66MaxPn     MaxCPn//当前电容器累计补偿投入时间和次数(测量点号)
#define AFN0CF67MaxPn     MaxCPn//当日、当月电容器累计补偿的无功电能量(测量点号)

#define AFN0CF73MaxPn     MaxDCPn//直流模拟量当前数据

#define AFN0CF81MaxPn     MaxTotalAddGroup//小时冻结总加有功功率
#define AFN0CF82MaxPn     MaxTotalAddGroup//小时冻结总加无功功率
#define AFN0CF83MaxPn     MaxTotalAddGroup//小时冻结总加有功总电能量
#define AFN0CF84MaxPn     MaxTotalAddGroup//小时冻结总加无功总电能量

#define AFN0CF89MaxPn     MaxRS485Pn//小时冻结有功功率
#define AFN0CF90MaxPn     MaxRS485Pn//小时冻结A相有功功率
#define AFN0CF91MaxPn     MaxRS485Pn//小时冻结B相有功功率
#define AFN0CF92MaxPn     MaxRS485Pn//小时冻结C相有功功率
#define AFN0CF93MaxPn     MaxRS485Pn//小时冻结无功功率
#define AFN0CF94MaxPn     MaxRS485Pn//小时冻结A无功功率
#define AFN0CF95MaxPn     MaxRS485Pn//小时冻结B无功功率
#define AFN0CF96MaxPn     MaxRS485Pn//小时冻结C无功功率
#define AFN0CF97MaxPn     MaxRS485Pn//小时冻结A相电压
#define AFN0CF98MaxPn     MaxRS485Pn//小时冻结B相电压
#define AFN0CF99MaxPn     MaxRS485Pn//小时冻结C相电压
#define AFN0CF100MaxPn     MaxRS485Pn//小时冻结A相电流
#define AFN0CF101MaxPn     MaxRS485Pn//小时冻结B相电流
#define AFN0CF102MaxPn     MaxRS485Pn//小时冻结C相电流
#define AFN0CF103MaxPn     MaxRS485Pn//小时冻结零序电流
#define AFN0CF105MaxPn     MaxRS485Pn//小时冻结正向有功总电能量
#define AFN0CF106MaxPn     MaxRS485Pn//小时冻结正向无功总电能量
#define AFN0CF107MaxPn     MaxRS485Pn//小时冻结反向有功总电能量
#define AFN0CF108MaxPn     MaxRS485Pn//小时冻结反向无功总电能量
#define AFN0CF109MaxPn     MaxRS485Pn//小时冻结正向有功总电能示值
#define AFN0CF110MaxPn     MaxRS485Pn//小时冻结正向无功总电能示值
#define AFN0CF111MaxPn     MaxRS485Pn//小时冻结反向有功总电能示值
#define AFN0CF112MaxPn     MaxRS485Pn//小时冻结反向无功总电能示值
#define AFN0CF113MaxPn     MaxRS485Pn//小时冻结总功率因数
#define AFN0CF114MaxPn     MaxRS485Pn//小时冻结A功率因数
#define AFN0CF115MaxPn     MaxRS485Pn//小时冻结B功率因数
#define AFN0CF116MaxPn     MaxRS485Pn//小时冻结C功率因数
#define AFN0CF121MaxPn     MaxDCPn//小时冻结直流模拟量


#define AFN0CF129MaxPn     MaxRS485AddCarrierPn//当前正向有功电能示值（总、费率1～M）
#define AFN0CF130MaxPn     MaxRS485AddCarrierPn//当前正向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0CF131MaxPn     MaxRS485AddCarrierPn//当前反向有功电能示值（总、费率1～M）
#define AFN0CF132MaxPn     MaxRS485AddCarrierPn//当前反向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0CF133MaxPn     MaxRS485AddCarrierPn//当前1象限无功电能示值（总、费率1～M）
#define AFN0CF134MaxPn     MaxRS485AddCarrierPn//当前2象限无功电能示值（总、费率1～M）
#define AFN0CF135MaxPn     MaxRS485AddCarrierPn//当前3象限无功电能示值（总、费率1～M）
#define AFN0CF136MaxPn     MaxRS485AddCarrierPn//当前4象限无功电能示值（总、费率1～M）
#define AFN0CF137MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）正向有功电能示值（总、费率1～M）
#define AFN0CF138MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0CF139MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）反向有功电能示值（总、费率1～M）
#define AFN0CF140MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0CF141MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）1象限无功电能示值（总、费率1～M）
#define AFN0CF142MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）2象限无功电能示值（总、费率1～M）
#define AFN0CF143MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）3象限无功电能示值（总、费率1～M）
#define AFN0CF144MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）4象限无功电能示值（总、费率1～M）
#define AFN0CF145MaxPn     MaxRS485AddCarrierPn//当月正向有功最大需量及发生时间（总、费率1～M）
#define AFN0CF146MaxPn     MaxRS485AddCarrierPn//当月正向无功最大需量及发生时间（总、费率1～M）
#define AFN0CF147MaxPn     MaxRS485AddCarrierPn//当月反向有功最大需量及发生时间（总、费率1～M）
#define AFN0CF148MaxPn     MaxRS485AddCarrierPn//当月反向无功最大需量及发生时间（总、费率1～M）
#define AFN0CF149MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
#define AFN0CF150MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
#define AFN0CF151MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
#define AFN0CF152MaxPn     MaxRS485AddCarrierPn//上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
#define AFN0CF153MaxPn     MaxRS485AddCarrierPn//第1时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF154MaxPn     MaxRS485AddCarrierPn//第2时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF155MaxPn     MaxRS485AddCarrierPn//第3时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF156MaxPn     MaxRS485AddCarrierPn//第4时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF157MaxPn     MaxRS485AddCarrierPn//第5时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF158MaxPn     MaxRS485AddCarrierPn//第6时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF159MaxPn     MaxRS485AddCarrierPn//第7时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF160MaxPn     MaxRS485AddCarrierPn//第8时区冻结正向有功电能示值（总、费率1～M）
#define AFN0CF161MaxPn     MaxRS485AddCarrierPn//电能表远程控制通断电状态及记录
#define AFN0CF162MaxPn     MaxRS485AddCarrierPn//电能表日历时钟
#define AFN0CF165MaxPn     MaxRS485AddCarrierPn//电能表开关操作次数及时间
#define AFN0CF166MaxPn     MaxRS485AddCarrierPn//电能表参数修改次数及时间
#define AFN0CF167MaxPn     MaxRS485AddCarrierPn//电能表购、用电信息
#define AFN0CF168MaxPn     MaxRS485AddCarrierPn//电能表结算信息
#if (Project/100)!=2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
#define AFN0CF169MaxPn     0//集中抄表中继路由信息	测量点号
#else
#define AFN0CF169MaxPn     MaxRS485AddCarrierPn//集中抄表中继路由信息	测量点号(临时从路由器中读取)
#endif
#define AFN0CF170MaxPn     MaxRS485AddCarrierPn//集中抄表电表抄读信息	测量点号

#define AFN0CF177MaxPn     MaxRS485AddCarrierPn//当前组合有功电能示值（总、费率1~M）测量点号
#define AFN0CF178MaxPn     MaxRS485AddCarrierPn//上1结算日组合有功电能示值（总、费率1~M）测量点号
#define AFN0CF180MaxPn     MaxRS485AddCarrierPn//从节点版本信息;测量点号


#endif


