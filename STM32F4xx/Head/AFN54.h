

#ifndef AFN54_H
#define AFN54_H
#if (USER/100)==14//福建系
#define ADDR_AFN54F1     ADDR_AFN54_Start  //终端外部数据采集模式
//终端外部数据采集模式设置  BS8 1
#define ADDR_AFN54F2     (ADDR_AFN54F1+1)//终端信道配置
//终端通信端口号(1-31)  31*5byte
//信道用途-信道类型  									BS8  1
//信道采用的协议字(保留字，暂不使用)  BS8  1
//通信控制字   												BS8  1
//本地通信模块应用超时时间						BS8  1
//接收等待报文超时时间								BIN  1
#define ADDR_AFN54F3     (ADDR_AFN54F2+31*5)//终端报文格式字
//报文格式字   1byte    
#define ADDR_AFN54F4     (ADDR_AFN54F3+1)//终端广播任务配置
//广播物理端口号(1-31)  31*100(70byte数据帧)
//执行结果						BIN	1  00:失败  01:成功  其他:未执行
//启动时间						TIM	6
//结束时间						TIM	6
//通信对象类型				BIN	1
//广播模式						BIN	1
//广播时长						BIN	1
//数据时标修正				BIN	2
//广播报文格式字			BIN	1
//广播报文长度N				BIN	1
//广播请求内容(报文)	BIN	N
#define ADDR_AFN54F5     (ADDR_AFN54F4+31*100)//任务方案配置(条件触发任务及定时任务)
#define LEN_AFN54F5_Pn	26//每个方案最大存储长度26
#define ADDR_AFN04F205   ADDR_AFN54F5
#define LEN_AFN04F205_Pn  LEN_AFN54F5_Pn
#define MAX_FUJ_TASK	6000//福建第四类任务数
//条件触发任务 14byte
//任务类型-扩展	BIN	1		0:触发任务 1:定时任务
//任务方案号		BIN	2
//通信端口号		BIN	1
//任务优先级		BIN	1
//触发条件			BIN	2
//执行对象			BIN	1
//任务执行有效时长	BIN	1
//失败重试次数	BIN	1
//任务数据存储次数	BIN	1
//安全认证模式	BIN	1
//保留字				BIN	2

//定时任务  26byte
//任务类型-扩展	BIN	1		0:触发任务 1:定时任务
//任务方案号	BIN	2
//通信端口号	BIN	1
//任务优先级	BIN	1
//任务重复执行周期数值	BIN	1
//任务重复执行周期单位	BS8	1
//失败重试周期数值	BIN	1
//失败重试周期单位	BS8	1
//失败重试次数	BIN	1
//任务数据存储次数	BIN	1
//任务执行开始时间	TIM	6
//任务执行结束时间	TIM	6
//安全认证模式			BIN	1
//保留字						BIN	2
#define ADDR_AFN54F10     (ADDR_AFN54F5+LEN_AFN54F5_Pn*MAX_FUJ_TASK)//任务主动上报配置
//任务方案号(1-60000)
//主动上报模式	BS8	1
//主动上报方式	BIN	1
#define ADDR_AFN54_TASK_LIST     (ADDR_AFN54F10+2*MAX_FUJ_TASK)//任务方案与具体任务关联表
//#define ADDR_AFN54_TASK_LIST_F		ADDR_AFN54_TASK_LIST//任务方案存储有效标志MAX_FUJ_TASK/8
//#define LEN_AFN54_TASK_LIST_F			8192
#define ADDR_AFN54_TASK_LIST_M		ADDR_AFN54_TASK_LIST//任务方案存储LEN_FuJTaskInfo*65535
#define ADDR_AFN54_TASK_LIST_S		(ADDR_AFN54_TASK_LIST_M+3*65535)//具体方案存储7*65535,便于以后扩展
#define MAX_PER_SUB_TASK	2040//每个任务方案对应的具体任务数
//任务方案与具体任务关联表
#define ADDR_AFN54F8     (ADDR_AFN54_TASK_LIST+(3+7)*65535)//具体任务配置
#define ADDR_AFN04F208   ADDR_AFN54F8
#define LEN_AFN54F8_Pn	100//每个方案最大存储长度100
#define LEN_AFN04F208_Pn  LEN_AFN54F8_Pn
#define MAX_FUJ_SUB_TASK	50000//最大具体任务配置数--1000的倍数增加
//具体任务									100byte
//具体任务存储序号					BIN		2//上限为50000
//具体任务号 								BIN		2//同一任务不允许重复，不同任务允许重复
//通信对象类型							BIN		1
//任务对象通信地址					ADDR	6
//帧序号										BS8		1
//报文重组、数据时标修正字	BS8		1
//时标修正值								BS8		1
//下行报文格式字						BIN		1
//任务请求内容报文长度L			BIN		1
//任务请求内容(报文)				BIN		L(<100-18)
//任务返回内容预计报文长度	BIN		1
//返回报文格式字						BIN		1
#define ADDR_AFN54F11     (ADDR_AFN54F8+LEN_AFN54F8_Pn*MAX_FUJ_SUB_TASK)//从节点主动注册任务配置
//允许注册的物理端口(1-31)
//允许主动注册的通信对象类型	BIN	1
//任务执行开始时间	TIM	6
//允许执行时间	BIN	2
#define ADDR_AFN54F20     (ADDR_AFN54F11+31*9)//从节点事件主动上报配置
//下行本地模块通信信道工作模式配置	BIN	1
#define ADDR_AFN54F22     (ADDR_AFN54F20+1)//第四类数据执行和主动上报控制
//第四类数据执行和主动上报控制字	BS8	1
#define ADDR_AFN54F23     (ADDR_AFN54F22+1)//本地通信模块通信协议切换
#define ADDR_AFN04F223   ADDR_AFN54F23
//物理端口号(1-31)
//本地通信模块通信协议工作类型	BIN	1
#define ADDR_AFN54F50     (ADDR_AFN54F23+31*1)//本地通信模块通信协议切换
#define MAX_POWER_RATE_CTRL	100
#define MAX_54F50_LEN	60
//功率控制方案配置
//功率控制任务方案号	BIN	1
//控制方案数据来源方案号	BIN	2
//控制方案通信对象地址	BIN	6
//控制任务开始时间	TIM	6
//控制任务结束时间	TIM	6
//控制方案每日有效时段
//每周有效时段	BS8	1
//受控轮次	BIN	1
//滑差时间	BIN	1
//控制阈值
//保留字	BIN	2
#define ADDR_AFN56F11_RS485_1			(ADDR_AFN54F50+MAX_POWER_RATE_CTRL*MAX_54F50_LEN)//2路485端口
#define LEN_PerAFN56F11	8
//执行状态									BIN		1  0x77等待判断 0x33启动 0x22执行 0x11写入结束标志位 0x00结束
//通信地址数量							BIN		2
//通信对象类型							BIN		1
//本次主动注册任务开始时间	TIM		6
//本次主动注册任务结束时间	TIM		6
//通信对象序号1							BIN		2
//通信对象地址1							Addr	6
//............							.......
//通信对象序号N							BIN		2
//通信对象地址N							Addr	6
#define ADDR_AFN56F11_RS485_2			(ADDR_AFN56F11_RS485_1+16+LEN_PerAFN56F11*MaxRS485User)//2路485端口
#define ADDR_AFN56F11_Carrier			(ADDR_AFN56F11_RS485_2+16+LEN_PerAFN56F11*MaxRS485User)

#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier+16+LEN_PerAFN56F11*MaxRS485AddCarrierPn-3)//错误定义;少了3字节,
//#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier+16+LEN_PerAFN56F11*MaxRS485AddCarrierPn)//正确定义,注册任务的记录定义
//上边3行定义有错,每个少1个字节
//#define ADDR_AFN56F11_RS485_2			(ADDR_AFN56F11_RS485_1+1+15+LEN_PerAFN56F11*MaxRS485User)//2路485端口
//#define ADDR_AFN56F11_Carrier			(ADDR_AFN56F11_RS485_2+1+15+LEN_PerAFN56F11*MaxRS485User)

//#define ADDR_Terminal_Number		(ADDR_AFN56F11_Carrier-2+15+LEN_PerAFN56F11*MaxRS485AddCarrierPn)
//12byte终端资产码

#define ADDR_AFN01Fx_FuJ	(ADDR_Terminal_Number+12)
//1byte 初始化指令-F4初始化时需对载波模块进行初始化 0x55-F4初始化
#define ADDR_ATMYSYSINFO	(ADDR_AFN01Fx_FuJ+1)
//1byte AT命令:强制锁网,2=2G, 3=3G, 4=4G, 其它=不锁网
#define ADDR_AFN54_LogStartTime    (ADDR_ATMYSYSINFO+1) //日志记录起始日期时间   6字节  年月日时  (分秒 备用)
#define ADDR_AFN54_End    ((ADDR_AFN54_LogStartTime+6+0x1ffff)&0xfffe0000) //结束地址128K对齐
#if (ADDR_AFN54_End-ADDR_AFN54_Start)>LEN_Terminal_EFLASH_AFN54
	#error (ADDR_AFN54_End-ADDR_AFN54_Start)>LEN_Terminal_EFLASH_AFN54
#endif
#endif
#endif



