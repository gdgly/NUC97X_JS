
#ifndef DL698_GET_4100_H
#define DL698_GET_4100_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 Get_MeterSpec(void);//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
u32 GET_Request_4100(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR




#ifdef __cplusplus
 }
#endif
#endif
