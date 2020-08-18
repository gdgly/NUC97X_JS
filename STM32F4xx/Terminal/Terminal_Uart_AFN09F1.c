
#include "Project.h"





const u8 ADDR_AFN09F1[]=//终端版本信息
{
#if FactoryDI==0//0=正泰,1=
	'C','H','N','T',//厂商代号	ASCII	4
#endif
#if FactoryDI==1//0=正泰,1=
	'C','H','N','T',//厂商代号	ASCII	4
#endif
	'1','2','3','4','5','6','7','8',//设备编号	ASCII	8
	
 #if (USER/100)==5//上海
 	'Z','Z',((SVER>>4)&0xf)+0x30,((SVER>>0)&0xf)+0x30,//终端软件版本号	ASCII	4
 #else
	#if (USER/100)==12//四川
		'1','6','C','4',//终端软件版本号	ASCII	4
	#else
		//((SV>>8)&0xf)+0x30,'.',((SV>>4)&0xf)+0x30,(SV&0xf)+0x30,//终端软件版本号	ASCII	4
		'V',((SVER>>8)&0xf)+0x30,'.',((SVER>>4)&0xf)+0x30,//终端软件版本号	ASCII	4
	#endif
 #endif
	SVDATE&0xff,(SVDATE>>8)&0xff,(SVDATE>>16)&0xff,//终端软件发布日期：日月年	见附录A.20	3
	
#if NAND_1G==1//0=没,1=有
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','1','2','8','M',' ',' ',' ','R','8','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','1','2','8','M',' ',' ','R','1','6','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','1','2','8','M',' ',' ','R','3','2','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','1','2','8','M',' ',' ','R','6','4','M',//终端配置容量信息码	ASCII	11
	#endif
#endif
#if NAND_2G==1//0=没,1=有
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','2','5','6','M',' ',' ',' ','R','8','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','2','5','6','M',' ',' ','R','1','6','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','2','5','6','M',' ',' ','R','3','2','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','2','5','6','M',' ',' ','R','6','4','M',//终端配置容量信息码	ASCII	11
	#endif
#endif
#if NAND_4G==1//0=没,1=有
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','5','1','2','M',' ',' ',' ','R','8','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','5','1','2','M',' ',' ','R','1','6','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','5','1','2','M',' ',' ','R','3','2','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','5','1','2','M',' ',' ','R','6','4','M',//终端配置容量信息码	ASCII	11
	#endif
#endif
#if NAND_8G==1//0=没,1=有
	#if (IC_MT48LC4M16|IC_MT48LC8M8)
	'F','1','0','2','4','M',' ',' ','R','8','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC8M16|IC_MT48LC16M8)
	'F','1','0','2','4','M',' ','R','1','6','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC16M16|IC_MT48LC32M8)
	'F','1','0','2','4','M',' ','R','3','2','M',//终端配置容量信息码	ASCII	11
	#endif
	#if (IC_MT48LC32M16|IC_MT48LC64M8)
	'F','1','0','2','4','M',' ','R','6','4','M',//终端配置容量信息码	ASCII	11
	#endif
#endif

#if USER<200//用户标识
	'1','3','0','1',//终端通信协议.版本号	ASCII	4
#else
	#if (USER/100)==2//山东系
	'1','3','7','6',//终端通信协议.版本号	ASCII	4
	#endif
	#if (USER/100)==3//福建系
	'1','3','7','6',//终端通信协议.版本号	ASCII	4
	#endif
	#if (USER/100)==4//陕西系
	'1','3','7','6',//终端通信协议.版本号	ASCII	4
	#endif
	#if (USER/100)==5//上海系
	'1','3','7','6',//终端通信协议.版本号	ASCII	4
	#endif
	#if (USER/100)>=6//湖南系
	'1','3','7','6',//终端通信协议.版本号	ASCII	4
	#endif
#endif
	
	//((HVER>>8)&0xf)+0x30,'.',((HVER>>4)&0xf)+0x30,(HVER&0xf)+0x30,//终端硬件版本号	ASCII	4
	'V',((HVER>>8)&0xf)+0x30,'.',((HVER>>4)&0xf)+0x30,//终端硬件版本号	ASCII	4
	HVDATE&0xff,(HVDATE>>8)&0xff,(HVDATE>>16)&0xff,//终端硬件发布日期：日月年	见附录A.20	3
};






