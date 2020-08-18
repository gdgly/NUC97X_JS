
#ifndef Terminal698_REPORT_H
#define Terminal698_REPORT_H
#ifdef __cplusplus
 extern "C" {
#endif

void REPORT_PORTn(msFILE_TypeDef *file);//通道不需上报时置已上报数据个数=当前数据个数；使用ADDR_DATABUFF
void REPORT_Notification(u32 PORTn);//上报通知(定时任务)
void EVENT_REPORT_Notification(u32 PORTn);//上报通知(事件)
void REPORT_Response(u32 PORTn);//上报应答
	 
	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
