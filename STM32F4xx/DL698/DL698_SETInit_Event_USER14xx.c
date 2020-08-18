
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"


#if (USER/100)==14//福建

//电能表失压事件
#define Init_Enable_Event_3000     0//事件有效标识bool
#define Init_Report_Event_3000     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表欠压事件
#define Init_Enable_Event_3001     0//事件有效标识bool
#define Init_Report_Event_3001     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表过压事件
#define Init_Enable_Event_3002     0//事件有效标识bool
#define Init_Report_Event_3002     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表断相事件
#define Init_Enable_Event_3003     0//事件有效标识bool
#define Init_Report_Event_3003     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表失流事件
#define Init_Enable_Event_3004     0//事件有效标识bool
#define Init_Report_Event_3004     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表过流事件
#define Init_Enable_Event_3005     0//事件有效标识bool
#define Init_Report_Event_3005     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表断流事件
#define Init_Enable_Event_3006     0//事件有效标识bool
#define Init_Report_Event_3006     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表潮流反向事件
#define Init_Enable_Event_3007     0//事件有效标识bool
#define Init_Report_Event_3007     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表过载事件
#define Init_Enable_Event_3008     0//事件有效标识bool
#define Init_Report_Event_3008     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表正向有功需量超限事件
#define Init_Enable_Event_3009     0//事件有效标识bool
#define Init_Report_Event_3009     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表反向有功需量超限事件
#define Init_Enable_Event_300A     0//事件有效标识bool
#define Init_Report_Event_300A     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表无功需量超限事件
#define Init_Enable_Event_300B     0//事件有效标识bool
#define Init_Report_Event_300B     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表功率因数超下限事件
#define Init_Enable_Event_300C     0//事件有效标识bool
#define Init_Report_Event_300C     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表全失压事件
#define Init_Enable_Event_300D     0//事件有效标识bool
#define Init_Report_Event_300D     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表辅助电源掉电事件
#define Init_Enable_Event_300E     0//事件有效标识bool
#define Init_Report_Event_300E     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电压逆相序事件
#define Init_Enable_Event_300F     1//事件有效标识bool
#define Init_Report_Event_300F     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电流逆相序事件
#define Init_Enable_Event_3010     0//事件有效标识bool
#define Init_Report_Event_3010     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表掉电事件
#define Init_Enable_Event_3011     0//事件有效标识bool
#define Init_Report_Event_3011     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表编程事件
#define Init_Enable_Event_3012     0//事件有效标识bool
#define Init_Report_Event_3012     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表清零事件
#define Init_Enable_Event_3013     0//事件有效标识bool
#define Init_Report_Event_3013     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表需量清零事件
#define Init_Enable_Event_3014     0//事件有效标识bool
#define Init_Report_Event_3014     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表事件清零事件
#define Init_Enable_Event_3015     0//事件有效标识bool
#define Init_Report_Event_3015     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表校时事件
#define Init_Enable_Event_3016     0//事件有效标识bool
#define Init_Report_Event_3016     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表时段表编程事件
#define Init_Enable_Event_3017     0//事件有效标识bool
#define Init_Report_Event_3017     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表时区表编程事件
#define Init_Enable_Event_3018     0//事件有效标识bool
#define Init_Report_Event_3018     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表周休日编程事件
#define Init_Enable_Event_3019     0//事件有效标识bool
#define Init_Report_Event_3019     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表结算日编程事件
#define Init_Enable_Event_301A     0//事件有效标识bool
#define Init_Report_Event_301A     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表开盖事件
#define Init_Enable_Event_301B     0//事件有效标识bool
#define Init_Report_Event_301B     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表开端钮盒事件
#define Init_Enable_Event_301C     0//事件有效标识bool
#define Init_Report_Event_301C     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电压不平衡事件
#define Init_Enable_Event_301D     0//事件有效标识bool
#define Init_Report_Event_301D     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电流不平衡事件
#define Init_Enable_Event_301E     0//事件有效标识bool
#define Init_Report_Event_301E     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表跳闸事件
#define Init_Enable_Event_301F     0//事件有效标识bool
#define Init_Report_Event_301F     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表合闸事件
#define Init_Enable_Event_3020     0//事件有效标识bool
#define Init_Report_Event_3020     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表节假日编程事件
#define Init_Enable_Event_3021     0//事件有效标识bool
#define Init_Report_Event_3021     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表有功组合方式编程事件
#define Init_Enable_Event_3022     0//事件有效标识bool
#define Init_Report_Event_3022     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表无功组合方式编程事件
#define Init_Enable_Event_3023     0//事件有效标识bool
#define Init_Report_Event_3023     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表费率参数表编程事件
#define Init_Enable_Event_3024     0//事件有效标识bool
#define Init_Report_Event_3024     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表阶梯表编程事件
#define Init_Enable_Event_3025     0//事件有效标识bool
#define Init_Report_Event_3025     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表密钥更新事件
#define Init_Enable_Event_3026     0//事件有效标识bool
#define Init_Report_Event_3026     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表异常插卡事件
#define Init_Enable_Event_3027     0//事件有效标识bool
#define Init_Report_Event_3027     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表购电记录
#define Init_Enable_Event_3028     0//事件有效标识bool
#define Init_Report_Event_3028     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表退费记录
#define Init_Enable_Event_3029     0//事件有效标识bool
#define Init_Report_Event_3029     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表恒定磁场干扰事件
#define Init_Enable_Event_302A     0//事件有效标识bool
#define Init_Report_Event_302A     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表负荷开关误动作事件
#define Init_Enable_Event_302B     0//事件有效标识bool
#define Init_Report_Event_302B     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电源异常事件
#define Init_Enable_Event_302C     0//事件有效标识bool
#define Init_Report_Event_302C     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表电流严重不平衡事件	
#define Init_Enable_Event_302D     0//事件有效标识bool
#define Init_Report_Event_302D     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表时钟故障事件
#define Init_Enable_Event_302E     0//事件有效标识bool
#define Init_Report_Event_302E     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表计量芯片故障事件
#define Init_Enable_Event_302F     0//事件有效标识bool
#define Init_Report_Event_302F     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//通信模块变更事件
#define Init_Enable_Event_3030     0//事件有效标识bool
#define Init_Report_Event_3030     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端初始化事件
#define Init_Enable_Event_3100     0//事件有效标识bool
#define Init_Report_Event_3100     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端版本变更事件
#define Init_Enable_Event_3101     0//事件有效标识bool
#define Init_Report_Event_3101     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端状态量变位事件
#define Init_Enable_Event_3104     0//事件有效标识bool
#define Init_Report_Event_3104     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表时钟超差事件
#define Init_Enable_Event_3105     0//事件有效标识bool
#define Init_Report_Event_3105     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端停/上电事件
#define Init_Enable_Event_3106     0//事件有效标识bool
#define Init_Report_Event_3106     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端直流模拟量越上限事件
#define Init_Enable_Event_3107     0//事件有效标识bool
#define Init_Report_Event_3107     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端直流模拟量越下限事件
#define Init_Enable_Event_3108     0//事件有效标识bool
#define Init_Report_Event_3108     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端消息认证错误事件
#define Init_Enable_Event_3109     0//事件有效标识bool
#define Init_Report_Event_3109     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//设备故障记录
#define Init_Enable_Event_310A     0//事件有效标识bool
#define Init_Report_Event_310A     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表示度下降事件
#define Init_Enable_Event_310B     0//事件有效标识bool
#define Init_Report_Event_310B     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能量超差事件
#define Init_Enable_Event_310C     0//事件有效标识bool
#define Init_Report_Event_310C     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表飞走事件
#define Init_Enable_Event_310D     0//事件有效标识bool
#define Init_Report_Event_310D     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表停走事件
#define Init_Enable_Event_310E     0//事件有效标识bool
#define Init_Report_Event_310E     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端抄表失败事件
#define Init_Enable_Event_310F     0//事件有效标识bool
#define Init_Report_Event_310F     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//月通信流量超限事件
#define Init_Enable_Event_3110     0//事件有效标识bool
#define Init_Report_Event_3110     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//发现未知电能表事件
#define Init_Enable_Event_3111     0//事件有效标识bool
#define Init_Report_Event_3111     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//跨台区电能表事件
#define Init_Enable_Event_3112     0//事件有效标识bool
#define Init_Report_Event_3112     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端对时事件
#define Init_Enable_Event_3114     0//事件有效标识bool
#define Init_Report_Event_3114     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//遥控跳闸记录
#define Init_Enable_Event_3115     0//事件有效标识bool
#define Init_Report_Event_3115     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//有功总电能量差动越限事件记录
#define Init_Enable_Event_3116     0//事件有效标识bool
#define Init_Report_Event_3116     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//输出回路接入状态变位事件记录
#define Init_Enable_Event_3117     0//事件有效标识bool
#define Init_Report_Event_3117     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端编程记录
#define Init_Enable_Event_3118     0//事件有效标识bool
#define Init_Report_Event_3118     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端电流回路异常事件
#define Init_Enable_Event_3119     0//事件有效标识bool
#define Init_Report_Event_3119     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表在网状态切换事件
#define Init_Enable_Event_311A     0//事件有效标识bool
#define Init_Report_Event_311A     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//终端对电表校时记录
#define Init_Enable_Event_311B     0//事件有效标识bool
#define Init_Report_Event_311B     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电能表数据变更监控记录
#define Init_Enable_Event_311C     0//事件有效标识bool
#define Init_Report_Event_311C     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//功控跳闸记录
#define Init_Enable_Event_3200     0//事件有效标识bool
#define Init_Report_Event_3200     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电控跳闸记录
#define Init_Enable_Event_3201     0//事件有效标识bool
#define Init_Report_Event_3201     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//购电参数设置记录
#define Init_Enable_Event_3202     0//事件有效标识bool
#define Init_Report_Event_3202     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）
//电控告警事件记录
#define Init_Enable_Event_3203     0//事件有效标识bool
#define Init_Report_Event_3203     0//不上报（0）,事件发生上报（1），事件恢复上报（2），事件发生恢复均上报（3）



//---事件类---
const u8 InitData_3000[]=//电能表失压事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3000,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3000,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3000_5[]=//电能表失压事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	4,
//{
//电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x06,0xB4,//78%参比电压
	((MeterUBhex*78/100)>>8)&0xff,((MeterUBhex*78+50)/100)&0xff,//78%参比电压
//  电压恢复下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x07,0x4E,//85%参比电压
	((MeterUBhex*85/100)>>8)&0xff,((MeterUBhex*85+50)/100)&0xff,//85%参比电压
//  电流触发下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//	0,0,0,75,//0.5%额定（基本）电流
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%额定（基本）电流
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3001[]=//电能表欠压事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3001,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3001,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3001_5[]=//电能表欠压事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x06,0xB4,//78%参比电压
	((MeterUBhex*78/100)>>8)&0xff,((MeterUBhex*78+50)/100)&0xff,//78%参比电压
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3002[]=//电能表过压事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3002,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3002,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3002_5[]=//电能表过压事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x0A,0x50,//120%参比电压
	((MeterUBhex*120/100)>>8)&0xff,((MeterUBhex*120+50)/100)&0xff,//120%参比电压
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3003[]=//电能表断相事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3003,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3003,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3003_5[]=//电能表断相事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	3,
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x05,0x28,//60%参比电压
	((MeterUBhex*60/100)>>8)&0xff,((MeterUBhex*60+50)/100)&0xff,//60%参比电压
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//	0,0,0,75,//0.5%额定（基本）电流
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%额定（基本）电流
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3004[]=//电能表失流事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3004,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3004,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3004_5[]=//电能表失流事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	4,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x06,0x04,//70%参比电压
	((MeterUBhex*70/100)>>8)&0xff,((MeterUBhex*70+50)/100)&0xff,//70%参比电压
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//0,0,0,75,//0.5%额定（基本）电流	
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%额定（基本）电流
//  电流恢复下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//	0,0,0x02,0xEE,//5%额定（基本）电流
	((MeterIBhex*500/100)>>24)&0xff,((MeterIBhex*500/100)>>16)&0xff,((MeterIBhex*500/100)>>8)&0xff,((MeterIBhex*500+50)/100)&0xff,//0.5%额定（基本）电流
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3005[]=//电能表过流事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3005,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3005,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200——事件发生时刻电压
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生时刻电流	
};
const u8 InitData_3005_5[]=//电能表过流事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  电流触发下限  double-long（单位：A，换算：-4），
	DataType_double_long,
//	0,1,0x19,0x40,//1.2Imax
	((MeterIMaxhex*120)>>24)&0xff,((MeterIMaxhex*120)>>16)&0xff,((MeterIMaxhex*120)>>8)&0xff,(MeterIMaxhex*120)&0xff,//1.2Imax

//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3006[]=//电能表断流事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3006,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3006,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3006_5[]=//电能表断流事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	3,
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
//	0x05,0x28,//60%参比电压
	((MeterUBhex*60/100)>>8)&0xff,((MeterUBhex*60+50)/100)&0xff,//60%参比电压
//  电流触发上限  double-long（单位：A，换算：-4），
	DataType_double_long,
//	0,0,0,75,//0.5%额定（基本）电流	
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%额定（基本）电流
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3007[]=//电能表功率反向事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3007,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3007,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_3007_5[]=//电能表功率反向事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
	DataType_double_long,
//	0,0,0,17,//0.5%单相基本功率
	((MeterIBhex*MeterUBhex/20000)>>24)&0xff,((MeterIBhex*MeterUBhex/20000)>>16)&0xff,((MeterIBhex*MeterUBhex/20000)>>8)&0xff,((MeterIBhex*MeterUBhex+10000)/20000)&0xff,//0.5%单相基本功率

//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3008[]=//电能表过载事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3008,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3008,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3008_5[]=//电能表过载事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
	DataType_double_long,
//	0,0,0x3D,0xE0,//1.2Imax和100%参比电压下的单相有功功率
	((MeterIMaxhex*MeterUBhex*12/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*12/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*12/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*12+500)/1000)&0xff,//1.2Imax和100%参比电压下的单相有功功率

//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3009[]=//电能表正向有功需量超限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3009,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3009,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3009_6[]=//电能表正向有功需量超限事件-5
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
	DataType_double_long_unsigned,
//	0,0,0xB9,0xA0,//1.2Imax和100%参比电压下的合相有功功率
	((MeterIMaxhex*MeterUBhex*36/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*36+500)/1000)&0xff,//1.2Imax和100%参比电压下的合相有功功率

//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300A[]=//电能表反向有功需量超限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300A,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300A,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_300A_6[]=//电能表反向有功需量超限事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
	DataType_double_long_unsigned,
//	0,0,0xB9,0xA0,//1.2Imax和100%参比电压下的合相有功功率
	((MeterIMaxhex*MeterUBhex*36/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*36+500)/1000)&0xff,//1.2Imax和100%参比电压下的合相有功功率
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300B[]=//电能表无功需量超限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300B,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300B,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_300B_5[]=//电能表无功需量超限事件-5
{
//属性5（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  触发下限 double-long-unsigned（单位：kvar，换算：-4），
	DataType_double_long_unsigned,
	0,0,0,0,//1.2Imax和100%参比电压下的合相无功功率
//  判定延时时间 unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300C[]=//电能表功率因数超下限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300C,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300C,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_300C_6[]=//电能表功率因数超下限事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  下限阀值  long（单位：%，换算：-1），
	DataType_long,
	1,0x2C,//0.3
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300D[]=//电能表全失压事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300D,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300D,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200——事件发生前电流
};
const u8 InitData_300D_6[]=//电能表全失压事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_300E[]=//电能表辅助电源掉电事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300E,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300E,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_300E_6[]=//电能表辅助电源掉电事件
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_300F[]=//电能表电压逆相序事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_300F,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_300F,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_300F_6[]=//电能表电压逆相序事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3010[]=//电能表电流逆相序事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3010,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3010,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3010_6[]=//电能表电流逆相序事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3011[]=//电能表掉电事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3011,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3011,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3011_6[]=//电能表掉电事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3012[]=//电能表编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3012,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3012,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3012_6[]=//电能表编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3013[]=//电能表清零事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3013,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3013,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x20,0x22,0x01,//00202201——事件发生前反向有功总电能	
};
const u8 InitData_3013_6[]=//电能表清零事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3014[]=//电能表需量清零事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3014,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3014,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3014_6[]=//电能表需量清零事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3015[]=//电能表事件清零事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3015,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3015,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3015_6[]=//电能表事件清零事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3016[]=//电能表校时事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3016,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3016,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x40,0x00,0x22,0x00,//40002200——事件发生前时间日期
	DataType_OAD,
	0x40,0x00,0x82,0x00,//40008200——事件结束后时间日期	
};
const u8 InitData_3016_6[]=//电能表校时事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3017[]=//电能表时段表编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3017,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3017,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3017_6[]=//电能表时段表编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3018[]=//电能表时区表编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3018,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3018,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3018_6[]=//电能表时区表编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3019[]=//电能表周休日编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3019,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3019,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3019_6[]=//电能表周休日编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301A[]=//电能表结算日编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301A,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301A,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_301A_6[]=//电能表结算日编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301B[]=//电能表开盖事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301B,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301B,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能
};
const u8 InitData_301B_6[]=//电能表开盖事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301C[]=//电能表开端钮盒事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301C,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301C,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_301C_6[]=//电能表开端钮盒事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301D[]=//电能表电压不平衡事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301D,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301D,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x20,0x26,0x62,0x00,//20266200——事件发生期间电压不平衡率
};
const u8 InitData_301D_6[]=//电能表电压不平衡事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  限值  long（单位：%，换算：-2），
	DataType_long,
	0x0B,0xB8,//30%
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_301E[]=//电能表电流不平衡事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301E,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301E,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_301E_6[]=//电能表电流不平衡事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  限值  long（单位：%，换算：-2），
	DataType_long,
	0x0B,0xB8,//30%
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_301F[]=//电能表跳闸事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_301F,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_301F,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_301F_6[]=//电能表跳闸事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3020[]=//电能表合闸事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3020,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3020,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3020_6[]=//电能表合闸事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3021[]=//电能表节假日编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3021,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3021,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3021_6[]=//电能表节假日编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3022[]=//电能表有功组合方式编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3022,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3022,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3022_6[]=//电能表有功组合方式编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3023[]=//电能表无功组合方式编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3023,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3023,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3023_6[]=//电能表无功组合方式编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3024[]=//电能表费率参数表编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3024,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3024,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3024_6[]=//电能表费率参数表编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3025[]=//电能表阶梯表编程事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3025,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3025,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3025_6[]=//电能表阶梯表编程事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3026[]=//电能表密钥更新事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3026,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3026,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3026_6[]=//电能表密钥更新事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3027[]=//电能表异常插卡事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3027,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3027,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3027_6[]=//电能表异常插卡事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3028[]=//电能表购电记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3028,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3028,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3028_6[]=//电能表购电记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3029[]=//电能表退费记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3029,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3029,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3029_6[]=//电能表退费记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_302A[]=//电能表恒定磁场干扰事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302A,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302A,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_302A_6[]=//电能表恒定磁场干扰事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302B[]=//电能表负荷开关误动作事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302B,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302B,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_302B_6[]=//电能表负荷开关误动作事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302C[]=//电能表电源异常事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302C,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302C,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能	
};
const u8 InitData_302C_6[]=//电能表电源异常事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302D[]=//电能表电流严重不平衡事件	
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302D,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302D,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_302D_6[]=//电能表电流严重不平衡事件	-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  限值  long（单位：%，换算：-2）
	DataType_long,
	0x23,0x28,//90%
//  判定延时时间  unsigned（单位：s，换算：0）
	DataType_unsigned,
	60,
//}
};

const u8 InitData_302E[]=//电能表时钟故障事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302E,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302E,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_302E_6[]=//电能表时钟故障事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302F[]=//电能表计量芯片故障事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_302F,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_302F,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_302F_6[]=//电能表计量芯片故障事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3030[]=//通信模块变更事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3030,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3030,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3030_6[]=//通信模块变更事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  判定延时  unsigned（单位：s，换算：0）
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3100[]=//终端初始化事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3100,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3100,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3100_6[]=//终端初始化事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3101[]=//终端版本变更事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3101,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3101,
//关联对象属性表∷=array OAD
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	DataType_array,
	2,
	DataType_OAD,
	0x43,0x00,0x22,0x02,//43002202——事件发生前软件版本号
	DataType_OAD,
	0x43,0x00,0x42,0x02,//43004202——事件发生后软件版本号
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	DataType_array,
	2,
	DataType_OAD,
	0x43,0x03,0x22,0x02,//43032202——事件发生前软件版本号
	DataType_OAD,
	0x43,0x03,0x42,0x02,//43034202——事件发生后软件版本号
#endif
#if ((Project/100)!=2)&&((Project/100)!=3)
	DataType_array,
	0,
#endif
};
const u8 InitData_3101_6[]=//终端版本变更事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3104[]=//终端状态量变位事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3104,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3104,
//关联对象属性表∷=array OAD
	DataType_array,
	5,
//201E4200——事件发生时间
	DataType_OAD,
	0x20,0x1e,0x42,0x00,
//F2034201——第1路状态量变位发生后状态（集中器）
	DataType_OAD,
	0xF2,0x03,0x42,0x01,
//F2034202——第2路状态量变位发生后状态
	DataType_OAD,
	0xF2,0x03,0x42,0x02,
//F2034203——第3路状态量变位发生后状态
	DataType_OAD,
	0xF2,0x03,0x42,0x03,
//F2034204——第4路状态量变位发生后状态
	DataType_OAD,
	0xF2,0x03,0x42,0x04,
};
const u8 InitData_3104_6[]=//终端状态量变位事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3105[]=//电能表时钟超差事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3105,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3105,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3105_6[]=//电能表时钟超差事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//异常判别阈值  long-unsigned（单位：秒），
	DataType_long_unsigned,
	300>>8,300&0xff,//300秒
//关联采集任务号 unsigned
	DataType_unsigned,
	28,
//}
};

const u8 InitData_3106[]=//终端停/上电事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3106,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3106,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3106_6[]=//终端停/上电事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//停电数据采集配置参数  structure
	DataType_structure,
	4,
//{
//采集标志  bit-string(SIZE(8))，
	DataType_bit_string,
	2,
	0xC0,
//停电事件抄读时间间隔（小时） unsigned ，
	DataType_unsigned,
	1,
//停电事件抄读时间限值（分钟） unsigned，
	DataType_unsigned,
	5,
//需要读取停电事件电能表       array TSA
	DataType_array,
	0,
//}，
//停电事件甄别限值参数  structure
	DataType_structure,
	6,
//{
//停电时间最小有效间隔（分钟） long-unsigned，
	DataType_long_unsigned,
	1>>8,1&0xff,
//停电时间最大有效间隔（分钟） long-unsigned，
	DataType_long_unsigned,
	4320>>8,4320&0xff,//4320
//停电事件起止时间偏差限值（分钟） long-unsigned，
	DataType_long_unsigned,
	5>>8,5&0xff,
//停电事件时间区段偏差限值（分钟） long-unsigned，
	DataType_long_unsigned,
	1>>8,1&0xff,
#if MeterUBhex==2200//参比电压xxx.x(V)
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	1320>>8,1320&0xff,//132.0
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	1760>>8,1760&0xff,//176.0
#endif
#if MeterUBhex==1000//参比电压xxx.x(V)
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	600>>8,600&0xff,//60.0
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	800>>8,800&0xff,//80.0
#endif
#if MeterUBhex==577//参比电压xxx.x(V)
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
	DataType_long_unsigned,
	346>>8,346&0xff,//34.6
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
	DataType_long_unsigned,
	462>>8,462&0xff,//46.2
#endif
//}
//}
//采集标志：
//bit0：置“1”有效，置“0”无效；
//bit1：置“1”随机选择测量点，置“0”只采集设置的测量点。
};


const u8 InitData_3107[]=//终端直流模拟量越上限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3107,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3107,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3107_6[]=//终端直流模拟量越上限事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  直流模拟量上限  double-long
	DataType_double_long,
	0,0,0,0,
//}
};

const u8 InitData_3108[]=//终端直流模拟量越下限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3108,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3108,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3108_6[]=//终端直流模拟量越下限事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  直流模拟量下限  double-long
	DataType_double_long,
	0,0,0,0,
//}
};

const u8 InitData_3109[]=//终端消息认证错误事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3109,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3109,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x44,0x01,0x22,0x01,//44012201 事件发生前安全认证密码
};
const u8 InitData_3109_6[]=//终端消息认证错误事件
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_310A[]=//设备故障记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310A,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310A,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_310A_6[]=//设备故障记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_310B[]=//电能表示度下降事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310B,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310B,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能
};
const u8 InitData_310B_6[]=//电能表示度下降事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  关联采集任务号 unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_310C[]=//电能量超差事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310C,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310C,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能
};
const u8 InitData_310C_6[]=//电能量超差事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
	DataType_double_long_unsigned,
	(400>>24)&0xff,(400>>16)&0xff,(400>>8)&0xff,400&0xff,//4.0
//  关联采集任务号 unsigned
	DataType_unsigned,
	0,
};

const u8 InitData_310D[]=//电能表飞走事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310D,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310D,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201——事件结束后正向有功总电能
};
const u8 InitData_310D_6[]=//电能表飞走事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
	DataType_double_long_unsigned,
	(600>>24)&0xff,(600>>16)&0xff,(600>>8)&0xff,600&0xff,//6.0
//  关联采集任务号 unsigned
	DataType_unsigned,
	0,
};

const u8 InitData_310E[]=//电能表停走事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310E,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310E,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201——事件发生前正向有功总电能
};
const u8 InitData_310E_6[]=//电能表停走事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  阈值           TI,
	DataType_TI,
	1,15>>8,15&0xff,
//  关联采集任务号 unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_310F[]=//终端抄表失败事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_310F,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_310F,
//关联对象属性表∷=array OAD
	DataType_array,
	3,
	DataType_OAD,
	0x60,0x41,0x22,0x00,//60412200—事件发生前最近一次抄表成功时间
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201—事件发生前最近一次抄表成功正向有功总电能示值
	DataType_OAD,
	0x00,0x30,0x22,0x01,//00302201—事件发生前最近一次抄表成功正向无功总电能示值
};
const u8 InitData_310F_6[]=//终端抄表失败事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	2,
//{
//  重试轮次      unsigned,
	DataType_unsigned,
	3,
//    关联采集任务号 unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3110[]=//月通信流量超限事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3110,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3110,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x22,0x00,0x42,0x02,//22004202-事件发生后当月已发生通信流量
	DataType_OAD,
	0x31,0x10,0x06,0x01,//31100601-月通信流量门限
};
const u8 InitData_3110_6[]=//月通信流量超限事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  通信流量门限  double-long-unsigned（单位：byte）
	DataType_double_long_unsigned,
	0,0,0,0,
//}
};

const u8 InitData_3111[]=//发现未知电能表事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3111,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3111,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3111_6[]=//发现未知电能表事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3112[]=//跨台区电能表事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3112,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3112,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3112_6[]=//跨台区电能表事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3114[]=//终端对时事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3114,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3114,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x40,0x00,0x22,0x00,//40002200—事件发生前对时时间
	DataType_OAD,
	0x40,0x00,0x82,0x00,//40008200—事件结束后对时时间
};
const u8 InitData_3114_6[]=//终端对时事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3115[]=//遥控跳闸记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3115,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3115,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3115_6[]=//遥控跳闸记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3116[]=//有功总电能量差动越限事件记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3116,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3116,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3116_6[]=//有功总电能量差动越限事件记录-6
{
//属性6（配置参数）∷=array 有功总电能量差动组配置
	DataType_array,
	0,
////有功总电能量差动组配置∷=structure
//	DataType_structure,
////{
////  有功总电能量差动组序号 unsigned，
//	DataType_unsigned,
////  对比的总加组           OI，
//	DataType_OI,
////  参照的总加组           OI，
//	DataType_OI,
////  参与差动的电能量的时间区间及对比方法标志 bit-string（SIZE（8）），
//	DataType_bit_string,
////  差动越限相对偏差值 integer（单位：%，换算：0），
//	DataType_integer,
////  差动越限绝对偏差值 long64（单位：kWh，换算：-4）
//	DataType_long64,
////}
//参与差动的电能量的时间区间及对比方法标志：
//bit0～bit1编码表示电能量的时间跨度，取值范围0～2依次表示60分钟电量、30分钟电量、15分钟电量，其他值无效。
//bit7表示对比方法标志，置“0”：相对对比，公式见公式（1）；置“1”：绝对对比，公式见公式（2）。
//bit2～bit6备用。
//	 ％  .....	(1)
//	  .............	(2)
//式中：
//    Q——对比的总加组总电能量；
//    q——参照的总加组总电能量。
};

const u8 InitData_3117[]=//输出回路接入状态变位事件记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3117,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3117,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3117_6[]=//输出回路接入状态变位事件记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3118[]=//终端编程记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3118,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3118,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3118_6[]=//终端编程记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3119[]=//终端电流回路异常事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3119,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3119,
//关联对象属性表∷=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x03,0x42,0x00,//20034200——事件发生时电压电流相角
	DataType_OAD,
	0x00,0x10,0x42,0x01,//00104201——事件发生时正向有功总电能
};
const u8 InitData_3119_6[]=//终端电流回路异常事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_311A[]=//电能表在网状态切换事件
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_311A,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_311A,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_311A_6[]=//电能表在网状态切换事件-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//   判定延时时间  long-unsigned（单位：s，换算：0）
	DataType_long_unsigned,
	0,0,
//}
};

const u8 InitData_311B[]=//终端对电表校时记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_311B,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_311B,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_311B_6[]=//终端对电表校时记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_311C[]=//电能表数据变更监控记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_311C,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_311C,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_311C_6[]=//电能表数据变更监控记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	1,
//{
//  关联采集任务序号    unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3200[]=//功控跳闸记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3200,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3200,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x23,0x01,0x23,0x00,//23012300——跳闸发生前总加有功功率 总加组1
};
const u8 InitData_3200_6[]=//功控跳闸记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3201[]=//电控跳闸记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3201,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3201,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x23,0x01,0x49,0x00,//23014900——跳闸发生时正向有功总加电能量	总加组1
};
const u8 InitData_3201_6[]=//电控跳闸记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3202[]=//购电参数设置记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3202,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3202,
//关联对象属性表∷=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x81,0x0c,0x22,0x01,//810C2201事件发生前购电控配置单元
};
const u8 InitData_3202_6[]=//购电参数设置记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3203[]=//电控告警事件记录
{
//有效标识∷=bool
	DataType_bool,
	Init_Enable_Event_3203,
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
	DataType_enum,
	Init_Report_Event_3203,
//关联对象属性表∷=array OAD
	DataType_array,
	0,
};
const u8 InitData_3203_6[]=//电控告警事件记录-6
{
//属性6（配置参数）∷=structure
	DataType_structure,
	0,
//{
//}
};




#endif




