

#ifndef Terminal_AFN0D_CongealData_H
#define Terminal_AFN0D_CongealData_H




void Init_Class2Data_UpPower(void);//类2数据初始化(上电)
void Init_Class2Data_Command(void);//类2数据初始化(命令,全部清除)
void Class2Congeal_AutoRW(void);//类2冻结自动IO读写或清0,完成后结果处理
void Terminal_Class2Data_Computer(void);//终端类2数据计算


void AFN0C_Full_CurveCongeal_Buff(u32 Addr_CongealBuff);//AFN0C实时数据填入曲线冻结缓冲
void AFN0C_Full_DateCongeal_Buff(u32 Addr_CongealBuff);//AFN0C实时数据填入日冻结缓冲
void AFN0C_Full_RMDateCongeal_Buff(u32 Addr_CongealBuff,u32 ADDR_CongealFlags);//AFN0C实时数据填入抄表日冻结缓冲;入口:测量点分端口号冻结标志已计算
void AFN0C_Full_MonthCongeal_Buff(u32 Addr_CongealBuff);//AFN0C实时数据填入月冻结缓冲


#endif
