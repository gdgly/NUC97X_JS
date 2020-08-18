
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_FMC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_ExtMem.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"

#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayError.h"
#include "../Display/DisplayFTP.h"
#include "../Display/DisplayLoop.h"

#include "../Device/MEMRW.h"
#include "Terminal_PowerDown.h"
#include "Terminal_Init.h"
#include "Terminal_Timer.h"
#include "Terminal698_Uart_Manage.h"

#include "../DL698/RM_TASK.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/CONGEAL_Record.h"
#include "../DL698/CALC_Voltage_Qualification_Rate.h"

#include "../DL698/EVENT_CALC.h"
#include "../DL698/EVENT_CALC_iMeter.h"
#include "../DL698/CALC_impIn.h"
#include "../DL698/CALC_TAG.h"
#include "../DL698/Contrl_Manage.h"
#include "../TEST/LOG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"

#include "../DL698/DL698TEST.h"
#include "../Test/Test_RAM.h"
#include "../MS/Data_Save.h"
#include "../DL698/DL698_JSON.h"

uint32_t SystemCoreClock = 16000000;//仅stm32f4xx_hal_rcc.c文件编译时需要,link时不需要
void _0(void)
{
}
void ERC14_Event(void);//终端停/上电事件

void RunLED(void)//运行LED指示灯
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	
	i=0;
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
		i=1;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
		i=1;
	}
	if(i==0)
	{
		Pin_OutL(PIN_RUNLED);
	}
	else
	{
		if(Comm_Ram->RTCBuff[0]&0x1)
		{
			Pin_OutH(PIN_RUNLED);
		}
		else
		{
			Pin_OutL(PIN_RUNLED);
		}
	}
}

void ACTION_RESET(void)//复位命令操作
{
	u32 PORTn;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->RESET!=0x55)//0x55=重启动(立即使用看门狗复位)
	{
		return;
	}
	PORTn=Comm_Ram->ACTION_RESET_PORTn;//操作复位的通信端口号
	switch(PORTn)
	{
		case RS232PORT://0//RS232
		case GPRSPORT://1//GPRS
		case RS485_1PORT://2//RS485_1通信口号
		case RS485_2PORT://3//RS485-2通信口号
		case RS485_3PORT://5//RS485_3通信口号(WI-FI)
		case IRPORT://6//红外通信口号
		case ENETPORT://10//以太网通信口号
		case SMSPORT://11//短信通信口号
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
			if(UARTCtrl->Task!=0)
			{//0=正在接收
				return;
			}
		#ifdef IC_NUC9xx
			//延时等待最后字节发送
			if(Comm_Ram->WaitUartCompleteMSTimer!=0)
			{
				return;
			}
		#endif
	}
	//延时等待最后字节发送
	Comm_Ram->GPMSTimer=1000/10;//18 通用(兼虚拟UART11) 每10MS减1定时器
	while(Comm_Ram->GPMSTimer)
	{
	}
	WWDT_RESET();//立即使能看门狗复位
	
}




void Terminal_Task(void)//I2分时任务(电网有电)
{
	u8 *p8;
	u32 *p32;
	//u32 i;
	
	p32=p32;
	p8=(u8*)(ADDR_I2Task);//I2主任务计数(原在SDRAM,因SDRAM死机后无法计数不运行SDRAM死机检查的可能故改放在IRAM)
  switch(p8[0])
  {
    case 0:
      MW(ADDR_MYMDHMS,ADDR_TFM_YMDHMS,6);//掉电前时钟
			Terminal_FMTimer();//掉电保持秒和分定时器
			Terminal_Timer();//终端非中断秒和分定时器
			RunLED();//运行LED指示灯
      break;
		case 1://终端停上电事件
			Event_Power();//终端停/上电事件
			break;
    case 2:
			RM_UpDate();//抄表参数更新;出口:判断,读采集档案配置表从flash到sdram,置位各PORTn各任务需重计算总需抄表数成功数等
      break;
    case 3:
			ACTION_RESET();//复位命令操作
			CALC_SwitchIn();//开关量输入
			FLOW_Count(GPRSPORT);//通信流量计数
			//FLOW_Count(ENETPORT);//通信流量计数
			POWER_Time();//供电时间
			break;
		case 4:
			CONGEAL_Record();//冻结
			break;
		case 5:
			CALC_Voltage_Qualification_Rate();//电压合格率计算
			break;
		case 6:
			EVENT_CALC_300F_3010();//电压电流逆相序事件
			EVENT_CALC_3003();//断相标志和事件(分项事件)
			
			break;
		case 7:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_impIn_Energy();//终端脉冲输入-电能
			Terminal_impIn_Demand();//终端脉冲输入-功率和最大需量
			Update_Group_Total_Data_240x();//更新总加组的数据,配置表是脉冲量
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_impIn_Energy();//终端脉冲输入-电能
				Terminal_impIn_Demand();//终端脉冲输入-功率和最大需量
				Update_Group_Total_Data_240x();//更新总加组的数据,配置表是脉冲量
			}
		#endif
			break;
		case 8:
			Terminal_Contrl_Manage();//终端控制管理(分任务运行)
			break;
		case 9:
			if(Terminal_Router->MinuteCollect!=0)
			{
				Terminal_PackJSON();
			}
			break;
		case 10:	
			//TaskReport_JsonFile(GPRSPORT);
			//TaskReport_JsonFile(ENETPORT);
			break;

    default:
#ifdef IC_STM32F4xx
	#if DEBUG==1
			//TEST
		#if RUN_DL698TEST_1==1//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度
			DL698TEST_1();//全部档案设为内部交采测试数据存储最大深度
		#endif
		#if RUN_DL698TEST_2==1//0无效,1=运行;测试601E规约库
			DL698TEST_2();//测试601E规约库
		#endif
		#if RUN_DL698TEST_USER==1//0无效,1=运行;用户版测试
			DL698TEST_USER();//用户版测试
		#endif
			//
		
			//SDRAM死机检查
			if(RAM_Test(ADDR_128KDATABUFF,1024))//RAM测试;返回:0=正确,1=错误
			{
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""SDRAM数据错误!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	#else
			if(RAM_Test(ADDR_128KDATABUFF,64))//RAM测试;返回:0=正确,1=错误
			{
	#endif
				Comm_Ram->GPMSTimer=1000/10;//18 通用(兼虚拟UART11) 每10MS减1定时器
				while(1)
				{
					Init_ExtMem();//sdram初始化
					SDRAM_FMC_Init();
					if(RAM_Test(ADDR_128KDATABUFF,64)==0)//RAM测试;返回:0=正确,1=错误
					{
						break;
					}
					if(Comm_Ram->GPMSTimer==0)//18 通用(兼虚拟UART11) 每10MS减1定时器
					{
						__disable_irq();
						p32=(u32*)(ADDR_Meter_SaveRam_Start+LEN_SaveRam-8);
						p32[0]=0x12345678;
						p32[1]=RAM_CRC32(ADDR_Meter_SaveRam_Start,LEN_SaveRam-4);//RAM中数据CRC32计算
						RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
						RTC->BKP0R|=0x5500;
						RTC->BKP19R=0x12345678;//停电上事件判断用上电标志;看门狗复位自动重启=0x12345678,其他值为上电
						//	RCC->CSR|=1;//LSI RC 振荡器开启
						while(IWDG->SR!=0)
						{
						}
						IWDG->KR=0x5555;
						IWDG->PR=0;//4 分频
						IWDG->RLR=1;
						IWDG->KR=0xAAAA;
						IWDG->KR=0xCCCC;
						while(1);
					}
				}
			}
#endif
      p8[0]=0;
      return;
  }
  p8[0]++;
}


__asm void Call_E2(u32 r0)//调用E2菜单,入口R0=PC地址
{
	PUSH {LR}
	LDR LR,=ADDR_SystemStack
	STR SP,[LR]
	CMP R0,#0
	BNE Call_E2_1
	LDR R0,=ADDR_Menu_ENTRY//E2入口地址
	LDR R0,[R0,#(4*1)];
Call_E2_1
	LDR LR,=Call_E2_End
	BX R0
Call_E2_End
	LDR LR,=ADDR_SystemStack
	MOV R0,#0
	STR R0,[LR]
	POP {PC}
	ALIGN
}

void Terminal_DisplayTask(void)//I2显示
{
	//菜单和终端循显
#if (IC_UC1698|IC_ST75161)//0=没,1=有
//	u32 i;
  u32 * p32;
#if (USER/100)==5//上海
	u8 *p8;
#endif

//ms_Type * ms;
//ms=Get_ms();
	
	
  if(MenuCount->Count==0)//菜单堆栈计数
  {//菜单堆栈计数=0
    if((Comm_Ram->ButtonVal&0x10)==0)
    {//无确认键按下,循显
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {
			#ifdef IC_STM32F4xx
				if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
			#endif
			#ifdef IC_NUC9xx
				if((FileDownload->USB0Task>=2)&&(FileDownload->USB0Task<=4))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
			#endif
				{//USB文件完成标志有效
					Comm_Ram->DisplayAllErrorTimer=0;
					Comm_Ram->DisplayScanTimer=5000/10;
					DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
					DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
				}
				else
				{
//					i=MRR(ADDR_FTPFileFlags+4,1);
//		      if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
//					{//FTP文件下载有效
//						DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
//						DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
//						DisplayFTP();//FTP文件下载显示
//					}
//					else
					{
//						if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
//						{
//							MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP文件下载且显示清除标志
//						}
					#if ErrorAutoDisplay==1//0=允许,1=禁止;系统错误自动立即显示
						if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
						{//不是工厂状态
							if(Comm_Ram->ErrorIC)
							{
								Comm_Ram->ErrorIC=0;
								Comm_Ram->DisplayAllErrorTimer=0;
							}
						}
					#endif
						if((Comm_Ram->ErrorIC)&&(Comm_Ram->DisplayAllErrorTimer))
						{//错误显示
							DOTMIXPMODE->Task=2;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
							DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayError();//硬件和软件模块错误显示
						}
						else
						{
						//循环显示
							DOTMIXPMODE->Task=4;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
							DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayLoop();//终端循显

						}
					}
				}
				//点阵显示输出
				if(DOTMIXPMODE->Task==0)
				{//上电屏幕
					Comm_Ram->DisplayScanTimer=30/10;
				}
				else
				{//其他屏幕
		    	Comm_Ram->DisplayScanTimer=1100/10;
				}
				
  			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
		  }
    }
    else
    {//确认键按下,进主菜单
			DOTMIXPMODE->Task=3;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
			DOTMIXPMODE->Mix=2;//2;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
      Comm_Ram->ButtonVal&=~0x10;//确认键使用后清除
      if(FileDownload->File&0x10)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
      {
        MenuCount->Count=1;
        p32=(u32 *)(ADDR_Menu_ENTRY);
        Stack->MenuStack[MenuCount->Count].PC=p32[1];
        Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
        Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
        Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
        Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
        Call_E2(0);//调用E2菜单
      }
			//点阵显示输出
	    Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
    }
  }
  else
  {//菜单堆栈计数!=0
		if(FileDownload->File&0x10)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
		{
			Call_E2(Stack->MenuStack[MenuCount->Count].PC);//调用E2菜单
		}
	  if(Comm_Ram->DisplayScanTimer==0x0)
	  {
		#if (USER/100)==5//上海
			p8=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
			if(p8[0]!=0)
			{
				DisplayClrRow(0,0);//清屏幕显示的指定行
				DisplayClrRow(9,9);//清屏幕显示的指定行
				DisplayLoop_TopICO(0);//顶图标和时钟显示
				DisplayLoop_BottomICO(9);//底图标显示
			}
		#endif
		#if (Project/100==2)&&(USER/100)==17//上海
				DisplayClrRow(0,0);//清屏幕显示的指定行
				DisplayClrRow(9,9);//清屏幕显示的指定行
				DisplayLoop_TopICO(0);//顶图标和时钟显示
				DisplayLoop_BottomICO(9);//底图标显示
		#endif
			//点阵显示输出
		 	 Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
		}
    if(Comm_Ram->MenuOutTimer==0)
    {//菜单退出 10MS定时器
      MenuCount->Count=0;
    }
  }
	
#endif//#if (IC_UC1698|IC_ST75161)//0=没,1=有
}


void Terminal_Main_PowerUp(void)//I2电网有电
{
/*
	u16 *p16;
	p16=(u16*)(ADDR_RUNSPEEDTEST);//循环运行速度测试(半字对齐)
	__disable_irq();
	p16[0]++;
	__enable_irq();
*/
//显示
	Terminal_DisplayTask();//I2显示
//UART
	Terminal_Uart_Manage();//Terminal_Uart管理

//分时任务
	Terminal_Task();//I2分时任务(电网有电)
}

void Terminal_Main_Battery(void)//I2电池供电
{
#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块
	return;
#else
	u32 i;
	if(Comm_Ram->BattWork_S_timer==0)//4 电池工作关机秒定时器
	{
		DeepPowerDown();//深度电源下降模式
	}
	else
	{
		Terminal_DisplayTask();//I2显示
	#ifdef IC_STM32F4xx
		if((RTC->BKP0R&0xff)<=1)//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
		{//无USB操作
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
	#endif
		if(Pin_Read(PIN_WKUP)!=0)
		{
			//1秒电源变化滤波
			for(i=0;i<1000;i++)
			{
				SoftDelayMS(1);//软件ms延时
				if(Pin_Read(PIN_WKUP)==0)
				{
					return;
				}
			}
			WWDT_RESET();//立即使能看门狗复位
		}
	}		
#endif
}
/*
const u32 JSON_ALIGN32_List[]=
{
	(u32)&JSON->g_Filesize,
	
};
	
const u32 JSON_ALIGN16_List[]=
{
	(u32)&JSON->CurFrzMetCount,
	(u32)&JSON->CurRealMetCount,
	(u32)&JSON->g_Block_Number,
	(u32)&JSON->g_Need_report_Block,
	(u32)&JSON->res2,
};

u32 Check_ALIGN32(u32* pList,u32 NUM)//检查4字节对齐:返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&3)
		{
			return 1;
		}
	}
	return 0;
}
u32 Check_ALIGN16(u32* pList,u32 NUM)//检查2字节对齐:返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&1)
		{
			return 1;
		}
	}
	return 0;
}
*/
void Terminal_Main(void)
{
	u32 i;

//---linux test----	
/*
	i=MRR(ADDR_DATABUFF,1);
	i++;
	MC(i,ADDR_DATABUFF,256);

	return;
*/	
//---linux test end----	
	
	
	i=i;
//---TEST---
//LOG(698,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//LOG(13762,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
//LOG(13763,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码

//	ABB_Password(0x12345678,0x90123456);//根据密匙key与远程通讯口令来计算加密口令字
	
//i=ADDR_6014+(9*512);
//if(i)
//{
//	i=i;
//}
//i=ADDR_6000;
//if(i)
//{
//	i=i;
//}
//DL698_SETINIT(7,0);
//	for(i=1;i<=NUM_RMmax;i++)
//	{
//		MWR(i,ADDR_DATABUFF,6);
//		MeterAutoReport_3011((u8*)ADDR_DATABUFF,1);
//	}
//	for(i=1;i<=NUM_RMmax;i++)
//	{
//		MWR(i,ADDR_DATABUFF,6);
//		MeterAutoReport_3011((u8*)ADDR_DATABUFF,1);
//	}
//	i=5;
//	MWR(i,ADDR_DATABUFF,6);
//	MeterAutoReport_3011((u8*)ADDR_DATABUFF,2);
	
//	Pin_Configure(PIN_ADC123_IN13,//引脚配置
//							0|//B3-B0:复用功能选择AF(0-15)
//				 (3<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
//				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
//				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
//				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
//				(0<<11));//B11  :当为输出时：0=输出0,1=输出1
//	i=ADCx(13,3,1,0);//ADC电压测量,入口:Channel=ADC通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址;返回:测量值(0xffffffff表示转换失败)
//	MWR(i,ADDR_VDD5V0,2);
	
//	ms_Type *ms;
//	
//	ms=Get_ms();
//	ms->msfile_format();
//i=FILENAMEmax_FLASH;
//if(i)
//{
//	i=i;
//}
//void RM_OK_FAIL(u32 PORTn,u32 OK);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//	i=ADDR_128KDATABUFF;
//	if(i)
//	{
//		i=i;
//	}
//	UARTCtrl_TypeDef *UARTCtrl;
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//	UARTCtrl->TaskID=1;
//	UARTCtrl->RMAddr[0]=1;
//	RM_OK_FAIL(RS485_2PORT,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//	UARTCtrl->RMAddr[0]=2;
//	RM_OK_FAIL(RS485_2PORT,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//	UARTCtrl->RMAddr[0]=3;
//	RM_OK_FAIL(RS485_2PORT,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//	UARTCtrl->RMAddr[0]=2;
//	RM_OK_FAIL(RS485_2PORT,1);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//	
	
//	recordFILE_write(0,(u8*)filedata,sizeof(filedata));//写记录文件,入口:ID=b31-b24按优先当前需执行的ID号,b23-b16方案类型,b15-b8方案编号,b7-b0文件计数;返回:写入字节数
//	u32 i;
//	RMokfailLIST_TypeDef rmOKFAILlist[4][8];
//	u8* p8;
//	

//	i=ADDR_Terminal_InDemandimp;
//	if(i)
//	{
//		i=i;
//	}
//	
//	MR(ADDR_128KDATABUFF,ADDR_6000,LENper_6000);
//	for(i=1;i<NUMmax_6000;i++)
//	{
//		MW(ADDR_128KDATABUFF,ADDR_6000+(i*LENper_6000),LENper_6000);
//	}

//	Event_Standard(0x3104,0,2);//标准事件记录
//	Event_Standard(0x3104,0,2);//标准事件记录
//	Get_EventRecord_OAD(ACSAMPLEPORT,0x31040201,(u8 *)ADDR_DATABUFF,1024*4);//读事件记录;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR

//	Event_Standard(0x3114,0,2);//标准事件记录;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
//	ADDR_Check();//ADDR_6000档案电能表地址重复检查;返回:0=没重复,1=有重复,并自动删除重复序号
//---TEST END---
	
#if ((Project/100)!=0)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  switch(Comm_Ram->POWER)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  {
	#if ((Project/100)>=2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
    case 0://0=电池供电
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//可充电池停止工作最电压4.40V
			{
			#ifdef IC_NUC9xx
				if(SuperCap_Voltage()<SuperCapBatteryStopMin)//超级电容电压ADC测量;返回:BCD格式2位小数
				{
					DeepPowerDown();//深度电源下降模式
				}
			#else
				DeepPowerDown();//深度电源下降模式
			#endif
			}
      Terminal_Main_Battery();//I2电池供电
      break;
		case 1://1=电网供电转电池供电重启动
			//DeepPowerDown();//深度电源下降模式
			WWDT_RESET();//立即使能看门狗复位(上电时必需PA3=1 ON ofter Dog power,否则不可使能WDT)
			break;
	#endif
    case 2://2=电网供电转电池供电
			//if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)//可充电池启动工作最小电压4.80V
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//可充电池停止工作最电压4.40V
			{
			#if HVER>=0x420//硬件版本号:4.20;增加超级电容电压ADC测量
				OffEMU();//关EMU(复位)
				OffA5V0();//关A5V0电源
				Meter_Ram->EMUInit=0;//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
			 #ifdef IC_STM32F4xx
				VDD5V0_Voltage();//VDD5V0电压ADC测量
				if(Comm_Ram->VDD5V0<SuperCapBatteryStopMin)//超级电容电压ADC测量;返回:BCD格式2位小数
			 #endif
			 #ifdef IC_NUC9xx
				i=SuperCap_Voltage();
				if((i<SuperCapBatteryStopMin)||(Comm_Ram->Factory==0x55))//超级电容电压ADC测量;返回:BCD格式2位小数
			 #endif
				{
					Battery_Data_Save();//电池工作时关机前数据保存
					DeepPowerDown();//深度电源下降模式
					return;
				}
			#else
				Battery_Data_Save();//电池工作时关机前数据保存
				DeepPowerDown();//深度电源下降模式
				return;
			#endif
			}
			if(Comm_Ram->DataSave==0)//数据保护:0=空,1=电网有电超过10秒数据需保护,2=数据已保护
			{
				DeepPowerDown();//深度电源下降模式
				return;
			}
			Terminal_PowerDown();//2=电网供电转电池供电
			Terminal_Uart_Manage();//Terminal_Uart管理
			Terminal_Timer();//终端非中断秒和分定时器
			RunLED();//运行LED指示灯
			Event_Power();//终端停/上电事件
			break;
    case 3://3=电网供电
      Terminal_Main_PowerUp();//I2电网有电
      break;
  }
#endif

}






