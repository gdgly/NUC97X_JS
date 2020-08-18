

#ifndef Terminal698_Set_H
#define Terminal698_Set_H
#ifdef __cplusplus
 extern "C" {
#endif


void SET_Request(u32 PORTn);
u32 SET_OAD_Element(u32 PORTn,u8 *p8rx,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD);//设置一个对象元素;返回:本元素的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
u32 SET_OAD(u32 PORTn,u8 *p8rxbuff,u32 LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR


#ifdef __cplusplus
 }
#endif
#endif
