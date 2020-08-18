
//按键
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../Driver/Button.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../NUC97x/NUC97x_RTC.h"



void ButtonInit(void)//按键上电初始化
{
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	Pin_Configure(PIN_NEXTD,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_NEXTI,0);
	Pin_Configure(PIN_LEFT,0);//引脚配置
	Pin_Configure(PIN_RIGHT,0);//引脚配置
	Pin_Configure(PIN_ENTER,0);//引脚配置
	Pin_Configure(PIN_ESC,0);//引脚配置
	Pin_Configure(PIN_FACTORY,0);//引脚配置
//	Pin_Configure(PIN_SET,0);//引脚配置
#endif
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	Pin_Configure(PIN_FACTORY,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
#endif
}

void UnButtonPin(void)//按键引脚为输出0
{
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	Pin_Configure(PIN_NEXTD,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
	Pin_Configure(PIN_NEXTI,1<<4);
	Pin_Configure(PIN_LEFT,1<<4);//引脚配置
	Pin_Configure(PIN_RIGHT,1<<4);//引脚配置
	Pin_Configure(PIN_ENTER,1<<4);//引脚配置
	Pin_Configure(PIN_ESC,1<<4);//引脚配置
	Pin_Configure(PIN_FACTORY,1<<4);//引脚配置
//	Pin_Configure(PIN_SET,0);//引脚配置
#endif
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	Pin_Configure(PIN_FACTORY,//引脚配置
							0|//B3-B0:复用功能选择AF(0-15)
				 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
				 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
				 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
				 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
				(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
#endif
}


void Button(void)//按键输入
{
	u32 i;
	u32 x;
	os_Type *os;
	
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	
  i=Pin_Read(PIN_NEXTD);//读引脚,入口引脚使用名;返回0或1
  i|=Pin_Read(PIN_NEXTI)<<1;//读引脚,入口引脚使用名;返回0或1
  i|=Pin_Read(PIN_LEFT)<<2;//读引脚,入口引脚使用名;返回0或1
  i|=Pin_Read(PIN_RIGHT)<<3;//读引脚,入口引脚使用名;返回0或1
  i|=Pin_Read(PIN_ENTER)<<4;//读引脚,入口引脚使用名;返回0或1
  i|=Pin_Read(PIN_ESC)<<5;//读引脚,入口引脚使用名;返回0或1
//  i|=Pin_Read(PIN_SET)<<6;//读引脚,入口引脚使用名;返回0或1
	x=Pin_Read(PIN_FACTORY);//读引脚,入口引脚使用名;返回0或1
	x=~x;
	x&=1;
	i|=x<<7;//读引脚,入口引脚使用名;返回0或1
	
//	i|=Pin_Read(PIN_OPEN1)<<8;//读引脚,入口引脚使用名;返回0或1
//	i|=Pin_Read(PIN_OPEN2)<<9;//读引脚,入口引脚使用名;返回0或1
  //i=~i;
	//i&=0x3ff;
#endif
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	i=0;
	x=Pin_Read(PIN_FACTORY);//读引脚,入口引脚使用名;返回0或1
	x=~x;
	x&=1;
	i|=x<<7;//读引脚,入口引脚使用名;返回0或1
#endif

  if(Comm_Ram->ButtonCurrent!=i)
  {
		x=Comm_Ram->ButtonCurrent;
    Comm_Ram->ButtonCurrent=i;
    Comm_Ram->ButtonTimer=50/10;//50ms延时
    Comm_Ram->ButtonKeepTimer=2900/10;//3s延时,留100MS能在3秒时显示刷新
    Comm_Ram->MenuOutTimer=60000/10;//菜单退出60s延时
		//按键自测标志
		x^=i;//按键变化位
		i=~i;
		x&=i;//按键从按下(0)返回放开(1)认为OK
		Comm_Ram->TESTKEY|=x;
    return;
  }
  if(Comm_Ram->ButtonTimer==0x0)
  {
    Comm_Ram->ButtonVal=i;
    Comm_Ram->ButtonTimer=0xffff;//655秒后重置
/*
    if(Comm_Ram->SelfTestCommand==0x55555555)
    {//终端自测1(硬件测试)
			if((i&0x3ff)!=0x0)
			{
	      BUZZ_Generate(1);//蜂鸣产生,入口蜂鸣号
	      Comm_Ram->BUZZCTRL[8]=1;//蜂鸣器声音允许/禁止,=0禁止,!=0允许
			}
    }
*/
    if((i&0x7f)!=0x0)
    {//有键按下立即显示扫描
//      Comm_Ram->DisplayScanTimer=0;//显示扫描MS定时器
//			Comm_Ram->BackLightTimer=60;//背光灯显示秒定时器
			Comm_Ram->DisplayAllErrorTimer=0;//上电全显或系统错误显示秒定时器
			Comm_Ram->BattWork_S_timer=30;//电池工作关机秒定时器
	#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
			Meter_Ram->ButtonVal=Comm_Ram->ButtonVal;
      Meter_Ram->DisplayScanTimer=0;//显示扫描MS定时器
			Meter_Ram->BackLightTimer=60;//背光灯显示秒定时器
	#endif
	#if ((Project/100)!=0)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
      Comm_Ram->DisplayScanTimer=0;//显示扫描MS定时器
			Comm_Ram->BackLightTimer=60;//背光灯显示秒定时器
	#endif
    }
	#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	  //电能表用设置键
	  if((Comm_Ram->ButtonVal&0x40)!=0)
	  {
	    if(MRR(ADDR_MeterFMTimer,2)==0)
	    {
	      MWR(240,ADDR_MeterFMTimer,2);//240分定时
	      MC(1,ADDR_03300000+3,1);//设置键按下后第1个编程数据标志
	    }
	    else
	    {
	      MWR(0,ADDR_MeterFMTimer,2);//清定时
	    }
	  }
	#else
		//#if ((Project/100)>=5)
	  //终端用设置键
	  if((Comm_Ram->ButtonVal&0x40)!=0)
	  {
	    if(MRR(ADDR_TerminalFMTimer+12,2)==0)
	    {
	      MWR(240,ADDR_TerminalFMTimer+12,2);//240分定时
	      //MC(1,ADDR_03300000+3,1);//设置键按下后第1个编程数据标志
	    }
	    else
	    {
	      MWR(0,ADDR_TerminalFMTimer+12,2);//清定时
	    }
	  }
	#endif//#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	  //工厂模式
		if((Comm_Ram->ButtonVal&0x80)!=0)
		{
			MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60分定时
			MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60分定时
		}
  }
	//按键保持
/*
  if(Comm_Ram->ButtonKeepTimer==0x0)
  {
    Comm_Ram->ButtonKeep=Comm_Ram->ButtonVal;
    Comm_Ram->ButtonKeepTimer=0xffff;//655秒后重置
		if(Comm_Ram->ButtonVal&0x40)
		{//设置键按下保持3S清工厂模式定时
			MC(0,ADDR_MeterFMTimer+14,2);
			MC(0,ADDR_TerminalFMTimer+14,2);
		}
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
		Meter_Ram->ButtonKeep=Comm_Ram->ButtonVal;
#endif
  }
*/
  if(Comm_Ram->ButtonKeepTimer==0x0)
  {
    Comm_Ram->ButtonKeepTimer=1000/10;//3s延时,留100MS能在3秒时显示刷新
		if(Comm_Ram->ButtonCurrent&0x20)
		{//ESC键按下保持3S清工厂模式定时
			MC(0,ADDR_MeterFMTimer+14,2);
			MC(0,ADDR_TerminalFMTimer+14,2);
		#if MainProtocol==698
			if(Comm_Ram->Factory==0x55)//工厂状态
			{//工厂状态退出时，当前通信地址保存为出厂通信地址
				MR(ADDR_DATABUFF,ADDR_4001,LENmax_4001_Factory);
				MW(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001_Factory);
			}
		#endif
			//清yaffs文件
			if(Comm_Ram->MS_Task!=0)
			{//os已启动
				//ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH块擦除
				//WWDT_RESET();//立即使能看门狗复位
				os=Get_os();
				os->yaffs_unlinkdir("user/TEST/");
				os->yaffs_unlinkdir("user/Update/");
				//os->yaffs_unlinkdir("user/Program Files/");
				
			}
		}
	}
	
	#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	if(MRR(ADDR_MeterFMTimer+14,2)==0)
	{
		Comm_Ram->Factory=0x0;//工厂状态
	}
	else
	{
		Comm_Ram->Factory=0x55;//工厂状态
	}
	#else
	if(MRR(ADDR_TerminalFMTimer+14,2)==0)
	{
		Comm_Ram->Factory=0x0;//工厂状态
	}
	else
	{
		Comm_Ram->Factory=0x55;//工厂状态
	}
	#endif
	
}












