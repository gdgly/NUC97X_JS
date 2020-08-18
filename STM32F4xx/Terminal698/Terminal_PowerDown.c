
//终端电源下降
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../DL698/DL698_DataDef.h"
#include "Terminal_PowerDown.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Device/GPRS_3763.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "Terminal698_Uart_Manage.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"

#include "../MS/Data_Save.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/EVENT_Record.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#endif




#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块
void Terminal_PowerDown(void)//2=电网供电转电池供电
{
}
#else
void Terminal_PowerDown(void)//2=电网供电转电池供电
{
//	u32 i;
	u32 PORTn;
//	u32 *p32;
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
//---test---
	//PORTn=ENETPORT;
//---
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Ram->PowerDownTask)//电源掉电过程任务
	{
		case 0://掉电事件产生
			//点阵显示输出
			DisplayStringAutoUp(0,9,(u8*)"\x0""电网停电");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=90;//60;//4 电池工作关机秒定时器
			Terminal_Ram->ENETHeartBeat_S_Timer=120;//2 以太网心跳秒定时器//收到心跳返回帧会再延时60秒
			Terminal_Ram->GPRSHeartBeat_S_Timer=120;//5 GPRS心跳秒定时器//收到心跳返回帧会再延时60秒
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 1://检查当前是否在线
		#if TEST_DISPVCAP==1//0无效,1=运行;显示超级电容电池充放电时间
			PORTn=GPRSPORT;
		#endif
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;//1=关电源
				UART1Ctrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
				UnInit_GPRS_3763();//GPRS模块引脚为输入下拉
				OffR5V0();//关R5V0电源
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				UARTCtrl->LinkTask=1;//1=关电源
				UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
			#ifdef IC_STM32F4xx
				UnInit_STM32F4xx_MAC();
				OffE5V0();//关E5V0电源
			#endif
			#ifdef IC_NUC9xx
				UnInit_NUC97x_MAC0();
				UnInit_NUC97x_MAC1();
				OffE5V0();//关E5V0电源
				Off2E5V0();//关E5V0电源
			#endif
				Terminal_Ram->PowerDownTask=50;//电源掉电过程任务.无上报
			}
			else
			{
				if(PORTn==GPRSPORT)
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=关电源
					UARTCtrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
				#ifdef IC_STM32F4xx
					UnInit_STM32F4xx_MAC();
					OffE5V0();//关E5V0电源
				#endif
				#ifdef IC_NUC9xx
					UnInit_NUC97x_MAC0();
					UnInit_NUC97x_MAC1();
					OffE5V0();//关E5V0电源
					Off2E5V0();//关E5V0电源
				#endif
				}
				else
				{
					UART1Ctrl->LinkTask=1;//1=关电源
					UART1Ctrl->PORT=1;//本机端口号,同时用于掉电时已连接过1次识别
					UnInit_GPRS_3763();//GPRS模块引脚为输入下拉
					OffR5V0();//关R5V0电源
				}
				//DisplayStringAutoUp(0,9,(u8*)"\x0""停电事件上报");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 2://在定时内等待上报
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 3://发退出登录
			DisplayStringAutoUp(0,9,(u8*)"\x0""停电退出登录");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=10;//等待发退出登录定时
			UARTCtrl->LinkTask=102;
			Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 以太网心跳秒定时器
			Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS心跳秒定时器
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 4://等待退出登录
			if((UARTCtrl->LinkTask==103)||(Comm_Ram->BattWork_S_timer==0))//电池工作关机秒定时器
			{
				Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			}
			break;
		case 5://关机
			UART1Ctrl->LinkTask=104;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			Comm_Ram->BattWork_S_timer=5;//等待模块关机定时
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 6://结束
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
			DisplayStringAutoUp(0,9,(u8*)"\x0""不在线不上报");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=5;//4 电池工作关机秒定时器
			Terminal_Ram->PowerDownTask++;//电源掉电过程任务
			break;
		case 51://等待定时后关机
			if(Comm_Ram->BattWork_S_timer==0)//电池工作关机秒定时器
			{
				if(PowerEvent->task<100)
				{//停电事件已记录完成
					Battery_Data_Save();//电池工作时关机前数据保存
					DeepPowerDown();//深度电源下降模式
				}
			}
			break;
			
		default:
			Terminal_Ram->PowerDownTask=0;//电源掉电过程任务
			break;
	}
}

#endif//#if ((Project/100)<2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块




//属性6（配置参数）∷=structure
//{
//停电数据采集配置参数  structure
//{
//采集标志  bit-string(SIZE(8))，
//停电事件抄读时间间隔（小时） unsigned ，
//停电事件抄读时间限值（分钟） unsigned，
//需要读取停电事件电能表       array TSA
//}，
//停电事件甄别限值参数  structure
//{
//停电时间最小有效间隔（分钟） long-unsigned，
//停电时间最大有效间隔（分钟） long-unsigned，
//停电事件起止时间偏差限值（分钟） long-unsigned，
//停电事件时间区段偏差限值（分钟） long-unsigned，
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
//}
//}
//采集标志：
//bit0：置“1”有效，置“0”无效；
//bit1：置“1”随机选择测量点，置“0”只采集设置的测量点。
//事件发生源∷=NULL
void RM_Meter_Event3106(void)//停/上电事件需抄读电表的配置序号(从0开始)
{
	u32 i;
	u32 j;
	u32 n;
	u32 x;
	u8* p8;
	
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	i=p8[6];//采集标志  bit-string(SIZE(8))
	if((i&0x80)==0)
	{//无效
		PowerEvent->NUMrm=0;//停/上电事件需抄读电表个数
		return;
	}
	j=0;//有效个数
	if((i&0x40)==0)
	{//只采集设置的测量点
		n=p8[12];//TSA数
		if(n>NUMmax_Event3106RM)
		{
			n=NUMmax_Event3106RM;
		}
		p8+=16;
		for(i=0;i<n;i++)
		{
			x=AddrToMeterSetNo((u16*)p8);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
			if(x<NUM_RMmax)
			{
				PowerEvent->setno[j]=x;//停/上电事件需抄读电表的配置序号(从0开始)
				j++;
			}
			p8+=9;
		}
	}
	else
	{//随机选择测量点
		//先选RS485口
		for(i=1;i<NUM_RMmax;i++)
		{
			if(j>=NUMmax_Event3106RM)
			{
				break;
			}
			p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			if(p8[0]==DataType_structure)
			{
				p8+=5;
				//规约类型
				p8=Get_Element(p8,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8==0)
				{
					continue;
				}
				x=p8[1];
				if((x==2)||(x==3))
				{//DL/T645-2007（2）,DL/T698.45（3）
					//端口
					p8+=2;
					x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
					x=OADtoPORTn(x);
					if((x==RS485_1PORT)||(x==RS485_2PORT))
					{
						PowerEvent->setno[j]=i;//停/上电事件需抄读电表的配置序号(从0开始)
						j++;
					}
				}
			}
		}
		//若无选中再选载波口
		if(j==0)
		{
			for(i=1;i<NUM_RMmax;i++)
			{
				if(j>=NUMmax_Event3106RM)
				{
					break;
				}
				p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
				if(p8[0]==DataType_structure)
				{
					p8+=5;
					//规约类型
					p8=Get_Element(p8,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8==0)
					{
						continue;
					}
					x=p8[1];
					if((x==2)||(x==3))
					{//DL/T645-2007（2）,DL/T698.45（3）
						//端口
						p8+=2;
						x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
						x=OADtoPORTn(x);
						if(x==RS485_4PORT)
						{
							PowerEvent->setno[j]=i;//停/上电事件需抄读电表的配置序号(从0开始)
							j++;
						}
					}
				}
			}
		}
	}
	PowerEvent->NUMrm=j;//停/上电事件需抄读电表个数
}

u32 CompVoltage(u32 V)//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
{
	u32 i;
	u8* p8;
	u16 *p16;
//停电事件甄别限值参数  structure
//{
//停电时间最小有效间隔（分钟） long-unsigned，
//停电时间最大有效间隔（分钟） long-unsigned，
//停电事件起止时间偏差限值（分钟） long-unsigned，
//停电事件时间区段偏差限值（分钟） long-unsigned，
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
//}
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		return 1;
	}
	if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		MC(0,ADDR_UA,3*4);//清交采电压值
	}
	if(V==0)
	{
		i=(p8[18]<<8)|p8[19];//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
	}
	else
	{
		i=(p8[15]<<8)|p8[16];//停电发生电压限值  long-unsigned（单位：V，换算：-1）
	}
	V=hex_bcd(i);
	for(i=0;i<3;i++)
	{
		p16=(u16 *)(ADDR_UA+2+(i*4));
		if(p16[0]>=V)
		{
			return 1;
		}
	}
	return 0;
}


void PowerEventTimerComp(u8* pEVENT)//停上电事件时间比较,入口:pEVENT指向698格式事件的开始,出口:PowerEvent->bit
{
	u32 x;
	u32 t1;
	u32 t2;
	u8* p8;
	
	if(PowerEvent->bit==0xc0)//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
	{
		return;
	}
//停/上电事件记录单元∷=structure
//{
//  2 事件记录序号  double-long-unsigned，
//  7 事件发生时间  date_time_s，
//  15 事件结束时间  date_time_s，
//...
//}
	x=Check_pYYMDHMS_hex(pEVENT+8);//年年月日时分秒合法检查,返回0合法,1非法
	if(x)
	{
		PowerEvent->TimeErr=1;//有抄读的电能表停电事件停电或上电时间不完整
		return;
	}
	x=Check_pYYMDHMS_hex(pEVENT+16);//年年月日时分秒合法检查,返回0合法,1非法
	if(x)
	{
		PowerEvent->TimeErr=1;//有抄读的电能表停电事件停电或上电时间不完整
		return;
	}
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		return;
	}
	x=(p8[9]<<8)|p8[10];//停电事件起止时间偏差限值（分钟） long-unsigned
	x*=60;
	t1=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,pEVENT+8);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC2=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
	if(t1>>31)
	{
		t1=~t1;
		t1++;
	}
	if(t1<x)
	{
		t1=YYMDHMS_Sub_hex(PowerEvent->Tstoptime,pEVENT+16);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC2=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
		if(t1>>31)
		{
			t1=~t1;
			t1++;
		}
		if(t1<x)
		{
			t1=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,PowerEvent->Tstoptime);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
			t2=YYMDHMS_Sub_hex(pEVENT+8,pEVENT+16);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
			if((t2>>31)==0)
			{
				t1-=t2;
				if(t1>>31)
				{
					t1=~t1;
					t1++;
				}
				x=(p8[12]<<8)|p8[13];//停电事件时间区段偏差限值（分钟） long-unsigned
				x*=60;
				if(t1<=x)
				{
					PowerEvent->bit|=0x40;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
				}
			}
		}
	}
}

//停电事件甄别限值参数  structure
//{
//停电时间最小有效间隔（分钟） long-unsigned，
//停电时间最大有效间隔（分钟） long-unsigned，
//停电事件起止时间偏差限值（分钟） long-unsigned，
//停电事件时间区段偏差限值（分钟） long-unsigned，
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
//}
void Event_Power(void)//终端停/上电事件
{
	u32 i;
//	u32 x;
	u32 t;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(PowerEvent->RTCS==Comm_Ram->DL698YMDHMS[6])//上次计算时RTC的秒
	{
		return;
	}
	
//---TEST---
//LCDDataOut();//数据测试永远刷新用,点阵数据输出到LCD屏
//---TEST END---
	
	PowerEvent->RTCS=Comm_Ram->DL698YMDHMS[6];//上次计算时RTC的秒
	switch(PowerEvent->task)
	{
		case 0://终端复位
			MR((u32)&PowerEvent->Tstarttime,ADDR_POWER_YMDHMS,7);//掉电事件判定掉电产生时时钟
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	#endif
	#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	#endif
			Terminal_Ram->Power_S_Timer=10;//38 停电事件秒定时器,同时从停电到开始抄表定时;注:电科院测试软件上电主动上报等待时间60秒
			PowerEvent->task++;
			break;
		case 1://掉电恢复稳定延时
			i=CompVoltage(0);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i)
			{
				if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
				{
					MW(ADDR_DL698YMDHMS,(u32)&PowerEvent->Tstoptime,7);//掉电恢复时间
				#ifdef IC_STM32F4xx
					if(RTC->BKP19R==0x12345678)//停电上事件判断用上电标志;看门狗复位自动重启=0x12345678,其他值为上电
					{//看门狗复位
						RTC->BKP19R=0;//停电上事件判断用上电标志;看门狗复位自动重启=0x12345678,其他值为上电
						PowerEvent->task=100;
						break;
					}
					RTC->BKP19R=0;//停电上事件判断用上电标志;看门狗复位自动重启=0x12345678,其他值为上电
				#endif
				#ifdef IC_NUC9xx
					p8=(u8*)(ADDR_REG_SYS_RSTSTS);
					if(p8[0]&(1<<5))
					{//看门狗复位
						p8[0]&=~(1<<5);
						PowerEvent->task=100;
						break;
					}
					p8[0]&=~(1<<5);
				#endif
					i=Check_pYYMDHMS_hex(PowerEvent->Tstarttime);//年年月日时分秒合法检查,返回0合法,1非法
					if(i==0)
					{
						i=Check_pYYMDHMS_hex(PowerEvent->Tstoptime);//年年月日时分秒合法检查,返回0合法,1非法
						if(i==0)
						{
							t=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,PowerEvent->Tstoptime);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC2=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
							if((t>>31)==0)
							{
								MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
								p8=(u8*)ADDR_128KDATABUFF;
								p8=Get_Element(p8,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
								if(p8)
								{
									i=(p8[3]<<8)|p8[4];//停电时间最小有效间隔（分钟） long-unsigned
									i*=60;
									if(t>=i)
									{
										i=(p8[6]<<8)|p8[7];//停电时间最大有效间隔（分钟） long-unsigned
										i*=60;
										if(t<i)
										{
											PowerEvent->bit=0x80;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
											Terminal_Ram->Power_S_Timer=10;//延时抄电能表记录(秒)
											PowerEvent->task++;
											break;
										}
									}
								}
							}
						}
					}
					//无效停电事件
					PowerEvent->bit=0;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
					Terminal_Ram->Power_S_Timer=0;//抄停电事件定时
					PowerEvent->task=20;//写上电事件记录
				}
				break;
			}
			Terminal_Ram->Power_S_Timer=10;//38 停电事件秒定时器;注:电科院测试软件上电主动上报等待时间60秒
			break;
		case 2://抄电能表记录延时等待
			i=CompVoltage(0);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
			{
				RM_Meter_Event3106();//停/上电事件需抄读电表的配置序号(从0开始)
			#if (USER/100)==0//电科院测试
				Terminal_Ram->Power_S_Timer=30;//抄停电事件定时(秒);注:电科院测试软件上电主动上报等待时间60秒
			#else
				if(PowerEvent->NUMrm==0)//停/上电事件需抄读电表个数
				{
					Terminal_Ram->Power_S_Timer=0;//抄停电事件定时(秒);注:电科院测试软件上电主动上报等待时间60秒
				}
				else
				{
					Terminal_Ram->Power_S_Timer=120;//抄停电事件定时(秒);注:电科院测试软件上电主动上报等待时间180秒
				}
			#endif
//				t=PowerEvent->NUMrm;//停/上电事件需抄读电表个数
//				for(i=0;i<t;i++)
//				{
//					p8=(u8*)ADDR_6000_SDRAM+(PowerEvent->setno[i]*LENper_6000);
//					p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
//					if(p8)
//					{
//						x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
//						if(x==0xF2090201)
//						{
//							//有载波表选中
//							Terminal_Ram->Power_S_Timer=300;//抄停电事件定时(秒);注:电科院测试软件上电主动上报等待时间60秒
//							break;
//						}
//					}
//				}
				PowerEvent->task=10;//抄电能表记录
			}
			break;
		
		case 10://抄电能表记录
			i=CompVoltage(0);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			if(PowerEvent->bit&0x40)//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
			{//事件有效
				Terminal_Ram->Power_S_Timer=0;
			}
			if(Terminal_Ram->Power_S_Timer==0)
			{
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#endif
				PowerEvent->task=20;//写上电事件记录
				break;
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
			{
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
				{
				#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)
					{
				#endif
				#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)
					{
				#endif
						
						PowerEvent->task=20;//写上电事件记录
						break;
						
				#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=		
					}
				#endif
				#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=		
					}
				#endif
				}
			}
			break;
			
		case 20://写上电事件记录
			i=CompVoltage(0);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			
//			Comm_Ram->DisplayAllErrorTimer=5;//上电全显或系统错误显示秒定时器			
//			DisplayStringAutoUp(0,9,(u8*)"\x0""上电事件记录");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			
			if((PowerEvent->bit&0x80)==0)//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
			{
				PowerEvent->bit=0;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
			}
			Event_31060200(0,0);//终端停/上电事件记录;入口:f=属性标志,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			PowerEvent->task=100;
			break;
		
		case 100://电源正常
			i=CompVoltage(1);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i==0)
			{
				Terminal_Ram->Power_S_Timer=10;//38 停电事件秒定时器;注:电科院测试软件掉电主动上报等待时间20秒
				PowerEvent->task++;
			}
			MW(ADDR_DL698YMDHMS,ADDR_POWER_YMDHMS,7);//掉电事件判定掉电产生时时钟
			break;
		case 101://掉电产生
			i=CompVoltage(1);//比较停电电压限值,入口:V=0恢复电压限值,V=1产生电压限值;返回:0=均小于参比值,1=有任一相大于参比值
			if(i)
			{
				PowerEvent->task=100;
			}
			else
			{
				if(Terminal_Ram->Power_S_Timer==0)//38 停电事件秒定时器
				{
					MW(ADDR_DL698YMDHMS,ADDR_POWER_YMDHMS,7);//掉电事件判定掉电产生时时钟
					MW(ADDR_DL698YMDHMS,(u32)&PowerEvent->Tstarttime,7);//掉电产生时间
					MC(0xff,(u32)&PowerEvent->Tstoptime,7);//掉电恢复时间
					PowerEvent->bit=0x80;//b7=置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断,b6=置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断
					Event_31060200(0,1);//终端停/上电事件记录;入口:f=属性标志,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					if(Comm_Ram->DataSave==2)//数据保护:0=空,1=电网有电超过10秒数据需保护,2=数据已保护
					{//数据已保护
						Battery_Data_Save();//电池工作时关机前数据保存
					}
					PowerEvent->task=0;
					
//					Comm_Ram->DisplayAllErrorTimer=5;//上电全显或系统错误显示秒定时器			
//					DisplayStringAutoUp(0,9,(u8*)"\x0""停电事件记录");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				}
			}
			break;
		default:
			PowerEvent->task=0;
			break;
	}
}

//电压上升下降流程
//1.额定电压 * 65%;延时等待 90 秒 ;正确，未产生终端停上电事件
//2.额定电压 * 55% 延时等待 90 秒  正确，产生终端停上电事件
//3.额定电压 * 75% 延时等待 90 秒  正确，未产生终端停上电事件
//4.额定电压 * 85% 延时等待 90 秒  正确，产生终端停上电事件
//5.额定电压 * 100% 延时等待 10 秒 允许终端主动上报
//6.额定电压 * 40% 延时等待 20 秒  正确，终端发生上报
//7.额定电压 * 100% 延时等待 60 秒 正确，终端发生上报
//8.额定电压 * 22%  延时等待 90 秒  设置模拟表,掉电事件
//9.额定电压 * 100% 延时等待 180 秒  事件记录次数正确：[2]  last 2正确，属性标识为 [ 正常、无效 ];last 1正确，属性标识为 [ 正常、有效 ]
//10.额定电压 * 22% 延时等待 90 秒 配置模拟表掉电事件
//11.额定电压 * 100% 延时等待 180 秒 last 2正确，属性标识为 [ 正常、无效 ];last 1正确，属性标识为 [ 正常、无效 ]
//12.额定电压 * 22% 延时等待 90 秒 配置模拟表掉电事件
//13.额定电压 * 100% 延时等待 180 秒 last 2正确，属性标识为 [ 正常、无效 ];last 1正确，属性标识为 [ 正常、无效 ]























