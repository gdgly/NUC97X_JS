

#ifndef Terminal698_GET_H
#define Terminal698_GET_H
#ifdef __cplusplus
 extern "C" {
#endif


void GET_Request(u32 PORTn);
void GET_Request_Frame(u32 PORTn);//分帧标志	
u32 GET_OAD_Element(u32 PORTn,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD,u8 *p8tx,u32 LENmax_TxSPACE);//读一个对象元素;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
u32 GET_OAD_DATA(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR



u32 GET_RecordOAD(u32 PORTn,u8 *p8rx);//读一个记录型对象属性;返回:b31-b16接收ROAD长度,b15-b0发送ROAD及数据字节数,数据访问结果UARTCtrl->DAR
u32 GET_RecordOAD_Next(u32 PORTn);//读一个记录型对象属性分帧响应的下一帧;返回：LEN_TxAPDU
	 

	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
