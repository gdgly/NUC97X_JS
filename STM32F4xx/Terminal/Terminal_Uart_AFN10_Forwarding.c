
//数据转发处理过程
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../Device/IC_BUZZ.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"


	#if(USER/100)==0
#define MaxReRMCount     2//1//抄表重发次数(仅重发次数,不算初始1次的发送);电力线载波=1,微功率无线=2
	#else
#define MaxReRMCount     0//抄表重发次数(仅重发次数,不算初始1次的发送)
	#endif


void Forwarding(u32 PORTn)//数据转发处理过程
{
	u32 i;
	u32 Fn;
	u32 DIaddr;
	u32 n;
	u32 DI;
	u64 CommAddr;
	u32 PORTRS485;
	u32 x;
	u32 y;
	u32 z;
	u32 Protocol;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u16 * p16;
	u32 * p32;
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
	u64 d64;
//	#if (USER/100)==0//电科院测试
//	u8 *p8d;
//	#endif
#endif
	

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	
#if (USER/100)==0//电科院测试
//载波通道总是无启抄

#else
 #if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
//	if(Terminal_Ram->PowerTask<=10)//停电事件任务
//	{
//		if(Comm_Ram->SelfTestCommand!=0x55555555)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
//		{
//			goto err;
//		}
//	}
 #endif
#endif//电科院测试

	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	i=DItoFnPn(PORTn);//数据标识分解为FnPn
	Fn=(i>>16);
//if(p8rxbuff[18]==2)
//{
//	p8rxbuff[18]=31;//仅模拟台体走载波口ESAM内部测试用
//}
	switch(p8rxbuff[18])
	{
		case 1://内部交流采样接口
			PORTRS485=ACSAMPLEPORT;
		case 2://RS485-1
			PORTRS485=RS485_1PORT;
			break;
		case 3://RS485-2
			PORTRS485=RS485_2PORT;
			break;
		case 4://RS485-3
			PORTRS485=RS485_3PORT;
			break;
		case 31://端口号=31,载波
			PORTRS485=RS485_4PORT;
			break;
		default:
			UARTCtrl->Task=0;
			return;
	}
	switch(Fn)
	{
		case 1://F1透明转发
			switch(UARTCtrl->Task)
			{
				case 8://8=等待通过其它口数据转发
					switch(p8rxbuff[18])
					{
						case 1://内部交流采样接口
							p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//转发字节数
							p8=(u8 *)((u32)p8rxbuff+18+6);
							//作为正确帧可能前导0xfe去除,从0x68开始
							while(i!=0x0)
							{
								if(p8[0]==0x68)
								{
									break;
								}
								p8++;
								i--;
							}
							if(i!=0x0)
							{
								if(i>Get_LEN_UARTnRx(ACSAMPLEPORT))
								{//转发长度太长错
									goto err;
								}
								MW((u32)p8,Get_ADDR_UARTnRx(ACSAMPLEPORT),i);
								MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//通信地址用0xaa
								i=UARTCtrlRS485->Task;//原任务寄存
								UARTCtrlRS485->Task=1;//1=接收完成
							  if(FileDownload->File&0x04)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
								}
								UARTCtrlRS485->Task=i;//原任务恢复
							}
							i=UARTCtrlRS485->TxByte;//发送字节数
//							if(i==0x0)
//							{//没收到数据
//								goto err;
//							}
							MW(Get_ADDR_UARTnTx(ACSAMPLEPORT),((u32)p8txbuff)+21,i);
							p8txbuff[19]=i;
							p8txbuff[20]=i>>8;
							i+=7;//4字节标识,1字节端口号,2字节长度
							i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							break;
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(p16timer[0]==0)//等待数据转发口空闲定时5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=通信功能正在被调用禁止其他程序再次调用
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrl->ReRMCount=0;//重发计数
									UARTCtrl->Task=9;//9=等待下帧启动发送延时
								}
							}
							break;
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
						case 31://端口号=31,载波
							//端口号=31,转发只能对DL645-1997或2007进行
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
							{
								case 0://等待口空闭时进入
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
									DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
									if(p16[0]!=p16[1])
									{//已接收到下帧
										goto err;
									}
								#endif
									if(p16timer[0]==0)//等待数据转发口空闲定时120S
									{
										goto err;
									}
									if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
									{
										break;
									}
									if(Terminal_Router->UpDateTask!=0)//软件更新任务
									{
										p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)	
										goto err;
									}
									if(UARTCtrlRS485->Lock==0x55)
									{
										goto err;//口下在被占用
									}
									else
									{//0x55=通信功能正在被调用禁止其他程序再次调用
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
											UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
										}
									}
									break;
								case 1://暂停路由
									if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//暂停路由标志
									Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
									break;
								case 2://暂停路由接收
									if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										UARTCtrl->ReRMCount=0;//重发计数
										UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
									}
									break;
								case 3://路由转发数据
									x=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//转发字节数
									if((x<14)||(x>(128-2)))
									{//DL645-1997最小14字节,DL645-2007最小16字节,最大转发(128-2)字节
										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										goto err;
									}
									y=0;//查前导0xfe个数
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if((p8rxbuff[18+6+y]!=0x68)||(p8rxbuff[18+6+y+7]!=0x68))
									{//不是DL645标准帧
										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										goto err;
									}
									for(i=0;i<6;i++)
									{
										if(p8rxbuff[18+6+y+1+i]!=0x99)
										{
											break;
										}
									}
									if(i==6)
									{//是广播命令
										UARTCtrl->SubTask=5;//子任务(临时操作路由模块任务)
										break;
									}
								#if(USER/100)==7//浙江用户
									if(p8rxbuff[18+6+y+8]==0x08)
									{//是指定表地址校时命令
										UARTCtrl->SubTask=10;//子任务(临时操作路由模块任务)
										break;
									}
								#endif
//								#if(USER/100)==0//国网送样
//									if(p8rxbuff[18+6+y+8]==0x14)
//									{//写命令
//										i=MRR(((u32)p8rxbuff)+18+6+y+8+2,4);
//										if((i==0x3733343F)||(i==0x37333435))
//										{
//											UARTCtrl->SubTask=10;//子任务(临时操作路由模块任务)
//											break;
//										}
//									}
//								#endif
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
									{
										p8[3]=0x43;//控制码;集中式路由载波通信
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
										{
											p8[3]=0x4a;//控制码;微功率无线通信
										}
										else
										{
											p8[3]=0x41;//控制码;集中式路由载波通信
										}
									}
									p8[4]=4;//通信模块标识=1
									p8[5]=0;
									p8[6]=0;//预计应答字节数
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									p8[9]=UARTCtrlRS485->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+6+y+1,6);//从节点地址(目的地址)
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+6+y+8];//取控制码
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997的最大需量清零
											p8[25]=1;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
										}
										else
										{
											p8[25]=2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
										}
									}
									else
									{
										p8[25]=1;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
									}
									p8[26]=0;//通信延时相关标志
									p8[27]=0;//附属节点数=0
									
									p8[28]=x-y;//DL645报文长度(前导0xfe不加入)
									MR(((u32)p8)+29,((u32)p8rxbuff)+18+6+y,x-y);//转发内容(前导0xfe不加入)
									
									i=29+p8[28]+2;//需总发送字节数
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
									{//微功率无线,档案地址用采集器地址
										CommAddr=MRR(((u32)p8)+16,6);
										x=Check_AFN04F10_Addr(31,CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//从节点地址(目的地址)
											}
										}
									}
									Uart_3762_CS((u32)p8);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
									UARTCtrlRS485->TxByte=i;//发送字节数(半字对齐)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
									CopyString((u8 *)"载波F1  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+6+y+1+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符?
									break;
								case 4://转发接收
									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//超时或否认回答
										y=0;//转发接收数据长度
									}
									else
									{
										y=p8[28];//返回645帧总长度
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											UARTCtrlRS485->RxByte=0;//接收字节数(半字对齐)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2直接否认帧(微功率无线有此情况)
												p16[0]=6000/10;//6秒超时时间
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
											}
											return;
										}
										//地址比较
										//查当前接收前导0xfe个数
										for(i=0;i<14;i++)
										{
											if(p8[29+i]!=0xfe)
											{
												break;
											}
										}
										i=DataComp(((u32)p8)+29+1+i,Get_ADDR_UARTnTx(PORTRS485)+29+1,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
										if(i!=0)
										{//不相同
											UARTCtrlRS485->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											UARTCtrlRS485->RxByte=0;//接收字节数(半字对齐)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(y==0)
											{//没收到数据
												p16[0]=6000/10;//6秒超时时间
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
											}
											return;
										}
									}
									if(y==0x0)
									{//没收到数据,载波路由有数据返回即为标准645帧不需检查返回帧合法性
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//重发计数
										{
										#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
											if(p16[0]==p16[1])
											{//没接收到下帧
												UARTCtrl->ReRMCount++;//重发计数
												UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
												return;
											}
										#else
											UARTCtrl->ReRMCount++;//重发计数
											UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
											return;
										#endif
										}
									#endif
									}
						BroadcastESC:
									Terminal_Router->StopFlags&=~(1<<2);//恢复路由
									Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									MW(((u32)p8)+28+1,((u32)p8txbuff)+21,y);
									p8txbuff[19]=y;
									p8txbuff[20]=y>>8;
									y+=7;//4字节标识,1字节端口号,2字节长度
									y+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
									p8txbuff[1]=y;
									p8txbuff[2]=y>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
									Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
									break;
								case 5://载波广播转发
									y=0;//查前导0xfe个数
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if(p8rxbuff[18+6+y+8]==0x08)
									{//是广播校时
										UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									i=p8rxbuff[18+6+y+8];//取控制码
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997的最大需量清零
											p8[0]=1;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
										}
										else
										{
											p8[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
										}
									}
									else
									{
										p8[0]=1;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
									}
									i=p8rxbuff[18+4];
									p8[1]=i;//报文长度
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,i);
									Uart_3762_TxData(PORTRS485,0x05,3,2+i);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
									UARTCtrl->SubTask=8;//子任务(临时操作路由模块任务)
									if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
									}
									WarningOccur(PORTRS485,(u8 *)"载波转发广播数据");//告警发生,入口告警代码和告警字符串
									break;
								case 6://广播校时.通信延时相关广播通信时长查询
									x=p8rxbuff[18+4];//转发字节数
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
								#else
									p8[0]=0;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
								#endif
									p8[1]=x;//报文长度
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x03,9,2+x);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)	
									WarningOccur(PORTRS485,(u8 *)"载波转发广播校时");//告警发生,入口告警代码和告警字
									break;
								case 7://广播校时.接收时长查询,发广播校时
									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x010103)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										goto err;
									}
									i=MRR(((u32)p8)+13,2);//广播通信延迟时间秒
									y=0;//查前导0xfe个数
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
								#else
									p8[0]=0;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
								#endif
									p8[1]=y+18;//报文长度
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//原广播时间减0x33
									{
										p8[2+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+2+y+10,6);
									d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
									MWR(d64,((u32)p8)+2+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[2+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[2+y+i];
									}
									p8[y+18]=x;
									p8[y+19]=0x16;
									Uart_3762_TxData(PORTRS485,0x05,3,y+20);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
									if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
									}
									break;
								case 8://载波广播转发.接收路由确认帧
									if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
									{//0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文
										switch(UARTCtrlRS485->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
										{
											case 1://1=完成1帧发收
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//转发接收数据长度
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
													UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
												}
												break;
											case 0://0=空闲
											case 2://2=接收超时(或错误)
												if(Terminal_Ram->RouterCheck_S_Timer==0)//最大广播超时时间秒(半字对齐)
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//转发接收数据长度
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
													UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
												}
												break;
										}
									}
									else
									{//1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
										switch(UARTCtrlRS485->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
										{
											case 1://1=完成1帧发收
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													//取等待时间
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													i=p8[17];
													i+=p8[18]<<8;
													i*=1000/10;
													if(i>0xffff)
													{
														i=0xffff;
													}
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=i;
													UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
													break;
												}
											case 0://0=空闲
											case 2://2=接收超时(或错误)
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//转发接收数据长度
												goto BroadcastESC;
										}
									}
									break;
								case 9://等待广播延时
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
									if(p16timer[0]==0)
									{
										p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
										y=0;//转发接收数据长度
										goto BroadcastESC;
									}
									break;
						#if (USER/100)==7//浙江用户
								case 10://指定表地址校时.通信延时相关广播通信时长查询
									x=p8rxbuff[18+4];//转发字节数
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[4]=0x04;
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
									y=0;//查前导0xfe个数
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									for(i=0;i<6;i++)
									{
										p8[16+i]=p8rxbuff[18+6+y+1+i];
									}

									p8+=128;
									p8[0]=0;//通信协议类型：00H为透明传输；01H为 DL/T 645-1997；02H为 DL/T 645-2007；03H～FFH保留
									p8[1]=1;//										
									p8[2]=0;//										
									p8[3]=x;//报文长度
									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)	
									WarningOccur(PORTRS485,(u8 *)"载波转发校时");//告警发生,入口告警代码和告警字
									break;
								case 11://指定表地址校时.接收时长查询,发广播校时
									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										goto err;
									}
									i=MRR(((u32)p8)+19,2);//广播通信延迟时间秒
									y=0;//查前导0xfe个数
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									p8[0]=y+18;//报文长度
									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//原广播时间减0x33
									{
										p8[1+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+1+y+10,6);
									d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
									MWR(d64,((u32)p8)+1+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[1+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[1+y+i];
									}
									p8[y+17]=x;
									p8[y+18]=0x16;
									Uart_3762_TxData(PORTRS485,0x14,3,y+19);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//从节点监控最大超时时间秒
									break;
								case 12://载波广播转发.接收路由确认帧
									switch(UARTCtrlRS485->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										case 1://1=完成1帧发收
											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
											i=MRR(((u32)p8)+22,3);
											if(i==0x000113)
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=p8[28];//返回645帧总长度
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
												UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//发送后接收超时定时值ms/10(半字对齐)
											}
											break;
										case 0://0=空闲
										case 2://2=接收超时(或错误)
											if(Terminal_Ram->RouterCheck_S_Timer==0)//从节点监控最大超时时间秒
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//转发接收数据长度
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
												UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//发送后接收超时定时值ms/10(半字对齐)
											}
											break;
									}
									break;
						#endif
//						#if(USER/100)==0//国网送样
//								case 10://指定表地址校时.通信延时相关广播通信时长查询
//									x=p8rxbuff[18+4];//转发字节数
//									if(x>(128-2))
//									{
//										x=(128-2);
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8[4]=0x04;
//									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
//									y=0;//查前导0xfe个数
//									for(i=0;i<14;i++)
//									{
//										if(p8rxbuff[18+6+i]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									for(i=0;i<6;i++)
//									{
//										p8[16+i]=p8rxbuff[18+6+y+1+i];
//									}
//
//									p8+=128;
//									p8[0]=0;//通信协议类型：00H为透明传输；01H为 DL/T 645-1997；02H为 DL/T 645-2007；03H～FFH保留
//									p8[1]=1;//										
//									p8[2]=0;//										
//									p8[3]=x;//报文长度
//									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
//									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
//									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)	
//									WarningOccur(PORTRS485,(u8 *)"载波转发F1");//告警发生,入口告警代码和告警字
//									break;
//								case 11://指定表地址校时.接收时长查询,发广播校时
//									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//									{
//										break;
//									}
//									p8d=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//									i=MRR(((u32)p8d)+10,3);
//									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
//									{
//										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
//										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
//										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
//										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
//										goto err;
//									}
//									i=MRR(((u32)p8d)+19,2);//广播通信延迟时间秒
//									y=0;//查前导0xfe个数
//									for(x=0;x<14;x++)
//									{
//										if(p8rxbuff[18+6+x]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8+=128;
//									p8[0]=MRR(((u32)p8d)+21,1);//报文长度
//									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,p8[0]);
//
//									x=MRR(((u32)p8d)+18+6+y+8+2,4);
//									if(x==0x3733343F)
//									{
//										for(x=0;x<7;x++)//原时间减0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=(MRR(((u32)p8)+1+y+22+4,3)<<24);
//										d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
//										MWR(d64,((u32)p8)+1+y+22,3);
//										MWR((d64>>24),((u32)p8)+1+y+22+4,3);
//										for(i=0;i<7;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									else
//									{//if(x==0x37333435)
//										for(x=0;x<3;x++)//原时间减0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=0x010101000000;
//										d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
//										MWR(d64,((u32)p8)+1+y+22,3);
//										for(i=0;i<3;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									x=0;
//									for(i=0;i<(p8[0]-y-2);i++)
//									{
//										x+=p8[1+y+i];
//									}
//									p8[p8[0]-1]=x;//cs
//									p8[p8[0]]=0x16;
//									
//									Uart_3762_TxData(PORTRS485,0x14,3,p8[0]+1);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
//									UARTCtrl->SubTask++;//子任务(临时操作路由模块任务)
//									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//从节点监控最大超时时间秒
//									break;
//								case 12://指定表地址校时.接收路由确认帧
//									switch(UARTCtrlRS485->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//									{
//										case 1://1=完成1帧发收
//											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//											i=MRR(((u32)p8)+22,3);
//											if(i==0x000113)
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=p8[28];//返回645帧总长度
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
//												UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
//											}
//											break;
//										case 0://0=空闲
//										case 2://2=接收超时(或错误)
//											if(Terminal_Ram->RouterCheck_S_Timer==0)//从节点监控最大超时时间秒
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=0;//转发接收数据长度
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX计数(已接收字节数)=0
//												UARTCtrlRS485->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
//											}
//											break;
//									}
//									break;
//						#endif
						
								default:
									UARTCtrl->Task=0;
									Terminal_Router->StopFlags&=~(1<<2);//恢复路由
									Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=等待下帧启动发送延时(原作为抄表口时置为的值）
					switch(p8rxbuff[18])
					{
						case 2://端口号=2.RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//得到UART定时器地址
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//转发字节数
								MW(((u32)p8rxbuff)+18+6,Get_ADDR_UARTnTx(PORTRS485),i);
								UARTCtrlRS485->BpsCtrl=p8rxbuff[19];
								Init_UARTn(PORTRS485);//初始化UART口,入口通信控制字已填入
								UARTCtrlRS485->RxByte=0;//接收字节数
								UARTCtrlRS485->TxByte=i;//发送字节数
								//p16=(u16 *)Get_ADDR_UARTn(PORTRS485);//得到UART缓冲首地址
								//p16[0]=0;//FIFO指针
								//p16[1]=0;//FIFO已读指针
								Start_UARTn_Tx(PORTRS485);//启动UART口发送
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
					#endif
								UARTCtrl->Task=10;//10=正在通过其它口发送数据
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=正在通过其它口发送数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								i=p8rxbuff[20];//透明转发接收等待报文超时时间
								if((i&0x80)==0x0)
								{//10ms
									i*=10;
								}
								else
								{//s
									i&=0x7f;
									i*=1000;
									if(i>0xffff)
									{
										i=0xffff;
									}
								}
								if(i<1000)
								{
									i=1000;//最小1000ms
								}
								if(i>ReadDL645OverTimer)
								{
									i=ReadDL645OverTimer;//最大ReadDL645OverTimer
								}
								i/=10;
								p16[0]=i;
								UARTCtrl->Task=11;//11=正在通过其它口接收数据
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=正在通过其它口接收数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
							p16=(u16 *)Get_ADDR_UARTn(PORTRS485);
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							DMA_UARTn_RX_Pointer(PORTRS485);//计算DMA_UARTn接收指针值(p0)
							if(p16[0]!=p16[1])
							{//有收到字节
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
					#endif
								if(p16[0]>p16[1])
								{
									i=p16[0]-p16[1];
									p16[1]=p16[0];
									UARTCtrlRS485->RxByte+=i;///接收字节数寄存
									i=p8rxbuff[21];//透明转发接收等待字节超时时间 NIB 10MS
									if(i<(50/10))
									{
										i=50/10;//最小50ms
									}
									if(i>(1000/10))
									{
										i=1000/10;//最大1000ms
									}
									p16timer[0]=i;
								}
								else
								{
									UARTCtrlRS485->RxByte=0;//清接收字节数寄存
									UARTCtrlRS485->Lock=0;//解除调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									goto err;
								}
							}
							else
							{
								if(p16timer[0]==0)
								{//接收时间到
									y=0;//查转发帧前导0xfe个数
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									i=UARTCtrlRS485->RxByte;//收到数据长度
									if(i==0x0)
									{//没收到数据
			F1_RS485_ReTx:
										for(i=0;i<6;i++)
										{//判是否广播地址
											if(p8rxbuff[18+6+y+1+i]!=0x99)
											{
												break;
											}
										}
										if(i!=6)
										{//不是广播地址,重发
											if(UARTCtrl->ReRMCount<2)//重发计数
											{
												UARTCtrl->ReRMCount++;//重发计数
												p16timer[0]=2000/10;//固定延时2秒后重发,仅防模拟表返回响应延时
												UARTCtrl->Task=9;//9=等待下帧启动发送延时
												return;
											}
										}
									}
									else
									{//有数据收到
										//检查数据帧
										x=0;//查转发接收帧前导0xfe个数
										p8=(u8*)(Get_ADDR_UARTn(PORTRS485)+10);
										for(i=0;i<14;i++)
										{
											if(p8[i]!=0xfe)
											{
												break;
											}
											x++;
										}
										//帧头0x68比较
										if((p8[x]!=0x68)||(p8[x+7]!=0x68))
										{
											goto F1_RS485_ReTx;
										}
										//地址比较
										for(i=0;i<6;i++)
										{
											if(p8rxbuff[18+6+y+1+i]!=0xaa)
											{
												if(p8rxbuff[18+6+y+1+i]!=p8[x+1+i])
												{
													break;
												}
											}
										}
										if(i!=6)
										{
											goto F1_RS485_ReTx;
										}
									}
									
									i=UARTCtrlRS485->RxByte;//收到数据长度
									UARTCtrlRS485->Lock=0;//解除调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									UARTCtrlRS485->RxByte=0;//清接收字节数寄存
									MW(Get_ADDR_UARTn(PORTRS485)+10,((u32)p8txbuff)+21,i);
									p8txbuff[19]=i;
									p8txbuff[20]=i>>8;
									i+=7;//4字节标识,1字节端口号,2字节长度
									i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
									Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
								}
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
err:
					//全部否认
					Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
					break;
				default:
					UARTCtrl->Task=0;
					break;
			}
			break;
/*
		case 9://F9转发主站直接对电表的抄读数据命令
			switch(UARTCtrl->Task)
			{
				case 8://8=等待通过其它口数据转发
					switch(p8rxbuff[18])
					{
						case 1://内部交流采样接口
							p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//转发中继级数n
							if(n==0xff)
							{
								n=0;//无指定中继路由
							}
							n*=6;//中继级数和地址字节长度
							n++;
							Protocol=p8rxbuff[19+n+6];//转发直接抄读的数据标识类型
							Protocol&=3;
							if(Protocol==0)
							{//0：符合DL/T645-1997， 1：符合DL/T645-2007
								goto err;
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//转发目标地址
							DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//转发直接抄读的数据标识
							Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							i=UARTCtrlRS485->Task;//原任务寄存
							UARTCtrlRS485->Task=1;//1=接收完成
						  if((Comm_Ram->SoftModule&(1<<1))==0)
						  {
								p32=(u32 *)(ADDR_I1_ENTRY);
						    (*(void(*)())p32[70])((u32)ACSAMPLEPORT);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
							}
							UARTCtrlRS485->Task=i;//原任务恢复
							p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
							if((p8[8]&0x40)!=0)
							{
								x=4;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
								y=4;//转发接收数据长度
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//转发直接抄读的数据标识
							}
							else
							{
								x=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
								y=p8[9];//转发接收数据长度
								for(i=0;i<y;i++)
								{//数据域减0x33
									p8[10+i]-=0x33;
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//转发目标地址
							p8txbuff[25]=x;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
							p8txbuff[26]=y;//转发直接抄读的数据内容字节数k+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//转发直接抄读的数据内容
							y+=13;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志,1字节数据内容字节数
							y+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							break;
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(p16timer[0]==0)//等待数据转发口空闲定时5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=通信功能正在被调用禁止其他程序再次调用
								if(UARTCtrlRS485->Task==0)
								{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrl->ReRMCount=0;//重发计数
									UARTCtrl->Task=9;//9=等待下帧启动发送延时
								}
							}
							break;
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
						case 31://端口号=31,载波
							//端口号=31,转发只能对DL645-1997或2007进行
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
							{
								case 0://等待口空闭时进入
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
									if(p16[0]!=p16[1])
									{//已接收到下帧
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										y=0;//转发接收数据长度
										goto F9err;
									}
									if(p16timer[0]==0)//等待数据转发口空闲定时120S
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										y=0;//转发接收数据长度
										goto F9err;
									}
									if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										y=0;//转发接收数据长度
										p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
										goto F9err;
									}
									if(Terminal_Router->UpDateTask!=0)//软件更新任务
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										y=0;//转发接收数据长度
										p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
										goto F9err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=通信功能正在被调用禁止其他程序再次调用
										//if(UARTCtrlRS485->Task==0)
										if(UARTCtrlRS485->Task!=4)
										{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
											UARTCtrl->SubTask=1;//子任务(临时操作路由模块任务)
										}
									}
									break;
								case 1://暂停路由
									if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//暂停路由
									Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrl->SubTask=2;//子任务(临时操作路由模块任务)
									break;
								case 2://暂停路由接收
									if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										UARTCtrl->ReRMCount=0;//重发计数
										UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
									}
									break;
								case 3://路由转发数据
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
									{
										p8[3]=0x41;//控制码;集中式路由载波通信
									}
									else
									{
										p8[3]=0x4a;//控制码;微功率无线通信
									}
									p8[4]=4;//通信模块标识=1
									p8[5]=0;
									p8[6]=0;//预计应答字节数
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									p8[9]=UARTCtrlRS485->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+2+n,6);//从节点地址(目的地址)
									CommAddr=MRR(((u32)p8)+16,6);//通信地址
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+2+n+6];//转发数据标识类型
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+18+2+n+6+1,4);//取数据标识
									p8[26]=0;//通信相关标志
									p8[27]=0;//附属节点数=0
									if(i==0x1)
									{//DL/T645-1997
										p8[28]=14;//DL645报文长度
										Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
									}
									else
									{
										p8[28]=16;//DL645报文长度
										Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
									}
									i=31+p8[28];//需总发送字节数
									p8[1]=i;
									p8[2]=i>>8;
									Uart_3762_CS((u32)p8);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
									UARTCtrlRS485->TxByte=i;//发送字节数(半字对齐)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
									UARTCtrl->SubTask=4;//子任务(临时操作路由模块任务)
									//WarningOccur(PORTRS485,(u8 *)"载波转发直接抄读F9");//告警发生,入口告警代码和告警字符串
									CopyString((u8 *)"载波F9  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+2+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符
									break;
								case 4://转发接收
									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										break;
									}
							
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									if(UARTCtrlRS485->Task!=1)
									{
										x=1;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										y=0;//转发接收数据长度
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											x=4;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
											y=0;//转发接收数据长度
										}
										else
										{
											x=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
											y=p8[28];//返回数据域长度
											if(y==0)
											{//接收数据长度=0
												x=4;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
												y=0;//转发接收数据长度
											}
											else
											{
												for(i=0;i<y;i++)
												{//去前导0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												y=p8[29+9];//返回数据域长度
												p8+=29;
												//控制码
												if((p8[8]&0x40)!=0x0)
												{//从站异常回答
													x=4;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
													y=0;//转发接收数据长度
												}
												else
												{
													//CS校验
													Fn=0;
													for(i=0;i<(y+10);i++)
													{
														Fn+=p8[i];
													}
													Fn&=0xff;
													if(Fn!=p8[i])
													{
														x=2;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
														y=0;//转发接收数据长度
													}
													//数据域减0x33
													p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29+10);
													for(i=0;i<y;i++)
													{
														p8[i]-=0x33;
													}
												}
											}
										}
									}
									Terminal_Router->StopFlags&=~(1<<2);//恢复路由
									Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									//目的地址比较
									if(x==5)
									{//5=转发接收数据
					 					if(DataComp(((u32)p8rxbuff)+18+2+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
										{//不同
											x=2;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
											y=0;//转发接收数据长度
										}
									}
									if(y==0x0)
									{//没收到数据,载波路由有数据返回即为标准645帧不需检查返回帧合法性
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//重发计数
										{
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											if(p16[0]==p16[1])
											{//没接收到下帧
												UARTCtrl->ReRMCount++;//重发计数
												UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
												return;
											}
										}
									#endif
									}
							F9err:
									UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									MR(((u32)p8txbuff)+18+1,((u32)p8rxbuff)+18+2+n,6);//转发目标地址
									p8txbuff[18+7]=x;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									if(y==0x0)
									{//无返回
										i=p8rxbuff[18+2+n+6];//取转发数据标识类型
										if(i==0x0)
										{//1997
											y=2;
										}
										else
										{//2007
											y=4;
										}
										p8txbuff[18+8]=y;//转发直接抄读的数据内容字节数k+2
										MR(((u32)p8txbuff)+18+9,((u32)p8rxbuff)+18+2+n+6+1,y);//转发直接抄读的数据标识
									}
									else
									{//有返回
										p8txbuff[18+8]=y;//转发直接抄读的数据内容字节数k+2
										MR(((u32)p8txbuff)+18+9,Get_ADDR_UARTnRx(PORTRS485)+29+10,y);
									}
									i=9+y+12;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ+4字节标识=12字节
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
									Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//恢复路由
									Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=等待下帧启动发送延时(原作为抄表口时置为的值）
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//得到UART定时器地址
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//转发中继级数n
								if(n==0xff)
								{
									n=0;//无指定中继路由
								}
								n*=6;//中继级数和地址字节长度
								n++;
								CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//转发目标地址
								DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//转发直接抄读的数据标识
								i=p8rxbuff[19+n+6];//转发直接抄读的数据标识类型
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(i==0)
								{//0：符合DL/T645-1997
									UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//默认1200
									Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
									UARTCtrlRS485->TxByte=4+14;//发送字节数
								}
								else
								{//1：符合DL/T645-2007
									UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//默认2400
									Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
									UARTCtrlRS485->TxByte=4+16;//发送字节数
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
								if(i!=0)
								{//找到
									//通信速率
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//不是默认
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//初始化UART口,入口通信控制字已填入
								Start_UARTn_Tx(PORTRS485);//启动UART口发送
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
					#endif
								UARTCtrl->Task=10;//10=正在通过其它口发送数据
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=正在通过其它口发送数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//转发接收等待报文超时时间
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//接收字节数
								UARTCtrl->Task=11;//11=正在通过其它口接收数据
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=正在通过其它口接收数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645帧接收;返回0没完成,1完成,2超时
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//转发中继级数n
							if(n==0xff)
							{
								n=0;//无指定中继路由
							}
							n*=6;//中继级数和地址字节长度
							n++;
							Protocol=p8rxbuff[19+n+6];//转发直接抄读的数据标识类型
							Protocol&=3;//0=符合DL/T645-1997,1=符合DL/T645-2007
							if(i==2)
							{//2=超时
								if(UARTCtrl->ReRMCount<2)//重发计数
								{
									UARTCtrl->ReRMCount++;//重发计数
									UARTCtrl->Task=9;//9=等待下帧启动发送延时
									break;
								}
							}
							if(i==1)
							{//1完成
								if(Protocol==0)
								{//0：符合DL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
								}
								else
								{//1：符合DL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
								}
								if(x!=0)
								{
									if(x!=2)
									{
										//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
										//继续接收
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
										UARTCtrlRS485->RxByte=0;//清接收字节数寄存
										break;
									}
								}
							}
							
							UARTCtrlRS485->Lock=0;//解除调用
							UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
							UARTCtrlRS485->RxByte=0;//清接收字节数寄存
							UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								x=1;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
								if(Protocol==0x0)
								{//1997
									y=2;//转发接收数据长度
								}
								else
								{//2007
									y=4;//转发接收数据长度
								}
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//转发直接抄读的数据标识
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									x=4;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									if(Protocol==0x0)
									{//1997
										y=2;//转发接收数据长度
									}
									else
									{//2007
										y=4;//转发接收数据长度
									}
									MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//转发直接抄读的数据标识
								}
								else
								{
									x=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									y=p8[9];//转发接收数据长度
								}
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//转发目标地址
							MWR(CommAddr,((u32)p8txbuff)+19,6);//转发目标地址
							p8txbuff[25]=x;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
							p8txbuff[26]=y;//转发直接抄读的数据内容字节数k+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//转发直接抄读的数据内容
							y+=13;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志,1字节数据内容字节数
							y+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志?
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
*/
	
		case 9://F9转发主站直接对电表的抄读数据命令
		case 10://F10转发主站直接对电表的批量抄读数据命令
			switch(UARTCtrl->Task)
			{
				case 8://8=等待通过其它口数据转发
					switch(p8rxbuff[18])
					{
						case 1://内部交流采样接口
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//转发中继级数n
							if(n==0xff)
							{
								n=0;//无指定中继路由
							}
							n*=6;//中继级数和地址字节长度
							CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//转发目标地址
							Protocol=p8rxbuff[20+n+6];//转发直接抄读的数据标识类型
							Protocol&=3;
							if(Protocol==0)
							{//0:符合DL/T645-1997,1:符合DL/T645-2007
								goto err;
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							
							UARTCtrl->F10ReadDICount=0;//F10已转发直接抄读的数据标识个数
							UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
							p8txbuff[26]=0;//发送区转发直接抄读的数据标识个数
							while(UARTCtrl->F10ReadDICount<p8rxbuff[20+n+7])
							{
								i=UARTCtrl->F10ReadDICount;//F10已转发直接抄读的数据标识个数
								DI=MRR(((u32)p8rxbuff)+20+n+8+(i*4),4);//转发直接抄读的数据标识
								if(UARTCtrl->SubsequentFrames==0)
								{
									Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								}
								else
								{
									Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa,UARTCtrl->SubsequentFrames);//DL645-2007读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址,后续帧号
								}
								x=UARTCtrlRS485->Task;//原任务寄存
								UARTCtrlRS485->Task=1;//1=接收完成
							  if(FileDownload->File&0x04)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
								}
								UARTCtrlRS485->Task=x;//原任务恢复
								p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//转发接收数据长度
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(i*4),4);//转发直接抄读的数据标识
									}
									else
									{
										y=0;//转发接收数据长度
									}
								}
								else
								{
									y=p8[9];//转发接收数据长度
									for(x=0;x<y;x++)
									{//数据域减0x33
										p8[10+x]-=0x33;
									}
									z=MRR(((u32)p8)+10,4);//返回的数据标识
									if(z!=DI)
									{
										y=0;
									}
								}
								x=0;//已有数据长度
								for(z=0;z<p8txbuff[26];z++)
								{
									x+=p8txbuff[27+x];
									x++;
								}
								if(UARTCtrl->SubsequentFrames==0)
								{//第1帧
									if((27+(x+1+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
									}
									else
									{
										MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//转发直接抄读的数据内容
										p8txbuff[27+x]=y;//转发直接抄读的数据内容字节数k+4
										if((p8[8]&0x20)&&(y!=0))
										{//有后续帧
											UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
										}
										else
										{//无后续帧
											UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
											p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
										}
									}
								}
								else
								{//后续帧
									if(y>5)
									{//4字节标识,1字节seq
										y-=5;
									}
									else
									{
										y=0;
									}
									z=p8txbuff[27+x];//本标识已读入长度
									z=255-z;//还可填长度
									if(y>z)
									{
										y=z;
									}
									if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
									}
									else
									{
										MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//转发直接抄读的数据内容
										p8txbuff[27+x]+=y;//转发直接抄读的数据内容字节数k+4
										if((p8[8]&0x20)&&(y!=0))
										{//有后续帧
											UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
										}
										else
										{//无后续帧
											UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
											p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
										}
									}
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//转发目标地址
							x=0;//已有数据长度
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							if(x>(p8txbuff[26]*5))//转发直接抄读的数据标识个数
							{
								p8txbuff[25]=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
							}
							else
							{
								p8txbuff[25]=2;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
							}
							if(Fn==9)
							{
								MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
								//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
								//x+=12;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志
								x+=20;
							}
							else
							{
								//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
								//x+=13;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志,1字节数据标识个数
								x+=21;
							}
							p8txbuff[1]=x;
							p8txbuff[2]=x>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							break;
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(p16timer[0]==0)//等待数据转发口空闲定时5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=通信功能正在被调用禁止其他程序再次调用
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									p16timer[0]=50/10;//恢复20ms后发送(考虑台体测试模拟表速度现用50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrl->ReRMCount=0;//重发计数
									UARTCtrl->Task=9;//9=等待下帧启动发送延时
									
									UARTCtrl->F10ReadDICount=0;//F10已转发直接抄读的数据标识个数
									UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									p8txbuff[26]=0;//发送区转发直接抄读的数据标识个数
								}
							}
							break;
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
						case 31://端口号=31,载波
							//端口号=31,转发只能对DL645-1997或2007进行
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
							{
								case 0://等待口空闭时进入
									p8txbuff[26]=0;//发送区转发直接抄读的数据标识个数
									
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
									DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
									if(p16[0]!=p16[1])
									{//已接收到下帧
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										goto F10err;
									}
								#endif
									if(p16timer[0]==0)//等待数据转发口空闲定时120S
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										goto F10err;
									}
									if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										goto F10err;
									}
									if(Terminal_Router->UpDateTask!=0)//软件更新任务
									{
										x=0;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										goto F10err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=通信功能正在被调用禁止其他程序再次调用
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
											UARTCtrl->SubTask=1;//子任务(临时操作路由模块任务)
										}
									}
									break;
								case 1://暂停路由
									if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//暂停路由
									Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrl->SubTask=2;//子任务(临时操作路由模块任务)
									break;
								case 2://暂停路由接收
									if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									{
										UARTCtrl->ReRMCount=0;//重发计数
										UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
										
										UARTCtrl->F10ReadDICount=0;//F10已转发直接抄读的数据标识个数
										UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									}
									break;
								case 3://路由转发数据
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
									DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
									if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
									{//F10已转发直接抄读的数据标识个数>=需读个数或有收到下帧
								#else
									if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
									{//F10已转发直接抄读的数据标识个数>=需读个数
								#endif
									F10End:
										Terminal_Router->StopFlags&=~(1<<2);//恢复路由
										Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
										
										x=0;//已有数据长度
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(x>(p8txbuff[26]*5))//转发直接抄读的数据标识个数
										{
											x=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										}
										else
										{
											x=2;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										}
									F10err:
										UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
										UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										p8txbuff[25]=x;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
										
										n=p8rxbuff[19];//取转发中继级数
										if(n==0xff)
										{
											n=0;
										}
										n*=6;
										MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//转发目标地址
										x=0;//已有数据长度
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(Fn==9)
										{
											MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
											//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
											//x+=12;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志
											x+=20;
										}
										else
										{
											//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
											//x+=13;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志,1字节数据标识个数
											x+=21;
										}
										p8txbuff[1]=x;
										p8txbuff[2]=x>>8;
										i=p8rxbuff[1]+(p8rxbuff[2]<<8);
										Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
										Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志�
										return;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
									{
										p8[3]=0x43;//控制码;集中式路由载波通信
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
										{
											p8[3]=0x4a;//控制码;微功率无线通信
										}
										else
										{
											p8[3]=0x41;//控制码;集中式路由载波通信
										}
									}
									p8[4]=4;//通信模块标识=1
									p8[5]=0;
									p8[6]=0;//预计应答字节数
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									p8[9]=UARTCtrlRS485->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
									MR(((u32)p8)+16,((u32)p8rxbuff)+20+n,6);//从节点地址(目的地址)
									CommAddr=MRR(((u32)p8)+16,6);//通信地址
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[20+n+6];//转发数据标识类型
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据标识
									p8[26]=0;//通信相关标志
									p8[27]=0;//附属节点数=0
									if(UARTCtrl->SubsequentFrames==0)
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645报文长度
											Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										}
										else
										{
											p8[28]=16;//DL645报文长度
											Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										}
									}
									else
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645报文长度
											Read_DL645_1997_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr);//DL645-1997读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										}
										else
										{
											p8[28]=17;//DL645报文长度
											Read_DL645_2007_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址,后续帧号
										}
									}
									i=31+p8[28];//需总发送字节数
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
									{//微功率无线,档案地址用采集器地址
										x=Check_AFN04F10_Addr(31,CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//从节点地址(目的地址)
											}
										}
									}
									Uart_3762_CS((u32)p8);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
									UARTCtrlRS485->TxByte=i;//发送字节数(半字对齐)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
									UARTCtrl->SubTask=4;//子任务(临时操作路由模块任务)
									if(Fn==9)
									{
										CopyString((u8 *)"载波F9  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									}
									else
									{
										CopyString((u8 *)"载波F10 ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
									}
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(20+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符
									break;
								case 4://转发接收
									if(UARTCtrlRS485->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									{
										break;
									}
									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//超时或否认回答
										y=0;//转发接收数据长度
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
											UARTCtrlRS485->RxByte=0;//接收字节数(半字对齐)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2直接否认帧(微功率无线有此情况)
												p16[0]=6000/10;//6秒超时时间
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
											}
											return;
										}
										else
										{
											y=p8[28];//返回645帧总长度
											if(y!=0)
											{
												for(i=0;i<y;i++)
												{//去前导0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												//控制码
												if((p8[29+8]&0x40)!=0x0)
												{//从站异常回答
													y=0;//转发接收数据长度
												}
												else
												{
													y=p8[29+9];//返回645帧数据域长度
													//CS校验
													z=0;
													for(i=0;i<(y+10);i++)
													{
														z+=p8[29+i];
													}
													z&=0xff;
													if(z!=p8[29+i])
													{
														y=0;//转发接收数据长度
													}
													else
													{
													#if (USER/100)==11//吉林版
														//吉林数据域不减0x33
														//数据标识减0x33
														for(i=0;i<4;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#else
														//数据域减0x33
														for(i=0;i<y;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#endif
														//目的地址比较
									 					if(DataComp(((u32)p8rxbuff)+20+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
														{//不同
															UARTCtrlRS485->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
															UARTCtrlRS485->RxByte=0;//接收字节数(半字对齐)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
															return;
														}
														//DI比较
														DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据标识
														z=MRR(((u32)p8)+29+10,4);//返回的数据标识
														i=p8rxbuff[20+n+6];//转发数据标识类型
														if(i==0)
														{//1997
															DI&=0xffff;
															z&=0xffff;
														}
														if(DI!=z)
														{
															if(z==0x04001501)
															{//是收到主动上报帧
//															#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//																d64=MRR(((u32)p8rxbuff)+20+n,6);
//																Write_AutoReportWord(31,d64,((u32)p8)+29+14);//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
//															#endif
																UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
																p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
																i=UARTCtrl->F10ReadDICount;//F10已转发直接抄读的数据标识
																i++;
																if(i>=256)
																{
																	goto F10End;
																}
																UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识
																UARTCtrl->ReRMCount=0;//重发计数
																UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
																return;
															}
															UARTCtrlRS485->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
															UARTCtrlRS485->RxByte=0;//接收字节数(半字对齐)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
															return;
														}
														else
														{
															y=p8[29+9];//返回数据域长度
															if(i==0)
															{//1997
																//2字节标识变为4字节
																if(y<2)
																{
																	y=0;
																}
																else
																{
																	y-=2;
																	MR(ADDR_DATABUFF,((u32)p8)+29+12,y);
																	MR(((u32)p8)+29+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据
																	MR(((u32)p8)+29+14,ADDR_DATABUFF,y);
																	y+=4;
																}
															}
														}
													}
												}
											}
										}
									}
									
									p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29);
									if(y==0x0)
									{//没收到数据
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//重发计数
										{
											UARTCtrl->ReRMCount++;//重发计数
											UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
											return;
										}
									#endif
										y=4;//转发接收数据长度
										p8[8]=0;//去后续帧标志
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据标识
									}
									Protocol=Get_LEN_UARTnTx(PORTn);
									x=0;//已有数据长度
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									i=UARTCtrl->F10ReadDICount;//F10已转发直接抄读的数据标识个数
									if(UARTCtrl->SubsequentFrames==0)
									{//第1帧
										if((27+(x+1+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
										}
										else
										{
											MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//转发直接抄读的数据内容
											p8txbuff[27+x]=y;//转发直接抄读的数据内容字节数k+4
											if((p8[8]&0x20)&&(y!=0))
											{//有后续帧
												UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
											}
											else
											{//无后续帧
												UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
												p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
											}
										}
									}
									else
									{//后续帧
										i=p8rxbuff[20+n+6];//转发数据标识类型
										if(i==0)
										{//1997
											if(y>4)
											{//4字节标识
												y-=4;
											}
											else
											{
												y=0;
											}
										}
										else
										{//2007
											if(y>5)
											{//4字节标识,1字节seq
												y-=5;
											}
											else
											{
												y=0;
											}
										}
										z=p8txbuff[27+x];//本标识已读入长度
										z=255-z;//还可填长度
										if(y>z)
										{
											y=z;
										}
										if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
										}
										else
										{
											MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//转发直接抄读的数据内容
											p8txbuff[27+x]+=y;//转发直接抄读的数据内容字节数k+4
											if((p8[8]&0x20)&&(y!=0))
											{//有后续帧
												UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
											}
											else
											{//无后续帧
												UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
												p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
											}
										}
									}
									UARTCtrl->ReRMCount=0;//重发计数
									UARTCtrl->SubTask=3;//子任务(临时操作路由模块任务)
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//恢复路由
									Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=等待下帧启动发送延时(原作为抄表口时置为的值）
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//得到UART定时器地址
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//转发中继级数n
								if(n==0xff)
								{
									n=0;//无指定中继路由
								}
								n*=6;//中继级数和地址字节长度
								
								p16=(u16 *)Get_ADDR_UARTn(PORTn);
							#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
								DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
								if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
								{//F10已转发直接抄读的数据标识个数>=需读个数或有收到下帧
							#else
								if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
								{//F10已转发直接抄读的数据标识个数>=需读个数
							#endif
								F10End_2:
									x=0;//已有数据长度
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(x>(p8txbuff[26]*5))//转发直接抄读的数据标识个数
									{
										x=5;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									}
									else
									{
										x=2;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									}
								//F10err_2:
									p8txbuff[25]=x;//转发结果标志:0=不能执行转发,1=转发接收超时,2=转发接收错误,3=转发接收确认,4=转发接收否认,5=转发接收数据,其他值无效
									UARTCtrlRS485->Lock=0;//解除调用
									UARTCtrlRS485->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									UARTCtrlRS485->RxByte=0;//清接收字节数寄存
									UARTCtrlRS485->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收


									n=p8rxbuff[19];//取转发中继级数
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//转发目标地址
									x=0;//已有数据长度
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(Fn==9)
									{
										MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
										//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
										//x+=12;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志
										x+=20;
									}
									else
									{
										//x+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
										//x+=13;//4字节标识,1字节端口号,6字节转发目标地址,1字节转发结果标志,1字节数据标识个数
										x+=21;
									}
									p8txbuff[1]=x;
									p8txbuff[2]=x>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
									Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志�
									return;
								}
								CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//转发目标地址
								DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据标识
								i=p8rxbuff[20+n+6];//转发直接抄读的数据标识类型
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(UARTCtrl->SubsequentFrames==0)
								{//第1帧
									if(i==0)
									{//0：符合DL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//默认1200
										Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										UARTCtrlRS485->TxByte=4+14;//发送字节数
									}
									else
									{//1：符合DL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//默认2400
										Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										UARTCtrlRS485->TxByte=4+16;//发送字节数
									}
								}
								else
								{//后续帧
									if(i==0)
									{//0：符合DL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//默认1200
										Read_DL645_1997_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										UARTCtrlRS485->TxByte=4+14;//发送字节数
									}
									else
									{//1：符合DL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//默认2400
										Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
										UARTCtrlRS485->TxByte=4+17;//发送字节数
									}
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
								if(i!=0)
								{//找到
									//通信速率
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//不是默认
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//初始化UART口,入口通信控制字已填入
								Start_UARTn_Tx(PORTRS485);//启动UART口发送
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
								}
					#endif
								UARTCtrl->Task=10;//10=正在通过其它口发送数据
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=正在通过其它口发送数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//转发接收等待报文超时时间
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//接收字节数
								UARTCtrl->Task=11;//11=正在通过其它口接收数据
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=正在通过其它口接收数据
					switch(p8rxbuff[18])
					{
						case 2://端口号=2,RS485-1
						case 3://端口号=3,RS485-2
						case 4://端口号=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645帧接收;返回0没完成,1完成,2超时
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//转发中继级数n
							if(n==0xff)
							{
								n=0;//无指定中继路由
							}
							n*=6;//中继级数和地址字节长度
							
							Protocol=p8rxbuff[20+n+6];//转发直接抄读的数据标识类型
							Protocol&=3;//0=符合DL/T645-1997,1=符合DL/T645-2007
							if(i==2)
							{//2=超时
								if(UARTCtrl->ReRMCount<2)//重发计数
								{
									UARTCtrl->ReRMCount++;//重发计数
									UARTCtrl->Task=9;//9=等待下帧启动发送延时
									break;
								}
							}
							if(i==1)
							{//1完成
								if(Protocol==0)
								{//0：符合DL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
								}
								else
								{//1：符合DL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
								}
								if(x!=0)
								{
									if(x==3)
									{//3=数据标识错
										p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
										//减0x33
										for(z=0;z<p8[9];z++)
										{
											p8[10+z]-=0x33;
										}
										z=MRR(((u32)p8)+10,4);//返回的数据标识
										if(z==0x04001501)//if(z==0x37334834)
										{//是收到主动上报帧
//										#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//											d64=MRR(((u32)p8)+1,6);
//											Write_AutoReportWord(p8rxbuff[18],d64,((u32)p8)+14);//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
//										#endif
											UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
											p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
											i=UARTCtrl->F10ReadDICount;//F10已转发直接抄读的数据标识个数
											i++;
											if(i>=256)
											{
												goto F10End_2;
											}
											UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
											UARTCtrl->ReRMCount=0;//重发计数
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//得到UART定时器地址
											p16[0]=0;
											UARTCtrl->Task=9;//9=等待下帧启动发送延时(原作为抄表口时置为的值）
											return;
										}
									}
									if(x!=2)
									{
										//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
										//继续接收
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
										UARTCtrlRS485->RxByte=0;//清接收字节数寄存
										break;
									}
								}
							}
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								y=4;//转发接收数据长度
								p8[8]=0;//去后续帧标志
								MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//转发接收数据长度
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据标识
									}
									else
									{
										y=0;//转发接收数据长度
									}
								}
								else
								{
									y=p8[9];//转发接收数据长度
									if(Protocol==0)
									{//1997
										//2字节标识变为4字节
										if(y<2)
										{
											y=0;
										}
										else
										{
											y-=2;
											MR(ADDR_DATABUFF,((u32)p8)+12,y);
											MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//转发直接抄读的数据
											MR(((u32)p8)+14,ADDR_DATABUFF,y);
											y+=4;
										}
									}
								}
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							x=0;//已有数据长度
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							i=UARTCtrl->F10ReadDICount;//F10已转发直接抄读的数据标识个数
							if(UARTCtrl->SubsequentFrames==0)
							{//第1帧
								if((27+(x+1+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
								}
								else
								{
									MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//转发直接抄读的数据内容
									p8txbuff[27+x]=y;//转发直接抄读的数据内容字节数k+4
									if((p8[8]&0x20)&&(y!=0))
									{//有后续帧
										UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
									}
									else
									{//无后续帧
										UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
									}
								}
							}
							else
							{//后续帧
								i=p8rxbuff[20+n+6];//转发数据标识类型
								if(i==0)
								{//1997
									if(y>4)
									{//4字节标识
										y-=4;
									}
									else
									{
										y=0;
									}
								}
								else
								{//2007
									if(y>5)
									{//4字节标识,1字节seq
										y-=5;
									}
									else
									{
										y=0;
									}
								}
								z=p8txbuff[27+x];//本标识已读入长度
								z=255-z;//还可填长度
								if(y>z)
								{
									y=z;
								}
								if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
								}
								else
								{
									MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//转发直接抄读的数据内容
									p8txbuff[27+x]+=y;//转发直接抄读的数据内容字节数k+4
									if((p8[8]&0x20)&&(y!=0))
									{//有后续帧
										UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
									}
									else
									{//无后续帧
										UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
										p8txbuff[26]++;//发送区转发直接抄读的数据标识个数
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10已转发直接抄读的数据标识个数
									}
								}
							}
							UARTCtrl->ReRMCount=0;//重发计数
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//得到UART定时器地址
							p16[0]=0;
							UARTCtrl->Task=9;//9=等待下帧启动发送延时(原作为抄表口时置为的值）
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
		default:
			UARTCtrl->Task=0;
			break;
	}
}












