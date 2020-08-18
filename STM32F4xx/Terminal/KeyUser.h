

#ifndef PnMap_H
#define PnMap_H


//重点户2类数据曲线Fn对应要抄读1类实时数据Fn库定义
typedef __packed struct
{
  vu8 AFN0DFn;//重点户曲线Fn
  vu8 AFN0CFn;//要抄读1类实时数据Fn,为0表示无效不抄
}KeyUserReadMeterLib_TypeDef;
u32 Get_KeyUserReadMeterLib_MaxFn(void);


void KeyUserPnList(void);//集中抄表重点户曲线冻结Pn列表
u32 KeyUserMapPn(u32 Pn);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
u32 KeyUserPnSet(u32 Pn);//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList




#endif
