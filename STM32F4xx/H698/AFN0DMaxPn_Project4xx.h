
//400=公变终端
//2类数据(日冻结,月冻结,曲线)各Fn最大Pn
#ifndef AFN0DMaxPn_Project4xx_H
#define AFN0DMaxPn_Project4xx_H



//2类数据.日冻结
#define AFN0DF1MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF2MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF3MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0DF4MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	#if (RMM_DateCongeal_0xEE==0)
	//#if ((RMM_DateCongeal_0xEE==0)||(AFN0CF41F42F43F44==0))
	//RMM_DateCongeal_0xEE=0:2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	//AFN0CF41F42F43F44==0:当日电能量AFN0CF41F42F43F44计算;0=示值实时计算,1=当前示值-当日0点冻结
#define AFN0DF5MaxPn     MaxRS485Pn//测量点正向有功电能量（总、费率1～M）
#define AFN0DF6MaxPn     MaxRS485Pn//测量点正向无功电能量（总、费率1～M）
#define AFN0DF7MaxPn     MaxRS485Pn//测量点反向有功电能量（总、费率1～M）
#define AFN0DF8MaxPn     MaxRS485Pn//测量点反向无功电能量（总、费率1～M）
	#else
		#if AFN0DF1MaxPn!=0
			#error AFN0DF1MaxPn!=0,AFN0DF5MaxPn==0
		#endif
#define AFN0DF5MaxPn     0//MaxRS485Pn//测量点正向有功电能量（总、费率1～M）
#define AFN0DF6MaxPn     0//MaxRS485Pn//测量点正向无功电能量（总、费率1～M）
#define AFN0DF7MaxPn     0//MaxRS485Pn//测量点反向有功电能量（总、费率1～M）
#define AFN0DF8MaxPn     0//MaxRS485Pn//测量点反向无功电能量（总、费率1～M）
	#endif
#define AFN0DF25MaxPn    MaxRS485Pn//测量点日总及分相最大有功功率及发生时间、有功功率为零时间
#define AFN0DF26MaxPn    MaxRS485Pn//测量点日总及分相最大需量及发生时间
#define AFN0DF27MaxPn    MaxRS485Pn//测量点日电压统计数据
#define AFN0DF28MaxPn    MaxRS485Pn//测量点日不平衡度越限累计时间
#define AFN0DF29MaxPn    MaxRS485Pn//测量点日电流越限统计
#define AFN0DF30MaxPn    MaxRS485Pn//测量点日视在功率越限累计时间
#define AFN0DF31MaxPn    MaxRS485Pn//测量点日负载率率统计
#define AFN0DF32MaxPn     MaxRS485Pn//测量点日电能表断相数据
#define AFN0DF41MaxPn     MaxCPn//测量点电容器投入累计时间和次数
#define AFN0DF42MaxPn     MaxCPn//测量点日、月电容器累计补偿的无功电能量
#define AFN0DF43MaxPn     MaxRS485Pn//测量点日功率因数区段累计时间
#define AFN0DF45MaxPn     MaxRS485Pn//测量点铜损、铁损有功电能示值

#define AFN0DF57MaxPn     MaxTotalAddGroup//总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
#define AFN0DF58MaxPn     MaxTotalAddGroup//总加组日累计有功电能量（总、费率1～M）
#define AFN0DF59MaxPn     MaxTotalAddGroup//总加组日累计无功电能量（总、费率1～M）
#define AFN0DF60MaxPn     MaxTotalAddGroup//总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
#define AFN0DF61MaxPn     MaxTotalAddGroup//总加组月累计有功电能量（总、费率1～M）
#define AFN0DF62MaxPn     MaxTotalAddGroup//总加组月累计无功电能量（总、费率1～M）
#define AFN0DF65MaxPn     MaxTotalAddGroup//总加组超功率定值的月累计时间、月累计电能量
#define AFN0DF66MaxPn     MaxTotalAddGroup//总加组超月电能量定值的月累计时间、累计电能量

	#if MeterHarmonic==0//谐波计算0=没,1=有
#define AFN0DF113MaxPn     0//MaxRS485Pn//测量点A相2～19次谐波电流日最大值及发生时间
#define AFN0DF114MaxPn     0//MaxRS485Pn//测量点B相2～19次谐波电流日最大值及发生时间
#define AFN0DF115MaxPn     0//MaxRS485Pn//测量点C相2～19次谐波电流日最大值及发生时间
#define AFN0DF116MaxPn     0//MaxRS485Pn//测量点A相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF117MaxPn     0//MaxRS485Pn//测量点B相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF118MaxPn     0//MaxRS485Pn//测量点C相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF121MaxPn     0//MaxRS485Pn//测量点A相谐波越限日统计数据
#define AFN0DF122MaxPn     0//MaxRS485Pn//测量点B相谐波越限日统计数据
#define AFN0DF123MaxPn     0//MaxRS485Pn//测量点C相谐波越限日统计数据
#define AFN0DF124MaxPn     0//MaxRS485Pn//测量点A相日冻结电压谐波电压含有率、谐波电流95%概率值
#define AFN0DF125MaxPn     0//MaxRS485Pn//测量点B相日冻结电压谐波电压含有率、谐波电流95%概率值
#define AFN0DF126MaxPn     0//MaxRS485Pn//测量点C相日冻结电压谐波电压含有率、谐波电流95%概率值
	#else
#define AFN0DF113MaxPn     MaxRS485Pn//测量点A相2～19次谐波电流日最大值及发生时间
#define AFN0DF114MaxPn     MaxRS485Pn//测量点B相2～19次谐波电流日最大值及发生时间
#define AFN0DF115MaxPn     MaxRS485Pn//测量点C相2～19次谐波电流日最大值及发生时间
#define AFN0DF116MaxPn     MaxRS485Pn//测量点A相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF117MaxPn     MaxRS485Pn//测量点B相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF118MaxPn     MaxRS485Pn//测量点C相2～19次谐波电压含有率及总畸变率日最大值及发生时间
#define AFN0DF121MaxPn     MaxRS485Pn//测量点A相谐波越限日统计数据
#define AFN0DF122MaxPn     MaxRS485Pn//测量点B相谐波越限日统计数据
#define AFN0DF123MaxPn     MaxRS485Pn//测量点C相谐波越限日统计数据
#define AFN0DF124MaxPn     MaxRS485Pn//测量点A相日冻结电压谐波电压含有率、谐波电流95%概率值
#define AFN0DF125MaxPn     MaxRS485Pn//测量点B相日冻结电压谐波电压含有率、谐波电流95%概率值
#define AFN0DF126MaxPn     MaxRS485Pn//测量点C相日冻结电压谐波电压含有率、谐波电流95%概率值
	#endif
#define AFN0DF129MaxPn     MaxDCPn//测量点直流模拟量越限日累计时间、最大/最小值及发生时间
#define AFN0DF153MaxPn     MaxRS485Pn//测量点分相正向有功电能示值
#define AFN0DF154MaxPn     MaxRS485Pn//测量点分相正向无功电能示值
#define AFN0DF155MaxPn     MaxRS485Pn//测量点分相反向有功电能示值
#define AFN0DF156MaxPn     MaxRS485Pn//测量点分相反向无功电能示值
#define AFN0DF161MaxPn     MaxRS485AddCarrierPn//测量点正向有功电能示值（总、费率1～M）
#define AFN0DF162MaxPn     MaxRS485AddCarrierPn//测量点正向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF163MaxPn     MaxRS485AddCarrierPn//测量点反向有功电能示值（总、费率1～M）
#define AFN0DF164MaxPn     MaxRS485AddCarrierPn//测量点反向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF165MaxPn     MaxRS485AddCarrierPn//测量点1象限无功电能示值（总、费率1～M）
#define AFN0DF166MaxPn     MaxRS485AddCarrierPn//测量点2象限无功电能示值（总、费率1～M）
#define AFN0DF167MaxPn     MaxRS485AddCarrierPn//测量点3象限无功电能示值（总、费率1～M）
#define AFN0DF168MaxPn     MaxRS485AddCarrierPn//测量点4象限无功电能示值（总、费率1～M）
#define AFN0DF185MaxPn     MaxRS485AddCarrierPn//测量点正向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF186MaxPn     MaxRS485Pn//测量点正向无功最大需量及发生时间（总、费率1～M）
#define AFN0DF187MaxPn     MaxRS485AddCarrierPn//测量点反向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF188MaxPn     MaxRS485Pn//测量点反向无功最大需量及发生时间（总、费率1～M）
#define AFN0DF209MaxPn     MaxRS485AddCarrierPn//测量点电能表远程控制通断电状态及记录
#define AFN0DF210MaxPn     MaxRS485AddCarrierPn//测量点电能表购、用电信息
#define AFN0DF211MaxPn     MaxRS485Pn//测量点三相电压、电流不平衡度95%概率值

//2类数据.抄表日冻结
#define AFN0DF9MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF10MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF11MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0DF12MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0DF169MaxPn     MaxRS485AddCarrierPn//测量点正向有功电能示值（总、费率1～M）
#define AFN0DF170MaxPn     MaxRS485AddCarrierPn//测量点正向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF171MaxPn     MaxRS485AddCarrierPn//测量点反向有功电能示值（总、费率1～M）
#define AFN0DF172MaxPn     MaxRS485AddCarrierPn//测量点反向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF173MaxPn     MaxRS485AddCarrierPn//测量点1象限无功电能示值（总、费率1～M）
#define AFN0DF174MaxPn     MaxRS485AddCarrierPn//测量点2象限无功电能示值（总、费率1～M）
#define AFN0DF175MaxPn     MaxRS485AddCarrierPn//测量点3象限无功电能示值（总、费率1～M）
#define AFN0DF176MaxPn     MaxRS485AddCarrierPn//测量点4象限无功电能示值（总、费率1～M）
#define AFN0DF189MaxPn     MaxRS485AddCarrierPn//测量点正向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF190MaxPn     MaxRS485AddCarrierPn//测量点正向无功最大需量及发生时间（总、费率1～M）
#define AFN0DF191MaxPn     MaxRS485AddCarrierPn//测量点反向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF192MaxPn     MaxRS485AddCarrierPn//测量点反向无功最大需量及发生时间（总、费率1～M）

//2类数据.月冻结
#define AFN0DF17MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF18MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
#define AFN0DF19MaxPn     0//MaxRS485AddCarrierPn//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
#define AFN0DF20MaxPn     0//MaxRS485AddCarrierPn//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
	#if (RMM_DateCongeal_0xEE==0)
	//#if ((RMM_DateCongeal_0xEE==0)||(AFN0CF45F46F47F48==0))
	//RMM_DateCongeal_0xEE=0:2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	//AFN0CF45F46F47F48=0:当月电能量AFN0CF45F46F47F48计算;0=示值实时计算,1=当前示值-当月0点冻结
#define AFN0DF21MaxPn     MaxRS485Pn//测量点正向有功电能量（总、费率1～M）
#define AFN0DF22MaxPn     MaxRS485Pn//测量点正向无功电能量（总、费率1～M）
#define AFN0DF23MaxPn     MaxRS485Pn//测量点反向有功电能量（总、费率1～M）
#define AFN0DF24MaxPn     MaxRS485Pn//测量点反向无功电能量（总、费率1～M）
	#else
		#if AFN0DF17MaxPn!=0
			#error AFN0DF17MaxPn!=0,AFN0DF21MaxPn==0
		#endif
#define AFN0DF21MaxPn     0//MaxRS485Pn//测量点正向有功电能量（总、费率1～M）
#define AFN0DF22MaxPn     0//MaxRS485Pn//测量点正向无功电能量（总、费率1～M）
#define AFN0DF23MaxPn     0//MaxRS485Pn//测量点反向有功电能量（总、费率1～M）
#define AFN0DF24MaxPn     0//MaxRS485Pn//测量点反向无功电能量（总、费率1～M）
	#endif
#define AFN0DF33MaxPn     MaxRS485Pn//测量点月总及分相最大有功功率及发生时间、有功功率为零时间
#define AFN0DF34MaxPn     MaxRS485Pn//测量点月总及分相有功最大需量及发生时间
#define AFN0DF35MaxPn     MaxRS485Pn//测量点月电压统计数据
#define AFN0DF36MaxPn     MaxRS485Pn//测量点月不平衡度越限累计时间
#define AFN0DF37MaxPn     MaxRS485Pn//测量点月电流越限统计
#define AFN0DF38MaxPn     MaxRS485Pn//测量点月视在功率越限累计时间
#define AFN0DF39MaxPn     MaxRS485Pn//测量点月负载率率统计
#define AFN0DF44MaxPn     MaxRS485Pn//测量点月功率因数区段累计时间
#define AFN0DF46MaxPn     MaxRS485Pn//测量点月冻结铜损、铁损有功电能示值

#define AFN0DF130MaxPn     MaxDCPn//直流模拟量越限月累计时间、最大/最小值及发生时间
#define AFN0DF157MaxPn     MaxRS485Pn//测量点月冻结分相正向有功电能示值
#define AFN0DF158MaxPn     MaxRS485Pn//测量点月冻结分相正向无功电能示值
#define AFN0DF159MaxPn     MaxRS485Pn//测量点月冻结分相反向有功电能示值
#define AFN0DF160MaxPn     MaxRS485Pn//测量点月冻结分相反向无功电能示值
#define AFN0DF177MaxPn     MaxRS485AddCarrierPn//测量点正向有功电能示值（总、费率1～M）
#define AFN0DF178MaxPn     MaxRS485AddCarrierPn//测量点正向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF179MaxPn     MaxRS485AddCarrierPn//测量点反向有功电能示值（总、费率1～M）
#define AFN0DF180MaxPn     MaxRS485AddCarrierPn//测量点反向无功（组合无功1）电能示值（总、费率1～M）
#define AFN0DF181MaxPn     MaxRS485AddCarrierPn//测量点1象限无功电能示值（总、费率1～M）
#define AFN0DF182MaxPn     MaxRS485AddCarrierPn//测量点2象限无功电能示值（总、费率1～M）
#define AFN0DF183MaxPn     MaxRS485AddCarrierPn//测量点3象限无功电能示值（总、费率1～M）
#define AFN0DF184MaxPn     MaxRS485AddCarrierPn//测量点4象限无功电能示值（总、费率1～M）
#define AFN0DF193MaxPn     MaxRS485AddCarrierPn//测量点正向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF194MaxPn     MaxRS485AddCarrierPn//测量点正向无功最大需量及发生时间（总、费率1～M）
#define AFN0DF195MaxPn     MaxRS485AddCarrierPn//测量点反向有功最大需量及发生时间（总、费率1～M）
#define AFN0DF196MaxPn     MaxRS485AddCarrierPn//测量点反向无功最大需量及发生时间（总、费率1～M）
#define AFN0DF201MaxPn     MaxRS485AddCarrierPn//测量点第1时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF202MaxPn     MaxRS485AddCarrierPn//测量点第2时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF203MaxPn     MaxRS485AddCarrierPn//测量点第3时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF204MaxPn     MaxRS485AddCarrierPn//测量点第4时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF205MaxPn     MaxRS485AddCarrierPn//测量点第5时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF206MaxPn     MaxRS485AddCarrierPn//测量点第6时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF207MaxPn     MaxRS485AddCarrierPn//测量点第7时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF208MaxPn     MaxRS485AddCarrierPn//测量点第8时区冻结正向有功电能示值（总、费率1～M）
#define AFN0DF212MaxPn     MaxRS485Pn//测量点三相电压、电流不平衡度95%概率值
#define AFN0DF213MaxPn     MaxRS485AddCarrierPn//测量点电能表开关操作次数及时间
#define AFN0DF214MaxPn     MaxRS485AddCarrierPn//测量点电能表参数修改次数及时间
#define AFN0DF215MaxPn     MaxRS485AddCarrierPn//测量点电能表购、用电信息
#define AFN0DF216MaxPn     MaxRS485AddCarrierPn//测量点电能表结算信息


//2类数据.曲线
#define AFN0DF73MaxPn     MaxTotalAddGroup//总加组有功功率曲线
#define AFN0DF74MaxPn     MaxTotalAddGroup//总加组无功功率曲线
#define AFN0DF75MaxPn     MaxTotalAddGroup//总加组有功电能量曲线
#define AFN0DF76MaxPn     MaxTotalAddGroup//总加组无功电能量曲线
#define AFN0DF81MaxPn     MaxKeyUser//测量点有功功率曲线
#define AFN0DF82MaxPn     MaxKeyUser//测量点A相有功功率曲线
#define AFN0DF83MaxPn     MaxKeyUser//测量点B相有功功率曲线
#define AFN0DF84MaxPn     MaxKeyUser//测量点C相有功功率曲线
#define AFN0DF85MaxPn     MaxKeyUser//测量点无功功率曲线
#define AFN0DF86MaxPn     MaxKeyUser//测量点A无功功率曲线
#define AFN0DF87MaxPn     MaxKeyUser//测量点B无功功率曲线
#define AFN0DF88MaxPn     MaxKeyUser//测量点C无功功率曲线
#define AFN0DF89MaxPn     MaxKeyUser//测量点A相电压曲线
#define AFN0DF90MaxPn     MaxKeyUser//测量点B相电压曲线
#define AFN0DF91MaxPn     MaxKeyUser//测量点C相电压曲线
#define AFN0DF92MaxPn     MaxKeyUser//测量点A相电流曲线
#define AFN0DF93MaxPn     MaxKeyUser//测量点B相电流曲线
#define AFN0DF94MaxPn     MaxKeyUser//测量点C相电流曲线
#define AFN0DF95MaxPn     MaxKeyUser//测量点零序电流曲线
	#if MaxImpInPort!=0//最大脉冲输入端口数(<=8)
#define AFN0DF97MaxPn     MaxRS485Pn//测量点正向有功总电能量
#define AFN0DF98MaxPn     MaxRS485Pn//测量点正向无功总电能量
#define AFN0DF99MaxPn     MaxRS485Pn//测量点反向有功总电能量
#define AFN0DF100MaxPn     MaxRS485Pn//测量点反向无功总电能量
	#else
#define AFN0DF97MaxPn     0//MaxRS485Pn//测量点正向有功总电能量
#define AFN0DF98MaxPn     0//MaxRS485Pn//测量点正向无功总电能量
#define AFN0DF99MaxPn     0//MaxRS485Pn//测量点反向有功总电能量
#define AFN0DF100MaxPn     0//MaxRS485Pn//测量点反向无功总电能量
	#endif
#define AFN0DF101MaxPn     MaxKeyUser//测量点正向有功总电能示值
#define AFN0DF102MaxPn     MaxKeyUser//测量点正向无功总电能示值
#define AFN0DF103MaxPn     MaxKeyUser//测量点反向有功总电能示值
#define AFN0DF104MaxPn     MaxKeyUser//测量点反向无功总电能示值
#define AFN0DF105MaxPn     MaxKeyUser//测量点总功率因数
#define AFN0DF106MaxPn     MaxKeyUser//测量点A相功率因数
#define AFN0DF107MaxPn     MaxKeyUser//测量点B相功率因数
#define AFN0DF108MaxPn     MaxKeyUser//测量点C相功率因数
#define AFN0DF109MaxPn     MaxKeyUser//测量点电压相位角曲线
#define AFN0DF110MaxPn     MaxKeyUser//测量点电流相位角曲线
#define AFN0DF138MaxPn     MaxDCPn//直流模拟量数据曲线
#define AFN0DF145MaxPn     MaxKeyUser//测量点1象限无功总电能示值曲线
#define AFN0DF146MaxPn     MaxKeyUser//测量点2象限无功总电能示值曲线
#define AFN0DF147MaxPn     MaxKeyUser//测量点3象限无功总电能示值曲线
#define AFN0DF148MaxPn     MaxKeyUser//测量点4象限无功总电能示值曲线
#define AFN0DF219MaxPn     MaxKeyUser//测量点组合数据曲线






#endif
