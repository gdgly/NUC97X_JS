
#ifndef RM_3762_H
#define RM_3762_H
#ifdef __cplusplus
 extern "C" {
#endif




void RM_3762(u32 PORTn);//3762抄表
void Terminal_RouterSyn(u32 PORTn);//路由器档案同步
void Terminal_RouterCtrl(u32 PORTn);//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0





#ifdef __cplusplus
 }
#endif
#endif
