

#include "../H698/Project698.h"
#include "../Terminal698/Terminal_Init.h"
#include "../MS/SetInit.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../MS/MS.h"
#include "Terminal698_Uart_Manage.h"

#include "Terminal_Contrl_Out.h"
//#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"

#include "../DL698/RM_TASK.h"
#include "../DL698/DL698_DATALIB.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/CALC_impIn.h"
#include "../DL698/CALC_TAG.h"

#include "../DL698/DL698TEST.h"



const u8 InitData_4500_5[]=//初始化数据.无线公网版本信息
{
	DataType_structure,
	6,
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//厂商代码
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//软件版本号
	DataType_visible_string,
	6,
	'0',' ',' ',' ',' ',' ',//软件版本日期
	DataType_visible_string,
	4,
	'0',' ',' ',' ',//硬件版本号
	DataType_visible_string,
	6,
	'0',' ',' ',' ',' ',' ',//硬件版本日期
	DataType_visible_string,
	8,
	'0',' ',' ',' ',' ',' ',' ',' ',//厂家扩展信息
};


const u8 Init_double_long[]=//初始化数据double_long
{
	DataType_double_long,
	0,0,0,0,
};


void FLASHtoSDRAM(void)//参数FLASHtoSDRAM
{
	
	MR(ADDR_6000_SDRAM,ADDR_6000,LENmax_6000);//采集档案配置表FLASHtoSDRAM
	MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//任务配置表FLASHtoSDRAM
	MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//普通采集方案FLASHtoSDRAM
	MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//事件采集方案FLASHtoSDRAM
	MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//上报方案FLASHtoSDRAM
	
}



const u8 Init_DATA_6000_TEST[]=//测试用采集档案配置
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x11,0x11,0x11,0x11,0x11,0x11,
	//0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	6,
	//3,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//ABB (5)
//LANDIS (6)
//}，
	DataType_enum,
	3,
//端口      OAD，
	DataType_OAD,
	//0xF2,0x08,0x02,0x01, //交流采样通信口号(虚拟)
	0xF2,0x01,0x02,0x01, //RS485_1
	//0xF2,0x01,0x02,0x02, //RS485_2
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	51,//1,
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};


void TEST_ConnectAppOI_LIST(void);//测试对象列表
void Init_Terminal(void)//启动初始化
{
	u32 i;
  u32 x;
//	u8* p8;
	u16 *p16;
  u32 * p32;


//IRAM数据清0(除显示外)
	Comm_Ram->ErrorIC|=RAM_Test(ADDR_TerminalDisplay_IRAM_End,ADDR_Terminal_IRAM_End-ADDR_TerminalDisplay_IRAM_End);//RAM测试;返回:0=正确,1=错误
//ERAM数据清0
	p32=(u32*)ADDR_SDRAM_CLR_START;
	x=ADDR_ERAM_END-ADDR_SDRAM_CLR_START;
	x/=4;
	for(i=0;i<x;i++)
	{
		p32[i]=0;
	}
	Terminal_Ram->S_PR=60;//1 60秒分频产生分
	Comm_Ram->BattWork_S_timer=30;//电池工作关机秒定时器

//以下只有在电网有电时进行
  if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  {
    return;
  }
	
	MC(0,ADDR_TerminalRealTimer,1);//0=掉电时间未计算,1=掉电时间已计算
	Terminal_Ram->SlipCYC_S_Timer=15;//23 终端输入脉冲功率计算和需量脉冲滑移秒定时器
//数据初始化
	Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_1PORT);
	p16[0]=10;//10秒上电后延时抄表
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_2PORT);
	p16[0]=10;//10秒上电后延时抄表
	p16=(u16 *)Get_ADDR_UARTnTimer_S(RS485_4PORT);//(集中器载波,专变控制模块)
	p16[0]=10;//10秒上电后延时抄表


//引脚初始化
	Pin_Configure(PIN_BUZZ,
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID0,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_GPRS_ID1,//输入有上拉
					0|//B3-B0:复用功能选择AF(0-15)
		 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Init_Contrl_Pin();//初始化终端输出控制和LED引脚

	Pin_Configure(PIN_RS485T1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485R1_LED,//抄表485_1LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485T2_LED,//抄表485_2LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_RS485R2_LED,//抄表485_2LED
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Terminal_Ram->RS485_1_TxLED_MS_Timer=2000/10;//10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
	Terminal_Ram->RS485_1_RxLED_MS_Timer=2000/10;//11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
	Terminal_Ram->RS485_2_TxLED_MS_Timer=2000/10;//12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
	Terminal_Ram->RS485_2_RxLED_MS_Timer=2000/10;//13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器

	OnE5V0();//开以太网电源以使LED灯能亮
	Pin_Configure(PIN_ENET_LINK_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	Pin_Configure(PIN_ENET_ACT_LED,//
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	p16[0]=2000/10;//上电时置关机延时2s作为LED亮灯检查
	Terminal_Ram->ENETLED_MS_Timer=2000/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器

#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	Terminal_Ram->ContrlLoopCheck_S_Timer=60;//36 控制输出回路检测有效等待秒定时器
#endif

	Comm_Ram->CheckLEDTimer=3000/10;//上电全部LED检查显示 每10MS减1定时器
//UART初始化
	Terminal_UartInit_Manage();//Terminal_Uart初始化管理
//


//新DL698
//---TEST START---

//	MC(0,ADDR_6000,16*LENper_6000);
//	MW((u32)Init_DATA_6000_TEST,ADDR_6000+(1*LENper_6000),sizeof(Init_DATA_6000_TEST));

//	MC(0,ADDR_6012_2,16*LENper_6012_2);
//	MC(0,ADDR_6014,16*LENper_6014);
//	MWR(1,ADDR_3110+1,1);
//	Event_OAD(0x31100200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF

//---TEST END---

	FLASHtoSDRAM();//参数FLASHtoSDRAM
	MR(ADDR_4500_5,(u32)InitData_4500_5,sizeof(InitData_4500_5));//初始化数据.无线公网版本信息
	Init_SwitchIn();//初始化终端开关量输入
	RESET_Count();//终端复位次数
#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	i=MRR(ADDR_8001_4+1,2);
	i=(i>>8)|((i&0xff)<<8);
	if(i)
	{//0表示上电不自动保电
		i*=60;
		if(i>65535)
		{
			i=65535;
		}
		Terminal_Ram->UpPower_Protect_S_Timer=i;//35 终端上电保电秒定时器
	}
#endif
	
#if DEBUG==1//0=发行,1=调试
TEST_ConnectAppOI_LIST();//测试对象列表
#endif
#if RUN_DL698TEST_USER==1//0无效,1=运行;用户版测试
DL698TEST_Init_USER();//用户版测试初台化
#endif
	
}


void Init_ALLDATA(void)//终端全部数据初始化(清0)
{
	u32 i;
	ms_Type* ms;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
//清全部文件
	ms=Get_ms();
	ms->msfile_format();
	ms->Init_my_heap();//初始化与内核RTOS无关的我的内存堆
	
//数据库中列表的数据
	DL698_DATALIB_INIT();//全部数据库中列表的数据初始化(清0)
//事件原始数据
	MC(0,ADDR_Event_Sour,NUM_RMmax*LEN_Event_Sour);
//采集任务监控
	for(i=0;i<RMTASKmax;i++)
	{
		RM_Monitor_Init(i);//采集任务监控初始化;入口:ID=任务号
	}
//
	Init_SwitchIn();//初始化终端开关量输入
//
	Init_impInData();//清0脉冲输入终端电能和需量数据
//
	Init_Data230x();//初始化总加组数据
//
#if (USER/100)==15//安徽
	MC(0,ADDR_2131_All,LENmax_2131_All*31*3);//存储每次收到ABC相电压合格率存储的数据,保存一个月31天的数据清空
#endif
#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
	MC(0,ADDR_SourMeterEventCount,LENper_SourMeterEventCount*NUM_RMmax);
#endif
//
	MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*0),7);//RS485-1初始搜表地址
	MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*1),7);//RS485-2初始搜表地址
//
	MC(0,ADDR_MeterAutoReport_3011_Count,4);//电能表停电主动上报事件记录序号（顺序生成）
//
	MC(0,ADDR_EventCurrentvalue_3003,LENper_EventCurrentvalue*NUM_EventCurrentvalue);//事件当前值记录表
	
//清0，网络拓扑信息，多网络信息，宽带载波芯片信息
	MC(0,ADDR_Read_Info_Time,7);
	MC(0,ADDR_AFN10F21,2);
	MC(0,ADDR_AFN10F111,2);
	MC(0,ADDR_AFN10F112,2);
	
	
//RS485-1口抄表任务初始化
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl->TaskID=0;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART2Ctrl-1024);
//RS485-2口抄表任务初始化
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	UARTCtrl->TaskID=0;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART3Ctrl-1024);
#if ((Project/100)==2)||((Project/100)==4)//集中器
//载波口抄表任务初始化
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	UARTCtrl->TaskID=0;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	MC(0,(u32)&UARTCtrl->TaskTime,LEN_UART4Ctrl-1024);
	Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
#endif
	
	
//内部交采数据清0
	if(Comm_Ram->Factory==0x55)
	{//是工厂模式
		//保护工厂模式定时
		i=MRR(ADDR_MeterFMTimer+14,2);//工厂模式定时
    //清SaveRam
    MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam*2);
		
		//恢复工厂模式定时
		MWR(i,ADDR_MeterFMTimer+14,2);//工厂模式定时
    //清EFLASH
    MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
		MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//仪表参数扩展
//		//电表曲线(电表清0时自动采用128K块擦除)
//		i=(ADDR_MeterCurve_End)-(ADDR_MeterCurve_Start);
//		MC(0xff,ADDR_MeterCurve_Start,i);//NAND_FLASH 块擦除
		//参数初始化
		
	}
//终端冻结时间
	//终端冻结时间不能清0,否则终端数据区初始化后复位次数为1而后因又判为月更新将月复位次数清0，结果复位次数小1次
	//MC(0,ADDR_TerminalCongeal_Time,7);//上次冻结时间
	
	

}




void DL698_INIT(u32 Command)//初始化,入口Command:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
{
	switch(Command)
	{
		case 0:
			return;
		case 1://硬件初始化
			return;
		case 2://数据区初始化
			Init_ALLDATA();//终端全部数据初始化(清0)
			return;
		case 3://参数及全体数据区初始化(即恢复至出厂配置)
		case 4://4/5=参数(除与系统主站通信有关的)及全体数据区初始化
		case 5://4/5=参数(除与系统主站通信有关的)及全体数据区初始化
			Init_ALLDATA();//终端全部数据初始化(清0)
			DL698_SETINIT(Command,0);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
			return;
		case 6://仅部份参数初始化
			DL698_SETINIT(Command,0);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
			return;
		case 7://7=参数初始化(档案和nand文件数据转移到新地址)
			DL698_SETINIT(Command,0);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
			return;
	}
}


#include "../Device/ATNo.h"
void Init_Terminal_SoftwareUpdate(void)//软件更新初始化
{
	//可能的相同AT命令的AT号变化需修正
	switch(UART1Ctrl->Task)
	{
		case 3://单帧数据正在发送
		case 6://多帧数据正在发送
			UART1Ctrl->ATCount=AT_MYNETWRITE;//发送数据
			break;
	}

	DL698_INIT(Terminal_UpDataInit);//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位

	//MS在BOOT更新，可能因选择NAND128kBuff和NAND4kBuff不相同,数据保护恢复可能错误,故在此先将数据写入FLASH
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
	NAND4kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
#else
	NAND128kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif
}




