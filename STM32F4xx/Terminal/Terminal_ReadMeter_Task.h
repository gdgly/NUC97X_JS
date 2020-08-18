

#ifndef Terminal_ReadMeter_Task_H


void ClrRMTaskCompletePn(u32 Pn);//清指定Pn的抄表任务完成标志;入口:Pn
void ClrRMTaskCompletes(u32 PORTn,u32 RMTask);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号



#endif

