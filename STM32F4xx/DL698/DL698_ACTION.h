
#ifndef Terminal698_ACTION_H
#define Terminal698_ACTION_H
#ifdef __cplusplus
 extern "C" {
#endif



void ACTION_Request(u32 PORTn);
u32 ACTION(u32 PORTn,u8 *p8rxbuff,u32 LEN_RxAPDU,u8 *p8txbuff,u32 LENmax_TxSPACE);//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR	 
	 
	 

#ifdef __cplusplus
 }
#endif
#endif
