
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif


__align(4) const u32 ADDR_Terminal_VER[]=//版本信息
{
//VER
	EnProject+(EnHV<<1)+(EnUSER<<2)+(EnSV<<3)+(EnMeter<<4)+(EnMainProtocol<<5),//软件更新时是否判别
	Project,//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=,500=集中器Ⅱ型,600=,700=
	HVER,//硬件版本号
	USER,//用户标识
	SVER,//软件版本号
#if SAME_Terminal==0//0=否,1=是;软件需同时更新标志
	0,
#else
	SAME_MS+(SAME_OS<<1)+(SAME_Meter<<2)+(SAME_Terminal<<3)+(SAME_Menu<<4)+(SAME_Font<<5),//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
#endif
	DEBUG,// 0=发行,1=调试
	MeterCtrl+(MeterType<<8)+(MeterSpec<<16),//电能表常数接线方式等电能表规格
	3,//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
	0+(UpDataReset<<8),//第0字节//KEIL直接写入=0,在PC中进行替换后下载=1
		//第1字节//软件更新后重启:0=否,1=是(注:若OS更新则强制重启)
		//第2字节
		//第3字节
//RAM
	ADDR_Comm_Ram_Start,//0
	ADDR_Meter_SaveRam_Start,//1
	ADDR_Meter_Ram_Start,//2
	ADDR_Terminal_SaveRam_Start,//3
	ADDR_Terminal_IRAM_Start,//4
	ADDR_DATABUFF,//5
	ADDR_4KWRITE_BUFF,//6
	0,//7
//ERAM
	ADDR_ERAM_START,//0
	ADDR_UARTnCtrl_End,//1
	0,
	0,
	0,
	0,
	0,
	0,
//EFLASH
	ADDR_DISK_Start,//0
	ADDR_Com_EFLASH_Start,//1
	ADDR_Meter_EFLASH_Start,//2
	ADDR_Terminal_EFLASH_Start,//3
	0,
	0,
	0,
	0,
//
};


	
	
	
	







