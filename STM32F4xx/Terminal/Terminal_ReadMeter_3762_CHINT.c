

//正泰载波模块
#include "AllHead.h"
#include "Terminal_ReadMeter_3762_CHINT.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "../LPC177x8x/lpc177x8x_IO.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"



#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块

#define RouterCheckTimerVal     (60*10)//下次载波路由器检查秒定时值

void Terminal_ReadMeter_3762_CHINT(u32 PORTn)//正泰载波模块
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u64 d64a;
	u64 d64b;
//	u8 * p8rxbuff;
//	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
//	u16 * p16timer;
//	u32 * p32;
	
//	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(Terminal_Router->RouterInitTask)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
		case 100://查删除
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清删除或添加节点数计数
					Terminal_Router->TempCount=0;//删除或添加节点数计数
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://查删除
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=Terminal_Router->NoteNum)
						{
							Terminal_Router->RouterInitSubTask=0;//子任务
							Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							return;
						}
						d64a=MEMReadReg((u32)&Terminal_Note->List[Terminal_Router->TempCount].Addr,6);
						z=ADDR_AFN04F10;
						for(x=0;x<AFN04F10MaxPn;x++)
						{
							d64b=MEMReadReg(z+6,6);
							if(d64a==d64b)
							{//地址相同
								y=MEMReadReg(z+2,2);
								if(y!=0x0)
								{//测量点有效
									if((MEMReadReg(z+4,1)&0x1f)==31)
									{//端口号31
										MEMWriteReg(x+1,(u32)&Terminal_Note->List[Terminal_Router->TempCount].MeterNo,2);//对应电能表/交流采样装置序号(从1开始)
										break;
									}
								}
							}
							z+=LEN_AFN04F10_Pn;
						}
						if(x==AFN04F10MaxPn)
						{//删除
							x=(Terminal_Router->NoteNum-(Terminal_Router->TempCount+1))*LEN_NoteList;//从地址表中删去
							MEMRead(ADDR_NoteList+(Terminal_Router->TempCount*LEN_NoteList),ADDR_NoteList+((Terminal_Router->TempCount+1)*LEN_NoteList),x);
							Terminal_Router->NoteNum--;
						}
						Terminal_Router->TempCount++;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 101://查添加
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清删除或添加节点数计数
					Terminal_Router->TempCount=0;//删除或添加节点数计数
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://查添加
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=AFN04F10MaxPn)
						{
							Terminal_Router->RouterInitSubTask=0;//子任务
							Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							return;
						}
						z=ADDR_AFN04F10+(LEN_AFN04F10_Pn*Terminal_Router->TempCount);
						x=MEMReadReg(z+2,2);
						if(x!=0x0)
						{//测量点有效
							if((MEMReadReg(z+4,1)&0x1f)==31)
							{//端口号31
								d64a=MEMReadReg(z+6,6);
								for(y=0;y<Terminal_Router->NoteNum;y++)
								{
									d64b=MEMReadReg((u32)&Terminal_Note->List[y].Addr,6);
									if(d64a==d64b)
									{
										break;
									}
								}
								if(y==Terminal_Router->NoteNum)
								{//添加
									MEMClr(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
									MEMRead((u32)&Terminal_Note->List[Terminal_Router->NoteNum].Addr,ADDR_AFN04F10+(LEN_AFN04F10_Pn*Terminal_Router->TempCount)+6,6);
									Terminal_Note->List[Terminal_Router->NoteNum].MeterNo=Terminal_Router->TempCount+1;//对应电能表/交流采样装置序号(从1开始)
									Terminal_Router->NoteNum++;
								}
							}
						}
						Terminal_Router->TempCount++;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 102:
			Terminal_Router->RouterInitTask=200;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		
		
		case 200://>=200路由抄表
//			p32=(u32*)(ADDR_E4_ENTRY);
//			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//软件更新任务
//			{
//				UpData_CHINT(PORTn);//路由软件更新
//				break;
//			}

			if(UARTCtrl->Task==0)//0=空闲
			{
				if(Terminal_Router->AFN04F10!=0)//AFN04F10设置变化标志0=没,1=变化
				{
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
				}
				
				if(Terminal_Ram->RouterCheck_S_Timer==0)//24 载波路由器检查秒定时器
				{
					Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//下次载波路由器检查秒定时器
					Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
					break;
				}
				if(MEMReadReg(ADDR_AFN05F49+30,1)==0x55)//命令指定通信端口暂停抄表
				{
					WarningOccur(PORTn,(u8 *)"载波命令暂停抄表");//告警发生,入口告警代码和告警字符串
					break;
				}
				i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//终端抄表运行参数设置,端口31
				if((i&0x01)!=0x0)
				{//D0置"1"不允许自动抄表，置"0" 要求终端根据抄表时段自动抄表
					WarningOccur(PORTn,(u8 *)"载波不允许自动抄表");//告警发生,入口告警代码和告警字符串
					break;
				}
				if(UARTCtrl->RMFlag==0)//1=抄表进行中,0=抄表间隔暂停
				{
					WarningOccur(PORTn,(u8 *)"载波非抄表时段");//告警发生,入口告警代码和告警字符串
					break;
				}
			}
			if(Terminal_Router->NoteNum==0)
			{//节点表空
				DisplayRouterVer(PORTn);//显示路由厂家和版本号
				break;
			}
			
			if(UARTCtrl->RMKeyUser==0)//抄读重点户标志,0=常规抄读,1=重点户抄读
			{
				GeneralReadMeter_3762_CHINT(PORTn);//载波模块,常规抄表
			}
			else
			{
				KeyUserReadMeter_3762_CHINT(PORTn);//载波模块,重点户抄表
			}
			break;
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}


extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);

void GeneralReadMeter_3762_CHINT(u32 PORTn)//载波模块,常规抄表
{
	u32 i;
	u32 x;
	u32 y;
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	
	u8 * p8RMLib;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=准备发送
	{
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			break;
		case 0://0=空闲
			i=MEMReadReg(ADDR_YMDHMS+1,2);
			if((i>0x2350)||(i<0x0010))
			{//在每日0点附近暂停抄表
				UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}

			ListNo=UARTCtrl->GRMCount;//抄读电能表计数(晓程和正泰载波)
			if(ListNo>=Terminal_Router->NoteNum)
			{//抄表计数>=当前路由器中总从节点数
				ListNo=0;
				UARTCtrl->GRMCount=0;
				break;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//取对应电能表/交流采样装置序号(从1开始)
			if((x==0)||(x>AFN04F10MaxPn))
			{//序号错
		RMOK:
				Terminal_Note->List[ListNo].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				goto RMEND;
			}
			i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//终端抄表运行参数设置,端口31
			if((i&0x02)!=0x0)
			{//D1置"1"要求终端只抄重点表，置"0"要求终端抄所有表
				y=MEMReadReg(ADDR_AFN04F35,1);//台区集中抄表重点户设置
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MEMReadReg(ADDR_AFN04F35+1+(2*i),2)==x)
					{
						break;
					}
				}
				if(i==y)
				{//不是重点户
					goto RMOK;//不抄,直接返回抄读成功
				}
			}
			x--;
			
			AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
			d64a=MEMReadReg(AddrAFN04F10+6,6);//取配置的通信地址
			Pn=MEMReadReg(AddrAFN04F10+2,2);//取配置的测量点号
			if(Pn==0)
			{//配置删除
				goto RMOK;//不抄,直接返回抄读成功
			}
			i=MEMReadReg(ADDR_AFN04F30+(Pn-1),1);//台区集中抄表停抄/投抄设置(测量点号)
			if(i!=0)
			{//停抄
				goto RMOK;//不抄,直接返回抄读成功
			}
			if(Terminal_Note->List[ListNo].ReCount>=1)
			{//抄表重发次数>=1
				Terminal_Note->List[ListNo].RMErr=1;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
				ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
				goto RMEND;//结束抄表
			}
			Terminal_Note->List[ListNo].ReCount++;//抄表重发计数+1
			Protocol=MEMReadReg(AddrAFN04F10+5,1);//取通信协议类型:1=DL/T645-1997,30=DL/T645-2007
			ClassNo=MEMReadReg(AddrAFN04F10+26,1);//取大小类号
			
			while(1)
			{
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(Terminal_Note->List[ListNo].FnCount>=i)
				{//Fn计数>=最大Fn个数,本轮抄表结束
			RMEND:
					DisplayRouterVer(PORTn);//显示路由厂家和版本号
					Terminal_Note->List[ListNo].RMFlag=2;//抄表标志;0=上电或日更新,1=命令强制重抄,2=完成一轮抄表
			 		Terminal_Note->List[ListNo].FnCount=0;//Fn计数
					Terminal_Note->List[ListNo].DICount=0;//抄表数据标识计数
					Terminal_Note->List[ListNo].ReCount=0;//抄表重发次数
					if(Terminal_Note->List[ListNo].RMErr==0)//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					{
						Terminal_Note->List[ListNo].OldRMErr=2;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
					}
					else
					{
						if(Terminal_Note->List[ListNo].OldRMErr<2)//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
						{
							Terminal_Note->List[ListNo].OldRMErr=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
						}
					}
					Terminal_Note->List[ListNo].RMErr=0;//当前抄表失败标志:0=空,1=至少有1个数据标识抄读失败
					UARTCtrl->GRMCount++;//抄读电能表计数(晓程和正泰载波)
					return;
				}

				if(Protocol==1)
				{//DL/T645-1997
					i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				else
				{//DL/T645-2007
					i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				if(i==0)
				{//本Fn是空
					goto NEXT_Fn;
				}
				p8RMLib=(u8 *)i;//抄表库
				if((p8RMLib[3]&0x04)==0)//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
				{
					goto NEXT_Fn;
				}
				
				
				y=0;
				if((p8RMLib[4]&0x01)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//1类数据,实时
					Fn=p8RMLib[0];//取实时Fn
					y=Class1Data_Fn(ClassNo,Fn);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					//i=GetClass1DataAddr(Fn,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
					//if(i!=0)
					//{
					//}
				}
				x=0;
				if((p8RMLib[4]&0x08)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据B3=日冻结
					Fn=p8RMLib[7];//取日冻结Fn
					i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//取原抄表时间年月日时分
						x|=DataComp(ADDR_DATABUFF+2,ADDR_YMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					}
				}
				if((p8RMLib[4]&0x20)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据B5=月冻结
					Fn=p8RMLib[9];//取月冻结Fn
					i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//取原抄表时间年月日时分
						x|=DataComp(ADDR_DATABUFF+3,ADDR_YMDHMS+4,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					}
				}
				if(x==0)
				{//抄表时标相同
					if(Terminal_Note->List[ListNo].RMFlag==1)//抄表标志;0=上电或日更新,1=命令强制重抄,2=完成
					{
						y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					}
				}
				else
				{
					y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				if(y==0x0)
				{//本Fn不抄
		NEXT_Fn:
					Terminal_Note->List[ListNo].FnCount++;//Fn计数+1
					Terminal_Note->List[ListNo].DICount=0;//抄表数据标识计数
					Terminal_Note->List[ListNo].ReCount=0;//重发计数
					continue;
				}
				if(p8RMLib[1]==0x0)
				{//本Fn抄表命令数=0
					if((p8RMLib[4]&0x01)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
					{//1类数据,实时
						Fn=p8RMLib[0];//取实时Fn
						switch(Fn)
						{
						//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 33:
							case 34:
							case 35:
							case 36:
						//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M�12�
							case 37:
							case 38:
							case 39:
							case 40:
								break;
							default:
								goto NEXT_Fn;
						}
						p8FnData[0]=0x53;//强制抄表成功
						ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
						goto NEXT_Fn;
					}
				}
				if(Terminal_Note->List[ListNo].DICount==0)//抄表数据标识计数
				{//抄表数据标识计数=0,清Fn数据缓冲
					MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				if(Terminal_Note->List[ListNo].DICount>=p8RMLib[1])
				{//抄表数据标识计数>=Fn需发命令数;本Fn抄表完成
					ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
					goto NEXT_Fn;
				}
				
				
				p8txbuff[0]=0x68;
				p8txbuff[3]=0x41;//控制码为0x41
				p8txbuff[4]=4;//通信模块标识=1
				p8txbuff[5]=0;
				p8txbuff[6]=0;
				p8txbuff[7]=0;
				p8txbuff[8]=0;
				p8txbuff[9]=0;
				MEMClr(0,((u32)p8txbuff)+10,6);
				MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//主节点地址(源地址)(同终端地址)
				MEMWriteReg(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
				p8txbuff[22]=0x13;//AFN=13H
				p8txbuff[23]=1;//Fn=1
				p8txbuff[24]=0;
				if(Protocol==1)
				{//dl645-1997
					p8txbuff[25]=1;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//附属节点数=0
					p8txbuff[27]=14;//DL645报文长度
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//从节点地址(目的地址)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x01;
					p8txbuff[37]=2;
					p8txbuff[38]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)]+0x33;//取DI同时加0x33
					p8txbuff[39]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=0;
					p8txbuff[41]=0x16;
					for(i=0;i<12;i++)
					{
						p8txbuff[40]+=p8txbuff[28+i];
					}
				}
				else
				{//dl645-2007
					i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
					switch(i)
					{//以下命令因帧太长不抄
						case 0x03300101://F27 最近一次电表清零发生时刻
						case 0x03300201://F27 最近一次需量清零发生时刻
						case 0x03300501://F166 最近一次电能表时段参数修改时间
						case 0x03300001://(仅正泰)F27 最近一次编程发生时刻
						case 0x1E0002FF://(仅正泰)F161 上1-10次合闸操作者代码
						case 0x1E0001FF://(仅正泰)F161 上1-10次合闸发生时刻
						case 0x1D0002FF://(仅正泰)F161 上1-10次跳闸操作者代码
						case 0x1D0001FF://(仅正泰)F161 上1-10次跳闸发生时刻
							Terminal_Note->List[ListNo].ReCount=0;//抄表重发计数=0
							Terminal_Note->List[ListNo].DICount++;//抄表命令计数
							return;
					}
					p8txbuff[25]=2;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//附属节点数=0
					p8txbuff[27]=16;//DL645报文长度
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//从节点地址(目的地址)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x11;
					p8txbuff[37]=4;
					p8txbuff[38]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)]+0x33;//取DI同时加0x33
					p8txbuff[39]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]+0x33;
					p8txbuff[41]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]+0x33;
					p8txbuff[42]=0;
					p8txbuff[43]=0x16;
					for(i=0;i<14;i++)
					{
						p8txbuff[42]+=p8txbuff[28+i];
					}
				}
				i=28+p8txbuff[27]+2;//需总发送字节数
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
				UARTCtrl->TxByte=i;//发送字节数(半字对齐)
				UARTCtrl->Task=3;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				UARTCtrl->RouterTimeOut=60000/10;//发送后接收超时定时值ms/10(半字对齐)

				//显示抄表地址
				CopyString((u8 *)"载波在抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				for(i=0;i<6;i++)
				{
					x=p8txbuff[16+5-i];
					MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
					MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
				}
				MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				return;
			}
			//break;
		case 1://1=完成1帧发收
			UARTCtrl->Task=0;//空闲1次
			ListNo=UARTCtrl->GRMCount;//抄读电能表计数(晓程和正泰载波)
			if(ListNo>=Terminal_Router->NoteNum)
			{//抄表计数>=当前路由器中总从节点数
				ListNo=0;
				UARTCtrl->GRMCount=0;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//取对应电能表/交流采样装置序号(从1开始)
			if((x==0)||(x>AFN04F10MaxPn))
			{//序号错
		rxerr:
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if(Pn==0)
			{//测量点号错
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//通信协议类型 BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//通信协议不知
				goto rxerr;
			}
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			p8RMLib=(u8 *)i;//抄表库
			
			d64a=MEMReadReg(x+4,6);//取配置的通信地址
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//否认回答
				goto rxerr;
//				Terminal_Note->List[ListNo].ReCount=0;//抄表重发计数=0
//				Terminal_Note->List[ListNo].DICount++;//抄表数据标识+1
//				break;
			}
			y=p8rxbuff[26];//返回数据域长度
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//返回的地址
			if(d64a!=d64b)
			{//表地址不相同
				goto rxerr;
			}
			x=0;//和校验计算
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//和校验错
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//从站异常回答,无此数据项,正常跳过
				Terminal_Note->List[ListNo].ReCount=0;//抄表重发计数=0
				Terminal_Note->List[ListNo].DICount++;//抄表数据标识+1
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//数据域减0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//DL/T645-1997
				x=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]<<8;
				y=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(x!=y)
				{//DI错误
					goto rxerr;
				}
				i=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+2];
				if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
				{//返回长度相同
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//抄表数据标识计数
					ReadMeter_DL645_1997_FnData(PORTn);//DL645_1997抄表接收数据填入Fn数据缓冲
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//返回的抄表数据标识计数
					ReadMeterNote_OK(PORTn,Pn);//集中抄表电表抄读信息.成功
				}
				Terminal_Note->List[ListNo].ReCount=0;//抄表重发计数=0
				Terminal_Note->List[ListNo].DICount++;//抄表数据标识+1
			}
			else
			{//DL/T645-2007
				x=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
				y=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(x!=y)
				{//DI错误
					goto rxerr;
				}
				i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+4];
				if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				{//返回长度相同
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//对应电能表配置序号(半字对齐)(从1开始)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//抄表Fn计数
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//抄表数据标识计数
					ReadMeter_DL645_2007_FnData(PORTn);//DL645_2007抄表接收数据填入Fn数据缓冲
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//返回的抄表数据标识计数
					ReadMeterNote_OK(PORTn,Pn);//集中抄表电表抄读信息.成功
				}
				Terminal_Note->List[ListNo].ReCount=0;//抄表重发计数=0
				Terminal_Note->List[ListNo].DICount++;//抄表数据标识+1
			}
			
			break;
	}
}

void KeyUserReadMeter_3762_CHINT(u32 PORTn)//载波模块,重点户抄表
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Pn;
	u64 d64a;
	u64 d64b;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p8FnData=(u8 *)ADDR_DATABUFF;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//重点户抄读任务
	{
		case 0://暂停路由
			if(UARTCtrl->Task==0)
			{
				if(Terminal_Router->RouterStop==0)//暂停路由标识:0=无,1=抄重点户,2=数据转发,3=UART读路由信息,4=广播校时,5=广播冻结,6=通断电控制,7=...
				{
					Terminal_Router->RouterStop=1;//暂停路由标识:0=无,1=抄重点户,2=数据转发,3=UART读路由信息,4=广播校时,5=广播冻结,6=通断电控制,7=...
					Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				}
				Terminal_Router->RouterKeyUserTask++;//重点户抄读任务
			}
			else
			{
				GeneralReadMeter_3762_CHINT(PORTn);//载波模块,常规抄表
			}
			break;
		case 1://AFN13 F1 路由数据转发:监控载波从节点
			i=MEMReadReg(ADDR_AFN04F35,1);//台区集中抄表重点户设置
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//重点户抄读电能表计数
			{//全部重点户抄完
				UARTCtrl->KeyUserRMCount=0;//重点户抄读电能表计数
				Terminal_Router->RouterKeyUserTask=0;//重点户抄读任务
				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//上次重点户抄读成功计数
				UARTCtrl->KeyUserRMOKCount=0;//当前重点户抄读成功计数
				if(Terminal_Router->RouterStop==1)//暂停路由标识:0=无,1=抄重点户,2=数据转发,3=UART读路由信息,4=广播校时,5=广播冻结,6=通断电控制,7=...
				{
					Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				}
				UARTCtrl->RMKeyUser=0;//抄读重点户标志,0=常规抄读,1=重点户抄读
				//小时更新在重点户抄完后若有AFN04F10设置变化则初始化路由器
				if(Terminal_Router->AFN04F10!=0)//AFN04F10设置变化标志0=没,1=变化
				{
					Terminal_Router->AFN04F10=0;//AFN04F10设置变化标志0=没,1=变化
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				}
				return;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//重点户的电能表/交流采样装置序号
			if((x==0)||(x>AFN04F10MaxPn))
			{//装置序号错
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//测量点号错
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				break;
			}
			i=MEMReadReg(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//端口不是载波
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				break;
			}
			i=MEMReadReg(x+3,1);//通信协议类型 BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//通信协议不知
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				break;
			}

			p8txbuff[0]=0x68;
			p8txbuff[3]=0x41;//控制码为0x41
			p8txbuff[4]=4;//通信模块标识=1
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			MEMClr(0,((u32)p8txbuff)+10,6);
			MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//主节点地址(源地址)(同终端地址)
			MEMRead(((u32)p8txbuff)+16,x+4,6);//从节点地址(目的地址)
			p8txbuff[22]=0x13;//AFN=13H
			p8txbuff[23]=1;//Fn=1
			p8txbuff[24]=0;
			if(Protocol==1)
			{//dl645-1997
				p8txbuff[25]=1;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//附属节点数=0
				p8txbuff[27]=14;//DL645报文长度
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//从节点地址(目的地址)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x01;
				p8txbuff[37]=2;
				p8txbuff[38]=0x0f+0x33;//DI=0x901f
				p8txbuff[39]=0x90+0x33;
				p8txbuff[40]=0;
				p8txbuff[41]=0x16;
				for(i=0;i<12;i++)
				{
					p8txbuff[40]+=p8txbuff[28+i];
				}
			}
			else
			{//dl645-2007
				p8txbuff[25]=2;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//附属节点数=0
				p8txbuff[27]=16;//DL645报文长度
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//从节点地址(目的地址)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x11;
				p8txbuff[37]=4;
				p8txbuff[38]=0x00+0x33;//DI=0x0001FF00
				p8txbuff[39]=0x32;//0xff+0x33;
				p8txbuff[40]=0x01+0x33;
				p8txbuff[41]=0x00+0x33;
				p8txbuff[42]=0;
				p8txbuff[43]=0x16;
				for(i=0;i<14;i++)
				{
					p8txbuff[42]+=p8txbuff[28+i];
				}
			}
			i=28+p8txbuff[27]+2;//需总发送字节数
			p8txbuff[1]=i;
			p8txbuff[2]=i>>8;
			Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=i;//发送字节数(半字对齐)
			UARTCtrl->Task=3;
			UARTCtrl->RouterTimeOut=60000/10;//发送后接收超时定时值ms/10(半字对齐)
			//显示抄表地址
			CopyString((u8 *)"抄重点户",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			Terminal_Router->RouterKeyUserTask++;//重点户抄读任务
			break;
		case 2://接收
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//重点户的电能表/交流采样装置序号
			if((x==0)||(x>AFN04F10MaxPn))
			{//装置序号错
				goto rxerr;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>AFN0CF129KeyUserMaxPn))
			{//测量点号错
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//通信协议类型 BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//通信协议不知
				goto rxerr;
			}
			d64a=MEMReadReg(x+4,6);//取配置的通信地址
			MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
			MEMRead(((u32)p8FnData)+1,ADDR_YMDHMS+1,5);//抄表时间
			
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//否认回答
		rxerr:
				UARTCtrl->Task=0;//空闲1次
				UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
				Terminal_Router->RouterKeyUserTask--;//重点户抄读任务
				break;
			}
			y=p8rxbuff[26];//返回数据域长度
			if(UARTCtrl->Task!=1)
			{
				y=0;//接收数据长度
			}
			else
			{
				i=MEMReadReg(((u32)p8rxbuff)+22,3);
				if(i!=0x113)
				{//AFN=13H,Fn=1
					y=0;//接收数据长度
				}
			}
			if(y==0)
			{//接收数据长度=0
				goto rxerr;
			}
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//返回的地址
			if(d64a!=d64b)
			{//表地址不相同
				goto rxerr;
			}
			x=0;//和校验计算
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//和校验错
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//控制码从站回答异常
				goto rxerr;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//数据域减0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				if(p8rxbuff[9]<=2)
				{//无数据
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(i!=0x901f)
				{
					goto rxerr;
				}
			}
			else
			{//2007
				if(p8rxbuff[9]<=4)
				{//无数据
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(i!=0x0001FF00)
				{
					goto rxerr;
				}
			}
			for(i=0;i<p8rxbuff[9];i++)
			{
				if(p8rxbuff[10+i]==0xff)
				{
					p8rxbuff[10+i]=0;//单相表块无效数据填0xff
				}
			}
			if(Protocol==1)
			{//1997
				i=p8rxbuff[9];
				if(i>=6)
				{
					i-=6;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=12;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			else
			{//2007
				i=p8rxbuff[9];
				if(i>=8)
				{
					i-=8;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=14;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			//SaveClass1Data(129,Pn,((u32)p8FnData)+1);//存贮类1数据
			MEMWrite(((u32)p8FnData)+1,ADDR_AFN0CF129_KeyUser+((6+(5*(1+MaxTariff)))*(Pn-1)),6+(5*(1+MaxTariff)));
			
			UARTCtrl->Task=0;//空闲1次
			UARTCtrl->KeyUserRMOKCount++;//当前重点户抄读成功计数
			UARTCtrl->KeyUserRMCount++;//重点户抄读电能表计数
			Terminal_Router->RouterKeyUserTask--;//重点户抄读任务
			break;
		default:
			Terminal_Router->RouterKeyUserTask=0;//重点户抄读初始化任务
			break;
	}	
}


void UpData_CHINT(u32 PORTn)//路由软件更新
{

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
