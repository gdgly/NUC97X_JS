

#ifndef Terminal_ReadMeter_Event_H
#define Terminal_ReadMeter_Event_H



void RMEveneTimer(u32 PORTn);//分级读电能表事件定时
u32 NewRMEvene(u32 PORTn);//新或恢复读电能表事件任务;返回0=没新进入,1=新进入
void DelRMEvene(u32 PORTn);//删除读电能表事件任务
void StoreRMEvene(u32 PORTn);//暂存正在读电能表事件的任务
void ClrRMEvene(u32 PORTn);//清读电能表事件
#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
void RMEveneTimer_ShH(u32 PORTn,u32 SuccessRate);//上海集中器分级读电能表事件定时
#endif


#endif
