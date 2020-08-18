

#ifndef Terminal_Init_H
#define Terminal_Init_H
#ifdef __cplusplus
 extern "C" {
#endif



void Init_Terminal(void);//启动初始化
void Init_Terminal_SoftwareUpdate(void);//软件更新初始化
void FLASHtoSDRAM(void);//参数FLASHtoSDRAM
void DL698_INIT(u32 Command);//初始化,入口Command:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化


#ifdef __cplusplus
 }
#endif
#endif
