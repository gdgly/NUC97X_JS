
#ifndef DL698_GET_6000_H
#define DL698_GET_6000_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 GET_NUMper(DL698_SETLIB_TypeDef *pLib);//计算当前元素个数
u32 GET_Request_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
u32 GET_Request_6002(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
	 



#ifdef __cplusplus
 }
#endif
#endif
