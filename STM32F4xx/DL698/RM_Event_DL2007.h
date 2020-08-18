

#ifndef RM_Event_DL2007_H
#define RM_Event_DL2007_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
u32 Get_Event645_DI(u32 OAD,u32 RMCount);//得到DL645事件抄读的DI，入口:OAD=事件OAD，RMCount=抄表命令计数(从0开始)，返回:抄读的DI(0=没有要抄读的数据标识)
u32 Get_RxDATA_DL2007_MeterEventCount(u32 PORTn);//从645读事件记录接收帧中取出电能表事件发生次数(事件记录序号);返回:hex发生次数,没找到时返回0xffffffff
u32 Change_Frame_645_to_698(u32 PORTn);//把645的事件数据帧转成698格式
	 
	 
 
	 
#ifdef __cplusplus
 }
#endif
#endif
