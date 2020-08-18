

//终端3类数据
#ifndef AFN0E_H
#define AFN0E_H




#define LEN_TERMINAL_EVENT     128//终端事件记录固定长度
#define ADDR_AFN0EF1     ADDR_AFN0E_Start//重要事件记录
//每事件固定占用LEN_TERMINAL_EVENT字节
//记录格式:
//事件代码ERC	BIN	1
//事件记录长度Le	BIN	1
//事件记录内容	
#define ADDR_AFN0EF2     ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*256)//一般事件记录
//每事件固定占用LEN_TERMINAL_EVENT字节
//记录格式:
//事件代码ERC	BIN	1
//事件记录长度Le	BIN	1
//事件记录内容

#define ADDR_AFN0EF3     (ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*256))
//ERC1
//2BYTE 已有事件计数
//15*LEN_TERMINAL_EVENT字节 15条事件记录
//...
//ERC68 15条
//2BYTE 已有事件计数
//15*LEN_TERMINAL_EVENT字节 15条事件记录
	

#define ADDR_AutoReportWord     (ADDR_AFN0EF3+(MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))))//电能表主动上报状态字
//AFN04F10配置序号1
//96byte 主动上报事件新增次数hex(偏移对应状态字的位号,初始化值0)
//1BYTE 上96BYTE的数据或(初始化值0)
//12byte 主动上报事件的位标志寄存,用于发复位命令(初始化值0)
//...
//AFN04F10配置序号n
#define LEN_AutoReportWord     (96+1+12)
#define ADDR_MeterEventCount     ADDR_AutoReportWord+(LEN_AutoReportWord*MaxRS485AddCarrierPn)//寄存电能表各事件原发生次数
//测量点1
//3byte 第1个电能表事件数据标识原发生次数BCD(初始化值0xff)
//1byte 新增次数hex(初始化值0xff)
//...
//3byte 第n个电能表事件数据标识原发生次数BCD(初始化值0xff)
//1byte 新增次数hex(初始化值0xff)

//3byte 0x03060000//39 B68辅助电源失电次数后的3byte总累计时间
//3byte 0x03050000//45 B74全失压次数后的3byte总累计时间

//...
//测量点n
#define NUM_MeterEventDI     (60+6)//抄读的标识个数
#define LEN_MeterEventCount     ((4*NUM_MeterEventDI)+6)//每测量点长度

#define ADDR_MeterEvent          ((ADDR_MeterEventCount+(LEN_MeterEventCount*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//读取的仪表事件
//电能表事件1
//1byte 端口号(1,2,3,4或31)
//6byte 电能表通信地址(BCD)

//1byte 长度(值4+3或4+6或4+18,其他值无效;专用于发生次数存储)//上海:长度(值4+3或4+6或4+18或4+3+4+3其他值无效,发生次数和总累计时间,4+3+4+3有单独的总累计时间数据标识和数据)
//2byte 总长度(1-10次事件数据存储)

//22byte 数据标识和数据(FLASH空间固定保留22byte;专用于发生次数存储)//上海:发生次数和总累计时间
//256byte 总数据标识个数,数据长度数据标识和数据,...(256或1030)//上海:无总数据标识个数,数据长度单元
//...
//电能表事件256
#define LEN_MeterEvent     (10+22+256)//(10+22+1030)//不能大于LEN_UARTnTx否则主动上报会不能上报死循环

#define ADDR_ACSampleCurve     ((ADDR_MeterEvent+(LEN_MeterEvent*256)+0xfff)&0xfffff000)//内部交采1分钟电压曲线,按冻结时的年月日时分对00年1月1日0时0分计算存储位置(初始化时自动选择扇区或块擦除为0xff值)
//5byte 年月日时分
//2byte A相电压曲线
//2byte B相电压曲线
//2byte C相电压曲线
#define LEN_ACSampleCurve     11//每点字节数


#define ADDR_AFN0E_End     (ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal))

#if (ADDR_AFN0E_End-ADDR_AFN0E_Start)>LEN_Terminal_EFLASH_AFN0E
	#error (ADDR_AFN0E_End-ADDR_AFN0E_Start)>LEN_Terminal_EFLASH_AFN0E
#endif

#endif
