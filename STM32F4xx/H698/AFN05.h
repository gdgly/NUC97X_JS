

#ifndef AFN05_H
#define AFN05_H



//终端AFN05控制命令数据从此开始
#define ADDR_AFN05F1     ADDR_AFN05_Start//遥控跳闸(控制轮次)
#define ADDR_AFN05F2     ADDR_AFN05F1//允许合闸(控制轮次)
//1byte 0=解除,0x55=投入,进入控制后0x5A
//告警延时时间	限电时间	BIN	1
#define ADDR_AFN05F9     ADDR_AFN05F1+(2*MaxControlRound)//时段功控投入(总加组号)
#define ADDR_AFN05F17     ADDR_AFN05F9//时段功控解除(总加组号)
//1byte 0=解除,0x55=投入
//时段功控投入标志	  BS8	1
//时段功控定值方案号 BIN	l
#define ADDR_AFN05F10     ADDR_AFN05F9+(3*MaxTotalAddGroup)//厂休功控投入(总加组号)
#define ADDR_AFN05F18     ADDR_AFN05F10//厂休功控解除(总加组号)
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F11     ADDR_AFN05F10+(1*MaxTotalAddGroup)//营业报停功控投入(总加组号)
#define ADDR_AFN05F19     ADDR_AFN05F11//营业报停功控解除(总加组号)
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F12     ADDR_AFN05F11+(1*MaxTotalAddGroup)//当前功率下浮控投入(总加组号)
#define ADDR_AFN05F20     ADDR_AFN05F12//当前功率下浮控解除(总加组号)
//1byte 0=解除,0x55=投入,进入控制后0x5A
//当前功率下浮控定值滑差时间	BIN	min	1
//当前功率下浮控定值浮动系数．	数据格式04	％	1
//控后总加有功功率冻结延时时间	BIN	min	1
//当前功率下浮控的控制时间	BIN	0.5h	1
//当前功率下浮控第1轮告警时间	BIN	min	1
//当前功率下浮控第2轮告警时间	BIN	min	1
//当前功率下浮控第3轮告警时间	BIN	min	1
//当前功率下浮控第4轮告警时间	BIN	min	1
#define ADDR_AFN05F15     ADDR_AFN05F12+(9*MaxTotalAddGroup)//月电控投入(总加组号)
#define ADDR_AFN05F23     ADDR_AFN05F15//月电控解除(总加组号)
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F16     ADDR_AFN05F15+(1*MaxTotalAddGroup)//购电控投入(总加组号)
#define ADDR_AFN05F24     ADDR_AFN05F16//购电控解除(总加组号)
//1byte 0=解除,0x55=投入

#define ADDR_AFN05F25     ADDR_AFN05F16+(1*MaxTotalAddGroup)//终端保电投入
#define ADDR_AFN05F33     ADDR_AFN05F25//终端保电解除
//1byte 0=解除,0x55=投入,进入定时后0x5A
//1byte 保电持续时间	    BIN	  0.5h	    1
#define ADDR_AFN05F26     ADDR_AFN05F25+2//催费告警投入
#define ADDR_AFN05F34     ADDR_AFN05F26//催费告警解除
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F27     ADDR_AFN05F26+1//允许终端与主站通话
#define ADDR_AFN05F35     ADDR_AFN05F27//禁止终端与主站通话
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F28     ADDR_AFN05F27+1//终端剔除投入
#define ADDR_AFN05F36     ADDR_AFN05F28//终端剔除解除
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F29     ADDR_AFN05F28+1//允许终端主动上报
#define ADDR_AFN05F37     ADDR_AFN05F29//禁止终端主动上报
//1byte 0=解除,0x55=投入
#define ADDR_AFN05F30     ADDR_AFN05F29+1//终端对电能表对时功能的开启与关闭
//1byte 0x55
//1byte 0=关闭，1=开启，其它值关闭
#define ADDR_AFN05F31     ADDR_AFN05F30+2//F31对时命令
//此命令要求立即执行
#define ADDR_AFN05F32     ADDR_AFN05F31+6//F32中文信息
//202*16普通信息16条;1byte+1byte+200byte
//202*16重要信息16条;1byte+1byte+200byte	

#define ADDR_AFN05F38     ADDR_AFN05F32+(202*16*2)//激活终端连接主站
#define ADDR_AFN05F39     ADDR_AFN05F38//命令终端断开连接
//1byte 0x55=激活终端连接主站,0xAA=命令终端断开连接,执行后清0

#define ADDR_AFN05F41     ADDR_AFN05F38+1//电容器控制投入(测量点号)
//1byte 0x55=收到此命令,执行后清0
//电容器组	  BSl6	 2
#define ADDR_AFN05F42     ADDR_AFN05F41+(3*AFN05F41MaxPn)//电容器控制切除(测量点号)
//1byte 0x55=收到此命令,执行后清0
//电容器组	  BSl6	 2
#define ADDR_AFN05F49     ADDR_AFN05F42+(3*AFN05F42MaxPn)//命令指定通信端口暂停抄表
#define ADDR_AFN05F50     ADDR_AFN05F49//命令指定通信端口恢复抄表
//1byte 通信端口1(0=恢复,0x55=暂停)
//...
//1byte 通信端口n(0=恢复,0x55=暂停)
#define ADDR_AFN05F51     ADDR_AFN05F49+(1*31)//命令指定通信端口重新抄表
//1byte 通信端口1(0x55=收到此命令,启动重新抄表后0x5A,完成后清0)
//...
//1byte 通信端口n(0x55=收到此命令,启动重新抄表后0x5A,完成后清0)
#define ADDR_AFN05F52     ADDR_AFN05F51+(1*31)//初始化指定通信端口下的全部中继路由信息
//1byte 通信端口1(0x55=收到此命令,执行后清0)
//...
//1byte 通信端口n(0x55=收到此命令,执行后清0)
#define ADDR_AFN05F53     ADDR_AFN05F52+(1*31)//删除指定通信端口下的全部电表
//1byte 通信端口1(0x55=收到此命令,执行后清0)
//...
//1byte 通信端口n(0x55=收到此命令,执行后清0)

#define ADDR_AFN05F103     ADDR_AFN05F53+(1*31)//启动集中器搜索未知电表(湖南系扩展)
//1byte 0=解除,0x55=收到此命令
//2byte 搜索允许的最大时间（分钟）	BIN，低字节在前	2
//1byte 执行标志	BS8	1

#define ADDR_AFN05F149     ADDR_AFN05F103+4//电能表通信参数自动维护功能开启与关闭
//1byte 0=解除,0x55=收到此命令
//1byte 自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
//1BYTE 0=解除,0x55=收到此命令,0x5a=立即执行,0xaa=每日执行;RS485-1
//1BYTE 0=解除,0x55=收到此命令,0x5a=立即执行,0xaa=每日执行;RS485-2
//1BYTE 0=解除,0x55=收到此命令,0x5a=立即执行,0xaa=每日执行;RS485-3
//1BYTE 0=解除,0x55=收到此命令,0x5a=立即执行,0xaa=每日执行;载波
#define ADDR_AFN05F150     ADDR_AFN05F149+6//复位测量点参数变更标志
//1byte 0x55=收到此命令,执行后清0

#define ADDR_kWCtrl1_FixedValue     ADDR_AFN05F150+1//当前功率下浮控定值
//总加组1
//2BYTE 数据格式2
//...
//总加组N


//终端AFN10数据转发命令
#define MaxAFN10F12TaskType     2//最大转发F12任务类型数(只身份认证和对时)
#define LEN_AFN10F12TaskData    256//最大数据转发F12内的任务数据长度
#define LEN_AFN10F12PerTask    (2+1+2+LEN_AFN10F12TaskData+2+(40*MaxRS485AddCarrierPn))//数据转发F12每个任务数据总长度
#define ADDR_AFN10F12     (ADDR_kWCtrl1_FixedValue+(2*MaxTotalAddGroup))//对批量电表下发任务命令
//任务类型0
//任务格式	BIN	2
//任务类型	BIN	1
//任务数据长度Ln	BIN	2
//任务数据	BIN	LEN_AFN10F12TaskData
//本次配置电表数量n	BIN	2
//表号1	BIN	8
//表的密钥密文1	BIN	32
//表号2	BIN	8
//表的密钥密文2	BIN	32
//...
//表号n	BIN	8
//表的密钥密文n	BIN	32

//任务类型1
//...
//任务类型n
//...
#define ADDR_AFN10F13     ADDR_AFN10F12+(LEN_AFN10F12PerTask*MaxAFN10F12TaskType)//批量电表下发任务的执行状态
//任务类型0
//1BYTE 0=没完成,1=完成,其它值=执行错误
//...
//1BYTE 表n
//任务类型1
//...
//任务类型n
//...
#define ADDR_AFN05_End     (ADDR_AFN10F13+(MaxRS485AddCarrierPn*MaxAFN10F12TaskType))//AFN05命令结束地址

#if (ADDR_AFN05_End-ADDR_AFN05_Start)>LEN_Terminal_EFLASH_AFN05
	#error (ADDR_AFN05_End-ADDR_AFN05_Start)>LEN_Terminal_EFLASH_AFN05
#endif

#endif

