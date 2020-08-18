
#ifndef Project_H
#define Project_H


#define IC_STM32F4xx
//#define IC_NUC9xx

//调试
#define DEBUG     			 0			// 0=发行,1=调试
#define Enable_sysprintf     0//NUC97x OS中UART0系统调试输出; 0=禁止,1=允许

#if (SE_FK_PROJECT==1)
	#define Project     300			  //方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//	#define USER     	  3000		    //标准版
//	#define USER     	  3100		  //广东版
//	#define USER     	  3200		  //广西版
//	#define USER     	  3300		  //贵州版
	#define USER     	  3400		  //云南版
//	#define USER     	  3500		  //海南版
#elif (SE_JZQ_PROJECT==1)
	#define Project     200			  //方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define USER     	  3000		    //标准版
//	#define USER     	  3100		  //广东版
//	#define USER     	  3200		  //广西版
//	#define USER     	  3300		  //贵州版
//	#define USER       	3400		  //云南版
//	#define USER     	  3500		  //海南版
#elif (SE_JZQ_II_PROJECT==1)
	#define Project     500			//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define USER     	  3000		    //标准版
//	#define USER     	  3100		  //广东版
//	#define USER     	  3200		  //广西版
//	#define USER     	  3300		  //贵州版
//	#define USER     	  3400		  //云南版
//	#define USER     	  3500		  //海南版
#elif (SE_PB_PROJECT==1)
	#define Project     900			//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#define USER     	  3000		    //标准版
//	#define USER     	  3100		  //广东版
//	#define USER     	  3200		  //广西版
//	#define USER     	  3300		  //贵州版
//	#define USER     	  3400		  //云南版
//	#define USER     	  3500		  //海南版


#endif
// 0xx=盲样电科院送检版
	//0=盲样电科院送检版
// 1xx=标准系
	//100=标准版
// 2xx=山东系
	//200=山东版
// 3xx=福建系
	//300=福建版
// 4xx=陕西系
	//400=陕西版
// 5xx=上海系
	//500=上海版
// 6xx=湖南系
	//600=湖南版
	//601=湖南湘潭版
// 7xx=浙江系
	//700=浙江版
// 8xx=通用市场系
	//800=通用市场版
// 9xx=河南系
	//900=河南版
// 10xx=河北系
	//1000=河北版
// 11xx=吉林系
	//1100=吉林版
// 12xx=四川系
	//1200=四川版
// 13xx=

// 14xx=新福建系
	//1400=福建版

//30xx=南网标准版
//31xx = 广东系
	//3100 = 广东版
//32xx = 广西系
	//3200 = 广西版
//33xx = 贵州系
	//3300 = 贵州版
//34xx = 云南系
	//3400 = 云南版
//35xx = 海南系
	//3500 = 海南版

	


#define FactoryDI     0		//生产厂商标识  0 = 正泰 1 = ?

#include "type.h"
#ifdef IC_STM32F4xx
	#include "stm32f4xx.h"
#endif
#ifdef IC_NUC9xx
	#include "../../NUC97x/Driver/Include/nuc970.h"
	#include "../../NUC97x/Driver/Include/sys.h"
#endif


#define DLT500_REPORT		0   //南网配变取DLT500报告时定义的宏，=0为无效，=1为有效，编译时选择在Project==300负控终端产品下将此宏置1
#define FactoryDI     	0		//生产厂商标识  0 = 正泰 1 = ?



#if ((Project/100)==2)	//集中器

	#if (USER/100)==30	//盲样电科院送检版
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//标准系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==31	//广东系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==32	//广西系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==33	//贵州系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==34	//云南系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==35	//海南系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#include "../Hcsg/Pin_ProjectCSG_2xx.h"
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_2xx.h"
	#endif
	
#endif

#if ((Project/100)==3)	//专变终端

	#if (USER/100)==30	//盲样电科院送检版
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//标准系
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==31	//广东系
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==32	//广西系
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==33	//贵州系
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==34	//云南系
		#include "../Hcsg/ProjectCSG_3xx_USER34xx.h"
	#endif
	
	#if (USER/100)==35	//海南系
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	//#include "../Hcsg/Pin_ProjectCSG_3xx.h"
	
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_3xx.h"
	#endif
	#ifdef IC_NUC9xx
		//#include "PinNUC97x_ProjectCSG_3xx.h"
		#include "../Hcsg/PinNUC97x_ProjectCSG_3xx.h"//err20190512 更改IO头文件
	#endif
	
#endif

#if ((Project/100)==5)	//II型集中器

	#if (USER/100)==30	//盲样电科院送检版
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//标准系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==31	//广东系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==32	//广西系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==33	//贵州系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==34	//云南系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==35	//海南系
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	//#include "../Hcsg/Pin_ProjectCSG_5xx.h"
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_5xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_5xx.h"
	#endif
	
#endif

#if ((Project/100)==9)	//配变监测计量终端

	#if (USER/100)==30	//盲样电科院送检版
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//标准系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==31	//广东系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==32	//广西系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==33	//贵州系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==34	//云南系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==35	//海南系
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#include "../Hcsg/Pin_ProjectCSG_2xx.h"
	
	#ifdef IC_STM32F4xx
		//#include "stm32f4xx.h"
		#include "../Hcsg/Pin_ProjectCSG_2xx.h"
	#endif
	#ifdef IC_NUC9xx

	#endif
	
#endif



#include "../Hcsg/IRAMCSG.h"
#include "../Hcsg/IEEPROMCSG.h"
#include "../Hcsg/EFLASHCSG.h"
#include "../Hcsg/ERAMCSG.h"



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
#define WDTTimerOutVal     				20000		//看门狗超时值；最大TimerOutMS=(0xfff*256)/32=32768ms
//可充电池
#define RechargeableBatteryGrid3    	0x520		//可充电池3格时电压
#define RechargeableBatteryGrid2     	0x500		//可充电池2格时电压
#define RechargeableBatteryGrid1     	0x480		//可充电池1格时电压
#define RechargeableBatteryStartMin     0x480		//可充电池启动工作最小电压4.80V
#define RechargeableBatteryStopMin     	0x440		//可充电池停止工作最小电压4.40V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//超级电容电池停止工作最小电压4.00V(实测约3.50V时发生硬件复位)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//超级电容电池停止工作最小电压4.00V(实测约3.50V时发生硬件复位)
	#endif

//LOG
//#define LOG_698          0//1//0=没,1=有
//#define LOG_1376_1       0//0=没,1=有
//#define LOG_1376_2       0//1//0=没,1=有
//#define LOG_1376_3       0//1//0=没,1=有
//#define LOG_645_1997     0//0=没,1=有
//#define LOG_645_2007     0//0=没,1=有
//调试打印到RS232
#define DEBUG_RS485_1toRS232  0//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
#define DEBUG_RS485_2toRS232  0//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
#define DEBUG_RS485_4toRS232  0//0=否,1=是;调试:RS485_4(载波)口1376.2收发数据打印到RS232口
#define DEBUG_GPRStoRS232     0//0=否,1=是;调试:GPRS口1376.3收发数据打印到RS232口
#define DEBUG_698toRS232      0//0=否,1=是;调试:698.45收发数据打印到RS232口
#define DEBUG_PORTtoRS232     (DEBUG_RS485_1toRS232|DEBUG_RS485_2toRS232|DEBUG_RS485_4toRS232|DEBUG_GPRStoRS232|DEBUG_698toRS232)


#endif







