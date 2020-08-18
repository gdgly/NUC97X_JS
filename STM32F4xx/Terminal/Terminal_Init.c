

#include "Project.h"
#include "../Terminal/Terminal_Init.h"
#include "../MS/SetInit.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "Terminal_Uart_Manage.h"
#include "Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "Terminal_impIn.h"
#include "Terminal_Contrl_Manage.h"
#include "Terminal_Contrl_Out.h"
#include "AutoSearchMeter.h"
#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"

void Init_Terminal(void)//启动初始化
{
	u32 i;
  u32 x;
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
		p32[0]=0;
	}
	Terminal_Ram->S_PR=60;//1 60秒分频产生分
	Comm_Ram->BattWork_S_timer=30;//电池工作关机秒定时器

//以下只有在电网有电时进行
  if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  {
    return;
  }
	MC(0,ADDR_TerminalRealTimer,1);//0=掉电时间未计算,1=掉电时间已计算
	Terminal_Ram->SlipCYC_S_Timer=15;//60;//23 终端输入脉冲功率计算和需量脉冲滑移秒定时器
//数据初始化
	Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
	#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	if(MRR(ADDR_AFN04F297_Ext,1)!=0)
	{//终端精确对时开关开启
		i=MRR(ADDR_AFN04F297_Ext+9,1);
		i*=60;
		if(i==0)
		{
			i=10;
		}
		Terminal_Ram->RMDelay_S_Timer=i;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
		MC(0,ADDR_ERC14SOUR+22,6);//ERC14上电记录用上电时时间:年月日时分秒
	}
	else
	{
		Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
	}
	#endif
	#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
	#endif
#else
	Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
#endif
	
	Init_Class1Data_UpPower();//类1数据初始化(上电)
	Init_Class2Data_UpPower();//类2数据初始化(上电)
	Init_Class3Data_UpPower();//类3数据(事件计算）初始化(上电)
	MC(0xee,ADDR_Average_Voltage,6);//内部交采A,B,C相1分钟平均电压
	#if IC_SDRAM
	MR(ADDR_HOURFLASH_RAM_START,ADDR_HOURFLASH_START,LEN_HOURFLASH_RAM);//恢复终端每小时RAM数据保存到FLASH
	#endif
	PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
	AFN04F150_Computer();//测量点有效标志计算
	
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

	Pin_Configure(PIN_ATTR_LED,//上电时检查指示LED故初始输出0(亮)
					0|//B3-B0:复用功能选择AF(0-15)
		 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
		 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
		 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
		 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
		(0<<11));//B11  :当为输出时：0=输出0,1=输出1
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

#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	Terminal_Ram->ContrlLoopCheck_S_Timer=60;//36 控制输出回路检测有效等待秒定时器
#endif

	Comm_Ram->CheckLEDTimer=3000/10;//上电全部LED检查显示 每10MS减1定时器
/*
	p32=(u32*)(ADDR_AUTO_ERROR_RESTART);//自动检测到如SDRAM等错误看门狗复位标志
	if(p32[0]==0x12345678)
	{
		Pin_CFG(PIN_ATTR_LED,0+(0<<3)+(1<<5)+(1<<17)+(1<<18));//上电时检查指示LED故初始输出0(亮)
		Terminal_Ram->RS485_1_TxLED_MS_Timer=0;//10 RS485_1抄表发送LED指示MS定时器,每10MS减1定时器
		Terminal_Ram->RS485_1_RxLED_MS_Timer=0;//11 RS485_1抄表接收LED指示MS定时器,每10MS减1定时器
		Terminal_Ram->RS485_2_TxLED_MS_Timer=0;//12 RS485_2抄表发送LED指示MS定时器,每10MS减1定时器
		Terminal_Ram->RS485_2_RxLED_MS_Timer=0;//13 RS485_2抄表接收LED指示MS定时器,每10MS减1定时器
		Pin_CFG(PIN_ENET_LINK_LED,0+(0<<3)+(0<<5)+(1<<17)+(1<<18));//上电时检查指示LED故初始输出0(亮)
		p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
		p16[0]=0;//上电时置关机延时2s作为LED亮灯检查
		Terminal_Ram->ENETLED_MS_Timer=0;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
		Comm_Ram->CheckLEDTimer=0;//上电全部LED检查显示 每10MS减1定时器
	}
*/
//UART初始化
	Terminal_UartInit_Manage();//Terminal_Uart初始化管理
	i=MRR(ADDR_AFN04F97+2,1);//停电事件抄读时间限值	BIN	1	单位为分钟
	i*=60;
	p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1停电事件抄读时间限值秒定时器(紧接上抄表定时器不分开)
	p16[0]=i;//RS485-1
	p16[1]=i;//RS485-2
	p16[2]=i;//RS485-3
	p16[3]=i;//RS485-4(载波)
	
//终端复位次数
	Terminal_Reset_Count();//终端复位次数

//
#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
	MR(ADDR_ERC14SOUR+2,ADDR_TFM_YMDHMS+1,5);//ERC14上电记录用停电时时间:年月日时分
#endif

}

void Init_EnergyInc_Souser(void)//测量点电能增量计算用原始数据清为0xee
{
	u32 i;
//正向有功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//正向无功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
//反向有功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
	}
//反向无功电能增量
	for(i=0;i<MaxRS485Pn;i++)
	{
		MC(0xee,ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*i)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
	}
}
void Init_IncRemainder(void)//总加组总加电能量增量小数位清为0x0
{
//当日总加有功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF19_Remainder,(4*(TMaxTariff+1)*AFN0CF19MaxPn));
//当日总加无功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF20_Remainder,(4*(TMaxTariff+1)*AFN0CF20MaxPn));
//当月总加有功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF21_Remainder,(4*(TMaxTariff+1)*AFN0CF21MaxPn));
//当月总加无功电能量增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF22_Remainder,(4*(TMaxTariff+1)*AFN0CF22MaxPn));
//终端当前剩余电量(费)增量小数位(总加组号)
	MC(0x0,ADDR_AFN0CF23_Remainder,(4*(TMaxTariff+1)*AFN0CF23MaxPn));
}

void Init_ReadMeter(void)//抄表初始化
{
  UARTCtrl_TypeDef * UARTCtrl;
	
	Terminal_Ram->ACSampleRM_S_Timer=0;
	Terminal_Ram->RS4851RM_S_Timer=0;
	Terminal_Ram->RS4852RM_S_Timer=0;
	Terminal_Ram->RS4853RM_S_Timer=0;
	if(Get_LEN_UARTnCtrl(ACSAMPLEPORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
	if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
		UARTCtrl->MeterNo=0;//电能表/交流采样装置序号(半字对齐)
		UARTCtrl->FnCount=0;//Fn计数
		UARTCtrl->RMCount=0;//抄表命令计数
	}
}


extern void Terminal_SetInitLib(void);
void Terminal_Init(u32 Command)//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
{
	u32 *p32;
#if POINT_AFN0DF89F90F91!=0//总冻结点数
	u32 i;
	u32 buff;
#endif

	switch(Command)
	{
		case 1://硬件初始化
			p32=(u32*)(ADDR_HourCongeal_AutoRW);
			while(p32[3]!=0)
			{//R2长度,值=0表示操作完成
				HourCongeal_AutoRW();//小时冻结自动IO读写或清0,完成后结果处理
				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
			}
			p32=(u32*)(ADDR_Class2Congeal_AutoRW);
			while(p32[3]!=0)
			{//R2长度,值=0表示操作完成
				Class2Congeal_AutoRW();//类2冻结自动IO读写或清0,完成后结果处理
				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
			}
			MC(0x0,ADDR_AFN01Fx,5);//清命令
			
			WWDT_RESET();//立即使能看门狗复位
			return;
		case 2://数据区初始化
			break;
		case 3://参数及全体数据区初始化(即恢复至出厂配置)
		case 5://参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
			//参数初始化
			MC(0,ADDR_AFN04F9,ADDR_AFN04_End-ADDR_AFN04F9);
			MC(0,ADDR_AFN05_Start,ADDR_AFN05_End-ADDR_AFN05_Start);
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[118],p32[119],1);//参数初始化,入口ADDR_LIB参数初始化库地址,ADDR_LIBEnd库结束地址,Communicate通信参数0=不初始化,1=初始化
			AFN04F150_Computer();//测量点有效标志计算
			break;
		case 4://参数(除与系统主站通信有关的)及全体数据区初始化
			//参数初始化
			MC(0,ADDR_AFN04F9,ADDR_AFN04_End-ADDR_AFN04F9);//菜单密码,仅在恢复至出厂配置参数初始化时清0
			MC(0,ADDR_AFN05_Start,ADDR_AFN05_End-ADDR_AFN05_Start);
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[118],p32[119],0);//参数初始化,入口ADDR_LIB参数初始化库地址,ADDR_LIBEnd库结束地址,Communicate通信参数0=不初始化,1=初始化
			AFN04F150_Computer();//测量点有效标志计算
			break;
		case 6://6=仅部份参数初始化
			p32=(u32*)ADDR_Terminal_ENTRY;
			SetInit(p32[116],p32[117],0);//参数初始化,入口ADDR_LIB参数初始化库地址,ADDR_LIBEnd库结束地址,Communicate通信参数0=不初始化,1=初始化
			AFN04F150_Computer();//测量点有效标志计算
			ERC1_Event(2);//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
			MC(0x0,ADDR_AFN01Fx,5);//清命令
			return;
		default://否
			return;
	}
	//数据区初始化
	Init_EnergyInc_Souser();//测量点电能增量计算用原始数据清为0xee
	Init_IncRemainder();//总加组总加电能量增量小数位清为0x0
	Init_Class1Data_Command();//类1数据初始化(命令,全部清除)
	Init_Class2Data_Command();//类2数据初始化(命令,全部清除)
	Init_Class3Data_Command();//类3数据(事件）初始化(命令,全部清除)
	Init_Contrl_Command();//终端控制数据初始化(命令,全部清除)
	Init_impInData_Command();//终端电能脉冲输入数据初始化(命令,全部清除)
	Init_StatisticData_Command();//终端统计数据初始化(命令,全部清除)
	ERC1_Event(1);//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
	Init_ReadMeter();//抄表初始化
	Init_Class2SourceData_Command();//类2原始数据初始化(命令,全部清除为0xff)
	MC(0x0,ADDR_AFN01Fx,5);//清命令
	//内部交采1分钟曲线清0初始化
#if POINT_AFN0DF89F90F91!=0//总冻结点数
	i=ADDR_AFN0DF89F90F91;
	while(1)
	{
		buff=ADDR_MeterCurve_End-i;
		if(buff<4096)
		{
			break;
		}
		if((i&0xffff)==0)
		{//64K对齐
			if(buff>=65536)
			{
				EFLASH_Block_Erase(i);//块擦除,入口R0=要擦除的块在本块中的任意地址(uses AMS-A15 address lines,每块32KWORD)
				i+=65536;
				continue;
			}
		}
		EFLASH_Sector_Erase(i);//扇区擦除,入口R0=要擦除的扇区在本扇区中的任意地址(每扇区2KWORD)
		i+=4096;
	}
#endif
	
//RAM每小时数据保存到FLASH
#if IC_SDRAM
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);//上次RAM保存到FLASH的实时钟时(上电清为0,日月冻结后清为0xff使立即进行保存)
#endif

	Comm_Ram->RESET=0x55;//0x55=重启动(立即使用看门狗复位)
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

	Terminal_Init(Terminal_UpDataInit);//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
	Init_AFN0CF1();//初始化终端类1数据F1,版本信息//ERC1_Event(2);//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
	if((Terminal_UpDataInit==3)||(Terminal_UpDataInit==4))//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
	{//参数有初始化
		Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
		Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
		Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
	#endif
	}
	
	if(Comm_Ram->RESET==0x55)//0x55=重启动(立即使用看门狗复位)
	{
		if(Terminal_UpDataInit!=5)
		{
			Comm_Ram->RESET=0;//取消0x55=重启动(立即使用看门狗复位)
		}
	}
	
	
	//MS在BOOT更新，可能因选择NAND128kBuff和NAND4kBuff不相同,数据保护恢复可能错误,故在此先将数据写入FLASH
#include "../Device/NAND.h"
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
	NAND4kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
#else
	NAND128kBuff_Write_NAND(ADDR_DATABUFF,ADDR_Download_ENTRY,0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif
}


void Terminal_Init_Command(void)//终端初始化命令
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	
	u8 * p8;
	u8 * p8d;
	
	i=MRR(ADDR_AFN01Fx+4,1);
	if(i!=0x0)
	{
		x=MRR(ADDR_AFN01Fx,4);
		if(x==0x55555555)
		{
			Terminal_Init(i);//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
		}
		else
		{
			if(x>ADDR_ERAM_START)
			{
				if(x<ADDR_ERAM_END)
				{
					p8=(u8 *)(x);
					if(p8[0]==0x0)
					{//原下发命令的口Task=0(返回发送回答已完成)
						if(Terminal_Ram->ResetCommand_S_Timer==0)//延时10S
						{
	            Terminal_Init(i);//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
						}
					}
				}
			}
		}
	}
  //删除指定通信端口下的全部电表
  MR(ADDR_DATABUFF,ADDR_AFN05F53,31);
	p8=(u8 *)(ADDR_DATABUFF);
	for(i=0;i<31;i++)
	{
		if(MRR(ADDR_AFN05F53+i,1)==0x55)
		{
		#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(i==30)
			{//载波口,路由需同步
				Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
				Terminal_Ram->RouterSynDelay_S_Timer=0;//30;//29 路由档案同步延时执行秒定时器
			}
			Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
		#endif	
			MC(0,ADDR_AFN05F53+i,1);
			x=0;//已查电表数
			while((y=(MaxRS485AddCarrierPn-x))!=0x0)
			{
				if(y>151)
				{
					y=151;//4096/LEN_AFN04F10_Pn=151
				}
				MR(ADDR_DATABUFF,ADDR_AFN04F10+(LEN_AFN04F10_Pn*x),LEN_AFN04F10_Pn*y);
				p8d=(u8 *)(ADDR_DATABUFF);
				for(z=0;z<y;z++)
				{
					if((p8d[4]&0x1f)==(i+1))
					{
						p8d[0]=0;
						p8d[1]=0;
						p8d[2]=0;
						p8d[3]=0;
					}
					p8d+=LEN_AFN04F10_Pn;
				}
				MW(ADDR_DATABUFF,ADDR_AFN04F10+(LEN_AFN04F10_Pn*x),LEN_AFN04F10_Pn*y);
				x+=y;
			}
			PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
			AFN04F150_Computer();//测量点有效标志计算
		}
	}
	//复位测量点参数变更标志
	if(MRR(ADDR_AFN05F150,1)==0x55)
	{
		MC(0,ADDR_AFN04F150+256,256);
		MC(0,ADDR_AFN05F150,1);
		Terminal_Ram->AutoUpdataAFN04F10_S_Timer=0;//25 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
		Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer=0;//26 自动更新F10主动上报ERC3事件秒定时器;集中器通过本地的自动发现，完成测量点新增、变更或删除后，应立即置测量点参数变更标志（F150参数）相应位为1，并立即上报参数变更事件（ERC3）到主站,除非进行了测量点参数变更标志的复位，否则，每次上报参数变更事件后的m分钟（m=0~30，一般取10）之内，无论参数是否发生变更均不再次上报
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0;//0=无,1=有AFN04F10更新但在10分内不再上报情况
	}
	
#if (((Project/100)==2)||((Project/100)==5))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Terminal_Ram->AutoUpdataAFN04F10ERC3==0x55)//0=无,1=有AFN04F10更新但在10分内不再上报情况
	{
		AutoUpdataAFN04F10ERC3();//自动AFN04F10的电能表配置的ERC3事件
	}
#endif
	if(Comm_Ram->TYMDHMS[0]!=Terminal_Ram->AFN04F150_Computer_RTCS)
	{//每秒计算1次
		Terminal_Ram->AFN04F150_Computer_RTCS=Comm_Ram->TYMDHMS[0];
		AFN04F150_Computer();//测量点有效标志计算
	}
}

