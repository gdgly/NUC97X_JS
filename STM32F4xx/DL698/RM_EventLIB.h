

#ifndef RM_EventLIB_H
#define RM_EventLIB_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 Get_Event645_DI(u32 OAD,u32 index);//获取当前的抄645表的数据项，OAD=事件OAD，index=当前OAD对应的第几个数据表示，从0开始，返回0=没有要抄读的数据标识
u32 Change_Frame_645_to_698(u32 PORTn);//把645的事件数据帧转成698格式
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
