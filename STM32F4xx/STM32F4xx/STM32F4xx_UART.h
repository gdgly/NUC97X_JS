
//UART驱动
#ifndef STM32F4xx_UART_H
#define STM32F4xx_UART_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 Get_ADDR_UARTn(u32 PORTn);//得到UART缓冲首地址
u32 Get_LEN_UARTnIntRx(u32 PORTn);//得到UART中断接收缓冲长度
u32 Get_LEN_UARTnCtrl(u32 PORTn);//得到UART控制数据长度
u32 Get_LEN_UARTnRx(u32 PORTn);//得到UART接收缓冲长度
u32 Get_LEN_UARTnTx(u32 PORTn);//得到UART发送缓冲长度
u32 Get_LEN_UARTnTxMax(u32 PORTn);//得到UART发送最大缓冲长度
u32 Get_LEN_UARTnFnDataBuff(u32 PORTn);//得到UART抄表数据缓冲长度
u32 Get_ADDR_UARTnTimer(u32 PORTn);//得到UART定时器地址
u32 Get_ADDR_UARTnCtrl(u32 PORTn);//得到UART控制区地址
u32 Get_ADDR_UARTnRx(u32 PORTn);//得到UART接收缓冲地址
u32 Get_ADDR_UARTnTx(u32 PORTn);//得到UART发送缓冲地址
u32 Get_ADDR_UARTnFnDataBuff(u32 PORTn);//得到UART抄表数据缓冲地址
	#if MainProtocol==698
u32 Get_ADDR_UARTnTimer_S(u32 PORTn);//得到UART秒定时器地址
	#endif
	 
u32 Get_UARTBps(u32 PORTn);//得到UART通信速率



void Init_UARTn(u32 PORTn);//初始化UART口,入口通信控制字已填入
void Start_UARTn_Tx(u32 PORTn);//启动UART口发送
u32 Wait_UARTn_Tx(u32 PORTn);//等待UART口发送,返回0=发送完成,1=发送没完成

u32 FeatureWord_CtrlWord(u32 word);//DL645-07特征字转为通信速率控制用控制字
void PrintString_UARTn(u32 PORTn,u8 * pStr);//UART打印字符串



#ifdef __cplusplus
 }
#endif 
#endif
