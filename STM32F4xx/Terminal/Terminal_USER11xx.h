

#ifndef Terminal_USER11xx_H
#define Terminal_USER11xx_H

u32 USER11xx_RxAddr(u32 PORTn);//吉林系接收帧终端地址比较,4字节长度变为标准帧的2字节;返回:0=地址不同,1=地址相同
void USER11xx_TxAddr(u32 PORTn);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2

#endif
