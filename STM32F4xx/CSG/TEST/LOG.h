

#ifndef LOG_H
#define LOG_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
void Init_LOG(void);//LOG上电初始化(在FS初始化后进行)
void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
void LOG_ATRx(u32 link);//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
void LOG_FILE(u32 protocol,u32 len);//LOG nand和ADDR_128K_DATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128K_DATABUFF中长度(当无此数据时用0)

#ifdef __cplusplus
 }
#endif 
#endif



