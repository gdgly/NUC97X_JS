

#ifndef SearchMeter_Record_H
#define SearchMeter_Record_H
#ifdef __cplusplus
 extern "C" {
#endif


void DEL_SearchMeter_Record(u32 FileID);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
void SearchMeter_Record(u32 PORTn,u32 FileID,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase);//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
void SearchMeter_Record_JS(u64 CollectorAddr,u32 NoteNum,u8 *p8rx);
void SearchMeter_CollectorAddrRecord_JS(u64 CollectorAddr);
//void SearchMeter_Record_JS_ONE(u64 CollectorAddr,u64 NoteNumAddr);
	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
