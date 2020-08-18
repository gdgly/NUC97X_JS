

#ifndef RM_TASK_H
#define RM_TASK_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 RM_MeterCount(u32 PORTn);//电能表计数+1;返回:0=正常,1=任务错
u32 RM_Addr(u32 PORTn,u16* pAddr);//指针指向的通信是否已抄;返回:0=没抄,1=已抄
u32 RM_Meter(u32 PORTn);//当前电能表计数是否要抄;入口:UARTCtrl->MeterCount[],UARTCtrl->TaskID;返回:0=不抄,1=要抄,2=完成,同时填写通信地址,波特率,抄表规约类型
u8 *RM_CSD(u32 PORTn);//取抄表数据标识CSD;返回:0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
	 
//p16=(u16*)(ADDR_6000_SDRAM+(MeterSetNo*LENper_6000)+8);//取采集档案配置序号的通信地址
u16* MeterSetNoToCollectorAddr(u32 MeterSetNo);//取采集档案配置序号的采集器地址;入口:从0开始的序号;返回:采集器地址指针,0=无效
void NUMrm(u32 PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
void RM_OK_FAIL(u32 PORTn,u32 OK);//抄表成功失败地址记录:入口:OK=0失败，=1成功,2=成功但数据为空
u32 DateSaveTI(u32 PORTn,u8* pOUT);//普通采集方案的采集存储时标;返回:时标类型1=任务开始时间,2=相对当日0点0分,3=...,6=;pOUT=date_time_s
void RM_UpDate(void);//抄表参数更新;出口:判断,重读采集档案配置表从flash到sdram,置位各PORTn各任务需重计算总需抄表数成功数等


void RM_ADDR_Display(u32 PORTn);//抄表地址显示
void RM_NUM_Display(u32 PORTn);//抄表数量显示
void RM_Monitor_Init(u32 ID);//采集任务监控初始化;入口:ID=任务号
void RM_Monitor_Init_PORTn(u32 PORTn,u32 ID);//采集任务监控初始化(按端口号);入口:PORTn=端口号,ID=任务号
void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数,NUM_Rx=接收条数
void RM_Monitor_TxNUM(u32 ID,u32 NUM_Tx);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数
void RM_Monitor_RxNUM(u32 ID,u32 NUM_Rx);//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
	 
void RM_State_SetUpDate(void);//采集状态,档案变更时更新(对应配置序号)
void RM_State_RMUpDate(u32 PORTn,u32 OK,u32 SetNo,u32 Class,u32 Phase);//采集状态,每次抄表后更新:入口:OK=0失败=1成功,OK=1时Class=中继级别,Phase=相位
//void RM_State_UpDate(u32 PORTn,u32 SetNo,u32 Phase,u32 Err);//采集状态更新:入口:SetNo=配置号,Phase=相位,Err=线路异常
//void RM_State_ReadRouterAllUpDate(u32 PORTn);//采集状态,读全部路由信息更新
	 
	 
void PROXY_ADDR_Display(u32 PORTn);//代理地址显示
	 
	
u32 Get_SETINIT_601E_List_NUM(void);//得到采集规则库初始设置库列表个数	



#ifdef __cplusplus
}
#endif
#endif



