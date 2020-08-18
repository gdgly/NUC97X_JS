

#ifndef ATString_H
#define ATString_H


u32 ATRxSearch(u32 ADDR_String);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
u32 GetATRxString(u32 ADDR_StrBuff);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
u32 GetATRxByte(void);//中断接收,计算当前p0对应p1还有未处理字节数
void GPRS_Signal(u32 Addr);//GPRS信号强度0-4


#endif
