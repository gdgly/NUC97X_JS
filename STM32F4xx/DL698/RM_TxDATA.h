

#ifndef RM_TxDATA_H
#define RM_TxDATA_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 RM_TxDATA_DL1997(u32 PORTn,u8 *p8CSD,u8 *p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
u32 RM_TxDATA_DL2007_DI(u32 PORTn,u32 DI);//抄表发送帧DI按上n次更改
u32 RM_TxDATA_DL2007(u32 PORTn,u8 *p8CSD,u8 *p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
u32 RM_SECURITY(u32 OI);//抄表安全模式字,入口:OI,出口:(同安全请求)b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
u32 RM_TxDATA_ABB(u32 PORTn,u8 *p8CSD,u8 *p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
u32 RM_TxDATA_Landis(u32 PORTn,u8 *p8CSD,u8 *p8tx);//Landis(IEC1107 模式C)抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
	 
u32 RM_TxDATA(u32 PORTn,u8 *p8CSD,u8 *p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
u32 RM_TxDATA_Manage(u32 PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停

u32 BroadcastTime_TxDATA_DL645(u8 *p8tx);//DL645广播校时发送帧;返回:组帧后总长度字节数
u32 BroadcastTime_TxDATA_DL698(u8 *p8tx);//DL698广播校时发送帧;返回:组帧后总长度字节数

void DL645_68H_Add33H_CS_16H(u8* p8tx);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16 
	 

#ifdef __cplusplus
 }
#endif
#endif
