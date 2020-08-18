

#ifndef MeterAutoReport_H
#define MeterAutoReport_H
#ifdef __cplusplus
 extern "C" {
#endif

typedef __packed struct
{
	u8 BIT;//对应DL645-2007主动上报状态字中的位B0-B95
	u32 OAD;//对应事件的OAD
	u32 DI_NUM;//对应DL645-2007事件发生次数的抄读标识,0=空
	u32 DI_TIME;//对应DL645-2007事件累计时间的抄读标识,0=空
	u32 DI_LASTEVENT1;//对应DL645-2007最近1次事件的抄读标识1,0=空
	u32 DI_LASTEVENT2;//对应DL645-2007最近1次事件的抄读标识2,0=空
}MeterAutoReportLIB_typedef;

u32 MeterAutoReportStateWord(u32 PORTn,u8* p8rx);//主动上报状态字;入口:自动上报接收帧;返回:0=没接受,1=接受
void MeterAutoReport_RS485(u32 PORTn);//电能表主动上报任务-rs485:0=空,1=有主动上报等待读取,2...抄读中
void MeterAutoReport_3762(u32 PORTn);//电能表主动上报任务-3762:0=空,1=有主动上报等待读取,2...抄读中



#ifdef __cplusplus
 }
#endif
#endif
