
#ifndef IRAM_TerminalSave_H
#define IRAM_TerminalSave_H


#define ADDR_TFM_YMDHMS     ADDR_Terminal_SaveRam_Start//掉电前时钟
//秒,分,时,日,月,年
#define ADDR_POWER_YMDHMS     ADDR_TFM_YMDHMS+6//ERC14事件判定掉电前的时钟
//秒,分,时,日,月,年
#define ADDR_TerminalFMTimer     ADDR_POWER_YMDHMS+6//终端FM定时器地址
#define TFMSTIMER_NUM     2//终端FM中秒定时器数
#define TFMMTIMER_NUM     (3+16+3)//终端FM中分定时器数
//6BYTE 上次计算的秒分时日月年
//秒定时器
//2BYTE 0 1 秒加计数计算分
//2BYTE 2 2 

//分定时器
//2BYTE 0 1 终端保电分定时器
//2BYTE 2 2 设置键按下分定时器hex
//2BYTE 4 3 工厂模式有效分定时器hex

//2BYTE 6 4 总加组1当前功率下浮控的控制时间分定时器
//2BYTE ...
//2BYTE 20 11 总加组8当前功率下浮控的控制时间分定时器

//2BYTE 22 12 控制轮次1遥控跳闸限电时间分定时器
//2BYTE ...
//2BYTE 36 19 控制轮次8遥控跳闸限电时间分定时器

//2BYTE 38 20 RS232功能转换分定时器
//2BYTE 40 21 RS485-1功能转换分定时器
//2BYTE 42 22 RS485-2功能转换分定时器
#define ADDR_TerminalRealTimer     ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+(TFMMTIMER_NUM*2)//实走定时器,调整时钟无效
#define TREALMTIMER_NUM     1//终端实走分定时器数
//1BYTE 0 0=掉电时间未计算,1=掉电时间已计算
//1BYTE 1 保留
//2BYTE 2 TESAM密钥失效分定时器
#define ADDR_TCongealCount     ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2)//终端冻结计数(注2类数据清0时仅将计数值清为0)
//1byte 不用//冻结标志(0正在冻结,1冻结完成)B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结;在实时钟日更新时清0
//1byte	
//5byte 2 上次小时冻结的年月日时分

//2byte 7 B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
//5byte 9 上次曲线冻结的年月日时分

//2byte 14 B0-B6日冻结计数,B7=1表示日冻结计数满
//3byte 16 上次日冻结的年月日
//2byte 19 上月抄表数据补冻为上上月冻结的年月

//2byte 21 B0-B6(抄表日)冻结计数,B7=1表示(抄表日)冻结计数满
//5byte 23 上次抄表日冻结的年月日时分

//2byte 28 B0-B6抄表月冻结计数,B7=1表示月冻结计数满
//2byte 30 上次月冻结的年月

//3byte 32 日冻结更新的年月日(上日抄表数据补冻为上上日冻结)
//3byte 35 日冻结更新的年月日(补抄上2日)
//3byte 38 日冻结更新的年月日(补抄上3日)
//3byte 41
#define LEN_TCongealCount     44
#define ADDR_GPRSTimerTaskNotes     ADDR_TCongealCount+LEN_TCongealCount//GPRS通道定时任务上报记录
//类1数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间
//类2数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间
#define ADDR_ENETTimerTaskNotes     ADDR_GPRSTimerTaskNotes+(6*MaxTimerTask*2)//ENET通道定时任务上报记录
//类1数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间
//类2数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间
#define ADDR_RS232TimerTaskNote     ADDR_ENETTimerTaskNotes+(6*MaxTimerTask*2)//RS232通道定时任务上报记录
//类1数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间
//类2数据任务
//6BYTE 秒分时日月年任务1上次定时上报时间
//...
//6BYTE 秒分时日月年任务1上次定时上报时间



//部分类1实时数据
#define ADDR_TERMINAL_CLASS1DATA_FM_START     ADDR_RS232TimerTaskNote+(6*MaxTimerTask*2)
#define ADDR_AFN0CF9     ADDR_TERMINAL_CLASS1DATA_FM_START//终端状态量及变位标志(第2字节变位标志在UART_READDATA中设置读标志,在终端状态标志处理子程序中清0)
//字节1的D0～D7按位对应l～8路状态量的状态ST，置"O"："分"状态；置"l"："合"状态。
//字节2的D0～D7按位对应1～8路状态量的变位CD，置"O"：自前次遥信传送后无状态变化；置"1"：自前次遥信传送后至少有一次状态变化

#define ADDR_AFN0CF10     ADDR_AFN0CF9+2//终端与主站当日、月通信流量
//4BYTE
//4BYTE
//1BYTE 事件记录标志,0没记录,1=已记录
#define ADDR_AFN0CF12     ADDR_AFN0CF10+9//控制输出回路开关接入状态量及变位标志
//1BYTE 状态ST BIN 1 D0～D7 按顺序对位，2 位一组，对应1～4 路控制输出的常开、常闭触点接入开关跳闸回路状态量的状态ST，置“0”：“未接入”状态；置“1”：“接入”状态。
//1BYTE 变位CD BIN 1 D0～D7 按顺序对位，2 为一组，对应1～4 路控制输出的常开、常闭触点接入开关跳闸回路状态量的变位CD，置“0”：自前次遥信传送后无状态变化；置“1”：自前次遥信传送后至少有一次状态变化。
#define ADDR_AFN0CF19     ADDR_AFN0CF12+2//当日总加有功电能量(总加组号)
//总加组1
//1BYTE 费率数M
//4BYTE 当日总加总电能量,数据格式03
//4BYTE 费率l当日总加电能量,数据格式03
//...
//4BYTE 费率M当日总加电能量,数据格式03
//......
//总加组N
#define ADDR_AFN0CF20     ADDR_AFN0CF19+((1+4+(4*TMaxTariff))*AFN0CF19MaxPn)//当日总加无功电能量(总加组号)
//总加组1
//1BYTE 费率数M
//4BYTE 当日总加无功总电能量,数据格式03
//4BYTE 费率l当日总加无功电能量,数据格式03
//...
//4BYTE 费率M当日总加无功电能量,数据格式03
//......
//总加组N
#define ADDR_AFN0CF21     ADDR_AFN0CF20+((1+4+(4*TMaxTariff))*AFN0CF20MaxPn)//当月总加有功电能量(总加组号)
//总加组1
//1BYTE 费率数M
//4BYTE 当月总加有功总电能量,数据格式03
//4BYTE 费率l当月总加有功电能量,数据格式03
//...
//4BYTE 费率M当月总加有功电能量,数据格式03
//......
//总加组N
#define ADDR_AFN0CF22     ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*AFN0CF21MaxPn)//当月总加无功电能量(总加组号)
//总加组1
//1BYTE 费率数M
//4BYTE 当月总加无功总电能量,数据格式03
//4BYTE 费率l当月总加无功电能量,数据格式03
//...
//4BYTE 费率M当月总加无功电能量,数据格式03
//......
//总加组N
#define ADDR_AFN0CF23     ADDR_AFN0CF22+((1+4+(4*TMaxTariff))*AFN0CF22MaxPn)//终端当前剩余电量(费)(总加组号)
//总加组1
//4BYTE 当前剩余电量(费),数据格式03
//......
//总加组N
#define ADDR_AFN0CF23_DP     ADDR_AFN0CF23+(4*AFN0CF23MaxPn)//当前剩余电量单位G=1时(MWH)小数点后数据正负符号同当前剩余电量数据格式03,格式hex=0.000
//2BYTE 总加组1小数点后剩余电量或电费
//....
//2BYTE 总加组N小数点后剩余电量或电费
#define ADDR_AFN0CF24     ADDR_AFN0CF23_DP+(2*AFN0CF23MaxPn)//当前功率下浮控控后总加有功功率冻结值(总加组号)
//总加组1
//2BYTE 控后总加有功功率冻结值,数据格式02,kW
//......
//总加组N
#define ADDR_AFN0CF41     ADDR_AFN0CF24+(2*AFN0CF24MaxPn)//当日正向有功电能量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF42     ADDR_AFN0CF41+((1+(4*(TMaxTariff+1)))*AFN0CF41MaxPn)//当日正向无功电能量(总、费率1～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF43     ADDR_AFN0CF42+((1+(4*(TMaxTariff+1)))*AFN0CF42MaxPn)//当日反向有功电能量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF44     ADDR_AFN0CF43+((1+(4*(TMaxTariff+1)))*AFN0CF43MaxPn)//当日反向无功电能量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF45     ADDR_AFN0CF44+((1+(4*(TMaxTariff+1)))*AFN0CF44MaxPn)//当月正向有功电能量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF46     ADDR_AFN0CF45+((1+(4*(TMaxTariff+1)))*AFN0CF45MaxPn)//当月正向无功电能量(总、费率1～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF47     ADDR_AFN0CF46+((1+(4*(TMaxTariff+1)))*AFN0CF46MaxPn)//当月反向有功电能量(总、费率1～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF48     ADDR_AFN0CF47+((1+(4*(TMaxTariff+1)))*AFN0CF47MaxPn)//当月反向无功电能量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率
//4BYTE总电量
//4BYTE费率1电量
//...
//4BYTE费率N电量
//....
//测量点N数据
#define ADDR_AFN0CF66     ADDR_AFN0CF48+((1+(4*(TMaxTariff+1)))*AFN0CF48MaxPn)//当前电容器累计补偿投入时间和次数(测量点号)
//测量点1数据
//....
//测量点N数据
#define ADDR_AFN0CF67     ADDR_AFN0CF66+(72*AFN0CF66MaxPn)//当日、当月电容器累计补偿的无功电能量(测量点号)
//测量点1数据
//....
//测量点N数据

#define ADDR_Inc_User_Flag     ADDR_AFN0CF67+(8*AFN0CF67MaxPn)//电能增量使用标志
//1BYTE 测量点电能增量已计算标志,D0-D3分别表示正向有功,正向无功,反向有功,反向无功
//1BYTE 总加组电能增量使用标志,D0-D5分别表示当日有功,当日无功,当月有功,当月无功,ERC22差动事件区间电量,剩余电量
#define ADDR_PKWH_INC     ADDR_Inc_User_Flag+2//正向有功电能增量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率(仅表示电表费率,存贮时按TMaxTariff固定间隔)
//4BYTE总电量 HEX 4位小数(初始化时清为0)
//4BYTE费率1电量 HEX 4位小数
//...
//4BYTE费率TMaxTariff电量 HEX 4位小数
//5BYTE 原始总电量示值(初始化时清为0xee)
//5BYTE 原始费率1电量示值
//...
//5BYTE 原始费率TMaxTariff电量示值
//....
//测量点N数据
#define ADDR_PKVARH_INC     ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*MaxRS485Pn)//正向无功电能增量(总、费率1～M)(测量点号)
//测量点1数据
//1BYTE费率(仅表示电表费率,存贮时按TMaxTariff固定间隔)
//4BYTE总电量 HEX 4位小数(初始化时清为0)
//4BYTE费率1电量 HEX 4位小数
//...
//4BYTE费率TMaxTariff电量 HEX 4位小数
//4BYTE 原始总电量示值(初始化时清为0xee)
//4BYTE 原始费率1电量示值
//...
//4BYTE 原始费率TMaxTariff电量示值
//....
//测量点N数据
#define ADDR_NKWH_INC     ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*MaxRS485Pn)//反向有功电能增量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率(仅表示电表费率,存贮时按TMaxTariff固定间隔)
//4BYTE总电量 HEX 4位小数(初始化时清为0)
//4BYTE费率1电量 HEX 4位小数
//...
//4BYTE费率TMaxTariff电量 HEX 4位小数
//5BYTE 原始总电量示值(初始化时清为0xee)
//5BYTE 原始费率1电量示值
//...
//5BYTE 原始费率TMaxTariff电量示值
//....
//测量点N数据
#define ADDR_NKVARH_INC     ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*MaxRS485Pn)//反向无功电能增量(总、费率l～M)(测量点号)
//测量点1数据
//1BYTE费率(仅表示电表费率,存贮时按TMaxTariff固定间隔)
//4BYTE总电量 HEX 4位小数(初始化时清为0)
//4BYTE费率1电量 HEX 4位小数
//...
//4BYTE费率TMaxTariff电量 HEX 4位小数
//4BYTE 原始总电量示值(初始化时清为0xee)
//4BYTE 原始费率1电量示值
//...
//4BYTE 原始费率TMaxTariff电量示值
//....
//测量点N数据
#define ADDR_AFN0CF19_Remainder     ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*MaxRS485Pn)//当日总加有功电能量增量小数位(总加组号)
//总加组1
//4BYTE 当日总加总电能量,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N
#define ADDR_AFN0CF20_Remainder     ADDR_AFN0CF19_Remainder+(4*(TMaxTariff+1)*AFN0CF19MaxPn)//当日总加无功电能量增量小数位(总加组号)
//总加组1
//4BYTE 当日总加总电能量,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N
#define ADDR_AFN0CF21_Remainder     ADDR_AFN0CF20_Remainder+(4*(TMaxTariff+1)*AFN0CF20MaxPn)//当月总加有功电能量增量小数位(总加组号)
//总加组1
//4BYTE 当日总加总电能量,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N
#define ADDR_AFN0CF22_Remainder     ADDR_AFN0CF21_Remainder+(4*(TMaxTariff+1)*AFN0CF21MaxPn)//当月总加无功电能量增量小数位(总加组号)
//总加组1
//4BYTE 当日总加总电能量,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M当日总加电能量,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N
#define ADDR_AFN0CF23_Remainder     ADDR_AFN0CF22_Remainder+(4*(TMaxTariff+1)*AFN0CF22MaxPn)//终端当前剩余电量(费)增量小数位(总加组号)
//总加组1
//4BYTE 总,HEX补码4位小数点单位为kWh;初始化时清0
//4BYTE 费率l,HEX补码4位小数点单位为kWh;初始化时清0
//...
//4BYTE 费率M,HEX补码4位小数点单位为kWh;初始化时清0
//......
//总加组N

#define ADDR_Terminal_CLASS1DATA_FM_End     ADDR_AFN0CF23_Remainder+(4*(TMaxTariff+1)*AFN0CF23MaxPn)



//类2数据原始(在日月冻结后清0;在终端下达命令数据区初始化时不清0若要清0可考虑数据是否放在终端统计数据区)
#define ADDR_Terminal_CLASS2DATA_FM_Start     ADDR_Terminal_CLASS1DATA_FM_End//



#define ADDR_Terminal_CLASS2DATA_FM_End     ADDR_Terminal_CLASS2DATA_FM_Start


//事件计数和状态
#define TERMINAL_CLASS3DATA_FM_Start     ADDR_Terminal_CLASS2DATA_FM_End//
#define ADDR_AFN0CF7_FM     TERMINAL_CLASS3DATA_FM_Start
	//1BYTE EC1重要事件计数
	//1BYTE EC2一般事件计数
//GPRS
	//1BYTE 当前还没上传被确认重要事件计数,=0无新重要事件或已上传
	//1BYTE 当前有?个重要事件主动上传需确认(在数据类3读指针,收到主站确认帧,重发次数到,中清0)
//以太网
	//1BYTE 当前还没上传被确认重要事件计数,=0无新重要事件或已上传
	//1BYTE 当前有?个重要事件主动上传需确认(在数据类3读指针,收到主站确认帧,重发次数到,中清0)
//RS232
	//1BYTE 当前还没上传被确认重要事件计数,=0无新重要事件或已上传
	//1BYTE 当前有?个重要事件主动上传需确认(在数据类3读指针,收到主站确认帧,重发次数到,中清0)
#define ADDR_AFN0CF8     ADDR_AFN0CF7_FM+8//F8终端事件标志状态p0
//8BYTE EC1-EC64
#define ADDR_ADCFlags     ADDR_AFN0CF8+8//重要事件等待访问标志
//GPRS
//1BYTE 重要事件等待访问0=没,1=有
//以太网
//1BYTE 重要事件等待访问0=没,1=有
//RS232
//1BYTE 重要事件等待访问0=没,1=有
#define ADDR_Terminal_CLASS3DATA_FM_END     (ADDR_ADCFlags+3)


//脉冲输入
#define ADDR_Terminal_impIn_FM_START     ADDR_Terminal_CLASS3DATA_FM_END
#define ADDR_Terminal_impIn_Energy     ADDR_Terminal_impIn_FM_START//终端脉冲输入端口电能
//脉冲端口1-MaxImpInPort
//4BYTE ((脉冲数*10000)/电度常数)的佘数hex
//5BYTE 总电量(HEX,4位小数)
//5BYTE 费率1电量(HEX,4位小数)
//...
//5BYTE 费率N电量(HEX,4位小数)
#define ADDR_Terminal_impIn_Demand     ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*MaxImpInPort)//脉冲端口当月最大需量和时间,月更新清0
//脉冲端口1
//总最大需量(数据格式23)
//总最大需量出现时间(分时日月)
//费率1最大需量(数据格式23)
//费率1最大需量出现时间(分时日月)
//...
//费率N最大需量(数据格式23)
//费率N最大需量出现时间(分时日月)
//脉冲端口?
//总最大需量(数据格式23)
//总最大需量出现时间(分时日月)
//费率1最大需量(数据格式23)
//费率1最大需量出现时间(分时日月)
//...
//费率N最大需量(数据格式23)
//费率N最大需量出现时间(分时日月)
//脉冲端口MaxImpInPort
//总最大需量(数据格式23)
//总最大需量出现时间(分时日月)
//费率1最大需量(数据格式23)
//费率1最大需量出现时间(分时日月)
//...
//费率N最大需量(数据格式23)
//费率N最大需量出现时间(分时日月)
#define ADDR_Terminal_impIn_FM_END     ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*MaxImpInPort)


//终端统计数据(在日月冻结后清0;在终端下达命令数据区初始化时会清0若要不清0可考虑数据是否放在类2数据原始区)
#define ADDR_Terminal_StatData_FM_Start     ADDR_Terminal_impIn_FM_END//终端统计数据在FM中开始地址
#define ADDR_AFN0DF49_SOUR     ADDR_Terminal_StatData_FM_Start//终端日供电时间、日复位累计次数
//终端日供电时间	BIN	min	2
//终端日复位累计次数	BIN	次	2
#define ADDR_AFN0DF51_SOUR     ADDR_AFN0DF49_SOUR+4//终端月供电时间、月复位累计次数
//终端月供电时间	BIN	min	2
//终端月复位累计次数	BIN	次	2
#define ADDR_AFN0DF50_SOUR     ADDR_AFN0DF51_SOUR+4//终端日控制统计数据
//月电控跳闸日累计次数	BIN	次	1
//购电控跳闸日累计次数	BIN	次	1
//功控跳闸日累计次数	BIN	次	1
//遥控跳闸日累计次数	BIN	次	1
#define ADDR_AFN0DF52_SOUR     ADDR_AFN0DF50_SOUR+4//终端月控制统计数据
//月电控跳闸月累计次数	BIN	次	1
//购电控跳闸月累计次数	BIN	次	1
//功控跳闸月累计次数	BIN	次	1
//遥控跳闸月累计次数	BIN	次	1
#define ADDR_AFN0DF25_SOUR     ADDR_AFN0DF52_SOUR+4//日总及分相最大有功功率及发生时间、有功功率为零时间
//三相总最大有功功率	见附录A.23	kW	3
//三相总最大有功功率发生时间	见附录A.18	分时日	3
//A相最大有功功率	见附录A.23	kW	3
//A相最大有功功率发生时间	见附录A.18	分时日	3
//B相最大有功功率	见附录A.23	kW	3
//B相最大有功功率发生时间	见附录A.18	分时日	3
//C相最大有功功率	见附录A.23	kW	3
//C相最大有功功率发生时间	见附录A.18	分时日	3
//三相总有功功率为零时间	BIN	min	2
//A相有功功率为零时间	BIN	min	2
//B相有功功率为零时间	BIN	min	2
//C相有功功率为零时间	BIN	min	2
#define ADDR_AFN0DF33_SOUR     ADDR_AFN0DF25_SOUR+(32*AFN0DF25MaxPn)//月总及分相最大有功功率及发生时间、有功功率为零时间
//三相总最大有功功率	见附录A.23	kW	3
//三相总最大有功功率发生时间	见附录A.18	分时日	3
//A相最大有功功率	见附录A.23	kW	3
//A相最大有功功率发生时间	见附录A.18	分时日	3
//B相最大有功功率	见附录A.23	kW	3
//B相最大有功功率发生时间	见附录A.18	分时日	3
//C相最大有功功率	见附录A.23	kW	3
//C相最大有功功率发生时间	见附录A.18	分时日	3
//三相总有功功率为零时间	BIN	min	2
//A相有功功率为零时间	BIN	min	2
//B相有功功率为零时间	BIN	min	2
//C相有功功率为零时间	BIN	min	2
#define ADDR_AFN0DF57_SOUR     ADDR_AFN0DF33_SOUR+(32*AFN0DF33MaxPn)//总加组日最大、最小有功功率及发生时间，功率为零累计时间
//日最大有功功率	见附录A.2	kW/MW	2
//日最大有功功率发生时间	见附录A.18	分时日	3
//日最有功小功率	见附录A.2	kW/MW	2
//日最小有功功率发生时间	见附录A.18	分时日	3
//有功功率为零日累计时间	BIN	min	2
#define ADDR_AFN0DF60_SOUR     ADDR_AFN0DF57_SOUR+(12*MaxTotalAddGroup)//总加组月最大、最小有功功率及发生时间，功率为零累计时间
//月最大有功功率	见附录A.2	kW/MW	2
//月最大有功功率发生时间	见附录A.18	分时日	3
//月最有功小功率	见附录A.2	kW/MW	2
//月最小有功功率发生时间	见附录A.18	分时日	3
//有功功率为零月累计时间	BIN	min	2
#define ADDR_AFN0DF27_SOUR     ADDR_AFN0DF60_SOUR+(12*MaxTotalAddGroup)//日电压统计数据
//0 A相电压越上上限日累计时间	BIN	min	2
//2 A相电压越下下限日累计时间	BIN	min	2
//4 A相电压越上限日累计时间	BIN	min	2
//6 A相电压越下限日累计时间	BIN	min	2
//8 A相电压合格日累计时间	BIN	min	2
//10 B相电压越上上限日累计时间	BIN	min	2
//12 B相电压越下下限日累计时间	BIN	min	2
//14 B相电压越上限日累计时间	BIN	min	2
//16 B相电压越下限日累计时间	BIN	min	2
//18 B相电压合格日累计时间	BIN	min	2
//20 C相电压越上上限日累计时间	BIN	min	2
//22 C相电压越下下限日累计时间	BIN	min	2
//24 C相电压越上限日累计时间	BIN	min	2
//26 C相电压越下限日累计时间	BIN	min	2
//28 C相电压合格日累计时间	BIN	min	2
//30 A相电压最大值	见附录A.7	V	2
//32 A相电压最大值发生时间	见附录A.18	分时日	3
//35 A相电压最小值	见附录A.7	V	2
//37 A相电压最小值发生时间	见附录A.18	分时日	3
//40 B相电压最大值	见附录A.7	V	2
//42 B相电压最大值发生时间	见附录A.18	分时日	3
//45 B相电压最小值	见附录A.7	V	2
//47 B相电压最小值发生时间	见附录A.18	分时日	3
//50 C相电压最大值	见附录A.7	V	2
//52 C相电压最大值发生时间	见附录A.18	分时日	3
//55 C相电压最小值	见附录A.7	V	2
//57 C相电压最小值发生时间	见附录A.18	分时日	3
//60 A相平均电压	见附录A.7	V	2
//62 B相平均电压	见附录A.7	V	2
//64 C相平均电压	见附录A.7	V	2

//66 A相电压越上限率 见附录A.25	%	3
//69 A相电压越下限率 见附录A.25	%	3
//72 A相电压合格率 见附录A.25	%	3
//75 B相电压越上限率 见附录A.25	%	3
//78 B相电压越下限率 见附录A.25	%	3
//81 B相电压合格率 见附录A.25	%	3
//84 C相电压越上限率 见附录A.25	%	3
//87 C相电压越下限率 见附录A.25	%	3
//90 C相电压合格率 见附录A.25	%	3
#define ADDR_AFN0DF27_PAR     ADDR_AFN0DF27_SOUR+((66+27)*AFN0DF27MaxPn)//日平均电压累加计数(日冻结后清0)
//2BYTE A电压累加计数
//4BYTE A电压累加hex
//2BYTE B电压累加计数
//4BYTE B电压累加hex
//2BYTE C电压累加计数
//4BYTE C电压累加hex
#define ADDR_AFN0DF35_SOUR     ADDR_AFN0DF27_PAR+(18*AFN0DF27MaxPn)//月电压统计数据
//0 A相电压越上上限日累计时间	BIN	min	2
//2 A相电压越下下限日累计时间	BIN	min	2
//4 A相电压越上限日累计时间	BIN	min	2
//6 A相电压越下限日累计时间	BIN	min	2
//8 A相电压合格日累计时间	BIN	min	2
//10 B相电压越上上限日累计时间	BIN	min	2
//12 B相电压越下下限日累计时间	BIN	min	2
//14 B相电压越上限日累计时间	BIN	min	2
//16 B相电压越下限日累计时间	BIN	min	2
//18 B相电压合格日累计时间	BIN	min	2
//20 C相电压越上上限日累计时间	BIN	min	2
//22 C相电压越下下限日累计时间	BIN	min	2
//24 C相电压越上限日累计时间	BIN	min	2
//26 C相电压越下限日累计时间	BIN	min	2
//28 C相电压合格日累计时间	BIN	min	2
//30 A相电压最大值	见附录A.7	V	2
//32 A相电压最大值发生时间	见附录A.18	分时日	3
//35 A相电压最小值	见附录A.7	V	2
//37 A相电压最小值发生时间	见附录A.18	分时日	3
//40 B相电压最大值	见附录A.7	V	2
//42 B相电压最大值发生时间	见附录A.18	分时日	3
//45 B相电压最小值	见附录A.7	V	2
//47 B相电压最小值发生时间	见附录A.18	分时日	3
//50 C相电压最大值	见附录A.7	V	2
//52 C相电压最大值发生时间	见附录A.18	分时日	3
//55 C相电压最小值	见附录A.7	V	2
//57 C相电压最小值发生时间	见附录A.18	分时日	3
//60 A相平均电压	见附录A.7	V	2
//62 B相平均电压	见附录A.7	V	2
//64 C相平均电压	见附录A.7	V	2

//66 A相电压越上限率 见附录A.25	%	3
//69 A相电压越下限率 见附录A.25	%	3
//72 A相电压合格率 见附录A.25	%	3
//75 B相电压越上限率 见附录A.25	%	3
//78 B相电压越下限率 见附录A.25	%	3
//81 B相电压合格率 见附录A.25	%	3
//84 C相电压越上限率 见附录A.25	%	3
//87 C相电压越下限率 见附录A.25	%	3
//90 C相电压合格率 见附录A.25	%	3
#define ADDR_AFN0DF35_PAR     ADDR_AFN0DF35_SOUR+((66+27)*AFN0DF35MaxPn)//月平均电压累加计数(月冻结后清0)
//2BYTE A电压累加计数
//4BYTE A电压累加hex
//2BYTE B电压累加计数
//4BYTE B电压累加hex
//2BYTE C电压累加计数
//4BYTE C电压累加hex
#define ADDR_AFN0DF29_SOUR     ADDR_AFN0DF35_PAR+(18*AFN0DF35MaxPn)//日电流越限数据
//A相电流越上上限累计时间	BIN	min	2
//A相电流越上限累计时间	BIN	min	2
//B相电流越上上限累计时间	BIN	min	2
//B相电流越上限累计时间	BIN	min	2
//C相电流越上上限累计时间	BIN	min	2
//C相电流越上限累计时间	BIN	min	2
//零序电流越上限累计时间	BIN	min	2
//A相电流最大值	见附录A.25	A	3
//A相电流最大值发生时间	见附录A.18	分时日	3
//B相电流最大值	见附录A.25	A	3
//B相电流最大值发生时间	见附录A.18	分时日	3
//C相电流最大值	见附录A.25	A	3
//C相电流最大值发生时间	见附录A.18	分时日	3
//零序电流最大值	见附录A.25	A	3
//零序电流最大值发生时间	见附录A.18	分时日	3
#define ADDR_AFN0DF37_SOUR     ADDR_AFN0DF29_SOUR+(38*AFN0DF29MaxPn)//月电流越限数据
//A相电流越上上限月累计时间	BIN	min	2
//A相电流越上限月累计时间	BIN	min	2
//B相电流越上上限月累计时间	BIN	min	2
//B相电流越上限月累计时间	BIN	min	2
//C相电流越上上限月累计时间	BIN	min	2
//C相电流越上限月累计时间	BIN	min	2
//零序电流越上限月累计时间	BIN	min	2
//A相电流最大值	见附录A.25	A	3
//A相电流最大值发生时间	见附录A.18	分时日	3
//B相电流最大值	见附录A.25	A	3
//B相电流最大值发生时间	见附录A.18	分时日	3
//C相电流最大值	见附录A.25	A	3
//C相电流最大值发生时间	见附录A.18	分时日	3
//零序电流最大值	见附录A.25	A	3
//零序电流最大值发生时间	见附录A.18	分时日	3
#define ADDR_AFN0DF28_SOUR      ADDR_AFN0DF37_SOUR+(38*AFN0DF37MaxPn)//日不平衡度越限累计时间
//电流不平衡度越限日累计时间	BIN	min	2
//电压不平衡度越限日累计时间 	BIN	min	2
//电流不平衡最大值	见附录A.5	％	2
//电流不平衡最大值发生时间	见附录A.18	分时日	3
//电压不平衡最大值	见附录A.5	％	2
//电压不平衡最大值发生时间	见附录A.18	分时日	3
#define ADDR_AFN0DF36_SOUR     ADDR_AFN0DF28_SOUR+(14*AFN0DF28MaxPn)//月不平衡度越限累计时间
//电流不平衡度越限月累计时间	BIN	min	2
//电压不平衡度越限月累计时间 	BIN	min	2
//电流不平衡最大值	见附录A.5	％	2
//电流不平衡最大值发生时间	见附录A.17	分时日月	4
//电压不平衡最大值	见附录A.5	％	2
//电压不平衡最大值发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0DF30_SOUR     ADDR_AFN0DF36_SOUR+(16*AFN0DF36MaxPn)//日视在功率越限累计时间
//视在功率越上上限累计时间	BIN	min	2
//视在功率越上限累计时间	BIN	min	2
#define ADDR_AFN0DF38_SOUR     ADDR_AFN0DF30_SOUR+(4*AFN0DF30MaxPn)//月视在功率越限累计时间
//视在功率越上上限月累计时间	BIN	min	2
//视在功率越上限月累计时间	BIN	min	2
#define ADDR_AFN0DF31_SOUR     ADDR_AFN0DF38_SOUR+(4*AFN0DF38MaxPn)//日负载率统计
//负载率最大值	见附录A.5	％	2
//负载率最大值发生时间	见附录A.18	分时日	3
//负载率最小值	见附录A.5	％	2
//负载率最小值发生时间	见附录A.18	分时日	3
#define ADDR_AFN0DF39_SOUR     ADDR_AFN0DF31_SOUR+(10*AFN0DF31MaxPn)//月负载率统计
//负载率最大值	见附录A.5	％	2
//负载率最大值发生时间	见附录A.17	分时日月	4
//负载率最小值	见附录A.5	％	2
//负载率最小值发生时间	见附录A.17	分时日月	4
#define ADDR_AFN0DF65_SOUR     ADDR_AFN0DF39_SOUR+(12*AFN0DF39MaxPn)//总加组超功率定值的月累计时间及月累计电能量
//超功率定值月累计时间	BIN	min	2
//超功率定值的月累计电能量	见附录A.3	kWh/MWh	4
#define ADDR_AFN0DF66_SOUR     ADDR_AFN0DF65_SOUR+(6*MaxTotalAddGroup)//总加组超月电能量定值的月累计时间及月累计电能量
//超月电能量定值月累计时间	BIN	min	2
//超月冻结电能量定值的月累计电能量	见附录A.3	kWh/MWh	4
#define ADDR_AFN0DF43_SOUR     ADDR_AFN0DF66_SOUR+(6*MaxTotalAddGroup)//日功率因数区段累计时间
//区段1累计时间（功率因数＜定值1）	BIN	min	2
//区段2累计时间（定值1≤功率因数＜定值2） 	BIN	min	2
//区段3累计时间（功率因数≥定值2）  	BIN	min	2
#define ADDR_AFN0DF44_SOUR     ADDR_AFN0DF43_SOUR+(6*AFN0DF43MaxPn)//月功率因数区段累计时间
//区段1累计时间（功率因数＜定值1）	BIN	min	2
//区段2累计时间（定值1≤功率因数＜定值2） 	BIN	min	2
//区段3累计时间（功率因数≥定值2）  	BIN	min	2
#define ADDR_AFN0DF129_SOUR      (ADDR_AFN0DF44_SOUR+(6*AFN0DF44MaxPn))//测量点直流模拟量越限日累计时间、最大/最小值及发生时间
//直流模拟量越上限日累计时间	BIN	min	2
//直流模拟量越下限日累计时间	BIN	min	2
//直流模拟量日最大值	数据格式见附录A2		2
//直流模拟量日最大值发生时间	见附录A.18	分时日	3
//直流模拟量日最小值	见附录A.2		2
//直流模拟量日最小值发生时间	见附录A.18	分时日	3
#define ADDR_AFN0DF130_SOUR      ADDR_AFN0DF129_SOUR+(14*AFN0DF129MaxPn)//直流模拟量越限月累计时间、最大/最小值及发生时间
//直流模拟量越上限月累计时间	BIN	min	2
//直流模拟量越下限月累计时间	BIN	min	2
//直流模拟量月最大值	见附录A.2		2
//直流模拟量月最大值发生时间	见附录A.18	分时日	3
//直流模拟量月最小值	见附录A.2		2
//直流模拟量月最小值发生时间	见附录A.18	分时日	3
#define ADDR_Terminal_StatData_FM_End     (ADDR_AFN0DF130_SOUR+(14*AFN0DF130MaxPn))//终端统计数据在FM中结束地址


#define ADDR_AutoSearchMeter_Start     ADDR_Terminal_StatData_FM_End//自动搜表地址
//6BYTE RS485-1当前搜表地址
//1BYTE RS485-1当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
//6BYTE RS485-2当前搜表地址
//1BYTE RS485-2当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
//6BYTE RS485-3当前搜表地址
//1BYTE RS485-3当前搜表的速率B0-B3:0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
													//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
#define LEN_PerPortAutoSearchMeter      7
#define ADDR_AutoSearchMeter_End     (ADDR_AutoSearchMeter_Start+(3*LEN_PerPortAutoSearchMeter))


#define ADDR_FLASHLifeCount     ADDR_AutoSearchMeter_End//FLASH寿命测试计数器
//4BYTE FLASH1寿命测试计数
//4BYTE FLASH2寿命测试计数
#define LEN_FLASHLifeCount     0//8//仅需要寿命测试时长度设为8
#define ADDR_FLASHLifeCount_End     (ADDR_FLASHLifeCount+LEN_FLASHLifeCount)



#define ADDR_Terminal_SaveRam_BaseEnd     ADDR_FLASHLifeCount_End


#if (USER/100)==5//上海
#define ADDR_AFN0CF242     ADDR_Terminal_SaveRam_BaseEnd//当日登录主站信息
//4BYTE 日登录次数n
//4BYTE 月登录次数n
#define ADDR_CURVECONGEAL_ExtCount     ADDR_AFN0CF242+8
//2byte 7 B0-B14曲线冻结计数,b15=1表示曲线冻结计数满
//5byte 9 上次曲线冻结的年月日时分
#define ADDR_Terminal_SaveRam_USERExtEnd     (ADDR_CURVECONGEAL_ExtCount+7)
#else//#if (USER/100)==5//上海
#define ADDR_Terminal_SaveRam_USERExtEnd     ADDR_Terminal_SaveRam_BaseEnd
#endif



//扩展
#define ADDR_MeterEventCount_FM     ADDR_Terminal_SaveRam_USERExtEnd//电能表事件指针计数
	//1BYTE 电能表事件存储指针计数
	//1BYTE 保留
//GPRS
	//1BYTE 当前还没上传被确认电能表事件计数,=0无新电能表事件或已上传
	//1BYTE 当前有?个电能表事件主动上传需确认(收到主站确认帧,重发次数到,中清0)
//以太网
	//1BYTE 当前还没上传被确认电能表事件计数,=0无新电能表事件或已上传
	//1BYTE 当前有?个电能表事件主动上传需确认(收到主站确认帧,重发次数到,中清0)
//RS232
	//1BYTE 当前还没上传被确认电能表事件计数,=0无新电能表事件或已上传
	//1BYTE 当前有?个电能表事件主动上传需确认(收到主站确认帧,重发次数到,中清0)
#define ADDR_Terminal_SaveRam_ExtEnd     ADDR_MeterEventCount_FM+8





#define ADDR_Terminal_SaveRam_End     (ADDR_Terminal_SaveRam_ExtEnd)




#endif
