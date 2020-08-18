

#ifndef ATT7022E_H
#define ATT7022E_H



void EMURW(void);//读写EMU(自动任务0或任务1)



u32 EMUReadWait(u32 Addr);//读EMU(直至本次读操作完成,仅用于校验时读)
void EMUWriteWait(u32 Addr,u32 Data);//写EMU(直至本次写操作完成,仅用于校验时写)



#endif
