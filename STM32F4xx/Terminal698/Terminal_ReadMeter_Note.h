
#ifndef Terminal_ReadMeter_Note_H
#define Terminal_ReadMeter_Note_H


void ReadMeterNote_OK(u32 PORTn,u32 Pn,u32 Info);//集中抄表电表抄读信息.成功
void ReadMeterNote_Fail(u32 PORTn,u32 Pn);//集中抄表电表抄读信息.失败
void DateRMFlags(u32 Pn,u32 f);//日抄表标志;入口:f=0成功,=1失败


#endif
