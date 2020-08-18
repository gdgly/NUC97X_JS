

#ifndef Terminal_AFN0D_CongealData_Init_H
#define Terminal_AFN0D_CongealData_Init_H


void Init_Class2Data(u32 Pn);//类2数据初始化(指定Pn数据)
void PnHourCongealCount(void);//测量点的小时冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
void PnCurveCongealCount(void);//测量点的曲线冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
void PnDateCongealCount(void);//测量点的日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
void PnRMDateCongealCount(void);//测量点的抄表日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
void PnMonthCongealCount(void);//测量点的月冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数

u32 PnHourCongeal(u32 Pn);//测量点小时冻结有效,返回=0无效,1=有效
u32 PnDateCongealNo(u32 CongealNo,u32 Pn);//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
u32 PnRMDateCongealNo(u32 CongealNo,u32 Pn);//测量点抄表日冻结号有效,返回=0xffffffff表示无效(没找到)
u32 PnMonthCongealNo(u32 CongealNo,u32 Pn);//测量点月冻结号有效,返回=0xffffffff表示无效(没找到)
u32 PnCurveCongealNo(u32 CongealNo,u32 Pn);//测量点曲线冻结号有效,返回=0xffffffff表示无效(没找到)


#endif

