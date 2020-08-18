
#ifndef MeterAutoReport_Record_H
#define MeterAutoReport_Record_H
#ifdef __cplusplus
 extern "C" {
#endif


void MeterAutoReport_Record(u32 PORTn,u8* pDATA,u32 FileID);//电能表主动上报记录文件;入口:pDATA=数据指针(分别为RecordFileHead+时标+帧个数和长度+数据)
void MeterAutoReport_3011(u8* p8rx,u32 Occur);//载波口上报从节点事件(电能表停电事件);入口:pAddr=从路由模块接收到的接收缓冲中的6字节地址(电表或采集器地址)

	 

#ifdef __cplusplus
 }
#endif
#endif
