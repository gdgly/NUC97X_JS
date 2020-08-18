

#ifndef NUC97x_MAC_H
#define NUC97x_MAC_H
#ifdef __cplusplus
 extern "C" {
#endif

void UnInit_NUC97x_MAC0(void);
void Init_NUC97x_MAC0(void);
void UnInit_NUC97x_MAC1(void);
void Init_NUC97x_MAC1(void);
void UnInit_NUC97x_MAC(void);
void Init_NUC97x_MAC(void);
	 
void ETH0_MAC_Config(void);//MAC0配置,在LINK后配置,读取PYH状态后按相应值配置
void ETH1_MAC_Config(void);//MAC1配置,在LINK后配置,读取PYH状态后按相应值配置 
void ETH_MAC_Config(void);//MAC配置,在LINK后配置,读取PYH状态后按相应值配置 

	 
void ETH_DMA_Config(u32 DMATDLA,u32 DMARDLA);//DMA配置;入口:DMATDLA=发送描述符列表地址,DMARDLA=接收描述符列表地址
void ETH_DMATxDescList(u32 DMATDLA,u32 ADDR_TxBuff1,u32 ADDR_TxBuff2,u32 Byte,u32 FS,u32 LS);//ETH发送描述符表并启动发送
	//入口:DMATDLA=发送描述符列表地址,TxBuff1=发送缓冲区地址,TxBuff2=下描述符地址,Byte=发送字节,FS=1(首段),LS=1(末段)
void ETH_DMARxDescList(u32 DMARDLA,u32 ADDR_RxBuff1,u32 ADDR_RxBuff2,u32 LEN_Buff,u32 FS,u32 LS);//ETH接收描述符表并启动接收
	//入口:DMARDLA=接收描述符列表地址,RxBuff1=接收缓冲区地址,RxBuff2=下描述符地址,LEN_Buff=接收缓冲区长度,FS=1(首段),LS=1(末段)


#ifdef __cplusplus
 }
#endif
#endif




