

#ifndef EVENT_Record_H
#define EVENT_Record_H
#ifdef __cplusplus
 extern "C" {
#endif

void RM_MeterEventAutoInc0x20220200(u32 PORTn);//电能表事件抄读列选择若无发生次数(事件记录序号)则自动加入
u32 Get_RxDATA_DL698_MeterEventCount(u32 PORTn);//从698读事件记录接收帧中取出电能表事件发生次数(事件记录序号);返回:发生次数,没找到时返加0xffffffff
u32 CompSourMeterEventCount(u32 SetNo,u32 OAD,u32 Count);//比较原电能表事件次数;入口:SetNo=档案配置号(0开始),Count=当前次数;返回:0=初始值,1=次数相同,2=次数不同,同时修改原计数,使用ADDR_DATABUFF

u32 GET_EventFileName(u32 OAD,u32 FileCount);//搜索事件记录文件名:入口:OAD,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
u32 DEPTH_EventFile(u32 OI);//事件文件存储深度;返回:存储深度
u32 GET_EventRecord_RCSD(u32 OAD,u8 *pRCSD,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
u32 GET_EventRecord(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE,u32 ALL);//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR 
u32 GET_LastEventOccurRecovery(u32 OAD,u8* pTSA);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
u32 GET_LastEventTime_OccurRecovery(u32 OAD,u8* pTSA,u8* pOut,u32 OccurRecovery);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
u32 GET_LastEventTime_Occur(u32 OAD,u8* pTSA,u8* pOut);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
u32 GET_LastEventTime_Recovery(u32 OAD,u8* pTSA,u8* pOut);//得到最近次事件恢复时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut


EventSour_typedef* GET_EventSour(u16* pRMAddr);//得到事件原值数据指针，入口:抄表地址, 返回:数据指针,0=错误
void CLR_EventRecord(u32 OI);//清相应OI的事件记录
	
	
	
void Event_OAD(u32 OAD,u8* pOccurSour,u32 Occur,u32 OccurkWh);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF

void Event_30300200(u8* pDATA, u8* pOccurSour,u32 FramLen);//模块ID变更事件记录;入口:pDATA=模块ID变更属性;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	 
void Event_31050200(u8* pOccurSour,u32 Occur,u8*p1);//电能表时钟超差记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
void Event_31060200(u8* pOccurSour,u32 Occur);//终端停/上电事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF	 

void Event_31110200(u8* pDATA);//发现未知电能表事件记录;入口:pDATA=1个搜表结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
void Event_31120200(u8* pDATA);//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	 
	 
void Event_31150200(u8* pOAD,u32 Occur);//遥控事件记录(控后2分钟),入口:pOAD=事件发生源OAD（继电器单元）,occur=0恢复=1产生=2无	 
 
	 
void Event_31180200(u8* pOAD);//终端编程事件记录;入口:pOAD=设置的OAD数组指针;使用ADDR_DATABUFF和ADDR_128KDATABUFF	 
	 
void Event_311C0200(u32 PORTn,u8* pCSD,u8* pDATA1,u8* pDATA2);//电能表数据变更监控记录,入口:pCSD=监控的CSD,pDATA1=变更前数据,pDATA2=变更后数据	 
 
void Event_32000200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst,u8* pAfter);//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
void Event_32010200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst);//电控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
void Event_32030200(u32 pOccurSour,u32 OI,u8* pFixedValue);//电控告警事件记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF

void Event_310A0200(u8* pOccurSour,u32 Occur,u8* pTSA);

void Event_311F0200(u8* pDATA,u8* pOccurSour,u32 FramLen);//拒绝从节点入网记录:入口:pDATA=1个拒绝从节点入网记录单元;使用ADDR_DATABUFF和ADDR_128KDATABUFF

#if (USER/100)==9//河南
void Event_34110200(u8* pDATA);//电表接线错误事件记录;入口:pDATA=1个错误结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF	 
#endif	 
void Event_31010200(void);//终端版本变更事件	 
	 
#ifdef __cplusplus
 }
#endif
#endif
