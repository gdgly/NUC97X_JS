
//终端
#ifndef IRAM_Terminal_H
#define IRAM_Terminal_H

#include "IRAM_TerminalDisplay698.h"




#define ADDR_IRAMTerminal_Start     (ADDR_TerminalDisplay_IRAM_End)

//脉冲输入
#define ADDR_Terminal_impIn_ESRAM_START     ADDR_IRAMTerminal_Start
#define ADDR_Terminal_impIn_Ctrl     ADDR_Terminal_impIn_ESRAM_START//终端脉冲输入控制(字对齐)
//1BYTE 输入1脉冲10ms计数
//...
//1BYTE 输入8脉冲10ms计数
#define ADDR_Terminal_impIn_Count     ADDR_Terminal_impIn_Ctrl+NUMmax_IMPIN
//2BYTE 输入1脉冲计数(半字对齐)
//...
//2BYTE 输入NUMmax_IMPIN脉冲计数(半字对齐)
#define ADDR_Terminal_InDemandimp     ADDR_Terminal_impIn_Count+(2*NUMmax_IMPIN)//终端脉冲输入,需量脉冲滑移(字对齐)
//4*15*2BYTE 脉冲输入1(半字对齐)
//...
//4*15*2BYTE 脉冲输入NUMmax_IMPIN(半字对齐)
#define LEN_Terminal_InDemandimp     (4*15*2)//每脉冲口每15秒滑移
#define ADDR_Terminal_impIn_ESRAM_END     ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*NUMmax_IMPIN)

#define ADDR_Average_Count     ADDR_Terminal_impIn_ESRAM_END//内部交采1分钟平均电压值计算计数
//1BYTE 上次计算实时钟秒
//1BYTE 上次计算实时钟分
//1BYTE 平均值计算加次数计数
//3BYTE HEX UA加
//3BYTE HEX UB加
//3BYTE HEX UC加
#define ADDR_Average_Voltage     ADDR_Average_Count+12//内部交采A,B,C相1分钟平均电压(上电初始为0xee)
//2BYTE_BCD 1位小数,A相,(上电初始为0xee)
//2BYTE_BCD 1位小数,B相,(上电初始为0xee)
//2BYTE_BCD 1位小数,C相,(上电初始为0xee)
#define ADDR_I2Task     ADDR_Average_Voltage+6//I2主任务计数(原在SDRAM,因SDRAM死机后无法计数不运行SDRAM死机检查的可能故改放在IRAM)
//1byte 计数
		
		
		
		
#define ADDR_IRAMTerminal_End     (ADDR_I2Task+1)
#if ((ADDR_IRAMTerminal_End)-(ADDR_IRAMTerminal_Start))>(1*1024)
	#error
#endif


#define ADDR_Terminal_IRAM_End     ((ADDR_IRAMTerminal_End+3)&0xfffffffc)//4字节对齐


#endif






























