
//设置参数
#ifndef AFN04MaxPn_H
#define AFN04MaxPn_H


#define AFN04F10MaxPn     MaxRS485AddCarrierPn//终端电能表／交流采样装置配置参数
#define AFN04F11MaxPn     MaxImpInPort//终端脉冲配置参数

#define AFN04F13MaxPn     MaxAnalogPort//终端电压／电流模拟量配置参数
#define AFN04F14MaxPn     MaxTotalAddGroup//终端总加组配置参数(固定空间相隔)
#define AFN04F15MaxPn     MaxDiffGroup//有功总电能量差动越限事件参数设置


#define AFN04F25MaxPn     MaxRS485AddCarrierPn//测量点基本参数
#define AFN04F26MaxPn     MaxRS485AddCarrierPn//测量点限值参数
#define AFN04F27MaxPn     MaxRS485AddCarrierPn//测量点铜损、铁损参数
#define AFN04F28MaxPn     MaxRS485AddCarrierPn//测量点功率因数分段限值
#define AFN04F29MaxPn     MaxRS485AddCarrierPn//终端当地电能表显示号
#define AFN04F30MaxPn     MaxRS485AddCarrierPn//台区集中抄表停抄/投抄设置
#define AFN04F31MaxPn     MaxRS485AddCarrierPn//载波从节点附属节点地址

#define AFN04F41MaxPn     MaxTotalAddGroup//时段功控定值(总加组号)(存贮时3个方案的时段固定空间相隔)
#define AFN04F42MaxPn     MaxTotalAddGroup//厂休功控参数(总加组号)
#define AFN04F43MaxPn     MaxTotalAddGroup//功率控制的功率计算滑差时间(总加组号)
#define AFN04F44MaxPn     MaxTotalAddGroup//营业报停控参数(总加组号)
#define AFN04F45MaxPn     MaxTotalAddGroup//功控轮次设定(总加组号)
#define AFN04F46MaxPn     MaxTotalAddGroup//月电量控定值(总加组号)
#define AFN04F47MaxPn     MaxTotalAddGroup//购电量(费)控参数(总加组号)
#define AFN04F48MaxPn     MaxTotalAddGroup//电控轮次设定(总加组号)
#define AFN04F49MaxPn     MaxControlRound//功控告警时间(控制轮次)


#define AFN04F65MaxPn     MaxTimerTask//定时发送1类数据任务设置(任务号)
#define AFN04F66MaxPn     MaxTimerTask//定时发送2类数据任务设置(任务号)
#define AFN04F67MaxPn     MaxTimerTask//定时发送1类数据任务启动／停止设置(任务号)
#define AFN04F68MaxPn     MaxTimerTask//定时发送2类数据任务启动／停止设置(任务号)

#define AFN04F73MaxPn     MaxCPn//电容器参数
#define AFN04F74MaxPn     MaxCPn//电容器投切运行参数
#define AFN04F75MaxPn     MaxCPn//电容器保护参数
#define AFN04F76MaxPn     MaxCPn//电容器投切控制方式
#define AFN04F81MaxPn     MaxDCPn//直流模拟量输入变比
#define AFN04F82MaxPn     MaxDCPn//直流模拟量限值
#define AFN04F83MaxPn     MaxDCPn//直流模拟量冻结参数
#define AFN04F84MaxPn     (7)//终端通信端口配置参数(通信端口数量n	0～31)

#if (USER/100)==2//用户标识:山东版
#define AFN04F88MaxPn     MaxRS485AddCarrierPn//山东增补协议电能表资产编号
#endif

#define AFN04F104MaxPn     MaxRS485AddCarrierPn//曲线冻结密度参数(测量点号)
#define AFN04F105MaxPn     MaxRS485AddCarrierPn//电能表数据分级归类参数(测量点号)
#define AFN04F106MaxPn     MaxMeterEventClass//电能表数据分级参数(数据分级类号)
#define AFN04F107MaxPn     MaxMeterEventLevel//电能表数据分级周期参数(数据等级号)


#endif
