
//终端电源下降
#include "Project.h"
#include "Terminal_PowerDown.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Device/GPRS_3763.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "Terminal_Uart_Manage.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_Uart_3762_Manage.h"
#include "Terminal_ReadMeter_3762.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../MS/Data_Save.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"



	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif


#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块
void Terminal_PowerDown(void)//2=电网供电转电池供电
{
}
#else
void Terminal_PowerDown(void)//2=电网供电转电池供电
{
	//u32 i;
	u32 PORTn;
	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;
	
	PORTn=0;
	if(UART1Ctrl->LinkTask>=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		PORTn=GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask=1;//1=关总电源
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask>=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		PORTn=ENETPORT;
	}
	else
	{
		UARTCtrl->LinkTask=1;//1=关总电源
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Ram->PowerDownTask)//电源掉电过程任务
	{
		case 0://掉电事件产生
			Terminal_Ram->PowerTask=0;//停电事件任务
			while(1)
			{
				p32=(u32*)(ADDR_HourCongeal_AutoRW);
				if(p32[3]==0)
				{//R2长度,值=0表示操作完成
					p32=(u32*)(ADDR_Class2Congeal_AutoRW);
					if(p32[3]==0)
					{//R2长度,值=0表示操作完成
					#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
						if(MRR(ADDR_ERC14SOUR,1)!=0)//ERC14上电事件已记录标志
					#else
						if(Terminal_Ram->PowerNote!=0)//上电事件记录:0=没记录,!=0已记录;
					#endif
						{
							ERC14_Event_Start(0xee);//终端停/上电事件(停电);入口:上电记录时事件记数指针需加1标志,0=不加1,0xee=需加1
							break;
						}
						else
						{
							break;
						}
					}
				}
				HourCongeal_AutoRW();//小时冻结自动IO读写或清0,完成后结果处理
	      Class2Congeal_AutoRW();//类2冻结自动IO读写或清0,完成后结果处理
				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
			}
			Comm_Ram->BattWork_S_timer=60;//4 电池工作关机秒定时器
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 1://检查当前是否在线
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;//1=关电源
				UART1Ctrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
				UnInit_GPRS_3763();//GPRS模块引脚为输入下拉
				OffR5V0();//关R5V0电源
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				UARTCtrl->LinkTask=1;//1=关电源
				UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
				UnInit_STM32F4xx_MAC();
				OffE5V0();//关E5V0电源
				Terminal_Ram->PowerDownTask=50;//电源掉电过程任务.无上报
			}
			else
			{
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 2://在定时内等待上报
			if(Terminal_Ram->PowerDownSubTask==0)//电源掉电过程子任务
			{
				//关闭非连接口电源
				Terminal_Ram->PowerDownSubTask=1;//电源掉电过程子任务
				if(PORTn==GPRSPORT)
				{
			#if ((USER/100)==5)&&(Project==300)
			//上海专变SIMCOM 4G模块使用了3.3V电源故不关
					
			#else
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=关电源
					UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
					UnInit_STM32F4xx_MAC();
					OffE5V0();//关E5V0电源
			#endif
				}
				else
				{
					UART1Ctrl->LinkTask=1;//1=关电源
					UART1Ctrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
					UnInit_GPRS_3763();//GPRS模块引脚为输入下拉
					OffR5V0();//关R5V0电源
				}
			}
			Terminal_Uart_Manage();//Terminal_Uart管理
			Terminal_AFN0CF2_Computer();//终端类1数据F2,终端日历时钟
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {//点阵显示输出
		    Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
				DisplayClr();//清屏
				DisplayLoop_TopICO(0);//顶图标和时钟显示
				DisplayLoop_BottomICO(9);//底图标显示
				DisplayString(1,0,0,(u8 *)"电网停电正在上报...");//显示字符串
  			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
			}
    	if(Pin_Read(PIN_WKUP))
    	{
			RESET:
				//RAM数据无效
				p32=(u32*)((ADDR_Meter_SaveRam_Start)+LEN_SaveRam-8);
				p32[0]=0;
				p32[1]=0;
				//引脚等硬件复位标志以产生事件
				RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
				__disable_irq();
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
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 3://发退出登录
			Comm_Ram->BattWork_S_timer=10;//等待发发退出登录定时
			Terminal_Uart_Manage();//Terminal_Uart管理
			UARTCtrl->LinkTask=102;
			Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 以太网心跳秒定时器
			Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS心跳秒定时器
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 4://等待退出登录
			Terminal_Uart_Manage();//Terminal_Uart管理
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {//点阵显示输出
		    Comm_Ram->DisplayScanTimer=1000/10;
				DisplayClr();//清屏
				DisplayLoop_TopICO(0);//顶图标和时钟显示
				DisplayLoop_BottomICO(9);//底图标显示
				DisplayString(1,0,0,(u8 *)"电网停电退出登录...");//显示字符串
  			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
			}
    	if(Pin_Read(PIN_WKUP)!=0)
    	{
				goto RESET;//立即使能看门狗复位
			}
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				Battery_Data_Save();//电池工作时关机前数据保存
				DeepPowerDown();//深度电源下降模式
			}
			if(UARTCtrl->LinkTask==103)
			{
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 5://关机
			UART1Ctrl->LinkTask=104;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 6://结束
			Terminal_Uart_Manage();//Terminal_Uart管理
    	if(Pin_Read(PIN_WKUP))
    	{
				goto RESET;//立即使能看门狗复位
			}
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				Battery_Data_Save();//电池工作时关机前数据保存
				DeepPowerDown();//深度电源下降模式
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//开总电源(等待关机延时)
			{
				Battery_Data_Save();//电池工作时关机前数据保存
				DeepPowerDown();//深度电源下降模式
			}
			break;
		
		case 50://无上报
			//点阵显示输出
	    Comm_Ram->DisplayScanTimer=1000/10;
			DisplayClr();//清屏
			DisplayString(1,0,0,(u8 *)"电网停电");//显示字符串
			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
			Comm_Ram->BattWork_S_timer=5;//4 电池工作关机秒定时器
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 51://等待定时后关机
    	if(Pin_Read(PIN_WKUP))
    	{
				goto RESET;//立即使能看门狗复位
			}
			if(Comm_Ram->ButtonVal&0x3f)//按键状态(使用后清0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出,B6=设置,B7=工厂,B8=表盖,B9=端盖
    	{
				goto RESET;//立即使能看门狗复位
			}
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				Battery_Data_Save();//电池工作时关机前数据保存
				DeepPowerDown();//深度电源下降模式
			}
			break;
			
		default:
			Terminal_Ram->PowerDownTask=0;//电源掉电过程任务
			break;
	}
}

#endif//#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=通信模块




#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
u32 VoltageComp(u32 V)//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
{
	u32 i;
	u16 *p16;
	GetACSample(0x0201ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
	p16=(u16 *)(Get_ADDR_UARTnTx(ACSAMPLEPORT)+14);
	for(i=0;i<3;i++)
	{
		if(p16[i]>=V)
		{
			break;
		}
	}
	if(i>=3)
	{
		return 0;
	}
	return 1;
}

u32 EventTimerComp(void)//停电事件时间比较;返回0=时间相同,1=不同且有表没抄,2=不同且全部表已抄
{
	u32 i;
	u32 x;
	u32 z;
	u32 t1;
	u32 t2;
	u8 *p8;
	u16 *p16;
	
	p8=(u8*)(ADDR_DATABUFF);
	p16=(u16*)(ADDR_F246PnList);//要抄读的掉电记录数据Pn列表
	z=0;//已抄表数
	for(i=0;i<F246MaxPn;i++)
	{
		if(p16[i]==0)
		{//不抄
			z++;//已抄表数
		}
		else
		{//要抄
			MR(ADDR_DATABUFF,ADDR_AFN0CF246_Sec+3+(15*i),6);//电表掉电时间
			if(p8[1]==0xee)
			{//没抄表
				continue;
			}
			MR(ADDR_DATABUFF+6,ADDR_AFN0CF246_Sec+3+(15*i)+6,6);//电表上电时间
			t1=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
			if(t1!=0)
			{//开始时间非法
				continue;
			}
			t1=Check_YMDHMS(MRR(ADDR_DATABUFF+6,6));//年月日时分秒合法检查,返回0合法,1非法
			if(t1!=0)
			{//结束时间非法
				continue;
			}
			z++;//已抄表数
			
			x=MRR(ADDR_AFN04F98+4,2);//停电事件起止时间偏差限值	BIN	2	单位为分钟
			x*=60;
			t1=TimeDifference(ADDR_ERC14SOUR+16,ADDR_DATABUFF);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
			if(t1>>31)
			{
				t1=~t1;
				t1++;
			}
			if(t1<x)
			{
				t1=TimeDifference(ADDR_ERC14SOUR+22,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(t1>>31)
				{
					t1=~t1;
					t1++;
				}
				if(t1<x)
				{
					t1=TimeDifference(ADDR_ERC14SOUR+16,ADDR_ERC14SOUR+22);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
					t2=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算
					if(t2>>31)
					{//开始时间-结束时间倒置非法
						continue;
					}
					t1-=t2;
					if(t1>>31)
					{
						t1=~t1;
						t1++;
					}
					x=MRR(ADDR_AFN04F98+6,2);//停电事件时间区段偏差限值	BIN	2	单位为秒
					if(t1<x)
					{
						return 0;//有效停电
					}
				}
			}
		}
	}
	if(z<F246MaxPn)
	{//没全抄抄表
		return 1;//1=不同且有表没抄
	}
	return 2;//2=不同且全部表已抄
}

void ERC14_Event(void)//终端停/上电事件
{
	u32 i;
	u32 x;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
#if (USER/100)==5//用户标识:上海系
	u32 *p32;
#endif
	
#if (USER/100)==5//用户标识:上海系
	//终端定时复位
	i=Terminal_Ram->PowerTask;//停电事件任务
	if((i==2)||(i>=99))
	{
		x=MRR(ADDR_TERMINAL,2);//取终端地址
		i=(x%7);//星期
		if(i==Comm_Ram->TYMDHMS[6])//当前周0-6
		{
			i=(x%40);//每5分钟为单位
			i*=5;
			x=i/60;
			x+=20;//在每天20:00-23:20时间段
			i%=60;
			x=hex_bcd(x);
			i=hex_bcd(i);
			x<<=8;
			x|=i;
			if(Comm_Ram->TYMDHMS[0]<=0x05)
			{//当前秒值在0-5秒
				i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);
				if(i==x)
				{//时分相同
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
					SoftDelayMS(5000);//软件ms延时
					WWDT_RESET();//立即使能看门狗
				}
			}
		}
	}
#endif
	
	i=((RTC->BKP0R>>8)&0xff);//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
	if(i!=0x55)
	{//引脚等硬件复位
		switch(Terminal_Ram->PowerTask)//停电事件任务
		{
			case 0://掉电到上电
				i=MRR(ADDR_AFN04F98+8,2);//终端停电发生电压限值
				i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
				if(i!=0)
				{
					if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
					{
						//清抄到的电表停上电记录
						MC(0xee,ADDR_AFN0CF246_Sec,15*F246MaxPn);//最近1次掉电记录(精确到秒)
						//RS485-1初始
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
						ClrRMTaskCompletes_RS485(RS485_1PORT,0,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
						UARTCtrl->RMTaskC=0;//当前抄表任务号0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						UARTCtrl->RMTaskFlags=1;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
						UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
						UARTCtrl->FnCount=0;//485抄表Fn计数
						UARTCtrl->RMCount=0;//485抄表命令计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
						UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
						UARTCtrl->AutoSearchTask=0;//自动搜表任务
						UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						//RS485-2初始
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
						ClrRMTaskCompletes_RS485(RS485_2PORT,0,1);//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号,All=0只清失败,=1清全部
						UARTCtrl->RMTaskC=0;//当前抄表任务号0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						UARTCtrl->RMTaskFlags=1;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
						UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
						UARTCtrl->FnCount=0;//485抄表Fn计数
						UARTCtrl->RMCount=0;//485抄表命令计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
						UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
						UARTCtrl->AutoSearchTask=0;//自动搜表任务
						UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						//RS485-4(载波)初始
				#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
						ClrRMTaskComplete_3762(0,1);//清端口抄表任务完成标志;RMTask=任务号,All=0只清失败,=1清全部
						UARTCtrl->RMTaskC=0;//当前抄表任务号0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						UARTCtrl->RMTaskFlags=1;//当前抄读任务数据分类(同抄表库):B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
						//UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485抄表电能表/交流采样装置序号(半字对齐)
						UARTCtrl->FnCount=0;//485抄表Fn计数
						UARTCtrl->RMCount=0;//485抄表命令计数
						UARTCtrl->ReRMCount=0;//485抄表命令重发计数
						UARTCtrl->RMError=0;//485抄表本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
						UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
						UARTCtrl->AutoSearchTask=0;//自动搜表任务
						UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
						for(i=0;i<MaxRS485AddCarrierPn;i++)
						{
							Terminal_Note->List[i].FnCount=0;//抄表Fn计数
							Terminal_Note->List[i].RMCount=0;//抄表数据标识计数
							Terminal_Note->List[i].ReRMCount=0;//重发计数
							Terminal_Note->List[i].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
						}
						if(Terminal_Router->RouterInitTask>=200)//路由初始化
						{
							Terminal_Router->RouterInitTask=0;//路由初始化
						}
						UARTCtrl->RMTaskN|=0x2;//每日任务
				#endif
						i=MRR(ADDR_AFN04F97+2,1);//停电事件抄读时间限值	BIN	1	单位为分钟
						i*=60;
						p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1停电事件抄读时间限值秒定时器(紧接上抄表定时器不分开)
						p16[0]=i;//RS485-1
						p16[1]=i;//RS485-2
						p16[2]=i;//RS485-3
				#if (USER/100)==0//电科院测试
						//载波通道总是无启抄
						p16[3]=0;//RS485-4(载波)
				#else
						p16[3]=i;//RS485-4(载波)
				#endif
						i=MRR(ADDR_AFN04F98+10,2);//终端停电恢复电压限值
						i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
						if(i!=0)
						{
						#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
							MR(ADDR_ERC14SOUR+16,ADDR_POWER_YMDHMS,6);//ERC14上电记录用停电时时间:年月日时分秒
							if(MRR(ADDR_AFN04F297_Ext,1)!=0)
							{//终端精确对时开关开启
								if(MRR(ADDR_ERC14SOUR+22,6)==0)
								{
									MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14上电记录用上电时时间:年月日时分秒
								}
								if(Terminal_Ram->RMDelay_S_Timer==0)//上电后延时抄表秒定时器(同时用于上电事件可记录延时)
								{
									if((UART1Ctrl->LinkTask!=101)&&(UART10Ctrl->LinkTask!=101))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
									{
										Terminal_Ram->PowerTask=1;//停电事件任务
									}
								}
								else
								{
									if(UART1Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
									{
										//请求主站时钟
										Terminal_AutoAFN0AF299(GPRSPORT);//辽宁、浙江扩展AFN0AF299终端请求主站时钟
									}
									if(UART10Ctrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
									{
										//请求主站时钟
										Terminal_AutoAFN0AF299(ENETPORT);//辽宁、浙江扩展AFN0AF299终端请求主站时钟
									}
								}
							}
							else
							{
								MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14上电记录用上电时时间:年月日时分秒
								Terminal_Ram->PowerTask=1;//停电事件任务
							}
						#else
							MR(ADDR_ERC14SOUR+16,ADDR_POWER_YMDHMS,6);//ERC14上电记录用停电时时间:年月日时分秒
							MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14上电记录用上电时时间:年月日时分秒
							Terminal_Ram->PowerTask=1;//停电事件任务
						#endif
						}
						else
						{
							Terminal_Ram->PowerTask=99;//停电事件任务;99=等待电压到恢复值
						}
					}
					break;
				}
				Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
				if(MRR(ADDR_AFN04F297_Ext,1)==0)
				{//终端精确对时开关关闭
					Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
				}
			#else
				Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
			#endif
				break;
			case 1://计算停电时间
				x=Check_YMDHMS(MRR(ADDR_ERC14SOUR+16,6));//年月日时分秒合法检查,返回0合法,1非法
				if(x==0)
				{
					x=Check_YMDHMS(MRR(ADDR_ERC14SOUR+22,6));//年月日时分秒合法检查,返回0合法,1非法
					if(x==0)
					{
						i=TimeDifference(ADDR_ERC14SOUR+16,ADDR_ERC14SOUR+22);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
						if((i>>31)==0)
						{
							x=MRR(ADDR_AFN04F98,2);//停电时间最小有效间隔分
							x*=60;
							if(i>=x)
							{//大于最小有效间隔分
								x=MRR(ADDR_AFN04F98+2,2);//停电时间最大有效间隔	BIN	2	单位为分钟
								x*=60;
								if(i<x)
								{//小于停电时间最大有效间隔
									Terminal_Ram->F246PnListUpDate=0xFF;//要抄读的掉电记录数据Pn列表已更新标志,0=要更新,!=0已更新
									F246PnList();//要抄读的F246掉电记录数据Pn列表
								#if ((USER/100)==7)//用户表示：浙江	
									ERC14_Event_End(1);//终端停/上电事件(上电);入口:属性标志
									Terminal_Ram->PowerNote=0xff;//上电记录
								#endif
									Terminal_Ram->PowerTask=10;//停电事件任务
									break;
								}
							}
						}
					}
				}
				//D0=间隔异常,D1=比对无效
				ERC14_Event_End(0);//终端停/上电事件(上电);入口:属性标志
				Terminal_Ram->PowerNote=0xff;//上电记录
				Terminal_Ram->PowerTask=100;//停电事件任务
				break;
			case 2://有电到停电
				i=MRR(ADDR_AFN04F98+8,2);//终端停电发生电压限值
				i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
				if(i!=0)
				{
					Terminal_Ram->PowerTask=100;//停电事件任务
					break;
				}
				if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
				{//停电
					ERC14_Event_Start(0);//终端停/上电事件(停电);入口:上电记录时事件记数指针需加1标志,0=不加1,0xee=需加1
				
				#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				  //关载波路由电源
					i=				0|//B3-B0:复用功能选择AF(0-15)
							 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(0<<11);//B11  :当为输出时：0=输出0,1=输出1	
					Pin_Configure(PIN_UART4_RX,i);//RXD输入,输入下拉
					Pin_Configure(PIN_UART4_TX,i);//TXD输入,输入下拉
					Pin_Configure(PIN_PLC_RST,i);//PLC_RST,输入下拉
					Pin_Configure(PIN_PLC_PWR,i);//PLC_PWR,输入下拉
					Pin_Configure(PIN_PLC_ID,i);//PLC_ID,输入下拉
					Terminal_Router->RouterInitTask=0;//路由初始化
				#endif
					Terminal_Ram->PowerNote=0;//上电记录
					Terminal_Ram->PowerTask=0;//停电事件任务
				}
				break;
				
			//ERC14电源状态<10掉电
			case 10://10=等待抄表完成比对电能表
				i=MRR(ADDR_AFN04F98+10,2);//终端停电恢复电压限值
				i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
				if(i==0)
				{
					Terminal_Ram->PowerTask=99;//等待电压到恢复值
					break;
				}
				i=EventTimerComp();//停电事件时间比较;返回0=时间相同,1=不同且有表没抄,2=不同且全部表已抄
				if(i==0)
				{
					ERC14_Event_End(3);//终端停/上电事件(上电);入口:属性标志
					Terminal_Ram->PowerNote=0xff;//上电记录
					Terminal_Ram->PowerTask=100;//停电事件任务
					break;
				}
				if(i==1)
				{//没完成
					//485-1
					if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//0x55=工厂模式
					{
					}
					else
					{
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
						if(UARTCtrl->RMTaskC==0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{
							break;//没完成上电抄表
						}
					}
					//485-2
					if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)
					{
					}
					else
					{
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
						if(UARTCtrl->RMTaskC==0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{
							break;//没完成上电抄表
						}
					}
					//载波
				#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(Terminal_Router->RouterInitTask<200)
					{
						break;
					}
					if(Terminal_Router->NoteNum!=0)
					{//节点表不为空
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
						if(UARTCtrl->RMTaskC==0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
						{
							break;//没完成上电抄表
						}
					}
				#endif
				}
				//无效停电
			#if ((USER/100)==7)//用户表示：浙江	
				ERC14_Event_End(0x81);//终端停/上电事件(上电);入口:属性标志,浙江重要事件需增加不上报标志用以识别(D8位)
			#else
				ERC14_Event_End(1);//终端停/上电事件(上电);入口:属性标志
			#endif
				Terminal_Ram->PowerNote=0xff;//上电记录
				Terminal_Ram->PowerTask=100;//停电事件任务
				break;
				
				
			case 99://等待电压到恢复值
				i=MRR(ADDR_AFN04F98+10,2);//终端停电恢复电压限值
				i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
				if(i!=0)
				{
					if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
					{
						Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
						Terminal_Ram->PowerTask=0;//停电事件任务
					}
					break;
				}
				Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
				break;
			case 100://停电判断
				i=MRR(ADDR_AFN04F98+8,2);//终端停电发生电压限值
				i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
				if(i==0)
				{//停电
					Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
					Terminal_Ram->PowerTask=2;//停电事件任务
				}
				break;
			default:
				Terminal_Ram->PowerTask=100;//停电事件任务
				break;
		}
	}
	else
	{//看门狗复位
		i=MRR(ADDR_AFN04F98+10,2);//终端停电恢复电压限值
		i=VoltageComp(i);//三相电压比较,返回:0=均小于参比值,1=有任一相大于参比值
		if(i!=0)
		{
			//停电事件抄读时间限值定时器=0秒,=不抄读
			
			//RS485-1初始
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
			//RS485-2初始
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->RMFlag=1;//0=抄表间隔暂停,1=抄表进行中
	#endif
	
			RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT看门狗复位标志,其他值为引脚等复位
			Terminal_Ram->PowerNote=0xff;//上电事件记录:0=没记录,!=0已记录;
			Terminal_Ram->PowerTask=100;//停电事件任务
			return;
		}
		Terminal_Ram->Power_S_Timer=5;//38 停电事件秒定时器
		Terminal_Ram->RMDelay_S_Timer=10;//10 上电后延时抄表秒定时器(同时用于上电事件可记录延时)
	}
}


#endif




//#define ADDR_AFN04F97     ADDR_AFN0CF3+32//停电数据采集配置参数P0
//停电数据采集标志	BS8	1	
//停电事件抄读时间间隔	BIN	1	单位为小时
//停电事件抄读时间限值	BIN	1	单位为分钟
//需要读取停电事件电能表个数n	BIN	2	
//测量点号1	BIN	2	第1块需要抄读的电能表
//……	……	……	……
//测量点号n	BIN	2	第n块需要抄读的电能表
//#define ADDR_AFN04F98     ADDR_AFN04F97+5+(2*MaxRS485AddCarrierPn)//停电事件甄别限值参数p0
//停电时间最小有效间隔	BIN	2	单位为分钟
//停电时间最大有效间隔	BIN	2	单位为分钟
//停电事件起止时间偏差限值	BIN	2	单位为分钟
//停电事件时间区段偏差限值	BIN	2	单位为秒
//终端停电发生电压限值	见附录A.7	2	V
//终端停电恢复电压限值	见附录A.7	2	V
//停电事件主动上报标志位	BS8	1	










