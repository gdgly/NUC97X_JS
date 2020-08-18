
//自动搜表
#include "Project.h"
#include "AutoSearchMeter.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "../Terminal/Terminal_Init.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0C_RealTimeData_Init.h"
#include "Terminal_AFN0D_CongealData_Init.h"


void AutoUpdataAFN04F10ERC3(void)//自动AFN04F10的电能表配置的ERC3事件
{
	u8 *p8;
	
	if(Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer==0)//26 自动更新F10主动上报ERC3事件秒定时器;集中器通过本地的自动发现，完成测量点新增、变更或删除后，应立即置测量点参数变更标志（F150参数）相应位为1，并立即上报参数变更事件（ERC3）到主站,除非进行了测量点参数变更标志的复位，否则，每次上报参数变更事件后的m分钟（m=0~30，一般取10）之内，无论参数是否发生变更均不再次上报
	{
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0;//0=无,1=有AFN04F10更新但在10分内不再上报情况
		Terminal_Ram->AutoUpdataAFN04F10ERC3_S_Timer=10*60;//在10分内不再上报
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=3;//ERC
		p8[1]=14;//长度Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
		p8[7]=0;//启动站地址
		p8[8]=0;//Pn=0
		p8[9]=0;
		p8[10]=0x2;//Fn=10
		p8[11]=1;
		p8[12]=0;//Pn=0
		p8[13]=0;
		p8[14]=0x20;//Fn=150
		p8[15]=18;
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1无
	}
	else
	{
		Terminal_Ram->AutoUpdataAFN04F10ERC3=0x55;//0=无,1=有AFN04F10更新但在10分内不再上报情况
	}
}
/*
void AutoDelAFN04F10(void)//自动删除AFN04F10的电能表配置;用Terminal_ReadMeter_3762.c中的代替void DateRMFlags(u32 Pn,u32 f)//日抄表标志;入口:f=0成功,=1失败
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	
	i=MRR(ADDR_AFN05F149+1,1);//自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
	if(i==0x1)
	{	
		if(Terminal_Ram->AutoUpdataAFN04F10_S_Timer==0)//25 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
		{
			for(Pn=0;Pn<AFN04F10MaxPn;Pn++)
			{
				i=MRR(ADDR_RMDateFlags+(4*Pn),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
				i=~i;
				i=BitNum(i);//计算置位为1的个数(0-32)
				if(i<=2)
				{//连续30天抄表失败
					MC(0,ADDR_AFN04F10+(LEN_AFN04F10_Pn*Pn)+2,2);//所属测量点号=0
					//测量点有效标志
				#if AFN04F150Dn==1//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
					Pn++;
					if(Pn>=2048)
					{
						return;
					}
				#endif
					x=Pn/8;
					i=Pn%8;
					y=MRR(ADDR_AFN04F150+x,1);
					y&=(~(1<<i));
					MWR(y,ADDR_AFN04F150+x,1);
					//测量点参数变更标志
					y=MRR(ADDR_AFN04F150+256+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+256+x,1);
					//ERC3参数变更事件
					AutoUpdataAFN04F10ERC3();//自动AFN04F10的电能表配置的ERC3事件
				}
			}
		}
	}
}
*/
u32 Scand_AFN04F10(u32 PORTn,u64 MeterAddr,u64 CollectorAddr)//检查搜索到的电表信息的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
{
	u32 i;
	u32 Addr;
	u32 Meter;
	u64 d64;

	Addr=ADDR_AFN04F10;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr+2,2);
		if(i!=0)
		{//测量点号有效
			i=MRR(Addr+4,1);
			i&=0x1f;
			if(i==PORTn)
			{//端口号相同
					d64=MRR(Addr+6,6);
					if(d64==MeterAddr)
					{//通信地址相同
						if(CollectorAddr==0xeeeeeeeeeeee)
						{//空采集器地址
							return Meter;//相同的配置序号
						}
						d64=MRR(Addr+20,6);
						if(d64==CollectorAddr)
						{//采集器地址相同
							return Meter;//相同的配置序号
						}
					}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;

}

u32 Scand_AFN0CF13(u32 PORTn,u64 MeterAddr,u64 CollectorAddr)//检查搜索到的电表信息的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
{
	u32 i;
	u32 x;
	u32 Addr;
	u32 Meter;
	u64 d64;
	
	x=MRR(ADDR_AFN0CF13,2);
	if(x>MaxRS485AddCarrierPn)
	{
		x=MaxRS485AddCarrierPn;
	}
	Addr=ADDR_AFN0CF13;
	for(Meter=1;Meter<=x;Meter++)
	{
		i=MRR(Addr+13,1);
		i&=0x1f;
		if(i==PORTn)
		{//端口号相同
				d64=MRR(Addr,6);
				if(d64==MeterAddr)
				{//通信地址相同
					if(CollectorAddr==0xeeeeeeeeeeee)
					{//空采集器地址
						return Meter;//相同的配置序号
					}
					d64=MRR(Addr+7,6);
					if(d64==CollectorAddr)
					{//采集器地址相同
						return Meter;//相同的配置序号
					}
				}
		}
		Addr+=LEN_PerMeterAFN0CF13;
	}
	return 0;
}

void AutoSetAFN04F10(u32 PORTn,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase)//自动设置AFN04F10的电能表配置
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u32 F10;
	u32 F13;
	
	switch(PORTn)
	{
		case RS485_1PORT:
			PORTn=2;
			break;
		case RS485_2PORT:
			PORTn=3;
			break;
		case RS485_3PORT:
			PORTn=4;
			break;
		case RS485_4PORT://载波
			PORTn=31;
			break;
		default:
			return;
	}
	F10=Scand_AFN04F10(PORTn,MeterAddr,CollectorAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
	F13=Scand_AFN0CF13(PORTn,MeterAddr,CollectorAddr);//检查搜索到的电表信息的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
	if((F10|F13)==0)
	{
		MWR(MeterAddr,ADDR_DATABUFF+LEN_TERMINAL_EVENT,6);//电表通信地址
		i=Phase;//0为不确定，1～3依次表示相别为第1相、第2相、第3相
		if((i!=0)&&(i<=3))
		{
			i--;
			i=1<<i;
		}
		else
		{
			i=0;
		}
		i<<=5;//D7～D5按位依次表示电表实际相位C相、B相、A相：置"1"有效，置"0"无效;D3～D0编码表示接收到的载波信号品质，数值范围1～15
		MWR(i,ADDR_DATABUFF+LEN_TERMINAL_EVENT+6,1);//电表所在相别及发现者接收到的信号品质
		MWR(1,ADDR_DATABUFF+LEN_TERMINAL_EVENT+7,1);//电表通信协议,b)	D1～D0编码表示电表通信协议，数值范围0～3，0表示DL/T 645-1997，1表示DL/T 645-2007，2～3备用
		ERC35_Event(PORTn,1,ADDR_DATABUFF+LEN_TERMINAL_EVENT);//发现未知电表事件;入口Port=端口号,n=未知电表数,DataAddr=未知电表通信地址等每表8字节数据存放的地址
	}
	i=MRR(ADDR_AFN05F149+1,1);//电能表通信参数自动维护功能开启与关闭
	switch(i)
	{
		case 1://自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
			if(F10==0)
			{//无相同
//				if(Terminal_Ram->AutoUpdataAFN04F10_S_Timer==0)//25 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
//				{
					//只注册采集器
					if(MeterAddr==0xeeeeeeeeeeee)
					{
						//找相同采集地址
						x=ADDR_AFN04F10+2;
						for(i=0;i<AFN04F10MaxPn;i++)
						{
							y=MRR(x,2);
							if(y!=0)
							{//测量点号有效
								y=MRR(x+2,1);
								y&=0x1f;
								if(y==PORTn)
								{//端口号相同
									if(MRR(x+18,6)==CollectorAddr)
									{
										return;//原已存在
									}
								}
							}
							x+=LEN_AFN04F10_Pn;
						}
					}
					//找通信地址为0xeeeeeeeeeeee,采集器地址相同的配置(找到第1只表后替换原虚拟的测量点)
					i=Scand_AFN04F10(PORTn,0xeeeeeeeeeeee,CollectorAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
					if(i!=0)
					{
						i--;
						x=ADDR_AFN04F10+2+(i*LEN_AFN04F10_Pn);
					}
					else
					{
						//找通信地址相同的原位置(可能更换采集器)
						i=AFN04F10MaxPn;
						if(MeterAddr!=0xeeeeeeeeeeee)
						{
							x=ADDR_AFN04F10+2;
							for(i=0;i<AFN04F10MaxPn;i++)
							{
								y=MRR(x,2);
								if(y!=0)
								{//测量点号有效
									y=MRR(x+2,1);
									y&=0x1f;
									if(y==PORTn)
									{//端口号相同
										if(MRR(x+4,6)==MeterAddr)
										{
											break;//找到通信地址相同的原位置
										}
									}
								}
								x+=LEN_AFN04F10_Pn;
							}
						}
						//找测量点号为0的空位置
						if(i>=AFN04F10MaxPn)
						{
							x=ADDR_AFN04F10+2;
						#if (USER/100)==7//浙江用户
							if(PORTn==31)
							{
								x=ADDR_AFN04F10+2+10*LEN_AFN04F10_Pn;
								for(i=10;i<AFN04F10MaxPn;i++)
								{
									if(MRR(x,2)==0)
									{
										break;//找到测量点号为0的空位置
									}
									x+=LEN_AFN04F10_Pn;
								}
							}
							else
							{
								for(i=0;i<AFN04F10MaxPn;i++)
								{
									if(MRR(x,2)==0)
									{
										break;//找到测量点号为0的空位置
									}
									x+=LEN_AFN04F10_Pn;
								}
							}
						#else
							for(i=0;i<AFN04F10MaxPn;i++)
							{
								if(MRR(x,2)==0)
								{
									break;//找到测量点号为0的空位置
								}
								x+=LEN_AFN04F10_Pn;
							}
						#endif
						}
					}
					if(i>=AFN04F10MaxPn)
					{//没空了
						return;
					}
					Pn=i+1;
					MC(0,ADDR_DATABUFF,LEN_AFN04F10_Pn);
					MWR(Pn,ADDR_DATABUFF,2);//电能表/交流采样装置序号 BIN 2 
					MWR(Pn,ADDR_DATABUFF+2,2);//所属测量点号 BIN 2 
				#if (USER/100)==7//浙江用户
					if(Bps==0)
					{
						if(Protocol==1)
						{//DLT645-1997
							Bps=2;//1200
						}
						else
						{//DLT645-2007
							Bps=3;//2400
						} 
					}
				#endif
					MWR(PORTn+(Bps<<5),ADDR_DATABUFF+4,1);//通信速率及端口号 BIN 1 
					MWR(Protocol,ADDR_DATABUFF+5,1);//通信协议类型 BIN 1 
					MWR(MeterAddr,ADDR_DATABUFF+6,6);//通信地址 见附录A.12 6 
						//通信密码 BIN 6 
					MWR(4,ADDR_DATABUFF+18,1);//电能费率个数 BS8 1
					MWR(0x09,ADDR_DATABUFF+19,1);//有功电能示值整数位及小数位个数 BS8 1 
					MWR(CollectorAddr,ADDR_DATABUFF+20,6);//所属采集器通信地址 见附录A.12 6 
					MWR(DefaultClassNo,ADDR_DATABUFF+26,1);//用户大类号及用户小类号 BS8 1;(居民用户E类5)
					x-=2;
					MW(ADDR_DATABUFF,x,LEN_AFN04F10_Pn);
					MC(0,ADDR_RMDateFlags+(4*i),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
				
					//测量点有效标志
			#if AFN04F150Dn==1//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
					i++;
					if(i>=2048)
					{
						return;
					}
			#endif
					x=i/8;
					i%=8;
					y=MRR(ADDR_AFN04F150+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+x,1);
					PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
					//测量点参数变更标志
					y=MRR(ADDR_AFN04F150+256+x,1);
					y|=(1<<i);
					MWR(y,ADDR_AFN04F150+256+x,1);
					//ERC3参数变更事件
					AutoUpdataAFN04F10ERC3();//自动AFN04F10的电能表配置的ERC3事件
					
					//测量点数据初始化(同UART设置时的测量点号变化)
					Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
					Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
					Init_Class1Data(pnType,Pn);//类1数据初始化(指定Pn数据)
					Init_Class2Data(Pn);//类2数据初始化(指定的测量点数据,Pn=1~n)
					Init_Class3Data(Pn);//类3数据(事件)初始化(指定的测量点数据,Pn=1~n)
					Init_EnergyInc_Souser_Pn(Pn);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
					Init_Energy_Pn(Pn);//测量点日月电能量清为0(指定的Pn数据)
		#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==31)
					{
						Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
						Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
					}
		#endif
//				}
			}
			//break;//同时写入AFN04F10和AFN0CF13
		case 2://自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
			if(F13==0)
			{
				MC(0,ADDR_DATABUFF,LEN_PerMeterAFN0CF13);
				MWR(MeterAddr,ADDR_DATABUFF,6);//6BYTE 电能表地址
				MWR(Protocol,ADDR_DATABUFF+6,1);//1BYTE 通信协议类型
				MWR(CollectorAddr,ADDR_DATABUFF+7,6);//6BYTE 所属采集器通信地址
				MWR(PORTn,ADDR_DATABUFF+13,1);//1BYTE 端口号
				MWR(Phase,ADDR_DATABUFF+14,1);//1BYTE 电表相位
				
				z=MRR(ADDR_AFN0CF13,2);
				if(z>=MaxRS485AddCarrierPn)
				{
					z=0;//记录满，则清全部记录重录入以册除不存在的表
				}
				//只注册采集器
				if(MeterAddr==0xeeeeeeeeeeee)
				{
					//找相同采集地址
					x=ADDR_AFN0CF13+2;
					for(i=0;i<z;i++)
					{
						y=MRR(x+13,1);
						y&=0x1f;
						if(y==PORTn)
						{//端口号相同
							if(MRR(x+7,6)==CollectorAddr)
							{
								return;//原已存在
							}
						}
						x+=LEN_PerMeterAFN0CF13;
					}
				}
				//找通信地址为0xeeeeeeeeeeee,采集器地址相同的配置(找到第1只表后替换原虚拟的测量点)
				i=Scand_AFN0CF13(PORTn,0xeeeeeeeeeeee,CollectorAddr);//检查搜索到的电表信息的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(i!=0)
				{
					i--;
					x=ADDR_AFN0CF13+2+(i*LEN_PerMeterAFN0CF13);
					MW(ADDR_DATABUFF,x,LEN_PerMeterAFN0CF13);
				}
				else
				{
					//找通信地址相同的原位置(可能更换采集器)
					i=MaxRS485AddCarrierPn;
					if(MeterAddr!=0xeeeeeeeeeeee)
					{
						x=ADDR_AFN0CF13+2;
						for(i=0;i<z;i++)
						{
							y=MRR(x+13,1);
							y&=0x1f;
							if(y==PORTn)
							{//端口号相同
								if(MRR(x,6)==MeterAddr)
								{
									//更换采集器
									MW(ADDR_DATABUFF,ADDR_AFN0CF13+2+(i*LEN_PerMeterAFN0CF13),LEN_PerMeterAFN0CF13);
									return;//原已存在
								}
							}
							x+=LEN_PerMeterAFN0CF13;
						}
					}
					//原没存在
					MW(ADDR_DATABUFF,ADDR_AFN0CF13+2+(z*LEN_PerMeterAFN0CF13),LEN_PerMeterAFN0CF13);
					z++;
					MWR(z,ADDR_AFN0CF13,2);
				}
			}
			break;
	}
}


