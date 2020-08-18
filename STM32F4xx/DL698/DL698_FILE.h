


#ifndef DL698_FILE_H
#define DL698_FILE_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 GET_Request_F001(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
u32 ACTION_F001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
u32 ACTION_F002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
	 

	 
	 

#ifdef __cplusplus
 }
#endif
#endif
