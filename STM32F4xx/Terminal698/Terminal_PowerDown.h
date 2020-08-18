

#ifndef PowerDown_H
#define PowerDown_H
#ifdef __cplusplus
 extern "C" {
#endif





void RM_Meter_Event3106(void);//停/上电事件需抄读电表的配置序号(从0开始)
void Terminal_PowerDown(void);//2=电网供电转电池供电
void Event_Power(void);//终端停/上电事件
	 
void PowerEventTimerComp(u8* pEVENT);//停上电事件时间比较,入口:pEVENT指向698格式事件的开始,出口:PowerEvent->bit






#ifdef __cplusplus
}
#endif
#endif


