
//终端
#ifndef IRAM_Terminal_H
#define IRAM_Terminal_H

#include "IRAM_TerminalDisplayCSG.h"

#define ADDR_Terminal_ClassDataIRAM_Satrt     (ADDR_TerminalDisplay_IRAM_End)
#define ADDR_Terminal_ClassDataLen     ADDR_Terminal_ClassDataIRAM_Satrt	//ERAM需字对齐
// 1类数据长度寄存器或2类数据费率寄存器(原2类数据直接改写FLASH,集中器的29GL写需0.5秒再若全部测量点太长太长时间,故增加费率寄存器)
// 1byte 当前取得的2类数据的费率数或谐波次数
// 1byte 当前取得的2类数据的费率字节偏移
// 1byte 1=当前取得2类数据的费率数或谐波次数需修正,0=不需修正
// 1byte
#define ADDR_HourCongeal_AutoRW     ADDR_Terminal_ClassDataLen+4			//小时冻结自动写EFLASH控制地址(字对齐)
// 1BYTE 读写操作代码0=IOCLR,1=IOR,2=IOW
// 1BYTE IOCLR的值
// 1BYTE 数据标记0=
// 1BYTE 数据计数
// 4BYTE R0地址(sram)
// 4BYTE R1地址(flash或其他)
// 4BYTE R2长度,值!=0表示操作未完成
// 4BYTE 冻结开始地址(时标地址),同初始值R1
#define ADDR_Class2Congeal_AutoRW     ADDR_HourCongeal_AutoRW+20			//类2数据冻结自动写EFLASH控制地址(字对齐)
// 1BYTE 读写操作代码0=IOCLR,1=IOR,2=IOW
// 1BYTE IOCLR的值
// 1BYTE 数据标记0=曲线冻结,1=日冻结,2=抄表日冻结,3=月冻结,4=日冻结更新,5=补抄上2日,6=补抄上3日
// 1BYTE 数据计数
// 4BYTE R0地址(sram)
// 4BYTE R1地址(flash或其他)
// 4BYTE R2长度,值!=0表示操作未完成
// 4BYTE 冻结开始地址(时标地址),同初始值R1
// 5BYTE 冻结时标年月日时分
// 3BYTE 

// 脉冲输入
#define ADDR_Terminal_impIn_ESRAM_START     (((ADDR_Class2Congeal_AutoRW+28)+3)&0xfffffffc)
#define ADDR_Terminal_impIn_Ctrl     ADDR_Terminal_impIn_ESRAM_START				//终端脉冲输入控制(字对齐)
// 1BYTE 输入1脉冲10ms计数
// ...
// 1BYTE 输入8脉冲10ms计数
#define ADDR_Terminal_impIn_Count     ADDR_Terminal_impIn_Ctrl+8
// 2BYTE 输入1脉冲计数(半字对齐)
// ...
// 2BYTE 输入NUMmax_IMPIN脉冲计数(半字对齐)
#define ADDR_Terminal_InDemandimp     ADDR_Terminal_impIn_Count+(2*NUMmax_IMPIN)	//终端脉冲输入,需量脉冲滑移(字对齐)
// 4*15*2BYTE 脉冲输入1(半字对齐)
// ...
// 4*15*2BYTE 脉冲输入NUMmax_IMPIN(半字对齐)
#define LEN_Terminal_InDemandimp     (4*15*2)										//每脉冲口每15秒滑移
#define ADDR_Terminal_impIn_ESRAM_END     ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*NUMmax_IMPIN)
#define ADDR_Average_Count     ADDR_Terminal_impIn_ESRAM_END						//内部交采1分钟平均电压值计算计数
// 1BYTE 上次计算实时钟秒
// 1BYTE 上次计算实时钟分
// 1BYTE 平均值计算加次数计数
// 3BYTE HEX UA加
// 3BYTE HEX UB加
// 3BYTE HEX UC加
#define ADDR_Average_Voltage     ADDR_Average_Count+12	//内部交采A,B,C相1分钟平均电压(上电初始为0xee)
// 2BYTE_BCD 1位小数,A相,(上电初始为0xee)
// 2BYTE_BCD 1位小数,B相,(上电初始为0xee)
// 2BYTE_BCD 1位小数,C相,(上电初始为0xee)

		
#define ADDR_Terminal_ClassDataIRAM_End     	(ADDR_Average_Voltage + 6)

#define ADDR_TaskCtrlIRAMStart					((ADDR_Terminal_ClassDataIRAM_End+3)&0xfffffffc)

#define LEN_Uart_Task_Ctrl						(32)
#define ADDR_RS232_Task_Ctrl					(ADDR_TaskCtrlIRAMStart)
#define ADDR_IR_Task_Ctrl						(ADDR_RS232_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_GPRS_UART_Task_Ctrl				(ADDR_IR_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_ETHE_UART_Task_Ctrl				(ADDR_GPRS_UART_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_RS485_CH1_Task_Ctrl				(ADDR_ETHE_UART_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_RS485_CH2_Task_Ctrl				(ADDR_RS485_CH1_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_RS485_CH3_Task_Ctrl				(ADDR_RS485_CH2_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_PLC_YX_UART_Task_Ctrl				(ADDR_RS485_CH3_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_ACSAMPLE_UART_Task_Ctrl			(ADDR_PLC_YX_UART_Task_Ctrl + LEN_Uart_Task_Ctrl)		

#define LEN_Uart_MultiFrameSendTask				(64)
#define ADDR_RS232_MultiFrameSendTask			(ADDR_ACSAMPLE_UART_Task_Ctrl + LEN_Uart_Task_Ctrl)
#define ADDR_IR_MultiFrameSendTask				(ADDR_RS232_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_GPRS_MultiFrameSendTask			(ADDR_IR_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_ETHE_MultiFrameSendTask			(ADDR_GPRS_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_RS485_CH1_MultiFrameSendTask		(ADDR_ETHE_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_RS485_CH2_MultiFrameSendTask		(ADDR_RS485_CH1_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_RS485_CH3_MultiFrameSendTask		(ADDR_RS485_CH2_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_PLC_YX_MultiFrameSendTask			(ADDR_RS485_CH3_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)

#define LEN_Uart_RX_ChCtrl						(32)
#define ADDR_RS232_RX_ChCtrl					(ADDR_PLC_YX_MultiFrameSendTask + LEN_Uart_MultiFrameSendTask)
#define ADDR_IR_RX_ChCtrl						(ADDR_RS232_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_GPRS_RX_ChCtrl						(ADDR_IR_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_ETHE_RX_ChCtrl						(ADDR_GPRS_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_RS485_CH1_RX_ChCtrl				(ADDR_ETHE_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_RS485_CH2_RX_ChCtrl				(ADDR_RS485_CH1_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_RS485_CH3_RX_ChCtrl				(ADDR_RS485_CH2_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_PLC_YX_RX_ChCtrl					(ADDR_RS485_CH3_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_ACSAMPLE_RX_ChCtrl					(ADDR_PLC_YX_RX_ChCtrl + LEN_Uart_RX_ChCtrl)		


#define ADDR_TerminalTaskCtrl					(ADDR_ACSAMPLE_RX_ChCtrl + LEN_Uart_RX_ChCtrl)
#define ADDR_PLCManageCtrl						(ADDR_TerminalTaskCtrl + 128)
#define LEN_PLCManageCtrl						(6900)

#define ADDR_RS232_RxBuf						(ADDR_PLCManageCtrl + LEN_PLCManageCtrl)
#define LEN_RS232_RxBuf							(0)

#define ADDR_IR_RxBuf							(ADDR_RS232_RxBuf + LEN_RS232_RxBuf)
#define LEN_IR_RxBuf							(0)

#define ADDR_GPRS_RxBuf							(ADDR_IR_RxBuf + LEN_IR_RxBuf)
#define LEN_GPRS_RxBuf							(0)

#define ADDR_ETHE_RxBuf							(ADDR_GPRS_RxBuf + LEN_GPRS_RxBuf)
#define LEN_ETHE_RxBuf							(0)

#define ADDR_RS485_CH1_RxBuf					(ADDR_ETHE_RxBuf + LEN_ETHE_RxBuf)
#define LEN_RS485_CH1_RxBuf						(0)

#define ADDR_RS485_CH2_RxBuf					(ADDR_RS485_CH1_RxBuf + LEN_RS485_CH1_RxBuf)
#define LEN_RS485_CH2_RxBuf						(0)

#define ADDR_RS485_CH3_RxBuf					(ADDR_RS485_CH2_RxBuf + LEN_RS485_CH2_RxBuf)
#define LEN_RS485_CH3_RxBuf						(0)

#define ADDR_PLC_YX_RxBuf						(ADDR_RS485_CH3_RxBuf + LEN_RS485_CH3_RxBuf)
#define LEN_PLC_YX_RxBuf						(0)

#define ADDR_PLC_AMR_DataBuf					(ADDR_PLC_YX_RxBuf + LEN_PLC_YX_RxBuf)
#define LEN_PLC_AMR_DataBuf						(256 * 6)

#define ADDR_RS485_CH1_AMR_DataBuf				(ADDR_PLC_AMR_DataBuf + LEN_PLC_AMR_DataBuf)
#define LEN_RS485_CH1_AMR_DataBuf				(1024)

#define ADDR_RS485_CH2_AMR_DataBuf				(ADDR_RS485_CH1_AMR_DataBuf + LEN_RS485_CH1_AMR_DataBuf)
#define LEN_RS485_CH2_AMR_DataBuf				(1024)

#define ADDR_RS485_CH3_AMR_DataBuf				(ADDR_RS485_CH2_AMR_DataBuf + LEN_RS485_CH2_AMR_DataBuf)
#define LEN_RS485_CH3_AMR_DataBuf				(1024)

#define ADDR_LastNormalTaskAMRTime				(ADDR_RS485_CH3_AMR_DataBuf + LEN_RS485_CH3_AMR_DataBuf)
#define LEN_LastNormalTaskAMRTime				(256*4)

#define ADDR_LastRelayTaskAMRTime				(ADDR_LastNormalTaskAMRTime + LEN_LastNormalTaskAMRTime)
#define LEN_LastRelayTaskAMRTime				(256*4)

#define ADDR_LastNormalTaskReportTime			(ADDR_LastRelayTaskAMRTime + LEN_LastRelayTaskAMRTime)
#define LEN_LastNormalTaskReportTime			(256)

#define ADDR_LastRelayTaskReportTime			(ADDR_LastNormalTaskReportTime + LEN_LastNormalTaskReportTime)
#define LEN_LastRelayTaskReportTime				(256)

#define ADDR_GPRS_ReportTask					(ADDR_LastRelayTaskReportTime + LEN_LastRelayTaskReportTime)
#define LEN_GPRS_ReportTask						(256)

#define ADDR_ETHE_ReportTask					(ADDR_GPRS_ReportTask + LEN_GPRS_ReportTask)
#define LEN_ETHE_ReportTask						(256)

#define ADDR_AMRTaskManageCtrl					(ADDR_ETHE_ReportTask + LEN_ETHE_ReportTask)
#define LEN_AMRTaskManageCtrl					(1024)

#define ADDR_RS485_CH1_RelayTaskCtrl			(ADDR_AMRTaskManageCtrl + LEN_AMRTaskManageCtrl)
#define LEN_AMRRelayTaskCtrl					(4)
#define ADDR_RS485_CH2_RelayTaskCtrl			(ADDR_RS485_CH1_RelayTaskCtrl + LEN_AMRRelayTaskCtrl)
#define ADDR_RS485_CH3_RelayTaskCtrl			(ADDR_RS485_CH2_RelayTaskCtrl + LEN_AMRRelayTaskCtrl)
#define ADDR_PLC_RelayTaskCtrl					(ADDR_RS485_CH3_RelayTaskCtrl + LEN_AMRRelayTaskCtrl)


#define ADDR_PnListBuf							(ADDR_DATABUFF)
#define LEN_PnListBuf							(2048)

#define ADDR_Terminal_IRAM_Over					((ADDR_PLC_RelayTaskCtrl + LEN_AMRRelayTaskCtrl + 3) & 0xfffffffc)	

#if ((ADDR_Terminal_IRAM_Over - ADDR_Terminal_ClassDataIRAM_Satrt) > (30*1024))
	#error ADDR_Terminal_IRAM_Over > (30*1024)
#endif


#define ADDR_Terminal_IRAM_End     ((ADDR_Terminal_IRAM_Over + 3) & 0xfffffffc)	// 4字节对齐


#endif






























