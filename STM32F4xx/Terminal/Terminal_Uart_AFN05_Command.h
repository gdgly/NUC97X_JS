

#ifndef Terminal_Uart_AFN05_Command_H
#define Terminal_Uart_AFN05_Command_H

typedef __packed struct
{
  vu32 Addr;//数据地址
	vu8 project;//支持方案:B7=,B6=通信模块,B5=集中器Ⅱ型,B4=公变终端,B3=专变终端,B2=集中器,B1=网络表,B0=智能表
  vu8 PnType;//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
  vu16 Len;//数据长度
  vu8 PnMax;//最大Pn
  vu8 Command;//命令码0x0或0x55
}TerminalCommandUartLib_TypeDef;


u32 Get_AFN05CommandLib_MaxFn(void);

#endif


