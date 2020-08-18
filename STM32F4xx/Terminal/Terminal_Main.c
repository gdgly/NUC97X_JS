
#include "Project.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_FMC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"

#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayError.h"
#include "../Display/DisplayFTP.h"
#include "../Display/DisplayLoop.h"

#include "../Device/MEMRW.h"
#include "Terminal_PowerDown.h"
#include "Terminal_Init.h"
#include "Terminal_Timer.h"
#include "Terminal_Uart_Manage.h"
#include "Terminal_Uart_3761.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "Terminal_Contrl_Manage.h"
#include "Average_Voltage.h"
#include "../Test/Test_RAM.h"



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
		if(Comm_Ram->TYMDHMS[0]&0x1)
		{
			Pin_OutH(PIN_RUNLED);
		}
		else
		{
			Pin_OutL(PIN_RUNLED);
		}
	}
}


void Terminal_Task(void)//I2分时任务(电网有电)
{
//	u32 i;
	u8 *p8;
	u32 *p32;
	
	p8=(u8*)(ADDR_I2Task);//I2主任务计数(原在SDRAM,因SDRAM死机后无法计数不运行SDRAM死机检查的可能故改放在IRAM)
  switch(p8[0])
  {
    case 0:
      MW(ADDR_TYMDHMS,ADDR_TFM_YMDHMS,6);//掉电前时钟
		#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
			if((Terminal_Ram->PowerTask==10)||(Terminal_Ram->PowerTask==100))//停电事件任务
			{//ERC14电源状态<10掉电
				MW(ADDR_TYMDHMS,ADDR_POWER_YMDHMS,6);//ERC14事件判定掉电前的时钟
			}
		#endif
			Terminal_FMTimer();//掉电保持秒和分定时器
			Terminal_Timer();//终端非中断秒和分定时器
			RunLED();//运行LED指示灯
      break;
		case 1://终端停上电事件
	#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
			if(Terminal_Ram->RMDelay_S_Timer==0)//上电后延时抄表秒定时器(同时用于上电事件可记录延时)
			{
				if(MRR(ADDR_ERC14SOUR,1)==0)//ERC14事件记录标志(上电)
				{
					MWR(0x14,ADDR_ERC14SOUR,1);//ERC14事件记录标志
					if((RTC->BKP0R>>8)&0xff)!=0x55)//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
					{
						ERC14_Event_End();//终端停/上电事件(上电),用专用事件数据缓冲
					}
				}
			}
	#endif
	#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
			ERC14_Event();//终端停/上电事件
	#endif
			break;
    case 2:
      Terminal_Init_Command();//终端初始化命令
      break;
    case 3:
			Terminal_Class1Data_Computer();//终端类1数据计算
			break;
		case 4:
			HourCongeal_AutoRW();//小时冻结自动IO读写或清0,完成后结果处理
			break;
		case 5:
			Terminal_Class2Data_Computer();//终端类2数据计算
			break;
		case 6:
			Class2Congeal_AutoRW();//类2冻结自动IO读写或清0,完成后结果处理
			break;
		case 7:
			Terminal_Statistic();//终端统计计算,分任务运行
			break;
		case 8:
			Terminal_Contrl_Manage();//终端控制管理(分任务运行)
			break;
		case 9:
			Average_Voltage();//内部交采1分钟平均电压计算和曲线冻结
			break;
		case 10:
			ERC10_Event();//电压回路异常(断相、失压)	
			break;

    default:
			//SDRAM死机检查
			if(RAM_Test(ADDR_128KDATABUFF,256))//RAM测试;返回:0=正确,1=错误
			{
				Comm_Ram->DisplayAllErrorTimer=3;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""SDRAM ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				Comm_Ram->GPMSTimer=1000/10;//18 通用(兼虚拟UART11) 每10MS减1定时器
				while(1)
				{
					SDRAM_FMC_Init();
					if(RAM_Test(ADDR_128KDATABUFF,128*1024)==0)//RAM测试;返回:0=正确,1=错误
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
/*			
			//nand检查
			p32=(u32*)(ADDR_128KDATABUFF);
//			for(i=0;i<(128*1024/4);i++)
//			{
//				p32[i]=i;
//			}
//			MW(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
			MR(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
			for(i=0;i<(128*1024/4);i++)
			{
				if(p32[i]!=i)
				{
					break;
				}
			}
			if(i<(128*1024/4))
			{
				Comm_Ram->DisplayAllErrorTimer=3;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""NAND ERROR");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			}
*/
			//
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
	u32 i;
  u32 * p32;
#if (USER/100)==5//上海
	u8 *p8;
#endif

  if(MenuCount->Count==0)//菜单堆栈计数
  {//菜单堆栈计数=0
    if((Comm_Ram->ButtonVal&0x10)==0)
    {//无确认键按下,循显
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {
				if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
				{//USB文件完成标志有效
					Comm_Ram->DisplayAllErrorTimer=0;
					Comm_Ram->DisplayScanTimer=5000/10;
					DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
					DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
				}
				else
				{
					i=MRR(ADDR_FTPFileFlags+4,1);
		      if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
					{//FTP文件下载有效
						DOTMIXPMODE->Task=1;//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
						DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
						DisplayFTP();//FTP文件下载显示
					}
					else
					{
						if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
						{
							MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP文件下载且显示清除标志
						}
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
/*
//自测命令
	if(Comm_Ram->SelfTestCommand==0x55555555)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[70])();//调用E1
		}
	#else
		SelfTest1();//终端自测1(硬件测试)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart管理
		Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
		Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761接收发送
		}
		return;
	}
	if(Comm_Ram->SelfTestCommand==0xAAAAAAAA)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[71])();//调用E1
		}	
	#else
		SelfTest2();//终端自测2(FLASH)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart管理
		Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
		Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761接收发送
		}
		return;
	}
	if(Comm_Ram->SelfTestCommand==0x5555AAAA)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[72])();//调用E1
		}	
	#else
		SelfTest3();//终端自测3(FLASH寿命测试)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart管理
		Uart_3761_RxTx(RS485_1PORT);//UART_3761接收发送
		Uart_3761_RxTx(RS485_2PORT);//UART_3761接收发送
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761接收发送
		}
		return;
	}
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
#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块
	return;
#else
	if(Comm_Ram->BattWork_S_timer==0)//4 电池工作关机秒定时器
	{
		DeepPowerDown();//深度电源下降模式
	}
	else
	{
		Terminal_DisplayTask();//I2显示
		if((RTC->BKP0R&0xff)<=1)//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
		{//无USB操作
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
  	if(Pin_Read(PIN_WKUP)!=0)
  	{
			WWDT_RESET();//立即使能看门狗复位
		}
	}		
#endif
}



void Terminal_Main(void)
{
	
#if ((Project/100)!=0)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  switch(Comm_Ram->POWER)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  {
	#if ((Project/100)>=2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
    case 0://0=电池供电
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//可充电池停止工作最电压4.40V
			{
				DeepPowerDown();//深度电源下降模式
			}
      Terminal_Main_Battery();//I2电池供电
      break;
		case 1://1=电网供电转电池供电重启动
			DeepPowerDown();//深度电源下降模式
			break;
	#endif
    case 2://2=电网供电转电池供电
			if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)//可充电池启动工作最小电压4.80V
			{
				DeepPowerDown();//深度电源下降模式
				return;
			}
			Terminal_PowerDown();//2=电网供电转电池供电
			break;
    case 3://3=电网供电
      Terminal_Main_PowerUp();//I2电网有电
      break;
  }
#endif

}
