
#ifndef Terminal_ReadMeter_RS485_H
#define Terminal_ReadMeter_RS485_H



void ReadMeter_RS485(u32 PROTn);//RS485抄表
void ClrRMTaskCompletes_RS485(u32 PORTn,u32 RMTask,u32 All);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部


#endif
