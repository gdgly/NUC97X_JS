
#ifndef IRAM_TerminalSave_H
#define IRAM_TerminalSave_H


#define ADDR_TFM_YMDHMS     ADDR_Terminal_SaveRam_Start			// 掉电前时钟
// 7byte hex年年月日时分秒
// 1byte 
#define ADDR_POWER_YMDHMS     ADDR_TFM_YMDHMS+8					// 掉电事件判定掉电产生时时钟
// 7byte hex年年月日时分秒
// 1byte 
#define ADDR_TerminalCongeal_Time     ADDR_POWER_YMDHMS+8		// 上次冻结时间(固定地址,仪表冻结时用此值判断终端是否已冻结，终端冻结后再仪表冻结，冻结数据清0在仪表进行)
// 7byte hex年年月日时分秒
// 1byte 

#define ADDR_TerminalFMTimer     ADDR_TerminalCongeal_Time+8	// 终端FM定时器地址
#define TFMSTIMER_NUM     2										// 终端FM中秒定时器数
#define TFMMTIMER_NUM     (3+16+3)								// 终端FM中分定时器数
// 6BYTE 上次计算的秒分时日月年
// 秒定时器
// 2BYTE 0 1 秒加计数计算分
// 2BYTE 2 2 

// 分定时器
// 2BYTE 0 1 终端保电分定时器
// 2BYTE 2 2 设置键按下分定时器hex
// 2BYTE 4 3 工厂模式有效分定时器hex

// 2BYTE 6 4 总加组1当前功率下浮控的控制时间分定时器
// 2BYTE ...
// 2BYTE 20 11 总加组8当前功率下浮控的控制时间分定时器

// 2BYTE 22 12 控制轮次1遥控跳闸限电时间分定时器
// 2BYTE ...
// 2BYTE 36 19 控制轮次8遥控跳闸限电时间分定时器

// 2BYTE 38 20 RS232功能转换分定时器
// 2BYTE 40 21 RS485-1功能转换分定时器
// 2BYTE 42 22 RS485-2功能转换分定时器
#define ADDR_TerminalRealTimer     ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2)	//实走定时器,调整时钟无效
#define TREALMTIMER_NUM     1		//终端实走分定时器数
// 1BYTE 0 0=掉电时间未计算,1=掉电时间已计算
// 1BYTE 1 保留
// 2BYTE 2 TESAM密钥失效分定时器

#define ADDR_ZY_IRAM_SAVE_START     		((ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2) + 3) & 0xFFFFFFFC)
#define LEN_ZY_IRAM_SAVE					(16 * 1024)


#define ADDR_MeterFileInfo					(ADDR_ZY_IRAM_SAVE_START)
#define LEN_MeterFileInfo					(64)

#define ADDR_TerminalSpecData				(ADDR_MeterFileInfo + LEN_MeterFileInfo)
#define LEN_TerminalSpecData				(128)

#define ADDR_RS485_CH1_AMR_TaskCtrl			(ADDR_TerminalSpecData + LEN_TerminalSpecData)
#define LEN_RS485_CH1_AMR_TaskCtrl			(720)

#define ADDR_RS485_CH2_AMR_TaskCtrl			(ADDR_RS485_CH1_AMR_TaskCtrl + LEN_RS485_CH1_AMR_TaskCtrl)
#define LEN_RS485_CH2_AMR_TaskCtrl			(720)

#define ADDR_RS485_CH3_AMR_TaskCtrl			(ADDR_RS485_CH2_AMR_TaskCtrl + LEN_RS485_CH2_AMR_TaskCtrl)
#define LEN_RS485_CH3_AMR_TaskCtrl			(720)

#define ADDR_PLC_AMR_TaskCtrl				(ADDR_RS485_CH3_AMR_TaskCtrl + LEN_RS485_CH3_AMR_TaskCtrl)
#define LEN_PLC_AMR_TaskCtrl				(720)//err20190103 增加PLC抄表标志的表格 err20190222 删除表格 没用到

#define ADDR_MeterDayFreezFailList			(ADDR_PLC_AMR_TaskCtrl + LEN_PLC_AMR_TaskCtrl)
#define LEN_MeterDayFreezFailList			(6*1024)
#define LEN_ListForOneMeter					(6)
//#define ADDR_MeterMonthFreezFailList		(ADDR_MeterDayFreezFailList + LEN_MeterDayFreezFailList)
//#define LEN_MeterMonthFreezFailList		(6*1024)

#define ADDR_RS485_CH1_AMS_TaskCtrl			(ADDR_MeterDayFreezFailList + LEN_MeterDayFreezFailList)
#define LEN_RS485_CH1_AMS_TaskCtrl			(64)

#define ADDR_RS485_CH2_AMS_TaskCtrl			(ADDR_RS485_CH1_AMS_TaskCtrl + LEN_RS485_CH1_AMS_TaskCtrl)
#define LEN_RS485_CH2_AMS_TaskCtrl			(64)

#define ADDR_RS485_CH3_AMS_TaskCtrl			(ADDR_RS485_CH2_AMS_TaskCtrl + LEN_RS485_CH2_AMS_TaskCtrl)
#define LEN_RS485_CH3_AMS_TaskCtrl			(64)


#define ADDR_Terminal_RunTime				(ADDR_RS485_CH3_AMS_TaskCtrl + LEN_RS485_CH3_AMS_TaskCtrl)

//分配空间 16M
//已用空间 64 + 128 + 720*4 + 6*1024 = 9216 (9*1024)
//剩余空间 16-9 = 7M



#define ADDR_MXN_IRAM_SAVE_START     		(ADDR_ZY_IRAM_SAVE_START + LEN_ZY_IRAM_SAVE)
#define LEN_MXN_IRAM_SAVE					(46 * 1024)

#define QCSG_GLOBAL_TIME					(ADDR_MXN_IRAM_SAVE_START)
#define QCSG_STATISTICS_START				(QCSG_GLOBAL_TIME + 8)
#define QCSG_VOLTAGE_PASSRATE_DAY			(QCSG_STATISTICS_START)
#define QCSG_A_VOLTAGE_PASSRATE_DAY			(QCSG_VOLTAGE_PASSRATE_DAY + 1024)
#define QCSG_B_VOLTAGE_PASSRATE_DAY			(QCSG_A_VOLTAGE_PASSRATE_DAY + 1024)
#define QCSG_C_VOLTAGE_PASSRATE_DAY			(QCSG_B_VOLTAGE_PASSRATE_DAY + 1024)
#define QCSG_VOLTAGE_PASSRATE_MONTH			(QCSG_C_VOLTAGE_PASSRATE_DAY + 1024)
#define QCSG_A_VOLTAGE_PASSRATE_MONTH		(QCSG_VOLTAGE_PASSRATE_MONTH + 1024)
#define QCSG_B_VOLTAGE_PASSRATE_MONTH		(QCSG_A_VOLTAGE_PASSRATE_MONTH + 1024)
#define QCSG_C_VOLTAGE_PASSRATE_MONTH		(QCSG_B_VOLTAGE_PASSRATE_MONTH + 1024)
#define QCSG_CURRENT_EXTREME_DAY			(QCSG_C_VOLTAGE_PASSRATE_MONTH + 1024)
#define QCSG_CURRENT_EXTREME_MONTH			(QCSG_CURRENT_EXTREME_DAY + 1024)
#define QCSG_N_CURRENT_EXTREME_DAY			(QCSG_CURRENT_EXTREME_MONTH + 1024)
#define QCSG_N_CURRENT_EXTREME_MONTH		(QCSG_N_CURRENT_EXTREME_DAY + 512)
#define QCSG_ACTIVE_POWER_DAY				(QCSG_N_CURRENT_EXTREME_MONTH + 512)
#define QCSG_ACTIVE_POWER_MONTH				(QCSG_ACTIVE_POWER_DAY + 512)
#define QCSG_REACTIVE_POWER_DAY				(QCSG_ACTIVE_POWER_MONTH + 512)
#define QCSG_REACTIVE_POWER_MONTH			(QCSG_REACTIVE_POWER_DAY + 512)
#define QCSG_POWER_FACTOR_DAY				(QCSG_REACTIVE_POWER_MONTH + 512)
#define QCSG_POWER_FACTOR_MONTH				(QCSG_POWER_FACTOR_DAY + 512)
#define QCSG_POWER_FACTOR_ZONE_DAY			(QCSG_POWER_FACTOR_MONTH + 512)
#define QCSG_POWER_FACTOR_ZONE_MONTH		(QCSG_POWER_FACTOR_ZONE_DAY + 1024)
#define QCSG_POWER_FACTOR_ZONE_A_DAY		(QCSG_POWER_FACTOR_ZONE_MONTH + 1024)
#define QCSG_POWER_FACTOR_ZONE_A_MONTH		(QCSG_POWER_FACTOR_ZONE_A_DAY + 1024)
#define QCSG_POWER_FACTOR_ZONE_B_DAY		(QCSG_POWER_FACTOR_ZONE_A_MONTH + 1024)
#define QCSG_POWER_FACTOR_ZONE_B_MONTH		(QCSG_POWER_FACTOR_ZONE_B_DAY + 1024)
#define QCSG_POWER_FACTOR_ZONE_C_DAY		(QCSG_POWER_FACTOR_ZONE_B_MONTH + 1024)
#define QCSG_POWER_FACTOR_ZONE_C_MONTH		(QCSG_POWER_FACTOR_ZONE_C_DAY + 1024)
#define QCSG_POWER_DOWN_NUM_DAY				(QCSG_POWER_FACTOR_ZONE_C_MONTH + 1024)
#define QCSG_POWER_DOWN_TIME_DAY			(QCSG_POWER_DOWN_NUM_DAY + 4)
#define QCSG_POWER_DOWN_NUM_MONTH			(QCSG_POWER_DOWN_TIME_DAY + 4)
#define QCSG_POWER_DOWN_TIME_MONTH			(QCSG_POWER_DOWN_NUM_MONTH + 4)
#define QCSG_STATISTICS_END					(QCSG_POWER_DOWN_TIME_MONTH + 4)
//日月电量记录结构体
#define QCSG_ENERGY_START					(QCSG_STATISTICS_END)
#define QCSG_DAY_ENERGY_LIST_CTRL			(QCSG_ENERGY_START)
#define QCSG_MONTH_ENERGY_LIST_CTRL			(QCSG_DAY_ENERGY_LIST_CTRL + 1024)
#define QCSG_DAY_ENERGY_LIST_LCD			(QCSG_MONTH_ENERGY_LIST_CTRL + 1024)
#define QCSG_MONTH_ENERGY_LIST_LCD			(QCSG_DAY_ENERGY_LIST_LCD + 1024)
#define QCSG_ENERGY_END						(QCSG_MONTH_ENERGY_LIST_LCD + 1024)
//测量点冻结存储记录结构体
#define QCSG_RECORD_FROZEN_PN_LIST			(QCSG_ENERGY_END)
//负荷管理功能相关结构体
#define QCSG_CTRL_POWERDOWN_SAVE_INFO		(QCSG_RECORD_FROZEN_PN_LIST + 256)

#define ADDR_Terminal_SaveRam_End     (ADDR_MXN_IRAM_SAVE_START + LEN_MXN_IRAM_SAVE)




#endif



























