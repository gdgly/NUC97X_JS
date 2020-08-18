

#ifndef Terminal_ReadMeter_3762_H
#define Terminal_ReadMeter_3762_H

void DisplayRouterVer(u32 PORTn);//显示路由厂家和版本号
void Terminal_ReadMeter_3762(u32 PORTn);//终端抄表Q/GDW376.2
void Terminal_RouterSyn(u32 PORTn);//路由器档案同步
void Terminal_RouterCtrl(u32 PORTn);//路由器控制
void RouterBroadcastTime(u32 PORTn);//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
void Terminal_RouterInfo(u32 PORTn);//读路由信息,入口:要读的测量点号已在Terminal_Router->RouterInfoPn,置Terminal_Router->RouterInfo=3=命令读

void ClrRMTaskComplete_3762(u32 RMTask,u32 All);//清端口抄表任务完成标志
#endif


