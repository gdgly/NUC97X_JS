
//终端开关量输入
#include "Project.h"
#include "Terminal_SwitchIn.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"




#if (((Project/100)!=2)&&((Project/100)!=3)&&((Project/100)!=4)&&((Project/100)!=5))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
void Init_SwitchIn(void)//初始化终端开关量输入
{
}
void Terminal_SwitchIn(void)//终端开关量输入
{
}
#else
const u16 PIN_SwitchIn_List[]=//终端开关量输入引脚列表
{
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	#endif
	#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
	#endif	
	#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	#endif
//	PIN_P208_SIN5,
//	PIN_P208_SIN6,
//	PIN_P208_DOOR,//门接点统一读入后放D7位
};

u32 Get_SwitchIn_Route(void)//得到开关量输入路数
{
	u32 i;
	i=sizeof(PIN_SwitchIn_List)/2;
	if(i>MaxSwitchInPort)//最大开关量输入端口数(脉冲输入端口数+门接点)
	{
		i=MaxSwitchInPort;//最大开关量输入端口数(脉冲输入端口数+门接点)
	}
	return i;
}

void Init_SwitchIn(void)//初始化终端开关量输入
{
	u32 i;
	u32 x;
	u32 y;
	u32 MaxPort;

	MaxPort=Get_SwitchIn_Route();//得到开关量输入路数
//读引脚
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//读引脚,入口引脚使用名;出口0或1
	}
	x|=(Pin_Read(PIN_DOOR_IN)<<7);//门接点统一读入后放D7位
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//门接点统一放D7位

	x=~x;//置"O"："分"状态；置"l"："合"状态
	x&=y;

//接入和分合状态
	for(i=0;i<8;i++)
	{
		if((MRR(ADDR_AFN04F12,1)&(1<<i))==0x0)
		{//未接入=置"O"："分"状态
			x&=~(1<<i);
		}
		else
		{//接入
			if((MRR(ADDR_AFN04F12+1,1)&(1<<i))==0x0)
			{//置"0"：常闭触点
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	MWR(x,ADDR_AFN0CF9,2);//写状态
}

void Terminal_SwitchIn(void)//终端开关量输入
{
	u32 i;
	u32 x;
	u32 y;
	u32 f;
	u32 MaxPort;
	u8 * p8;
	
	MaxPort=Get_SwitchIn_Route();//得到开关量输入路数
//读引脚
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//读引脚,入口引脚使用名;出口0或1
	}
	x|=(Pin_Read(PIN_DOOR_IN)<<7);//门接点统一读入后放D7位
	
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//门接点统一放D7位
	
	x=~x;//置"O"："分"状态；置"l"："合"状态
	x&=y;
//输入滤波
	f=0;//滤波OK标志
	p8=(u8*)ADDR_SwitchIn;//开关量输入
	for(i=0;i<8;i++)
	{
		if((x&(1<<i))!=p8[2*i])
		{
			p8[2*i]=x&(1<<i);//开关量输入原状态
			p8[(2*i)+1]=0;//开关量输入滤波次数
		}
		else
		{
			if(p8[(2*i)+1]<3)//约1秒
			{
				p8[(2*i)+1]++;//开关量输入滤波次数
			}
			else
			{
				f|=(1<<i);//滤波OK标志
			}
		}
	}
//接入和分合状态
	for(i=0;i<8;i++)
	{
		if((MRR(ADDR_AFN04F12,1)&(1<<i))==0x0)
		{//未接入=置"O"："分"状态
			x&=~(1<<i);
		}
		else
		{//接入
			if((MRR(ADDR_AFN04F12+1,1)&(1<<i))==0x0)
			{//置"0"：常闭触点
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
//屏蔽无效位
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//门接点统一放D7位
	x&=y;
	f&=y;
//变位标志
	MR(ADDR_DATABUFF,ADDR_AFN0CF9,2);//读原状态
	p8=(u8 *)(ADDR_DATABUFF);
	y=x^p8[0];//变位标志
	y&=f;
	p8[1]|=y;//变位标志
	p8[0]&=(~f);//取没稳定位的原位
	x&=f;//取当前稳定的位
	p8[0]|=x;//当前状态
	MW(ADDR_DATABUFF,ADDR_AFN0CF9,2);//写状态
	if(y!=0x0)
	{//有变位
		ERC4_Event(y,x);//状态量变位记录;入口sc=状态变位,ss=变位后状态
	}
}
#endif

//#define AFN0CF9_ADDR     TERMINAL_CLASS1DATA_FM_START_ADDR//终端状态量及变位标志(第2字节变位标志在UART_READDATA中设置读标志,在终端状态标志处理子程序中清0)
////字节1的D0～D7按位对应l～8路状态量的状态ST，置"O"："分"状态；置"l"："合"状态。
////字节2的D0～D7按位对应1～8路状态量的变位CD，置"O"：自前次遥信传送后无状态变化；置"1"：自前次遥信传送后至少有一次状态变化
//F12：终端状态量输入参数
//	表31　终端状态量输入参数数据单元格式
//数据内容	数据格式	字节数
//状态量接入标志位（对应1～8路状态量）	BS8	1
//状态量属性标志位（对应1～8路状态量）	BS8	1
//
//--状态量接入标志位：D0～D7按顺序对位表示第1～8路状态量输入，置"1"：接入，置"0"：未接入。
//--状态量属性标志位：D0～D7按顺序对位表示第1～8路状态量输入，置"1"常开触点。置"0"：常闭触点


