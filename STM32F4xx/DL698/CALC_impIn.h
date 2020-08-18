

#ifndef CALC_impIn_H
#define CALC_impIn_H
#ifdef __cplusplus
 extern "C" {
#endif

void Init_impInData(void);//清0脉冲输入终端电能和需量数据
void Terminal_impIn_Energy(void);//终端脉冲输入-电能
void Terminal_impIn_Demand(void);//终端脉冲输入-功率和最大需量

#ifdef __cplusplus
 }
#endif
#endif







