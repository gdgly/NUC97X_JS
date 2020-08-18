
#ifndef IRAM_TerminalSave_H
#define IRAM_TerminalSave_H


#define ADDR_TFM_YMDHMS     ADDR_Terminal_SaveRam_Start//掉电前时钟
//7byte hex年年月日时分秒
//1byte 
#define ADDR_POWER_YMDHMS     ADDR_TFM_YMDHMS+8//掉电事件判定掉电产生时时钟
//7byte hex年年月日时分秒
//1byte 
#define ADDR_TerminalCongeal_Time     ADDR_POWER_YMDHMS+8//上次冻结时间(固定地址,仪表冻结时用此值判断终端是否已冻结，终端冻结后再仪表冻结，冻结数据清0在仪表进行)
//7byte hex年年月日时分秒
//1byte 上次判断冻结时间的实时钟秒

#define ADDR_TerminalFMTimer     ADDR_TerminalCongeal_Time+8//终端FM定时器地址
//6BYTE 上次计算的秒分时日月年

//秒定时器
//2BYTE 0 1 秒加计数计算分
//2BYTE 2 2 
#define TFMSTIMER_NUM     2//终端FM中秒定时器数

//分定时器
//2BYTE 0 1 终端保电分定时器
//2BYTE 2 2 设置键按下分定时器hex
//2BYTE 4 3 工厂模式有效分定时器hex

//2BYTE 6 4 总加组1当前功率下浮控的控制时间分定时器
//2BYTE ...
//2BYTE 20 11 总加组8当前功率下浮控的控制时间分定时器

//2BYTE 22 12 控制轮次1遥控跳闸限电时间分定时器
//2BYTE ...
//2BYTE 36 19 控制轮次8遥控跳闸限电时间分定时器

//2BYTE 38 20 RS232功能转换分定时器
//2BYTE 40 21 RS485-1功能转换分定时器
//2BYTE 42 22 RS485-2功能转换分定时器
#define TFMMTIMER_NUM     (3+16+3)//终端FM中分定时器数
#define ADDR_TerminalRealTimer     ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2)//实走定时器,调整时钟无效
#define TREALMTIMER_NUM     1//终端实走分定时器数
//1BYTE 0 0=掉电时间未计算,1=掉电时间已计算
//1BYTE 1 保留
//2BYTE 2 TESAM密钥失效分定时器


#define ADDR_2131     ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2)//当月A相电压合格率
//属性2（电压合格率数据）∷=structure
//{
//  当日电压合格率         电压合格率，
//  当月电压合格率         电压合格率
//}
//电压合格率∷=structure
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
//电压合格率   long-unsigned（单位：%，换算：-2），
//电压超限率   long-unsigned（单位：%，换算：-2），
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
//}
#define LENmax_2131     48
#define ADDR_2132     ADDR_2131+LENmax_2131//当月B相电压合格率
//属性2（电压合格率数据）∷=structure
//{
//  当日电压合格率         电压合格率，
//  当月电压合格率         电压合格率
//}
//电压合格率∷=structure
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
//电压合格率   long-unsigned（单位：%，换算：-2），
//电压超限率   long-unsigned（单位：%，换算：-2），
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
//}
#define LENmax_2132     48
#define ADDR_2133     ADDR_2132+LENmax_2132//当月C相电压合格率
//属性2（电压合格率数据）∷=structure
//{
//  当日电压合格率         电压合格率，
//  当月电压合格率         电压合格率
//}
//电压合格率∷=structure
//{
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
//电压合格率   long-unsigned（单位：%，换算：-2），
//电压超限率   long-unsigned（单位：%，换算：-2），
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
//}
#define LENmax_2133     48
#define ADDR_2140     ADDR_2133+LENmax_2133//日最大有功功率及发生时间
//最大功率及发生时间∷=structure
//{
//  最大功率值 double-long-unsigned，
//  发生时间   date_time_s
//}
//功率单位：kW，换算：-4
#define LENmax_2140     15
#define ADDR_2141     ADDR_2140+LENmax_2140//月最大有功功率及发生时间
//最大功率及发生时间∷=structure
//{
//  最大功率值 double-long-unsigned，
//  发生时间   date_time_s
//}
//功率单位：kW，换算：-4
#define LENmax_2141     15

#define ADDR_2200     ADDR_2141+LENmax_2141//通信流量
//数值∷=structure
//{
//当日通信流量  double-long-unsigned，
//当月通信流量  double-long-unsigned
//}
//单位：byte，换算：0
#define LENmax_2200     12
#define ADDR_2200_EventRecordFags     ADDR_2200+LENmax_2200//通信流量超限事件已记录标志,月更新时清0
//1byte 0=没记录,1=已记录

#define ADDR_2203     ADDR_2200_EventRecordFags+1//供电时间
//数值∷=structure
//{
//日供电累计时间  double-long-unsigned，
//月供电累计时间  double-long-unsigned
//}
//单位：分钟，换算：0
//1byte 上次计算供电时间的实时钟分
#define LENmax_2203     13
#define ADDR_2204     ADDR_2203+LENmax_2203//复位累计次数
//数值∷=structure
//{
//日复位累计次数  long-unsigned，
//月复位累计次数  long-unsigned
//}
#define LENmax_2204     8


//脉冲输入
#define ADDR_impEnergy_Start     ADDR_2204+LENmax_2204
#define ADDR_impEnergy_Remainder     ADDR_impEnergy_Start//脉冲电能余数
//4BYTE 脉冲计量1((脉冲数*10000)/电度常数)的余数hex
//...
//4BYTE 脉冲计量NUMmax_IMPIN((脉冲数*10000)/电度常数)的余数hex
#define LEN_impEnergy_Remainder     (4*NUMmax_IMPIN)
#define ADDR_impEnergy     ADDR_impEnergy_Remainder+LEN_impEnergy_Remainder//脉冲计量电能示值
//脉冲计量1∷=array 电能示值
//脉冲计量1∷=double-long-unsigned 4位小数
//...
//脉冲计量NUMmax_IMPIN∷=array 电能示值
//脉冲计量NUMmax_IMPIN∷=double-long-unsigned 4位小数
#define LENper_impEnergy     (2+(5*(TMaxTariff+1)))
#define ADDR_impEnergy_Day     ADDR_impEnergy+(LENper_impEnergy*NUMmax_IMPIN)//脉冲计量当日电能量
//脉冲计量1∷=array 电能量
//脉冲计量1∷=double-long-unsigned 4位小数
//...
//脉冲计量NUMmax_IMPIN∷=array 电能量
//脉冲计量NUMmax_IMPIN∷=double-long-unsigned 4位小数
#define ADDR_impEnergy_Month     ADDR_impEnergy_Day+(LENper_impEnergy*NUMmax_IMPIN)//脉冲计量当月电能量
//脉冲计量1∷=array 电能量
//脉冲计量1∷=double-long-unsigned 4位小数
//...
//脉冲计量NUMmax_IMPIN∷=array 电能量
//脉冲计量NUMmax_IMPIN∷=double-long-unsigned 4位小数
#define ADDR_impDemand     ADDR_impEnergy_Month+(LENper_impEnergy*NUMmax_IMPIN)//脉冲当月最大需量和时间,月更新清0
//脉冲计量1总及费率最大需量数组∷=array 最大需量及发生时间
//最大需量及发生时间∷=structure
//{
//  最大需量值 CHOICE，double-long [5]，double-long-unsigned  [6]
//  发生时间   date_time_s
//}
//...
//脉冲计量NUMmax_IMPIN总及费率最大需量数组∷=array 最大需量及发生时间
#define LENper_impDemand     (2+(15*(TMaxTariff+1)))
#define ADDR_impEnergy_End     ADDR_impDemand+(LENper_impDemand*NUMmax_IMPIN)



//总加组
#define ADDR_230x_Sour     ADDR_impEnergy_End//总加电量计算用原始电能示值
//1byte 已有成员数据个数
//6+4*9*(TMaxTariff+1) byte 电能表地址,原始电能示值1 正有 正无 反有 反无
//...
//6+4*9*(TMaxTariff+1) byte 电能表地址,原始电能示值n 正有 正无 反有 反无
//精度是 0.1WH
#define LENsig_230x_sour	(6+4*9*(TMaxTariff+1))//186
#define LENper_230x_Sour    (1+LENsig_230x_sour*NUMmax_230x)//1489
#define LENmax_230x_Sour	(LENper_230x_Sour*NUMmax_TAG)//
#define ADDR_230x_7     ADDR_230x_Sour+LENmax_230x_Sour//总加日有功电量
//总加日有功电量∷=array 电能量
//电能量∷=long64
//精度是 0.1WH
#define LENper_230x_7     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_7     (LENper_230x_7*NUMmax_TAG)
#define ADDR_230x_8     ADDR_230x_7+LENmax_230x_7//总加日无功电量
//总加日无功电量∷=array 电能量
//电能量∷=long64
#define LENper_230x_8     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_8     (LENper_230x_8*NUMmax_TAG)
#define ADDR_230x_9     ADDR_230x_8+LENmax_230x_8//总加月有功电量
//总加月有功电量∷=array 电能量
//电能量∷=long64
//精度是 0.1WH
#define LENper_230x_9     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_9     (LENper_230x_9*NUMmax_TAG)
#define ADDR_230x_10     ADDR_230x_9+LENmax_230x_9//总加月无功电量
//总加月无功电量∷=array 电能量
//电能量∷=long64
#define LENper_230x_10     (2+(9*(TMaxTariff+1)))
#define LENmax_230x_10     (LENper_230x_10*NUMmax_TAG)
#define ADDR_230x_11     ADDR_230x_10+LENmax_230x_10//总加剩余电量（费）
//总加剩余电量(费)∷=long64
#define LENper_230x_11     9
#define LENmax_230x_11     (LENper_230x_11*NUMmax_TAG)


#define ADDR_AutoSearchMeter_Start     (ADDR_230x_11+LENmax_230x_11)//自动搜表地址
//6BYTE RS485-1当前搜表地址
//1BYTE RS485-1当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
//6BYTE RS485-2当前搜表地址
//1BYTE RS485-2当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
//6BYTE RS485-3当前搜表地址
//1BYTE RS485-3当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
#define LEN_PerPortAutoSearchMeter      7
#define ADDR_AutoSearchMeter_End     (ADDR_AutoSearchMeter_Start+(3*LEN_PerPortAutoSearchMeter))



#define ADDR_MeterAutoReport_3011_Count     ADDR_AutoSearchMeter_End//电能表停电主动上报事件记录序号（顺序生成）
//4BYTE
#define ADDR_EventCurrentvalue_3003     ADDR_MeterAutoReport_3011_Count+4//事件当前值记录表-电能表断相事件
//当前值记录表∷=array structure
//{
//	事件发生次数 double-long-unsigned，
//	事件累计时间 double-long-unsigned 
//}
//数组元素按顺序对应事件记录表1、事件记录表2、事件记录表3、事件记录表4，如无该记录表，则该元素为NULL


#define LENper_EventCurrentvalue     50//每事件当前值记录表字节数
#define NUM_EventCurrentvalue     1//事件当前值记录表个数(用于清0)


#if (USER/100)==15//安徽
	#define ADDR_2131_OneDay   		ADDR_EventCurrentvalue_3003+(LENper_EventCurrentvalue*NUM_EventCurrentvalue)//存储每次收到A相电压合格率存储的数据,保存每天的数据
	#define LENmax_2131_OneDay     (48*NUM_RMmax)
	#define ADDR_Terminal_SaveRam_End     ADDR_2131_OneDay+(LENmax_2131_OneDay*3)
#endif

#ifndef ADDR_Terminal_SaveRam_End
	#define ADDR_Terminal_SaveRam_End     ADDR_EventCurrentvalue_3003+(LENper_EventCurrentvalue*NUM_EventCurrentvalue)
#endif





#endif



























