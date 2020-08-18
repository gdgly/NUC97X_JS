
#ifndef Project_H
#define Project_H


#define Project     200//方案0=智能表,100=网络表,200=I型集中器,300=专变终端,400=三合一终端,500=Ⅱ型集中器,600=通信模块,700=
//#define IC_STM32F4xx
#define IC_NUC9xx
#define LINUX


#define DEBUG     1//0=发行,1=调试
#define Enable_sysprintf     0//NUC97x OS中UART0系统调试输出; 0=禁止,1=允许

#define USER     1700//901//1501//1501//1401//1600//1501//901//用户标识
//0xx=盲样电科院送检版
	//0=盲样电科院送检版
//1xx=标准系
	//100=标准版
//2xx=山东系
	//200=山东版
//3xx=
	//300=
//4xx=陕西系
	//401=陕西版
//5xx=上海系
	//501=上海版
//6xx=湖南系
	//600=
	//601=湖南版
//7xx=浙江系
	//701=浙江版
//8xx=通用市场系
	//801=通用市场版
//9xx=河南系
	//900=省电科院台体测试版(仅终端事件设置参数同国网电科院测试版)
	//901=河南版
//10xx=河北系
	//1001=河北版
//11xx=吉林系
	//1101=吉林版
//12xx=四川系
	//1200=省电科院台体测试版
	//1201=四川版
//13xx=

//14xx=福建系
	//1401=福建版
//15xx=安徽系
	//1500=省电科院台体测试版
	//1501=安徽版
//16xx=内蒙系
	//1600=省电科院台体测试版
	//1601=内蒙版
//17xx=江苏系
	//1700=省电科院台体测试版
	//1701=江苏版


	
#define FactoryDI     0//生产厂商标识
//0=正泰
//1=

#ifdef IC_STM32F4xx
	#include "type.h"
	#include "stm32f4xx.h"
#endif
#ifdef IC_NUC9xx
 #ifndef LINUX
	#include "type.h"
	#include "../../NUC97x/Driver/Include/nuc970.h"
	#include "../../NUC97x/Driver/Include/sys.h"
 #else
		#define __disable_irq()    off_irq()
		#define __enable_irq()     destore_irq(0)

		#ifdef LINUX_DRIVE
		#define ADDR_ERAM_linux    0x02000000
		#else
		#define ADDR_ERAM_linux    0xB9000000
		#endif

	#ifdef LINUX_CROSS_GCC
		#define    __packed  //__attribute__ ((packed))
		#ifdef LINUX_DRIVE

		#else
			#include </mnt/hgfs/eclipse-698/include/drive/nuc970.h>
			#include </mnt/hgfs/eclipse-698/include/drive/sys.h>
		#endif

		#include </mnt/hgfs/eclipse-698/include/public/type.h>
	#else
		#include "type.h"
		#include "../../NUC97x/Driver/Include/nuc970.h"
		#include "../../NUC97x/Driver/Include/sys.h"
	#endif

 #endif
#endif


#if ((Project/100)==0)//智能表
#include "Project698_0xx.h"
#include "Pin_Project698_0xx.h"
#endif

#if ((Project/100)==1)//网络表
	#if (USER/100)==0//盲样电科院送检版
		#include "Project698_1xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project698_1xx_USER0xx.h"
	#endif
#include "Pin_Project698_1xx.h"
#endif

//---集中器---
#if ((Project/100)==2)//集中器
	#if (USER/100)==0//盲样电科院送检版
		#include "Project698_2xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project698_2xx_USER0xx.h"
	#endif
	#if (USER/100)==2//山东系
		#include "Project698_2xx_USER2xx.h"
	#endif
	#if (USER/100)==3//福建系
		#include "Project698_2xx_USER3xx.h"
	#endif
	#if (USER/100)==4//陕西系
		#include "Project698_2xx_USER4xx.h"
	#endif
	#if (USER/100)==5//上海系
		#include "Project698_2xx_USER5xx.h"
	#endif
	#if (USER/100)==6//湖南系
		#include "Project698_2xx_USER6xx.h"
	#endif
	#if (USER/100)==7//浙江系
		#include "Project698_2xx_USER7xx.h"
	#endif
	#if (USER/100)==8//通用市场系
		#include "Project698_2xx_USER8xx.h"
	#endif
	#if (USER/100)==9//河南系
		#include "Project698_2xx_USER9xx.h"
	#endif
	
	#if (USER/100)==11//吉林系
		#include "Project698_2xx_USER11xx.h"
	#endif
	
	#if (USER/100)==14//福建系
		#include "Project698_2xx_USER14xx.h"
	#endif
	
	#if (USER/100)==17//江苏系
		#include "Project698_2xx_USER17xx.h"
	#endif
	
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project698_2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_2xx.h"
	#endif

#endif
//---集中器 END---

//---专变终端---
#if ((Project/100)==3)//专变终端
	#if (USER/100)==0//盲样电科院送检版
		#include "Project698_3xx_USER0xx.h"
	#endif
	#if (USER/100)==1//标准系
		#include "Project698_3xx_USER0xx.h"
	#endif
	#if (USER/100)==2//山东系
		#include "Project698_3xx_USER2xx.h"
	#endif
	#if (USER/100)==3//福建系
		#include "Project698_3xx_USER3xx.h"
	#endif
	#if (USER/100)==4//陕西系
		#include "Project698_3xx_USER4xx.h"
	#endif
	#if (USER/100)==5//上海系
		#include "Project698_3xx_USER5xx.h"
	#endif
	#if (USER/100)==6//湖南系
		#include "Project698_3xx_USER6xx.h"
	#endif
	
	#if (USER/100)==8//通用市场系
		#include "Project698_3xx_USER8xx.h"
	#endif
	#if (USER/100)==10//河北系
		#include "Project698_3xx_USER10xx.h"
	#endif
	
	#if (USER/100)==12//四川系
		#include "Project698_3xx_USER12xx.h"
	#endif
	#if (USER/100)==15//安徽系
		#include "Project698_3xx_USER15xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project698_3xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_3xx.h"
	#endif
#endif
//---专变终端 END---

//---三合一终端---
#if ((Project/100)==4)//三合一终端

	#if (USER/100)==16//内蒙系
		#include "Project698_4xx_USER16xx.h"
	#endif

	#ifdef IC_STM32F4xx
		#include "Pin_Project698_4xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_4xx.h"
	#endif
#endif
//---三合一终端 END---

//---集中器Ⅱ型---
#if ((Project/100)==5)//集中器Ⅱ型
	#if (USER/100)==0//盲样电科院送检版
		#include "Project698_5xx_USER0xx.h"
	#endif

	#ifdef IC_STM32F4xx
		#include "Pin_Project698_5xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_5xx.h"
	#endif

#endif
//---集中器Ⅱ型 END---

#if ((Project/100)==6)//通信模块
#include "Project6xx.h"
#include "Pin_Project6xx.h"
#endif



#include "../H698/IRAM698.h"
#include "../H698/IEEPROM698.h"
#include "../H698/EFLASH698.h"
#include "../H698/ERAM698.h"









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
#define RechargeableBatteryStartMin  0x480//可充电池启动工作最小电压4.80V
#define RechargeableBatteryStopMin   0x440//可充电池停止工作最小电压4.40V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//超级电容电池停止工作最小电压4.00V(实测约3.50V时发生硬件复位)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//超级电容电池停止工作最小电压3.00V(实测2.70V时升压电路还能输出4.8V，2.2V时CPU还能工作)
	#endif
	
//LOG
#define LOG_698          0//0=没,1=有
#define LOG_1376_1       0//0=没,1=有
#define LOG_1376_2       0//0=没,1=有
#define LOG_1376_3       0//1//0=没,1=有
#define LOG_645_1997     0//0=没,1=有
#define LOG_645_2007     0//0=没,1=有
//调试打印到RS232
#define DEBUG_RS485_1toRS232  0//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
#define DEBUG_RS485_2toRS232  0//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
#define DEBUG_RS485_4toRS232  0//0=否,1=是;调试:RS485_4(载波)口1376.2收发数据打印到RS232口
#define DEBUG_GPRStoRS232     0//0=否,1=是;调试:GPRS口1376.3收发数据打印到RS232口
#define DEBUG_698toRS232      0//0=否,1=是;调试:698.45收发数据打印到RS232口
#define DEBUG_PORTtoRS232     (DEBUG_RS485_1toRS232|DEBUG_RS485_2toRS232|DEBUG_RS485_4toRS232|DEBUG_GPRStoRS232|DEBUG_698toRS232)
//调试运行
#define RUN_DL698TEST_1   	  0//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度
#define RUN_DL698TEST_2   	  0//0无效,1=运行;测试601E规约库
#define RUN_DL698TEST_USER    0//0无效,1=运行;用户版测试
#define TEST_DISPVDD5V0       0//0无效,1=运行;显示VDD5V0电压


#endif







