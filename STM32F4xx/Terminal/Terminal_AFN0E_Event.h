


#ifndef Terminal_Event_H
#define Terminal_Event_H



#if ((Project/100)==1)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "../Head/ERC_Project1_USER0xx.h"
#endif

#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#if(USER/100)==4//陕西
#include "../Head/ERC_Project2_USER4xx.h"
	#else
		#if(USER/100)==5//上海
#include "../Head/ERC_Project2_USER5xx.h"
		#else
			#if(USER/100)==6//湖南
#include "../Head/ERC_Project2_USER6xx.h"
			#else
				#if(USER/100)==7//浙江
#include "../Head/ERC_Project2_USER7xx.h"
				#else
					#if(USER/100)==8//通用市场
#include "../Head/ERC_Project2_USER8xx.h"
					#else
						#if(USER/100)==9//河南
#include "../Head/ERC_Project2_USER9xx.h"
						#else
							#if(USER/100)==11//吉林
#include "../Head/ERC_Project2_USER11xx.h"
							#else
								#if(USER/100)==12//四川
#include "../Head/ERC_Project2_USER12xx.h"
								#else
									#if(USER/100)==3//福建
#include "../Head/ERC_Project2_USER3xx.h"
									#else
#include "../Head/ERC_Project2_USER0xx.h"
									#endif
								#endif
							#endif
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#if(USER/100)==5//上海
#include "../Head/ERC_Project3_USER5xx.h"
	#else
		#if(USER/100)==10//河北
#include "../Head/ERC_Project3_USER10xx.h"
		#else
			#if(USER/100)==12//四川
#include "../Head/ERC_Project3_USER12xx.h"
			#else
				#if(USER/100)==15//西藏
#include "../Head/ERC_Project3_USER15xx.h"
				#else
					#if(USER/100)==8//通用市场
#include "../Head/ERC_Project3_USER8xx.h"
					#else
#include "../Head/ERC_Project3_USER0xx.h"
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "../Head/ERC_Project4_USER0xx.h"
#endif

#if ((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

#endif

#if ((Project/100)==6)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

#endif










void Init_Class3Data_UpPower(void);//类3数据(事件计算）初始化(上电)
void Init_Class3Data_Command(void);//类3数据(事件)初始化(命令,全部清除)
void Init_Class3Data(u32 Pn);//类3数据(事件)初始化(指定的测量点数据,Pn=1~n)

u32 ERC_Event_Set(u32 EventNo);//检查事件记录设置,返回0=不记录,1=记录
u32 ERC_Event_Save(u32 BuffAddr);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
u32 Get_ADDR_UARTnRMEvent(u32 PORTn);//得到UART抄读电能表事件栈结构地址
u32 Check_Meter_Event_Save(void);//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
void Meter_Event_WriteBuff(u32 PORTn,u32 Pn,u32 Addr_FnDataBuff,u8 *p8RMLib);//读取的电能表事件写入缓冲ADDR_DATABUFF
void Meter_Event_Save(u32 Addr_Buff);//直接存贮读取的电能表事件
void WriteAutoReportWord(u32 PORTn,u32 rxbuff);//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
u32 Check_AutoReportWord(u32 PORTn);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
u32 RMEvene_YesNo(u32 PORTn,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol);//按分类分级事件抄读的Fn是否要抄;返回0=不抄,1=要抄;Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)
void Search_AFN04F106_Evene_DI(u32 PORTn,u32 Pn,u32 RMCount);//按分类分级事件的当前事件抄读DI搜索AFN04F106设置中非发生次数的事件类抄读DI,返回:有设置UARTCtrl->RMDI=DI,没设置UARTCtrl->RMDI=0xffffffff
u32 Search_AFN04F106_NoEvene_DI(u32 PORTn,u32 Pn);//按分类分级事件搜索AFN04F106设置中非事件类抄读;返回0=没找到,1=找到


void ERC1_Event(u32 f);//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
void ERC2_Event(u32 f);//参数丢失记录;f=1终端参数丢失,f=2测量点参数丢失,f=3终端参数丢失同时测量点参数丢失
void ERC3_Event(u32 rxbuff);//参数变更记录,入口接收缓冲首址
void ERC4_Event(u32 sc,u32 ss);//状态量变位记录;入口sc=状态变位,ss=变位后状态
void ERC5_Event(u32 n,u32 kw1,u32 kw2);//遥控跳闸记录;入口n=跳闸轮次(0-7),kw1=跳闸时功率(总加功率),kw2=跳闸后2分钟的功率(总加功率)
void ERC6_Event(u32 Pn,u32 Bin,u32 Type,u32 kW1,u32 kW2,u32 kW3);//功控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
void ERC7_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2);//电控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type电控类别,kwh1跳闸时电能量,kwh2跳闸时电能量定值
void ERC8_Event(u32 Pn,u32 FnDataBuff);//电能表参数变更,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
void ERC9_3_Event(u32 Pn);//电流回路异常.反向,入口:Pn测量点号(从1开始)
//void ERC10_Event(u32 Pn);//电压回路异常(断相、失压),入口:Pn测量点号(从1开始)
void ERC10_Event(void);//电压回路异常(断相、失压)
void ERC11_Event(u32 Pn,u32 f);//相序异常,入口:Pn测量点号(从1开始),f=0恢复=1产生
void ERC12_Event(u32 Pn);//电能表时间超差,入口:Pn测量点号(从1开始)
void ERC13_Event(u32 Pn,u32 FnDataBuff);//电表故障信息,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
	#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
void ERC14_Event_Start(void);//终端停/上电事件(停电)
void ERC14_Event_End(void);//终端停/上电事件(上电),用专用事件数据缓冲
	#else
void ERC14_Event_Start(u32 f);//终端停/上电事件(停电);入口:上电记录时事件记数指针需加1标志,0=不加1,0xee=需加1
void ERC14_Event_End(u32 f);//终端停/上电事件(上电),用专用事件数据缓冲
	#endif
//ERC15谐波越限告警(已包含在统计中)
void ERC16_Event(u32 Pn,u32 ss,u32 f,u32 dc);//直流模拟量越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=越限标志,dc=当前直流模拟量
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni);//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
//ERC18电容器投切自锁记录
void ERC19_Event(u32 Pn,u32 RxAddr,u32 kWh1,u32 kWh2);//购电参数设置记录
void ERC20_Event(u64 PWL,u64 PWH,u32 MSA);//消息认证错误记录,入口:PWH_PWL消息认证码,MSA=启动站地址
void ERC21_Event(void);//ERC21终端故障记录
void ERC22_Event(void);//有功总电能量差动越限计算和事件记录
void ERC23_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2);//电控告警记录;入口Pn总加组号,Bin跳闸轮次,Type电控类别,kwh1告警时电能量,kwh2告警时电能量定值
void ERC24_Event(u32 Pn,u32 ss,u32 f);//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
void ERC25_Event(u32 Pn,u32 ss,u32 f);//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
void ERC26_Event(u32 Pn,u32 ss,u32 f,u32 kva1,u32 kva2);//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
void ERC27_Event(u32 Pn,u32 FnDataBuff);//电能表示度下降记录(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
//ERC28电能量超差记录
void ClrERC29SOUR(u32 Pn);//集中抄表失败清ERC29电能表飞走,超差事件原始数据
void ERC29_Event(u32 Pn,u32 FnDataBuff);//电能表飞走,超差(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
void ERC30_Event(u32 Pn);//电能表停走事件,入口:Pn=测量点号(1开始)
void ERC31_Event(u32 MeterNo,u32 Pn,u32 f);//终端485抄表失败事件,入口测量点号,f=1产生=0恢复
void ERC32_Event(u32 n1,u32 n2);//通信流量超门限事件,n1=当月已发生的通信流量,n2=月通信流量门限
void ERC33_Event(u32 Pn,u32 FnDataBuff);//电能表运行状态字变位事件(在类1数据存贮前写事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
//ERC34 CT异常事件
void ERC35_Event(u32 Port,u32 n,u32 DataAddr);//发现未知电表事件;入口Port=端口号,n=未知电表数,DataAddr=未知电表通信地址等每表8字节数据存放的地址
void ERC36_Event(u32 CD,u32 ST);//控制输出回路开关接入状态量变位记录;入口：CD=变位,ST=状态
void ERC37_Event(u32 Pn,u32 ADDR_DATA);//电能表开表盖事件记录;入口:Pn测量点号(从1开始)
void ERC38_Event(u32 Pn,u32 ADDR_DATA);//电能表开端钮盒事件记录;入口:Pn测量点号(从1开始)
//void ERC39_Event(u32 Fn,u32 Pn,u32 f);//补抄失败事件记录;入口:当前抄读的FnPn,f=0抄表成功,f=1补抄失败
void ERC39_Event(u32 Pn);//补抄失败事件记录
void ERC39_Event_Cancel(u32 Pn);//补抄失败事件恢复
void ERC40_Meter_Event(u32 FnDataAddr,u32 Pn);//ERC40直接读电能表磁场异常事件记录;入口:Fn数据首地址,测量点号(1开始)
void ERC41_Event(u32 ADDR_BUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn);//ERC41直接读电能表对时事件记录;入口:Fn数据首地址,测量点号(1开始)


void ERC45_Event(void);//终端电池失效事件
void PerERC_Event_Save(u32 BuffAddr);//分类ERC事件记录
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
void ERC51_Event(u32 Pn,u32 ADDR_BUFF);//校时记录事件;入口:Pn测量点号(从0开始)
#endif

#endif








