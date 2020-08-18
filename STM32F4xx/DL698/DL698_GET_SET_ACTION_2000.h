

#ifndef DL698_GET_SET_ACTION_2000_H
#define DL698_GET_SET_ACTION_2000_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 GET_23xx_24xx(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//读总加组和脉冲计量;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
u32 SET_23xx_24xx(u32 PORTn,u8* p8rx,u32 LEN_RxAPDU);//设置总加组和脉冲计量;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
u32 ACTION_23xx_24xx(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//方法总加组和脉冲计量;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
	 
	 

#ifdef __cplusplus
 }
#endif
#endif




