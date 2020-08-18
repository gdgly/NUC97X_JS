

#ifndef Terminal_AFN0C_RealTimeData_H
#define Terminal_AFN0C_RealTimeData_H


void Init_AFN0CF1(void);//初始化终端类1数据F1,版本信息
void Init_Class1Data_UpPower(void);//类1数据初始化(上电)
void Init_Class1Data_Command(void);//类1数据初始化(命令,全部清除)
void Terminal_Class1Data_Computer(void);//终端类1数据计算
void HourCongeal_AutoRW(void);//小时冻结自动IO读写或清0,完成后结果处理
void TotalAddEnergyInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//冻结区间总加电能量计算,结果值在DATABUFF
void PointkWhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//冻结区间测量点有功电能量计算,结果值在DATABUFF
void PointkvarhInc_Computer(u32 SourAddr,u32 CurrAddr,u32 Pn);//冻结区间无功电能量计算,结果值在DATABUFF
void Terminal_AFN0CF2_Computer(void);//终端类1数据F2,终端日历时钟


#endif


