
//终端
#ifndef IRAM_Terminal_H
#define IRAM_Terminal_H

#include "IRAM_TerminalDisplay.h"

#define ADDR_Terminal_ClassDataIRAM_Satrt     (ADDR_TerminalDisplay_IRAM_End)
#define ADDR_Terminal_ClassDataLen     ADDR_Terminal_ClassDataIRAM_Satrt//ERAM需字对齐
//1类数据长度寄存器或2类数据费率寄存器(原2类数据直接改写FLASH,集中器的29GL写需0.5秒再若全部测量点太长太长时间,故增加费率寄存器)
//1byte 当前取得的2类数据的费率数或谐波次数
//1byte 当前取得的2类数据的费率字节偏移
//1byte 1=当前取得2类数据的费率数或谐波次数需修正,0=不需修正
//1byte
#define ADDR_HourCongeal_AutoRW     ADDR_Terminal_ClassDataLen+4//小时冻结自动写EFLASH控制地址(字对齐)
//1BYTE 读写操作代码0=IOCLR,1=IOR,2=IOW
//1BYTE IOCLR的值
//1BYTE 数据标记0=
//1BYTE 数据计数
//4BYTE R0地址(sram)
//4BYTE R1地址(flash或其他)
//4BYTE R2长度,值!=0表示操作未完成
//4BYTE 冻结开始地址(时标地址),同初始值R1
#define ADDR_Class2Congeal_AutoRW     ADDR_HourCongeal_AutoRW+20//类2数据冻结自动写EFLASH控制地址(字对齐)
//1BYTE 读写操作代码0=IOCLR,1=IOR,2=IOW
//1BYTE IOCLR的值
//1BYTE 数据标记0=曲线冻结,1=日冻结,2=抄表日冻结,3=月冻结,4=日冻结更新,5=补抄上2日,6=补抄上3日
//1BYTE 数据计数
//4BYTE R0地址(sram)
//4BYTE R1地址(flash或其他)
//4BYTE R2长度,值!=0表示操作未完成
//4BYTE 冻结开始地址(时标地址),同初始值R1
//5BYTE 冻结时标年月日时分
//3BYTE 

//脉冲输入
#define ADDR_Terminal_impIn_ESRAM_START     (((ADDR_Class2Congeal_AutoRW+28)+3)&0xfffffffc)
#define ADDR_Terminal_impIn_Ctrl     ADDR_Terminal_impIn_ESRAM_START//终端脉冲输入控制(字对齐)
//1BYTE 输入1脉冲10ms计数
//...
//1BYTE 输入8脉冲10ms计数
#define ADDR_Terminal_impIn_Count     ADDR_Terminal_impIn_Ctrl+8
//2BYTE 输入1脉冲计数(半字对齐)
//...
//2BYTE 输入MaxImpInPort脉冲计数(半字对齐)
#define ADDR_Terminal_InDemandimp     ADDR_Terminal_impIn_Count+(2*MaxImpInPort)//终端脉冲输入,需量脉冲滑移(字对齐)
//4*15*2BYTE 脉冲输入1(半字对齐)
//...
//4*15*2BYTE 脉冲输入MaxImpInPort(半字对齐)
#define LEN_Terminal_InDemandimp     (4*15*2)//每脉冲口每15秒滑移
#define ADDR_Terminal_impIn_ESRAM_END     ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*MaxImpInPort)
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
		
		
#define ADDR_Terminal_ClassDataIRAM_End     (ADDR_I2Task+1)


#if ((ADDR_Terminal_ClassDataIRAM_End-ADDR_Terminal_ClassDataIRAM_Satrt)>(1*1024))
	#error ADDR_Terminal_ClassDataIRAM>1024
#endif


#define ADDR_Terminal_IRAM_End     ((ADDR_Terminal_ClassDataIRAM_End+3)&0xfffffffc)//4字节对齐


#endif






























