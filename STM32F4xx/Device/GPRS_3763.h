
//无线GPRS模块
#ifndef GPRS_3763_H
#define GPRS_3763_H
#ifdef __cplusplus
 extern "C" {
#endif



void GPRS_3763_NextATCount(void);//下1个AT
void LineATIntRx(u32 ADDR_ATIntRx,u32 ADDR_BUFF,u32 Byte);//取AT中断接收数据线性放在缓冲中

void UnInit_GPRS_3763(void);//全部GPRS模块引脚为输入无上拉
void GPRS_3763_RxTx(void);//GPRS_3763模块接收发送


#ifdef __cplusplus
 }
#endif
#endif


