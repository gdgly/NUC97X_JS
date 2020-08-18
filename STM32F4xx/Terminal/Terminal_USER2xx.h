
//用户标识:山东版
#ifndef Terminal_USER200_H
#define Terminal_USER200_H


void DayAveragePower(u32 Pn,u32 Power,u32 Count);//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
void ERC29_Event_Day(u32 Pn,u64 LastDayEnergy,u64 TodayEnergy);//判断周期为日冻结电量的电能表飞走,超差,入口Pn=测量点号(1开始),LastDayEnergy=上是冻结电量,ToDayEnergy=今日冻结电量


#endif

