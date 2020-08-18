
#include "Project.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "RouterBroadcastTime_Collector.h"

#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#if ((USER/100)!=7)&&((USER/100)!=8)//不是浙江、通用版
void RouterBroadcastTime_Collector(u32 PORTn)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
{
}
#else
void RouterBroadcastTime_Collector(u32 PORTn)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 d64;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	u32 BroadcastTime;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
		return;
	}
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	BroadcastTime=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);
	switch(BroadcastTime)//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
	{
		default:
			break;
		case 3://3=命令读
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Terminal_Router->StopFlags|=(1<<4);//暂停路由
				Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
				BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				WarningOccur(PORTn,(u8 *)"载波准备校时");//告警发生,入口告警代码和告警字符串
				MWR(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			}
			break;
		case 4://等待1分
			p16timer[0]=60000/10;
			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 5://通信延时相关广播通信时长查询
			if(p16timer[0]==0)
			{
				Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
				i=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+2,2);//所属测量点号 BIN 2
				if(i!=0)
				{
					x=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
					x&=0x1f;//端口号
					if(x==31)
					{
						i=MRR(ADDR_AFN0CF250_Ext1+10*(i-1)+5,2);
						if((i>>15)!=0)
						{//负
							i=~i;
							i++;
						}
						if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
						{//误差在阈值范围内无需进行校时
							BroadcastTime=10;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							break;
						}
						p8txbuff[4]=0x04;
						Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
						MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
						MR(((u32)p8txbuff)+16,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
						p8txbuff+=128;
						p8txbuff[0]=0;//通信协议类型：00H为透明传输；01H为 DL/T 645-1997；02H为 DL/T 645-2007；03H～FFH保留
						p8txbuff[1]=1;//										
						p8txbuff[2]=0;//										
			
						x=MRR(ADDR_AFN04F298_Ext,1);
						if(x==2)
						{//指定采集器地址的广播对时命令
							p8txbuff[3]=(22+14);//报文长度
							//报文头8字节地址域为对应的表地址
							p8txbuff[4]=0x68;
							MR(((u32)p8txbuff)+5,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
							p8txbuff[11]=0x68;
							//控制码1字节
							p8txbuff[12]=0x33;
							//1字节L=2+645报文长度
							p8txbuff[13]=24;
							//控制域2字节
							p8txbuff[14]=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
							p8txbuff[14]>>=5;
							if(p8txbuff[14]==0)
							{//默认
								if(MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+5,1)==1)
								{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007
									//97默认1200波特率
									p8txbuff[14]=2;
								}
								else
								{
									//07默认2400波特率
									p8txbuff[14]=3;
								}
							}
							p8txbuff[15]=0;
				
							//645报文
							MC(0xfe,((u32)p8txbuff)+2+14,4);
							p8txbuff[6+14]=0x68;
							MC(0x99,((u32)p8txbuff)+7+14,6);
							p8txbuff[13+14]=0x68;
							p8txbuff[14+14]=0x08;
							p8txbuff[15+14]=0x06;
							MR(((u32)p8txbuff)+16+14,ADDR_TYMDHMS,6);
							for(i=0;i<6;i++)
							{
								p8txbuff[16+i+14]+=0x33;
							}
							x=0;
							for(i=0;i<16;i++)
							{
								x+=p8txbuff[6+i+14];
							}
							p8txbuff[22+14]=x;
							p8txbuff[23+14]=0x16;
				
							//CS
							x=0;
							for(i=0;i<34;i++)
							{
								x+=p8txbuff[4+i];
							}
							p8txbuff[38]=x;
							//16
							p8txbuff[39]=0x16;
				
							Uart_3762_TxData(PORTn,0x13,1,26+14);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
							CopyString((u8 *)"载波校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							for(i=0;i<6;i++)
							{
								x=p8txbuff[10-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
						}
						else
						{//指定表地址的广播对时命令
							p8txbuff[3]=22;//报文长度
							//645报文
							MC(0xfe,((u32)p8txbuff)+2+2,4);
							p8txbuff[6+2]=0x68;
							MR(((u32)p8txbuff)+7+2,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
							p8txbuff[13+2]=0x68;
							p8txbuff[14+2]=0x08;
							p8txbuff[15+2]=0x06;
							MR(((u32)p8txbuff)+16+2,ADDR_TYMDHMS,6);
							for(i=0;i<6;i++)
							{
								p8txbuff[16+i+2]+=0x33;
							}
							x=0;
							for(i=0;i<16;i++)
							{
								x+=p8txbuff[6+i+2];
							}
							p8txbuff[22+2]=x;
							p8txbuff[23+2]=0x16;
				
							Uart_3762_TxData(PORTn,0x13,1,26);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
							CopyString((u8 *)"载波校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							for(i=0;i<6;i++)
							{
								x=p8txbuff[14-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
						}
			
						Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
						Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//从节点监控最大超时时间秒
						BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
					}
					else
					{
						BroadcastTime=10;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
					}
				}
				else
				{
					BroadcastTime=10;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
			}
			break;
		case 6://发广播校时
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=MRR(((u32)p8rxbuff)+10,3);
			if((i!=0x000414)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
				{
					Terminal_Router->ReRouterCtrl++;
					BroadcastTime--;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{
					BroadcastTime=10;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				break;
			}
			i=MRR(((u32)p8rxbuff)+19,2);//广播通信延迟时间秒

			p8txbuff[4]=0x04;
			Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
			MR(((u32)p8txbuff)+16,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
			p8txbuff+=128;

			x=MRR(ADDR_AFN04F298_Ext,1);
			if(x==2)
			{//指定采集器地址的广播对时命令
				p8txbuff[0]=(22+14);//报文长度
				//报文头8字节地址域为对应的表地址
				p8txbuff[1]=0x68;
				MR(((u32)p8txbuff)+2,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
				p8txbuff[8]=0x68;
				//控制码1字节
				p8txbuff[9]=0x33;
				//1字节L=2+645报文长度
				p8txbuff[10]=24;
				//控制域2字节
				p8txbuff[11]=MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+4,1);
				p8txbuff[11]>>=5;
				if(p8txbuff[11]==0)
				{//默认
					if(MRR(ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+5,1)==1)
					{//1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007
						//97默认1200波特率
						p8txbuff[11]=2;
					}
					else
					{
						//07默认2400波特率
						p8txbuff[11]=3;
					}
				}
				p8txbuff[12]=0;
	
				//645报文
				MC(0xfe,((u32)p8txbuff)+13,4);
				p8txbuff[17]=0x68;
				MC(0x99,((u32)p8txbuff)+18,6);
				p8txbuff[24]=0x68;
				p8txbuff[25]=0x08;
				p8txbuff[26]=0x06;
				d64=MRR(ADDR_TYMDHMS,6);
				d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
				MWR(d64,((u32)p8txbuff)+27,6);
				for(i=0;i<6;i++)
				{
					p8txbuff[27+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8txbuff[17+i];
				}
				p8txbuff[33]=x;
				p8txbuff[34]=0x16;
	
				//CS
				x=0;
				for(i=0;i<34;i++)
				{
					x+=p8txbuff[1+i];
				}
				p8txbuff[35]=x;
				//16
				p8txbuff[36]=0x16;
	
				Uart_3762_TxData(PORTn,0x14,3,37);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			}
			else
			{//指定表地址的广播对时命令
				p8txbuff[0]=22;//报文长度
				//645报文
				MC(0xfe,((u32)p8txbuff)+1,4);
				p8txbuff[5]=0x68;
				MR(((u32)p8txbuff)+6,ADDR_AFN04F10+(Pn*LEN_AFN04F10_Pn)+6,6);
				p8txbuff[12]=0x68;
				p8txbuff[13]=0x08;
				p8txbuff[14]=0x06;
				d64=MRR(ADDR_TYMDHMS,6);
				d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
				MWR(d64,((u32)p8txbuff)+15,6);
				for(i=0;i<6;i++)
				{
					p8txbuff[15+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8txbuff[5+i];
				}
				p8txbuff[21]=x;
				p8txbuff[22]=0x16;
	
				Uart_3762_TxData(PORTn,0x14,3,23);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			}

			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//从节点监控最大超时时间秒
			BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 7://接收确认帧
			switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 1://1=完成1帧发收
					i=MRR(((u32)p8rxbuff)+22,3);
					if(i==0x000113)
					{
						p16timer[0]=0;
						BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
					}
					else
					{
						UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
						UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
					}
					break;
				case 0://0=空闲
				case 2://2=接收超时(或错误)
					if(Terminal_Ram->RouterCheck_S_Timer==0)//从节点监控最大超时时间秒
					{
						p16timer[0]=0;
						BroadcastTime++;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
					}
					else
					{
						UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
						UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
					}
					break;
			}
			break;
		case 8://等待广播延时
			if(p16timer[0]==0)
			{
				BroadcastTime=10;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
//		case 9:
//			break;
		case 10:
			Pn=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			Pn++;
			MWR(Pn,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn+1,2);
			if(Pn>MaxRS485AddCarrierPn)
			{
				BroadcastTime=11;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			else
			{
				Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
				BroadcastTime=5;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
		case 11:
			Terminal_Router->StopFlags&=~(1<<4);//恢复路由
			Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			BroadcastTime=0;//采集器扩展校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
	}
	MWR(BroadcastTime,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);
}
#endif
#endif
