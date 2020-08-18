

//终端2类数据
#ifndef AFN0D_H
#define AFN0D_H


//类2数据日冻结
#define ADDR_DATECONGEAL_START     ADDR_AFN0D_Start//日冻结开始地址
//3BYTE 冻结时标(当前时钟上一日的日月年)
#define ADDR_DATECONGEALFLAG     3//日冻结标志
//1byte 冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#define  ADDR_AFN0DF1     ADDR_DATECONGEALFLAG+1//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//日冻结
#define  ADDR_AFN0DF2      ADDR_AFN0DF1+((6+(17*(1+TMaxTariff)))*AFN0DF1MaxPn)//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//日冻结
#define  ADDR_AFN0DF3      ADDR_AFN0DF2+((6+(17*(1+TMaxTariff)))*AFN0DF2MaxPn)//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//日冻结
#define  ADDR_AFN0DF4      ADDR_AFN0DF3+((6+(14*(1+TMaxTariff)))*AFN0DF3MaxPn)//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//日冻结
#define  ADDR_AFN0DF5      ADDR_AFN0DF4+((6+(14*(1+TMaxTariff)))*AFN0DF4MaxPn)//测量点正向有功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF6      ADDR_AFN0DF5+((1+(4*(1+TMaxTariff)))*AFN0DF5MaxPn)//测量点正向无功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF7      ADDR_AFN0DF6+((1+(4*(1+TMaxTariff)))*AFN0DF6MaxPn)//测量点反向有功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF8      ADDR_AFN0DF7+((1+(4*(1+TMaxTariff)))*AFN0DF7MaxPn)//测量点反向无功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF25      ADDR_AFN0DF8+((1+(4*(1+TMaxTariff)))*AFN0DF8MaxPn)//测量点日总及分相最大有功功率及发生时间、有功功率为零时间
//日冻结
#define  ADDR_AFN0DF26      ADDR_AFN0DF25+(32*AFN0DF25MaxPn)//测量点日总及分相最大需量及发生时间
//日冻结
#define  ADDR_AFN0DF27      ADDR_AFN0DF26+(24*AFN0DF26MaxPn)//测量点日电压统计数据
//日冻结
	#if (USER/100)==11//吉林
#define  ADDR_AFN0DF28      ADDR_AFN0DF27+((66+27)*AFN0DF27MaxPn_ReadMeter)//测量点日不平衡度越限累计时间
	#else
#define  ADDR_AFN0DF28      ADDR_AFN0DF27+((66+27)*AFN0DF27MaxPn)//测量点日不平衡度越限累计时间
	#endif
//日冻结
#define  ADDR_AFN0DF29      ADDR_AFN0DF28+(14*AFN0DF28MaxPn)//测量点日电流越限统计
//日冻结
#define  ADDR_AFN0DF30      ADDR_AFN0DF29+(38*AFN0DF29MaxPn)//测量点日视在功率越限累计时间
//日冻结
#define  ADDR_AFN0DF31      ADDR_AFN0DF30+(4*AFN0DF30MaxPn)//测量点日负载率率统计
//日冻结
#define  ADDR_AFN0DF32      ADDR_AFN0DF31+(10*AFN0DF31MaxPn)//测量点日电能表断相数据
//日冻结
#define  ADDR_AFN0DF41      ADDR_AFN0DF32+(61*AFN0DF32MaxPn)//测量点电容器投入累计时间和次数
//日冻结
#define  ADDR_AFN0DF42      ADDR_AFN0DF41+(72*AFN0DF41MaxPn)//测量点日、月电容器累计补偿的无功电能量
//日冻结
#define  ADDR_AFN0DF43      ADDR_AFN0DF42+(4*AFN0DF42MaxPn)//测量点日功率因数区段累计时间
//日冻结
#define  ADDR_AFN0DF45      ADDR_AFN0DF43+(6*AFN0DF43MaxPn)//测量点铜损、铁损有功电能示值
//日冻结
#define  ADDR_AFN0DF49      ADDR_AFN0DF45+(10*AFN0DF45MaxPn)//终端日供电时间、日复位累计次数
//日冻结
#define  ADDR_AFN0DF50      ADDR_AFN0DF49+4//终端日控制统计数据
//日冻结
#define  ADDR_AFN0DF53      ADDR_AFN0DF50+4//终端与主站日通信流量
//日冻结
#define  ADDR_AFN0DF57      ADDR_AFN0DF53+4//总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
//日冻结
#define  ADDR_AFN0DF58      ADDR_AFN0DF57+(12*AFN0DF57MaxPn)//总加组日累计有功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF59      ADDR_AFN0DF58+((1+(4*(1+TMaxTariff)))*AFN0DF58MaxPn)//总加组日累计无功电能量（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF113      ADDR_AFN0DF59+((1+(4*(1+TMaxTariff)))*AFN0DF59MaxPn)//测量点A相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF114      ADDR_AFN0DF113+(114*AFN0DF113MaxPn)//测量点B相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF115      ADDR_AFN0DF114+(114*AFN0DF114MaxPn)//测量点C相2～19次谐波电流日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF116      ADDR_AFN0DF115+(114*AFN0DF115MaxPn)//测量点A相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF117      ADDR_AFN0DF116+(114*AFN0DF116MaxPn)//测量点B相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF118      ADDR_AFN0DF117+(114*AFN0DF117MaxPn)//测量点C相2～19次谐波电压含有率及总畸变率日最大值及发生时间
//日冻结
#define  ADDR_AFN0DF121      ADDR_AFN0DF118+(114*AFN0DF118MaxPn)//测量点A相谐波越限日统计数据
//日冻结
#define  ADDR_AFN0DF122      ADDR_AFN0DF121+(77*AFN0DF121MaxPn)//测量点B相谐波越限日统计数据
//日冻结
#define  ADDR_AFN0DF123      ADDR_AFN0DF122+(77*AFN0DF122MaxPn)//测量点C相谐波越限日统计数据
//日冻结
#define  ADDR_AFN0DF124      ADDR_AFN0DF123+(77*AFN0DF123MaxPn)//测量点A相日冻结电压谐波电压含有率、谐波电流95%概率值
//日冻结
#define  ADDR_AFN0DF125      ADDR_AFN0DF124+(81*AFN0DF124MaxPn)//测量点B相日冻结电压谐波电压含有率、谐波电流95%概率值
//日冻结
#define  ADDR_AFN0DF126      ADDR_AFN0DF125+(81*AFN0DF125MaxPn)//测量点C相日冻结电压谐波电压含有率、谐波电流95%概率值
//日冻结
#define  ADDR_AFN0DF129      ADDR_AFN0DF126+(81*AFN0DF123MaxPn)//测量点直流模拟量越限日累计时间、最大/最小值及发生时间
//日冻结
#define  ADDR_AFN0DF153      ADDR_AFN0DF129+(14*AFN0DF129MaxPn)//测量点分相正向有功电能示值
//日冻结
#define  ADDR_AFN0DF154      ADDR_AFN0DF153+(20*AFN0DF153MaxPn)//测量点分相正向无功电能示值
//日冻结
#define  ADDR_AFN0DF155      ADDR_AFN0DF154+(17*AFN0DF154MaxPn)//测量点分相反向有功电能示值
//日冻结
#define  ADDR_AFN0DF156      ADDR_AFN0DF155+(20*AFN0DF155MaxPn)//测量点分相反向无功电能示值
//日冻结
#define  ADDR_AFN0DF161      ADDR_AFN0DF156+(17*AFN0DF156MaxPn)//测量点正向有功电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//正向有功总电能示值	见附录A.14	kWh	5
//费率1正向有功电能示值	见附录A.14	kWh	5
//……	……	……	……
//费率M正向有功电能示值	见附录A.14	kWh	5
#define  ADDR_AFN0DF162      ADDR_AFN0DF161+((6+(5*(1+TMaxTariff)))*AFN0DF161MaxPn)//测量点正向无功（组合无功1）电能示值（总、费率1～M）
//终端抄表时间	见附录A.15	分时日月年	5
//费率数M	BIN	个	1
//正向无功（组合无功1）总电能示值	见附录A.11	kvarh	4
//费率1正向无功（组合无功1）电能示值	见附录A.11	kvarh	4
//……	……	……	……
//费率M正向无功（组合无功1）电能示值	见附录A.11	kvarh	4
#define  ADDR_AFN0DF163      ADDR_AFN0DF162+((6+(4*(1+TMaxTariff)))*AFN0DF162MaxPn)//测量点反向有功电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF164      ADDR_AFN0DF163+((6+(5*(1+TMaxTariff)))*AFN0DF163MaxPn)//测量点反向无功（组合无功1）电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF165      ADDR_AFN0DF164+((6+(4*(1+TMaxTariff)))*AFN0DF164MaxPn)//测量点1象限无功电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF166      ADDR_AFN0DF165+((6+(4*(1+TMaxTariff)))*AFN0DF165MaxPn)//测量点2象限无功电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF167      ADDR_AFN0DF166+((6+(4*(1+TMaxTariff)))*AFN0DF166MaxPn)//测量点3象限无功电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF168      ADDR_AFN0DF167+((6+(4*(1+TMaxTariff)))*AFN0DF167MaxPn)//测量点4象限无功电能示值（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF185      ADDR_AFN0DF168+((6+(4*(1+TMaxTariff)))*AFN0DF168MaxPn)//测量点正向有功最大需量及发生时间（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF186      ADDR_AFN0DF185+((6+(7*(1+TMaxTariff)))*AFN0DF185MaxPn)//测量点正向无功最大需量及发生时间（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF187      ADDR_AFN0DF186+((6+(7*(1+TMaxTariff)))*AFN0DF186MaxPn)//测量点反向有功最大需量及发生时间（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF188      ADDR_AFN0DF187+((6+(7*(1+TMaxTariff)))*AFN0DF187MaxPn)//测量点反向无功最大需量及发生时间（总、费率1～M）
//日冻结
#define  ADDR_AFN0DF209      ADDR_AFN0DF188+((6+(7*(1+TMaxTariff)))*AFN0DF188MaxPn)//测量点电能表远程控制通断电状态及记录
//日冻结
#define  ADDR_AFN0DF210      ADDR_AFN0DF209+((5+13)*AFN0DF209MaxPn)//测量点电能表购、用电信息
//日冻结
#define  ADDR_AFN0DF211      ADDR_AFN0DF210+(41*AFN0DF210MaxPn)//测量点三相电压、电流不平衡度95%概率值
//日冻结
#define  ADDR_AFN0DF246      ADDR_AFN0DF211+(4*AFN0DF211MaxPn)//日冻结掉电记录数据(测量点)
//掉电总次数	BCD	3
//上1次停上电事件停电时间	见附录A.15	5
//上1次停上电事件上电时间	见附录A.15	5
//……	……	……
//上10次停上电事件停电时间	见附录A.15	5
//上10次停上电事件上电时间	见附录A.15	5
#define  ADDR_AFN0DF246Pn     ADDR_AFN0DF246+(103*F246MaxPn)//日冻结掉电记录数据映射的Pn表
//2*F246MaxPn字节
#define  ADDR_DATECONGEAL_BASEEND     (ADDR_AFN0DF246Pn+(2*F246MaxPn))


	#if (USER/100)==5//上海
#define  ADDR_AFN0DF241      ADDR_DATECONGEAL_BASEEND//日冻结登录主站次数(p0)
//4BYTE 日登录次数n
#define  ADDR_AFN0DF243      ADDR_AFN0DF241+4//日冻结信号强度(p0)
//1BYTE 
#define LEN_DATECONGEAL     (ADDR_AFN0DF243+1)//每次日冻结数据总长度
	#else
#define LEN_DATECONGEAL     (ADDR_DATECONGEAL_BASEEND)//每次日冻结数据总长度
	#endif
#define ADDR_DATECONGEAL_END     (ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*MaxDateCongeal))




//类2数据抄表日冻结
#define ADDR_RMDATECONGEAL_START     ADDR_DATECONGEAL_END//抄表日冻结开始地址
//3BYTE 冻结时标(日月年)
#define ADDR_RMDATECONGEALFLAG     3//抄表日冻结标志
//1byte 冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#define  ADDR_AFN0DF9     ADDR_RMDATECONGEALFLAG+1//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//抄表日冻结
#define  ADDR_AFN0DF10      ADDR_AFN0DF9+((6+(17*(1+TMaxTariff)))* AFN0DF9MaxPn)//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//抄表日冻结
#define  ADDR_AFN0DF11      ADDR_AFN0DF10+((6+(17*(1+TMaxTariff)))*AFN0DF10MaxPn)//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//抄表日冻结
#define  ADDR_AFN0DF12      ADDR_AFN0DF11+((6+(17*(1+TMaxTariff)))*AFN0DF11MaxPn)//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//抄表日冻结
#define  ADDR_AFN0DF169      ADDR_AFN0DF12+((6+(17*(1+TMaxTariff)))*AFN0DF12MaxPn)//测量点正向有功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF170      ADDR_AFN0DF169+((6+(5*(1+TMaxTariff)))*AFN0DF169MaxPn)//测量点正向无功（组合无功1）电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF171      ADDR_AFN0DF170+((6+(4*(1+TMaxTariff)))*AFN0DF170MaxPn)//测量点反向有功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF172      ADDR_AFN0DF171+((6+(5*(1+TMaxTariff)))*AFN0DF171MaxPn)//测量点反向无功（组合无功1）电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF173      ADDR_AFN0DF172+((6+(4*(1+TMaxTariff)))*AFN0DF172MaxPn)//测量点1象限无功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF174      ADDR_AFN0DF173+((6+(4*(1+TMaxTariff)))*AFN0DF173MaxPn)//测量点2象限无功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF175      ADDR_AFN0DF174+((6+(4*(1+TMaxTariff)))*AFN0DF174MaxPn)//测量点3象限无功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF176      ADDR_AFN0DF175+((6+(4*(1+TMaxTariff)))*AFN0DF175MaxPn)//测量点4象限无功电能示值（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF189      ADDR_AFN0DF176+((6+(4*(1+TMaxTariff)))*AFN0DF176MaxPn)//测量点正向有功最大需量及发生时间（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF190      ADDR_AFN0DF189+((6+(7*(1+TMaxTariff)))*AFN0DF189MaxPn)//测量点正向无功最大需量及发生时间（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF191      ADDR_AFN0DF190+((6+(7*(1+TMaxTariff)))*AFN0DF190MaxPn)//测量点反向有功最大需量及发生时间（总、费率1～M）
//抄表日冻结
#define  ADDR_AFN0DF192      ADDR_AFN0DF191+((6+(7*(1+TMaxTariff)))*AFN0DF191MaxPn)//测量点反向无功最大需量及发生时间（总、费率1～M）
//抄表日冻结

#define LEN_RMDATECONGEAL     ( ADDR_AFN0DF192+((6+(7*(1+TMaxTariff)))*AFN0DF192MaxPn))//(( ADDR_AFN0DF192+((6+(7*(1+TMaxTariff)))*MAX_POINT))-(RMDATECONGEAL_START))//每次抄表日冻结数据总长度
#define ADDR_RMDATECONGEAL_END     ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*MaxRMDateCongeal)


//类2数据月冻结
#define ADDR_MONTHCONGEAL_START     ADDR_RMDATECONGEAL_END//月冻结开始地址
//2byte 冻结时标(当时钟年月的上1月)
#define ADDR_MONTHCONGEALFLAG     2//月冻结标志
//1byte 冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#define  ADDR_AFN0DF17     ADDR_MONTHCONGEALFLAG+1//测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
//月冻结
#define  ADDR_AFN0DF18      ADDR_AFN0DF17+((6+(17*(1+TMaxTariff)))*AFN0DF17MaxPn)//测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
//月冻结
#define  ADDR_AFN0DF19      ADDR_AFN0DF18+((6+(17*(1+TMaxTariff)))*AFN0DF18MaxPn)//测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//月冻结
#define  ADDR_AFN0DF20      ADDR_AFN0DF19+((6+(14*(1+TMaxTariff)))*AFN0DF19MaxPn)//测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
//月冻结
#define  ADDR_AFN0DF21      ADDR_AFN0DF20+((6+(14*(1+TMaxTariff)))*AFN0DF20MaxPn)//测量点正向有功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF22      ADDR_AFN0DF21+((1+(4*(1+TMaxTariff)))*AFN0DF21MaxPn)//测量点正向无功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF23      ADDR_AFN0DF22+((1+(4*(1+TMaxTariff)))*AFN0DF22MaxPn)//测量点反向有功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF24      ADDR_AFN0DF23+((1+(4*(1+TMaxTariff)))*AFN0DF23MaxPn)//测量点反向无功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF33      ADDR_AFN0DF24+((1+(4*(1+TMaxTariff)))*AFN0DF24MaxPn)//测量点月总及分相最大有功功率及发生时间、有功功率为零时间
//月冻结
#define  ADDR_AFN0DF34      ADDR_AFN0DF33+(32*AFN0DF33MaxPn)//测量点月总及分相有功最大需量及发生时间
//月冻结
#define  ADDR_AFN0DF35      ADDR_AFN0DF34+(24*AFN0DF34MaxPn)//测量点月电压统计数据
//月冻结
	#if (USER/100)==11//吉林
#define  ADDR_AFN0DF36      ADDR_AFN0DF35+((66+27)*AFN0DF35MaxPn_ReadMeter)//测量点日不平衡度越限累计时间
	#else
#define  ADDR_AFN0DF36      ADDR_AFN0DF35+((66+27)*AFN0DF35MaxPn)//测量点月不平衡度越限累计时间
	#endif
//月冻结
#define  ADDR_AFN0DF37      ADDR_AFN0DF36+(16*AFN0DF36MaxPn)//测量点月电流越限统计
//月冻结
#define  ADDR_AFN0DF38      ADDR_AFN0DF37+(38*AFN0DF37MaxPn)//测量点月视在功率越限累计时间
//月冻结
#define  ADDR_AFN0DF39      ADDR_AFN0DF38+(4*AFN0DF38MaxPn)//测量点月负载率率统计
//月冻结
#define  ADDR_AFN0DF44      ADDR_AFN0DF39+(12*AFN0DF39MaxPn)//测量点月功率因数区段累计时间
//月冻结
#define  ADDR_AFN0DF46      ADDR_AFN0DF44+(6*AFN0DF44MaxPn)//测量点月冻结铜损、铁损有功电能示值
//月冻结
#define  ADDR_AFN0DF51      ADDR_AFN0DF46+(10*AFN0DF46MaxPn)//终端月供电时间、月复位累计次数
//月冻结
#define  ADDR_AFN0DF52      ADDR_AFN0DF51+4//终端月控制统计数据
//月冻结
#define  ADDR_AFN0DF54      ADDR_AFN0DF52+4//终端与主站月通信流量
//月冻结
#define  ADDR_AFN0DF60      ADDR_AFN0DF54+4//总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
//月冻结
#define  ADDR_AFN0DF61      ADDR_AFN0DF60+(12*AFN0DF60MaxPn)//总加组月累计有功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF62      ADDR_AFN0DF61+((1+(4*(1+TMaxTariff)))*AFN0DF61MaxPn)//总加组月累计无功电能量（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF65      ADDR_AFN0DF62+((1+(4*(1+TMaxTariff)))*AFN0DF62MaxPn)//总加组超功率定值的月累计时间、月累计电能量
//月冻结
#define  ADDR_AFN0DF66      ADDR_AFN0DF65+(6*AFN0DF65MaxPn)//总加组超月电能量定值的月累计时间、累计电能量
//月冻结
#define  ADDR_AFN0DF130      ADDR_AFN0DF66+(6*AFN0DF66MaxPn)//直流模拟量越限月累计时间、最大/最小值及发生时间
//月冻结
#define  ADDR_AFN0DF157      ADDR_AFN0DF130+(14*AFN0DF130MaxPn)//测量点月冻结分相正向有功电能示值
//月冻结
#define  ADDR_AFN0DF158      ADDR_AFN0DF157+(20*AFN0DF157MaxPn)//测量点月冻结分相正向无功电能示值
//月冻结
#define  ADDR_AFN0DF159      ADDR_AFN0DF158+(17*AFN0DF158MaxPn)//测量点月冻结分相反向有功电能示值
//月冻结
#define  ADDR_AFN0DF160      ADDR_AFN0DF159+(20*AFN0DF159MaxPn)//测量点月冻结分相反向无功电能示值
//月冻结
#define  ADDR_AFN0DF177      ADDR_AFN0DF160+(17*AFN0DF160MaxPn)//测量点正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF178      ADDR_AFN0DF177+((6+(5*(1+TMaxTariff)))*AFN0DF177MaxPn)//测量点正向无功（组合无功1）电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF179      ADDR_AFN0DF178+((6+(4*(1+TMaxTariff)))*AFN0DF178MaxPn)//测量点反向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF180      ADDR_AFN0DF179+((6+(5*(1+TMaxTariff)))*AFN0DF179MaxPn)//测量点反向无功（组合无功1）电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF181      ADDR_AFN0DF180+((6+(4*(1+TMaxTariff)))*AFN0DF180MaxPn)//测量点1象限无功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF182      ADDR_AFN0DF181+((6+(4*(1+TMaxTariff)))*AFN0DF181MaxPn)//测量点2象限无功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF183      ADDR_AFN0DF182+((6+(4*(1+TMaxTariff)))*AFN0DF182MaxPn)//测量点3象限无功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF184      ADDR_AFN0DF183+((6+(4*(1+TMaxTariff)))*AFN0DF183MaxPn)//测量点4象限无功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF193      ADDR_AFN0DF184+((6+(4*(1+TMaxTariff)))*AFN0DF184MaxPn)//测量点正向有功最大需量及发生时间（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF194      ADDR_AFN0DF193+((6+(7*(1+TMaxTariff)))*AFN0DF193MaxPn)//测量点正向无功最大需量及发生时间（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF195      ADDR_AFN0DF194+((6+(7*(1+TMaxTariff)))*AFN0DF194MaxPn)//测量点反向有功最大需量及发生时间（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF196      ADDR_AFN0DF195+((6+(7*(1+TMaxTariff)))*AFN0DF195MaxPn)//测量点反向无功最大需量及发生时间（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF201      ADDR_AFN0DF196+((6+(7*(1+TMaxTariff)))*AFN0DF196MaxPn)//测量点第1时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF202      ADDR_AFN0DF201+((6+(5*(1+TMaxTariff)))*AFN0DF201MaxPn)//测量点第2时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF203      ADDR_AFN0DF202+((6+(5*(1+TMaxTariff)))*AFN0DF202MaxPn)//测量点第3时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF204      ADDR_AFN0DF203+((6+(5*(1+TMaxTariff)))*AFN0DF203MaxPn)//测量点第4时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF205      ADDR_AFN0DF204+((6+(5*(1+TMaxTariff)))*AFN0DF204MaxPn)//测量点第5时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF206      ADDR_AFN0DF205+((6+(5*(1+TMaxTariff)))*AFN0DF205MaxPn)//测量点第6时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF207      ADDR_AFN0DF206+((6+(5*(1+TMaxTariff)))*AFN0DF206MaxPn)//测量点第7时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF208      ADDR_AFN0DF207+((6+(5*(1+TMaxTariff)))*AFN0DF207MaxPn)//测量点第8时区冻结正向有功电能示值（总、费率1～M）
//月冻结
#define  ADDR_AFN0DF212      ADDR_AFN0DF208+((6+(5*(1+TMaxTariff)))*AFN0DF208MaxPn)//测量点三相电压、电流不平衡度95%概率值
//月冻结
#define  ADDR_AFN0DF213      ADDR_AFN0DF212+(4*AFN0DF212MaxPn)//测量点电能表开关操作次数及时间
//月冻结
#define  ADDR_AFN0DF214      ADDR_AFN0DF213+((5+18)*AFN0DF213MaxPn)//测量点电能表参数修改次数及时间
//月冻结
#define  ADDR_AFN0DF215      ADDR_AFN0DF214+((5+24)*AFN0DF214MaxPn)//测量点电能表购、用电信息
//月冻结
#define  ADDR_AFN0DF216      ADDR_AFN0DF215+((5+36)*AFN0DF215MaxPn)//测量点电能表结算信息
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
//故障电量	见附录A.14	kWh	5

#define  ADDR_MONTHCONGEAL_BASEEND     (ADDR_AFN0DF216+((6+(10*(1+TMaxTariff))+5)*AFN0DF216MaxPn))


	#if (USER/100)==5//上海
#define  ADDR_AFN0DF242      ADDR_MONTHCONGEAL_BASEEND//月冻结登录主站次数(p0)
//4byte 月登录次数n

#define LEN_MONTHCONGEAL     (ADDR_AFN0DF242+4)//每次月冻结数据总长度
	#else
#define LEN_MONTHCONGEAL     (ADDR_MONTHCONGEAL_BASEEND)//每次月冻结数据总长度
	#endif
#define ADDR_MONTHCONGEAL_END     ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*MaxMonthCongeal)






//类2数据曲线
#define ADDR_CURVECONGEAL_START     ADDR_MONTHCONGEAL_END
//5BYTE冻结时标(分时日月年)
#define ADDR_CURVECONGEALFLAG     5//曲线冻结标志
//1byte 冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#define  ADDR_AFN0DF73     ADDR_CURVECONGEALFLAG+1//总加组有功功率曲线
//
#define  ADDR_AFN0DF74      ADDR_AFN0DF73+(2*AFN0DF73MaxPn)//总加组无功功率曲线
//
#define  ADDR_AFN0DF75      ADDR_AFN0DF74+(2*AFN0DF74MaxPn)//总加组有功电能量曲线
//
#define  ADDR_AFN0DF76      ADDR_AFN0DF75+(4*AFN0DF75MaxPn)//总加组无功电能量曲线
//
#define  ADDR_AFN0DF81      ADDR_AFN0DF76+(4*AFN0DF76MaxPn)//测量点有功功率曲线
//
#define  ADDR_AFN0DF82      ADDR_AFN0DF81+(3*AFN0DF81MaxPn)//测量点A相有功功率曲线
//
#define  ADDR_AFN0DF83      ADDR_AFN0DF82+(3*AFN0DF82MaxPn)//测量点B相有功功率曲线
//
#define  ADDR_AFN0DF84      ADDR_AFN0DF83+(3*AFN0DF83MaxPn)//测量点C相有功功率曲线
//
#define  ADDR_AFN0DF85      ADDR_AFN0DF84+(3*AFN0DF84MaxPn)//测量点无功功率曲线
//
#define  ADDR_AFN0DF86      ADDR_AFN0DF85+(3*AFN0DF85MaxPn)//测量点A无功功率曲线
//
#define  ADDR_AFN0DF87      ADDR_AFN0DF86+(3*AFN0DF86MaxPn)//测量点B无功功率曲线
//
#define  ADDR_AFN0DF88      ADDR_AFN0DF87+(3*AFN0DF87MaxPn)//测量点C无功功率曲线
//
#define  ADDR_AFN0DF89      ADDR_AFN0DF88+(3*AFN0DF88MaxPn)//测量点A相电压曲线
//
#define  ADDR_AFN0DF90      ADDR_AFN0DF89+(2*AFN0DF89MaxPn)//测量点B相电压曲线
//
#define  ADDR_AFN0DF91      ADDR_AFN0DF90+(2*AFN0DF90MaxPn)//测量点C相电压曲线
//
#define  ADDR_AFN0DF92      ADDR_AFN0DF91+(2*AFN0DF91MaxPn)//测量点A相电流曲线
//
#define  ADDR_AFN0DF93      ADDR_AFN0DF92+(3*AFN0DF92MaxPn)//测量点B相电流曲线
//
#define  ADDR_AFN0DF94      ADDR_AFN0DF93+(3*AFN0DF93MaxPn)//测量点C相电流曲线
//
#define  ADDR_AFN0DF95      ADDR_AFN0DF94+(3*AFN0DF94MaxPn)//测量点零序电流曲线
//
#define  ADDR_AFN0DF97      ADDR_AFN0DF95+(3*AFN0DF95MaxPn)//测量点正向有功总电能量曲线
//
#define  ADDR_AFN0DF98      ADDR_AFN0DF97+(4*AFN0DF97MaxPn)//测量点正向无功总电能量曲线
//
#define  ADDR_AFN0DF99      ADDR_AFN0DF98+(4*AFN0DF98MaxPn)//测量点反向有功总电能量曲线
//
#define  ADDR_AFN0DF100      ADDR_AFN0DF99+(4*AFN0DF99MaxPn)//测量点反向无功总电能量曲线
//
#define  ADDR_AFN0DF101      ADDR_AFN0DF100+(4*AFN0DF100MaxPn)//测量点正向有功总电能示值
//
#define  ADDR_AFN0DF102      ADDR_AFN0DF101+(4*AFN0DF101MaxPn)//测量点正向无功总电能示值
//
#define  ADDR_AFN0DF103      ADDR_AFN0DF102+(4*AFN0DF102MaxPn)//测量点反向有功总电能示值
//
#define  ADDR_AFN0DF104      ADDR_AFN0DF103+(4*AFN0DF103MaxPn)//测量点反向无功总电能示值
//
#define  ADDR_AFN0DF105      ADDR_AFN0DF104+(4*AFN0DF104MaxPn)//测量点总功率因数
//
#define  ADDR_AFN0DF106      ADDR_AFN0DF105+(2*AFN0DF105MaxPn)//测量点A相功率因数
//
#define  ADDR_AFN0DF107      ADDR_AFN0DF106+(2*AFN0DF106MaxPn)//测量点B相功率因数
//
#define  ADDR_AFN0DF108      ADDR_AFN0DF107+(2*AFN0DF107MaxPn)//测量点C相功率因数
//
#define  ADDR_AFN0DF109      ADDR_AFN0DF108+(2*AFN0DF108MaxPn)//测量点电压相位角曲线
//
#define  ADDR_AFN0DF110      ADDR_AFN0DF109+(6*AFN0DF109MaxPn)//测量点电流相位角曲线
//
#define  ADDR_AFN0DF138      ADDR_AFN0DF110+(6*AFN0DF110MaxPn)//直流模拟量数据曲线
//
#define  ADDR_AFN0DF145      ADDR_AFN0DF138+(2*AFN0DF138MaxPn)//测量点1象限无功总电能示值曲线
//
#define  ADDR_AFN0DF146      ADDR_AFN0DF145+(4*AFN0DF145MaxPn)//测量点4象限无功总电能示值曲线
//
#define  ADDR_AFN0DF147      ADDR_AFN0DF146+(4*AFN0DF146MaxPn)//测量点2象限无功总电能示值曲线
//
#define  ADDR_AFN0DF148      ADDR_AFN0DF147+(4*AFN0DF147MaxPn)//测量点3象限无功总电能示值曲线
//
//#define  ADDR_AFN0DF217      ADDR_AFN0DF148+(4*AFN0DF148MaxPn)//台区集中抄表载波主节点白噪声曲线
//
//#define  ADDR_AFN0DF218      ADDR_AFN0DF217+1//台区集中抄表载波主节点色噪声曲线
//
//F219 测量点组合数据曲线
#define  ADDR_KeyUserPnListCongeal     (ADDR_AFN0DF148+(4*AFN0DF148MaxPn))//自扩展:集中抄表重点户曲线冻结Pn列表
//重点户曲线冻结测量点号1-MaxKeyUser对应的实际测量点号,0或0xff为无效

	#if (ADDR_KeyUserPnListCongeal+(2*MaxKeyUser))<1024
#define LEN_CURVECONGEAL     1024//最小1K(仅为FLASH寿命)
	#else
#define LEN_CURVECONGEAL     (ADDR_KeyUserPnListCongeal+(2*MaxKeyUser))
	#endif

#define ADDR_CURVECONGEAL_BASEEND     ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*MaxCurveCongeal)//通用曲线结束


#if (USER/100)==5//上海
#define ADDR_AFN0CF242_Next     ADDR_CURVECONGEAL_BASEEND//当日登录主站信息
//6BYTE 登录时间1 见附录A.1(清除值0xff)
//...
//6BYTE 登录时间n 见附录A.1(清除值0xff)
#define AFN0CF242MaxCount     256//当日登录主站信息最大记录次数
#define ADDR_AFN0CF243     ADDR_AFN0CF242_Next+(6*AFN0CF242MaxCount)//当日信号强度
//00:00信号强度	BIN	1(清除值0xff)
//01:00信号强度	BIN	1(清除值0xff)
//……	BIN	1
//22:00信号强度	BIN	1(清除值0xff)
//23:00信号强度	BIN	1(清除值0xff)
#define ADDR_CURVECONGEAL_ExtStart     ADDR_AFN0CF243+24//扩展曲线开始
//5BYTE冻结时标(分时日月年)
#define ADDR_CURVECONGEALFLAG_Ext     5//曲线冻结标志
//1byte 冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#define  ADDR_AFN0DF235     ADDR_CURVECONGEALFLAG_Ext+1
//2byte 终端AB线电压曲线
#define  ADDR_AFN0DF236     ADDR_AFN0DF235+2
//2byte 终端BC线电压曲线
#define  ADDR_AFN0DF237     ADDR_AFN0DF236+2
//2byte 终端CA线电压曲线

#define LEN_CURVECONGEAL_Ext     (ADDR_AFN0DF237+2)
#define ADDR_CURVECONGEAL_ExtEND     ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*MaxCurveCongeal_Ext)//扩展曲线结束
#else//#if (USER/100)==5//上海
#define ADDR_CURVECONGEAL_ExtEND     ADDR_CURVECONGEAL_BASEEND//扩展曲线结束
#endif


#define ADDR_CURVECONGEAL_END     ADDR_CURVECONGEAL_ExtEND//曲线结束







#define ADDR_AFN0D_End     (ADDR_CURVECONGEAL_END)//类2数据结束地址

#if (ADDR_AFN0D_End-ADDR_AFN0D_Start)>LEN_Terminal_EFLASH_AFN0D
	#error (ADDR_AFN0D_End-ADDR_AFN0D_Start)>LEN_Terminal_EFLASH_AFN0D
#endif

#endif


