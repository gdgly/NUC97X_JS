
//终端统计
#ifndef Terminal_Statistic_H
#define Terminal_Statistic_H



void Init_StatisticData_Command(void);//终端统计数据初始化(命令,全部清除)
void Terminal_Statistic(void);//终端统计计算,分任务运行
void Terminal_Reset_Count(void);//终端复位次数
void Terminal_Ctrl_Count(u32 ci);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸

void AFN0DF27F35_Computer(void);//终端统计.电压统计数据
u32 ReNewValue(u32 b,u32 c);//恢复值计算,入口b=限值,c=恢复系数
void AFN0DF26F34_Computer(void);//F26F34日月总及分相最大需量及发生时间

#endif





