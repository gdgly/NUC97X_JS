
//无线GPRS模块
#ifndef GPRS_QCSG_H
#define GPRS_QCSG_H

void GPRS_QCSG_NextATCount(void);					// 下1个AT

void LineATIntRx(u32 ADDR_ATIntRx,u32 ADDR_BUFF,u32 Byte);		// 取AT中断接收数据线性放在缓冲中

void UnInit_GPRS_QCSG(void);

void GPRS_QCSG_RxTx(u32 PORTn);



#endif


