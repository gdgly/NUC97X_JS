

#ifndef Data_Save_H
#define Data_Save_H

void PowerDown_ISR(void);//电源掉电中断
void PowerDown_Data_Save(void);//电源掉电数据保存
void Battery_Data_Save(void);//电池工作时关机前数据保存
//void Timer_Data_Save(void);//定时数据保存
void PowerUp_Data_Recovery(void);//电源上电时数据恢复
void Power_Down_Display(void);//掉电数据保护监测时间显示(在1mS中断开启前调用,避免1mS中断计数)

#endif

