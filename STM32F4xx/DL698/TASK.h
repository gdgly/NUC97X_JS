

#ifndef TASK_H
#define TASK_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
u32 Check_OtherTask(u32 PORTn);//检查是否有其他任务；返回:0=没,!=0有
void TASK(u32 PORTn,u32 RM);//任务;入口:RM=0上报任务,RM=1采集任务

	
	
	
#ifdef __cplusplus
 }
#endif
#endif
