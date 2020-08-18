

#ifndef DL698_EventLIB_H
#define DL698_EventLIB_H
#ifdef __cplusplus
 extern "C" {
#endif





DL698_EVENTLIB_TypeDef *Get_DL698_EVENT_pLib(u32 OAD);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
void DL698_SETINIT_EVENT(u32 Command,u8* pOAD);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化



#ifdef __cplusplus
 }
#endif
#endif


