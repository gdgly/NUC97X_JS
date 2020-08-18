
//菜单1 实时数据
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Display/Draw.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"



void Menu_1_5_1(void);

extern const u8 QUnitList[][4];//电量单位列表
extern const u8 QCUnitList[][7];//电量(费)单位列表
extern const u8 PurchaseWayList[][5];//购电方式列表

const u8 YCPList[][6]=//曲线比例列表
{
	"10:1",//10个点代表1kW
	"10:2",//10个点代表2kW
	"10:3",
	"10:4",
	"10:5",
	"10:6",
	"10:7",
	"10:8",
	"10:9",
	"10:10",
	"10:11",
	"10:12",
	"10:13",
	"10:14",
	"10:15",
	"10:16",
};

const u8 UnitList[][6]=//通用单位列表
{
	"kW",    //0
	"kvar",  //1
	"kWh",   //2
	"kvarh", //3
	"V",     //4
	"A",     //5
	"%",     //6
	"h",     //7
	"min",   //8
	"s",     //9
	"次",    //10
};

const u8 SwitchStateList[][3]=//开关状态列表
{
	"分",
	"合",
};

const u8 ShiftList[][7]=//变位信息列表
{
	"不变位",
	"变位",
};

const u8 SwitchNatureList[][5]=//开关属性列表
{
	"常闭",
	"常开",
};

const u8 SList[][2]=//正负极性列表
{
	" ",//+不显示
	"-",
};

const u8 pMenu_1[]=//专变
{
	"实时数据\n\r"
	"  1.当前功率\n\r"
	"  2.当前电量\n\r"
	"  3.负荷曲线\n\r"
	"  4.开关状态\n\r"
	"  5.事件记录\n\r"
};

const u8 pMenu4_1[]=//公变
{
	"实时数据\n\r"
	"  1.当前电量\n\r"
	"  2.负荷曲线\n\r"
	"  3.开关状态\n\r"
	"  4.事件记录\n\r"
};

const u8 pMenu_1_1[]=
{
	"当前功率\n\r"
	"  1.各脉冲功率\n\r"
	"  2.各总加组功率\n\r"
};

const u8 pMenu_1_1_1[]=
{
	"当前功率\n\r"
	"脉冲\n\r"
};

const u8 PulsePowerList[][8]=//脉冲功率列表
{
	"测量点:",
	"有功总:",
	"无功总:",
};

const u8 NPulseList[][20]=
{
	" 本脉冲未配置测量点",
};

const u8 pMenu_1_1_2[]=
{
	"当前功率\n\r"
	"总加组\n\r"
};

const u8 AddPowerList[][20]=//总加组功率列表
{
	"有功:",
	"无功:",
};

const u8 pMenu_1_2[]=
{
	"当前电量\n\r"
	"  1.当日正有功电量\n\r"
	"  2.当日正无功电量\n\r"
	"  3.当日反有功电量\n\r"
	"  4.当日反无功电量\n\r"
	"  5.当月正有功电量\n\r"
	"  6.当月正无功电量\n\r"
	"  7.当月反有功电量\n\r"
	"  8.当月反无功电量\n\r"
};

	#if ((Project/100)==3 && (USER/100)==5)//上海专变
const u8 EQList[][20]=//电量列表
{
	"总:",
	"峰:",
	"平:",
	"尖:",
	"谷:",
};
	#else
const u8 EQList[][20]=//电量列表
{
	"总:",
	"尖:",
	"峰:",
	"平:",
	"谷:",
};
	#endif

const u8 pMenu_1_2_1[]=
{
	"当日正向有功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_2[]=
{
	"当日正向无功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_3[]=
{
	"当日反向有功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_4[]=
{
	"当日反向无功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_5[]=
{
	"当月正向有功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_6[]=
{
	"当月正向无功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_7[]=
{
	"当月反向有功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_2_8[]=
{
	"当月反向无功电量\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_3[]=
{
	"负荷曲线\n\r"
	"  1.有功功率曲线\n\r"
	"  2.无功功率曲线\n\r"
};

const u8 pMenu_1_3_1[]=
{
	"有功功率曲线\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_3_2[]=
{
	"无功功率曲线\n\r"
	"测量点\n\r"
};

const u8 pMenu_1_4[]=
{
	"开关状态\n\r"
	"  1.开关状态\n\r"
	"  2.变位信息\n\r"
	"  3.开关属性\n\r"
};

const u8 pMenu_1_4_1[]=
{
	" \n\r"
	"开关状态:\n\r"
};

const u8 pMenu_1_4_2[]=
{
	" \n\r"
	"变位信息:\n\r"
};

const u8 pMenu_1_4_3[]=
{
	" \n\r"
	"开关属性:\n\r"
};

const u8 PowerControlList[][20]=//功控类别列表
{
	"时段控",
	"厂休控",
	"营业报停控",
	"功率下浮控",
};

const u8 EControlList[][20]=//电控类别列表
{
	"月电控",
	"购电控",
};


	#if ((Project/100)==3 && (USER/100)==5)//上海专变
const u8 ERC5List[][20]=//遥控记录列表
{
	"跳闸时间:",//0
	"",			//1
	"跳闸轮次:",//2
	"跳时功率:",//3
	"跳后功率:",//4
};
const u8 ERC6List[][20]=//功控记录列表
{
	"跳闸时间:",//0
	"",			//1
	"总加组号:",//2
	"跳闸轮次:",//3
	"功控类别:",//4
	"跳前功率:",//5
	"跳后功率:",//6
	"跳时定值:",//7
};
const u8 ERC7List[][20]=//电控记录列表
{
	"跳闸时间:",//0
	"",			//1
	"总加组号:",//2
	"跳闸轮次:",//3
	"电控类别:",//4
	"跳时电量:",//5
	"跳时定值:",//6
};
const u8 ERC14List[][20]=//失电记录列表
{
	"失电时间:",//0
	"",			//1
	"恢复时间:",//2
};
	#else
const u8 EventList[][20]=
{
	"ERC:",
	"重要事件:",
	"一般事件:",
};

const u8 LogoutList[][21]=//事件记录列表
{
	"数据初始化和版本变更", //ERC1
	"参数丢失",             //ERC2
	"参数变更",             //ERC3
	"状态量变位",           //ERC4
	"遥控跳闸",             //ERC5
	"功控跳闸",             //ERC6
	"电控跳闸",             //ERC7
	"电能表参数变更",       //ERC8
	"电流回路异常",         //ERC9
	"电压回路异常",         //ERC10
	"相序异常",             //ERC11
	"电能表时间超差",       //ERC12
	"电表故障信息",         //ERC13
	"终端停/上电",          //ERC14
	"谐波越限告警",         //ERC15
	"直流模拟量越限",       //ERC16
	"电压电流不平衡度越限", //ERC17
	"电容器投切自锁",       //ERC18
	"购电参数设置",         //ERC19
	"消息认证错误",         //ERC20
	"终端故障",             //ERC21
	"有功总电能量差动越限", //ERC22
	"电控告警",             //ERC23
	"电压越限",             //ERC24
	"电流越限",             //ERC25
	"视在功率越限",         //ERC26
	"电能表示度下降",       //ERC27
	"电能量超差",           //ERC28
	"电能表飞走",           //ERC29
	"电能表停走",           //ERC30
	"终端485抄表失败",      //ERC31
	"通信流量超门限",       //ERC32
	"电能表运行状态字变位", //ERC33
	"CT异常",               //ERC34
	"发现未知电表",         //ERC35
	"控制回路状态量变位",   //ERC36
	"电能表开表盖事件",     //ERC37
	"电能表开端钮盒事件",   //ERC38
	"补抄失败事件",         //ERC39
//	"从节点主动上报事件",   //ERC40
	"磁场异常事件",         //ERC40
	"对时事件",             //ERC41
};
const u8 ERC1[]=
{
	"数据初始化和版本变更\n\r"
	"  ERC1最近 ? 次\n\r"
	"变更时间:\n\r"
	"\n\r"
	"事件标志:\n\r"
	"\n\r"
	"变更前软件版本:\n\r"
	"变更后软件版本:\n\r"
};

const u8 ERC2[]=
{
	"参数丢失\n\r"
	"  ERC2最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"事件标志:\n\r"
};

const u8 ERC3[]=
{
	"参数变更\n\r"
	"  ERC3最近 ? 次\n\r"
	"变更时间:\n\r"
	"\n\r"
	"启动站地址:\n\r"
	"数据标识:\n\r"
};

const u8 ERC4[]=
{
	"状态量变位\n\r"
	"  ERC4最近 ? 次\n\r"
	"变位时间:\n\r"
	"\n\r"
	"状态变位:\n\r"
	"变后状态:\n\r"
};

const u8 ERC5[]=
{
	"遥控跳闸\n\r"
	"  ERC5最近 ? 次\n\r"
	"跳闸时间:\n\r"
	"\n\r"
	"跳闸轮次:\n\r"
	"跳时功率:\n\r"
	"跳后功率:\n\r"
};

const u8 ERC6[]=
{
	"功控跳闸\n\r"
	"  ERC6最近 ? 次\n\r"
	"跳闸时间:\n\r"
	"\n\r"
	"总加组号:\n\r"
	"跳闸轮次:\n\r"
	"功控类别:\n\r"
	"跳前功率:\n\r"
	"跳后功率:\n\r"
	"跳时定值:\n\r"
};

const u8 ERC7[]=
{
	"电控跳闸\n\r"
	"  ERC7最近 ? 次\n\r"
	"跳闸时间:\n\r"
	"\n\r"
	"总加组号:\n\r"
	"跳闸轮次:\n\r"
	"电控类别:\n\r"
	"跳时电量:\n\r"
	"跳时定值:\n\r"
};

const u8 ERC8[]=
{
	"电能表参数变更\n\r"
	"  ERC8最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"测量点号:\n\r"
	"变更标志: 费率时段？\n\r"
	"变、编程时间？变、抄\n\r"
	"表日？变、脉冲常数？\n\r"
	"变、互感器倍率？变、\n\r"
	"电能表最大需量？清零\n\r"
};

const u8 IUnusualList[][21]=//电流回路异常列表
{
	"短路",
	"开路",
	"反向",
};

const u8 ERC9[]=
{
	"电流回路异常\n\r"
	"  ERC9最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志: ---\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
	"Ｗ   :\n\r"//发生时电表正向有功总电能示值
};

const u8 ERC10[]=
{
	"电压回路异常\n\r"
	" ERC10最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志: ---\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
	"Ｗ   :\n\r"//发生时电表正向有功总电能示值
};

const u8 ERC11[]=
{
	"相序异常\n\r"
	" ERC11最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"∠U,Ia:\n\r"
	"∠U,Ib:\n\r"
	"∠U,Ic:\n\r"
	"Ｗ:\n\r"//发生时电表正向有功总电能示值
};

const u8 ERC12[]=
{
	"电能表时间超差\n\r"
	" ERC12最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
};

const u8 ERC13[]=
{
	"电表故障信息\n\r"
	" ERC13最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志: 编程次数或\n\r"
	"最大需量清零次数？变\n\r"
	"、断相次数？变、失压\n\r"
	"次数？变、停电次数？\n\r"
	"变、电池？欠压\n\r"
};

const u8 ERC14[]=
{
	"失电记录\n\r"
	" ERC14最近 ? 次\n\r"
	"失电时间:\n\r"
	"\n\r"
	"恢复时间:\n\r"
	"\n\r"
};

const u8 ERC15[]=
{
	"谐波越限告警\n\r"
	" ERC15最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志:     ---\n\r"
};

const u8 ERC16[]=
{
	"直流模拟量越限\n\r"
	" ERC16最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"越限标志:\n\r"
	"\n\r"
	"越限时直流模拟量数据\n\r"
};

const u8 ERC17_1[]=
{
	"电压/流不平衡越限\n\r"
	" ERC17最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志:\n\r"
	"\n\r"
	"\n\r"
	"发生时电压不平衡度:\n\r"
	"\n\r"
};

const u8 ERC17_2[]=
{
	"电压/流不平衡越限\n\r"
	" ERC17最近 ? 次\n\r"
	"发生时电流不平衡度:\n\r"
	"\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
};

const u8 ERC18[]=
{
	"电容器投切自锁\n\r"
	" ERC18最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志:\n\r"
};

const u8 ERC19_1[]=
{
	"购电参数设置\n\r"
	" ERC19最近 ? 次\n\r"
	"设置时间:\n\r"
	"\n\r"
	"总加组号:\n\r"
	"购电单号:\n\r"
	"追加刷新:\n\r"
	"购电量值:\n\r"
	"\n\r"
};

const u8 ERC19_2[]=
{
	"购电参数设置\n\r"
	" ERC19最近 ? 次\n\r"
	"报警门限:\n\r"
	"\n\r"
	"跳闸门限:\n\r"
	"\n\r"
	"购电前剩余电量(费):\n\r"
	"\n\r"
	"购电后剩余电量(费):\n\r"
};

const u8 ERC20[]=
{
	"消息认证错误\n\r"
	" ERC20最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"PW:\n\r"
	"\n\r"
	"MSA:\n\r"
};

const u8 TerminalBugList[][21]=//终端故障列表
{
	"终端主板内存",//故障1
	"时钟",        //故障2
	"主板通信",    //故障3
	"485抄表",     //故障4
	"显示板",      //故障5
	"载波通道异常",//故障6
};

const u8 ERC21[]=
{
	"终端故障\n\r"
	" ERC21最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"故障:\n\r"
};

const u8 ERC22[]=
{
	"有功总电能量差动越限\n\r"
	" ERC22最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"越限时总加有功电能量\n\r"
	"    对比:\n\r"
	"    参照:\n\r"
	"相对偏差:\n\r"
	"绝对偏差:\n\r"
};

const u8 ERC23[]=
{
	"电控告警\n\r"
	" ERC23最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"总加组号:\n\r"
	"投入轮次:\n\r"
	"电控类别:\n\r"
	"告警电量:\n\r"
	"告警定值:\n\r"
};

const u8 ERC24[]=
{
	"电压越限\n\r"
	" ERC24最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"越限标志: ---\n\r"
	"\n\r"
	"发生时Ua:\n\r"
	"发生时Ub:\n\r"
	"发生时Uc:\n\r"
};

const u8 ERC25[]=
{
	"电流越限\n\r"
	" ERC25最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"越限标志: ---\n\r"
	"\n\r"
	"发生时Ia:\n\r"
	"发生时Ib:\n\r"
	"发生时Ic:\n\r"
};

const u8 ERC26[]=
{
	"视在功率越限\n\r"
	" ERC26最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"越限标志:\n\r"
	"发生时功率:\n\r"
	"发生时限值:\n\r"
};

const u8 ERC27[]=
{
	"电能表示度下降\n\r"
	" ERC27最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"降前正有功总电能示值\n\r"
	"\n\r"
	"降后正有功总电能示值\n\r"
};

const u8 ERC28[]=
{
	"电能量超差\n\r"
	" ERC28最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"超差前正有功总示值\n\r"
	"\n\r"
	"超差后正有功总示值\n\r"
	"\n\r"
	"超差阈值:\n\r"
};

const u8 ERC29[]=
{
	"电能表飞走\n\r"
	" ERC29最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"飞前正有功总电能示值\n\r"
	"\n\r"
	"飞后正有功总电能示值\n\r"
	"\n\r"
	"飞走阈值:\n\r"
};

const u8 ERC30[]=
{
	"电能表停走\n\r"
	" ERC30最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"停时正有功总电能示值\n\r"
	"\n\r"
	"停走阈值:\n\r"
};

const u8 ERC31[]=
{
	"终端485抄表失败\n\r"
	" ERC31最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"最近一次抄表成功时间\n\r"
	"\n\r"
	"正向总电能示值:\n\r"
	"  有功:\n\r"
	"  无功:\n\r"
};

const u8 ERC32[]=
{
	"通信流量超门限\n\r"
	" ERC32最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"月通信流量:\n\r"
	"\n\r"
	"月流量门限:\n\r"
	"\n\r"
};

const u8 ERC33[]=
{
	"电能表运行状态字变位\n\r"
	" ERC33最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"测量点号:\n\r"
};

const u8 ERC34List[][21]=//ERC33通信流量超门限列表
{
	"一次侧短路",
	"二次侧短路", 
	"三次测短路",
};

const u8 ERC34[]=
{
	"CT异常\n\r"
	" ERC34最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"异常标志:\n\r"
};

const u8 ERC35[]=
{
	"发现未知电表\n\r"
	" ERC35最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"通信端口:\n\r"
	"发现块数:\n\r"
};

const u8 ERC36[]=
{
	"控制回路状态量变位\n\r"
	" ERC36最近 ? 次\n\r"
	"变位时间:\n\r"
	"\n\r"
	"状态变位:\n\r"
	"变后状态:\n\r"
};

const u8 ERC37[]=
{
	"电能表开表盖事件\n\r"
	" ERC37最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"测量点号:\n\r"
	"开盖总次数:\n\r"
};

const u8 ERC38[]=
{
	"电能表开端钮盒事件\n\r"
	" ERC38最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"测量点号:\n\r"
	"开盒总次数:\n\r"
};

const u8 ERC39[]=
{
	"补抄失败事件\n\r"
	" ERC39最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"最近一次抄表成功时间\n\r"
	"\n\r"
	"正向总电能示值:\n\r"
	"  有功:\n\r"
	"  无功:\n\r"
};

//const u8 ERC40[]=
//{
//	"从节点主动上报事件\n\r"
//	" ERC40最近 ? 次\n\r"
//	"发生时间:\n\r"
//	"\n\r"
//	"事件从节点地址:\n\r"
//	"\n\r"
//	"从节点类型:\n\r"
//};

//const u8 NodeTypeList[][15]=//从节点类型列表
//{
//	"终端外接传感器",
//	"采集器",
//	"电能表",
//};

const u8 ERC40[]=
{
	"磁场异常事件\n\r"
	" ERC40最近 ? 次\n\r"
	"发生时间:\n\r"
	"\n\r"
	"起止、Pn: ？？、?\n\r"
	"设备类型:\n\r"
	"\n\r"
	"设备地址:\n\r"
	"\n\r"
	"异常类型:\n\r"
};

const u8 DeviceTypeList[][15]=//设备类型列表
{
	"终端外接传感器",//00H
	"采集器",        //01H
	"电能表",        //02H
	"保留",          //03H-FFH
};

const u8 AbnormalTypeList[][9]=//磁场异常类型列表
{
	"高频磁场",//0
	"工频磁场",//1
	"强磁场",  //2
};


const u8 ERC41[]=
{
	"对时事件\n\r"
	" ERC41最近 ? 次\n\r"
	"测量点号:\n\r"
	"对前时间:\n\r"
	"\n\r"
	"对后时间:\n\r"
	"\n\r"
};
	#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_8
// Descriptions:        当月反向无功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_8(void)//菜单1_2_8
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_8);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当月反向无功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(48,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_7
// Descriptions:        当月反向有功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_7(void)//菜单1_2_7
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_7);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}

	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当月反向有功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(47,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_6
// Descriptions:        当月正向无功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_6(void)//菜单1_2_6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_6);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当月正向无功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(46,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_5
// Descriptions:        当月正向有功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_5(void)//菜单1_2_5
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_5);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}

	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当月正向有功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(45,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_4
// Descriptions:        当日反向无功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_4(void)//菜单1_2_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_4);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}

	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当日反向无功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(44,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_3
// Descriptions:        当日反向有功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_3(void)//菜单1_2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_3);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当日反向有功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(43,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_2
// Descriptions:        当日正向无功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_2(void)//菜单1_2_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_2);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当日正向无功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(42,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_1
// Descriptions:        当日正向有功电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1(void)//菜单1_2_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_1);//显示菜单
			if(MenuCount->v2>MaxRS485User)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485User;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485User);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxRS485User && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//当日正向有功电量 总尖峰平谷
		for(i=0;i<=4;i++)
		{
			y=MRR(GetClass1DataAddr(41,MenuCount->v2)+1+i*4,4);	   
			if(y==0xEEEEEEEE)
			{//无此数据填---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+4*2);
				DisplayData_OffZero(y,8,4,p16);
			}
			DisplayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效测量点号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2
// Descriptions:        当前电量
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_2(void)//菜单1_2
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_2_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_2_2);
						break;
					case 3:
						MenuCreate((u32)Menu_1_2_3);
						break;
					case 4:
						MenuCreate((u32)Menu_1_2_4);
						break;
					case 5:
						MenuCreate((u32)Menu_1_2_5);
						break;
					case 6:
						MenuCreate((u32)Menu_1_2_6);
						break;
					case 7:
						MenuCreate((u32)Menu_1_2_7);
						break;
					case 8:
						MenuCreate((u32)Menu_1_2_8);
						break;
				}        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}

#if ((Project/100)==3)//专变终端
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_2
// Descriptions:        当前各总加组功率
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_2(void)//菜单1_1_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1_2);//显示菜单
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2记录总加组号
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://任务1:选择
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+3);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)AddPowerList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
		//有功
		DisplayString(Ln+2,16,0,(u8 *)UnitList[0]);//0 kW
		y=MRR(GetClass1DataAddr(17,MenuCount->v2),2);	       
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Ln+2,6,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+(6*2));
			PowerP16(y,p16);
			DisplayString(Ln+2,5,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		}
		//无功
		DisplayString(Ln+3,16,0,(u8 *)UnitList[1]);//1 kvar
		y=MRR(GetClass1DataAddr(18,MenuCount->v2),2);	       
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Ln+2,6,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+(6*2));
			PowerP16(y,p16);
			DisplayString(Ln+3,5,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"无效总加组号");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_1
// Descriptions:        当前有功功率
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1_1(void)//菜单1_1_1
{
#if MaxImpInPort
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1_1);
			if(MenuCount->v2>MaxImpInPort)
			{//MenuCount->v2记录脉冲输入端口号
				MenuCount->v2=MaxImpInPort;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}		
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://任务1:选择
			NdigitSelect(2,9,1,MaxImpInPort);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+4);//清屏幕显示的指定行
	if(MenuCount->v2<=MaxImpInPort && MenuCount->v2>=1)
	{
		MenuCount->v3=MRR(ADDR_AFN04F11+5*(MenuCount->v2-1)+1,1);//脉冲端口所属测量点号
		if(MenuCount->v3>0 && MenuCount->v3<=MaxRS485AddCarrierPn)
		{
			for(i=0;i<3;i++)
			{
				DisplayString(Ln+2+i,0,0,(u8 *)PulsePowerList[i]);
			}
			//脉冲端口所属测量点号
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+8*2);
			DisplayData_OffZero(MenuCount->v3,4,0,p16);
			//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			//当前有功功率 总
			y=MRR(GetClass1DataAddr(25,MenuCount->v3)+5,3);	        
			DisplayString(Ln+3,16,0,(u8 *)UnitList[0]);//0 kW
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+3,8,0,"-------");	
			}
			else
			{
				DisplayString(Ln+3,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			}
			//当前无功功率 总
			y=MRR(GetClass1DataAddr(25,MenuCount->v3)+17,3);	    
			DisplayString(Ln+4,16,0,(u8 *)UnitList[1]);//1 kvar
			if(y==0xEEEEEE)
			{//无此数据填-------
				DisplayString(Ln+4,8,0,"-------");	
			}
			else
			{
				DisplayString(Ln+4,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);
			}
		}
		else
		{
			DisplayString(Ln+2,0,0,(u8 *)NPulseList[0]);
		}
	}
	else
	{
		DisplayString(Ln+2,5,0,"无效脉冲号");	
	}
#endif
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1
// Descriptions:        当前功率菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_1(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_1_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_1_2);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}
#endif

#if ((Project/100)==3 || (Project/100)==4)//专变终端、公变终端
	#if ((Project/100)==3 && (USER/100)==5)//上海专变
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3_1
// Descriptions:        功率曲线菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_3_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 ADDR;
	u32 i;
	u32 j;
	u32 l;
	u32 m;
	u32 k;
	u32 min;
	u32 hour;
	u32 max;
	u32 Dot;//当日曲线冻结的点数
	u32 xCP;//x坐标比例
	u32 yCP;//y坐标比例
	u64 y;
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u8 * p8d;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			DisplayClr();//清全屏
			Draw_Clr();
			Stack->MenuStack[MenuCount->Count].Task=1;
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 1:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_1);//显示菜单字符串
					ADDR=ADDR_AFN0DF81;
					break;
				case 2:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_2);//显示菜单字符串
					ADDR=ADDR_AFN0DF85;
					break;
			}
			DispalyAlign(0,0,2);//行显示对齐(1左对齐,2居中,3右对齐) 
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinColN(1,11,8);  
			
			Dot = (24*60/CurveCongealTime);
			if(Dot)
			{
				Draw_Clr();
				Draw_Line(16,96,144,96);//横轴
				Draw_Line(144,96,140,92);
				Draw_Line(144,96,140,100);
				Draw_Line(40,32,40,160);//纵轴
				Draw_Line(40,32,36,36);
				Draw_Line(40,32,44,36);
				DisplayString(6,4,0,"0");
				xCP=1;
				DisplayString(5,17,0,"1:1");

				p16=(u16 *)ADDR_DATABUFF;
				for(j=0;j<Dot;j++)
				{
					p16[j]=0xEEEE;
				}
				//2类数据清0时仅将计数值清为0;查找2类数据是否有，在计数值内查，当计数标志为满时才在全部内查。
				m=MRR(ADDR_TCongealCount+7,2);//曲线计数
				if(m&0x8000)
				{
					l=MaxCurveCongeal;
				}
				else
				{
					l=(m&0x7fff);
				}
				if(m)
				{
					for(i=0;i<l;i++)
					{
						if(MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+2,3) == MRR(ADDR_TYMDHMS+3,3))
						{//冻结时标与当前日期相同
							min = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i),1);//时标 分
							hour = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+1,1);//时标 时
							if(((min>>4)*10+(min&0xf))%CurveCongealTime == 0)
							{
								j = ((hour>>4)*10+(hour&0xf))*(60/CurveCongealTime) + ((min>>4)*10+(min&0xf))/CurveCongealTime;
								y = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR+3*(MenuCount->v2-1),3);
								for(k=0;k<5;k++)
								{
									if(((y>>(k*4))&0x0f) > 9)
									{
										break;
									}
								}
								if(k==5)
								{
									p16[j] = ((y>>20)&0x7)*100+((y>>16)&0xf)*10+((y>>12)&0xf)*1+((y>>8)&0x8000);//最高位为符号位
								}
							}
						}
					}
					max=0;
					for(i=0;i<Dot;i++)
					{
						if((p16[i]!=0xEEEE) && ((p16[i]&0x7fff)>max))
						{
							max=(p16[i]&0x7fff);
						}
					}
	
					yCP = (max/60)+1;
					DisplayString(2,0,0,(u8 *)YCPList[max/60]);//y轴比例
					for(i=0;i<Dot;i++)
					{
						if(p16[i]>>15)
						{//负数
							if(p16[i]!=0xEEEE)
							{
								p16[i]=96+(p16[i]&0x7fff)/yCP;
							}
						}
						else
						{
							p16[i]=96-(p16[i]&0x7fff)/yCP;
						}
					}
					if(Dot==1)
					{
						if(p16[0]!=0xEEEE)
						{
							Draw_Line(40+0*xCP,p16[0],40+0*xCP,p16[0]);
						}
					}
					else
					{
						for(i=0;i<Dot-1;i++)
						{
							if((p16[i]!=0xEEEE) && (p16[i+1]!=0xEEEE))
							{
								Draw_Line(40+i*xCP,p16[i],40+(i+1)*xCP,p16[i+1]);
							}
							else
							{
								if((p16[i]!=0xEEEE) && (p16[i+1]==0xEEEE))
								{
									Draw_Line(40+i*xCP,p16[i],40+i*xCP,p16[i]);
								}
								if((p16[i]==0xEEEE) && (p16[i+1]!=0xEEEE))
								{
									Draw_Line(40+(i+1)*xCP,p16[i+1],40+(i+1)*xCP,p16[i+1]);
								}
							}
						}
					}
				}
			}
			break;
		case 1://任务1:选择
			NdigitSelect_SH(1,4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
#if ((Project/100)==3 && (USER/100)==5)//上海专变
				p8d=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
				p8d[0]=1;//菜单显示顶、底行用，0=否，1=是
#endif
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3_1
// Descriptions:        功率曲线菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_3_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 ADDR;
	u32 i;
	u32 j;
	u32 l;
	u32 m;
	u32 k;
	u32 min;
	u32 hour;
	u32 max;
	u32 Dot;//当日曲线冻结的点数
	u32 xCP;//x坐标比例
	u32 yCP;//y坐标比例
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			DisplayClr();//清全屏
			Draw_Clr();
			Stack->MenuStack[MenuCount->Count].Task=1;
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 1:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_1);//显示菜单字符串
					ADDR=ADDR_AFN0DF81;
					break;
				case 2:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_2);//显示菜单字符串
					ADDR=ADDR_AFN0DF85;
					break;
			}
			DispalyAlign(0,0,2);//行显示对齐(1左对齐,2居中,3右对齐) 
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinColN(Stack->MenuStack[MenuCount->Count].EditRow,11,8);  
			
			Dot = (24*60/CurveCongealTime);
			if(Dot)
			{
				Draw_Clr();
				Draw_Line(16,96,144,96);//横轴
				Draw_Line(144,96,140,92);
				Draw_Line(144,96,140,100);
				Draw_Line(40,32,40,160);//纵轴
				Draw_Line(40,32,36,36);
				Draw_Line(40,32,44,36);
				DisplayString(6,4,0,"0");
				xCP=1;
				DisplayString(5,17,0,"1:1");

				p16=(u16 *)ADDR_DATABUFF;
				for(j=0;j<Dot;j++)
				{
					p16[j]=0xEEEE;
				}
				//2类数据清0时仅将计数值清为0;查找2类数据是否有，在计数值内查，当计数标志为满时才在全部内查。
				m=MRR(ADDR_TCongealCount+7,2);//曲线计数
				if(m&0x8000)
				{
					l=MaxCurveCongeal;
				}
				else
				{
					l=(m&0x7fff);
				}
				if(m)
				{
					for(i=0;i<l;i++)
					{
						if(MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+2,3) == MRR(ADDR_TYMDHMS+3,3))
						{//冻结时标与当前日期相同
							min = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i),1);//时标 分
							hour = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+1,1);//时标 时
							if(((min>>4)*10+(min&0xf))%CurveCongealTime == 0)
							{
								j = ((hour>>4)*10+(hour&0xf))*(60/CurveCongealTime) + ((min>>4)*10+(min&0xf))/CurveCongealTime;
								y = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR+3*(MenuCount->v2-1),3);
								for(k=0;k<5;k++)
								{
									if(((y>>(k*4))&0x0f) > 9)
									{
										break;
									}
								}
								if(k==5)
								{
									p16[j] = ((y>>20)&0x7)*100+((y>>16)&0xf)*10+((y>>12)&0xf)*1+((y>>8)&0x8000);//最高位为符号位
								}
							}
						}
					}
					max=0;
					for(i=0;i<Dot;i++)
					{
						if((p16[i]!=0xEEEE) && ((p16[i]&0x7fff)>max))
						{
							max=(p16[i]&0x7fff);
						}
					}
	
					yCP = (max/60)+1;
					DisplayString(2,0,0,(u8 *)YCPList[max/60]);//y轴比例
					for(i=0;i<Dot;i++)
					{
						if(p16[i]>>15)
						{//负数
							if(p16[i]!=0xEEEE)
							{
								p16[i]=96+(p16[i]&0x7fff)/yCP;
							}
						}
						else
						{
							p16[i]=96-(p16[i]&0x7fff)/yCP;
						}
					}
					if(Dot==1)
					{
						if(p16[0]!=0xEEEE)
						{
							Draw_Line(40+0*xCP,p16[0],40+0*xCP,p16[0]);
						}
					}
					else
					{
						for(i=0;i<Dot-1;i++)
						{
							if((p16[i]!=0xEEEE) && (p16[i+1]!=0xEEEE))
							{
								Draw_Line(40+i*xCP,p16[i],40+(i+1)*xCP,p16[i+1]);
							}
							else
							{
								if((p16[i]!=0xEEEE) && (p16[i+1]==0xEEEE))
								{
									Draw_Line(40+i*xCP,p16[i],40+i*xCP,p16[i]);
								}
								if((p16[i]==0xEEEE) && (p16[i+1]!=0xEEEE))
								{
									Draw_Line(40+(i+1)*xCP,p16[i+1],40+(i+1)*xCP,p16[i+1]);
								}
							}
						}
					}
				}
			}
			break;
		case 1://任务1:选择
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
}
	#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3
// Descriptions:        负荷曲线菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_3(void)
{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u8 * p8;
	p8=(u8 *)ADDR_MenuICO;//菜单显示顶、底行用，0=否，1=是
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_3);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
					case 2:
#if ((Project/100)==3 && (USER/100)==5)//上海专变
						p8[0]=0;//菜单显示顶、底行用，0=否，1=是
#endif
						MenuCreate((u32)Menu_1_3_1);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_3
// Descriptions:        开关属性
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_4_3(void)
{
#if (MaxSwitchInPort)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<MaxSwitchInPort;i++)
	{
		j|=1<<i;
	}

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_3);//显示菜单
	x=MRR(ADDR_AFN04F12,1);//状态量接入标志位
#if ((Project/100)==3)//专变终端
	j&=(~(1<<(MaxSwitchInPort-1)));
	j|=1<<7;
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN04F12+1,1);//开关属性                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort-1;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchNatureList[(y>>i)&0x01]);
				Row++;
			}
		}                            	 
		//门禁
		if((x>>7)&0x01)
		{
			DisplayString(Row,2,0,"门禁 :");
			DisplayString(Row,9,0,(u8 *)SwitchNatureList[(y>>7)&0x01]);
		}                            	 
	}
#endif
#if (((Project/100)==4) || ((Project/100)==2))//公变终端、集中器
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN04F12+1,1);//开关属性                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchNatureList[(y>>i)&0x01]);
				Row++;
			}
		}                            	 
	}
#endif
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_2
// Descriptions:        变位信息
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_4_2(void)
{
#if (MaxSwitchInPort)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<MaxSwitchInPort;i++)
	{
		j|=1<<i;
	}

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_2);//显示菜单
	x=MRR(ADDR_AFN04F12,1);//状态量接入标志位
#if ((Project/100)==3)//专变终端
	j&=(~(1<<(MaxSwitchInPort-1)));
	j|=1<<7;
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN0CF9+1,1);//变位信息                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort-1;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)ShiftList[(y>>i)&0x01]);
				Row++;
			}
		}                            	 
		//门禁
		if((x>>7)&0x01)
		{
			DisplayString(Row,2,0,"门禁 :");
			DisplayString(Row,9,0,(u8 *)ShiftList[(y>>7)&0x01]);
		}                            	 
	}
#endif
#if (((Project/100)==4) || ((Project/100)==2))//公变终端、集中器
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN0CF9+1,1);//变位信息                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)ShiftList[(y>>i)&0x01]);
				Row++;
			}
		}                            	 
	}
#endif
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_1
// Descriptions:        开关状态
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_4_1(void)
{
#if (MaxSwitchInPort)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//上海专变
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<MaxSwitchInPort;i++)
	{
		j|=1<<i;
	}

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_1);//显示菜单
	x=MRR(ADDR_AFN04F12,1);//状态量接入标志位
#if ((Project/100)==3)//专变终端
	j&=(~(1<<(MaxSwitchInPort-1)));
	j|=1<<7;
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN0CF9,1);//开关状态                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort-1;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchStateList[(y>>i)&0x01]);
				Row++;
			}
		}
		//门禁
		if((x>>7)&0x01)
		{
			DisplayString(Row,2,0,"门禁 :");
			DisplayString(Row,9,0,(u8 *)SwitchStateList[(y>>7)&0x01]);
		}                            	 
	}
#endif
#if (((Project/100)==4) || ((Project/100)==2))//公变终端、集中器
	if((x&j)==0)
	{//状态量都没接入
		DisplayString(Row,2,0,"无状态量接入");
	}
	else
	{
		y=MRR(ADDR_AFN0CF9,1);//开关状态                            	 
		//遥信  
		for(i=0;i<MaxSwitchInPort;i++)
		{
			if((x>>i)&0x01)
			{
				DisplayString(Row,2,0,"遥信 :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchStateList[(y>>i)&0x01]);
				Row++;
			}
		}
	}
#endif
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4
// Descriptions:        开关状态菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4);//显示菜单
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_4_2);
						break;
					case 3:
						MenuCreate((u32)Menu_1_4_3);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}

#if (((Project/100)==3 && (USER/100)!=5) || (Project/100)==4)//非上海专变终端、公变终端
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5_1_1
// Descriptions:        事件记录内容(第2屏)
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_5_1_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 x;
	u32 ADDR;
	u64 z;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>((MenuCount->v3)>>8))
			{
				MenuCount->v2=1;
			}
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 2:
					x = Event(1,(MenuCount->v3)&0xff,MenuCount->v2);//m=1重要事件，m=0一般事件；事件记录代码ERC；最近n次 
					ADDR = ADDR_AFN0EF1;
					break;
				case 3:
					x = Event(0,(MenuCount->v3)&0xff,MenuCount->v2);//m=1重要事件，m=0一般事件；事件记录代码ERC；最近n次
					ADDR = ADDR_AFN0EF2;
					break;
			}
			if(x)
			{
				switch((MenuCount->v3)&0xff)
				{
					case 17:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC17_2);//显示菜单
						UpArrow();
						//发生时电流不平衡度
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,2);
						if(y==0xEEEE)
						{//无此数据填-----
							DisplayString(3,6,0,"-----");	
						}
						else
						{
							DisplayString(3,6,0,(u8 *)SList[y>>15]);//+ -
							y&=0x7fff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+7*2);
							DisplayData_OffZero(y,4,1,p16);
						}
						DisplayString(3,14,0,(u8 *)UnitList[6]);//6 %
						//发生时电压电流ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14+2*i,2);   
							if(y==0xEEEE)
							{//无此数据填-----
								DisplayString(4+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+20+3*i,3);   
							if(y==0xEEEEEE)
							{//无此数据填------
								DisplayString(4+i,12,0,"------");	
							}
							else
							{
								DisplayString(4+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						break;
					case 19:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC19_2);//显示菜单
						UpArrow();
						//报警门限、跳闸门限、剩余电量费
						for(i=0;i<=3;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+17+4*i,4);
							if(y==0xEEEEEEEE)
							{//无此数据填-------
								DisplayString(3+2*i,3,0,"-------");	
							}
							else
							{
								z=y&0xfffffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2*i)*84+3*2);
								p16=DisplayData_OffZero(z,7,0,p16);
								DisplayString(3+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
								for(j=4;j<20;j++)
								{
									if(p16 == (u16 *)(ADDR_STRINGBUFF+(3+2*i)*84+4+j*2))
									{
										break;
									}
								}
								DisplayString(3+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
							}
							DispalyAlign(3+2*i,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
						}
						break;
				}
				//最近第 ? 次
				p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
				bcd_p8(p8,MenuCount->v2,3);
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
				p16[0]=0x2020;
				p16[1]=0x2020;
				p16[2]=32;//'左右箭头'
				p16[3]=33;
			}
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,12,10);
			break;
		case 1://任务1:选择
			NdigitSelect(3,10,1,(MenuCount->v3)>>8);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuCount->v2 = ((MenuCount->v3)&0xff);
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			switch((MenuCount->v3)&0xff)
			{
				case 17:
				case 19:
					if(Stack->MenuStack[MenuCount->Count].EditCol==0)
					{
						if(Stack->MenuStack[MenuCount->Count].EditCol==0)
						{
							if((Comm_Ram->ButtonVal&0x02)!=0)
							{//上键
								CreateScreen((u32)Menu_1_5_1);
							}
							Comm_Ram->ButtonVal&=~0x02;//键使用后清0
						}
					}
					break;
			}
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5_1
// Descriptions:        事件记录内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_5_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u16 *pa16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 ADDR;
	u64 z;
	u64 y;
	u32 Ln;
	u32 Col;
	u32 Fn;
	u32 pn;
	u32 m1;
	u32 m2;
	u32 n;
	u32 m;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>((MenuCount->v3)>>8))
			{
				MenuCount->v2=1;
			}
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 2:
					x = Event(1,(MenuCount->v3)&0xff,MenuCount->v2);//m=1重要事件，m=0一般事件；事件记录代码ERC；最近n次 
					ADDR = ADDR_AFN0EF1;
					break;
				case 3:
					x = Event(0,(MenuCount->v3)&0xff,MenuCount->v2);//m=1重要事件，m=0一般事件；事件记录代码ERC；最近n次
					ADDR = ADDR_AFN0EF2;
					break;
			}
			if(x)
			{
				switch((MenuCount->v3)&0xff)
				{
					case 1:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC1);//显示菜单
						//参数变更时间

						//事件标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y&0x1)
						{
							DisplayString(4,10,0,"初始化");
						}
						else
						{
							DisplayString(4,10,0,"未初始化");
						}
						if(y&0x2)
						{
							DisplayString(5,10,0,"版本变更");
						}
						else
						{
							DisplayString(5,10,0,"版本未变更");
						}
						//变更前、后软件版本号
						p8=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,8);
						p=(u8 *)(ADDR_STRINGBUFF+6*84+4+16*2);
						for(i=0;i<=3;i++)
						{
							p[i*2]=p8[i];
							p[i*2+1]=0x20;
						}
						p=(u8 *)(ADDR_STRINGBUFF+7*84+4+16*2);
						for(i=0;i<=3;i++)
						{
							p[i*2]=p8[4+i];
							p[i*2+1]=0x20;
						}
						break;
					case 2:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC2);//显示菜单
						//发生时间

						//事件标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y&0x1)
						{
							DisplayString(5,4,0,"终端参数丢失");
						}
						else
						{
							DisplayString(5,4,0,"终端参数未丢失");
						}
						if(y&0x2)
						{
							DisplayString(6,4,0,"测量点参数丢失");
						}
						else
						{
							DisplayString(6,4,0,"测量点参数未丢失");
						}
						break;
					case 3:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC3);//显示菜单
						//参数变更时间

						//启动站地址
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+12*2);
						DisplayData_OffZero(y,3,0,p16);
						//变更参数数据单元标识
						k=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+1,1);//长度
						p8=(u8 *)(ADDR_DATABUFF);
						k=((k-6)/4);
						Ln=5;
						Col=9;
						for(j=0;j<k;j++)
						{
							MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+8+4*j,4);
							for(i=0;i<8;i++)
							{
								//Fnpn
								Fn=DT1DT2_Fn(p8[2],p8[3],i);
								if(Fn)
								{
									if(p8[0]==0 && p8[1]==0)
									{
										pn=0;
										n=1;
										for(m1=0;m1<4;m1++)
										{
											n*=10;
											if((Fn/n)==0)
											{
												break;
											}
										}
										n=1;
										for(m2=0;m2<4;m2++)
										{
											n*=10;
											if((pn/n)==0)
											{
												break;
											}
										}
										if(Col>=20)
										{
											Ln++;
											Col=1;
										}
										else
										{
											if((19-Col)<(m1+m2+2))
											{
												Ln++;
												Col=1;
											}
											else
											{
												Col++;
											}
										}
										p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Col-1)*2);
										p16[0]=0x2000+'F';
										p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+Col*2);
										pa16=DisplayData_OffZero(hex_bcd(Fn&0xff),3,0,p16);
										pa16[0]=0x2000+'p';
										pa16=DisplayData_OffZero(hex_bcd(pn),3,0,pa16+1);
										Col+=(pa16-p16+1);
									}
									else
									{
										for(m=0;m<8;m++)
										{
											pn=DA1DA2_pn(p8[0],p8[1],m);
											if(pn)
											{
												n=1;
												for(m1=0;m1<4;m1++)
												{
													n*=10;
													if((Fn/n)==0)
													{
														break;
													}
												}
												n=1;
												for(m2=0;m2<4;m2++)
												{
													n*=10;
													if((pn/n)==0)
													{
														break;
													}
												}
												if(Col>=20)
												{
													Ln++;
													Col=1;
												}
												else
												{
													if((19-Col)<(m1+m2+2))
													{
														Ln++;
														Col=1;
													}
													else
													{
														Col++;
													}
												}
												p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Col-1)*2);
												p16[0]=0x2000+'F';
												p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+Col*2);
												pa16=DisplayData_OffZero(hex_bcd(Fn&0xff),3,0,p16);
												pa16[0]=0x2000+'p';
												pa16=DisplayData_OffZero(hex_bcd(pn),3,0,pa16+1);
												Col+=(pa16-p16+1);
											}
										}
									}
								}
							}
							if(Ln==9 && Col>16)
							{
								DisplayString(9,17,0,"...");
								break;
							}
						}
						break;
					case 4:
					case 36://ERC4、ERC36记录的内容数据格式一致
						if(((MenuCount->v3)&0xff)==4)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC4);//显示菜单
						}
						if(((MenuCount->v3)&0xff)==36)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC36);//显示菜单
						}
						//变位时间

						//状态变位
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//发生变位
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//变位后状态
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if(((y>>i)&0x01)==1)
							{
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						break;
					case 5:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC5);//显示菜单
						//跳闸时间

						//跳闸轮次
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//受控
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//跳闸时功率
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(5,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(5,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(5,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//跳闸后2分钟的功率
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(6,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(6,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(6,i,0,(u8 *)UnitList[0]);//0 kW
						}
						break;
					case 6:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC6);//显示菜单
						//跳闸时间

						//总加组号
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//跳闸轮次
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//受控
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//功控类别
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<4;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)PowerControlList[i]);
							}
						}
						//跳闸前功率
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(7,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(7,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//跳闸后2分钟的功率
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(8,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//跳闸时功率定值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(9,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(9*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(9,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+9*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(9,i,0,(u8 *)UnitList[0]);//0 kW
						}
						break;
					case 7:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC7);//显示菜单
						//跳闸时间

						//总加组号
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//跳闸轮次
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//受控
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//电控类别
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<2;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)EControlList[i]);
							}
						}
						//跳闸时电量
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(7,10,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
						}
						//跳闸时定值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
						}
						break;
					case 8:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC8);//显示菜单
						//发生时间

						//测量点号
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//变更标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if((y>>0)&0x01)
						{//电能表费率时段
							DisplayString(5,18,0,"已");
						}
						else
						{
							DisplayString(5,18,0,"未");
						}
						if((y>>1)&0x01)
						{//电能表编程时间
							DisplayString(6,12,0,"已");
						}
						else
						{
							DisplayString(6,12,0,"未");
						}
						if((y>>2)&0x01)
						{//电能表抄表日
							DisplayString(7,4,0,"已");
						}
						else
						{
							DisplayString(7,4,0,"未");
						}
						if((y>>3)&0x01)
						{//电能表脉冲常数
							DisplayString(7,18,0,"已");
						}
						else
						{
							DisplayString(7,18,0,"未");
						}
						if((y>>4)&0x01)
						{//电能表互感器倍率
							DisplayString(8,14,0,"已");
						}
						else
						{
							DisplayString(8,14,0,"未");
						}
						if((y>>5)&0x01)
						{//电能表最大需量清零
							DisplayString(9,14,0,"已");
						}
						else
						{
							DisplayString(9,14,0,"未");
						}
						break;
					case 9:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC9);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if((y>>i)&0x01)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						DisplayString(5,15,0,(u8 *)IUnusualList[((y>>6)&0x3)-1]);
						//发生时电压电流ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);   
							if(y==0xEEEE)
							{//无此数据填-----
								DisplayString(6+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16+3*i,3);   
							if(y==0xEEEEEE)
							{//无此数据填------
								DisplayString(6+i,12,0,"------");	
							}
							else
							{
								DisplayString(6+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						//发生时正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+25,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(9,6,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+6*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 10:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC10);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if((y>>i)&0x01)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(5,15,0,"断相");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(5,15,0,"失压");
						}
						//发生时电压电流ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);   
							if(y==0xEEEE)
							{//无此数据填-----
								DisplayString(6+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16+3*i,3);   
							if(y==0xEEEEEE)
							{//无此数据填------
								DisplayString(6+i,12,0,"------");	
							}
							else
							{
								DisplayString(6+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						//发生时正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+25,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(9,6,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+6*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 11:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC11);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//角度
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9+2*i,2);
							if(y==0xEEEE)
							{
								DisplayString(5+i,7,0,"-----,");
							}	       
							else
							{
								DisplayString(5+i,7,0,(u8 *)SList[y>>15]);//+ -
								y&=0x7fff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(5+i)*84+8*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+15+2*i,2);	       
							if(y==0xEEEE)
							{
								DisplayString(5+i,14,0,"-----");
							}	       
							else
							{
								DisplayString(5+i,14,0,(u8 *)SList[y>>15]);//+ -
								y&=0x7fff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(5+i)*84+15*2);
								DisplayData_OffZero(y,4,1,p16);
							}
						}
						//发生时正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+21,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 12:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC12);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						break;
					case 13:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC13);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if((y>>0)&0x01)
						{//编程次数或最大需量清零次数
							DisplayString(6,16,0,"已");
						}
						else
						{
							DisplayString(6,16,0,"未");
						}
						if((y>>1)&0x01)
						{//断相次数
							DisplayString(7,10,0,"已");
						}
						else
						{
							DisplayString(7,10,0,"未");
						}
						if((y>>2)&0x01)
						{//失压次数
							DisplayString(8,4,0,"已");
						}
						else
						{
							DisplayString(8,4,0,"未");
						}
						if((y>>3)&0x01)
						{//停电次数
							DisplayString(8,18,0,"已");
						}
						else
						{
							DisplayString(8,18,0,"未");
						}
						if((y>>4)&0x01)
						{//电池欠压
							DisplayString(9,8,0,"已");
						}
						else
						{
							DisplayString(9,8,0,"未");
						}
						break;
					case 14:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC14);//显示菜单
//#define NewERC14	0//ERC14停电事件记录方法0=原标准，1=国网营销部2014.1.15通知
						//失电时间
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2+NewERC14,5);
						p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						//恢复时间
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7+NewERC14,5);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						break;
					case 15://公变、集中器的功能
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC15);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+14*2);
						for(i=0;i<3;i++)
						{
							if((y>>i)&0x01)
							{
								p16[i]=i+0x2041;
							}
						}
						if(y>>7)
						{//电流
							DisplayString(5,10,0,"电流");
						}
						else
						{//电压
							DisplayString(5,10,0,"电压");
						}
						break;
					case 16://公变、集中器的功能
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC16);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>7)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x3f);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//越限标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						if(y&0x1)
						{
							DisplayString(5,10,0,"越上限");
						}
						else
						{
							DisplayString(5,10,0,"未越上限");
						}
						if(y&0x2)
						{
							DisplayString(6,10,0,"越下限");
						}
						else
						{
							DisplayString(6,10,0,"未越下限");
						}
						//越限时直流模拟量数据
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,2);
						if(y==0xEEEE)
						{//无此数据填-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+(11*2));
							PowerP16(y,p16);
							DisplayString(8,10,0,(u8 *)SList[(y>>12)&0x01]);//+ -
						}
						break;
					case 17:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC17_1);//显示菜单
						DownArrow();
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y&0x1)
						{
							DisplayString(6,2,0,"电压不平衡度越限");
						}
						else
						{
							DisplayString(6,2,0,"电压不平衡度未越限");
						}
						if((y>>1)&0x1)
						{
							DisplayString(7,2,0,"电流不平衡度越限");
						}
						else
						{
							DisplayString(7,2,0,"电流不平衡度未越限");
						}
						//发生时电压不平衡度
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//无此数据填-----
							DisplayString(9,8,0,"-----");	
						}
						else
						{
							DisplayString(9,7,0,(u8 *)SList[y>>15]);//+ -
							y&=0x7fff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+8*2);
							DisplayData_OffZero(y,4,1,p16);
						}
						DisplayString(9,15,0,(u8 *)UnitList[6]);//6 %
						break;
					case 18://暂无此功能
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC18);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y&0x01)
						{
							DisplayString(5,10,0,"过压");
						}
						else
						{
							DisplayString(5,10,0,"无过压");
						}
						if(y&0x01)
						{
							DisplayString(6,10,0,"装置故障");
						}
						else
						{
							DisplayString(6,10,0,"装置无故障");
						}
						if(y&0x01)
						{
							DisplayString(7,6,0,"执行回路故障");
						}
						else
						{
							DisplayString(7,6,0,"执行回路无故障");
						}
						break;
					case 19:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC19_1);//显示菜单
						DownArrow();
						//设置时间

						//总加组号
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//购电单号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,4);
						y=hex_bcd16(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+10*2);
						DisplayData(y,10,0,p16);
						//追加/刷新标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,1);
						switch(y)
						{
							case 0x55:
								i=0;
								break;
							case 0xAA:
								i=1;
								break;
							default:
								i=2;
								break;
						}
						DisplayString(6,10,0,(u8 *)PurchaseWayList[i]);//0追加 1刷新 2无效
						//购电量费值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,3,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(j=4;j<20;j++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+j*2))
								{
									break;
								}
							}
							DisplayString(8,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/厘 1 MWh/元
						}
						DispalyAlign(8,17,3);//行显示对齐(1左对齐,2居中,3右对齐)
						break;
					case 20:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC20);//显示菜单
						//发生时间

						//消息认证码PW
						p8=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,16);
						for(j=0;j<=1;j++)
						{
							p=(u8 *)(ADDR_STRINGBUFF+(4+j)*84+4+4*2);
							for(i=0;i<8;i++)
							{
								if((p8[i+8*j]>>4)<=9)
								{
									p[(2*i)*2]=(p8[i+8*j]>>4)+0x30;
								}
								else
								{
									p[(2*i)*2]=(p8[i+8*j]>>4)+0x37;
								}
								p[(2*i)*2+1]=0x20;
								if((p8[i+8*j]&0xf)<=9)
								{
									p[(2*i+1)*2]=(p8[i+8*j]&0xf)+0x30;
								}
								else
								{
									p[(2*i+1)*2]=(p8[i+8*j]&0xf)+0x37;
								}
								p[(2*i+1)*2+1]=0x20;
							}
						}
						//启动站地址MSA
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+23,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+4*2);
						DisplayData_OffZero(y,3,0,p16);
						break;
					case 21:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC21);//显示菜单
						//发生时间

						//终端故障
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y<=6 && y>0)
						{
							DisplayString(4,6,0,(u8 *)TerminalBugList[y-1]);
						}
						break;
					case 22:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC22);//显示菜单
						//发生时间

						//起/止标志、电能量差动组号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>7)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7f);
						DisplayData_OffZero(y,2,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//越限时对比总加组有功总电能量、越限时参照总加组有功总电能量
						for(i=0;i<2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8+4*i,4);
							if(y==0xEEEEEEEE)
							{//无此数据填-------
								DisplayString(6+i,10,0,"-------");	
							}
							else
							{
								DisplayString(6+i,10,0,(u8 *)SList[(y>>28)&0x01]);//+ -
								z=y&0xfffffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+11*2);
								p16=DisplayData_OffZero(z,7,0,p16);
								if((y>>30)&0x01)
								{
									p16[0]=0x2000+'M';
								}
								else
								{
									p16[0]=0x2000+'k';
								}
							}
						}
						//相对偏差值
						p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						p16[0]=0x2000+'%';
						//绝对偏差值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+17,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(9,10,0,"-------");	
						}
						else
						{
							DisplayString(9,10,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+11*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							if((y>>30)&0x01)
							{
								p16[0]=0x2000+'M';
							}
							else
							{
								p16[0]=0x2000+'k';
							}
						}
						break;
					case 23:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC23);//显示菜单
						//发生时间

						//总加组号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=hex_bcd(y);
						DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//投入轮次
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if(((y>>i)&0x01)==1)
							{//受控
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//电控类别
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<2;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)EControlList[i]);
							}
						}
						//告警时电能量
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(7,6,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(7,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
						}
						//告警时电控定值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,6,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(8,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
						}
						break;
					case 24:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC24);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//越限标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(6,10,0,"越上上限");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(6,10,0,"越下下限");
						}
						//发生时的电压ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);  
							if(y==0xEEEE)
							{//无此数据填-----
								DisplayString(7+i,10,0,"----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(7+i)*84+10*2);
								DisplayData_OffZero(y,4,1,p16);
							}
							DisplayString(7+i,16,0,(u8 *)UnitList[4]);//4 V
						}
						break;
					case 25:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC25);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//越限标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(6,10,0,"越上上限");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(6,10,0,"越上限");
						}
						//发生时的电流ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+3*i,3);   
							if(y==0xEEEEEE)
							{//无此数据填------
								DisplayString(7+i,10,0,"------");	
							}
							else
							{
								DisplayString(7+i,10,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(7+i)*84+11*2);
								DisplayData_OffZero(y,6,3,p16);
							}
							DisplayString(7+i,18,0,(u8 *)UnitList[5]);//4 A
						}
						break;
					case 26:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC26);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//越限标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(5,10,0,"越上上限");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(5,10,0,"越上限");
						}
						//发生时视在功率
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,3);
						if(y==0xEEEEEE)
						{//无此数据填-------
							DisplayString(6,12,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+12*2);
							DisplayData_OffZero(y,6,4,p16);
						}
						//发生时视在功率限值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,3);
						if(y==0xEEEEEE)
						{//无此数据填-------
							DisplayString(7,12,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+12*2);
							DisplayData_OffZero(y,6,4,p16);
						}

						break;
					case 27:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC27);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//下降前正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(6,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//下降后正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 28://暂不支持 与ERC29记录的内容数据格式一致
					case 29:
						if(((MenuCount->v3)&0xff)==28)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC28);//显示菜单
						}
						if(((MenuCount->v3)&0xff)==29)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC29);//显示菜单
						}
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//飞走前、后正向有功总电能示值
						for(i=0;i<=1;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9+5*i,5);
							if(y==0xEEEEEEEEEE)
							{//无此数据填-------
								DisplayString(6+2*i,4,0,"-----------");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+2*i)*84+4*2);			  
								DisplayData_OffZero(y,10,4,p16);
							}
						}
						//飞走阈值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+19,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+10*2);			  
						DisplayData_OffZero(y,2,1,p16);
						break;
					case 30:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC30);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//电能表停走时正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(6,8,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+9*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//电能表停走阈值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,1);
						if(y==0xEE)
						{//无此数据填-------
							DisplayString(7,10,0,"---");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							DisplayData_OffZero(y,3,0,p16);
						}
						break;
					case 31:
					case 39://ERC31、ERC39记录的内容数据格式一致
						if(((MenuCount->v3)&0xff)==31)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC31);//显示菜单
						}
						if(((MenuCount->v3)&0xff)==39)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC39);//显示菜单
						}
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//最近一次抄表成功时间
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+2*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+9+3*i);
						}
						p16[11]=0x2000+':';
						//最近一次抄表成功正向有功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,5);
						if(y==0xEEEEEEEEEE)
						{//无此数据填-------
							DisplayString(8,7,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+7*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//最近一次抄表成功正向无功总电能示值
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+19,4);
						if(y==0xEEEEEEEE)
						{//无此数据填-------
							DisplayString(9,7,0,"---------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+7*2);			  
							DisplayData_OffZero(y,8,2,p16);
						}
						break;
					case 32:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC32);//显示菜单
						//发生时间

						//当月已发生的通信流量
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,4);
						p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+4*2);
						y=hex_bcd16(y);			  
						DisplayData_OffZero(y,10,0,p16);
						//月通信流量门限
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+11,4);
						p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+4*2);			  
						y=hex_bcd16(y);			  
						DisplayData_OffZero(y,10,0,p16);
						break;
					case 33:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC33);//显示菜单
						//发生时间

						//测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=hex_bcd(y);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						break;
					case 34://暂无此功能
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC34);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//异常标志
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3)>0)
						{
							DisplayString(6,10,0,(u8 *)ERC34List[((y>>6)&0x3)-1]);
						}
						break;
					case 35://暂无此功能
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC35);//显示菜单
						//发生时间

						//终端通信端口号D0~D5
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y&=0x3F;
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						DisplayData_OffZero(y,2,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//发现块数
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						break;
					case 37://电能表开表盖事件记录
					case 38://电能表开端钮盒事件记录//ERC37、ERC38记录的内容数据格式一致
						if(((MenuCount->v3)&0xff)==37)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC37);//显示菜单
						}
						if(((MenuCount->v3)&0xff)==38)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC38);//显示菜单
						}
						//发生时间

						//测量点号
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//开盖总次数
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+11*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,3);
						p16=DisplayData_OffZero(y,6,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						break;
//					case 40://从节点主动上报事件记录
//						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC40);//显示菜单
//						//发生时间
//
//						//事件从节点地址
//						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+4*2);
//						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,6);
//						p16=DisplayData_OffZero(y,12,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
//						//从节点类型
//						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,1);
//						if(y<3)
//						{//03H-FFH保留
//							DisplayString(7,4,0,(u8 *)NodeTypeList[y]);
//						}
//						break;
					case 40://磁场异常事件记录
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC40);//显示菜单
						//发生时间

						//起/止标志、测量点号
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"发生");
						}
						else
						{
							DisplayString(4,10,0,"恢复");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//设备类型
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y>3)
						{//03H-FFH保留
							y=3;
						}
						DisplayString(6,4,0,(u8 *)DeviceTypeList[y]);
						//设备地址
						p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+4*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,6);
						p16=DisplayData_OffZero(y,12,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//磁场异常类型
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16,1);
						if(y<3)
						{
							DisplayString(9,10,0,(u8 *)AbnormalTypeList[y]);
						}
						break;
					case 41:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC41);//显示菜单
						//发生时间
						
						//测量点号
						p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2,2);
						y=hex_bcd(y&0xfff);
						p16=DisplayData_OffZero(y,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
						//对时前时间
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+4,6);
						y&=0xff1fffffffff;
						p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(5-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(2-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						p16[5]=0x2000+':';
						//对时后时间
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,6);
						y&=0xff1fffffffff;
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(5-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(2-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						p16[5]=0x2000+':';
						break;
				}
				//事件发生时间
				if((((MenuCount->v3)&0xff)!=41) && (((MenuCount->v3)&0xff)!=14))
				{//ERC41对时事件记录无发生时间、ERC14终端停上电事件记录新老标准不一样，发生时间写在上面代码
					y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2,5);
					p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
					for(i=0;i<3;i++)
					{
						DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
					}
					p16[2]=0x2000+'-';
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
					for(i=0;i<2;i++)
					{
						DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
					}
					p16[2]=0x2000+':';
				}
				//最近第 ? 次
				p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
				bcd_p8(p8,MenuCount->v2,3);
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
				p16[0]=0x2020;
				p16[1]=0x2020;
				p16[2]=32;//'左右箭头'
				p16[3]=33;
			}
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,12,10);
			break;
		case 1://任务1:选择
			NdigitSelect(3,10,1,(MenuCount->v3)>>8);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuCount->v2 = ((MenuCount->v3)&0xff);
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//下键
					switch((MenuCount->v3)&0xff)
					{
						case 17:
						case 19:
							CreateScreen((u32)Menu_1_5_1_1);
							break;
					}
				}
				Comm_Ram->ButtonVal&=~0x01;//键使用后清0
			}
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5
// Descriptions:        事件记录计数
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1_5(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 m=0;
	u32 n=0;
	u32 y;
	u32 EventNumMax=41;//事件记录编号最大值

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(1,0,0,(u8 *)EventList[0]);
			Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
			if(MenuCount->v2>EventNumMax)
			{//MenuCount->v2记录事件编号
				MenuCount->v2=EventNumMax;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+1*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinColN(1,10,9);
			break;
		case 1://任务1:选择
			if(Stack->MenuStack[MenuCount->Count].EditRow==1)
			{
				NdigitSelect(2,9,1,EventNumMax);
			}
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//下移键按下
					if(Stack->MenuStack[MenuCount->Count].EditRow<3)
					{
						Stack->MenuStack[MenuCount->Count].EditRow++;
					}
				}
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//上移键按下
					if(Stack->MenuStack[MenuCount->Count].EditRow>1)
					{
						Stack->MenuStack[MenuCount->Count].EditRow--;
					}
				}
				if(Stack->MenuStack[MenuCount->Count].EditRow==1)
				{
					p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
					p16[0]=0x2020;
					p16[1]=0x2020;
					p16[2]=32;//'左右箭头'
					p16[3]=33;
					NegativeLnMaxColMinColN(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x23;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(0,0);
	DisplayClrRow(2,3);
	if(MenuCount->v2<=EventNumMax && MenuCount->v2>=1)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(2+i,0,0,(u8 *)EventList[i+1]);
		}
		DisplayString(0,0,0,(u8 *)LogoutList[MenuCount->v2-1]);
		//重要事件记录
		for(i=1;i<=256;i++)
		{
			y=MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*(i-1)),1);//i:1~256，第i条重要事件记录ERC代码
			if(y==MenuCount->v2)
			{
				m++;//记录重要事件ERCy的个数
			}
		}
		p16=(u16*)(ADDR_STRINGBUFF+2*84+4+9*2);
		m=hex_bcd(m);
		p16=DisplayData_OffZero(m,3,0,p16);
		//一般事件记录
		for(i=1;i<=256;i++)
		{
			y=MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*(i-1)),1);//i:1~256，第i条一般事件记录ERC代码
			if(y==MenuCount->v2)
			{
				n++;//记录一般事件ERCy的个数
			}
		}
		p16=(u16*)(ADDR_STRINGBUFF+3*84+4+9*2);
		n=hex_bcd(n);
		p16=DisplayData_OffZero(n,3,0,p16);
		if(Stack->MenuStack[MenuCount->Count].EditRow!=1)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+18*2);
			p16[0]=0;
			p16[1]=0;
			for(j=0;j<10;j++)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+(j*84)+4);
				for(i=0;i<20;i++)
				{
					p16[i]&=0x7fff;
				}
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4);
			for(i=0;i<20;i++)
			{
				p16[i]|=0x8000;
			}														
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow)
				{
					case 2:
						m=bcd_hex(m);
						if(m)
						{
							MenuCount->v3=(m<<8)+MenuCount->v2;
							MenuCreate((u32)Menu_1_5_1);//创建下级菜单
						}
						break;
					case 3:
						n=bcd_hex(n);
						if(n)
						{
							MenuCount->v3=(n<<8)+MenuCount->v2;
							MenuCreate((u32)Menu_1_5_1);//创建下级菜单
						}
						break;
				}
			}
			Comm_Ram->ButtonVal&=~0x10;//键使用后清0
		}
	}
	else
	{
		DisplayString(0,0,0,"无效ERC号");
	}
	DispalyAlign(0,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
}
#endif

#if((Project/100)==3 && (USER/100)==5)//上海专变
void Menu_SH_1_1(void)//当前功控记录
{
	u32 i;
	u32 x;
	u32 Row=1;
	u64 y;
	u16 *p16;

	//清全屏
	DisplayClr();
	Draw_Clr();
	for(i=0;i<8;i++)
	{
		DisplayString(i+1,0,0,(u8 *)ERC6List[i]);
	}

	//已有事件计数
	x=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT))),2);
	if(x>15)
	{
		x=15;
	}
	if(x>0)
	{
		//跳闸时间
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT),5);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<3;i++)
		{
			DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+'-';
		p16[5]=0x2000+'-';
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<2;i++)
		{
			DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+':';
		//总加组号
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+5+((x-1)*LEN_TERMINAL_EVENT),1);
		y=hex_bcd(y);
		p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		//跳闸轮次
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+6+((x-1)*LEN_TERMINAL_EVENT),1);
		for(i=0;i<=7;i++)
		{
			if((y>>i)&0x01)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//功控类别
		Row++;
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+7+((x-1)*LEN_TERMINAL_EVENT),1);
		for(i=0;i<4;i++)
		{
			if((y>>i)&0x01)
			{
				DisplayString(Row,10,0,(u8 *)PowerControlList[i]);
			}
		}
		//跳闸前功率
		Row++;
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+8+((x-1)*LEN_TERMINAL_EVENT),2);
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//跳闸后2分钟的功率
		Row++;
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+10+((x-1)*LEN_TERMINAL_EVENT),2);
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//跳闸时功率定值
		Row++;
		y=MRR(ADDR_AFN0EF3+(5*(2+(15*LEN_TERMINAL_EVENT)))+4+12+((x-1)*LEN_TERMINAL_EVENT),2);
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)UnitList[0]);//0 kW
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}
void Menu_SH_1_2(void)//当前电控记录
{
	u32 i;
	u32 x;
	u32 Row=1;
	u64 y;
	u64 z;
	u16 *p16;

	//清全屏
	DisplayClr();
	Draw_Clr();
	for(i=0;i<7;i++)
	{
		DisplayString(i+1,0,0,(u8 *)ERC7List[i]);
	}

	//已有事件计数
	x=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT))),2);
	if(x>15)
	{
		x=15;
	}
	if(x>0)
	{
		//跳闸时间
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT),5);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<3;i++)
		{
			DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+'-';
		p16[5]=0x2000+'-';
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<2;i++)
		{
			DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+':';
		//总加组号
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+5+((x-1)*LEN_TERMINAL_EVENT),1);
		y=hex_bcd(y);
		p16=DisplayData_OffZero(y,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		//跳闸轮次
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+6+((x-1)*LEN_TERMINAL_EVENT),1);
		for(i=0;i<=7;i++)
		{
			if((y>>i)&0x01)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//电控类别
		Row++;
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+7+((x-1)*LEN_TERMINAL_EVENT),1);
		for(i=0;i<2;i++)
		{
			if((y>>i)&0x01)
			{
				DisplayString(Row,10,0,(u8 *)EControlList[i]);
			}
		}
		//跳闸时电量
		Row++;
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+8+((x-1)*LEN_TERMINAL_EVENT),4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
		}
		//跳闸时定值
		Row++;
		y=MRR(ADDR_AFN0EF3+(6*(2+(15*LEN_TERMINAL_EVENT)))+4+12+((x-1)*LEN_TERMINAL_EVENT),4);
		if(y==0xEEEEEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuCount->v2 = ((MenuCount->v3)&0xff);
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

void Menu_SH_1_3(void)//遥控记录
{
	u32 i;
	u32 x;
	u32 Row=1;
	u64 y;
	u16 *p16;
	//清全屏
	DisplayClr();
	Draw_Clr();
	for(i=0;i<5;i++)
	{
		DisplayString(i+1,0,0,(u8 *)ERC5List[i]);
	}

	//已有事件计数
	x=MRR(ADDR_AFN0EF3+(4*(2+(15*LEN_TERMINAL_EVENT))),2);
	if(x>15)
	{
		x=15;
	}
	if(x>0)
	{
		//跳闸时间
		y=MRR(ADDR_AFN0EF3+(4*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT),5);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<3;i++)
		{
			DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+'-';
		p16[5]=0x2000+'-';
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<2;i++)
		{
			DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+':';
		//跳闸轮次
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		y=MRR(ADDR_AFN0EF3+(4*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT)+5,1);
		for(i=0;i<=7;i++)
		{
			if((y>>i)&0x01)
			{//受控
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//跳闸时功率
		Row++;
		y=MRR(ADDR_AFN0EF3+(4*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT)+6,2);
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//跳闸后2分钟的功率
		Row++;
		y=MRR(ADDR_AFN0EF3+(4*(2+(15*LEN_TERMINAL_EVENT)))+4+((x-1)*LEN_TERMINAL_EVENT)+8,2);
		if(y==0xEEEE)
		{//无此数据填-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)UnitList[0]);//0 kW
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuCount->v2 = ((MenuCount->v3)&0xff);
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x20;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_SH_1
// Descriptions:        实时数据第二屏
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_SH_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"实时数据第二屏\n\r"
																			"  6.功控记录\n\r"
																			"  7.电控记录\n\r"
																			"  8.遥控记录\n\r"
																			"  9.开关状态\n\r"
																			);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x0C)!=0)
			{//按左键或右键
				//实时数据第一屏
				CreateScreen((u32)Menu_1);//翻屏
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_SH_1_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_SH_1_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_SH_1_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_1_4);//创建下级菜单
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回										 
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}
void Menu_SH_1_4(void)//上海失电记录
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 Row=3;
	u64 x;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"失电记录\n\r"
																			" ERC14最近 ? 次\n\r"
																			"失电时间:\n\r"
																			"\n\r"
																			"恢复时间:\n\r"
																			);//显示菜单
			if(MenuCount->v2>15)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=15;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2记录测量点的全局变量
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+2*84+4+10*2);
			bcd_p8(p8,MenuCount->v2,3);
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'左右箭头'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,12,10);
			break;
		case 1://任务1:选择
			NdigitSelect(3,10,1,15);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		default:
			break;
	}
	DisplayClrRow(Row,5);//清屏幕显示的指定行
	//已有事件计数
	x=MRR(ADDR_AFN0EF3+(13*(2+(15*LEN_TERMINAL_EVENT))),2);
	if(x>15)
	{
		x=15;
	}
	if((MenuCount->v2<=x) && x!=0)
	{
		for(i=0;i<3;i++)
		{
			DisplayString(Row+i,0,0,(u8 *)ERC14List[i]);
		}
		//失电时间
		y=MRR(ADDR_AFN0EF3+(13*(2+(15*LEN_TERMINAL_EVENT)))+2+(x-MenuCount->v2)*LEN_TERMINAL_EVENT+3,5);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<3;i++)
		{
			DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+'-';
		p16[5]=0x2000+'-';
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<2;i++)
		{
			DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+':';	
		//恢复时间
		Row++;
		y=MRR(ADDR_AFN0EF3+(13*(2+(15*LEN_TERMINAL_EVENT)))+2+(x-MenuCount->v2)*LEN_TERMINAL_EVENT+3+5,5);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<3;i++)
		{
			DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+'-';
		p16[5]=0x2000+'-';
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+10*2);
		for(i=0;i<2;i++)
		{
			DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
		}
		p16[2]=0x2000+':';
	}
	else
	{
		DisplayString(Row,6,0,"无此事件");	
	}
}

void Menu_SH_1_5_1_3(void)//失压统计第三屏
{
	u16 *p16;
	u32 Row;
	u64 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    失压统计\n\r"
																	"全失压次数:      次\n\r"
																	"总累计时间:      分\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=hex_bcd(MenuCount->v3);
	DisplayData_OffZero(x,4,0,p16);
	//全失压总次数
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+401,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//全失压总累计时间
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+401+3,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_1_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_1_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_SH_1_5_1_2(void)//失压统计第二屏
{
	u16 *p16;
	u32 Row;
	u64 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    失压统计\n\r"
																	"A 失压次数:      次\n\r"
																	"总累计时间:      分\n\r"
																	"B 失压次数:      次\n\r"
																	"总累计时间:      分\n\r"
																	"C 失压总数:      次\n\r"
																	"总累计时间:      分\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=hex_bcd(MenuCount->v3);
	DisplayData_OffZero(x,4,0,p16);

	//A相失压总次数
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+3,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//A相失压总累计时间
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+15,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//B相失压总次数
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+6,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//B相失压总累计时间
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+18,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//C相失压总次数
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+9,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//C相失压总累计时间
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+21,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_1_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_1_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_SH_1_5_1_1(void)//失压统计第一屏
{
	u16 *p16;
	u32 Row;
	u64 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    失压统计\n\r"
																	"失压总次数:      次\n\r"
																	"总累计时间:      分\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=hex_bcd(MenuCount->v3);
	DisplayData_OffZero(x,4,0,p16);
	//失压总次数
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}
	//失压总累计时间
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(MenuCount->v3-1)+5+12,3);
	if(x==0xEEEEEE)
	{
		DisplayString(Row,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
		DisplayData_OffZero(x,6,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_1_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_1_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}

void Menu_SH_1_5_1(void)//上海失压记录-失压统计
{
	u32 i;
	u32 j;
	u32 Row;
	u32 x;
	u32 k;
	u8* p;
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"失压统计\n\r"
																			"测量点号:\n\r"
																			"     确定  取消\n\r"
																			);//显示菜单
			if(MenuCount->v3>MaxRS485AddCarrierPn)
			{//MenuCount->v3测量点号
				MenuCount->v3=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3测量点号
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+(2)*84+4+9*2);
			bcd_p8(p,MenuCount->v3,4);
			if(Stack->MenuStack[MenuCount->Count].EditRow==1)
			{
	 			ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
				MenuCount->v1=(12<<8)+9;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							MenuCount->v1=(12<<8)+9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=3)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
							for(Row=0;Row<10;Row++)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4);
								for(i=0;i<20;i++)
								{
									p16[i]&=0x7fff;
								}
							}
							for(i=5;i<9;i++)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+i*2);
								*p16|=0x8000;//反显指定行、列    
							}
							Stack->MenuStack[MenuCount->Count].Task=2;
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(2,16,0,"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+(MenuCount->v1)*2);
								//测量点号存储数据错误则初始化
								for(i=0;i<=3;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,1,4);
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select((MenuCount->v1)>>8,(MenuCount->v1)&0xff);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(2)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		case 2:
			for(j=0;j<2;j++)
			{
				BoxSelect();
				switch(Stack->MenuStack[MenuCount->Count].EditRow)
				{
					case 1:
						ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						MenuCount->v1=(12<<8)+9;
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
						Stack->MenuStack[MenuCount->Count].Task=1;
						break;
					case 2:
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+5*2);
							if(*p16&0x8000)
							{
								for(Row=0;Row<10;Row++)
								{
									for(i=5;i<9;i++)
									{
										p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+i*2);
										*p16&=0x7fff;//反显指定行、列    
									}
								}
								for(i=11;i<15;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+i*2);
									*p16|=0x8000;//反显指定行、列    
								}
							}
						}
						if((Comm_Ram->ButtonVal&0x04)!=0)
						{//左键
							p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+11*2);
							if(*p16&0x8000)
							{
								for(i=11;i<15;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+i*2);
									*p16&=0x7fff;//反显指定行、列    
								}
								for(i=5;i<9;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+i*2);
									*p16|=0x8000;//反显指定行、列    
								}
							}
						}
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认
							p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+9*2);
							//测量点号
							MenuCount->v3=(p8[0]-0x30)*1000+(p8[2]-0x30)*100+(p8[4]-0x30)*10+(p8[6]-0x30);
							p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+5*2);
							if(*p16&0x8000)
							{//确认
								MenuCreate((u32)Menu_SH_1_5_1_1);//失压统计第一屏
							}
							p16=(u16 *)(ADDR_STRINGBUFF+(3*84)+4+11*2);
							if(*p16&0x8000)
							{//取消
								MenuDel();//返回上级菜单
							}
							Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
						}
						else
						{
							Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
							break;
						}
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x20;//键使用后清0
				break;
			}
			break;
		default:
			break;
	}
}
void Menu_SH_1_5_2_3(void)//最近n次失压记录第三屏
{
	u16 *p16;
	u8 *p;
	u8 *p8;
	u32 Row;
	u64 x;
	u32 i;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    \n\r"
																	"    最近  次失压\n\r"
																	"全失压发生时刻:\n\r"
																	"\n\r"
																	"电流值:       A\n\r"
																	"全失压结束时刻:\n\r"
																	"\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
	x=hex_bcd((MenuCount->v3)&0xff);
	DisplayData_OffZero(x,4,0,p16);
	//最近次数
	Row++;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+8*2);
	x=hex_bcd((MenuCount->v3>>8)&0xf);
	DisplayData_OffZero(x,2,0,p16);
//全失压发生时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+407+15*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}
	
//电流值
	Row++;
	x=MRR(ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+407+6+15*(((MenuCount->v3>>8)&0xf)-1),3);
	if(x==0xeeeeee)
	{
		DisplayString(Row,7,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+7*2);
		DisplayData_OffZero(x,6,3,p16);
	}

//全失压结束时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+407+9+15*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_2_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_2_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_SH_1_5_2_2(void)//最近n次失压记录第二屏
{
	u16 *p16;
	u8 *p;
	u8 *p8;
	u32 Row;
	u64 x;
	u32 i;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    \n\r"
																	"    最近  次失压\n\r"
																	"B相失压结束时刻:\n\r"
																	"\n\r"
																	"C相失压发生时刻:\n\r"
																	"\n\r"
																	"C相失压结束时刻:\n\r"
																	"\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
	x=hex_bcd((MenuCount->v3)&0xff);
	DisplayData_OffZero(x,4,0,p16);
	//最近次数
	Row++;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+8*2);
	x=hex_bcd((MenuCount->v3>>8)&0xf);
	DisplayData_OffZero(x,2,0,p16);
//B相失压结束时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+180+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}
	
//C相失压发生时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+240+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}
	
//C相失压结束时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+300+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}
		
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_2_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_2_1);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_SH_1_5_2_1(void)//最近n次失压记录第一屏
{
	u16 *p16;
	u8 *p;
	u8 *p8;
	u32 Row;
	u64 x;
	u32 i;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"测量点    \n\r"
																	"    最近  次失压\n\r"
																	"A相失压发生时刻:\n\r"
																	"\n\r"
																	"A相失压结束时刻:\n\r"
																	"\n\r"
																	"B相失压发生时刻:\n\r"
																	"\n\r"
																	);//显示菜单
	//测量点号
	Row=1;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+11*2);
	x=hex_bcd((MenuCount->v3)&0xff);
	DisplayData_OffZero(x,4,0,p16);
	//最近次数
	Row++;
	p16=(u16*)(ADDR_STRINGBUFF+Row*84+4+8*2);
	x=hex_bcd((MenuCount->v3>>8)&0xf);
	DisplayData_OffZero(x,2,0,p16);
//A相失压发生时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
	}
	}
	
//A相失压结束时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+60+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}

//B相失压发生时刻
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF26_Ext+LEN_AFN0CF26_Ext*(((MenuCount->v3)&0xff)-1)+41+120+6*(((MenuCount->v3>>8)&0xf)-1),6);
	if((p8[0]==0xee)&&(p8[1]==0xee)&&(p8[2]==0xee)&&(p8[3]==0xee)&&(p8[4]==0xee)&&(p8[5]==0xee))
	{
		DisplayString(Row,2,0,"------  ------");
	}
	else
	{
		//日期
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+2*2);
		p[0*2]=(p8[5]>>4)+0x30;//年
		p[1*2]=(p8[5]&0xf)+0x30;
		p[2*2]='-';
		p[3*2]=(p8[4]>>4)+0x30;//月
		p[4*2]=(p8[4]&0xf)+0x30;
		p[5*2]='-';
		p[6*2]=(p8[3]>>4)+0x30;//日
		p[7*2]=(p8[3]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
		//时间
		p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+12*2);
		p[0*2]=(p8[2]>>4)+0x30;//时
		p[1*2]=(p8[2]&0xf)+0x30;
		p[2*2]=':';
		p[3*2]=(p8[1]>>4)+0x30;//分
		p[4*2]=(p8[1]&0xf)+0x30;
		p[5*2]=':';
		p[6*2]=(p8[0]>>4)+0x30;//秒
		p[7*2]=(p8[0]&0xf)+0x30;
		for(i=0;i<=7;i++)
		{
			p[i*2+1]=0x20;
		}
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//右移
		CreateScreen((u32)Menu_SH_1_5_2_2);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//位=1表示按下;B0=下移,B1=上移,B2=左移,B3=右移,B4=确认,B5=退出
	{//左移
		CreateScreen((u32)Menu_SH_1_5_2_3);//翻屏
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x2C;//键使用后清0
}
void Menu_SH_1_5_2(void)//上海失压记录-最近n次失压记录
{
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row;
	u8* p;
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"最近n次失压记录\n\r"
																			"测量点号:\n\r"
																			"最近次数:\n\r"
																			"     确定  取消\n\r"
																			);//显示菜单
			if((MenuCount->v3&0xff)>MaxRS485AddCarrierPn)
			{//测量点号
				MenuCount->v3&=0xf00;
				MenuCount->v3+=MaxRS485AddCarrierPn;
			}
			if((MenuCount->v3&0xff)<1)
			{//测量点号
				MenuCount->v3&=0xf00;
				MenuCount->v3+=1;
			}
			if(((MenuCount->v3>>8)&0xf)>10)
			{//最近次数
				MenuCount->v3&=0x0ff;
				MenuCount->v3+=0xA00;
			}
			if(((MenuCount->v3>>8)&0xf)<1)
			{//最近次数
				MenuCount->v3&=0x0ff;
				MenuCount->v3+=0x100;
			}
			p=(u8*)(ADDR_STRINGBUFF+(2)*84+4+9*2);
			bcd_p8(p,(MenuCount->v3&0xff),4);
			p=(u8*)(ADDR_STRINGBUFF+(3)*84+4+9*2);
			bcd_p8(p,((MenuCount->v3>>8)&0xf),2);
			if(Stack->MenuStack[MenuCount->Count].EditRow==1)
			{
	 			ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
				MenuCount->v1=(12<<8)+9;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
			}
			break;
		case 1://任务1:选择
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							MenuCount->v1=(12<<8)+9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
							break;
						case 2:
							ArrowLR(3,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
							MenuCount->v1=(10<<8)+9;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
							break;
						case 3:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=4)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
							for(Row=0;Row<10;Row++)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4);
								for(i=0;i<20;i++)
								{
									p16[i]&=0x7fff;
								}
							}
							for(i=5;i<9;i++)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+i*2);
								*p16|=0x8000;//反显指定行、列    
							}
							Stack->MenuStack[MenuCount->Count].Task=2;
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//右键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(3)*84+4+9*2);
								x=p8_bcd(p8,2);
								if(x<10)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//左键
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(3)*84+4+9*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,10,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(2,16,0,"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(2)*84+4+(MenuCount->v1)*2);
								//测量点号存储数据错误则初始化
								for(i=0;i<=3;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,1,4);
								}
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(3,16,0,"输入");
								p8=(u8 *)(ADDR_STRINGBUFF+(3)*84+4+(MenuCount->v1)*2);
								//测量点号存储数据错误则初始化
								for(i=0;i<=3;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,1,4);
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select((MenuCount->v1)>>8,(MenuCount->v1)&0xff);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//确认
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(2)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(3)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'左右箭头'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+9*2);
								x=p8_bcd(p8,2);
								if(x>10 || x<1)
								{
									bcd_p8(p8,1,2);
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x20;//键使用后清0
			break;
		case 2:
			for(j=0;j<2;j++)
			{
				BoxSelect();
				switch(Stack->MenuStack[MenuCount->Count].EditRow)
				{
					case 1:
						ArrowLR(2,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						MenuCount->v1=(12<<8)+9;
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
						Stack->MenuStack[MenuCount->Count].Task=1;
						break;
					case 2:
						ArrowLR(3,18);//在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
						MenuCount->v1=(10<<8)+9;
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,(MenuCount->v1)>>8,(MenuCount->v1)&0xff);
						Stack->MenuStack[MenuCount->Count].Task=1;
						break;
					case 3:
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//右键
							p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+5*2);
							if(*p16&0x8000)
							{
								for(Row=0;Row<10;Row++)
								{
									for(i=5;i<9;i++)
									{
										p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+i*2);
										*p16&=0x7fff;//反显指定行、列    
									}
								}
								for(i=11;i<15;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+i*2);
									*p16|=0x8000;//反显指定行、列    
								}
							}
						}
						if((Comm_Ram->ButtonVal&0x04)!=0)
						{//左键
							p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+11*2);
							if(*p16&0x8000)
							{
								for(Row=0;Row<10;Row++)
								{
									for(i=11;i<15;i++)
									{
										p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+i*2);
										*p16&=0x7fff;//反显指定行、列    
									}
								}
								for(i=5;i<9;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+i*2);
									*p16|=0x8000;//反显指定行、列    
								}
							}
						}
						if((Comm_Ram->ButtonVal&0x10)!=0)
						{//确认
							//测量点号
							p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+9*2);
							MenuCount->v3=(p8[0]-0x30)*1000+(p8[2]-0x30)*100+(p8[4]-0x30)*10+(p8[6]-0x30);
							//最近次数
							p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+9*2);
							i=((p8[0]-0x30)*10+(p8[2]-0x30));
							MenuCount->v3+=(i<<8);																				  
							p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+5*2);
							if(*p16&0x8000)
							{//确认																				
								MenuCreate((u32)Menu_SH_1_5_2_1);//最近n次失压记录第一屏
							}
							p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4+11*2);
							if(*p16&0x8000)
							{//取消
								MenuDel();//返回上级菜单
							}
							Comm_Ram->ButtonVal&=~0x1C;//键使用后清0
						}
						else
						{
							Comm_Ram->ButtonVal&=~0x0C;//键使用后清0
							break;
						}
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//返回
					MenuDel();//返回上级菜单
				}
				Comm_Ram->ButtonVal&=~0x23;//键使用后清0
				break;
			}
			break;
		default:
			break;
	}
}
void Menu_SH_1_5(void)//上海失压记录
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"失压记录\n\r"
																			"  1.失压统计\n\r"
																			"  2.最近n次失压记录\n\r"
																			);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_SH_1_5_1);//失压统计
						break;
					case 2:
						MenuCreate((u32)Menu_SH_1_5_2);//最近n次失压记录
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        实时数据第一屏
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//菜单1
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"实时数据第一屏\n\r"
																			"  1.当前电量\n\r"
																			"  2.当前功率\n\r"
																			"  3.负荷曲线\n\r"
																			"  4.失电记录\n\r"
																			"  5.失压记录\n\r"
																			);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x0C)!=0)
			{//按左键或右键
				//实时数据第二屏
				CreateScreen((u32)Menu_SH_1);//翻屏
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_2);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_1_1);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_1_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_SH_1_4);//SH上海失电记录
						break;
					case 5:
						MenuCreate((u32)Menu_SH_1_5);//SH上海失压记录
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        实时数据菜单内容
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//菜单1
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
		#if ((Project/100)==3)//专变终端
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//显示菜单
		#endif
		#if ((Project/100)==4)//公变终端
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu4_1);//显示菜单
		#endif
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
			#if ((Project/100)==3)//专变终端
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_1);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_1_2);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_1_3);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_1_4);//创建下级菜单
						break;
					case 5:
						MenuCreate((u32)Menu_1_5);
						break;
					default:
						break;
				}
			#endif        
			#if ((Project/100)==4)//公变终端
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_2);//创建下级菜单
						break;
					case 2:
						MenuCreate((u32)Menu_1_3);//创建下级菜单
						break;
					case 3:
						MenuCreate((u32)Menu_1_4);//创建下级菜单
						break;
					case 4:
						MenuCreate((u32)Menu_1_5);//创建下级菜单
						break;
					default:
						break;
				}
			#endif        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}
#endif




