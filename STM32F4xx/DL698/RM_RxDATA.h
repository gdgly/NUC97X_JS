

#ifndef RM_RxDATA_H
#define RM_RxDATA_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
//UART抄表数据缓冲定义
#define LEN_RM_DI_CSD     1024
	#if LEN_UART2FnDataBuff<8192//UART2(RS485-1)抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
	 #error
	#endif
	#if LEN_UART3FnDataBuff<8192//UART3(RS485-2)抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
	 #error
	#endif
	#if LEN_UART4FnDataBuff<8192//UART4(RS485-4)(载波)抄表Fn数据缓冲长度(注:确保>LEN_RM_DI_CSD+4)
	 #error
	#endif
//抄表数据缓冲1,总长度=LEN_RM_DI_CSD
//0 2byte 长度=时标+通信地址+已采集的CSD(不包括本身2字节)
//2 7byte 采集启动时标  date_time_s(无数据类型)
//9 7byte 采集成功时标  date_time_s(无数据类型)
//16 7byte 采集存储时标  date_time_s(无数据类型)
//23 采集通信地址 octet-string(无数据类型)
//30  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD 2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)

u32 RM_Rx(u32 PORTn);//抄表接收帧;入口:PORTn;返回:0=正在接收,1=完成1帧接收,2=接收超时
void RM_RxDATA_START(u32 PORTn);//抄表接收数据处理-开始
void RM_RxDATA_END(u32 PORTn);//抄表接收数据处理-结束
u32 RM_RxDATA_DL1997_CHECK(u32 PORTn);//DL645_1997帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
u32 RM_RxDATA_DL2007_CHECK(u32 PORTn);//DL645_2007帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
u32 RM_RxDATA_DL698_CHECK(u32 PORTn);//DL698帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
u32 RM_RxDATA_ABB_CHECK(u32 PORTn);//ABB帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
u32 RM_RxDATA_Landis_CHECK(u32 PORTn);//Landis(IEC1107 模式C)帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错

u32 RM_RxDATA_CHECK(u32 PORTn);//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错



u32 RM_Rx_Unknown(u32 PORTn);//未知规约接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
u32 RM_Rx_DL645(u32 PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
u32 RM_Rx_ABB(u32 PORTn);//ABB接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
u32 RM_Rx_Landis(u32 PORTn);//Landis接收;返回:0=正在接收,1=完成1帧接收,2=接收超时

void RM_RxDATA(u32 PORTn);//抄表接收数据处理
u32 RM_RxDATA_DL1997(u32 PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127
u32 RM_RxDATA_DL2007(u32 PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127
u32 RM_RxDATA_DL698(u32 PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127
u32 RM_RxDATA_ABB(u32 PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
u32 RM_RxDATA_Landis(u32 PORTn);//Landis(IEC1107 模式C)抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数


u32 RM_One_CSD(u32 PORTn);//检查抄表是否完成1个完整的CSD的数据;返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127


	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
