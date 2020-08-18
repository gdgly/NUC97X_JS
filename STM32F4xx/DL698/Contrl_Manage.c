
//终端控制管理
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../DL698/CALC_TAG.h"




#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Terminal_Contrl_Protect(void)//终端控制-保电
{
	u32 i;
	u32 n;
	u32 state;//保电-属性2（保电状态，只读）:解除（0），保电（1），自动保电（2）
	u8 * p8;
	
	state=0;//保电-属性2（保电状态，只读）:解除（0），保电（1），自动保电（2）
//#define ADDR_8001_127     ADDR_8000_130+LENmax_8000_130//方法127：投入保电
////方法127：投入保电（参数）参数∷=NULL
////用于投入保电状态，即禁止一切执行跳闸的继电器输出控制，且恢复已跳闸的继电器输出控制。
////方法128：解除保电（参数）参数∷=NULL
////用于解除保电状态。
////投入=0x55,解除=0x0
//#define LENmax_8001_127     1
//#define ADDR_8001_3     ADDR_8001_127+LENmax_8001_127//保电-允许与主站最大无通信时长（分钟）
////属性3∷=long-unsigned
////允许与主站最大无通信时长（分钟），0表示不自动保电。
//#define LENmax_8001_3     3
//#define ADDR_8001_4     ADDR_8001_3+LENmax_8001_3//保电-上电自动保电时长（分钟）
////属性4∷=long-unsigned
////上电自动保电时长（分钟），0表示上电不自动保电。
//#define LENmax_8001_4     3
//#define ADDR_8001_5     ADDR_8001_4+LENmax_8001_4//保电-自动保电时段
////属性5∷=array 自动保电时段
////自动保电时段∷=structure
////{
////起始时间（时）  unsigned，
////结束时间（时）  unsigned
////}
////时间段区间规则为前闭后开。
//#define LENmax_8001_5     (8*24)

	MR(ADDR_DATABUFF,ADDR_8001_127,LENmax_8001_127+LENmax_8001_3+LENmax_8001_4+LENmax_8001_5);
	p8=(u8*)ADDR_DATABUFF;
	if(p8[0]==0x55)
	{//终端保电投入
		state=1;//保电-属性2（保电状态，只读）:解除（0），保电（1），自动保电（2）
	}
	else
	{//终端保电解除,自动保电
		//保电-允许与主站最大无通信时长（分钟）
		p8+=1;
		i=p8[1]|p8[2];
		if(i)
		{
			if(Terminal_Ram->DisconAutoProtect_M_Timer==0x0)//自动保电允许与主站连续无通信时间分定时器
			{
				state=2;
			}
		}
		//保电-上电自动保电时长（分钟）
		p8+=3;
		i=p8[1]|p8[2];
		if(i)
		{
			if(Terminal_Ram->UpPower_Protect_S_Timer)//35 终端上电保电秒定时器
			{
				state=2;
			}
		}
		//保电-自动保电时段
		p8+=3;
		n=p8[1];
		p8+=4;
		i=Comm_Ram->DL698YMDHMS[4];//当前时
		while(n--)
		{
			if((i>=p8[1])&&(i<p8[3]))
			{
				state=2;
				break;
			}
			p8+=6;
		}
	}
	p8=(u8 *)(ADDR_8001_2);
	p8[0]=DataType_enum;
	p8[1]=state;
}

void Terminal_Time_Period(void)//终端费率时段和功控时段
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u8* p8;
	u8 * p8d;
	u8 * p8rtc;

//费率
	p8=(u8*)ADDR_METER07_CTRL_SRAM;
	p8d=(u8 *)(ADDR_Terminal_Period);//终端费率时段和功控时段
	i=p8[0];
	p8d[0]=i;
//功控时段
	//当前RTC的时分按30分计数
	p8rtc=(u8 *)(ADDR_DL698YMDHMS);
	n=p8rtc[4];
	n*=2;
	if(p8rtc[5]>=30)
	{
		n++;
	}
	n++;//0点算1个30分
	MR(ADDR_DATABUFF,ADDR_8101_2,LENmax_8101_2);//终端功控时段
	p8=(u8*)ADDR_DATABUFF;
	x=p8[3];//取值取错了，将数据标识获取来了。
	x&=0x3;//原控制代码
	y=1;//时段号从1开始(相当于I值)
	for(i=0;i<n;i++)
	{
		z=p8[2+(2*(i/4))+1];
		z>>=(i%4)*2;
		z&=0x3;
		if(z!=x)
		{//控制码不相同
			x=z;
			y++;//时段号+1
		}
	}
	if(y>8)
	{//只能1-8,>9作为0
		y=0;
	}
	p8d=(u8 *)(ADDR_Terminal_Period);//终端费率时段和功控时段
	p8d[2]=y;
	if(p8d[3]!=x)
	{//控制代码变化
		p8d[3]=x;
		//控制时段变化告警
		BUZZ_Generate(3);//蜂鸣产生,入口蜂鸣号
		//清原时段功控
		MC(0,ADDR_Terminal_kWCtrl4_State,Terminal_Ctrl_Pn_Len*NUMmax_TAG);
	}
}

#endif




void Terminal_Contrl_Manage(void)//终端控制管理(分任务运行)
{
	switch(Terminal_Ram->I2SUBTask5)
	{
		case 0:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Calc_TAG_Power();//计算全部总加组的功率
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Calc_TAG_Power();//计算全部总加组的功率
			}
		#endif
			break;
		case 1:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_Protect();//终端控制-保电
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_Protect();//终端控制-保电
			}
		#endif
			break;
		case 2:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Remote_Control();//终端控制-遥控跳闸
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Remote_Control();//终端控制-遥控跳闸
			}
		#endif
			break;
		case 3:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Time_Period();//终端费率时段和功控时段
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Time_Period();//终端费率时段和功控时段
			}
		#endif
			break;
		case 4:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl1();//终端控制-功控.当前功率下浮控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl1();//终端控制-功控.当前功率下浮控
			}
		#endif
			break;
		case 5:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl2();//终端控制-功控.营业报停功控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl2();//终端控制-功控.营业报停功控
			}
		#endif
			break;
		case 6:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl3();//终端控制-功控.厂休功控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl3();//终端控制-功控.厂休功控
			}
		#endif
			break;
		case 7:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl4();//终端控制-功控.时段功控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl4();//终端控制-功控.时段功控
			}
		#endif
			break;
		case 8:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_MonthEnergy();//终端控制-月电控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_MonthEnergy();//终端控制-月电控
			}
		#endif
			break;
		case 9:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_BuyEnergy();//终端控制-购电控
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_BuyEnergy();//终端控制-购电控
			}
		#endif
			break;
		case 10:
			Terminal_Contrl_Out();//终端控制-输出.继电器和LED
			break;
		case 11:
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Contrl_State();//总加组当前控制状态
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Contrl_State();//总加组当前控制状态
			}
		#endif
			break;
		
		default:
			Terminal_Ram->I2SUBTask5=0;
			return;
	}
	Terminal_Ram->I2SUBTask5++;
}












