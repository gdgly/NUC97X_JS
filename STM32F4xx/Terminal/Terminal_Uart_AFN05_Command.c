
//UART 控制命令
#include "Project.h"
#include "Terminal_Uart_AFN05_Command.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Contrl_Remote.h"

#include "Terminal_Statistic.h"
#include "../Device/RTC.h"


extern TerminalCommandUartLib_TypeDef	TerminalCommandUartLib[];


void Uart_AFN05(u32 PORTn)//控制命令
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 f;
	
	u8 * p8s;
	u8 * p8d;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8* p8;

  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
		goto err;
	}
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+16+6))
		{
			goto err;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
//PW认证
	i=Uart_376_PW(PORTn);//消息认证码字段PW;入口PW首址,出口0有效,1无效,4已返错误帧
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
	}
/*
//若红外口需设置开关
	if(PORTn==IRPORT)
	{
    if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_MeterFMTimer,2)==0))
		{
			goto err;
		}
	}
*/
	
	f=0;//f=0检查全部数据是否全部有效,f=1写入
loop:
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识和参数数据长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+16+6);
	}
	else
	{
		DIlen-=(8+16);
	}
	if(DIlen<4)
	{
		goto err;
	}
//数据(启动站功能码10（请求1级数据）用于应用层请求确认（CON=1）的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			p8s=(u8 *)(RXaddr);
			if(Fn==0x0)
			{
				goto err;
			}
		#if ((USER/100)==5)//上海
			if(Fn==201)
			{
				goto AFN05F201;
			}
		#endif
			if(Fn>Get_AFN05CommandLib_MaxFn())
			{
				goto err;
			}
			if(Pn>TerminalCommandUartLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalCommandUartLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalCommandUartLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//本方案无此Fn功能
				goto err;
			}
			switch(Fn)//Fn
			{
				case 31://对时命令
			#if ((USER/100)==5)//上海
				AFN05F201:
			#endif
					if(f==0x0)
					{
					#if ((USER/100)==5)//上海
						if(Fn==31)
						{
							if((DIlen-4)<6)
							{
								goto err;
							}
						}
						if(Fn==201)
						{
							if((DIlen-4)<8)
							{
								goto err;
							}
						}
					#else
						if((DIlen-4)<6)
						{
							goto err;
						}
					#endif
					}
					else
					{
						p8d=(u8 *)(ADDR_RTCBUFF);
						p8d[0]=p8s[0];
						p8d[1]=p8s[1];
						p8d[2]=p8s[2];
						p8d[3]=p8s[4]>>5;
						if(p8d[3]==7)
						{
							p8d[3]=0;
						}
						p8d[4]=p8s[3];
						p8d[5]=p8s[4]&0x1f;
						p8d[6]=p8s[5];
						if(Check_HMS(MRR((u32)p8d,3))!=0)//时分秒合法检查,返回0合法,1非法
						{
							goto err;
						}
						if(Check_YMD(MRR(((u32)p8d)+4,3))!=0)//年月日合法检查,返回0合法,1非法
						{
							goto err;
						}
						
//            WriteLPCRTC(ADDR_RTCBUFF);//写内部RTC
//            SyncLPCRTC();//同步内部RTC(读内部写外部)
						WriteRTC(ADDR_RTCBUFF);//写外部硬实时钟
						
						Comm_Ram->MS_Task=0;//下步即读RTC
						Comm_Ram->ReadRTCTimer=0;//25 读ERTC每10MS减1定时器
						Terminal_Ram->I2SUBTask1=0;//下步即算实时数据RTC
						
						//对时事件记录
						MWR(0,ADDR_DATABUFF+2,2);//测量点号
						MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//对时前时间
						MR(ADDR_DATABUFF+10,(u32)p8s,6);//对时后时间
						ERC41_Event(ADDR_DATABUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
						
						//主站下发对时命令给终端时，终端应将跳过的时间段的电压统计为合格；对时跳过的时间段应计入监测时间
						MR(ADDR_DATABUFF,ADDR_DATABUFF+4,12);//对时前时间,对时后时间
						p8d=(u8 *)(ADDR_DATABUFF);
						p8d[4]&=0x1f;//对时前时间去月上的星期
						p8d[6+4]&=0x1f;//对时后时间去月上的星期
						x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
						if((x>>31)==0)
						{//正值
							i=MRR(ADDR_DATABUFF+3,3);
							i=YMD_Add1D(i);//对时前时间年月日加1日,返回加1后的年月日
							MC(0,ADDR_DATABUFF+12,3);
							MWR(i,ADDR_DATABUFF+15,3);
							i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
							if(x>i)
							{
								x=i;//用当日到24点差
							}
							x/=60;//秒变为分
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
							//A相电压合格日累计时间
								y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
							//B相电压合格日累计时间
								y=MRR(ADDR_AFN04F25+(i*11)+10,1);//取测量点基本参数的电源接线方式
								if(y!=1)
								{//不是三相三线
									y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
									y+=x;
									if(y>0xffff)
									{
										y=0xffff;
									}
									MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
								}
							//C相电压合格日累计时间
								y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
							}
							x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
							i=MRR(ADDR_DATABUFF+4,2);
							i=YM_Add1M(i);//年月加1月,返回加1后的年月
							MC(0,ADDR_DATABUFF+12,3);
							MWR(1,ADDR_DATABUFF+15,1);
							MWR(i,ADDR_DATABUFF+16,2);
							i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
							if(x>i)
							{
								x=i;//用当月未(30或31日)24点差
							}
							x/=60;//秒变为分
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
							//A相电压合格月累计时间
								y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
							//B相电压合格月累计时间
								y=MRR(ADDR_AFN04F25+(i*11)+10,1);//取测量点基本参数的电源接线方式
								if(y!=1)
								{//不是三相三线
									y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
									y+=x;
									if(y>0xffff)
									{
										y=0xffff;
									}
									MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
								}
							//C相电压合格月累计时间
								y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
							}
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
								AFN0DF27F35_Computer();//终端统计.电压统计数据
							}
						}
						
					}
				#if ((USER/100)==5)//上海
					if(Fn==31)
					{
						RXaddr+=6;
						DIlen-=6;
					}
					if(Fn==201)
					{
						if(f==1)//f=0检查全部数据是否全部有效,f=1写入
						{
							MW(RXaddr,ADDR_AFN05F201+1,8);
							i=0;
	 						//RS485-1
							if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)!=0))//终端抄表
							{
								i|=1;
							}
							//RS485-2
							if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)!=0)//终端抄表
							{
		 						i|=(1<<1);
							}
	
						#if ((Project/100))==2//集中器
							//载波
	 						i|=(1<<3);
						#endif
							MWR(i,ADDR_AFN05F201,1);
						}
						RXaddr+=8;
						DIlen-=8;
					}
				#else
					RXaddr+=6;
					DIlen-=6;
				#endif
					continue;
				case 32://中文信息
					if(f==0x0)
					{
						if((DIlen-4)<(p8s[1]+2))
						{
							goto err;
						}
					}
					else
					{
						MW(RXaddr,TerminalCommandUartLib[Fn-1].Addr+(((p8s[0]>>4)&0x1)*202*16)+((p8s[0]&0x0f)*202),p8s[1]+2);	
					}
					RXaddr+=p8s[1]+2;
					DIlen-=p8s[1]+2;
					continue;
				case 49://命令指定通信端口暂停抄表
				case 50://命令指定通信端口恢复抄表
				case 51://命令指定通信端口重新抄表
				case 52://初始化指定通信端口下的全部中继路由信息
				case 53://删除指定通信端口下的全部电表
					if(f==0x0)
					{
						if((DIlen-4)<1)
						{
							goto err;
						}
						if(p8s[0]>31)
						{
							goto err;
						}
						if(p8s[0]==0x0)
						{
							goto err;
						}
					}
					else
					{
						MC(TerminalCommandUartLib[Fn-1].Command,TerminalCommandUartLib[Fn-1].Addr+p8s[0]-1,1);
					}
					RXaddr+=1;
					DIlen-=1;
					continue;
				case 57://删除指定事件代码ERC下的全部事件
					if(f==0x0)
					{
						if((DIlen-4)<1)
						{
							goto err;
						}
					}
					else
					{
						if(p8s[0]==0)
						{//n=0，则清空所有事件代码ERC 对应的事件
							MC(0,ADDR_AFN0EF3,MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
						}
						else
						{
							for(i=0;i<p8s[0];i++)
							{
								x=p8s[1+i];//要清0的ERC代码
								if((x==0)||(x>MaxERC))
								{
									continue;
								}
								x--;
								MC(0,ADDR_AFN0EF3+(x*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2+(MaxClassEventNum*LEN_TERMINAL_EVENT));
							}
						}
//----终端事件分类立即计算，电科院测试软件可能马上招读AFN0CF245---
						p8=(u8*)(ADDR_AFN0CF245);//终端事件分类
						x=0;//终端当前事件类型总数n
						for(i=0;i<MaxERC;i++)
						{
							y=ADDR_AFN0EF3+(i*(2+(15*LEN_TERMINAL_EVENT)));
							y=MRR(y,2);
							if(y>15)
							{
								y=15;
							}
							if(y!=0)
							{
								p8[1+(2*x)+0]=i+1;//事件代码ERC
								p8[1+(2*x)+1]=y;//事件数量
								x++;//终端当前事件类型总数n
							}
						}
						p8[0]=x;//总数n
//---终端事件分类立即计算 END---
					}
					i=p8s[0];
					i++;
					RXaddr+=i;
					DIlen-=i;
					continue;
					
				default:
					i=TerminalCommandUartLib[Fn-1].Len;
					break;
			}
			if(TerminalCommandUartLib[Fn-1].Addr==0x0)
			{
				goto err;
			}
			if(TerminalCommandUartLib[Fn-1].Command!=0x0)
			{//不是解除命令判定长度
				if((DIlen-4)<i)
				{
					goto err;
				}
			}
			if(f==0x1)
			{
				p8s=(u8 *)(RXaddr);
				p8d=(u8 *)(ADDR_DATABUFF);
				p8d[0]=TerminalCommandUartLib[Fn-1].Command;
				if(TerminalCommandUartLib[Fn-1].Command!=0x0)
				{//不是解除命令
					for(x=0;x<i;x++)
					{
						p8d[1+x]=p8s[x];
					}
				}
				else
				{//是解除命令
					for(x=0;x<i;x++)
					{
						p8d[1+x]=0;
					}
				}
				if(Pn==0x0)
				{
					MW(ADDR_DATABUFF,TerminalCommandUartLib[Fn-1].Addr,i+1);
				}
				else
				{
					MW(ADDR_DATABUFF,TerminalCommandUartLib[Fn-1].Addr+((Pn-1)*(i+1)),i+1);
				}
				switch(Fn)
				{
					case 1://遥控跳闸(电科院测试时发本命令后只有1秒延时即发F2允许合闸,终端控制循环中只有1秒不能急时处理,故在此先运行几次能记录遥控跳闸次数)
						for(x=0;x<4;x++)
						{
							Terminal_Remote_Control();//终端控制-遥控跳闸
						}
						break;
				#if (USER/100)==6//用户:湖南
					case 103://启动集中器搜索未知电表(湖南系扩展)
						x=MRR(ADDR_AFN05F103+3,1);
						if(x&1)//执行标志：D0位=1清除上次搜索结果,=0保留上次搜索结果
						{
							MC(0,ADDR_AFN0CF13,2);//搜索到的电表信息
						}
						//修改AFN05F149的自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
						if(x&2)//执行标志：D1位=1未知电表直接加入电表档案中,=0不自动加入
						{
							MWR(0x555555550100,ADDR_AFN05F149,6);
							
						}
						else
						{
							MWR(0x555555550200,ADDR_AFN05F149,6);
						}
						break;
				#endif
					case 149://电能表通信参数自动维护功能开启与关闭(各个端口需分别各自进行)
				#if (USER/100)==6//用户:湖南
						MWR((12*60)<<8,ADDR_AFN05F103,3);//清F103启动标志,同时设搜索允许的最大时间=12*60分
				#endif
						x=MRR(ADDR_AFN05F149+1,1);
						if((x==1)||(x==2))//自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
						{
							x=0x55555555;
							MC(0,ADDR_AFN0CF13,2);//搜索到的电表信息
						}
						else
						{
							x=0;
						}
						MWR(x,ADDR_AFN05F149+2,4);
						break;
				}
			}
			if(TerminalCommandUartLib[Fn-1].Command!=0x0)
			{//不是解除命令减长度
				RXaddr+=i;
				DIlen-=i;
			}
		}
	}
	if(f==0x0)
	{
		f=1;
		goto loop;
	}
	else
	{
		//全部确认
		Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
	}
}



