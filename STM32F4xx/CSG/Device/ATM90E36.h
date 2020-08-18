

#ifndef ATM90E36_H
#define ATM90E36_H



//-------------------------------------------------------------------------------------------------
// Function name:       EMURW
// Descriptions:        计量芯片EMU读写
// input parameters:    有2个相同任务,任务0系统占用,非系统软件只能使用任务1
                      //SSP1Task1:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
                      //SSP1Addr1读写数据地址1
                      //SSP1Data1读写数据1
// output parameters:   SSP1Task1,SSP1Data1
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void EMURW(void);//读写EMU(自动任务0或任务1)



u32 EMUReadWait(u32 Addr);//读EMU(直至本次读操作完成,仅用于校验时读)
void EMUWriteWait(u32 Addr,u32 Data);//写EMU(直至本次写操作完成,仅用于校验时写)
void EMURead(u32 Addr1,u32 Addr2);//读EMU多个数据,SSP1Task1Count=n表示读n个完成


#endif


























