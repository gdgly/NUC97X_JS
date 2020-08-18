

#ifndef Terminal698_Uart_H
#define Terminal698_Uart_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 Uart_698_HCS(u8 *p8);//帧头校验计算;返回0=正确，1=错误
u32 Uart_698_HCSFCS(u8 *p8);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
u32 FrameProtocol(u8* p8,u32 Len);//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004
u32 Uart_698_Rx(u32 PORTn);//UART_698接收;返回0没完成,1完成,2超时
void Uart_698_RxTx(u32 PORTn);//UART_698接收发送 
void Uart_698_Data(u32 PORTn,u32 SECURITY);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用

#define Rxoffset     64//原接收帧在接收缓冲区的偏移字节
#define TxAPDUOffset    32//发送APDU在发送缓冲区的偏移字节
u32 Uart_TSA_Comp(u8 *p8rxbuff,u8 *pFLASH);//通信地址比较,pFLASH数据读到ADDR_128K_DDATABUFF中比较,返回:0=相同,!=0不相同
u32 Uart_698_TxHead(u8 *p8txbuff,u32 C,u32 CA);//Uart698帧头填入发送缓冲区,入口:C=控制域,CA=客户机地址;返回:帧头长度
void Uart_698_TxResponse(u32 PORTn,u32 TI,u32 LEN_TxAPDU);//DL698发送响应数据,数据已在p8txbuff+TxAPDUOffset的开始地址中
u32 Uart_TimeTag(u8* p8rx);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
