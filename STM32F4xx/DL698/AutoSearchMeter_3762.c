

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/RM_TxDATA.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"





//属性2（所有搜表结果）∷=array 一个搜表结果
//一个搜表结果∷=structure
//{
//  通信地址         TSA，
//  所属采集器地址   TSA，
//	规约类型  enum
//	{
//  	未知          （0），
//		DL/T645-1997  （1），
//		DL/T645-2007  （2），
//		DL/T698.45    （3），
//		CJ/T 188-2004 （4）
//	}，
//	相位             enum{未知（0），A（1），B（2），C（3）}，
//		信号品质         unsigned，
//	搜到的时间       date_time_s，
//	搜到的附加信息   array附加信息
//}
//附加信息∷=structure
//{
//	对象属性描述  OAD，
//	属性值        Data
//}

//属性5（跨台区搜表结果）∷=array 一个跨台区结果
//一个跨台区结果∷=structure
//{
//	通信地址       TSA，
//	主节点地址     TSA，
//	变更时间       date_time_s
//}

#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void AutoSearchMeter_3762(u32 PORTn)//自动搜表3762
{
	switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微
	{
		case 2://鼎信
			AutoSearchMeter_3762_TC(PORTn);//自动搜表3762-鼎信
			break;
		case 3://东软
		case 9://9=华瑞
			AutoSearchMeter_3762_ES(PORTn);//自动搜表3762-东软
			break;
		case 1://正泰
		case 5://瑞斯康
		default://其他
			AutoSearchMeter_3762_01(PORTn);//自动搜表3762-瑞斯康
			break;
	}
}


//自动搜表3762-鼎信
const u8 TC_Example1[]=
{0x68,0x1F,0x00,0x41,0x00,0x00,0x28,0x64,0x00,0x00,0x05,0x04,0x00,0x02,0x0E,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x02,0x36,0x33,0x4A,0x16,0x90,0x16,};

void AutoSearchMeter_3762_TC(u32 PORTn)//自动搜表3762-鼎信
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8tx;
	u8 * p8rx;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//自动搜表任务:0=空,1=等待,2...过程
	{
		case 1://等待
			UARTCtrl->AutoSearchTask++;//自动搜表任务:0=空,1=等待,2...过程
			break;
		case 2://暂停路由
			WarningOccur(PORTn,(u8 *)"载波正在自动搜表");//告警发生,入口告警代码和告警字符串
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			Terminal_Router->StopFlags|=(1<<13);//暂停路由
			Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
			UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
		
	#if (USER/100)==0
		//送检模式
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
				Terminal_Router->KeepTime=120;//注册持续时间定时值分
			}
			else
			{//每日执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//到23:00:00的时间
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask=4+1;//自动搜表任务
	#else
		//正常功能
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
//			#if RMM_Type==0//集中抄表类型:0=载波表,1=RS485表通过采集器,2=混合
				Terminal_Router->RS485Time=0;//采集器搜485电表定时值分(BCD)
				Terminal_Ram->AutoSearch_S_Timer=0;//28 载波自动搜表秒定时器
				i=UARTCtrl->AutoSearchTime;//自动搜表持续时间分，0表示不限时间直至搜表结束
				if(i==0)
				{
					i=65535;
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
				UARTCtrl->AutoSearchTask=4+1;//自动搜表任务=4://激活从节点主动注册
				return;
//			#else
//				Terminal_Router->RS485Time=0x03;//采集器搜485电表定时值分(BCD)
//				Terminal_Router->KeepTime=10;//注册持续时间定时值分
//			#endif
			}
			else
			{//每日执行
				Terminal_Router->n=0;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Router->RS485Time=0;//0x60;//采集器搜485电表定时值分(BCD)
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//到23:00:00的时间
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask++;//自动搜表任务
	#endif
			break;
		case 3://启动广播(采集器搜485电表)
			MR((u32)p8tx,(u32)TC_Example1,sizeof(TC_Example1));
			i=Terminal_Router->RS485Time;
			MWR(i,((u32)p8tx)+25,2);//采集器搜485电表的时间
			i=bcd_hex(i);
			i*=60;
			i+=10;//加10秒
			Terminal_Ram->AutoSearch_S_Timer=i;//28 载波自动搜表秒定时器
			DL645_68H_Add33H_CS_16H(p8tx+15);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
			UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
			p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
			Uart_3762_CS((u32)p8tx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=sizeof(TC_Example1);//发送字节数(半字对齐)
//			浙江要求自动搜表，在测每日20:00-23：00搜表时，偶尔会遇到进入20:00后马上液晶提示搜表结束，
//			分析原因为发送启动广播后模块没有在超时时间内回确认帧，发送后接收超时值UARTCtrl->TxToRxTimerOver可能时间过短但不知在哪付的值，故这里重新赋值。
//			其他地方各个版本搜表也有该情况，但都没有要求用自动搜表，所以该地方只修改浙江，其他地方若有要求搜表再改。
			UARTCtrl->TxToRxTimerOver=60000/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->Task=3;//等待发送
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 4://接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					UARTCtrl->AutoSearchTask++;//自动搜表任务
					break;
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 5://激活从节点主动注册
			if(Terminal_Ram->AutoSearch_S_Timer!=0)//28 载波自动搜表秒定时器
			{//启动广播(采集器搜485电表)定时完成
				break;
			}
			MR(((u32)p8tx)+128,ADDR_MYMDHMS,6);//开始时间	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8tx)+128+6,2);//持续时间(min) 分
			MWR(0x0101,((u32)p8tx)+128+6+2,2);//从节点重发次数	BIN	1,随机等待时间片个数(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 6://接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					UARTCtrl->AutoSearchTask++;//自动搜表任务
					break;
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 7://自动搜表接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//超时定时
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 载波自动搜表秒定时器
			{
				UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask++;//自动搜表任务
				break;
			}
			break;
		case 8://读路由运行状态
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 9://读路由运行状态接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
	#if (USER/100)!=0
			if(Terminal_Ram->AutoSearch_M_Timer==0)//2 自动搜表允许时间分定时器
			{
				goto AutoSearch_End;//自动搜表结束
			}
	#endif
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
			#if (USER/100)==0
			//送检模式
					i=p8rx[13];
					i&=2;
					if(i==0)
					{//运行状态字:D1=工作标志(0为停止工作,1=正在工作),D0=路由学习(0=未完成,1=学习完成)
						goto AutoSearch_End;//自动搜表结束
					}
			#else
			//正常功能
						if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微
						{//是鼎信
							i=MRR(((u32)p8rx)+13+13,3);
							if(i==0x080808)
							{//路由三相空闲即三相工作步骤均为08时,自动搜表结束
								i=0;
							}
							else
							{
								i=2;
							}
						}
						else
						{//其他厂家
							i=p8rx[13];
							i&=2;
						}
						if(i==0)
						{//运行状态字:D1=工作标志(0为停止工作,1=正在工作),D0=路由学习(0=未完成,1=学习完成)
							switch(Terminal_Router->n)
							{
								case 1://立即执行第1轮结束
									Terminal_Router->n=2;//轮次1-3=立即搜表,0=每日搜表
									Terminal_Router->RS485Time=0;//0x10;//采集器搜485电表定时值分(BCD)
									Terminal_Router->KeepTime=20;//注册持续时间定时值分
									UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
									return;
								case 2://立即执行第2轮结束
									Terminal_Router->n=3;//轮次1-3=立即搜表,0=每日搜表
									Terminal_Router->RS485Time=0;//0x17;//采集器搜485电表定时值分(BCD)
									Terminal_Router->KeepTime=12*60;//注册持续时间定时值分
									UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
									return;
								case 0://每日执行结束
								default://立即执行第3轮结束
								#if (USER/100)!=0
									if(UARTCtrl->AutoSearchTime)//自动搜表持续时间分，0表示不限时间直至搜表结束
									{
										if(Terminal_Ram->AutoSearch_M_Timer>17)//2 自动搜表允许时间分定时器
										{
											Terminal_Router->RS485Time=0;//0x17;//采集器搜485电表定时值分(BCD)
											Terminal_Router->KeepTime=12*60;//注册持续时间定时值分
											UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
											return;
										}
									}
								#endif
									goto AutoSearch_End;//自动搜表结束
							}
						}
			#endif
					Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchTask=6+1;//自动搜表任务
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
			
		case 100://终止主动注册
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 101://终止主动注册接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//自动搜表结束
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			
AutoSearch_End://自动搜表结束
			WarningOccur(PORTn,(u8 *)"载波自动搜表结束");//告警发生,入口告警代码和告警字符串
			UARTCtrl->TaskID=0;
			UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=开始,2...过程

			//解除锁定
			UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
		#if (USER/100)==0//电科院送检
			Terminal_Router->StopFlags&=~(1<<13);//暂停路由
			//Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
		#else
//			//复位模块
//			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}


//自动搜表3762-东软
const u8 ES_Example1[]=//广播启动采集器搜表
{0x68,35,0x00,0x41,0x00,0x00,0x28,0x64,0x00,0x00,0x05,0x04,0x00,0x80,0x20,0x01,16,0x68,0x99,0x99,0x99,0x99,0x99,0x99,0x68,0x1F,0x04,0xAB,0xAB,0xAB,0xAB,0x00,0x16,0x90,0x16,};


void AutoSearchMeter_3762_ES(u32 PORTn)//自动搜表3762-东软
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8tx;
	u8 * p8rx;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//自动搜表任务:0=空,1=等待,2...过程
	{
		case 1://等待
			UARTCtrl->AutoSearchTask++;//自动搜表任务:0=空,1=等待,2...过程
			break;
		case 2://暂停路由
			WarningOccur(PORTn,(u8 *)"载波正在自动搜表");//告警发生,入口告警代码和告警字符串
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			Terminal_Router->StopFlags|=(1<<13);//暂停路由
			Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
			UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
	#if (USER/100)==0
		//送检模式
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
				Terminal_Router->KeepTime=120;//注册持续时间定时值分
			}
			else
			{//每日执行
				Terminal_Router->n=0;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Router->RS485Time=0;//30;//采集器搜485电表定时值分(HEX),最小为12，最高建议不超过120
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x223000,ADDR_DATABUFF+6,3);//到23:00:00的时间,其中30分是启动采集器搜485电表定时
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask=4+1;//自动搜表任务
	#else
		//正常功能
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
//			#if RMM_Type==0//集中抄表类型:0=载波表,1=RS485表通过采集器,2=混合
				Terminal_Router->RS485Time=0;//采集器搜485电表定时值分(BCD)
				Terminal_Ram->AutoSearch_S_Timer=0;//28 载波自动搜表秒定时器
				i=UARTCtrl->AutoSearchTime;//自动搜表持续时间分，0表示不限时间直至搜表结束
				if(i==0)
				{
					i=65535;
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
				UARTCtrl->AutoSearchTask=4+1;//自动搜表任务=4://激活从节点主动注册
				return;
//			#else
//				Terminal_Router->RS485Time=15;//采集器搜485电表定时值分(HEX),最小为12，最高建议不超过120
//				Terminal_Router->KeepTime=15;//注册持续时间定时值分
//			#endif
			}
			else
			{//每日执行
				Terminal_Router->n=0;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Router->RS485Time=0;//30;//采集器搜485电表定时值分(HEX),最小为12，最高建议不超过120
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x223000,ADDR_DATABUFF+6,3);//到23:00:00的时间,其中30分是启动采集器搜485电表定时
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask++;//自动搜表任务
	#endif
			break;
		case 3://启动广播(采集器搜485电表)
			MR((u32)p8tx,(u32)ES_Example1,sizeof(ES_Example1));
			i=Terminal_Router->RS485Time;
			MWR(i,((u32)p8tx)+14,2);//采集器搜485电表的时间
			i*=60;
			i+=10;//加10秒
			Terminal_Ram->AutoSearch_S_Timer=i;//28 载波自动搜表秒定时器
			DL645_68H_Add33H_CS_16H(p8tx+17);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
			UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
			p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
			Uart_3762_CS((u32)p8tx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=sizeof(ES_Example1);//发送字节数(半字对齐)
//			浙江要求自动搜表，在测每日20:00-23：00搜表时，偶尔会遇到进入20:00后马上液晶提示搜表结束，
//			分析原因为发送启动广播后模块没有在超时时间内回确认帧，发送后接收超时值UARTCtrl->TxToRxTimerOver可能时间过短但不知在哪付的值，故这里重新赋值。
//			其他地方各个版本搜表也有该情况，但都没有要求用自动搜表，所以该地方只修改浙江，其他地方若有要求搜表再改。
			UARTCtrl->TxToRxTimerOver=60000/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->Task=3;//等待发送
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 4://接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
	#if (USER/100)==0
		//送检模式
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
	#else
		//正常功能
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					UARTCtrl->AutoSearchTask++;//自动搜表任务
					break;
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			goto AutoSearch_End;//自动搜表结束
	#endif
		case 5://激活从节点主动注册
			if(Terminal_Ram->AutoSearch_S_Timer!=0)//28 载波自动搜表秒定时器
			{//启动广播(采集器搜485电表)定时完成
				break;
			}
			MR(((u32)p8tx)+128,ADDR_MYMDHMS,6);//开始时间	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8tx)+128+6,2);//持续时间(min) 分
			MWR(0x0101,((u32)p8tx)+128+6+2,2);//从节点重发次数	BIN	1,随机等待时间片个数(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 6://接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					UARTCtrl->AutoSearchTask++;//自动搜表任务
					break;
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 7://自动搜表接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//超时定时
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 载波自动搜表秒定时器
			{
				UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask++;//自动搜表任务
				break;
			}
			break;
		case 8://读路由运行状态
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 9://读路由运行状态接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
	#if (USER/100)!=0
			if(Terminal_Ram->AutoSearch_M_Timer==0)//2 自动搜表允许时间分定时器
			{
				goto AutoSearch_End;//自动搜表结束
			}
	#endif
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
			#if (USER/100)==0
			//送检模式
					i=p8rx[13];
					i&=2;
					if(i==0)
					{//运行状态字:D1=工作标志(0为停止工作,1=正在工作),D0=路由学习(0=未完成,1=学习完成)
						goto AutoSearch_End;//自动搜表结束
					}
			#else
			//正常功能
					i=p8rx[13];
					i&=2;
					if(i==0)
					{//运行状态字:D1=工作标志(0为停止工作,1=正在工作),D0=路由学习(0=未完成,1=学习完成)
						switch(Terminal_Router->n)
						{
							case 1://立即执行第1轮结束
								Terminal_Router->n=2;//轮次1-3=立即搜表,0=每日搜表
								Terminal_Router->RS485Time=0;//30;//采集器搜485电表定时值分(HEX),最小为12，最高建议不超过120
								Terminal_Router->KeepTime=30;//注册持续时间定时值分
								UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
								return;
							case 2://立即执行第2轮结束
								Terminal_Router->n=3;//轮次1-3=立即搜表,0=每日搜表
								Terminal_Router->RS485Time=0;//60;//采集器搜485电表定时值分(HEX),最小为12，最高建议不超过120
								Terminal_Router->KeepTime=12*60;//注册持续时间定时值分
								UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
								return;
							case 0://每日执行结束
							default://立即执行第3轮结束
							#if (USER/100)!=0
								if(UARTCtrl->AutoSearchTime)//自动搜表持续时间分，0表示不限时间直至搜表结束
								{
									if(Terminal_Ram->AutoSearch_M_Timer>17)//2 自动搜表允许时间分定时器
									{
										Terminal_Router->RS485Time=0;//0x17;//采集器搜485电表定时值分(BCD)
										Terminal_Router->KeepTime=12*60;//注册持续时间定时值分
										UARTCtrl->AutoSearchTask=2+1;//启动广播(采集器搜485电表)
										return;
									}
								}
							#endif
								goto AutoSearch_End;//自动搜表结束
						}
					}
			#endif
					Terminal_Ram->AutoSearch_S_Timer=1*60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchTask=6+1;//自动搜表任务
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 100://终止主动注册
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 101://终止主动注册接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//自动搜表结束
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			
AutoSearch_End://自动搜表结束
			WarningOccur(PORTn,(u8 *)"载波自动搜表结束");//告警发生,入口告警代码和告警字符串
			UARTCtrl->TaskID=0;
			UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=开始,2...过程
			//解除锁定
			UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
		#if (USER/100)==0//电科院送检
			Terminal_Router->StopFlags&=~(1<<13);//暂停路由
			//Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
		#else
//			//复位模块
//			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}

void AutoSearchMeter_3762_01(u32 PORTn)//自动搜表3762-瑞斯康
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8tx;
	u8 * p8rx;
	u16 *p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->AutoSearchTask)//自动搜表任务:0=空,1=等待,2...过程
	{
		case 1://等待
			UARTCtrl->AutoSearchTask++;//自动搜表任务:0=空,1=等待,2...过程
			break;
		case 2://暂停路由
			WarningOccur(PORTn,(u8 *)"载波正在自动搜表");//告警发生,入口告警代码和告警字符串
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			Terminal_Router->StopFlags|=(1<<13);//暂停路由
			Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
			UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
	#if (USER/100)==0
		//送检模式
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
				Terminal_Router->KeepTime=120;//注册持续时间定时值分
			}
			else
			{//每日执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Ram->AutoSearch_S_Timer=60;//28 载波自动搜表秒定时器
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//到23:00:00的时间
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask=4+1;//自动搜表任务
	#else
		//正常功能
			i=UARTCtrl->TaskID;//方案号3=实时启动搜表
			i&=0xff;
			if(i==3)//立即执行
			{//立即执行
				Terminal_Router->n=1;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Router->RS485Time=0;//0x03;//采集器搜485电表定时值分(BCD)
//			#if RMM_Type==0//集中抄表类型:0=载波表,1=RS485表通过采集器,2=混合
				i=UARTCtrl->AutoSearchTime;//自动搜表持续时间分，0表示不限时间直至搜表结束
				if(i==0)
				{
					i=65535;
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
//			#else
//				Terminal_Router->KeepTime=12*60;//注册持续时间定时值分
//			#endif
			}
			else
			{//每日执行
				Terminal_Router->n=0;//轮次1-3=立即搜表,0=每日搜表
				Terminal_Router->RS485Time=0;//0x60;//采集器搜485电表定时值分(BCD)
				MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
				MR(ADDR_DATABUFF+6,ADDR_MYMDHMS,6);
				MWR(0x230000,ADDR_DATABUFF+6,3);//到23:00:00的时间
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{
					i=0;
				}
				i/=60;
				if(i>120)
				{
					i=120;
				}
				if(i==0)
				{
					goto AutoSearch_End;//自动搜表结束
				}
				Terminal_Router->KeepTime=i;//注册持续时间定时值分
			}
			UARTCtrl->AutoSearchTask++;//自动搜表任务
	#endif
			break;
		case 3://启动广播(采集器搜485电表)
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 4://接收
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 5://激活从节点主动注册
			MW(ADDR_MYMDHMS,((u32)p8tx)+128,6);//开始时间	BCD	6
			i=Terminal_Router->KeepTime;
			MWR(i,((u32)p8tx)+128+6,2);//持续时间(min) 分
			//从节点重发次数	BIN	1,随机等待时间片个数(150ms)	BIN	1
			Uart_3762_TxData(PORTn,0x11,5,10);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 6://接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					Terminal_Ram->AutoSearch_S_Timer=10*60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					UARTCtrl->AutoSearchTask++;//自动搜表任务
					break;
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 7://自动搜表接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->RxByte=0;
			p16timer[0]=1000/10;//超时定时
			if(Terminal_Ram->AutoSearch_S_Timer==0)//28 载波自动搜表秒定时器
			{
				UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask++;//自动搜表任务
				break;
			}
			break;
		case 8://读路由运行状态
			Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 9://读路由运行状态接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
	#if (USER/100)!=0
			if(Terminal_Ram->AutoSearch_M_Timer==0)//2 自动搜表允许时间分定时器
			{
				goto AutoSearch_End;//自动搜表结束
			}
	#endif
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrl->AutoSearchReCount=0;//自动搜表任务重发计数
					i=p8rx[13];
					i&=2;
					if(i==0)
					{//运行状态字:D1=工作标志(0为停止工作,1=正在工作),D0=路由学习(0=未完成,1=学习完成)
						
						goto AutoSearch_End;//自动搜表结束
					}
					Terminal_Ram->AutoSearch_S_Timer=1*60;//28 载波自动搜表秒定时器
					UARTCtrl->AutoSearchTask=6+1;//自动搜表任务
					break;
				}
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			goto AutoSearch_End;//自动搜表结束
		case 100://终止主动注册
			Uart_3762_TxData(PORTn,0x11,6,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
			UARTCtrl->AutoSearchTask++;//自动搜表任务
			break;
		case 101://终止主动注册接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			if(UARTCtrl->Task==1)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)
				{//AFN=0x00,Fn=1
					goto AutoSearch_End;//自动搜表结束
				}
				p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->AutoSearchReCount<2)//自动搜表任务重发计数
			{
				UARTCtrl->AutoSearchReCount++;//自动搜表任务重发计数
				UARTCtrl->AutoSearchTask--;//自动搜表任务
				break;
			}
			
AutoSearch_End://自动搜表结束
			WarningOccur(PORTn,(u8 *)"载波自动搜表结束");//告警发生,入口告警代码和告警字符串
			UARTCtrl->TaskID=0;
			UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=开始,2...过程
			//解除锁定
			UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
		#if (USER/100)==0//电科院送检
			Terminal_Router->StopFlags&=~(1<<13);//暂停路由
			//Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
		#else
//			//复位模块
//			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		#endif
			break;
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}
#endif

//0x68
//总长度L
//总长度L
//控制码
//信息域R-1
//信息域R-2
//信息域R-3
//信息域R-4
//信息域R-5
//信息域R-6
//地址域A1,A2,A3
//应用功能码AFN
//数据单元标识DT1
//数据单元标识DT2
//数据单元
//CS
//0x16








