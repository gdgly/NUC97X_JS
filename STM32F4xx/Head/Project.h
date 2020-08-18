
#ifndef Project_H
#define Project_H


#define Project     300//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//#define IC_STM32F4xx
#define IC_NUC9xx

#define DEBUG     0//0=发行,1=调试
#define Enable_sysprintf     0//NUC97x OS中UART0系统调试输出; 0=禁止,1=允许
#define USER     1200//500//1400//1200//用户标识
//0xx=盲样电科院送检版
	//0=盲样电科院送检版
//1xx=标准系
	//100=标准版
//2xx=山东系
	//200=山东版
//3xx=福建系(不完整弃用)
	//300=福建版
//4xx=陕西系
	//400=陕西版
//5xx=上海系
	//500=上海版
//6xx=湖南系
	//600=湖南版
	//601=湖南湘潭版
//7xx=浙江系
	//700=浙江版
//8xx=通用市场系
	//800=通用市场版
//9xx=河南系
	//900=河南版
//10xx=河北系
	//1000=河北版
//11xx=吉林系
	//1100=吉林版
//12xx=四川系
	//1200=四川版
//13xx=

//14xx=新福建系
	//1400=福建版


#define FactoryDI     0//生产厂商标识
//0=正泰
//1=

#include "type.h"
#ifdef IC_STM32F4xx
	#include "stm32f4xx.h"
#endif
#ifdef IC_NUC9xx
	#include "../../NUC97x/Driver/Include/nuc970.h"
	#include "../../NUC97x/Driver/Include/sys.h"
#endif






#if ((Project/100)==0)//智能表
#include "Project0xx.h"
#include "Pin_Project0xx.h"
#endif

#if ((Project/100)==1)//网络表
	#if (USER/100)==0//盲样电科院送检版
		#include "Project1xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project1xx_USER0xx.h"
	#endif
#include "Pin_Project1xx.h"
#endif

#if ((Project/100)==2)//集中器
	#if (USER/100)==0//盲样电科院送检版
		#include "Project2xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project2xx_USER0xx.h"
	#endif
	#if (USER/100)==2//山东系
		#include "Project2xx_USER2xx.h"
	#endif
	#if (USER/100)==3//福建系
		#include "Project2xx_USER3xx.h"
	#endif
	#if (USER/100)==4//陕西系
		#include "Project2xx_USER4xx.h"
	#endif
	#if (USER/100)==5//上海系
		#include "Project2xx_USER5xx.h"
	#endif
	#if (USER/100)==6//湖南系
		#include "Project2xx_USER6xx.h"
	#endif
	#if (USER/100)==7//浙江系
		#include "Project2xx_USER7xx.h"
	#endif
	#if (USER/100)==8//通用市场系
		#include "Project2xx_USER8xx.h"
	#endif
	#if (USER/100)==9//河南系
		#include "Project2xx_USER9xx.h"
	#endif
	
	#if (USER/100)==11//吉林系
		#include "Project2xx_USER11xx.h"
	#endif
	#if (USER/100)==12//四川系
		#include "Project2xx_USER12xx.h"
	#endif
	
	#if (USER/100)==14//福建系
		#include "Project2xx_USER14xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_2xx.h"
	#endif
#endif

#if ((Project/100)==3)//专变终端
	#if (USER/100)==0//盲样电科院送检版
		#include "Project3xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project3xx_USER0xx.h"
	#endif
	#if (USER/100)==2//山东系
		#include "Project3xx_USER2xx.h"
	#endif
	#if (USER/100)==3//福建系
		#include "Project3xx_USER3xx.h"
	#endif
	#if (USER/100)==4//陕西系
		#include "Project3xx_USER4xx.h"
	#endif
	#if (USER/100)==5//上海系
		#include "Project3xx_USER5xx.h"
	#endif
	#if (USER/100)==6//湖南系
		#include "Project3xx_USER6xx.h"
	#endif
	
	#if (USER/100)==8//通用市场系
		#include "Project3xx_USER8xx.h"
	#endif
	#if (USER/100)==10//河北系
		#include "Project3xx_USER10xx.h"
	#endif
	
	#if (USER/100)==12//四川系
		#include "Project3xx_USER12xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project3xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_3xx.h"
	#endif
#endif

#if ((Project/100)==4)//公变终端
	#if (USER/100)==0//盲样电科院送检版
		#include "Project4xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project4xx_USER0xx.h"
	#endif
#include "Pin_Project4xx.h"
#endif

#if ((Project/100)==5)//集中器Ⅱ型
#include "Project5xx.h"
#include "Pin_Project5xx.h"
#endif

#if ((Project/100)==6)//通信模块
#include "Project6xx.h"
#include "Pin_Project6xx.h"
#endif

#include "AFN04MaxPn.h"
#include "AFN05MaxPn.h"
#include "AFN0CMaxPn.h"
#include "AFN0DMaxPn.h"
#include "AFN0EMaxPn.h"

#include "IRAM.h"
#include "IEEPROM.h"
#include "EFLASH.h"
#include "ERAM.h"


//Pn类型
#define p0Type     0//终端信息点
#define pnType     1//测量点号
#define GnType     2//总加组号
#define CnType     3//电容器测量点号
#define DnType     4//直流模拟量号
#define RnType     5//控制轮次号
#define TnType     6//任务号
#define EnType     7//事件ERC号
#define MDCnType   8//电能表数据分级类号
#define MDEnType   9//电能表数据等级号



//看门狗超时值
#define WDTTimerOutVal     20000//看门狗超时值；最大TimerOutMS=(0xfff*256)/32=32768ms
//可充电池
#define RechargeableBatteryGrid3     0x520//可充电池3格时电压
#define RechargeableBatteryGrid2     0x500//可充电池2格时电压
#define RechargeableBatteryGrid1     0x480//可充电池1格时电压
#define RechargeableBatteryStartMin     0x480//可充电池启动工作最小电压4.80V
#define RechargeableBatteryStopMin     0x460//可充电池停止工作最小电压4.60V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//超级电容电池停止工作最小电压4.00V(实测约3.50V时发生硬件复位)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//超级电容电池停止工作最小电压4.00V(实测约3.50V时发生硬件复位)
	#endif
//标称4.8V可充电池:
//充满约5.5V-5.6V
//放置1个月自消耗约20%
//5.0V时约还有20%容量
//4.8V时容量在5%以内
//4.4V以下已有过放情况
//4.0V以下可恢复约70%-80%容量
//4.8V-5.0V时补充电损坏最小




#endif







