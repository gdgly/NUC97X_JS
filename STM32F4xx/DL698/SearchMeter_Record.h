

#ifndef SearchMeter_Record_H
#define SearchMeter_Record_H
#ifdef __cplusplus
 extern "C" {
#endif


void DEL_SearchMeter_Record(u32 FileID);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
void SearchMeter_Record(u32 PORTn,u32 FileID,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
void SearchMeter_Record_JS(u64 CollectorAddr,u32 NoteNum,u8 *p8rx);
void SearchMeter_CollectorAddrRecord_JS(u64 CollectorAddr);
//void SearchMeter_Record_JS_ONE(u64 CollectorAddr,u64 NoteNumAddr);
	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
