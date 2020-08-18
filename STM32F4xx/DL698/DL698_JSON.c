
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_UART.h"

#include "../DL698/Contrl_Manage.h"

#include "../DL698/DL698_JSON.h"
//#include "../DL698/cJSON.h"
#include "../Calculate/ascii.h"
#include "../Calculate/Calculate.h"

#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"

#pragma  diag_suppress 870     //防止KEIL报警告
//江苏上报全网实时数据的OAD
__align(4) const u16 fcstab[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

const u32 RealDataOAD_JSON[]=//实时JSON文件打包的数据项
{
	0x40020200,	//采集存储时标
	0x20000201,	//A相电压
	0x20000202,	//B相电压
	0x20000203,	//C相电压

	0x20010201,	//A相电流
	0x20010202,	//B相电流
	0x20010203,	//C相电流

	0x20010400,	//零线电流
	0x20010500,	//剩余电流

	0x20040201,	//总有功功率
	0x20040202,	//A相有功功率
	0x20040203,	//B相有功功率
	0x20040204,	//C相有功功率
	
	0x43140506,	//(室外温度)
	0x43140505,	//(室内温度)
	0x43140501,	//(空调设置温度)
	0x43140400,	//(空调开关机状态)
	0x43110502,	//(热水器加热温度)
	0x43110400,	//(热水器开关机状态)

	0x00100201, //正有总
	0x00100203,	//正有锋
	0x00100205,	//正有谷

	0x00200201, //反有总
	0x00200203,	//反有锋
	0x00200205,	//反有谷
};		
//江苏上报全网实时数据的OAD数量
const u32 RealDataOAD_JSONSIZE=sizeof(RealDataOAD_JSON)/sizeof(u32);

typedef struct
{
	u16 OI;
	u16 POINTNUM;//小数点位数
}POINTNUM_type;

const POINTNUM_type OAD_PointNUM_list[]=//OAD对应小数点位数
{
	{0x2000,	1},//	电压	数据类型：long-unsigned，单位：V，换算：-1
	{0x2001,	3},//	电流	数据类型：double-long，单位：A 换算：-3
	{0x2002,	1},//	电压相角	数据类型：long-unsigned，单位：度，换算：-1
	{0x2003,	1},//	电压电流相角	数据类型：long-unsigned，单位：度，换算：-1
	{0x2004,	1},//	有功功率	数据类型：double-long，单位：W，换算：-1
	{0x2005,	1},//	无功功率	数据类型：double-long，单位：var，换算：-1
	{0x2006,	1},//	视在功率	数据类型：double-long，单位：VA，换算：-1
	{0x2007,	1},//	一分钟平均有功功率 数据类型：double-long，单位：W，换算：-1
	{0x2008,	1},//	一分钟平均无功功率 数据类型：double-long，单位：var，换算：-1
	{0x2009,	1},//	一分钟平均视在功率 数据类型：double-long，单位：VA，换算：-1
	{0x200A,	3},//	功率因数	数据类型：long，单位：无，换算：-3
	{0x200B,	2},//	电压波形失真度	数据类型：long，单位：%，换算：-2
	{0x200C,	2},//	电流波形失真度	数据类型：long，单位：%，换算：-2
	{0x200D,	2},//	电压谐波含有量（总及 2?n 次） 数据类型：long，单位：%，换算：-2
	{0x200E,	2},//	电流谐波含有量（总及 2?n 次） 数据类型：long，单位：%，换算：-2
	{0x200F,	2},//	电网频率	数据类型：long-unsigned，单位：Hz，换算：-2
	 
	
	//对象标识 OI 接口类 IC	对象名称	实例的对象属性及方法定义
	{0x2010,	1},//	表内温度	数据类型：long，单位：℃，换算：-1
	{0x2011,	2},//	时钟电池电压	数据类型：long-unsigned，单位：V，换算：-2
	{0x2012,	2},//	停电抄表电池电压	数据类型：long-unsigned，单位：V，换算：-2
	//{0x2013,	0},//	时钟电池工作时间	数据类型：double-long-unsigned，单位：分钟，无换算
	//2014	6	电能表运行状态字	数据类型：array bit-string，无单位，无换算，包括电能 表运行状态字 1…7，见附  录 G
	
	//2015	6	电能表跟随上报状态字	数据类型：bit-string(SIZE(32))，无单位，无换算，见附 录  G
	//属性 4（电能表跟随上报模式字）∷= bit-string(SIZE(32)) 方法 127（确认电能表跟随上报状态字）∷=
	//bit-string(SIZE(32))
	{0x2017,	4},//	当前有功需量	数据类型：double-long，单位：kW，换算：-4
	{0x2018,	4},//	当前无功需量	数据类型：double-long，单位：kvar，换算：-4
	{0x2019,	4},//	当前视在需量	数据类型：double-long，单位：kVA，换算：-4
	{0x201A,	4},//	当前电价	数据类型：double-long-unsigned，单位：元/kWh，换算：-4
	{0x201B,	4},//	当前费率电价	数据类型：double-long-unsigned，单位：元/kWh，换算：-4
	{0x201C,	4},//	当前阶梯电价	数据类型：double-long-unsigned，单位：元/kWh，换算：-4
	//201E	8	事件发生时间	数据类型：date_time_s
	//2020	8	事件结束时间	数据类型：date_time_s
	//2021	8	数据冻结时间	数据类型：date_time_s
	//2022	8	事件记录序号	数据类型：double-long-unsigned
	//2023	8	冻结记录序号	数据类型：double-long-unsigned
	//2024	8	事件发生源 具体对象定义。
	//2025	8	事件当前值 structure
	//{
	//事件发生次数 double-long-unsigned，
	//事件累计时间 double-long-unsigned（单位：秒，无换 算）
	//}
	{0x2026,	2},//	电压不平衡率	数据类型：long-unsigned，单位：%，换算：-2
	{0x2027,	2},//	电流不平衡率	数据类型：long-unsigned，单位：%，换算：-2
	{0x2028,	2},//	负载率	数据类型：long-unsigned，单位：%，换算：-2
	{0x2029,	2},//	安时值	属性 2 安时数值∷=array 相安时值相安时值∷=double-long-unsigned，单位：Ah，换算：-2 相安时值包总、A、B、C 相。
	//202A	8	目标服务器地址	属性 2∷=TSA
	 
	
	//对象标识 OI 接口类 IC	对象名称	实例的对象属性及方法定义
	{0x202C,	2},//	（当前）钱包文件	数值∷=structure
	//{
	//剩余金额	double-long-unsigned（单位：元，换算：-2）， 购电次数	double-long-unsigned
	//}
	{0x202D,	2},//	（当前）透支金额	数据类型：double-long-unsigned，单位：元，换算：-2
	{0x202E,	2},//	累计购电金额	数据类型：double-long-unsigned，单位：元，换算：-2
	{0x2031,	2},//	月度用电量 属性 2 用电量∷=double-long-unsigned， 单位：kWh，换算：-2
	{0x2032,	2},//	阶梯结算用电量	属性 2 用电量∷=double-long-unsigned， 单位：kWh，换算：-2
	//2040	6	控制命令执行状态字 数据类型：bit-string(SIZE(16))，无单位，无换算
	//2041	6	控制命令错误状态字 数据类型：bit-string(SIZE(16))，无单位，无换算
	//2100	14	分钟区间统计	统计周期单位为分钟
	//2101	14	小时区间统计	统计周期单位为小时
	//2102	14	日区间统计 统计周期单位为日
	//2103	14	月区间统计 统计周期单位为月
	//2104	14	年区间统计 统计周期单位为年
	//2110	15	分钟平均	统计周期单位为分钟
	//2111	15	小时平均	统计周期单位为时
	//2112	15	日平均	统计周期单位为日
	//2113	15	月平均	统计周期单位为月
	//2114	15	年平均	统计周期单位为年
	//2120	16	分钟极值	统计周期单位为分钟
	//2121	16	小时极值	统计周期单位为时
	//2122	16	日极值	统计周期单位为日
	//2123	16	月极值	统计周期单位为月
	//2124	16	年极值	统计周期单位为年
	//2131	6	当月 A 相电压合格率 属性 2（电压合格率数据）∷=structure
	//{
	//当日电压合格率	电压合格率，
	//当月电压合格率	电压合格率
	//}
	
	
	//电压合格率∷=structure
	//{
	//电压监测时间 double-long-unsigned（单位：分钟，无换 算），
	 
	
	//对象标识 OI 接口类 IC	对象名称	实例的对象属性及方法定义
	{0x2131,	2},//	当月 A 相电压合格率 电压合格率 long-unsigned（单位：%，换算：-2）， 电压超限率	long-unsigned（单位：%，换算：-2）， 电压超上限时间  double-long-unsigned（单位：分钟，无换算），电压超下限时间 double-long-unsigned（单位：分钟， 无换算）}
	{0x2132,	2},//	当月 B 相电压合格率 同 2131
	{0x2133,	2},//	当月 C 相电压合格率 同 2131
	//2140	2	日最大有功功率及发生时间	最大功率及发生时间∷=structure
	//{
	//最大功率值 double-long-unsigned， 发生时间	date_time_s
	//}
	//功率单位：kW，换算：-4
	{0x2141,	4},//	月最大有功功率及发生时间	同 2140。
	//2200	6	通信流量	数值∷=structure
	//{
	//当日通信流量 double-long-unsigned， 当月通信流量 double-long-unsigned
	//}
	//单位：byte，换算：0
	//2203	6	供电时间	数值∷=structure
	//{
	//日供电累计时间 double-long-unsigned， 月供电累计时间 double-long-unsigned
	//}
	//单位：分钟，换算：0
	//2204	6	复位次数	数值∷=structure
	//{
	//日复位累计次数 long-unsigned， 月复位累计次数 long-unsigned
	//}
	//2301	23	总加组 1 
	//2302	23	总加组 2 
	//2303	23	总加组 3 
	//2304	23	总加组 4 
	//2305	23	总加组 5 
	 
	
	//对象标识 OI 接口类 IC	对象名称	实例的对象属性及方法定义
	//2306	23	总加组 6 
	//2307	23	总加组 7 
	//2308	23	总加组 8 
	//2401	12	脉冲计量 1	
	//2402	12	脉冲计量 2	
	//2403	12	脉冲计量 3	
	//2404	12	脉冲计量 4	
	//2405	12	脉冲计量 5	
	//2406	12	脉冲计量 6	
	//2407	12	脉冲计量 7	
	//2408	12	脉冲计量 8	
	//2500	6	累计水（热）流量	数据类型：double-long-unsigned，单位：m3，换算：-4
	//（注：水表和热表共用）
	{0x2501,	4},//	累计气流量 数据类型：double-long-unsigned，单位：m3，换算：-4
	{0x2502,	2},//	累计热量	数据类型：double-long-unsigned，单位：J，换算：-2
	{0x2503,	2},//	热功率	数据类型：double-long-unsigned，单位：J/h，换算：-2
	//2504	6	累计工作时间	数据类型：double-long-unsigned，单位：小时，换算：0
	{0x2505,	2},//	水温	数值∷=structure
	//{
	//供水温度 double-long-unsigned， 回水温度 double-long-unsigned
	//}
	//单位：℃，换算：-2。
	//2506	6	（仪表）状态 ST	数值∷=structure
	///{
	//阀门状态  enum{开（0），关（1），保留（2），异常（3）}， 电池电压  enum{正常（0），欠压（0）}
	//}
};
const u32 OAD_PointNUMSIZE=sizeof(OAD_PointNUM_list)/sizeof(POINTNUM_type);



u32 Str_CRandLF(u8 *p)//回车和换行
{
#ifdef UNFORMAT_JSON
	*p=0x0D;
	p++;
	*p=0x0A;

	return 2;
#else
	return 0;
#endif
}

u32 Str_CR(u8 *p)//回车
{
	*p=0x0D;
	return 1;
}

u32 Str_LF(u8 *p)//换行
{
	*p=0x0A;
	return 1;
}

u32 Str_SPASE(u8 *p)//空格
{
	*p=0x20;
	return 1;
}

u32 Str_nSPASE(u8 *p,u32 NUM)//空格
{
#ifdef UNFORMAT_JSON
	u32 i;
	for(i=0;i<NUM;i++)
	{
		*p=0x20;
		p++;
	}
	return NUM;
#else
	return 0;
#endif
}

u32 Str_COMMA(u8 *p)//逗号
{
	*p=0x2C;
	return 1;
}

u32 Str_QM(u8 *p)//引号
{
	*p=0x22;
	return 1;
}

u32 Str_COL(u8 *p)//冒号
{
	*p=0x3A;
	return 1;
}
u32 Str_L_SQB(u8 *p)//左方括号
{
	*p=0x5B;
	return 1;
}

u32 Str_R_SQB(u8 *p)//右方括号
{
	*p=0x5D;
	return 1;
}
u32 Str_L_CUB(u8 *p)//左花括号
{
	*p=0x7B;
	return 1;
}

u32 Str_R_CUB(u8 *p)//右花括号
{
	*p=0x7D;
	return 1;
}

void myprintf(char *pStr,u32 param1,u32 param2,u32 param3)
{
#ifdef DEBUG_JSON
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf((char *)pStr,param1,param2,param3);
#endif
}

u32 mystrlen(const char *p)//计算字符串长度
{
	u32 i;
	i=0;
	while((*p!='\0')||(*p!=0))
	{
		i++;
		p++;
	}
	return i;
}

void ByteInverte(u8 *ps,u8 *pc,u32 Len)//ps中Len字节倒叙输出到pc中
{
	u32 i;
	u32 j;
	j=0;
	for(i=Len;i>0;i--)
	{
		pc[j]=ps[i-1];
		j++;
	}
}
u32 mymemcmp(u8 *p,u32 val,u32 Len)//比较两个数据,1不相同,0相同
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		if(*p!=val)
		{
			return 1;
		}
		p++;
	}
	return 0;
}
void mystrcpy(char *p,const char *pc)//pc字节拷贝到p里面去
{
	u32 pclen;
	u32 i;
	pclen=mystrlen(pc);
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncpy(char *p,const char *pc,u32 Len)//pc字节拷贝n个到p里面去
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}
void mystrncat(char *p,const char *pc,u32 Len)//p后面连接Len长度pc字符
{
	u32 plen;
	//u32 pclen;
	u32 i;
	plen=mystrlen(p);
	//pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<Len;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Data_Ascii(u64 ida,u8 *p,u32 NUM)//将ida值存到p中,NUM为小数点的位数
{
	u32 i;
	u64 data;
	u32 Len=0;
	u8 tm;
	data=hex_bcd16(ida);
	if(0==data)
	{	
		*p=0x30;
		p++;
		Len++;
		for(i=0;i<NUM;i++)
		{			
			if(0==i)
			{
				*p='.';	
				p++;
				Len++;
				*p=0x30;
				Len++;
			}else
			{
				*p=0x30;
				Len++;
			}
			p++;
		}
	}else
	{
		for(i=16;i>0;i--)
		{
			tm=(data>>((i-1)*4))&0xf;
			if(NUM==i)
			{
				*p='.';	
				p++;	
				Len++;
			}
			if(0!=tm)
			{
				*p=tm+0x30;
				p++;
				Len++;
			}else
			{
				if(Len!=0||NUM+1==i)
				{
					*p=tm+0x30;
					p++;
					Len++;
				}
			}
		}
	}
	*p=0;
	return Len;
}

void mystrcat(char *p,const char *pc)//p字符连接pc到p
{
	u32 plen;
	u32 pclen;
	u32 i;
	plen=mystrlen(p);
	pclen=mystrlen(pc);
	p+=plen;
	for(i=0;i<pclen;i++)
	{
		*p=*pc;
		pc++;
		p++;
	}
	*p=0;
}

u32 Cmp_Ascii_Inverte(u8 *p,u8 *pc,u32 Byte)//字节pc转成asc码倒序跟p比较
{
	u32 i;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pc[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

u32 Cmp_Ascii(u8 *p,u8 *pc,u32 Byte)//字节pc转成asc码跟p比较
{
	u32 i;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Add_StrToASCII_Inverte(u8 *pStr,u8 *pbuf,u32 Byte)//pbuf里面的字节倒序输出到pstr并转成asc码
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=Byte;i>0;i--)
	{
		data=pbuf[i-1];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Add_StrToASCII(u8 *pStr,u8 *pbuf,u32 Byte)//pbuf里面的字节输出到pstr并转成asc码
{
	u32 i;
	u32 j=0;
	u8 data;
	u32 x;
	for(i=0;i<Byte;i++)
	{
		data=pbuf[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		pStr[j++]=x;
		
	}
	pStr[j]=0;
}

void Get698CurTime_Ascii(u8 *p8,u32 subD)//得到698当前时间转换成asc码,减去subD
{
	u32 i;
	u32 x;
	u8 data;
	u8 *p8s;
	//写时间
	//年
	u32 YYMD;
	YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
	if(subD!=0)
	{
		for(i=0;i<subD;i++)
			YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日	
	}
	
	p8s=(u8*)ADDR_4KWRITE_BUFF;	
	p8s[0]=YYMD>>24;
	p8s[1]=YYMD>>16;
	p8s[2]=YYMD>>8;
	p8s[3]=YYMD>>0;
	p8s[4]=Comm_Ram->DL698YMDHMS[4];//时间:时
	p8s[5]=Comm_Ram->DL698YMDHMS[5];//时间:分
	p8s[6]=Comm_Ram->DL698YMDHMS[5];//时间:秒
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(p8s[0]<<8)|p8s[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p8s[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

void Get698Time_Ascii(u8 *p,u8 *p8)//p时间前两个字节表示年份p8输出asc码
{
	u32 i;
	u32 x;
	u8 data;
	//写时间
	//年
	x=(p[0]<<8)|p[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<7;i++)
	{
		data=p[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	*p8=0;
}

u32 Cmp698Time_Ascii(u8 *p,u8 *pc,u32 Byte)//698时间比较pc前两个字节代表年份
{
	u32 i;
	u8 data;
	u32 x;
	u8 ptim[4]={0};
	//写时间
	//年
	x=(pc[0]<<8)|pc[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(ptim,(u8 *)&x,2);
	//先比较年份
	for(i=0;i<4;i++)
	{
		if(*p!=ptim[i])
		{
			return 1;
		}
	}
	for(i=2;i<Byte;i++)
	{
		data=pc[i];
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		if(*p!=x)
		{
			return 1;
		}
		p++;
	}
	return 0;
}

void Get698Time_HMS_Ascii(u8 *pt,u8 *p8,u8 hour,u8 min,u8 sec)//p时间前两个字节表示年份p8输出asc码
{
	u32 i;
	u32 x;
	u8 data;
	//写时间
	//年
	//x=(Comm_Ram->DL698YMDHMS[0]<<8)|Comm_Ram->DL698YMDHMS[1];
	x=(pt[0]<<8)|pt[1];
	x=hex_bcd(x);
	Add_StrToASCII_Inverte(p8,(u8 *)&x,2);
	p8+=4;
	for(i=2;i<4;i++)
	{
		//data=Comm_Ram->DL698YMDHMS[i];
		data=pt[i];
		data=hex_bcd(data);
		x=data>>4;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
		x=data&0xf;
		if(x<10)
		{
			x+=0x30;
		}
		else
		{
			x+=55;
		}
		*p8=x;
		p8++;
	}
	data=hex_bcd(hour);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(min);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	data=hex_bcd(sec);
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	*p8=x;
	p8++;
	*p8=0;
}
void GetNullTime(u8 *pt)//得到一个JSON格式要求的空时间格式
{
	u32 offset;
	offset=0;
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]='-';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=' ';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=':';
	pt[offset++]='0';
	pt[offset++]='0';
	pt[offset++]=0;
}
void GetJsonTime(u8 *pt)//把处理好的时间buf pt转换成JSON文件里面的时间格式（2020-05-01 20:00:00）
{
	u8 tt[14];
	MW((u32)pt,(u32)tt,sizeof(tt));
	//年
	pt[0]=tt[0];
	pt[1]=tt[1];
	pt[2]=tt[2];
	pt[3]=tt[3];	
	//月
	pt[4]='-';
	pt[5]=tt[4];
	pt[6]=tt[5];
	//日
	pt[7]='-';
	pt[8]=tt[6];
	pt[9]=tt[7];

	pt[10]=' ';
	//时
	pt[11]=tt[8];
	pt[12]=tt[9];
	pt[13]=':';
	//分
	pt[14]=tt[10];
	pt[15]=tt[11];
	pt[16]=':';
	//秒
	pt[17]=tt[12];
	pt[18]=tt[13];
	pt[19]=0;
}

void WriteNullString(u8 *p8)//数据BUF里面写ASC 的NULL
{
	p8[0]='n';
	p8[1]='u';
	p8[2]='l';
	p8[3]='l';
	p8[4]=0;	
}

u32 Get_DataPointNUM(u16 OI)//在OAD_PointNUM_list表格中找OAD并得到这个OAD的小数位数
{
	u32 i;
	u32 ret=0;
	for(i=0;i<OAD_PointNUMSIZE;i++)
	{
		if(OAD_PointNUM_list[i].OI==OI)
		{
			ret=OAD_PointNUM_list[i].POINTNUM;
			break;
		}
	}
	return ret;
}
u32 DealMetData_Ascii(u16 OI,u8 *pdata,u8 *pout,u32 isReal)//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
{
	u32 i;
	u32 datalen;
	u32 pointnum;
	u64 data;
	i=Get_DL698DataLen_S(pdata,0);
	if(0==i)
	{		
		return i;
	}
	if(pdata[0])
	{
		i--;
		data=0;
		if(i>8)
		{
			i=8;
		}
		datalen=i;
		while(i>0)
		{
			pdata++;
			i--;
			data|=((*pdata)<<(i*8));
		}
		if((data&0xffffffff)==0xffffffff || (data&0xffffffff)==0x7fffffff)
		{
			if(!isReal)
			{
				WriteNullString(pout);//数据BUF里面写ASC 的NULL
				i=4;
			}else
			{
				pout[0]=0;
				i=1;
			}
		}else
		{
			switch (OI>>8)
			{
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
					pointnum=2;
					break;
				case 0x10:
				case 0x11:
					pointnum=4;
					break;
				default:
					pointnum=Get_DataPointNUM(OI);
					break;		
			}
			i=0;
			i=(data>>((datalen-1)*8));
			if(i>>4)//这个数据是负数
			{
				*pout='-';
				pout++;
				i=(u32)data;
				data=(u64)(~(i)+1);
			}
			i=Data_Ascii(data,pout,pointnum);//将ida值存到p中,NUM为小数点的位数			
		}
	}else
	{
		if(!isReal)
		{
			WriteNullString(pout);//数据BUF里面写ASC 的NULL
			i=4;
		}else
		{
			pout[0]=0;
			i=1;
		}
	}
	return i;
}

u32 Createitem_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//创建JSON格式: "XXXXX":"XXXXXXX",
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//冒号
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//逗号
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemNum_Json(u8 *pout,u8 *ph,u8 *pd,u32 spacenum)//创建JSON格式: "XXXXX":XXXXXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//冒号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)pd);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_COMMA(pout+CJSONLEN);//逗号
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateitemArrayHead_Json(u8 *pout,u8 *ph,u32 spacenum)//创建JSON格式: "XXXXX":[
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//冒号
	CJSONLEN+=i;
	i=Str_L_SQB(pout+CJSONLEN);//左方括号
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateitemArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//创建JSON格式: ],
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_R_SQB(pout+Len+CJSONLEN);//右方括号
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//逗号
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateArrayHead_Json(u8 *pout,u32 spacenum)//创建JSON格式: {
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_L_CUB(pout+CJSONLEN);//左花括号
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 CreateArrayEnd_Json(u8 *pout,u32 Len,u32 spacenum)//创建JSON格式: },
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pout+Len+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+Len+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_R_CUB(pout+Len+CJSONLEN);//右花括号
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//逗号
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}

u32 CreateDataHead_Json(u8 *pout,u8 *ph,u32 spacenum)//创建JSON格式: "XXXXXXX","
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=Str_CRandLF(pout+CJSONLEN);//回车和换行
	CJSONLEN+=i;
	i=Str_nSPASE(pout+CJSONLEN,spacenum);//空格
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	mystrcpy((char *)pout+CJSONLEN,(const char *)ph);
	i=mystrlen((const char *)pout+CJSONLEN);
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COL(pout+CJSONLEN);//冒号
	CJSONLEN+=i;
	i=Str_QM(pout+CJSONLEN);//引号
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateData_Json(u8 *pout,u8 *pd)//创建JSON格式: XXXX,
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	if(pd[0]!=0)
	{
		mystrcpy((char *)pout+CJSONLEN, (const char *)pd);
		i=mystrlen((const char *)pout+CJSONLEN);
		CJSONLEN+=i;
	}
	i=Str_COMMA(pout+CJSONLEN);//逗号
	CJSONLEN+=i;

	return CJSONLEN;
}

u32 CreateDataEnd_Json(u8 *pout,u32 Len)//创建JSON格式: ",
{
	u32 i;
	u32 CJSONLEN;
	u8 flag=0;
	CJSONLEN=0;
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	//CJSONLEN=Len;
	i=Str_QM(pout+Len+CJSONLEN);//引号
	CJSONLEN+=i;
	i=Str_COMMA(pout+Len+CJSONLEN);//逗号
	CJSONLEN+=i;
	if(0!=flag)
	{
		CJSONLEN--;
	}
	return CJSONLEN;
}
u32 Delet_COMMA(u8 *pout,u32 Len)//删除逗号,返回总长
{
	if(Len>0)
	{
		if(','==pout[Len-1])
		{
			Len--;
		}
	}
	return Len;
}
u32 Get_OneFreDataLen_FromFlash(u8 *pIn)//计算一个分时段数据包数据的长度,pin导入的是完整的包包含21个时标字节
{
	u8 OADNUM;

	u32 i;
	u32 x;

	u32 Offset;
	Offset=21;

	Offset+=4;//ROAD

	Offset+=4;//OADNUM
	OADNUM=pIn[Offset];
	Offset++;
	Offset+=OADNUM*4;

	for(x=0;x<OADNUM;x++)
	{		
		i=Get_DL698DataLen_S(pIn+Offset,0);//算出第一个数据的长度
		Offset+=i;
	}
	return Offset;
}

u32 Day_Month_PackJson(u8 *pInbuf,u8 *pout)//日月冻结数据打包pINbuf前两个字节表示一共多少数据包
{
	u8 *p8;
	u8 *p8s;
	u8 *p8Str;
	u8 *pdata;
	u16 *p16;
	
	u8 NUM;//数据个数
	u32 ROAD;//ROAD起始位置
	u8 OADNUM;//OAD的数量
	u8 OADPORT;//OAD起始位置
	u8 DataNUM;
	
	u16 TotalNUM;
	u32 OAD;

	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Offset;
	u32 CJSONLEN=0;

	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];//一共的采集数量
	pInbuf+=2;
	while(TotalNUM>0)
	{
		TotalNUM--;	
		Offset=21;//7个字节采集启动时标,采集成功时标,采集结束时标
		if(pInbuf[Offset]!=1)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		p8=(u8 *)ADDR_DATABUFF; 
		Offset++;
		NUM=pInbuf[Offset];//数据个数
		//myprintf("[%s:%d]:NUM=%d\n",(u32)&__func__,(u32)__LINE__,NUM);
		Offset++;
	for(x=0;x<NUM;x++)
	{
		p8Str=Get_Element(pInbuf+21,1+x,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(!p8Str)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
			#endif
			goto JSON_ERROR;
		}
		Offset=0;
		if(p8Str[Offset]!=0x5b)//PROAD
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:采集到的数据存储格式有错误 p8Str[Offset]=%02x\n",(u32)&__func__,__LINE__,p8Str[Offset]);
			#endif
			goto JSON_ERROR;
		}
		Offset+=2;
		
		ROAD=(p8Str[Offset]<<24)|(p8Str[Offset+1]<<16)|(p8Str[Offset+2]<<8)|p8Str[Offset+3];
		//myprintf("[%s:%d]:ROAD=%08x\n",(u32)&__func__,__LINE__,ROAD);
		Offset+=4;
		OADNUM=p8Str[Offset];//OAD的数量
		Offset++;
		OADPORT=Offset;//OAD起始位置
		Offset+=OADNUM*4;
		p8s=p8Str+Offset;
		//找冻结时标的位置
		for(y=0;y<OADNUM;y++)
		{
			p8s=p8Str+Offset;
			p8s=Get_Element(p8s,y+1,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(!p8s)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
				#endif
				goto JSON_ERROR;
			}
			OAD=(p8Str[OADPORT+y*4]<<24)|(p8Str[OADPORT+1+y*4]<<16)|(p8Str[OADPORT+2+y*4]<<8)|(p8Str[OADPORT+3+y*4]);
			if(0x20210200==OAD)//冻结时标
			{
				p8s+=8;//一个标识7个数据
				continue;
			}
			if(1==p8s[0])//数组
			{
				DataNUM=p8s[1];
				if(0==p8s[1])
				{
					DataNUM=1;
				}
			}else
			{
				DataNUM=1;
			}
			for(z=0;z<DataNUM;z++)
			{
				if(0!=p8s[1])
				{
					pdata=Get_Element(p8s,z+1,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
					if(!pdata)
					{
						#ifdef DEBUG_JSON
						myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
						#endif
						goto JSON_ERROR;
					}
				}
				i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=ROAD>>16;
				Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf里面的字节倒序输出到pstr并转成asc码
				i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=OAD+z+1;
				Add_StrToASCII_Inverte(p8,(u8 *)&i,4);//pbuf里面的字节倒序输出到pstr并转成asc码(p8, p8+4,4);
				i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+7,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				Get698Time_Ascii(pInbuf+14,p8);
				GetJsonTime(p8);
				i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//将ida值存到p中,NUM为小数点的位数
				i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=Data_Ascii(1,p8,0);//将ida值存到p中,NUM为小数点的位数
				i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				
				i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				if(0==p8s[1])
				{//全部写NULL
					if(((OAD>>28)&0xf) == 1)//需量
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
						
						GetNullTime(p8);
						i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;	
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=0;
						i=DealMetData_Ascii(OAD>>16,(u8 *)&i,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}
				}else
				{
					if(((OAD>>28)&0xf) == 1)//需量
					{	
						i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
		
						if(pdata[0]==0x02)
						{		
							pdata+=2;
							//处理需量的数据
							i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
							if(0==i)
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
								#endif
								goto JSON_ERROR;
							}
							i=CreateitemNum_Json(pout+CJSONLEN, STR_MV, p8, DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
							//处理需量的最大发生时间
							i=Get_DL698DataLen_S(pdata,0);
							pdata+=i;
							if(pdata[0]==0x1c)//需量的时间
							{
								pdata++;
								Get698Time_Ascii(pdata,p8);
								GetJsonTime(p8);
							}else
							{
								GetNullTime(p8);
							}
							i=Createitem_Json(pout+CJSONLEN, STR_MDT,p8,DEBUG_THIRD_SPACENUM);
							CJSONLEN+=i;
						}
						
						i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_THIRD_SPACENUM);
						CJSONLEN+=i;
					}else
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
							#endif
							goto JSON_ERROR;
						}

						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;

					}	
				}
				i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json( pout, CJSONLEN, DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
			}
		}
	}
	i=Get_OneFreDataLen_FromFlash(pInbuf);
	pInbuf+=i;
}
	return CJSONLEN;
JSON_ERROR:
	return 0;
}

void Add_NewOADToBuff(u8 *pOAD,u8 *pBuff,u8 OADNUM)//pBuff中的OAD添加到pPAD中 数量为OADNUM
{
	u8 x;	
	pOAD[0]=OADNUM;
	pOAD++;
	for(x=0;x<OADNUM;x++)
	{
		MW((u32)pBuff+x*4,(u32)pOAD+x*4,4);
	}
}

u32 IsFindOneOADFromBuff(u8 *pOAD,u8 *pBuff)//从储存好的POAD buf中找PBUFF对应OAD,如果找到就说明在pOAD中存在pBuff这个OAD
{

	u8 x;
	u8 NUM;
	u32 OAD;
	u32 OAD1;
	u32 ret=1;
	NUM=pOAD[0];
	pOAD++;

	OAD1=(pBuff[0]<<24)|(pBuff[1]<<16)|(pBuff[2]<<8)|pBuff[3];
	if(0x20210200==OAD1)
	{
		return 0;
	}
	for(x=0;x<NUM;x++)
	{
		OAD=(pOAD[x*4]<<24)|(pOAD[x*4+1]<<16)|(pOAD[x*4+2]<<8)|pOAD[x*4+3];	
		if(OAD==OAD1)
		{
			break;
		}
	}
	if(x>=NUM)
	{
		ret=0;
	}	
	return ret;
}

u8 *Find_DataHead(u8 *pIn,u8 *pOAD,u32 oadcnt)//根据Flash里面的存储格式用pIn里面OAD的位置去找pIn里面数据的位置,pOAD第一字节是OAD数量,oadcnt是pOAD里面的偏移,返回Pin里面数据的正确位置
{
	u8 OADNUM;
	u32 i;
	u32 y;
	u32 Offset;
	pIn+=21;
	Offset=0;
	Offset+=4;
	Offset+=4;

	OADNUM=pIn[Offset];
	Offset++;
	Offset+=(OADNUM*4);
	
	if(oadcnt>OADNUM-1)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:搜索出来的OAD下标大于实际的OAD总数\n",(u32)&__func__,__LINE__,0);
		#endif
		return 0;
	}
	pIn+=Offset;
	Offset=0;
	for(i=0;i<=oadcnt;i++)
	{
		if(i==oadcnt)
		{			
			return pIn+Offset;
		}
		y=Get_DL698DataLen_S(pIn+Offset,0);//算出第一个数据的长度
		Offset+=y;
	}
	return 0;
}

u32 JudegMin_FromTime(u8 *pch,u32 cnt,u32 Min)//从flash里面的时间去判断这个时间点的分钟存不存在(如有1440分钟从0分钟开始判断)
{
	u8 hh;
	u8 mm;
	u32 i;
	pch+=14;
	//采集存储时标
	//07 00 01 02 00 00 00
	pch+=4;
	hh=pch[0];
	mm=pch[1];
	i=hh*60+mm;
	
	if(i==(cnt*Min))
	{
		return 1;
	}
	return 0;
}

u32 Is_SingleOADNUM_FromData(u8 *pOAD,u8 *pData,u32 OADNUM)//比如说这个OAD是00100200 他一共有5项 总尖峰平谷
{
	u32 i;
	u32 n;
	u32 Len;
	u32 OAD;
	OAD=(pOAD[0]<<24)|(pOAD[1]<<16)|(pOAD[2]<<8)|pOAD[3];

	if((OAD&0xff)==0)
	{
		Len=0;
		for(i=0;i<OADNUM;i++)
		{
			n=Get_DL698DataLen_S(pData+Len,0);//算出第一个数据的长度
			Len+=n;
		}
		if(1==pData[Len])//是个数组（一般都是数组）
		{
			i=pData[Len+1];
			if(i>1)
			{
				return i;
			}else
			{
				return 0;
			}
		}
	}
	return 0;
}

u32 Min_PackJson(u8 *pInbuf,u8 *pout,u32 Min)//分钟冻结数据的打包MiN区分是15分钟还是1分钟
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN;
	u8 *p8;
	u8 *pOAD;
	u8 *pbuf=pInbuf;
	u8 *pdata;//指向数据区的指针

	u32 Offset;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//记录报文中ROAD
	u32 TIMESPOT;	//时间标签的位置
	u8 OADNUM;		//记录报文中ROAD数量
	u8 OADNUM_S;
	u32 OADSPOT; 	//记录报文中ROAD数量的位置//20210200
	u32 Fre_Dst;	//数据冻结密度
	u16 *p16;
	CJSONLEN=0;
	p16=(u16 *)pbuf;
	TotalNUM=p16[0];
	if(0==TotalNUM)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:没有数据内容 %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	pbuf+=2;//数据总数长度
	for(TIMESPOT=0;TIMESPOT<1440;TIMESPOT++)
	{
		x=mymemcmp(pbuf+TIMESPOT*ONEDATASIZE, 0, 2);
		if(x)
		{
			break;
		}
	}
	if(TIMESPOT>=1440)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:没有数据内容 %d\n", (u32)&__func__, __LINE__, TotalNUM);
		#endif
		return 0;
	}
	Offset=TIMESPOT*ONEDATASIZE;//先找到一个有效,拿这一个有效的头搞事情
	p8=(u8 *)ADDR_DATABUFF;
	
	pOAD=(u8 *)ADDR_DATABUFF+1024;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);

	Offset+=21;

	Offset+=4;
	ROAD=(pbuf[Offset]<<24)|(pbuf[Offset+1]<<16)|(pbuf[Offset+2]<<8)|pbuf[Offset+3];
	if(ROAD!=0x50020200)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:不是分钟冻结数据\n", (u32)&__func__, __LINE__, 0);
		#endif
		goto Min_JSON_ERROR;
	}
	Offset+=4;
	OADNUM=pbuf[Offset];
	Offset++;	
	Add_NewOADToBuff(pOAD,pbuf+Offset,OADNUM);//pBuff中的OAD添加到pPAD中 数量为OADNUM
	OADSPOT=Offset;
	Offset+=(OADNUM*4);	
	for(y=0;y<OADNUM;y++)
	{
		if(!IsFindOneOADFromBuff(pOAD,pbuf+OADSPOT+y*4))//从储存好的POAD buf中找PBUFF对应OAD,如果找到就说明在pOAD中存在pBuff这个OAD
		{	
			continue;
		}
		
		i=Is_SingleOADNUM_FromData(pbuf+OADSPOT+y*4,pbuf+Offset,y);
		if(0==i)
		{
			i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=ROAD>>16;
			Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf里面的字节倒序输出到pstr并转成asc码
			i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]<<0);
			Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbuf里面的字节输出到pstr并转成asc码
			i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//p时间前两个字节表示年份p8输出asc码
			//Get698Time_HMS_Ascii(p8,0,0,0);
			//p8[3]-=1;
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			switch(Min)
			{
				case 1:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//采集存储时标(数据时标)
					break;
				case 15:
					Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
					//cJSON_AddStringToObject(obj,"dt",(char*)p8);//采集存储时标(数据时标)
					break;
			}
			GetJsonTime(p8);
			i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=Data_Ascii((u64)Min,p8,0);//将ida值存到p中,NUM为小数点的位数
			i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			Fre_Dst=(24*60)/Min;
			i=Data_Ascii((u64)Fre_Dst,p8,0);//将ida值存到p中,NUM为小数点的位数
			i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			//判断分钟点数据存不存在		

			for(z=0;z<Fre_Dst;z++)
			{	
				x=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
				if(x)
				{
					pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//根据Flash里面的存储格式用pIn里面OAD的位置去找pIn里面数据的位置,pOAD第一字节是OAD数量,oadcnt是pOAD里面的偏移,返回Pin里面数据的正确位置
					if(pdata)
					{	
						i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
						if(0==i)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
							#endif
							goto Min_JSON_ERROR;
						}
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}else
					{
						WriteNullString(p8);//数据BUF里面写ASC 的NULL
						i=CreateData_Json(pout+CJSONLEN, p8);
						CJSONLEN+=i;
					}
				}else
				{
					WriteNullString(p8);//数据BUF里面写ASC 的NULL
					i=CreateData_Json(pout+CJSONLEN, p8);
					CJSONLEN+=i;
				}
			}

			i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
			CJSONLEN+=i;
			
			i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
		}
		else//处理交采的OAD,有OAD合并的项
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:交采15分钟数据处理\n",(u32)&__func__,__LINE__,0);
			#endif
				OADNUM_S=i;//数组的个数就是OAD的分量的个数
				OAD=(pbuf[OADSPOT+y*4]<<24)|(pbuf[OADSPOT+y*4+1]<<16)|(pbuf[OADSPOT+y*4+2]<<8)|(pbuf[OADSPOT+y*4+3]);
				for(x=1;x<=OADNUM_S;x++)//循环记录每一个OAD有多少个子OAD
				{
					i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
					i=ROAD>>16;
					Add_StrToASCII_Inverte(p8,(u8 *)&(i),2);//pbuf里面的字节倒序输出到pstr并转成asc码
					i=Createitem_Json(pout+CJSONLEN, STR_TYPE, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					OAD&=0xffffff00;
					OAD|=x;
					Add_StrToASCII_Inverte(p8,(u8 *)&OAD,4);//pbuf里面的字节输出到pstr并转成asc码
					i=Createitem_Json(pout+CJSONLEN, STR_ITEMID, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Get698Time_Ascii(pbuf+TIMESPOT*ONEDATASIZE+7,p8);//p时间前两个字节表示年份p8输出asc码
					//Get698Time_HMS_Ascii(p8,0,0,0);
					//p8[3]-=1;
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_OT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					switch(Min)
					{
						case 1:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,59,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//采集存储时标(数据时标)
							break;
						case 15:
							Get698Time_HMS_Ascii(pbuf+TIMESPOT*ONEDATASIZE+14,p8,23,45,0);
							//cJSON_AddStringToObject(obj,"dt",(char*)p8);//采集存储时标(数据时标)
							break;
					}
					GetJsonTime(p8);
					i=Createitem_Json(pout+CJSONLEN, STR_DT, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=Data_Ascii((u64)Min,p8,0);//将ida值存到p中,NUM为小数点的位数
					i=CreateitemNum_Json(pout+CJSONLEN, STR_INTY, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					Fre_Dst=(24*60)/Min;
					i=Data_Ascii((u64)Fre_Dst,p8,0);//将ida值存到p中,NUM为小数点的位数
					i=CreateitemNum_Json(pout+CJSONLEN, STR_NUM, p8, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateitemArrayHead_Json(pout+CJSONLEN, STR_DL, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;

					//判断分钟点数据存不存在	
					for(z=0;z<Fre_Dst;z++)
					{
						i=mymemcmp(pbuf+z*ONEDATASIZE*Min, 0, 2);
						if(i)
						{
							pdata=Find_DataHead(pbuf+z*ONEDATASIZE*Min,pOAD,y);//根据Flash里面的存储格式用pIn里面OAD的位置去找pIn里面数据的位置,pOAD第一字节是OAD数量,oadcnt是pOAD里面的偏移,返回Pin里面数据的正确位置
							pdata=Get_Element(pdata,x,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
							if(pdata)
							{	
								i=DealMetData_Ascii(OAD>>16,pdata,p8,0);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii
								if(0==i)
								{
									#ifdef DEBUG_JSON
									myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
									#endif
									goto Min_JSON_ERROR;
								}
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}else
							{
								WriteNullString(p8);//数据BUF里面写ASC 的NULL
								i=CreateData_Json(pout+CJSONLEN, p8);
								CJSONLEN+=i;
							}
						}else
						{
							WriteNullString(p8);//数据BUF里面写ASC 的NULL
							i=CreateData_Json(pout+CJSONLEN, p8);
							CJSONLEN+=i;
						}
					}

					i=CreateitemArrayEnd_Json(pout, CJSONLEN, DEBUG_THIRD_SPACENUM);
					CJSONLEN+=i;
					
					i=CreateArrayEnd_Json(pout,CJSONLEN, DEBUG_SECOND_SPACENUM);
					CJSONLEN+=i;
				}
			}
	}
	
	return CJSONLEN;
Min_JSON_ERROR:
	return 0;
}

u32 RCSDtoFileID_JS(u16 *pAddr,u32 ROAD,u32 Min)//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff，MIN，1分钟还是15分钟做区分
{	
	if(0==ROAD)
	{	
		if(Min==15)
		{
			return 0x13f12000;
		}
		if(Min==1)
		{
			return 0x13f17000;
		}
	}else
	{
		switch(ROAD)
		{
			case 0x50040200:
				return 0x13f16000;
			case 0x50020200:
				if(Min==15)
				{
					return 0x13f19000;
				}
				if(Min==1)
				{
					return 0x13f22000;
				}
			case 0x50060200:
				return 0x13f18000;
		}
	}				
	return 0;//返回:0表示没找到无效FileID
}

u32 Filter_Time(u8 *pfiletm,u32 ROAD,u32 isReal,u32 All)//取文件的存储时间进行过滤,成功返回1,失败0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://分钟冻结
			//比较时标是上一天的
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}		
		  	break;
		case 0x50040200://日冻结
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			x=(pfiletm[0]<<16)|(pfiletm[1]<<24)|(pfiletm[2]<<8)|(pfiletm[3]);
			if(i==x)
			{
				ret=1;
			}
			break;
		case 0x50060200://月冻结
			//比较时标是上一天的
			i=(JSON->Lastrealtime[0]<<8)|(JSON->Lastrealtime[1]<<16)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 1);//年年月日减1日,返回减1后的年年月日
			x=(pfiletm[0]<<8)|(pfiletm[1]<<16)|pfiletm[2];
			if(i==x)
			{
				if(JSON->Lastrealtime[3]==1)//每个月的一号才回去打包月冻结
				{
					ret=1;
				}
			}			
			break;
		default://实时数据
		break;
	}
	return ret;
}

u32 Min_FrzNUM(u8 *pIn)//返回把小时分钟,转换成分钟数
{
	u8 hh;
	u8 mm;
	u32 i;
	pIn+=14;
	pIn+=4;
	hh=pIn[0];
	mm=pIn[1];
	i=60*hh+mm;
	return i;
}

u32 Filter_Same_Data(u8 *pt,u32 datanum,u8 *pdata)//用采集存储时标去判断是不是重复的数据,返回1是重复的数据,返回0不是重复的数据
{
	u32 i;
	u32 x;
	if(0==datanum)//一个数据都没有的时候默认没有重复的数据
	{
		return 0;
	}
	pdata+=14;//指向采集存储时标
	for(i=0;i<datanum;i++)
	{
		//删选到分钟
		for(x=0;x<6;x++)
		{
			if(pt[x]!=pdata[x])
			{
				break;
			}
		}
		if(x>=6)
		{
			return 1;
		}
		pt+=7;
	}
	return 0;
}

u32 Is_1MinData(u8 *pdata,u8 ACSAMPLE_FLAG)//判断这包数据是不是1分钟数据,1是.0不是,ACSAMPLE_FLAG交采没有1分钟数据
{
	u32 i;
	u32 OAD;
	u32 OADnum;
	if(ACSAMPLE_FLAG!=0)
	{
		return 0;
	}
	pdata+=21;
	pdata+=4;
	pdata+=4;
	OADnum=*pdata;
	pdata++;
	
	for(i=0;i<OADnum;i++)
	{
		OAD=(pdata[0]<<24)|(pdata[1]<<16)|(pdata[2]<<8)|(pdata[3]);
		if((OAD>>8)==0x200002)
		{
			return 1;
		}
		pdata+=4;
	}
	return 0;
}

u32 Filter_Realtime(u8 *ptime)//取文件的存储时间进行过滤,成功返回1,失败0
{
	u32 ret=0;
	u32 i;
	i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
	if(i==1)
	{
		ret=1;
	}	
	return ret;
}

u32 Is_ACSAMPLE(u16 *pAddr)
{
	u8 *p8;
	u8 *p8s;
	//u16 *p16;
	u16 *p16s;
	u32 i;
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p8s=Get_Element(p8,2,4,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(0x08!=p8s[2])//交采
			{
				p8+=LENper_6000;
				continue;
			}
			p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
			if(pAddr[0]==p16s[0])
			{
				if(pAddr[1]==p16s[1])
				{
					if(pAddr[2]==p16s[2])
					{
						return 0;
					}
				}
			}
					
		}
		p8+=LENper_6000;
	}
	#ifdef DEBUG_JSON
	myprintf("%04x %04x \n",pAddr[0],p16s[0],0);
	myprintf("%04x %04x \n",pAddr[1],p16s[1],0);
	myprintf("%04x %04x \n",pAddr[2],p16s[2],0);
	#endif
	return 1;
}

u32 Get_RecordData(u32 ROAD,u32 FileID,u16 *pAddr,u8 *pstack,u32 *DataLen,u32 isReal,u32 All)//用Flash中获取数据,返回一个数据指针,该指针需要Free释放
{
	//u32 iii;
	u32 z;
	u8 *p8;
	u8 *p8time;
	u32 MetNUM;
	u32 i;
	u32 n;
	u32 offset;
	u32 DataNUM;
	u32 filename;
	u8 ACSAMPLE_FLAG;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
	
	DataNUM=0;
	*DataLen=0;
	Comm_Ram->msFILESearchCount=0;	
	
	ACSAMPLE_FLAG=0;
	if(ROAD==0x50020200&&FileID==0x13f19000)//判断是不是交采档案,15分钟曲线
	{
		//是不是交采档案
		i=Is_ACSAMPLE(pAddr);
		if(i==0)
		{
			ACSAMPLE_FLAG=1;
		}
	}
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{		
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		if(filename==0xffffffff)
		{
			continue;
		}
		//只在Flash文件里面查找 RAM文件里面没有数据
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		if(0==ROAD)//实时增量用采集启动时标来判断
		{
			i=Filter_Realtime(file->save_date_time_s);//取文件的存储时间进行过滤,成功返回1,失败0
		}else
		{	
			i=Filter_Time(file->save_date_time_s,ROAD,isReal,All);//取文件的存储时间进行过滤,成功返回1,失败0
		}
		if(0==i)
		{
			continue;
		}
		if(0==ROAD)//里面可能有重复的地址但是数据内容不同也要取出来
		{
			p8time=(u8 *)ms->ms_malloc(7*1440);
			if(!p8time)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:申请空间失败\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				return 0;
			}
			n=file->NUM_DATA;
			//myprintf("[%s:%d]:新增表的数量 n=%d\n ",(u32)&__func__,(u32)__LINE__,n);	
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(MetNUM=0;MetNUM<n;MetNUM++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							i=RecordFileHead->LEN_DATA;
							if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
							{
								#ifdef DEBUG_JSON
								myprintf("[%s:%d]:取到的内容长度超限\n ",(u32)&__func__,(u32)__LINE__,0);	
								#endif
								continue;
							}
							ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
							p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
							if(0!=p8[0])//取到的数据内容有可能都是0的直接过滤掉,为了减少循环次数就查第一个字节
							{
								if(!Filter_Same_Data(p8time,DataNUM,p8))//用采集存储时标去判断是不是重复的数据,返回1是重复的数据,返回0不是重复的数据
								{
									MW((u32)p8,(u32)pstack+(*DataLen),i);
									MW((u32)p8+14,(u32)p8time+(DataNUM*7),7);//保存采集存储时标方便查重
									(*DataLen)+=i;
									DataNUM++;//查到一个新的数据自动加1	
								}
							}	
						}
								
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			ms->ms_free(p8time);
			p8time=NULL;
		}else
		{
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i>=n)
			{//没找到		
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:地址没有找到\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			i=RecordFileHead->LEN_DATA;
			if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:取到的内容长度超限\n ",(u32)&__func__,(u32)__LINE__,0);	
				#endif
				continue;
			}
			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);		
			
			p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));	
			
			if(0x50020200==ROAD)//如果是分钟,那就在帧的前面多加两个字节作为序号
			{
				z=Min_FrzNUM(p8);
				if(i<ONEDATASIZE&&z<1440)
				{
					if(FileID==0x13f22000)
					{
						if(Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//查到一个新的数据自动加1	
						}
					}else
					{
						if(!Is_1MinData(p8,ACSAMPLE_FLAG))
						{
							MW((u32)p8,(u32)pstack+z*ONEDATASIZE,i);	
							DataNUM++;//查到一个新的数据自动加1	
						}
					}
					
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("冻结数据从Flash里面读的有问题,需要修改 i=%d,z=%d\n ",i,z,0);	
					#endif
				}
				
			}else
			{	
				MW((u32)p8,(u32)pstack+(*DataLen),i);
				(*DataLen)+=i;
				DataNUM++;//查到一个新的数据自动加1	
			}
		}
	}
	return DataNUM;
}

u8 *Get_MetData_ROAD(u16 *pAddr,u32 ROAD,u32 Min,u32 isReal,u32 All)//根据ROAD来取Flash中的数据,pAddr表地址,Min取分钟冻结的时候用,1表示1分钟15表示15分钟,返回0无数据,否则返回一个数据buf指针,isReal是否是全网实时
{
	u16 *p16;
	u16 DataNUM;
	u32 Len;
	u32 i;
	u32 x;
	u8 *pstack=NULL;
	u32 FileID;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pstack=(u8 *)ms->ms_malloc(1024*1024);
	if(!pstack)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:申请空间失败\n ",(u32)&__func__,(u32)__LINE__,0);	
		#endif
		return 0;
	}
	MC(0,(u32)pstack,1024*1024);
	Len=0;
	DataNUM=0;
	switch(ROAD)
	{
		case 0x50020200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			break;
		case 0x50040200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f11000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		case 0x50060200:
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
			FileID=0x13f13000;
			x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
			Len+=i;
			DataNUM+=x;
			break;
		default://实时
			FileID=RCSDtoFileID_JS(pAddr,ROAD,Min);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff
			if(0!=FileID)
			{
				x=Get_RecordData(ROAD,FileID,pAddr,pstack+2+Len,&i,isReal,All);
				Len+=i;
				DataNUM+=x;
			}
		break;
	}
	if(0==DataNUM)
	{
		ms->ms_free(pstack);
		pstack=NULL;
	}else
	{
		p16=(u16 *)pstack;
		p16[0]=DataNUM;	
	}
	return pstack;
}

u32 FreData_PackJson(u32 ROAD,u8 *pInbuf,u8 *pout,u32 Min)//冻结数据打包用ROAD区分 分钟冻结和日月冻结
{
	u32 ret=0;
	switch(ROAD)
	{
		case 0x50020200:
			ret=Min_PackJson(pInbuf,pout,Min);
			break;
		case 0x50040200:
		case 0x50060200:
			ret=Day_Month_PackJson(pInbuf,pout);
			break;
		default:
			break;
	}
	return ret;
}

void GetMetAddr(void)//取得所有表的地址
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	u32 i;
	u32 AddrNUM;
//1.取得表档案的所有有效地址,过滤交采地址
	AddrNUM=0;
	p16=(u16*)(ADDR_AddrList+2);
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			p16+=3;
			AddrNUM+=1;
		}
		p8+=LENper_6000;
	}
	p16=(u16*)(ADDR_AddrList);
	p16[0]=AddrNUM;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:取得档案中所有的表地址 数量:%d\n",(u32)&__func__, __LINE__, AddrNUM);
	#endif
}

u8 *CreateRealJsonFile(void)//创建一个新的实时json项目文件
{	
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//创建文件夹
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//得到698当前时间转换成asc码		
	Get698Time_Ascii(JSON->Lastrealtime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");	
	i=mystrlen((const char *)p8);
	if(sizeof(JSON->RealFileName)>=i+1)
	{
		mystrncpy((char *)JSON->RealFileName,(const char *)p8,i);
	}else
	{
		#ifdef DEBUG_JSON
		myprintf("\n[%s:%d]:JSON->RealFileName buf空间大小不够 所需i+1=%d\n", (u32)&__func__, __LINE__, i+1);
		#endif
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	myprintf("\n[%s:%d]:字符:%s \n", (u32)&__func__, __LINE__, (u32 )JSON->RealFileName);
	#endif
	return p8;
}

u8 *CreateFreJsonFile(void)//创建一个新的冻结json项目文件
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//创建文件夹
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//得到698当前时间转换成asc码	
	mystrcpy((char *)p8+Len+8, ".json");	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	return p8;
}

u8 *CreateFreJsonFile_RAM(void)//在RAM区,创建一个新的冻结项目文件
{
	u8 *paddr;
	u8 *p8;
	u32 i;
	u32 Len;
	//创建文件夹
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//得到698当前时间转换成asc码	
	mystrcpy((char *)p8+Len+8, ".json");	
	
	return p8;
}

void Compress_JSONFile(u8 *fname,u32 fnamesiz,u32 isReal)//将JSON文件打包成tar.gz格式,isReal,1实时文件,0冻结文件
{
	u8 *p8;
	u32 i;
	u32 SLen;
	ms_Type *ms;
	ms=Get_ms();
	p8=(u8 *)ADDR_DATABUFF+1024;
	SLen=0;
	//把文件夹里面的文件移动到当前目录下来
	mystrcpy((char *)p8+SLen,"mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen,(const char *)fname);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	mystrcpy((char *)p8+SLen," .");
	ms->link_system((char *)p8);
	//打包这个文件
	SLen=0;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, "tar jcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONREALPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}else
	{
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		for(i=0;i<SLen;i++)
		{
			if('.'==p8[i])
			{
				break;
			}
		}
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		i=mystrlen(JSONFREPATH);
		mystrcpy((char *)p8+SLen, (const char *)fname+i);
	}	
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:解压文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//把.JSON文件删除掉,然后再把压缩包移动到对应的位置
	SLen=0;
	mystrcpy((char *)p8+SLen, "mv ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		i=mystrlen(JSONREALPATH);
	}else
	{
		i=mystrlen(JSONFREPATH);
	}
	mystrcpy((char *)p8+SLen, (const char *)fname+i);
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	for(i=0;i<SLen;i++)
	{
		if('.'==p8[i])
		{
			break;
		}
	}
	if(isReal)
	{
		mystrcpy((char *)p8+i, ".tar.bz2 ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONREALPATH);
	}else
	{
		mystrcpy((char *)p8+i, ".tar.gz ");
		i=mystrlen((const char *)p8);
		SLen=i;
		mystrcpy((char *)p8+SLen, JSONFREPATH);
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:移动文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	//删除当前的.json文件
	SLen=0;
	mystrcpy((char *)p8+SLen, "rm ");
	i=mystrlen((const char *)p8+SLen);
	SLen+=i;
	if(isReal)
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_REAL);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}else
	{
		mystrcpy((char *)p8+SLen, JSONFILENAMEHEAD_FRE);
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		p8[SLen++]='*';
		p8[SLen++]=0;
	}
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除当前JSON文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
	if(isReal)
	{
		Delet_JsonFile(JSON->RealFileName);
	}else
	{
		p8=(u8 *)ADDR_DATABUFF+1024;
		mystrcpy((char *)p8, (const char *)fname);
		Delet_JsonFile(p8);
	}
}

u32 HeadStart_JSON(u8 *pstc)//创建JSON格式: [
{
	u32 Offset;
	u32 i;
	Offset=0;
	i=Str_L_SQB(pstc+Offset);//左方括号
	Offset+=i;
	return Offset;
}	

u32 HeadEnd_JSON(u8 *pstc,u32 Len)//创建JSON格式: ]
{
	u32 flag=0;
	u32 Offset;
	u32 i;
	Offset=0;
	if(Len>0)
	{
		if(','==pstc[Len-1])
		{
			Len--;
			flag=1;
		}
	}
	i=Str_CRandLF(pstc+Len+Offset);//回车和换行
	Offset+=i;
	i=Str_R_SQB(pstc+Len+Offset);//右方括号
	Offset+=i;
	if(1==flag)
	{
		Offset--;
	}
	return Offset;
}

u32 FreDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal)
{
	u32 i;
	u8 *pDATA;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,0);//根据ROAD来取Flash中的数据,pAddr表地址,Min取分钟冻结的时候用,1表示1分钟15表示15分钟,返回0无数据,否则返回一个数据buf指针
	if(NULL==pDATA)
	{	
		return 0;	
	}
	i=FreData_PackJson(ROAD,pDATA,pd,Min);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 AllDataPack(u16 *pAddr,u8 *pd)
{
	u32 i;
	u32 CJSONLEN;
	CJSONLEN=0;
	//获取冻结数据并打包成JSON格式,返回0没有找到数据
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50040200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50060200,0,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,15,0);
	CJSONLEN+=i;
	i=FreDataPack(pAddr,pd+CJSONLEN,0x50020200,1,0);
	CJSONLEN+=i;	
	return CJSONLEN;
}

void Delet_FrzJsonFile(void)//删除Flash空间里面,所有的冻结文件
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

void Crl_AllJsonFileAndTar(void)//清空所有有关的JSON文件内容
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILEPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.bz2");
	i=mystrlen((const char *)p8+Len);
	ms->link_system((char *)p8);
}


void Delet_Yesterday_Data_Flash(void)
{
	//删除昨天的日冻结
	Delet_All_FromFlash(0x13f16000);
	Delet_All_FromFlash(0x13f11000);
	//删除昨天的月冻结
	Delet_All_FromFlash(0x13f18000);
	Delet_All_FromFlash(0x13f13000);
	//删除昨天的分钟冻结
	Delet_All_FromFlash(0x13f19000);
	Delet_All_FromFlash(0x13f22000);	
}

u32 Get_OneRealDataLen_FromFlash(u8 *pIn)//计算一个实时数据包数据的长度,pin导入的是完整的包包含21个时标字节
{
	u8 *p8;
	u32 i;
	u32 x;
	u32 OADNUM;
	u32 Len;
	Len=0;
	Len+=21;//跳过21个时间字节
	OADNUM=pIn[Len+1];//OAD数量
	Len+=2;
	Len+=OADNUM*6;
	p8=pIn+Len;//指向数据部分
	for(x=0;x<OADNUM;x++)
	{
		i=Get_DL698DataLen_S(p8,0);
		p8+=i;
		Len+=i;
	}
	return Len;
}

u32 RealData_PackJson(u8 *pInbuf,u8 *pout,u8 *pAddr,u32 All)//处理实时数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;	
	u32 CJSONLEN=0;
	u32 CJSONL;
	u8 *p8;
	u8 *pdata;//指向数据区的指针
	u8 val;

	u32 Offset=0;
	u32 TotalNUM;
	u32 OAD;
	u32 ROAD;		//记录报文中ROAD
	u8 OADNUM;		//记录报文中ROAD数量
	u8 OADSPOT; 	//记录报文中ROAD数量的位置//20210200
	u32 DataLen;	//数据大小

	u16 *p16;
	p16=(u16 *)pInbuf;
	TotalNUM=p16[0];

	pInbuf+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MC(0,(u32)ADDR_DATABUFF+1024,1024);
	DataLen=0;
	val=0;
	WWDT_Disable();
	if(TotalNUM==0)
	{
		if(All!=0)
		{
			TotalNUM=1;
		}
	}
	for(z=0;z<TotalNUM;z++)
	{
		Offset=21;
		Offset+=4;
		ROAD=(pInbuf[Offset]<<24)|(pInbuf[Offset+1]<<16)|(pInbuf[Offset+2]<<8)|pInbuf[Offset+3];
		if(ROAD!=0x50020200)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:不是分钟冻结数据:%08x\n", (u32)&__func__, __LINE__, ROAD);
			#endif
			goto Real_JSON_ERROR;
		}
		Offset+=4;
		OADNUM=pInbuf[Offset];
		Offset++;
		OADSPOT=Offset;
		Offset+=(OADNUM*4); 
		//=====================
		CJSONL=CJSONLEN;
		val=0;
		i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		Add_StrToASCII(p8,pAddr,6);//pbuf里面的字节输出到pstr并转成asc码
		i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
		CJSONLEN+=i;
		
		//数据时间
		Get698Time_Ascii(pInbuf+14,p8);//p时间前两个字节表示年份p8输出asc码
		GetJsonTime(p8);
		i=CreateData_Json(pout+CJSONLEN, p8);
		CJSONLEN+=i;
		for(y=1;y<RealDataOAD_JSONSIZE;y++)//y从1开始算 因为第一个是时间在前面已经写好了
		{
			for(x=0;x<OADNUM;x++)
			{
				OAD=(pInbuf[OADSPOT+x*4]<<24)|(pInbuf[OADSPOT+x*4+1]<<16)|(pInbuf[OADSPOT+x*4+2]<<8)|pInbuf[OADSPOT+x*4+3];
				
				if((RealDataOAD_JSON[y])==(OAD))
				{
					break;
				}else
				{
					if((OAD&0xff)==0)//如果OAD最后一个字节是0有可能就是电压 电量这种合项的
					{
						if((RealDataOAD_JSON[y]>>8)==(OAD>>8))
						{
							break;
						}	
					}
				}
			}
			if(x<OADNUM)//找到了有效OAD
			{	
				pdata=Find_DataHead(pInbuf,pInbuf+OADSPOT,x);//根据Flash里面的存储格式用pIn里面OAD的位置去找pIn里面数据的位置,pOAD第一字节是OAD数量,oadcnt是pOAD里面的偏移,返回Pin里面数据的正确位置
				if((RealDataOAD_JSON[y]!=OAD)&&((OAD&0xff)==0))
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:是交采数据\n",(u32)&__func__,__LINE__,0);
					myprintf("[%s:%d]:是交采数据OAD=%08x\n",(u32)&__func__,__LINE__,OAD);
					
					for(i=0;i<50;i++)
							myprintf("%02x ",pdata[i],0,0);
						myprintf("\n",0,0,0);
					#endif
					i=RealDataOAD_JSON[y]&0xff;
					pdata=Get_Element(pdata,i,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
				}
				if(!pdata)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:没有找到有效的数据\n",(u32)&__func__,__LINE__,0);
					#endif
					continue;
				}
				i=DealMetData_Ascii(OAD>>16,pdata,p8,1);//处理抄读回电表的数据,返回存入pout的字节数量,pout已经处理好的ASCii			
				if(0==i)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:采集到的数据存储格式有错误\n",(u32)&__func__,__LINE__,0);
					#endif
					goto Real_JSON_ERROR;
				}
				if(All)
				{//全网
					val=1;
				}else
				{//增量  过滤掉0
					if(0==p8[0])
					{
						continue;
					}else
					{
						val=1;
					}
				}
				i=CreateData_Json(pout+CJSONLEN, p8);
				CJSONLEN+=i;
			}else
			{
				i=Str_COMMA(pout+CJSONLEN);//逗号
				CJSONLEN+=i;
			}
		}
		i=CreateDataEnd_Json(pout,CJSONLEN);
		CJSONLEN+=i;
		i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
		CJSONLEN+=i;
		i=Str_LF(pout+CJSONLEN);
		CJSONLEN+=i;
		DataLen=Get_OneFreDataLen_FromFlash(pInbuf);//计算一个分时段数据包数据的长度,pin导入的是完整的包包含21个时标字节
		if(z<TotalNUM)
		{	
			pInbuf+=DataLen;
		}
		if(!All)
		{
			if(0==val)
			{
				CJSONLEN=CJSONL;
			}
		}
	}
	if(!All)
	{
		if(0==val)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:数据为空!!!!!!\n",(u32)&__func__,__LINE__,0);
			#endif
			return 0;
		}
	}
	return CJSONLEN;
Real_JSON_ERROR:
	return 0;
}

u32 RealDataPackNull(u8 *pout,u8 *pAddr)//全网实时创建空数据列表
{
	u8 *p8;
	u32 i;
	u32 y;
	u32 CJSONLEN;
	CJSONLEN=0;
	i=CreateArrayHead_Json(pout+CJSONLEN, DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	p8=(u8 *)ADDR_DATABUFF;
	Add_StrToASCII(p8,pAddr,6);//pbuf里面的字节输出到pstr并转成asc码
	i=CreateDataHead_Json(pout+CJSONLEN, p8, DEBUG_THIRD_SPACENUM);
	CJSONLEN+=i;
	
	//数据时间
	//Get698CurTime_Ascii(p8);
	Get698Time_Ascii(JSON->Lastrealtime,p8);//p时间前两个字节表示年份p8输出asc码
	GetJsonTime(p8);
	i=CreateData_Json(pout+CJSONLEN, p8);
	CJSONLEN+=i;	
	for(y=1;y<RealDataOAD_JSONSIZE;y++)//y从1开始算 因为第一个是时间在前面已经写好了
	{
		i=Str_COMMA(pout+CJSONLEN);//逗号
		CJSONLEN+=i;
	}
	i=CreateDataEnd_Json(pout,CJSONLEN);
	CJSONLEN+=i;
	i=CreateArrayEnd_Json(pout, CJSONLEN,DEBUG_SECOND_SPACENUM);
	CJSONLEN+=i;
	i=Str_LF(pout+CJSONLEN);
	CJSONLEN+=i;
	return CJSONLEN;
}

u32 RealDataPack(u16 *pAddr,u8 *pd,u32 ROAD,u32 Min,u32 isReal,u32 All)
{
	u32 i;
	u8 *pDATA;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pDATA=Get_MetData_ROAD(pAddr,ROAD,Min,isReal,All);//根据ROAD来取Flash中的数据,pAddr表地址,Min取分钟冻结的时候用,1表示1分钟15表示15分钟,返回0无数据,否则返回一个数据buf指针
	if(NULL==pDATA)
	{	
		if(All)
		{
			p8=(u8 *)pAddr;
			i=RealDataPackNull(pd,p8);
			ms->ms_free(pDATA);
			pDATA=NULL;	
			return i;
		}
		return 0;	
	}
	p8=(u8 *)pAddr;
	i=RealData_PackJson(pDATA,pd,p8,All);
	ms->ms_free(pDATA);
	pDATA=NULL;	
	return i;
}

u32 DeletFlashData_SelectTime(u32 ROAD,u8 *ptime)//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
{
	u32 i;
	u32 x;
	u32 ret=0;
	switch (ROAD)
	{
		case 0x50020200://分钟冻结
			//比较时标是上一天的
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}		
			break;
		case 0x50040200://日冻结
		//打包的时候是第二天
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|(JSON->Lastrealtime[3]);
		//减一天是昨天的日冻结
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
			x=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|(ptime[3]);
			if(i>=x)
			{
				ret=1;
			}
			break;
		case 0x50060200://月冻结(保存一个月的时间)
			//比较时标是上一天的
			i=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8)|JSON->Lastrealtime[2];
			i=YYM_SubM_hex(i, 2);//年年月日减1日,返回减1后的年年月日
			x=(ptime[0]<<8)|(ptime[1]<<16)|ptime[2];
			if(i>=x)
			{
				ret=1;
			}			
			break;
		case 0x5002020E:
		case 0x50020201:
			i=Compare_DL698DataString(JSON->Lastrealtime,ptime,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==1)
			{
				ret=1;
			}		
			break;	
		default:
		break;
	}
	return ret;
}

void Delet_All_FromFlash(u32 FileID)
{
	u32 i;
	u32	filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
	Comm_Ram->msFILESearchCount=0;
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
	{	
		filename=Get_RecordFileName_JS(FileID,0xffffffff,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		if(filename==0xffffffff)
		{
			continue;
		}
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			continue;
		}
		if(file->BLOCK_ENTRY==0)
		{
			continue;
		}
		switch (FileID)
		{
			case 0x13f11000://日冻结
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f13000://月冻结
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f16000://日冻结
				//i=Filter_Time(file->save_date_time_s,0x50040200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50040200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f18000://月冻结
				//i=Filter_Time(file->save_date_time_s,0x50060200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50060200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f19000://15分钟冻结
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f22000://1分钟冻结
				//i=Filter_Time(file->save_date_time_s,0x50020200,0,0);//取文件的存储时间进行过滤,成功返回1,失败0
				i=DeletFlashData_SelectTime(0x50020200,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f12000://15分钟冻结
				//i=1;//默认全部删除
				i=DeletFlashData_SelectTime(0x5002020E,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			case 0x13f17000://1分钟冻结
				//i=1;//默认全部删除
				i=DeletFlashData_SelectTime(0x50020201,file->save_date_time_s);//在每一次打包之后处理删除的数据项(从NANDFLASH),成功就返回1,失败返回0
				break;
			default:
				return;
		}
		if(0==i)
		{
			continue;
		}
		file->BLOCK_ENTRY=0;
		ms->msfile_Delete(filename);
	}
}

void Delet_FileName(void)
{
	Delet_All_FromFlash(0x13f12000);//删除这个时间点的文件
	Delet_All_FromFlash(0x13f17000);//删除这个时间点的文件
}

void Delet_JsonFile(u8 *fanme)//删除当前的Realtime.json文件,保留压缩包
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)fanme);
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除tmp里面JSON文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_JsonFile_PATH(const char *path)//删除路径path下的json文件
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除tmp里面JSON文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}
void Delet_AllData_PATH(const char *path)//删除路径path下的所有文件
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)path);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除tmp里面JSON文件字符:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);
}

void Delet_CurPath_JSON_And_TarGz(void)//删除当前目录下的json文件和tar.gz压缩文件
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".json");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除命令:%s \n", (u32)&__func__, __LINE__, (u32 )p8);	
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	mystrcpy((char *)p8+Len, ".tar.gz");
	#ifdef DEBUG_JSON
	myprintf("\n[%s:%d]:删除命令:%s \n", (u32)&__func__, __LINE__, (u32 )p8);
	#endif
	ms->link_system((char *)p8);
}


void Delet_RealTime_BZ2(void)//删除实时的压缩文件,(上报之后要删除)
{
	u32 i;
	u32 Len;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	p8=(u8 *)ADDR_DATABUFF+1024;
	Len=0;	
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, (const char *)pfilename);
	ms->link_system((char *)p8);
}

void Mkdir_JsonFile(const char *path)//创建path路径文件夹
{
	u32 i;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	mystrcpy((char *)p8, "mkdir ");
	i=mystrlen((const char *)p8);
	mystrcpy((char *)p8+i, (const char *)path);
	ms->link_system((char *)p8);
}

u32 Is_MinData_Flash(u32 FileID)//在Flash空间里面这个数字是不是分钟数据,0不删除 1删除
{
	u32 ret=0;
	//根据FileID来删除不同的文件
	switch(FileID)
	{
		//case 0x13f12000:
		//case 0x13f17000:
		case 0x13f16000://日冻结
		case 0x13f11000:
		case 0x13f18000://月冻结
		case 0x13f13000:
		case 0x13f19000://15分钟冻结
		//case 0x13f22000://1分钟冻结
		case 3://搜表结果不删除
		case 4:
			break;
		default:
			ret=1;
		break;
	}
	return ret;
}

void Delet_MinData_Flash(u32 Delcnt)//在Flash里面删除Delcnt点的数据量
{
	u32 i;
	u32 FileCount;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=Get_ms();
	FileCount=0;	
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(Delcnt<=FileCount)//如果删除的次数已经到了,那就退出
		{
			break;
		}
		if(file->BLOCK_ENTRY)
		{
			//i=file->FILECOUNT;
			FileID=file->FILEID;
			i=Is_MinData_Flash(FileID);//在Flash空间里面这个数字是不是分钟数据,0不删除 1删除
			if(i)
			{
				ms->msfile_Delete(filename);
				FileCount++;
			}
		}
		file++;
	}
}

void Delet_AllFile(u8 *ptime)//删除临时文件下面的所有文件 不包括文件夹
{
	u32  i;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	//u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return;
	}
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "mkdir ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//得到698当前时间转换成asc码		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+8, "* ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//文件的名字
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
	Len=0;
	mystrcpy((char *)p8+Len, "mv ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='/';
	p8[Len++]='*';
	p8[Len++]=' ';
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//文件的名字
	mystrcpy((char *)p8+Len, JSONTEMPFILENAEM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]=' ';
	p8[Len++]='-';
	p8[Len++]='r';
	p8[Len++]='f';
	p8[Len++]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);

	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	ms->link_system((char *)p8);
	
}

u32 Check_TarFtzFile(u8 *ptime)
{
	u32 i;
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	i=Check_pYYMDHMS_hex(ptime);
	if(i)
	{
		return 0;
	}
	if(ptime[4]==0&&ptime[5]==0)
	{
		JSON->IsCreateFrzJson=0;
	}
	i=ptime[4]*60+ptime[5];
	if(i<70)//这个时间段内不会有这个压缩文件生成
	{
		return 0;
	}
	if(JSON->IsCreateFrzJson!=0)
	{
		return 0;
	}
	Mkdir_JsonFile(JSONFREPATH);
	pname=GetFrzFileJsonName_Compress();//得到压缩包的名字带路径
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);//测试文件存不存在
	if(fd!=-1)//说明文件不存在
	{
		//那就说明今天没有打包,或者说是打包已经失败了,需要重新再打包
		//1.删除当前目录下面所有的.json和.tar
		Delet_CurPath_JSON_And_TarGz();
		Delet_AllFile(ptime);//删除上一天的文件夹内容
		ms->link_close(fd);
		return 1;
	}else
	{
		//文件存在今天就不进行全量文件打包操作
		JSON->IsCreateFrzJson=1;
	}
	return 0;
}

u32 IsTimeTask(u8 *pt)//用时间判断是不是打包任务时间到了,pt上一次打包的时间
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 ret=0;
	YYMDHM1=0;
	YYMDHM1=((u64)pt[0]<<32)|((u64)pt[1]<<40)|((u64)pt[2]<<24)|((u64)pt[3]<<16)|((u64)pt[4]<<8)|((u64)pt[5]);
	//存在Flash文件里面的时标
	YYMDHM2=0;
	YYMDHM2=((u64)Comm_Ram->DL698YMDHMS[0]<<32)|((u64)Comm_Ram->DL698YMDHMS[1]<<40)|((u64)Comm_Ram->DL698YMDHMS[2]<<24)|((u64)Comm_Ram->DL698YMDHMS[3]<<16)|((u64)Comm_Ram->DL698YMDHMS[4]<<8)|((u64)Comm_Ram->DL698YMDHMS[5]);
	if(JSON->reportfiletime[4]==0&&0==JSON->reportfiletime[5])
	{
		Delet_AllFile(JSON->reportfiletime);
	}
	if(YYMDHM1<YYMDHM2)
	{
		if(0==pt[5])
		{
			return 3;
		}
		return 1;
	}
	if(YYMDHM1>YYMDHM2)
	{
		return 2;
	}
	return ret;
}

u32 Can_GetVaildMetAddr(void)
{
	u8 *p8;
	u16 *p16s;
	u32 i;
	for(i=0;i<NUMmax_6000;i++)
	{
		p8=(u8*)ADDR_6000_SDRAM+i*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
			if(p16s[0]==0)
			{
				if(p16s[1]==0)
				{
					if(p16s[2]==0)
					{
						continue;
					}
				}
			}
			break;
		}
	}
	if(i>=NUMmax_6000)
	{
		return 0;
	}
	return 1;
}

void Terminal_FrzPack(void)
{
	u32 i;
	u32 CJSONLEN;//JSON字符串计数器,初始化0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//地址u8类型指针
	u8 *pname;
	u16 *p16;
	s32 fd;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch (JSON->PackFrzJsonTask)
	{
		case 0://初始化数据
			JSON->PackFrzJsonTask++;
			break;
		case 1://时间的判断	
			//检查是不是因为停上电引起的打包失败或者压缩失败
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=i=Check_TarFtzFile(JSON->Lastrealtime);//只有过了一分钟才会执行相应的任务
			if(i!=0)//日冻结打包任务
			{
				#ifdef DEBUG_JSON
				myprintf("==============================================================================\n", 0, 0, 0);
				myprintf("==========================准备开始创建冻结JSON格式文件========================\n", 0, 0, 0);
				myprintf("==============================================================================\n", 0, 0, 0);
				#endif
				JSON->PackFrzJsonTask++;
			}
			break;
		case 2://打包之前要做的工作
			//TEST_PACK();
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:打包前时间\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackFrzJsonTask=0;
				return;
			}
			JSON->CurFrzMetCount=0;
			//打包之前清空RAM下面所有的空间
			Delet_RAM_FrzFile();
			JSON->PackFrzJsonTask++;
			break;
		case 3://数据正在处理中
			//#ifndef DEBUG_JSON
			i=GetOneMetAddr((u16 *)&JSON->CurFrzMetCount);
			//#else
			//i=GetOneMetAddr_TEST(&JSON->CurFrzMetCount);
			//#endif
			if(i==0)//全部表地址已经打包结束
			{
				JSON->PackFrzJsonTask++;;//重置每天打包日冻结的标志
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:数据处理完毕开始压缩\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurFrzMetCount++;//下一次循环就找下一只表
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:JSON->CurFrzMetCount=%d\n",(u32)&__func__,(u32)__LINE__,JSON->CurFrzMetCount);
			#endif
			//得到文件名字
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=RAMTOFLASH_COPYMAXSIZE))
				{
					//从RAM空间移动到Flash空间
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:申请空间失败i\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//前面一个是表的数量,后面一个是表的地址
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbuf里面的字节输出到pstr并转成asc码
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//说明是有数据的
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//删除逗号
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//这一只表示有数据的
			{
				//下面开始往RAM空间里面写数据
				i=CalCulate_Space(JSONFREPATH_RAM);//计算这个空间里面剩余的容量
				if(i>=CJSONLEN)//如果剩余的空间够用
				{
					//得到文件名字
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
							#endif
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//关闭文件句柄
			}
			ms->ms_free(p8);
			break;
		case 4://数据处理结束,准备拷贝文件压缩
			//得到文件名字
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:打包处理完开始压缩\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//从RAM空间移动到Flash空间
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				WWDT_Disable();
				Compress_JSONFile(pname,1024,0);//将JSON文件打包成tar.gz格式
				//Delet_Yesterday_Data_Flash();//删除flash里面昨天所有的数据
				Delet_JsonFile_PATH(JSONFREPATH_RAM);//删除JSON文件
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				//说明打包失败了
				if(JSON->IsCreateFrzJson>=1)//今天的数据打包失败了
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
					Delet_JsonFile_PATH(JSONFREPATH_RAM);//删除JSON文件
				}
				else
				{
					JSON->IsCreateFrzJson++;//再给一次机会打包
				}
			}
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:打包后时间\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",Comm_Ram->DL698YMDHMS[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			break;
		default:
			JSON->PackFrzJsonTask=0;
		break;
	}
}

u32 GetOneMetAddr(u16 *MetCount)//找到一个表地址
{
	u8 *p8;
	u16 *p16;
	u16 *p16s;
	#ifdef DEBUG_JSON
	u32 i;
	#endif
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	for(;Met<NUMmax_6000;Met++)
	{
		p8=(u8*)ADDR_6000_SDRAM+(Met)*LENper_6000;
		if(p8[0]==DataType_structure)
		{
			p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
			p16[0]=p16s[0];
			p16[1]=p16s[1];
			p16[2]=p16s[2];
			break;
		}
	}
	if(Met>=NUMmax_6000)
	{
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;

	p8=(u8 *)(ADDR_AddrList+2);
	#ifdef DEBUG_JSON
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

void Terminal_RealPack(void)
{
	s8 *pc="dateTime(鏁版嵁鏃堕棿),20000201(A鐩哥數鍘�),20000202(B鐩哥數鍘�),20000203(C鐩哥數鍘�),20010201(A鐩哥數娴�),20010202(B鐩哥數娴�),20010203(C鐩哥數娴�),20010400(闆剁嚎鐢垫祦),20010500(鍓╀綑鐢垫祦),20040201(鎬绘湁鍔熷姛鐜�),20040202(A鐩告湁鍔熷姛鐜�),20040203(B鐩告湁鍔熷姛鐜�),20040204(C鐩告湁鍔熷姛鐜�),43140506(瀹ゅ娓╁害),43140505(瀹ゅ唴娓╁害),43140501(绌鸿皟璁剧疆娓╁害),43140400(绌鸿皟寮�鍏虫満鐘舵��),43110502(鐑按鍣ㄥ姞鐑俯搴�),43110400(鐑按鍣ㄥ紑鍏虫満鐘舵��),00100201(姝ｅ悜鏈夊姛鎬荤數鑳�),00100203(姝ｅ悜鏈夊姛宄扮數鑳�),00100205(姝ｅ悜鏈夊姛璋风數鑳�),00200201(鍙嶅悜鏈夊姛鎬荤數鑳�),00200203(鍙嶅悜鏈夊姛宄扮數鑳�),00200205(鍙嶅悜鏈夊姛璋风數鑳�)";
	s8 *pc1="]";
	u32 i;
	//u32 x;
	u32 y;
	u32 MAXMETNUM=1;
	u32 CJSONLEN;
	u8 *p8;                               
	u8 *pname;
	u16 *p16;
	//u16 AddrNUM;
	s32 fd;
	ms_Type *ms;	
	ms=(ms_Type *)Get_ms();
	switch (JSON->PackRealJsonTask)
	{
		case 0://初始化数据
			Delet_FileName();//删除这个时间点的文件
			pYYMDHMS_AddS_hex(JSON->Lastrealtime,60);//每执行一次打包任务之后 分钟打包的时间标志都加1分钟
			JSON->PackRealJsonTask++;
			break;
		case 1://时间的判断	
			//检查是不是因为停上电引起的打包失败或者压缩失败
			i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
			if(i)
			{
				MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			}			
			i=IsTimeTask(JSON->Lastrealtime);//只有过了一分钟才会执行相应的任务
			switch (i)
			{
				case 1:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=0;
					break;
				case 2:
					MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
					break;
				case 3:
					JSON->PackRealJsonTask++;
					JSON->Is_AllRealData=1;
					break;
				default:
					break;
			}	
			break;
		case 2://实时文件先写一个头
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:实时时间标志\n",(u32)&__func__,(u32)__LINE__,0);
			for(i=0;i<7;i++)
				myprintf("%02x ",JSON->Lastrealtime[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			i=Can_GetVaildMetAddr();
			if(i==0)
			{
				JSON->PackRealJsonTask=0;//重新开始走流程
				return;
			}
			JSON->CurRealMetCount=0;
			//先处理实时文件的头
			p8=(u8 *)ms->ms_calloc(4*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc空间不足\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				return ;
			}
			CJSONLEN=0;
			i=HeadStart_JSON(p8+CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=CreateDataHead_Json(p8+CJSONLEN, (u8 *)STR_COLOUMNHEAD, DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateData_Json(p8+CJSONLEN, (u8 *)pc);
			CJSONLEN+=i;
			i=CreateDataEnd_Json(p8,CJSONLEN);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN,DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;
			Mkdir_JsonFile(JSONREALPATH);//创建这个文件夹
			pname=CreateRealJsonFile();//创建一个新的项目文件,返回文件句柄()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1!=fd)//已经存在该文件就删除
			{
				ms->link_remove((char *)pname);
				#ifdef DEBUG_JSON
				myprintf("\n\n[%s:%d]:删除文件:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
				#endif
			}
			ms->link_close(fd);
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				ms->ms_free(p8);
				JSON->PackRealJsonTask=0;//重新开始走流程
				return;
			}
			y=ms->link_write(fd,(char *)p8,CJSONLEN);
			if(y==CJSONLEN)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:写入文件成功  字符数量CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
				#endif
				JSON->PackRealJsonTask++;
			}else
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:写入文件失败  字符数量y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
			}
			ms->link_close(fd);
			ms->ms_free(p8);
			break;
		case 3://数据处理过程
			while(MAXMETNUM>0)
			{
			MAXMETNUM--;	
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//全部表地址已经打包结束
			{
				//不是打包全量文件,那就是没有表有数据
				Delet_JsonFile_PATH(JSONREALPATH);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:没有一只表有数据\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//重新开始走流程
				break;
			}
			JSON->CurRealMetCount++;//下一次循环就找下一只表		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc空间不足\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//重新开始走流程
				return;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->Is_AllRealData);
			CJSONLEN+=i;

			if((CJSONLEN>0))
			{
				Mkdir_JsonFile(JSONREALPATH);//创建这个文件夹
				pname=CreateRealJsonFile();//创建一个新的项目文件,返回文件句柄()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//不存在这个文件
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:写入文件成功  字符数量CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
					JSON->PackRealJsonTask=0xff;
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:写入文件失败  字符数量y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}else
			{
				//myprintf("[%s:%d]:没有数据\n",(u32)&__func__,(u32)__LINE__,0);
			}
			ms->ms_free(p8);
			if(JSON->PackRealJsonTask==0xff)
			{
				break;
			}
			}
			break;
		case 4://开始压缩流程
			Mkdir_JsonFile(JSONREALPATH);//创建这个文件夹
			pname=CreateRealJsonFile();//创建一个新的项目文件,返回文件句柄()
			fd=ms->link_open((char *)pname,O_RDWR);
			if(-1==fd)//不存在这个文件
			{
				JSON->PackRealJsonTask=2;
				return;
			}
			i=ms->link_lseek(fd,0,SEEK_END);
			if(i>1)
			{
				ms->link_lseek(fd,i-2,SEEK_SET);
			}
			y=ms->link_write(fd,pc1,1);
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
			#endif
			if(y==1)
			{
				Compress_JSONFile(pname,1024,1);//将JSON文件打包成tar.gz格式
				//删除Flash里面的filename
				SetReportInfo();//设置上报信息
			}else
			{
				y=ms->link_write(fd,(char *)p8,1);
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:y=%d\n",(u32)&__func__,(u32)__LINE__,y);
				#endif
				if(y==1)
				{
					Compress_JSONFile(pname,1024,1);//将JSON文件打包成tar.gz格式
					//删除Flash里面的filename
					SetReportInfo();//设置上报信息
				}
			}
			JSON->PackRealJsonTask=0;//重新开始走流程
			ms->link_close(fd);
			break;
		case 0xff://数据处理过程
			while (MAXMETNUM>0)
			{
			MAXMETNUM--;
			i=GetOneMetAddr((u16 *)&JSON->CurRealMetCount);
			//i=GetOneMetAddr_TEST(JSON->CurRealMetCount);
			if(i==0)//全部表地址已经打包结束
			{
				JSON->PackRealJsonTask=4;;//开始压缩
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:数据处理完毕开始压缩\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				break;
			}
			JSON->CurRealMetCount++;//下一次循环就找下一只表		
			p16=(u16*)(ADDR_AddrList);
			p16++;	
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:malloc空间不足\n",(u32)&__func__,(u32)__LINE__,0);
				#endif
				JSON->PackRealJsonTask=0;//重新开始走流程
				return ;
			}
			CJSONLEN=0;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 15, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;
			i=RealDataPack(p16, p8+CJSONLEN, 0, 1, 1,JSON->IsCreateFrzJson);
			CJSONLEN+=i;

			if(CJSONLEN>0)
			{
				Mkdir_JsonFile(JSONREALPATH);//创建这个文件夹
				pname=CreateRealJsonFile();//创建一个新的项目文件,返回文件句柄()
				fd=ms->link_open((char *)pname,O_RDWR);
				if(-1==fd)//不存在这个文件
				{
					ms->ms_free(p8);
					JSON->PackRealJsonTask=2;
					return;
				}
				ms->link_lseek(fd,0,SEEK_END);
				y=ms->link_write(fd,(char *)p8,CJSONLEN);
				if(y==CJSONLEN)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:写入文件成功  字符数量CJSONLEN=%d\n",(u32)&__func__,(u32)__LINE__,CJSONLEN);
					#endif
				}else
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:写入文件失败  字符数量y=%d\n",(u32)&__func__,(u32)__LINE__,y);
					#endif
				}
				ms->link_close(fd);
			}	
			ms->ms_free(p8);
			}
			break;
		default:
			JSON->PackRealJsonTask=0;
		break;
	}		
}

void Terminal_PackJSON(void)//终端打包JSON文件任务
{
	u32 i;
	if(FileDownload->New==0)//远程升级完成,不进来打包
	{
		return;
	}
	switch (JSON->PackJsonTask)
	{
		case 0://初始化数据
			JSON->IsCreateFrzJson=0;//刚上电判断这个值初始化为0
			i=Check_pYYMDHMS_hex(Comm_Ram->DL698YMDHMS);
			if(i)
			{
				return;
			}	
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->Lastrealtime, sizeof(JSON->Lastrealtime));
			MW((u32)Comm_Ram->DL698YMDHMS, (u32)JSON->reportfiletime, sizeof(JSON->reportfiletime));
			//Mkdir_JsonFile();//创建文件夹
			//TEST_PACK();
			//tar_thread();
			//tar_text();
			//Crl_AllJsonFileAndTar();
			#ifdef DEBUG_JSON
			//log_Flash();
			//test_saveMIN15();
			//test_saveMIN();
			//test_saveDAY();
			#endif
			//TEST_RAM() ;
			//return;
			JSON->PackRealJsonTask=1;//实时从1开始
			JSON->PackJsonTask++;
			break;
		case 1:
			Terminal_FrzPack();//日冻结数据打包
			Terminal_RealPack();//实时数据打包
			break;
		default:
			JSON->PackJsonTask=0;
		break;
	}
	
}
		
//=======================================全网实时数据上报=====================================
void SetReportInfo(void)//上报任务从0开始,可以上报
{
	JSON->g_ReportFlag=1;
}
u8 GetReportInfo(void)//得到是不是需要上报有没有生成新的上报文件
{
	return JSON->g_ReportFlag;	
}
void ClrReportInfo(void)//清掉上报标志,和上报文件名
{
	JSON->Reportjsonfile_Task=0;
	JSON->g_ReportFlag=0;
	MC(0,(u32)JSON->RealFileName,sizeof(JSON->RealFileName));
	pYYMDHMS_AddS_hex(JSON->reportfiletime,60);//每执行一次打包任务之后 分钟打包的时间标志都加1分钟
}
u32 IsReportEnd(void)//是不是上报成功,总上报数=已经上报数
{
	if(JSON->g_Block_Number==JSON->g_Need_report_Block)
	{
		return 1;
	}
	return 0;
}
void Next_DataBlock(void)//指向下一个上报的数据块
{
	JSON->g_Block_Number++;
}

u32 Merge_RealTimeData_JsonFile(u8 *ptime)//合并两个上报压缩文件成为一个新的压缩文件
{
	u64 YYMDHM1;
	u64 YYMDHM2;
	u32 i;
	u32 n;
	u8 isemptyfile;//如果找到的是第一个文件
	u8 *pfilename;
	s32 fd1=-1;
	s32 fd_new=-1;
	u8 *p8;
	//u8 *paddr;
	u32 Len;
	u8 time[7];
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	MW((u32)ptime,(u32)time,sizeof(time));
	i=Check_pYYMDHMS_hex(time);
	if(i)
	{
		return 0;
	}
	pYYMDHMS_SubS_hex(time,60);//每执行一次打包任务之后
	isemptyfile=1;
	pfilename=GetFileJsonName_Compress(time);//得到文件的路径	
	fd_new=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd_new)
	{
		//创建一个新的文件
		p8=GetRealTime_FileJson(time);
		//创建一个新的文件,文件的名字更新到最新的打包时间
		fd_new=ms->link_open((char *)p8,O_CREAT|O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		ms->link_lseek(fd_new,0,SEEK_SET);
		isemptyfile=0;
	}else
	{
		ms->link_close(fd_new);//马上释放掉这个文件句柄
		//开始解压缩
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jxvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:解压文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);

		p8=GetRealTime_FileJson(time);

		fd_new=ms->link_open((char *)p8,O_RDWR);
		if(-1==fd_new)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		isemptyfile=1;
	}
	
	YYMDHM2=0;
	YYMDHM2=((u64)JSON->reportfiletime[0]<<32)|((u64)JSON->reportfiletime[1]<<40)|((u64)JSON->reportfiletime[2]<<24)|((u64)JSON->reportfiletime[3]<<16)|((u64)JSON->reportfiletime[4]<<8)|((u64)JSON->reportfiletime[5]);
	while(1){
		pYYMDHMS_SubS_hex(time,60);//每执行一次打包任务之后 分钟打包的时间标志都加1分钟
		YYMDHM1=0;
		YYMDHM1=((u64)time[0]<<32)|((u64)time[1]<<40)|((u64)time[2]<<24)|((u64)time[3]<<16)|((u64)time[4]<<8)|((u64)time[5]);
		
		if(YYMDHM2>YYMDHM1)
		{
			break;
		}	
		//从最大的那个时间点的文件开始找
		pfilename=GetFileJsonName_Compress(time);//得到文件的路径	
		fd1=ms->link_open((char *)pfilename,O_RDWR);
		if(-1!=fd1)
		{
			ms->link_close(fd1);//马上释放掉这个文件句柄
			//开始解压缩
			p8=(u8 *)ADDR_DATABUFF+1024;
			Len=0;
			mystrcpy((char *)p8+Len, "tar jxvf ");
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			mystrcpy((char *)p8+Len, (const char *)pfilename);
			i=mystrlen((const char *)p8+Len);
			Len+=i;
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:解压文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
			#endif
			ms->link_system((char *)p8);
			
			//打开这个文件
			p8=GetRealTime_FileJson(time);

			fd1=ms->link_open((char *)p8,O_RDWR);
			if(-1!=fd1)
			{
				Len=ms->link_lseek(fd1,0,SEEK_END);
				if(Len)
				{	
					p8=(u8 *)ms->ms_malloc(Len);
					if(p8)
					{	
						if(0==isemptyfile)//找到了第一个文件,//需要保存所有的文件信息
						{
							isemptyfile=1;//接下来找的都不是第一个文件 要做特殊处理
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);
							if(i==Len)
							{
								i=ms->link_write(fd_new,(u8 *)p8,Len);
							}
						}else
						{
							ms->link_lseek(fd1,0,SEEK_SET);
							i=ms->link_read(fd1,(u8 *)p8,Len);	
						
							if(i==Len&&Len>0)
							{
								for(i=0;i<Len-1;i++)
								{
									if(p8[i]==',')
									{
										if(p8[i+1]==0x0A)//换行符号
										{
											break;
										}
									}
								}
								if(i<Len)
								{
									n=ms->link_lseek(fd_new,0,SEEK_END);
									if(n>0)
									{
										ms->link_lseek(fd_new,n-1,SEEK_SET);
										Len-=i;
										ms->link_write(fd_new,(u8 *)p8+i,Len);
									}	
								}
								
							}
						}
						ms->ms_free(p8);
						p8=NULL;
					}
				}
				ms->link_close(fd1);//马上释放掉这个文件句柄
			}
		}
		
	}
	
	Len=ms->link_lseek(fd_new,0,SEEK_END);
	if(Len)
	{	
		MW((u32)ptime,(u32)JSON->reportfiletime,sizeof(JSON->reportfiletime));
		i=Check_pYYMDHMS_hex(JSON->reportfiletime);
		if(i)
		{
			ms->link_close(fd_new);
			return 0;
		}
		pYYMDHMS_SubS_hex(JSON->reportfiletime,60);//每执行一次打包任务之后
		//得到新文件的文件名字
		pfilename=GetRealTime_FileJson_Pack(JSON->reportfiletime);
		
		//压缩新的文件
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "tar jcvf ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " ");
		Len++;

		n=mystrlen((const char *)pfilename);
		for(i=0;i<n;i++)
		{
			if(pfilename[i]=='.')
			{
				break;
			}
		}
		if(i>=n)
		{
			ms->link_close(fd_new);
			return 0;
		}
		
		mystrcpy((char *)pfilename+i, ".json");
		mystrcpy((char *)p8+Len, (const char *)pfilename);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:解压文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
		#endif
		ms->link_system((char *)p8);
		//移动文件到相应的目录下面
		p8=(u8 *)ADDR_DATABUFF+1024;
		Len=0;
		mystrcpy((char *)p8+Len, "mv *.tar.bz2 ");
		i=mystrlen((const char *)p8+Len);
		Len+=i;	
		mystrcpy((char *)p8+Len, JSONREALPATH);
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		mystrcpy((char *)p8+Len, " -f");
		i=mystrlen((const char *)p8+Len);
		Len+=i;
		ms->link_system((char *)p8);
	}
	//删除掉当前Json文件
	
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm *.json");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	ms->link_system((char *)p8);
	
	ms->link_close(fd_new);
	return 1;
}
   
u32 Selec_ReportTime(void)//上报时间的删选
{
	u32 i;
	u8 time[7];
	i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
	if(i==0)
	{
		return 1;
	}
	if(i==1)
	{
		if(JSON->Lastrealtime[5]==0)
		{
			MR((u32)time,(u32)JSON->reportfiletime,7);
			pYYMDHMS_AddS_hex(time,300);//每执行一次打包任务之后 分钟打包的时间标志都加1分钟
			i=Compare_DL698DataString(JSON->Lastrealtime,time,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i!=2)
			{
				//把这10分钟之内的东西拼在一起上报一个文件
				Merge_RealTimeData_JsonFile(JSON->Lastrealtime);
			}
		}
		return 1;
	}
	return 0;
}

u8 *GetFileJsonName(u8 *ptime)//得到上报文件的压缩包名字 (不带路径)
{
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	Len=0;
	p8=(u8 *)ADDR_DATABUFF;
	//打包完成之后处理文件的名字,供上传的时候调用
	Len=0;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:组帧里面文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);	
	#endif
	return p8;
}

u8 *GetFileJsonName_Compress(u8 *ptime)//得到上报文件压缩包的名字(带绝对路径)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONREALPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;	
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:上报文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetFrzFileJsonName_Compress(void)//得到上报文件压缩包的名字(带绝对路径)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	mystrcpy((char *)p8+Len, JSONFREPATH);
	i=mystrlen((const char *)p8);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_FRE);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	Get698CurTime_Ascii(p8+Len,1);//得到698当前时间转换成asc码		
	mystrcpy((char *)p8+Len+8, ".tar.gz");
	//myprintf("[%s:%d]:文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	return p8;
}

u8 *GetRealTime_FileJson(u8 *ptime)//得到上报文件压缩包的名字(带绝对路径)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//得到698当前时间转换成asc码		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".json");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:上报文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u8 *GetRealTime_FileJson_Pack(u8 *ptime)//得到上报文件压缩包的名字(带绝对路径)
{
	
	u32 i;
	//u32 SLen;
	u32 Len;
	u8 *p8;
	u8 *paddr;
	p8=(u8 *)ADDR_DATABUFF;

	Len=0;
	//mystrcpy((char *)p8+Len, JSONREALPATH);
	//i=mystrlen((const char *)p8);
	//Len+=i;
	mystrcpy((char *)p8+Len, JSONFILENAMEHEAD_REAL);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写地址
	MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);
	paddr=(u8 *)ADDR_DATABUFF+1024;
	Add_StrToASCII(p8+Len,paddr+1,paddr[0]);//pbuf里面的字节输出到pstr并转成asc码
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//写时间
	mystrcpy((char *)p8+Len, "_");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	//Get698CurTime_Ascii(p8+Len);//得到698当前时间转换成asc码		
	Get698Time_Ascii(ptime, p8+Len);
	mystrcpy((char *)p8+Len+12, ".tar.bz2");
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:上报文件的名字  %s \n",(u32)&__func__,(u32)__LINE__,(u32)p8);
	#endif
	return p8;
}

u32 AnalyzeReportFile(void)//分析上报的文件,(文件大小,文件块总数)
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	u32 filesize;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDWR);
	if(-1==fd)
	{
		i=Compare_DL698DataString(JSON->Lastrealtime,JSON->reportfiletime,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
		if(i==1)
		{
			pYYMDHMS_AddS_hex(JSON->reportfiletime, 60);
		}
		return 0;
	}
	#ifdef DEBUG_JSON
	myprintf("===============开始分析文件============= \n",0,0,0);
	#endif
	filesize=ms->link_lseek(fd,0,SEEK_END);
	if(0==filesize)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:文件为空\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	JSON->g_Filesize=filesize;
	//需要上传块数量
	JSON->g_Need_report_Block=filesize/REPORT_FILEJSON_DATASIZE;
	//至少上传一块
	if(0==JSON->g_Need_report_Block)
	{
		JSON->g_Need_report_Block=1;
	}else
	{
		i=filesize%REPORT_FILEJSON_DATASIZE;
		if(i!=0)
		{
			JSON->g_Need_report_Block++;//超出整块再加一块
		}
	}
	JSON->g_Block_Number=0;
	ms->link_close(fd);
	
	return 1;
}

u32 GetCurBlockSize(void)//返回这一块上报数据大小
{
	u32 LEN;
	//07 01 08 f0 01 08 00 02 02 12 00 00 09
	LEN=0;
	if(JSON->g_Filesize>((JSON->g_Block_Number+1)*REPORT_FILEJSON_DATASIZE))
	{
		LEN=REPORT_FILEJSON_DATASIZE;
	}else
	{
		if(JSON->g_Filesize>(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE))
		{
			LEN=JSON->g_Filesize-(JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE);
		}else
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:传输序号错误 JSON->g_Block_Number=%d\n",(u32)&__func__,(u32)__LINE__,JSON->g_Block_Number);
			#endif
			return 0;
		}
	}
	return LEN;
}

u32 GetCurBlockData(u8 *pout,u32 Len)//返回上报这一块具体内容
{
	u8 *pfilename;
	s32 fd;
	u32 i;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	if(0==Len)
	{
		return 0;
	}
	//ms->link_system("cd /tmp/real_data");
	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		return 0;
	}
	//fd=ms->link_open((char *)JSON->RealFileName,O_RDONLY);
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		return 0;
	}
	ms->link_lseek(fd,JSON->g_Block_Number*REPORT_FILEJSON_DATASIZE,SEEK_SET);
	i=ms->link_read(fd,(u8 *)pout,Len);
	if(i!=Len)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:读上报文件失败 i=%d\n",(u32)&__func__,(u32)__LINE__,i);
		#endif
		return 0;
	}
	ms->link_close(fd);
	return i;
}

u32 File_HCS(void)//算出文件的校验(算不出来问题应该也不是很大)
{
	s32 fd;
	u32 i;
	u32 Len;
	u32 HCS;
	u8 *p8;
	u8 *pfilename;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	

	pfilename=GetFileJsonName_Compress(JSON->reportfiletime);
	if(!pfilename)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:取文件名失败\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	fd=ms->link_open((char *)pfilename,O_RDONLY);
	if(-1==fd)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:打开文件失败\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	i=ms->link_lseek(fd,0,SEEK_END);
	p8=(u8 *)ms->ms_malloc(i);
	if(!p8)
	{
		ms->link_close(fd);
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:分配空间失败\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	ms->link_lseek(fd,0,SEEK_SET);
	Len=ms->link_read(fd,(u8 *)p8,i);
	if(Len!=i)
	{
		ms->link_close(fd);
		ms->ms_free(p8);
		p8=NULL;
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:读数据长度不对i=%d,",(u32)&__func__,(u32)__LINE__,i);
		myprintf("Len=%d\n",Len,0,0);
		#endif
		return 0;
	}
	HCS=0xffff;
	for(i=0;i<Len;i++)
	{
		HCS=(HCS >> 8) ^ fcstab[(HCS ^ p8[i]) & 0xff];
	}
	HCS ^= 0xffff; //complement
	
	ms->link_close(fd);
	ms->ms_free(p8);
	p8=NULL;
	return HCS;
}

u32 Report_FileInfo(u16 *pTime,u32 PORTn)//F0010700//启动传输
{
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	//u8* p8;
	u8* p8tx;
	u8* pfilename;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	
	
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif


UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

//APDU：88 04 01 01 40 00 02 00 01 1C 07 E0 0B 0E 00 02 00 00 00	
		p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
		p8tx[0]=0x88;//上报通知 REPORT-Notification
		p8tx[1]=0x04;//通知请求客户机服务上报透明数据
		i=UARTCtrl->REPORT_PIID;
		i++;
		i&=0x3f;
		UARTCtrl->REPORT_PIID=i;
		p8tx[2]=i;//PIID
//--------------上报启动传输命令帧，帧采用字符串格式，由于启动传输命令帧内容固定，所有字符串的总长度可以固定------//			
		p8tx[3]=0x81;
		p8tx[4]=0x89;//启动传输命令帧的字符串长度
		
		p8tx[5]=0x07;//方法7，启动文件传输
		p8tx[6]=0x01;
		p8tx[7]=i;//PIID
		
		p8tx[8]=0xf0;
		p8tx[9]=0x01;
		p8tx[10]=0x07;
		p8tx[11]=0x00;//文件传输OMD=F0010700
		
		p8tx[12]=0x02;
		p8tx[13]=0x03;//参数结构体
		
		p8tx[14]=0x02;
		p8tx[15]=0x06;//文件信息结构体
		
		p8tx[16]=DataType_visible_string;//源文件
		p8tx[17]=0x2f;
		LEN_TxAPDU=18;
		LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）

		pfilename=GetFileJsonName(JSON->reportfiletime);
		if(!pfilename)
		{
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:文件传输的name有错\n",(u32)&__func__,(u32)__LINE__,0);
			#endif
			return 0;
		}
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;
		p8tx[LEN_TxAPDU]=DataType_visible_string;//目标文件
		p8tx[LEN_TxAPDU+1]=0x2f;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		i=mystrlen((const char*)pfilename);
		MW((u32 )pfilename,(u32)p8tx+LEN_TxAPDU,i);
		LEN_TxAPDU+=i;
		LENmax_TxSPACE-=i;

		p8tx[LEN_TxAPDU]=0x06;//文件大小
		LEN_TxAPDU++;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>24)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>16)&0xff;
		p8tx[LEN_TxAPDU++]=(JSON->g_Filesize>>8)&0xff;
		p8tx[LEN_TxAPDU++]=JSON->g_Filesize&0xff;
		LENmax_TxSPACE-=5;

		p8tx[LEN_TxAPDU]=0x04;//文件属性
		p8tx[LEN_TxAPDU+1]=0x08;//长度
		p8tx[LEN_TxAPDU+2]=0x80;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x0a;
		p8tx[LEN_TxAPDU+1]=0x04;
		p8tx[LEN_TxAPDU+2]=0x56;
		p8tx[LEN_TxAPDU+3]=0x31;
		p8tx[LEN_TxAPDU+4]=0x2e;
		p8tx[LEN_TxAPDU+5]=0x30;
		LEN_TxAPDU+=6;
		LENmax_TxSPACE-=6;
		//
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x12;//传输块大小
		p8tx[LEN_TxAPDU+1]=REPORT_FILEJSON_DATASIZE/256;
		p8tx[LEN_TxAPDU+2]=REPORT_FILEJSON_DATASIZE%256;
		LEN_TxAPDU+=3;
		LENmax_TxSPACE-=3;
		
		p8tx[LEN_TxAPDU]=0x02;//校验
		p8tx[LEN_TxAPDU+1]=0x02;
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;
		
		p8tx[LEN_TxAPDU]=0x16;
		p8tx[LEN_TxAPDU+1]=0;
		p8tx[LEN_TxAPDU+2]=0x09;
		p8tx[LEN_TxAPDU+3]=0x02;
		//计算校验
		i=File_HCS();
		p8tx[LEN_TxAPDU+4]=(i>>8)&0xff;
		p8tx[LEN_TxAPDU+5]=i&0xff;
		p8tx[LEN_TxAPDU+6]=0x0;
		LEN_TxAPDU+=7;
		LENmax_TxSPACE-=7;
		
		p8tx[LEN_TxAPDU+0]=0;//时间标签
		p8tx[LEN_TxAPDU+1]=0;//上报信息FollowReport OPTIONAL
		LEN_TxAPDU+=2;
		LENmax_TxSPACE-=2;

//----------------启动传输END------------------------// 		

			p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=0x68;
			//2byte长度域L
			p8tx[3]=0x83;//控制域C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
			p8tx[11]=0;//客户机地址CA
			//2byte帧头校验HCS
			MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			//2byte校验
			LEN_Tx=14+LEN_TxAPDU+2;
			p8tx[LEN_Tx]=0x16;
			LEN_Tx++;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=(LEN_Tx)&0xff;
			p8tx[2]=(LEN_Tx>>8)&0xff;

			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误

			UARTCtrl->Task=2;//2=等待单帧数据发送
			//发送帧缓存在DataBuff+LEN_UARTnTx,用以重发
			i=Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
			i+=Get_LEN_UARTnTx(PORTn);
			MW((u32)p8tx,i,LEN_Tx+2);
			//上报响应超时时间	TI
			pTime[0]=0;//默认最小值
			UARTCtrl->REPORT_OVER_SECOND=0;//上报响应超时时间寄存
			UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
			UARTCtrl->HostACK=8;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
			//最大上报次数
			#ifdef DEBUG_JSON
			myprintf("======================增量上报字节数:%d==========\n",UARTCtrl->TxByte,0,0);
			//create_file();
			for(i=0;i<UARTCtrl->TxByte;i++)
				myprintf("%02x ",p8tx[i],0,0);
			myprintf("\n",0,0,0);
			#endif
			return 1;
}

u32 Report_FileData(u16 *pTime,u32 PORTn)//F0010800//上报数据内容
{
	u8 *p8tx;
	u32 i;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u16 filesize;
	u32 LENmax_TxSPACE;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef* ClientConnectInfo;
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
	#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
	#else
		//客户机接收缓冲长度
		ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
		i=ClientConnectInfo->LEN_Rx;
		i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
		if(i<2048)
		{
			i=2048;
		}
		if(i>(TxAPDUOffset+1))
		{//不为0
			i-=(TxAPDUOffset+1);
			if(LENmax_TxSPACE>i)
			{
				LENmax_TxSPACE=i;
			}
		}
	#endif
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	LEN_TxAPDU=0;
	p8tx[LEN_TxAPDU++]=0x88;
	p8tx[LEN_TxAPDU++]=0x04;
	i=UARTCtrl->REPORT_PIID;
	i++;
	i&=0x3f;
	UARTCtrl->REPORT_PIID=i;
	p8tx[LEN_TxAPDU++]=i;//PIID
	
	filesize=GetCurBlockSize();
	//长度
	if(256>16+filesize)
	{
		p8tx[LEN_TxAPDU++]=0x81;//
		p8tx[LEN_TxAPDU++]=16+filesize;//长度
	}else
	{		
		p8tx[LEN_TxAPDU++]=0x82;//
		p8tx[LEN_TxAPDU++]=(17+filesize)>>8;//长度
		p8tx[LEN_TxAPDU++]=(17+filesize)&0xff;//长度	
	}
	
	p8tx[LEN_TxAPDU++]=0x07;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=i;

	p8tx[LEN_TxAPDU++]=0xf0;
	p8tx[LEN_TxAPDU++]=0x01;
	p8tx[LEN_TxAPDU++]=0x08;
	p8tx[LEN_TxAPDU++]=0x00;

	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x02;
	p8tx[LEN_TxAPDU++]=0x12;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number>>8;
	p8tx[LEN_TxAPDU++]=JSON->g_Block_Number&0xff;

	p8tx[LEN_TxAPDU++]=0x09;
	if(LENmax_TxSPACE<LEN_TxAPDU+REPORT_FILEJSON_DATASIZE)
	{
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:字符长度超\n",(u32)&__func__,(u32)__LINE__,0);
		#endif
		return 0;
	}
	LENmax_TxSPACE-=(LEN_TxAPDU+REPORT_FILEJSON_DATASIZE);
	//从文件中提取数字
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:字符长度filesize=%d\n",(u32)&__func__,(u32)__LINE__,filesize);
	#endif
	if(filesize<256)
	{
		p8tx[LEN_TxAPDU++]=0x81;
		p8tx[LEN_TxAPDU++]=filesize;
	}else
	{
		p8tx[LEN_TxAPDU++]=0x82;
		p8tx[LEN_TxAPDU++]=filesize/256;
		p8tx[LEN_TxAPDU++]=filesize%256;
	}
	GetCurBlockData(p8tx+LEN_TxAPDU,filesize);
	LEN_TxAPDU+=filesize;	
	p8tx[LEN_TxAPDU++]=0;
	
	p8tx[LEN_TxAPDU++]=0;//时间标签
	p8tx[LEN_TxAPDU++]=0;//上报信息FollowReport OPTIONAL
				
	//----------------启动传输END------------------------//			

	p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
	p8tx[0]=0x68;
	//2byte长度域L
	p8tx[3]=0x83;//控制域C
	p8tx[4]=5;//TSA
	MR((u32)p8tx+5,ADDR_4001+2,6);
	Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
	p8tx[11]=0;//客户机地址CA
	//2byte帧头校验HCS
	MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
	//2byte校验
	LEN_Tx=14+LEN_TxAPDU+2;
	p8tx[LEN_Tx]=0x16;
	LEN_Tx++;
	UARTCtrl->TxByte=LEN_Tx;
	LEN_Tx-=2;
	p8tx[1]=(LEN_Tx)&0xff;
	p8tx[2]=(LEN_Tx>>8)&0xff;
	
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	
	UARTCtrl->Task=2;//2=等待单帧数据发送
	//发送帧缓存在DataBuff+LEN_UARTnTx,用以重发
	i=Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	i+=Get_LEN_UARTnTx(PORTn);
	MW((u32)p8tx,i,LEN_Tx+2);
	//上报响应超时时间  TI
	pTime[0]=0;//默认最小值
	UARTCtrl->REPORT_OVER_SECOND=0;//上报响应超时时间寄存
	UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
	UARTCtrl->HostACK=8;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟,8分钟上报任务
	#ifdef DEBUG_JSON
	myprintf("======================增量上报字节数:%d==========\n",LEN_Tx,0,0);
	for(i=0;i<UARTCtrl->TxByte;i++)
		myprintf("%02x ",p8tx[i],0,0);
	myprintf("\n",0,0,0);
	#endif
	return 1;
}

u32 Report_JsonFile(u16 *pTime,u32 PORTn)//PORTn端口号,pTime端口超时时间
{
	//u16 *p16time;
	u32 i;
	u32 ret;
	ret=0;
	if(FileDownload->New==0)//远程升级完成,上报停止
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->reportfiletime);
	if(i)
	{
		return ret;
	}
	i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
	if(i)
	{
		return ret;
	}
	#ifdef DEBUG_JSON
	myprintf("===============进入上报的程序JSON->Reportjsonfile_Task = %d============= \n",JSON->Reportjsonfile_Task,0,0);
	#endif
	switch(JSON->Reportjsonfile_Task)
	{
		case 0://分析上报文件
		//从今天的第一个生成的文件开始上报	
			i=Selec_ReportTime();
			if(0==i)
			{
				return ret;
			}
			i=AnalyzeReportFile();
			if(i)
			{
				#ifdef DEBUG_JSON
				myprintf("===============分析文件成功,下面开始上传文件============= \n",0,0,0);
				#endif
				JSON->Reportjsonfile_Task++;
				ret=1;
			}
			break;
		case 1://启动传输
			i=Report_FileInfo(pTime,PORTn);
			if(i)
			{
				JSON->Reportjsonfile_Task++;
				ret=1;
			}else
			{
				//上报失败的时候,上报时间需要加一跳过这个文件,不然一直死循环
				ClrReportInfo();
			}
			break;
		case 2://上传文件
			#ifdef DEBUG_JSON
			myprintf("===============开始上传文件============= \n",0,0,0);
			#endif
			i=Report_FileData(pTime,PORTn);
			if(i)
			{
				Next_DataBlock();
				if(IsReportEnd())//结束了
				{
					#ifdef DEBUG_JSON
					myprintf("===============上传文件成功============= \n",0,0,0);
					#endif
					ClrReportInfo();
				}
				ret=1;
			}else
			{
				ClrReportInfo();
			}
			break;
		default:
			JSON->Reportjsonfile_Task=0;
		break;
	}
	return ret;
}
/*
void tar_text(void)
{
	
	u32 i;
	u32 SLen;
	u32 size;
	u8 *p8;

	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	
	p8=(u8 *)ADDR_DATABUFF;
	//while(1)
	{
		SLen=0;
		mystrcpy((char *)p8+SLen, "tar zcvf ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "111.tar.gz ");
		i=mystrlen((const char *)p8+SLen);
		SLen+=i;
		mystrcpy((char *)p8+SLen, "/usr/app/frz_data/");
		size=ms->ms_malloc_sizemax();
		//p8=(u8 *)ms->ms_malloc(13*1024*1024);
		//if(!p8)
		{
			//return;
		}
		myprintf("size=%d\n",size,0,0);
		
		myprintf("开始压缩 \n",0,0,0);
		myprintf("[%s:%d]:命令 %s\n",(u32)&__func__,(u32)__LINE__,(u32)p8);
		ms->link_system((char *)p8);
		myprintf("压缩结束 \n",0,0,0);
		//ms->link_sleep(1);
	}
}

void save(u8 *pbuf,u32 FileID)
{
	u32 i;
	u32 x;
	u32 y;
	//u32 FileID;
	//u32 EventID;//事件关联的ID
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	//u8* p8;
	//UARTCtrl_TypeDef *UARTCtrl;
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//FileID=Get_FileID(PORTn);//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	Comm_Ram->msFILESearchCount=0;

		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{	
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
		}
		else
		{
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
		}

		//log_Flash();

	
	if(filename!=0xffffffff)
	{
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		y=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
		if(y>NUM_RMmax)
		{
			y=NUM_RMmax;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
		RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
		Addr0=pbuf[0]|(pbuf[1]<<8);
		Addr1=pbuf[2]|(pbuf[3]<<8);
		Addr2=pbuf[4]|(pbuf[5]<<8);
		for(i=0;i<y;i++)
		{
			if(RecordFileHead->Addr0==Addr0)
			{
				if(RecordFileHead->Addr1==Addr1)
				{
					if(RecordFileHead->Addr2==Addr2)
					{
						break;
					}
				}
			}
			RecordFileHead++;
		}
		if(i<y)
		{//地址已存在
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//补抄曲线等数据时可能会重复,故不能删除文件
			}
		}
	}
	if(filename==0xffffffff)
	{//没找到

		{//FLASH文件	
			while(1)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
				{
					if(file->BLOCK_ENTRY==0)
					{
						break;
					}
					file++;
				}
				if(filename>=recordFILENAMEmax_FLASH)
				{//没空
					//myprintf("[%s:%d]:开始执行删除命令\n",(u32)&__func__,(u32)__LINE__,0);
					i=AutoDeleteFile(0);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
					if(i)
					{//有删除
						continue;
					}
					return;
				}
				break;
			}
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//空文件
		ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;

			file->LEN_DATA=0;
			file->NUM_DATA=0;
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//采集存储时标
				MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//采集启动时间
			}else
			{
				MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//采集存储时标
				MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//采集启动时间
			}
	}
//写文件
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//错
		ms->msfile_Delete(filename);
		return;
	}
	//写数据
	if(filename>=FILENAMEmax_FLASH)
	{//RAM中文件
		i=ms->ms_malloc_sizemax();//最大可分配的尺寸(字节数)
		if(i<(1024*1024))
		{//空间<1M,保留至少1M空间用于记录数据读取
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y==0)
			{//没删除
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	//myprintf("[%s:%d]:RecordFileHead->Addr0=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr0);
	///myprintf("[%s:%d]:RecordFileHead->Addr1=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr1);
	//myprintf("[%s:%d]:RecordFileHead->Addr2=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->Addr2);
	//myprintf("[%s:%d]:RecordFileHead->LEN_DATA=%04x\n ",(u32)&__func__,(u32)__LINE__,RecordFileHead->LEN_DATA);

	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//写地址列表
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//数据数+1
	x++;
	file->NUM_DATA=x;
	//myprintf("[%s:%d]:写入Flash成功\n",(u32)&__func__,(u32)__LINE__,0);

}

u32 MoreThan_StackLen(u32 LENMAX)
{
	return 0;
}
u32 GetOneMetAddr_TEST(u16 *MetCount)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u16 Met=*MetCount;
	p16=(u16*)(ADDR_AddrList+2);
	for(;Met<TEST_METNUM;Met++)
	{	
		//i=bcd_hex((Met)+1);
		i=Met;
		//myprintf("i=%d \n",i,0,0);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;
		p16+=3;
		break;
	}
	if((Met)>=TEST_METNUM)
	{
		p16[0]=0;
		return 0;
	}
	*MetCount=Met;
	p16=(u16*)ADDR_AddrList;
	p16[0]=1;
	p8=(u8 *)(ADDR_AddrList+2);
	for(i=6;i>0;i--)
	{
		myprintf("%02x",p8[i-1],0,0);
	}
	myprintf("\n",0,0,0);
	return 1;
}
 */
u32 CalCulate_Space(const char *path)
{
	return 8*1024*1024;
}

void Delet_RAM_FrzFile(void)
{
	u32 i;
	u32 Len;
	u8 *p8;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	p8=(u8 *)ADDR_DATABUFF;
	Len=0;
	mystrcpy((char *)p8+Len, "rm ");
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	mystrcpy((char *)p8+Len, JSONFREPATH_RAM);
	i=mystrlen((const char *)p8+Len);
	Len+=i;
	p8[Len++]='*';
	p8[Len]=0;
	ms->link_system((char *)p8);
}

u32  Copy_RamToFlash(s32 fd)
{
	s32 fd1;
	u32 i;
	u32 filelen;
	u32 x;
	u32 y;
	u8 *p8;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	Mkdir_JsonFile(JSONFREPATH);
	pname=CreateFreJsonFile();//创建一个新的项目文件,返回文件句柄
	fd1=ms->link_open((char *)pname,O_RDWR);
	if(fd1==-1)//能找到这个文件
	{
		fd1=ms->link_open((char *)pname,O_CREAT|O_RDWR);
		if(fd1==-1)
		{
			return 1;
		}
	}
		ms->link_lseek(fd1,0,SEEK_END);
		p8=(u8 *)ms->ms_malloc(RAMTOFLASH_COPYONESIZE);
		if(!p8)
		{
			ms->link_close(fd1);
			return 1;
		}
		filelen=ms->link_lseek(fd,0,SEEK_END);//RAM空间下的文件大小
		if(filelen<=(RAMTOFLASH_COPYONESIZE))
		{
			//可以直接拷贝过去
			ms->link_lseek(fd,0,SEEK_SET);//RAM空间下的文件大小
			i=ms->link_read(fd,(u8 *)p8,filelen);
			if(i==filelen)//读成功
			{
				i=ms->link_write(fd1,(u8 *)p8,filelen);
				if(i==filelen)
				{
					ms->link_close(fd1);
					return 0;
				}else//写操作失败有可能是存储空间已经满了Flash,需要删除Filename
				{
					
				}
			}
		}else
		{
			y=RAMTOFLASH_COPYONESIZE;
			x=0;
			while(x<filelen)
			{
				ms->link_lseek(fd,x,SEEK_SET);//RAM空间下的文件大小
				if(filelen>=(y+x))//总文件大小=最大一次写入量+已经写入量
				{//那就用最大写入量写入
					i=ms->link_read(fd,(u8 *)p8,y);
					if(y==i)
					{
						i=ms->link_write(fd1,(u8 *)p8,y);
					}else//写操作失败有可能是存储空间已经满了Flash,需要删除Filename
					{
						
					}
					x+=y;
				}else
				{
					if(filelen>x)
					{
						y=filelen-x;
						i=ms->link_read(fd,(u8 *)p8,y);
						if(y==i)
						{
							i=ms->link_write(fd1,(u8 *)p8,y);
							if(y==i)
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 0;
							}else
							{
								Delet_RAM_FrzFile();
								ms->link_close(fd1);
								return 1;
							}
						}
					}else//说明已经全部写完了
					{
						Delet_RAM_FrzFile();
						ms->link_close(fd1);
						return 0;
					}
				}
				
			}
		}
		ms->link_close(fd1);
		return 1;
	
}

u32 Find_TodayFrzFile()
{
	s32 fd;
	u8 *pname;
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();
	pname=GetFrzFileJsonName_Compress();//创建一个新的项目文件,返回文件句柄
	fd=ms->link_open((char *)pname,O_CREAT|O_EXCL);
	if(fd!=-1)
	{
		ms->link_close(fd);
		return 0;
	}
	return 1;
}

/*
void TEST_PACK(void)
{
	u32 i;
	u32 x;
	u32 CJSONLEN;//JSON字符串计数器,初始化0
	u32 JSONDataLen;
	u8 *p8;
	u8 *p8s;
	u8 *pAddr;//地址u8类型指针
	u8 *pname;
	u16 *p16;
	u16 AddrNUM;//档案的有效数量
	s32 fd;
	//u8 addr[6]={0};
	ms_Type *ms;
	ms=(ms_Type *)Get_ms();	
	
	switch(FrzPackTask)
	{
		case 0://打包之前需要做的准备
			CurMetCount=0;
			FrzPackTask++;
			//打包之前清空RAM下面所有的空间
			Delet_RAM_FrzFile();
			break;
		case 1:	
			//i=GetOneMetAddr();
			i=GetOneMetAddr_TEST();
			if(i==0)//全部表地址已经打包结束
			{
				FrzPackTask=2;//重置每天打包日冻结的标志
				myprintf("[%s:%d]:数据处理完毕开始压缩\n",(u32)&__func__,(u32)__LINE__,0);
				break;
			}
			CurMetCount++;//下一次循环就找下一只表
			//得到文件名字
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_END);
				if((i>=8*1024*1024))
				//if(1024)
				{
					//从RAM空间移动到Flash空间
					i=Copy_RamToFlash(fd);
					if(i!=0)
					{
						ms->link_close(fd);
						return ;
					}
				}
				ms->link_close(fd);
			}
			p8=(u8 *)ms->ms_calloc(1024*1024);
			if(!p8)
			{
				myprintf("[%s:%d]:申请空间失败i\n",(u32)&__func__,(u32)__LINE__,0);
				return ;
			}
			CJSONLEN=0;
			JSONDataLen=0;
			p8s=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_AddrList);
			p16++;//前面一个是表的数量,后面一个是表的地址
			pAddr=(u8 *)p16;
			i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			Add_StrToASCII(p8s,pAddr,6);//pbuf里面的字节输出到pstr并转成asc码
			i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
			JSONDataLen+=i;
			CJSONLEN+=i;
			i=AllDataPack(p16,p8+CJSONLEN);
			CJSONLEN+=i;
			if(i>0)//说明是有数据的
			{
				i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
				CJSONLEN+=i;
				i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
				CJSONLEN+=i;
				CJSONLEN=Delet_COMMA(p8,CJSONLEN);//删除逗号
				i=Str_LF(p8+CJSONLEN);
				CJSONLEN+=i;			
			}else
			{		
				if(CJSONLEN>=JSONDataLen)
				{
					CJSONLEN-=JSONDataLen;
				}
			}
			
			if(CJSONLEN>0)//这一只表示有数据的
			{
				//下面开始往RAM空间里面写数据
				i=CalCulate_Space(JSONFREPATH_RAM);//计算这个空间里面剩余的容量
				if(i>=CJSONLEN)//如果剩余的空间够用
				{
					//得到文件名字
					Mkdir_JsonFile(JSONFREPATH_RAM);
					pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
					fd=ms->link_open((char *)pname,O_RDWR);
					if(fd!=-1)
					{
						ms->link_lseek(fd,0,SEEK_END);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}else
					{
						fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
						if(-1==fd)
						{
							myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
							ms->ms_free(p8);
							return;
						}
						ms->link_lseek(fd,0,SEEK_SET);
						i=ms->link_write(fd,p8,CJSONLEN);
						if(i==CJSONLEN)
						{
							myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}else
						{
							i=ms->link_write(fd,p8,CJSONLEN);
							myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
						}
					}
				}else
				{
					
				}
				ms->link_close(fd);//关闭文件句柄
			}
			ms->ms_free(p8);
			break;
		case 2:
			//得到文件名字
			Mkdir_JsonFile(JSONFREPATH_RAM);
			pname=CreateFreJsonFile_RAM();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				i=ms->link_lseek(fd,0,SEEK_SET);
				//从RAM空间移动到Flash空间
				i=Copy_RamToFlash(fd);
				if(i!=0)
				{
					ms->link_close(fd);
					return ;
				}
				ms->link_close(fd);
			}
			Mkdir_JsonFile(JSONFREPATH);
			pname=CreateFreJsonFile();//创建一个新的项目文件,返回文件句柄
			fd=ms->link_open((char *)pname,O_RDWR);
			if(fd!=-1)
			{
				ms->link_close(fd);
				Compress_JSONFile(pname,1024,0);//将JSON文件打包成tar.gz格式
				JSON->IsCreateFrzJson=1;
				JSON->PackFrzJsonTask=1;
			}else
			{
				FrzPackTask=0;	
				//说明打包失败了
				if(JSON->IsCreateFrzJson==2)
				{
					JSON->IsCreateFrzJson=1;
					JSON->PackFrzJsonTask=1;
				}
				else
				{
					JSON->IsCreateFrzJson++;
				}
			}	
			break;
		default:
			FrzPackTask=0;
		break;
	}

//===============================1.取档案里面所有的表地址=========================== 
	//GetMetAddr();//取得所有表的地址	
	p16=(u16*)(ADDR_AddrList);//取得档案的数量用unsigned short表示
	p16[0]=2;
	p16++;	
	for(x=0;x<2;x++)
	{	
		i=hex_bcd(x+1);
		p16[0]=(i&0xff)|(i>>8)&0xff;
		p16[1]=(i>>16)&0xff|(i>>24)&0xff;
		p16[2]=0;

		p8=(u8 *)p16;
		for(i=0;i<3;i++)
			myprintf("%04x ",p16[i],0,0);
		myprintf("\n",0,0,0);
		p16+=3;
	}
	AddrNUM=2;
	//p16++;		
//===============================2.向堆申请空间=====================================
	p8=(u8 *)ms->ms_calloc(1024);
	if(!p8)
	{
		myprintf("[%s:%d]:malloc空间不足\n",(u32)&__func__,(u32)__LINE__,0);
		return ;
	}
//==============================3.开始处理数据========================================
	CJSONLEN=0;//JSON字符串计数器,初始化0
	p8s=(u8 *)ADDR_DATABUFF;

	p16=(u16*)(ADDR_AddrList);//取得档案的数量用unsigned short表示
	p16++;
	for(x=0;x<AddrNUM;x++)
	{
		if(0!=x)
		{
			p16+=3;
		}
		pAddr=(u8 *)p16;
		JSONDataLen=0;
		i=CreateArrayHead_Json(p8+CJSONLEN, DEBUG_FIRST_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		Add_StrToASCII(p8s,pAddr,6);//pbuf里面的字节输出到pstr并转成asc码
		i=Createitem_Json(p8+CJSONLEN, STR_ADDR, p8s, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=CreateitemArrayHead_Json(p8+CJSONLEN, STR_ITEMLIST, DEBUG_SECOND_SPACENUM);
		JSONDataLen+=i;
		CJSONLEN+=i;
		i=AllDataPack(p16,p8+CJSONLEN);
		CJSONLEN+=i;
		
		if(i>0)//说明是有数据的
		{
			i=CreateitemArrayEnd_Json(p8, CJSONLEN,DEBUG_SECOND_SPACENUM);
			CJSONLEN+=i;
			i=CreateArrayEnd_Json(p8, CJSONLEN, DEBUG_FIRST_SPACENUM);
			CJSONLEN+=i;
			CJSONLEN=Delet_COMMA(p8,CJSONLEN);//删除逗号
			i=Str_LF(p8+CJSONLEN);
			CJSONLEN+=i;			
		}else
		{		
			if(CJSONLEN>=JSONDataLen)
			{
				CJSONLEN-=JSONDataLen;
			}
		}	
	}
	if(CJSONLEN)
	{
		Delet_FrzJsonFile();//删除掉原来的JSON文件
		pname=CreateFreJsonFile();//创建一个新的项目文件,返回文件句柄
		fd=ms->link_open((char *)pname,O_RDWR);
		if(-1!=fd)//已经存在该文件就删除
		{
			//ms->link_remove((char *)pname);
			//myprintf("\n\n[%s:%d]:删除文件:%s \n\n", (u32)&__func__, __LINE__, (u32 )pname);
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//将JSON文件打包成tar.gz格式
			}else
			{
				myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}else
		{
			fd=ms->link_open((char *)pname,O_CREAT|O_RDWR);
			if(-1==fd)
			{
				myprintf("[%s:%d]:创建文件失败\n",(u32)&__func__,(u32)__LINE__,0);
				return;
			}
			i=ms->link_write(fd,p8,CJSONLEN);
			if(i==CJSONLEN)
			{
				myprintf("[%s:%d]:写入文件成功  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
				Compress_JSONFile(pname,1024,0);//将JSON文件打包成tar.gz格式
			}else
			{
				myprintf("[%s:%d]:写入文件失败  字符数量i=%d\n",(u32)&__func__,(u32)__LINE__,i);
			}
		}
		ms->link_close(fd);
	}
	ms->ms_free(p8);
	*/
	/*
u8 day[]={0x01,0x01,0x5b,0x01,0x50,0x04,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x00,0x00,0x20,0x02,0x00,0x1C,0x07,0xE4,0x07,0x0A,0x00,0x00,0x00,0x01,0x05,0x06,0x00,0x1C,0x34,0x6B,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x52,0x9D,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x0C,0xE1,0xCD,0x01,0x05,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x08,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
//u8 min15[]={0x50,0x02,0x02,0x00,0x03,0x20,0x21,0x02,0x00,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x01,0x1C,0x06,0x06,0x0A,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1C,0x34,0x6B,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x2D,0x84,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x21,0xC8,0x48,0x00,0x00,0x00,0x0C,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x20,0x5D,0xF2,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,
	//0x09,0x16,0x00,0x00,0x00,0x1F,0x78,0xC6,0x00,0x00,0x00,0x34,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x26,0xC3,0xD3,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1E,0xFB,0xD1,0x00,0x00,0x00,0x08,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1F,0x91,0xAF,0x00,0x00,0x00,0x09,0x07,0xE4,0x07,0x09,0x16,0x00,0x00,0x00,0x1D,0x34,0xD5,0x00,0x00,0x00,0x08,
	//0x00,0x00
//};

u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x0B,0x20,0x00,0x02,0x01,0x20,0x00,0x02,0x02,0x20,0x00,0x02,0x03,0x20,0x01,0x02,0x01,0x20,0x01,0x02,0x02,0x20,0x01,0x02,0x03,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x20,0x04,0x02,0x03,0x20,0x04,0x02,0x04,0x01,0x12,0x12,0x12,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x06,0x08,0xF8,0x08,0xED,0x08,0xF1,0x00,0x00,0x08,0xEB,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC7,0x00,0x00,0x03,0x58,0x00,0x00,
	0x4C,0x5C,0x00,0x00,0x14,0x79,0x00,0x00,0x1A,0xA7,0x00,0x00,0x1D,0x3A,0x08,0xFB,0x08,0xF0,0x08,0xF5,0x00,0x00,0x08,0xF4,0x00,0x00,0x0B,0xAF,0x00,0x00,0x0C,0xCA,0x00,0x00,0x03,0x52,0x00,0x00,0x4C,0x9B,0x00,0x00,0x14,0x96,0x00,0x00,0x1A,0xB5,0x00,0x00,0x1D,0x4E,0x08,0xFA,0x08,0xF0,0x08,0xF4,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x50,0x00,0x00,0x14,0x70,0x00,0x00,0x1A,0xA8,0x00,
	0x00,0x1D,0x38,0x08,0xFC,0x08,0xF1,0x08,0xF6,0x00,0x00,0x08,0xF0,0x00,0x00,0x0B,0xAD,0x00,0x00,0x0C,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0xA0,0x00,0x00,0x14,0x94,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xF0,0x08,0xF3,0x00,0x00,0x08,0xE7,0x00,0x00,0x0B,0xAB,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x70,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xA8,0x00,0x00,0x1D,0x4C,0x08,0xF9,0x08,0xEF,0x08,0xF3,0x00,0x00,
	0x08,0xED,0x00,0x00,0x0B,0xAA,0x00,0x00,0x0C,0xC5,0x00,0x00,0x00,0x00,0x00,0x00,0x4C,0x90,0x00,0x00,0x14,0x84,0x00,0x00,0x1A,0xBC,0x00,0x00,0x1D,0x4C,0x00,0x00
};

//u8 min[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x05,0x20,0x00,0x02,0x01,0x20,0x01,0x02,0x01,0x20,0x01,0x04,0x00,0x20,0x04,0x02,0x01,0x20,0x04,0x02,0x02,0x01,0x12,0x07,0x05,0x05,0x05,0x0A,0x00,0x00,0x00};

u8 min15[]={0x01,0x01,0x5b,0x01,0x50,0x02,0x02,0x00,0x06,0x00,0x10,0x02,0x01,0x00,0x20,0x02,0x01,0x00,0x50,0x02,0x01,0x00,0x60,0x02,0x01,0x00,0x70,0x02,0x01,0x00,0x80,0x02,0x01,0x01,0x06,0x06,0x06,0x06,0x06,0x06,0x0C,0x00,0x07,0x72,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0xA8,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x72,0xD5,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x73,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x4F,0xB4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x32,0xC9,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x32,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAD,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xAF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x08,0x33,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0xB0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x00,0x07,0x71,0xF7,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x24,0x00,0x00,0x00,0x00,0x00,
	0x00,0x51,0xA7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x51,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xA9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x72,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

VOID test_saveMIN(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
		p8=(u8 *)ADDR_DATABUFF;
	myprintf("分钟存储 \n",0,0,0);

	for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min,sizeof(min));
			Offset+=sizeof(min);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f22000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("全部都写完成 \n",0,0,0);
}

VOID test_saveMIN15(void)
{
	u32 i;
	u16 datalen;
	u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1A,0x00,0x00,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("分钟存储 \n",0,0,0);

	for(FILEID=0;FILEID<96;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(min15);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)min15,sizeof(min15));
			Offset+=sizeof(min15);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f19000);
			//break;
		}
		pYYMDHMS_AddS_hex(time, 900);
		//break;
	}
	myprintf("全部都写完成 \n",0,0,0);
}

VOID test_saveDAY(void)
{
	u32 i;
	u16 datalen;
	//u32 FILEID;
	u32 addrcnt;
	u32 Offset;
	//u32 z;
	//UARTCtrl_TypeDef *UARTCtrl;
	u32 hex;
	u8 *p8;
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	u8 addr[6]={0};
	u8 time[]={0x07,0xE4,0x07,0x1a,0x17,0x3b,0x00};
	p8=(u8 *)ADDR_DATABUFF;
	myprintf("进入天存储 \n",0,0,0);
	//for(FILEID=0;FILEID<TEST_TIME;FILEID++)
	{
		for(addrcnt=0;addrcnt<TEST_METNUM;addrcnt++)
		{
			Offset=0;
			i=addrcnt+1;
			//hex=bcd_hex(i);
			hex=i;
			//myprintf("hex=%d \n",hex,0,0);
			addr[0]=(hex)&0xff;
			addr[1]=(hex>>8)&0xff;
			addr[2]=(hex>>16)&0xff;
			addr[3]=(hex>>24)&0xff;
			addr[4]=0;
			addr[5]=0;
			MR((u32)p8+Offset,(u32)addr,sizeof(addr));
			Offset+=sizeof(addr);
			
			datalen=sizeof(day);
			p8[Offset++]=(datalen+21)&0xff;
			p8[Offset++]=((datalen+21)>>8)&0xff;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
			MR((u32)p8+Offset,(u32)time,sizeof(time));
			Offset+=7;
						
			MR((u32)p8+Offset,(u32)day,sizeof(day));
			Offset+=sizeof(day);			
			//for(z=0;z<Offset;z++)
				//myprintf("%02x ",p8[z],0,0);
			//myprintf("\n",0,0,0);
			save(p8,0x13f16000);
			//break;
		}
		//pYYMDHMS_AddS_hex(time, 60);
		//break;
	}
	myprintf("全部都写完成 \n",0,0,0);
}
*/

