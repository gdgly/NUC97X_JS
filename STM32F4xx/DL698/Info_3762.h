


#ifndef Info_3762_H
#define Info_3762_H
#ifdef __cplusplus
 extern "C" {
#endif


u8* UART_3762_AFNxFn(u32 AFN,u32 Fn);//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口: AFN,Fn;返回:数据单元指针 
void UART_3762_AFN10F21(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//查询网络拓扑信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
void UART_3762_AFN10F31(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//查询相位信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
void UART_3762_AFN03F101(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff);//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
void UART_3762_AFN5F6(u32 PORTn,u32 EnableFlag);//网络拓扑识别开关 0：关闭 1：打开
void UART_3762_AFN5F16(u32 PORTn,u8* data);// 设置网络通讯频段
void UART_3762_AFN5F200(u32 PORTn,u8* data);//拒绝节点上报使能标志：禁止（0），使能（1）

void READ_Info(void);//宽带载波按F209_6设置的TI周期读网络拓扑信息,多网络信息等(不需要周期读取时可设置读取的TI周期=0)
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
