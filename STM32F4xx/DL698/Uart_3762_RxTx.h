

#ifndef Uart_3762_RxTx_H
#define Uart_3762_RxTx_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 UART_3762_Rx(u32 PORTn);//UART 3762接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
u32 Uart_3762_RxTx(u32 PORTn);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收;返回:0=正常返回,1=代理返回
u32 Uart_3762_CS(u32 ADDR_BUFF);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
void Uart_3762_TxData(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址

void Uart_3762_AFN13F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
u32 Uart_3762_AFN13F1_Rx(u32 PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错

u32 Get_Uart_3762_RxData_AFN_Fn(u32 PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
u32 Uart_3762_RxData_AFN00_F1(u32 PORTn,u32 AFN,u32 Fn,u32 LEN_Data);//UART_3762接收AFN00F1确认数据,入口AFN,Fn,LEN_Data接收失败重发用参数;返回0未确认1确认2失败
void Uart_3762_RxData(u32 PORTn);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收

	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif

