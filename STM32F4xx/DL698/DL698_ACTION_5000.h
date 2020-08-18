


#ifndef DL698_ACTION_5000_H
#define DL698_ACTION_5000_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 ACTION_5000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
	 
	 
	 

	 

#ifdef __cplusplus
 }
#endif
#endif


































