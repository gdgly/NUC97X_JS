

#ifndef FLASH_class8_H
#define FLASH_class8_H
#ifdef __cplusplus
 extern "C" {
#endif

// 公共数据-----------------------------------------------------------------------------------------------
#define ADDR_Com_EFLASH_Start     	ADDR_QCSG_SET_START
#define ADDR_RTCOffice     			ADDR_Com_EFLASH_Start	//终端时钟相对仪表时钟差值(仅网络表时用)
// 4 BYTE HEX补码,秒
#define ADDR_Language     			ADDR_RTCOffice+4		//显示语言同步RAM中的Comm_Ram->Language
// 1 BYTE 0=中文,1=英文,2=
// 3 BYTE 保留
#define ADDR_FileDown     			ADDR_Language+4			//自定义软件下载
//软件下载1
// 1 BYTE 信道0=RS232,1=GPRS或RJ45
// 1 BYTE 保留
// 1 BYTE 文件传输控制	BIN	1
// 1 BYTE 文件编号(0-255)	BIN	1
// 16 BYTE 文件名称	ASCII	16
// 2 BYTE 文件总段数n(n>=1)	BIN	2
// 2 BYTE 文件数据当前段数号（i=1~n）	BIN	2
// 2 BYTE 文件每段字节长度(L)	BIN	2
// 4 BYTE 已下载总字节数
// 2 BYTE 保留
// 软件下载2
// ....
// 软件下载8
// ...
#define ADDR_LEN_AFN0FF1    		ADDR_FileDown+(32*8)//376.1 F1下载文件的总长度计算后寄存
// 4byte
#define ADDR_AFN0FF1     			ADDR_LEN_AFN0FF1+4//376.1 F1下载
// 1BYTE 文件标识:00H清除下装文件;01H-06H升级文件
// 1BYTE 文件属性：起始、中间帧：00H；结束帧：01H。
// 1BYTE 文件指令：00H：报文方式下装；01H：FTP 方式下装；02H：启动组地址升级。
// 2BYTE 总段数n
// 4BYTE 第i 段标识或偏移（i=0～n）
// 2BYTE 第i 段数据长度Lf
// xBYTE 文件数据
#define LEN_AFN0FF1     			256
#define ADDR_AFN0CF14     			ADDR_AFN0FF1+LEN_AFN0FF1//文件下装未收到数据段
// 4BYTE 组号 BIN 4
// 128BYTE 组内各数据段未收到标志 BS8 128
#define ADDR_FTPFileFlags     		ADDR_AFN0CF14+4+128//FTP文件下载标志
// 4BYTE 空
// 1BYTE 下载文件计数;0-x,=MaxFileNum关FTP,=(MaxFileNum+1)返回帧,=(MaxFileNum+2)下载完成显示,=(MaxFileNum+3)结束下载显示
// 1BYTE MS文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE OS文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE Meter文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE Terminal文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE Menu文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE Font文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE Module文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 1BYTE 文件 :0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
// 8BYTE 
#define ADDR_AFN0FF1_Rx     		ADDR_FTPFileFlags+5+16		//FTP接收帧的前256字节保存
// 256BYTE
#define ADDR_AFN0FF1_Tx     		ADDR_AFN0FF1_Rx+256			//FTP返回帧的前18字节保存
// 18BYTE
#define ADDR_UART_PORT_SET     		ADDR_AFN0FF1_Tx+18		//通信口设置
// UART0 (RS232)
// 1BYTE 功能 0=终端监视口,!=0终端维护口
// 1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE 保留
// 1BYTE 保留
// UART2(RS485-1)
// 1BYTE 功能 0=终端维护口,!=0终端抄表口
// 1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE 保留
// 1BYTE 保留
// UART3(RS485-2)
// 1BYTE功能 0=终端维护口,!=0终端抄表口
// 1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE 保留
// 1BYTE 保留
// UART4(RS485-3)
// 1BYTE 功能 0=终端维护口,!=0终端抄表口
// 1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE 保留
// 1BYTE 保留
// UART5(红外)
// 1BYTE 功能 0=终端维护口,!=0终端抄表口
// 1BYTE 初始速率 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE 保留
// 1BYTE 保留
#define LEN_UART_PORT_SET     	4

// 终端运行时数据

#define ADDR_E1800001			(ADDR_UART_PORT_SET+(LEN_UART_PORT_SET*5))
#define LEN_E1800001			(2)
#define ADDR_E1800002			(ADDR_E1800001 + LEN_E1800001)
#define LEN_E1800002			(1)
#define ADDR_E1800003			(ADDR_E1800002 + LEN_E1800002)
#define LEN_E1800003			(1)
#define ADDR_E1800004			(ADDR_E1800003 + LEN_E1800003)
#define LEN_E1800004			(2)
#define ADDR_E1800005			(ADDR_E1800004 + LEN_E1800004)
#define LEN_E1800005			(2)
#define ADDR_E1800006			(ADDR_E1800005 + LEN_E1800005)
#define LEN_E1800006			(2)
#define ADDR_E1800007			(ADDR_E1800006 + LEN_E1800006)
#define LEN_E1800007			(2)
#define ADDR_E1800008			(ADDR_E1800007 + LEN_E1800007)
#define LEN_E1800008			(2)
#define ADDR_E1800009			(ADDR_E1800008 + LEN_E1800008)
#define LEN_E1800009			(16)

#define ADDR_E1800010			(ADDR_E1800009 + LEN_E1800009)
#define LEN_E1800010			(3)
#define ADDR_E1800011			(ADDR_E1800010 + LEN_E1800010)
#define LEN_E1800011			(2)
#define ADDR_E1800012			(ADDR_E1800011 + LEN_E1800011)
#define LEN_E1800012			(2)
#define ADDR_E1800013			(ADDR_E1800012 + LEN_E1800012)
#define LEN_E1800013			(3)
#define ADDR_E1800014			(ADDR_E1800013 + LEN_E1800013)
#define LEN_E1800014			(3)
#define ADDR_E1800015			(ADDR_E1800014 + LEN_E1800014)
#define LEN_E1800015			(1)
#define ADDR_E1800016			(ADDR_E1800015 + LEN_E1800015)
#define LEN_E1800016			(2)
#define ADDR_E1800017			(ADDR_E1800016 + LEN_E1800016)
#define LEN_E1800017			(2)
#define ADDR_E1800018			(ADDR_E1800017 + LEN_E1800017)
#define LEN_E1800018			(9)
#define ADDR_E1800020			(ADDR_E1800018 + LEN_E1800018)
#define LEN_E1800020			(40)

#define ADDR_GUIPASSWORD		(ADDR_E1800020 + LEN_E1800020)
#define LEN_GUIPASSWORD			(4)
#define ADDR_TerminalResetCnt	(ADDR_GUIPASSWORD + LEN_GUIPASSWORD)
#define LEN_TerminalResetCnt	(4)

#define ADDR_Com_EFLASH_BASEEnd     (ADDR_Com_EFLASH_Start + 128*1024)


#define LEN_Com_EFLASH     			(128*1024)	//限定总长度字节数(1个扇区,128K)
#define ADDR_Com_EFLASH_End     	ADDR_Com_EFLASH_BASEEnd

#if (ADDR_Com_EFLASH_End-ADDR_Com_EFLASH_Start)>LEN_Com_EFLASH
#error ADDR_Com_EFLASH_End Over
#endif
//公共数据结束--------------------------------------------------------------------------------------------


// 参数区-------------------------------------------------------------------------------------------------
#define ADDR_TERMINAL_PARA_START	(ADDR_QCSG_SET_START + LEN_Com_EFLASH)
#define LEN_TERMINAL_PARA_EFLASH	(384*1024)


#define ADDR_CommunicationPara		ADDR_TERMINAL_PARA_START
#define ADDR_AlarmEventMaskPara		(ADDR_CommunicationPara + 160)
#define ADDR_SettlementDatePara		(ADDR_AlarmEventMaskPara + 100)//err20190109 256字节为存放掉电存储告警的标志
#define ADDR_IDAuthenticationPara	(ADDR_SettlementDatePara + 8)
#define ADDR_ThresholdPara			(ADDR_IDAuthenticationPara + 68)

#define ADDR_AMRChTimeTbl			(ADDR_ThresholdPara + 24)
#define AMR_CH_NUM					(5)
#define ADDR_AMRChSetTbl			(ADDR_AMRChTimeTbl + AMR_CH_NUM*12)

#define ADDR_RS485PortParaTbl		(ADDR_AMRChSetTbl + AMR_CH_NUM*652)	//3260
#define ADDR_AttachmentPara			(ADDR_RS485PortParaTbl + 3*8)
#define ADDR_LoadManagementPara		(ADDR_AttachmentPara + 40)
#define ADDR_MeterPointCtrlPara		(ADDR_LoadManagementPara + 536)



#define ADDR_AnalogPara				(ADDR_MeterPointCtrlPara + 68)
#define ADDR_PulsePara				(ADDR_AnalogPara + 52)
#define ADDR_CaculationPara			(ADDR_PulsePara + 52)
#define ADDR_DiffPara				(ADDR_CaculationPara + 552)

#define ADDR_NormalTaskPara			(ADDR_DiffPara + 296)
#define NormalTaskPara_Num			(32)
#define ADDR_RelayTaskPara			(ADDR_NormalTaskPara + 672*NormalTaskPara_Num)	//21504
#define RelayTaskPara_Num			(256)	//(RelayTaskPara Len: 296*256) //75776
#define ADDR_SelfAdaptionPara		(ADDR_RelayTaskPara + 296*RelayTaskPara_Num)	//自适应通信参数 0:非自适应 1:自适应
//SelfAdaptionPara_Len = 4
#define ADDR_AlarmSavePara			(ADDR_SelfAdaptionPara + 4)//err20190216 告警标志掉电存储区域
//AlarmSavePara_Len = 258


//合计:102612+4+258字节  		约100M
// 
#define ADDR_TERMINAL_PARA_END     ADDR_AlarmSavePara+258
#if ((ADDR_TERMINAL_PARA_END)-(ADDR_TERMINAL_PARA_START))>LEN_TERMINAL_PARA_EFLASH
	#error
#endif

// 参数区结束-------------------------------------------------------------------------------------------------


// 测量点档案存储区---512K------------------------------------------------------------------------------------------
#define ADDR_MP_START				(ADDR_TERMINAL_PARA_START + LEN_TERMINAL_PARA_EFLASH)
#define LEN_MP_RECORD_SIZE			(512*1024)
#define MP_SIZE						(64)
#define POWER_QUALITY_SIZE			(104)
#define MP_RECORD_SIZE				(MP_SIZE + POWER_QUALITY_SIZE)

#define ADDR_MP_END     ADDR_MP_START+MP_RECORD_SIZE
#if ((ADDR_MP_END)-(ADDR_MP_START))>LEN_MP_RECORD_SIZE
	#error
#endif



#define ADDR_QCSG_SET_END     ADDR_MP_START+LEN_MP_RECORD_SIZE

#if ((ADDR_QCSG_SET_END)-(ADDR_QCSG_SET_START)) > LEN_FLASH_QCSG_SET
#error LEN_FLASH_QCSG_SET
#endif



#ifdef __cplusplus
 }
#endif
#endif



