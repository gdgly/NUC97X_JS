#ifndef _AIRTHMETIC_H
#define _AIRTHMETIC_H

#include <stdint.h>

extern uint8_t ChangeFloatTo376BCD(float data,uint8_t *pdata,uint8_t type,uint8_t type2);
extern uint8_t  ChangeAddress(uint8_t *pdata,uint8_t startposition);
extern uint16_t  Crc_HX(uint8_t * buffer, uint32_t len); //红相表计算校验
extern uint8_t CheckHXTxData(uint8_t *pdata,uint8_t startposition,uint8_t datalength);
extern uint32_t CheckHXRxData(uint8_t *pdata,uint32_t startposition,uint32_t datalength);
extern uint8_t _8HexToBCD(uint8_t pdata);
extern uint8_t _32HexToBCD(uint8_t *pdata,uint8_t *targetdata,uint32_t targetsp); //如果pdata超过100，则只取后面2个数字，百位直接扔掉
extern uint8_t ChangeHxAdd(uint8_t *temp,uint8_t *pdata,uint8_t startposition);
extern uint32_t YMDCalculateWeek(uint32_t RTCAddr);//指定地址缓冲中的年月日计算周


#endif
